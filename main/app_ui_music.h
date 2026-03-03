#pragma once

#include <stdbool.h>

// 兼容原有接口：初始化 MP3 播放器并显示界面
void mp3_player_init(void);

// 仅负责在当前屏幕上创建音乐播放界面（由 app_music_open() 调用）
void music_ui(void);

// 类似 WiFi 的对外接口：通过 custom 手势/按钮控制
void app_music_open(void);
void app_music_close(void);
bool app_music_is_open(void);

