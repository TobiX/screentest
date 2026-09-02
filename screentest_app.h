/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  https://tobix.github.io/screentest/
 *  Copyright (C) 2025 Markus Prasser
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef SCREENTEST_APP_H
#define SCREENTEST_APP_H

#include <gtk/gtk.h>

#define SCREENTEST_APP_TYPE (screentest_app_get_type())
G_DECLARE_FINAL_TYPE(ScreentestApp, screentest_app, SCREENTEST, APP,
                     GtkApplication)

ScreentestApp *screentest_app_new(void);

#endif // SCREENTEST_APP_H
