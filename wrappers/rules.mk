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

.PHONY: install-wrappers install-wrappers-bin

install-wrappers: install-wrappers-bin

install-wrappers-bin:
	mkdir -p $(WRAPPERS_INSDIR)
	install -m 555 $(WRAPPERS) $(WRAPPERS_INSDIR)

.PHONY: uninstall-wrappers uninstall-wrappers-bin

uninstall-wrappers: uninstall-wrappers-bin

uninstall-wrappers-bin:
	rm -f $(patsubst %, $(WRAPPERS_INSDIR)/%, $(WRAPPERS_MODULES))

# Present make targets separately in help if we are not alone

ifneq ($(WRAPPERS_SRCDIR), .)
WRAPPERS_SPECIFIC_HELP := 1
else
ifneq ($(WRAPPERS_SUBTARGETS),)
WRAPPERS_SPECIFIC_HELP := 1
endif
endif

ifeq ($(WRAPPERS_SPECIFIC_HELP), 1)
help::
	@echo " install-wrappers       -- installs $(WRAPPERS_MODULES)"; \
	echo " install-wrappers-bin   -- installs $(WRAPPERS_MODULES) in $(WRAPPERS_INSDIR)"; \
	echo " uninstall-wrappers     -- uninstalls $(WRAPPERS_MODULES)"; \
	echo " uninstall-wrappers-bin -- uninstalls $(WRAPPERS_MODULES) from $(WRAPPERS_INSDIR)"; \
	echo
endif

help-macros::
	@echo "WRAPPERS_SRCDIR = $(WRAPPERS_SRCDIR)"; \
	echo "WRAPPERS_INSDIR = $(WRAPPERS_INSDIR)"; \
	echo "WRAPPERS_MODULES = $(WRAPPERS_MODULES)"; \
	echo "WRAPPERS = $(WRAPPERS)"; \
	echo

