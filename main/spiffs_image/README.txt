把需要给 LVGL 显示的图片文件放在这个目录里（例如 `heiji1.bin`、`heiji8.bin`）。

构建时会自动把该目录打包成 SPIFFS 镜像并烧录到 flash 的 `spiffs` 分区。

运行时 SPIFFS 会挂载到 `/spiffs`，并且 LVGL 通过 POSIX 文件驱动读取：
- `F:/heiji8.bin`  ->  `/spiffs/heiji8.bin`（取决于你在 menuconfig 里配置的盘符和路径）

