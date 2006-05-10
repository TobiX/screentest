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
#include <stdlib.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

GdkColor fgcolors[COLOR_MAX];
gint fg_color = COLOR_WHITE, bg_color = COLOR_BLACK;
GdkColor grays[GRAYS_MAX];
GdkGC *gc, *backgc;

static GtkWidget *popup = NULL;
static GtkWidget *area = NULL;
static struct test_ops *current_test = &basic_ops;

static void get_color(GdkColormap * cmap, gint num, gchar * name)
{
	if (!gdk_color_parse(name, &fgcolors[num])) {
		printf("Cannot find color %s\n", name);
		exit(1);
	}
	if (!gdk_colormap_alloc_color(cmap, &fgcolors[num], FALSE, TRUE)) {
		printf("Cannot allocate color %s - trying non-exact.\n",
		       name);
		if (!gdk_colormap_alloc_color(cmap, &fgcolors[num], FALSE,
					      FALSE)) {
			printf("Even non-exact alloc failed (%s)!\n",
			       name);
			exit(1);
		}
	}
}

static void get_gray(GdkColormap * cmap, gint num)
{
	GdkColor *col = &grays[num];

	col->red = col->green = col->blue =
	    num * ((1 << 16) - 1) / (GRAYS_MAX - 1);
	if (!gdk_colormap_alloc_color(cmap, col, FALSE, TRUE)) {
		printf("Cannot allocate gray %d - trying non-exact.\n",
		       num);
		if (!gdk_colormap_alloc_color(cmap, col, FALSE, FALSE)) {
			printf("Even non-exact alloc failed (%d)!\n", num);
			exit(1);
		}
	}
}

void on_mainwin_realize(GtkWidget * widget, gpointer user_data)
{
	gint i;
	GdkColormap *cmap = gdk_colormap_get_system();
	GdkGCValues gcv;

#ifdef DEBUG
	gdk_window_resize(widget->window, 800, 600);
#else
	gtk_window_fullscreen(GTK_WINDOW(widget));
#endif

	gdk_color_white(cmap, &fgcolors[COLOR_WHITE]);
	gdk_color_black(cmap, &fgcolors[COLOR_BLACK]);

	get_color(cmap, COLOR_RED, "#ff0000");
	get_color(cmap, COLOR_GREEN, "#00ff00");
	get_color(cmap, COLOR_BLUE, "#0000ff");
	get_color(cmap, COLOR_CYAN, "#00ffff");
	get_color(cmap, COLOR_MAGENTA, "#ff00ff");
	get_color(cmap, COLOR_YELLOW, "#ffff00");

	for (i = 0; i < GRAYS_MAX; i++)
		get_gray(cmap, i);

	area = widget;

	gcv.foreground = fgcolors[COLOR_WHITE];
	gcv.background = fgcolors[COLOR_BLACK];
	gcv.function = GDK_COPY;

	gc = gdk_gc_new_with_values(area->window, &gcv, GDK_GC_FOREGROUND
				    | GDK_GC_BACKGROUND | GDK_GC_FUNCTION);

	gcv.foreground = fgcolors[COLOR_BLACK];
	gcv.background = fgcolors[COLOR_WHITE];
	backgc = gdk_gc_new_with_values(area->window, &gcv, GDK_GC_FOREGROUND
					| GDK_GC_BACKGROUND |
					GDK_GC_FUNCTION);

	if (current_test->init != NULL)
		current_test->init(widget);
}

static void change_color(gint num)
{
	if (num >= COLOR_MAX || num < 0)
		return;
	gdk_gc_set_foreground(gc, &fgcolors[num]);
	gdk_gc_set_background(backgc, &fgcolors[num]);
	fg_color = num;

	gdk_window_invalidate_rect(area->window, NULL, FALSE);
}

static void change_bgcolor(gint num)
{
	if (num >= COLOR_MAX || num < 0)
		return;
	gdk_gc_set_foreground(backgc, &fgcolors[num]);
	gdk_gc_set_background(gc, &fgcolors[num]);
	bg_color = num;

	gdk_window_invalidate_rect(area->window, NULL, FALSE);
}

gboolean
on_mainwin_button_press_event(GtkWidget * widget,
			   GdkEventButton * event, gpointer user_data)
{
	switch (event->button) {
	case 1:
		if (current_test->cycle != NULL) {
			current_test->cycle(widget);
			gdk_window_invalidate_rect(area->window, NULL, FALSE);
		}
		break;
	case 2:
		if (++fg_color >= COLOR_MAX)
			fg_color = 0;
		change_color(fg_color);
		break;
	case 3:
		if (popup == NULL)
			popup = create_popup();
		gtk_menu_popup(GTK_MENU(popup), NULL, NULL, NULL, NULL,
			       event->button, event->time);
		break;
	}
	return TRUE;
}


/*
 * FIXME: GTK_WINDOW_POPUP does not seem to be able to receive
 * KeyPress events. How to enable this?
 */
gboolean
on_mainwin_key_press_event(GtkWidget * widget,
			GdkEventKey * event, gpointer user_data)
{
	gtk_main_quit();
	return FALSE;
}

gboolean
on_mainwin_expose_event(GtkWidget * widget,
		     GdkEventExpose * event, gpointer user_data)
{
	gdk_window_set_background(area->window, &fgcolors[bg_color]);
	gdk_window_clear(area->window);

	if (current_test->draw != NULL)
		current_test->draw(widget);
	return TRUE;
}

void on_color_change(GtkMenuItem * menuitem, gpointer user_data)
{
	GtkCheckMenuItem *checkmenuitem = GTK_CHECK_MENU_ITEM(menuitem);
	if (!checkmenuitem->active)
		return;

	change_color((gint) user_data);
}

void on_bgcolor_change(GtkMenuItem * menuitem, gpointer user_data)
{
	GtkCheckMenuItem *checkmenuitem = GTK_CHECK_MENU_ITEM(menuitem);
	if (!checkmenuitem->active)
		return;

	change_bgcolor((gint) user_data);
}

void on_mode_change(GtkMenuItem * menuitem, gpointer user_data)
{
	GtkCheckMenuItem *checkmenuitem = GTK_CHECK_MENU_ITEM(menuitem);

	gint mode = (gint) user_data;

	if (!checkmenuitem->active) {
		if (current_test->close != NULL) {
			current_test->close(area);
			current_test = NULL;
		}
	} else {
		switch (mode) {
		case MODE_BASIC:
			current_test = &basic_ops;
			break;
		case MODE_GRID:
			current_test = &grid_ops;
			break;
		case MODE_HORIZONTAL:
			current_test = &horizontal_ops;
			break;
		case MODE_VERTICAL:
			current_test = &vertical_ops;
			break;
		case MODE_BLINK:
			current_test = &blink_ops;
			break;
		case MODE_TEXT:
			current_test = &text_ops;
			break;
		}
		if (current_test->init != NULL) {
			current_test->init(area);
		}
		on_mainwin_expose_event(area, NULL, NULL);
	}
}

