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

static gchar *fontnames[] = {
    "-adobe-times-medium-r-normal-*-*-80-*-*-p-*-iso8859-1",
    "-adobe-times-medium-r-normal-*-*-100-*-*-p-*-iso8859-1",
    "-adobe-times-medium-r-normal-*-*-120-*-*-p-*-iso8859-1",
    "-adobe-times-medium-r-normal-*-*-140-*-*-p-*-iso8859-1",
    "-adobe-times-medium-r-normal-*-*-180-*-*-p-*-iso8859-1",
    "-adobe-times-medium-r-normal-*-*-240-*-*-p-*-iso8859-1",
    NULL};

static gint font_num;

static gchar text[] =
    "The Screentest home page: https://tobix.github.io/screentest/";

GdkFont *font;
gint baselineskip, textwidth;

static void font_init(G_GNUC_UNUSED GtkWidget *widget) {
  gint lbear, rbear, width, asc, desc;

  font = gdk_font_load(fontnames[font_num]);

  if (!font) {
    printf("Cannot load font %s, trying 'fixed'.\n", fontnames[font_num]);
    font = gdk_font_load("fixed");
  }

  gdk_string_extents(font, text, &lbear, &rbear, &width, &asc, &desc);
  baselineskip = 6 * (asc + desc) / 5; /* 1.2x text height */
  textwidth = width + baselineskip;    /* the actual width plus space */
}

static void text_init(GtkWidget *widget) {
  font_num = 1;
  font_init(widget);
}

static void text_draw(GtkWidget *widget) {
  GdkWindow *win = gtk_widget_get_window(widget);
  gint w, h;
  gint x, y;

  gdk_drawable_get_size(win, &w, &h);

  x = w + textwidth;
  for (y = 0; y < h; y += baselineskip)
    for (x -= w + textwidth; x < w; x += textwidth)
      gdk_draw_string(win, font, gc, x, y, text);
}

static void text_close(G_GNUC_UNUSED GtkWidget *widget) {
  gdk_font_unref(font);
}

static void text_cycle(GtkWidget *widget) {
  gdk_font_unref(font);
  if (fontnames[++font_num] == NULL)
    font_num = 0;
  font_init(widget);
}

G_MODULE_EXPORT struct test_ops text_ops = {
    .init = text_init,
    .draw = text_draw,
    .cycle = text_cycle,
    .close = text_close,
};
