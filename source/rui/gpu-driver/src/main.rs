#![no_std]
#![no_main]

// 引入共享类型和内核API
use common::{IpcMessage, kernel, Pid, Pixel};
use core::{
    ptr::null_mut,
    ffi::CStr,
};
// x86_64 crate用于硬件指令（如hlt）
use x86_64::instructions::hlt;

// ==========================
// 显卡状态与VESA信息
// ==========================
/// VESA VBE模式信息（从内核获取）
#[derive(Debug, Clone)]
#[repr(C)]
struct VesaInfo {
    width: u16,
    height: u16,
    bpp: u8,
    fb_phys: u64,
}

/// GPU驱动全局状态
struct GpuState {
    vesa: Option<VesaInfo>, // VESA信息（初始化后才有值）
    fb_virt: *mut Pixel,    // 帧缓冲区用户态虚拟地址（映射后才有值）
}

impl GpuState {
    const fn new() -> Self {
        GpuState {
            vesa: None,
            fb_virt: null_mut(),
        }
    }
}

// 全局状态（单例，驱动唯一实例）
static mut GPU_STATE: GpuState = GpuState::new();

// ==========================
// 与内核交互：获取VESA信息
// ==========================
/// 从内核获取VESA VBE模式信息（引导程序已在实模式下初始化VESA）
unsafe fn get_vesa_info_from_kernel() -> Option<VesaInfo> {
    // 内核提供的接口：返回VESA信息结构体指针
    extern "C" {
        fn kernel_get_vesa_info() -> *const VesaInfo;
    }
    let vesa_ptr = kernel_get_vesa_info();
    if vesa_ptr.is_null() {
        return None; // 内核未提供VESA信息（如显卡不支持）
    }
    Some((*vesa_ptr).clone()) // 复制内核提供的VESA信息
}

// ==========================
// 帧缓冲区映射：物理→虚拟
// ==========================
/// 将显卡帧缓冲区物理地址映射到用户态虚拟地址
unsafe fn map_framebuffer(phys_addr: u64, size: usize) -> *mut Pixel {
    // 调用内核的内存映射API：phys_addr → 虚拟地址
    let virt_addr = kernel::map_phys(phys_addr, size);
    virt_addr as *mut Pixel // 转换为Pixel指针（方便操作像素）
}

// ==========================
// 硬件刷新：通知显卡更新屏幕
// ==========================
/// 触发屏幕刷新（部分VESA显卡需要显式调用）
unsafe fn flush_screen() {
    // 对于VESA VBE，多数情况下写入帧缓冲区后自动刷新
    // 如需显式刷新，可通过out指令操作显卡寄存器（示例）：
    // x86_64::instructions::port::outw(0x3D4, 0x0A); // 具体端口因显卡而异
}

// ==========================
// IPC消息处理：响应渲染服务
// ==========================
/// 处理来自渲染服务的消息
unsafe fn handle_ipc_message(sender_pid: Pid, msg: IpcMessage) {
    match msg {
        // 渲染服务请求初始化显卡
        IpcMessage::GpuInit { width: _, height: _ } => {
            // 1. 从内核获取VESA信息
            if let Some(vesa) = get_vesa_info_from_kernel() {
                GPU_STATE.vesa = Some(vesa.clone());
                // 2. 计算帧缓冲区大小（宽×高×每像素字节数）
                let pixel_size = vesa.bpp / 8; // 如24bpp → 3字节/像素（但我们用u32对齐）
                let fb_size = (vesa.width as usize) * (vesa.height as usize) * (pixel_size as usize);
                // 3. 映射帧缓冲区到用户态
                GPU_STATE.fb_virt = map_framebuffer(vesa.fb_phys, fb_size);
                // 4. 回复渲染服务：帧缓冲区信息
                let reply = IpcMessage::GpuFbReply {
                    fb: GPU_STATE.fb_virt,
                    width: vesa.width,
                    height: vesa.height,
                };
                kernel::ipc_send(sender_pid, &reply);
            } else {
                // 初始化失败：回复空帧缓冲区
                let reply = IpcMessage::GpuFbReply {
                    fb: null_mut(),
                    width: 0,
                    height: 0,
                };
                kernel::ipc_send(sender_pid, &reply);
            }
        }

        // 渲染服务请求获取帧缓冲区信息
        IpcMessage::GpuGetFb => {
            if let Some(vesa) = unsafe { &*core::ptr::addr_of!(GPU_STATE.vesa) } {
                let reply = IpcMessage::GpuFbReply {
                    fb: GPU_STATE.fb_virt,
                    width: vesa.width,
                    height: vesa.height,
                };
                kernel::ipc_send(sender_pid, &reply);
            } else {
                // 未初始化：回复空
                let reply = IpcMessage::GpuFbReply {
                    fb: null_mut(),
                    width: 0,
                    height: 0,
                };
                kernel::ipc_send(sender_pid, &reply);
            }
        }

        // 渲染服务请求刷新屏幕
        IpcMessage::GpuFlush => {
            flush_screen();
        }

        // 忽略其他类型消息
        _ => {}
    }
}

// ==========================
// 驱动入口与主循环
// ==========================
#[no_mangle]
pub extern "C" fn main() -> ! {
    unsafe {
        // 1. 注册驱动到内核（让其他服务能通过"gpu-driver"名称找到本服务）
        let driver_name = CStr::from_bytes_with_nul(b"gpu-driver\0").unwrap();
        extern "C" {
            fn kernel_register_service(name: *const u8) -> bool;
        }
        let _ = kernel_register_service(driver_name.as_ptr() as *const u8); // 忽略注册结果（简化）

        // 2. 主循环：等待并处理IPC消息
        loop {
            let mut sender_pid: Pid = 0;
            let mut msg = IpcMessage::GpuInit { width: 0, height: 0 };
            kernel::ipc_recv(&mut sender_pid, &mut msg); // 阻塞等待消息
            handle_ipc_message(sender_pid, msg);       // 处理消息
            hlt(); // 停机等待下一条消息（降低CPU占用）
        }
    }
}

// ==========================
// 异常处理（no_std必需）
// ==========================
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    // 驱动崩溃时简单停机（内核可检测到并重启驱动）
    loop {
        hlt();
    }
}