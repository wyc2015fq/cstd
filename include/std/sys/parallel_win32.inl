
//#define SMP_DEBUG
#define STDERR stderr

#define MAX_CPU_NUMBER 16

typedef long BLASLONG;
typedef unsigned long BLASULONG;

typedef struct parallel_job_t {
  void *arg;
  int beg, end;
  parallel_callback fun;
  
  struct parallel_job_t *next;
  HANDLE finish_event;
} parallel_job_t;

#define LOCK_COMMAND(x) sys_lock(x)
#define UNLOCK_COMMAND(x) sys_unlock(x)

#define MB
#define WMB
#define YIELDING

/* This is a thread implementation for Win32 lazy implementation */

/* Thread server common infomation */
typedef struct{
  CRITICAL_SECTION lock;
  HANDLE filled;
  int killed;
  
  parallel_job_t *queue; /* Parameter Pointer */
  
  /* We need this grobal for cheking if initialization is finished. */
  int avail;
  
  HANDLE parallel_threads[MAX_CPU_NUMBER];
  DWORD parallel_threads_id[MAX_CPU_NUMBER];
  
  /* Local Variables */
  BLASULONG server_lock;
  int num_threads;
} parallel_server_t;

/* This is a main fun of threads. Each thread waits until job is */
/* queued.        */


static parallel_server_t gpool[1] = {0};

static DWORD WINAPI parallel_thread_server(void *arg) {
#if 0
  parallel_server_t *pool = (parallel_server_t*)arg;
  /* Thread identifier */
#ifdef SMP_DEBUG
  BLASLONG cpu = sys_thread_id();
#endif
#else
  BLASLONG cpu = (int)arg;
  parallel_server_t *pool = gpool;
#endif
  
  parallel_job_t *queue;
  DWORD action;
  
#ifdef SMP_DEBUG
  fprintf(STDERR, "Server[%2ld] Thread is started!\n", cpu);
#endif
  
  while (1) {
    action = WaitForSingleObject(pool->filled, INFINITE);
    
    if (action != WAIT_OBJECT_0) continue;
    
    if (pool->killed) break;
    
    EnterCriticalSection(&pool->lock);
    queue = pool->queue;
    if (queue) pool->queue = queue->next;
    LeaveCriticalSection(&pool->lock);
    
    if (queue) {
      if (pool->queue) SetEvent(pool->filled);
      queue->fun(queue->arg, queue->beg, queue->end);
      //fprintf(STDERR, "Server[%2ld] Running! [%d %d]\n", cpu, queue->beg, queue->end);
    } else {
      continue; //if queue == NULL
    }
    
    SetEvent(queue->finish_event);
  }
  
  /* Shutdown procedure */
  
#ifdef SMP_DEBUG
  fprintf(STDERR, "Server[%2ld] Shutdown!\n", cpu);
#endif
  
  return 0;
}


static void __inline sys_lock(volatile BLASULONG *address) {
  
  int ret;
  
  do {
    while (*address) {YIELDING;};
    
#if defined(_MSC_VER) && !defined(__clang__)
    // use intrinsic instead of inline assembly
    ret = InterlockedExchange((volatile LONG *)address, 1);
    // inline assembly
    /*__asm {
    mov eax, address
    mov ebx, 1
    xchg [eax], ebx
    mov ret, ebx
  }*/
#else
    __asm__ __volatile__(
      "xchgl %0, %1\n"
      : "=r"(ret), "=m"(*address)
      : "0"(1), "m"(*address)
      : "memory");
#endif
    
  } while (ret);
  
}

static __inline void sys_unlock(volatile BLASULONG *address) {
  MB;
  *address = 0;
}

