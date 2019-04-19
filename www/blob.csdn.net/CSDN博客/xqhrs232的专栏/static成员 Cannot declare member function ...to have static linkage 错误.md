# static成员"Cannot declare member function ...to have static linkage"错误 - xqhrs232的专栏 - CSDN博客
2018年08月20日 15:56:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：70
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/zhangxiao93/article/details/51344195](https://blog.csdn.net/zhangxiao93/article/details/51344195)
**（一）问题描述**
.h文件中声明类中静态成员函数，在.cpp文件中定义，编译出现"Cannot declare member function ...to have static linkage"错误。
**（二）环境：**
ubuntu 14.04
g++ 4.8.4
代码如下：
**test.h**
- 
`#ifndef __TEST_H__`
- 
`#define __TEST_H__`
- 
`class test`
- 
`{`
- 
`public:`
- 
`test(){}`
- 
`~test(){}`
- 
- 
`static void func();`
- 
`};`
- 
`#endif`
**test.cpp**
- 
`#include "test.h"`
- 
- 
`#include <iostream>`
- 
`static void test::func()`
- 
`{`
- 
`std::cout<<"func"<<std::endl;`
- 
`}`
在终端执行
`g++ -c test.cpp`
出现错误：
"Cannot declare member function ...to have static linkage"
**（三）解决方案**
在.cpp文件中去掉static关键字
原因参考：http://blog.csdn.net/guoxiaojie_415/article/details/5364354
简单翻译一下是因为，static的用法有好几种，在类中成员函数的声明使用static关键字则是规定说该成员函数为该类所有实例所共享也就是所谓的"one-per-class"，而在.cpp文件中使用static关键字的作用也是说该函数或变量仅仅只有该文件可以访问，这就与第一个用法冲突了。不过在这种情况时，定义static的类成员变量不需要再使用static关键字了，或者直接在.h文件中声明+定义
