AC_INIT([hotstuff], [0])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])

OPT_FLAGS="-march=native -O3 -flto"
DEBUG_FLAGS="-ggdb -O0"

CXXFLAGS="-std=gnu++2a $OPT_FLAGS $CXXFLAGS"
CXX="g++"

AC_PROG_CXX
AC_LANG(C++)
AX_APPEND_COMPILE_FLAGS([-pthread])
AS_IF([test -z "${WFLAGS+set}"], AX_APPEND_COMPILE_FLAGS([-Wall], WFLAGS))

AC_PROG_RANLIB

CXXFLAGS="$CXXFLAGS $WFLAGS"

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([libsodium], [libsodium])
PKG_CHECK_MODULES([lmdb], [lmdb])
PKG_CHECK_MODULES([Catch2], [catch2])

export CXXFLAGS

AC_MSG_CHECKING($xdrpp_INTERNAL)

AS_IF([test -z "$xdrpp_INTERNAL"], 
	[
		AC_MSG_NOTICE("using pkg-config for xdrpp")
		PKG_CHECK_MODULES([xdrpp], [xdrpp])
		AC_PATH_PROG(XDRC, [xdrc])
	], 
	[
		xdrpp_LIBS=$global_xdrpp_LIBS
		xdrpp_CFLAGS=$global_xdrpp_CFLAGS
		XDRC=$global_XDRC
		AC_MSG_CHECKING(using xdrpp with xdrc=$XDRC lib=$xdrpp_LIBS cflags=$xdrpp_CFLAGS)
	])

AC_MSG_RESULT(ac is $ac_top_srcdir try2 = $ac_pwd)

AC_SUBST(r_abs_top_srcdir, $ABS_TOP_DIR)
AC_SUBST(r_top_srcdir, $ABS_TOP_DIR)
AC_SUBST(r_top_builddir, $ABS_TOP_DIR)
AC_SUBST(r_abs_top_builddir, $ABS_TOP_DIR)

export CXXFLAGS

#AX_PKGCONFIG_SUBDIR(xdrpp)
#AC_MSG_CHECKING(for xdrc)
#AS_IF([test -n "$XDRC"],,
#	    [test -n "$xdrpp_INTERNAL" -a x"$cross_compiling" != xyes],
#	    [XDRC='$(top_builddir)/xdrpp/xdrc/xdrc$(EXEEXT)'],
#	    [AC_PATH_PROG(XDRC, [xdrc])])
#if test -z "$XDRC"; then
#   AC_MSG_ERROR(Cannot find xdrc)
#fi

if test -z "$XDRC"; then
   AC_MSG_ERROR(Cannot find xdrc)
fi
AC_MSG_RESULT($XDRC)
AC_SUBST(XDRC)



AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile hotstuff.pc hotstuff-uninstalled.pc])
AC_OUTPUT
