#
# launchmail - http://libslack.org/launchmail/
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
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# or visit http://www.gnu.org/copyleft/gpl.html
#

# 20001127 raf <raf@raf.org>

ifneq ($(LAUNCH_TARGET),./$(LAUNCH_NAME))

.PHONY: $(LAUNCH_NAME)

$(LAUNCH_NAME): $(LAUNCH_TARGET)

endif

$(LAUNCH_TARGET): $(LAUNCH_OFILES) $(LAUNCH_SUBMODULES)
	$(CC) -o $(LAUNCH_TARGET) $(LAUNCH_OFILES) $(LAUNCH_LDFLAGS)

# The following had to be moved to the bottom because make
# too clever for its own good :)
#
#$(LAUNCH_SRCDIR)/%.o: $(LAUNCH_SRCDIR)/%.c
#	$(CC) $(LAUNCH_CFLAGS) -o $@ -c $<

.PHONY: man-launchmail html-launchmail

man-launchmail: $(LAUNCH_MANFILES)

$(LAUNCH_SRCDIR)/%.$(APP_MANSECT): $(LAUNCH_SRCDIR)/%.c
	pod2man --center='$(APP_MANSECTNAME)' --section=$(APP_MANSECT) $< > $@

html-launchmail: $(LAUNCH_HTMLFILES)

$(LAUNCH_SRCDIR)/%.$(APP_MANSECT).html: $(LAUNCH_SRCDIR)/%.c
	pod2html --noindex < $< > $@ 2>/dev/null

.PHONY: install-launchmail install-launchmail-bin install-launchmail-man

install-launchmail: install-launchmail-bin install-launchmail-man

install-launchmail-bin:
	install -m 555 $(LAUNCH_TARGET) $(APP_INSDIR)
	strip $(patsubst %, $(APP_INSDIR)/%, $(notdir $(LAUNCH_TARGET)))

install-launchmail-man: man-launchmail
	install -m 444 $(LAUNCH_MANFILES) $(APP_MANDIR)

.PHONY: uninstall-launchmail uninstall-launchmail-bin uninstall-launchmail-man

uninstall-launchmail: uninstall-launchmail-bin uninstall-launchmail-man

uninstall-launchmail-bin:
	rm -f $(patsubst %, $(APP_INSDIR)/%, $(notdir $(LAUNCH_TARGET)))

uninstall-launchmail-man:
	rm -f $(patsubst %, $(APP_MANDIR)/%, $(notdir $(LAUNCH_MANFILES)))

# Present make targets separately in help if we are not alone
ifeq ($(LAUNCH_IS_ROOT), undefined)
LAUNCH_HELP := 1
else
ifeq ($(LAUNCH_HAS_SUBTARGETS), 1)
LAUNCH_HELP := 1
endif
endif

ifeq ($(LAUNCH_HELP), 1)
help::
	@echo "make $(LAUNCH_NAME)                -- makes $(LAUNCH_TARGET) and $(LAUNCH_SUBMODULES)"; \
	echo "make man-$(LAUNCH_NAME)            -- makes the $(LAUNCH_NAME) manpages"; \
	echo "make html-$(LAUNCH_NAME)           -- makes the $(LAUNCH_NAME) manpages in html"; \
	echo "make install-launchmail        -- installs $(LAUNCH_NAME) and its manpage"; \
	echo "make install-launchmail-bin    -- installs $(LAUNCH_NAME) in $(APP_INSDIR)"; \
	echo "make install-launchmail-man    -- installs the $(LAUNCH_NAME) manpage in $(APP_MANDIR)"; \
	echo "make uninstall-launchmail      -- uninstalls $(LAUNCH_NAME) and its manpage"; \
	echo "make uninstall-launchmail-bin  -- uninstalls $(LAUNCH_NAME) from $(APP_INSDIR)"; \
	echo "make uninstall-launchmail-man  -- uninstalls the $(LAUNCH_NAME) manpage from $(APP_MANDIR)"; \
	echo
endif

help-macros::
	@echo "LAUNCH_NAME = $(LAUNCH_NAME)"; \
	echo "LAUNCH_VERSION = $(LAUNCH_VERSION)"; \
	echo "LAUNCH_ID = $(LAUNCH_ID)"; \
	echo "LAUNCH_DIST = $(LAUNCH_DIST)"; \
	echo "LAUNCH_TARGET = $(LAUNCH_TARGET)"; \
	echo "LAUNCH_MODULES = $(LAUNCH_MODULES)"; \
	echo "LAUNCH_SUBMODULES = $(LAUNCH_SUBMODULES)"; \
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
	echo

include $(SLACK_SRCDIR)/rules.mk
include $(WRAPPERS_SRCDIR)/rules.mk

# This is down here because make is too clever for its own good :)

$(LAUNCH_SRCDIR)/%.o: $(LAUNCH_SRCDIR)/%.c
	$(CC) $(LAUNCH_CFLAGS) -o $@ -c $<

