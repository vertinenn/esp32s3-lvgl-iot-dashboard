// Simple LVGL file system driver that maps drive letter 'S' to ESP-IDF's /sdcard mount point.
// Call lv_fs_sdcard_init() after LVGL is initialized (e.g. after bsp_lvgl_start()).

#ifndef SD_LVGL_FS_H
#define SD_LVGL_FS_H

#ifdef __cplusplus
extern "C" {
#endif

void lv_fs_sdcard_init(void);

#ifdef __cplusplus
}
#endif

#endif // SD_LVGL_FS_H

