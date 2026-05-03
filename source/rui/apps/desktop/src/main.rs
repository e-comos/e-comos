#![no_std]
#![no_main]
extern crate alloc;
use alloc::vec::Vec;
use rui_lib::{Widget, Icon, Button, Rgb};
use common::{IpcMessage, kernel, Pid, WindowId};
use core::{
    ffi::CStr,
    ptr,
};
use x86_64::instructions::hlt;

// ==========================
// 数据结构定义
// ==========================
/// 任务栏（顶部）
struct Taskbar {
    x: u16,
    y: u16,
    width: u16,
    height: u16,
    window_buttons: Vec<Button>, // 已打开窗口的按钮
    time_text: &'static str,     // 时间文本（简化：静态）
}

/// Dock栏（底部）
struct Dock {
    x: u16,          // 居中显示，x坐标动态计算
    y: u16,
    height: u16,
    icons: Vec<Icon>, // 常用应用图标
}

/// 桌面全局状态
struct Desktop {
    screen_width: u16,
    screen_height: u16,
    wm_pid: Pid,     // 窗口管理器PID
    render_pid: Pid, // 渲染服务PID
    taskbar: Taskbar,
    dock: Dock,
}

// ==========================
// 实现：任务栏
// ==========================
impl Taskbar {
    /// 创建任务栏（顶部，高度24px）
    fn new(screen_width: u16) -> Self {
        Taskbar {
            x: 0,
            y: 0,
            width: screen_width,
            height: 24,
            window_buttons: Vec::new(),
            time_text: "12:00", // 静态时间，后续替换为实时时间
        }
    }

    /// 绘制任务栏
    fn draw(&self, render_pid: Pid) {
        unsafe {
            // 1. 绘制任务栏背景（深灰）
            kernel::ipc_send(render_pid, &IpcMessage::RenderDrawRect {
                x: self.x,
                y: self.y,
                w: self.width,
                h: self.height,
                color: Rgb { r: 50, g: 50, b: 50 },
            });

            // 2. 绘制左侧标题（白色文字）
            kernel::ipc_send(render_pid, &IpcMessage::RenderDrawText {
                x: 10,
                y: 4, // 文字基线偏移
                text: "RUI Desktop",
                color: Rgb { r: 255, g: 255, b: 255 },
            });

            // 3. 绘制右侧时间（白色文字）
            kernel::ipc_send(render_pid, &IpcMessage::RenderDrawText {
                x: self.width - 50, // 右对齐
                y: 4,
                text: self.time_text,
                color: Rgb { r: 255, g: 255, b: 255 },
            });

            // 4. 绘制窗口按钮（中间区域）
            let mut btn_x = 120; // 按钮起始x坐标（避开标题）
            for btn in &self.window_buttons {
                btn.draw(WindowId(0), render_pid); // 临时用0作为窗口ID
                btn_x += btn.width + 5; // 按钮间距5px
            }
        }
    }

    /// 添加窗口按钮（窗口打开时调用）
    fn add_window_button(&mut self, title: &'static str, window_id: WindowId, wm_pid: Pid) {
        self.window_buttons.push(Button::new(
            0, 0, // x/y会在draw时动态计算
            100, 20, // 按钮尺寸
            title,
            Rgb { r: 100, g: 100, b: 100 }, // 按钮背景
            move || {
                // 点击按钮：通知WM切换焦点到该窗口
                unsafe {
                    kernel::ipc_send(wm_pid, &IpcMessage::WmFocusWindow { window_id });
                }
            },
        ));
    }
}

// ==========================
// 实现：Dock栏
// ==========================
impl Dock {
    /// 创建Dock栏（底部，高度48px）
    fn new(screen_width: u16, screen_height: u16, wm_pid: Pid) -> Self {
        // 初始化3个常用应用图标
        let icons = vec![
            // 计算器图标
            Icon::new(
                0, 0, // x/y动态计算（居中）
                32, 32, // 图标尺寸
                "calc", // 图标名称
                || unsafe { kernel::spawn_process("rui-app-calculator") }, // 启动计算器
            ),
            // 文本编辑器图标
            Icon::new(
                0, 0,
                32, 32,
                "editor",
                || unsafe { kernel::spawn_process("rui-app-text-editor") },
            ),
            // 终端图标
            Icon::new(
                0, 0,
                32, 32,
                "terminal",
                || unsafe { kernel::spawn_process("rui-app-terminal") },
            ),
        ];

        // 计算Dock总宽度（图标宽×3 + 间距×2）
        let total_width = 32 * 3 + 10 * 2;
        Dock {
            x: (screen_width - total_width) / 2, // 居中显示
            y: screen_height - 48, // 底部
            height: 48,
            icons,
        }
    }

