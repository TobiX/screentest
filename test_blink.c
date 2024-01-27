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

static guint timeout;

static gint blink_type;
static gint blink_step;

void (*set_color1)(cairo_t *cr);
void (*set_color2)(cairo_t *cr);

static void blink_draw(GtkWidget *widget, cairo_t *cr) {
  GdkWindow *win = gtk_widget_get_window(widget);
  gint w, h;

  h = gdk_window_get_height(win);
  w = gdk_window_get_width(win);

  if (blink_step) {
    set_color1 = set_color_bg;
    set_color2 = set_color_fg;
  } else {
    set_color1 = set_color_fg;
    set_color2 = set_color_bg;
  }

  set_color_fg(cr);
  cairo_paint(cr);

  set_color_bg(cr);
  cairo_rectangle(cr, 1, 1, w - 2, h - 2);
  cairo_fill(cr);

  if (blink_type) {
    set_color1(cr);
    cairo_rectangle(cr, 5, 5, w / 3 - 5, h - 10);
    cairo_fill(cr);
    set_color2(cr);
    cairo_rectangle(cr, w / 3, 5, w / 3, h - 10);
    cairo_fill(cr);
    set_color1(cr);
    cairo_rectangle(cr, 2 * w / 3, 5, w / 3 - 5, h - 10);
    cairo_fill(cr);
  } else {
    set_color1(cr);
    cairo_rectangle(cr, 5, 5, w - 10, h / 3 - 5);
    cairo_fill(cr);
    set_color2(cr);
    cairo_rectangle(cr, 5, h / 3, w - 10, h / 3);
    cairo_fill(cr);
    set_color1(cr);
    cairo_rectangle(cr, 5, 2 * h / 3, w - 10, h / 3 - 5);
    cairo_fill(cr);
  }
}

static gboolean blink_timeout(gpointer data) {
  GtkWidget *widget = (GtkWidget *)data;
  blink_step = !blink_step;

  gtk_widget_queue_draw(widget);

  return TRUE;
}

static void blink_init(GtkWidget *widget) {
  blink_type = 0;
  blink_step = 0;
  timeout = g_timeout_add(1000, blink_timeout, widget);
}

void blink_cycle(G_GNUC_UNUSED GtkWidget *widget) { blink_type = !blink_type; }

static void blink_close(G_GNUC_UNUSED GtkWidget *widget) {
  g_source_remove(timeout);
}

G_MODULE_EXPORT struct test_ops blink_ops = {.init = blink_init,
                                             .draw = blink_draw,
                                             .cycle = blink_cycle,
                                             .close = blink_close};
