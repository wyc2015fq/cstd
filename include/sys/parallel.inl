#undef LOCK_COMMAND
#undef MAX_CPU_NUMBER
#undef MB
#undef STDERR
#undef UNLOCK_COMMAND
#undef WMB
#undef YIELDING


#define STDERR stderr
#define MAX_CPU_NUMBER 16

typedef struct parallel_job_t {
  void *arg;
  int beg, end;
  parallel_callback fun;
  struct parallel_job_t *next;
  event_t finish_event[1];
} parallel_job_t;


#define LOCK_COMMAND(x)   sys_spin_lock(x)
#define UNLOCK_COMMAND(x) sys_spin_unlock(x)

#define MB
#define WMB
#define YIELDING

/* This is a thread implementation for Win32 lazy implementation */

/* Thread server common infomation */
typedef struct{
  lock_t lock[1];
  event_t filled[1];
  int killed;
  
  parallel_job_t *queue; /* Parameter Pointer */
  
  /* We need this grobal for cheking if initialization is finished. */
  int avail;
  
  thread_t parallel_threads[MAX_CPU_NUMBER];
  
  /* Local Variables */
  long server_lock;
  int num_threads;
} parallel_server_t;

/* This is a main fun of threads. Each thread waits until job is */
/* queued. */


static int parallel_thread_server(void *arg) {
  parallel_server_t *pool = (parallel_server_t*)arg;
  parallel_job_t *queue;
  
  while (1) {
    sys_event_wait(pool->filled, CC_INFINITE);
    if (pool->killed) break;
    sys_lock_lock(pool->lock);
    queue = pool->queue;
    if (queue) pool->queue = queue->next;
    sys_lock_unlock(pool->lock);
    
    if (queue) {
      if (pool->queue) sys_event_signal(pool->filled);
      queue->fun(queue->arg, queue->beg, queue->end);
      //fprintf(STDERR, "Server[%2ld] Running! [%d %d]\n", cpu, queue->beg, queue->end);
    } else {
      continue; //if queue == NULL
    }
    
    sys_event_signal(queue->finish_event);
  }
  
  return 0;
}

static int parallel_set_num_threads(parallel_server_t *pool, int num_threads) {
  long i;
  if (num_threads > MAX_CPU_NUMBER) num_threads = MAX_CPU_NUMBER;
  if (num_threads > pool->num_threads) {
    LOCK_COMMAND(&pool->server_lock);
    //increased_threads = 1;
    if (!pool->avail) {
      sys_lock_init(pool->lock);
      sys_event_init(pool->filled, 0, 0);
      pool->killed = 0;
      
      pool->queue = NULL;
      pool->avail = 1;
    }
    
    for(i = pool->num_threads; i < num_threads; i++) {
      pool->parallel_threads[i].run = parallel_thread_server;
      pool->parallel_threads[i].arg = pool;
      sys_thread_create(pool->parallel_threads+i);
    }
    
    pool->num_threads = num_threads;
    UNLOCK_COMMAND(&pool->server_lock);
  }
  return 0;
}

// User can call one of two routines.
// parallel_async ... immediately returns after jobs are queued.
// parallel_exec ... returns after jobs are finished.
static int parallel_async(parallel_server_t *pool, parallel_job_t *queue) {
  parallel_job_t *current = queue;
  while (current) {
    sys_event_init(current->finish_event, 0, 0);
    current = current->next;
  }
  
  sys_lock_lock(pool->lock);
  if (pool->queue) {
    current = pool->queue;
    while (current->next) current = current->next;
    current->next = queue;
  } else {
    pool->queue = queue;
  }
  sys_lock_unlock(pool->lock);
  sys_event_signal(pool->filled);
  return 0;
}

static int parallel_async_wait(parallel_server_t *pool, long num, parallel_job_t *queue) {
  while (num && queue) {
    sys_event_wait(queue->finish_event, CC_INFINITE);
    sys_event_destroy(queue->finish_event);
    queue = queue->next;
    num --;
  }
  return 0;
}

/* Execute Threads */
static int parallel_exec(parallel_server_t *pool, long num, parallel_job_t *queue) {
  if ((num <= 0) || (queue == NULL)) return 0;
#if 0
  if ((num > 1) && queue->next) {
    parallel_async(pool, queue->next);
  }
  queue->fun(queue->arg, queue->beg, queue->end);
  if ((num > 1) && queue->next) {
    parallel_async_wait(pool, num - 1, queue->next);
  }
#else
  parallel_async(pool, queue);
  parallel_async_wait(pool, num, queue);
#endif
  return 0;
}

/* Shutdown procedure, but user don't have to call this fun. The */
/* kernel automatically kill threads. */
static int parallel_exit(parallel_server_t *pool) {
  int i;
  if (!pool->avail) return 0;
  LOCK_COMMAND(&pool->server_lock);
  
  if (pool->avail) {
    pool->killed = 1;
    sys_event_signal(pool->filled);
    
    for(i = 0; i < pool->num_threads - 1; i++) {
      sys_thread_join(pool->parallel_threads+i); //INFINITE);
      //TerminateThread(pool->parallel_threads[i],0);
    }
    
    pool->avail = 0;
  }
  
  UNLOCK_COMMAND(&pool->server_lock);
  return 0;
}

int parallel_for(void* arg, int beg, int end, parallel_callback fun) {
  static parallel_server_t gpool[1] = {0};
  int i, k, n = end-beg, num = 8;
  static int inited =0;
  num = BOUND(num, 1, MAX_CPU_NUMBER);
  k = MAX(1, (n+num-1)/num);
  if (beg < end) {
    if (num>1) {
      parallel_job_t info[MAX_CPU_NUMBER] = { 0 };
      parallel_server_t *pool = gpool;
      if (!inited) {
        parallel_set_num_threads(pool, 4);
        inited = 1;
      }
      //memset(info, 0, num*sizeof(parallel_job_t));
      for (i = 0; i < num; ++i) {
        int b = info[i].beg = beg + i*k;
        info[i].end = MIN(b + k, end);
        info[i].fun = fun;
        info[i].arg = arg;
        info[i].next = info + i + 1;
      }
      info[num - 1].next = NULL;
      parallel_exec(pool, num, info);
    } else {
      fun(arg, beg, end);
    }
  }
  return 0;
}
