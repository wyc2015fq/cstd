# VC6环境下USB编程链接错误解决办法 - 2019跟着小虎玩着去软考 - CSDN博客
2010年08月10日 15:57:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：2284标签：[编程																[include																[mfc																[service																[windows																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[C/C++](https://blog.csdn.net/littletigerat/article/category/666612)
**VC6****环境下USB编程链接错误解决办法**
## 背景介绍：
编程环境：Visual C++ 6.0 + SP6
操作系统：Windows XP Service Pack 3
## 提出问题：
在MFC应用程序中引用经典的USBView源码过程中，出现如下错误：
usb.obj : error LNK2001: unresolved external symbol "unsigned long __stdcall CM_Get_Parent(unsigned long *,unsigned long,unsigned long)" (?CM_Get_Parent@@YGKPAKKK@Z)
usb.obj : error LNK2001: unresolved external symbol "unsigned long __stdcall CM_Get_Sibling(unsigned long *,unsigned long,unsigned long)" (?CM_Get_Sibling@@YGKPAKKK@Z)
usb.obj : error LNK2001: unresolved external symbol "unsigned long __stdcall CM_Get_Child(unsigned long *,unsigned long,unsigned long)" (?CM_Get_Child@@YGKPAKKK@Z)
usb.obj : error LNK2001: unresolved external symbol "unsigned long __stdcall CM_Get_DevNode_Registry_PropertyA(unsigned long,unsigned long,unsigned long *,void *,unsigned long *,unsigned long)" (?CM_Get_DevNode_Registry_PropertyA@@YGKKKPAKPAX0K@
Z)
usb.obj : error LNK2001: unresolved external symbol "unsigned long __stdcall CM_Locate_DevNodeA(unsigned long *,char *,unsigned long)" (?CM_Locate_DevNodeA@@YGKPAKPADK@Z)
../bin/Debug/ATSync.exe : fatal error LNK1120: 5 unresolved externals
## 分析问题：
CM_Locate_DevNode
CM_Get_DevNode_Registry_Property 
CM_Get_Sibling
CM_Get_Child
CM_Get_Parent
这些函数都来自于同一个cfgmgr32.lib静态链接库中，
而cfgmgr32.lib库应该是使用C语言进行编译的，所以在使用C++进行编程时，在引用cfgmgr32.h头文件时不能简单地使用include，即#include "cfgmgr32.h"，而需要加入extern “C”的声明。
## 解决问题：
#ifdef __cplusplus
extern "C"{
#include "cfgmgr32.h"
}
#endif
