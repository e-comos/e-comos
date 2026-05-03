#![no_std]

// 基础类型：进程ID、窗口ID、像素值
pub type Pid = u64;
pub type WindowId = u32;
pub type Pixel = u32; // 24位色：0xRRGGBB

// RGB颜色结构体（安全的颜色操作）
#[derive(Debug, Clone, Copy, PartialEq)]
#[repr(C)]
pub struct Rgb {
    pub r: u8,
    pub g: u8,
    pub b: u8,
}
impl Rgb {
    // 转换为帧缓冲区的像素值（0xRRGGBB）
    pub fn to_pixel(&self) -> Pixel {
        ((self.r as Pixel) << 16) | ((self.g as Pixel) << 8) | (self.b as Pixel)
    }
}

// IPC消息类型：所有服务都用这套消息通信
#[derive(Debug)]
#[repr(C)]
pub enum IpcMessage {
    // GPU驱动相关消息（渲染服务→GPU驱动）
    GpuInit { width: u16, height: u16 }, // 设置分辨率
    GpuGetFb,                            // 请求帧缓冲区地址
    GpuFlush,                            // 刷新屏幕
    GpuFbReply { fb: *mut Pixel, width: u16, height: u16 }, // GPU驱动的回复

    // 渲染服务相关消息（窗口管理器→渲染服务）
    RenderDrawPixel { x: u16, y: u16, color: Rgb },
    RenderDrawRect { x: u16, y: u16, w: u16, h: u16, color: Rgb },

    WmCreateWindow {
        x: u16,
        y: u16,
        w: u16,
        h: u16,
        title: &'static str, // 窗口标题
    },

    // 鼠标事件消息（鼠标驱动→WM）
    MouseClick {
        x: u16,
        y: u16,
    },

    // 窗口管理器→应用：窗口已打开通知
     WmWindowOpened {
        title: &'static str,
        window_id: WindowId,
    },

    // 桌面→窗口管理器：切换窗口焦点
    WmFocusWindow {
        window_id: WindowId,
    },

    // 鼠标移动事件消息（鼠标驱动→WM）
    MouseMove {
        x: u16,
        y: u16,
    },

    // 渲染服务：绘制文字（补充之前的定义）
    RenderDrawText {
        x: u16,
        y: u16,
        text: &'static str,
        color: Rgb,
    },
}

// 内核提供的基础API（通过FFI调用，Rust包装）
pub mod kernel {
    use super::*;
    extern "C" {
        // 发送IPC消息：目标PID + 消息
        pub fn ipc_send(pid: Pid, msg: *const IpcMessage);
        // 接收IPC消息：返回发送者PID，消息通过指针传递
        pub fn ipc_recv(sender_pid: *mut Pid, msg: *mut IpcMessage);
        // 获取指定服务的PID（比如"gpu-driver"→对应的进程ID）
        pub fn get_service_pid(name: *const u8) -> Pid;
        // 映射物理地址到用户态（用于帧缓冲区）
        pub fn map_phys(phys_addr: u64, size: usize) -> *mut u8;
        // 注册服务到内核（让其他服务能通过名称找到本服务）
        pub fn kernel_register_service(name: *const u8) -> bool;
    }
}