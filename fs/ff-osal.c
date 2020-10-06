/* Copyright (C) 2018, Ward Jaradat
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

#include <os/lib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <fs/dirent.h>
#include <fs/stat.h>
#include <fs/diskio.h>
#include <fs/ff.h>
#include <fs/osal.h>

extern int init_disk(void)
{
	int result;
	FRESULT fr;
	while (1)
	{
		if (blk_has_initialised())
		{
			fr = f_mount(&PrimaryDisk, "0:", 0);
			if (fr == FR_OK)
			{
				/* This is a startup information message */
				printk("file system  \t: initialised for block device 0\n");
				result = 1;
			}
			else
			{
				/* This is a startup information message */
				printk("file system  \t: uninitialised\n");
				result = 0;
			}
			break;
		}
		sleep(1000);
	}
	return result;
}

char* concat(const char *s1, const char *s2)
{
	char *result = malloc(strlen(s1)+strlen(s2)+1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

int stat(const char *pathname, struct stat *statbuf)
{
    FRESULT fr;
    FILINFO fno;

    char *p = concat("0:", pathname);
    fr = f_stat(p, &fno);
    if (fr == FR_OK)
    {
        if (fno.fattrib == AM_DIR)
        {
            statbuf->st_mode = S_IFDIR;
        }
        return 0;
    }
    errno = EINVAL;
    return -1;
}

int s_isdir(uint16_t st_mode)
{
    if (st_mode == S_IFDIR)
    {
        return 1;
    }
    return 0;
}

struct dirent *readdir(DIR *dir)
{
    FRESULT fr;
    static FILINFO fno;
    fr = f_readdir(dir, &fno);
    if (fr != FR_OK || fno.fname[0] == 0)
    {
        return NULL;
    }
    if (fno.fattrib & AM_DIR)
    {
        struct dirent *dnt = malloc(sizeof(struct dirent));
        int i;
        // int limit = FF_MAX_LFN + 1;
        for (i = 0; i < strlen(fno.fname) + 1; i++)
        {
            dnt->d_name[i] = fno.fname[i];
        }
        return dnt;
    }
}

DIR *opendir(const char *name)
{
    FRESULT fr;
    DIR *dir = malloc(sizeof(DIR));
    char *p;
    p = (char *)concat("0:", name);
    fr = f_opendir(dir, p);
    if (fr == FR_OK)
    {
        return dir;
    }
}

int lstat(const char *pathname, struct stat *statbuf)
{
    FRESULT fr;
    FILINFO fno;
    char *p = concat("0:", pathname);
    fr = f_stat(p, &fno);
    if (fr == FR_OK)
    {
        if (fno.fattrib == AM_DIR)
        {
            statbuf->st_mode = S_IFDIR;
        }
        return 0;
    }
    errno = EINVAL;
    return -1;
}

char *getcwd(char *buf, size_t size)
{
    FRESULT fr;
    fr = f_getcwd(buf, size);
    if (fr == FR_OK)
    {
        return buf;
    }
    errno = EINVAL;
    return NULL;
}

size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    FRESULT fr;
    UINT bw;
    UINT btr = (unsigned int)size * (unsigned int)nmemb;
    fr = f_write((FIL *)stream, ptr, btr, &bw);
    if (fr == FR_OK)
    {
        return nmemb;
    }
    return 0;
}

long ftell(FILE *stream)
{
    FSIZE_t fptr;
    fptr = f_tell((FIL *)stream);
    return (long)fptr;
}

int fseek(FILE *stream, unsigned long offset, int whence)
{
    FRESULT fr;
    if (whence == SEEK_SET)
    {
        fr = f_lseek((FIL *)stream, offset);
    }
    else if (whence == SEEK_CUR)
    {
        fr = f_lseek((FIL *)stream, (FSIZE_t)ftell(stream));
    }
    else if (whence == SEEK_END)
    {
        fr = f_lseek((FIL *)stream, f_size((FIL *)stream));
    }
    if (fr == FR_OK)
    {
        return 0;
    }
    errno = EBADF;
    return -1;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    FRESULT fr;
    UINT br;
    UINT btr = (unsigned int)size * (unsigned int)nmemb;
    fr = f_read((FIL *)stream, ptr, btr, &br);
    if (fr == FR_OK)
    {
        return nmemb;
    }
    return 0;
}

FILE * fopen(const char *path, const char *mode)
{
    FRESULT fr;
    FIL *fil;
    char *p;
    fil = (FIL *)malloc(sizeof(FIL));
    p = concat("0:", path);
    if (strcmp(mode, "r") == 0)
    {
        fr = f_open(fil, p, FA_READ);
    }
    else if (strcmp(mode, "w") == 0)
    {
        fr = f_open(fil, p, FA_WRITE | FA_CREATE_ALWAYS);
    }
    if (fr == FR_OK)
    {
        return (FILE *)fil;
    }
    errno = EINVAL;
    return NULL;
}

int fclose(FILE *stream)
{
    FRESULT res;
    res = f_close((FIL *)stream);
    if (res == FR_OK)
    {
        return 0;
    }
    errno = EBADF;
    return EOF;
}

int closedir(DIR *dir)
{
    FRESULT fr;
    fr = f_closedir(dir);
    if (fr == FR_OK)
    {
        return 0;
    }
}

int scandir(const char *dir, struct dirent ***namelist, int (*select)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **))
{
    DIR *d;
    struct dirent *D;
    int num = 0;
    if (!(d = opendir(dir)))
    {
        return -1;
    }
    *namelist = 0;
    while ((D = readdir(d)))
    {
        if (select == 0 || select(D))
        {
            struct dirent **tmp;
            if (!(tmp = realloc(*namelist, (num + 1) * sizeof(struct dirent **))) || !(tmp[num] = malloc(sizeof(struct dirent))))
            {
                int i;
                for (i = 0; i < num; ++i)
                    free((*namelist)[i]);
                free(*namelist);
                closedir(d);
                return -1;
            }
            memccpy(tmp[num]->d_name, D->d_name, 0, 255);
            ++num;
            *namelist = tmp;
        }
    }
    closedir(d);
    return num;
}
