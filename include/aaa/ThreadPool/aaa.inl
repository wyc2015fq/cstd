#include <deque>
#include <list>
class CThreadPool
{
  friend class CWorkerThread;
public:
  static void* tpool_thread_run1(void* arg) {
    CThreadPool* m_pThreadPool = (CThreadPool*)arg;
    while (1) {
      tpool_job_t* pJob = m_pThreadPool->GetJob();
      if (pJob) {
        pJob->fun(pJob->arg);
        free(pJob);
      }
    }
    return 0;
  }
  CThreadPool::CThreadPool(int nIniNum) : m_bInit(1) {
    critical_section_init(this->m_synJobList);
    cond_init(this->m_hJobEvent);
    for (int i = 0; i < nIniNum; ++i) {
      HANDLE pNew = CreateThread(NULL, 0, (thread_cb)tpool_thread_run1, this, 0, NULL);
      m_WorkThred.push_back(pNew);
    }
  }
  CThreadPool::~CThreadPool() {
    m_bInit = 0;
    THREAD_LIST::iterator it = m_WorkThred.begin();
    while (it != m_WorkThred.end()) {
      cond_signal(m_hJobEvent);
      ++it;
    }
    it = m_WorkThred.begin();
    m_WorkThred.clear();
    critical_section_enter(m_synJobList);
    JOB_LIST::iterator jonit = m_JobList.begin();
    while (jonit != m_JobList.end()) {
      free(*jonit);
      ++jonit;
    }
    m_JobList.clear();
    critical_section_leave(m_synJobList);
  }
public:
  void CThreadPool::create(void* fun, void* arg) {
    if (!m_bInit) {
      return;
    }
    tpool_job_t* job = (tpool_job_t*)malloc(sizeof(tpool_job_t));
    memset(job, 0, sizeof(tpool_job_t));
    job->fun = (thread_cb)fun;
    job->arg = arg;
    critical_section_enter(m_synJobList);
    m_JobList.push_back(job);
    cond_signal(m_hJobEvent);
    critical_section_leave(m_synJobList);
  }
private:
  tpool_job_t* CThreadPool::GetJob() {
    if (!m_bInit) {
      return NULL;
    }
    tpool_job_t* pJob = NULL;
    critical_section_enter(m_synJobList);
    if (!m_JobList.empty()) {
      pJob = m_JobList.front();
      m_JobList.pop_front();
    }
    critical_section_leave(m_synJobList);
    if (pJob == NULL && m_bInit) {
      cond_wait(m_hJobEvent, INFINITE);
    }
    return pJob;
  }
private:
  typedef std::list<HANDLE> THREAD_LIST;
  typedef std::deque<tpool_job_t*> JOB_LIST;
  critical_section_t m_synJobList[1];
  cond_t m_hJobEvent[1];
  JOB_LIST m_JobList;
  THREAD_LIST m_WorkThred;
  int m_bInit;
};
int test_threadpool()
{
  int i;
  CThreadPool po(5);
  for (i = 0; i < 20; ++i) {
    po.create(test_thread_func1, NULL);
  }
  sleep(1000);
  return 0;
}

