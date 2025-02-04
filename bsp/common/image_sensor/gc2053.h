/**
 * @file image_sensor.h
 * @brief
 *
 * Copyright (c) 2022 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */
#ifndef __GC2053_H__
#define __GC2053_H__

#include "image_sensor.h"

static struct image_sensor_command_s gc2053_init_list[] = {
    { 0xfe, 0x80 }, // page select
    { 0xfe, 0x80 },
    { 0xfe, 0x80 },
    { 0xfe, 0x00 },
    { 0xf2, 0x00 }, // [7] OTP_clk_gate [6:4] OTP_mode_temp [1] I2C_open_ena [0]pwd_dn
    { 0xf3, 0x0f }, // [7] OTP_finish [6] OTP_busy [5] Spad_vb_hiz_mode [4] Spad_buf_mode [3] Sdata_pad_io [2:0] Ssync_pad_io
    { 0xf4, 0x36 }, // [7] reduce_power_mode, [6:4]PLL_ldo_set, [3:0]spi_clk_div
    { 0xf5, 0xc0 }, // [7] soc_mclk_enable, [6] pll_ldo_en, [5:4]cp_clk_del, [3:0]cp_clk_div
    { 0xf6, 0x44 }, // [7:3]wpllclk_div, [2:0]refmp_div
    { 0xf7, 0x01 }, // [7]refdiv2d5_en [6]refdiv1d5_en [5:4]scaler_mode(dvpmode) [3]refmp_enb [2]freq div2 [1]div2en [0]pll_en
    { 0xf8, 0x63 }, // [7:0]pllmp_div
    { 0xf9, 0x40 }, // [7:3]rpllclk_div [2:1]pllmp_prediv [0]analog_pwc
    { 0xfc, 0x8e }, // [7] regf clk enable [6] sys_rclk_sel [5] sys_wclk_sel [4:3]spi_sel_mode [2] serail_clk enable [1] re_lock_pll [0] not_use_pll
    /*cisctl&analog*/
    { 0xfe, 0x00 },
    { 0x87, 0x18 }, // Debug_mode disable
    { 0xee, 0x30 },
    { 0xd0, 0xb7 },
    { 0x03, 0x01 },
    { 0x04, 0x19 },
    { 0x05, 0x05 }, //h_blank, Line length = 1320 *2   [3:0] Line length[11:8] X2
    { 0x06, 0x28 }, //                                 [7:0] Line length[7:0]  X2
    { 0x07, 0x00 },
    { 0x08, 0x11 },
    { 0x09, 0x00 }, // row start
    { 0x0a, 0x02 },
    { 0x0b, 0x00 }, // col start
    { 0x0c, 0x02 },
    { 0x0d, 0x04 }, // valid line time:Window width 1088
    // Windows height default is 1936
    { 0x0e, 0x40 },
    { 0x12, 0xe2 },
    { 0x13, 0x16 },
    { 0x19, 0x0a },
    { 0x21, 0x1c },
    { 0x28, 0x0a },
    { 0x29, 0x24 },
    { 0x2b, 0x04 },
    { 0x32, 0xf8 },
    { 0x37, 0x03 },
    { 0x39, 0x15 },
    { 0x43, 0x07 },
    { 0x44, 0x40 },
    { 0x46, 0x0b },
    { 0x4b, 0x20 },
    { 0x4e, 0x08 },
    { 0x55, 0x20 },
    { 0x66, 0x05 },
    { 0x67, 0x05 },
    { 0x77, 0x01 },
    { 0x78, 0x00 },
    { 0x7c, 0x93 },
    { 0x8c, 0x12 },
    { 0x8d, 0x92 },
    { 0x90, 0x01 },
    { 0x9d, 0x10 },
    { 0xce, 0x7c },
    { 0xd2, 0x41 },
    { 0xd3, 0xdc },
    { 0xe6, 0x50 },
    /*gain*/
    { 0xb6, 0xc0 },
    { 0xb0, 0x70 },
    { 0xb1, 0x01 },
    { 0xb2, 0x14 },
    { 0xb3, 0x30 },
    { 0xb4, 0x00 },
    { 0xb8, 0x01 },
    { 0xb9, 0x2c },
    /*blk*/
    { 0x26, 0x30 },
    { 0xfe, 0x01 },
    { 0x40, 0x23 }, // BLK_mode1. [7] not smooth [6:4] blk_smooth_speed [3]blk_dd_map [2]dark_sel_map [1]dark_current_en [0]offset_en
    { 0x55, 0x07 },
    { 0x60, 0x40 }, // DARK OFFSET
    { 0xfe, 0x04 },
    { 0x14, 0x78 }, // Ndark_ratio_G1
    { 0x15, 0x78 }, // Ndark_ratio_R1
    { 0x16, 0x78 }, // Ndark_ratio_B2
    { 0x17, 0x78 }, // Ndark_ratio_G2
    /*window*/
    { 0xfe, 0x01 },
    { 0x92, 0x01 }, // output win start y
    { 0x94, 0x02 }, // output win start x
    { 0x95, 0x04 }, // output win height 1080
    { 0x96, 0x38 },
    { 0x97, 0x07 }, // outout win width  1920
    { 0x98, 0x80 },
    /*ISP*/
    { 0xfe, 0x01 },
    { 0x01, 0x05 },
    { 0x02, 0x89 },
    { 0x04, 0x01 },
    { 0x07, 0xa6 },
    { 0x08, 0xa9 },
    { 0x09, 0xa8 },
    { 0x0a, 0xa7 },
    { 0x0b, 0xff },
    { 0x0c, 0xff },
    { 0x0f, 0x00 },
    { 0x50, 0x1c },
    { 0x89, 0x03 },
    { 0xfe, 0x04 },
    { 0x28, 0x86 },
    { 0x29, 0x86 },
    { 0x2a, 0x86 },
    { 0x2b, 0x68 },
    { 0x2c, 0x68 },
    { 0x2d, 0x68 },
    { 0x2e, 0x68 },
    { 0x2f, 0x68 },
    { 0x30, 0x4f },
    { 0x31, 0x68 },
    { 0x32, 0x67 },
    { 0x33, 0x66 },
    { 0x34, 0x66 },
    { 0x35, 0x66 },
    { 0x36, 0x66 },
    { 0x37, 0x66 },
    { 0x38, 0x62 },
    { 0x39, 0x62 },
    { 0x3a, 0x62 },
    { 0x3b, 0x62 },
    { 0x3c, 0x62 },
    { 0x3d, 0x62 },
    { 0x3e, 0x62 },
    { 0x3f, 0x62 },
    /****DVP & MIPI****/
    { 0xfe, 0x01 },
    // {0x8c,0x01}, // test mode
    //{0x9a,0x06},    /* VSYNC low pulse */
    { 0x9a, 0x02 }, /* VSYNC high pulse */
    { 0xfe, 0x00 },
    { 0x7b, 0x2b },
    { 0x23, 0x2d },
    { 0xfe, 0x03 },
    { 0x01, 0x20 }, // DPHY_analog_mode1
    { 0x02, 0x56 }, // DPHY_analog_mode2
    { 0x03, 0xb2 }, // DPHY_analog_mode3
    { 0x12, 0x80 }, // LWC_set[7:0]
    { 0x13, 0x07 }, // LWC_set[15:8]

    { 0xfe, 0x00 },
    { 0x3e, 0x40 },
};

static struct image_sensor_config_s gc2053_config = {
    .name = "GC2053",
    .output_format = IMAGE_SENSOR_FORMAT_YUV422_YUYV,
    .slave_addr = 0x37,
    .id_size = 2,
    .reg_size = 1,
    .h_blank = 0x528,
    .resolution_x = 1920,
    .resolution_y = 1080,
    .id_addr = 0xf0f1,
    .id_value = 0x2053,
    .pixel_clock = 24000000,
    .init_list_len = sizeof(gc2053_init_list)/sizeof(gc2053_init_list[0]),
    .init_list = gc2053_init_list,
};

#endif /* __GC2053_H__ */
