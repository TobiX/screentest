/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  https://tobix.github.io/screentest/
 *  Copyright (C) 2001 Jan "Yenya" Kasprzak <kas@fi.muni.cz>
 *  Copyright (C) 2006-2017 Tobias Gruetzmacher <tobias-screentest@23.gs>
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

#include "callbacks.h"

enum GRADIENT_ORIENTATION {
  GO_TOP_LEFT_TO_RIGHT_BOTTOM,
  GO_TOP_TO_BOTTOM,
  GO_TOP_RIGHT_TO_LEFT_BOTTOM,
  GO_RIGHT_TO_LEFT,
  GO_BOTTOM_RIGHT_TO_TOP_LEFT,
  GO_BOTTOM_TO_TOP,
  GO_BOTTOM_LEFT_TO_TOP_RIGHT,
  GO_LEFT_TO_RIGHT,
  GO_GRADIENT_ORIENTATIONS_QUANTITY,
};

static enum GRADIENT_ORIENTATION gradient_orientation;

static void gradient_cycle(G_GNUC_UNUSED GtkWidget *widget) {
  gradient_orientation =
      (gradient_orientation + 1) % GO_GRADIENT_ORIENTATIONS_QUANTITY;
}

static void gradient_draw(GtkWidget *widget, cairo_t *cr) {
  GdkWindow *win;
  cairo_pattern_t *pat;
  gint w, h;

  win = gtk_widget_get_window(widget);

  h = gdk_window_get_height(win);
  w = gdk_window_get_width(win);

  switch (gradient_orientation) {
  default:
  case GO_TOP_LEFT_TO_RIGHT_BOTTOM:
    pat = cairo_pattern_create_linear(0.0, 0.0, w, h);
    break;
  case GO_TOP_TO_BOTTOM:
    pat = cairo_pattern_create_linear((float)w / 2, 0.0, (float)w / 2, h);
    break;
  case GO_TOP_RIGHT_TO_LEFT_BOTTOM:
    pat = cairo_pattern_create_linear(w, 0.0, 0.0, h);
    break;
  case GO_RIGHT_TO_LEFT:
    pat = cairo_pattern_create_linear(w, (float)h / 2, 0.0, (float)h / 2);
    break;
  case GO_BOTTOM_RIGHT_TO_TOP_LEFT:
    pat = cairo_pattern_create_linear(w, h, 0.0, 0.0);
    break;
  case GO_BOTTOM_TO_TOP:
    pat = cairo_pattern_create_linear((float)w / 2, h, (float)w / 2, 0.0);
    break;
  case GO_BOTTOM_LEFT_TO_TOP_RIGHT:
    pat = cairo_pattern_create_linear(0.0, h, w, 0.0);
    break;
  case GO_LEFT_TO_RIGHT:
    pat = cairo_pattern_create_linear(0.0, (float)h / 2, w, (float)h / 2);
    break;
  }

  // Let the gradient start with the foreground color ...
  cairo_pattern_add_color_stop_rgba(pat, 0.0, fg_color->red, fg_color->green,
                                    fg_color->blue, fg_color->alpha);
  // ... and end with the background color
  cairo_pattern_add_color_stop_rgba(pat, 1.0, bg_color->red, bg_color->green,
                                    bg_color->blue, bg_color->alpha);
  cairo_rectangle(cr, 0.0, 0.0, w, h);

  // Draw the gradient
  cairo_set_source(cr, pat);
  cairo_fill(cr);

  cairo_pattern_destroy(pat);
  pat = NULL;
}

static void gradient_init(G_GNUC_UNUSED GtkWidget *widget) {
  gradient_orientation = GO_TOP_LEFT_TO_RIGHT_BOTTOM;
}

G_MODULE_EXPORT struct test_ops gradient_ops = {.init = gradient_init,
                                                .draw = gradient_draw,
                                                .cycle = gradient_cycle,
                                                .close = NULL};
