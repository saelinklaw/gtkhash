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

#ifndef GTKHASH_NAUTILUS_PROPERTIES_H
#define GTKHASH_NAUTILUS_PROPERTIES_H

#include <gtk/gtk.h>

#include "../hash/hash-file.h"

#if ENABLE_NLS
	#include <glib/gi18n-lib.h>
#else
	#define _(X) (X)
#endif

struct page_s {
	GSettings *settings;
	char *uri;
	GtkWidget *box, *hbox_inputs;
	GtkProgressBar *progressbar;
	GtkTreeView *treeview;
	GtkTreeSelection *treeselection;
	GtkCellRendererToggle *cellrendtoggle;
	GtkMenu *menu;
	GtkMenuItem *menuitem_copy;
	GtkCheckMenuItem *menuitem_show_funcs;
	GtkToggleButton *togglebutton_hmac;
	GtkEntry *entry_check, *entry_hmac;
	GtkButton *button_hash, *button_stop;
	struct hash_file_s *hfile;
	struct hash_func_s funcs[HASH_FUNCS_N];
	bool busy;
};

void gtkhash_properties_idle(struct page_s *page);

#endif
