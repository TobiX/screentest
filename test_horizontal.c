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

static gint horizontal_step;

static void horizontal_init(G_GNUC_UNUSED GtkWidget *widget) {
  horizontal_step = GRID_STEP;
}

static void horizontal_cycle(G_GNUC_UNUSED GtkWidget *widget) {
  horizontal_step /= 2;
  if (horizontal_step < GRID_STEP_MIN)
    horizontal_step = GRID_STEP;
}

static void horizontal_draw(GtkWidget *widget) {
  GdkWindow *win = gtk_widget_get_window(widget);
  gint w, h;
  gint i;
  gint d;

  h = gdk_window_get_height(win);
  w = gdk_window_get_width(win);

  d = w / 4;
  if (d > h / 4)
    d = h / 4;

  for (i = ((h - 1) % horizontal_step) / 2; i < h; i += horizontal_step)
    gdk_draw_line(win, gc, 0, i, w - 1, i);
}

G_MODULE_EXPORT struct test_ops horizontal_ops = {.init = horizontal_init,
                                                  .draw = horizontal_draw,
                                                  .cycle = horizontal_cycle,
                                                  .close = NULL};