int parallel_set_num_threads(parallel_server_t *pool, int num_threads) {
  long i;
  if (num_threads > MAX_CPU_NUMBER) num_threads = MAX_CPU_NUMBER;
  if (num_threads > pool->num_threads) {
    LOCK_COMMAND(&pool->server_lock);
    //increased_threads = 1;
    if (!pool->avail) {
      
      InitializeCriticalSection(&pool->lock);
      pool->filled = CreateEvent(NULL, FALSE, FALSE, NULL);
      pool->killed = 0;
      
      pool->queue = NULL;
      pool->avail = 1;
    }
    
#ifdef SMP_DEBUG
    fprintf(STDERR, "Initializing Thread(Num. threads = %d)\n",
      num_threads);
#endif
    
    for(i = pool->num_threads; i < num_threads; i++) {
#if 0
      pool->parallel_threads[i] = CreateThread(NULL, 0, parallel_thread_server, (void *)pool,
        0, &pool->parallel_threads_id[i]);
#else
      pool->parallel_threads[i] = CreateThread(NULL, 0, parallel_thread_server, (void *)i,
        0, &pool->parallel_threads_id[i]);
#endif
    }
    
    pool->num_threads = num_threads;
    UNLOCK_COMMAND(&pool->server_lock);
  }
  return 0;
}

// User can call one of two routines.
// parallel_async ... immediately returns after jobs are queued.
// parallel_exec ... returns after jobs are finished.

int parallel_async(parallel_server_t *pool, parallel_job_t *queue) {
  parallel_job_t *current = queue;
  while (current) {
    current->finish_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    current = current->next;
  }
  
  EnterCriticalSection(&pool->lock);
  if (pool->queue) {
    current = pool->queue;
    while (current->next) current = current->next;
    current->next = queue;
  } else {
    pool->queue = queue;
  }
  LeaveCriticalSection(&pool->lock);
  SetEvent(pool->filled);
  return 0;
}

int parallel_async_wait(parallel_server_t *pool, BLASLONG num, parallel_job_t *queue) {
  
#ifdef SMP_DEBUG
  fprintf(STDERR, "Synchronization Waiting.\n");
#endif
  
  while (num) {
#ifdef SMP_DEBUG
    fprintf(STDERR, "Waiting Queue ..\n");
#endif
    
    WaitForSingleObject(queue->finish_event, INFINITE);
    CloseHandle(queue->finish_event);
    
    queue = queue->next;
    num --;
  }
  
#ifdef SMP_DEBUG
  fprintf(STDERR, "Completely Done.\n\n");
#endif
  
  return 0;
}

/* Execute Threads */
int parallel_exec(parallel_server_t *pool, BLASLONG num, parallel_job_t *queue) {
  if ((num <= 0) || (queue == NULL)) return 0;
#if 1
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
/* kernel automatically kill threads.    */
int parallel_exit(parallel_server_t *pool) {
  int i;
  
  if (!pool->avail) return 0;
  
  LOCK_COMMAND(&pool->server_lock);
  
  if (pool->avail) {
    pool->killed = 1;
    SetEvent(pool->filled);
    
    for(i = 0; i < pool->num_threads - 1; i++) {
      WaitForSingleObject(pool->parallel_threads[i], 5); //INFINITE);
      TerminateThread(pool->parallel_threads[i],0);
    }
    
    pool->avail = 0;
  }
  
  UNLOCK_COMMAND(&pool->server_lock);
  
  return 0;
}

int parallel_for(void* arg, int beg, int end, parallel_callback fun) {
  int i, n = end-beg;
  int num = 4;
  num = BOUND(num, 1, MAX_CPU_NUMBER);
  int k = MAX(1, (n+num-1)/num);
  static int inited =0;
  parallel_job_t info[MAX_CPU_NUMBER] = {0};
  parallel_server_t *pool = gpool;
  if (!inited) {
    parallel_set_num_threads(pool, 4);
    inited = 1;
  }
  memset(info, 0, num*sizeof(parallel_job_t));
  for (i=0; i<num; ++i) {
    int b = info[i].beg = beg+i*k;
    info[i].end = MIN(b+k, end);
    info[i].fun = fun;
    info[i].arg = arg;
    info[i].next = info + i+1;
  }
  info[num-1].next = NULL;
  parallel_exec(pool, num, info);
  return 0;
}
