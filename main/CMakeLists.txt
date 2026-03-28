# 1. 递归收集所有需要编译的 .c 文件
file(GLOB_RECURSE srcs
    *.c
    esp32_s3_szp.c
    main.c
    custom/*.c
    font_alipuhui20.c
    app_ui.c
    app_ui_music.c
    asr/*.c
    img_bilibili120.c
    generated/*.c
    generated/guider_customer_fonts/*.c
    generated/guider_fonts/*.c
    generated/images/*.c
)

# 2. 定义头文件搜索目录
set(include_dirs
    custom
    weather
    asr
    generated
    generated/guider_customer_fonts
    generated/guider_fonts
    generated/images
)

# 3. 注册 ESP-IDF 组件
idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS ${include_dirs}
    INCLUDE_DIRS "."
    REQUIRES fatfs vfs sdmmc spiffs
    REQUIRES lvgl esp_lvgl_port driver
    REQUIRES esp_wifi nvs_flash esp_http_client json
)

# 4. 强制定义宏，让代码走 #include "lvgl.h" 分支
target_compile_definitions(${COMPONENT_LIB} PRIVATE
    LV_LVGL_H_INCLUDE_SIMPLE
)

# 把 spiffs_image/ 目录打包进名为 "spiffs" 的分区
spiffs_create_partition_image(spiffs "${CMAKE_CURRENT_SOURCE_DIR}/spiffs_image" FLASH_IN_PROJECT)