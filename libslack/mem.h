/*
* libslack - http://libslack.org/
*
* Copyright (C) 1999, 2000 raf <raf@raf.org>
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
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
* or visit http://www.gnu.org/copyleft/gpl.html
*
* 20000902 raf <raf@raf.org>
*/

#ifndef LIBSLACK_MEM_H
#define LIBSLACK_MEM_H

#include <stdlib.h>

#include <slack/hdr.h>

#ifndef null
#define null NULL
#endif

#ifndef nul
#define nul '\0'
#endif

typedef struct Pool Pool;

__START_DECLS
#define mem_new(type) malloc(sizeof(type))
#define mem_create(size, type) malloc((size) * sizeof(type))
#define mem_resize(mem, size) mem_resize_fn((void *)&(mem), (size) * sizeof(*(mem)))
void *mem_resize_fn __PROTO ((void **mem, size_t size));
#define mem_release(mem) free(mem)
#define mem_destroy(mem) mem_destroy_fn((void **)&(mem))
void *mem_destroy_fn __PROTO ((void **mem));
void *mem_create_secure __PROTO ((size_t size));
void mem_release_secure __PROTO ((void *mem));
#define mem_destroy_secure(mem) mem_destroy_secure_fn((void **)&(mem))
void *mem_destroy_secure_fn __PROTO ((void **mem));
char *mem_strdup __PROTO ((const char *str));
#define mem_create2d(i, j, type) ((type **)mem_create_space(sizeof(type), (i), (j), 0))
#define mem_create3d(i, j, k, type) ((type ***)mem_create_space(sizeof(type), (i), (j), (k), 0))
#define mem_create4d(i, j, k, l, type) ((type ****)mem_create_space(sizeof(type), (i), (j), (k), (l), 0))
void *mem_create_space __PROTO ((size_t size, ...));
size_t mem_space_start __PROTO ((size_t size, ...));
#define mem_release2d(space) mem_release_space(space)
#define mem_release3d(space) mem_release_space(space)
#define mem_release4d(space) mem_release_space(space)
#define mem_release_space(space) mem_release(space)
#define mem_destroy2d(space) mem_destroy_space(space)
#define mem_destroy3d(space) mem_destroy_space(space)
#define mem_destroy4d(space) mem_destroy_space(space)
#define mem_destroy_space(space) mem_destroy(space)
Pool *pool_create __PROTO ((size_t size));
void pool_release __PROTO ((Pool *pool));
#define pool_destroy(pool) pool_destroy_fn(&(pool))
void *pool_destroy_fn __PROTO ((Pool **pool));
Pool *pool_create_secure __PROTO ((size_t size));
void pool_release_secure __PROTO ((Pool *pool));
#define pool_destroy_secure(pool) pool_destroy_secure_fn(&(pool))
void *pool_destroy_secure_fn __PROTO ((Pool **pool));
void pool_clear_secure __PROTO ((Pool *pool));
#define pool_new(pool, type) pool_alloc((pool), sizeof(type))
#define pool_newsz(pool, size, type) pool_alloc((pool), (size) * sizeof(type))
void *pool_alloc __PROTO ((Pool *pool, size_t bytes));
void pool_clear __PROTO ((Pool *pool));
__END_DECLS

#endif

/* vi:set ts=4 sw=4: */
