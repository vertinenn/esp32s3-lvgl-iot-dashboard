#pragma once

#include <stdbool.h>
#include <string.h>
#include "math.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_types.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_touch_ft5x06.h"
#include "esp_lvgl_port.h"

#include "esp_codec_dev.h"
#include "esp_codec_dev_defaults.h"
#include "driver/spi_master.h"
#include "driver/i2s_std.h"


/******************************************************************************/
/***************************  I2C ↓ *******************************************/
#define BSP_I2C_SDA           (GPIO_NUM_1)   // SDA引脚
#define BSP_I2C_SCL           (GPIO_NUM_2)   // SCL引脚

#define BSP_I2C_NUM           (0)            // I2C外设
#define BSP_I2C_FREQ_HZ       100000         // 100kHz

esp_err_t bsp_i2c_init(void);   // 初始化I2C接口
/***************************  I2C ↑  *******************************************/
/*******************************************************************************/


/*******************************************************************************/
/***************************  姿态传感器 QMI8658 ↓   ****************************/
#define  QMI8658_SENSOR_ADDR       0x6A   // QMI8658 I2C地址

// QMI8658寄存器地址
enum qmi8658_reg
{
    QMI8658_WHO_AM_I,
    QMI8658_REVISION_ID,
    QMI8658_CTRL1,
    QMI8658_CTRL2,
    QMI8658_CTRL3,
    QMI8658_CTRL4,
    QMI8658_CTRL5,
    QMI8658_CTRL6,
    QMI8658_CTRL7,
    QMI8658_CTRL8,
    QMI8658_CTRL9,
    QMI8658_CATL1_L,
    QMI8658_CATL1_H,
    QMI8658_CATL2_L,
    QMI8658_CATL2_H,
    QMI8658_CATL3_L,
    QMI8658_CATL3_H,
    QMI8658_CATL4_L,
    QMI8658_CATL4_H,
    QMI8658_FIFO_WTM_TH,
    QMI8658_FIFO_CTRL,
    QMI8658_FIFO_SMPL_CNT,
    QMI8658_FIFO_STATUS,
    QMI8658_FIFO_DATA,
    QMI8658_STATUSINT = 45,
    QMI8658_STATUS0,
    QMI8658_STATUS1,
    QMI8658_TIMESTAMP_LOW,
    QMI8658_TIMESTAMP_MID,
    QMI8658_TIMESTAMP_HIGH,
    QMI8658_TEMP_L,
    QMI8658_TEMP_H,
    QMI8658_AX_L,
    QMI8658_AX_H,
    QMI8658_AY_L,
    QMI8658_AY_H,
    QMI8658_AZ_L,
    QMI8658_AZ_H,
    QMI8658_GX_L,
    QMI8658_GX_H,
    QMI8658_GY_L,
    QMI8658_GY_H,
    QMI8658_GZ_L,
    QMI8658_GZ_H,
    QMI8658_COD_STATUS = 70,
    QMI8658_dQW_L = 73,
    QMI8658_dQW_H,
    QMI8658_dQX_L,
    QMI8658_dQX_H,
    QMI8658_dQY_L,
    QMI8658_dQY_H,
    QMI8658_dQZ_L,
    QMI8658_dQZ_H,
    QMI8658_dVX_L,
    QMI8658_dVX_H,
    QMI8658_dVY_L,
    QMI8658_dVY_H,
    QMI8658_dVZ_L,
    QMI8658_dVZ_H,
    QMI8658_TAP_STATUS = 89,
    QMI8658_STEP_CNT_LOW,
    QMI8658_STEP_CNT_MIDL,
    QMI8658_STEP_CNT_HIGH,
    QMI8658_RESET = 96
};

// 倾角结构体
typedef struct{
    int16_t acc_x;
	int16_t acc_y;
	int16_t acc_z;
	int16_t gyr_x;
	int16_t gyr_y;
	int16_t gyr_z;
	float AngleX;
	float AngleY;
	float AngleZ;
}t_sQMI8658;

void qmi8658_init(void);  // QMI8658初始化
void qmi8658_fetch_angleFromAcc(t_sQMI8658 *p);  // 获取倾角

/***************************  姿态传感器 QMI8658 ↑  ****************************/
/*******************************************************************************/

/******************************************************************************/
/***************************   I2S  ↓    **************************************/

/* Example configurations */
#define EXAMPLE_RECV_BUF_SIZE   (2400)
#define EXAMPLE_SAMPLE_RATE     (16000)
#define EXAMPLE_MCLK_MULTIPLE   (384) // If not using 24-bit data width, 256 should be enough
#define EXAMPLE_MCLK_FREQ_HZ    (EXAMPLE_SAMPLE_RATE * EXAMPLE_MCLK_MULTIPLE)
#define EXAMPLE_VOICE_VOLUME    (70)

