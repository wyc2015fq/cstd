# __attribute__((visibility(""))) - Koma Hub - CSDN博客
2019年03月30日 20:17:07[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：141
个人分类：[C/C++																[GCC](https://blog.csdn.net/Rong_Toa/article/category/8661289)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
**Table of Contents**
[__attribute__((visibility("default")))](#__attribute__((visibility(%22default%22))))
[1. 当-fvisibility=hidden时](#1.%20%E5%BD%93-fvisibility%3Dhidden%E6%97%B6)
[2. 当-fvisibility=default时](#2.%20%E5%BD%93-fvisibility%3Ddefault%E6%97%B6)
```cpp
// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define SRSLTE_IMPORT __declspec(dllimport)
  #define SRSLTE_EXPORT __declspec(dllexport)
  #define SRSLTE_LOCAL
#else
  #if __GNUC__ >= 4
    #define SRSLTE_IMPORT __attribute__ ((visibility ("default")))
    #define SRSLTE_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define SRSLTE_IMPORT
    #define SRSLTE_EXPORT
    #define SRSLTE_LOCAL
  #endif
#endif
```
# __attribute__((visibility("default")))
可以看到OBJC_VISIBLE 的定义方式是 __attribute__((visibility("default"))).
那么究竟有什么作用呢? 下面举例说明.
GCC 有个visibility属性, 该属性是说, 启用这个属性:
### 1. 当-fvisibility=hidden时
动态库中的函数默认是被隐藏的即 hidden. 除非显示声明为__attribute__((visibility("default"))).
### 2. 当-fvisibility=default时
 动态库中的函数默认是可见的.除非显示声明为__attribute__((visibility("hidden"))).
特别说明: 这个特性是 GCC4.0 以后才有的.
基于GCC系列: 加载静态链接库的例子, 谈谈 visibility.
**Car.c**
```cpp
#include<stdio.h>  
#include<stdlib.h> 
 
__attribute ((visibility("default"))) int drive()
{
    printf("Car driving...\n");
 
    return 0;
}
 
void stop()
{
    printf("Car stop.\n");
}
```
注意: attribute 的定义和使用方法.
使用 visibility 属性来编译动态链接库.
g++-4.9 -shared -o libCar.so -fvisibility=hidden Car.c
这样一来, drive 方法是可见的, 但是 stop 是不可见的.
> 
作者：veryitman 
来源：CSDN 
原文：https://blog.csdn.net/veryitman/article/details/46756683 
版权声明：本文为博主原创文章，转载请附上博文链接！
在Linux下动态库(.so)中，通过GCC的C++ visibility属性可以控制共享文件导出符号。在GCC 4.0及以上版本中，有个visibility属性，可见属性可以应用到函数、变量、模板以及C++类。
限制符号可见性的原因：从动态库中尽可能少地输出符号是一个好的实践经验。输出一个受限制的符号会提高程序的模块性，并隐藏实现的细节。动态库装载和识别的符号越少，程序启动和运行的速度就越快。导出所有符号会减慢程序速度，并耗用大量内存。
“default”：用它定义的符号将被导出，动态库中的函数默认是可见的。”hidden”：用它定义的符号将不被导出，并且不能从其它对象进行使用，动态库中的函数默认是被隐藏的。default意味着该方法对其它模块是可见的。而hidden表面该方法符号不会被放到动态符号表里，所以其它模块(可执行文件或者动态库)不可以通过符号表访问该方法。
要定义GNU属性，需要包含__attribute__和用括号括住的内容。可以将符号的可见性指定为visibility(“hidden”)，这将不允许它们在库中被导出，但是可以在源文件之间共享。实际上，隐藏的符号将不会出现在动态符号表中，但是还被留在符号表中用于静态链接。
导出列表由编译器在创建共享库的时候自动生成，也可以由开发人员手工编写。导出列表的原理是显式地告诉编译器可以通过外部文件从对象文件导出的符号是哪些。GNU用户将此类外部文件称作为”导出映射”。
> 
作者：fengbingchun 
来源：CSDN 
原文：https://blog.csdn.net/fengbingchun/article/details/78898623 
版权声明：本文为博主原创文章，转载请附上博文链接！
