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

WRAPPERS_INSDIR := $(DESTDIR)/usr/sbin

WRAPPERS_MODULES := sendmail # smtppush
WRAPPERS := $(patsubst %, $(WRAPPERS_SRCDIR)/%, $(WRAPPERS_MODULES))

ifeq ($(WRAPPERS_SRCDIR), .)
WRAPPERS_MAIN := 1
endif

ifeq ($(WRAPPERS_MAIN), 1)
INSTALL_TARGETS += install-wrappers
UNINSTALL_TARGETS += uninstall-wrappers
endif