/* I2S port and GPIOs */
#define I2S_NUM         (0)
#define I2S_MCK_IO      (GPIO_NUM_38)
#define I2S_BCK_IO      (GPIO_NUM_14)
#define I2S_WS_IO       (GPIO_NUM_13)
#define I2S_DO_IO       (GPIO_NUM_45)
#define I2S_DI_IO       (-1)


/***********************************************************/
/***************    IO扩展芯片 ↓   *************************/
#define PCA9557_INPUT_PORT              0x00
#define PCA9557_OUTPUT_PORT             0x01
#define PCA9557_POLARITY_INVERSION_PORT 0x02
#define PCA9557_CONFIGURATION_PORT      0x03

#define LCD_CS_GPIO                 BIT(0)    // PCA9557_GPIO_NUM_1
#define PA_EN_GPIO                  BIT(1)    // PCA9557_GPIO_NUM_2
#define DVP_PWDN_GPIO               BIT(2)    // PCA9557_GPIO_NUM_3

#define PCA9557_SENSOR_ADDR             0x19        /*!< Slave address of the MPU9250 sensor */

#define SET_BITS(_m, _s, _v)  ((_v) ? (_m)|((_s)) : (_m)&~((_s)))

void pca9557_init(void);
void lcd_cs(uint8_t level);
void pa_en(uint8_t level);
void dvp_pwdn(uint8_t level);
/***************    IO扩展芯片 ↑   *************************/
/***********************************************************/


/***********************************************************/
/****************    LCD显示屏 ↓   *************************/
#define BSP_LCD_PIXEL_CLOCK_HZ     (80 * 1000 * 1000)
#define BSP_LCD_SPI_NUM            (SPI3_HOST)
#define LCD_CMD_BITS               (8)
#define LCD_PARAM_BITS             (8)
#define BSP_LCD_BITS_PER_PIXEL     (16)
#define LCD_LEDC_CH          LEDC_CHANNEL_0

#define BSP_LCD_H_RES              (320)
#define BSP_LCD_V_RES              (240)

#define BSP_LCD_SPI_MOSI      (GPIO_NUM_40)
#define BSP_LCD_SPI_CLK       (GPIO_NUM_41)
#define BSP_LCD_SPI_CS        (GPIO_NUM_NC)
#define BSP_LCD_DC            (GPIO_NUM_39)
#define BSP_LCD_RST           (GPIO_NUM_NC)
#define BSP_LCD_BACKLIGHT     (GPIO_NUM_42)  

#define BSP_LCD_DRAW_BUF_HEIGHT    (40)//LVGL缓存大小

esp_err_t bsp_display_brightness_init(void);
esp_err_t bsp_display_brightness_set(int brightness_percent);
esp_err_t bsp_display_backlight_off(void);
esp_err_t bsp_display_backlight_on(void);
esp_err_t bsp_lcd_init(void);
void lcd_set_color(uint16_t color);
void lcd_draw_pictrue(int x_start, int y_start, int x_end, int y_end, const unsigned char *gImage);
void bsp_lvgl_start(void);
/***************    LCD显示屏 ↑   *************************/
/***********************************************************/


/***********************************************************/
/****************    摄像头 ↓   ****************************/
#define CAMERA_EN    0
#if CAMERA_EN
#include "esp_camera.h"

#define CAMERA_PIN_PWDN -1
#define CAMERA_PIN_RESET -1
#define CAMERA_PIN_XCLK 5
#define CAMERA_PIN_SIOD 1
#define CAMERA_PIN_SIOC 2

#define CAMERA_PIN_D7 9
#define CAMERA_PIN_D6 4
#define CAMERA_PIN_D5 6
#define CAMERA_PIN_D4 15
#define CAMERA_PIN_D3 17
#define CAMERA_PIN_D2 8
#define CAMERA_PIN_D1 18
#define CAMERA_PIN_D0 16
#define CAMERA_PIN_VSYNC 3
#define CAMERA_PIN_HREF 46
#define CAMERA_PIN_PCLK 7


#define XCLK_FREQ_HZ 24000000

void bsp_camera_init(void);
void app_camera_lcd(void);

#endif
/********************    摄像头 ↑   *************************/
/***********************************************************/
/*********************    音频 ↓   *************************/
#define VOLUME_DEFAULT    60

/*
 * ESP-SR / 模型侧：送入 AFE 的仍是「每路 16 kHz、16 bit」量级的 PCM（见 `bsp_get_feed_data` 解出的 int16）。
 *
 * ES7210 在实战派一类板子上常开 **TDM/多槽**：同样 I2S 帧里要塞 **多于 2 路** ADC 数据。若仿照普通
 * I2S **双声道 16 kHz、16 bit** 去理解总线，却未按多槽加倍位时钟/位宽，**每路麦克风的等效采样率会被摊薄**，
 * 官方说明里会出现「看起来还是 16 kHz、16 bit，实际只有约 **8 kHz** 有效」的现象。
 *
 * 官方 speech 例程因此用 **帧采样率仍填 16000**，但 I2S 数据 **CODEC_DEFAULT_BIT_WIDTH=32**、
 * **CODEC_DEFAULT_CHANNEL=2**（双槽各载多路 16 bit），与 `12-speech_recognition` 一致；读物理解时不要把它当成
 * 「要把 AFE 的 sample_rate 改成 32000」——若文档写 32 kHz 多指 **位宽/时钟预算** 或别种 TDM 配法，改 32 kHz
 * 给 AFE 须另加重采样，否则与 16 kHz 训练模型不一致。
 */
