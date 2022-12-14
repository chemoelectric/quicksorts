#
# Copyright © 2022 Barry Schwartz
#
# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License, as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received copies of the GNU General Public License
# along with this program. If not, see
# <https://www.gnu.org/licenses/>.
#

#--------------------------------------------------------------------------

AC_PREREQ([2.71])
AC_INIT([quicksorts],[1.0.0],[],[quicksorts],[])

AC_CONFIG_SRCDIR([quicksorts/unstable-quicksort.h])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CANONICAL_BUILD                 dnl On what platform are we compiling?
AC_CANONICAL_HOST                  dnl For what platform are we compiling?
AC_USE_SYSTEM_EXTENSIONS           dnl Define such macros as _GNU_SOURCE.

AM_INIT_AUTOMAKE
AM_SILENT_RULES
AM_MAINTAINER_MODE
LT_INIT

AM_CONDITIONAL([DEPENDENCY_TRACKING],
  [test x"${enable_dependency_tracking}" != xno])

#--------------------------------------------------------------------------
#
# Checks for programs.

AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_CXX_C_O
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_AWK
AC_PROG_SED
AC_PROG_FGREP
AC_PROG_EGREP

# Enable optional code coverage analysis.
AX_CODE_COVERAGE

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

#--------------------------------------------------------------------------
#
# Checks for libraries.

#--------------------------------------------------------------------------
#
# Checks for header files.

#--------------------------------------------------------------------------
#
# Checks for typedefs, structures, and compiler characteristics.

#--------------------------------------------------------------------------
#
# Checks for library functions.

#--------------------------------------------------------------------------

CPPFLAGS="${CPPFLAGS}${CPPFLAGS+ }\$(CODE_COVERAGE_CPPFLAGS)"
CFLAGS="${CFLAGS}${CFLAGS+ }\$(CODE_COVERAGE_CFLAGS)"

# You can use ‘@GNU@’ to make GNU Make constructs unbothersome to
# Automake. (By the way, you often can use ‘$(eval ...)’ as well, but
# @GNU@ here may be better.)
AC_SUBST([GNU],[])

StM_REQUIRE_GNU_MAKE_IN_PATH
StM_CONFIG_MAKEFILES

m4_define([my_config_executable],
    [AC_CONFIG_FILES([$1],[chmod +x $1])])

my_config_executable([tests/test-unstable_qsort])
my_config_executable([tests/test-unstable_qsort_r])
my_config_executable([tests/test-unstable-defaults])
my_config_executable([tests/test-unstable-defaults-typed])
my_config_executable([tests/test-unstable-random-insertion])
my_config_executable([tests/test-unstable-random-insertion-typed])
my_config_executable([tests/test-unstable-middle-insertion])
my_config_executable([tests/test-unstable-middle-insertion-typed])
my_config_executable([tests/test-unstable-median3-insertion])
my_config_executable([tests/test-unstable-median3-insertion-typed])
my_config_executable([tests/test-unstable-random-shell])
my_config_executable([tests/test-unstable-random-shell-typed])
my_config_executable([tests/test-unstable-middle-shell])
my_config_executable([tests/test-unstable-middle-shell-typed])
my_config_executable([tests/test-unstable-median3-shell])
my_config_executable([tests/test-unstable-median3-shell-typed])
my_config_executable([tests/test-unstable-random-insertion-big])
my_config_executable([tests/test-unstable-random-insertion-typed-big])
my_config_executable([tests/test-unstable-median3-insertion-big])
my_config_executable([tests/test-unstable-median3-insertion-typed-big])
my_config_executable([tests/test-unstable-random-shell-big])
my_config_executable([tests/test-unstable-random-shell-typed-big])
my_config_executable([tests/test-unstable-median3-shell-big])
my_config_executable([tests/test-unstable-median3-shell-typed-big])

AC_CONFIG_FILES([quicksorts.pc])

AC_OUTPUT

#--------------------------------------------------------------------------
# local variables:
# coding: utf-8
# end:
