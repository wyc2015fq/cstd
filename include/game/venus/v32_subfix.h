// ****************************************************************************************************
// 文件: prefix.h
// 注释:
// 内部编译, 定义__V32_DLL_API_再包含此文件, 内部编译结束再包含subfix.h
// ****************************************************************************************************
#ifndef __V32_SFX_H_
#define __V32_SFX_H_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#ifdef __V32_PFX_H_
#undef __V32_PFX_H_
#endif

#ifdef __V32_DLL_API_
#undef __V32_DLL_API_   // 取消内部编译标识
#endif

#ifdef __V32_MACRO_     // VFC在文件外有定义, 恢复它
#undef VFC          // 取消宏
#define VFC __V32_MACRO_  // 恢复宏
#undef __V32_MACRO_     // 取消宏
#else
#undef VFC          // VFC在文件外无定义, 取消它
#endif

#endif