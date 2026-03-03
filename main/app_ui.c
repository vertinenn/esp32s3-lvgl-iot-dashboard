#include "app_ui.h"
#include "esp32_s3_szp.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "weather_client.h"
#include "gui_guider.h"

static const char *TAG = "app_ui";

LV_FONT_DECLARE(font_alipuhui20);

extern lv_ui guider_ui;

extern int screen_1_digital_clock_1_hour_value;
extern int screen_1_digital_clock_1_min_value;
extern int screen_1_digital_clock_1_sec_value;
extern char screen_1_digital_clock_1_meridiem[];

lv_obj_t *wifi_scan_page;     // wifi扫描页面 obj
lv_obj_t *wifi_connect_page;  // wifi连接页面 obj
lv_obj_t *wifi_password_page; // wifi密码页面 obj
lv_obj_t *wifi_list;          // wifi列表  list
lv_obj_t *label_wifi_connect; // wifi连接页面label 
lv_obj_t *ta_pass_text;       // 密码输入文本框 textarea
lv_obj_t *roller_num;         // 数字roller
lv_obj_t *roller_letter_low;  // 小写字母roller
lv_obj_t *roller_letter_up;   // 大写字母roller
lv_obj_t *label_wifi_name;    // wifi名称label

// “WLAN扫描中...” 提示文本对象，用于异步扫描任务中安全删除
static lv_obj_t *label_wifi_scan = NULL;
// 当前正在运行的 WiFi 扫描任务句柄（避免重复创建）
static TaskHandle_t s_wifi_scan_task_handle = NULL;

#define DEFAULT_SCAN_LIST_SIZE   10                // 最大扫描wifi个数

// wifi事件组
static EventGroupHandle_t s_wifi_event_group;
static bool s_wifi_stack_inited = false;
static esp_event_handler_instance_t s_instance_any_id;
static esp_event_handler_instance_t s_instance_got_ip;
static esp_netif_t *s_sta_netif = NULL;
// wifi事件
#define WIFI_CONNECTED_BIT    BIT0
#define WIFI_FAIL_BIT         BIT1
#define WIFI_START_BIT        BIT2
// wifi最大重连次数
#define EXAMPLE_ESP_MAXIMUM_RETRY  3

// wifi账号队列
static QueueHandle_t xQueueWifiAccount = NULL;
static bool s_wifi_connect_task_started = false;
// 队列要传输的内容
typedef struct {
    char wifi_ssid[32];  // 获取wifi名称
    char wifi_password[64]; // 获取wifi密码         
} wifi_account_t;

static void style_init_or_reset(lv_style_t *style)
{
    if (style->prop_cnt > 1) {
        lv_style_reset(style);
    } else {
        lv_style_init(style);
    }
}

// 密码roller的遮罩显示效果
static void mask_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    static int16_t mask_top_id = -1;
    static int16_t mask_bottom_id = -1;

    if(code == LV_EVENT_COVER_CHECK) {
        lv_event_set_cover_res(e, LV_COVER_RES_MASKED);
    }
    else if(code == LV_EVENT_DRAW_MAIN_BEGIN) {
        /* add mask */
        const lv_font_t * font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        lv_coord_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);
        lv_coord_t font_h = lv_font_get_line_height(font);

        lv_area_t roller_coords;
        lv_obj_get_coords(obj, &roller_coords);

        lv_area_t rect_area;
        rect_area.x1 = roller_coords.x1;
        rect_area.x2 = roller_coords.x2;
        rect_area.y1 = roller_coords.y1;
        rect_area.y2 = roller_coords.y1 + (lv_obj_get_height(obj) - font_h - line_space) / 2;

        lv_draw_mask_fade_param_t * fade_mask_top = lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_top, &rect_area, LV_OPA_TRANSP, rect_area.y1, LV_OPA_COVER, rect_area.y2);
        mask_top_id = lv_draw_mask_add(fade_mask_top, NULL);

        rect_area.y1 = rect_area.y2 + font_h + line_space - 1;
        rect_area.y2 = roller_coords.y2;

        lv_draw_mask_fade_param_t * fade_mask_bottom = lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_bottom, &rect_area, LV_OPA_COVER, rect_area.y1, LV_OPA_TRANSP, rect_area.y2);
        mask_bottom_id = lv_draw_mask_add(fade_mask_bottom, NULL);

    }
    else if(code == LV_EVENT_DRAW_POST_END) {
        lv_draw_mask_fade_param_t * fade_mask_top = lv_draw_mask_remove_id(mask_top_id);
        lv_draw_mask_fade_param_t * fade_mask_bottom = lv_draw_mask_remove_id(mask_bottom_id);
        lv_draw_mask_free_param(fade_mask_top);
        lv_draw_mask_free_param(fade_mask_bottom);
        lv_mem_buf_release(fade_mask_top);
        lv_mem_buf_release(fade_mask_bottom);
        mask_top_id = -1;
        mask_bottom_id = -1;
    }
}

