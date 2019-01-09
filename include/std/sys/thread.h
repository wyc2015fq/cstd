#ifndef _THREAD_INL_
#define _THREAD_INL_
#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif // ASSERT
#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
typedef HANDLE thread_t;
typedef HANDLE mutex_t;
typedef HANDLE cond_t;

#define sleep(x)  Sleep(x)
#define getpid() GetCurrentProcessId()
#define thread_exit  thread_detach
#else
#include <pthread.h>
typedef pthread_t thread_t;
typedef void* (*thread_cb)(void* pParam);
static int thread_create(thread_t* th, thread_cb fun, void* pParam)
{
  return pthread_create(th, NULL, (thread_cb)fun, pParam);
}
static int thread_join(thread_t* hThread)
{
  unsigned long m_nRet = 0;
  int ret = 0;
  ret = (0 == pthread_join(*hThread, (void**)&m_nRet));
  return ret;
}
#define thread_detach(hThread)  (0 == pthread_detach(hThread))
#define thread_self(th)    (int)pthread_self()
#define WINAPI
#define INFINITE 0xFFFFFFFF
typedef struct critical_section_t {
  pthread_mutex_t   m_sect;
  pthread_mutexattr_t m_attr;
} critical_section_t;

static int critical_section_init(critical_section_t* c)
{
  pthread_mutexattr_init(&c->m_attr);
  pthread_mutexattr_settype(&c->m_attr, PTHREAD_MUTEX_RECURSIVE_NP);
  pthread_mutex_init(&c->m_sect, &c->m_attr);
  return 0;
}
static int critical_section_destroy(critical_section_t* c)
{
  pthread_mutexattr_destroy(&c->m_attr);
  pthread_mutex_destroy(&c->m_sect);
  return 0;
}
#define critical_section_lock  critical_section_enter
#define critical_section_unlock  critical_section_leave
static int critical_section_enter(critical_section_t* c)
{
  pthread_mutex_lock(&c->m_sect);
  return 0;
}
static int critical_section_leave(critical_section_t* c)
{
  pthread_mutex_unlock(&c->m_sect);
  return 0;
}
typedef struct cond_t {
  pthread_cond_t    m_hEvent;
  pthread_mutex_t   m_sect;
  pthread_mutexattr_t m_attr;
} cond_t;
static int cond_init(cond_t* c) {
  pthread_cond_init(&c->m_hEvent, 0);
  pthread_mutexattr_init(&c->m_attr);
  pthread_mutexattr_settype(&c->m_attr, PTHREAD_MUTEX_TIMED_NP);
  pthread_mutex_init(&c->m_sect, &c->m_attr);
  return 0;
}
static int cond_destroy(cond_t* c) {
  pthread_cond_destroy(&c->m_hEvent);
  pthread_mutexattr_destroy(&c->m_attr);
  pthread_mutex_destroy(&c->m_sect);
  return 0;
}
static int cond_wait(cond_t* c, int dwMilliseconds) {
  pthread_mutex_lock(&c->m_sect);
  if (dwMilliseconds == INFINITE) {
    pthread_cond_wait(&c->m_hEvent, &c->m_sect);
  }
  else {
    struct timespec timeout;
    timeout.tv_sec = 0;
    timeout. tv_nsec = dwMilliseconds * 10000000;
    pthread_cond_timedwait(&c->m_hEvent, &c->m_sect, &timeout);
  }
  pthread_mutex_unlock(&c->m_sect);
  return 0;
}
static int cond_signal(cond_t* c) {
  pthread_mutex_lock(&c->m_sect);
  pthread_cond_signal(&c->m_hEvent);
  pthread_mutex_unlock(&c->m_sect);
  return 0;
}
static int thread_joins(int n, thread_t* th) {
  int i;
  for (i=0; i<n; ++i) {
    thread_join(th+i);
  }
  return 0;
}
#endif
// 要执行的任务
typedef struct tpool_job_t {
  thread_cb fun; // 任务函数
  void* arg; // 传入任务函数的参数
} tpool_job_t;
#define MAXTHNUM 2
typedef struct tpool_t {
  int shutdown; // 线程池是否销毁
  thread_t th[MAXTHNUM]; // 线程ID数组
  tpool_job_t** joblist;
  int jobnum;
  int thnum;
  cond_t m_hJobEvent[1];
  critical_section_t m_synJobList[1];
} tpool_t;
// 工作者线程函数, 从任务链表中取出任务并执行
static tpool_job_t* tpool_opt(tpool_job_t* job, int opt);
static void* tpool_thread_run(void* arg)
{
  tpool_t* pool = (tpool_t*)arg;
  while (!pool->shutdown) {
    tpool_job_t* job = tpool_opt(NULL, 2);
    if (job) {
      if (job->fun) {
        job->fun(job->arg);
      }
      free(job);
    }
  }
  return 0;
}
static int tpool_create(void* fun, void* arg)
{
  tpool_job_t* job = (tpool_job_t*)malloc(sizeof(tpool_job_t));
  memset(job, 0, sizeof(tpool_job_t));
  job->fun = (thread_cb)fun;
  job->arg = arg;
  tpool_opt(job, 1);
  return 0;
}
static int tpool_joinall()
{
  tpool_opt(NULL, 3);
  return 0;
}
static tpool_job_t* tpool_opt(tpool_job_t* job, int opt)
{
  static tpool_t pool[1] = {0};
  static int inited = 0;
  int i;
  if (!inited) {
    inited = 1;
    critical_section_init(pool->m_synJobList);
    cond_init(pool->m_hJobEvent);
  }
  switch (opt) {
  case 1:
    critical_section_enter(pool->m_synJobList);
    pool->jobnum++;
    pool->joblist = (tpool_job_t**)realloc(pool->joblist, sizeof(void*)*pool->jobnum);
    pool->joblist[pool->jobnum - 1] = job;
    cond_signal(pool->m_hJobEvent);
    if (pool->jobnum > pool->thnum && pool->thnum < MAXTHNUM) {
      thread_create(pool->th + pool->thnum, tpool_thread_run, pool);
      pool->thnum++;
    }
    critical_section_leave(pool->m_synJobList);
    return 0;
  case 2:
    job = NULL;
    cond_wait(pool->m_hJobEvent, INFINITE);
    critical_section_enter(pool->m_synJobList);
    if (pool->jobnum > 0) {
      pool->jobnum--;
      job = pool->joblist[pool->jobnum];
      //memcpy(pool->joblist, pool->joblist+1, pool->jobnum*sizeof(void*));
    }
    critical_section_leave(pool->m_synJobList);
    return job;
  case 3:
    while(pool->jobnum>0) {
      cond_signal(pool->m_hJobEvent);
    }
    critical_section_enter(pool->m_synJobList);
    critical_section_leave(pool->m_synJobList);
    pool->shutdown = 1;
    for (i = 0; i < pool->thnum; ++i) {
      thread_join(pool->th+i);
    }
    thread_joins(pool->thnum, pool->th);
    critical_section_destroy(pool->m_synJobList);
    cond_destroy(pool->m_hJobEvent);
    free(pool->joblist);
    return 0;
  case 4:
    sleep(1000);
    critical_section_enter(pool->m_synJobList);
    pool->shutdown = 1;
    cond_signal(pool->m_hJobEvent);
    for (i = 0; i < pool->thnum; ++i) {
      //thread_join(pool->th+i);
    }
    critical_section_leave(pool->m_synJobList);
    thread_joins(pool->thnum, pool->th);
    critical_section_destroy(pool->m_synJobList);
    cond_destroy(pool->m_hJobEvent);
    free(pool->joblist);
    return 0;
  }
  return 0;
}

#endif // _THREAD_INL_

