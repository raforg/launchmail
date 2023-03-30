/*
* launchmail - https://libslack.org/launchmail
*
* Copyright (C) 2000 raf <raf@raf.org>
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
* along with this program; if not, see <https://www.gnu.org/licenses/>.
*
* 20230330 raf <raf@raf.org>
*/

/*

=head1 NAME

I<launchmail> - an SMTP client with a I<sendmail>-compatible wrapper

=head1 SYNOPSIS

 launchmail [options] [filename]
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
  -N, --noheaders            - Do not insert any headers
  -q, --quiet                - Remain silent when an error occurs

=head1 DESCRIPTION

I<launchmail> is a simple SMTP client. It prepares headers but otherwise
ignores the content of the message being sent. It doesn't do attachments or
MIME encoding. Some other software has to prepare the message. This just
sends the message to the SMTP server of your choice. It probably shouldn't
even prepare any headers (hence the C<-r> and C<-N> options).

To use I<launchmail> as a drop-in replacement for I<sendmail(8)>, install
the I<sendmail> wrapper script as C</usr/sbin/sendmail> and make sure that
the environment variable C<$SMTPSERVER> is set. If C<$SMTPSERVER> is not
set, the C<sendmail> wrapper will use C<localhost> as the default which will
probably not be very useful. The most seamless way to replace I<sendmail>
with I<launchmail> is to edit the I<sendmail> wrapper so that it knows which
SMTP server to connect to.

=head1 OPTIONS

=over 4

=item C<-h>, C<--help>

Display a help message and exit.

=item C<-V>, C<--version>

Display a version message and exit.

=item C<-v>I<[level]>, C<--verbose>I<[=level]>

Set the message verbosity level to C<level>. I<launchmail> does
not have any verbose messages so this has no effect.

=item C<-d>I<[level]>, C<--debug>I<[=level]>

Set the debug message level to C<level>. Set to level 1 for a trace of
what I<launchmail> is doing. Set to level 2 to include a trace when
I<launchmail> is waiting.

=item C<-f>I<address>, C<--from=>I<address>

Specify the sender address. This address is used as the C<MAIL FROM:>
address during the SMTP dialogue (unless C<--mailfrom> option is supplied)
and it is used to create the C<From:> header (unless the C<--noheaders>
option is supplied). If this option is not supplied, its value is determined
automatically.

=item C<-s>I<subject>, C<--subject=>I<subject>

Specify the subject of the message. This is used to create the C<Subject:>
header (unless the C<--noheaders> option is supplied).

=item C<-t>I<addresses>, C<--to=>I<addresses>

Specify primary recipients for the message. Multiple addresses may be given
as a comma separated list or with multiple C<--to> options. The addresses
are used as C<RCPT TO:> addresses during the SMTP dialogue and they are used
to create the C<To:> header (unless the C<--noheaders> option is supplied).

=item C<-T>I<filename>, C<--tofile=>I<filename>

Specify the name of a file containing primary recipient addresses. The
addresses in the file are used as C<RCPT TO:> addresses during the SMTP
dialogue and they are used to create C<To:> headers (unless the
C<--noheaders> option is supplied).

=item C<-c>I<addresses>, C<--cc=>I<addresses>

Specify secondary recipients for the message. Multiple addresses may be
given as a comma separated list or with multiple C<--cc> options. The
addresses are used as C<RCPT TO:> addresses during the SMTP dialogue and
they are used to create the C<Cc:> header (unless the C<--noheaders> option
is supplied).

=item C<-C>I<filename>, C<--ccfile=>I<filename>

Specify the name of a file containing secondary recipient addresses. The
addresses in the file are used as C<RCPT TO:> addresses during the SMTP
dialogue and they are used to create C<Cc:> headers (unless the
C<--noheaders> option is supplied).

=item C<-b>I<addresses>, C<--bcc=>I<addresses>

Specify blind carbon copy recipients for the message. Multiple addresses may
be given as a comma separated list or with multiple C<--bcc> options. The
addresses are only used as C<RCPT TO:> addresses during the SMTP dialogue.

=item C<-B>I<filename>, C<--bccfile=>I<filename>

Specify the name of a file containing blind recipient addresses. The
addresses in the file are only used as C<RCPT TO:> addresses during the SMTP
dialogue.

=item C<-r>, C<--readto>

Scan the header section in the message for C<To:>, C<Cc:> and C<Bcc:>
headers and use the addresses found as C<RCPT TO:> addresses suring the SMTP
dialogue. Exclude any C<Bcc:> header found when sending the message. This
option implies C<--noheaders>.

=item C<-!>, C<--sendbcc>

Send the C<Bcc:> header with the message. Normally, a C<Bcc:> header is not
generated or sent to the SMTP server, whether it originated from a C<--bcc>
or C<--bccfile> option or from the header section of the message itself
(with the C<--readto> option). This option causes the C<Bcc:> header to be
sent to the SMTP server. There's probably no need for this.

=item C<-g>, C<--gmtime>

Adds a C<Date:> header to the message in GMT. This is not done by default.

=item C<-l>, C<--localtime>

Adds a C<Date:> header to the message in the local timezone. This is not
done by default.

=item C<-x>I<'name: value'>, C<--header=>I<'name: value'>

Specify an arbitrary header to be included in the message (unless the
C<--noheaders> option is supplied). For example:

 -x'Organization: n. the act or process of organizing or of being organized'
 --header='Sender: sender@f.q.d.n'
 --header='X-Loop: BeenHere'

=item C<-X>I<filename>, C<--headerfile=>I<filename>

Specify the name of a file containing arbitrary headers to be included in
the message (unless the C<--noheaders> option is supplied).

=item C<-m>I<address>, C<--mailfrom=>I<address>

Specify the envelope sender address. This address is only used as the C<MAIL
FROM:> address during the SMTP dialogue. If this option is not supplied, the
address given in the C<-f> option (or determined automatically) is used
instead.

=item C<-n>I<hostname>, C<--hostname=>I<hostname>

Specify the local host's name. This is used as the C<HELO> host during the
SMTP dialogue. If this option is not supplied, it's value is determined
automatically.

=item C<-S>I<hostname>, C<--server=>I<hostname>

Specify the SMTP server host to connect to.

=item C<-P>I<#>, C<--port=>I<#>

Specify the tcp port number to connect to when connecting to the SMTP server.

=item C<-o>I<#>, C<--timeout=>I<#>

Specify the number of seconds to wait for responses from the SMTP server.
This defaults to 10. There's probably no need to change this.

=item C<-N>, C<--noheaders>

Do not insert any headers into the message.

=item C<-q>, C<--quiet>

Remain silent when an error occurs.

=back

=head1 FILES

The C<--tofile>, C<--ccfile>, C<--bccfile> and C<--headerfile> options take
a file name as an argument. I<launchmail> will use the contents of these
files if and only if it deems them to be safe. This means that neither the
files themselves nor the directories in which they reside are group or world
writable.

The files may contain comments (C<`#'> until the end of the line) and lines
may be continued by placing a C<`\'> at the end of the line. There may be
whitespace after the C<`\'> and even a comment but nothing else.

Addresses in the recipient files may appear one per line or many per line,
comma separated. Headers in the header file must appear one per line.

=head1 EXAMPLES

Send message to me@home with subject and date headers:

    launchmail -S smtphost -t me@home -s subject -g message

Send message to the recipients in its own headers:

    launchmail -S smtphost -r message

=head1 SEE ALSO

L<mutt(1)|mutt(1)>,
L<sendmail(8)|sendmail(8)>,
L<qmail(8)|qmail(8)>,
L<postfix(8)|postfix(8)>,
L<metamail(1)|metamail(1)>,
L<metasend(1)|metasend(1)>,
L<uuencode(1)|uuencode(1)>,
L<mpack(1)|mpack(1)>

=head1 AUTHOR

20230330 raf <raf@raf.org>

=cut

*/

