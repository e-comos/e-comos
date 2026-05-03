#![no_std]
#![no_main]

use common::{IpcMessage, Rgb, kernel, Pid, Pixel};
use core::{
    ffi::CStr,
    ptr,
};
use font8x8::{BASIC_FONTS, UnicodeFonts}; // 引入8x8点阵字体库
use x86_64::instructions::hlt;

// 渲染服务状态：帧缓冲区、分辨率、GPU驱动PID
struct RenderState {
    fb: *mut Pixel,       // 帧缓冲区虚拟地址（映射自GPU驱动）
    width: u16,           // 屏幕宽度（像素）
    height: u16,          // 屏幕高度（像素）
    gpu_pid: Pid,         // GPU驱动的PID
    initialized: bool,    // 是否初始化完成
}

impl RenderState {
    const fn new() -> Self {
        RenderState {
            fb: ptr::null_mut(),
            width: 0,
            height: 0,
            gpu_pid: 0,
            initialized: false,
        }
    }

    /// 初始化：连接GPU驱动并获取帧缓冲区
    fn init(&mut self) -> bool {
        unsafe {
            // 1. 获取GPU驱动的PID
            let gpu_name = CStr::from_bytes_with_nul(b"gpu-driver\0").expect("Invalid CStr");
            self.gpu_pid = kernel::get_service_pid(gpu_name.as_ptr() as *const u8);
            if self.gpu_pid == 0 {
                return false; // 未找到GPU驱动
            }

            // 2. 向GPU驱动发送初始化请求（800x600分辨率）
            let init_msg = IpcMessage::GpuInit { width: 800, height: 600 };
            kernel::ipc_send(self.gpu_pid, &init_msg);

            // 3. 接收GPU驱动的回复，获取帧缓冲区信息
            let mut sender_pid: Pid = 0;
            let mut reply = IpcMessage::GpuFbReply { fb: ptr::null_mut(), width: 0, height: 0 };
            kernel::ipc_recv(&mut sender_pid, &mut reply);
            if let IpcMessage::GpuFbReply { fb, width, height } = reply {
                if fb.is_null() || width == 0 || height == 0 {
                    return false; // 帧缓冲区无效
                }
                self.fb = fb;
                self.width = width;
                self.height = height;
                self.initialized = true;
                true
            } else {
                false // 未收到预期回复
            }
        }
    }
}

// 全局渲染状态
static mut RENDER_STATE: RenderState = RenderState::new();

/// 安全绘制像素（检查边界，避免越界访问）
unsafe fn draw_pixel(x: u16, y: u16, color: Rgb) {
    if !RENDER_STATE.initialized {
        return; // 未初始化则忽略
    }
    if x >= RENDER_STATE.width || y >= RENDER_STATE.height {
        return; // 坐标越界
    }
    // 计算像素在帧缓冲区中的索引（行优先）
    let idx = (y as usize) * (RENDER_STATE.width as usize) + (x as usize);
    *RENDER_STATE.fb.add(idx) = color.to_pixel();
}

/// 绘制填充矩形
unsafe fn draw_rect(x: u16, y: u16, w: u16, h: u16, color: Rgb) {
    if !RENDER_STATE.initialized {
        return;
    }
    // 裁剪矩形（避免超出屏幕范围）
    let max_x = RENDER_STATE.width - 1;
    let max_y = RENDER_STATE.height - 1;
    let x = x.min(max_x);
    let y = y.min(max_y);
    let w = w.min(max_x - x + 1);
    let h = h.min(max_y - y + 1);

    // 逐像素绘制矩形
    for dy in 0..h {
        for dx in 0..w {
            draw_pixel(x + dx, y + dy, color);
        }
    }
    // 通知GPU刷新屏幕
    kernel::ipc_send(RENDER_STATE.gpu_pid, &IpcMessage::GpuFlush);
}

/// 绘制文字（使用8x8点阵字体）
unsafe fn draw_text(x: u16, y: u16, text: &str, color: Rgb) {
    if !RENDER_STATE.initialized {
        return;
    }
    // 逐个字符绘制
    for (char_idx, c) in text.chars().enumerate() {
        // 获取字符的8x8点阵数据（只支持ASCII）
        let font_data = match BASIC_FONTS.get(c) {
            Some(data) => data,
            None => return, // 不支持的字符
        };

        // 绘制字符的每个像素
        for row in 0..8 {
            let row_bits = font_data[row]; // 一行的8个像素（bit位表示）
            for col in 0..8 {
                if (row_bits >> col) & 1 != 0 {
                    // 点阵为1的位置绘制像素
                    let px = x + (char_idx as u16) * 8 + col;
                    let py = y + row as u16; // 将 row 转换为 u16
                    draw_pixel(px, py, color);
                }
            }
        }
    }
    // 刷新屏幕
    kernel::ipc_send(RENDER_STATE.gpu_pid, &IpcMessage::GpuFlush);
}

/// 处理IPC消息（来自窗口管理器或应用）
unsafe fn handle_ipc_message(_sender_pid: Pid, msg: IpcMessage) {
    match msg {
        IpcMessage::RenderDrawPixel { x, y, color } => {
            draw_pixel(x, y, color);
        }
        IpcMessage::RenderDrawRect { x, y, w, h, color } => {
            draw_rect(x, y, w, h, color);
        }
        IpcMessage::RenderDrawText { x, y, text, color } => {
            draw_text(x, y, text, color);
        }
        _ => {} // 忽略其他类型消息
    }
}

// 渲染服务入口函数
#[no_mangle]
pub extern "C" fn main() -> ! {
    unsafe {
        // 1. 注册服务到内核（让其他服务可通过"render"名称找到）
        let service_name = CStr::from_bytes_with_nul(b"render\0").expect("Invalid CStr");
        extern "C" {
            fn kernel_register_service(name: *const u8) -> bool;
        }
        let _ = kernel_register_service(service_name.as_ptr() as *const u8);

        // 2. 初始化渲染服务（连接GPU驱动）
        if !(&mut *core::ptr::addr_of_mut!(RENDER_STATE)).init() {
            // 初始化失败：进入死循环（内核可检测并重启服务）
            loop { hlt(); }
        }

        // 3. 测试：绘制初始化成功提示（左上角白色文字）
        draw_text(10, 10, "RUI Render Service Ready", Rgb { r: 255, g: 255, b: 255 });

        // 4. 消息循环：处理绘图请求
        loop {
            let mut sender_pid: Pid = 0;
            let mut msg = IpcMessage::RenderDrawPixel { x: 0, y: 0, color: Rgb { r: 0, g: 0, b: 0 } };
            kernel::ipc_recv(&mut sender_pid, &mut msg);
            handle_ipc_message(sender_pid, msg);
            hlt();
        }
    }
}

// panic处理（服务崩溃时安全停机）
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop { hlt(); }
}