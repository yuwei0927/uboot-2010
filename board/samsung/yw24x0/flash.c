/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
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

#include <common.h>

ulong myflush (void);


#define FLASH_BANK_SIZE	PHYS_FLASH_SIZE

#ifndef MAIN_SECT_SIZE
#define MAIN_SECT_SIZE  0x10000	/* 64 KByte/sector */
#endif

flash_info_t flash_info[CONFIG_SYS_MAX_FLASH_BANKS];


#define CMD_READ_ARRAY		0x000000F0
#define CMD_UNLOCK1			0x000000AA
#define CMD_UNLOCK2			0x00000055
#define CMD_ERASE_SETUP		0x00000080
#define CMD_ERASE_SECTOR	0x00000030
#define CMD_ERASE_BLOCK		0x00000050
#define CMD_ERASE_CHIP		0x00000010
#define CMD_PROGRAM			0x000000A0
#define CMD_UNLOCK_BYPASS	0x00000020
#define CMD_QUERY_ID		0x00000088
#define CMD_PRODUCT_ID		0x00000090
#define CMD_CFI_ENTRY		0x00000098
#define CMD_CFI_EXIT		0x000000F0


#define MEM_FLASH_ADDR1		(*(volatile u16 *)(CONFIG_SYS_FLASH_BASE + (0x000005555 << 1)))
#define MEM_FLASH_ADDR2		(*(volatile u16 *)(CONFIG_SYS_FLASH_BASE + (0x000002AAA << 1)))

#define BIT_ERASE_DONE		0x00000080
#define BIT_RDY_MASK		0x00000080
#define BIT_PROGRAM_ERROR	0x00000020
#define BIT_TIMEOUT			0x80000000	/* our flag */

#define READY 1
#define ERR   2
#define TMO   4

/*-----------------------------------------------------------------------
 */

ulong flash_init (void)
{
	int i, j;
	ulong size = 0;

	for (i = 0; i < CONFIG_SYS_MAX_FLASH_BANKS; i++) {
		ulong flashbase = 0;

		flash_info[i].flash_id =
#if defined(CONFIG_SST_39VF1601)
			(SST_MANUFACT & FLASH_VENDMASK) |
			(SST_ID_xF1601 & FLASH_TYPEMASK);
#else
#error "Unknown flash configured"
#endif
		flash_info[i].size = FLASH_BANK_SIZE;
		flash_info[i].sector_count = CONFIG_SYS_MAX_FLASH_SECT;
		memset (flash_info[i].protect, 0, CONFIG_SYS_MAX_FLASH_SECT);
		if (i == 0)
			flashbase = PHYS_FLASH_1;
		else
			panic ("configured too many flash banks!\n");
		for (j = 0; j < flash_info[i].sector_count; j++) {
#ifdef CONFIG_SST_39VF1601		
			flash_info[i].start[j] =
				flashbase + (j) * MAIN_SECT_SIZE;
#endif
		}
		size += flash_info[i].size;
	}

	flash_protect (FLAG_PROTECT_SET,
		       CONFIG_SYS_FLASH_BASE,
		       CONFIG_SYS_FLASH_BASE + monitor_flash_len - 1,
		       &flash_info[0]);

	flash_protect (FLAG_PROTECT_SET,
		       CONFIG_ENV_ADDR,
		       CONFIG_ENV_ADDR + CONFIG_ENV_SIZE - 1, &flash_info[0]);

	return size;
}


static ushort flash_read_ushort (uint offset)
{
	uchar *addr;
	vu_short retval;

	MEM_FLASH_ADDR1 = CMD_UNLOCK1;
	
	/* arm simple, non interrupt dependent timer */
	reset_timer_masked ();
	//udelay(10);
	MEM_FLASH_ADDR2 = CMD_UNLOCK2;
	
	/* arm simple, non interrupt dependent timer */
	reset_timer_masked ();
	//udelay(10);
	MEM_FLASH_ADDR1 = CMD_CFI_ENTRY;
	//udelay(100);
	/* arm simple, non interrupt dependent timer */
	reset_timer_masked ();

	while (1)
	{

		if ((*addr & 0x40) != (*addr & 0x40)) 
			continue; 
		if (*addr & 0x80)
			break;
	}
	retval = (*(volatile u16 *)(CONFIG_SYS_FLASH_BASE + (offset << 1)));

	MEM_FLASH_ADDR1 = CMD_UNLOCK1;
	MEM_FLASH_ADDR2 = CMD_UNLOCK2;
	MEM_FLASH_ADDR1 = CMD_CFI_EXIT;

	return retval;
}

