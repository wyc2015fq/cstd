# 程序性能分析工具gprof介绍 - xiahouzuoxin - CSDN博客





2012年09月09日 22:40:51[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2178







性能分析工具gprof介绍

Ver：1.0



目录

1.    GPROF介绍    4

2.    使用步骤    4

3.    使用举例    4

3.1    测试环境    4

3.2    测试代码    4

3.3    数据分析    5

3.3.1    flat profile模式    6

3.3.2    call graph模式    7

4.    链接库中的函数    7

5.    使用局限    8

6.    分析示例    12


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

1.    gprof介绍

gprof是GNU profile工具，可以运行于linux、AIX、Sun等操作系统进行C、C++、Pascal、Fortran程序的性能分析，用于程序的性能优化以及程序瓶颈问题的查找和解决。通过分


析应用程序运行时产生的“flat profile”，可以得到每个函数的调用次数，每个函数消耗的处理器时间，也可以得到函数的“调用关系图”，包括函数调用的层次关系，每个函


数调用花费了多少时间。


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

2.    使用步骤

1)    用gcc、g++、xlC编译程序时，使用-pg参数，如：g++ -pg -o test.exe test.cpp

编译器会自动在目标代码中插入用于性能测试的代码片断，这些代码在程序运行时采集并记录函数的调用关系和调用次数，并记录函数自身执行时间和被调用函数的执行时间。

2)    执行编译后的可执行程序，如：./test.exe。该步骤运行程序的时间会稍慢于正常编译的可执行程序的运行时间。程序运行结束后，会在程序所在路径下生成一个缺省文


件名为gmon.out的文件，这个文件就是记录程序运行的性能、调用关系、调用次数等信息的数据文件。

3)    使用gprof命令来分析记录程序运行信息的gmon.out文件，如：gprof test.exe gmon.out则可以在显示器上看到函数调用相关的统计、分析信息。上述信息也可以采用

gprof test.exe gmon.out> gprofresult.txt重定向到文本文件以便于后续分析。


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

3.    使用举例

3.1    测试环境

本文提供的样例的测试环境如下：

?    Linux server164 2.6.9-22.ELsmp #1 SMP Mon Sep 19 18:32:14 EDT 2005 i686 i686 i386 GNU/Linux

?    gcc version 3.2.3 20030502 (Red Hat Linux 3.2.3-47.3)

?    GNU gprof 2.15.92.0.2

3.2    测试代码

清单 1. 耗时测试应用程序示例

example1.c


#include <stdio.h>

int a(void) 

{

  int i=0,g=0;

  while(i++<100000)

  {

     g+=i;

  }

  return g;

}


int b(void)

{

  int i=0,g=0;

  while(i++<400000)

  {

    g +=i;

  }

  return g;

}

int main(int argc, char** argv)

{

   int iterations;

   if(argc != 2)

   {

      printf("Usage %s <No of Iterations>\n", argv[0]);

      exit(-1);

   }

   else

      iterations = atoi(argv[1]);

   printf("No of iterations = %d\n", iterations);

   while(iterations--)

   {

      a();

      b();

   }

}

这个应用程序包括两个函数：a 和 b，它们通过运行不同次数的循环来消耗不同的CPU时间。main 函数中采用了一个循环来反复调用这两个函数。函数b中循环的次数是 a 函数的 


4 倍，因此我们期望通过gprof的分析结果可以观察到大概有 20% 的时间花在了 a 函数中，而 80% 的时间花在了 b 函数中。

3.3    数据分析

在 gcc 编译命令中加上 –pg参数即可。编译方法如下：
gcc example1.c -pg -o example1 -O2 -lc

在编译好这个应用程序之后，按照普通方式运行这个程序：
./example1 50000

程序运行完之后，应该会看到在当前目录中新创建了一个文件 gmon.out。


3.3.1    flat profile模式

使用 gprof 命令分析gmon.out 文件，如下所示：

gprof example1 gmon.out -p


-p参数标识“flat profile”模式，在分析结果中不显示函数的调用关系，AIX平台默认此参数有效。

输出以下内容：

清单 2. flat profile 的结果

Flat profile:

Each sample counts as 0.01 seconds.

  %    cumulative   self              self     total           

 time   seconds   seconds    calls  ms/call  ms/call  name    

 80.38    203.27   203.27    50000     4.07     4.07    b

 19.61    252.87    49.60    50000     0.99     0.99    a

  0.00    252.88     0.01                                  main


上面结果中各个列的含义如下：

