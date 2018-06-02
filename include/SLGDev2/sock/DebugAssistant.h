// DebugAssistant.h: interface for the DebugAssistant class.
//
#if !defined(AFX_DEBUGASSISTANT_H__ABA986C8_ED65_4019_9DAB_9683AB180B28__INCLUDED_)
#define AFX_DEBUGASSISTANT_H__ABA986C8_ED65_4019_9DAB_9683AB180B28__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TcharEx.h"
#include "CriticalSection.h"
#include <stdio.h>
enum DEBUG_TYPE {
  DEBUG_CONSOLE = 1,
  DEBUG_STDOUT  = 2,
  DEBUG_FILE    = 3
};
class DebugAssistant
{
  DEBUG_TYPE m_DebugMode;
  FILE*      m_pFile;
  void*     m_OutputHandle;
  CriticalSection m_SynLog;
public:
  DebugAssistant(DEBUG_TYPE mode = DEBUG_STDOUT ,  const char* fileName = NULL);
  virtual ~DebugAssistant();
public:
  void OutputInfo(const char* format , ...);
};
#ifdef _OUTPUT_INFO_
extern DebugAssistant _DebugInfo;
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

