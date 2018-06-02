#ifndef _DEBUGASSISTANT_INL_
#define _DEBUGASSISTANT_INL_
#include "DebugAssistant.h"
#include <stdarg.h>
#include <string.h>
#ifdef _OUTPUT_INFO_
DebugAssistant _DebugInfo(DEBUG_CONSOLE/*DEBUG_FILE , _T( "chsjcard.log" )*/);
#endif
DebugAssistant::DebugAssistant(DEBUG_TYPE mode , const char* fileName):
  m_DebugMode(mode),
  m_OutputHandle(NULL),
  m_pFile(NULL)
{
  if (DEBUG_CONSOLE == m_DebugMode) {
#ifdef WIN32
    AllocConsole();
    m_OutputHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
#else
    m_OutputHandle = stdout;
#endif
  }

  if (NULL != fileName) {
    m_pFile = fopen(fileName , ("wb"));
  }
}
DebugAssistant::~DebugAssistant()
{
  if (DEBUG_CONSOLE == m_DebugMode) {
#ifdef WIN32

    if (m_OutputHandle) {
      ::CloseHandle(m_OutputHandle);
      m_OutputHandle = NULL;
    }

    FreeConsole();
#else
    m_OutputHandle = NULL;
#endif
  }

  if (m_pFile) {
    fclose(m_pFile);
  }
}
void DebugAssistant::OutputInfo(const char* format , ...)
{
  char msg[1024] = {0};
  va_list arg_ptr;
  va_start(arg_ptr, format);
  vsprintf(msg , format, arg_ptr);
  strcat(msg , ("\n"));
  va_end(arg_ptr);
  int length = strlen(msg);
  AoutLock lock(&m_SynLog);

  switch (m_DebugMode) {
  case DEBUG_CONSOLE: {
#ifdef WIN32
    ULONG beWritten = 0;
    ::WriteConsole(m_OutputHandle , msg , length , &beWritten , NULL);
#else
    fprintf((FILE*)m_OutputHandle, "%s", msg);
#endif
  }
  break;

  case DEBUG_STDOUT:
#ifdef WIN32
    ::OutputDebugStringA(msg);
#else
    fprintf((FILE*)m_OutputHandle, "%s", msg);
#endif
    break;

  case DEBUG_FILE:
    if (m_pFile) {
      ::fwrite(msg , sizeof(char) , length , m_pFile);
      ::fwrite(("\n") , sizeof(char) , 1 , m_pFile);
    }

    break;

  default:
    break;
  }
}
#endif // _DEBUGASSISTANT_INL_

