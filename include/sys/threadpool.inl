
#ifndef _THREADPOOL_INL_
#define _THREADPOOL_INL_

#include "cstd.h"

#define THREADPOOL_MAX  16

typedef struct jobgroup_t {
  job_t* jobs;
  int candel;
  int finished_count;
  int count;
  event_t finished[1];
  struct jobgroup_t* prev;
  struct jobgroup_t* next;
}jobgroup_t;

struct threadpool_t;
struct threadpool_worker_t {
  thread_t thd[1];
  int running;
  struct threadpool_t* pool;
};

struct threadpool_t
{
  lock_t synjoblist[1];
  event_t m_hJobEvent[1];
  jobgroup_t*  groups;
  threadpool_worker_t work[THREADPOOL_MAX];
  int nwork;
  int inited;
};

int worker_run(void* x);
void threadpool_init(threadpool_t* pool, int nIniNum)
{
  if (!pool->inited) {
    int i;
    nIniNum = BOUND(1, nIniNum, THREADPOOL_MAX);
    pool->nwork = nIniNum;
    sys_lock_init(pool->synjoblist);
    sys_event_init(pool->m_hJobEvent, 0, 0);
    for (i = 0; i < nIniNum; ++i) {
      pool->work[i].running = 1;
      pool->work[i].thd->run = worker_run;
      pool->work[i].thd->arg = pool->work+i;
      pool->work[i].pool = pool;
      sys_thread_create(pool->work[i].thd);
    }
    pool->inited = 1;
  }
}

job_t* jobgroup_get_job(jobgroup_t *g) {
  job_t* pJob = NULL;
  LIST_POPFRONT(g->jobs, pJob);
  return pJob;
}
job_t* threadpool_get_job(threadpool_t *pool, jobgroup_t** pg) {
  job_t* pJob = NULL;
  if (!pool->inited) {
    return NULL;
  }
  sys_lock_lock(pool->synjoblist);
  //LIST_POPFRONT(pool->groups, pJob);
  jobgroup_t* g = pool->groups;
  for (; g && pJob==NULL; g = g->next) {
    if (g->jobs) {
      pJob = jobgroup_get_job(g);
      *pg = g;
    }
  }
  sys_lock_unlock(pool->synjoblist);
  
  if (pJob == NULL && pool->inited) {
    sys_event_wait(pool->m_hJobEvent, CC_INFINITE);
  }
  
  return pJob;
}

int worker_run(void* x) {
  threadpool_worker_t* worker = (threadpool_worker_t*)x;
  threadpool_t *pool = worker->pool;
  while (worker->running) {
    jobgroup_t* g = NULL;
    job_t* pJob = threadpool_get_job(pool, &g);
    if (pJob) {
      pJob->run(pJob->arg);
      sys_lock_lock(pool->synjoblist);
      g->count--;
      if (g->count<=0) {
        sys_event_signal(g->finished);
        LIST_DEL(pool->groups, g);
      }
      sys_lock_unlock(pool->synjoblist);
    }
  }
  return 0;
}

int jobgroup_free(jobgroup_t* g) {
  sys_event_destroy(g->finished);
  while (g->jobs) {
    job_t *pJob = NULL;
    LIST_POPFRONT(g->jobs, pJob);
  }
  return 0;
}
int jobgroup_add_job(jobgroup_t* g, job_t* pJob) {
  LIST_ADDFRONT1(g->jobs, pJob);
  //LIST_PUSHBACK_T(g->jobs, pJob, job_t);
  g->count++;
  return 0;
}
int threadpool_free(threadpool_t* pool)
{
  if(pool && pool->inited) {
    int i;
    pool->inited = 0;
    for (i=0; i<pool->nwork; ++i) {
      pool->work[i].running = 0;
      sys_event_signal(pool->m_hJobEvent);
      sys_thread_join(pool->work[i].thd);
    }
    pool->nwork = 0;
    sys_lock_lock(pool->synjoblist);
    while (pool->groups) {
      jobgroup_t* g = NULL;
      LIST_POPFRONT(pool->groups, g);
      jobgroup_free(g);
    }
    sys_lock_unlock(pool->synjoblist);
    sys_lock_destroy(pool->synjoblist);
    sys_event_destroy(pool->m_hJobEvent);
  }
  return 0;
}

#if 1

typedef int (*parallel_callback)(void* arg, int beg, int end);

struct parallel_info_t {
  job_t job[1];
  int beg, end;
  void* arg;
  parallel_callback fun;
};

int parallel_for_work(void* arg)
{
  parallel_info_t *info = (parallel_info_t*)arg;
  int id = sys_thread_id();
  info->fun(info->arg, info->beg, info->end);
  return 0;
}

static threadpool_t g_pool[1] = {0};

int jobgroup_join(jobgroup_t* g, threadpool_t* pool) {
  if (pool) {
    if (!pool->inited) {
      threadpool_init(pool, 4);
    }
    sys_event_init(g->finished, 0, 0);
    sys_lock_lock(pool->synjoblist);
    LIST_ADDFRONT1(pool->groups, g);
    sys_lock_unlock(pool->synjoblist);
    int n = g->count;
    for (;--n;) {
      sys_event_signal(pool->m_hJobEvent);
    }
    sys_event_wait(g->finished, CC_INFINITE);
  } else {
    job_t* job = g->jobs;
    for (;job;job=job->next) {
      sys_thread_create(job);
    }
    while (g->jobs) {
      LIST_POPFRONT(g->jobs, job);
      sys_thread_join(job);
    }
  }
  return 0;
}

int parallel_for(void* arg, int beg, int end, parallel_callback fun, threadpool_t* pool) {
  jobgroup_t jobgroup[1] = {0};
  int i, n = end-beg;
  int k = MAX(1, n/(8));
  parallel_info_t *info = MALLOC(parallel_info_t, n);
  memset(info, 0, n*sizeof(parallel_info_t));
  for (i=0; i<end; i+=k) {
    info[i].beg = beg+i;
    info[i].end = MIN(beg+i+k, end);
    info[i].fun = fun;
    info[i].arg = arg;
    info[i].job->run = parallel_for_work;
    info[i].job->arg = info+i;
    jobgroup_add_job(jobgroup, info[i].job);
  }
  jobgroup_join(jobgroup, pool);
  jobgroup_free(jobgroup);
  FREE(info);
  return 0;
}
#endif

#endif // _THREADPOOL_INL_
