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

#ifndef LIBSLACK_THREAD_H
#define LIBSLACK_THREAD_H

#include <pthread.h>

#include <slack/hdr.h>

#ifdef NEEDS_PTHREAD_RWLOCK

typedef struct pthread_rwlock_t pthread_rwlock_t;
typedef struct pthread_rwlockattr_t pthread_rwlockattr_t;

struct pthread_rwlock_t
{
	pthread_mutex_t lock;   /* Lock for structure */
	pthread_cond_t readers; /* Are there readers waiting? */
	pthread_cond_t writers; /* Are there writers waiting? */
	int waiters;            /* Number of writers waiting */
	int state;              /* State: -1 -> writer, 0 -> idle, +ve -> readers */
};

struct pthread_rwlockattr_t
{
	int pshared;            /* Shared between processes or not */
};

#define PTHREAD_RWLOCK_INITIALIZER \
	{ \
		PTHREAD_MUTEX_INITIALIZER, \
		PTHREAD_COND_INITIALIZER, \
		PTHREAD_COND_INITIALIZER, \
		0, 0 \
	}

#endif

#ifdef NEEDS_PTHREAD_BARRIER

typedef struct pthread_barrier_t pthread_barrier_t;
typedef struct pthread_barrierattr_t pthread_barrierattr_t;

struct pthread_barrier_t
{
	pthread_mutex_t lock;  /* Lock for structure */
	pthread_cond_t cond;   /* Waiting list control */
	int count;             /* Number of threads to wait for */
	int sleepers;          /* Number of threads waiting */
	int releasing;         /* Still waking up sleepers */
};

struct pthread_barrierattr_t
{
	int pshared;           /* Shared between processes or not */
};

#define PTHREAD_BARRIER_INITIALIZER \
	{ \
		PTHREAD_MUTEX_INITIALIZER, \
		PTHREAD_COND_INITIALIZER, \
		0, 0, 0 \
	}

#endif

__START_DECLS
int thread_attr_init __PROTO ((pthread_attr_t *attr));
int thread_attr_set __PROTO ((pthread_attr_t *attr, int scope, int detachstate, int inheritsched, int schedpolicy, struct sched_param *schedparam));
int thread_setcancel __PROTO ((int type, int state, int *oldtype, int *oldstate));
int thread_init __PROTO (());
#ifdef NEEDS_PTHREAD_RWLOCK
int pthread_rwlock_init __PROTO ((pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr));
int pthread_rwlock_destroy __PROTO ((pthread_rwlock_t *rwlock));
int pthread_rwlock_rdlock __PROTO ((pthread_rwlock_t *rwlock));
int pthread_rwlock_tryrdlock __PROTO ((pthread_rwlock_t *rwlock));
int pthread_rwlock_wrlock __PROTO ((pthread_rwlock_t *rwlock));
int pthread_rwlock_trywrlock __PROTO ((pthread_rwlock_t *rwlock));
int pthread_rwlock_unlock __PROTO ((pthread_rwlock_t *rwlock));
int pthread_rwlockattr_init __PROTO ((pthread_rwlockattr_t *attr));
int pthread_rwlockattr_destroy __PROTO ((pthread_rwlockattr_t *attr));
int pthread_rwlockattr_getpshared __PROTO ((const pthread_rwlockattr_t * attr, int *pshared));
int pthread_rwlockattr_setpshared __PROTO ((pthread_rwlockattr_t *attr, int pshared));
#endif
#ifdef NEEDS_PTHREAD_BARRIER
int pthread_barrier_init __PROTO ((pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count));
int pthread_barrier_destroy __PROTO ((pthread_barrier_t *barrier));
int pthread_barrier_wait __PROTO ((pthread_barrier_t *barrier));
int pthread_barrierattr_init __PROTO ((pthread_barrierattr_t *attr));
int pthread_barrierattr_destroy __PROTO ((pthread_barrierattr_t *attr));
int pthread_barrierattr_getpshared __PROTO ((const pthread_barrierattr_t *attr, int *pshared));
int pthread_barrierattr_setpshared __PROTO ((pthread_barrierattr_t *attr, int pshared));
#endif
#ifdef NEEDS_PTHREAD_SIGNAL_LINUX_HACK
/* XXX linux mt signal handling fix */
#endif
__END_DECLS

#endif

/* vi:set ts=4 sw=4: */
