/**
 * Project-specific LVGL configuration overrides.
 * This file is included by lv_conf_internal.h when LV_CONF_INCLUDE_SIMPLE is defined.
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Enable the digital clock widget (dclock) */
#ifndef LV_USE_DCLOCK
#define LV_USE_DCLOCK 1
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_CONF_H */