// 数字键 处理函数
static void btn_num_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "Btn-Num Clicked");
        char buf[2]; // 接收roller的值
        lv_roller_get_selected_str(roller_num, buf, sizeof(buf));
        lv_textarea_add_text(ta_pass_text, buf);
    }
}

// 小写字母确认键 处理函数
static void btn_letter_low_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "Btn-Letter-Low Clicked");
        char buf[2]; // 接收roller的值
        lv_roller_get_selected_str(roller_letter_low, buf, sizeof(buf));
        lv_textarea_add_text(ta_pass_text, buf);
    }
}

// 大写字母确认键 处理函数
static void btn_letter_up_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "Btn-Letter-Up Clicked");
        char buf[2]; // 接收roller的值
        lv_roller_get_selected_str(roller_letter_up, buf, sizeof(buf));
        lv_textarea_add_text(ta_pass_text, buf);
    }
}

// WiFi 覆盖页附加手势（在文件后面实现，这里先声明）
static void wifi_attach_page_gesture(lv_obj_t *page);

static void lv_wifi_connect(void)
{
    lv_obj_del(wifi_password_page); // 删除密码输入界面
    wifi_password_page = NULL;

    // 创建一个面板对象
    static lv_style_t style;
    style_init_or_reset(&style);
    lv_style_set_bg_opa( &style, LV_OPA_COVER );
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 0);
    lv_style_set_radius(&style, 0);  
    lv_style_set_width(&style, 320);  
    lv_style_set_height(&style, 240); 

    wifi_connect_page = lv_obj_create(lv_scr_act());
    lv_obj_add_style(wifi_connect_page, &style, 0);
    wifi_attach_page_gesture(wifi_connect_page);

    // 绘制label提示
    label_wifi_connect = lv_label_create(wifi_connect_page);
    lv_label_set_text(label_wifi_connect, "WLAN连接中...");
    lv_obj_set_style_text_font(label_wifi_connect, &font_alipuhui20, 0);
    lv_obj_align(label_wifi_connect, LV_ALIGN_CENTER, 0, -50);
}

// WiFi连接按钮 处理函数
static void btn_connect_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "OK Clicked");
        const char *wifi_ssid = lv_label_get_text(label_wifi_name);
        const char *wifi_password = lv_textarea_get_text(ta_pass_text);
        if(*wifi_password != '\0') // 判断是否为空字符串
        {
            wifi_account_t wifi_account;
            strcpy(wifi_account.wifi_ssid, wifi_ssid);
            strcpy(wifi_account.wifi_password, wifi_password);
            ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                    wifi_account.wifi_ssid, wifi_account.wifi_password);
            lv_wifi_connect(); // 显示wifi连接界面
            // 发送WiFi账号密码信息到队列
            xQueueSend(xQueueWifiAccount, &wifi_account, portMAX_DELAY); 
        }
    }
}

// 删除密码按钮
static void btn_del_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "Clicked");
        lv_textarea_del_char(ta_pass_text);
    }
}

// 返回按钮
static void btn_back_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        ESP_LOGI(TAG, "btn_back Clicked");
        lv_obj_del(wifi_password_page); // 删除密码输入界面
        wifi_password_page = NULL;
    }
}

