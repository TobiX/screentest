/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  http://screentest.sourceforge.net/
 *  Copyright (C) 2001 Jan "Yenya" Kasprzak <kas@fi.muni.cz>
 *  Copyright (C) 2006-2007 Tobias Gruetzmacher <tobias@portfolio16.de>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

#include "callbacks.h"
#include "main.h"

GdkColor fgcolors[COLOR_MAX];
GdkColor *fg_color, *bg_color;
GdkColor grays[GRAYS_MAX];
GdkGC *gc, *backgc;
int fg_count = COLOR_WHITE;

static GtkWidget *mainwin = NULL;
static struct test_ops *current_test = &basic_ops;

void on_mainwin_realize(GtkWidget * widget, gpointer user_data)
{
	gint i;

#ifdef DEBUG
	gdk_window_resize(widget->window, 800, 600);
#else
	gtk_window_fullscreen(GTK_WINDOW(widget));
#endif

	memset(fgcolors, 0, COLOR_MAX * sizeof(GdkColor));

	fgcolors[COLOR_WHITE].red = fgcolors[COLOR_WHITE].green =
		fgcolors[COLOR_WHITE].blue = 65535;

	fgcolors[COLOR_RED].red = 65535;

	fgcolors[COLOR_GREEN].green = 65535;

	fgcolors[COLOR_BLUE].blue = 65535;

	fgcolors[COLOR_CYAN].green = 65535;
	fgcolors[COLOR_CYAN].blue = 65535;

	fgcolors[COLOR_MAGENTA].red = 65535;
	fgcolors[COLOR_MAGENTA].blue = 65535;

	fgcolors[COLOR_YELLOW].red = 65535;
	fgcolors[COLOR_YELLOW].green = 65535;

	/* COLOR_BLACK is 0 already */

	fg_color = gdk_color_copy(&fgcolors[COLOR_WHITE]);
	bg_color = gdk_color_copy(&fgcolors[COLOR_BLACK]);

	for (i = 0; i < GRAYS_MAX; i++)
		grays[i].red = grays[i].green = grays[i].blue = i *
			((1 << 16) - 1) / (GRAYS_MAX - 1);

	mainwin = widget;

	gc = gdk_gc_new(mainwin->window);
	backgc = gdk_gc_new(mainwin->window);

	if (current_test->init != NULL)
		current_test->init(widget);
}

static void update_fg_color(void)
{
	gdk_gc_set_rgb_fg_color(gc, fg_color);
	gdk_gc_set_rgb_bg_color(backgc, fg_color);

	gdk_window_invalidate_rect(mainwin->window, NULL, FALSE);
}

static void update_bg_color(void)
{
	gdk_rgb_find_color(gtk_widget_get_colormap(GTK_WIDGET(mainwin)), bg_color);
	gdk_gc_set_rgb_bg_color(gc, bg_color);
	gdk_gc_set_rgb_fg_color(backgc, bg_color);

	gdk_window_invalidate_rect(mainwin->window, NULL, FALSE);
}

gboolean
on_mainwin_button_press_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	GtkWidget *popup;

	switch (event->button) {
	case 1:
		if (current_test->cycle != NULL) {
			current_test->cycle(widget);
			gdk_window_invalidate_rect(mainwin->window, NULL, FALSE);
		}
		break;
	case 2:
		if (++fg_count >= COLOR_MAX)
			fg_count = COLOR_WHITE;
		gdk_color_free(fg_color);
		fg_color = gdk_color_copy(&fgcolors[fg_count]);
		update_fg_color();
		break;
	case 3:
		popup = glade_xml_get_widget(glade, "popup");
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
on_mainwin_key_press_event(GtkWidget *widget, GdkEventKey *event,
		gpointer user_data)
{
	gtk_main_quit();
	return FALSE;
}

gboolean
on_mainwin_expose_event(GtkWidget *widget, GdkEventExpose *event,
		gpointer user_data)
{
	gdk_window_set_background(widget->window, bg_color);
	gdk_window_clear(widget->window);

	if (current_test->draw != NULL)
		current_test->draw(widget);
	return TRUE;
}

void on_mode_change(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkCheckMenuItem *checkmenuitem = GTK_CHECK_MENU_ITEM(menuitem);

	if (!checkmenuitem->active) {
		if (current_test->close != NULL) {
			current_test->close(mainwin);
			current_test = NULL;
		}
	} else {
		const char *mode = glade_get_widget_name(GTK_WIDGET(menuitem));
		if (strcmp(mode, "basic") == 0)
			current_test = &basic_ops;
		else if (strcmp(mode, "grid") == 0)
			current_test = &grid_ops;
		else if (strcmp(mode, "horizontal") == 0)
			current_test = &horizontal_ops;
		else if (strcmp(mode, "vertical") == 0)
			current_test = &vertical_ops;
		else if (strcmp(mode, "blink") == 0)
			current_test = &blink_ops;
		else if (strcmp(mode, "text") == 0)
			current_test = &text_ops;
		else
			g_assert_not_reached();
		if (current_test->init != NULL) {
			current_test->init(mainwin);
		}
		on_mainwin_expose_event(mainwin, NULL, NULL);
	}
}

void on_fg_color_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkColorSelection *colorsel;
	GtkWidget *fg_color_selector;

	fg_color_selector = glade_xml_get_widget(glade, "fg_color_selector");

	colorsel = GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(fg_color_selector)->colorsel);
	gtk_color_selection_set_current_color(colorsel, fg_color);
	switch (gtk_dialog_run(GTK_DIALOG(fg_color_selector))) {
		case GTK_RESPONSE_OK:
			gtk_color_selection_get_current_color(colorsel,
					fg_color);
			update_fg_color();
			break;
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_DELETE_EVENT:
			break;
		default:
			g_assert_not_reached();
	}
	gtk_widget_hide(fg_color_selector);
}

void on_bg_color_activate(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkColorSelection *colorsel;
	GtkWidget *bg_color_selector;

	bg_color_selector = glade_xml_get_widget(glade, "bg_color_selector");

	colorsel = GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(bg_color_selector)->colorsel);
	gtk_color_selection_set_current_color(colorsel, bg_color);
	switch (gtk_dialog_run(GTK_DIALOG(bg_color_selector))) {
		case GTK_RESPONSE_OK:
			gtk_color_selection_get_current_color(colorsel,
					bg_color);
			update_bg_color();
			break;
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_DELETE_EVENT:
			break;
		default:
			g_assert_not_reached();
	}
	gtk_widget_hide(bg_color_selector);
}