ushort flash_read(flash_info_t * info, ulong dest)
{
	return flash_read_ushort (dest);
}

/*-----------------------------------------------------------------------
 */
void flash_print_info (flash_info_t * info)
{
	int i;

	switch (info->flash_id & FLASH_VENDMASK) {
	case (AMD_MANUFACT & FLASH_VENDMASK):
		printf ("AMD: ");
		break;
	case (SST_MANUFACT & FLASH_VENDMASK):
		printf ("SST: ");
		break;
	default:
		printf ("Unknown Vendor ");
		break;
	}

	switch (info->flash_id & FLASH_TYPEMASK) {
	case (AMD_ID_LV400B & FLASH_TYPEMASK):
		printf ("1x Amd29LV400BB (4Mbit)\n");
		break;
	case (AMD_ID_LV800B & FLASH_TYPEMASK):
		printf ("1x Amd29LV800BB (8Mbit)\n");
		break;
	case (SST_ID_xF1601 & FLASH_TYPEMASK):
		printf ("1x SST39xF1601 (16Mbit)\n");
		break;
	default:
		printf ("Unknown Chip Type\n");
		goto Done;
		break;
	}

	printf ("  Size: %ld MB in %d Sectors\n",
		info->size >> 20, info->sector_count);

	printf ("  Sector Start Addresses:");
	for (i = 0; i < info->sector_count; i++) {
		if ((i % 5) == 0) {
			printf ("\n   ");
		}
		printf (" %08lX%s", info->start[i],
			info->protect[i] ? " (RO)" : "     ");
	}
	printf ("\n");

      Done:;
}


/*-----------------------------------------------------------------------
 */

int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	//ushort result;
	int iflag, cflag, prot, sect;
	int rc = ERR_OK;
	//int chip;

	/* first look for protection bits */

	if (info->flash_id == FLASH_UNKNOWN)
		return ERR_UNKNOWN_FLASH_TYPE;

	if ((s_first < 0) || (s_first > s_last)) {
		return ERR_INVAL;
	}

#ifdef CONFIG_SST_39VF1601
	if ((info->flash_id & FLASH_VENDMASK) !=
		(SST_MANUFACT & FLASH_VENDMASK)) {
		return ERR_UNKNOWN_FLASH_VENDOR;
	}
#endif

	prot = 0;
	for (sect = s_first; sect <= s_last; ++sect) {
		if (info->protect[sect]) {
			prot++;
		}
	}
	if (prot)
		return ERR_PROTECTED;

	/*
	 * Disable interrupts which might cause a timeout
	 * here. Remember that our exception vectors are
	 * at address 0 in the flash, and we don't want a
	 * (ticker) exception to happen while the flash
	 * chip is in programming mode.
	 */
	cflag = icache_status ();
	icache_disable ();
	iflag = disable_interrupts ();

	/* Start erase on unprotected sectors */
	for (sect = s_first; sect <= s_last && !ctrlc (); sect++) {
		printf ("Erasing sector %2d ... ", sect);

		/* arm simple, non interrupt dependent timer */
		reset_timer_masked ();

		if (info->protect[sect] == 0) {	/* not protected */
			vu_short *addr = (vu_short *) (info->start[sect]);

			MEM_FLASH_ADDR1 = CMD_UNLOCK1;
			MEM_FLASH_ADDR2 = CMD_UNLOCK2;
			MEM_FLASH_ADDR1 = CMD_ERASE_SETUP;

			MEM_FLASH_ADDR1 = CMD_UNLOCK1;
			MEM_FLASH_ADDR2 = CMD_UNLOCK2;
			*addr = CMD_ERASE_SECTOR;

#ifdef CONFIG_SST_39VF1601
			/* wait until flash is ready */
			while(1){
				unsigned short i;
				i = *((volatile unsigned short *)addr) & 0x40;
				if(i != (*((volatile unsigned short *)addr) & 0x40))
					continue;
				if((*((volatile unsigned short *)addr)) & 0x80)
					break;
			}
			printf ("ok.\n");

		} else {   /* it was protected */
		printf ("protected!\n");
		}
	}
