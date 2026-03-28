/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"

#if LV_USE_FS_RAWFS

const rawfs_size_t rawfs_file_count = 18;
rawfs_file_t rawfs_files[18] = {
	0x0, 0, 230404, "/heiji7.bin",
	0x38404, 0, 230404, "/heiji5.bin",
	0x70808, 0, 9079, "/heiji3.bin",
	0x72b7f, 0, 9079, "/heiji3.bin",
	0x74ef6, 0, 9079, "/heiji1.bin",
	0x7726d, 0, 9079, "/heiji1.bin",
	0x795e4, 0, 9079, "/heiji6.bin",
	0x7b95b, 0, 9079, "/heiji6.bin",
	0x7dcd2, 0, 9079, "/heiji1.bin",
	0x80049, 0, 9079, "/heiji1.bin",
	0x823c0, 0, 9079, "/heiji4.bin",
	0x84737, 0, 9079, "/heiji4.bin",
	0x86aae, 0, 9079, "/heiji1.bin",
	0x88e25, 0, 9079, "/heiji1.bin",
	0x8b19c, 0, 230404, "/heiji8.bin",
	0xc35a0, 0, 9079, "/heiji1.bin",
	0xc5917, 0, 230404, "/heiji9.bin",
	0xfdd1b, 0, 230404, "/heiji10.bin",

};

#endif  /*LV_USE_FS_RAWFS*/ 