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

#include <config.h>

#include <gtk/gtk.h>

#include "callbacks.h"

#define GRID_STEP 64
#define GRID_STEP_MIN 2

static gint vertical_step;

static void vertical_init(G_GNUC_UNUSED GtkWidget *widget) {
  vertical_step = GRID_STEP;
}

static void vertical_cycle(G_GNUC_UNUSED GtkWidget *widget) {
  vertical_step /= 2;
  if (vertical_step < GRID_STEP_MIN)
    vertical_step = GRID_STEP;
}

static void vertical_draw(GtkWidget *widget) {
  cairo_t *cr;
  GdkWindow *win = gtk_widget_get_window(widget);
  gint w, h;
  gint i;

  h = gdk_window_get_height(win);
  w = gdk_window_get_width(win);

  cr = gdk_cairo_create(gtk_widget_get_window(widget));

  set_color_bg(cr);
  cairo_paint(cr);

  set_color_fg(cr);
  for (i = ((w - 1) % vertical_step) / 2; i < w; i += vertical_step) {
    cairo_rectangle(cr, i, 0, 1, w - 1);
    cairo_fill(cr);
  }

  cairo_destroy(cr);
  cr = NULL;
}

G_MODULE_EXPORT struct test_ops vertical_ops = {.init = vertical_init,
                                                .draw = vertical_draw,
                                                .cycle = vertical_cycle,
                                                .close = NULL};
