#ifndef _THREAD_H_
#define _THREAD_H_

#include "sys_c.h"
#include "stddef_c.h"

typedef struct { void* x; } handel_t;
typedef handel_t thread_t;
typedef handel_t job_t;
typedef handel_t event_t;
typedef handel_t lock_t;
typedef handel_t mutex_t;
typedef handel_t thread_attr_t;

typedef void* (*thread_callback)(void* arg);
typedef void* (*parallel_callback)(void* arg, int beg, int end);
#define CC_INFINITE 0xffffffff
#define PUSHARG(args, j, T, NAME)     args[j++] = &NAME;
#define POPARG(args, j, T, NAME)      T& NAME = *(T*)args[j++];
static int parallel_for(void* arg, int beg, int end, parallel_callback fun);


#define sys_thread_cancel(x)
//////////////////////////////////////////////////////////////////////////
//

struct _thread_t {
  thread_callback run;
  void* handle;
  void* arg;
  void* security;
  unsigned stack_size;
  unsigned initflag;
  unsigned threadID;
  void* x;
  void* data[4];
  struct _thread_t* prev;
  struct _thread_t* next;
};

#define MB
#define WMB
#define YIELDING

#ifdef _WIN32
#else
#endif

#ifdef _WIN32
#else
#include <pthread.h>
typedef struct critical_section_t {
  pthread_mutex_t   m_sect;
} critical_section_t;
#endif


/////////////////////////////////////////////////////////////////////////////////////////
// win32
int sys_sleep(int ms)
{
#ifdef _WIN32
  Sleep(ms);
#else
#endif
  return 0;
}

int sys_spin_lock(volatile long* address)
{
#ifdef _WIN32
  int ret;
  do {
    while (*address) {YIELDING;};
#if defined(_MSC_VER) && !defined(__clang__)
    // use intrinsic instead of inline assembly
    ret = InterlockedExchange((volatile LONG*)address, 1);
    // inline assembly
    /*__asm {
    mov eax, address
    mov ebx, 1
    xchg [eax], ebx
    mov ret, ebx
    }*/
#else
#endif
  } while (ret);
  return ret;
#else
#endif
}

int sys_spin_unlock(volatile long* address)
{
#ifdef _WIN32
  MB;
  *address = 0;
#else
#endif
  return 0;
}
int sys_lock_init(lock_t* c)
{
#ifdef _WIN32
  CRITICAL_SECTION* cs = (CRITICAL_SECTION*)malloc(sizeof(CRITICAL_SECTION));
  c->x = cs;
  InitializeCriticalSection(cs);
  return 0;
#else
  critical_section_t* cs = (critical_section_t*)malloc(sizeof(critical_section_t));
  pthread_mutexattr_t m_attr;
  c->x = cs;
  pthread_mutexattr_init(&m_attr);
  pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE_NP);
  pthread_mutex_init(&cs->m_sect, &m_attr);
  pthread_mutexattr_destroy(&m_attr);
  return 0;
#endif
}
int sys_lock_destroy(lock_t* c)
{
#ifdef _WIN32
  CRITICAL_SECTION* cs = (CRITICAL_SECTION*)c->x;
  DeleteCriticalSection(cs);
  free(cs);
  c->x = NULL;
  return 0;
#else
  critical_section_t* cs = (critical_section_t*)c->x;
  pthread_mutex_destroy(&cs->m_sect);
  free(cs);
  c->x = NULL;
  return 0;
#endif
}

int sys_lock_lock(lock_t* c)
{
#ifdef _WIN32
  CRITICAL_SECTION* cs = (CRITICAL_SECTION*)c->x;
  EnterCriticalSection(cs);
  return 0;
#else
  critical_section_t* cs = (critical_section_t*)c->x;
  return pthread_mutex_lock(&cs->m_sect);
#endif
}
int sys_lock_unlock(lock_t* c)
{
#ifdef _WIN32
  CRITICAL_SECTION* cs = (CRITICAL_SECTION*)c->x;
  LeaveCriticalSection(cs);
  return 0;
#else
  critical_section_t* cs = (critical_section_t*)c->x;
  return pthread_mutex_unlock(&cs->m_sect);
#endif
}

