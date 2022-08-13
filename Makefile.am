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

.DELETE_ON_ERROR:

ACLOCAL_AMFLAGS = -I m4 --install
AUTOMAKE_OPTIONS =
AUTOMAKE_OPTIONS += foreign
AUTOMAKE_OPTIONS += subdir-objects
#AUTOMAKE_OPTIONS += -Wno-portability

include $(top_srcdir)/aminclude_static.am
@GNU@ include $(top_srcdir)/silent-rules.mk

EXTRA_DIST =
MOSTLYCLEANFILES =
CLEANFILES =
DISTCLEANFILES =
BUILT_SOURCES =
AM_CPPFLAGS =
AM_CFLAGS =
AM_LDFLAGS =
LDADD =
bin_PROGRAMS =
lib_LTLIBRARIES =
EXTRA_PROGRAMS =
nobase_nodist_pkginclude_HEADERS =
nodist_pkgconfig_DATA =

EXTRA_DIST += $(srcdir)/silent-rules.mk
EXTRA_DIST += COPYING
EXTRA_DIST += INSTALL
EXTRA_DIST += README
EXTRA_DIST += AUTHORS

DISTCLEANFILES += Makefile
DISTCLEANFILES += GNUmakefile

AM_CPPFLAGS += -I$(builddir)
AM_CPPFLAGS += -I$(srcdir)

LDADD += $(CODE_COVERAGE_LIBS)

CODE_COVERAGE_IGNORE_PATTERN =
CODE_COVERAGE_IGNORE_PATTERN += /usr/include/*
CODE_COVERAGE_IGNORE_PATTERN += /usr/include/*/*

clean-local: code-coverage-clean tests-clean
distclean-local: code-coverage-dist-clean

#---------------------------------------------------------------------
#
# Some GNU Make aids you might want to use.
#

comma := ,
empty :=
space := $(empty) $(empty)

# Escape things for sed expressions, etc.
escape = $(subst $$,\$$,$(subst ^,\^,$(subst ],\],$(subst [,\[,$(subst .,\.,$(subst \,\\,$(1)))))))

# #--------------------------------------------------------------------------

lib_LTLIBRARIES += libquicksorts.la
libquicksorts_la_SOURCES =
libquicksorts_la_SOURCES += unstable_qsort.c
libquicksorts_la_SOURCES += unstable_qsort_r.c
libquicksorts_la_SOURCES += lcg-seed.c
libquicksorts_la_SOURCES += $(nobase_dist_include_HEADERS)

nobase_dist_include_HEADERS =
nobase_dist_include_HEADERS += quicksorts/unstable-qsort.h
nobase_dist_include_HEADERS += quicksorts/unstable-quicksort.h
nobase_dist_include_HEADERS += quicksorts/internal/quicksorts-common.h

#--------------------------------------------------------------------------

TESTS =
TESTS += tests/test-unstable_qsort
TESTS += tests/test-unstable_qsort_r
TESTS += tests/test-unstable-defaults
TESTS += tests/test-unstable-defaults-typed
TESTS += tests/test-unstable-random-insertion
TESTS += tests/test-unstable-random-insertion-typed
TESTS += tests/test-unstable-middle-insertion
TESTS += tests/test-unstable-median3-insertion
TESTS += tests/test-unstable-random-shell
TESTS += tests/test-unstable-middle-shell
TESTS += tests/test-unstable-median3-shell
TESTS += tests/test-unstable-random-insertion-big
TESTS += tests/test-unstable-median3-insertion-big
TESTS += tests/test-unstable-random-shell-big
TESTS += tests/test-unstable-median3-shell-big

check: tests/test-int-patterns
check: tests/test-big

EXTRA_PROGRAMS += tests/test-int-patterns
CLEANFILES += tests/test-int-patterns
tests_test_int_patterns_SOURCES =
tests_test_int_patterns_SOURCES += tests/test-int-patterns.c
tests_test_int_patterns_DEPENDENCIES =
tests_test_int_patterns_DEPENDENCIES += libquicksorts.la
tests_test_int_patterns_LDADD =
tests_test_int_patterns_LDADD += libquicksorts.la

EXTRA_PROGRAMS += tests/test-big
CLEANFILES += tests/test-big
tests_test_big_SOURCES =
tests_test_big_SOURCES += tests/test-big.c
tests_test_big_DEPENDENCIES =
tests_test_big_DEPENDENCIES += libquicksorts.la
tests_test_big_LDADD =
tests_test_big_LDADD += libquicksorts.la

tests-clean:
	-rm -f tests/*.$(OBJEXT)

#--------------------------------------------------------------------------

nodist_pkgconfig_DATA += quicksorts.pc

#--------------------------------------------------------------------------
# local variables:
# coding: utf-8
# end:
