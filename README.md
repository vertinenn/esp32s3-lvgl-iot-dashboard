ESP32-S3 LVGL IoT 仪表板
===========================

> 基于ESP32-S3的智能HMI面板，具有LVGL UI、Wi-Fi网络、天气显示、SD卡媒体和自定义启动动画。

## 概述

这个项目在**ESP32-S3**开发板上实现了一个小型**IoT风格的仪表板**。  
它使用**LVGL**作为GUI框架来构建多个屏幕（启动屏幕、密码/主屏幕、音乐UI等），集成：

- Wi-Fi网络
- 天气信息显示
- 文件系统（通过FATFS的SD卡、SPIFFS）
- 简单多媒体（MP3音频播放、图像/动画显示）

重点在于**嵌入式工程和系统集成**：使所有外围设备和软件组件在资源受限的MCU上平稳协同工作。

## 视频链接：【立创黄山派做个简单的gui交互系统-哔哩哔哩】 https://b23.tv/Rpwc6wR
   
## 开发博客：https://blog.csdn.net/2501_92263832?type=blog

## 功能特性

- **基于LVGL的GUI**
  - 多个屏幕由NXP Gui Guider生成，并进行手动定制。
  - 使用`lv_animimg`的自定义**启动屏幕**（启动时播放一次全屏动画，然后自动切换到主UI）。
  - 触摸输入处理和基本的UI导航。

- **Wi-Fi & 网络**
  - 在`app_main`中尽早初始化Wi-Fi协议栈，以减少首次进入延迟。
  - 以站点模式连接到接入点（在自定义模块中实现，例如`custom_init` / `app_wifi_init`）。
  - 设计用于通过HTTP/REST API获取在线数据（例如天气）。

- **天气显示**
  - UI元素用于显示**当前城市、温度和天气状态**。
  - 天气数据通过网络请求获取并显示在LVGL屏幕上。

- **文件系统**
  - **SD卡**使用`esp_vfs_fat_sdmmc_mount`挂载在`/sdcard`。
  - **SPIFFS**挂载在`/spiffs`用于内部资源。
  - 自定义LVGL文件系统驱动`sd_lvgl_fs`：
    - 将LVGL驱动字母**`S:`**映射到VFS路径**`/sdcard`**。
    - LVGL小部件可以直接从SD卡加载图像/资源（例如PNG、GIF等）。
  - 启用FATFS长文件名（LFN）支持，以处理长文件名如`frame_0001.jpg`。

- **多媒体**
  - 使用板载音频编解码器和`bsp_codec_init`从SD卡播放MP3。
  - SD存储的图像和动画集成到LVGL UI中。
  - 实验GIF、多JPEG（MJPEG风格）、带有嵌入帧的`lv_animimg`和性能调优。

- **性能 & 资源优化**
  - 调优LVGL显示缓冲区大小、缓冲模式、DMA使用和PSRAM/SRAM放置。
  - 通过以下方式减少闪存使用：
    - 限制嵌入动画帧的数量。
    - 一旦不需要，禁用未使用的LVGL图像解码器（PNG/JPG）。
  - 解决的问题如：
    - 由于大型`.rodata`（嵌入图像）导致的应用分区溢出。
    - 全屏动画帧率过高导致的屏幕撕裂。

## 硬件要求

- ESP32-S3开发板（例如LCSC“实战派 S3”或类似），具有：
  - SPI或RGB TFT LCD面板（例如320x240）
  - 电容式或电阻式触摸控制器（通过I2C连接）
  - 板载音频编解码器（用于MP3播放）
  - SD卡插槽（本项目中使用SDMMC 1位模式）
- microSD卡（格式化为FAT/FAT32）
- USB线缆用于刷写和串行监视器

## 软件栈

- **ESP-IDF**（推荐v5.x）
- **LVGL**（通过托管组件`lvgl__lvgl`，在`lv_conf.h`和`sdkconfig`中配置）
- **NXP Gui Guider**（用于生成初始LVGL屏幕和布局）
- **CMake**构建系统（ESP-IDF默认）

