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

#ifndef _DIRENT_H
#define _DIRENT_H       1

#include <os/lib.h>
#include <stdlib.h>
#include <fs/cdefs.h>
#include <fs/types.h>
#include <limits.h>
#include <string.h>
#include <fs/ff.h>

struct dirent
{
    char  d_name[256];
};

DIR *opendir(const char *name);

struct dirent *readdir(DIR *dir);

int scandir(const char *dir, struct dirent ***namelist, int (*selection)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **));

static inline int alphasort(const struct dirent **a, const struct dirent **b) {
    return strcmp((*a)->d_name,(*b)->d_name);
}

#endif
