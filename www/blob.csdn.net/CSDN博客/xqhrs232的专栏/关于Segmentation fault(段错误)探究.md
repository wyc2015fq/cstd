# 关于Segmentation fault(段错误)探究 - xqhrs232的专栏 - CSDN博客
2016年10月15日 15:19:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：393
原文地址::[http://blog.csdn.net/love_gaohz/article/details/6597857](http://blog.csdn.net/love_gaohz/article/details/6597857)
在rhel5.4上用GCC编程好长时间了（其实也就1个多月！），经常遇到这种情况：一个程序编译没有任何问题，当执行
./a.out的时候出现：“Segmentation fault(段错误)”，下面就是最近的一次情况（目的只是测试strcat函数）：
 26 #include<stdio.h>
 25 #include<string.h>
 26 int main()
 27 {
 28         char *str1 = "Wang";
 29         char *str2 = "Lei";
 30         printf("%s/n", strcat(str1, str2));
 31         return 0;
 32 }
（前面的数字只是标号，不是程序部分）。
编译运行：
[root@localhost C]# cc my_strcat.c
[root@localhost C]# ./a.out
Segmentation fault
于是开始查找信息，下面是查找的情况。(我把上面代码的调试放在了最后！是一个内存溢出的错误！)
=======================>8=[http://www.xianjiaxin198.com/](http://www.xianjiaxin198.com/)[西安嘉信有限公司官网](http://www.xianjiaxin198.com/)===================================================
(没经作者容许就剪贴过来了，在此表示歉意！)
**1）往受到系统保护的内存地址写数据**
有些内存是内核占用的或者是其他程序正在使用，为了保证系统正常工作，所以会受到系统的保护，而不能任意访问.
`#include <stdio.h>`
int
main()
{
int i = 0;
scanf ("%d", i); /* should have used &i */
printf ("%d/n", i);
return 0;
}
编译和执行一下, 咋一看，好像没有问题哦，不就是读取一个数据然后给输出来吗？
> 
falcon@falcon:~/temp$ gcc -g -o segerr segerr.c –加-g选项查看调试信息
falcon@falcon:~/temp$ gdb ./segerr
GNU gdb 6.4-debian
Copyright 2005 Free Software Foundation, Inc.
GDB is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type “show copying” to see the conditions.
There is absolutely no warranty for GDB. Type “show warranty” for details.
This GDB was configured as “i486-[Linux](http://lib.csdn.net/base/linux)-gnu”…Using host libthread_db library
 “/ lib/tls/i686/cmov/libthread_db.so.1″.
(gdb) l –用l(list)显示我们的源代码
1 #include <stdio.h>
2
3 int
4 main()
5 {
6 int i = 0;
7
8 scanf (”%d”, i); /* should have used &i */
9 printf (”%d/n”, i);
10 return 0;
(gdb) b 8 –用b(break)设置断点
Breakpoint 1 at 0×80483b7: file segerr.c, line 8.
(gdb) p i –用p(print)打印变量i的值[看到没，这里i的值是0哦]
$1 = 0
(gdb) r –用r(run)运行，直到断点处
Starting program: /home/falcon/temp/segerr
Breakpoint 1, main () at segerr.c:8
8 scanf (”%d”, i); /* should have used &i */ –[试图往地址0处写进一个值]
(gdb) n –用n(next)执行下一步
10
Program received signal SIGSEGV, Segmentation fault.
0xb7e9a1ca in _IO_vfscanf () from /lib/tls/i686/cmov/libc.so.6
(gdb) c –在上面我们接收到了SIGSEGV,然后用c(continue)继续执行
Continuing.
Program terminated with signal SIGSEGV, Segmentation fault.
The program no longer exists.
(gdb) quit –退出gdb
果然
我们“不小心”把&i写成了i
而我们刚开始初始化了i为0,这样我们不是试图向内存地址0存放一个值吗？
[补充：
可以通过man 7 signal查看SIGSEGV的信息。
> 
falcon@falcon:~/temp$ man 7 signal | grep SEGV
Reformatting signal(7), please wait…
SIGSEGV 11 Core Invalid memory reference
例子2：
```
#include <stdio.h>
int
main()
{
char *p;
```
p = NULL;
*p = ‘x’;
printf(”%c”, *p);
return 0;
}
很容易发现，这个例子也是试图往内存地址0处写东西。
这里我们通过gdb来查看段错误所在的行
> 
falcon@falcon:~/temp$ gcc -g -o segerr segerr.c
falcon@falcon:~/temp$ gdb ./segerr
GNU gdb 6.4-debian
Copyright 2005 Free Software Foundation, Inc.
GDB is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type “show copying” to see the conditions.
There is absolutely no warranty for GDB. Type “show warranty” for details.
This GDB was configured as “i486-linux-gnu”…Using host libthread_db library “/lib/tls/i686/cmov/libthread_db.so.1″.
(gdb) r –直接运行，我们看到抛出段错误以后，自动显示出了出现段错误的行，这就是一个找出段错误的方法
Starting program: /home/falcon/temp/segerr
Program received signal SIGSEGV, Segmentation fault.
0×08048516 in main () at segerr.c:10
10 *p = ‘x’;
(gdb)
**2）内存越界(数组越界，变量类型不一致等)**
`#include <stdio.h>`
int
main()
{
char test[1];
printf(”%c”, test[1000000000]);
return 0;
}
这里是比较极端的例子，但是有时候可能是会出现的，是个明显的数组越界的问题
或者是这个地址是根本就不存在的
例子4：
`#include <stdio.h>`
int
main()
{
int b = 10;
printf(”%s/n”, b);
return 0;
}
我们试图把一个整数按照字符串的方式输出出去，这是什么问题呢？
由于还不熟悉调试动态链接库，所以
我只是找到了printf的源代码的这里
声明部分：
```
int pos =0 ,cnt_printed_chars =0 ,i ;
unsigned char *chptr ;
va_list ap ;
/* %s格式控制部分：*/
case 's':
chptr =va_arg (ap ,unsigned char *);
i =0 ;
while (chptr [i ])
{...
cnt_printed_chars ++;
putchar (chptr [i ++]);
}
```
由于我没有仔细分析代码，大致的原因也可能是地址越界的原因？不过我可不确定哦。
如果大家知道怎么调试printf函数，麻烦帮忙找出越界的真正原因吧，这个段错误也可能是
处在va_start和va_arg等函数里头？或者直接看看这个这里的printf源代码的分析，看看是否
可以找出出错的地方：
http://www.wangchao[.NET](http://lib.csdn.net/base/dotnet).cn/bbsdetail_47325.html
类似的，还有诸如：sprintf等的格式控制问题
比如，试图把char型或者是int的按照%s输出或存放起来，如：
```
#include <stdio.h>
#include <string.h>
char c=’c';
int i=10;
char buf[100];
```
printf(”%s”, c); //试图把char型按照字符串格式输出
printf(”%s”, i); //试图把int型按照字符串输出
memset(buf, 0, 100);
sprintf(buf, “%s”, c); //试图把char型按照字符串格式转换
memset(buf, 0, 100);
sprintf(buf, “%s”, i); //试图把int型按照字符串转换
**3）其他**
其实大概的原因都是一样的，就是段错误的定义。
但是更多的容易出错的地方就要自己不断积累，不段发现，或者吸纳前人已经积累的经验，并且注意避免再次发生。
例如：
<1>定义了指针后记得初始化，在使用的时候记得判断是否为NULL
<2>在使用数组的时候是否被初始化，数组下标是否越界，数组元素是否存在等
<3>在变量处理的时候变量的格式控制是否合理等
一个比较不错的例子：
我在进行一个多线程编程的例子里头，定义了一个线程数组
#define THREAD_MAX_NUM
pthread_t thread[THREAD_MAX_NUM];
用pthread_create创建了各个线程，然后用pthread_join来等待线程的结束
刚开始
我就直接等待，在创建线程都成功的时候，pthread_join能够顺利等待各个线程结束
但是一旦创建线程失败，那用pthread_join来等待那个本不存在的线程时自然会存在访问不存在的内存的情况，从而导致段错误的发生
后来
通过不断调试和思考，并且得到网络上资料的帮助，找到了上面的出错原因和解决办法
解决办法是：
在创建线程之前，先初始化我们的线程数组
在等待线程的结束的时候，判断线程是否为我们的初始值
如果是的话，说明我们的线程并没有创建成功，所以就不能等拉。
上面给出了很常见的几种出现段错误的地方，这样在遇到它们的时候就容易避免拉。
但是人有时候肯定也会有疏忽的，甚至可能还是会经常出现上面的问题或者其他常见的问题
所以对于一些大型一点的程序，如何跟踪并找到程序中的段错误位置就是需要掌握的一门技巧拉。
4。如何发现程序中的段错误？
有个网友对这个做了比较全面的总结，除了感谢他外，我把地址弄了过来。
文章名字叫《段错误bug的调试》
地址是：http://www.cublog.cn/u/5251/showart.[PHP](http://lib.csdn.net/base/php)?id=173718
应该说是很全面的。
而我常用的调试方法有：
1）在程序内部的关键部位输出(printf)信息，那样可以跟踪 段错误 在代码中可能的位置
为了方便使用这种调试方法，可以用条件编译指令#ifdef DEBUG和#endif把printf函数给包含起来，编译的时候加上-DDEBUG参数就可以查看调试信息。反之，不加上该参数进行调试就可以。
2）用gdb来调试，在运行到段错误的地方，会自动停下来并显示出错的行和行号
这个应该是很常用的，如果需要用gdb调试，记得在编译的时候加上-g参数，用来显示调试信息
对于这个，网友在《段错误bug的调试》文章里创造性的使用这样的方法，使得我们在执行程序的时候就可以动态扑获段错误可能出现的位置：
通过扑获SIGSEGV信号来触发系统调用gdb来输出调试信息。
如果加上上面提到的条件编译，那我们就可以非常方便的进行段错误的调试拉。
===================================>8==========================================
原来“Segmentation fault“的调试是一个很大的知识点！（！嘿）
，在上面的剪贴中，谈到的《段错误bug的调试》的文章，这里也粘贴过来！
=================================>8==========================================
**段错误bug的调试**
我们在用C/C++语言写程序的时侯，内存管理的绝大部分工作都是需要我们来做的。实际上，内存管理是一个比较繁琐的工作，无论你多高明，经验多丰富，难免会在此处犯些小错误，而通常这些错误又是那么的浅显而易于消除。但是手工“除虫”（debug），往往是效率低下且让人厌烦的，本文将就"段错误"这个内存访问越界的错误谈谈如何快速定位这些"段错误"的语句。
下面将就以下的一个存在段错误的程序介绍几种调试方法：
     1  dummy_function (void)
     2  {
     3          unsigned char *ptr = 0x00;
     4          *ptr = 0x00;
     5  }
     6
     7  int main (void)
     8  {
     9          dummy_function ();
    10
    11          return 0;
    12  }
作为一个熟练的C/C++程序员，以上代码的bug应该是很清楚的，因为它尝试操作地址为0的内存区域，而这个内存区域通常是不可访问的禁区，当然就会出错了。我们尝试编译运行它:
xiaosuo@gentux test $ ./a.out
段错误
果然不出所料，它出错并退出了。
1.利用gdb逐步查找段错误:
这种方法也是被大众所熟知并广泛采用的方法，首先我们需要一个带有调试信息的可执行程序，所以我们加上“-g -rdynamic"的参数进行编译，然后用gdb调试运行这个新编译的程序,具体步骤如下:
xiaosuo@gentux test $ gcc -g -rdynamic d.c
xiaosuo@gentux test $ gdb ./a.out
GNU gdb 6.5
Copyright (C) 2006 Free Software Foundation, Inc.
GDB is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type "show copying" to see the conditions.
There is absolutely no warranty for GDB.  Type "show warranty" for details.
This GDB was configured as "i686-pc-linux-gnu"...Using host libthread_db library "/lib/libthread_db.so.1".
(gdb) r
Starting program: /home/xiaosuo/test/a.out
Program received signal SIGSEGV, Segmentation fault.
0x08048524 in dummy_function () at d.c:4
4               *ptr = 0x00;
(gdb)                      
哦？！好像不用一步步调试我们就找到了出错位置d.c文件的第4行，其实就是如此的简单。
从这里我们还发现进程是由于收到了SIGSEGV信号而结束的。通过进一步的查阅文档(man 7 signal)，我们知道SIGSEGV默认handler的动作是打印”段错误"的出错信息，并产生Core文件，由此我们又产生了方法二。
2.分析Core文件：
Core文件是什么呢？
The  default action of certain signals is to cause a process to terminate and produce a core dump file, a disk file containing an image of the process's memory  at the time of termination.  A list of the signals which cause
 a process to dump core can be found in signal(7).
以上资料摘自man page(man 5 core)。不过奇怪了，我的系统上并没有找到core文件。后来，忆起为了渐少系统上的拉圾文件的数量（本人有些洁癖，这也是我喜欢Gentoo的原因之一），禁止了core文件的生成，查看了以下果真如此，将系统的core文件的大小限制在512K大小，再试:
xiaosuo@gentux test $ ulimit -c
0
xiaosuo@gentux test $ ulimit -c 1000
xiaosuo@gentux test $ ulimit -c
1000
xiaosuo@gentux test $ ./a.out
段错误 (core dumped)
xiaosuo@gentux test $ ls
a.out  core  d.c  f.c  g.c  pango.c  test_iconv.c  test_regex.c
core文件终于产生了，用gdb调试一下看看吧:
xiaosuo@gentux test $ gdb ./a.out core
GNU gdb 6.5
Copyright (C) 2006 Free Software Foundation, Inc.
GDB is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type "show copying" to see the conditions.
There is absolutely no warranty for GDB.  Type "show warranty" for details.
This GDB was configured as "i686-pc-linux-gnu"...Using host libthread_db library "/lib/libthread_db.so.1".
warning: Can't read pathname for load map: 输入/输出错误.
Reading symbols from /lib/libc.so.6...done.
Loaded symbols for /lib/libc.so.6
Reading symbols from /lib/ld-linux.so.2...done.
Loaded symbols for /lib/ld-linux.so.2
Core was generated by `./a.out'.
Program terminated with signal 11, Segmentation fault.
#0  0x08048524 in dummy_function () at d.c:4
4               *ptr = 0x00;
哇，好历害，还是一步就定位到了错误所在地，佩服一下Linux/Unix系统的此类设计。
接着考虑下去，以前用windows系统下的ie的时侯，有时打开某些网页，会出现“运行时错误”，这个时侯如果恰好你的机器上又装有windows的编译器的话，他会弹出来一个对话框，问你是否进行调试，如果你选择是，编译器将被打开，并进入调试状态，开始调试。
Linux下如何做到这些呢？我的大脑飞速地旋转着，有了，让它在SIGSEGV的handler中调用gdb，于是第三个方法又诞生了:
3.段错误时启动调试:
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
void dump(int signo)
{
        char buf[1024];
        char cmd[1024];
        FILE *fh;
        snprintf(buf, sizeof(buf), "/proc/%d/cmdline", getpid());
        if(!(fh = fopen(buf, "r")))
                exit(0);
        if(!fgets(buf, sizeof(buf), fh))
                exit(0);
        fclose(fh);
        if(buf[strlen(buf) - 1] == '/n')
                buf[strlen(buf) - 1] = '/0';
        snprintf(cmd, sizeof(cmd), "gdb %s %d", buf, getpid());
        system(cmd);
        exit(0);
}
        void
dummy_function (void)
{
        unsigned char *ptr = 0x00;
        *ptr = 0x00;
}
        int
main (void)
{
        signal(SIGSEGV, &dump);
        dummy_function ();
        return 0;
}
编译运行效果如下:
xiaosuo@gentux test $ gcc -g -rdynamic f.c
xiaosuo@gentux test $ ./a.out
GNU gdb 6.5
Copyright (C) 2006 Free Software Foundation, Inc.
GDB is free software, covered by the GNU General Public License, and you are
welcome to change it and/or distribute copies of it under certain conditions.
Type "show copying" to see the conditions.
There is absolutely no warranty for GDB.  Type "show warranty" for details.
This GDB was configured as "i686-pc-linux-gnu"...Using host libthread_db library "/lib/libthread_db.so.1".
Attaching to program: /home/xiaosuo/test/a.out, process 9563
Reading symbols from /lib/libc.so.6...done.
Loaded symbols for /lib/libc.so.6
Reading symbols from /lib/ld-linux.so.2...done.
Loaded symbols for /lib/ld-linux.so.2
0xffffe410 in __kernel_vsyscall ()
(gdb) bt
#0  0xffffe410 in __kernel_vsyscall ()
#1  0xb7ee4b53 in waitpid () from /lib/libc.so.6
#2  0xb7e925c9 in strtold_l () from /lib/libc.so.6
#3  0x08048830 in dump (signo=11) at f.c:22
#4  <signal handler called>
#5  0x0804884c in dummy_function () at f.c:31
#6  0x08048886 in main () at f.c:38
怎么样？是不是依旧很酷？
以上方法都是在系统上有gdb的前提下进行的，如果没有呢？其实glibc为我们提供了此类能够dump栈内容的函数簇，详见/usr/include/execinfo.h（这些函数都没有提供man page，难怪我们找不到），另外你也可以通过[gnu的手册](http://www.gnu.org/software/libc/manual/html_node/Backtraces.html) 进行学习。
4.利用backtrace和objdump进行分析:
重写的代码如下:
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
/* A dummy function to make the backtrace more interesting. */
        void
dummy_function (void)
{
        unsigned char *ptr = 0x00;
        *ptr = 0x00;
}
void dump(int signo)
{
        void *array[10];
        size_t size;
        char **strings;
        size_t i;
        size = backtrace (array, 10);
        strings = backtrace_symbols (array, size);
        printf ("Obtained %zd stack frames./n", size);
        for (i = 0; i < size; i++)
                printf ("%s/n", strings[i]);
        free (strings);
        exit(0);
}
        int
main (void)
{
        signal(SIGSEGV, &dump);
        dummy_function ();
        return 0;
}
编译运行结果如下：
xiaosuo@gentux test $ gcc -g -rdynamic g.c
xiaosuo@gentux test $ ./a.out
Obtained 5 stack frames.
./a.out(dump+0x19) [0x80486c2]
[0xffffe420]
./a.out(main+0x35) [0x804876f]
/lib/libc.so.6(__libc_start_main+0xe6) [0xb7e02866]
./a.out [0x8048601]
这次你可能有些失望,似乎没能给出足够的信息来标示错误,不急,先看看能分析出来什么吧,用objdump反汇编程序,找到地址0x804876f对应的代码位置:[http://www.xianjiaxin198.com/](http://www.xianjiaxin198.com/)[西安嘉信有限公司官网](http://www.xianjiaxin198.com/)
xiaosuo@gentux test $ objdump -d a.out 8048765:       e8 02 fe ff ff          call   804856c <signal@plt>
 804876a:       e8 25 ff ff ff          call   8048694 <dummy_function>
804876f :       b8 00 00 00 00          mov    $0x0,%eax
 8048774:       c9                      leave
我们还是找到了在哪个函数(dummy_function)中出错的,信息已然不是很完整,不过有总比没有好的啊!
后记:
本文给出了分析"段错误"的几种方法,不要认为这是与孔乙己先生的"回"字四种写法一样的哦,因为每种方法都有其自身的适用范围和适用环境,请酌情使用,或遵医嘱。
================================>8===============================================
哈，现在可以用上面的方法调试一下自己的程序了！
为了方便调试，我把strcat函数也写进来（这里的strcat是自己些的）
  1 
  2 #include<stdio.h>
  3 char *
  4 strcat(dest, src)
  5         char *dest;
  6         const char *src;
  7 {
  8         char *s1 = dest;
  9         const char *s2 = src;
 10         while(*s1++ != '/0')
 11                 ;
 12 
 13         s1 -= 2;
 14 
 15         for(; *s2 != '/0'; *++s1 = *s2++)
 16                 ;
 17 
 18         return dest;
 19 }
 20 int main()
 21 {
 22         char *str1 = "wang";
 23         char *str2 = "lei";
 24 
 25         printf("%s/n", strcat(str1, str2));
 26         return 0;
 27 }
 28
gdb调试：  
//调试说明：编译后第一次调试的结果可能与第二次调试的结果不同。比如，在下面的调试中，当收到SIGSEGV，第一次并不直接结束，等待输入c 命令后才结束。第二次的时候当收到SIGSEGV后程序就终止了！当我再次编译程序后调试又是第一次的结果！甚至当我输入命令p s1的时候，打印的却是s2的内容！
[root@localhost C]# cc -g my_strcat.c
[root@localhost C]# gdb ./a.out
GNU gdb Fedora (6.8-37.el5)
Copyright (C) 2008 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-redhat-linux-gnu"...
(gdb) l   //注意这里是字母L的小写，是显示程序的命令（开始是显示包含main()函数的一部分代码，我也不知道能不能显示全部的代码，不管这么多了，其实也不需要显示全部！）
13              s1 -= 2;
14
15              for(; *s2 != '/0'; *++s1 = *s2++)
16                      ;
17
18              return dest;
19      }
20      int main()
21      {
22              char *str1 = "wang";
(gdb) b 15   //把15行设置为断点，也就是说程序在14行结尾，15行之前断了！
Breakpoint 1 at 0x4004ee: file my_strcat.c, line 15.
(gdb) r      //从头开始运行程序啦！
Starting program: /root/C/a.out 
Breakpoint 1, strcat (dest=0x400658 "wang", src=0x40065d "lei")
    at my_strcat.c:15
15              for(; *s2 != '/0'; *++s1 = *s2++)
(gdb) p s1  //把14行之前的最后一个s1打印出来看看！
$1 = 0x40065b "g"
(gdb) p s2
$2 = 0x40065d "lei"
//到这里为止，15还没有被执行！
(gdb) c   //现在开始执行15行啦！
Continuing.
Program received signal SIGSEGV, Segmentation fault.
0x0000000000400500 in strcat (dest=0x400658 "wang", src=0x40065d "lei")
    at my_strcat.c:15  
//程序在执行完第一个“*++s1 = *s2++” 时，s1=0x40065c "",  s2=0x40065d "lei", 也就是说把"lei"中的l放在内存
0x40065c的位置（事实表明这个操作并未成功！至于为啥失败，我也纳闷着呢！如果说把“lei”中的l不能放在
0x40065d的位置，这道好理解：缓冲区绝对溢出，并且dest与src重叠！所以我就委屈这样理解了：所谓溢出指的是该空间内已经不能存放一个完整的字符串了！）。
15              for(; *s2 != '/0'; *++s1 = *s2++)
(gdb) p s1
$3 = 0x40065c ""
(gdb) p s2
$4 = 0x40065d "lei"
(gdb) c
Continuing.
Program terminated with signal SIGSEGV, Segmentation fault.
The program no longer exists.
(gdb) q
[root@localhost C]# 
这下清楚了，原来是一个内存溢出的错误！所以在用到strcat的时候一定要保证dest的内存空间足以能存放的下后面的字符串！否则将造成段错误[http://www.xianjiaxin198.com/](http://www.xianjiaxin198.com/)[西安嘉信有限公司官网](http://www.xianjiaxin198.com/)Segmentation
 fault.