#ifndef _BSD_SOURCE
#define _BSD_SOURCE /* For SIGWINCH and CEOF on OpenBSD-4.7 */
#endif

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE /* New name for _BSD_SOURCE */
#endif

#ifndef __BSD_VISIBLE
#define __BSD_VISIBLE 1 /* For SIGWINCH on FreeBSD-8.0 */
#endif

#ifndef _NETBSD_SOURCE
#define _NETBSD_SOURCE /* For CEOF, chroot() on NetBSD-5.0.2 */
#endif

#include <slack/std.h>
#include <slack/lib.h>
#include <sys/utsname.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <pwd.h>

static struct
{
	const char *message;
	const char *from;
	const char *subject;
	List *to;
	List *cc;
	List *bcc;
	List *headers;
	const char *mailfrom;
	const char *hostname;
	const char *server;
	int port;
	long timeout;
	int quiet;
	int noheaders;
	int readto;
	int sendbcc;
	int gmtime;
	int localtime;
}
g =
{
	null, /* message */
	null, /* from */
	null, /* subject */
	null, /* to */
	null, /* cc */
	null, /* bcc */
	null, /* headers */
	null, /* mailfrom */
	null, /* hostname */
	null, /* server */
	0,    /* port */
	10,   /* timeout */
	0,    /* quiet */
	0,    /* noheaders */
	0,    /* readto */
	0,    /* sendbcc */
	0,    /* gmtime */
	0     /* localtime */
};

