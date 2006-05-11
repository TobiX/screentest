## Process this file with automake to produce Makefile.in

SUBDIRS = po

AM_CPPFLAGS = $(GTK_CFLAGS)

EXTRA_DIST = screentest.glade2 NEW_TESTS screentest.spec

bin_PROGRAMS = screentest

screentest_SOURCES = main.c support.c support.h interface.c interface.h \
		     callbacks.c callbacks.h basic_test.c blink_test.c \
		     grid_test.c horizontal_test.c text_test.c \
		     vertical_test.c

screentest_LDADD = $(GTK_LIBS)
