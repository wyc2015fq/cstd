# G++不同版本的C/C++软件移植指南 - 三少GG - CSDN博客
2014年01月04日 01:58:27[三少GG](https://me.csdn.net/scut1135)阅读数：2198
1.
**strchr   ** error: invalid conversion from ‘const char*’ to ‘char’ [-fpermissive]
# [针对GCC 4.4版本的C/C++软件移植指南](http://guiquanz.github.io/2012/09/21/porting-to-gcc-4.4)Sep 21, 2012
本文是Porting to GCC系列的第二篇。主要介绍GCC 4.4版本的特性及C/C++软件移植相关的问题。
## 扩展阅读
如果你希望更加深入的了解GCC相关特性、体系及编程等，可参考以下的材料：
- 各类官方文档
- The Definitive Guide to GCC（William von Hagen著）：全面介绍GCC相关内容，包括auto*工具链使用及语言特性的扩展等。
- Unix to Linux Porting (Alfredo mendoza等著)：这是*nix软件移植开发最好的指南。
## GCC 4.4版本的变化
### C语言相关问题
#### 预处理器条件都被执行
当用`#elif`时候，现在其参数将被执行，即使前面的`#if`或`#elif`条件执行结果为非0(onoe-zero）。这样做的目的是确保，它们（宏）是有效的常量表达式。示例代码，如下：
- #if 1
- #elif
- #endif
在新版本下，会产生以下错误诊断：
error:#elifwithno expression
修正手段：要么使用不带参数`#else`，要么为`#elif`提供常量表达式。
### 更加严格的“别名”要求
当优化代码时，GCC会对很多“类型双关type-punning”情况，发出告警。例如，
- struct A 
- {
- char data[14];
- int i;
- };
- 
- void foo()
- {
- char buf[sizeof(struct A)];
- ((struct A*)buf)->i =4;
- }
在新版本下，会产生以下错误诊断：
- warning: dereferencing type-punned pointer will break strict-aliasing rules
可以使用`-fno-strict-aliasing`为变通方案或使用`-Wno-strict-aliasing`忽略此类问题。要修正问题，可以通过`相等类型的指针`访问结构体，用一个union，用memcpy或（如果是C++）用`new`操作替换。
### C++语言相关问题
#### 头文件依赖调整
针对头文件依赖，一些标准C++库include文件已被优化，仅include最小可能数量的附加文件。因此，使用了`std::printf`而没有include <cstdio> 的C++程序，或使用了`uint32_t`而没有include <stdint.h> 的，将不再编译，而是报错。
具体细节，如下:
文件<cstdio>将不再作为<string>,<ios>,<iomanip>,<streambuf>,或<locale>的一部分被include。
文件<stdint.h>将不再作为<string>或<ios>的一部分被include.
#### 限制null终止（null-terminated）序列的函数
一些C++库include已被修改，来替换C库函数的重载，以提高常量的正确性（const-correctness）：接收const char*，同时返回const char*的函数。相关变更，如下表所示：
|头文件|函数|
|----|----|
|<cstring>|`strchr`, `strpbrk`, `strrchr`, `strstr`,`memchr`|
|<cwchar>|`wcschr``wcspbrk`, `wcsrchr`, `wcsstr`,`wmemchr`|
应用示例，如下：
- #include<cstring>
- 
- constchar* str1;
- char* str2 = strchr(str1,'a');
编译时，会报如下错误：
error: invalid conversion from‘constchar*’ to ‘char*’
修正方式，如下：
- **#include<cstring>**
- 
- constchar* str1;
- `**const char* str2**`= strchr(str1,'a');
#### 初始化调整
GCC 4.4默认不再支持如下的代码：
- struct A {virtual~A ();};
- 
- struct B :public A {int i;};
- 
- struct C
- {
- const B a;
-   C(){ bar(&a);}
- void bar(const B*);
- };
编译这个代码会报以下错误：
In constructor 'C::C()':
error: uninitialized member 'C::a'with'const' type 'const B'
用`成员初始化列表`初始化成员即可解决相关问题。具体，如下：
- C(): a(B()){ bar(&a);}
2.
# [Compiler error: memset was not declared in this scope](http://stackoverflow.com/questions/2505365/compiler-error-memset-was-not-declared-in-this-scope)
You should** include `<string.h>`** (or its C++ equivalent,
`<cstring>`).
|50down vote|Whevever you get a problem like this just go to the [man page for the function](http://linux.die.net/man/3/memset) in question and it will tell you what header you are missing, e.g.```$ man memsetMEMSET(3)                BSD Library Functions Manual                MEMSET(3)NAME     memset -- fill a byte string with a byte valueLIBRARY     Standard C Library (libc, -lc)SYNOPSIS     #include <string.h>     void *     memset(void *b, int c, size_t len);```|
3. ‘int32_t’ does not name a type
||```[Add missing**<stdint.h>** include](https://github.com/rlsosborne/tool_axe/commit/94a5d60cdecefb57cff35beb0f2b294038f4d5bb)```|
4.  ‘bzero’ was not declared in this scope
     ‘strncpy’ was not declared in this scope
**#include <string.h>**