// 进入输入密码界面
static void list_btn_cb(lv_event_t * e)
{
    // 获取点击到的WiFi名称
    const char *wifi_name=NULL;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        wifi_name = lv_list_get_btn_text(wifi_list, obj);
        ESP_LOGI(TAG, "WLAN Name: %s", wifi_name);
    }

    // 创建密码输入页面
    wifi_password_page = lv_obj_create(lv_scr_act());
    wifi_attach_page_gesture(wifi_password_page);
    lv_obj_set_size(wifi_password_page, 320, 240);
    lv_obj_set_style_border_width(wifi_password_page, 0, 0); // 设置边框宽度
    lv_obj_set_style_pad_all(wifi_password_page, 0, 0);  // 设置间隙
    lv_obj_set_style_radius(wifi_password_page, 0, 0); // 设置圆角

    // 创建返回按钮
    lv_obj_t *btn_back = lv_btn_create(wifi_password_page);
    lv_obj_align(btn_back, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(btn_back, 60, 40);
    lv_obj_set_style_border_width(btn_back, 0, 0); // 设置边框宽度
    lv_obj_set_style_pad_all(btn_back, 0, 0);  // 设置间隙
    lv_obj_set_style_bg_opa(btn_back, LV_OPA_TRANSP, LV_PART_MAIN); // 背景透明
    lv_obj_set_style_shadow_opa(btn_back, LV_OPA_TRANSP, LV_PART_MAIN); // 阴影透明
    lv_obj_add_event_cb(btn_back, btn_back_cb, LV_EVENT_ALL, NULL); // 添加按键处理函数

    lv_obj_t *label_back = lv_label_create(btn_back); 
    lv_label_set_text(label_back, LV_SYMBOL_LEFT);  // 按键上显示左箭头符号
    lv_obj_set_style_text_font(label_back, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label_back, lv_color_hex(0x000000), 0); 
    lv_obj_align(label_back, LV_ALIGN_TOP_LEFT, 10, 10);

    // 显示选中的wifi名称
    label_wifi_name = lv_label_create(wifi_password_page);
    lv_obj_set_style_text_font(label_wifi_name, &font_alipuhui20, 0);
    lv_label_set_text(label_wifi_name, wifi_name);
    lv_obj_align(label_wifi_name, LV_ALIGN_TOP_MID, 0, 10);

    // 创建密码输入框
    ta_pass_text = lv_textarea_create(wifi_password_page);
    lv_obj_set_style_text_font(ta_pass_text, &lv_font_montserrat_20, 0);
    lv_textarea_set_one_line(ta_pass_text, true);  // 一行显示
    lv_textarea_set_password_mode(ta_pass_text, false); // 是否使用密码输入显示模式
    lv_textarea_set_placeholder_text(ta_pass_text, "password"); // 设置提醒词
    lv_obj_set_width(ta_pass_text, 150); // 宽度
    lv_obj_align(ta_pass_text, LV_ALIGN_TOP_LEFT, 10, 40); // 位置
    lv_obj_add_state(ta_pass_text, LV_STATE_FOCUSED); // 显示光标

    // 创建“连接按钮”
    lv_obj_t *btn_connect = lv_btn_create(wifi_password_page);
    lv_obj_align(btn_connect, LV_ALIGN_TOP_LEFT, 170, 40);
    lv_obj_set_width(btn_connect, 65); // 宽度
    lv_obj_add_event_cb(btn_connect, btn_connect_cb, LV_EVENT_ALL, NULL); // 事件处理函数

    lv_obj_t *label_ok = lv_label_create(btn_connect);
    lv_label_set_text(label_ok, "OK");
    lv_obj_set_style_text_font(label_ok, &lv_font_montserrat_20, 0);
    lv_obj_center(label_ok);

    // 创建“删除按钮”
    lv_obj_t *btn_del = lv_btn_create(wifi_password_page);
    lv_obj_align(btn_del, LV_ALIGN_TOP_LEFT, 245, 40);
    lv_obj_set_width(btn_del, 65); // 宽度
    lv_obj_add_event_cb(btn_del, btn_del_cb, LV_EVENT_ALL, NULL);  // 事件处理函数

    lv_obj_t *label_del = lv_label_create(btn_del);
    lv_label_set_text(label_del, LV_SYMBOL_BACKSPACE);
    lv_obj_set_style_text_font(label_del, &lv_font_montserrat_20, 0);
    lv_obj_center(label_del);

    // 创建roller样式
    static lv_style_t style;
    style_init_or_reset(&style);
    lv_style_set_bg_color(&style, lv_color_black());
    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_border_width(&style, 0);
    lv_style_set_pad_all(&style, 0);
    lv_style_set_radius(&style, 0);

    // 创建"数字"roller
    const char * opts_num = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9";

    roller_num = lv_roller_create(wifi_password_page);
    lv_obj_add_style(roller_num, &style, 0);
    lv_obj_set_style_bg_opa(roller_num, LV_OPA_50, LV_PART_SELECTED);

    lv_roller_set_options(roller_num, opts_num, LV_ROLLER_MODE_INFINITE);
    lv_roller_set_visible_row_count(roller_num, 3); // 显示3行
    lv_roller_set_selected(roller_num, 5, LV_ANIM_OFF); // 默认选择
    lv_obj_set_width(roller_num, 90);
    lv_obj_set_style_text_font(roller_num, &lv_font_montserrat_20, 0);
    lv_obj_align(roller_num, LV_ALIGN_BOTTOM_LEFT, 15, -53);
    lv_obj_add_event_cb(roller_num, mask_event_cb, LV_EVENT_ALL, NULL); // 事件处理函数

    // 创建"数字"roller 的确认键
    lv_obj_t *btn_num_ok = lv_btn_create(wifi_password_page);
    lv_obj_align(btn_num_ok, LV_ALIGN_BOTTOM_LEFT, 15, -10); // 位置
    lv_obj_set_width(btn_num_ok, 90); // 宽度
    lv_obj_add_event_cb(btn_num_ok, btn_num_cb, LV_EVENT_ALL, NULL); // 事件处理函数

    lv_obj_t *label_num_ok = lv_label_create(btn_num_ok);
    lv_label_set_text(label_num_ok, LV_SYMBOL_OK);
    lv_obj_set_style_text_font(label_num_ok, &lv_font_montserrat_20, 0);
    lv_obj_center(label_num_ok);

    // 创建"小写字母"roller
    const char * opts_letter_low = "a\nb\nc\nd\ne\nf\ng\nh\ni\nj\nk\nl\nm\nn\no\np\nq\nr\ns\nt\nu\nv\nw\nx\ny\nz";

    roller_letter_low = lv_roller_create(wifi_password_page);
    lv_obj_add_style(roller_letter_low, &style, 0);
    lv_obj_set_style_bg_opa(roller_letter_low, LV_OPA_50, LV_PART_SELECTED); // 设置选中项的透明度
    lv_roller_set_options(roller_letter_low, opts_letter_low, LV_ROLLER_MODE_INFINITE); // 循环滚动模式
    lv_roller_set_visible_row_count(roller_letter_low, 3);
    lv_roller_set_selected(roller_letter_low, 15, LV_ANIM_OFF); // 
    lv_obj_set_width(roller_letter_low, 90);
    lv_obj_set_style_text_font(roller_letter_low, &lv_font_montserrat_20, 0);
    lv_obj_align(roller_letter_low, LV_ALIGN_BOTTOM_LEFT, 115, -53);
    lv_obj_add_event_cb(roller_letter_low, mask_event_cb, LV_EVENT_ALL, NULL); // 事件处理函数

    // 创建"小写字母"roller的确认键
    lv_obj_t *btn_letter_low_ok = lv_btn_create(wifi_password_page);
    lv_obj_align(btn_letter_low_ok, LV_ALIGN_BOTTOM_LEFT, 115, -10);
    lv_obj_set_width(btn_letter_low_ok, 90); // 宽度
    lv_obj_add_event_cb(btn_letter_low_ok, btn_letter_low_cb, LV_EVENT_ALL, NULL); // 事件处理函数

    lv_obj_t *label_letter_low_ok = lv_label_create(btn_letter_low_ok);
    lv_label_set_text(label_letter_low_ok, LV_SYMBOL_OK);
    lv_obj_set_style_text_font(label_letter_low_ok, &lv_font_montserrat_20, 0);
    lv_obj_center(label_letter_low_ok);

    // 创建"大写字母"roller
    const char * opts_letter_up = "A\nB\nC\nD\nE\nF\nG\nH\nI\nJ\nK\nL\nM\nN\nO\nP\nQ\nR\nS\nT\nU\nV\nW\nX\nY\nZ";

    roller_letter_up = lv_roller_create(wifi_password_page);
    lv_obj_add_style(roller_letter_up, &style, 0);
    lv_obj_set_style_bg_opa(roller_letter_up, LV_OPA_50, LV_PART_SELECTED); // 设置选中项的透明度
    lv_roller_set_options(roller_letter_up, opts_letter_up, LV_ROLLER_MODE_INFINITE); // 循环滚动模式
    lv_roller_set_visible_row_count(roller_letter_up, 3);
    lv_roller_set_selected(roller_letter_up, 15, LV_ANIM_OFF);
    lv_obj_set_width(roller_letter_up, 90);
    lv_obj_set_style_text_font(roller_letter_up, &lv_font_montserrat_20, 0);
    lv_obj_align(roller_letter_up, LV_ALIGN_BOTTOM_LEFT, 215, -53);
    lv_obj_add_event_cb(roller_letter_up, mask_event_cb, LV_EVENT_ALL, NULL); // 事件处理函数

    // 创建"大写字母"roller的确认键
    lv_obj_t *btn_letter_up_ok = lv_btn_create(wifi_password_page);
    lv_obj_align(btn_letter_up_ok, LV_ALIGN_BOTTOM_LEFT, 215, -10);
    lv_obj_set_width(btn_letter_up_ok, 90); 
    lv_obj_add_event_cb(btn_letter_up_ok, btn_letter_up_cb, LV_EVENT_ALL, NULL); // 事件处理函数

    lv_obj_t *label_letter_up_ok = lv_label_create(btn_letter_up_ok);
    lv_label_set_text(label_letter_up_ok, LV_SYMBOL_OK);
    lv_obj_set_style_text_font(label_letter_up_ok, &lv_font_montserrat_20, 0);
    lv_obj_center(label_letter_up_ok);

}

