#pragma once

#include <stdbool.h>

// WiFi UI 对外接口
void app_wifi_connect(void);
void app_wifi_close(void);  // 关闭 WiFi 页面，返回主界面
bool app_wifi_is_open(void);
void app_wifi_init(void);   // 开机初始化 WiFi 协议栈（只执行一次）
