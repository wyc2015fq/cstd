# AOT JIT and Interpretation - 电影旅行敲代码 - CSDN博客
2016年04月05日 20:54:22[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：697
在接触虚拟机的时候，首先遇到的三个概念就是**AOT**、**JIT** 和 **Interpretation**，那么这三个概念有什么区别呢？
## AOT
Ahead-of-time(AOT) 是一种编译方式，现在常见的高级语言都会采用这种方式，例如C/C++代码以及Java中间代码，它们都可以在程序执行前编译成可执行完文件。这里的 **time** 就是 **runtime**，例如编译C/C++代码到**ELF**或者**PE**格式的可执行文件然后运行。另外Java也可以采用AOT编译，例如在软件安装时编译成ELF的可执行文件，然后再执行时效率就会比纯解释的高很多。
## JIT
Just-in-time(JIT)编译，也是一种编译技术，这种技术是在程序运行的时候编译成可执行文件，然后运行该文件。这里的 **time** 表示的也是 **runtime** 。注意这个和AOT的区别是JIT可以接触到运行时的信息，而AOT接触不到运行时的，编译完之后基本上不会再被改变了。
## Interpretation
**Interpretation**和上面的两种方式都不同，AOT和JIT还属于编译的范畴，终归会生成可执行文件，而**Interpretation**直接解释执行，一条条取出经过前端编译后的字节码，然后解释执行，基于栈或者基于寄存器。很多脚本语言都是解释执行的方式，例如Lua, PHP, Perl, JS等等。
**Interpretation** 也有很多种实现方式。
- Bytecode interpreters
- Abstract Syntax Tree interpreters
- Just-in-time compilation
## AOT vs JIT vs Interpretation
AOT对编译时间不是很敏感，可以实行高效的优化算法，得到执行效率或者code size更优的代码。但是JIT可以接触到运行时的信息，所以可以根据特定平台代码的运行信息，二次编译得到更优的代码，或者是多次编译。
- **AOT compilers can spend more time doing advanced optimizations.**
- **JIT has access to run-time information that can aid in optimization. This includes data about the machine it’s executing on, enabling platform-specific native optimization.**
可以看到AOT和JIT各有神通，都可以根据自己的方式生成更优的代码。但是两者同样有自己的不足。
AOT编译得到的代码，虽然较为高效，但肯定不是最高效的代码，毕竟AOT没有考虑运行时的信息。但是JIT在运行时执行再优化，肯定会引入将字节码编译成二进制的开销，这肯定会大大影响程序启动时间，在实时应用中，肯定更加无法容忍。另外如果运行时收集到的信息没有可以参考的价值，那么JIT就没有什么存在价值了。
- 
JIT causes a slight delay to a noticeable delay in initial execution of an application, due to the time taken to load and compile the bytecode. Somttimes this delay is called “startup time delay”. Ingeneral, the more optimization JIT performs, the better the code it will generate, **but the initial delay will also increase**. A JIT compiler therefore has to make a trade-off between the compilation time and the quality of the code it hopes to generate.
- 
**AOT compilers can perform complex and advanced code optimizations which in most cases of JITling will be considered much too costly.**
- 
AOT usually cannot perform some optimizations possible in JIT, like runtime profiled-guided optimizations, pseudo-constant propagation or indirect/virtual function inlining.
而其中**Interpretation**中规中矩，就是一种范畴很广的代码编译执行方式。
