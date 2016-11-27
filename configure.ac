#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT([image-filter], [1], [snswinhaiku dot lo at gmail dot com])
AC_CONFIG_SRCDIR([src/noiser_main.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([m4/tools])
AC_CONFIG_MACRO_DIR([m4/generated])

AC_CANONICAL_SYSTEM

LT_INIT([disable-shared])
AM_INIT_AUTOMAKE([foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
m4_ifdef([PKG_CHECK_MODULES],
	 [pkg_check_modules_dummy_defined=0],
	 [m4_define([PKG_CHECK_MODULES], [$4])
	 pkg_check_modules_dummy_defined=1
	 AC_MSG_WARN([[PKG_CHECK_MODULES is not found! You must install pkg-config for extra functions]])])
__conf_saved_cflags="$CFLAGS"
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
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

AX_CHECK_COMPILE_FLAG([-Wall], [AX_APPEND_FLAG([-Wall], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Wextra], [AX_APPEND_FLAG([-Wextra], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Wconversion], [AX_APPEND_FLAG([-Wconversion], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-Werror=implicit-function-declaration], [AX_APPEND_FLAG([-Werror=implicit-function-declaration], [warn_flags])], [])
AX_CHECK_COMPILE_FLAG([-pedantic], [AX_APPEND_FLAG([-pedantic], [warn_flags])], [])
# for libSegFault.so, backtrace
AX_CHECK_LINK_FLAG([-rdynamic], [AX_APPEND_FLAG([-rdynamic], [LDFLAGS])], [])

# Checks for libraries.
PKG_PROG_PKG_CONFIG

AC_ARG_WITH([libpng],
			[AS_HELP_STRING([--with-libpng],
							[support png r/w, default: auto])],
			[case "${withval}" in
				yes) libpng_support=yes;;
				no)  libpng_support=no;;
				auto)libpng_support=auto;;
				*)   AC_MSG_ERROR([bad value ${withval} for --with-libpng]);;
			esac],
			[libpng_support=auto])
use_libpng=no

AC_MSG_CHECKING([whether libpng support is required])
AC_MSG_RESULT([$libpng_support])
if test "x$libpng_support" = xno; then
	: do nothing
else
	MY_CHECK_MODULES_LINK([libpng], [libpng], [png_free])
	if test "x$libpng_support$has_libpng" = "xyesno"; then
		AC_MSG_ERROR([--with-libpng requires libpng])
	fi
	if test "x$has_libpng" = xyes; then
		AC_DEFINE([USE_LIBPNG], [1], [libpng])
		use_libpng=yes
	fi
fi
AM_CONDITIONAL([USE_LIBPNG], [test $use_libpng = yes])
AC_SUBST([libpng_CFLAGS])
AC_SUBST([libpng_LIBS])

AC_ARG_WITH([libsixel],
			[AS_HELP_STRING([--with-libsixel],
							[support png r/w, default: auto])],
			[case "${withval}" in
				yes) libsixel_support=yes;;
				no)  libsixel_support=no;;
				auto)libsixel_support=auto;;
				*)   AC_MSG_ERROR([bad value ${withval} for --with-libsixel]);;
			esac],
			[libsixel_support=auto])
use_libsixel=no

AC_MSG_CHECKING([whether libsixel support is required])
AC_MSG_RESULT([$libsixel_support])
if test "x$libsixel_support" = xno; then
	: do nothing
else
	MY_CHECK_MODULES_LINK([libsixel], [libsixel >= 1.6], [sixel_encoder_new])
	if test "x$libsixel_support$has_libsixel" = "xyesno"; then
		AC_MSG_ERROR([--with-libsixel requires libsixel])
	fi
	if test "x$has_libsixel" = xyes; then
		AC_DEFINE([USE_LIBSIXEL], [1], [libsixel])
		use_libsixel=yes
	fi
fi
AM_CONDITIONAL([USE_LIBSIXEL], [test $use_libsixel = yes])
AC_SUBST([libsixel_CFLAGS])
AC_SUBST([libsixel_LIBS])

AC_ARG_WITH([gtk3],
			[AS_HELP_STRING([--with-gtk3],
							[support png r/w, default: auto])],
			[case "${withval}" in
				yes) gtk_support=yes;;
				no)  gtk_support=no;;
				auto)gtk_support=auto;;
				*)   AC_MSG_ERROR([bad value ${withval} for --with-gtk]);;
			esac],
			[gtk_support=auto])
use_gtk=no
AC_MSG_CHECKING([whether gtk support is required])
AC_MSG_RESULT([$gtk_support])
if test "x$gtk_support" = xno; then
	: do nothing
else
	MY_CHECK_MODULES_LINK([gtk], [gtk+-3.0], [gtk_init])
	if test "x$gtk_support$has_gtk" = "xyesno"; then
		AC_MSG_ERROR([--with-gtk requires gtk])
	fi
	if test "x$has_gtk" = xyes; then
		use_gtk=yes
		AC_DEFINE([USE_GTK], [1], [gtk])
	fi
fi
AM_CONDITIONAL([USE_GTK], [test $use_gtk = yes])
AC_SUBST([gtk_CFLAGS])
AC_SUBST([gtk_LIBS])
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
saved_LIBS="$LIBS"
LIBS=
AC_SEARCH_LIBS([sqrt], [m], [], [AC_MSG_ERROR([sqrt is not found])])
mathlib_LIBS="$LIBS"
AC_SUBST([mathlib_LIBS])
LIBS="$saved_LIBS"

# leak sanitizer spends more time
AC_ARG_ENABLE(maintainer-mode,
	AS_HELP_STRING(
		[--enable-maintainer-mode],
		[enable compiler option for maintainer, default: no]),
	[case "${enableval}" in
		yes) maintainer_mode=true;;
		no)  maintainer_mode=false;;
		*)   AC_MSG_ERROR([bad value ${enableval} for --enable-maintainer-mode]);;
	esac],
	[maintainer_mode=false])

if test "$maintainer_mode" = true; then
	# -fsanitize=leak fails with linker error (-llsan is not found)
	AX_CHECK_COMPILE_FLAG([-fsanitize=leak],
						  [AX_CHECK_LINK_FLAG([-fsanitize=leak], [AX_APPEND_FLAG([-fsanitize=leak], [])])], [])
fi

AC_ARG_ENABLE(coverage,
	AS_HELP_STRING(
		[--enable-coverage],
		[enable coverage option, default: no]),
	[case "${enableval}" in
		yes)  coverage_mode=true;;
		no)   coverage_mode=false;;
		*)    AC_MSG_ERROR([bad value ${enableval} for --enable-coverage]);;
	esac],
	[coverage_mode=false])

if test "$coverage_mode" = true; then
	AX_CHECK_COMPILE_FLAG([-fprofile-arcs -ftest-coverage],
						  [AX_APPEND_FLAG([-fprofile-arcs -ftest-coverage], [])],
						  [AC_MSG_ERROR([--enable-coverage passed but compiler not supported])])
fi

AC_SUBST([CFLAGS])
AC_SUBST([warn_flags])
AC_CONFIG_FILES([Makefile externals/Makefile src/Makefile test/Makefile])
AC_OUTPUT

if test "x$pkg_check_modules_dummy_defined" = x1; then
	use_libpng="no (pkg-config is not installed?)"
	use_libsixel="no (pkg-config is not installed?)"
	use_gtk="no (pkg-config is not installed?)"
fi
echo "------------------------------------"
echo "libpng:        $use_libpng"
echo "libsixel:      $use_libsixel"
echo "gtk:           $use_gtk"
echo "CFLAGS:        $warn_flags $CFLAGS"
