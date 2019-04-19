# Linux 平台中调试 C/C++ 内存泄漏方法 - fanyun的博客 - CSDN博客
2019年02月23日 22:09:26[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：36
       在C++中，编译器能够报出的错误一般都是简单且容易处理的错误，而内存泄露往往是我们C++程序员最头痛的错误。与许多其他类型的常见错误不同，内存错误通常具有隐蔽性，即它们很难再现，症状通常不能在相应的源代码中找到。例如，无论何时何地发生内存泄漏，都可能表现为应用程序完全无法接受，同时内存泄漏不容易被发现。存在内存错误的 C 和 C++ 程序会导致各种问题。如果它们泄漏内存，则运行速度会逐渐变慢，并最终停止运行，造成机器宕机死机；如果覆盖内存，则会变得非常脆弱，还容易受到恶意用户的攻击。
      因此，出于这些原因，需要特别关注 C 和 C++ 编程的内存问题，特别是内存泄漏。本文先从如何发现内存泄漏，然后是用不同的方法和工具定位内存泄漏，最后对这些工具进行了比较，另外还简单介绍了资源泄漏的处理，如句柄泄漏。内存泄漏一般指的是堆内存的泄漏。堆内存是指程序从堆中分配的、大小任意的（内存块的大小可以在程序运行期决定）、使用完后必须显示的释放的内存。应用程序一般使用malloc、realloc、new 等函数从堆中分配到一块内存，使用完后，程序必须负责相应的调用 free 或 delete 释放该内存块。否则，这块内存就不能被再次使用，我们就说这块内存泄漏了。
## 1. 如何发现内存泄漏
       有些简单的内存泄漏问题可以从在代码的检查阶段确定。还有些泄漏比较严重的，即在很短的时间内导致程序或系统崩溃，或者系统报告没有足够内存，也比较容易发现。最困难的就是泄漏比较缓慢，需要观测几天、几周甚至几个月才能看到明显异常现象。那么如何在比较短的时间内检测出有没有潜在的内存泄漏问题呢？实际上不同的系统都带有内存监视工具，我们可以从监视工具收集一段时间内的堆栈内存信息，观测增长趋势，来确定是否有内存泄漏。在 Linux 平台可以用 ps 命令，来监视内存的使用，比如下面的命令 ，观测指定进程的VSZ值：
       如：ps -aux命令
## 2. 静态分析
包括手动检测和静态工具分析，这是代价最小的调试方法。
2.1 手动检测
当使用 C/C++ 进行开发时，采用良好的一致的编程规范是防止内存问题第一道也是最重要的措施。检测是编码标准的补充。二者各有裨益，但结合使用效果特别好。专业的 C 或 C++ 专业人员甚至可以浏览不熟悉的源代码，并以极低的成本检测内存问题。通过少量的实践和适当的文本搜索，您能够快速验证平衡的 *alloc() 和 free() 或者 new 和 delete 的源主体。人工查看此类内容通常会出现像清单 1 中一样的问题，可以定位出在函数 LeakTest 中的堆变量 Logmsg 没有释放。
清单1. 简单的内存泄漏
```cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int LeakTest(char * Para)
{
        if(NULL==Para){
                //local_log("LeakTest Func: empty parameter\n");
                return -1;
        }
        char * Logmsg = new char[128];
        if(NULL == Logmsg){
                //local_log("memeory allocation failed\n");
                return -2;
        }
        sprintf(Logmsg,"LeakTest routine exit: '%s'.\n", Para);
        //local_log(Logmsg);
        return 0;
}
int   main(int argc,char **argv )
{
        char szInit [] = "testcase1";
        LeakTest(szInit);
        return 0;
}
```
2.2 静态代码分析工具
代码静态扫描和分析的工具比较多，比如 splint, PC-LINT, BEAM 等。因为 BEAM 支持的平台比较多，这以 BEAM 为例，做个简单介绍，其它有类似的处理过程。
BEAM 可以检测四类问题: 没有初始化的变量；废弃的空指针；内存泄漏；冗余计算。而且支持的平台比较多。
BEAM 支持以下平台：
Linux x86 (glibc 2.2.4)
Linux s390/s390x (glibc 2.3.3 or higher)
Linux (PowerPC, USS) (glibc 2.3.2 or higher)
AIX (4.3.2+)
Window2000 以上
清单2. 用作 Beam 分析的代码
```cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int *p;
void
foo(int a)
{
  int b, c;
  b = 0;
  if(!p) 
    c = 1;
  if(c > a)
    c += p[1];
}
int LeakTest(char * Para)
{
        char * Logmsg = new char[128];
        if((Para==NULL)||(Logmsg == NULL))
                return -1;        
        sprintf(Logmsg,"LeakTest routine exit: '%s'.\n", Para);        
        return 0;
}
int   main(int argc,char **argv )
{
        char szInit [] = "testcase1";
        LeakTest(szInit);
        return 0;
}
```
下面以 X86 Linux 为例,代码如清单 2，具体的环境如下:
OS: Red Hat Enterprise Linux AS release 4 (Nahant Update 2)
GCC: gcc version 3.4.4
BEAM: 3.4.2; https://w3.eda.ibm.com/beam/
可以把 BEAM 看作一个 C/C++ 编译器，按下面的命令进行编译 (前面两个命令是设置编译器环境变量)：
./beam-3.4.2/bin/beam_configure  --c gcc
./beam-3.4.2/bin/beam_configure  --cpp g++
./beam-3.4.2/bin/beam_compile  --beam::compiler=compiler_cpp_config.tcl  -cpp code2.cpp
从下面的编译报告中，我们可以看到这段程序中有三个错误:”内存泄漏”；“变量未初始化”；“ 空指针操作”
```cpp
"code2.cpp", line 10: warning: variable "b" was set but never used
    int b, c;
        ^
BEAM_VERSION=3.4.2
BEAM_ROOT=/home/hanzb/memdetect
BEAM_DIRECTORY_WRITE_INNOCENTS=
BEAM_DIRECTORY_WRITE_ERRORS=
-- ERROR23(heap_memory)     /*memory leak*/     >>>ERROR23_LeakTest_7b00071dc5cbb458
"code2.cpp", line 24: memory leak
ONE POSSIBLE PATH LEADING TO THE ERROR:
 "code2.cpp", line 22: allocating using `operator new[]' (this memory will not be freed)
 "code2.cpp", line 22: assigning into `Logmsg'
 "code2.cpp", line 24: deallocating `Logmsg' because exiting its scope 
                       (losing last pointer to the memory)
-- ERROR1     /*uninitialized*/     >>>ERROR1_foo_60c7889b2b608
"code2.cpp", line 16: uninitialized `c'
ONE POSSIBLE PATH LEADING TO THE ERROR:
 "code2.cpp", line 10: allocating `c'
 "code2.cpp", line 13: the if-condition is false
 "code2.cpp", line 16: getting the value of `c'
 VALUES AT THE END OF THE PATH:
  p != 0 
-- ERROR2     /*operating on NULL*/     >>>ERROR2_foo_af57809a2b615
"code2.cpp", line 17: invalid operation involving NULL pointer
ONE POSSIBLE PATH LEADING TO THE ERROR:
 "code2.cpp", line 13: the if-condition is true (used as evidence that error is possible)
 "code2.cpp", line 16: the if-condition is true
 "code2.cpp", line 17: invalid operation `[]' involving NULL pointer `p'
 VALUES AT THE END OF THE PATH:
  c = 1 
  p = 0 
  a <= 0
```
2.3 内嵌程序
可以重载内存分配和释放函数 new 和 delete,然后编写程序定期统计内存的分配和释放，从中找出可能的内存泄漏。或者调用系统函数定期监视程序堆的大小，关键要确定堆的增长是泄漏而不是合理的内存使用。这类方法比较复杂，在这就不给出详细例子了。
3. 动态运行检测
实时检测工具主要有 valgrind, Rational purify 等。
3.1 Valgrind
valgrind 是帮助程序员寻找程序里的 bug 和改进程序性能的工具。程序通过 valgrind 运行时，valgrind 收集各种有用的信息，通过这些信息可以找到程序中潜在的 bug 和性能瓶颈。
Valgrind 现在提供多个工具，其中最重要的是 Memcheck，Cachegrind，Massif 和 Callgrind。Valgrind 是在 Linux 系统下开发应用程序时用于调试内存问题的工具。它尤其擅长发现内存管理的问题，它可以检查程序运行时的内存泄漏问题。其中的 memecheck 工具可以用来寻找 c、c++ 程序中内存管理的错误。可以检查出下列几种内存操作上的错误：
读写已经释放的内存
读写内存块越界（从前或者从后）
使用还未初始化的变量
将无意义的参数传递给系统调用
内存泄漏
3.2 Rational purify
Rational Purify 主要针对软件开发过程中难于发现的内存错误、运行时错误。在软件开发过程中自动地发现错误，准确地定位错误，提供完备的错误信息，从而减少了调试时间。同时也是市场上唯一支持多种平台的类似工具，并且可以和很多主流开发工具集成。Purify 可以检查应用的每一个模块，甚至可以查出复杂的多线程或进程应用中的错误。另外不仅可以检查 C/C++，还可以对 Java 或 .NET 中的内存泄漏问题给出报告。
在 Linux 系统中，使用 Purify 需要重新编译程序。通常的做法是修改 Makefile 中的编译器变量。下面是用来编译本文中程序的 Makefile：
CC=purify gcc
首先运行 Purify 安装目录下的 purifyplus_setup.sh 来设置环境变量，然后运行 make 重新编译程序。
./purifyplus_setup.sh
下面给出编译一个代码文件的示例，源代码文件命名为 test3.cpp. 用 purify 和 g++ 的编译命令如下，‘-g’是编译时加上调试信息。
purify g++ -g test3.cpp –o test
运行编译生成的可执行文件 test，就可以得到图1，可以定位出内存泄漏的具体位置。
./test
清单3. Purify 分析的代码
```cpp
#include <unistd.h> 
 char * Logmsg;
int LeakTest(char * Para)
{
        if(NULL==Para){
                //local_log("LeakTest Func: empty parameter\n");
                return -1;
        }
        Logmsg = new char[128];
		for (int i = 0 ; i < 128; i++)
			Logmsg[i] = i%64;
        if(NULL == Logmsg){
                //local_log("memeory allocation failed\n");
                return -2;
        }
        sprintf(Logmsg,"LeakTest routine exit: '%s'.\n", Para);
        //local_log(Logmsg);
        return 0;
}
int   main(int argc,char **argv )
{
        char szInit [] = "testcase1";
		int i;
         LeakTest(szInit);
		for (i=0; i < 2; i++){
			if(i%200 == 0)
				LeakTest(szInit);
			sleep(1);
		}        
        return 0;
}
```
需要指出的是，程序必须编译成调试版本才可以定位到具体哪行代码发生了内存泄漏。即在 gcc 或者 g++ 中，必须使用 "-g" 选项。
图 1 purify 的输出结果：
![](https://img-blog.csdn.net/20170325184028646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
