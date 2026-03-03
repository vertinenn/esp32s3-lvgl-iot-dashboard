#include "sd_lvgl_fs.h"

#include "lvgl.h"
#include <stdio.h>
#include "esp_log.h"

// Map LVGL drive letter 'S' to the ESP-IDF VFS path "/sdcard".
// LVGL strips the "S:" prefix before calling our callbacks, so `path`
// here is e.g. "gif/Video1.gif".

static const char *TAG = "lv_fs_sdcard";

static void *sd_open(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode)
{
    LV_UNUSED(drv);

    char full_path[256];

    /* LVGL 传进来的 path 可能是 "mjpg/..." 也可能是 "/mjpg/..."，
     * 这里统一成 "/sdcard/mjpg/..."，避免出现 "/sdcard//mjpg/..." */
    const char *rel = path ? path : "";
    if(rel[0] == '/') {
        rel++;  /* 去掉前导 '/' */
    }

    int n = snprintf(full_path, sizeof(full_path), "/sdcard/%s", rel);
    if (n <= 0 || n >= (int)sizeof(full_path)) {
        ESP_LOGE(TAG, "sd_open: path too long or snprintf error, path=\"%s\"", path ? path : "(null)");
        return NULL;
    }

    const char *fmode = NULL;
    if ((mode & LV_FS_MODE_WR) && (mode & LV_FS_MODE_RD)) {
        fmode = "r+b";
    } else if (mode & LV_FS_MODE_WR) {
        fmode = "wb";
    } else {
        fmode = "rb";
    }

    FILE *fp = fopen(full_path, fmode);
    if (fp == NULL) {
        ESP_LOGE(TAG, "sd_open: fopen failed, full_path=\"%s\", mode=\"%s\"", full_path, fmode);
    } else {
        ESP_LOGI(TAG, "sd_open: fopen OK, full_path=\"%s\", mode=\"%s\"", full_path, fmode);
    }
    return fp;
}

static lv_fs_res_t sd_close(lv_fs_drv_t *drv, void *file_p)
{
    LV_UNUSED(drv);
    if (file_p == NULL) {
        return LV_FS_RES_INV_PARAM;
    }
    fclose((FILE *)file_p);
    ESP_LOGI(TAG, "sd_close: file closed");
    return LV_FS_RES_OK;
}

static lv_fs_res_t sd_read(lv_fs_drv_t *drv, void *file_p, void *buf, uint32_t btr, uint32_t *br)
{
    LV_UNUSED(drv);
    if (file_p == NULL || buf == NULL) {
        return LV_FS_RES_INV_PARAM;
    }

    size_t n = fread(buf, 1, btr, (FILE *)file_p);
    if (br) {
        *br = (uint32_t)n;
    }

    if (ferror((FILE *)file_p)) {
        ESP_LOGE(TAG, "sd_read: ferror set");
        return LV_FS_RES_UNKNOWN;
    }

    return LV_FS_RES_OK;
}

static lv_fs_res_t sd_seek(lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence)
{
    LV_UNUSED(drv);
    if (file_p == NULL) {
        return LV_FS_RES_INV_PARAM;
    }

    int origin = SEEK_SET;
    switch (whence) {
    case LV_FS_SEEK_SET:
        origin = SEEK_SET;
        break;
    case LV_FS_SEEK_CUR:
        origin = SEEK_CUR;
        break;
    case LV_FS_SEEK_END:
        origin = SEEK_END;
        break;
    default:
        origin = SEEK_SET;
        break;
    }

    if (fseek((FILE *)file_p, (long)pos, origin) != 0) {
        return LV_FS_RES_UNKNOWN;
    }

    return LV_FS_RES_OK;
}

static lv_fs_res_t sd_tell(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p)
{
    LV_UNUSED(drv);
    if (file_p == NULL || pos_p == NULL) {
        return LV_FS_RES_INV_PARAM;
    }

    long p = ftell((FILE *)file_p);
    if (p < 0) {
        return LV_FS_RES_UNKNOWN;
    }

    *pos_p = (uint32_t)p;
    return LV_FS_RES_OK;
}

void lv_fs_sdcard_init(void)
{
    static lv_fs_drv_t drv;
    lv_fs_drv_init(&drv);

    drv.letter = 'S';     // Use S: as drive letter for SD card
    drv.cache_size = 0;   // No internal caching

    drv.open_cb = sd_open;
    drv.close_cb = sd_close;
    drv.read_cb = sd_read;
    drv.seek_cb = sd_seek;
    drv.tell_cb = sd_tell;

    // We don't implement write or directory operations for now.
    drv.write_cb = NULL;
    drv.dir_close_cb = NULL;
    drv.dir_open_cb = NULL;
    drv.dir_read_cb = NULL;

    lv_fs_drv_register(&drv);
}

