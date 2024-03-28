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
#include <stdio.h>

#include "callbacks.h"
#include "screentest_colors.h"

static gint font_sizes[] = {8, 10, 12, 14, 18, 24};

static gint font_size_num;

static gchar text[] =
    "The Screentest home page: https://tobix.github.io/screentest/";

static void text_init(GtkWidget *widget) { font_size_num = 1; }

static void text_draw(GtkWidget *widget, cairo_t *cr) {
  PangoFontDescription *pft;
  PangoLayout *pl;
  GdkWindow *win = gtk_widget_get_window(widget);
  GString *dtext;
  gint i;
  PangoRectangle ink_rect;
  gint w, h;
  gint repetitions;

  h = gdk_window_get_height(win);
  w = gdk_window_get_width(win);

  cairo_set_line_width(cr, 1.0);

  pft = pango_font_description_new();
  pango_font_description_set_size(
      pft, pango_units_from_double(font_sizes[font_size_num]));

  pl = pango_cairo_create_layout(cr);
  pango_layout_set_font_description(pl, pft);
  pango_layout_set_justify(pl, TRUE);
  pango_layout_set_width(pl, pango_units_from_double(w));

  screentest_set_color_bg(cr);
  cairo_paint(cr);

  screentest_set_color_fg(cr);

  pango_layout_set_text(pl, text, -1);
  pango_layout_get_extents(pl, &ink_rect, NULL);
  repetitions = (w * h) / (pango_units_to_double(ink_rect.width) *
                           pango_units_to_double(ink_rect.height));

  dtext = g_string_sized_new(repetitions * (strlen(text) + 1));
  dtext = g_string_append(dtext, text);
  for (i = 0; i < repetitions; ++i) {
    dtext = g_string_append_c(dtext, ' ');
    dtext = g_string_append(dtext, text);
  }
  pango_layout_set_text(pl, dtext->str, -1);
  pango_cairo_show_layout(cr, pl);

  pango_font_description_free(pft);
  pft = NULL;
}

static void text_cycle(GtkWidget *widget) {
  if (++font_size_num == (sizeof(font_sizes) / sizeof(font_sizes[0])))
    font_size_num = 0;
}

G_MODULE_EXPORT struct test_ops text_ops = {
    .init = text_init,
    .draw = text_draw,
    .cycle = text_cycle,
    .close = NULL,
};
