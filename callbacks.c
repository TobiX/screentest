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
#include <stdlib.h>
#include <string.h>

#include "callbacks.h"
#include "gettext.h"
#include "main.h"
#define _(String) gettext(String)

GdkRGBA fgcolors[COLOR_MAX];
GdkRGBA *fg_color, *bg_color;
GdkRGBA grays[GRAYS_MAX];
int fg_count = COLOR_WHITE;

static GtkWidget *mainwin = NULL;
static struct test_ops *current_test = &basic_ops;

G_MODULE_EXPORT void on_mainwin_realize(GtkWidget *widget,
                                        G_GNUC_UNUSED gpointer user_data) {
  gint i;

#ifndef DEBUG
  gtk_window_fullscreen(GTK_WINDOW(widget));
#endif

  memset(fgcolors, 0, COLOR_MAX * sizeof(GdkRGBA));

  fgcolors[COLOR_WHITE].red = fgcolors[COLOR_WHITE].green =
      fgcolors[COLOR_WHITE].blue = fgcolors[COLOR_WHITE].alpha = 1.0;

  fgcolors[COLOR_RED].red = fgcolors[COLOR_RED].alpha = 1.0;

  fgcolors[COLOR_GREEN].green = fgcolors[COLOR_GREEN].alpha = 1.0;

  fgcolors[COLOR_BLUE].blue = fgcolors[COLOR_BLUE].alpha = 1.0;

  fgcolors[COLOR_CYAN].green = 1.0;
  fgcolors[COLOR_CYAN].blue = 1.0;
  fgcolors[COLOR_CYAN].alpha = 1.0;

  fgcolors[COLOR_MAGENTA].red = 1.0;
  fgcolors[COLOR_MAGENTA].blue = 1.0;
  fgcolors[COLOR_MAGENTA].alpha = 1.0;

  fgcolors[COLOR_YELLOW].red = 1.0;
  fgcolors[COLOR_YELLOW].green = 1.0;
  fgcolors[COLOR_YELLOW].alpha = 1.0;

  fgcolors[COLOR_BLACK].alpha = 1.0; // The other fields are 0 already

  fg_color = gdk_rgba_copy(&fgcolors[COLOR_WHITE]);
  bg_color = gdk_rgba_copy(&fgcolors[COLOR_BLACK]);

  for (i = 0; i < GRAYS_MAX; i++) {
    grays[i].red = grays[i].green = grays[i].blue = i / (float)(GRAYS_MAX - 1);
    grays[i].alpha = 1.0;
  }

  mainwin = widget;

  if (current_test->init != NULL)
    current_test->init(widget);
}

G_MODULE_EXPORT gboolean
on_mainwin_button_press_event(GtkWidget *widget, GdkEventButton *event,
                              G_GNUC_UNUSED gpointer user_data) {
  GObject *popup;

  switch (event->button) {
  case 1:
    if (current_test->cycle != NULL) {
      current_test->cycle(widget);
      gdk_window_invalidate_rect(gtk_widget_get_window(mainwin), NULL, FALSE);
    }
    break;
  case 2:
    if (++fg_count >= COLOR_MAX)
      fg_count = COLOR_WHITE;
    gdk_rgba_free(fg_color);
    fg_color = gdk_rgba_copy(&fgcolors[fg_count]);
    gdk_window_invalidate_rect(gtk_widget_get_window(mainwin), NULL, FALSE);
    break;
  case 3:
    popup = gtk_builder_get_object(builder, "popup");
#if GTK_CHECK_VERSION(3, 22, 0)
    gtk_menu_popup_at_pointer(GTK_MENU(popup), (GdkEvent *)event);
#else  // GTK_CHECK_VERSION(3,22,0)
    gtk_menu_popup(GTK_MENU(popup), NULL, NULL, NULL, NULL, event->button,
                   event->time);
#endif // GTK_CHECK_VERSION(3,22,0)
    break;
  }
  return TRUE;
}

/*
 * FIXME: GTK_WINDOW_POPUP does not seem to be able to receive
 * KeyPress events. How to enable this?
 */
