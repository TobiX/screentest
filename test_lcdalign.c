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

static void lcdalign_draw(GtkWidget *widget) {
  GdkWindow *win = gtk_widget_get_window(widget);
  GdkGC *linegc = gdk_gc_new(win);
  gint w, h;
  gint i;
  gint8 d[] = {1, 1};

  h = gdk_window_get_height(win);
  w = gdk_window_get_width(win);

  /* Border */
  gdk_draw_line(win, gc, 0, 0, w - 1, 0);
  gdk_draw_line(win, gc, 0, 0, 0, h - 1);
  gdk_draw_line(win, gc, w - 1, 0, w - 1, h - 1);
  gdk_draw_line(win, gc, 0, h - 1, w - 1, h - 1);

  /* Pattern */
  gdk_gc_copy(linegc, gc);
  gdk_gc_set_line_attributes(linegc, 1, GDK_LINE_ON_OFF_DASH, GDK_CAP_NOT_LAST,
                             GDK_JOIN_MITER);
  gdk_gc_set_dashes(linegc, 0, d, 2);
  for (i = 1; i < h - 1; i++)
    gdk_draw_line(win, linegc, (i % 2) + 1, i, w - 1, i);

  g_object_unref(linegc);
}

G_MODULE_EXPORT struct test_ops lcdalign_ops = {
    .init = NULL, .draw = lcdalign_draw, .cycle = NULL, .close = NULL};
