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

CXXFLAGS="$CXXFLAGS $WFLAGS"

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([libsodium], [libsodium])
PKG_CHECK_MODULES([lmdb], [lmdb])
PKG_CHECK_MODULES([tbb], [tbb])
PKG_CHECK_MODULES([Catch2], [Catch2])

AS_IF([test -z "${ROOT_DB_DIRECTORY}"], [ROOT_DB_DIRECTORY="database_root/"])
AS_IF([test -z "${HOTSTUFF_BLOCKS}"], [HOTSTUFF_BLOCKS="hotstuff_blocks/"])
AS_IF([test -z "${HOTSTUFF_INDEX}"], [HOTSTUFF_INDEX="hotstuff_index/"])

AC_DEFINE_UNQUOTED([ROOT_DB_DIRECTORY], ["$ROOT_DB_DIRECTORY"], [Root directory for storing lmdbs + txs])
AC_DEFINE_UNQUOTED([HOTSTUFF_BLOCKS], ["$HOTSTUFF_BLOCKS"], [Subdirectory of ROOT_DB_DIRECTORY for hotstuff blocks])
AC_DEFINE_UNQUOTED([HOTSTUFF_INDEX], ["$HOTSTUFF_INDEX"], [Subdirectory of ROOT_DB_DIRECTORY for hotstuff block index])

AS_MKDIR_P([$ROOT_DB_DIRECTORY$HOTSTUFF_BLOCKS])
AS_MKDIR_P([$ROOT_DB_DIRECTORY$HOTSTUFF_INDEX])

export CXXFLAGS

AX_PKGCONFIG_SUBDIR(xdrpp)
AC_MSG_CHECKING(for xdrc)
AS_IF([test -n "$XDRC"],,
	    [test -n "$xdrpp_INTERNAL" -a x"$cross_compiling" != xyes],
	    [XDRC='$(top_builddir)/xdrpp/xdrc/xdrc$(EXEEXT)'],
	    [AC_PATH_PROG(XDRC, [xdrc])])
if test -z "$XDRC"; then
   AC_MSG_ERROR(Cannot find xdrc)
fi
AC_MSG_RESULT($XDRC)
AC_SUBST(XDRC)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