static void weather_task(void *pvParameters)
{
    weather_now_t now;
    esp_err_t err = weather_get_now(&now);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Weather fetched in task: city=%s, text=%s, temp=%s, last=%s",
                 now.city, now.text, now.temperature, now.last_update);

        /* 更新主界面的天气相关 UI */
        lvgl_port_lock(0);
        if (lv_obj_is_valid(guider_ui.screen_1_label_1)) {
            // 城市名称
            lv_label_set_text(guider_ui.screen_1_label_1, now.city);
            lv_obj_set_style_text_font(guider_ui.screen_1_label_1, &font_alipuhui20, LV_PART_MAIN);
        }
        if (lv_obj_is_valid(guider_ui.screen_1_label_2)) {
            /* 天气描述（中文） */
            lv_label_set_text(guider_ui.screen_1_label_2, now.text);
            lv_obj_set_style_text_font(guider_ui.screen_1_label_2, &font_alipuhui20, LV_PART_MAIN);
        }
        if (lv_obj_is_valid(guider_ui.screen_1_label_3)) {
            /* 温度显示为 “数字+℃”，使用中文字体 */
            char tbuf[16];
            snprintf(tbuf, sizeof(tbuf), "%s℃", now.temperature);
            lv_label_set_text(guider_ui.screen_1_label_3, tbuf);
            lv_obj_set_style_text_font(guider_ui.screen_1_label_3, &font_alipuhui20, LV_PART_MAIN);
        }
        if (lv_obj_is_valid(guider_ui.screen_1_datetext_1) && now.last_update[0] != '\0') {
            int year, month, day, hour, min, sec;
            if (sscanf(now.last_update, "%4d-%2d-%2dT%2d:%2d:%2d",
                       &year, &month, &day, &hour, &min, &sec) == 6) {
                char date_buf[20];
                snprintf(date_buf, sizeof(date_buf), "%04d/%02d/%02d", year, month, day);
                lv_label_set_text(guider_ui.screen_1_datetext_1, date_buf);

                /* 同步更新数字时钟初始值（12 小时制） */
                int hour12 = hour;
                const char *meridiem = "AM";
                if (hour == 0) {
                    hour12 = 12;
                    meridiem = "AM";
                } else if (hour == 12) {
                    hour12 = 12;
                    meridiem = "PM";
                } else if (hour > 12) {
                    hour12 = hour - 12;
                    meridiem = "PM";
                } else {
                    meridiem = "AM";
                }
                screen_1_digital_clock_1_hour_value = hour12;
                screen_1_digital_clock_1_min_value = min;
                screen_1_digital_clock_1_sec_value = sec;
                strncpy(screen_1_digital_clock_1_meridiem, meridiem, 3);
            }
        }
        lvgl_port_unlock();
    } else {
        ESP_LOGE(TAG, "Failed to fetch weather: %s", esp_err_to_name(err));
    }

    vTaskDelete(NULL);
}

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    static int s_retry_num = 0;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        xEventGroupSetBits(s_wifi_event_group, WIFI_START_BIT);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    
        // 在这里启动天气任务（只拉一次天气）
        xTaskCreate(weather_task, "weather_task", 4096, NULL, 5, NULL);
    }

}

