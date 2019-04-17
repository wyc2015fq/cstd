# unresolved external symbol __endthreadex错误解决 - 战斗蜗牛的专栏 - CSDN博客





2011年11月07日 14:33:55[vbskj](https://me.csdn.net/vbskj)阅读数：701标签：[mfc																[library																[generation																[dll																[windows																[system](https://so.csdn.net/so/search/s.do?q=system&t=blog)
个人分类：[C/C++/Visual C++](https://blog.csdn.net/vbskj/article/category/630758)










在用VC6.0写程序调试时，初学者总是会遇到一些错误，针对如下错误主要是因为MFC类库没有引用所出现的问题。

错误现象：

nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolved external symbol __endthreadex

nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolved external symbol __beginthreadex

Debug/jnHid.exe : fatal error LNK1120: 2 unresolved externals

Error executing link.exe.

解决错误的方法：

选择Project－Settings--General--Microsoft foundation Classes

下拉列表中有三个选项：

1、Not using MFC

2、Use MFC in a Static Libray

3、Use MFC in a Shared DLL 


本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/Leolzy/archive/2007/01/15/1483489.aspx](http://blog.csdn.net/Leolzy/archive/2007/01/15/1483489.aspx)




错误提示：


nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolved external symbol __endthreadex

nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolved external symbol __beginthreadex

libcd.lib(crt0.obj) : error LNK2001: unresolved external symbol _main

Debug/Hello.exe : fatal error LNK1120: 3 unresolved externals


第一种解决方法：


1、“Project”->“settings”->“c/c++”


“Catagory” 选择“Code Generation”


   “use run-time library”选择“debug multithreaded”


此时确定一般解决问题，也有可能出现如下问题提示：


libcmtd.lib(crt0.obj) : error LNK2001: unresolved external symbol _main

Debug/Hello.exe : fatal error LNK1120: 1 unresolved externals


此时，进行第二步操作：


2、


[Project] --> [Settings] --> 选择"Link"属性页,

在Project Options中将/subsystem:console改成/subsystem:windows


温馨提示：在Project Options窗口，右边滑动条向下滑动就会找到该项！


总结：进行以上两步操作后，问题解决！




第二种解决方法：


1.检查是否包含头文件afx.h

2.打开project->settings->general->microsoft foundation classes->

选use MFC in a static library

或选 use MFC in a shared DLL(需要把MFC的动态库放在system32文件夹下) .  




本人是碰到的


nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolved external symbol __endthreadex

nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolved external symbol __beginthreadex


这两行错误，故用第二种方法解决，分析原因是我调用了MFC类库，但建立的控制台程序默认不加载MFC类库，故需要改此处设置。


第一种方法是在网上搜的，碰到类似的问题可以尝试一下。](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=generation&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)