static const char * const squote = "\"[(";
static const char * const equote = "\"])";
static const int comment[3] = { 0, 0, 1 };

#define fail { close(smtp); return -1; }
#define try_cleanup(action, cleanup) if ((action) == -1) { debugsys((1, "%s failed", #action)) cleanup; fail }
#define try_str(action) if (!(action)) { debug((1, "%s failed", #action)) fail }
#define try(action) try_cleanup(action, /* nop */)
#define try_send(args) try(net_send args)
#define try_send_cleanup(args, cleanup) try_cleanup(net_send args, cleanup)
#define try_expect(args, cnv, resp) \
	try(rc = net_expect args) \
	if (rc != (cnv) || code != (resp)) { debug((1, "SMTP protocol error")) close(smtp); return set_errno(EPROTO); }

int header(int smtp, List *headers, const char *name)
{
	String *tmp, *header;
	List *para;

	if (!headers || !list_length(headers))
		return 0;

	if (!(tmp = str_join(headers, ", ")))
		return set_errno(ENOMEM);

	para = str_fmt(tmp, 72, ALIGN_LEFT);
	str_release(tmp);
	if (!para)
		return set_errno(ENOMEM);

	header = str_join(para, "\r\n\t");
	list_release(para);
	if (!header)
		return set_errno(ENOMEM);

	debug((1, "Sending: %s: %s", name, cstr(header)))
	try_send_cleanup((smtp, g.timeout, "%s: %s\r\n", name, cstr(header)), str_release(header))
	str_release(header);

	return 0;
}

int headers(int smtp, List *headers)
{
	while (list_has_next(headers))
	{
		String *header = list_next(headers);
		debug((1, "Sending: %s", cstr(header)))
		try_send((smtp, g.timeout, "%s\r\n", cstr(header)))
	}

	return 0;
}

int body(int smtp, FILE *input)
{
	char buf[BUFSIZ];
	int newline = 1;

	while (fgetline(buf, BUFSIZ - 2, input))
	{
		size_t len = strlen(buf);

		if (newline && buf[0] == '.' && (buf[1] == '\n' || buf[1] == '\0'))
			buf[1] = '.', buf[2] = '\n', ++len;

		newline = (buf[len - 1] == '\n');
		if (newline)
			strcpy(buf + len - 1, "\r\n"), ++len;

		if (net_write(smtp, g.timeout, buf, len) == -1)
			return errorsys("An error occurred while sending the message");
	}

	if (ferror(input))
		return error("An error occurred while reading the message");

	if (!feof(input))
		return error("Failed to reach end of file while reading the message");

	return 0;
}

