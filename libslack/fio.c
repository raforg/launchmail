/*
# libslack - http://libslack.org/
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
*/

/*

=head1 NAME

I<libslack(fio)> - fifo and file control module and some I/O

=head1 SYNOPSIS

    #include <slack/fio.h>

    char *fgetline(char *line, size_t size, FILE *stream);
    int fcntl_set_flag (int fd, int flag);
    int fcntl_clear_flag (int fd, int flag);
    int fcntl_lock (int fd, int cmd, int type, int whence, int start, int len);
    int nonblock_set (int fd, int arg);
    int nonblock_on (int fd);
    int nonblock_off (int fd);
    int fifo_exists (const char *path, int prepare);
    int fifo_has_reader (const char *path, int prepare);
    int fifo_open (const char *path, mode_t mode, int lock);

=head1 DESCRIPTION

This module provides various I/O related functions: reading a line of text
no matter what line endings are used; exclusively opening a fifo for reading;
and some random shorthand functions for manipulating file flags and locks.

=over 4

=cut

*/

#include "std.h"

#include <fcntl.h>

#include <sys/stat.h>
#ifndef BSD_COMP
#define BSD_COMP /* for Solaris::FIONBIO */
#endif
#include <sys/ioctl.h>
#undef BSD_COMP

#include "err.h"

/*

=item C<char *fgetline(char *line, size_t size, FILE *stream)>

Similar to fgets except that it recognises UNIX ("\n"), DOS ("\r\n") and
Macintosh ("\r") line endings (even different line ending in the same file).
Reads in at most one less than C<size> characters from C<stream> and stores
them into the buffer pointed to by C<line>. Reading stops after an C<EOF> or
the end of the line. If the end of the line was read, a newline is stored
into the buffer. A C<'\0'> is stored after the last character in the buffer.
On success, returns C<line>. On error, and when the end of file occurs while
no characters have been read, returns C<NULL>. Calls to this function can be
mixed with calls to other input functions from the I<stdio> library for the
same input stream.

=cut

*/

void flockfile(FILE *stream); /* Missing from old glibc headers */
void funlockfile(FILE *stream);

char *fgetline(char *line, size_t size, FILE *stream)
{
	char *s = line;
	char *end = line + size - 1;
	int c = '\0', c2;

	flockfile(stream);

	while (s < end && (c = getc_unlocked(stream)) != EOF)
	{
		if (c == '\n')
		{
			*s++ = c;
			break;
		}
		else if (c == '\r')
		{
			*s++ = '\n';

			if ((c2 = getc_unlocked(stream)) == '\n')
				break;

			ungetc(c2, stream);
			break;
		}
		else
			*s++ = c;
	}

	funlockfile(stream);

	if (s < end && c == EOF)
	{
		if (ferror(stream))
			return NULL;

		if (s == line && feof(stream))
			return NULL;
	}

	*s = '\0';

	return line;
}

/*

=item C<int fcntl_set_flag(int fd, int flag)>

Shorthand for setting the file flag C<flag> on file descriptor C<fd> using
I<fcntl(2)>. All other flags are unaffected. Returns the same as I<fcntl(2)>
with C<F_GETFL> or C<F_SETFL> as the command.

=cut

*/

int fcntl_set_flag(int fd, int flag)
{
	int flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
		return -1;

	return fcntl(fd, F_SETFL, flags | flag);
}

/*

=item C<int fcntl_clear_flag(int fd, int flag)>

Shorthand for clearing the file flag C<flag> from file descriptor C<fd>
using I<fcntl(2)>. All other flags are unaffected. Returns the same as
I<fcntl(2)> with C<F_GETFL> or C<F_SETFL> as the command.

=cut

*/

int fcntl_clear_flag(int fd, int flag)
{
	int flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
		return -1;

	return fcntl(fd, F_SETFL, flags & ~flag);
}

/*

=item C<int fcntl_lock(int fd, int cmd, int type, int whence, int start, int len)>

Shorthand for performing discretionary file locking operations on file
descriptor C<fd>. C<cmd> is the locking command and is passed to
I<fcntl(2)>. C<type>, C<whence>, C<start> and C<len> are used to fill a
I<flock> structure which is passed to I<fcntl(2)>. Returns the same as
I<fcntl(2)> with C<cmd> as the command.

=cut

*/

int fcntl_lock(int fd, int cmd, int type, int whence, int start, int len)
{
	struct flock lock[1];

	lock->l_type = type;
	lock->l_whence = whence;
	lock->l_start = start;
	lock->l_len = len;

	return fcntl(fd, cmd, lock);
}

/*

=item C<int nonblock_set(int fd, int arg)>

Sets non-blocking mode for file descriptor C<fd> if C<arg> is non-zero.
Clears non-blocking mode if C<arg> is zero. Returns the same as I<ioctl(2)>
with C<FIONBIO> as the command.

=cut

*/

int nonblock_set(int fd, int arg)
{
	return ioctl(fd, FIONBIO, &arg);
}

/*

=item C<int nonblock_on(int fd)>

Sets non-blocking mode for file descriptor C<fd>. Returns the same as
I<ioctl(2)> with C<FIONBIO> as the command.

=cut

*/

int nonblock_on(int fd)
{
	return nonblock_set(fd, 1);
}

/*

=item C<int nonblock_off(int fd)>

Clears non-blocking mode for the file descriptor C<fd>. Returns the same as
I<ioctl(2)> with C<FIONBIO> as the command.

=cut

*/

int nonblock_off(int fd)
{
	return nonblock_set(fd, 0);
}

/*

=item C<int fifo_exists(const char *path, int prepare)>

Determines whether or not C<path> refers to a fifo. Returns 0 if C<path>
doesn't exist or doesn't refer to a fifo. If C<path> refers to a fifo,
returns 1. If C<prepare> is non-zero, and C<path> refers to a non-fifo, it
will be unlinked. On error, returns -1 with C<errno> set by I<stat(2)>.

=cut

*/

int fifo_exists(const char *path, int prepare)
{
	struct stat status[1];

	if (stat(path, status) == -1)
		return (errno == ENOENT) ? 0 : -1;

	if (S_ISFIFO(status->st_mode) == 0)
	{
		if (prepare)
			unlink(path);

		return 0;
	}

	return 1;
}

/*

=item C<int fifo_has_reader(const char *path, int prepare)>

Determines whether or not C<path> refers to a fifo that is being read by
another process. If C<path> does not exist or does not refer to a fifo or if
the fifo can't be opened for non-blocking I<write(2)>, returns 0. If
C<prepare> is non-zero, and path refers to a non-fifo, it will be unlinked.
On error, returns -1 with C<errno> set by I<stat(2)> or I<open(2)>.

=cut

*/

int fifo_has_reader(const char *path, int prepare)
{
	int fd;

	/*
	** Check that fifo exists and is a fifo.
	** If not, there can be no reader process.
	*/

	switch (fifo_exists(path, prepare))
	{
		case  0: return 0;
		case -1: return -1;
	}

	/*
	** Open the fifo for non-blocking write.
	** If there is no reader process, open()
	** will fail with errno == ENXIO.
	*/

	if ((fd = open(path, O_WRONLY | O_NONBLOCK)) == -1)
		return (errno == ENXIO) ? 0 : -1;

	close(fd);

	return 1;
}

/*

=item C<int fifo_open(char *path, mode_t mode, int lock)>

Creates a fifo named C<path> with creation mode C<mode> for reading. If
C<path> already exists, is a fifo and has a reader process, return -1 with
C<errno> set to C<EADDRINUSE>. If the fifo is created (or an existing one
can be reused), two file descriptors are opened to the fifo. A read
descriptor and a write descriptor. The read descriptor is returned by this
function on success. The write descriptor only exists to ensure that there
is always at least one writer process for the fifo. This allows a I<read(2)>
on the read descriptor to block until another process writes to the fifo
rather than returning an C<EOF> condition. This is done in a POSIX compliant
way. If C<lock> is non-zero, the fifo is exclusively locked. On error,
returnd -1 with C<errno> set by I<stat(2)>, I<open(2)>, I<mkfifo(2)>,
I<fstat(2)>, I<fcntl(2)> or I<ioctl(2)>.

=cut

*/

int fifo_open(const char *path, mode_t mode, int lock)
{
	struct stat status[1];
	int rfd, wfd;

	/* Don't open the fifo for reading twice. */

	switch (fifo_has_reader(path, 1))
	{
		case  1: return set_errno(EADDRINUSE);
		case -1: return -1;
	}

	/* Create the fifo. */

	if (mkfifo(path, S_IFIFO | mode) == -1 && errno != EEXIST)
		return -1;

	/*
	** Open the fifo for non-blocking read only.
	** This prevents blocking while waiting for a
	** writer process. We are about to supply our
	** own writer.
	*/

	if ((rfd = open(path, O_RDONLY | O_NONBLOCK)) == -1)
		return -1;

	/*
	** A sanity check to make sure that what we have just
	** opened is really a fifo. Someone may have just replacd
	** the fifo with a file between fifo_has_reader and here.
	*/

	if (fstat(rfd, status) == -1 || S_ISFIFO(status->st_mode) == 0)
	{
		close(rfd);
		return -1;
	}

	/*
	** Open the fifo for write only and leave this fd open.
	** This guarantees that there is always at least one
	** writer process. This prevents EOF indications being
	** returned from read() when there are no other writer
	** processes.
	**
	** Just opening the fifo "rw" should work but it's undefined
	** by POSIX.
	*/

	if ((wfd = open(path, O_WRONLY)) == -1)
	{
		close(rfd);
		return -1;
	}

	/* A sanity test on what we have just opened. */

	if (fstat(wfd, status) == -1 || S_ISFIFO(status->st_mode) == 0)
	{
		close(rfd);
		close(wfd);
		return -1;
	}

	/*
	** Exclusively lock the fifo to prevent two invocations
	** deciding that there's no reader and opening this fifo
	** at the same time.
	*/

	if (lock && fcntl_lock(wfd, F_SETLK, F_WRLCK, SEEK_SET, 0, 0) == -1)
	{
		close(rfd);
		close(wfd);
		return -1;
	}

	/* Now put the reader into blocking mode. */

	if (nonblock_off(rfd) == -1)
	{
		close(rfd);
		close(wfd);
		return -1;
	}

	/*
	** Flaw: If someone unceremoniously unlinks our fifo, we won't know about
	** it and nothing will stop another invocation from creating a new fifo and
	** handling it. This process would sleep forever in select().
	*/

	return rfd;
}

/*

=back

=head1 SEE ALSO

L<fcntl(2)|fcntl(2)>,
L<ioctl(2)|ioctl(2)>,
L<stat(2)|stat(2)>,
L<fstat(2)|fstat(2)>,
L<open(2)|open(2)>,
L<write(2)|write(2)>,
L<read(2)|read(2)>,
L<mkfifo(2)|mkfifo(2)>,
L<daemon(3)|daemon(3)>,
L<err(3)|err(3)>,
L<hsort(3)|hsort(3)>,
L<lim(3)|lim(3)>,
L<list(3)|list(3)>,
L<log(3)|log(3)>,
L<map(3)|map(3)>,
L<mem(3)|mem(3)>,
L<msg(3)|msg(3)>,
L<net(3)|net(3)>,
L<opt(3)|opt(3)>,
L<prog(3)|prog(3)>,
L<prop(3)|prop(3)>,
L<sig(3)|sig(3)>,
L<str(3)|str(3)>

=head1 AUTHOR

20000902 raf <raf@raf.org>

=cut

*/

#ifdef TEST

