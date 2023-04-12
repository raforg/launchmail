#
# launchmail - https://libslack.org/launchmail
#
# Copyright (C) 2000, 2023 raf <raf@raf.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <https://www.gnu.org/licenses/>.
#
# 20230330 raf <raf@raf.org>
#

CC := gcc
# CC := cc
# CC := other
AR := ar
RANLIB := ranlib
POD2MAN := pod2man
POD2HTML := pod2html
GZIP := gzip -f -9

DESTDIR :=
PREFIX := /usr/local
# PREFIX := /usr/pkg
APP_INSDIR := $(PREFIX)/bin
LIB_INSDIR := $(PREFIX)/lib
MAN_SYSDIR := $(PREFIX)/share/man
MAN_LOCDIR := $(PREFIX)/share/man
ifeq ($(PREFIX),/usr)
MAN_INSDIR := $(MAN_SYSDIR)
else
MAN_INSDIR := $(MAN_LOCDIR)
endif
HDR_INSDIR := $(PREFIX)/include
APP_MANSECT := 1
LIB_MANSECT := 3
APP_MANDIR := $(MAN_INSDIR)/man$(APP_MANSECT)
LIB_MANDIR := $(MAN_INSDIR)/man$(LIB_MANSECT)
APP_MANSECTNAME := User Commands
LIB_MANSECTNAME := C Library Functions - libslack
MAN_GZIP := 1

CCFLAGS += -O3
CCFLAGS += -Wall -pedantic

# CCFLAGS += -xO4

CLEAN_FILES += tags core Makefile.bak .makefile.bak pod2htm*

LAUNCH_SRCDIR := .
LAUNCH_INCDIRS := libslack
LAUNCH_LIBDIRS := libslack
include $(LAUNCH_SRCDIR)/macros.mk

.PHONY: all ready test check man html install uninstall dist

all: ready $(ALL_TARGETS)
ready: $(READY_TARGETS)
check test: all $(TEST_TARGETS)
man: $(MAN_TARGETS)
html: $(HTML_TARGETS)
install: all $(INSTALL_TARGETS)
uninstall: $(UNINSTALL_TARGETS)
dist: $(DIST_TARGETS)

.PHONY: help help-macros depend clean clobber distclean

help::
	@echo "This makefile provides the following targets."; \
	echo; \
	echo " help                  -- shows this list of targets"; \
	echo " help-macros           -- shows the values of all make macros"; \
	echo " ready                 -- prepares the source directory for make"; \
	echo " all                   -- makes $(LAUNCH_TARGET) and $(LAUNCH_SUBTARGETS) (default)"; \
	echo " ready                 -- prepares the source directory for make"; \
	echo " test                  -- generates and performs library unit tests"; \
	echo " check                 -- same as test"; \
	echo " man                   -- generates all manpages"; \
	echo " html                  -- generates all manpages in html"; \
	echo " install               -- installs launchmail under $(PREFIX)"; \
	echo " uninstall             -- uninstalls launchmail from $(PREFIX)"; \
	echo " depend                -- generates source dependencies using makedepend"; \
	echo " tags                  -- generates a tags file using ctags"; \
	echo " clean                 -- removes object files, tags, core and Makefile.bak"; \
	echo " clobber               -- same as clean but also removes $(LAUNCH_TARGET), $(LAUNCH_SUBTARGETS) and tests"; \
	echo " distclean             -- same as clobber but also removes source dependencies"; \
	echo " dist                  -- creates the distribution: ../$(LAUNCH_DIST)"; \
	echo

help-macros::
	@echo "CC = $(CC)"; \
	echo "PREFIX = $(PREFIX)"; \
	echo "APP_INSDIR = $(APP_INSDIR)"; \
	echo "LIB_INSDIR = $(LIB_INSDIR)"; \
	echo "MAN_INSDIR = $(MAN_INSDIR)"; \
	echo "HDR_INSDIR = $(HDR_INSDIR)"; \
	echo "APP_MANSECT = $(APP_MANSECT)"; \
	echo "LIB_MANSECT = $(LIB_MANSECT)"; \
	echo "APP_MANDIR = $(APP_MANDIR)"; \
	echo "LIB_MANDIR = $(LIB_MANDIR)"; \
	echo "TAG_FILES = $(TAG_FILES)"; \
	echo "DEPEND_CFILES = $(DEPEND_CFILES)"; \
	echo "DEPEND_HFILES = $(DEPEND_HFILES)"; \
	echo "CCFLAGS = $(CCFLAGS)"; \
	echo "READY_TARGETS = $(READY_TARGETS)"; \
	echo "ALL_TARGETS = $(ALL_TARGETS)"; \
	echo "TEST_TARGETS = $(TEST_TARGETS)"; \
	echo "MAN_TARGETS = $(MAN_TARGETS)"; \
	echo "HTML_TARGETS = $(HTML_TARGETS)"; \
	echo "INSTALL_TARGETS = $(INSTALL_TARGETS)"; \
	echo "UNINSTALL_TARGETS = $(UNINSTALL_TARGETS)"; \
	echo "CLEAN_FILES = $(CLEAN_FILES)"; \
	echo "CLOBBER_FILES = $(CLOBBER_FILES)"; \
	echo "DIST_TARGETS = $(DIST_TARGETS)"; \
	echo

tags: $(TAG_FILES)
	@ctags $(TAG_FILES)

depend: ready $(DEPEND_CFILES) $(DEPEND_HFILES)
	@makedepend $(LAUNCH_CPPFLAGS) $(DEPEND_CFILES)

clean::
	@rm -rf $(CLEAN_FILES)

clobber::
	@rm -rf $(CLEAN_FILES) $(CLOBBER_FILES)

distclean:: clobber
	@perl -pi -e 'last if /[D]O NOT DELETE/;' $(patsubst %, %/Makefile, $(LAUNCH_SRCDIR) $(LAUNCH_SUBDIRS))

include $(LAUNCH_SRCDIR)/rules.mk