#ifdef _WIN32
int sys_event_init(event_t* c, bool manual_reset, bool init_state)
{
  c->x = CreateEventA(NULL, manual_reset, init_state, NULL);
  return 0;
}
int sys_event_destroy(event_t* c)
{
  CloseHandle((HANDLE)c->x);
  c->x = NULL;
  return 0;
}
int sys_event_wait(event_t* c, int dwMillises)
{
  DWORD ret = WaitForSingleObject((HANDLE)c->x, dwMillises);
  if (ret == WAIT_OBJECT_0) {
    return 0;
  }
  if (ret == WAIT_TIMEOUT) {
    return 1;
  }
  return -1;
}
int sys_event_signal(event_t* c)
{
  return !SetEvent((HANDLE)c->x);
}
int sys_event_reset(event_t* c)
{
  int ret = ResetEvent((HANDLE)c->x);
  //ResetEvent 返回非零表示成功
  if (ret) { return 0; }
  return -1;
}
//typedef WINAPI void* (*thread_cb)(void* pParam);
//创建一个匿名互斥对�?
int sys_mutex_init(mutex_t* c, const char* name)
{
  c->x = CreateMutex(NULL, FALSE, NULL);
  return 0;
}
//销毁互斥对象，释放资源
int sys_mutex_destroy(mutex_t* c)
{
  CloseHandle((HANDLE)c->x);
  c->x = 0;
  return 0;
}
//确保拥有互斥对象的线程对被保护资源的独自访问
int sys_mutex_lock(mutex_t* c)
{
  DWORD d = WaitForSingleObject((HANDLE)c->x, INFINITE);
  return 0;
}
//释放当前线程拥有的互斥对象，以使其它线程可以拥有互斥对象，对被保护资源进行访�?
int sys_mutex_unlock(mutex_t* c)
{
  ReleaseMutex((HANDLE)c->x);
  return 0;
}
static DWORD WINAPI ThreadProxy(LPVOID args)
{
  _thread_t* _th = (_thread_t*)(args);
  void* ret = 0;
  if (_th && _th->run) {
    ret = _th->run(_th->arg);
  }
  free(_th);
  return (DWORD)(intptr_t)ret;
}

//typedef void* uintptr_t;
int sys_thread_create(thread_t* th, const thread_attr_t* attr, thread_callback start_rtn, void* arg)
{
  _thread_t* _th = (_thread_t*)malloc(sizeof(_thread_t));
  memset(_th, 0, sizeof(*_th));
  th->x = _th;
  _th->arg = arg;
  _th->run = start_rtn;
  HANDLE handle = CreateThread((LPSECURITY_ATTRIBUTES)(0), 0, ThreadProxy, _th, _th->initflag, (unsigned long*)&_th->threadID);
  _th->handle = handle;
  return handle != INVALID_HANDLE_VALUE;
}
int sys_thread_join(thread_t* th)
{
  int i = 0, ret = 1;
  if (WaitForSingleObject((HANDLE)th->x, INFINITE) != WAIT_OBJECT_0) {
    TerminateThread((HANDLE)th->x, 0);
    memset(th, 0, sizeof(*th));
    ret = 0;
  }
  return ret;
}
static int sys_thread_wait(int count, thread_t* th)
{
  int waitingThreadsCount = count;
  int index = 0, i;
  DWORD res = 0;
  HANDLE handles[MAXIMUM_WAIT_OBJECTS];
  while (waitingThreadsCount > 0) {
    int len = MIN(waitingThreadsCount, MAXIMUM_WAIT_OBJECTS);
    for (i = 0; i < len; ++i) {
      handles[i] = (HANDLE)th[index].x;
    }
    res = WaitForMultipleObjects(len, handles, TRUE, INFINITE);
    if (res == WAIT_TIMEOUT || res == WAIT_FAILED) {
      printf("1. Wait Failed.");
      return res;
    }
    waitingThreadsCount -= MAXIMUM_WAIT_OBJECTS;
    index += MAXIMUM_WAIT_OBJECTS;
  }
  return res;
}
static int sys_thread_joins(int n, thread_t* th)
{
  int i = 0, ret = 1;
  if (1 == n) {
    if (WaitForSingleObject((HANDLE)th->x, INFINITE) != WAIT_OBJECT_0) {
      TerminateThread((HANDLE)th->x, 0);
      memset(th, 0, sizeof(*th));
      ret = 0;
    }
  } else {
    if (sys_thread_wait(n, th) != WAIT_OBJECT_0) {
      for (i = 0; i < n; ++i) {
        TerminateThread((HANDLE)th[i].x, 0);
      }
      memset(th, 0, n * sizeof(*th));
      ret = 0;
    }
  }
  return ret;
}
static int sys_thread_detach(int n, thread_t* th)
{
  int i, ret = 1;
  for (i = 0; i < n; ++i) {
    int ret1 = (TRUE == CloseHandle((HANDLE)th[i].x));
    ret = ret && ret1;
  }
  memset(th, 0, n * sizeof(thread_t));
  return ret;
}
static int sys_thread_self(thread_t* th)
{
  // GetCurrentThread();
  return 0;
}
int sys_thread_id()
{
  DWORD tid = GetCurrentThreadId();
  return tid;
}
int sys_getpid()
{
  DWORD pid = GetCurrentProcessId();
  return pid;
}
#endif
////////////////////////////////////////////////////////////////////
#ifdef __linux__

