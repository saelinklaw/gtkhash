/*
 *   Copyright (C) 2007-2020 Tristan Heaven <tristan@tristanheaven.net>
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

#ifndef GTKHASH_HASH_HASH_FUNC_H
#define GTKHASH_HASH_HASH_FUNC_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "digest-format.h"

#define HASH_FUNCS_N (HASH_FUNC_ADLER32 + 1)
#define HASH_FUNC_IS_VALID(X) (((X) >= 0) && ((X) < HASH_FUNCS_N))
#define HASH_FUNC_IS_DEFAULT(X) ( \
	(X) == HASH_FUNC_MD5 || \
	(X) == HASH_FUNC_SHA1 || \
	(X) == HASH_FUNC_SHA256 || \
	(X) == HASH_FUNC_CRC32)

// All supported hash functions
// Note: Default ordering is defined here
enum hash_func_e {
	HASH_FUNC_INVALID = -1,
	HASH_FUNC_MD2,
	HASH_FUNC_MD4,
	HASH_FUNC_MD5,
	HASH_FUNC_MD6_224,
	HASH_FUNC_MD6_256,
	HASH_FUNC_MD6_384,
	HASH_FUNC_MD6_512,
	HASH_FUNC_SHA1,
	HASH_FUNC_SHA224,
	HASH_FUNC_SHA256,
	HASH_FUNC_SHA384,
	HASH_FUNC_SHA512,
	HASH_FUNC_SHA3_224,
	HASH_FUNC_SHA3_256,
	HASH_FUNC_SHA3_384,
	HASH_FUNC_SHA3_512,
	HASH_FUNC_BLAKE2B,
	HASH_FUNC_BLAKE2S,
	HASH_FUNC_BLAKE2BP,
	HASH_FUNC_BLAKE2SP,
	HASH_FUNC_RIPEMD128,
	HASH_FUNC_RIPEMD160,
	HASH_FUNC_RIPEMD256,
	HASH_FUNC_RIPEMD320,
	HASH_FUNC_TIGER192,
	HASH_FUNC_WHIRLPOOL,
	HASH_FUNC_SM3,
	HASH_FUNC_GOST,
	HASH_FUNC_MDC2,
	HASH_FUNC_CRC32,
	HASH_FUNC_CRC32C,
	HASH_FUNC_ADLER32,
};

struct hash_func_s {
	const char *name;
	struct digest_s *digest;
	void *lib_data;
	struct hash_func_s *hmac_data;
	enum hash_func_e id;
	uint8_t digest_size;
	uint8_t block_size;
	bool supported:1;
	bool hmac_supported:1;
	bool enabled:1;
};

enum hash_func_e gtkhash_hash_func_get_id_from_name(const char *name);
void gtkhash_hash_func_set_digest(struct hash_func_s *func, uint8_t *digest,
	size_t size);
char *gtkhash_hash_func_get_digest(struct hash_func_s *func,
	enum digest_format_e format);
void gtkhash_hash_func_clear_digest(struct hash_func_s *func);
void gtkhash_hash_func_init(struct hash_func_s *func,
	enum hash_func_e id);
void gtkhash_hash_func_init_all(struct hash_func_s *funcs);
void gtkhash_hash_func_deinit(struct hash_func_s *func);
void gtkhash_hash_func_deinit_all(struct hash_func_s *funcs);

#endif
