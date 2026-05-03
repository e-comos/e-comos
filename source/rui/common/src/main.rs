#![no_std]
#![no_main]

// 引入common自身的类型和内核API
use common::{Rgb, IpcMessage, kernel, Pid, WindowId};
// no_std环境下需要核心库的调试功能
use core::fmt::Write;

// 自定义一个简单的"调试输出"（假设内核提供了这个打印函数，没有的话可以注释掉）
extern "C" {
    fn debug_print(s: *const u8);
}

// 封装调试打印，方便用字符串
fn print(s: &str) {
    unsafe {
        // 字符串结尾加个null终止符，符合C函数的预期
        let mut buf = s.as_bytes().to_vec();
        buf.push(0);
        debug_print(buf.as_ptr());
    }
}

// no_std环境的入口点（不能用普通main，需要标记no_mangle）
#[no_mangle]
pub extern "C" fn main() -> ! {
    // 1. 测试Rgb颜色转换
    let red = Rgb { r: 0xFF, g: 0x00, b: 0x00 };
    let red_pixel = red.to_pixel();
    if red_pixel == 0xFF0000 {
        print("✅ Rgb.to_pixel() 测试通过：红色转换正确\n");
    } else {
        print(&format!("❌ 红色转换错误，预期0xFF0000，实际0x{:X}\n", red_pixel));
    }

    // 2. 测试创建各种IpcMessage（验证语法和内存布局）
    // 测试GpuInit消息
    let gpu_init = IpcMessage::GpuInit { width: 1024, height: 768 };
    print(&format!("✅ 创建GpuInit消息：{:?}\n", gpu_init));

    // 测试RenderDrawRect消息
    let rect_msg = IpcMessage::RenderDrawRect {
        x: 10, y: 20, w: 100, h: 50,
        color: Rgb { r: 0x00, g: 0xFF, b: 0x00 } // 绿色
    };
    print(&format!("✅ 创建矩形绘制消息：{:?}\n", rect_msg));

    // 测试窗口创建消息
    let create_win = IpcMessage::WmCreateWindow {
        x: 50, y: 50, w: 800, h: 600,
        title: "测试窗口"
    };
    print(&format!("✅ 创建窗口消息：{:?}\n", create_win));

    // 3. 测试内核API的调用语法（不实际执行，只验证类型匹配）
    let dummy_pid: Pid = 123;
    let dummy_window_id: WindowId = 456;

    // 模拟发送消息（注意：实际运行需要内核支持，这里只检查编译）
    unsafe {
        // 发送一个鼠标点击消息
        let mouse_click = IpcMessage::MouseClick { x: 150, y: 250 };
        kernel::ipc_send(dummy_pid, &mouse_click as *const _);
        print("✅ 内核ipc_send调用语法正确\n");

        // 模拟获取服务PID（比如获取GPU驱动的PID）
        let gpu_name = b"gpu-driver\0"; // C字符串需要null终止
        let gpu_pid = kernel::get_service_pid(gpu_name.as_ptr());
        print(&format!("✅ 获取服务PID语法正确，模拟结果：{}\n", gpu_pid));
    }

    // 裸机环境需要死循环（不能return）
    loop {
        unsafe { x86_64::instructions::hlt(); } // 暂停CPU，等待中断
    }
}

// no_std环境必须的panic处理
#[panic_handler]
fn panic(info: &core::panic::PanicInfo) -> ! {
    print(&format!("❌ 发生恐慌：{}\n", info));
    loop { unsafe { x86_64::instructions::hlt(); } }
}