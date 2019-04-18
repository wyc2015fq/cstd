# c/c++ main函数执行之前/后 - xcw0754 - 博客园
# [c/c++ main函数执行之前/后](https://www.cnblogs.com/xcw0754/p/4942441.html)
转载自：http://bbs.csdn.net/topics/300103318#r_78088969
main函数之前--真正的函数执行入口或开始
一种解释：
实际上，在可执行文件被加载之后，控制权立即交给由编译器插入的Start函数，它将对后面这些全局变量进行准备：
　　　_osver 操作系统的构件编号
      _winmajor　操作系统的主版本号
      _winminor　操作系统的次版本号
      _winver　操作系统完全版本号
      __argc　命令行参数个数
　     __argv　指向参数字符串的指针数组
      _environ　指向环境变量字符串的指针数组
Start函数初始化堆并调用main函数．mian函数返回之后,Start函数调用Exit函数结束该进程．
启动函数Start的源代码在：
　　　crt0.c Microsoft Visual C++
      c0w.asm Borladn C++
另一种解释
Some of the stuff that has to happen before main(): 
set up initial stack pointer 
initialize static and global data 
zero out uninitialized data 
run global constructors
Some of this comes with the runtime library's crt0.o file or its __start() function. Some of it you need to do yourself.
Crt0 is a synonym for the C runtime library. 
Depending on the system you're using the follwing may be incomplete, but it should give you an idea. Using newlib-1.9.0/libgloss/m68k/crt0.S as an outline, the steps are: 
1. Set stack pointer to value of __STACK if set 
2. Set the initial value of the frame pointer 
3. Clear .bss (where all the values that start at zero go) 
4. Call indirect of hardware_init_hook if set to initialize hardware 
5. Call indirect of software_init_hook if set to initialize software 
6. Add __do_global_dtors and __FINI_SECTION__ to the atexit function so destructors and other cleanup functions are called when the program exits by either returning from main, or calling exit 
7. setup the paramters for argc, argv, argp and call main 
8. call exit if main returns
第三种解释：囫囵C语言（三）：谁调用了我的 main？
　　　　现在最重要的是要跟得上潮流，所以套用比较时髦的话，谁动了我的奶酪。谁调用了我的 main？不过作为计算机工作者，我劝大家还是不要赶时髦，今天Java热，明天 .net 流行，什么时髦就学什么。我的意思是先花几年把基本功学好，等你赶时髦的时候也好事半功倍。废话不多说了。
　　　　我们都听说过一句话：“main是C语言的入口”。我至今不明白为什么这么说。就好像如果有人说：“挣钱是泡妞”，肯定无数砖头拍过来。这句话应该是“挣钱是泡妞的一个条件，只不过这个条件特别重要”。那么上面那句话应该是 “main是C语言中一个符号，只不过这个符号比较特别。”
　　　　我们看下面的例子：
　　　　/* file name test00.c */
　　　　int main(int argc, char* argv)
　　　　{
　　　　 return 0;
　　　　}
　　　　编译链接它：
　　　　cc test00.c -o test.exe
　　　　会生成 test.exe
　　　　但是我们加上这个选项： -nostdlib (不链接标准库)
　　　　cc test00.c -nostdlib -o test.exe
　　　　链接器会报错：
　　　　undefined symbol: __start
　　　　也就是说：
　　　　1. 编译器缺省是找 __start 符号，而不是 main
　　　　2. __start 这个符号是程序的起始点
　　　　3. main 是被标准库调用的一个符号
　　　　再来思考一个问题：
　　　　我们写程序，比如一个模块，通常要有 initialize 和 de-initialize，但是我们写 C 程序的时候为什么有些模块没有这两个过程么呢？比如我们程序从 main 开始就可以 malloc，free，但是我们在 main 里面却没有初始化堆。再比如在 main 里面可以直接 printf，可是我们并没有打开标准输出文件啊。(不知道什么是 stdin，stdout，stderr 以及 printf 和 stdout 关系的群众请先看看 C 语言中文件的概念)。
　　　　有人说，这些东西不需要初始化。如果您真得这么想，请您不要再往下看了，我个人认为计算机软件不适合您。
　　　　聪明的人民群众会想，一定是在 main 之前干了些什么。使这些函数可以直接调用而不用初始化。通常，我们会在编译器的环境中找到一个名字类似于 crt0.o 的文件，这个文件中包含了我们刚才所说的 __start 符号。（crt 大概是 C Runtime 的缩写，请大家帮助确认一下。）
　　　　那么真正的 crt0.s 是什么样子呢？下面我们给出部分伪代码：
　　　　///////////////////////////////////////////////////////
　　　　section .text:
　　　　__start:
　　　　 :
　　　　 init stack;
　　　　 init heap;
　　　　 open stdin;
　　　　 open stdout;
　　　　 open stderr;
　　　　 :
　　　　 push argv;
　　　　 push argc;
　　　　 call _main; (调用 main)
　　　　 :
　　　　 destory heap;
　　　　 close stdin;
　　　　 close stdout;
　　　　 close stderr;
　　　　 :
　　　　 call __exit;
　　　　////////////////////////////////////////////////////
　　　　实际上可能还有很多初始化工作，因为都是和操作系统相关的，笔者就不一一列出了。
　　　　注意：
　　　　1. 不同的编译器，不一定缺省得符号都是 __start。
　　　　2. 汇编里面的 _main 就是 C 语言里面的 main，是因为汇编器和C编译器对符号的命名有差异（通常是差一个下划线'_'）。
　　　　3. 目前操作系统结构有两个主要的分支：微内核和宏内核。微内核的优点是，结构清晰，简单，内核组件较少，便于维护；缺点是，进程间通信较多，程序频繁进出内核，效率较低。宏内核正好相反。我说这个是什么目的是：没办法保证每个组件都在用户空间（标准库函数）中初始化，有些组件确实可能不要初始化，操作系统在创建进程的时候在内核空间做的。这依赖于操作系统的具体实现，比如堆，宏内核结构可能在内核初始化，微内核结构在用户空间；即使同样是微内核，这个东东也可能会被拿到内核空间初始化。
　　　　随着 CPU 技术的发展，存储量的迅速扩展，代码复杂程度的增加，微内核被越来越多的采用。你会为了 10% 的效率使代码复杂度增加么？要知道每隔 18 个月 CPU 的速度就会翻一番。所以我对程序员的要求是，我首先不要你的代码效率高，我首先要你的代码能让 80% 的人迅速看懂并可以维护。
总结：
main函数执行之前，主要就是初始化系统相关资源：
1.设置栈指针
2.初始化static静态和global全局变量，即data段的内容
3.将未初始化部分的赋初值：数值型short，int，long等为0，bool为FALSE，指针为NULL，等等，即.bss段的内容
4.运行全局构造器，估计是C++中构造函数之类的吧
5.将main函数的参数，argc，argv等传递给main函数，然后才真正运行main函数

