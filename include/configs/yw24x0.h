/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <gj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * Configuation settings for the SAMSUNG SMDK2410 board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM920T		1	/* This is an ARM920T Core	*/
#define	CONFIG_S3C2440		1	/* in a SAMSUNG S3C2410 SoC     */
#define CONFIG_S3C24X0		1	/* in a SAMSUNG S3C24x0-type SoC	*/
#define CONFIG_SMDK2410		1	/* on a SAMSUNG SMDK2410 Board  */

/* input clock of PLL */
#define CONFIG_SYS_CLK_FREQ	12000000/* the SMDK2410 has 12MHz input clock */


#define USE_920T_MMU		1

//#define CONFIG_USB_DEVICE   1

#ifdef CONFIG_USB_DEVICE
#define CONFIG_USE_IRQ		1
#endif

#if 0
#define CONFIG_JFFS2_CMDLINE 1
#define CONFIG_JFFS2_NAND    1

#define MTDIDS_DEFAULT "nand0=nandflash0"
#define MTDPARTS_DEFAULT "mtdparts=nandflash0:2m@0(kernel)," \
                            "8m(jffs2)," \
                            "-(yaffs)"
#endif
/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128*1024)
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* size in bytes reserved for initial data */

/*
 * Hardware drivers
 */
 
#define CONFIG_NET_MULTI	1
//#define CONFIG_CS8900	1	/* we have a CS8900 on-board */
#ifdef CONFIG_CS8900
#define CONFIG_CS8900_BASE	0x19000300
#define CONFIG_CS8900_BUS16	/* the Linux driver does accesses as shorts */
#endif


#define CONFIG_DRIVER_DM9000		1
#ifdef CONFIG_DRIVER_DM9000
#define CONFIG_DM9000_USE_16BIT 	1
#if 0
#define CONFIG_DM9000_BASE			0x20000000
#else
#define CONFIG_DM9000_BASE               0x20000300
#endif
#define DM9000_IO                        CONFIG_DM9000_BASE 
#define DM9000_DATA                      (CONFIG_DM9000_BASE + 4)
#endif


/*
 * select serial console configuration
 */
#define CONFIG_S3C24X0_SERIAL
#define CONFIG_SERIAL1          1	/* we use SERIAL 1 on SMDK2410 */

/************************************************************
 * RTC
 ************************************************************/
#define	CONFIG_RTC_S3C24X0	1

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BAUDRATE		115200


#if 0
/* for tag(s) to transfer message to kernel, www.arm9.net */
#define CONFIG_SETUP_MEMORY_TAGS    1
#define CONFIG_CMDLINE_TAG          1
#endif

/***********************************************************
 * Command definition
 ***********************************************************/
#if 0
#define CONFIG_COMMANDS \
                        ((CONFIG_CMD_DFL | \
                        CFG_CMD_CACHE    | \
                    /* Start: by www.arm9.net */ \
                        /* CFG_CMD_PING     | */ \
                        CFG_CMD_JFFS2    | \
                        CFG_CMD_NAND     | \
                    /* End: by www.arm9.net */ \
                        /*CFG_CMD_EEPROM |*/ \
                       /* CFG_CMD_I2C    |*/ \
                       /* CFG_CMD_USB    |*/ \
                        CFG_CMD_REGINFO  | \
                        CFG_CMD_DATE     | \
                        CFG_CMD_ELF))


/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>
#endif
#include <config_cmd_default.h>

#define CONFIG_CMD_CACHE
#define CONFIG_CMD_DATE
#define CONFIG_CMD_ELF
#define CONFIG_CMD_NAND


#define CONFIG_BOOTDELAY	1
#define CONFIG_BOOTARGS    	"noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0"
#define CONFIG_ETHADDR	    08:00:3e:26:0a:5b
#define CONFIG_NETMASK      255.255.0.0
#define CONFIG_IPADDR		172.17.1.150
#define CONFIG_SERVERIP		172.17.1.110
/*#define CONFIG_BOOTFILE	"elinos-lart" */
#define CONFIG_BOOTCOMMAND	"nand read.jffs2 0x32000000 kernel; bootm 0x32000000"

#define CONFIG_NET_MULTI	1	/* Multi ethernet cards support */

