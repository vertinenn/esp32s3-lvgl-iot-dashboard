/**
 * @file weather_client.h
 * @brief Simple client for Seniverse (心知天气) "now" weather API.
 */

#ifndef WEATHER_CLIENT_H
#define WEATHER_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

typedef struct {
    char city[32];         /**< 城市名称，例如 "北京" */
    char text[16];         /**< 天气描述，例如 "晴" */
    char temperature[8];   /**< 温度字符串，例如 "6" 或 "6℃" */
    char last_update[32];  /**< 心知返回的 last_update，形如 "2026-02-27T15:54:06+08:00" */
} weather_now_t;

/**
 * @brief 同步获取当前天气（"now" 接口）。
 *
 * 需要在 WiFi 已经成功联网之后再调用。
 *
 * @param[out] out  解析后的天气结果结构体
 * @return esp_err_t
 *         - ESP_OK:       请求并解析成功
 *         - 其他错误码:    HTTP、解析或网络错误
 */
esp_err_t weather_get_now(weather_now_t *out);

#ifdef __cplusplus
}
#endif

#endif /* WEATHER_CLIENT_H */