%time       函数以及衍生函数（函数内部再次调用的子函数）所占的总运行时间的百分比 

cumulative seconds 函数累计执行的时间

self seconds  函数执行占用的时间

calls          函数的调用次数

self   ms/call   每一次调用函数花费的时间microseconds，不包括衍生函数的运行时间

total  ms/call    每一次调用函数花费的时间microseconds，包括衍生函数的运行时间

name           函数名称

列的含义，在gprof的输出结果中都有详细的说明。

从输出结果中可以看到，正如我们期望的一样，b 函数所花费的时间大概是 a 函数所花费的时间的 4倍。

很多函数调用（例如 printf）在上面的输出结果中都没有出现。这是因为大部分函数都是在C链接库（libc.so）中，而链接库并没有使用 -pg 进行编译，因此就没有对链接库中


的函数收集调度信息。


3.3.2    call graph模式

如果希望反映函数之间的调用关系，需要采用如下命令：

gprof example1 gmon.out –q


-q参数标识“call graph”模式，在分析结果中显示函数的调用关系。

输出以下内容：

清单 3. Call graph

granularity: each sample hit covers 4 byte(s) for 0.00% of 252.72 seconds


index % time    self  children    called     name

                                                 <spontaneous>

[1]    100.0    0.00  252.72                 main [1]

               201.41    0.00   50000/50000       b [2]

                51.31    0.00   50000/50000       a [3]

-----------------------------------------------

               201.41    0.00   50000/50000       main [1]

[2]     79.7 201.41    0.00   50000         b [2]

-----------------------------------------------

                51.31    0.00   50000/50000       main [1]

[3]     20.3   51.31    0.00   50000         a [3]

-----------------------------------------------

上面结果中各个列的含义如下：

index   每个函数第一次出现时都分配了一个编号，根据编号可以方便的查找函数的具体分析数据

%time  函数以及衍生函数（函数内部再次调用的子函数）所占的总运行时间的百分比 

self    函数的总运行时间

children 衍生函数执行的总时间

called   函数被调用的次数，不包括递归调用

name   函数名称

在name列中，可以看出函数之间的调用关系，main函数调用a、b函数，b函数被main函数调用，a函数被main函数调用。通过函数名称后面的数字来标识这个文件中的函数，从而可


以快速定位函数的分析数据的位置，经过一层层的逐步深入的分析就得到各个函数的调用关系以及各个函数的性能数据。


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

4.    链接库中的函数

正如在前面曾经介绍的，对于代码剖析的支持是由编译器增加的，因此如果希望从链接库（比如libc.a）中获得剖析信息，就需要使用 -pg 来编译这些库。很多操作系统默认提供


了已经启用代码剖析支持而编译的链接库版本，例如：libc.a对应的采用-pg编译的文件为libc_p.a。对于没有按照标准提供类似libc_p.a链接库的操作系统版本来说，就需要安装


已经编译好的启用代码剖析的链接库版本或者自己下载链接库的源代码进行编译。

使用“启用代码剖析的链接库版本”的应用场景，举例如下：
gcc example1.c -g -pg -o example1 -O2 -lc_p

然后，像普通情况下一样运行gprof对gmon.out进行分析，可以获得 flat profile 或 call graph，而此时的分析结果会非常丰富，包含很多C的标准库函数的调用信息，例如：


printf、write等。

注意：上例的libc_p.a是静态链接库。gprof目前还不支持对动态链接库中的函数进行性能分析。


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

5.    使用局限

gprof的特点是它只能分析应用程序在运行过程中所消耗掉的CPU时间，只有当应用程序的函数消耗CPU的时候，gprof才能够获取函数的性能数据。如果应用程序在运行过程中暂时


挂起，并在系统内核唤醒应用程序后进一步执行，那么在应用程序中间暂停的时间性能数据是无法统计的；而且在应用程序等待I/O操作返回的时间，性能数据也是无法统计的。

如果对清单 1 稍加修改，就可以清楚地看出这个问题：

清单 5. 为清单 1 添加sleep()函数调用

example2.c:


#include <stdio.h>

int a(void)

{

  sleep(1);               /*调用系统函数进行sleep*/

  return 0;

}

int b(void)

{

  sleep(4);               /*调用系统函数进行sleep*/

  return 0;

}

int main(int argc, char** argv)

{

   int iterations;

   if(argc != 2)

   {

      printf("Usage %s <No of Iterations>\n", argv[0]);

      exit(-1);

   }

   else

      iterations = atoi(argv[1]);

   printf("No of iterations = %d\n", iterations);

   while(iterations--)

   {

      a();

      b();

   }

}


