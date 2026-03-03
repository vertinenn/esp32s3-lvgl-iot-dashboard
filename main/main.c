#include <stdio.h>
#include "esp32_s3_szp.h"
#include "logo_en_240x240_lcd.h"
#include "yingwu.h"
// #include "demos/lv_demos.h"
#include "gui_guider.h"
#include "lvgl.h"
#include "custom.h"
#include "sd_lvgl_fs.h"
#include "app_ui_music.h"
//sd卡相关头文件
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "app_ui.h"
#include "nvs_flash.h"

#include "esp_spiffs.h"

#define BSP_SD_CLK          (47)
#define BSP_SD_CMD          (48)
#define BSP_SD_D0           (21)

static const char *TAG = "main";

#define MOUNT_POINT              "/sdcard"//定义SD卡挂载点
#define EXAMPLE_MAX_CHAR_SIZE    64

lv_ui guider_ui;

esp_err_t sd_card_mount(void)// 初始化并挂载SD卡
{
    esp_err_t ret;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,   // 如果挂载不成功是否需要格式化SD卡
        .max_files = 5, // 允许打开的最大文件数
        .allocation_unit_size = 16 * 1024  // 分配单元大小
    };
    
    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");
    ESP_LOGI(TAG, "Using SDMMC peripheral");

    sdmmc_host_t host = SDMMC_HOST_DEFAULT(); // SDMMC主机接口配置
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT(); // SDMMC插槽配置
    slot_config.width = 1;  // 设置为1线SD模式
    slot_config.clk = BSP_SD_CLK; 
    slot_config.cmd = BSP_SD_CMD;
    slot_config.d0 = BSP_SD_D0;
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP; // 打开内部上拉电阻

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card); // 挂载SD卡

    if (ret != ESP_OK) {  // 如果没有挂载成功
        if (ret == ESP_FAIL) { // 如果挂载失败
            ESP_LOGE(TAG, "Failed to mount filesystem. ");
        } else { // 如果是其它错误 打印错误名称
            ESP_LOGE(TAG, "Failed to initialize the card (%s). ", esp_err_to_name(ret));
        }
        return ret;
    }
    ESP_LOGI(TAG, "Filesystem mounted"); // 提示挂载成功
    
    return ESP_OK;
}

static esp_err_t spiffs_mount(void)
{
    ESP_LOGI(TAG, "Mounting SPIFFS filesystem");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",//挂载点
        .partition_label = "spiffs",//分区标签
        .max_files = 8,//最大文件数
        .format_if_mount_failed = true,//如果挂载失败是否格式化
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount SPIFFS (%s)", esp_err_to_name(ret));
        return ret;
    }

    size_t total = 0;
    size_t used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "SPIFFS mounted: total=%u, used=%u", (unsigned)total, (unsigned)used);
    } else {
        ESP_LOGW(TAG, "Failed to get SPIFFS info (%s)", esp_err_to_name(ret));
    }

    return ESP_OK;
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    // 提前初始化 WiFi 协议栈（只做一次，不扫描），减少第一次进入 WiFi 页面卡顿
    app_wifi_init();

    bsp_i2c_init();  // I2C初始化
    pca9557_init();  // IO扩展芯片初始化
    bsp_codec_init(); // 音频初始化
    ESP_ERROR_CHECK(sd_card_mount());
    ESP_LOGI(TAG, "SD卡挂载成功");

    ESP_ERROR_CHECK(spiffs_mount());
    ESP_LOGI(TAG, "SPIFFS 挂载成功");

#if LV_USE_FS_POSIX
    lv_fs_posix_init();   // 注册 LVGL 的 POSIX 驱动，盘符/路径由 menuconfig 中的 LV_FS_POSIX_* 决定
    ESP_LOGI(TAG, "LVGL 文件系统驱动注册成功 (POSIX)");
#endif

    bsp_lvgl_start(); // 初始化液晶屏lvgl接口
    lv_fs_sdcard_init(); // 注册自定义 SD 卡文件系统驱动，将 S: 映射到 /sdcard
    setup_ui(&guider_ui); // 初始化界面
    custom_init(&guider_ui); // 初始化自定义功能（如 WiFi / 音乐等）
    /* 下面5个demos 只打开1个运行 */
    // lv_demo_benchmark(); 
    // lv_demo_keypad_encoder(); 
    // lv_demo_music(); 
    // lv_demo_stress(); 
    // lv_demo_widgets(); 
}
