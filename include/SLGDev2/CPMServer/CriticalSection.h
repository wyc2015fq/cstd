

#ifndef _CRITICAL_SECTION_
#define _CRITICAL_SECTION_

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class CriticalSection
{
#ifdef WIN32
  CRITICAL_SECTION m_sect;
#else
  pthread_mutex_t m_sect;
#endif

#ifdef _DEBUG
  long m_Count;
#endif

public:
  CriticalSection() {
#ifdef _DEBUG
    m_Count = 0;
#endif
#ifdef WIN32
    ::InitializeCriticalSection(&m_sect);
#else
    pthread_mutex_init(&m_sect, NULL);
#endif
  }
  ~CriticalSection() {
#ifdef _DEBUG

    if (m_Count != 0) {
      ::MessageBoxW(NULL , L"锁计数器不为" , L"warning!", MB_OK);
    }

#endif
#ifdef WIN32
    ::DeleteCriticalSection(&m_sect);
#else
    pthread_mutex_destroy(&m_sect);
#endif
  }


  void Lock(void) {
#ifdef _DEBUG
    InterlockedIncrement(&m_Count);
#endif

#ifdef WIN32
    ::EnterCriticalSection(&m_sect);
#else
    pthread_mutex_lock(&m_sect);
#endif
  }

  void Unlock(void) {
#ifdef _DEBUG
    InterlockedDecrement(&m_Count);
#endif
#ifdef WIN32
    ::LeaveCriticalSection(&m_sect);
#else
    pthread_mutex_unlock(&m_sect);
#endif
  }
};


class AoutLock
{
  CriticalSection* m_pCritalSection;
public:
  AoutLock(CriticalSection* pCritalSection) {
    if (pCritalSection) {
      pCritalSection->Lock();
      m_pCritalSection = pCritalSection;
    }
  }
  ~AoutLock() {
    if (m_pCritalSection) {
      m_pCritalSection->Unlock();
    }
  }
};

#endif