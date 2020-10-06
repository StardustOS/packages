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

#include <stdlib.h>
#include <stdio.h>

extern int fs_read_test(int * file_len) 
{
    int read_len;
    int i;
    FILE *fd;
    char *data;

    fd = fopen("sample.txt", "r");
    fseek(fd, 0L, SEEK_END);
    *file_len = ftell(fd);
    fseek(fd, 0L, SEEK_SET);
    data = malloc(*file_len);
    read_len = fread(data, sizeof(char), *file_len, fd);

    for (i = 0; i < read_len; i++)
    {
        printf("%c", data[i]);
    }

    fclose(fd);
    free(data);
}