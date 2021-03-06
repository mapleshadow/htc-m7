/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */



#include "msm_fb.h"
#include "msm_fb_panel.h"
#include "mipi_dsi.h"
#include "mipi_tc358764_dsi2lvds.h"

#define MHZ (1000*1000)

static struct msm_panel_info chimei_wuxga_pinfo;

static struct mipi_dsi_phy_ctrl dsi_video_mode_phy_db = {
	
	.regulator = {0x03, 0x0a, 0x04, 0x00, 0x20}, 
	
	.ctrl = {0x5f, 0x00, 0x00, 0x10}, 
	
	.strength = {0xff, 0x00, 0x06, 0x00}, 
	
	.timing = { 0xC9, 0x92, 0x29, 
	0, 
	0x2D, 0x9B, 0x2B, 0x94, 0x2D, 0x03, 0x04},  

	
	.pll = { 0x00, 
	
	0x30, (0x01 | 0x30) , (0x19 | 0xC0), 
	0x00, 0x50, 0x48, 0x63,
	0x77, 0x88, 0x99, 
	0x00, 0x14, 0x03, 0x00, 0x02, 
	0x00, 0x20, 0x00, 0x01 }, 
};

static int __init mipi_chimei_wuxga_init(void)
{
	int ret;
	struct msm_panel_info *pinfo = &chimei_wuxga_pinfo;

	if (msm_fb_detect_client("mipi_video_chimei_wuxga"))
		return 0;

	pr_info("mipi-dsi chimei wuxga (1200x1920) driver ver 1.0.\n");

	
	pinfo->xres = 1200;
	pinfo->yres = 1920;
	pinfo->type =  MIPI_VIDEO_PANEL;
	pinfo->pdest = DISPLAY_1; 
	pinfo->wait_cycle = 0;
	pinfo->bpp = 24; 
	pinfo->fb_num = 2; 


	pinfo->clk_rate = 635 * MHZ ; 


	pinfo->lcdc.h_front_porch = 160-48-32;	
	pinfo->lcdc.h_back_porch = 48;	
	pinfo->lcdc.h_pulse_width = 32;	

	pinfo->lcdc.v_front_porch = 26-3-6;	
	pinfo->lcdc.v_back_porch = 3;	
	pinfo->lcdc.v_pulse_width = 6;	

	pinfo->lcdc.border_clr = 0;		
	pinfo->lcdc.underflow_clr = 0xff;	

	pinfo->lcdc.hsync_skew = 0;

	
	pinfo->bl_max = PWM_LEVEL;
	pinfo->bl_min = 1;

	
	pinfo->mipi.vc = 0; 
	pinfo->mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo->mipi.tx_eot_append = true;
	pinfo->mipi.t_clk_post = 34;		
	pinfo->mipi.t_clk_pre = 69;		

	pinfo->mipi.dsi_phy_db = &dsi_video_mode_phy_db;

	
	pinfo->mipi.frame_rate = 60;
	pinfo->mipi.data_lane0 = true;
	pinfo->mipi.data_lane1 = true;
	pinfo->mipi.data_lane2 = true;
	pinfo->mipi.data_lane3 = true;
	pinfo->mipi.esc_byte_ratio = 6;

	pinfo->mipi.mode = DSI_VIDEO_MODE;
	pinfo->mipi.dst_format = DSI_VIDEO_DST_FORMAT_RGB565;

	
	pinfo->mipi.traffic_mode = DSI_NON_BURST_SYNCH_EVENT;
	pinfo->mipi.pulse_mode_hsa_he = false; 

	pinfo->mipi.hfp_power_stop = false;
	pinfo->mipi.hbp_power_stop = false;
	pinfo->mipi.hsa_power_stop = false;
	pinfo->mipi.eof_bllp_power_stop = false;
	pinfo->mipi.bllp_power_stop = false;

	
	pinfo->mipi.te_sel = 1; 
	pinfo->mipi.interleave_max = 1;
	
	pinfo->mipi.insert_dcs_cmd = false;
	pinfo->mipi.wr_mem_continue = 0;
	pinfo->mipi.wr_mem_start = 0;
	pinfo->mipi.stream = false; 
	pinfo->mipi.mdp_trigger = DSI_CMD_TRIGGER_NONE;
	pinfo->mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo->mipi.no_max_pkt_size = 1;
	pinfo->mipi.force_clk_lane_hs = 1;

	pinfo->is_3d_panel = FB_TYPE_3D_PANEL;

	ret = mipi_tc358764_dsi2lvds_register(pinfo, MIPI_DSI_PRIM, 1);
	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	return ret;
}

module_init(mipi_chimei_wuxga_init);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Chimei WUXGA LVDS Panel driver");
MODULE_AUTHOR("Amir Samuelov <amirs@codeaurora.org>");