## 项目结构（简化）

- `main/`
  - `main.c`  
    - 入口点：初始化NVS、Wi-Fi栈（`app_wifi_init`）、I2C、音频编解码器、SD卡（`sd_card_mount`）、SPIFFS（`spiffs_mount`）、LVGL端口（`bsp_lvgl_start`）、LVGL文件系统驱动（`lv_fs_posix_init`、`lv_fs_sdcard_init`）和UI（`setup_ui`、`custom_init`）。
  - `esp32_s3_szp.c`  
    - 板支持包：LCD、触摸、音频编解码器、LVGL端口配置（显示缓冲区、DMA、PSRAM/SRAM）。
  - `sd_lvgl_fs.c/.h`  
    - 自定义LVGL文件系统驱动，将`S:` → `/sdcard`映射，使用ESP-IDF VFS上的标准C `fopen`/`fread`实现。
  - `custom.c/.h`  
    - 自定义应用逻辑：Wi-Fi、音乐播放器UI集成、天气显示等。
  - `app_ui*.c`  
    - 特定屏幕的附加UI逻辑（例如音乐控制页面）。
  - `generated/`  
    - NXP Gui Guider自动生成的LVGL UI代码：
      - `gui_guider.c/.h` – 顶级UI结构和`setup_ui`。
      - `setup_scr_*.c` – 每屏幕设置（小部件、布局、样式）。
      - `events_init.c` – LVGL事件回调。
      - `widgets_init.c/.h` – 小部件和图像/动画资源。

## 构建 & 刷写

1. **安装ESP-IDF**

   按照官方ESP-IDF文档安装工具链并设置环境：

   ```bash
   # 示例（Linux/macOS）
   git clone --recursive https://github.com/espressif/esp-idf.git
   cd esp-idf
   ./install.sh
   . ./export.sh
   ```

2. **配置项目**

   在项目根目录：

   ```bash
   idf.py menuconfig
   ```

   检查或调整：

   - 串行端口、闪存大小和分区表。
   - Wi-Fi SSID/密码（如果存储在`sdkconfig`或自定义配置中）。
   - FATFS LFN选项：
     - `CONFIG_FATFS_LFN_HEAP=y`
     - `CONFIG_FATFS_MAX_LFN=255`
   - LVGL选项：
     - 根据需要启用/禁用图像解码器（GIF/PNG/JPG）。
     - FS选项（例如`LV_USE_FS_POSIX`）如果使用。

3. **构建**

   ```bash
   idf.py build
   ```

4. **刷写 & 监视**

   ```bash
   idf.py -p COMx flash monitor
   # 在Windows上将COMx替换为您的串行端口
   ```

## SD卡 & 资源

- SD卡在ESP-IDF中挂载在`/sdcard`。
- LVGL通过逻辑驱动`S:`访问同一SD卡（例如`S:/gif/xxx.gif`、`S:/img/xxx.bin`）。
- SD卡上的典型目录布局：

  ```text
  /gif        - GIF动画文件（如果使用）
  /music      - MP3或其他音频文件
  /images     - PNG/JPG/BIN图像资源
  ```

确保SD卡格式化为FAT/FAT32并在启动前插入。

## 已知问题 & 注意事项

- 全屏动画以高帧率（例如320x240下>15 fps）会导致由于SPI/LCD带宽限制而出现可见撕裂。  
  本项目选择**较低帧率或降低分辨率**以获得更平滑的感知运动。
- 将太多320x240帧作为C数组嵌入将快速膨胀`.rodata`，可能溢出应用分区。  
  考虑：
  - 减少帧数。
  - 使用SD卡上的外部资源。
  - 使用LVGL的二进制图像格式（`.bin`）以加快加载。

## 许可证

这个项目基于：

- ESP-IDF（Apache 2.0）
- LVGL（MIT许可证）
- NXP Gui Guider生成的代码（请参阅`generated/`文件中的NXP许可证头）

用户应用代码旨在用于学习和个人项目。  
请在使用商业产品前检查每个第三方组件的许可证。

