dnl Process this file with autoconf to produce a configure script.

AC_INIT(screentest, 1.0)
AC_CONFIG_SRCDIR([screentest.glade2])
AM_INIT_AUTOMAKE([foreign])
AM_CONFIG_HEADER(config.h)

AC_ISC_POSIX
AC_PROG_CC
AC_HEADER_STDC

AM_PATH_GTK_2_0

dnl Use -Wall if we have gcc.
if test "x$GCC" = "xyes"; then
  case " $CFLAGS " in
  *[[\ \	]]-Wall[[\ \	]]*) ;;
  *) CFLAGS="$CFLAGS -Wall" ;;
  esac
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

