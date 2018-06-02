// DebugAssistant.h: interface for the DebugAssistant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGASSISTANT_H__ABA986C8_ED65_4019_9DAB_9683AB180B28__INCLUDED_)
#define AFX_DEBUGASSISTANT_H__ABA986C8_ED65_4019_9DAB_9683AB180B28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TcharEx.h"
#include "CriticalSection.h"
#include <stdio.h>
#include <stdarg.h>

enum DEBUG_TYPE {
  DEBUG_CONSOLE = 1,
  DEBUG_STDOUT  = 2,
  DEBUG_FILE    = 3
};

class DebugAssistant
{
  DEBUG_TYPE m_DebugMode;
  FILE*      m_pFile;
#ifdef WIN32
  HANDLE     m_OutputHandle;
  CriticalSection m_SynLog;
#endif
public:
  DebugAssistant(DEBUG_TYPE mode = DEBUG_STDOUT ,  const char* fileName = NULL)
    : m_DebugMode(mode)
#ifdef WIN32
    , m_OutputHandle(NULL)
#endif
    , m_pFile(NULL) {
#ifdef WIN32

    if (DEBUG_CONSOLE == m_DebugMode) {
      AllocConsole();
      m_OutputHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
    }

#endif

    if (NULL != fileName) {
      m_pFile = _tfopen(fileName , _T("wb"));
    }
  }
  virtual ~DebugAssistant() {
#ifdef WIN32

    if (DEBUG_CONSOLE == m_DebugMode) {
      if (m_OutputHandle) {
        ::CloseHandle(m_OutputHandle);
        m_OutputHandle = NULL;
      }

      FreeConsole();
    }

#endif

    if (m_pFile) {
      fclose(m_pFile);
    }
  }
public:
  void OutputInfo(const char* format , ...) {
    TCHAR msg[1024] = {0};
    va_list arg_ptr;
    va_start(arg_ptr, format);
    _vstprintf(msg , format, arg_ptr);
    _tcscat(msg , _T("\n"));
    va_end(arg_ptr);
    long length = _tcslen(msg);
#ifdef WIN32
    AoutLock lock(&m_SynLog);
#endif

    switch (m_DebugMode) {
    case DEBUG_CONSOLE:
#ifdef WIN32
      {
        ULONG beWritten = 0;
        ::WriteConsole(m_OutputHandle , msg , length , &beWritten , NULL);
      }
      break;
#endif

    case DEBUG_STDOUT:
#ifdef WIN32
      //::OutputDebugString( msg );
      printf("%s", msg);
#else
      printf("%s", msg);
#endif
      break;

    case DEBUG_FILE:
      if (m_pFile) {
        ::fwrite(msg , sizeof(TCHAR) , length , m_pFile);
        ::fwrite(_T("\n") , sizeof(TCHAR) , 1 ,  m_pFile);
      }

      break;

    default:
      break;
    }
  }
};

#ifdef _OUTPUT_INFO_
DebugAssistant _DebugInfo;
#define DEBUG_INFO1( format , param1 ) _DebugInfo.OutputInfo( (format) , (param1) );
#define DEBUG_INFO2( format , param1 , param2 )  _DebugInfo.OutputInfo( (format) , (param1) , (param2) );
#define DEBUG_INFO3( format , param1 , param2 , param3 )  _DebugInfo.OutputInfo( (format) , (param1) , (param2) , (param3) );
#define DEBUG_INFO4( format , param1 , param2 , param3 , param4 )  _DebugInfo.OutputInfo( (format) , (param1) , (param2) , (param3) , (param4) );
#define DEBUG_INFO5( format , param1 , param2 , param3 , param4 , param5 )  _DebugInfo.OutputInfo( (format) , (param1) , (param2) , (param3) , (param4) , (param5) );
#else
#define DEBUG_INFO1( format , param1 )                   0
#define DEBUG_INFO2( format , param1 , param2 )          0
#define DEBUG_INFO3( format , param1 , param2 , param3 ) 0
#define DEBUG_INFO4( format , param1 , param2 , param3 , param4 ) 0
#define DEBUG_INFO5( format , param1 , param2 , param3 , param4 , param5 ) 0
#endif

#endif // !defined(AFX_DEBUGASSISTANT_H__ABA986C8_ED65_4019_9DAB_9683AB180B28__INCLUDED_)
