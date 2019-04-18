# 分析函数调用关系图(call graph)的几种方法 - gauss的专栏 - CSDN博客
2013年01月04日 00:26:12[gauss](https://me.csdn.net/mathlmx)阅读数：696
绘制函数调用关系图对理解大型程序大有帮助。我想大家都有过一边读源码（并在头脑中维护一个调用栈），一边在纸上画函数调用关系，然后整理成图的经历。如果运气好一点，借助调试器的单步跟踪功能和call stack窗口，能节约一些脑力。不过如果要分析的是脚本语言的代码，那多半只好老老实实用第一种方法了。如果在读代码之前，手边就有一份调用图，岂不妙哉？下面举出我知道的几种免费的分析C/C++函数调用关系的工具。
函数调用关系图（call graph）是图（graph），而且是有向图，多半还是无环图（无圈图）——如果代码中没有直接或间接的递归的话。[Graphviz](http://www.graphviz.org/)是专门绘制有向图和无向图的工具，所以很多call graph分析工具都以它为后端(back end)。那么前端呢？就看各家各显神通了。
调用图的分析分析大致可分为“静态”和“动态”两种，所谓静态分析是指在不运行待分析的程序的前提下进行分析，那么动态分析自然就是记录程序实际运行时的函数调用情况了。
静态分析又有两种方法，一是分析源码，二是分析编译后的目标文件。
分析源码获得的调用图的质量取决于分析工具对编程语言的理解程度，比如能不能找出正确的C++重载函数。[Doxygen](http://www.stack.nl/~dimitri/doxygen/index.html)是源码文档化工具，也能绘制调用图，它似乎是自己分析源码获得函数调用关系的。[GNU
 cflow](http://www.gnu.org/software/cflow/)也是类似的工具，不过它似乎偏重分析流程图（flowchart）。
对编程语言的理解程度最好的当然是编译器了，所以有人想出给编译器打补丁，让它在编译时顺便记录函数调用关系。[CodeViz](http://www.csn.ul.ie/~mel/projects/codeviz/)（其灵感来自[Martin Devera
 (Devik) 的工具](http://luxik.cdi.cz/~devik/mm.htm)）就属于此类，它（1.0.9版）给GCC 3.4.1打了个补丁。另外一个工具[egypt](http://www.gson.org/egypt/egypt.html)的思路更巧妙，不用大动干戈地给编译器打补丁，而是让编译器自己dump出调用关系，然后分析分析，交给Graphviz去绘图。不过也有人另起炉灶，自己写个C语言编译器（[ncc](http://students.ceid.upatras.gr/~sxanth/ncc/)），专门分析调用图，勇气可嘉。不如要是对C++语言也这么干，成本不免太高了。分析C++的调用图，还是借助编译器比较实在。
分析目标文件听起来挺高深，其实不然，反汇编的工作交给binutils的objdump去做，只要分析一下反汇编出来的文本文件就行了。下面是Cygwin下objdump -d a.exe的部分结果：
00401050 <_main>:
  401050:       55                      push   %ebp
  401051:       89 e5                   mov    %esp,%ebp
  401053:       83 ec 18                sub    $0x18,%esp
   ......
 40107a:       c7 44 24 04 00 20 40    movl   $0x402000,0x4(%esp)
  401081:       00
  401082:       c7 04 24 02 20 40 00    movl   $0x402002,(%esp)
  401089:       e8 f2 00 00 00          call   401180 <_fopen>
从中可以看出，main()调用了fopen()。CodeViz带有分析目标文件的功能。
动态分析是在程序运行时记录函数的调用，然后整理成调用图。与静态分析相比，它能获得更多的信息，比如函数调用的先后顺序和次数；不过也有一定的缺点，比如程序中语句的某些分支可能没有执行到，这些分支中调用的函数自然就没有记录下来。
动态分析也有两种方法，一是借助gprof的call graph功能（参数-q），二是利用GCC的 
```
-finstrument-functions
```
 参数。
gprof生成的输出如下：
index % time    self  children    called     name
                0.00    0.00       4/4           foo [4]
[3]      0.0    0.00    0.00       4         bar [3]
-----------------------------------------------
                0.00    0.00       1/2           init [5]
                0.00    0.00       1/2           main [45]
[4]      0.0    0.00    0.00       2         foo [4]
                0.00    0.00       4/4           bar [3]
-----------------------------------------------
                0.00    0.00       1/1           main [45]
[5]      0.0    0.00    0.00       1         init [5]
                0.00    0.00       1/2           foo [4]
-----------------------------------------------
从中可以看出，bar()被foo()调用了4次，foo()被init()和main()各调用了一次，init()被main()调用了一次。用Perl脚本分析gprof的输出，生成Graphviz的dot输入，就能绘制call graph了。这样的脚本不止一个人写过：[http://www.graphviz.org/Resources.php](http://www.graphviz.org/Resources.php)，[http://www.ioplex.com/~miallen/](http://www.ioplex.com/~miallen/)。
GCC的-finstrument-functions 参数的作用是在程序中加入hook，让它在每次进入和退出函数的时候分别调用下面这两个函数：
```
void __cyg_profile_func_enter( void *func_address, void *call_site )
                                __attribute__ ((no_instrument_function));
void __cyg_profile_func_exit ( void *func_address, void *call_site )
                                __attribute__ ((no_instrument_function));
```
当然，这两个函数本身不能被钩住（使用no_instrument_function这个__attribute__），不然就反反复复万世不竭了:) 这里获得的是函数地址，需要用binutils中的addr2line这个小工具转换为函数名，如果是C++函数，还要用c++filt进行name demangle。具体方法在《  [用Graphviz
 可视化函数调用](http://www-128.ibm.com/developerworks/cn/linux/l-graphvis/)》中有详细介绍，这里不再赘述。
从适应能力上看，源码分析法是最强的，即便源码中有语法错，头文件不全也没关系，它照样能分析个八九不离十。而基于编译器的分析法对源码的要求要高一些，至少能编译通过（gcc 参数 -c）——能产生object file，不一定要链接得到可执行文件。这至少要求源码没有语法错，其中调用的函数不一定有定义(definition)，但要有声明(declaration)，也就是说头文件要齐全。当然，真的不全也没关系，自己放几个函数声明在前面就能糊弄编译器:) 至于动态分析，要求最高——程序需得运行起来。如果你要分析的是操作系统中某一部分，比如内存管理或网络协议栈，那么这里提到的两种动态分析法恐怕都不适用了。
我发现前面列举的所有免费工具几乎都和GCC、GNU Binutils脱不了干系。这里在把它们整理一下，用Graphviz绘成图：
![](http://blog.csdn.net/images/blog_csdn_net/Solstice/call.gif)
