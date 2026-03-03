# ESP32-S3 LVGL Demo – WiFi UI 集成说明

## 1. 概述

本项目在原 NXP GuiGuider 生成的 LVGL Demo 基础上，集成了一个 **WiFi 扫描 + 连接 UI**，并且：

- 使用 **右滑/左滑手势** 在主菜单 (`screen_1`) 呼出/关闭 WiFi 界面；
- 使用 **右上角 WiFi 图标** 点击也能进入 WiFi 界面；
- WiFi 协议栈只在 **开机初始化一次**，后续进入 WiFi 界面无需重复初始化；
- WiFi UI 打开/关闭支持多次反复进入，不再出现重启/崩溃。

## 2. 目录与文件

- `main.c`
  - 应用入口 `app_main()`，负责：
    - NVS 初始化
    - **WiFi 协议栈预初始化：`app_wifi_init()`**
    - SD 卡挂载
    - SPIFFS 挂载（存放 LVGL 图片资源）
    - LVGL + GuiGuider UI 初始化

- `app_ui.c` / `app_ui.h`
  - WiFi 扫描与连接逻辑
  - WiFi UI 页面（扫描页、密码输入页、连接状态页）
  - WiFi 任务与队列管理

- `custom/custom.c` / `custom/custom.h`
  - 在 `screen_1` 上创建右上角 WiFi 图标
  - 处理 `screen_1` 的左右滑动手势（右滑打开 WiFi、左滑关闭 WiFi）

- `generated/events_init.c`
  - GuiGuider 生成的事件初始化代码
  - 在 `events_init_screen_1()` 中挂接 WiFi 图标与手势回调

## 3. 启动流程

1. `app_main()`：
   - 初始化 NVS
   - 调用 `app_wifi_init()` 初始化 WiFi 协议栈（只执行一次，不扫描）
   - 初始化 I2C / IO 扩展、挂载 SD 卡和 SPIFFS
   - 启动 LVGL 与 GuiGuider UI (`setup_ui`, `custom_init`)

2. 用户进入主菜单 `screen_1` 后：
   - `events_init_screen_1()` 被调用：
     - 绑定三个图标按钮的点击事件（跳转到其他 screen）
     - 调用 `wifi_icon_create_on_screen_1()` 创建右上角 WiFi 图标
     - 为 `screen_1` 开启左右滑动手势，并关闭默认滚动

## 4. WiFi 模块设计

### 4.1 公共接口

在 `app_ui.h` 中导出：

- `void app_wifi_init(void);`
  - 在 `app_main()` 中开机调用一次，用于初始化 WiFi 协议栈。
- `void app_wifi_connect(void);`
  - 打开 WiFi 扫描/连接 UI。
- `void app_wifi_close(void);`
  - 关闭所有 WiFi 相关 UI 页面。
- `bool app_wifi_is_open(void);`
  - 判断当前是否已有 WiFi 页面处于打开状态（用于防重入）。

### 4.2 WiFi 协议栈初始化（一次性）

在 `app_ui.c` 中：

- `wifi_stack_init_once()`：
  - `esp_netif_init()`
  - `esp_event_loop_create_default()`
  - `esp_netif_create_default_wifi_sta()`
  - `esp_wifi_init()`
  - 注册 WiFi/IP 事件回调
  - 设置 `WIFI_MODE_STA` 并调用 `esp_wifi_start()`
  - 用 `s_wifi_stack_inited` 标志保证只会执行一次

- `app_wifi_init()`：
  - 对外暴露，内部直接调用 `wifi_stack_init_once()`
  - 在 `app_main()` 内部调用一次即可。

### 4.3 WiFi 扫描与连接

- `wifi_scan(ap_info, &ap_number)`：
  - 调用 `wifi_stack_init_once()` 确保 WiFi 栈已就绪
  - 调用 `esp_wifi_scan_start(NULL, true)` 同步扫描
  - 使用 `esp_wifi_scan_get_ap_num` / `esp_wifi_scan_get_ap_records` 获取扫描结果

