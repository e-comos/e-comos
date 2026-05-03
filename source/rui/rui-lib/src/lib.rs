#![no_std]

use common::{WindowId, IpcMessage, kernel, Pid};
use core::ffi::CStr;

pub use common::Rgb;  // 保留这行
pub use common;  // 重新导出common模块

// 控件基类（所有控件继承此特性）
pub trait Widget {
    fn draw(&self, window_id: WindowId, render_pid: Pid);
    fn on_click(&mut self, x: u16, y: u16);
}

// 按钮控件
pub struct Button {
    x: u16,
    y: u16,
    width: u16,
    height: u16,
    _text: &'static str,
    color: Rgb,
    callback: fn(),  // 点击回调函数
}

impl Button {
    pub fn new(x: u16, y: u16, width: u16, height: u16, text: &'static str, color: Rgb, callback: fn()) -> Self {
        Button { x, y, width, height, _text: text, color, callback }
    }
}

impl Widget for Button {
    fn draw(&self, _window_id: WindowId, render_pid: Pid) {
        // 绘制按钮背景（调用渲染服务）
        let msg = IpcMessage::RenderDrawRect {
            x: self.x,
            y: self.y,
            w: self.width,
            h: self.height,
            color: self.color,
        };
        unsafe { kernel::ipc_send(render_pid, &msg); }
    }

    fn on_click(&mut self, x: u16, y: u16) {
        // 检查点击是否在按钮范围内
        if x >= self.x && x < self.x + self.width && y >= self.y && y < self.y + self.height {
            (self.callback)();  // 触发回调
        }
    }
}

// 窗口结构体（供应用创建窗口）
pub struct Window {
    id: WindowId,
    _wm_pid: Pid,  // 窗口管理器PID
}

impl Window {
    // 创建新窗口（向WM发送请求）
    pub fn new(title: &'static str, x: u16, y: u16, width: u16, height: u16) -> Self {
        unsafe {
            // 获取窗口管理器PID
            let wm_name = CStr::from_bytes_with_nul(b"wm\0").unwrap();
            let wm_pid = kernel::get_service_pid(wm_name.as_ptr() as *const u8);
            // 发送创建窗口消息
            let msg = IpcMessage::WmCreateWindow { x, y, w: width, h: height, title };
            kernel::ipc_send(wm_pid, &msg);

            // 简化：假设窗口ID为1（实际应从WM的回复中获取）
            Window { id: 1, _wm_pid: wm_pid }
        }
    }

    pub fn id(&self) -> WindowId {
        self.id
    }
}