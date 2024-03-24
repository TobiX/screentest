/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  https://tobix.github.io/screentest/
 *  Copyright (C) 2011-2017 Tobias Gruetzmacher <tobias-screentest@23.gs>
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

static void lcdalign_draw(GtkWidget *widget, cairo_t *cr) {
  GdkWindow *win = gtk_widget_get_window(widget);
  gint w, h;
  gint i;
  static const double d[] = {1.0};

  h = gdk_window_get_height(win);
  w = gdk_window_get_width(win);

  /* Background/Border */
  set_color_fg(cr);
  cairo_paint(cr);

  /* Pattern */
  set_color_bg(cr);
  cairo_set_line_width(cr, 1.0);
  cairo_set_dash(cr, d, 1, 0);
  for (i = 1; i < h - 1; i += 2) {
    cairo_move_to(cr, 1, i + 0.5);
    cairo_line_to(cr, w - 1, i + 0.5);
  }
  cairo_stroke(cr);
  cairo_set_dash(cr, d, 1, 1);
  for (i = 2; i < h - 1; i += 2) {
    cairo_move_to(cr, 1, i + 0.5);
    cairo_line_to(cr, w - 1, i + 0.5);
  }
  cairo_stroke(cr);
}

G_MODULE_EXPORT struct test_ops lcdalign_ops = {
    .init = NULL, .draw = lcdalign_draw, .cycle = NULL, .close = NULL};
