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

#include <config.h>

#include <gtk/gtk.h>
#include <stdio.h>

#include "callbacks.h"
#include "gettext.h"
#include "screentest_colors.h"
#define _(String) gettext(String)
#define N_(String) gettext_noop(String)

#define BASIC_STEP 40

static void draw_boxes(cairo_t *cr, const GdkRGBA *colors, gint ncols, gint x,
                       gint y, gint d) {
  const GdkRGBA *col;
  int i;

  for (i = 0; i < ncols; i++) {
    col = &colors[i];
    cairo_set_source_rgb(cr, col->red, col->green, col->blue);

    cairo_rectangle(cr, x, y, d, d);
    cairo_fill(cr);
    x += d;
  }

  screentest_set_color_fg(cr);
}

static void basic_draw(GtkWidget *widget, cairo_t *cr) {
  PangoLayout *pl;
  GdkWindow *win = gtk_widget_get_window(widget);
  gint w, h;
  gint i, b, d;
  PangoRectangle ink_rect;
  gint maxwidth, maxheight;
  gint widths[7];
  static gchar *text[] = {
      "Screentest v" VERSION,
      "(C) 2001 Jan \"Yenya\" Kasprzak <kas@fi.muni.cz>",
      "(C) 2006-2017 Tobias Gruetzmacher <tobias-screentest@23.gs>",
      "(C) 2021 Apr Thorsten Kattanek <thorsten.kattanek@gmx.de>",
      N_("Left Button - param cycle, if any"),
      N_("Middle Button - color cycle"),
      N_("Right Button - menu"),
  };

  h = gdk_window_get_height(win);
  w = gdk_window_get_width(win);

  cairo_set_line_width(cr, 1.0);

  pl = pango_cairo_create_layout(cr);

  screentest_set_color_bg(cr);
  cairo_paint(cr);

  screentest_set_color_fg(cr);

  for (i = ((w - 1) % BASIC_STEP) / 2; i < w; i += BASIC_STEP)
    cairo_rectangle(cr, i, 0, 1, h);
  for (i = ((h - 1) % BASIC_STEP) / 2; i < h; i += BASIC_STEP)
    cairo_rectangle(cr, 0, i, w, 1);
  cairo_fill(cr);

  d = w / 4;
  if (d > h / 4)
    d = h / 4;

  maxheight = 0;
  maxwidth = 0;
  for (i = 0; i < 7; i++) {
    pango_layout_set_text(pl, gettext(text[i]), -1);
    pango_layout_get_extents(pl, &ink_rect, NULL);
    double x = pango_units_to_double(ink_rect.height);
    if (x > maxheight)
      maxheight = x;
    widths[i] = pango_units_to_double(ink_rect.width);
    if (widths[i] > maxwidth)
      maxwidth = widths[i];
  }

  maxwidth += 20;
  maxheight = 3 * maxheight / 2;

  cairo_rectangle(cr, (w - maxwidth) / 2 + 0.5, d / 2 - 2 * maxheight + 0.5,
                  maxwidth, 5 * maxheight);
  cairo_rectangle(cr, (w - maxwidth) / 2 + 0.5, h - d / 2 - 2 * maxheight + 0.5,
                  maxwidth, 4 * maxheight);
  cairo_stroke(cr);

  screentest_set_color_bg(cr);
  cairo_rectangle(cr, (w - maxwidth) / 2 + 1, d / 2 - 2 * maxheight + 1,
                  maxwidth - 1, 5 * maxheight - 1);
  cairo_rectangle(cr, (w - maxwidth) / 2 + 1, h - d / 2 - 2 * maxheight + 1,
                  maxwidth - 1, 4 * maxheight - 1);
  cairo_fill(cr);

  screentest_set_color_fg(cr);

  cairo_move_to(cr, (w - widths[0]) / 2, d / 2 - 4 * maxheight / 3);
  pango_layout_set_text(pl, gettext(text[0]), -1);
  pango_cairo_show_layout(cr, pl);
  cairo_move_to(cr, (w - widths[1]) / 2, d / 2 - maxheight / 3);
  pango_layout_set_text(pl, gettext(text[1]), -1);
  pango_cairo_show_layout(cr, pl);
  cairo_move_to(cr, (w - widths[2]) / 2, d / 2 + 2 * maxheight / 3);
  pango_layout_set_text(pl, gettext(text[2]), -1);
  pango_cairo_show_layout(cr, pl);
  cairo_move_to(cr, (w - widths[3]) / 2, d / 2 + 5 * maxheight / 3);
  pango_layout_set_text(pl, gettext(text[3]), -1);
  pango_cairo_show_layout(cr, pl);
  cairo_move_to(cr, (w - widths[4]) / 2, h - d / 2 - 4 * maxheight / 3);
  pango_layout_set_text(pl, gettext(text[4]), -1);
  pango_cairo_show_layout(cr, pl);
  cairo_move_to(cr, (w - widths[5]) / 2, h - d / 2 - maxheight / 3);
  pango_layout_set_text(pl, gettext(text[5]), -1);
  pango_cairo_show_layout(cr, pl);
  cairo_move_to(cr, (w - widths[6]) / 2, h - d / 2 + 2 * maxheight / 3);
  pango_layout_set_text(pl, gettext(text[6]), -1);
  pango_cairo_show_layout(cr, pl);

  b = 7 * d / 4;
  draw_boxes(cr, fgcolors, SCREENTEST_COLORS_MAX, (w - b) / 2,
             h / 2 - b / SCREENTEST_COLORS_MAX, b / SCREENTEST_COLORS_MAX);
  draw_boxes(cr, grays, SCREENTEST_GRAYS_MAX, (w - b) / 2, h / 2,
             b / SCREENTEST_GRAYS_MAX);

  cairo_arc(cr, 0 + d / 2 + 0.5, 0 + d / 2 + 0.5, d / 2, 0,
            2 * G_PI); // Upper left
  cairo_new_sub_path(cr);
  cairo_arc(cr, 0 + d / 2 + 0.5, h - d / 2 - 0.5, d / 2, 0,
            2 * G_PI); // Lower left
  cairo_new_sub_path(cr);
  cairo_arc(cr, w - d / 2 - 0.5, h - d / 2 - 0.5, d / 2, 0,
            2 * G_PI); // Lower right
  cairo_new_sub_path(cr);
  cairo_arc(cr, w - d / 2 - 0.5, 0 + d / 2 + 0.5, d / 2, 0,
            2 * G_PI); // Upper right
  cairo_new_sub_path(cr);
  cairo_arc(cr, w / 2, h / 2, d, 0, 2 * G_PI);
  cairo_stroke(cr);
}

G_MODULE_EXPORT struct test_ops basic_ops = {
    .init = NULL, .draw = basic_draw, .cycle = NULL, .close = NULL};
