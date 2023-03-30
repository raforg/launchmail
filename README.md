# README

*launchmail* - an SMTP client with a *sendmail*-compatible wrapper

# INTRODUCTION

*Launchmail* is a simple SMTP client. It prepares headers but otherwise
ignores the content of the message being sent. It doesn't do attachments or
MIME encoding. Some other software has to prepare the message. This just
sends the message to the SMTP server of your choice. It probably shouldn't
even prepare any headers (hence the `-r` and `-N` options).

To use *launchmail* as a drop-in replacement for *sendmail(8)*, install
the *sendmail* wrapper script (to be found in the `wrappers` directory) as
`/usr/sbin/sendmail` and make sure that the environment variable
`$SMTPSERVER` is set. If `$SMTPSERVER` is not set, the *sendmail* wrapper
will use `localhost` as the default which will probably not be very useful.
The most seamless way to use *launchmail* as *sendmail* is to edit
the *sendmail* wrapper so that it knows which SMTP server to connect to.

# SYNOPSIS

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
    See the launchmail(1) manual entry for more information.

# DOCUMENTATION

*Launchmail*'s documentation can be read here:

- <https://libslack.org/launchmail/manual/launchmail.1.html>
- <https://raf.org/launchmail/manual/launchmail.1.html>

# DOWNLOAD

*Launchmail*'s source distribution can be downloaded from these locations:

- <https://libslack.org/launchmail/download/launchmail-0.2.tar.gz>
- <https://raf.org/launchmail/download/launchmail-0.2.tar.gz>
- <https://github.com/raforg/launchmail/releases/download/v0.2/launchmail-0.2.tar.gz>

This is free software released under the terms of the GNU General Public
Licence version 3 or later (*GPLv3+*).

# INSTALL

*Launchmail* should be easy to install on at least the following systems:

	Linux, FreeBSD, OpenBSD, NetBSD, macOS, Solaris,
	OpenSolaris, kFreeBSD, GNU/Hurd

For these systems, just run the `configure` script in the source directory.
It will run the appropriate script in the `conf` directory for the current
host. Perl and GNU make are required for building.

Note: There isn't an *autoconf* configure script so you will no doubt encounter
problems on other systems. An ISO C and POSIX/XPG4 environment will help
greatly. If your system doesn't have *snprintf(3)*, GNU *getopt_long(3)*,
*vsscanf(3)*, *strcasecmp(3)*, *strncasecmp(3*), *strlcpy(3)* or *strlcat(3)*,
uncomment the relevant lines in the `libslack/config.h` file to include them
in *libslack*.

If your system doesn't have POSIX 1003.2 compliant regex functions, or they
are buggy, either: install the GNU implementation,
ftp://ftp.gnu.org/gnu/regex/regex-0.12.tar.gz [290K]
(doesn't support internationalisation);
or install Henry Spencer's implementation,
ftp://ftp.zoo.toronto.edu/pub/regex.shar [157K].

To build *launchmail*:

        tar xzf launchmail-0.2.tar.gz
        cd launchmail-0.2
        ./configure # iff linux, openbsd, freebsd, netbsd, macosx, solaris, gnuhurd or kfreebsd
        make        # must be GNU make
        sudo make install

This will install (approximately, depending on the operating system):

        /usr/local/bin/launchmail
        /usr/local/share/man/man1/launchmail.1

To uninstall *launchmail*:

        sudo make uninstall

To install/uninstall under `/usr` instead of `/usr/local`:

        sudo make PREFIX=/usr install
        sudo make PREFIX=/usr uninstall

To install the *sendmail* wrapper:

        sudo make install-wrappers

To uninstall the *sendmail* wrapper:

        sudo make uninstall-wrappers

To check out the `configure` script which can override paths and features:

        ./configure --help

To see what other things the `Makefile` can do:

        make help

REQUIREMENTS
============

- Requires perl to run the configure scripts.
- Requires a C compiler to compile the source.
- Requires pod2man (comes with perl) to make the manual entry.
- Requires pod2html (comes with perl) to make the html manual entry.
- Requires perl and GNU tar to make the distribution.
- Requires POSIX 1003.2 compliant regex functions. See INSTALL.
- Requires libpthread.

--------------------------------------------------------------------------------

    URL: https://libslack.org/launchmail
    URL: https://raf.org/launchmail
    GIT: https://github.com/raforg/launchmail
    GIT: https://codeberg.org/raforg/launchmail
    Date: 20230330
    Author: raf <raf@raf.org>

