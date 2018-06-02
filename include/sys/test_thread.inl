
//调用实例
#include <stdio.h>
static int test_thread_func1(void* x)
{
  int i;
  for (i = 0; i < 5; ++i) {
    printf("%d..\n", i);
    sys_sleep(10);
  }
  return 0;
}
static int test_thread_func2(void* arg)
{
  thread_t th[1] = {0};
  int id = sys_thread_id();
  //  printf("I am new thread!\n");
  printf("New process: PID: %d,TID: %u.\n", sys_getpid(), id); //why pthread_self
  return 0;
}
#if 0
int test_threadpool() {
  int i;
  if (0) {
    for (i=0; i<20; ++i) {
      //tpool_create(test_thread_func1, NULL);
    }
    //tpool_joinall();
  }
  
  if (0) {
    cond_t ccc[1] = {0};
    thread_t th[1] = {0};
    cond_init(ccc);
    for (i=0; i<10; ++i) {
      cond_signal(ccc);
    }
    thread_create(th, test_thread_func1, 0);
    thread_join(th);
    sleep(20000);
  }
  if (1) {
    for (i = 0; i < 100; ++i) {
      tpool_create(test_thread_func1, 0);
    }
    tpool_joinall();
    sleep(2000);
  }
#if 0
  if (1) {
    CThreadPool po;
    for (i = 0; i < 10; ++i) {
      po.Run((LPTHREAD_START_ROUTINE)test_thread_func1, 0);
    }
    sleep(2000);
    sleep(2000);
  }
#endif
  return 0;
}
#endif
static int test_thread()
{
  if (0) {
    thread_t th[1] = {0};
    thread_t th2[1] = {0};
    int id, id2;
    id = sys_thread_id(); // 返回调用线程的线程ID
    id2 = sys_thread_id();
    printf("Thread ID: %lu.\n", id2);
    if (id2 == id) {
      printf("Thread ID: %lu.\n", id);
      printf("Equal!\n");
    }
    else {
      printf("Not equal!\n");
    }
  }
  if (1) {
    thread_t tid[1] = {0};
    thread_t th[1] = {0};
    int id;
    // 创建线程tid，且线程函数由test_thread_func2指向，是test_thread_func2的入口点，即马上执行此线程函数
    tid->run = test_thread_func2;
    sys_thread_create(tid);
    printf("New process: PID: %d,TID: %u.\n", (int)sys_getpid(), (unsigned int)tid); //why pthread_self
    if (tid == 0) {
      printf("Create thread error!\n");
    }
    printf("TID in pthread_create function: %u.\n", (unsigned int)tid);
    id = sys_thread_id();
    printf("Main process: PID: %d,TID: %u.\n", sys_getpid(), (unsigned int)id);
    if (0 == sys_thread_join(tid)) {
      printf("join failed!\n");
    }
    //sys_thread_detach(tid); //分离线程
  }
  if (0) {
    thread_t th[1] = {0};
    th->run = test_thread_func1;
    sys_thread_create(th);
    if (0 == sys_thread_join(th)) {
      printf("join failed!\n");
    }
    //sys_thread_detach(th); //分离线程
  }
  return 0;
}