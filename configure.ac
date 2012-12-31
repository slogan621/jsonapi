#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([jsonapi], [1.0], [xxx@gmail.com])
AC_CONFIG_SRCDIR([src/jsonapi.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(jsonapi, 1.0)
LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_PROG_LEX
AC_PROG_YACC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([memory.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([strstr])

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])
AC_OUTPUT
