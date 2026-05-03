#![no_std]
#![no_main]
extern crate alloc;
use alloc::vec::Vec;
use common::{
    IpcMessage, Rgb, kernel, Pid, WindowId,
    // 引入渲染服务的消息类型（复用common中定义的）
};
use core::{
    ffi::CStr,
    sync::atomic::{AtomicU32, Ordering},
};
use linked_list_allocator::LockedHeap;
use x86_64::instructions::hlt;

#[global_allocator]
static GLOBAL_ALLOCATOR: LockedHeap = LockedHeap::empty();

// ==========================
// 数据结构：窗口元数据与状态
// ==========================
/// 窗口状态（正常/最小化/关闭）
#[derive(Debug, Clone, Copy, PartialEq)]
enum WindowState {
    Normal,
    Minimized,
    Closed,
}

/// 窗口元数据（所有需要的信息都存在这里）
#[derive(Debug, Clone, Copy)]  // 添加 Clone 和 Copy
struct Window {
    id: WindowId,          // 唯一ID
    _owner_pid: Pid,        // 所属应用的PID
    x: u16,                // 左上角X坐标
    y: u16,                // 左上角Y坐标
    width: u16,            // 宽度
    height: u16,           // 高度
    _title: &'static str,   // 窗口标题
    state: WindowState,    // 窗口状态
    is_focused: bool,      // 是否获得焦点（影响标题栏颜色）
}

impl Window {
    /// 创建新窗口（自动生成唯一ID）
    fn new(owner_pid: Pid, x: u16, y: u16, width: u16, height: u16, title: &'static str) -> Self {
        static NEXT_WINDOW_ID: AtomicU32 = AtomicU32::new(1); // 从1开始（0为无效ID）
        let id = NEXT_WINDOW_ID.fetch_add(1, Ordering::Relaxed) as WindowId;
        Window {
            id,
            _owner_pid: owner_pid,
            x,
            y,
            width,
            height,
            _title: title,
            state: WindowState::Normal,
            is_focused: false,
        }
    }

    /// 绘制窗口（调用渲染服务）
    fn draw(&self, render_pid: Pid) {
        if self.state == WindowState::Closed {
            return;
        }

        // 1. 绘制窗口背景（浅灰色）
        let bg_color = Rgb { r: 240, g: 240, b: 240 };
        let draw_bg_msg = IpcMessage::RenderDrawRect {
            x: self.x,
            y: self.y,
            w: self.width,
            h: self.height,
            color: bg_color,
        };
        unsafe { kernel::ipc_send(render_pid, &draw_bg_msg); }

        // 2. 绘制标题栏（焦点窗口用深蓝色，非焦点用灰色）
        let titlebar_height = 24; // 标题栏高度固定24像素
        let title_color = if self.is_focused {
            Rgb { r: 0, g: 50, b: 150 } // 焦点：深蓝色
        } else {
            Rgb { r: 180, g: 180, b: 180 } // 非焦点：灰色
        };
        let draw_titlebar_msg = IpcMessage::RenderDrawRect {
            x: self.x,
            y: self.y,
            w: self.width,
            h: titlebar_height,
            color: title_color,
        };
        unsafe { kernel::ipc_send(render_pid, &draw_titlebar_msg); }

        // 3. TODO：绘制标题文字（后续扩展render的DrawText消息）
        // 简化：暂时不画文字，只画标题栏背景
    }
}

// ==========================
// 窗口管理器状态管理
// ==========================
/// 窗口管理器全局状态
struct WmState {
    windows: Vec<Window>,       // 所有窗口列表（动态数组，安全管理）
    render_pid: Pid,            // 渲染服务的PID
    focused_window_id: WindowId, // 当前焦点窗口ID
}

impl WmState {
    fn new(render_pid: Pid) -> Self {
        WmState {
            windows: Vec::new(),
            render_pid,
            focused_window_id: 0, // 初始无焦点
        }
    }

    /// 添加新窗口并绘制
    fn add_window(&mut self, window: Window) {
        self.windows.push(window);
        // 新窗口默认获得焦点
        self.focused_window_id = window.id;
        self.update_focus();
        // 绘制新窗口
        self.draw_all_windows();
    }

    /// 更新窗口焦点状态（只让焦点窗口的is_focused为true）
    fn update_focus(&mut self) {
        for window in &mut self.windows {
            window.is_focused = window.id == self.focused_window_id;
        }
    }

    /// 绘制所有窗口（按Z轴顺序，后添加的窗口在顶层）
    fn draw_all_windows(&self) {
        for window in &self.windows {
            window.draw(self.render_pid);
        }
    }

    /// 处理鼠标点击事件（简化：点击窗口标题栏则获取焦点）
    fn handle_mouse_click(&mut self, x: u16, y: u16) {
        // 从顶层窗口开始检查（逆序遍历，后添加的在顶层）
        for window in self.windows.iter().rev() {
            // 检查点击位置是否在窗口标题栏内
            let titlebar_height = 24;
            let in_titlebar = x >= window.x 
                && x < window.x + window.width 
                && y >= window.y 
                && y < window.y + titlebar_height;

            if in_titlebar && window.state == WindowState::Normal {
                self.focused_window_id = window.id;
                self.update_focus();
                self.draw_all_windows(); // 重绘以更新标题栏颜色
                break;
            }
        }
    }
}

// ==========================
// 入口与消息循环
// ==========================
#[no_mangle]
pub extern "C" fn main() -> ! {
    unsafe {
        // 1. 初始化：获取渲染服务PID
        let render_name = CStr::from_bytes_with_nul(b"render\0").unwrap();
        let render_pid = kernel::get_service_pid(render_name.as_ptr() as *const u8);

        // 2. 初始化窗口管理器状态
        let mut wm_state = WmState::new(render_pid);

        // 3. 测试：添加一个默认窗口（模拟应用请求）
        let test_window = Window::new(
            100, // 假设应用PID为100
            50,  // x
            50,  // y
            400, // width
            300, // height
            "测试窗口 - RUI Demo",
        );
        wm_state.add_window(test_window);

        // 4. 消息循环：处理应用请求和输入事件
        loop {
            let mut sender_pid: Pid = 0;
            let mut msg = IpcMessage::WmCreateWindow { x: 0, y: 0, w: 0, h: 0, title: "" };
            kernel::ipc_recv(&mut sender_pid, &mut msg);
            match msg {
                // 处理应用的"创建窗口"请求
                IpcMessage::WmCreateWindow { x, y, w, h, title } => {
                    let new_window = Window::new(sender_pid, x, y, w, h, title);
                    wm_state.add_window(new_window);
                }
                // 处理鼠标驱动的"点击事件"（假设鼠标驱动发送此消息）
                IpcMessage::MouseClick { x, y } => {
                    wm_state.handle_mouse_click(x, y);
                }
                _ => {} // 忽略其他消息
            }
            hlt();
        }
    }
}

// panic处理（no_std必需）
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop { hlt(); }
}