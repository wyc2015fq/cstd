// ****************************************************************************************************
// 文件: asm.h
// 注释:
// 汇编函数模块, C语言没有函数重载,  "C" 无效
// ****************************************************************************************************
#ifndef __V32_ASM_H_
#define __V32_ASM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CRITICAL_SECTION cs;     // 全局表访问锁

CBase** g_ppObjects;     // 全局对象指针表
UINT g_dwObjects;

// __declspec(naked) for implementation only

// v32.cpp
void fnTableInit();
void fnTableLock();
void fnTableLoop();
void fnTableExit();

// asm_index.cpp

// asm_strlen.cpp

// asm_unique.cpp
int  fnArrayUniqueDummy(void* lpVoid, UINT dwSize, void* lpRefer);

// asm_memcpy.cpp

// asm_memset.cpp

// asm_sort.cpp
int  fnArraySortDummy(void* lpVoid, UINT dwSize, void* lpIndex);

#endif