# 一次segfault错误的排查过程 - xqhrs232的专栏 - CSDN博客
2016年10月21日 15:11:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1215
原文地址::[http://blog.csdn.net/zhaohaijie600/article/details/45246569](http://blog.csdn.net/zhaohaijie600/article/details/45246569)
相关文章
1、[LINUX 段错误查找记录 -- segfault at fffffffffffffff9 ip 0000003c97e7b81c
 sp 00007fffad7c0638 error 4 in lib](http://blog.csdn.net/senlin1202/article/details/51011129)----[http://blog.csdn.net/senlin1202/article/details/51011129](http://blog.csdn.net/senlin1202/article/details/51011129)
                                                                                                    一次segfault错误的排查过程
正常运行了几年的程序忽然崩溃了，由于机器没有设置CORE文件，无法从CORE中取得错误信息，程序运行在centOS 7上， 本来对centOS用的也是不熟，只能边查资料边查问题。
首先、我需要确认程序是否真的崩溃了，还是别人误操作关闭了。如果程序真的崩溃了，会在系统中留下痕迹，我查了一下，在messages文件中发现了一条信息：
xxxxx.o[2374]: segfault at7f0ed0bfbf70 ip 00007f0edd646fe7 sp 00007f0ed3603978 error 4 inlibc-2.17.so[7f0edd514000+1b6000]
由上面信息看出，系统确实是崩溃了，发生了段错误。
查看messages需要root权限，用命令：cat /var/log/messages 就可以了，还有一个命令dmesg也可以查到上面的信息。
从上面的信息，我们可以得到以下信息：
1、从libc-2.17.so[7f0edd514000+1b6000]可以看出错误发生在libc上，libc在此程序中映射的内存基址为7f0edd514000，这个信息是个坏消息，这个so上的东西太多了；
2、segfault at和error 4这两条信息可以得出是内存读出错，4的意义如下，可以对照参考：
bit2:值为1表示是用户态程序内存访问越界，值为0表示是内核态程序内存访问越界
bit1: 值为1表示是写操作导致内存访问越界，值为0表示是读操作导致内存访问越界
bit0: 值为1表示没有足够的权限访问非法地址的内容，值为0表示访问的非法地址根本没有对应的页面，也就是无效地址
4正好为用户态内存读操作访问出界。
3、7f0ed0bfbf70，00007f0edd646fe7，00007f0ed3603978这三个值：第一个值为出错的地址，用处不大；第二个值为发生错误时指令的地址，这个值在有些错误时是错误的，下面会讲一下，第三个值为堆栈指针。
除了以上信息，就是六七万行的代码。感觉没有太大的指望。
C++段错误就几类，读写错误，这个主要是参数没有控制好，这种错误比较常见，我们经常把NULL指针、未初始化或非法值的指针传递给函数，从而引出此错误；指令地址出错，这类错误主要是由虚函数，回调函数引起，最常出现的是虚函数，由于虚函数保存在类变量中，如果不小心用了非安全函数，就可能把虚数指针覆盖掉，从而影响出现错误。但指令地址出错的情况相对参数出错来讲还是要少很多的，因为用到此功能的还是需要一定的水平的，不容易犯一些低级错误。
从上面分析的第二点来看，基本上属于读写错误，但从六七万行代码找出问题，可能性不大，只能缩小范围，我决定从上面提到的三点，找到出错的函数，然后再从代码中找出所有出错函数调用的地方来定位问题。由于错误指出出错的组件为libc，而且基本上是参数出现，所以发现错误的指令地址应该是可信的，我们可以根据指令地址查出是哪个函数。指令地址为：00007f0edd646fe7 ，libc指令的基地址为：7f0edd514000，可以根据这两个值计算一下该指令的相对地址为132FE7，下面我们需要找到相对代码段地址为132FE7的地方为什么函数。
开始我想得到反汇编代码，但这个组件代码太多，看不到头，于是我找了个取巧的办法，查看导出函数和基地址，结果所以还是很多，我就用132和133进行了一下过滤，得出以下信息
[root@localhostlib64]# objdump -tT libc-2.17.so | grep 132
000000000008284fl     F .text  000000000000001b              _L_unlock_1325
0000000000082ebfl     F .text  000000000000001c              _L_lock_11322
000000000010b952l     F .text  000000000000001b              _L_unlock_132
000000000010ce62l     F .text  000000000000001b              _L_unlock_132
00000000001132e0l     F .text  00000000000001cf              ruserok2_sa
00000000000f1320l     F .text  00000000000001ce              __ecvt_r
00000000000bf370l     F .text  0000000000000132              __statfs_link_max
0000000000132080l     F .text  0000000000000068              __nss_gshadow_lookup
0000000000132f50l     F .text  0000000000000fd9              __strncmp_sse42
00000000001320f0l     F .text  00000000000000a5              __strchr_sse42
0000000000132020l     F .text  000000000000005e              __nss_aliases_lookup
00000000001321a0l     F .text  0000000000000da9              __strcmp_sse42
00000000001153b0g     F .text  0000000000000132              setnetgrent
00000000000f1320g     F .text  00000000000001ce              ecvt_r
0000000000112b50g     F .text  0000000000000132              ether_ntohost
00000000000f1320g    DF .text  00000000000001ce  GLIBC_2.2.5 ecvt_r
0000000000112b50g    DF .text  0000000000000132  GLIBC_2.2.5 ether_ntohost
00000000001153b0g    DF .text  0000000000000132  GLIBC_2.2.5 setnetgrent
[root@localhostlib64]# objdump -tT libc-2.17.so | grep 133
000000000006e3cal     F .text  000000000000001b              _L_unlock_133
0000000000075055l     F .text  0000000000000018              _L_unlock_133
000000000008286al     F .text  000000000000001f              _L_unlock_1335
000000000008305al     F .text  000000000000001b              _L_lock_13385
0000000000133f30l     F .text  000000000000019a              __strrchr_sse42
注意，我标红的部分，132f50和132fe7很接近，很大可能是这个函数出现，而且又是读地址非法，这个函数有可能会出错这个问题，__strncmp_sse42这个函数是被strncmp调用的，看到这个函数基本上可以确定应该是这个函数惹得祸。
我又写了两行代码确认了一下
strncmp(0,“1234”, 5);
strncmp(“1234”,0, 5);
分别编绎成两个应用运行，出错，messages中的错误信息如下：
Apr23 01:38:03 localhost kernel: a.out[3254]: segfault at 0 ip 00007f58a5386f7c sp00007fffb3de5d18 error 4 in libc-2.17.so[7f58a5254000+1b6000]
Apr23 01:39:34 localhost kernel: a.out[3267]: segfault at 0 ip 00007f8bb1908f80 sp00007fff61695408 error 4 in libc-2.17.so[7f8bb17d6000+1b6000]
计算了一下，出错的相对地址为：132F80和132F7C，和我们遇到的错误地址很接近，于是，把__strncmp_sse42的汇编代码打印了部分如下：
Dump of assemblercode for function __strncmp_sse42:
   0x00007ffff732ef50 <+0>: test   %rdx,%rdx
   0x00007ffff732ef53 <+3>: je     0x7ffff732ff14<__strncmp_sse42+4036>
   0x00007ffff732ef59 <+9>: cmp    $0x1,%rdx
   0x00007ffff732ef5d <+13>: je     0x7ffff732ff20<__strncmp_sse42+4048>
   0x00007ffff732ef63 <+19>: mov    %rdx,%r11
   0x00007ffff732ef66 <+22>: mov    %esi,%ecx
   0x00007ffff732ef68 <+24>: mov    %edi,%eax
   0x00007ffff732ef6a <+26>: and    $0x3f,%rcx
   0x00007ffff732ef6e <+30>: and    $0x3f,%rax
   0x00007ffff732ef72 <+34>: cmp    $0x30,%ecx
   0x00007ffff732ef75 <+37>: ja     0x7ffff732efc0 <__strncmp_sse42+112>
   0x00007ffff732ef77 <+39>: cmp    $0x30,%eax
   0x00007ffff732ef7a <+42>: ja     0x7ffff732efc0 <__strncmp_sse42+112>
 0x00007ffff732ef7c <+44>: movdqu(%rdi),%xmm1
           0x00007ffff732ef80 <+48>:movdqu (%rsi),%xmm2
   0x00007ffff732ef84 <+52>: pxor   %xmm0,%xmm0
   0x00007ffff732ef88 <+56>: pcmpeqb%xmm1,%xmm0
   0x00007ffff732ef8c <+60>: pcmpeqb%xmm2,%xmm1
   0x00007ffff732ef90 <+64>: psubb  %xmm0,%xmm1
   0x00007ffff732ef94 <+68>: pmovmskb%xmm1,%edx
   0x00007ffff732ef98 <+72>: sub    $0xffff,%edx
   0x00007ffff732ef9e <+78>: jne    0x7ffff732ff00 <__strncmp_sse42+4016>
   0x00007ffff732efa4 <+84>: sub    $0x10,%r11
   0x00007ffff732efa8 <+88>: jbe    0x7ffff732ff14 <__strncmp_sse42+4036>
   0x00007ffff732efae <+94>: add    $0x10,%rsi
   0x00007ffff732efb2 <+98>: add    $0x10,%rdi
   0x00007ffff732efb6 <+102>: nopw   %cs:0x0(%rax,%rax,1)
   0x00007ffff732efc0 <+112>: and    $0xfffffffffffffff0,%rsi
   0x00007ffff732efc4 <+116>: and    $0xfffffffffffffff0,%rdi
   0x00007ffff732efc8 <+120>: mov    $0xffff,%edx
   0x00007ffff732efcd <+125>: xor    %r8d,%r8d
   0x00007ffff732efd0 <+128>: and    $0xf,%ecx
   0x00007ffff732efd3 <+131>: and    $0xf,%eax
   0x00007ffff732efd6 <+134>: pxor   %xmm0,%xmm0
   0x00007ffff732efda <+138>: cmp    %eax,%ecx
   0x00007ffff732efdc <+140>: je     0x7ffff732f010 <__strncmp_sse42+192>
   0x00007ffff732efde <+142>: ja     0x7ffff732efe7 <__strncmp_sse42+151>
   0x00007ffff732efe0 <+144>: mov    %edx,%r8d
   0x00007ffff732efe3 <+147>: xchg   %eax,%ecx
   0x00007ffff732efe4 <+148>: xchg   %rsi,%rdi
0x00007ffff732efe7<+151>: movdqa (%rdi),%xmm2
       0x00007ffff732efeb <+155>: movdqa(%rsi),%xmm1
   0x00007ffff732efef <+159>: lea    0xf(%rax),%r9
   0x00007ffff732eff3 <+163>: sub    %rcx,%r9
   0x00007ffff732eff6 <+166>: lea    0x4d4c3(%rip),%r10
红色部分为我自己写的测试程序出错的地址，
绿色部分为所要查的程序出错的地址，从这部分可以看到，出错的参数为strncmp的第二个参数。
四、多线程
由于用UE对源代码进行查找，还好只有335处调用，对第二个参数为常量和不被调用的代码再排除，只有20多处可疑。
现在需要再分析一下，第二个参数读非法，那么变量如果在栈中话，需要访问到栈外的空间，这样，如果是第二个参数为栈变量的话，第三个参数值应该会很大。如果在堆变量，情况就比较复杂了，由于第三个参数值都很少，最大的为128，所以又排除了栈变量出错的可能性，从上面的数据来看，出错的地址比栈小了很多，也证明了不是栈变量出错。最后只剩下8处，我对这8处仔细查了一下，感觉都没有问题，参数控制，变量长度，控制得都没有问题。
如果代码上没有问题，那么问题就比较难查了，我再进行分析一下，函数用户和变量上表面上没有问题，那么就可能出现几种可能：变量指针覆盖，内存移动。变量指针覆盖在实际中出现的概率比较大，往往使用了strcpy和memcpy之类的函数，没有进行边界值检查，把一些指针的值覆盖了，对于这种情况，我对代码进行了检查，可能性很小，而且上面给出的出错的参数值不像随意覆盖的值。如果这种情况可能性很小，对于一个变量只在单个线程被访问的情况出错的可能性也很小，于是，我又排除了6处。
剩下的两处代码在一起的，参数为一个类的内部变量，这个类可能会被多个线程访问，所以此此出现的可能性很大，我仔细看了一下，边界检查也做了，线程互斥访问控制也进行了处理，没有明显的问题，还好用到这个变量的只有一个文件，我对整个文件对这个变量进行搜索，发现了一个函数realloc。感觉可能是这个函数导致的问题，此变量存放的是一个数组，数组会不断增长，当空间不够时，就用realloc再重申请，看到这个函数后，心中一阵激动，感觉问题就在这儿，我把这几段代码仔细看了几遍，访问控制，锁住修改，动态申请，最终发现了一个问题，在使用realloc时，临界区没有保护好。
事情似乎比较明朗了，在一个线程对这个变量指针进行访问时（即用strnmcp处），线程切换到另一个线程对这个变量指针进行了realloc操作，realloc重新申请了新的空间，把老的数据移动的新的空间，然后把旧空间处的内存彻底释放掉了，然后又切换到原先的线程，执行strncmp函数，这个函数用的指针参数还是旧的空间地址，而旧的空间已经被释放了，所以出现了访问非法的错误。
我查了一下程序日志，发现出现错误时，正好需要重新申请空间，应该是这个问题导致的。
这种情况发生的概率很低，很难重现，因为realloc和strncmp函数执行时间都很短，而且realloc的概率不高，所以运行了4，5年才出现错误，但是总体来讲，还是比较幸运的，如果旧的空间没有被释放，那么接下来的各种操作全在旧的空间上，可能会导致更大的损失。
———赵海杰
20150424
