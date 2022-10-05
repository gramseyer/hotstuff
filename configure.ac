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
		XDRC=$global_XDRC
		AC_MSG_CHECKING(using xdrpp with xdrc=$XDRC lib=$xdrpp_LIBS cflags=$xdrpp_CFLAGS)
	])

AS_IF([test -z "$XDRC"],
	[AC_MSG_ERROR(failed to find XDRC $XDRC)],
	[AC_MSG_NOTICE(found XDRC $XDRC)])


AS_IF([test -z "$ABS_TOP_BUILDDIR"],
	[
		ABS_TOP_BUILDDIR=$ac_pwd
		ABS_TOP_SRCDIR=$ac_pwd
	],
	)

AC_SUBST(r_abs_top_srcdir, $ABS_TOP_SRCDIR)
AC_SUBST(r_abs_top_builddir, $ABS_TOP_BUILDDIR)

if test -z "$XDRC"; then
   AC_MSG_ERROR(Cannot find xdrc)
fi
AC_MSG_RESULT($XDRC)
AC_SUBST(XDRC)

AS_IF([test -z "$utility_INTERNAL"], 
	[
		AC_MSG_NOTICE("using pkg-config for utility")
		PKG_CHECK_MODULES([utility], [utility])
	], 
	[
		AC_MSG_CHECKING(using utility with lib=$utility_LIBS cflags=$utility_CFLAGS)
	])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile hotstuff.pc hotstuff-uninstalled.pc])
AC_OUTPUT
