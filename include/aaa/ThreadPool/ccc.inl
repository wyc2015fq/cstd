typedef struct threadpool_job_t {
  thread_cb fun;
  void* param;
  HANDLE hEvent;
  HANDLE hEventEnd;
  int id;
} threadpool_job_t;
typedef struct threadpool_t {
  critical_section_t cs[1];
  thread_t* th;
  int nth;
  threadpool_job_t* job;
} threadpool_t;
int WINAPI threadpool_fun(void* x)
{
  threadpool_job_t* job = (threadpool_job_t*)x;
  while (TRUE) {
    WaitForSingleObject(job->hEvent, INFINITE);
    if (job->fun) {
      job->fun(job->param);
    }
    //printf("threadpool_fun %x\n", job->id);
    ResetEvent(job->hEvent);
    SetEvent(job->hEventEnd);
  }
  return 0;
}
static int threadpool_create(int n, thread_t* th, void* fun, void* pParam, int paramsize, threadpool_t* p)
{
  int i;
  char* pcParam = (char*)pParam;
  if (p) {
    if (p->job == NULL) {
      p->job = (threadpool_job_t*)realloc(p->job, n * sizeof(threadpool_job_t));
    }
    for (i = 0; i < n; ++i) {
      p->job[i].param = (void*)(pcParam + i * paramsize);
      p->job[i].fun = (thread_cb)fun;
      p->job[i].id = i;
    }
    if (p->th) {
      ASSERT(p->nth == n);
      ASSERT(p->th == th);
      for (i = 0; i < n; ++i) {
        SetEvent(p->job[i].hEvent);
      }
    }
    else {
      for (i = 0; i < n; ++i) {
        p->job[i].hEvent = CreateEvent(NULL, TRUE, TRUE, NULL); //没有名字
        p->job[i].hEventEnd = CreateEvent(NULL, TRUE, FALSE, NULL); //没有名字
      }
      for (i = 0; i < n; ++i) {
        th[i].h = CreateThread(NULL, 0, (thread_cb)threadpool_fun, p->job + i, 0, NULL);
      }
      p->nth = n;
      p->th = th;
    }
  }
  else {
    for (i = 0; i < n; ++i) {
      th[i].h = CreateThread(NULL, 0, (thread_cb)fun, pcParam + i * paramsize, 0, NULL);
    }
  }
  return 0;
}
static int threadpool_join(int n, thread_t* th, int dwMilliseconds, threadpool_t* p)
{
  int i;
  if (p) {
    if (1 == n) {
      if (WaitForSingleObject(p->job[0].hEventEnd, dwMilliseconds) != WAIT_OBJECT_0) {
        return 0;
      }
      ResetEvent(p->job[0].hEventEnd);
    }
    else {
      HANDLE hd[100];
      ASSERT(n < 100);
      for (i = 0; i < n; ++i) {
        hd[i] = p->job[i].hEventEnd;
      }
      if (WaitForMultipleObjects(n, hd, TRUE, dwMilliseconds) != WAIT_OBJECT_0) {
        return 0;
      }
    }
    for (i = 0; i < n; ++i) {
      ResetEvent(p->job[i].hEventEnd);
    }
    return -1;
  }
  else {
    return thread_joins(n, th, dwMilliseconds);
  }
  return -1;
}

