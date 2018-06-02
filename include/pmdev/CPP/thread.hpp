
#ifndef _THREAD_HPP_
#define _THREAD_HPP_

class CThread
{
public:
  CThread() :
  m_hTread(0),
    m_bRunning(false)
  {
  }
  
  virtual ~CThread()
  {
    StopThread();
  }

protected:
  void StartThread()
  {
    if ( !m_bRunning )
    {
      m_bRunning = true;
#ifdef WIN32	
      m_hTread = (HANDLE)_beginthread(CThread::process_callback, 0, this);
#else
typedef void* (*thread_cb)(void* pParam);
      pthread_create(&m_hTread, NULL, (thread_cb)((void*)process_callback), this) ;
#endif
    }
  }
  
  void StopThread()
  {
    if ( m_bRunning )
    {
      m_bRunning = false;
#ifdef WIN32
      if ( WaitForSingleObject(m_hTread, 3000) != WAIT_OBJECT_0 )
      {
        TerminateThread(m_hTread, 0);
      }
      m_hTread = NULL;
#else
      pthread_join(m_hTread, NULL);
#endif
    }
}

protected:
  static void process_callback(void* param)
  {
    CThread* pthread = (CThread*)param;
    if ( pthread ) 
    {
      pthread->process();
    }
  }

  virtual void process() = 0 ;
protected:
  bool      m_bRunning;
  T_HANDLE    m_hTread;
};

#endif // _THREAD_HPP_
