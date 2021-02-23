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

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <gtk/gtk.h>

#include "hash.h"
#include "main.h"
#include "gui.h"
#include "list.h"
#include "hash/hash-func.h"
#include "hash/hash-string.h"
#include "hash/hash-file.h"
#include "util/util.h"

struct hash_s hash;

static struct {
	struct hash_file_s *hfile;
	unsigned int list_row;
} hash_priv = {
	.hfile = NULL,
	.list_row = 0,
};

void gtkhash_hash_string_finish_cb(void)
{
	gui_check_digests();
}

void gtkhash_hash_string_digest_cb(const enum hash_func_e id,
	const char *digest)
{
	gtk_entry_set_text(gui.hash_widgets[id].entry_text, digest);
}

void gtkhash_hash_file_report_cb(G_GNUC_UNUSED void *data,
	const goffset file_size, const goffset total_read, GTimer *timer)
{
	gtk_progress_bar_set_fraction(gui.progressbar,
		(double)total_read /
		(double)file_size);

	const double elapsed = g_timer_elapsed(timer, NULL);
	if (elapsed <= 1)
		return;

	char *text = gtkhash_format_progress(file_size, total_read, elapsed);
	gtk_progress_bar_set_text(gui.progressbar, text);
	g_free(text);
}

void gtkhash_hash_file_digest_cb(const enum hash_func_e id,
	const char *digest, G_GNUC_UNUSED void *data)
{
	switch (gui.view) {
		case GUI_VIEW_FILE:
			gtk_entry_set_text(gui.hash_widgets[id].entry_file, digest);
			break;
		case GUI_VIEW_FILE_LIST:
			list_set_digest(hash_priv.list_row, id, digest);
			break;
		default:
			g_assert_not_reached();
	}
}

void gtkhash_hash_file_finish_cb(void *uri)
{
	g_free(uri);

	if (gui.view == GUI_VIEW_FILE_LIST) {
		list_check_digests(hash_priv.list_row);

		if (++hash_priv.list_row < list.rows) {
			gtk_progress_bar_set_fraction(gui.progressbar, 0.0);
			gtk_progress_bar_set_text(gui.progressbar, " ");

			// Next file
			hash_file_start(list_get_uri(hash_priv.list_row));
			return;
		} else
			hash_priv.list_row = 0;
	} else {
		// Reset enabled state following single-function hash
		gui_update_hash_funcs();
	}

	gui_check_digests();
	gui_set_state(GUI_STATE_IDLE);
}

void gtkhash_hash_file_stop_cb(void *uri)
{
	g_free(uri);

	if (gui.view == GUI_VIEW_FILE_LIST)
		hash_priv.list_row = 0;
	else {
		// Reset enabled state following single-function hash
		gui_update_hash_funcs();
	}

	gui_set_state(GUI_STATE_IDLE);
}

void hash_file_start(const char *uri)
{
	g_assert(hash_funcs_count_enabled());

	const enum digest_format_e format = gui_get_digest_format();

	size_t key_size = 0;
	const uint8_t *hmac_key = gui_get_hmac_key(&key_size);

	gtkhash_hash_file(hash_priv.hfile, uri, format, hmac_key, key_size, uri);
}

void hash_file_list_start(void)
{
	g_assert(hash_funcs_count_enabled());
	g_assert(!hash_priv.list_row);

	char *uri = list_get_uri(hash_priv.list_row);
	hash_file_start(uri);
}

void hash_file_stop(void)
{
	gtkhash_hash_file_cancel(hash_priv.hfile);
}

void hash_string(void)
{
	if (!hash_funcs_count_enabled())
		return;

	const char *str = gtk_entry_get_text(gui.entry_text);
	const enum digest_format_e format = gui_get_digest_format();

	size_t key_size = 0;
	const uint8_t *hmac_key = gui_get_hmac_key(&key_size);

	gtkhash_hash_string(hash.funcs, str, format, hmac_key, key_size);
}

unsigned int hash_funcs_count_enabled(void)
{
	unsigned int ret = 0;

	for (int i = 0; i < HASH_FUNCS_N; i++) {
		if (hash.funcs[i].enabled)
			ret++;
	}

	return ret;
}

void hash_funcs_enable_strv(const char **funcs)
{
	for (int i = 0; funcs[i]; i++) {
		enum hash_func_e id = gtkhash_hash_func_get_id_from_name(funcs[i]);
		if (HASH_FUNC_IS_VALID(id) && hash.funcs[id].supported)
			hash.funcs[id].enabled = true;
		else
			g_message(_("Unknown Hash Function name \"%s\""), funcs[i]);
	}
}

void hash_init(void)
{
	gtkhash_hash_func_init_all(hash.funcs);

	hash_priv.hfile = gtkhash_hash_file_new(hash.funcs);
}

void hash_deinit(void)
{
	gtkhash_hash_file_free(hash_priv.hfile);
	hash_priv.hfile = NULL;

	gtkhash_hash_func_deinit_all(hash.funcs);
}
