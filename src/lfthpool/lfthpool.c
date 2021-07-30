/* ********************************
 * Author:       Johan Hanssen Seferidis
 * License:	     MIT
 * Description:  Library providing a threading pool where you can add
 *               work. For usage, check the lfthpool.h file or README.md
 *
 *//** @file lfthpool.h *//*
 *
 ********************************/

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#if defined(__linux__)
#include <sys/prctl.h>
#endif

#include "lfthpool/lfthpool.h"

#include <concurrent/mpmc_ring_queue.h>
#include <concurrent/queuedef.h>

/* ========================== STRUCTURES ============================ */

/**
 * Struct to hold data for an individual task for a thread pool
 */
typedef struct task {
	void (*function)(void *); //pointer to the function the task executes
	void *arg;
} task_t;

/**
 * Struct to hold data for an individual thread pool.
 */
struct lfthpool {
	int shutdown;
	int hold; /* hold task queue */
	size_t running_count;	
	pthread_t *lfthpool; /* lfthpool */
	volatile size_t thread_count;
	mpmc_ring_queue *task_queue;    /* task queue */
	size_t queue_size;
};

/* ========================== THREADPOOL ============================ */

static void* _lfthpool_worker(void* _pool);

/* ========================== THREADPOOL ============================ */

lfthpool_t lfthpool_create(int workers, size_t queue_size) {
	int err;
	size_t i;
	lfthpool_t pool;

	if (workers < 1 || queue_size < 2) {
		errno = EINVAL;
		return NULL;
	}
	/* allocate new pool */
	pool = (lfthpool_t) malloc(sizeof(struct lfthpool)); 
	if (pool == NULL)
		goto ERROR_ERRNO;

	/* Pool settings */
	pool->queue_size = size_to_power_of_2((size_t) queue_size);
	pool->thread_count = (size_t) workers;

	pool->running_count = 0;
	pool->hold = 0;
	/* allocate thread array */
	pool->lfthpool = (pthread_t*) malloc(sizeof(pthread_t) * (size_t) pool->thread_count);
	/* allocate task queue */
	pool->task_queue = mpmc_ring_queue_new(pool->queue_size, NULL);

	if (pool->lfthpool == NULL || pool->task_queue == NULL) {
		err = ENOMEM;
		goto ERROR;
	}

	pool->shutdown = 0;

	for (i = 0; i < (pool->thread_count); i++) {
		pool->lfthpool[i] = 0;
	}

	/* instantiate worker lfthpool */
	for (i = 0; i < (pool->thread_count); i++) {
		if ((err = pthread_create(&pool->lfthpool[i], NULL, _lfthpool_worker, (void *) pool))) {
			goto ERROR_ERRNO;
		}
	}

	return pool;

ERROR_ERRNO:
	err = errno;
ERROR:
	if (pool) {
		lfthpool_destroy(pool);
	}
	errno = err;
	return NULL;
}

size_t lfthpool_workers_count(lfthpool_t pool) {
	return pool->thread_count;
}

int lfthpool_add_task(lfthpool_t pool, void (*function)(void *), void* arg) {
	/* set up task */
	task_t *task = malloc(sizeof(task_t));
	if (task == NULL) {
		return -1;
	}

	task->function = function;
	task->arg = arg;

	if (mpmc_ring_queue_enqueue(pool->task_queue, task) != QERR_OK) {
		free(task);
		errno = EAGAIN;
		return -1;
	}

	return 0;
}

void lfthpool_pause(lfthpool_t pool) {
	__atomic_store_n(&(pool->hold), 1, __ATOMIC_RELEASE);
}

void lfthpool_resume(lfthpool_t pool) {
	__atomic_store_n(&(pool->hold), 0, __ATOMIC_RELEASE);
}

size_t lfthpool_active_tasks(lfthpool_t pool) {
	return __atomic_add_fetch(&pool->running_count, 0, __ATOMIC_RELAXED);
}

size_t lfthpool_total_tasks(lfthpool_t pool) {
	return __atomic_add_fetch(&pool->running_count, 0, __ATOMIC_RELAXED) + mpmc_ring_queue_len_relaxed(pool->task_queue);
}

void lfthpool_wait(lfthpool_t pool) {
	while (mpmc_ring_queue_len_relaxed(pool->task_queue) + lfthpool_active_tasks(pool)) {
		sched_yield();
		usleep(10);
	}
}

void lfthpool_shutdown(lfthpool_t pool) {
	size_t i;
	__atomic_store_n(&pool->shutdown, 1, __ATOMIC_RELEASE);
	for (i = 0; i < pool->thread_count; i++) {
		pthread_join(pool->lfthpool[i], NULL);
	}
}

void lfthpool_destroy(lfthpool_t pool) {
	if (pool) {
		lfthpool_shutdown(pool);
		free(pool->lfthpool);
		mpmc_ring_queue_delete(pool->task_queue, free);
		free(pool);
	}
}

int lfthpool_worker_try_once(lfthpool_t pool) {
	task_t *task = mpmc_ring_queue_dequeue(pool->task_queue);

	/* wait for notification of new task when pool is empty */
	if (task == NULL) {
		errno = EAGAIN;
		return -1;
	}

	/* ignore thread pool hold */

	/* increment active tasks count */
	__atomic_add_fetch(&pool->running_count, 1, __ATOMIC_RELAXED);

	/* grab the next task in the queue and run it */

	/* execute task*/
	(*task->function)(task->arg);

	free(task);

	__atomic_sub_fetch(&pool->running_count, 1, __ATOMIC_RELAXED);

	return 0;
}

/* pool background worker */
static void* _lfthpool_worker(void* p) {
	lfthpool_t pool = (lfthpool_t) p;

	while (1) {
		task_t *task;

		/* check shutdown flag */		
		if (__atomic_add_fetch(&pool->shutdown, 0, __ATOMIC_ACQUIRE) == 1) {
			return NULL;
		}

		/* check thread pool hold */
		if ( __atomic_add_fetch(&(pool->hold), 0, __ATOMIC_RELEASE)) {
			sleep(1);
			continue;
		}

		/* wait for notification of new task when pool is empty */
		if ((task = mpmc_ring_queue_dequeue(pool->task_queue)) == NULL) {
			usleep(1);
			continue;
		}

		/* increment active tasks count */
		__atomic_add_fetch(&pool->running_count, 1, __ATOMIC_RELAXED);
		
		/* execute task*/
		(task->function)(task->arg);

		/* decrement active tasks count */
		__atomic_sub_fetch(&pool->running_count, 1, __ATOMIC_RELAXED);

		free(task);
	}

	return NULL;
}

/* ========================== THREADPOOL THREAD ===================== */
