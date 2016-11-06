#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT([image-filter], [1], [snswinhaiku dot lo at gmail dot com])
AC_CONFIG_SRCDIR([src/noiser_main.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([m4/tools])
AC_CONFIG_MACRO_DIR([m4/generated])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

LT_INIT([disable-shared])
AM_INIT_AUTOMAKE([foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
CFLAGS="$__conf_saved_cflags"

AC_ARG_ENABLE(debug,
	AS_HELP_STRING(
		[--enable-debug],
		[enable debugging, default: no]),
		[case "${enableval}" in
			yes) debug=true;;
			no)  debug=false;;
			*)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]);;
		esac],
	[debug=false])
if test "$debug" = true; then
	CFLAGS="-g -Og${CFLAGS:+ }${CFLAGS-}"
else
	CFLAGS="-g -O2${CFLAGS:+ }${CFLAGS-}"
fi

AX_CHECK_COMPILE_FLAG([-Wall], [AX_APPEND_FLAG([-Wall], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Wextra], [AX_APPEND_FLAG([-Wextra], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Wconversion], [AX_APPEND_FLAG([-Wconversion], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-pedantic], [AX_APPEND_FLAG([-pedantic], [warn_flags])], [])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_SUBST([CFLAGS])
AC_SUBST([warn_flags])
AC_CONFIG_FILES([Makefile externals/Makefile src/Makefile test/Makefile])
AC_OUTPUT
