/*
 *  Screentest - CRT monitor testing utility.
 *  http://www.fi.muni.cz/~kas/screentest/
 *  Copyright (C) 2001 Jan "Yenya" Kasprzak <kas@fi.muni.cz>
 *  Copyright (C) 2006 Tobias Gruetzmacher <tobias@portfolio16.de>
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdio.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define BASIC_STEP 40

static gchar fontname[] =
    "-adobe-helvetica-bold-r-normal-*-14-*-*-*-p-*-iso8859-1";
static GdkFont *font;

static void basic_init(GtkWidget * widget)
{
	font = gdk_font_load(fontname);

	if (!font) {
		printf("Cannot load font %s, trying 'fixed'.\n", fontname);
		font = gdk_font_load("fixed");
	}
}

static void draw_boxes(GdkWindow * win, GdkColor * colors, gint ncols,
		       gint x, gint y, gint d)
{
	int i;

	for (i = 0; i < ncols; i++) {
		gdk_gc_set_foreground(gc, &colors[i]);

		gdk_draw_rectangle(win, gc, TRUE, x, y, d, d);
		x += d;
	}

	gdk_gc_set_foreground(gc, &fgcolors[fg_color]);
}

static void basic_draw(GtkWidget * widget)
{
	GdkWindow *win = widget->window;
	gint w, h;
	gint i, b, d;
	gint maxwidth, maxheight;
	gint widths[6];
	static gchar *text[] = {
		"Screentest v" VERSION,
		"(C) 2001 Jan \"Yenya\" Kasprzak <kas@fi.muni.cz>",
		"(C) 2006 Tobias Gruetzmacher <tobias@portfolio16.de>",
		"Left Button - param cycle, if any",
		"Middle Button - color cycle",
		"Right Button - menu",
	};

	gdk_window_get_size(win, &w, &h);

	for (i = ((w - 1) % BASIC_STEP) / 2; i < w; i += BASIC_STEP)
		gdk_draw_line(win, gc, i, 0, i, h - 1);
	for (i = ((h - 1) % BASIC_STEP) / 2; i < h; i += BASIC_STEP)
		gdk_draw_line(win, gc, 0, i, w - 1, i);

	d = w / 4;
	if (d > h / 4)
		d = h / 4;

	maxheight = 0;
	for (i = 0; i < 5; i++) {
		int x = gdk_string_height(font, text[i]);
		if (x > maxheight)
			maxheight = x;
	}

	maxwidth = 0;
	for (i = 0; i < 6; i++) {
		widths[i] = gdk_string_width(font, text[i]);
		if (widths[i] > maxwidth)
			maxwidth = widths[i];
	}

	maxwidth += 20;
	maxheight = 3 * maxheight / 2;
	gdk_draw_rectangle(win, gc, FALSE, (w - maxwidth) / 2,
			   d / 2 - 2 * maxheight, maxwidth, 4 * maxheight);
	gdk_draw_rectangle(win, gc, FALSE, (w - maxwidth) / 2,
			   h - d / 2 - 2 * maxheight, maxwidth,
			   4 * maxheight);
	gdk_draw_rectangle(win, backgc, TRUE, (w - maxwidth) / 2 + 1,
			   d / 2 - 2 * maxheight + 1, maxwidth - 1,
			   4 * maxheight - 1);
	gdk_draw_rectangle(win, backgc, TRUE, (w - maxwidth) / 2 + 1,
			   h - d / 2 - 2 * maxheight + 1, maxwidth - 1,
			   4 * maxheight - 1);

	gdk_draw_string(win, font, gc, (w - widths[0]) / 2,
			d / 2 - 2 * maxheight / 3, text[0]);
	gdk_draw_string(win, font, gc, (w - widths[1]) / 2,
			d / 2 + maxheight / 3, text[1]);
	gdk_draw_string(win, font, gc, (w - widths[2]) / 2,
			d / 2 + 4 * maxheight / 3, text[2]);

	gdk_draw_string(win, font, gc, (w - widths[3]) / 2,
			h - d / 2 - 2 * maxheight / 3, text[3]);
	gdk_draw_string(win, font, gc, (w - widths[4]) / 2,
			h - d / 2 + maxheight / 3, text[4]);
	gdk_draw_string(win, font, gc, (w - widths[5]) / 2,
			h - d / 2 + 4 * maxheight / 3, text[5]);

	b = 7 * d / 4;
	draw_boxes(win, fgcolors, COLOR_MAX, (w - b) / 2,
		   h / 2 - b / COLOR_MAX, b / COLOR_MAX);
	draw_boxes(win, grays, GRAYS_MAX, (w - b) / 2, h / 2,
		   b / GRAYS_MAX);
	w--;
	h--;
	gdk_draw_arc(win, gc, FALSE, 0, 0, d, d, 0, 360 * 64);
	gdk_draw_arc(win, gc, FALSE, 0, h - d, d, d, 0, 360 * 64);
	gdk_draw_arc(win, gc, FALSE, w - d, h - d, d, d, 0, 360 * 64);
	gdk_draw_arc(win, gc, FALSE, w - d, 0, d, d, 0, 360 * 64);
	gdk_draw_arc(win, gc, FALSE, w / 2 - d, h / 2 - d, d * 2, d * 2, 0,
		     360 * 64);
}

static void basic_close(GtkWidget * widget)
{
	gdk_font_unref(font);
}

struct test_ops basic_ops = {
	init:basic_init,
	draw:basic_draw,
	cycle:NULL,
	close:basic_close
};
