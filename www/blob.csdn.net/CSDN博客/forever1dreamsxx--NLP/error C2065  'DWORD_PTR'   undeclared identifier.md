
# error C2065: 'DWORD_PTR' : undeclared identifier - forever1dreamsxx--NLP - CSDN博客


2013年05月08日 10:36:21[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：1509个人分类：[C/C++																](https://blog.csdn.net/forever1dreamsxx/article/category/1226589)[计算机编程																](https://blog.csdn.net/forever1dreamsxx/article/category/1180978)[
							](https://blog.csdn.net/forever1dreamsxx/article/category/1226589)


转载地址：[http://wenku.baidu.com/view/4535663b5a8102d276a22f5e.html](http://wenku.baidu.com/view/4535663b5a8102d276a22f5e.html)
--------------------Configuration: CGridListCtrlEx - Win32 Debug--------------------
Compiling...
CGridListCtrlEx.cpp
C:\Documents and Settings\Administrator\桌面\CGridListCtrlEx_Demo\CGridListCtrlEx\CGridListCtrlEx.cpp(1887) : error C2065: 'DWORD_PTR' : undeclared identifier
C:\Documents and Settings\Administrator\桌面\CGridListCtrlEx_Demo\CGridListCtrlEx\CGridListCtrlEx.cpp(1887) : error C2146: syntax error : missing ';' before identifier 'nRowItemData'
C:\Documents and Settings\Administrator\桌面\CGridListCtrlEx_Demo\CGridListCtrlEx\CGridListCtrlEx.cpp(1887) : error C2065: 'nRowItemData' : undeclared identifier
Error executing cl.exe.
CGridListCtrlEx.exe - 3 error(s), 0 warning(s)
如上是该程序经过编译出现的错误，主要原因是DWORD_PTR没有定义
我的编译条件：window xp sp3(32位机) + vc6.0
该程序本来是没有问题的，该程序编译不通过是因为window的版本问题，该程序
原本是在64位机上编译的，所以该程序在我的机器上不兼容，没有定义DWORD_PTR
解决方案：
在stdafx.h文件(建议放在这里)中或者出现DWORD_PTR的文件(前提是DWORD_PTR只在一个文件中出现过，目的是
避免出现重复定义的错误)中定义如下：
\#if defined(_WIN64)
typedef unsigned __int64 ULONG_PTR;
\#else
typedef unsigned long ULONG_PTR;
\#endif
typedef ULONG_PTR DWORD_PTR;
经过测试，该程序编译通过！

