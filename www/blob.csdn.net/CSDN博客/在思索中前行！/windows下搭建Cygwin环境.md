# windows下搭建Cygwin环境 - 在思索中前行！ - CSDN博客





2014年11月03日 16:53:20[_Tham](https://me.csdn.net/txl16211)阅读数：730标签：[windows 模拟Unix																[Cygwin安装使用](https://so.csdn.net/so/search/s.do?q=Cygwin安装使用&t=blog)](https://so.csdn.net/so/search/s.do?q=windows 模拟Unix&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)










在上一篇博文《[Android学习——windows下搭建NDK_r9环境](http://www.cnblogs.com/xiaoxuetu/p/3461867.html)》中，我们详细的讲解了在windows下进行Android
 NDK开发环境的配置，我们也讲到了在NDk r7以后，我们可以不需要配置Cygwin也可以进行编译了，那么有特殊情况呢？下面就来讲解一下Cygwin这个工具吧。

## 1.Cygwin简介

Cygwin是在Windows下模拟Linux环境的工具集，它主要包含一下两个部分：

   1) cywin1.dll : 该文件模拟了真是的LinuxAPI，是一个API模拟层，开发人员可以将在Linux下编写的C/C++ 源代码在Cygwin中进行编译，在编译的过程中，如果C/C++ 调用了Linux中的API，Cygwin就会利用cygwin1.dll 来编译 C／C++源代码，从而可以在windows下生成Linux下的lib...so文件

   2) 模拟Linux环境的工具集

## 2. Cygwin 下载  

下载地址：[http://www.cygwin.com/](http://www.cygwin.com/)

配置详细见博文：[http://www.cnblogs.com/xiaoxuetu/p/3467613.html](http://www.cnblogs.com/xiaoxuetu/p/3467613.html)





