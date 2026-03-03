/*
 * Copyright 2023 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

void custom_init(lv_ui *ui);

// 在 screen_1 上创建右上角 WiFi 图标
void wifi_icon_create_on_screen_1(lv_ui *ui);

// screen_1 手势事件处理函数（右滑呼出 WiFi，左滑退出 WiFi）
void screen_1_gesture_cb(lv_event_t *e);

#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */
