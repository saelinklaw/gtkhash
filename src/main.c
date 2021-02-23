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
#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include "main.h"
#include "opts.h"
#include "hash.h"
#include "gui.h"
#include "prefs.h"
#include "check.h"

#if ENABLE_NLS
static void nls_init(void)
{
#ifdef G_OS_WIN32
	char *pkgdir = g_win32_get_package_installation_directory_of_module(NULL);
	char *localedir = g_build_filename(pkgdir, "share", "locale", NULL);
	bindtextdomain(GETTEXT_PACKAGE, localedir);
	g_free(localedir);
	g_free(pkgdir);
#else
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
#endif

	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
}
#endif

int main(int argc, char **argv)
{
#if ENABLE_NLS
	nls_init();
#endif

	hash_init();

	opts_preinit(&argc, &argv);

	gtk_init(NULL, NULL);

	gui_init();
	prefs_init();
	check_init();

	opts_postinit();

	gui_run();

	check_deinit();
	prefs_deinit();
	gui_deinit();
	hash_deinit();

	return EXIT_SUCCESS;
}