现在 a 函数和 b 函数不再处理繁忙的循环了，而是分别调用sleep()来挂起1秒和4秒。

使用“启用代码剖析的链接库版本”，编译这个应用程序：
gcc example2.c -g -pg -o example2 -O2 -lc_p

并让这个应用程序循环 30 次：
./example2 30

执行gprof example2 gmon.out –p所生成的结果如下：

清单 6. flat profile 显示了系统调用的结果

Flat profile:


Each sample counts as 0.01 seconds.

 no time accumulated


  %   cumulative   self              self     total           

 time   seconds   seconds    calls  Ts/call  Ts/call  name    

  0.00      0.00     0.00       21     0.00     0.00  _IO_file_overflow

  0.00      0.00     0.00       13     0.00     0.00  strncmp

  0.00      0.00     0.00        8     0.00     0.00  memset

  0.00      0.00     0.00        8     0.00     0.00  sigprocmask

  0.00      0.00     0.00        7     0.00     0.00  getenv

  0.00      0.00     0.00        6     0.00     0.00  memcpy

  0.00      0.00     0.00        5     0.00     0.00  _int_malloc

  0.00      0.00     0.00        5     0.00     0.00  malloc

  0.00      0.00     0.00        5     0.00     0.00  sigaction

  0.00      0.00     0.00        5     0.00     0.00  strsep

  0.00      0.00     0.00        4     0.00     0.00  nanosleep

  0.00      0.00     0.00        4     0.00     0.00  sleep

  0.00      0.00     0.00        4     0.00     0.00  strpbrk

  0.00      0.00     0.00        3     0.00     0.00  _IO_new_file_xsputn

  0.00      0.00     0.00        3     0.00     0.00  ____strtoul_l_internal

  0.00      0.00     0.00        3     0.00     0.00  __cxa_atexit

  0.00      0.00     0.00        3     0.00     0.00  __strtoul_internal

  0.00      0.00     0.00        2     0.00     0.00  __errno_location

  0.00      0.00     0.00        2     0.00     0.00  __find_specmb

  0.00      0.00     0.00        2     0.00     0.00  a

  0.00      0.00     0.00        2     0.00     0.00  b

  0.00      0.00     0.00        2     0.00     0.00  mempcpy

  0.00      0.00     0.00        1     0.00     0.00  _IO_default_xsputn

  0.00      0.00     0.00        1     0.00     0.00  _IO_doallocbuf

  0.00      0.00     0.00        1     0.00     0.00  _IO_file_doallocate

  0.00      0.00     0.00        1     0.00     0.00  _IO_file_stat

  0.00      0.00     0.00        1     0.00     0.00  _IO_file_write

  0.00      0.00     0.00        1     0.00     0.00  _IO_setb

  0.00      0.00     0.00        1     0.00     0.00  ____strtol_l_internal

  0.00      0.00     0.00        1     0.00     0.00  ___fxstat64

  0.00      0.00     0.00        1     0.00     0.00  __init_misc

  0.00      0.00     0.00        1     0.00     0.00  __libc_csu_fini

  0.00      0.00     0.00        1     0.00     0.00  __libc_csu_init

  0.00      0.00     0.00        1     0.00     0.00  __libc_init_first

  0.00      0.00     0.00        1     0.00     0.00  __libc_init_secure

  0.00      0.00     0.00        1     0.00     0.00  __libc_setup_tls

  0.00      0.00     0.00        1     0.00     0.00  __libc_start_main

  0.00      0.00     0.00        1     0.00     0.00  __pthread_initialize_minimal

  0.00      0.00     0.00        1     0.00     0.00  __setfpucw

  0.00      0.00     0.00        1     0.00     0.00  __strtol_internal

  0.00      0.00     0.00        1     0.00     0.00  _dl_aux_init

  0.00      0.00     0.00        1     0.00     0.00  _dl_important_hwcaps

  0.00      0.00     0.00        1     0.00     0.00  _dl_init_paths

  0.00      0.00     0.00        1     0.00     0.00  _dl_non_dynamic_init

  0.00      0.00     0.00        1     0.00     0.00  _itoa_word

  0.00      0.00     0.00        1     0.00     0.00  _mcleanup

  0.00      0.00     0.00        1     0.00     0.00  atexit

  0.00      0.00     0.00        1     0.00     0.00  atoi

  0.00      0.00     0.00        1     0.00     0.00  exit

  0.00      0.00     0.00        1     0.00     0.00  fillin_rpath

  0.00      0.00     0.00        1     0.00     0.00  fini

  0.00      0.00     0.00        1     0.00     0.00  flockfile

  0.00      0.00     0.00        1     0.00     0.00  funlockfile

  0.00      0.00     0.00        1     0.00     0.00  main

  0.00      0.00     0.00        1     0.00     0.00  mmap

  0.00      0.00     0.00        1     0.00     0.00  printf

  0.00      0.00     0.00        1     0.00     0.00  setitimer

  0.00      0.00     0.00        1     0.00     0.00  strrchr

  0.00      0.00     0.00        1     0.00     0.00  uname

  0.00      0.00     0.00        1     0.00     0.00  vfprintf

  0.00      0.00     0.00        1     0.00     0.00  write


