dnl Process this file with autoconf to produce a configure script.

AC_INIT(screentest, 1.1)
AC_CONFIG_SRCDIR([screentest.glade])
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

AC_CONFIG_HEADER(config.h)

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_LIBTOOL
AC_HEADER_STDC

AM_PATH_GTK_2_0

PKG_CHECK_MODULES(GLADE, libglade-2.0)
AC_SUBST(GLADE_CFLAGS)
AC_SUBST(GLADE_LIBS)

ALL_LINGUAS="de"
AM_GLIB_GNU_GETTEXT
AM_GLIB_DEFINE_LOCALEDIR(LOCALEDIR)
GETTEXT_PACKAGE=$PACKAGE
AC_SUBST(GETTEXT_PACKAGE)

dnl Use -Wall if we have gcc.
if test "x$GCC" = "xyes"; then
  case " $CFLAGS " in
  *[[\ \	]]-Wall[[\ \	]]*) ;;
  *) CFLAGS="$CFLAGS -Wall" ;;
  esac
fi

AC_CONFIG_FILES([Makefile po/Makefile.in])
AC_OUTPUT

