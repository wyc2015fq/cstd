// ****************************************************************************************************
// 文件: macro.h
// 注释:
// 通用宏定义模块, 仅替换而不编译, 不存在类型声明的问题
// .def文件或使用extern "C"(不以C++规则输出函数名, 编译器会忽略, 仍输出C++规则的函数名)
// .cpp定义__V32_DLL_INC_使用不同修饰符(可重复包含)
// 宏定义是全局的, 文件仅包含一次, 若已定义则不再定义, 但编译会显示信息
// ****************************************************************************************************
#ifndef __V32_MACRO_H_
#define __V32_MACRO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// *** 函数修饰符宏定义 ***

#ifndef NAKED
#define NAKED
#else
#endif

// *** 安全释放符宏定义 ***

#ifndef SafeDelete
#define SafeDelete(p)   if(p){delete p; p = NULL;}
#else
#endif

#ifndef SafeErase
#define SafeErase(p)    if(p){delete[] p; p = NULL;}
#else
#endif

#ifndef SafeFree
#define SafeFree(p)     if(p){free(p); p = NULL;}
#else
#endif

#ifndef SafeRelease
#define SafeRelease(p)    if(p){p->Release(); p = NULL;}
#else
#endif

#ifndef SafeDeleteObject
#define SafeDeleteObject(p) if(p){DeleteObject(p); p = NULL;}
#else
#endif

#ifndef SafeCloseHandle
#define SafeCloseHandle(p)  if(p){CloseHandle(p); p = NULL;}
#else
#endif

// *** 内联汇编宏定义 ***

// *** 结构体相关宏定义 ***
#define V32_CACHE_FIXED   0x00000001  // cache is fixed

#define V32_CACHE_ARRAY   0x01000000  // cache is array
#define V32_CACHE_CHAR    0x00000100  // cache array is 8 bit
#define V32_CACHE_WORD    0x00000200  // cache array is 16 bit
#define V32_CACHE_UINT    0x00000300  // cache array is 32 bit
#define V32_CACHE_QWORD   0x00000400  // cache array is 64 bit, reserved

// *** 常量宏定义, 这些宏不能重复定义 ***
#define V32_PROCESS_ATTACH  1 // DLL_PROCESS_ATTACH
#define V32_THREAD_ATTACH 2 // DLL_THREAD_ATTACH
#define V32_THREAD_DETACH 3 // DLL_THREAD_DETACH
#define V32_PROCESS_DETACH  0 // DLL_PROCESS_DETACH

#define V32_MAJOR_VERSION 2 // major
#define V32_MINOR_VERSION 0 // minor
#define V32_BUILD_VERSION 10  // build
#define V32_DEBUG_VERSION 13  // update

#endif