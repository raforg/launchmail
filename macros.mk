#
# launchmail - http://libslack.org/libslack/
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

# Uncomment this to disable debugging completely
#
# LAUNCH_DEFINES += -DNDEBUG

LAUNCH_NAME := launchmail
LAUNCH_VERSION := 0.1
LAUNCH_DATE := 20001127
LAUNCH_ID := $(LAUNCH_NAME)-$(LAUNCH_VERSION)
LAUNCH_DIST := $(LAUNCH_ID).tar.gz
LAUNCH_HAS_SUBTARGETS := 1
LAUNCH_DEFINES += -DLAUNCH_NAME=\"$(LAUNCH_NAME)\"
LAUNCH_DEFINES += -DLAUNCH_VERSION=\"$(LAUNCH_VERSION)\"
LAUNCH_DEFINES += -DLAUNCH_DATE=\"$(LAUNCH_DATE)\"

LAUNCH_TARGET := $(LAUNCH_SRCDIR)/$(LAUNCH_NAME)
LAUNCH_MODULES := launchmail

LAUNCH_CFILES := $(patsubst %, $(LAUNCH_SRCDIR)/%.c, $(LAUNCH_MODULES))
LAUNCH_OFILES := $(patsubst %, $(LAUNCH_SRCDIR)/%.o, $(LAUNCH_MODULES))
LAUNCH_PODFILES := $(LAUNCH_CFILES)
LAUNCH_MANFILES := $(patsubst %.c, %.$(APP_MANSECT), $(LAUNCH_PODFILES))
LAUNCH_HTMLFILES := $(patsubst %.c, %.$(APP_MANSECT).html, $(LAUNCH_PODFILES))

TAG_FILES += $(LAUNCH_HFILES) $(LAUNCH_CFILES)
DEPEND_CFILES += $(LAUNCH_CFILES)
DEPEND_HFILES += $(LAUNCH_HFILES)

ALL_TARGETS += launchmail
MAN_TARGETS += man-launchmail
HTML_TARGETS += html-launchmail
INSTALL_TARGETS += install-launchmail
UNINSTALL_TARGETS += uninstall-launchmail

CLEAN_FILES += $(LAUNCH_OFILES) $(LAUNCH_MANFILES) $(LAUNCH_HTMLFILES)
CLOBBER_FILES += $(LAUNCH_TARGET) $(LAUNCH_SRCDIR)/tags

LAUNCH_CPPFLAGS += $(LAUNCH_DEFINES) $(patsubst %, -I%, $(LAUNCH_INCDIRS))
LAUNCH_CCFLAGS += $(CCFLAGS)
LAUNCH_CFLAGS += $(LAUNCH_CPPFLAGS) $(LAUNCH_CCFLAGS)
LAUNCH_LIBS += slack pthread

# Uncomment this on Solaris for sockets
#
# LAUNCH_LIBS += xnet

LAUNCH_LDFLAGS += $(patsubst %, -L%, $(LAUNCH_LIBDIRS)) $(patsubst %, -l%, $(LAUNCH_LIBS))

SLACK_SRCDIR := libslack
SLACK_INCDIRS := libslack
SLACK_LIBDIRS := libslack
include $(SLACK_SRCDIR)/macros.mk

WRAPPERS_SRCDIR := wrappers
include $(WRAPPERS_SRCDIR)/macros.mk

LAUNCH_SUBMODULES := $(SLACK_TARGET) $(WRAPPERS_TARGET)

