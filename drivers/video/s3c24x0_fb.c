/*
 * (C) Copyright 2006 by OpenMoko, Inc.
 * Author: Harald Welte <laforge@openmoko.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
 
#if defined(CONFIG_VIDEO_S3C2440)
 
#include <video_fb.h>
#include <asm/arch/s3c2410.h>
//#include <asm/arch-s3c24x0/s3c24x0.h>

#include "videomodes.h"
//#include <s3c2410.h>
/*
 * Export Graphic Device
 */
static GraphicDevice smi;
 
#define VIDEO_MEM_SIZE  0x200000        /* 240x320x16bit = 0x25800 bytes */
 
//extern void board_video_init(GraphicDevice *pGD);
/*
 * Add by yanghao
 */
#if defined(CONFIG_VIDEO_S3C2440)
#if   1
//#define MVAL                   (13)
#define MVAL                   (0)
#define MVAL_USED              (0)
#define INVVDEN                (1)
#define BSWP                   (0)
#define HWSWP                  (1)
//TFT 240320
#define LCD_XSIZE_TFT_240320   (320)
#define LCD_YSIZE_TFT_240320   (240)
//TFT 240320
#define HOZVAL_TFT_240320      (LCD_XSIZE_TFT_240320-1)
#define LINEVAL_TFT_240320     (LCD_YSIZE_TFT_240320-1)
//Timing parameter for WXCAT35-TG#001
#if 1
#define VBPD_240320            (3)
#define VFPD_240320            (5)
#define VSPW_240320            (15)
#define HBPD_240320            (58)
#define HFPD_240320            (15)
#define HSPW_240320_WXCAT35    (8)//adjust the horizontal displacement of the screen
#endif
#if 0
#define VBPD_240320            (14)
#define VFPD_240320            (11)
#define VSPW_240320            (2)
#define HBPD_240320            (37)
#define HFPD_240320            (19)
#define HSPW_240320_WXCAT35    (29)
#define HSPW_240320_WXCAT35    (8)//adjust the horizontal displacement of the screen
#endif
#define CLKVAL_TFT_240320      (7)
//FCLK = 405MHZ, HCLK = 101.25MHZ, VCLK=4602272HZ

void board_video_init(GraphicDevice *pGD)
{
    struct s3c24x0_lcd * const lcd = s3c24x0_get_base_lcd();
   /*Configuration for fl2440*/
  // lcd->LCDCON1 = 0x00000778;
   lcd->LCDCON1 = (CLKVAL_TFT_240320 <<8)|(MVAL_USED <<7)|(3<<5)|(12<<1)|0;
   lcd->LCDCON2 = (VBPD_240320<<24)|(LINEVAL_TFT_240320<<14)|(VFPD_240320<<6)|(VSPW_240320);
   lcd->LCDCON3 = (HBPD_240320<<19)|(HOZVAL_TFT_240320<<8)|(HFPD_240320);
   lcd->LCDCON4 = (MVAL<<8)|(HSPW_240320_WXCAT35);
  // lcd->LCDCON5 = 0x00000f09;
   lcd->LCDCON5 = (1<<11)|(1<<10)|(1<<9)|(1<<8)|(1<<3)|(BSWP<<1)|(HWSWP);
   lcd->LPCSEL = 0x00000000;
}

#endif
#endif /*CONFIG_VIDEO_S3C2440*/
 

/*******************************************************************************
 *
 * Init video chip with common Linux graphic modes (lilo)
 */
