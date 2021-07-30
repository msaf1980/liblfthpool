/*
 * Try to run lfthpool with a non-zero heap and stack
 */
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <lfthpool/lfthpool.h>

size_t LOOP_COUNT = 10000000;

size_t v = 0;
int ret = 0;

static void task(void *arg) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"	
	size_t *n = (size_t *)arg;
	__atomic_add_fetch(n, 1, __ATOMIC_RELAXED);
#pragma GCC diagnostic pop
}

static uint64_t getCurrentTime(void) {
    struct timeval now;
    uint64_t now64;
    gettimeofday(&now, NULL);
    now64 = (uint64_t) now.tv_sec;
    now64 *= 1000000;
    now64 += ((uint64_t) now.tv_usec);
    return now64;
}

void bench(size_t loop_count, int lfthpool) {
	size_t i;
	uint64_t start, end, duration;
	lfthpool_t pool;
	v = 0;
	pool = lfthpool_create(lfthpool, LOOP_COUNT);

	start = getCurrentTime();
	
	for (i = 0; i < loop_count; i++) {
		lfthpool_add_task(pool, task, (void *)&v);
	};
	lfthpool_wait(pool);

	end = getCurrentTime();

	lfthpool_destroy(pool);
	duration = end - start;
	if (v != loop_count) {
		ret++;	
	}
	printf("%d lfthpool (%f ms, %lu iterations, %llu ns/op, %llu op/s) [%s]\n",
		lfthpool,
		((double) end - (double) start) / 1000,
		(unsigned long) loop_count,
		(unsigned long long) duration * 1000 / loop_count,
		(unsigned long long) 1000000 * loop_count / duration,			
		v == loop_count ? "OK" : "ERR");
}

int main() {
	char *COUNT_STR = getenv("LOOP_COUNT");
	if (COUNT_STR) {
		unsigned long c = strtoul(COUNT_STR, NULL, 10);
		if (c > 0) {
			LOOP_COUNT = c;
		}
	}
	bench(LOOP_COUNT, 4);
	return ret;
}
