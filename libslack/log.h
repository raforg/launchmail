/*
* libslack - http://libslack.org/
*
* Copyright (C) 1999, 2000 raf <raf@raf.org>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
* or visit http://www.gnu.org/copyleft/gpl.html
*
* 20000902 raf <raf@raf.org>
*
*/

#ifndef LIBSLACK_LOG_H
#define LIBSLACK_LOG_H

#include <sys/syslog.h>

#include <slack/hdr.h>

__START_DECLS
int syslog_lookup_facility __PROTO ((const char *facility));
int syslog_lookup_priority __PROTO ((const char *priority));
const char *syslog_facility_str __PROTO ((int spec));
const char *syslog_priority_str __PROTO ((int spec));
int syslog_parse __PROTO ((const char *spec, int *facility, int *priority));
__END_DECLS

#endif

/* vi:set ts=4 sw=4: */
