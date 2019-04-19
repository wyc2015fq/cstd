# C++内存泄漏处理对策 - fanyun的博客 - CSDN博客
2016年01月28日 14:36:44[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1226
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
** 引言：     **
      使用C/C++这种没有内存回收的机制的语言，怎么合理分配和释放内存，就显得尤为重要。尤其是长时间运行的服务器程序上，内存出现一个小小的问题，时间长了也可能造成意想不到的异常，导致服务器宕机或者某些关键服务的失败。因此，作为一名C/C++语言开发者，如何处理内存泄漏问题是十分重要的。
**那么什么是内存泄露？**
内存泄漏指由于疏忽或错误造成程序未能释放已经不再使用的内存的情况。内存泄漏并非指内存在物理上的消失，而是应用程序分配某段内存后，由于设计错误，导致在释放该段内存之前就失去了对该段内存的控制，从而造成了内存的浪费。
**内存泄漏的原因：**
   在C/C++系统开发中，内存泄露这种错误是很难检测和被发现的，有时甚至在系统上线运行了很久，也似乎没有出现什么异常，但是某一天，系统就莫名地出故障了：从性能不良（并且逐渐降低）到内存完全用尽。更糟的是，泄漏的程序可能会用掉太多内存，以致另一个程序失败，而使用户无从查找问题的真正根源。此外，即使无害的内存泄漏也可能是其他问题的征兆。实质上未能正确释放以前分配的内存的
 bug。只发生一次的小的内存泄漏可能不会被注意，但泄漏大量内存的程序或泄漏日益增多的程序就会导致如上错误。
内存泄漏会因为减少可用内存的数量从而降低计算机的性能。最终，在最糟糕的情况下，过多的可用内存被分配掉导致全部或部分设备停止正常工作，或者应用程序崩溃。内存泄漏可能不严重，甚至能够被常规的手段检测出来。在现代操作系统中，一个应用程序使用的常规内存在程序终止时被释放。这表示一个短暂运行的应用程序中的内存泄漏不会导致严重后果。
    常见的内存泄漏如下所示：
·      程序运行后置之不理，并且随着时间的流失消耗越来越多的内存（比如服务器上的后台任务，尤其是嵌入式系统中的后台任务，这些任务可能被运行后很多年内都置之不理）；
·      新的内存被频繁地分配，比如当显示电脑游戏或动画视频画面时；
·      程序能够请求未被释放的内存（比如共享内存），甚至是在程序终止的时候；
·      泄漏在操作系统内部发生；
·      泄漏在系统关键驱动中发生；
·      内存非常有限，比如在嵌入式系统或便携设备中；
·      当运行于一个终止时内存并不自动释放的操作系统（比如AmigaOS）之上，而且一旦丢失只能通过重启来恢复。
**如何处理内存泄漏？**
# 1.Windows平台下的内存泄漏检测
## 1.1.检测是否存在内存泄漏问题
Windows平台下面Visual Studio调试器和 C
运行时(CRT) 库为我们提供了检测和识别内存泄漏的有效方法，原理大致如下：内存分配要通过CRT在运行时实现，只要在分配内存和释放内存时分别做好记录，程序结束时对比分配内存和释放内存的记录就可以确定是不是有内存泄漏。在vs中启用内存检测的方法如下：
·      STEP1，在程序中包括以下语句：（#include语句必须采用上文所示顺序。如果更改了顺序，所使用的函数可能无法正常工作。）
|123|`#define _CRTDBG_MAP_ALLOC``#include <stdlib.h>``#include <crtdbg.h>`|
通过包括 crtdbg.h，将 malloc 和 free 函数映射到它们的调试版本，即 _malloc_dbg 和 _free_dbg，这两个函数将跟踪内存分配和释放。此映射只在调试版本（在其中定义了_DEBUG）中发生。发布版本使用普通的 malloc 和 free 函数。
#define 语句将 CRT堆函数的基版本映射到对应的“Debug”版本。并非绝对需要该语句；但如果没有该语句，内存泄漏转储包含的有用信息将较少。
·      STEP2，在添加了上述语句之后，可以通过在程序中包括以下语句（通常应恰好放在程序退出位置之前）来转储内存泄漏信息：
|1|`_CrtDumpMemoryLeaks();`|
此时，完整的代码如下：
```cpp
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
 
#include <iostream>
using namespace std;
 
void GetMemory(char *p, int num)
{
    p = (char*)malloc(sizeof(char) * num);
}
 
int main(int argc,char** argv)
{
    char *str = NULL;
    GetMemory(str, 100);
    cout<<"Memory leak test!"<<endl;
    _CrtDumpMemoryLeaks();
    return 0;
}
```
当在调试器下运行程序时，_CrtDumpMemoryLeaks 将在“输出”窗口中显示内存泄漏信息。内存泄漏信息如下所示：
如果没有使用#define _CRTDBG_MAP_ALLOC语句，内存泄漏转储将如下所示：
未定义_CRTDBG_MAP_ALLOC时，所显示的会是：
·       内存分配编号（在大括号内）。
·       块类型（普通、客户端或 CRT）。
·       “普通块”是由程序分配的普通内存。
·       “客户端块”是由 MFC程序用于需要析构函数的对象的特殊类型内存块。MFC
 new操作根据正在创建的对象的需要创建普通块或客户端块。
·       “CRT 块”是由 CRT库为自己使用而分配的内存块。CRT库处理这些块的释放，因此您不大可能在内存泄漏报告中看到这些块，除非出现严重错误（例如CRT库损坏）。
从不会在内存泄漏信息中看到下面两种块类型：
·       “可用块”是已释放的内存块。
·       “忽略块”是您已特别标记的块，因而不出现在内存泄漏报告中。
·       十六进制形式的内存位置。
·       以字节为单位的块大小。
·       前 16字节的内容（亦为十六进制）。
定义了_CRTDBG_MAP_ALLOC时，还会显示在其中分配泄漏的内存的文件。文件名后括号中的数字（本示例中为 10）是该文件中的行号。
注意：如果程序总是在同一位置退出，调用 _CrtDumpMemoryLeaks 将非常容易。如果程序从多个位置退出，则无需在每个可能退出的位置放置对 _CrtDumpMemoryLeaks 的调用，而可以在程序开始处包含以下调用：
+View Code
该语句在程序退出时自动调用 _CrtDumpMemoryLeaks。必须同时设置_CRTDBG_ALLOC_MEM_DF 和 _CRTDBG_LEAK_CHECK_DF 两个位域，如前面所示。
## 1.2.定位具体的内存泄漏地方
通过上面的方法，我们几乎可以定位到是哪个地方调用内存分配函数malloc和new等，如上例中的GetMemory函数中，即第10行！但是不能定位到，在哪个地方调用GetMemory()导致的内存泄漏，而且在大型项目中可能有很多处调用GetMemory。如何要定位到在哪个地方调用GetMemory导致的内存泄漏？
定位内存泄漏的另一种技术涉及在关键点对应用程序的内存状态拍快照。 CRT库提供一种结构类型 _CrtMemState，您可用它存储内存状态的快照：
|1|`_CrtMemState s1, s2, s3;`|
若要在给定点对内存状态拍快照，请向 _CrtMemCheckpoint 函数传递 _CrtMemState 结构。该函数用当前内存状态的快照填充此结构：
|1|`_CrtMemCheckpoint( &s1 );`|
通过向 _CrtMemDumpStatistics 函数传递 _CrtMemState 结构，可以在任意点转储该结构的内容：
|1|`_CrtMemDumpStatistics( &s1 );`|
若要确定代码中某一部分是否发生了内存泄漏，可以在该部分之前和之后对内存状态拍快照，然后使用 _CrtMemDifference比较这两个状态：
|123456|`_CrtMemCheckpoint( &s1 );``// memory allocations take place here``_CrtMemCheckpoint( &s2 );``if``( _CrtMemDifference( &s3, &s1, &s2) )``   _CrtMemDumpStatistics( &s3 );`|
顾名思义，_CrtMemDifference 比较两个内存状态（s1和s2），生成这两个状态之间差异的结果（s3）。在程序的开始和结尾放置 _CrtMemCheckpoint 调用，并使用_CrtMemDifference 比较结果，是检查内存泄漏的另一种方法。如果检测到泄漏，则可以使用 _CrtMemCheckpoint 调用通过二进制搜索技术来划分程序和定位泄漏。
如上面的例子程序我们可以这样来定位确切的调用GetMemory的地方：
+View Code
调试时，程序输出如下结果：
这说明在s1和s2之间存在内存泄漏！！！如果GetMemory不是在s1和s2之间调用，那么就不会有信息输出。
# 2.Linux平台下的内存泄漏检测
在上面我们介绍了，vs中在代码中“包含crtdbg.h，将 malloc 和 free 函数映射到它们的调试版本，即 _malloc_dbg 和_free_dbg，这两个函数将跟踪内存分配和释放。此映射只在调试版本（在其中定义了_DEBUG）中发生。发布版本使用普通的 malloc 和 free 函数。”即为malloc和free做了钩子，用于记录内存分配信息。
Linux下面也有原理相同的方法——mtrace，http://en.wikipedia.org/wiki/Mtrace。方法类似，我这就不具体描述，参加给出的链接。这节我主要介绍一个非常强大的工具valgrind。如下图所示：
如上图所示知道：
==6118==100 bytes in 1 blocks are definitely lost in loss record 1 of 1 
==6118==    at 0x4024F20: malloc (vg_replace_malloc.c:236) 
==6118==    by 0x8048724: GetMemory(char*, int) (in/home/netsky/workspace/a.out) 
==6118==    by 0x804874E: main (in /home/netsky/workspace/a.out)
是在main中调用了GetMemory导致的内存泄漏，GetMemory中是调用了malloc导致泄漏了100字节的内存。
Things to notice: 
• There is a lot of information in each errormessage; read it carefully. 
• The 6118 is the process ID; it’s usuallyunimportant. 
• The ﬁrst line ("Heap Summary") tellsyou what kind of error it is. 
• Below the ﬁrst line is a stack trace telling youwhere the problem occurred. Stack traces can get quite large, and be 
confusing, especially if you are using the C++STL. Reading them from the bottom up can help.
• The code addresses (eg. 0x4024F20)are usually unimportant, but occasionally crucial for tracking downweirder 
bugs.
Thestack trace tells you where the leaked memory was allocated. Memcheck cannottell you why the memory leaked, 
unfortunately. (Ignore the "vg_replace_malloc.c", that’s animplementation detail.) 
There are several kinds of leaks; the two most important categories are: 
• "deﬁnitely lost": your program is leaking memory -- ﬁx it! 
• "probably lost": your program is leaking memory, unless you’redoing funny things with pointers (such as moving 
them to point to the middle of a heap block)
Valgrind的使用请见手册http://valgrind.org/docs/manual/manual.html。
**总结：**
         其实内存泄漏的原因可以概括为：调用了malloc/new等内存申请的操作，但缺少了对应的free/delete，总之就是，malloc/new比free/delete的数量多。我们在编程时需要注意这点，保证每个malloc都有对应的free，每个new都有对应的deleted。
         良好的编程习惯是避免内存泄露的最有效手段，再就是，要保证malloc/new和free/delete匹配！