const char *endof(const char *s, int type)
{
	char sq = squote[type];
	char eq = equote[type];
	int nest = comment[type];
	int level = 1;

	for (++s; *s; ++s)
	{
		if (*s == '\\')
		{
			++s;
			continue;
		}

		if (*s == eq)
		{
			if (!nest || !--level)
			{
				++s;
				break;
			}

			continue;
		}

		if (*s == sq)
		{
			if (nest)
			{
				++level;
				continue;
			}
			else
			{
				++s;
				break;
			}
		}
	}

	return s;
}

int readto(FILE *input, String **hdrs)
{
	char buf[BUFSIZ], rcpt[BUFSIZ];
	List **target = null;
	const char *start, *end, *t, *b = null;
	char *s = null;

	if (!(*hdrs = str_create("")))
		fatal("out of memory");

	for (;;)
	{
		if (!fgetline(buf, BUFSIZ, input))
			break;

		if (!str_append(*hdrs, "%s", buf))
			fatal("out of memory");

		if (cstr(*hdrs)[str_length(*hdrs) - 1] == '\n')
			if (!str_insert(*hdrs, str_length(*hdrs) - 1, "\r"))
				fatal("out of memory");

		if (*buf == '\n' || *buf == '\0')
			break;
	}

	if (feof(input))
		fatal("Unexpected end of file while reading headers");

	if (ferror(input))
		fatal("An error occurred while reading headers");

	start = cstr(*hdrs);
	end = start + str_length(*hdrs);

	for (t = start; t < end; ++t)
	{
		/* Start of header or end of headers */

		if ((t == start || t[-1] == '\n') && (*t != ' ' && *t != '\t'))
		{
			if (target) /* Previous header was interesting */
			{
				String *addr;
				if (!*target && !(*target = list_create((list_release_t *)str_release)))
					fatal("out of memory");
				*s = '\0';
				if (!(addr = str_create("%s", rcpt)))
					fatal("out of memory");
				if (!list_append(*target, str_squeeze(str_trim(addr))))
					fatal("out of memory");
				s = null;

				if (target == &g.bcc && !g.sendbcc) /* Previous header was secret */
				{
					if (!str_remove_range(*hdrs, b - start, t - b))
						fatal("out of memory");
					end -= t - b;
					t = b;
				}

				target = null;
			}

			if (!strncasecmp("To:", t, 3))
				target = &g.to, t += 3, s = rcpt;
			else if (!strncasecmp("Cc:", t, 3))
				target = &g.cc, t += 3, s = rcpt;
			else if (!strncasecmp("Bcc:" , t, 4))
				target = &g.bcc, b = t, t += 4, s = rcpt;
			else /* Start of uninteresting header or end of headers */
			{
				do /* Skip uninteresting header */
				{
					if (!(t = strchr(t + 1, '\n')))
						fatal("Invalid header");
				}
				while (t[1] == ' ' || t[1] == '\t');

				continue;
			}
		}

		if (target) /* In amongst an interesting header's data */
		{
			const char *q = strchr(squote, *t);

			if (q)
			{
				int type = q - squote;
				const char *stop = endof(t, type);

				if (!comment[type])
				{
					memcpy(s, t, stop - t);
					s += stop - t;
				}

				t = stop - 1;
			}
			else if (*t == ',') /* End of recipient address */
			{
				String *addr;
				*s = '\0';
				if (!*target && !(*target = list_create((list_release_t *)str_release)))
					fatal("out of memory");
				if (!(addr = str_create("%s", rcpt)))
					fatal("out of memory");
				if (!list_append(*target, str_squeeze(str_trim(addr))))
					fatal("out of memory");
				s = rcpt;
			}
			else /* In recipient address */
			{
				*s++ = *t;
			}
		}
	}

	return 0;
}

char *rfc822(char *buf, size_t max, struct tm *tm)
{
	size_t size = strftime(buf, max, "%a, %d %b %Y %H:%M:%S %z", tm);

	if (size == 0)
		return null;

	return buf;
}

char *rfc822_localtime(char *buf, size_t max, time_t time)
{
	return rfc822(buf, max, localtime(&time));
}

