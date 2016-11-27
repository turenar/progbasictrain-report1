dnl $1 = name
dnl $2 = pkgname
dnl $3 = function
dnl
dnl output: has_$1 = yes|no
AC_DEFUN([MY_CHECK_MODULES_LINK],[dnl
PKG_CHECK_MODULES([$1], [$2],
	[AC_MSG_CHECKING([whether compiler works with $1])
	 __mcml_cflags="$CFLAGS"
	 __mcml_libs="$LIBS"
	 CFLAGS="$CFLAGS $[]$1[]_CFLAGS"
	 LIBS="$LIBS $[]$1[]_LIBS"
	 AC_LINK_IFELSE([AC_LANG_CALL([], [$3])],
		[AC_MSG_RESULT([yes])
		 has_[]$1=yes],
		[AC_MSG_RESULT([no])
		 AC_MSG_WARN([pkg-config found package but compiler denied working; you are trying to compile cross environment?])
		 has_[]$1=no
		 $1[]_CFLAGS=
		 $1[]_LIBS=])
	 CFLAGS="$__mcml_cflags"
	 LIBS="$__mcml_libs"],
        [has[]$1=no])])
