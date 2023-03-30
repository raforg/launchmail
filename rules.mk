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
# 20230330 raf <raf@raf.org>
#

ifneq ($(LAUNCH_TARGET),./$(LAUNCH_NAME))

.PHONY: $(LAUNCH_NAME)

$(LAUNCH_NAME): $(LAUNCH_TARGET)

endif

$(LAUNCH_TARGET): $(LAUNCH_OFILES) $(LAUNCH_SUBTARGETS)
	$(CC) -o $(LAUNCH_TARGET) $(LAUNCH_OFILES) $(LAUNCH_LDFLAGS)

.PHONY: man-launchmail html-launchmail

man-launchmail: $(LAUNCH_MANFILES)

html-launchmail: $(LAUNCH_HTMLFILES)

.PHONY: install-launchmail install-launchmail-bin install-launchmail-man

install-launchmail: install-launchmail-bin install-launchmail-man

install-launchmail-bin:
	mkdir -p $(DESTDIR)$(APP_INSDIR)
	install -m 755 $(LAUNCH_TARGET) $(DESTDIR)$(APP_INSDIR)
	case "$$DEB_BUILD_OPTIONS" in *nostrip*);; *) strip $(patsubst %, $(DESTDIR)$(APP_INSDIR)/%, $(notdir $(LAUNCH_TARGET)));; esac

install-launchmail-man: man-launchmail
	@mkdir -p $(DESTDIR)$(APP_MANDIR); \
	install -m 644 $(LAUNCH_MANFILES) $(DESTDIR)$(APP_MANDIR)

.PHONY: uninstall-launchmail uninstall-launchmail-bin uninstall-launchmail-man

uninstall-launchmail: uninstall-launchmail-bin uninstall-launchmail-man

uninstall-launchmail-bin:
	rm -f $(patsubst %, $(DESTDIR)$(APP_INSDIR)/%, $(notdir $(LAUNCH_TARGET)))

uninstall-launchmail-man:
	rm -f $(patsubst %, $(APP_MANDIR)/%, $(notdir $(LAUNCH_MANFILES)))

.PHONY: dist-launhmail

dist-launchmail: distclean
	@set -e; \
	up="`pwd`/.."; \
	cd $(LAUNCH_SRCDIR); \
	src=`basename \`pwd\``; \
	dst=$(LAUNCH_ID); \
	cd ..; \
	[ "$$src" != "$$dst" -a ! -d "$$dst" ] && ln -s $$src $$dst; \
	tar chzf $$up/$(LAUNCH_DIST) --exclude='.git*' $$dst; \
	[ -h "$$dst" ] && rm -f $$dst; \
	tar tzfv $$up/$(LAUNCH_DIST); \
	ls -l $$up/$(LAUNCH_DIST)

# Present make targets separately in help if we are not alone

ifneq ($(LAUNCH_SRCDIR), .)
LAUNCH_SPECIFIC_HELP := 1
else
ifeq ($(LAUNCH_SUBTARGETS),)
LAUNCH_SPECIFIC_HELP := 1
endif
endif

ifeq ($(LAUNCH_SPECIFIC_HELP), 1)
help::
	@echo " $(LAUNCH_NAME)                -- makes $(LAUNCH_TARGET) and $(LAUNCH_SUBTARGETS)"; \
	echo " man-$(LAUNCH_NAME)            -- makes the $(LAUNCH_NAME) manpages"; \
	echo " html-$(LAUNCH_NAME)           -- makes the $(LAUNCH_NAME) manpages in html"; \
	echo " install-launchmail        -- installs $(LAUNCH_NAME) and its manpage"; \
	echo " install-launchmail-bin    -- installs $(LAUNCH_NAME) in $(APP_INSDIR)"; \
	echo " install-launchmail-man    -- installs the $(LAUNCH_NAME) manpage in $(APP_MANDIR)"; \
	echo " uninstall-launchmail      -- uninstalls $(LAUNCH_NAME) and its manpage"; \
	echo " uninstall-launchmail-bin  -- uninstalls $(LAUNCH_NAME) from $(APP_INSDIR)"; \
	echo " uninstall-launchmail-man  -- uninstalls the $(LAUNCH_NAME) manpage from $(APP_MANDIR)"; \
	echo " dist-launchmail           -- makes a source tarball for launchmail"; \
	echo
endif

help-macros::
	@echo "LAUNCH_NAME = $(LAUNCH_NAME)"; \
	echo "LAUNCH_VERSION = $(LAUNCH_VERSION)"; \
	echo "LAUNCH_ID = $(LAUNCH_ID)"; \
	echo "LAUNCH_DIST = $(LAUNCH_DIST)"; \
	echo "LAUNCH_TARGET = $(LAUNCH_TARGET)"; \
	echo "LAUNCH_MODULES = $(LAUNCH_MODULES)"; \
	echo "LAUNCH_SRCDIR = $(LAUNCH_SRCDIR)"; \
	echo "LAUNCH_INCDIRS = $(LAUNCH_INCDIRS)"; \
	echo "LAUNCH_LIBDIRS = $(LAUNCH_LIBDIRS)"; \
	echo "LAUNCH_LIBS = $(LAUNCH_LIBS)"; \
	echo "LAUNCH_CFILES = $(LAUNCH_CFILES)"; \
	echo "LAUNCH_OFILES = $(LAUNCH_OFILES)"; \
	echo "LAUNCH_HFILES = $(LAUNCH_HFILES)"; \
	echo "LAUNCH_PODFILES = $(LAUNCH_PODFILES)"; \
	echo "LAUNCH_MANFILES = $(LAUNCH_MANFILES)"; \
	echo "LAUNCH_HTMLFILES = $(LAUNCH_HTMLFILES)"; \
	echo "LAUNCH_DEFINES = $(LAUNCH_DEFINES)"; \
	echo "LAUNCH_CPPFLAGS = $(LAUNCH_CPPFLAGS)"; \
	echo "LAUNCH_CCFLAGS = $(LAUNCH_CCFLAGS)"; \
	echo "LAUNCH_CFLAGS = $(LAUNCH_CFLAGS)"; \
	echo "LAUNCH_LDFLAGS = $(LAUNCH_LDFLAGS)"; \
	echo "LAUNCH_SUBTARGETS = $(LAUNCH_SUBTARGETS)"; \
	echo "LAUNCH_SUBDIRS = $(LAUNCH_SUBDIRS)"; \
	echo

include $(SLACK_SRCDIR)/rules.mk
include $(WRAPPERS_SRCDIR)/rules.mk

$(LAUNCH_SRCDIR)/%.o: $(LAUNCH_SRCDIR)/%.c
	$(CC) $(LAUNCH_CFLAGS) -o $@ -c $<

$(LAUNCH_SRCDIR)/%.$(APP_MANSECT): $(LAUNCH_SRCDIR)/%.c
	$(POD2MAN) --section=$(APP_MANSECT) --center='$(APP_MANSECTNAME)' --name=$(shell echo $(LAUNCH_NAME) | tr abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ) --release=$(LAUNCH_ID) --date=$(LAUNCH_DATE) --quotes=none $< > $@

$(LAUNCH_SRCDIR)/%.gz: $(LAUNCH_SRCDIR)/%
	$(GZIP) $<

$(LAUNCH_SRCDIR)/%.$(APP_MANSECT).html: $(LAUNCH_SRCDIR)/%.c
	$(POD2HTML) --noindex < $< > $@ 2>/dev/null

