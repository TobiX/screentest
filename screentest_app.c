/*
 *  Screentest - CRT/LCD monitor testing utility.
 *  https://tobix.github.io/screentest/
 *  Copyright (C) 2025 Markus Prasser
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

#include <glib/gi18n.h>

#include "screentest_app.h"

#include "main.h"

struct _ScreentestApp {
  GtkApplication parent;
};

G_DEFINE_TYPE(ScreentestApp, screentest_app, GTK_TYPE_APPLICATION);

static void screentest_app_activate(GApplication *app);
static void screentest_app_class_init(ScreentestAppClass *class);
static void screentest_app_init(ScreentestApp *app);
static void screentest_app_startup(GApplication *app);

ScreentestApp *screentest_app_new(void) {
  return g_object_new(SCREENTEST_APP_TYPE, "application-id",
                      "com.github.tobix.screentest", "flags",
                      G_APPLICATION_NON_UNIQUE, NULL);
}

static void screentest_app_activate(GApplication *app) {
  guint err;
  GtkWidget *mainwin;

  bindtextdomain(PACKAGE, LOCALEDIR);
  bind_textdomain_codeset(PACKAGE, "UTF-8");
  textdomain(PACKAGE);

  builder = gtk_builder_new();
  err = gtk_builder_add_from_file(builder, DATADIR "/screentest/screentest.ui",
                                  NULL);
  if (err == 0)
    err = gtk_builder_add_from_file(builder, "screentest.ui", NULL);
  if (err == 0) {
    GtkWidget *dialog = gtk_message_dialog_new(
        NULL, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
        _("The interface definition file was not found.\n"
          "Please make sure this program is installed correctly."));
    gtk_window_set_title(GTK_WINDOW(dialog), PACKAGE_NAME);
    gtk_dialog_run(GTK_DIALOG(dialog));
    exit(1);
  }

  gtk_builder_connect_signals(builder, NULL);

  mainwin = GTK_WIDGET(gtk_builder_get_object(builder, "mainwin"));
  gtk_widget_show_all(mainwin);

  gtk_window_set_application(GTK_WINDOW(mainwin), GTK_APPLICATION(app));
}

static void screentest_app_class_init(ScreentestAppClass *class) {
  GApplicationClass *application_class = G_APPLICATION_CLASS(class);

  application_class->activate = screentest_app_activate;
  application_class->startup = screentest_app_startup;
}

static void screentest_app_init(ScreentestApp *app) {}

static void screentest_app_startup(GApplication *app) {
  G_APPLICATION_CLASS(screentest_app_parent_class)->startup(app);
}
