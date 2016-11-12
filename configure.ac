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

debug_flag="-g"
if test "$debug" = true; then
	AX_CHECK_COMPILE_FLAG([-Og], [AX_APPEND_FLAG([-Og], [debug_flag])], [AX_APPEND_FLAG([-O0], [debug_flag])])
	AC_DEFINE([_DEBUG], [], [Debug mode])
else
	AX_APPEND_FLAG([-O2], [debug_flag])
	AC_DEFINE([NDEBUG], [], [Release mode])
fi
CFLAGS="${debug_flag}${CFLAGS:+ }${CFLAGS-}"

AX_CHECK_COMPILE_FLAG([-Wall], [AX_APPEND_FLAG([-Wall], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Wextra], [AX_APPEND_FLAG([-Wextra], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Wconversion], [AX_APPEND_FLAG([-Wconversion], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Werror=implicit-function-declaration], [AX_APPEND_FLAG([-Werror=implicit-function-declaration], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-pedantic], [AX_APPEND_FLAG([-pedantic], [warn_flags])], [])
AX_CHECK_LINK_FLAG([-rdynamic], [AX_APPEND_FLAG([-rdynamic], [LDFLAGS])], [])

# Checks for libraries.

AC_ARG_WITH([libpng],
			[AS_HELP_STRING([--with-libpng],
							[support png r/w, default: auto])],
			[case "${enableval}" in
				yes) libpng_support=true;;
				no)  libpng_support=false;;
				auto)libpng_support=auto;;
				*)   AC_MSG_ERROR([bad value ${enableval} for --with-libpng]);;
			esac],
			[libpng_support=auto])
if test "x$libpng_support" != xauto; then
	: do nothing
else
	PKG_CHECK_MODULES([libpng], [libpng], [has_libpng=yes], [has_libpng=no])
	if test "x$libpng_support$has_libpng" = "xyesno"; then
		AC_MSG_ERROR([--with-libpng requires libpng])
	fi
	AC_DEFINE([USE_LIBPNG], [1], [libpng])
fi
AC_SUBST([libpng_CFLAGS])
AC_SUBST([libpng_LIBS])

AC_ARG_WITH([libsixel],
			[AS_HELP_STRING([--with-libsixel],
							[support png r/w, default: auto])],
			[case "${enableval}" in
				yes) libsixel_support=true;;
				no)  libsixel_support=false;;
				auto)libsixel_support=auto;;
				*)   AC_MSG_ERROR([bad value ${enableval} for --with-libsixel]);;
			esac],
			[libsixel_support=auto])
if test "x$libsixel_support" != xauto; then
	: do nothing
else
	PKG_CHECK_MODULES([libsixel], [libsixel], [has_libsixel=yes], [has_libsixel=no])
	if test "x$libsixel_support$has_libsixel" = "xyesno"; then
		AC_MSG_ERROR([--with-libsixel requires libsixel])
	fi
	AC_DEFINE([USE_LIBSIXEL], [1], [libsixel])
fi
AC_SUBST([libsixel_CFLAGS])
AC_SUBST([libsixel_LIBS])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_SUBST([CFLAGS])
AC_SUBST([warn_flags])
AC_CONFIG_FILES([Makefile externals/Makefile src/Makefile test/Makefile])
AC_OUTPUT
