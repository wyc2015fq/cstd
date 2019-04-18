# 在2010以Release方式编译项目时出现的错误 - ljx0305的专栏 - CSDN博客
2010年11月18日 16:02:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1147标签：[2010																[多线程																[dll																[mfc																[class																[application](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)
今日使用vs2010编译项目，碰到这样一个错误：
-----------------------------------------------------------------------------------------------------
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: _strstr 已经在 libcmtd.lib(strstr.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: _strncpy 已经在 libcmtd.lib(strncpy.obj) 中定义
1>msvcrtd.lib(ti_inst.obj) : error LNK2005: "private: __thiscall type_info::type_info(class type_info const &)" ([??0type_info@@AAE@ABV0@@Z](mailto:??0type_info@@AAE@ABV0@@Z) ) 已经在 libcmtd.lib(typinfo.obj) 中定义
1>msvcrtd.lib(ti_inst.obj) : error LNK2005: "private: class type_info & __thiscall type_info::operator=(class type_info const &)" ([??4type_info@@AAEAAV0@ABV0@@Z](mailto:??4type_info@@AAEAAV0@ABV0@@Z) ) 已经在 libcmtd.lib(typinfo.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: __resetstkoflw 已经在 libcmtd.lib(resetstk.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: __CrtDbgReportW 已经在 libcmtd.lib(dbgrptw.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: _wcslen 已经在 libcmtd.lib(wcslen.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: _wcscpy_s 已经在 libcmtd.lib(wcscpy_s.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: _malloc 已经在 libcmtd.lib(dbgheap.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: _free 已经在 libcmtd.lib(dbgheap.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: _calloc 已经在 libcmtd.lib(dbgheap.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: __recalloc 已经在 libcmtd.lib(dbgheap.obj) 中定义
1>msvcrtd.lib(MSVCR80D.dll) : error LNK2005: __setmbcp 已经在 libcmtd.lib(mbctype.obj) 中定义
-----------------------------------------------------------------------------------------------------
解决方案中包含两个项目，
项目1生成一个静态链接库：pro1.lib；
项目2是一个exe项目，编译时链入pro1.lib。
看了下两个项目的编译属性中“运行时库”选项，分别是
项目1：多线程调试 DLL (/MDd)
项目2：多线程调试(/MTd)
先将项目2改成 “多线程调试 DLL (/MDd)”，编译，错误变成：
1>c:/program files/microsoft visual studio 8/vc/atlmfc/include/afx.h(24) : fatal error C1189: #error :  Building MFC application with /MD[d] (CRT dll version) requires MFC shared dll version. Please #define _AFXDLL or do not use /MD[d]
在项目2中加入预编译符 “_AFXDLL”
编译通过！
看来当不同项目运行时库选择不同时，就会出现类似msvcrtd.lib(MSVCR80D.dll) : error LNK2005: _free 已经在 libcmtd.lib(dbgheap.obj) 中定义的错误
具体原因分析中……
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/hongjiqin/archive/2009/04/23/4104959.aspx](http://blog.csdn.net/hongjiqin/archive/2009/04/23/4104959.aspx)
