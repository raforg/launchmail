#
# launchmail - http://libwrappers.org/launchmail/
#
# Copyright (C) 1999, 2000 raf <raf@raf.org>
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

.PHONY: install-wrappers install-wrappers-bin

install-wrappers: install-wrappers-bin

install-wrappers-bin:
	install -m 555 $(WRAPPERS) $(WRAPPERS_INSDIR)

.PHONY: uninstall-wrappers uninstall-wrappers-bin

uninstall-wrappers: uninstall-wrappers-bin

uninstall-wrappers-bin:
	rm -f $(patsubst %, $(WRAPPERS_INSDIR)/%, $(WRAPPERS_MODULES))

# Present make targets separately in help if we are not alone
ifeq ($(WRAPPERS_IS_ROOT),)
WRAPPERS_HELP := 1
else
ifeq ($(WRAPPERS_HAS_SUBTARGETS), 1)
WRAPPERS_HELP := 1
endif
endif

ifeq ($(WRAPPERS_HELP), 1)
help::
	@echo "make install-wrappers          -- installs $(WRAPPERS_NAME)"; \
	echo "make install-wrappers-bin      -- installs $(WRAPPERS_NAME) in $(WRAPPERS_INSDIR)"; \
	echo "make uninstall-wrappers        -- uninstalls $(WRAPPERS_NAME)"; \
	echo "make uninstall-wrappers-bin    -- uninstalls $(WRAPPERS_NAME) from $(WRAPPERS_INSDIR)"; \
	echo
endif

help-macros::
	@echo "WRAPPERS_SRCDIR = $(WRAPPERS_SRCDIR)"; \
	echo "WRAPPERS_INSDIR = $(WRAPPERS_INSDIR)"; \
	echo "WRAPPERS_MODULES = $(WRAPPERS_MODULES)"; \
	echo "WRAPPERS = $(WRAPPERS)"; \
	echo

