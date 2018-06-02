// Thread.cpp: implementation of the CThread class.
//
//////////////////////////////////////////////////////////////////////

//#include "StdAfx.h"
#include "ThreadPool.h"



#if 0
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CThread::CThread() :
  m_hTread(0),
  m_bRunning(false)
{
}

CThread::~CThread()
{
  StopThread();
}

void CThread::StartThread()
{
  if (!m_bRunning) {
    m_bRunning = true;
#ifdef WIN32
    m_hTread = (HANDLE)_beginthread(CThread::Run, 0, this);
#else
    pthread_create(&m_hTread, NULL, CThread::Run, this) ;
#endif
  }
}

void CThread::StopThread()
{
  if (m_bRunning) {
    m_bRunning = false;
#ifdef WIN32

    if (WaitForSingleObject(m_hTread, 3000) == WAIT_TIMEOUT) {
      TerminateThread(m_hTread, 100);
    }

    m_hTread = NULL;
#else
    pthread_join(m_hTread, NULL);
    m_hTread = 0;
#endif
  }
}

T_VOID CThread::Run(void* param)
{
  CThread* pthread = (CThread*)param;
  assert(pthread != NULL);
  pthread->Run();
#ifndef WIN32
  return pthread;
#endif
}
#endif

CWorkerThread::CWorkerThread(CThreadPool* pThreadPool) :
  m_pThreadPool(pThreadPool)
{
}

CWorkerThread::~CWorkerThread()
{
  StopThread();
}


void CWorkerThread::Start()
{
  StartThread();
}

void CWorkerThread::process()
{
  while (m_bRunning) {
    CJob* pJob = m_pThreadPool->GetJob();

    if (pJob) {
      pJob->Run();
      CPMDelete(pJob, CMemPool::GetInstance());
    }
  }
}

CThreadPool::CThreadPool(int nIniNum) :
  m_bInit(1)
{
  for (int i = 0; i < nIniNum; ++i) {
    CWorkerThread* pNew = new CWorkerThread(this);
    m_WorkThred.push_back(pNew);
    pNew->Start();
  }
}

CThreadPool::~CThreadPool()
{
  m_bInit = 0;
  THREAD_LIST::iterator it = m_WorkThred.begin();

  while (it != m_WorkThred.end()) {
    m_hJobEvent.Signal();
    ++it;
  }

  it = m_WorkThred.begin();

  while (it != m_WorkThred.end()) {
    delete *it;
    ++it;
  }

  m_WorkThred.clear();

  AoutLock lock(&m_synJobList);
  JOB_LIST::iterator jonit = m_JobList.begin();

  while (jonit != m_JobList.end()) {
    CPMDelete((*jonit), CMemPool::GetInstance());
    ++jonit;
  }

  m_JobList.clear();
}

void CThreadPool::SetJob(CJob* pJob)
{
  if (!m_bInit) {
    return;
  }

  AoutLock lock(&m_synJobList);
  m_JobList.push_back(pJob);
  m_hJobEvent.Signal();
}

CJob* CThreadPool::GetJob()
{
  if (!m_bInit) {
    return NULL;
  }

  CJob* pJob = NULL;
  {
    AoutLock lock(&m_synJobList);

    if (!m_JobList.empty()) {
      pJob = m_JobList.front();
      m_JobList.pop_front();
    }
  }

  if (pJob == NULL && m_bInit) {
    m_hJobEvent.Wait(INFINITE);
  }

  return pJob;
}













