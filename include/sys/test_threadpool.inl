
#include "threadpool.inl"

struct test_threadpool_work_t {
  job_t job[1];
  char buf[32];
};
int test_threadpool_work(void* arg)
{
  test_threadpool_work_t *p = (test_threadpool_work_t*) arg;
  int id = sys_thread_id();
  printf("thread id=%d str = %s.\n", id, p->buf);
  sys_sleep(100);
  return 0;
}

int test_threadpool() {
  threadpool_t pool[1] = {0};
  jobgroup_t jobgroup[1] = {0};
  int i;
  test_threadpool_work_t info[100] = {0};
  for (i=0; i<countof(info); ++i) {
    _snprintf(info[i].buf, 32, "%d", i);
    info[i].job->run = test_threadpool_work;
    info[i].job->arg = info+i;
    jobgroup_add_job(jobgroup, info[i].job);
  }
  jobgroup_join(jobgroup, pool);
  jobgroup_free(jobgroup);
  threadpool_free(pool);
  return 0;
}

#if 0
#include "E:\code\c\base\callstack\CallStack.cpp"

int test_callstack()
{
    if(!CallStack::LoadDbgHelpLib())
    {
         printf("Load dbghelp.dll failed!\n");
         return -1;
    }
    CallStack *pStack = new CallStack();
    pStack->GetCallStack();
    pStack->GenCallStackDetails();
    delete pStack;
    return 0;
}
#endif
