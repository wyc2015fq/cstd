/*
 此文件用于在调试期发现内存泄漏仅在VC++编译器中适用
 (包括Intel C++,因为它使用了相同的库)
 MFC中提供的debug new虽然加了锁,但我在实际测试的时候发现多线程并发
 调用的时候还是抛出了系统错误,所以我在这里加了一个线程互斥量.
 debug new和debug delete之间需不需要互斥我并不知道,保险起见,我同样
 加了线程互斥量.
 按照C++标准规定,在operator new失败后应当调用set_new_handler设置的
 函数,但是MSDN中却说"头文件new中的set_new_handler是stub的,而应该使
 用头文件new.h中的_set_new_handler",这简直是滑天下之大稽.
 以下是VC++6.0中的set_new_handler定义:
 new_handler __cdecl set_new_handler( new_handler new_p ) {
   assert( new_p == 0 ); // cannot use stub to register a new handler
   _set_new_handler( 0 );
   return 0;
 }
 所以我也无计可施,只能舍弃set_new_handler的作用.
//warning C4005: 'new' : macro redefinition
//#pragma warning(disable: 4005)
使用范例:
  在这里进行内存分配
static void test_debug_new() {
  char* p = malloc(2);
  MEM_CHECK_BEGIN();
  p[0] = 'A';
  p[1] = 'B';
  MEM_CHECK_END();
  free(p);
}
MEM_CHECK_BEGIN();
MEM_CHECK_END();
*/
#ifndef _DEBUG_H_
#define _DEBUG_H_
#ifdef _DEBUG
#include <windows.h>
#include <malloc.h>
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h> // for vsprintf
#include <tchar.h>
//#include "tchar_x.h"
// 声明内存状态结构变量
typedef _CrtMemState MemState[ 3 ];
static int _RegDebugNew_init = 0;
#define _RegDebugNew() (_CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF ), 1)
static void mem_recheck(MemState s)
{
  if (0 == _RegDebugNew_init) {
    _RegDebugNew_init = _RegDebugNew();
  }
  //memset(s, 0, 3*sizeof(_CrtMemState));
  // 获取第一个内存状态快照
  _CrtMemCheckpoint(&s[ 0 ]);
}
static void mem_dump(MemState s)
{
  // 获取第二个内存状态快照
  _CrtMemCheckpoint(&s[ 1 ]);
  // 比较两个内存快照的差异， dump 差异结果
  if (_CrtMemDifference(&s[ 2 ], &s[ 0 ], &s[ 1 ])) {
    _CrtMemDumpStatistics(&s[ 2 ]);
  }
  // 输出内存泄漏报告，显示在 Output 窗口的 Debug 页中
  _CrtDumpMemoryLeaks();
}
#ifndef _VT_BUF_LEN
#define _VT_BUF_LEN (4096)
#endif
static void _vtrace_dbg(const TCHAR* fmt, va_list argptr)
{
#if 0
  int i = 0, fmtlen, buflen;
  TCHAR* buf;
  TCHAR buf2[ _VT_BUF_LEN + 1 ];
  fmtlen = _tcslen(fmt);
  fmtlen *= 10;
  buf = (TCHAR*) malloc(fmtlen);
  memset(buf, 0, fmtlen);
  _vsntprintf(buf, fmtlen, fmt, argptr);
  buflen = _tcslen(buf);
  for (i = 0; i < buflen; i += _VT_BUF_LEN) {
    memcpy(buf2, buf + i, _VT_BUF_LEN * sizeof(TCHAR));
    buf2[ _VT_BUF_LEN ] = 0;
    OutputDebugString(buf2);
    if (buflen > _VT_BUF_LEN) {
      OutputDebugString(_T("\n"));
    }
  }
  free(buf);
#else
  TCHAR buf[ _VT_BUF_LEN ] = {0};
  _vsntprintf(buf, _VT_BUF_LEN, fmt, argptr);
  OutputDebugString(buf);
#endif
}
static int _trace_dbg(const TCHAR* fmt, ...)
{
  va_list argptr;
  va_start(argptr, fmt);
  _vtrace_dbg(fmt, argptr);
  va_end(argptr);
  return 0;
}
#define malloc(nSize) _malloc_dbg( nSize, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define realloc(_p, nSize) _realloc_dbg( _p, nSize, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define free(p)       _free_dbg( p, _NORMAL_BLOCK )
#define dprintf       _trace_dbg
#ifdef __cplusplus
inline void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine, int pp)
{
  return _malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}
