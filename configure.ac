# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_CONFIG_MACRO_DIR([m4])

AC_PREREQ([2.69])
AC_INIT(JavaReadline, 0.8.2, [https://github.com/bengtmartensson/java-readline])
AC_CONFIG_SRCDIR([src/main/native/org_gnu_readline_Readline.c])

# Prevent autotools to peek in the parent and grandparent directory, see
# https://lists.gnu.org/archive/html/bug-automake/2015-02/msg00005.html
AC_CONFIG_AUX_DIR([.])

AM_INIT_AUTOMAKE([foreign])
LT_INIT([disable-static])

# Checks for programs.
AC_PROG_CC
AX_PROG_JAVAH

# Checks for libraries.
AC_CHECK_LIB(readline,rl_digit_argument,, [AC_MSG_ERROR([*** libreadline not found, install e.g. package readline])])
AC_CHECK_LIB(history, using_history,, [AC_MSG_ERROR([*** libhistory not found, install it.])])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])
AC_CHECK_HEADERS([readline/readline.h readline/history.h],,[AC_MSG_ERROR([*** readline.h not found, install e.g. readline-devel])])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/main/native/Makefile])
AC_OUTPUT
