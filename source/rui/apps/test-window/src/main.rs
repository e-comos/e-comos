#![no_std]
#![no_main]

use rui_lib::{Window, Button, common::Rgb,Widget};
use x86_64::instructions::hlt;
// 按钮点击回调：暂时只做占位
fn on_button_click() {
    // 后续可扩展：发送消息给窗口管理器修改窗口
}

#[no_mangle]
pub extern "C" fn main() -> ! {
    // 创建窗口
    let window = Window::new("测试窗口", 100, 100, 400, 300);

    // 创建按钮
    let button = Button::new(
        50, 50, 100, 30, "点击我",
        Rgb { r: 0, g: 120, b: 215 },  // 蓝色按钮
        on_button_click
    );

    // 绘制按钮
    let render_pid = unsafe {
        let render_name = core::ffi::CStr::from_bytes_with_nul(b"render\0").unwrap();
        common::kernel::get_service_pid(render_name.as_ptr() as *const u8)
    };
    button.draw(window.id(), render_pid);

    // 事件循环
    loop {
        hlt();
    }
}

#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop { hlt(); }
}