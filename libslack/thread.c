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
*/

/*

=head1 NAME

I<libslack(thread)> - thread module

=head1 SYNOPSIS

    #include <slack/thread.h>

    XXX thread module prototypes

=head1 DESCRIPTION

This module provides a few convenience functions for using the I<pthread>
library as well as a few synchronisation variables that are defined in
recent standards but may not be on your system yet (i.e. reader/writer locks
and barriers).

One of the nicest things about POSIX MT programs is how it simplifies signal
handling. A single thread can be devoted to handling signals synchronously
with I<sigwait(3)> while all other threads go about their business, free of
signals. Unfortunately, if you have a Linux system, you may have noticed
that the MT signal handling is not POSIX compliant. This is due to lack of
kernel support. This module provides a (non-transparent) remedy for Linux MT
signal handling that simulates this simplified signal handling method.

=over 4

=cut

*/

#define _XOPEN_SOURCE
#include "std.h"

#define NEEDS_PTHREAD_RWLOCK
#define NEEDS_PTHREAD_BARRIER
#define NEEDS_PTHREAD_SIGNAL_LINUX_HACK

#include "thread.h"

#define try(action) { int rc = (action); if (rc != 0) return rc; }

/*

=item C<int thread_attr_init(pthread_attr_t *attr)>

Initialises C<attr> for creating detached threads with system scope and
explicit "other" scheduling. I prefer this to the POSIX default attributes
(and Linux doesn't have process scope scheduling anyway). On success,
returns 0. On error, returns the error code from the underlying I<pthread>
library call that failed.

=cut

*/

int thread_attr_init(pthread_attr_t *attr)
{
	if (!attr)
		return EINVAL;

	return thread_attr_set(attr, PTHREAD_SCOPE_SYSTEM, PTHREAD_CREATE_DETACHED, PTHREAD_EXPLICIT_SCHED, SCHED_OTHER, 0);
}

/*

=item C<int thread_attr_set(pthread_attr_t *attr, int scope, int detachstate, int inheritsched, int schedpolicy, struct sched_param *schedparam)>

Initialises C<attr> with the following arguments. Each argument is passed to
the I<pthread_attr_set> function whose name corresponds with its name. On
success, returns 0. On error, returns the error code from the underlying
I<pthread> library call that failed.

=cut

*/

int thread_attr_set(pthread_attr_t *attr, int scope, int detachstate, int inheritsched, int schedpolicy, struct sched_param *schedparam)
{
	if (!attr)
		return EINVAL;

	try(pthread_attr_init(attr))
	try(pthread_attr_setscope(attr, scope))
	try(pthread_attr_setdetachstate(attr, detachstate))
	try(pthread_attr_setinheritsched(attr, inheritsched))
	try(pthread_attr_setschedpolicy(attr, schedpolicy))
	try(pthread_attr_setschedparam(attr, schedparam))

	return 0;
}

/*

=item C<int thread_setcancel(int type, int state, int *oldtype, int *oldstate)>

Sets the cancellation type and state for the current thread to C<type> and
C<state>, respectively. If C<oldtype> is not C<NULL>, the previous cancellation
type is stored there. If C<oldstate> is not C<NULL>, the previous cancellation
state is stored there. On success, return 0. On error, returns the error code
from the underlying I<pthread> library call that failed.

=cut

*/

int thread_setcancel(int type, int state, int *oldtype, int *oldstate)
{
	try(pthread_setcanceltype(type, oldtype))
	try(pthread_setcancelstate(state, oldstate))

	return 0;
}

/*

=item C<int thread_init()>

Enables deferred cancellation on the current thread. This is the same as the
POSIX default. On success, returns 0. On error, returns the error code from
the underlying I<pthread> library call that failed.

=cut

*/

int thread_init()
{
	try(thread_setcancel(PTHREAD_CANCEL_DEFERRED, PTHREAD_CANCEL_ENABLE, NULL, NULL))

	return 0;
}

/*

=item C<int thread_start(void *thread)>

A wrapper function for starting threads. What should it do? XXX

=cut

*/


int thread_start(void *thread)
{
	/* XXX ((void *(*start_routine)(void *))thread)(NULL); */
	return ENOSYS;
}

#ifdef NEEDS_PTHREAD_RWLOCK

/*

=item C<funcproto>

Blah.

=cut

*/

int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr)
{
	pthread_mutexattr_t mutexattr;
	pthread_condattr_t condattr;
	int pshared;

	if (!rwlock)
		return EINVAL;

	try(pthread_mutexattr_init(&mutexattr))
	try(pthread_condattr_init(&condattr))

	if (attr)
		try(pthread_rwlockattr_getpshared(attr, &pshared))
	else
		pshared = PTHREAD_PROCESS_PRIVATE;

	try(pthread_mutexattr_setpshared(&mutexattr, pshared))
	try(pthread_condattr_setpshared(&condattr, pshared))

	try(pthread_mutex_init(&rwlock->lock, &mutexattr))
	try(pthread_cond_init(&rwlock->readers, &condattr))
	try(pthread_cond_init(&rwlock->writers, &condattr))

	rwlock->waiters = 0;
	rwlock->state = 0;

	return 0;
}

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
	if (!rwlock)
		return EINVAL;

	try(pthread_mutex_destroy(&rwlock->lock))
	try(pthread_cond_destroy(&rwlock->readers))
	try(pthread_cond_destroy(&rwlock->writers))

	return 0;
}

static void rdlock_cleanup(void *arg)
{
	pthread_rwlock_t *rwlock = (pthread_rwlock_t *)arg;

	pthread_mutex_unlock(&rwlock->lock);
}

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
	if (!rwlock)
		return EINVAL;

	/* Wait until there are no active or queued writers */

	try(pthread_mutex_lock(&rwlock->lock))
	pthread_cleanup_push(rdlock_cleanup, rwlock);

	while (rwlock->state == -1 || rwlock->waiters)
		try(pthread_cond_wait(&rwlock->readers, &rwlock->lock))

	pthread_cleanup_pop(1);
	++rwlock->state;

	return 0;
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
	if (!rwlock)
		return EINVAL;

	/* Are there no active or waiting writers? */

	try(pthread_mutex_lock(&rwlock->lock))

	if (rwlock->state != -1 && !rwlock->waiters)
	{
		++rwlock->state;
		try(pthread_mutex_unlock(&rwlock->lock))

		return 0;
	}

	try(pthread_mutex_unlock(&rwlock->lock))

	return EBUSY;
}

static void wrlock_cleanup(void *arg)
{
	pthread_rwlock_t *rwlock = (pthread_rwlock_t *)arg;

	if (--rwlock->waiters == 0 && rwlock->state != -1)
		pthread_cond_broadcast(&rwlock->readers);

	pthread_mutex_unlock(&rwlock->lock);
}

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
	if (!rwlock)
		return EINVAL;

	try(pthread_mutex_lock(&rwlock->lock))
	++rwlock->waiters;
	pthread_cleanup_push(wrlock_cleanup, rwlock);

	while (rwlock->state != 0)
		try(pthread_cond_wait(&rwlock->writers, &rwlock->lock))

	rwlock->state = -1;
	pthread_cleanup_pop(1);

	return 0;
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
	if (!rwlock)
		return EINVAL;

	try(pthread_mutex_lock(&rwlock->lock))

	/* Are there any active or queued writers? */

	if (rwlock->state == 0 && rwlock->waiters == 0)
	{
		rwlock->state = -1;
		try(pthread_mutex_unlock(&rwlock->lock))

		return 0;
	}

	try(pthread_mutex_unlock(&rwlock->lock))

	return EBUSY;
}

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
	if (!rwlock)
		return EINVAL;

	if (rwlock->state == -1)
	{
		rwlock->state = 0;

		if (rwlock->waiters)
			try(pthread_cond_signal(&rwlock->writers))
		else
			try(pthread_cond_broadcast(&rwlock->readers))
	}
	else
	{
		if (--rwlock->state == 0)
			try(pthread_cond_signal(&rwlock->writers))
	}

	try(pthread_mutex_unlock(&rwlock->lock))

	return 0;
}

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr)
{
	if (!attr)
		return EINVAL;

	attr->pshared = PTHREAD_PROCESS_PRIVATE;

	return 0;
}

int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr)
{
	if (!attr)
		return EINVAL;

	return 0;
}

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr, int *pshared)
{
	if (!attr || !pshared)
		return EINVAL;

	*pshared = attr->pshared;

	return 0;
}

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared)
{
	if (!attr)
		return EINVAL;

	if (pshared != PTHREAD_PROCESS_PRIVATE && pshared != PTHREAD_PROCESS_SHARED)
		return EINVAL;

	attr->pshared = pshared;

	return 0;
}

#endif

#ifdef NEEDS_PTHREAD_BARRIER

int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count)
{
	pthread_mutexattr_t mutexattr;
	pthread_condattr_t condattr;
	int pshared;

	if (!barrier)
		return EINVAL;

	try(pthread_mutexattr_init(&mutexattr))
	try(pthread_condattr_init(&condattr))

	if (attr)
		try(pthread_barrierattr_getpshared(attr, &pshared))
	else
		pshared = PTHREAD_PROCESS_PRIVATE;

	try(pthread_mutexattr_setpshared(&mutexattr, pshared))
	try(pthread_condattr_setpshared(&condattr, pshared))

	try(pthread_mutex_init(&barrier->lock, &mutexattr))
	try(pthread_cond_init(&barrier->cond, &condattr))

	barrier->count = count;
	barrier->sleepers = 0;
	barrier->releasing = 0;

	return 0;
}

int pthread_barrier_destroy(pthread_barrier_t *barrier)
{
	if (!barrier)
		return EINVAL;

	try(pthread_mutex_destroy(&barrier->lock))
	try(pthread_cond_destroy(&barrier->cond))

	return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier)
{
	int release = 0;

	try(pthread_mutex_lock(&barrier->lock))

	/* If previous cycle still releasing, wait */

	while (barrier->releasing)
		try(pthread_cond_wait(&barrier->cond, &barrier->lock))

	/* Start release if we are the last thread to arrive, otherwise wait */

	if (++barrier->sleepers == barrier->count)
		release = barrier->releasing = 1;
	else
		while (!barrier->releasing)
			try(pthread_cond_wait(&barrier->cond, &barrier->lock))

	if (--barrier->sleepers == 0)
		barrier->releasing = 0, release = 1;

	try(pthread_mutex_unlock(&barrier->lock))

	/* Wake up waiters (if any) for next cycle */

	if (release)
		try(pthread_cond_broadcast(&barrier->cond))

	return 0;
}

int pthread_barrierattr_init(pthread_barrierattr_t *attr)
{
	if (!attr)
		return EINVAL;

	attr->pshared = PTHREAD_PROCESS_PRIVATE;

	return 0;
}

int pthread_barrierattr_destroy(pthread_barrierattr_t *attr)
{
	if (!attr)
		return EINVAL;

	return 0;
}

int pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared)
{
	if (!attr || !pshared)
		return EINVAL;

	*pshared = attr->pshared;

	return 0;
}

int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared)
{
	if (!attr)
		return EINVAL;

	if (pshared != PTHREAD_PROCESS_PRIVATE && pshared != PTHREAD_PROCESS_SHARED)
		return EINVAL;

	attr->pshared = pshared;

	return 0;
}

#endif

/*

=back

=head1 EXAMPLE

XXX thread module example

=head1 SEE ALSO

L<pthread_attr_init(3)|pthread_attr_init(3)>,
L<pthread_attr_setscope(3)|pthread_attr_setscope(3)>,
L<pthread_attr_setdetachstate(3)|pthread_attr_setdetachstate(3)>,
L<pthread_attr_setinheritsched(3)|pthread_attr_setinheritsched(3)>,
L<pthread_attr_setschedpolicy(3)|pthread_attr_setschedpolicy(3)>,
L<pthread_attr_setschedparam(3)|pthread_attr_setschedparam(3)>,
L<pthread_setcanceltype(3)|pthread_setcanceltype(3)>,
L<pthread_setcancelstate(3)|pthread_setcancelstate(3)>,
L<daemon(3)|daemon(3)>,
L<err(3)|err(3)>,
L<fio(3)|fio(3)>,
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
	return 0;
}

#endif

/* vi:set ts=4 sw=4: */
