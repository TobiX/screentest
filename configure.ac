dnl Process this file with autoconf to produce a configure script.

AC_INIT(screentest, 2.1)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([screentest.ui])
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

AC_CONFIG_HEADER(config.h)

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_LIBTOOL
AC_HEADER_STDC

AM_PATH_GTK_2_0(2.16.0)

ALL_LINGUAS="de"
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.40.0])
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

dnl Windows-detection
case $host in
  i[[3456789]]86-*-mingw32*) WIN32="yes" ;;
  *cygwin*) WIN32="yes" ;;
  *) WIN32="no" ;;
esac
AM_CONDITIONAL([WIN32], test "x$WIN32" = "xyes")

AC_CONFIG_FILES([Makefile po/Makefile.in])
AC_OUTPUT