static void wifi_stack_init_once(void)
{
    if(s_wifi_stack_inited) {
        return;
    }

    if(s_wifi_event_group == NULL) {
        s_wifi_event_group = xEventGroupCreate();
    }

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    s_sta_netif = esp_netif_create_default_wifi_sta();
    assert(s_sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &s_instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &s_instance_got_ip));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    s_wifi_stack_inited = true;
}

void app_wifi_init(void)
{
    wifi_stack_init_once();
}

// 扫描附近wifi
static void wifi_scan(wifi_ap_record_t ap_info[], uint16_t *ap_number)
{
    wifi_stack_init_once();

    uint16_t ap_count = 0;
    
    memset(ap_info, 0, *ap_number * sizeof(wifi_ap_record_t));

    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));

    ESP_LOGI(TAG, "Max AP number ap_info can hold = %u", *ap_number);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));  // 获取扫描到的wifi数量
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(ap_number, ap_info)); // 获取真实的获取到wifi数量和信息
    ESP_LOGI(TAG, "Total APs scanned = %u, actual AP number ap_info holds = %u", ap_count, *ap_number);
}

// 异步扫描任务：在后台执行 WiFi 扫描并刷新 UI 列表
static void wifi_scan_task(void *arg)
{
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_number = DEFAULT_SCAN_LIST_SIZE;

    wifi_scan(ap_info, &ap_number);  // 阻塞当前任务执行扫描

    lvgl_port_lock(0);
    if (wifi_scan_page && lv_obj_is_valid(wifi_scan_page)) {
        // 删除 “WLAN扫描中...” 提示
        if (label_wifi_scan && lv_obj_is_valid(label_wifi_scan)) {
            lv_obj_del(label_wifi_scan);
            label_wifi_scan = NULL;
        }

        // 创建或清空列表
        if (!wifi_list || !lv_obj_is_valid(wifi_list)) {
            wifi_list = lv_list_create(wifi_scan_page);
            lv_obj_set_size(wifi_list, lv_pct(100), lv_pct(100));
            lv_obj_set_style_border_width(wifi_list, 0, 0);
            lv_obj_set_style_text_font(wifi_list, &font_alipuhui20, 0);
            lv_obj_set_scrollbar_mode(wifi_list, LV_SCROLLBAR_MODE_OFF); // 隐藏wifi_list滚动条
        } else {
            lv_obj_clean(wifi_list);
        }

        // 显示 WiFi 信息
        lv_obj_t *btn;
        for (int i = 0; i < ap_number; i++) {
            ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
            ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
            btn = lv_list_add_btn(wifi_list, LV_SYMBOL_WIFI, (const char *)ap_info[i].ssid);
            lv_obj_add_event_cb(btn, list_btn_cb, LV_EVENT_CLICKED, NULL);
        }
    }
    lvgl_port_unlock();

    s_wifi_scan_task_handle = NULL;
    vTaskDelete(NULL);
}

