/* Copyright (C) 2019, Ward Jaradat
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

#ifndef SRC_INCLUDE_FS_FS_EXTRA_H_
#define SRC_INCLUDE_FS_FS_EXTRA_H_

#include <fs/diskio.h>
#include <fs/ff.h>
#include <fs/integer.h>

FATFS PrimaryDisk;

#define MAX_PATH_LENGTH 255 

typedef struct {
	WORD	year;
	BYTE	month;
	BYTE	mday;
	BYTE	wday;
	BYTE	hour;
	BYTE	min;
	BYTE	sec;
} RTC;

int copy_read_buffer (char *ptr, BYTE *buf, int len);
int copy_write_buffer (char *ptr, const BYTE *buf, int len);

extern int init_disk(void);
extern int disk_ready; 

#endif
