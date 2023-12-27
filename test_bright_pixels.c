/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  https://tobix.github.io/screentest/
 *  Copyright (C) 2011-2017 Tobias Gruetzmacher <tobias-screentest@23.gs>
 *  Copyright (C) 2021 Apr Thorsten Kattanek <thorsten.kattanek@gmx.de>
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

#include <config.h>

#include <gtk/gtk.h>
#include <stdio.h>

#include "callbacks.h"

#define COLOR_COUNT 5
static const int color_cycle[COLOR_COUNT] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE,
                                             COLOR_WHITE, COLOR_BLACK};
static int current_color_idx;

static void bright_pixels_init(G_GNUC_UNUSED GtkWidget *widget) {
  current_color_idx = 0;
}

static void bright_pixels_cycle(G_GNUC_UNUSED GtkWidget *widget) {
  current_color_idx++;
  if (current_color_idx == COLOR_COUNT)
    current_color_idx = 0;
}

static void bright_pixels_draw(GtkWidget *widget) {
  GdkColor *col;
  cairo_t *cr;

  cr = gdk_cairo_create(gtk_widget_get_window(widget));

  col = &fgcolors[color_cycle[current_color_idx]];
  cairo_set_source_rgb(cr, col->red / (double)UINT16_MAX,
                       col->green / (double)UINT16_MAX,
                       col->blue / (double)UINT16_MAX);
  cairo_paint(cr);

  cairo_destroy(cr);
  cr = NULL;
}

G_MODULE_EXPORT struct test_ops bright_pixels_ops = {.init = bright_pixels_init,
                                                     .draw = bright_pixels_draw,
                                                     .cycle =
                                                         bright_pixels_cycle,
                                                     .close = NULL};
