# C++条件编译详解 - fanyun的博客 - CSDN博客
2018年11月28日 21:26:57[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：80
        一般情况下，在进行编译时对源程序中的每一行都要编译。但是有时希望程序中某一部分内容只在满足一定条件时才进行编译，也就是指定对程序中的一部分内容进行编译的条件。如果不满足这个条件，就不编译这部分内容。这就是“条件编译”。 其实这跟事物具有多样性一样。我们需要对不同的状况下采取不同的操作。例如程序的运行平台具有这种多样性，我们在window平台下编写的程序，可能使用某一个库或者与硬件相关的属性、方法。现在要将我们的程序移植到别的计算机系统的时候，例如linux系统。那么程序上依赖的库或者有些和硬件相关联的属性和方法不得不更改，那么我们只能在编写程序的时候提高程序的健壮性，此时就需要条件编译语句为我们实现这样的功能。
常见的预编译和条件编译指令：
#define            定义一个预处理宏
#undef            取消宏的定义
#if                   编译预处理中的条件命令，相当于C语法中的if语句
#ifdef              判断某个宏是否被定义，若已定义，执行随后的语句
#ifndef            与#ifdef相反，判断某个宏是否未被定义
#elif                若#if, #ifdef, #ifndef或前面的#elif条件不满足，则执行#elif之后的语句，相当于C语法中的else-if
#else              与#if, #ifdef, #ifndef对应, 若这些条件不满足，则执行#else之后的语句，相当于C语法中的else
#endif             #if, #ifdef, #ifndef这些条件命令的结束标志.
defined         　与#if, #elif配合使用，判断某个宏是否被定义
          有时，希望当满足某条件时对一组语句进行编译，而当条件不满足时则编译另一组语句。
条件编译命令常用的有以下形式：
1) #ifdef 标识符
    程序段1
#else
    程序段2
#endif
          它的作用是当所指定的标识符已经被#define命令定义过，则在程序编译阶段只编译程序段1，否则编译程序段2。#endif用来限定#ifdef命令的范围。其中#else部分也可以没有。
2) #if 表达式
    程序段1
#else
    程序段2
#endif
         它的作用是当指定的表达式值为真（非零）时就编译程序段1，否则编译程序段2。可以事先给定一定条件，使程序在不同的条件下执行不同的功能。
条件编译是根据实际定义宏（某类条件）进行代码静态编译的手段。可根据表达式的值或某个特定宏是否被定义来确定编译条件。
最常见的条件编译是防止重复包含头文件的宏，形式跟下面代码类似：
```cpp
#ifndef FA_H
#define FA_H
// ... some declaration codes
#endif // #ifndef FA_H
```
```cpp
#ifdef _DEBUG
 
 // ... do some operations
 
 #endif
 
 #ifdef _WIN32
 
 // ... use  Win32 API
 #endif
```
在调试程序时，常常希望输出一些所需的信息，而在调试完成后不再输出这些信息，可以在源程序中插入条件编译段。下面是一个简单的示例。   
```cpp
#include <iostream>
using namespace std;
#define OUTPUTLOG //在调试程序时使之成为注释行
int main( )
{
  int x=1, y=2, z=3;
  #ifndef OUTPUTLOG //本行为条件编译命令
  cout<<"x="<<x<<", y="<<y<<", z="<<z<<"\n"; //在调试程序时需要输出这些信息
  #endif //本行为条件编译命令
  cout<<"x*y*z="<<x*y*z<<endl;
}
```
      第3行用#define命令的目的不在于用OUTPUTLOG代表一个字符串，而只是表示已定义过OUTPUTLOG，因此OUTPUTLOG后面写什么字符串都无所谓，甚至可以不写字符串。在调试程序时去掉第3行（或在行首加//，使之成为注释行），由于无此行，故未对OUTPUTLOG定义，第6行据此决定编译第7行，运行时输出x，y，z的值，以便用户分析有关变量当前的值。运行程序输出：
```cpp
x=1, y=2, z=3
x*y*z=6
```
在调试完成后，在运行之前，加上第3行，重新编译，由于此时OUTPUTLOG已被定义过，则该cout语句不被编译，因此在运行时不再输出x，y，z的值。运行情况为：
```cpp
x*y*z=6
```
### 1. #define、#undef
#define命令定义一个宏:
#define MACRO_NAME[(args)] [tokens[(opt)]]
之后出现的MACRO_NAME将被替代为所定义的标记(tokens)。宏可带参数，而后面的标记也是可选的。
宏定义，按照是否带参数通常分为对象宏、函数宏两种。
对象宏: 不带参数的宏被称为"对象宏(objectlike macro)"。对象宏多用于定义常量、通用标识。例如：
```cpp
// 常量定义
#define MAX_LENGTH 100
// 通用标识，日志输出宏
#define SLog printf
// 预编译宏
#define _DEBUG
```
函数宏：带参数的宏。利用宏可以提高代码的运行效率: 子程序的调用需要压栈出栈, 这一过程如果过于频繁会耗费掉大量的CPU运算资源。 所以一些代码量小但运行频繁的代码如果采用带参数宏来实现会提高代码的运行效率。但多数c++程序不推荐使用函数宏，调试上有一定难度，可考虑使用c++的inline代替之。例如：
```cpp
// 最小值函数
#define MIN(a,b) ((a)>(b)? (a):(b))
// 安全释放内存函数
#define SAFE_DELETE(p) {if(NULL!=p){delete p; p = NULL;}}
```
#undef可以取消宏定义，与#define对应。
### 2. defined
defined用来测试某个宏是否被定义。defined(name): 若宏被定义，则返回1，否则返回0。
它与#if、#elif、#else结合使用来判断宏是否被定义，乍一看好像它显得多余, 因为已经有了#ifdef和#ifndef。defined可用于在一条判断语句中声明多个判别条件；#ifdef和#ifndef则仅支持判断一个宏是否定义。
`#if defined(VAX) && defined(UNIX) && !defined(DEBUG) `
和#if、#elif、#else不同，#ifdef、#ifndef、defined测试的宏可以是对象宏，也可以是函数宏。
### 3. #ifdef、#ifndef、#else、#endif
条件编译中相对常用的预编译指令。模式如下：
```cpp
#ifdef ABC
// ... codes while definded ABC
#elif (CODE_VERSION > 2)
// ... codes while CODE_VERSION > 2
#else
// ... remained cases
#endif // #ifdef ABC
```
#ifdef用于判断某个宏是否定义，和#ifndef功能正好相反，二者仅支持判断单个宏是否已经定义，上面例子中二者可以互换。如果不需要多条件预编译的话，上面例子中的#elif和#else均可以不写。
### 4. #if、#elif、#else、#endif
#if可支持同时判断多个宏的存在，与常量表达式配合使用。常用格式如下：
```cpp
#if 常量表达式1
// ... some codes
#elif 常量表达式2
// ... other codes
#elif 常量表达式3
// ...
...
#else
// ... statement
#endif
```
常量表达式可以是包含宏、算术运算、逻辑运算等等的合法C常量表达式，如果常量表达式为一个未定义的宏, 那么它的值被视为0。
```cpp
#if MACRO_NON_DEFINED // 等价于 #if 0
```
在判断某个宏是否被定义时，应当避免使用#if，因为该宏的值可能就是被定义为0。而应当使用#ifdef或#ifndef。
注意: #if、#elif之后的宏只能是对象宏。如果宏未定义，或者该宏是函数宏，则编译器可能会有对应宏未定义的警告。