    /// 绘制Dock栏
    fn draw(&self, render_pid: Pid) {
        unsafe {
            // 1. 绘制Dock背景（半透明灰，简化为实色）
            kernel::ipc_send(render_pid, &IpcMessage::RenderDrawRect {
                x: self.x - 10, // 左右各扩展10px边距
                y: self.y,
                w: 32 * 3 + 10 * 4, // 总宽度+边距
                h: self.height,
                color: Rgb { r: 80, g: 80, b: 80 },
            });

            // 2. 绘制图标（动态计算x坐标，居中排列）
            let mut icon_x = self.x;
            for icon in &self.icons {
                // 临时修改图标x坐标（居中排列）
                let mut icon = icon.clone();
                icon.x = icon_x;
                icon.y = self.y + 8; // 垂直居中
                icon.draw(WindowId(0), render_pid);
                icon_x += 32 + 10; // 图标宽+间距
            }
        }
    }

    /// 处理鼠标悬停（简化：图标放大）
    fn handle_hover(&mut self, x: u16, y: u16) {
        for icon in &mut self.icons {
            // 检查鼠标是否在图标范围内
            let in_icon = x >= icon.x && x < icon.x + icon.width
                && y >= icon.y && y < icon.y + icon.height;

            if in_icon {
                icon.width = 40;
                icon.height = 40;
            } else {
                icon.width = 32;
                icon.height = 32;
            }
        }
    }
}

// ==========================
// 桌面主逻辑
// ==========================
impl Desktop {
    /// 初始化桌面
    fn init() -> Self {
        unsafe {
            // 1. 获取屏幕分辨率（假设800x600，实际应从GPU驱动获取）
            let screen_width = 800;
            let screen_height = 600;

            // 2. 获取依赖服务PID
            let wm_pid = kernel::get_service_pid(CStr::from_bytes_with_nul(b"wm\0").unwrap().as_ptr());
            let render_pid = kernel::get_service_pid(CStr::from_bytes_with_nul(b"render\0").unwrap().as_ptr());

            // 3. 初始化任务栏和Dock
            let taskbar = Taskbar::new(screen_width);
            let dock = Dock::new(screen_width, screen_height, wm_pid);

            Desktop {
                screen_width,
                screen_height,
                wm_pid,
                render_pid,
                taskbar,
                dock,
            }
        }
    }

    /// 绘制整个桌面
    fn draw(&self) {
        unsafe {
            // 1. 绘制桌面背景（浅灰）
            kernel::ipc_send(self.render_pid, &IpcMessage::RenderDrawRect {
                x: 0,
                y: 0,
                w: self.screen_width,
                h: self.screen_height,
                color: Rgb { r: 240, g: 240, b: 240 },
            });

            // 2. 绘制任务栏和Dock
            self.taskbar.draw(self.render_pid);
            self.dock.draw(self.render_pid);
        }
    }
}

// ==========================
// 入口函数与事件循环
// ==========================
#[no_mangle]
pub extern "C" fn main() -> ! {
    let mut desktop = Desktop::init();
    desktop.draw(); // 初始绘制

    // 测试：添加一个默认窗口按钮到任务栏
    desktop.taskbar.add_window_button(
        "测试窗口",
        WindowId(1), // 假设窗口ID为1
        desktop.wm_pid,
    );
    desktop.taskbar.draw(desktop.render_pid); // 重绘任务栏

    // 事件循环：处理鼠标事件和窗口状态更新
    loop {
        let (_, msg) = unsafe { kernel::ipc_recv() };
        match msg {
            // 处理鼠标点击
            IpcMessage::MouseClick { x, y } => {
                // 检查是否点击Dock图标
                for icon in &mut desktop.dock.icons {
                    icon.on_click(x, y);
                }
                // 检查是否点击任务栏窗口按钮
                for btn in &mut desktop.taskbar.window_buttons {
                    btn.on_click(x, y);
                }
            }
            // 处理鼠标移动（Dock图标放大）
            IpcMessage::MouseMove { x, y } => {
                desktop.dock.handle_hover(x, y);
                desktop.dock.draw(desktop.render_pid); // 重绘Dock
            }
            // 处理窗口打开事件（来自WM）
            IpcMessage::WmWindowOpened { title, window_id } => {
                desktop.taskbar.add_window_button(title, window_id, desktop.wm_pid);
                desktop.taskbar.draw(desktop.render_pid); // 重绘任务栏
            }
            _ => {}
        }
        hlt();
    }
}

// panic处理
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop { hlt(); }
}