char *rfc822_gmtime(char *buf, size_t max, time_t time)
{
	return rfc822(buf, max, gmtime(&time));
}

String *addressof(const char *addr)
{
	const char *a, *l = null, *r = null;

	for (a = addr; *a; ++a)
	{
		const char *q = strchr(squote, *a);

		if (q)
			a = endof(a, q - squote) - 1;
		else if (*a == '<')
			l = a;
		else if (*a == '>')
			r = a;
	}

	if (l && r)
		return (l > r) ? null : substr(l, 0, r + 1 - l);

	return str_create("<%s>", addr);
}

int rcpt(int smtp, List *recipients)
{
	while (list_has_next(recipients))
	{
		String *recipient = list_next(recipients);
		String *addr;
		int rc, code;

		try_str(addr = addressof(cstr(recipient)))
		debug((1, "Sending: RCPT TO: %s", cstr(addr)))
		try_send((smtp, g.timeout, "RCPT TO: %s\r\n", cstr(addr)))
		str_destroy(&addr);
		debug((2, "Expecting server response"))
		try_expect((smtp, g.timeout, "%d", &code), 1, 250)
	}

	return 0;
}

int launch(FILE *input)
{
	int smtp;
	int code;
	String *hdrs = null, *addr;
	int rc;
	char c;

	if (g.readto)
	{
		debug((1, "Reading headers in message"))
		if (readto(input, &hdrs) == -1)
			return -1;

		if (!list_length(g.to))
			fatal("No recipients given");
	}

	debug((1, "Connecting to %s:%d", g.server, g.port))
	smtp = net_client(g.server, null, g.port, g.timeout, 0, 0, null, null);
	if (smtp == -1)
		return -1;

	debug((1, "Expecting server greeting"))
	try_expect((smtp, g.timeout, "%d%c", &code, &c), 2, 220)
	while (c == '-')
		try_expect((smtp, g.timeout, "%d%c", &code, &c), 2, 220)

	debug((1, "Sending: HELO %s", g.hostname))
	try_send((smtp, g.timeout, "HELO %s\r\n", g.hostname))
	debug((2, "Expecting server response"))
	try_expect((smtp, g.timeout, "%d", &code), 1, 250)
	try_str(addr = addressof(g.mailfrom))
	debug((1, "Sending: MAIL FROM: %s", cstr(addr)))
	try_send((smtp, g.timeout, "MAIL FROM: %s\r\n", cstr(addr)))
	str_destroy(&addr);
	debug((2, "Expecting server response"))
	try_expect((smtp, g.timeout, "%d", &code), 1, 250)
	try(rcpt(smtp, g.to))
	try(rcpt(smtp, g.cc))
	try(rcpt(smtp, g.bcc))
	debug((1, "Sending: DATA"))
	try_send((smtp, g.timeout, "DATA\r\n"))
	debug((2, "Expecting server response"))
	try_expect((smtp, g.timeout, "%d", &code), 1, 354)

	if (!g.noheaders)
	{
		char buf[BUFSIZ];

		if (g.gmtime || g.localtime)
		{
			if (g.gmtime)
				rfc822_gmtime(buf, BUFSIZ, time(null));
			else
				rfc822_localtime(buf, BUFSIZ, time(null));

			debug((1, "Sending: Date: %s", buf))
			try_send((smtp, g.timeout, "Date: %s\r\n", buf))
		}

		debug((1, "Sending: From: %s", g.from))
		try_send((smtp, g.timeout, "From: %s\r\n", g.from))

		if (g.subject)
		{
			debug((1, "Sending: Subject: %s", g.subject))
			try_send((smtp, g.timeout, "Subject: %s\r\n", g.subject))
		}

		try(header(smtp, g.to, "To"))
		try(header(smtp, g.cc, "Cc"))
		if (g.sendbcc)
			try(header(smtp, g.bcc, "Bcc"))
		try(headers(smtp, g.headers))
		debug((1, "Ending message header"))
		try_send((smtp, g.timeout, "\r\n"))
	}

	if (hdrs)
	{
		debug((1, "Sending headers in message"))
		try_send((smtp, g.timeout, "%s", cstr(hdrs)))
		str_destroy(&hdrs);
	}

	debug((1, "Sending message body"))
	try(body(smtp, input))
	debug((1, "Ending message body"))
	try_send((smtp, g.timeout, "\r\n.\r\n"))
	debug((2, "Expecting server response"))
	try_expect((smtp, g.timeout, "%d", &code), 1, 250)
	debug((1, "Sending: QUIT"))
	try_send((smtp, g.timeout, "QUIT\r\n"))
	debug((2, "Expecting server response"))
	try_expect((smtp, g.timeout, "%d", &code), 1, 221)
	debug((1, "Closing connection"))
	close(smtp);

	return 0;
}

