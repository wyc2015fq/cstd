
typedef struct tjob_t {
  thread_cb run; // 任务函数
  void* arg; // 传入任务函数的参数
  struct tjob_t* next;
} tjob_t;

typedef struct {
  lock_t m_synJobList[1];
  event_t  m_hJobEvent[1];
  event_t  m_hJobFlishEvent[1];
  tjob_t* m_JobList;
  thread_t* m_WorkThred;
  int running;
  int nIniNum;
  bool waitjobs;
} tpool_t;

tjob_t* tpool_getjob(tpool_t* pool)
{
  tjob_t* pJob = NULL;
  //if ( !pool->running ) { return NULL; }
  {
    sys_lock_lock(pool->m_synJobList);
    LIST_POPFRONT(pool->m_JobList, pJob);
    sys_lock_unlock(pool->m_synJobList);
  }

  if (pJob == NULL) {
    if (pool->waitjobs) {
      sys_event_signal(pool->m_hJobFlishEvent);
    }

    if (pool->running) {
      sys_event_wait(pool->m_hJobEvent, -1);
    }
  }

  return pJob;
}

int tpool_run(void* arg)
{
  tpool_t* pool = (tpool_t*)arg;

  while (1) {
    tjob_t* pJob = tpool_getjob(pool);

    if (pJob) {
      pJob->run(pJob->arg);
      sys_free(pJob);
    }
    else if (!pool->running) {
      break;
    }
  }

  return 0;
}

int tpool_init(tpool_t* pool, int nIniNum)
{
  int i;
  pool->running = 1;
  pool->nIniNum = nIniNum;
  pool->m_WorkThred = (thread_t*)sys_malloc(nIniNum * sizeof(*pool->m_WorkThred));
  memset(pool->m_WorkThred, 0, nIniNum * sizeof(*pool->m_WorkThred));
  sys_lock_init(pool->m_synJobList);
  sys_event_init(pool->m_hJobEvent, NULL);
  sys_event_init(pool->m_hJobFlishEvent, NULL);

  for (i = 0; i < nIniNum; ++i) {
    pool->m_WorkThred[i].arg = pool;
    pool->m_WorkThred[i].run = tpool_run;
    sys_thread_create(pool->m_WorkThred + i);
  }

  return 0;
}

void tpool_addjob(tpool_t* pool, thread_cb run, void* arg)
{
  tjob_t* pJob = NULL;

  if (!pool->running) {
    return;
  }

  pJob = (tjob_t*)sys_malloc(sizeof(*pJob));
  memset(pJob, 0, sizeof(*pJob));
  pJob->run = run;
  pJob->arg = arg;
  {
    sys_lock_lock(pool->m_synJobList);
    LIST_ADDFRONT1(pool->m_JobList, pJob);
    sys_event_signal(pool->m_hJobEvent);
    sys_lock_unlock(pool->m_synJobList);
  }
}

int tpool_exit(tpool_t* pool)
{
  int i;
  pool->running = false;

  for (i = 0; i < pool->nIniNum; ++i) {
    sys_event_signal(pool->m_hJobEvent);
  }

  for (i = 0; i < pool->nIniNum; ++i) {
    sys_thread_join(pool->m_WorkThred + i);
  }

  sys_free(pool->m_WorkThred);

  sys_lock_lock(pool->m_synJobList);
  tjob_t* pJob = pool->m_JobList;

  for (; pJob; pJob = pJob->next) {
    sys_free(pJob);
  }

  sys_lock_unlock(pool->m_synJobList);
  sys_lock_destroy(pool->m_synJobList);
  sys_event_destroy(pool->m_hJobEvent);
  sys_event_destroy(pool->m_hJobFlishEvent);
  memset(pool, 0, sizeof(*pool));
  return 0;
}

void tpool_waitjobs(tpool_t* pool, const tjob_t* pJobs, int n)
{
  int i;

  if (!pool->running) {
    return;
  }

  sys_lock_lock(pool->m_synJobList);

  for (i = 0; i < n; ++i) {
    tjob_t* pJob = (tjob_t*)sys_malloc(sizeof(tjob_t));
    *pJob = pJobs[n - 1 - i];
    LIST_ADDFRONT1(pool->m_JobList, pJob);
  }

  sys_event_signal(pool->m_hJobEvent);
  sys_lock_unlock(pool->m_synJobList);
  pool->waitjobs = true;
  sys_event_wait(pool->m_hJobFlishEvent, -1);
  pool->waitjobs = false;
}

int test_tpool_func(void* arg)
{
  printf("threadid:%5d num: %5d\n", sys_thread_id(), *(int*)arg);
  sys_sleep(10);
  return 0;
}

int test_tpool()
{
  tpool_t pool[1] = {0};
  int i;
  enum {nth = 100};
  int workingnum[nth];

  if (1) {
    tjob_t job[nth] = {0};
    tpool_init(pool, 4);
    int j = 0;

    for (j = 0; j < 2; ++j) {
      for (i = 0; i < nth; i++) {
        workingnum[i] = i;
        job[i].run = test_tpool_func;
        job[i].arg = &workingnum[i];
      }

      tpool_waitjobs(pool, job, nth);
    }

    tpool_exit(pool);
  }

  if (0) {
    tpool_init(pool, 4);

    for (i = 0; i < nth; i++) {
      workingnum[i] = i;
      tpool_addjob(pool, test_tpool_func, &workingnum[i]);
    }

    //等待所有任务完成 销毁线程池
    tpool_exit(pool);
  }

  return 0;
}