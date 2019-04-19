# 【C++繁星】(2):assert ; cerr ; cmath - 三少GG - CSDN博客
2010年09月21日 19:57:00[三少GG](https://me.csdn.net/scut1135)阅读数：1613
## C/C++ 中的assert()宏 断言机制
ASSERT()是一个调试程序时经常使用的宏，在程序运行时它计算括号内的表达式，如果表达式为FALSE (0), 程序将报告错误，并终止执行。如果表达式不为0，则继续执行后面的语句。这个宏通常原来判断程序中是否出现了明显非法的数据，如果出现了终止程序以免导致严重后果，同时也便于查找错误。
原型定义：
#include <assert.h>
void assert( int expression_r_r_r );
assert的作用是现计算表达式 expression_r_r_r ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行。请看下面的程序清单badptr.c：
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
int main( void )
{
       FILE *fp;
       fp = fopen( "test.txt", "w" );//以可写的方式打开一个文件，如果不存在就创建一个同名文件
       assert( fp );                           //所以这里不会出错
       fclose( fp );
       fp = fopen( "noexitfile.txt", "r" );//以只读的方式打开一个文件，如果不存在就打开失败
       assert( fp );                           //所以这里出错
       fclose( fp );                           //程序永远都执行不到这里来
       return 0;
}
[root@localhost error_process]# gcc badptr.c
[root@localhost error_process]# ./a.out
a.out: badptr.c:14: main: Assertion `fp'' failed.
已放弃
使用assert的缺点是，频繁的调用会极大的影响程序的性能，增加额外的开销。
在调试结束后，可以通过在包含#include <assert.h>的语句之前插入 #define NDEBUG 来禁用assert调用，示例代码如下：
#include <stdio.h>
#define NDEBUG
#include <assert.h>
用法总结与注意事项：
1)在函数开始处检验传入参数的合法性
如:
int resetBufferSize(int nNewSize)
{
  //功能:改变缓冲区大小,
  //参数:nNewSize 缓冲区新长度
  //返回值:缓冲区当前长度
  //说明:保持原信息内容不变     nNewSize<=0表示清除缓冲区
  assert(nNewSize >= 0);
  assert(nNewSize <= MAX_BUFFER_SIZE);
  ...
}
2)每个assert只检验一个条件,因为同时检验多个条件时,如果断言失败,无法直观的判断是哪个条件失败
不好: assert(nOffset>=0 && nOffset+nSize<=m_nInfomationSize);
好: assert(nOffset >= 0);
assert(nOffset+nSize <= m_nInfomationSize);
3)不能使用改变环境的语句,因为assert只在DEBUG个生效,如果这么做,会使用程序在真正运行时遇到问题
错误: assert(i++ < 100)
这是因为如果出错，比如在执行之前i=100,那么这条语句就不会执行，那么i++这条命令就没有执行。
正确: assert(i < 100)
         i++;
4)assert和后面的语句应空一行,以形成逻辑和视觉上的一致感
5)有的地方,assert不能代替条件过滤
   ASSERT只有在Debug版本中才有效，如果编译为Release版本则被忽略掉。（在C中，ASSERT是宏而不是函数），使用ASSERT“断言”容易在debug时输出程序错误所在。
   而assert()的功能类似，它是ANSI C标准中规定的函数，它与ASSERT的一个重要区别是可以用在Release版本中。
[](http://blog.csdn.net/samantha_sun/archive/2009/06/01/4232512.aspx)

------------------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------
[http://hi.baidu.com/xingxing_whu/blog/item/a96267d4a5561409a18bb701.html](http://hi.baidu.com/xingxing_whu/blog/item/a96267d4a5561409a18bb701.html)
简述C++里assert的使用（转载）
2009-04-30 20:56
assert宏的原型定义在<assert.h>中，其作用是如果它的条件返回错误，则终止程序执行，原型定义如下：
#include <assert.h>
void assert( int expression );
assert的作用是现计算表达式 expression ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行。
请看下面的程序清单badptr.c：
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
int main( void )
{
       FILE *fp;
       fp = fopen( "test.txt", "w" ); //以可写的方式打开一个文件，如果不存在就创建一个同名文件
       assert( fp );                        //所以这里不会出错
       fclose( fp );
       fp = fopen( "noexitfile.txt", "r" ); //以只读的方式打开一个文件，如果不存在就打开文件失败
       assert( fp );                             //所以这里可能会出错
       fclose( fp );                               //如果出错，程序永远都执行不到这里来
       return 0;
}
在命令行下运行该程序，结果如下：
[root@localhost error_process]# gcc badptr.c 
[root@localhost error_process]# ./a.out 
a.out: badptr.c:14: main: Assertion `fp'' failed.
已放弃
使用assert的缺点是，频繁的调用会极大的影响程序的性能，增加额外的开销。
在调试结束后，可以通过在包含#include <assert.h>的语句之前插入 #define NDEBUG 来禁用assert调用，示例代码如下：
#include <stdio.h>
#define NDEBUG
#include <assert.h>
assert的用法总结与注意事项：
1、在函数开始处检验传入参数的合法性，如：
int resetBufferSize(int nNewSize)
{
 //函数功能:改变缓冲区大小,
   //函数参数:nNewSize 缓冲区新长度
   //函数返回值:缓冲区当前长度 
   //说明:保持原信息内容不变
   //nNewSize<=0表示清除缓冲区
   assert(nNewSize >= 0);
   assert(nNewSize <= MAX_BUFFER_SIZE);
   ...
}
2、每个assert只检验一个条件，因为同时检验多个条件时，如果断言失败，无法直观地判断是哪个条件失败，比较下面两种形式的代码：
不好： assert(nOffset>=0 && nOffset+nSize<=m_nInfomationSize);
好：     assert(nOffset >= 0);
           assert(nOffset+nSize <= m_nInfomationSize);
3、不能使用改变环境的语句，因为assert只在DEBUG时生效，这么做的话会使程序在真正运行时遇到问题
错误，例如：
assert(i++ < 100);
如果在执行之前i=100，那么这条语句就不会执行，i++这条命令就没有执行。
正确的写法： assert(i < 100);
                      i++;
4、assert和后面的语句应空一行，以形成逻辑和视觉上的一致感。
5)注意，有的地方，assert不能代替条件过滤。
文章出处：[http://www.diybl.com/course/3_program/c++/cppjs/20071111/85534.html](http://www.diybl.com/course/3_program/c++/cppjs/20071111/85534.html)
以下内容摘自《高质量C/C++编程指南》Page 41-42...
程序一般分为Debug 版本和Release 版本，Debug 版本用于内部调试，Release 版本发行给用户使用。assert 是仅在Debug 版本起作用的宏，它用于检查“不应该”发生的情况
**assert 不是函数，而是宏。**程序员可以把assert看成一个在任何系统状态下都可以安全使用的无害测试手段。如果程序在 assert 处终止了，并不是说含有该assert 的函数有错误，而是调用者出了差错，assert 可以帮助我们找到发生错误的原因。
assert宏中应该包含的元素：判断条件；输出当前断言失败的位置（文件、行数等）；返回错误；终止程序...
几种典型的assert的写法：
**VC中的写法：**
#define ASSERT(f) /
do /
{ /
   if (!(f) && AfxAssertFailedLine(THIS_FILE, __LINE__)) /
   AfxDebugBreak(); /
} while (0) /
#define _ASSERT(expr) /
        do { if (!(expr) && /
                (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL))) /
             _CrtDbgBreak(); } while (0)
