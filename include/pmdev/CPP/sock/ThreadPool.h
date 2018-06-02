// Thread.h: interface for the CThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_H__F81B0E38_DD05_42D7_9151_5B10A0E18631__INCLUDED_)
#define AFX_THREAD_H__F81B0E38_DD05_42D7_9151_5B10A0E18631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "MemPool.h"

inline void* operator new(size_t size_in, CMemPool& pool)
{
  return pool.GetMem(size_in);
}

inline void  operator delete(void* lpMem, CMemPool& pool)
{
  pool.FreeMem((int8*)lpMem);
}

#define CPMNew(a) new( (a) )

template < class T >
void CPMDelete(T* pObj, CMemPool& pool)
{
  pObj->~T();
  pool.FreeMem((int8*)pObj);
}

class CJob
{
public:
  CJob() {};
  virtual ~CJob() {};
public:
  virtual void Run() = 0;
};


class CEvent
{
public:
  CEvent() {
#ifdef WIN32
    m_hEvent = CreateEvent(NULL, 0, 0, NULL);
#else
    pthread_cond_init(&m_hEvent, 0);
    pthread_mutexattr_init(&m_attr);
    pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_TIMED_NP);
    pthread_mutex_init(&m_sect, &m_attr);
#endif
  }
  ~CEvent() {
#ifdef WIN32
    CloseHandle(m_hEvent);
#else
    pthread_cond_destroy(&m_hEvent);
    pthread_mutexattr_destroy(&m_attr);
    pthread_mutex_destroy(&m_sect);
#endif
  }
public:
  void Wait(DWORD dwMilliseconds) {
#ifdef WIN32
    WaitForSingleObject(m_hEvent, dwMilliseconds);
#else
    pthread_mutex_lock(&m_sect);

    if (dwMilliseconds == INFINITE) {
      pthread_cond_wait(&m_hEvent, &m_sect);
    }
    else {
      timespec timeout;
      timeout.tv_sec = 0;
      timeout. tv_nsec = dwMilliseconds * 10000000;
      pthread_cond_timedwait(&m_hEvent, &m_sect, &timeout);
    }

    pthread_mutex_unlock(&m_sect);
#endif
  }

  void Signal() {
#ifdef WIN32
    SetEvent(m_hEvent);
#else
    pthread_mutex_lock(&m_sect);
    pthread_cond_signal(&m_hEvent);
    pthread_mutex_unlock(&m_sect);
#endif
  }
private:

#ifdef WIN32
  HANDLE m_hEvent;
#else
  pthread_cond_t    m_hEvent;
  pthread_mutex_t   m_sect;
  pthread_mutexattr_t m_attr;
#endif

};

#if 0
class CThread1
{
public:
  CThread1();
  virtual ~CThread1();
public:
  void StartThread();
  void StopThread();
protected:
  static T_VOID Run(void* param);
  virtual void Run() = 0 ;
protected:
  BOOL      m_bRunning;
  T_HANDLE  m_hTread;
};
#endif

class CThreadPool;
class CWorkerThread : public CThread
{
public:
  CWorkerThread(CThreadPool* pThreadPool);
  ~CWorkerThread();
public:
  void Start();
protected:
  virtual void process();
private:
  CThreadPool* m_pThreadPool;
};

class CThreadPool
{
  friend class CWorkerThread;
public:
  CThreadPool(int nIniNum);
  ~CThreadPool();
public:
  void SetJob(CJob* pJob);
private:
  CJob* GetJob();

private:
  typedef std::list<CWorkerThread*> THREAD_LIST;
  typedef std::deque<CJob*>         JOB_LIST;

  CriticalSection m_synJobList;
  CEvent          m_hJobEvent;
  JOB_LIST        m_JobList;
  THREAD_LIST     m_WorkThred;
  int             m_bInit;
};

#endif // !defined(AFX_THREAD_H__F81B0E38_DD05_42D7_9151_5B10A0E18631__INCLUDED_)
