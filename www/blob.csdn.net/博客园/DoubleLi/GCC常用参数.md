# GCC常用参数 - DoubleLi - 博客园






GCC--GNU C Compiler c语言编译器（远不止c语言）

介绍：

作为自由软件的旗舰项目，Richard Stallman 在十多年前刚开始写作 GCC 的时候，还只是把它当作仅仅一个C 程序语言的编译器；GCC 的意思也只是 GNU C Compiler 而已。经过了这么多年的发展，GCC 已经不仅仅能支持 C 语言；它现在还支持 Ada 语言，C++ 语言，Java 语言，Objective C 语言，Pascal 语言，COBOL 语言，以及支持函数式编程和逻辑编程的 Mercury 语言，等等。而 GCC 也不再单只是 GNU C 语言编译器的意思了，而是变成了 GNU Compiler Collection 也即是 GNU 编译器家族的意思了 
另一方面，说到 GCC 对于各种硬件平台的支持，概括起来就是一句话：无所不在。几乎所有有点实际用途的硬件平台，甚至包括有些不那么有实际用途的硬件平台，比如 Don Knuth 设计的 MMIX 计算机，GCC 都提供了完善的支持。 
如果把我们自己的程序语言的实现建立于 GCC 之上，也立刻使得我们的程序语言的实现版本可以运行在几乎所有有用的硬件平台之上。这对于程序语言的作者来说，也是一个确实的有极大诱惑力的好处。



GCC常用选项

选项 
含义

--help 
--target-help 
显示 gcc 帮助说明。‘target-help’是显示目标机器特定的命令行选项。

--version 
显示 gcc 版本号和版权信息 。

**-o **outfile 
输出到指定的文件。

-x language 
指明使用的编程语言。允许的语言包括：c c++ assembler none 。 ‘none’意味着恢复默认行为，即根据文件的扩展名猜测源文件的语言。

-v 
打印较多信息，显示编译器调用的程序。

-### 
与 -v 类似，但选项被引号括住，并且不执行命令。

**-E **
仅作预处理（code.i），不进行编译、汇编和链接。

**-S **
仅编译到汇编语言(code.s)，不进行汇编和链接。

**-c **
编译、汇编到目标代码(code.o)，不进行链接。

-pipe 
使用管道代替临时文件。

-combine 
将多个源文件一次性传递给汇编器。




其他GCC选项

更多有用的GCC选项：

命令 
描述
-shared 

生成共享目标文件。通常用在建立共享库时。
-static

禁止使用共享连接。
-l library 
**-llibrary **
进行链接时搜索名为library的库。 
例子： $ gcc test.c -lm -o test

**-Idir**
把dir 加入到搜索头文件的路径列表中。 
例子： $ gcc test.c -I../inc_dir -o test

**-Ldir **
把dir 加入到搜索库文件的路径列表中。 
例子： $ gcc -I/home/foo -L/home/foo -ltest test.c -o test

-Dname 
预定义一个名为name 的宏，值为1。 
例子： $ gcc -DTEST_CONFIG test.c -o test

-Dname =definition 
预定义名为name ，值为definition 的宏。

-ggdb 
-ggdblevel 
为调试器 gdb 生成调试信息。level 可以为1，2，3，默认值为2。

**-g **
-glevel 
生成操作系统本地格式的调试信息。-g 和 -ggdb 并不太相同， -g 会生成 gdb 之外的信息。level 取值同上。
**-Wall **
会打开一些很有用的警告选项，建议编译时加此选项。


-w 
禁止显示所有警告信息。




Optimization

**-O0 **
禁止编译器进行优化。默认为此项。

-O 
-O1 
尝试优化编译时间和可执行文件大小。

**-O2 **
更多的优化，会尝试几乎全部的优化功能，但不会进行“空间换时间”的优化方法。

-O3 
在 -O2 的基础上再打开一些优化选项：-finline-functions， -funswitch-loops 和 -fgcse-after-reload 。

-Os 

对生成文件大小进行优化。它会打开 -O2 开的全部选项，除了会那些增加文件大小的。




Standard


-ansi 
支持符合ANSI标准的C程序。这样就会关闭GNU C中某些不兼容ANSI C的特性。

-std=c89 
-iso9899:1990 
指明使用标准 ISO C90 作为标准来编译程序。

-std=c99 
-std=iso9899:1999 
指明使用标准 ISO C99 作为标准来编译程序。