#endif
	if (ctrlc ())
		printf ("User Interrupt!\n");

      outahere:
	/* allow flash to settle - wait 10 ms */
	udelay_masked (10000);

	if (iflag)
		enable_interrupts ();

	if (cflag)
		icache_enable ();

	return rc;
}

/*-----------------------------------------------------------------------
 * Copy memory to flash
 */

volatile static int write_hword (flash_info_t * info, ulong dest, ushort data)
{
	vu_short *addr = (vu_short *) dest;
	ushort result;
	int rc = ERR_OK;
	int cflag, iflag;
//	int chip;

	/*
	 * Check if Flash is (sufficiently) erased
	 */
	result = *addr;
	if ((result & data) != data)
		return ERR_NOT_ERASED;


	/*
	 * Disable interrupts which might cause a timeout
	 * here. Remember that our exception vectors are
	 * at address 0 in the flash, and we don't want a
	 * (ticker) exception to happen while the flash
	 * chip is in programming mode.
	 */
	cflag = icache_status ();
	icache_disable ();
	iflag = disable_interrupts ();

	MEM_FLASH_ADDR1 = CMD_UNLOCK1;
	MEM_FLASH_ADDR2 = CMD_UNLOCK2;
//	MEM_FLASH_ADDR1 = CMD_UNLOCK_BYPASS;
	MEM_FLASH_ADDR1 = CMD_PROGRAM;
	*addr = data;

	/* arm simple, non interrupt dependent timer */
	reset_timer_masked ();
#ifdef CONFIG_SST_39VF1601
	/* wait until flash is ready */
	while(1){
		unsigned short i = *(volatile unsigned short *)addr & 0x40;
		if(i != (*(volatile unsigned short *)addr & 0x40))   //D6 == D6
			continue;
		if((*(volatile unsigned short *)addr & 0x80) == (data & 0x80)){
			rc = ERR_OK;
			break;     //D7 == D7
		}
	}
#endif
	if (iflag)
		enable_interrupts ();

	if (cflag)
		icache_enable ();

	return rc;
}

/*-----------------------------------------------------------------------
 * Copy memory to flash.
 */

int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{
	ulong cp, wp;
	int l;
	int i, rc;
	ushort data;

	wp = (addr & ~1);	/* get lower word aligned address */

	/*
	 * handle unaligned start bytes
	 */
	if ((l = addr - wp) != 0) {
		data = 0;
		for (i = 0, cp = wp; i < l; ++i, ++cp) {
			data = (data >> 8) | (*(uchar *) cp << 8);
		}
		for (; i < 2 && cnt > 0; ++i) {
			data = (data >> 8) | (*src++ << 8);
			--cnt;
			++cp;
		}
		for (; cnt == 0 && i < 2; ++i, ++cp) {
			data = (data >> 8) | (*(uchar *) cp << 8);
		}

		if ((rc = write_hword (info, wp, data)) != 0) {
			return (rc);
		}
		wp += 2;
	}

	/*
	 * handle word aligned part
	 */
	while (cnt >= 2) {
		data = *((vu_short *) src);
		if ((rc = write_hword (info, wp, data)) != 0) {
			return (rc);
		}
		src += 2;
		wp += 2;
		cnt -= 2;
	}

	if (cnt == 0) {
		return ERR_OK;
	}

	/*
	 * handle unaligned tail bytes
	 */
	data = 0;
	for (i = 0, cp = wp; i < 2 && cnt > 0; ++i, ++cp) {
		data = (data >> 8) | (*src++ << 8);
		--cnt;
	}
	for (; i < 2; ++i, ++cp) {
		data = (data >> 8) | (*(uchar *) cp << 8);
	}

	return write_hword (info, wp, data);
}
