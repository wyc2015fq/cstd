# OpenMP初试及parallel for - 三少GG - CSDN博客
2011年04月09日 21:34:00[三少GG](https://me.csdn.net/scut1135)阅读数：2102标签：[parallel																[compiler																[多线程																[编译器																[fortran																[properties](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=fortran&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=parallel&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
OpenMP是由OpenMP Architecture Review Board牵头提出的，并已被广泛接受的，用于共享内存并行系统的多线程程序设计的一套指导性注释(Compiler Directive)。OpenMP支持的编程语言包括C语言、C++和Fortran；而支持OpenMP的[编译器](http://writeblog.csdn.net/view/487018.htm)包括Sun Compiler，GNU Compiler和Intel Compiler等。OpenMP提供了对并行算法的高层的抽象描述，程序员通过在[源代码](http://writeblog.csdn.net/view/60376.htm)中加入专用的pragma来指明自己的意图，由此编译器可以自动将程序进行并行化，并在必要之处加入同步互斥以及通信。当选择忽略这些pragma，或者编译器不支持OpenMP时，程序又可退化为通常的程序(一般为串行)，代码仍然可以正常运作，只是不能利用多线程来加速程序执行。 
　　OpenMP提供的这种对于并行描述的高层抽象降低了并行编程的难度和复杂度，这样程序员可以把更多的精力投入到并行算法本身，而非其具体实现细节。对基于数据分集的多线程程序设计，OpenMP是一个很好的选择。同时，使用OpenMP也提供了更强的灵活性，可以较容易的适应不同的并行[系统配置](http://writeblog.csdn.net/view/29850.htm)。线程粒度和负载平衡等是传统多线程程序设计中的难题，但在OpenMP中，OpenMP库从程序员手中接管了部分这两方面的工作。 
　　但是，作为高层抽象，OpenMP并不适合需要复杂的线程间同步和互斥的场合。OpenMP的另一个缺点是不能在非共享内存系统(如计算机集群)上使用。在这样的系统上，MPI使用较多。
　要在[Visual C++](http://writeblog.csdn.net/view/100377.htm) 2005 中使用OpenMP其实不难，只要将 Project 的Properties中C/C++里Language的OpenMP Support开启（参数为 /openmp），就可以让VC++2005 在编译时支持OpenMP 的语法了；而在编写使用OpenMP 的程序时，则需要先include OpenMP的头文件：omp.h
********************************************************************************
并行计算从1加到100的值[parallel 和 parallel for的区别]
从1加到100的并行计算代码
/*
并行计算从1加到100的值。
*/
#include <stdio.h>
#include <omp.h>
#include <windows.h>
#include <stdlib.h> 
int main(int argc, char * argv[])
{
int ans=0,i;
#pragma omp parallel for //注意for!!!
for(i=1;i<=100;++i)
ans+=i;
printf("%d/n",ans);
return 0;
}
/*
表示并行执行for里面的内容，比如**OMP_NUM_THREADS==4**
那么可以分成第一个线程执行1~25，第二个线程26~50，……
核心：parallel for，表示并行执行for里面的内容。
#pragma omp parallel for
for(i=1;i<=100;++i)
ans+=i;
5050
*/
/*
++++++++++++++++++++++++++++++++++++++++
如下代码段表示并行执行prrallel后面的代码段
每个线程执行如下一个完整的for循环
每个线程都会循环100次
#pragma omp parallel
for(i=1;i<=100;++i)
ans+=i;
20200
请按任意键继续. . .
重要区别：
parallel:并行执行被该编译指导语句后面的代码段
parallel for:并行执行for里面的东西，每个线程完成for里面的一部分
[转]OpenMP并行程序设计（二）
[http://z1y1m1.blog.163.com/blog/static/51837327200882155458130/](http://z1y1m1.blog.163.com/blog/static/51837327200882155458130/)
**2、OpenMP指令和库函数介绍**
下面来介绍OpenMP的基本指令和常用指令的用法， 
在C/C++中，OpenMP指令使用的格式为
**＃****pragma omp ****指令**** [****子句****[****子句****]…]**
前面提到的parallel for就是一条指令，有些书中也将OpenMP的“指令”叫做“编译指导语句”，后面的子句是可选的。例如：
#pragma omp parallel private(i, j)
parallel 就是指令， private是子句
为叙述方便把包含#pragma和OpenMP指令的一行叫做语句，如上面那行叫parallel语句。
OpenMP的指令有以下一些：
**parallel**，用在一个代码段之前，表示这段代码将被多个线程并行执行
**for**，用于for循环之前，将循环分配到多个线程中并行执行，必须保证每次循环之间无相关性。
**parallel for**， parallel 和 for语句的结合，也是用在一个for循环之前，表示for循环的代码将被多个线程并行执行。
**sections**，用在可能会被并行执行的代码段之前
**3、parallel 指令的用法**
parallel 是用来构造一个并行块的，也可以使用其他指令如for、sections等和它配合使用。
在C/C++中，parallel的使用方法如下：
#pragma omp parallel [for | sections] [子句[子句]…]
{
       //代码
}
parallel语句后面要跟一个大括号对将要并行执行的代码括起来。
void main(int argc, char *argv[]) {
#pragma omp parallel 
{
              printf(“Hello, World!/n”);
}
}
执行以上代码将会打印出以下结果
Hello, World!
Hello, World!
Hello, World!
Hello, World!
可以看得出parallel语句中的代码被执行了四次，说明总共创建了4个线程去执行parallel语句中的代码。
也可以指定使用多少个线程来执行，需要使用num_threads子句：
void main(int argc, char *argv[]) {
#pragma omp parallel num_threads(8)
{
              printf(“Hello, World!, ThreadId=%d/n”, omp_get_thread_num() );
}
}
执行以上代码，将会打印出以下结果：
Hello, World!, ThreadId = 2
Hello, World!, ThreadId = 6
Hello, World!, ThreadId = 4
Hello, World!, ThreadId = 0
Hello, World!, ThreadId = 5
Hello, World!, ThreadId = 7
Hello, World!, ThreadId = 1
Hello, World!, ThreadId = 3
从ThreadId的不同可以看出创建了8个线程来执行以上代码。所以parallel指令是用来为一段代码创建多个线程来执行它的。parallel块中的每行代码都被多个线程重复执行。
和传统的创建线程函数比起来，相当于为一个线程入口函数重复调用创建线程函数来创建线程并等待线程执行完
