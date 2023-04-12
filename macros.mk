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

# Uncomment this to disable debugging completely
#
# LAUNCH_DEFINES += -DNDEBUG

LAUNCH_NAME := launchmail
LAUNCH_VERSION := 0.2
LAUNCH_DATE := 20230330
LAUNCH_URL := https://libslack.org/launchmail
LAUNCH_ID := $(LAUNCH_NAME)-$(LAUNCH_VERSION)
LAUNCH_DIST := $(LAUNCH_ID).tar.gz

LAUNCH_DEFINES += -DLAUNCH_NAME=\"$(LAUNCH_NAME)\"
LAUNCH_DEFINES += -DLAUNCH_VERSION=\"$(LAUNCH_VERSION)\"
LAUNCH_DEFINES += -DLAUNCH_DATE=\"$(LAUNCH_DATE)\"
LAUNCH_DEFINES += -DLAUNCH_URL=\"$(LAUNCH_URL)\"

# Uncomment this if your system has POSIX threads reader/writer locks.
#
LAUNCH_DEFINES += -DHAVE_PTHREAD_RWLOCK=1

# Uncomment these as appropriate
#
LAUNCH_DEFINES += -DHAVE_SNPRINTF=1
LAUNCH_DEFINES += -DHAVE_VSSCANF=1
LAUNCH_DEFINES += -DHAVE_GETOPT_LONG=1
# LAUNCH_DEFINES += -DNO_POSIX_C_SOURCE=1
# LAUNCH_DEFINES += -DNO_POSIX_SOURCE=1
# LAUNCH_DEFINES += -DNO_XOPEN_SOURCE=1

LAUNCH_TARGET := $(LAUNCH_SRCDIR)/$(LAUNCH_NAME)
LAUNCH_MODULES := launchmail

LAUNCH_CFILES := $(patsubst %, $(LAUNCH_SRCDIR)/%.c, $(LAUNCH_MODULES))
LAUNCH_OFILES := $(patsubst %, $(LAUNCH_SRCDIR)/%.o, $(LAUNCH_MODULES))
LAUNCH_PODFILES := $(LAUNCH_CFILES)
LAUNCH_MANFILES := $(patsubst %.c, %.$(APP_MANSECT), $(LAUNCH_PODFILES))
LAUNCH_HTMLFILES := $(patsubst %.c, %.$(APP_MANSECT).html, $(LAUNCH_PODFILES))

ifeq ($(MAN_GZIP), 1)
LAUNCH_MANFILES := $(patsubst %, %.gz, $(LAUNCH_MANFILES))
endif

TAG_FILES += $(LAUNCH_HFILES) $(LAUNCH_CFILES)
DEPEND_CFILES += $(LAUNCH_CFILES)
DEPEND_HFILES += $(LAUNCH_HFILES)

ifeq ($(LAUNCH_SRCDIR), .)
LAUNCH_MAIN := 1
endif

ALL_TARGETS += launchmail
MAN_TARGETS += man-launchmail
HTML_TARGETS += html-launchmail
ifeq ($(LAUNCH_MAIN), 1)
INSTALL_TARGETS += install-launchmail
UNINSTALL_TARGETS += uninstall-launchmail
endif
DIST_TARGETS += dist-launchmail

CLEAN_FILES += $(LAUNCH_OFILES) $(LAUNCH_MANFILES) $(LAUNCH_HTMLFILES)
CLOBBER_FILES += $(LAUNCH_TARGET) $(LAUNCH_SRCDIR)/tags

# Uncomment these on MacOSX to create universal binaries
#
# LAUMCH_CCFLAGS += -arch x86_64 -arch i386 # -arch ppc -arch ppc64
# LAUMCH_LDFLAGS += -arch x86_64 -arch i386 # -arch ppc -arch ppc64

# Uncomment these on 64-bit OpenSolaris or Solaris10 to compile for 64-bit
#
# LAUNCH_CCFLAGS += -m64
# LAUNCH_LDFLAGS += -m64

LAUNCH_CPPFLAGS += $(LAUNCH_DEFINES) $(patsubst %, -I%, $(LAUNCH_INCDIRS))
LAUNCH_CCFLAGS += $(CCFLAGS)
# LAUNCH_CCFLAGS += -Wno-pointer-bool-conversion
# LAUNCH_LDFLAGS += -pthread
LAUNCH_CFLAGS += $(LAUNCH_CPPFLAGS) $(LAUNCH_CCFLAGS)
LAUNCH_LIBS += slack
LAUNCH_LIBS += pthread
LAUNCH_LIBS += util

# Uncomment this on Solaris for sockets
#
# LAUNCH_LIBS += xnet
# LAUNCH_LIBS += socket
# LAUNCH_LIBS += nsl

LAUNCH_LDFLAGS += $(patsubst %, -L%, $(LAUNCH_LIBDIRS)) $(patsubst %, -l%, $(LAUNCH_LIBS))

# Inherit $(CPPFLAGS), $(CFLAGS) and $(LDFLAGS) from the caller
#
LAUNCH_CPPFLAGS += $(CPPFLAGS)
LAUNCH_CFLAGS += $(CFLAGS)
LAUNCH_LDFLAGS += $(LDFLAGS)

# Include the libslack sub-target

SLACK_SRCDIR := libslack
SLACK_INCDIRS := $(SLACK_SRCDIR)
SLACK_LIBDIRS := $(SLACK_SRCDIR)
include $(SLACK_SRCDIR)/macros.mk
LAUNCH_SUBTARGETS += $(SLACK_TARGET)
LAUNCH_SUBDIRS += $(SLACK_SRCDIR) $(SLACK_SUBDIRS)

# Include the wrappers sub-target

WRAPPERS_SRCDIR := wrappers
include $(WRAPPERS_SRCDIR)/macros.mk
LAUNCH_SUBTARGETS += $(WRAPPERS_TARGET)
LAUNCH_SUBDIRS += $(WRAPPERS_SRCDIR) $(WRAPPERS_SUBDIRS)

