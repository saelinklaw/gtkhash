/*
 *   Copyright (C) 2007-2016 Tristan Heaven <tristan@tristanheaven.net>
 *
 *   This file is part of GtkHash.
 *
 *   GtkHash is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   GtkHash is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GtkHash. If not, see <https://gnu.org/licenses/gpl-2.0.txt>.
 */

#ifndef GTKHASH_HASH_H
#define GTKHASH_HASH_H

#include "hash/hash-func.h"
#include "hash/hash-file.h"

extern struct hash_s {
	struct hash_func_s funcs[HASH_FUNCS_N];
} hash;

void hash_file_start(const char *uri);
void hash_file_list_start(void);
void hash_file_stop(void);
void hash_string(void);
unsigned int hash_funcs_count_enabled(void);
void hash_funcs_enable_strv(const char **funcs);
void hash_init(void);
void hash_deinit(void);

#endif
