# 解决VC2015包含stdint.h头文件冲突问题 - 深之JohnChen的专栏 - CSDN博客

2018年03月30日 09:03:44[byxdaz](https://me.csdn.net/byxdaz)阅读数：726


stdint.h是C99的标准，主要用于统一跨平台数据定义。
MSVC中不带有这个头文件，直到VS2010。

在之前的版本里面，我们可以：
（1）下载这个头文件
download a MS version of this header from:

http://msinttypes.googlecode.com/svn/trunk/stdint.h

A portable one can be found here:

http://www.azillionmonkeys.com/qed/pstdint.h
（2）将头文件放到（以VS2015为例）：
C:\Program Files\Microsoft Visual Studio 14.0\VC\include

出现“C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\include\stdint.h(17): error C2632: “char”后面的“char”非法”错误。stdint.h文件17行是typedef signed char        int8_t;
解决方法：
先查找工程中引用int8_t的地方，屏蔽掉这段代码，增加 #include <cstdint>

总的解决方法如下：
Features of C standard Library are also provided in the C++ Standard library and as a general naming convention they are pre-pended by an c to the corresponding names in C standard library.
In C++, You should be using: 
#include <cstdint>
and fully qualify the symbol names you use with std::
while in C, You should use:
#include <stdint.h>

https://stackoverflow.com/questions/13642827/cstdint-vs-stdint-h

