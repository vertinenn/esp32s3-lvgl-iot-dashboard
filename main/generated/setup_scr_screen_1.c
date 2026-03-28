/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



int screen_1_digital_clock_1_min_value = 25;
int screen_1_digital_clock_1_hour_value = 11;
int screen_1_digital_clock_1_sec_value = 50;
char screen_1_digital_clock_1_meridiem[] = "AM";
void setup_scr_screen_1(lv_ui *ui)
{
    //Write codes screen_1
    ui->screen_1 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_1, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_img_1
    ui->screen_1_img_1 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_1, LV_OBJ_FLAG_CLICKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_img_set_src(ui->screen_1_img_1, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji5.png");
#else
    lv_img_set_src(ui->screen_1_img_1, "F:/heiji5.bin");
#endif
    lv_img_set_pivot(ui->screen_1_img_1, 50,50);
    lv_img_set_angle(ui->screen_1_img_1, 0);
    lv_obj_set_pos(ui->screen_1_img_1, -1, 0);
    lv_obj_set_size(ui->screen_1_img_1, 320, 240);

    //Write style for screen_1_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_imgbtn_1
    ui->screen_1_imgbtn_1 = lv_imgbtn_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_imgbtn_1, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imgbtn_set_src(ui->screen_1_imgbtn_1, LV_IMGBTN_STATE_RELEASED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji3.png", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_1, LV_IMGBTN_STATE_PRESSED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji3.png", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_1, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji1.png", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_1, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji1.png", NULL);
#else
    lv_imgbtn_set_src(ui->screen_1_imgbtn_1, LV_IMGBTN_STATE_RELEASED, NULL, "F:/heiji3.bin", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_1, LV_IMGBTN_STATE_PRESSED, NULL, "F:/heiji3.bin", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_1, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, "F:/heiji1.bin", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_1, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, "F:/heiji1.bin", NULL);
#endif
    ui->screen_1_imgbtn_1_label = lv_label_create(ui->screen_1_imgbtn_1);
    lv_label_set_text(ui->screen_1_imgbtn_1_label, "");
    lv_label_set_long_mode(ui->screen_1_imgbtn_1_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_imgbtn_1_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_imgbtn_1, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_1_imgbtn_1, 14, 182);
    lv_obj_set_size(ui->screen_1_imgbtn_1, 55, 55);

    //Write style for screen_1_imgbtn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_imgbtn_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_imgbtn_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_imgbtn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_imgbtn_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_1_imgbtn_1, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->screen_1_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_1_imgbtn_1, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_1_imgbtn_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_1_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_1_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_1_imgbtn_1, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->screen_1_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_1_imgbtn_1, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_1_imgbtn_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_1_imgbtn_1, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_1_imgbtn_1, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_1_imgbtn_1, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_imgbtn_1, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->screen_1_imgbtn_1, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes screen_1_imgbtn_2
    ui->screen_1_imgbtn_2 = lv_imgbtn_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_imgbtn_2, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imgbtn_set_src(ui->screen_1_imgbtn_2, LV_IMGBTN_STATE_RELEASED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji6.png", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_2, LV_IMGBTN_STATE_PRESSED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji6.png", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_2, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji1.png", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_2, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji1.png", NULL);
#else
    lv_imgbtn_set_src(ui->screen_1_imgbtn_2, LV_IMGBTN_STATE_RELEASED, NULL, "F:/heiji6.bin", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_2, LV_IMGBTN_STATE_PRESSED, NULL, "F:/heiji6.bin", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_2, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, "F:/heiji1.bin", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_2, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, "F:/heiji1.bin", NULL);
#endif
    ui->screen_1_imgbtn_2_label = lv_label_create(ui->screen_1_imgbtn_2);
    lv_label_set_text(ui->screen_1_imgbtn_2_label, "");
    lv_label_set_long_mode(ui->screen_1_imgbtn_2_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_imgbtn_2_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_imgbtn_2, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_1_imgbtn_2, 124, 182);
    lv_obj_set_size(ui->screen_1_imgbtn_2, 55, 55);

    //Write style for screen_1_imgbtn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_imgbtn_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_imgbtn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_imgbtn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_imgbtn_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_1_imgbtn_2, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->screen_1_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_1_imgbtn_2, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_1_imgbtn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_1_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_1_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_1_imgbtn_2, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->screen_1_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_1_imgbtn_2, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_1_imgbtn_2, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_1_imgbtn_2, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_1_imgbtn_2, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_1_imgbtn_2, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_imgbtn_2, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->screen_1_imgbtn_2, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes screen_1_imgbtn_3
    ui->screen_1_imgbtn_3 = lv_imgbtn_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_imgbtn_3, LV_OBJ_FLAG_CHECKABLE);
#if LV_USE_GUIDER_SIMULATOR
    lv_imgbtn_set_src(ui->screen_1_imgbtn_3, LV_IMGBTN_STATE_RELEASED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji4.png", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_3, LV_IMGBTN_STATE_PRESSED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji4.png", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_3, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji1.png", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_3, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, "C:\\Users\\TX\\Desktop\\gui_project\\esp32-s3\\import\\image\\heiji1.png", NULL);
#else
    lv_imgbtn_set_src(ui->screen_1_imgbtn_3, LV_IMGBTN_STATE_RELEASED, NULL, "F:/heiji4.bin", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_3, LV_IMGBTN_STATE_PRESSED, NULL, "F:/heiji4.bin", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_3, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, "F:/heiji1.bin", NULL);
    lv_imgbtn_set_src(ui->screen_1_imgbtn_3, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, "F:/heiji1.bin", NULL);
#endif
    ui->screen_1_imgbtn_3_label = lv_label_create(ui->screen_1_imgbtn_3);
    lv_label_set_text(ui->screen_1_imgbtn_3_label, "");
    lv_label_set_long_mode(ui->screen_1_imgbtn_3_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_1_imgbtn_3_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_1_imgbtn_3, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_1_imgbtn_3, 238, 182);
    lv_obj_set_size(ui->screen_1_imgbtn_3, 55, 55);

    //Write style for screen_1_imgbtn_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_imgbtn_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_imgbtn_3, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_imgbtn_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_imgbtn_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_1_imgbtn_3, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->screen_1_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_1_imgbtn_3, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_1_imgbtn_3, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_1_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_1_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_1_imgbtn_3, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->screen_1_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_1_imgbtn_3, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_1_imgbtn_3, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_1_imgbtn_3, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_1_imgbtn_3, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_1_imgbtn_3, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_imgbtn_3, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->screen_1_imgbtn_3, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes screen_1_digital_clock_1
    static bool screen_1_digital_clock_1_timer_enabled = false;
    ui->screen_1_digital_clock_1 = lv_dclock_create(ui->screen_1, "11:25:50 AM");
    if (!screen_1_digital_clock_1_timer_enabled) {
        lv_timer_create(screen_1_digital_clock_1_timer, 1000, NULL);
        screen_1_digital_clock_1_timer_enabled = true;
    }
    lv_obj_set_pos(ui->screen_1_digital_clock_1, 8, 31);
    lv_obj_set_size(ui->screen_1_digital_clock_1, 122, 39);

    //Write style for screen_1_digital_clock_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->screen_1_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_digital_clock_1, lv_color_hex(0x2dc105), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_digital_clock_1, &lv_font_Alatsi_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_digital_clock_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_digital_clock_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_digital_clock_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_digital_clock_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_datetext_1
    ui->screen_1_datetext_1 = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_datetext_1, "2023/07/31");
    lv_obj_set_style_text_align(ui->screen_1_datetext_1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_add_flag(ui->screen_1_datetext_1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(ui->screen_1_datetext_1, screen_1_datetext_1_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_pos(ui->screen_1_datetext_1, -1, 83);
    lv_obj_set_size(ui->screen_1_datetext_1, 149, 35);

    //Write style for screen_1_datetext_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_1_datetext_1, lv_color_hex(0x2dc105), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_datetext_1, &lv_font_Alatsi_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_datetext_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_datetext_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_datetext_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_1_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_datetext_1, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_datetext_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_label_1
    ui->screen_1_label_1 = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_label_1, "locate");
    lv_label_set_long_mode(ui->screen_1_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_1, 145, 118);
    lv_obj_set_size(ui->screen_1_label_1, 136, 25);

    //Write style for screen_1_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_1, lv_color_hex(0x2dc105), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_1, &lv_font_Alatsi_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_label_2
    ui->screen_1_label_2 = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_label_2, "weather");
    lv_label_set_long_mode(ui->screen_1_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_2, 154, 62);
    lv_obj_set_size(ui->screen_1_label_2, 123, 24);

    //Write style for screen_1_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_2, lv_color_hex(0x2dc105), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_2, &lv_font_Alatsi_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_img_2
    ui->screen_1_img_2 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_1_img_2, &_sun_alpha_32x32);
    lv_img_set_pivot(ui->screen_1_img_2, 50,50);
    lv_img_set_angle(ui->screen_1_img_2, 0);
    lv_obj_set_pos(ui->screen_1_img_2, 277, 62);
    lv_obj_set_size(ui->screen_1_img_2, 32, 32);

    //Write style for screen_1_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_label_3
    ui->screen_1_label_3 = lv_label_create(ui->screen_1);
    lv_label_set_text(ui->screen_1_label_3, "tem");
    lv_label_set_long_mode(ui->screen_1_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_1_label_3, 26, 134);
    lv_obj_set_size(ui->screen_1_label_3, 93, 25);

    //Write style for screen_1_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_1_label_3, lv_color_hex(0x2dc105), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_1_label_3, &lv_font_Alatsi_Regular_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_1_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_1_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_1_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_img_3
    ui->screen_1_img_3 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_1_img_3, &_wifi_alpha_32x32);
    lv_img_set_pivot(ui->screen_1_img_3, 50,50);
    lv_img_set_angle(ui->screen_1_img_3, 0);
    lv_obj_set_pos(ui->screen_1_img_3, 275, 12);
    lv_obj_set_size(ui->screen_1_img_3, 32, 32);

    //Write style for screen_1_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_img_4
    ui->screen_1_img_4 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_1_img_4, &_tem_alpha_32x32);
    lv_img_set_pivot(ui->screen_1_img_4, 50,50);
    lv_img_set_angle(ui->screen_1_img_4, 0);
    lv_obj_set_pos(ui->screen_1_img_4, -1, 130);
    lv_obj_set_size(ui->screen_1_img_4, 32, 32);

    //Write style for screen_1_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_1_img_5
    ui->screen_1_img_5 = lv_img_create(ui->screen_1);
    lv_obj_add_flag(ui->screen_1_img_5, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_1_img_5, &_locate_alpha_32x32);
    lv_img_set_pivot(ui->screen_1_img_5, 50,50);
    lv_img_set_angle(ui->screen_1_img_5, 0);
    lv_obj_set_pos(ui->screen_1_img_5, 275, 114);
    lv_obj_set_size(ui->screen_1_img_5, 32, 32);

    //Write style for screen_1_img_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_1_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_1_img_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_1_img_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_1_img_5, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_1.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_1);

    //Init events for screen.
    events_init_screen_1(ui);
}