int launchmail()
{
	debug((1, "launchmail %s", (g.message) ? g.message : "<stdin>"))

	if (g.message)
	{
		int rc;
		FILE *input = fopen(g.message, "rb");
		if (!input)
			fatalsys("Failed to open %s for reading", g.message);
		rc = launch(input);
		fclose(input);
		return rc;
	}

	return launch(stdin);
}

void *null_copy(const void *item)
{
	return (void *)item;
}

void append(List *list, const char *text, const char *delim)
{
	if (delim)
	{
		List *add = regexpr_split(text, delim, 0, 0);
		if (!add)
			fatalsys("out of memory");
		if (!list_append_list(list, add, null_copy))
			fatalsys("out of memory");
		list_disown(add);
		list_release(add);
	}
	else
	{
		String *add = str_create("%s", text);
		if (!add)
			fatal("out of memory");
		if (!list_append(list, add))
			fatal("out of memory");
	}
}

void add(List **target, const char *arg, const char *delim)
{
	if (!*target && !(*target = list_create((list_release_t *)str_release)))
		fatal("out of memory");

	append(*target, arg, delim);
}

const char *gdelim = null;

void parse(void *obj, const char *path, char *line, size_t lineno)
{
	append(obj, line, gdelim);
}

void addfile(List **target, const char *arg, const char *delim)
{
	char explanation[1024];
	size_t explanation_size = 1024;

	if (!*target && !(*target = list_create((list_release_t *)str_release)))
		fatal("out of memory");

	switch (daemon_path_is_safe(arg, explanation, explanation_size))
	{
		case -1:
			errorsys("Failed to check file: %s (%s)", arg, explanation);
			return;
		case 0:
			error("Ignoring unsafe file: %s", arg);
			return;
	}

	gdelim = delim;

	if (!daemon_parse_config(arg, *target, parse))
		error("Failed to parse %s", arg);
}

const char *comma = " *, *";

void add_to(const char *arg)
{
	add(&g.to, arg, comma);
}

void addfile_to(const char *arg)
{
	addfile(&g.to, arg, comma);
}

void add_cc(const char *arg)
{
	add(&g.cc, arg, comma);
}

void addfile_cc(const char *arg)
{
	addfile(&g.cc, arg, comma);
}

void add_bcc(const char *arg)
{
	add(&g.bcc, arg, comma);
}

void addfile_bcc(const char *arg)
{
	addfile(&g.bcc, arg, comma);
}

void add_headers(const char *arg)
{
	add(&g.headers, arg, null);
}

void addfile_headers(const char *arg)
{
	addfile(&g.headers, arg, null);
}

void check_config()
{
	if (!g.readto && !list_length(g.to))
		fatal("No recipients given");

	if (!g.server)
		fatal("No SMTP server given");

	if (!g.port)
	{
		struct servent *servent = getservbyname("smtp", "tcp");
		g.port = (servent) ? ntohs(servent->s_port) : 25;
	}

	if (!g.hostname)
	{
		struct utsname utsname[1];
		struct hostent *hostent;

		if (uname(utsname) == -1)
			fatalsys("Failed to get hostname");

		if (!(hostent = gethostbyname(utsname->nodename)))
			fatal("Failed to get host's FQDN");

		if (!(g.hostname = mem_strdup(hostent->h_name)))
			fatal("out of memory");
	}

	if (!g.from)
	{
		struct passwd *passwd;
		String *from;
		if (!(passwd = getpwuid(getuid())))
			fatal("Failed to get username for uid %d", getuid());
		if (!(from = str_create("%s@%s", passwd->pw_name, g.hostname)))
			fatal("out of memory");
		g.from = cstr(from); /* leak from */
	}

	if (!g.mailfrom)
		g.mailfrom = g.from;

	if (g.readto)
		++g.noheaders;
}