#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200		/* speed to run kgdb serial port */
/* what's this ? it's not used anywhere */
#define CONFIG_KGDB_SER_INDEX	1		/* which serial port to use */
#endif

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_SYS_LONGHELP				/* undef to save memory		*/
#define	CONFIG_SYS_PROMPT		"uboot # "	/* Monitor Command Prompt	*/
#define	CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size	*/
#define	CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) /* Print Buffer Size */
#define	CONFIG_SYS_MAXARGS		16		/* max number of command args	*/
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size	*/
	
#define CONFIG_SYS_MEMTEST_START	0x30000000	/* memtest works on	*/
#define CONFIG_SYS_MEMTEST_END		0x33F00000	/* 63 MB in DRAM	*/

//#undef  CFG_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define	CONFIG_SYS_LOAD_ADDR		0x33000000	/* default load address	*/

/* the PWM TImer 4 uses a counter of 15625 for 10 ms, so we need */
/* it to wrap 100 times (total 1562500) to get 1 sec. */
#define	CONFIG_SYS_HZ			1562500

/* valid baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	   /* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		0x30000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	0x04000000 /* 64 MB */

#define PHYS_FLASH_1		0x00000000 /* Flash Bank #1 */

#define CONFIG_SYS_FLASH_BASE		PHYS_FLASH_1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */

#define CONFIG_SST_39VF1601	1	/* uncomment this if you have a SST 39VF1601 flash */
#ifdef CONFIG_SST_39VF1601
#define PHYS_FLASH_SIZE		0x00200000 /* 2MB */
#define CONFIG_SYS_MAX_FLASH_SECT	(512)	/* max number of sectors on one chip, 512 sectors */
//根据SST39VF1601的芯片手册描述，对其进行操作有两种方式：块方式和扇区方式。
//现采用扇区方式(sector) sector = 2Kword = 4Kbyte，所对应的Nor Flash共有512个sector
#define CONFIG_ENV_ADDR		(CONFIG_SYS_FLASH_BASE + 0x100000) /* addr of environment */
#define MAIN_SECT_SIZE		0x1000	/* 4 KByte/sector */
#endif

/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(5*CONFIG_SYS_HZ) /* Timeout for Flash Erase */
#define CONFIG_SYS_FLASH_WRITE_TOUT	(5*CONFIG_SYS_HZ) /* Timeout for Flash Write */

//#define	CONFIG_ENV_IS_IN_FLASH	1
#define CONFIG_ENV_IS_IN_NAND  1
#define CONFIG_ENV_OFFSET      0x40000
#define CONFIG_ENV_SIZE		0x10000	/* Total Size of Environment Sector */

/*-----------------------------------------------------------------------
 * NAND flash settings
 */
#define CONFIG_NAND_S3C2440		1
#define CONFIG_SYS_NAND_BASE           0
#define CONFIG_SYS_MAX_NAND_DEVICE     1
#define NAND_MAX_CHIPS          1

/* Audio */
#define CONFIG_SPEAKER			1
//#define CONFIG_HEADPHONE		0
//#define CONFIG_AUDIO_UDA1341TS	0

/*LCD support*/
#define CONFIG_VIDEO_S3C2440     1
#define CONFIG_VIDEO_LOGO        1	//display Linux logo in upper left corner
#define CONFIG_VIDEO_BMP_LOGO           1
//#define VIDEO_FB_16BPP_PIXEL_SWAP       1
#define VIDEO_FB_16BPP_WORD_SWAP       1
//#define CONFIG_CMD_BMP                  1
//#define CONFIG_LCD               1
#define CONFIG_VIDEO               1
#define CONFIG_CFB_CONSOLE       1
//#define CONFIG_VGA_AS_SINGLE_DEVICE 1          //see Cfb_console.c/drv_video_init
#define CFG_CONSOLE_INFO_QUIET        //support display of console information at boot
#define CONFIG_CONSOLE_MUX

#define CONFIG_SYS_CONSOLE_IS_IN_ENV



#define LCD_VIDEO_ADDR          0x33b00000

/*for PC-keyboard*/
#define VIDEO_KBD_INIT_FCT      0
/*for PC-keyboard*/

#define VIDEO_TSTC_FCT          serial_tstc
#define VIDEO_GETC_FCT          serial_getc

#define CONFIG_SERIAL_MULTI     1



#define CONFIG_DISPLAY_CPUINFO	1
#define	CONFIG_DISPLAY_BOARDINFO	1

//#define LED_DEBUG				1


#endif	/* __CONFIG_H */