int main(int ac, char **av)
{
	const char * const fifoname = "./fio.fifo";
	const char * const filename = "./fio.fgetline";
	const mode_t mode = S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH;
	FILE *file;
	char line[BUFSIZ];
	const int lock = 1;
	int errors = 0;
	int fd;

	printf("Testing: fio\n");

	if ((fd = fifo_open(fifoname, mode, lock)) == -1)
		++errors, printf("Test1: fifo_open(\"%s\", %d, %d) failed (%s)\n", fifoname, (int)mode, lock, strerror(errno));

	if ((fcntl_lock(fd, F_SETLK, F_WRLCK, SEEK_SET, 0, 0)) != -1)
		++errors, printf("Test2: fcntl_lock() failed\n");

	/* Should really test that the following non-blocking changes do occur */

	if (nonblock_on(fd) == -1)
		++errors, printf("Test3: nonblock_on() failed (%s)\n", strerror(errno));

	if (nonblock_off(fd) == -1)
		++errors, printf("Test4: nonblock_off() failed (%s)\n", strerror(errno));

	if (fcntl_set_flag(fd, O_NONBLOCK) == -1)
		++errors, printf("Test5: fcntl_set_flag() failed (%s)\n", strerror(errno));

	if (fcntl_clear_flag(fd, O_NONBLOCK) == -1)
		++errors, printf("Test6: fcntl_clear_flag() failed (%s)\n", strerror(errno));

	close(fd);
	unlink(fifoname);

#define CHECK_FGETLINE(i, size, expected) \
	if ((expected) && !fgetline(line, (size), file)) \
		++errors, printf("Test%d: fgetline() failed\n", (i)); \
	else if ((expected) && strcmp(line, (expected))) \
		++errors, printf("Test%d: fgetline() read \"%s\", not \"%s\"\n", (i), line, (expected));

#define TEST_FGETLINE(i, size, contents, line1, line2, line3) \
	if (!(file = fopen(filename, "wb"))) \
		++errors, printf("Test%d: failed to run test: failed to create test file\n", (i)); \
	else \
	{ \
		if (fwrite((contents), 1, strlen(contents), file) != strlen(contents)) \
			++errors, printf("Test%d: failed to run test: failed to write to test file\n", (i)); \
		else \
		{ \
			fclose(file); \
			if (!(file = fopen(filename, "r"))) \
				++errors, printf("Test%d: failed to run test: failed to open test file for reading\n", (i)); \
			else \
			{ \
				CHECK_FGETLINE((i), (size), (line1)) \
				CHECK_FGETLINE((i), (size), (line2)) \
				CHECK_FGETLINE((i), (size), (line3)) \
				if (fgetline(line, BUFSIZ, file)) \
					++errors, printf("Test%d: fgetline() failed to return NULL at end of file\n", (i)); \
			} \
		} \
		fclose(file); \
		unlink(filename); \
	}

	TEST_FGETLINE(7, BUFSIZ, "abc\ndef\r\nghi\r", "abc\n", "def\n", "ghi\n")
	TEST_FGETLINE(8, BUFSIZ, "abc\rdef\nghi\r\n", "abc\n", "def\n", "ghi\n")
	TEST_FGETLINE(9, BUFSIZ, "abc\r\ndef\rghi\n", "abc\n", "def\n", "ghi\n")
	TEST_FGETLINE(10, BUFSIZ, "abc\ndef\rghi", "abc\n", "def\n", "ghi")
	TEST_FGETLINE(11, BUFSIZ, "", (char *)NULL, (char *)NULL, (char *)NULL)
	TEST_FGETLINE(12, 5, "abc", "abc", (char *)NULL, (char *)NULL)
	TEST_FGETLINE(13, 5, "abc\n", "abc\n", (char *)NULL, (char *)NULL)
	TEST_FGETLINE(14, 5, "abc\r\n", "abc\n", (char *)NULL, (char *)NULL)
	TEST_FGETLINE(15, 5, "abc\r", "abc\n", (char *)NULL, (char *)NULL)
	TEST_FGETLINE(16, 3, "abc\r", "ab", "c\n", (char *)NULL)

	if (errors)
		printf("%d/16 tests failed\n", errors);
	else
		printf("All tests passed\n");

	return 0;
}

#endif

/* vi:set ts=4 sw=4: */
