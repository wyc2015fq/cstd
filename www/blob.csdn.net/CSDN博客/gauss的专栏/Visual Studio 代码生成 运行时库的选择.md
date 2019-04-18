# Visual Studio 代码生成 运行时库的选择 - gauss的专栏 - CSDN博客
2013年01月08日 22:15:51[gauss](https://me.csdn.net/mathlmx)阅读数：447
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
2012-08-14 07:02
196人阅读[评论](http://blog.csdn.net/pizi0475/article/details/7862658#comments)(0)收藏[举报](http://blog.csdn.net/pizi0475/article/details/7862658#report)
![](http://pic002.cnblogs.com/images/2011/354674/2011120115350358.png)
　　运行时库的选项有：多线程(/MT)、多线程调试(/MTD)、多线程DLL(/MD)、多线程调试(/MDd) 有时调用外部的一些库，这个选择不对，老编译不通过；
c运行时库                                             库文件
single thread(static link) ML                  libc.lib
debug single thread(static link) mld       libcd.lib
multithread(static link)MT                     libcmt.lib
debug multithread(static link) MTD         libcmtd.lib
multithread(dynamic link)MD                 msvert.lib
debug multithread(dynamic link)MDd      msertd.lib 
 各种C运行库的区别：
- 静态链接的单线程库
只能用于单线程的应用程序，C运行时库的目标代码最终被编译在二进制文件中。通过/ML编译选项可以设置Visual c++使用静态链接的单线程库。
- 静态链接的多线程库
静态链接的多线程库的目标代码也最终被编译在应用程序的二进制文件中，但是它可以在多线程应用程序中使用。通过/MT编译选项可以设置Visual c++ 使用静态链接库的多线程库。
- 动态链接的运行时库
动态连接的运行时库将所有的c库函数保存在一个单独的动态链接库MSVCRxx.DLL中，其处理了多线程问题。使用/MD编译选项可以设置Visual c++使用动态链接的运行时库。 
　　静态链接的目标代码最终都会被编译在应用程序的二进制文件中
　　/MDd、/MTd选项使用Debug runtime library(调试版本的运行时刻函数库)，与/MD、MT对应为release版本。debug版本的Runtime Library 包含了调试信息，并采用了一些保护机制以帮助发现错误，将强了错误的检测，因此性能方面比不上release版本。
