README
~~~~~~
launchmail - an SMTP client with a sendmail compatible wrapper

launchmail is a simple SMTP client. It prepares headers but otherwise
ignores the content of the message being sent. It doesn't do attachments or
MIME encoding. Some other software has to prepare the message. This just
sends the message to the SMTP server of your choice. It probably shouldn't
even prepare any headers (hence the -r and -N options).

To use launchmail as a drop-in replacement for sendmail(8), install
the sendmail wrapper script (to be found in the wrappers directory) as
/usr/sbin/sendmail and make sure that the environment variable
$SMTPSERVER is set. If $SMTPSERVER is not set, the sendmail wrapper
will use localhost as the default which will probably not be very useful.
The most seamless way to replace sendmail with launchmail is to edit
the sendmail wrapper so that it knows which SMTP server to connect to.

DESCRIPTION
~~~~~~~~~~~
usage: launchmail [options] [filename]
options:

      -h, --help                 - Print a help message then exit
      -V, --version              - Print a version message then exit
      -v, --verbose=level        - Set the verbosity level
      -d, --debug=level          - Set the debug level

      -f, --from=address         - Sender address
      -s, --subject=subject      - Message subject
      -t, --to=addresses         - Recipient address list (comma separated)
      -T, --tofile=filename      - Recipient address list filename
      -c, --cc=addresses         - Carbon Copy address list
      -C, --ccfile=filename      - Carbon Copy address list filename
      -b, --bcc=addresses        - Blind Carbon Copy address list
      -B, --bccfile=filename     - Blind Carbon Copy address list filename
      -r, --readto               - Read message for recipients
      -!, --sendbcc              - Don't remove Bcc header (when --readto)
      -g, --gmtime               - Add Date: header in gmtime
      -l, --localtime            - Add Date: header in localtime
      -x, --header="name: value" - Arbitrary header
      -X, --headerfile=filename  - Arbitrary header filename
      -m, --mailfrom=address     - SMTP mail from address
      -n, --hostname=hostname    - SMTP helo hostname
      -S, --server=hostname      - SMTP server to connect to
      -P, --port=#               - Port to use when connecting to SMTP server
      -o, --timeout=#            - Seconds to wait during SMTP dialogue
      -q, --quiet                - Remain silent when an error occurs
      -N, --noheaders            - Do not insert any headers

Launchmail is an STMP client.
See the launchmail(1) manpage for more information.

Name: launchmail
Version: 0.1
Date: 20001127
Author: raf <raf@raf.org>
URL: http://libslack.org/launchmail/

Copyright (C) 2000 raf <raf@raf.org>

This is free software released under the terms of the GPL:

    http://www.gnu.org/copyleft/gpl.html

There is no warranty; not even for merchantability or fitness
for a particular purpose.

Report bugs to raf <raf@raf.org>

INSTALL
~~~~~~~
Currently this is only known to work on Linux Redhat 6.0 and Solaris 2.6.
Compiling on Solaris 2.6 requires running conf/solaris in the source
directory. It is ready to compile on Linux as distributed so it is not
necessary to run conf/linux. There isn't a real configure script so you
will no doubt encounter problems on other systems. An ANSI C and POSIX
environment will help greatly.

If your system doesn't have snprintf(3), GNU getopt_long(3) or vsscanf(3),
uncomment the relevant lines in the libslack/macros.mk file to include them
in libslack.

If your system doesn't have POSIX 1003.2 compliant regex functions, either:
install the GNU implementation, ftp://ftp.gnu.org/gnu/regex/regex-0.12.tar.gz
[290K] (doesn't support internationalisation); or install Henry Spencer's
implementation, ftp://ftp.zoo.toronto.edu/pub/regex.shar [157K].

If you really, really, really don't want the regular expression functions,
uncomment REGEX_MISSING in libslack/macros.mk to enable the rest of the
library to be compiled.

If you have Linux system with a version of LinuxThreads that is older than 0.9,
I urge you to upgrade to the latest version (and the latest corresponding
version of glibc). They are available from http://ftp.gnu.org/pub/gnu/glibc/

To build and test:

        tar xzf launchmail-0.1.tar.gz
        cd launchmail-0.1
        conf/<your-system>         # if applicable (i.e. solaris)
        make depend
        make
        make test                  # note: compiling the tests can take ages

To install the launchmail program and its manpage:

        make install-launchmail

To install the sendmail wrapper:

        make install-wrappers

To install libslack and its manpages:

        make install-slack

To install both:

        make install

For more details:

        make help

The manpage for launchmail is launchmail(1). There is one manpage for each
module in libslack (rather than one for each function). They are daemon(3),
err(3), fio(3), hsort(3), lim(3), list(3), log(3), map(3), mem(3), msg(3),
net(3), opt(3), prog(3), prop(3), sig(3) and str(3). If necessary, the
manpages getopt(3), snprintf(3) and vsscanf(3) are created as well.

REQUIREMENTS
~~~~~~~~~~~~
Requires perl to run the scripts in the conf directory.
Requires gcc to compile the source.
Requires pod2man (comes with perl) to make the manpages.
Requires pod2html (comes with perl) to make the html manpages.
Requires perl and GNU tar to make the distribution.
Requires POSIX 1003.2 compliant regex functions. See INSTALL.
Requires libpthread.

COPYING
~~~~~~~
launchmail - an SMTP client with a sendmail compatible wrapper
Copyright (C) 2000 raf <raf@raf.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    or visit http://www.gnu.org/copyleft/gpl.html

libslack - A UNIX/C library of general utilities for programmers with slack
Copyright (C) 1999, 2000 raf <raf@raf.org>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    or visit http://www.gnu.org/copyleft/gpl.html

HISTORY
~~~~~~~
0.1
    - Initial version

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
URL: http://libslack.org/launchmail/
Date: 20001127
Author: raf <raf@raf.org>