/*

C<Option launchmail_options_table[];>

Application specific command line options.

*/

static Option launchmail_options_table[] =
{
	{
		"from", 'f', "address", "Sender address",
		required_argument, OPT_STRING, OPT_VARIABLE, &g.from, null
	},
	{
		"subject", 's', "subject", "Message subject",
		required_argument, OPT_STRING, OPT_VARIABLE, &g.subject, null
	},
	{
		"to", 't', "addresses", "Recipient address list (comma separated)",
		required_argument, OPT_STRING, OPT_FUNCTION, null, (func_t *)add_to
	},
	{
		"tofile", 'T', "filename", "Recipient address list filename",
		required_argument, OPT_STRING, OPT_FUNCTION, null, (func_t *)addfile_to
	},
	{
		"cc", 'c', "addresses", "Carbon Copy address list",
		required_argument, OPT_STRING, OPT_FUNCTION, null, (func_t *)add_cc
	},
	{
		"ccfile", 'C', "filename", "Carbon Copy address list filename",
		required_argument, OPT_STRING, OPT_FUNCTION, null, (func_t *)addfile_cc
	},
	{
		"bcc", 'b', "addresses", "Blind Carbon Copy address list",
		required_argument, OPT_STRING, OPT_FUNCTION, null, (func_t *)add_bcc
	},
	{
		"bccfile", 'B', "filename", "Blind Carbon Copy address list filename",
		required_argument, OPT_STRING, OPT_FUNCTION, null, (func_t *)addfile_bcc
	},
	{
		"readto", 'r', null, "Read message for recipients",
		no_argument, OPT_INTEGER, OPT_VARIABLE, &g.readto, null
	},
	{
		"sendbcc", '!', null, "Don't remove Bcc header (when --readto)",
		no_argument, OPT_INTEGER, OPT_VARIABLE, &g.sendbcc, null
	},
	{
		"gmtime", 'g', null, "Add Date: header in gmtime",
		no_argument, OPT_INTEGER, OPT_VARIABLE, &g.gmtime, null
	},
	{
		"localtime", 'l', null, "Add Date: header in localtime",
		no_argument, OPT_INTEGER, OPT_VARIABLE, &g.localtime, null
	},
	{
		"header", 'x', "'name: value'", "Arbitrary header",
		required_argument, OPT_STRING, OPT_FUNCTION, null, (func_t *)add_headers
	},
	{
		"headerfile", 'X', "filename", "Arbitrary header filename",
		required_argument, OPT_STRING, OPT_FUNCTION, null, (func_t *)addfile_headers
	},
	{
		"mailfrom", 'm', "address", "SMTP mail from address",
		required_argument, OPT_STRING, OPT_VARIABLE, &g.mailfrom, null
	},
	{
		"hostname", 'n', "hostname", "SMTP helo hostname",
		required_argument, OPT_STRING, OPT_VARIABLE, &g.hostname, null
	},
	{
		"server", 'S', "hostname", "SMTP server to connect to",
		required_argument, OPT_STRING, OPT_VARIABLE, &g.server, null
	},
	{
		"port", 'P', "#", "Port to use when connecting to SMTP server",
		required_argument, OPT_INTEGER, OPT_VARIABLE, &g.port, null
	},
	{
		"timeout", 'o', "#", "Seconds to wait during SMTP dialogue",
		required_argument, OPT_INTEGER, OPT_VARIABLE, &g.timeout, null
	},
	{
		"noheaders", 'N', null, "Do not insert any headers",
		no_argument, OPT_INTEGER, OPT_VARIABLE, &g.noheaders, null
	},
	{
		"quiet", 'q', null, "Remain silent when an error occurs",
		no_argument, OPT_INTEGER, OPT_VARIABLE, &g.quiet, null
	},
	{
		null, '\0', null, null, 0, 0, 0, null, null
	}
};