G_MODULE_EXPORT gboolean on_mainwin_key_press_event(
    G_GNUC_UNUSED GtkWidget *widget, G_GNUC_UNUSED GdkEventKey *event,
    G_GNUC_UNUSED gpointer user_data) {
  gtk_main_quit();
  return FALSE;
}

G_MODULE_EXPORT gboolean on_mainwin_draw_event(
    GtkWidget *widget, cairo_t *cr, G_GNUC_UNUSED gpointer user_data) {
  gdk_window_set_background_rgba(gtk_widget_get_window(widget), bg_color);

  if (current_test && current_test->draw)
    current_test->draw(widget);
  return TRUE;
}

G_MODULE_EXPORT void on_mode_change(GtkMenuItem *menuitem,
                                    G_GNUC_UNUSED gpointer user_data) {
  GtkCheckMenuItem *checkmenuitem = GTK_CHECK_MENU_ITEM(menuitem);

  if (!gtk_check_menu_item_get_active(checkmenuitem)) {
    if (current_test->close != NULL) {
      current_test->close(mainwin);
      current_test = NULL;
    }
  } else {
    const char *test_name = gtk_buildable_get_name(GTK_BUILDABLE(menuitem));
    g_assert(test_name != NULL);
    GModule *exe = g_module_open(NULL, 0);
    g_assert(exe != NULL);
    gchar *test_struct = g_strconcat(test_name, "_ops", NULL);
    if (g_module_symbol(exe, test_struct, (gpointer)&current_test) != TRUE) {
      GtkWidget *dialog = gtk_message_dialog_new(
          GTK_WINDOW(mainwin),
          GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR,
          GTK_BUTTONS_CLOSE, _("No implementation for the test \"%s\" found."),
          test_name);
      gtk_window_set_title(GTK_WINDOW(dialog), PACKAGE_NAME);
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_main_quit();
    }
    free(test_struct);
    g_module_close(exe);
    if (current_test != NULL && current_test->init != NULL) {
      current_test->init(mainwin);
    }
    gtk_widget_queue_draw(mainwin);
  }
}

G_MODULE_EXPORT void on_fg_color_activate(G_GNUC_UNUSED GtkMenuItem *menuitem,
                                          G_GNUC_UNUSED gpointer user_data) {
  GObject *fg_color_selector;

  fg_color_selector = gtk_builder_get_object(builder, "fg_color_selector");

  gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(fg_color_selector), FALSE);
  gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(fg_color_selector), fg_color);
  switch (gtk_dialog_run(GTK_DIALOG(fg_color_selector))) {
  case GTK_RESPONSE_OK:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(fg_color_selector), fg_color);
    gdk_window_invalidate_rect(gtk_widget_get_window(mainwin), NULL, FALSE);
    break;
  case GTK_RESPONSE_CANCEL:
  case GTK_RESPONSE_DELETE_EVENT:
    break;
  default:
    g_assert_not_reached();
  }
  gtk_widget_hide(GTK_WIDGET(fg_color_selector));
}

G_MODULE_EXPORT void on_bg_color_activate(G_GNUC_UNUSED GtkMenuItem *menuitem,
                                          G_GNUC_UNUSED gpointer user_data) {
  GObject *bg_color_selector;

  bg_color_selector = gtk_builder_get_object(builder, "bg_color_selector");
  gtk_color_chooser_set_use_alpha(GTK_COLOR_CHOOSER(bg_color_selector), FALSE);
  gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(bg_color_selector), bg_color);
  switch (gtk_dialog_run(GTK_DIALOG(bg_color_selector))) {
  case GTK_RESPONSE_OK:
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(bg_color_selector), bg_color);
    gdk_window_invalidate_rect(gtk_widget_get_window(mainwin), NULL, FALSE);
    break;
  case GTK_RESPONSE_CANCEL:
  case GTK_RESPONSE_DELETE_EVENT:
    break;
  default:
    g_assert_not_reached();
  }
  gtk_widget_hide(GTK_WIDGET(bg_color_selector));
}

void set_color_bg(cairo_t *cr) {
  cairo_set_source_rgb(cr, bg_color->red, bg_color->green, bg_color->blue);
}

void set_color_fg(cairo_t *cr) {
  cairo_set_source_rgb(cr, fg_color->red, fg_color->green, fg_color->blue);
}
