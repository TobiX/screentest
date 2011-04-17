/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  http://screentest.sourceforge.net/
 *  Copyright (C) 2001 Jan "Yenya" Kasprzak <kas@fi.muni.cz>
 *  Copyright (C) 2006-2011 Tobias Gruetzmacher <tobias-screentest@23.gs>
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

#include "callbacks.h"

#define GRID_STEP 64
#define GRID_STEP_MIN 2

static gint grid_step;

static void grid_init(G_GNUC_UNUSED GtkWidget * widget)
{
	grid_step = GRID_STEP;
}

static void grid_cycle(G_GNUC_UNUSED GtkWidget * widget)
{
	grid_step /= 2;
	if (grid_step < GRID_STEP_MIN)
		grid_step = GRID_STEP;
}

static void grid_draw(GtkWidget * widget)
{
	GdkWindow *win = gtk_widget_get_window(widget);
	gint w, h;
	gint i;
	gint d;

	gdk_drawable_get_size(win, &w, &h);

	d = w / 4;
	if (d > h / 4)
		d = h / 4;

	for (i = ((w - 1) % grid_step) / 2; i < w; i += grid_step)
		gdk_draw_line(win, gc, i, 0, i, h - 1);
	for (i = ((h - 1) % grid_step) / 2; i < h; i += grid_step)
		gdk_draw_line(win, gc, 0, i, w - 1, i);
}

G_MODULE_EXPORT struct test_ops grid_ops = {
	init:grid_init,
	draw:grid_draw,
	cycle:grid_cycle,
	close:NULL
};
