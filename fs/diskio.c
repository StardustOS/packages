/* Copyright (C) 2017, Ward Jaradat
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <fs/diskio.h>
#include <fs/osal.h>
#include <os/console.h>
#include <os/blkfront-extra.h>
#include <os/kernel.h>
#include <os/hypervisor.h>
#include <os/mm.h>
#include <os/events.h>
#include <os/time.h>
#include <os/sched.h>
#include <os/smp.h>
#include <os/gnttab.h> 
#include <os/types.h>
#include <os/lib.h>
#include <os/spinlock.h>
#include <os/xmalloc.h>
#include <os/mutexes.h>
#include <os/xenbus.h>

DSTATUS disk_status (BYTE pdrv)
{
	if (blk_has_initialised())
	{
		return 0;
	}
	return STA_NOINIT;
}

DSTATUS disk_initialize (BYTE pdrv)
{
	if (blk_has_initialised())
	{
		return 0;
	}
	return STA_NOINIT;
}

int copy_read_buffer (char *ptr, BYTE *buf, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		buf[i] = ptr[i];
	}
	return len;
}

DRESULT disk_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
	int res;
	int device_id, len;
	void * buf;
	device_id = (int) pdrv;
	buf = (unsigned char *) alloc_pages( get_order(512 * count) );
	len = 512 * count;
	res = blk_read(device_id, sec_to_addr(sector), buf, 512* count);
	if (res > 0)
	{
		int copy_read_buffer_res;
		copy_read_buffer_res = copy_read_buffer(buf, buff, len);
		if (copy_read_buffer_res == len)
		{
			return RES_OK;
		}
	}
	return RES_ERROR;
}

int copy_write_buffer (char *ptr, const BYTE *buf, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		ptr[i] = buf[i];
	}
	return len;
}

DRESULT disk_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
	int res;
	int device_id, len;
	void * buf;
	device_id = (int) pdrv;
	buf = (unsigned char *) alloc_pages( get_order(512 * count) );
	len = 512 * count;
	copy_write_buffer(buf, buff, len);
	res = blk_write(device_id, sec_to_addr(sector), buf, 512 * count);
	if (res > 0)
	{
		return RES_OK;
	}
	return RES_ERROR;
}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void *buff)
{
	int device_id;
	device_id = (int) pdrv;

	if (cmd == GET_SECTOR_COUNT)
	{
		*((DWORD*)buff)=(DWORD) blk_get_sectors(device_id);
		return RES_OK;
	}
	else if (cmd == GET_SECTOR_SIZE)
	{
		*((DWORD*)buff)=(DWORD) blk_get_sector_size(device_id);
		return RES_OK;
	}
	else if (cmd == GET_BLOCK_SIZE)
	{
		return 1;
	} else if (cmd == CTRL_SYNC)
	{
		return RES_OK;
	}
	return RES_PARERR;
}

DWORD get_fattime (void)
{
	RTC rtc;
	/* Currently there is no function to get the local time so it is going to be manually set for now */
	rtc.year = 2017;
	rtc.month = 1;
	rtc.mday = 1;
	rtc.wday = 1;
	rtc.hour = 12;
	rtc.min = 0;
	rtc.sec = 0;
	return	  ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.month << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);
}