inline void operator delete(void* pData, LPCSTR /* lpszFileName */, int /* nLine */, int pp)
{
  _free_dbg(pData, _NORMAL_BLOCK);
}
#define _DEBUG_NEW new(__FILE__, __LINE__, 0)
#define new _DEBUG_NEW
#define _DEBUG_DELETE delete(__FILE__, __LINE__, 0)
//#define delete _DEBUG_DELETE
#endif
#undef MEM_CHECK_BEGIN
#undef MEM_CHECK_END
#define MEM_CHECK_BEGIN  { MemState __mm; mem_recheck(__mm); {int __aux=0
#define MEM_CHECK_END    } mem_dump(__mm); }
#if 0
MEM_CHECK_BEGIN;
MEM_CHECK_END;
#endif
#ifdef _DEBUG
#define printf       _trace_dbg
#endif
#define __out_ecount_opt(x)
#include <dbghelp.h>
#include <stdio.h>
#pragma comment(lib,"dbghelp.lib")
static LPSTR GetSymbolSearchpath(LPSTR path)
{
  char   directory [MAX_PATH];
  char   drive [MAX_PATH];
  HMODULE module;
  memset(path, 0, MAX_PATH * sizeof(path[0]));
  // Oddly, the symbol handler ignores the link to the PDB embedded in the
  // executable image. So, we'll manually add the location of the executable
  // to the search path since that is often where the PDB will be located.
  path[0] = '\0';
  module = GetModuleHandle(NULL);
  GetModuleFileNameA(module, path, MAX_PATH);
  _splitpath(path, drive, directory, NULL, 0);
  strcpy(path, drive);
  strcat(path, directory);
  // Append the working directory.
  strcat(path, ";.\\");
  return path;
}
void uninit_sym(void)
{
  SymCleanup(GetCurrentProcess());
}
int init_sym(BOOL fInvadeProcess)
{
  char  symbolpath[MAX_PATH];
  GetSymbolSearchpath(symbolpath);
  SymInitialize(GetCurrentProcess(), symbolpath, fInvadeProcess);
  atexit(uninit_sym);
  return 0;
}
char* dbg_fuctionname(DWORD dwAddr)
{
  static char szFuctionName[32];
  BYTE  symbolBuffer[sizeof(SYMBOL_INFO) + 256 * sizeof(CHAR)] = { 0 };
  PSYMBOL_INFO pSymbol = (PSYMBOL_INFO) &symbolBuffer;
  static inited = 0;
  if (0 == inited) {
    inited = 1;
    init_sym(1);
  }
  memset(szFuctionName, 0, sizeof(szFuctionName));
  pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  pSymbol->MaxNameLen = 256;
  if (SymFromAddr(GetCurrentProcess(), dwAddr, 0, pSymbol)) {
    strcpy(szFuctionName, pSymbol->Name);
  }
  else {
    printf("SymFromAddr Failed! %d\n", GetLastError());
  }
  return szFuctionName;
}
static DWORD GetEIP()
{
  DWORD dwCallerAddr;
  __asm {
    push DWORD  ptr[ebp+4]
    pop  DWORD  ptr[dwCallerAddr]
    sub  DWORD  ptr[dwCallerAddr], 5 //减去 call GetEIP() 的长度
  }
  return dwCallerAddr;
}
#define __func_for_vc6__   (dbg_fuctionname(GetEIP()))
#else // _DEBUG
/*
MEM_CHECK_BEGIN()
MEM_CHECK_END()
*/
#define MEM_CHECK_BEGIN()
#define MEM_CHECK_END()
#endif // _DEBUG
#endif // _CRTDBG_H_

