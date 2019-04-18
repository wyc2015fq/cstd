# 使用SWIG将C++绑定到python - 长歌行 - CSDN博客





2012年12月30日 17:17:18[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1064
个人分类：[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)

所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









原文地址：http://blog.csdn.net/hugewave/article/details/7705392




一、为什么要绑定

c++和python都是通用的编程语言，能完成网络、界面、数据库、计算、商务、游戏等任意的应用，但是相比较而言，python比c++更高级一些。python是个解释语言，C++语言需要编译，解决一个相同的问题，pyhon比C++节省5倍以上的代码量，有人认为一个python程序员2个月能完成的任务，两个C++程序样一年也完成不了，python还有许多比较好的特性，比如动态数据类型定义、不需要指针、模块、宏预处理、不需要明确的分配内存，所以python应用比较普遍，在大多数情况下，python还算是快的，但对于快有更高要求的情况下，c++就更能满足要求，简而言之，python比C++用起来跟方便，但是C++运行起来更快，所以需用C++编写需要大运算量的场合，然后供python调用。也就是把c++绑定到python里。


二，什么是绑定

简而言之，在一个语言里可以调用另一个语言的功能，就叫绑定。

三，什么是SWIG

SWIG就是simplified wrapper interface generator也就是简化的绑定接口生成器。

SWIG可以提供多个语言之间的绑定接口生成，但是本文只讲C++与python之间的绑定生成问题。

四、如何实现

SWIG如何安装之类的就略去了。

最好的例子就是直接上SWIG的网站

http://www.swig.org/tutorial.html



五、需要注意的几点

如下几点说明主要是针对的是使用windows，并用Visual studio进行程序开发的用户


1、根据swig的文档的说明把PYTHON_INCLUDE和PYTHON_LIB两个环境变量设置好

2、用visual studio创建一个dll的项目

3、生成.i文件并对其进行属性设置

4、对项目属性进行设置

具体可参见下面的说明：

**How to use SWIG under VS2010.**

lets define you want to obtain mylib.py, so you created some SWIG interface files with mylib.i as "main" file. I assume that you already have a solution with project with your C++ classes there.

**(1)** First create C++ project for SWIG interface. Use Visual C++->Class library project which should create a C++ DLL stub. I put all .i files there. And set visual studio to highlight .i as .h - it is handy.


**(1.1)** Add mylib_wrap.cxx file to the project (create empty file while swig hasn't generated one yet)

**(2)**

a) Press right button over the mylib.i, choose properties.

b) set ItemType as "Custom build tool".

In custom build step window:

c) Command line field should be something like: 

```
echo In order to function correctly, please ensure the following environment variables are
correctly set: 
echo PYTHON_INCLUDE: %PYTHON_INCLUDE% 
echo PYTHON_LIB: %PYTHON_LIB% 
echo on 
C:\swig\swig.exe -c++ -python %(FullPath)
```

change C:\swig\swig.exe to your path to SWIG

d) In Outputs field:

```
$(InputName)_wrap.cxx
```

![Custom build window](http://i.stack.imgur.com/TkL90.png)

**(3)** Go to this project properties: 

a) **C++ tab** -> **Additional Include Directories**

add $(PYTHON_INCLUDE); ...

c) **Linker** -> **Output File**
*Path-You-Needed*\_mylib.pyd 

d) **Linker** -> **Enable Incremental Linking**

set as No (/INCREMENTAL:NO)

e) **Linker** -> **Input** -> **Additional Dependencies**

add $(PYTHON_LIB);...

f) **C/C++** -> **Precompiled Headers**:Switch off precompiled headers, set **Not Using Precompiled Headers* and remove stdafx files after

g) **General** tab. Just check that these are set:

Configuration type = Dynamic Library (.dll)

Character set = Use Unicode Character Set

Common Language Runtime Support = No Common Language Runtime Support 

It compiles.

这个说的够详细了，参照这个来就可以搞定



