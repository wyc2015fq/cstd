// ****************************************************************************************************
// 文件: prefix.h
// 注释:
// 内部编译, 定义__V32_DLL_API_再包含此文件, 内部编译结束再包含subfix.h
// ****************************************************************************************************
#ifndef __V32_PFX_H_
#define __V32_PFX_H_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#ifdef __V32_SFX_H_
#undef __V32_SFX_H_
#endif

#ifdef __V32_MACRO_     // 内部保留的宏(限文件头内部使用)
#error "__V32_MACRO_ is reserved for Valiant header files!\r\n"
#endif

#ifdef VFC          // Valiant foudation class
#define __V32_MACRO_ VFC  // 暂存宏
#undef VFC          // 取消宏
#endif

#ifndef __V32_DLL_API_          // 内部编译标识
#define VFC    // .DLL外部引用
#else
#define VFC    // .DLL内部编译
#endif

#ifdef V32API
#undef V32API       // 取消宏
#endif

#ifndef V32API
#ifdef __cplusplus      // .cpp自动定义
#ifndef __V32_DLL_API_          // 内部编译标识(VFC文件头外被替换, 不能正确展开)
#define V32API
#else
#define V32API
#endif
#else
#ifndef __V32_DLL_API_          // 内部编译标识(VFC文件头外被替换, 不能正确展开)
#define V32API
#else
#define V32API
#endif
#endif
#endif

#endif
