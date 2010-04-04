## Process this file with automake to produce Makefile.in

SUBDIRS = po

AM_CPPFLAGS = $(GTK_CFLAGS) -DDATADIR=\"$(pkgdatadir)\"

EXTRA_DIST =  screentest.ui test_NEWS

pkgdata_DATA = screentest.ui

bin_PROGRAMS = screentest

screentest_SOURCES = main.c main.h callbacks.c callbacks.h test_basic.c \
		     test_blink.c test_grid.c test_horizontal.c test_text.c \
		     test_vertical.c gettext.h

screentest_LDADD = $(GTK_LIBS)

if WIN32
screentest_LDFLAGS = -mwindows
else
screentest_LDFLAGS = -export-dynamic
endif

ACLOCAL_AMFLAGS = -I m4
