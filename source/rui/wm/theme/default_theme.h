#ifndef DEFAULT_THEME_H
#define DEFAULT_THEME_H

#include <stdint.h>

// 颜色结构体（与common模块的Rgb保持一致，确保跨语言兼容）
struct ThemeRgb {
    uint8_t r; // 红色通道（0-255）
    uint8_t g; // 绿色通道（0-255）
    uint8_t b; // 蓝色通道（0-255）
};

// ==========================
// 窗口基础尺寸（单位：像素）
// ==========================
#define TITLEBAR_HEIGHT        24  // 标题栏高度
#define WINDOW_BORDER_WIDTH    2   // 窗口边框宽度
#define BUTTON_SIZE            18  // 标题栏按钮（关闭/最小化）尺寸
#define BUTTON_MARGIN          3   // 标题栏按钮之间的间距

// ==========================
// 颜色定义（支持聚焦/非聚焦状态）
// ==========================

// 标题栏颜色
#define FOCUSED_TITLEBAR_COLOR     (struct ThemeRgb){0, 50, 150}   // 聚焦窗口标题栏：深蓝色
#define UNFOCUSED_TITLEBAR_COLOR   (struct ThemeRgb){180, 180, 180} // 非聚焦窗口标题栏：灰色
#define TITLE_TEXT_COLOR           (struct ThemeRgb){255, 255, 255} // 标题文字：白色

// 窗口边框颜色
#define FOCUSED_BORDER_COLOR       (struct ThemeRgb){0, 30, 100}   // 聚焦窗口边框：深一点的蓝色
#define UNFOCUSED_BORDER_COLOR     (struct ThemeRgb){200, 200, 200} // 非聚焦窗口边框：浅灰色

// 窗口背景颜色
#define WINDOW_BACKGROUND_COLOR    (struct ThemeRgb){240, 240, 240} // 窗口内容区：浅灰色
#define SCREEN_BACKGROUND_COLOR    (struct ThemeRgb){200, 200, 200} // 桌面背景：中灰色

// 标题栏按钮颜色
#define CLOSE_BUTTON_NORMAL        (struct ThemeRgb){200, 50, 50}   // 关闭按钮默认：红色
#define CLOSE_BUTTON_HOVER         (struct ThemeRgb){255, 70, 70}   // 关闭按钮hover：亮红色
#define MIN_BUTTON_NORMAL          (struct ThemeRgb){200, 200, 50}  // 最小化按钮默认：黄色
#define MIN_BUTTON_HOVER           (struct ThemeRgb){255, 255, 70}  // 最小化按钮hover：亮黄色

// ==========================
// 文字样式
// ==========================
#define TITLE_FONT_SIZE   12  // 标题文字大小（像素，对应8x8或12x12字体）
#define TITLE_TEXT_OFFSET 5   // 标题文字与标题栏左侧的间距

#endif // DEFAULT_THEME_H