#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([unnatural_instincts], 0.1, [halkeye@gmail.com])
AM_INIT_AUTOMAKE
# autobuild
AB_INIT

AC_CONFIG_SRCDIR([src])
AM_CONFIG_HEADER(config.h)


# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])
AC_CXX_HAVE_STL
AC_CXX_HAVE_EXT_SLIST

if test "$ac_cv_cxx_have_stl" = no; then
  AC_MSG_ERROR([at least one implementation of STL is required])
fi

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h netinet/in.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_HEADER_TIME

AC_MSG_CHECKING([for some Win32 platform])
case "$host" in
  *-*-mingw*|*-*-cygwin*)
    platform_win32=yes
    ;;
  *)
    platform_win32=no
    ;;
esac
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL(PLATFORM_WIN32, test "$platform_win32" = "yes")


# Do we need to use -ldl?
AC_CHECK_LIB([dl], [dlopen], 
             LDADD="$LDADD -ldl",
             AC_MSG_WARN([libdl doesn't seem to be needed on this system]))

#AM_PATH_XML2(2.5.0,,AC_MSG_ERROR([
#            *** Libxml is required to build unaturalinstincts; Available at
#            http://www.libxml.org/.]))

# Checks for library functions.
AC_HEADER_STDC
AC_FUNC_SELECT_ARGTYPES

PKG_CHECK_MODULES([LIBXML2], [libxml-2.0])
AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_LIBS)

# Check for Functions
AC_CHECK_FUNCS([select socket])

AC_ARG_WITH(crypt,[  --with-crypt,       enabling ecrypting password support])
AC_COND_WITH(crypt,no)

# doxygen
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(unaturalinstincts, doxygen.cfg, doxygen)

# Output Make Files
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
