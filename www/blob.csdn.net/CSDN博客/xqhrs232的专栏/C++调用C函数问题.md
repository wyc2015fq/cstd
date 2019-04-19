# C++调用C函数问题 - xqhrs232的专栏 - CSDN博客
2011年06月24日 16:03:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：748标签：[c																[c++																[编译器																[mfc](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/nataka/archive/2005/09/17/482671.aspx](http://blog.csdn.net/nataka/archive/2005/09/17/482671.aspx)
相关网帖
1。**c++中调用c函数**
[http://hi.baidu.com/zhenxigly/blog/item/6c130c1d7252746ff624e4a4.html](http://hi.baidu.com/zhenxigly/blog/item/6c130c1d7252746ff624e4a4.html)
碰到一个问题，在MFC的类中调用自己写的C函数，出现错误说无法找到函数的定义，原来是由于C 和C++对函数的命名方式不同，C++的函数命名中还包括了各参数类型，因此通常情况下，C++程序无法找到C库中的函数，需要在声明C函数时加上 extern "C"的说明：
 extern "C" void foo();C++编译器就会用C的命名方式来进行连接。同样，当C需要调用C++函数时，该C++函数也必须声明为extern "C"。通常可以在C的头文件里这样定义：
 #ifdef __cplusplus extern "C" { #endif ... #ifdef __cplusplus } #endif就可兼容C和C++程序。看来C的好多东西还没有掌握，尤其是跟编译器相关的部分，需要全面的补一下课。平时遇到不清楚的问题一定要当时就搞明白，不能放过。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/nataka/archive/2005/09/17/482671.aspx](http://blog.csdn.net/nataka/archive/2005/09/17/482671.aspx)