**其他平台的写法：**# define ASSERT(x) ((x) || (dbg_printf("assertion failed ("__FILE__":%d): /"%s/"/n",__LINE__,#x), break_point(), FALSE))
---------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------**cerr**
**　cerr与cout的主要区分就是，cout输出的信息可以重定向，而cerr只能输出到标准输出（显示器）上。**　　例如下面程序编译后生成test.exe　　// test.cpp　　#include <iostream.h>　　int main()　　{　　cout << "hello world---cout" << endl ;　　cerr << "hello world---cerr" << endl ;　　return 0;　　}　　在命令行模式下键入下面的命令：　　test >>cout.txt　　运行结果是：　　在生成的cout.txt文件中输出了"hello world---cout"　　同时在显示器上输出了"hello world---cerr"　　也就是说cout的输出可以重定向到一个文件中，而cerr必须输出在显示器上。　　clog流也是标准错误流,作用和cerr一样,区别在于cerr不经过缓冲区,直接向显示器输出信息,而clog中的信息存放在缓冲区,缓冲区满或者遇到endl时才输出.　　对于为什么有cerr和clog　　比如，你的程序遇到调用栈用完了的威胁（无限，没有出口的递归）。　　你说，你到什么地方借内存，存放你的错误信息？　　所以有了cerr。其目的，就是在你最需要它的紧急情况下，还能得到输出功能的支持。　　缓冲区的目的，就是减少刷屏的次数——比如，你的程序输出圣经中的一篇文章。不带缓冲的话，就会每写一个字母，就输出一个字母，然后刷屏。有了缓冲，你将看到若干句子“同时”就出现在了屏幕上（由内存翻新到显存，然后刷新屏幕）。
================================================================================
cmath并不等同于math.h
尽管大部分的函数都是一样的..但是math.h是C语言的库.cmath是C++的...  两者并不等同...
C++程序推荐使用
#include <cmath>
C程序推荐使用
#include <math.h> 
并不存在cmath.h和math这两个头文件。      #inlcude <cmath> 等价于 #include <math.h> 
★在c里只能用math.h 
在C++里两个都可以用——发表日期:2008-7-27 20:55
★cmath是标准C++里面推荐使用的库。 
math是C语言的旧头文件。 
不过目前大部分编译器的实现上两者没有任何区别。——发表日期:2008-7-27 21:44
---------------------
-----------------
**#include <cmath> 标准C＋＋函数，都不代H  切记**
