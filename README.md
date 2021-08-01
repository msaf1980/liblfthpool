# Thread pool

# lfthpool_t (mutex-locked thread pool without allocation during task add)

This is a minimal threadpool implementation

  * ANCI C and POSIX compliant
  * Pause/resume/wait as you like

The threadpool is under MIT license.

## Basic usage

1. Include the header in your source file: `#include <lfthpool/lfthpool.h>
2. Create a thread pool with number of lfthpool you want: `threadpool pool = lfthpool_init(4, 1024);`
3. Add work to the pool: `lfthpool_add_task(pool, (void*)function_p, (void*)arg_p);`

The workers(lfthpool) will start their work automatically as fast as there is new work
in the pool. If you want to wait for all added work to be finished before continuing
you can use `lfthpool_wait(pool);`. If you want to destroy the pool you can use
`lfthpool_destroy(pool);`.


## API

For a deeper look into the documentation check in the [lfthpool.h](https://github.com/Pithikos/C-Thread-Pool/blob/master/lfthpool.h) file. Below is a fast practical overview.

| Function example                | Description                                                         |
|---------------------------------|---------------------------------------------------------------------|
| ***lfthpool_create(4, 1024)***            | Will return a new threadpool with `4` lfthpool and 1024 max queued (unproccessed) tasks.                        |
| ***lfthpool_t lfthpool_create_sched(4, 1024, coro_yield)***             | Will return a new threadpool with `4` lfthpool, 1024 max queued (unproccessed) tasks and sleep function, integrated with custom scheduler.                        |
| ***lfthpool_workers_count(pool)*** | Will return count of workers lfthpool in thread poool               |
| ***lfthpool_add_task(pool, (void&#42;)function_p, (void&#42;)arg_p)*** | Will add new work to the pool. Work is simply a function. You can pass a single argument to the function if you wish. If not, `NULL` should be passed. Failed, if task queue is full. |
| ***lfthpool_add_task_try(pool, (void&#42;)function_p, (void&#42;)arg_p, usec, max_try)*** | Will add new work to the pool. Work is simply a function. You can pass a single argument to the function if you wish. If not, `NULL` should be passed. |
| ***lfthpool_wait(pool)***       | Will wait for all jobs (both in queue and currently running) to finish. |
| ***lfthpool_destroy(pool)***    | This will destroy the threadpool. If jobs are currently being executed, then it will wait for them to finish. |
| ***lfthpool_pause(pool)***      | All lfthpool in the threadpool will pause no matter if they are idle or executing work. |
| ***lfthpool_resume(pool)***      | If the threadpool is paused, then all lfthpool will resume from where they were.   |
| ***lfthpool_active_tasks(pool)***  | Will return the number of active tasks (currently working lfthpool).   |
| ***lfthpool_total_tasks(pool)***  | Will return the number of tasks (queued and active).   |
| ***lfthpool_worker_try_once(pool)***  | Process task in current thread (foreground).   |
