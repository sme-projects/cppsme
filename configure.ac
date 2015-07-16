AC_INIT([libsme], [0.0.1])
AM_INIT_AUTOMAKE

AC_CONFIG_MACRO_DIRS([m4])

AC_PROG_CXX

# Enable compiler warnings/errors
AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"], [], [])
AX_CHECK_COMPILE_FLAG([-Werror], [CXXFLAGS="$CXXFLAGS -Werror"], [], [])
AX_CHECK_COMPILE_FLAG([-pedantic], [CXXFLAGS="$CXXFLAGS -pedantic"], [], [])

AX_CHECK_COMPILE_FLAG([-pthread], [CXXFLAGS="$CXXFLAGS -pthread"], [], [])

AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

LT_INIT

AX_WITH_CURSES
AM_PATH_PYTHON([3.4])

AX_CREATE_PKGCONFIG_INFO([src/libsme.pc],,[-lsme])

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])
AC_OUTPUT