- `wifi_connect(void *arg)` 任务：
  - 通过 `xQueueReceive(xQueueWifiAccount, &wifi_account, portMAX_DELAY)` 阻塞等待账号
  - 收到账号后：
    - 设置 `wifi_config`
    - 清空 WiFi 事件组标志位，调用 `esp_wifi_connect()`
    - 使用 `xEventGroupWaitBits` 等待 `WIFI_CONNECTED_BIT` 或 `WIFI_FAIL_BIT`
    - 在 LVGL 线程安全保护下更新 `label_wifi_connect` 提示文本
      - 更新前用 `label_wifi_connect && lv_obj_is_valid(label_wifi_connect)` 判断对象仍然有效，防止 UI 被关闭后任务再访问无效指针。

- 队列与任务“单例”策略：
  - `xQueueWifiAccount` 只在第一次 `app_wifi_connect()` 时创建
  - `wifi_connect` 任务只创建一次，使用 `s_wifi_connect_task_started` 标志防重复创建。

### 4.4 WiFi UI 页面生命周期

- 打开：
  - `app_wifi_connect()`：
    - 如果 `app_wifi_is_open() == true` 直接返回（防重入）
    - 创建 `wifi_scan_page` 并显示 “WLAN扫描中…”
    - 同步调用 `wifi_scan()` 获取 AP 列表
    - 删除 “扫描中” 文本，创建 `wifi_list` 与每个 AP 的按钮
    - 创建 WiFi 队列/任务（首次）

- 进入密码输入页：
  - 点击 AP 按钮时 `list_btn_cb()` 创建 `wifi_password_page`，包括：
    - WiFi 名称 label
    - 密码输入框 + OK/删除按钮
    - 数字、小写、大写三个 roller 及其确认按钮

- 连接状态页：
  - 密码 OK 后，`btn_connect_cb()` 将账号放入队列，并调用 `lv_wifi_connect()`：
    - 删除密码页
    - 创建 `wifi_connect_page` 与“WLAN连接中…”提示 label
  - 后续由 `wifi_connect` 任务更新该 label 为“成功/失败/异常”

- 关闭：
  - `app_wifi_close()`：
    - 安全删除 `wifi_password_page`、`wifi_connect_page`、`wifi_scan_page`
    - 清空所有相关对象指针
    - WiFi 协议栈与后台任务保持运行，可重复进入 WiFi UI

## 5. 手势与 WiFi 图标

- **WiFi 图标**
  - 在 `custom.c` 中的 `wifi_icon_create_on_screen_1()` 里，在 `screen_1` 右上角创建一个透明按钮，内部放置 `LV_SYMBOL_WIFI`。
  - 点击图标触发 `app_wifi_connect()`。

- **左右滑动手势**
  - `screen_1_gesture_cb()` 处理 `LV_EVENT_GESTURE`：
    - 右滑 (`LV_DIR_RIGHT`) → `app_wifi_connect()`
    - 左滑 (`LV_DIR_LEFT`) → `app_wifi_close()`
    - 使用 `lv_indev_wait_release(lv_indev_get_act())` 防止一次滑动被多次触发。

  - 在 `events_init_screen_1()` 中：
    - 关闭 `screen_1` 的默认滚动（`LV_OBJ_FLAG_SCROLLABLE` + `lv_obj_set_scroll_dir(..., LV_DIR_NONE)`）
    - 清除 `LV_OBJ_FLAG_GESTURE_BUBBLE`，使手势事件停留在 `screen_1` 对象上
    - 注册 `LV_EVENT_GESTURE` 对应的回调。

## 6. 使用说明

- **进入 WiFi 页面**
  - 在主菜单 `screen_1`：
    - 右滑手势，或
    - 点击右上角 WiFi 图标

- **退出 WiFi 页面**
  - 左滑手势，或
  - 使用页面内的返回按钮（从密码输入页返回）

- **多次进入**
  - WiFi 协议栈、任务、队列只在第一次初始化，之后多次进入/退出都不会再导致重启或重复初始化。