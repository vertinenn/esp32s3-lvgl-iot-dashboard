#include <string.h>

#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"

#include "weather_client.h"

static const char *TAG_WEATHER = "weather_client";

/* 心知天气 now 接口基础 URL（使用 HTTP，便于在嵌入式环境下调试。
 * 如需 HTTPS，可将 http 改为 https，并配置证书或相关 TLS 选项。 */
#define WEATHER_API_BASE_URL "http://api.seniverse.com/v3/weather/now.json"

/* TODO: 如需保护密钥，可改为从配置或 NVS 读取 */
#define WEATHER_API_KEY      "S0g69aztR8rs4AvBD"
#define WEATHER_LOCATION     "beijing"
#define WEATHER_LANG         "zh-Hans"
#define WEATHER_UNIT         "c"

#define WEATHER_HTTP_BUF_SIZE 512

esp_err_t weather_get_now(weather_now_t *out)
{
    if (out == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    char url[256];
    int n = snprintf(url, sizeof(url),
                     WEATHER_API_BASE_URL "?key=%s&location=%s&language=%s&unit=%s",
                     WEATHER_API_KEY, WEATHER_LOCATION, WEATHER_LANG, WEATHER_UNIT);
    if (n <= 0 || n >= (int)sizeof(url)) {
        ESP_LOGE(TAG_WEATHER, "URL buffer too small");
        return ESP_ERR_NO_MEM;
    }

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 5000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG_WEATHER, "Failed to init http client");
        return ESP_FAIL;
    }

    // 1) 建立连接
    esp_err_t err = esp_http_client_open(client, 0 /* write_len=0 for GET */);
    if (err != ESP_OK) {
        ESP_LOGE(TAG_WEATHER, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return err;
    }

    // 2) 读取响应头信息
    int content_length = esp_http_client_fetch_headers(client);
    int status_code = esp_http_client_get_status_code(client);
    ESP_LOGI(TAG_WEATHER, "HTTP status = %d, content_length = %d", status_code, content_length);

    // 3) 读取 body（可以按需多次 read，这里因为数据不大直接一次性读完）
    char buffer[WEATHER_HTTP_BUF_SIZE] = {0};
    int total_read = 0;
    while (1) {
        int read_len = esp_http_client_read(client,
                                            buffer + total_read,
                                            sizeof(buffer) - 1 - total_read);
        if (read_len <= 0) {
            break;  // <=0 表示读完或者出错
        }
        total_read += read_len;
        if (total_read >= (int)sizeof(buffer) - 1) {
            break;  // 防止溢出
        }
    }
    buffer[total_read] = '\0';

    if (total_read <= 0) {
        ESP_LOGE(TAG_WEATHER, "No data in HTTP body");
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    // 4) 关闭连接
    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    ESP_LOGI(TAG_WEATHER, "Raw weather JSON: %s", buffer);

    // 5) 用 cJSON 解析（这里你可以继续沿用现在的解析代码）
    cJSON *root = cJSON_Parse(buffer);
    if (root == NULL) {
        ESP_LOGE(TAG_WEATHER, "Failed to parse JSON");
        return ESP_FAIL;
    }

    esp_err_t ret = ESP_FAIL;
    cJSON *results = cJSON_GetObjectItem(root, "results");
    if (cJSON_IsArray(results) && cJSON_GetArraySize(results) > 0) {
        cJSON *first = cJSON_GetArrayItem(results, 0);
        if (cJSON_IsObject(first)) {
            cJSON *loc = cJSON_GetObjectItem(first, "location");
            cJSON *now = cJSON_GetObjectItem(first, "now");
            if (cJSON_IsObject(loc) && cJSON_IsObject(now)) {
                cJSON *name = cJSON_GetObjectItem(loc, "name");
                cJSON *text = cJSON_GetObjectItem(now, "text");
                cJSON *temp = cJSON_GetObjectItem(now, "temperature");
                cJSON *last = cJSON_GetObjectItem(first, "last_update");
                if (cJSON_IsString(name) && cJSON_IsString(text) && cJSON_IsString(temp)) {
                    memset(out, 0, sizeof(*out));
                    strncpy(out->city, name->valuestring, sizeof(out->city) - 1);
                    strncpy(out->text, text->valuestring, sizeof(out->text) - 1);
                    strncpy(out->temperature, temp->valuestring, sizeof(out->temperature) - 1);
                    if (cJSON_IsString(last)) {
                        strncpy(out->last_update, last->valuestring, sizeof(out->last_update) - 1);
                    }
                    ret = ESP_OK;
                }
            }
        }
    }

    cJSON_Delete(root);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG_WEATHER, "Weather: city=%s, text=%s, temp=%s",
                 out->city, out->text, out->temperature);
    } else {
        ESP_LOGE(TAG_WEATHER, "Failed to extract weather fields from JSON");
    }

    return ret;
}

