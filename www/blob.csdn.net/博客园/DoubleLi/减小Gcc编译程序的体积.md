# 减小Gcc编译程序的体积 - DoubleLi - 博客园






> 
众所周知，Gcc编译的原始程序一般很大，其实有几种方法能大大减小目标代码的体积，一般有以下几种方法。

     基本知识来源：[http://www.mingw.org/wiki/Large_executables](http://www.mingw.org/wiki/Large_executables)



1、禁用调试信息

     Release编译时不要加上-g开关。

2、使用-Os编译程序。

    不要使用-funroll-loops等可以加速程序执行但是会大大增加目标代码体积的开关。


> 
3、编译后的程序使用strip去除符号和偏移（限于可执行程序和共享库，其他易出问题）。

4、如果你不需要RTTI，编译时加入-fno-rtti。

5、如果你不需要处理C++异常，编译时加入-fno-exceptions。

6、使用UPX之类的可执行程序压缩程序（只推荐用于可执行程序，用于其他也可，但是较浪费内存）。


> 
     例如，使用wxWidgets动态编译的程序初始体积可能有429KB，经过以上方式（未经UPX压缩）处理后，体积减少为108KB，UPX压缩后体积为35KB。