#if 0
typedef struct cond_t {
  pthread_cond_t    m_hEvent;
  pthread_mutex_t   m_sect;
  pthread_mutexattr_t m_attr;
} cond_t;
int sys_event_reset(event_t* c)
{
  cond_t* cs = (cond_t*)c->x;
  pthread_cond_init(&cs->m_hEvent, 0);
  pthread_mutexattr_init(&cs->m_attr);
#ifdef PTHREAD_MUTEX_TIMED_NP
  pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_TIMED_NP);
#endif // PTHREAD_MUTEX_TIMED_NP
  pthread_mutex_init(&cs->m_sect, &cs->m_attr);
  pthread_mutexattr_destroy(&cs->m_attr);
  return 0;
}
int sys_event_init(event_t* c, const char* name)
{
  cond_t* cs = (cond_t*)malloc(sizeof(cond_t));
  memset(cs, 0, sizeof(*cs));
  c->x = cs;
  sys_event_reset(c);
  return 0;
}
int sys_event_destroy(event_t* c)
{
  cond_t* cs = (cond_t*)c->x;
  pthread_cond_destroy(&cs->m_hEvent);
  pthread_mutexattr_destroy(&cs->m_attr);
  pthread_mutex_destroy(&cs->m_sect);
  free(cs);
  c->x = NULL;
  return 0;
}
int sys_event_wait(event_t* c, int dwMillises)
{
  cond_t* cs = (cond_t*)c->x;
  pthread_mutex_lock(&cs->m_sect);
  if (dwMillises < 0) {
    pthread_cond_wait(&cs->m_hEvent, &cs->m_sect);
  } else {
    struct timespec timeout;
    timeout.tv_sec = 0;
    timeout. tv_nsec = dwMillises * 10000000;
    pthread_cond_timedwait(&cs->m_hEvent, &cs->m_sect, &timeout);
  }
  pthread_mutex_unlock(&cs->m_sect);
  return 0;
}
int sys_event_signal(event_t* c)
{
  cond_t* cs = (cond_t*)c->x;
  pthread_mutex_lock(&cs->m_sect);
  pthread_cond_signal(&cs->m_hEvent);
  pthread_mutex_unlock(&cs->m_sect);
  return 0;
}
#else
typedef struct {
  bool state;
  bool manual_reset;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} cond_t;

