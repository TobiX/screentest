/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  https://tobix.github.io/screentest/
 *  Copyright (C) 2001 Jan "Yenya" Kasprzak <kas@fi.muni.cz>
 *  Copyright (C) 2006-2017 Tobias Gruetzmacher <tobias-screentest@23.gs>
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

#ifndef SCREENTEST_CALLBACKS_H
#define SCREENTEST_CALLBACKS_H

#include <gtk/gtk.h>

struct test_ops {
  void (*init)(GtkWidget *widget);
  void (*draw)(GtkWidget *widget);
  void (*cycle)(GtkWidget *widget);
  void (*close)(GtkWidget *widget);
};

extern struct test_ops basic_ops;

enum test_color {
  COLOR_WHITE,
  COLOR_RED,
  COLOR_GREEN,
  COLOR_BLUE,
  COLOR_CYAN,
  COLOR_MAGENTA,
  COLOR_YELLOW,
  COLOR_BLACK,
  COLOR_MAX
};

#define GRAYS_MAX COLOR_MAX

extern GdkGC *gc, *backgc;
extern GdkColor fgcolors[];
extern GdkColor *fg_color;
extern GdkColor grays[];

#endif // SCREENTEST_CALLBACKS_H
