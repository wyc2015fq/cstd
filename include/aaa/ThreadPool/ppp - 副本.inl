/* 要执行的任务链表 */
typedef struct tpool_work {
  thread_cb fun; /* 任务函数 */
  void* arg; /* 传入任务函数的参数 */
  struct tpool_work* next;
} tpool_work_t;
#define MAX_THR_NUM 4
typedef struct pool {
  int shutdown; /* 线程池是否销毁 */
  thread_t thr_id[MAX_THR_NUM]; /* 线程ID数组 */
  tpool_work_t* queue_head; /* 线程链表 */
  critical_section_t queue_lock[1];
  cond_t queue_ready[1];
} tpool_t;
/* 工作者线程函数, 从任务链表中取出任务并执行 */
static void* thread_routine(void* arg)
{
  tpool_t* pool = (tpool_t*)arg;
  tpool_work_t* work;
  while (1) {
    /* 如果线程池没有被销毁且没有任务要执行，则等待 */
    while (!pool->queue_head && !pool->shutdown) {
      cond_wait(pool->queue_ready, INFINITE);
    }
    critical_section_lock(pool->queue_lock);
    if (pool->shutdown) {
      critical_section_unlock(pool->queue_lock);
      break;
    }
    work = pool->queue_head;
    pool->queue_head = pool->queue_head->next;
    critical_section_unlock(pool->queue_lock);
    work->fun(work->arg);
    free(work);
  }
  return NULL;
}
tpool_t* tpool_get(tpool_work_t* work, int opt);
/* 销毁线程池 */
int tpool_destroy()
{
  tpool_get(NULL, 3);
  return 0;
}
tpool_t* tpool_get(tpool_work_t* work, int opt)
{
  static tpool_t pool[1] = {0};
  int i;
  static int inited = 0;
  if (3 != opt) {
    if (!inited) {
      /* 初始化 */
      pool->shutdown = 0;
      pool->queue_head = NULL;
      critical_section_init(pool->queue_lock);
      cond_init(pool->queue_ready);
      /* 创建工作者线程 */
      for (i = 0; i < MAX_THR_NUM; ++i) {
        thread_create(pool->thr_id + i, thread_routine, pool);
      }
      inited = 1;
    }
    if (2 == opt) {
      tpool_work_t* member;
      critical_section_lock(pool->queue_lock);
      member = pool->queue_head;
      if (!member) {
        pool->queue_head = work;
      }
      else {
        while (member->next) {
          member = member->next;
        }
        member->next = work;
      }
      /* 通知工作者线程，有新任务添加 */
      critical_section_unlock(pool->queue_lock);
      cond_signal(pool->queue_ready);
    }
  }
  else {
    if (inited) {
      tpool_work_t* member;
      if (pool->shutdown) {
        return 0;
      }
      pool->shutdown = 1;
      /* 通知所有正在等待的线程 */
      critical_section_lock(pool->queue_lock);
      cond_signal(pool->queue_ready);
      critical_section_unlock(pool->queue_lock);
      for (i = 0; i < MAX_THR_NUM; ++i) {
        thread_join(pool->thr_id + i);
      }
      free(pool->thr_id);
      while (pool->queue_head) {
        member = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        free(member);
      }
      critical_section_destroy(pool->queue_lock);
      cond_destroy(pool->queue_ready);
      free(pool);
    }
  }
  return pool;
}
/* 向线程池添加任务 */
int tpool_create(void* fun, void* arg)
{
  tpool_work_t* work;
  if (!fun) {
    printf("%s:Invalid argument\n", __FILE__);
    return -1;
  }
  work = (tpool_work_t*)malloc(sizeof(tpool_work_t));
  if (!work) {
    printf("%s:malloc failed\n", __FILE__);
    return -1;
  }
  work->fun = (thread_cb)fun;
  work->arg = arg;
  work->next = NULL;
  tpool_get(work, 2);
  return 0;
}

