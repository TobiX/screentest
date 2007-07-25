## Process this file with automake to produce Makefile.in

SUBDIRS = po

AM_CPPFLAGS = $(GTK_CFLAGS) $(GLADE_CFLAGS) \
	      -DDATADIR=\"$(pkgdatadir)\"

EXTRA_DIST = screentest.glade NEW_TESTS

pkgdata_DATA = screentest.glade

bin_PROGRAMS = screentest

screentest_SOURCES = main.c main.h callbacks.c callbacks.h basic_test.c \
		     blink_test.c grid_test.c horizontal_test.c text_test.c \
		     vertical_test.c gettext.h

screentest_LDADD = $(GTK_LIBS) $(GLADE_LIBS)
screentest_LDFLAGS = -export-dynamic
