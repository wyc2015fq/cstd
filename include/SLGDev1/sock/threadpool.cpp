// Thread.cpp: implementation of the CThread class.
//
#include "../slgdev_private.h"
#include "threadpool.h"
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
    if (!m_bRunning)
    {
        m_bRunning = true;
#ifdef WIN32
        m_hTread = (HANDLE)CreateThread(0, 0, (thread_cb)CThread::Run1, this, 0, 0);
#else
        pthread_create(&m_hTread, NULL, (thread_cb)CThread::Run1, this) ;
#endif
    }

}

void CThread::StopThread()
{
    if (m_bRunning)
    {
        m_bRunning = false;
#ifdef WIN32

        if (WaitForSingleObject(m_hTread, 3000) == WAIT_TIMEOUT)
        {
            TerminateThread(m_hTread, 100);
        }

        m_hTread = NULL;
#else
        pthread_join(m_hTread, NULL);
        m_hTread = 0;
#endif
    }

}

unsigned long CThread::Run1(void* param)
{
    CThread* pthread = (CThread*)param;
    assert(pthread != NULL);
    pthread->Run();
    return 0;
}

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

void CWorkerThread::Run()
{
    while (m_bRunning)
    {
        CJob* pJob = m_pThreadPool->GetJob();

        if (pJob)
        {
            pJob->Run();
            SLGDelete(pJob, CMemPool::GetInstance());
        }

    }

}

CThreadPool::CThreadPool(int nIniNum) :
    m_bInit(1)
{
    for (int i = 0; i < nIniNum; ++i)
    {
        CWorkerThread* pNew = new CWorkerThread(this);
        m_WorkThred.push_back(pNew);
        pNew->Start();
    }

}

CThreadPool::~CThreadPool()
{
    m_bInit = 0;
    THREAD_LIST::iterator it = m_WorkThred.begin();

    while (it != m_WorkThred.end())
    {
        m_hJobEvent.Signal();
        ++it;
    }

    it = m_WorkThred.begin();

    while (it != m_WorkThred.end())
    {
        delete *it;
        ++it;
    }

    m_WorkThred.clear();
    AoutLock lock(&m_synJobList);
    JOB_LIST::iterator jonit = m_JobList.begin();

    while (jonit != m_JobList.end())
    {
        SLGDelete((*jonit), CMemPool::GetInstance());
        ++jonit;
    }

    m_JobList.clear();
}

void CThreadPool::SetJob(CJob* pJob)
{
    if (!m_bInit)
    {
        return;
    }

    AoutLock lock(&m_synJobList);
    m_JobList.push_back(pJob);
    m_hJobEvent.Signal();
}

CJob* CThreadPool::GetJob()
{
    if (!m_bInit)
    {
        return NULL;
    }

    CJob* pJob = NULL;
    {
        AoutLock lock(&m_synJobList);

        if (!m_JobList.empty())
        {
            pJob = m_JobList.front();
            m_JobList.pop_front();
        }

    }

    if (pJob == NULL && m_bInit)
    {
        m_hJobEvent.Wait(INFINITE);
    }

    return pJob;
}

