/* 要执行的任务链表 */
typedef struct tpool_work {
  thread_cb fun; /* 任务函数 */
  void* arg; /* 传入任务函数的参数 */
  struct tpool_work* next;
} tpool_work_t;
typedef struct tpool {
  int shutdown; /* 线程池是否销毁 */
  int max_thr_num; /* 最大线程数 */
  thread_t* thr_id; /* 线程ID数组 */
  tpool_work_t* queue_head; /* 线程链表 */
  critical_section_t queue_lock[1];
  cond_t queue_ready[1];
} tpool_t1;
static tpool_t1* tpool = NULL;
/* 工作者线程函数, 从任务链表中取出任务并执行 */
static void* thread_routine(void* arg)
{
  tpool_work_t* work;
  while (1) {
    /* 如果线程池没有被销毁且没有任务要执行，则等待 */
    while (!tpool->queue_head && !tpool->shutdown) {
      cond_wait(tpool->queue_ready, INFINITE);
    }
    critical_section_lock(tpool->queue_lock);
    if (tpool->shutdown) {
      critical_section_unlock(tpool->queue_lock);
      break;
    }
    work = tpool->queue_head;
    tpool->queue_head = tpool->queue_head->next;
    critical_section_unlock(tpool->queue_lock);
    work->fun(work->arg);
    free(work);
  }
  return NULL;
}
/*
 * 创建线程池
 */
int tpool_init(int max_thr_num)
{
  int i;
  tpool = (tpool_t1*)calloc(1, sizeof(tpool_t1));
  if (!tpool) {
    printf("%s: calloc failed\n", __FILE__);
    return 0;
  }
  /* 初始化 */
  tpool->max_thr_num = max_thr_num;
  tpool->shutdown = 0;
  tpool->queue_head = NULL;
  critical_section_init(tpool->queue_lock);
  cond_init(tpool->queue_ready);
  /* 创建工作者线程 */
  tpool->thr_id = (thread_t*)calloc(max_thr_num, sizeof(thread_t));
  for (i = 0; i < max_thr_num; ++i) {
    thread_create(tpool->thr_id + i, thread_routine, NULL);
  }
  return 0;
}
/* 销毁线程池 */
void tpool_destroy()
{
  int i;
  tpool_work_t* member;
  if (tpool->shutdown) {
    return;
  }
  tpool->shutdown = 1;
  /* 通知所有正在等待的线程 */
  critical_section_lock(tpool->queue_lock);
  cond_signal(tpool->queue_ready);
  critical_section_unlock(tpool->queue_lock);
  for (i = 0; i < tpool->max_thr_num; ++i) {
    thread_join(tpool->thr_id + i);
  }
  free(tpool->thr_id);
  while (tpool->queue_head) {
    member = tpool->queue_head;
    tpool->queue_head = tpool->queue_head->next;
    free(member);
  }
  critical_section_destroy(tpool->queue_lock);
  cond_destroy(tpool->queue_ready);
  free(tpool);
}
/* 向线程池添加任务 */
int tpool_create(void* fun, void* arg)
{
  tpool_work_t* work, *member;
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
  critical_section_lock(tpool->queue_lock);
  member = tpool->queue_head;
  if (!member) {
    tpool->queue_head = work;
  }
  else {
    while (member->next) {
      member = member->next;
    }
    member->next = work;
  }
  /* 通知工作者线程，有新任务添加 */
  critical_section_unlock(tpool->queue_lock);
  cond_signal(tpool->queue_ready);
  return 0;
}

