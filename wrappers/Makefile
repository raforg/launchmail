#
# launchmail - https://libslack.org/launchmail
#
# Copyright (C) 2000 raf <raf@raf.org>
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
# 20001127 raf <raf@raf.org>
#

DESTDIR :=
PREFIX := /usr/local
# PREFIX := /usr/pkg
APP_INSDIR := $(PREFIX)/sbin

WRAPPERS_SRCDIR := .
include $(WRAPPERS_SRCDIR)/macros.mk

.PHONY: all install uninstall

all: $(ALL_TARGETS)
install: all $(INSTALL_TARGETS)
uninstall: $(UNINSTALL_TARGETS)

.PHONY: help help-macros

help::
	@echo "This makefile provides the following targets."; \
	echo; \
	echo " help                  -- shows this list of targets"; \
	echo " help-macros           -- shows the values of all make macros"; \
	echo " all                   -- does nothing (default)"; \
	echo " install               -- installs wrapper(s) under $(PREFIX)"; \
	echo " uninstall             -- uninstalls wrapper(s) from $(PREFIX)"; \
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

include $(WRAPPERS_SRCDIR)/rules.mk

