// ****************************************************************************************************
// 文件: api.h
// 注释:
// 接口定义模块, 定义输出的类和函数接口
// ****************************************************************************************************
#ifndef __V32_H_
#define __V32_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __V32_DLL_INC_
#define __V32_DLL_API_        // 内部编译标识
#endif

#include "v32_prefix.h"   // 预编译宏, 切换内外部编译

// auto include
#include "v32_macro.h"    // 宏定义
#include "v32_type.h"   // 类型

// ****************************************************************************************************
// Begin export function declaration, __declspec(naked) for definition only
// ****************************************************************************************************
int V32Version(Valiant::PBETA pVersion);

int V32ByteSize(const char* szText);
int V32CharSize(const char* szText);
int V32TextSize(const char* szText, long* pdwCount);

void V32Clear(void* p, const unsigned long v, const long n);
void V32Clear8(void* p, const unsigned char v, const long n);
void V32Clear16(void* p, const unsigned short v, const long n);
void V32Clear32(void* p, const unsigned long v, const long n);

void V32Series8(void* p, const char v, unsigned long n);
void V32Series16(void* p, const short v, unsigned long n);
void V32Series32(void* p, const long v, unsigned long n);
void V32Series(void* p, unsigned long n);
void V32SeriesEx(void* p, const long v, const long d, unsigned long n);
void V32Copy(void* dst, const void* src, const unsigned long n);

int V32ArrayUniqueValue8(void* lpVoid, UINT dwSize);
int V32ArrayUniqueIndex8(void* lpVoid, UINT dwSize, void* lpRefer);
int V32ArrayUniqueValue16(void* lpVoid, UINT dwSize);
int V32ArrayUniqueIndex16(void* lpVoid, UINT dwSize, void* lpRefer);
int V32ArrayUniqueValue32(void* lpVoid, UINT dwSize);
int V32ArrayUniqueIndex32(void* lpVoid, UINT dwSize, void* lpRefer);

int V32ArraySortIndexAsc8(void* lpVoid, UINT dwSize, void* lpSort);
int V32ArraySortValueAsc8(void* lpVoid, UINT dwSize);
int V32ArraySortIndexDesc8(void* lpVoid, UINT dwSize, void* lpSort);
int V32ArraySortValueDesc8(void* lpVoid, UINT dwSize);
int V32ArraySortIndexAsc16(void* lpVoid, UINT dwSize, void* lpSort);
int V32ArraySortValueAsc16(void* lpVoid, UINT dwSize);
int V32ArraySortIndexDesc16(void* lpVoid, UINT dwSize, void* lpSort);
int V32ArraySortValueDesc16(void* lpVoid, UINT dwSize);
int V32ArraySortIndexAsc32(void* lpVoid, UINT dwSize, void* lpSort);
int V32ArraySortValueAsc32(void* lpVoid, UINT dwSize);
int V32ArraySortIndexDesc32(void* lpVoid, UINT dwSize, void* lpSort);
int V32ArraySortValueDesc32(void* lpVoid, UINT dwSize);

int V32ArrayIndex8(void* p, const unsigned char v, unsigned long n);
int V32ArrayIndex16(void* p, const unsigned short v, unsigned long n);
int V32ArrayIndex32(void* p, const unsigned long v, unsigned long n);
// ****************************************************************************************************
// End export function declaration
// ****************************************************************************************************

// ****************************************************************************************************
// Begin cplusplus class declaration
// ****************************************************************************************************

  static int strlenb(const char* szText);
  static int strlenc(const char* szText);
  static int strlen(const char* szText, long* pdwCount);

  static void Clear(void* p, unsigned long v, unsigned long n); // Remove const for overloaded function
  static void Clear(void* p, const unsigned char v, const long n);
  static void Clear(void* p, const unsigned short v, const long n);
  static void Clear(void* p, const unsigned long v, const long n);

  static void Series(void* p, const char v, unsigned long n);
  static void Series(void* p, const short v, unsigned long n);
  static void Series(void* p, const long v, unsigned long n);
  static void Series(void* p, unsigned long n);
  static void Series(void* p, const long v, const long d, unsigned long n);
  static void Copy(void* dst, const void* src, const unsigned long n);

  static int Unique(unsigned char* p, const unsigned long n, void* pp); // For overloaded function
  static int Unique(unsigned char* p, const unsigned long n);
  static int Unique(unsigned short* p, const unsigned long n, void* pp);
  static int Unique(unsigned short* p, const unsigned long n);
  static int Unique(unsigned long* p, const unsigned long n, void* pp);
  static int Unique(unsigned long* p, const unsigned long n);

  static int SortIndexAsc(unsigned char* p, const unsigned long n, void* pi);
  static int SortValueAsc(unsigned char* p, const unsigned long n);
  static int SortIndexDesc(unsigned char* p, const unsigned long n, void* pi);
  static int SortValueDesc(unsigned char* p, const unsigned long n);
  static int SortIndexAsc(unsigned short* p, const unsigned long n, void* pi);
  static int SortValueAsc(unsigned short* p, const unsigned long n);
  static int SortIndexDesc(unsigned short* p, const unsigned long n, void* pi);
  static int SortValueDesc(unsigned short* p, const unsigned long n);
  static int SortIndexAsc(unsigned long* p, const unsigned long n, void* pi);
  static int SortValueAsc(unsigned long* p, const unsigned long n);
  static int SortIndexDesc(unsigned long* p, const unsigned long n, void* pi);
  static int SortValueDesc(unsigned long* p, const unsigned long n);

  static int Index(void* p, const unsigned char v, unsigned long n);
  static int Index(void* p, const unsigned short v, unsigned long n);
  static int Index(void* p, const unsigned long v, unsigned long n);


// ****************************************************************************************************
// End cplusplus class declaration
// ****************************************************************************************************

#include "v32_subfix.h"   // 预编译宏, 切换内外部编译

#endif