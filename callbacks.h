/* $Id: callbacks.h,v 1.1.1.1 2001/08/20 18:10:55 kas Exp $ */

/*
 *  Screentest - CRT monitor testing utility.
 *  http://www.fi.muni.cz/~kas/screentest/
 *  Copyright (C) 2001 Jan "Yenya" Kasprzak <kas@fi.muni.cz>
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
#include <gtk/gtk.h>

enum test_mode {
	MODE_BASIC,
	MODE_GRID,
	MODE_HORIZONTAL,
	MODE_VERTICAL,
	MODE_BLINK,
	MODE_TEXT,
	MODE_MAX
};

struct test_ops {
	void (*init) (GtkWidget * widget);
	void (*draw) (GtkWidget * widget, gboolean clear);
	void (*cycle) (GtkWidget * widget);
	void (*close) (GtkWidget * widget);
};

extern struct test_ops basic_ops, grid_ops, horizontal_ops, vertical_ops,
    blink_ops, text_ops;

enum test_color {
	COLOR_WHITE,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_CYAN,
	COLOR_MAGENTA,
	COLOR_YELLOW,
	COLOR_BLACK,
	COLOR_MAX
};

#define GRAYS_MAX COLOR_MAX

extern GdkGC *gc, *backgc;
extern GdkColor fgcolors[];
extern gint fg_color;
extern GdkColor grays[];

void on_mainwin_realize(GtkWidget * widget, gpointer user_data);

gboolean
on_area_button_press_event(GtkWidget * widget,
			   GdkEventButton * event, gpointer user_data);

gboolean
on_area_key_press_event(GtkWidget * widget,
			GdkEventKey * event, gpointer user_data);

gboolean
on_area_expose_event(GtkWidget * widget,
		     GdkEventExpose * event, gpointer user_data);

void on_area_realize(GtkWidget * widget, gpointer user_data);

gboolean
on_mainwin_key_press_event(GtkWidget * widget,
			   GdkEventKey * event, gpointer user_data);

void on_exit1_activate(GtkMenuItem * menuitem, gpointer user_data);

void on_color_change(GtkMenuItem * menuitem, gpointer user_data);

void on_mode_change(GtkMenuItem * menuitem, gpointer user_data);

void on_bgcolor_change(GtkMenuItem * menuitem, gpointer user_data);
