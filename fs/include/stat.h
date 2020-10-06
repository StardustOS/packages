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
 * 
 * Notes:
 * 
 * Some code has been ported from dietlibc and this file needs to be 
 * cleaned.
 */

#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <fs/cdefs.h>
#include <fs/types.h>
#include <endian.h>
#include <errno.h>

__BEGIN_DECLS

struct stat {
	uint16_t	st_mode;
	unsigned long	st_size;
	time_t		st_mtime;
	time_t		st_atime;
};

int s_isdir(uint16_t st_mode);

#define S_ISDIR(m)	(s_isdir(m))

#define S_IFMT	    00170000
#define S_IFSOCK	0140000
#define S_IFLNK		0120000
#define S_IFREG		0100000
#define S_IFBLK		0060000
#define S_IFDIR		0040000
#define S_IFCHR		0020000
#define S_IFIFO		0010000
#define S_ISUID		0004000
#define S_ISGID		0002000
#define S_ISVTX		0001000

#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)

#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001

static inline int fstat(int __fd, struct stat *__buf) {
	errno = EBADF;
	return -1;
}

int lstat(const char *pathname, struct stat *statbuf);


#if __WORDSIZE == 64
#define __NO_STAT64
#else
extern int stat64(const char *__file, struct stat64 *__buf) __THROW;
extern int fstat64(int __fd, struct stat64 *__buf) __THROW;
extern int lstat64(const char *__file, struct stat64 *__buf) __THROW;

#if defined _FILE_OFFSET_BITS && _FILE_OFFSET_BITS == 64
#define lstat lstat64
#define fstat fstat64
#define stat stat64
#define pread pread64
#define pwrite pwrite64
#endif
#endif

#define major(dev) ({ unsigned long long l=(dev); ((l>>8) & 0xfff) | (l >> 32) & 0xfffff000; })
#define minor(dev) ({ unsigned long long l=(dev); (l & 0xff) | ((l>>12)&0xffffff00); })
#define makedev(maj,min) ({ unsigned long long a=(maj), i=(min); ((a&0xfff)<<8) | (i&0xff) | ((a&~0xfff)<<32) | ((i&0xfffff00)<<12); })

static inline int chmod (const char *__file, mode_t __mode) {
	errno = EINVAL;
    return -1;
}

extern int fchmod (int __fd, mode_t __mode) __THROW;
extern mode_t umask (mode_t __mask) __THROW;
static inline int mkdir (const char *__path, mode_t __mode) {
 	errno = EINVAL;
    return -1;
}
extern int mknod (const char *__path, mode_t __mode, dev_t __dev) __THROW;
extern int mkfifo (const char *__path, mode_t __mode) __THROW;

#define S_IREAD S_IRUSR
#define S_IEXEC S_IXUSR

#if defined(_ATFILE_SOURCE) || ((_XOPEN_SOURCE + 0) >= 700) || ((_POSIX_C_SOURCE + 0) >= 200809L)

#endif

__END_DECLS

#endif