static Options options[1] = {{ prog_options_table, launchmail_options_table }};

#ifndef NDEBUG
void show_config()
{
	debug((1, "Message: %s", g.message))
	debug((1, "From: %s", g.from))
	debug((1, "Subject: %s", (g.subject) ? g.subject: ""))

	while (list_has_next(g.to))
	{
		String *rcpt = list_next(g.to);
		debug((1, "To: %s", cstr(rcpt)))
	}

	while (list_has_next(g.cc))
	{
		String *rcpt = list_next(g.cc);
		debug((1, "Cc: %s", cstr(rcpt)))
	}

	while (list_has_next(g.bcc))
	{
		String *rcpt = list_next(g.bcc);
		debug((1, "Bcc: %s", cstr(rcpt)))
	}

	debug((1, "Date: %s", (g.gmtime) ? "gmtime" : (g.localtime) ? "localtime" : "none"))

	while (list_has_next(g.headers))
	{
		String *header = list_next(g.headers);
		debug((1, "Header: %s", cstr(header)))
	}

	debug((1, "Server: %s", g.server))
	debug((1, "Port: %d", g.port))
	debug((1, "Timeout: %d", g.timeout))
	debug((1, "HELO %s", g.hostname))
	debug((1, "MAIL FROM: %s", g.mailfrom))
	debug((1, "ReadTo: %d", g.readto))
	debug((1, "SendBcc: %d", g.sendbcc))
	debug((1, "NoHeaders: %d", g.noheaders))
	debug((1, "Quiet: %d", g.quiet))
}
#endif

int main(int ac, char **av)
{
	int a;

	prog_init();
	prog_set_name(LAUNCH_NAME);
	prog_set_version(LAUNCH_VERSION);
	prog_set_date(LAUNCH_DATE);
	prog_set_syntax("[options] [filename]");
	prog_set_options(options);
	prog_set_author("raf <raf@raf.org>");
	prog_set_contact(prog_author());
	prog_set_url(LAUNCH_URL);
	prog_set_legal
	(
		"Copyright (C) 2000 raf <raf@raf.org>\n"
		"\n"
		"This is free software released under the terms of the GPLv2+:\n"
		"\n"
		"    http://www.gnu.org/copyleft/gpl.html\n"
		"\n"
		"There is no warranty; not even for merchantability or fitness\n"
		"for a particular purpose.\n"
#ifndef HAVE_GETOPT_LONG
		"\n"
		"Includes the GNU getopt functions:\n"
		"    Copyright (C) 1997, 1998 Free Software Foundation, Inc.\n"
#endif
#ifndef HAVE_SNPRINTF
		"\n"
		"Includes snprintf:\n"
		"    Copyright (C) 1995 Patrick Powell\n"
#endif
	);

	prog_set_desc
	(
		"launchmail - an SMTP client with a sendmail-compatible wrapper.\n"
		"See the launchmail(1) manpage for more information.\n"
	);

	if (daemon_revoke_privileges() == -1)
		fatalsys("failed to revoke set uid/gid privileges: uid %d euid %d gid %d egid %d", getuid(), geteuid(), getgid(), getegid());

	if (daemon_prevent_core() == -1)
		fatalsys("failed to prevent core file generation");

	if ((a = prog_opt_process(ac, av)) != ac - 1 && a != ac)
		prog_usage_msg("Wrong number of arguments");

	g.message = av[a];

	check_config();

	if (g.quiet)
		prog_err_none();

#ifndef NDEBUG
	if (prog_debug_level() >= 1)
		show_config();
#endif

	if (launchmail() == -1)
		fatal("failed to launch mail");

	return 0;
}

/* vi:set ts=4 sw=4: */