int sys_event_reset(event_t* c)
{
  cond_t* hevent = (cond_t*)c->x;
  if (pthread_mutex_lock(&hevent->mutex) != 0) {
    return -1;
  }
  hevent->state = false;
  if (pthread_mutex_unlock(&hevent->mutex) != 0) {
    return -1;
  }
  return 0;
}
//
int sys_event_init(event_t* c, bool manual_reset, bool init_state)
{
  cond_t* hevent = (cond_t*)malloc(sizeof(cond_t));
  hevent->state = init_state;
  hevent->manual_reset = manual_reset;
  if (pthread_mutex_init(&hevent->mutex, NULL)) {
    free(hevent);
    return 0;
  }
  if (pthread_cond_init(&hevent->cond, NULL)) {
    pthread_mutex_destroy(&hevent->mutex);
    free(hevent);
    return 0;
  }
  c->x = hevent;
  return 1;
}
int sys_event_destroy(event_t* c)
{
  cond_t* hevent = (cond_t*)c->x;
  pthread_cond_destroy(&hevent->cond);
  pthread_mutex_destroy(&hevent->mutex);
  free(hevent);
  c->x = NULL;
  return 0;
}
int sys_event_wait(event_t* c, int milliseconds)
{
  cond_t* hevent = (cond_t*)c->x;
  if (milliseconds < 0) {
    if (pthread_mutex_lock(&hevent->mutex)) {
      return -1;
    }
    while (!hevent->state) {
      if (pthread_cond_wait(&hevent->cond, &hevent->mutex)) {
        pthread_mutex_unlock(&hevent->mutex);
        return -1;
      }
    }
    if (!hevent->manual_reset) {
      hevent->state = false;
    }
    if (pthread_mutex_unlock(&hevent->mutex)) {
      return -1;
    }
    return 0;
  } else {
    int rc = 0;
    struct timespec abstime;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    abstime.tv_sec = tv.tv_sec + milliseconds / 1000;
    abstime.tv_nsec = tv.tv_usec * 1000 + (milliseconds % 1000) * 1000000;
    if (abstime.tv_nsec >= 1000000000) {
      abstime.tv_nsec -= 1000000000;
      abstime.tv_sec++;
    }
    if (pthread_mutex_lock(&hevent->mutex) != 0) {
      return -1;
    }
    while (!hevent->state) {
      if (rc = pthread_cond_timedwait(&hevent->cond, &hevent->mutex, &abstime)) {
        if (rc == ETIMEDOUT) { break; }
        pthread_mutex_unlock(&hevent->mutex);
        return -1;
      }
    }
    if (rc == 0 && !hevent->manual_reset) {
      hevent->state = false;
    }
    if (pthread_mutex_unlock(&hevent->mutex) != 0) {
      return -1;
    }
    if (rc == ETIMEDOUT) {
      //timeout return 1
      return 1;
    }
  }
  //wait event success return 0
  return 0;
}
int sys_event_signal(event_t* c)
{
  cond_t* hevent = (cond_t*)c->x;
  if (pthread_mutex_lock(&hevent->mutex) != 0) {
    return -1;
  }
  hevent->state = true;
  if (hevent->manual_reset) {
    if (pthread_cond_broadcast(&hevent->cond)) {
      return -1;
    }
  } else {
    if (pthread_cond_signal(&hevent->cond)) {
      return -1;
    }
  }
  if (pthread_mutex_unlock(&hevent->mutex) != 0) {
    return -1;
  }
  return 0;
}
#endif
int sys_mutex_init(mutex_t* mut, const char* name)
{
  //pthread_mutex_init(&mutex,NULL);
  return 0;
}
int sys_mutex_destroy(mutex_t* mut)
{
  return 0;
}
int sys_mutex_lock(mutex_t* mut)
{
  return 0;
}
int sys_mutex_unlock(mutex_t* mut)
{
  return 0;
}
#if 0
static void* ThreadProxy(void* args)
{
  pthread_t* th = (thread_t*)(args);
  int ret = 0;
  UNUSED(ret);
  if (th && th->run) {
    ret = th->run(th->arg);
  }
  return NULL;
}
#endif

int sys_thread_create(thread_t* th, const thread_attr_t* attr, thread_callback start_rtn, void* arg)
{
  pthread_t pth = 0;
  typedef void *(*pthread_start_routine) (void *);
  int ret = pthread_create(&pth, (pthread_attr_t*)attr, (pthread_start_routine)start_rtn, arg);
  th->x = (void*)pth;
  return ret;
}
int sys_thread_join(thread_t* th)
{
  unsigned long m_nRet = 0;
  int ret = 0;
  ret = (0 == pthread_join((pthread_t)(th->x), (void**)&m_nRet));
  return ret;
}
int sys_thread_id()
{
  return pthread_self();
}
int sys_getpid()
{
  int pid = getpid();
  return pid;
}

#endif

#endif // _THREAD_H_
