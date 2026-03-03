/*
 * Copyright 2023 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include "app_ui.h"
#include "app_ui_music.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

// WiFi 图标点击回调
static void wifi_icon_click_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        app_wifi_connect();  // 打开 WiFi 连接界面
    }
}

// screen_1 手势事件处理：
// - 左滑：呼出 WiFi 界面
// - 右滑：呼出音乐播放器界面（MP3 UI）
void screen_1_gesture_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_GESTURE) {
        lv_indev_t *indev = lv_indev_get_act();
        lv_dir_t dir = lv_indev_get_gesture_dir(indev);
        
        // 防止一次滑动触发多次手势/点击
        lv_indev_wait_release(indev);

        if (dir == LV_DIR_LEFT) {
            // 左滑：呼出 WiFi 连接页面
            app_wifi_connect();
        } else if (dir == LV_DIR_RIGHT) {
            // 右滑：呼出音乐播放器界面
            app_music_open();
        }
    }
}

// 在 screen_1 右上角创建 WiFi 图标
void wifi_icon_create_on_screen_1(lv_ui *ui)
{
    if (ui->screen_1 == NULL) {
        return;  // screen_1 还没创建，直接返回
    }

    // 创建一个按钮作为 WiFi 图标容器
    lv_obj_t *btn_wifi = lv_btn_create(ui->screen_1);
    lv_obj_set_size(btn_wifi, 40, 40);
    lv_obj_align(btn_wifi, LV_ALIGN_TOP_RIGHT, -8, 8);  // 右上角，留 8px 边距
    
    // 设置按钮样式（可选：透明背景，只显示图标）
    lv_obj_set_style_bg_opa(btn_wifi, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn_wifi, 0, LV_PART_MAIN);
    
    // 在按钮里创建一个 label 显示 WiFi 符号
    lv_obj_t *label_wifi = lv_label_create(btn_wifi);
    lv_label_set_text(label_wifi, LV_SYMBOL_WIFI);  // LVGL 内置 WiFi 符号
    lv_obj_center(label_wifi);
    lv_obj_set_style_text_color(label_wifi, lv_color_hex(0xFFFFFF), LV_PART_MAIN);  // 白色图标
    lv_obj_set_style_text_font(label_wifi, &lv_font_montserratMedium_16, LV_PART_MAIN);
    
    // 绑定点击事件
    lv_obj_add_event_cb(btn_wifi, wifi_icon_click_cb, LV_EVENT_CLICKED, NULL);
}

/**
 * Create a demo application
 */
void custom_init(lv_ui *ui)
{
    /* 目前不需要在这里做事
     * WiFi 图标在 screen_1 创建完成后，
     * 由 events_init_screen_1() 调用 wifi_icon_create_on_screen_1() 来添加，
     * 这样可以保证 ui->screen_1 已经有效。
     */
}

