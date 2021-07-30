#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sched.h>

#include <lfthpool/lfthpool.h>

#include <ctest.h>

static void sleep_job(void *p){
	int *n = (int *) p;
	__atomic_add_fetch(n, 1, __ATOMIC_RELEASE);
	puts("SLEPT");
}


CTEST(lfthpool_api, test) {
	lfthpool_t pool;
	int n;

	/* Test if we can get the current number of working lfthpool */
	n = 0;
	pool = lfthpool_create(10, 10);
	lfthpool_add_task(pool, sleep_job, &n);
	lfthpool_add_task(pool, sleep_job, &n);
	lfthpool_add_task(pool, sleep_job, &n);
	lfthpool_add_task(pool, sleep_job, &n);
	
	lfthpool_wait(pool);

	ASSERT_EQUAL_U(0, lfthpool_active_tasks(pool));
	ASSERT_EQUAL_U(0, lfthpool_total_tasks(pool));
	ASSERT_EQUAL(4, __atomic_add_fetch(&n, 0, __ATOMIC_RELEASE));

	lfthpool_destroy(pool);

	/* Test (same as above) */
	n  = 0;
	pool = lfthpool_create(5, 5);
	lfthpool_add_task(pool, sleep_job, &n);
	lfthpool_add_task(pool, sleep_job, &n);

	lfthpool_wait(pool);

	ASSERT_EQUAL_U(0, lfthpool_active_tasks(pool));
	ASSERT_EQUAL_U(0, lfthpool_total_tasks(pool));
	ASSERT_EQUAL(2, __atomic_add_fetch(&n, 0, __ATOMIC_RELEASE));

	lfthpool_destroy(pool);
}
