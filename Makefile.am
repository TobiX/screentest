## Process this file with automake to produce Makefile.in

INCLUDES = \
	@GTK_CFLAGS@

EXTRA_DIST = screentest.glade NEW_TESTS screentest.spec install-sh \
	mkinstalldirs missing

bin_PROGRAMS = screentest
noinst_PROGRAMS = screentest.static

screentest_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	basic_test.c blink_test.c grid_test.c horizontal_test.c text_test.c \
	vertical_test.c 


screentest_LDADD = @GTK_LIBS@ -lpthread -lXdmcp -lXau

screentest_static_SOURCES = $(screentest_SOURCES)
screentest_static_LDADD = $(screentest_LDADD)
screentest_static_LDFLAGS = -static -s

