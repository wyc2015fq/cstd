# C 程序编译过程 - youbingchen的博客 - CSDN博客





2016年07月21日 14:27:26[youbingchen](https://me.csdn.net/youbingchen)阅读数：495








> 
通常我们使用gcc来生成可执行程序，命令为: gcc hello.c,默认生成可执行文件a.out,其中经过下面几个步骤：

- 预处理（processing）
- 编译（compilation）
- 汇编 (Assembly)
- 链接（Linking)

![这里写图片描述](https://img-blog.csdn.net/20160721142645930)
![这里写图片描述](https://img-blog.csdn.net/20160721142658240)
> 
接下来将介绍每个步骤相应都做了那些工作


# 预处理（preprocessing）
- 将所有的#define删除，并且展开所有的宏定义
- 处理所有的条件编译指令，比如#if #ifdef #elif #else #endif等
- 处理#include预编译指令，并包含的文件插入到该编译器指令等 
- 删除所有注释“//”和“/*”,”*/”
- 添加行号和文件标识，以便编译器产生调试用的行号以及编译错误警告行号
- 保留所有的#pragma编译指令，因为编译器使用它。
`gcc -E hello.c -o hello.i`
# 编译（compilation)

编译过程就是把预处理完的文件进行一系列的词法分析，语法分析，语义分析及优化后生成相应的汇编代码。
`gcc –S hello.i –o hello.s`
> 
注：现在版本的GCC把预处理和编译两个步骤合成一个步骤，用cc1工具来完成。gcc其实是后台程序的一些包装，根据不同参数去调用其他的实际处理程序，比如：预编译编译程序cc1、汇编器as、连接器ld。


# 汇编(Assembly)

汇编器是将汇编代码转变成机器可以执行的命令，每一个汇编语句几乎都对应一条机器指令。汇编相对于编译过程比较简单，根据汇编指令和机器指令的对照表一一翻译即可。

# 链接(Linking)

通过调用链接器ld来链接程序运行需要的一大堆目标文件，以及所依赖的其它库文件，最后生成可执行文件。 

ld -static crt1.o crti.o crtbeginT.o hello.o -start-group -lgcc -lgcc_eh -lc-end-group crtend.o crtn.o (省略了文件的路径名)。

# 编译和链接详解

> 
编译过程可分为6步：扫描（词法分析）、语法分析、语义分析、源代码优化、代码生成、目标代码优化。


词法分析：扫描器（Scanner）将源代的字符序列分割成一系列的记号（Token）。lex工具可实现词法扫描。 

语法分析：语法分析器将记号（Token）产生语法树（Syntax Tree）。yacc工具可实现语法分析(yacc: Yet Another Compiler Compiler)。 

语义分析：静态语义（在编译器可以确定的语义）、动态语义（只能在运行期才能确定的语义）。 

源代码优化：源代码优化器(Source Code Optimizer)，将整个语法书转化为中间代码（Intermediate Code）（中间代码是与目标机器和运行环境无关的）。中间代码使得编译器被分为前端和后端。编译器前端负责产生机器无关的中间代码；编译器后端将中间代码转化为目标机器代码。 

目标代码生成：代码生成器(Code Generator). 

目标代码优化：目标代码优化器(Target Code Optimizer)。
链接的主要内容是把各个模块之间相互引用的部分处理好，使得各个模块之间能够正确地衔接。链接的主要过程包括：地址和空间分配（Address and Storage Allocation），符号决议（Symbol Resolution），重定位（Relocation）等。链接分为静态链接和动态链接。态链接是指在编译阶段直接把静态库加入到可执行文件中去，这样可执行文件会比较大。而动态链接则是指链接阶段仅仅只加入一些描述信息，而程序执行时再从系统中把相应动态库加载到内存中去。 

关于 静态链接和动态链接之间的差别可以看这篇博客[静态库和动态库](http://my.oschina.net/youbingchen/blog/715550)





