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

#include <stdlib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

#include "main.h"
#include "gettext.h"
#define _(String) gettext(String)

GladeXML *glade;

int main(int argc, char *argv[])
{
	bindtextdomain(PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(PACKAGE, "UTF-8");
	textdomain(PACKAGE);

	gtk_init(&argc, &argv);

	glade = glade_xml_new(DATADIR "/screentest.glade", NULL, NULL);
	if (glade == NULL)
		glade = glade_xml_new("screentest.glade", NULL, NULL);
	if (glade == NULL) {
		GtkWidget *dialog = gtk_message_dialog_new(NULL,
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
			_("The GLADE interface definition file was not found.\n"
			"Please make sure this program is installed correctly."));
		gtk_window_set_title(GTK_WINDOW(dialog), PACKAGE_NAME);
		gtk_dialog_run(GTK_DIALOG(dialog));
		exit(1);
	}

	glade_xml_signal_autoconnect(glade);

	gtk_widget_show(glade_xml_get_widget(glade, "mainwin"));
	gtk_main();
	return 0;
}