#define CODEC_DEFAULT_SAMPLE_RATE          (16000)
#define CODEC_DEFAULT_BIT_WIDTH            (32)
/* ES7210 输入增益(dB)，过低则唤醒词难触发；过高易削顶，可 30~42 间微调 */
#define CODEC_DEFAULT_ADC_VOLUME           (38.0)
#define CODEC_DEFAULT_CHANNEL              (2)

/**
 * ES7210 麦克风选择：与官方 speech_recognition（ESP-S3-Korvo / 实战派等）一致为四路 + 片内 TDM，
 * MIC3 常接 ES8311 输出作 AEC 参考。若你的板子只有双麦，可改为仅 (ES7120_SEL_MIC1 | ES7120_SEL_MIC2)
 * 并在 app_sr 里把 AFE 配成 2 路、关 AEC。
 */
#ifndef BSP_ES7210_MIC_SELECTED
#define BSP_ES7210_MIC_SELECTED \
    (ES7120_SEL_MIC1 | ES7120_SEL_MIC2 | ES7120_SEL_MIC3 | ES7120_SEL_MIC4)
#endif

/** ES7210 `esp_codec_dev_read` 交错 int16 路数（与官方 12-speech_recognition 一致） */
#define ADC_I2S_CHANNEL                    (4)

/**
 * 送进 AFE `feed()` 的 `pcm_config.total_ch_num`，须与 `bsp_get_feed_data` 压实路数一致：
 * - 3（默认）：2×mic + 1×ref，与官方 12-speech_recognition / ESP-S3-Korvo / 多数 ES7210+ES8311 实战派一致，`DET_MODE_2CH_90`
 * - 2：仅当你确认硬件是 1×拾音 + 1×回采、且日志里 |mic| 与 |ref| 量级合理时用；否则易出现 |ref|>>|mic|、AEC 把语音抵消、MultiNet vol 很低
 */
#ifndef BSP_AFE_PCM_TOTAL_CH
#define BSP_AFE_PCM_TOTAL_CH               (3)
#endif

/** 仅 BSP_AFE_PCM_TOTAL_CH==2 时使用：4×int16 里 mic、ref 槽位；若安静时 |ref| 远大于 |mic| 可尝试 mic=0 ref=1 对调 */
#ifndef BSP_AFE_RAW_MIC_IDX
#define BSP_AFE_RAW_MIC_IDX                (1)
#endif
#ifndef BSP_AFE_RAW_REF_IDX
#define BSP_AFE_RAW_REF_IDX                (0)
#endif

#if (BSP_AFE_PCM_TOTAL_CH != 2) && (BSP_AFE_PCM_TOTAL_CH != 3)
#error "BSP_AFE_PCM_TOTAL_CH must be 2 or 3"
#endif

#define BSP_I2S_NUM                  I2S_NUM_1

#define GPIO_I2S_LRCK       (GPIO_NUM_13)
#define GPIO_I2S_MCLK       (GPIO_NUM_38)
#define GPIO_I2S_SCLK       (GPIO_NUM_14)
#define GPIO_I2S_SDIN       (GPIO_NUM_12)
#define GPIO_I2S_DOUT       (GPIO_NUM_45)
#define GPIO_PWR_CTRL       (GPIO_NUM_NC)

esp_err_t bsp_codec_init(void);
esp_err_t bsp_i2s_write(void *audio_buffer, size_t len, size_t *bytes_written, uint32_t timeout_ms);
esp_err_t bsp_codec_set_fs(uint32_t rate, uint32_t bits_cfg, i2s_slot_mode_t ch);

/** 返回 ADC_I2S_CHANNEL（4） */
int bsp_get_feed_channel(void);
/**
 * 读入 4×int16；若非 raw，原位压成 BSP_AFE_PCM_TOTAL_CH 路：2 路为 mic、ref；3 路同官方 demo（mic、mic、ref）。
 * buffer_len = get_feed_chunksize * sizeof(int16) * ADC_I2S_CHANNEL。
 */
esp_err_t bsp_get_feed_data(bool is_get_raw_channel, int16_t *buffer, int buffer_len);
esp_err_t bsp_codec_mute_set(bool enable);
esp_err_t bsp_codec_volume_set(int volume, int *volume_set);

/*********************    音频 ↑   *************************/
/***********************************************************/