// WiFi 页面手势：右滑关闭 WiFi 界面，回到主界面
static void wifi_page_gesture_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_GESTURE) {
        return;
    }

    lv_indev_t *indev = lv_indev_get_act();
    if (indev == NULL) {
        return;
    }
    lv_dir_t dir = lv_indev_get_gesture_dir(indev);
    lv_indev_wait_release(indev);

    if (dir == LV_DIR_RIGHT) {
        app_wifi_close();
    }
}

static void wifi_attach_page_gesture(lv_obj_t *page)
{
    if (page == NULL) {
        return;
    }
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(page, LV_DIR_NONE);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(page, wifi_page_gesture_cb, LV_EVENT_GESTURE, NULL);
}

// lcd处理任务
static void wifi_connect(void *arg)
{
    wifi_account_t wifi_account;

    while (true)
    {
        // 如果收到wifi账号队列消息
        if(xQueueReceive(xQueueWifiAccount, &wifi_account, portMAX_DELAY))
        {
            wifi_config_t wifi_config = {
                .sta = {
                    .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                    .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
                    .sae_h2e_identifier = "",
                    },
            };
            strcpy((char *)wifi_config.sta.ssid, wifi_account.wifi_ssid);
            strcpy((char *)wifi_config.sta.password, wifi_account.wifi_password);
            ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
            ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 wifi_config.sta.ssid, wifi_config.sta.password);
            xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT);
            esp_wifi_connect();
            /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
            * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
            EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

            /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
            * happened. */
            if (bits & WIFI_CONNECTED_BIT) {
                ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                        wifi_config.sta.ssid, wifi_config.sta.password);
                lvgl_port_lock(0);
                if(label_wifi_connect && lv_obj_is_valid(label_wifi_connect)) {
                    lv_label_set_text(label_wifi_connect, "WLAN连接成功");
                }
                lvgl_port_unlock();
            } else if (bits & WIFI_FAIL_BIT) {
                ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                        wifi_config.sta.ssid, wifi_config.sta.password);
                lvgl_port_lock(0);
                if(label_wifi_connect && lv_obj_is_valid(label_wifi_connect)) {
                    lv_label_set_text(label_wifi_connect, "WLAN连接失败");
                }
                lvgl_port_unlock();
            } else {
                ESP_LOGE(TAG, "UNEXPECTED EVENT");
                lvgl_port_lock(0);
                if(label_wifi_connect && lv_obj_is_valid(label_wifi_connect)) {
                    lv_label_set_text(label_wifi_connect, "WLAN连接异常");
                }
                lvgl_port_unlock();
            }
        }
    }
}