void *video_hw_init (void)
{
    struct s3c24x0_lcd * const lcd = s3c24x0_get_base_lcd();
    GraphicDevice *pGD = (GraphicDevice *)&smi;
    int videomode;
    unsigned long t1, hsynch, vsynch;
    char *penv;
    int tmp, i, bits_per_pixel;
    struct ctfb_res_modes *res_mode;
    struct ctfb_res_modes var_mode;
//    unsigned char videoout;
 
    /* Search for video chip */
    printf("Video: ");

    tmp = 0;
 
         videomode = CONFIG_SYS_DEFAULT_VIDEO_MODE;
         /* get video mode via environment */
         if ((penv = getenv ("videomode")) != NULL) {
                 /* deceide if it is a string */
                 if (penv[0] <= '9') {
                         videomode = (int) simple_strtoul (penv, NULL, 16);
                         tmp = 1;
                 }
         } else {
                 tmp = 1;
         }
         if (tmp) {
                 /* parameter are vesa modes */
                 /* search params */
                 for (i = 0; i < VESA_MODES_COUNT; i++) {
                         if (vesa_modes[i].vesanr == videomode)
                                 break;
                 }
                 if (i == VESA_MODES_COUNT) {
                         printf ("no VESA Mode found, switching to mode 0x%x ", CONFIG_SYS_DEFAULT_VIDEO_MODE);
                         i = 0;
                }
                 res_mode =
                         (struct ctfb_res_modes *) &res_mode_init[vesa_modes[i].
                                                                  resindex];
                 bits_per_pixel = vesa_modes[i].bits_per_pixel;
         } else {
 
                res_mode = (struct ctfb_res_modes *) &var_mode;
                 bits_per_pixel = video_get_params (res_mode, penv);
         }
 
         /* calculate hsynch and vsynch freq (info only) */
         t1 = (res_mode->left_margin + res_mode->xres +
               res_mode->right_margin + res_mode->hsync_len) / 8;
         t1 *= 8;
         t1 *= res_mode->pixclock;
         t1 /= 1000;
         hsynch = 1000000000L / t1;
         t1 *=
                 (res_mode->upper_margin + res_mode->yres +
                  res_mode->lower_margin + res_mode->vsync_len);
         t1 /= 1000;
         vsynch = 1000000000L / t1;
 
         /* fill in Graphic device struct */
         sprintf (pGD->modeIdent, "%dx%dx%d %ldkHz %ldHz", res_mode->xres,
                  res_mode->yres, bits_per_pixel, (hsynch / 1000),
                  (vsynch / 1000));
         printf ("%s\n", pGD->modeIdent);
         pGD->winSizeX = res_mode->xres;
         pGD->winSizeY = res_mode->yres;
         pGD->plnSizeX = res_mode->xres;
         pGD->plnSizeY = res_mode->yres;
             
         switch (bits_per_pixel) {
         case 8:
                 pGD->gdfBytesPP = 1;
                 pGD->gdfIndex = GDF__8BIT_INDEX;
                 break;
         case 15:
                 pGD->gdfBytesPP = 2;
                 pGD->gdfIndex = GDF_15BIT_555RGB;
                 break;
         case 16:
                 pGD->gdfBytesPP = 2;
                 pGD->gdfIndex = GDF_16BIT_565RGB;
                 break;
         case 24:
                 pGD->gdfBytesPP = 3;
                 pGD->gdfIndex = GDF_24BIT_888RGB;
                 break;
         }
 
         /* statically configure settings */
        pGD->winSizeX = pGD->plnSizeX = 320;
         pGD->winSizeY = pGD->plnSizeY = 240;
         pGD->gdfBytesPP = 2;
         pGD->gdfIndex = GDF_16BIT_565RGB;
 
         pGD->frameAdrs = LCD_VIDEO_ADDR;
         pGD->memSize = VIDEO_MEM_SIZE;
 
         board_video_init(pGD);
 
         lcd->LCDSADDR1 = pGD->frameAdrs >> 1;
 
         /* This marks the end of the frame buffer. */
         lcd->LCDSADDR2 = (lcd->LCDSADDR1&0x1fffff) + (pGD->winSizeX+0) * pGD->winSizeY;
         lcd->LCDSADDR3 = (pGD->winSizeX & 0x7ff);
 
         /* Clear video memory */
         memset((void *)pGD->frameAdrs, 0, pGD->memSize);
 
         /* Enable  Display  */
         lcd->LCDCON1 |= 0x01;   /* ENVID = 1 */
 
         return ((void*)&smi);
 }
 #if 0
 void
 video_set_lut (unsigned int index,      /* color number */
                unsigned char r, /* red */
                unsigned char g, /* green */
                unsigned char b  /* blue */
		)
 {
 }
#endif
 
 #endif /* CONFIG_VIDEO_S3C24x0 */