对以上输出结果进行分析可见，尽管 profile 已经记录了每个函数被调用的确切次数，但是为这些函数记录的时间（实际上是所有函数）都是 0.00。这是因为 sleep 函数实际上


是执行了一次对内核空间的调用，从而将应用程序的执行挂起了，然后有效地暂停执行，并等待内核再次将其唤醒。由于花在用户空间执行的时间与花在内核中睡眠的时间相比非


常小，因此就被取整成零了。其原因是 gprof 以固定的周期对程序运行时间进行采样测量，当程序挂起时，gprof就不会对程序进行采样测量。

Gprof的特性使得有些程序非常难以进行优化，例如花费大部分时间睡眠等待内核唤醒的程序，或者由于外部因素（例如操作系统的 I/O 子系统过载）而运行得非常慢的程序。

通常，有一个很好的基准测试可以用来查看 gprof 对于应用程序的优化能起多大作用，方法是在 time 命令下面执行应用程序。此命令会显示一个应用程序运行完成需要多少时间


，并且在用户空间和内核空间各花费了多少时间。

例如：time ./example2 30

输出结果如下所示：

清单 7. time 命令的输出结果

No of iterations = 30

real    2m30.295s

user    0m0.000s

sys     0m0.004s

我们可以看出应用程序整体运行150秒左右，但大部分时间在睡眠状态，几乎没有多少时间被花费在执行用户空间和内核空间的代码上，此时gprof的分析结果无法体现函数的实际


运行时间。


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

6.    分析示例

在新疆联通进行综合入库程序上线前的测试过程中，测试人员发现程序运行效率低下。下面以整个调优的过程来举例说明基于gprof的性能分析方法。

性能分析前的准备阶段，性能分析数据的生成步骤如下：

1)    首先更改makefile文件中的CC=g++ -g –pg 行，添加-pg参数。

2)    执行make clean删除所有的目标文件、可执行文件

3)    重新编译

4)    在命令行执行可执行程序，并通过控制台发送启动命令

5)    程序退出后在fileindb执行程序目录下生成gmon.out(不是控制台目录)

6)    执行gprof fileindb > gsmindb_mon_result.txt把分析结果重定向到gsmindb_mon_result.txt文件

下面就是从gsmindb_mon_result.txt文件中截取的部分内容：


打开gsmindb_mon_result.txt文件后，可以看到127行：

[3]     37.2    0.00       41.90       1         .main [3]

                  0.00       41.88       1/1       .CMainManager::Run(int&) [4] 

可见main函数中的CMainManager::Run运行时间(41.88)几乎占据了所有的运行时间(41.90)，再查CMainManager::Run的分析数据在148行：

[4]     37.2    0.00       41.88       1         .CMainManager::Run(int&) [4]

               0.00       41.81       1/1       .CBaseBillFile::DoEveryBill() [5] 

可见CMainManager::Run 函数中CBaseBillFile::DoEveryBill执行占了几乎所有的时间，对于占用时间比重很小的函数可以忽略。采用上面的方法一步一步向下查找，最终确定289


行：

           0.56       11.98  849922/849922      .CMobileFavour::CheckAllCondition(int,int) [11]

[12] 11.1    0.56       11.98  849922             .CGsmFavourCheck::CheckAllCondition() [12]

           0.06        5.82  849922/849922      .CGsmFavourCheck::CheckCallKind() [14]

在CGsmFavourCheck::CheckAllCondition()所调用的函数中CGsmFavourCheck::CheckCallKind()占用时间的比重过大，几乎达到50%；CGsmFavourCheck::CheckAllCondition总共用


时11.98，而CGsmFavourCheck::CheckCallKind()函数就占用了5.82，从而说明CGsmFavourCheck::CheckCallKind()函数需要作进一步优化，以提高整体的运行效率。