// wifi连接
void app_wifi_connect(void)
{
    if (app_wifi_is_open()) {
        return;
    }

    lvgl_port_lock(0);
    // 创建WLAN扫描页面
    static lv_style_t style;
    style_init_or_reset(&style);
    lv_style_set_bg_opa( &style, LV_OPA_COVER ); // 背景透明度
    lv_style_set_border_width(&style, 0); // 边框宽度
    lv_style_set_pad_all(&style, 0);  // 内间距
    lv_style_set_radius(&style, 0);   // 圆角半径
    lv_style_set_width(&style, 320);  // 宽
    lv_style_set_height(&style, 240); // 高
    wifi_scan_page = lv_obj_create(lv_scr_act());
    lv_obj_add_style(wifi_scan_page, &style, 0);
    // 使手势在 WiFi 页面自身处理（右滑关闭），不冒泡到 screen_1
    wifi_attach_page_gesture(wifi_scan_page);
    // 在WLAN扫描页面显示提示（保存到全局指针，便于异步任务中删除）
    label_wifi_scan = lv_label_create(wifi_scan_page);
    lv_label_set_text(label_wifi_scan, "WLAN扫描中...");
    lv_obj_set_style_text_font(label_wifi_scan, &font_alipuhui20, 0);
    lv_obj_align(label_wifi_scan, LV_ALIGN_CENTER, 0, -50);
    lvgl_port_unlock();

    // 异步扫描 WLAN 信息：在后台任务中执行，避免阻塞 UI 线程
    if (s_wifi_scan_task_handle == NULL) {
        xTaskCreatePinnedToCore(
            wifi_scan_task,
            "wifi_scan_task",
            4 * 1024,
            NULL,
            5,
            &s_wifi_scan_task_handle,
            1);
    }
    
    // 创建 wifi 账号队列 + wifi 连接任务（只创建一次，避免多次调用导致重启/崩溃）
    if (xQueueWifiAccount == NULL) {
        xQueueWifiAccount = xQueueCreate(2, sizeof(wifi_account_t));
    }
    if (!s_wifi_connect_task_started) {
        xTaskCreatePinnedToCore(wifi_connect, "wifi_connect", 4 * 1024, NULL, 5, NULL, 1);
        s_wifi_connect_task_started = true;
    }
}

bool app_wifi_is_open(void)
{
    return (wifi_scan_page != NULL) || (wifi_connect_page != NULL) || (wifi_password_page != NULL);
}

// 关闭 WiFi 页面，返回主界面
void app_wifi_close(void)
{
    lvgl_port_lock(0);
    
    // 删除所有 WiFi 相关页面（按创建顺序的逆序删除）
    if (wifi_password_page != NULL) {
        lv_obj_del(wifi_password_page);
        wifi_password_page = NULL;
    }
    if (wifi_connect_page != NULL) {
        lv_obj_del(wifi_connect_page);
        wifi_connect_page = NULL;
    }
    if (wifi_scan_page != NULL) {
        lv_obj_del(wifi_scan_page);
        wifi_scan_page = NULL;
    }
    wifi_list = NULL;
    label_wifi_connect = NULL;
    ta_pass_text = NULL;
    roller_num = NULL;
    roller_letter_low = NULL;
    roller_letter_up = NULL;
    label_wifi_name = NULL;
    
    lvgl_port_unlock();
}

