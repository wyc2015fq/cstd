// ****************************************************************************************************
// 文件: debug.cpp
// 注释:
// 推箱子调试函数模块, 错误代码功能, 调试版本输出调试信息到程序目录下的debug.txt
// ****************************************************************************************************
#include <stdio.h> // FILE, vsprintf
#include <stdarg.h> // va_start, va_end
#define __VENUS_DLL_INC_ // DLL内部编译
#include "api.h"
#include "debug.h"
using namespace Valiant::Gaming;
using namespace Valiant;
// globals
char g_szText[256] = {'\0'};
UINT g_dwCode = 0;
#ifdef _DEBUG
const char* g_szTitle[] = {"上", "下", "左", "右"};
FILE* pfo = NULL;
#endif
UINT fnStageCode(UINT dwCode)
{
  UINT dwNum = g_dwCode;
  if (dwCode != SEC_ERROR_DUMP) {
    g_dwCode = dwCode;
  }
  return dwNum; // 返回旧的代码
}
#ifdef _DEBUG
int fnDebugInit()
{
  if (pfo) {
    fnDebugExit();
  }
  pfo = fopen("debug.txt", "wb");
  if (pfo == NULL) {
    return 1;
  }
  return 0;
}
const char* fnGetText(unsigned int dwFlags)
{
  return g_szTitle[dwFlags];
}
int fnPrint(const char* szText, ...)
{
  va_list args;
  int ret;
  va_start(args, szText); // 第一个参数为指向可变参数字符指针的变量,第二个参数是可变参数的第一个参数,通常用于指定可变参数列表中参数的个数
  if (pfo) {
    ret = vfprintf(pfo, szText, args);
  }
  va_end(args); // 将存放可变参数字符串的变量清空
  return ret;
}
void fnPrintBox(PSTAGE pStage)
{
  PSPOT ps = pStage->Stars;
  for (UINT dwLoop = 0; dwLoop < pStage->Count; dwLoop++) {
    fnPrint("\tBox%d=(%d, %d)", dwLoop, ps->X, ps->Y);
    ps++;
  }
  fnPrint("\r\n");
}
void fnPrintStage(PSTAGE pStage, int printTrees)
{
  long i, j;
  while (pStage) {
    fnPrint("Stage: 0x%08X:\r\n", pStage);
    for (j = 0; j < pStage->SizeY; j++) {
      for (i = 0; i < pStage->SizeX; i++) {
        //switch(pStage->Matrix[j * pStage->SizeX + i] & SMT_FILTER){
        //case SMT_UNKNOW:
        //case SMT_OBJECT:
        //case SMT_TARGET:
        //case SMT_BORDER:
        //case SMT_PORTER:
        //}
        fnPrint(" %02X", pStage->Matrix[j * pStage->SizeX + i] & SMT_FILTER);
      }
      fnPrint("\r\n");
    }
    fnPrint("\r\n");
    if (printTrees) {
      pStage = pStage->Host;
    }
  }
}
void fnOutput()
{
  if (pfo) {
    fflush(pfo);
  }
}
int fnDebugExit()
{
  if (pfo) {
    fclose(pfo);
    pfo = NULL;
  }
  return 0;
}
#endif

