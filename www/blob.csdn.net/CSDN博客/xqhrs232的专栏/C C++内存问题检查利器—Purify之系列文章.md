# C/C++内存问题检查利器—Purify之系列文章 - xqhrs232的专栏 - CSDN博客
2012年04月24日 22:48:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：705
原文地址::[http://blog.csdn.net/haoel/article/details/2900](http://blog.csdn.net/haoel/article/details/2900)
相关网帖
1.[C/C++内存问题检查利器—Purify （一）](http://blog.csdn.net/haoel/article/details/2900)----[http://blog.csdn.net/haoel/article/details/2900](http://blog.csdn.net/haoel/article/details/2900)
2.[C/C++内存问题检查利器—Purify （二）](http://blog.csdn.net/haoel/article/details/2901)----[http://blog.csdn.net/haoel/article/details/2901](http://blog.csdn.net/haoel/article/details/2901)
3.[C/C++内存问题检查利器—Purify （三）](http://blog.csdn.net/haoel/article/details/2902)----[http://blog.csdn.net/haoel/article/details/2902](http://blog.csdn.net/haoel/article/details/2902)
4.[C/C++内存问题检查利器—Purify （四）](http://blog.csdn.net/haoel/article/details/2903)----[http://blog.csdn.net/haoel/article/details/2903](http://blog.csdn.net/haoel/article/details/2903)
5.[C/C++内存问题检查利器—Purify （五）](http://blog.csdn.net/haoel/article/details/2904)----[http://blog.csdn.net/haoel/article/details/2904](http://blog.csdn.net/haoel/article/details/2904)
**C／C++内存问题检查利器——Purify**
**陈皓**
## 一、引言
我们都知道软件的测试（在以产品为主的软件公司中叫做QA—Quality Assessment）占了整个软件工程的30%
 -50%，但有这么一种说法，即使是最优秀测试专家设计出来的测试案例，也无法彻底地检测到内存上的问题。
使用C/C++开发的团队一定有被其内存问题折磨过的经历，内存问题一直是C/C++开发人员的心头之痛。特别当程序越来越多时，类的继承和关联越来越多时，内存问题也就越来越多，很多时候，开发人员在不经意的时候就带入了内存问题。这是C/C++世界中很难避免的东西，哪怕是有10年以上开发经验的老手，也难以避免或是杜绝内存问题。
而且，内存的问题是让人很难察觉的，特别是对于内存问题排名第一的Memory Leak来说，在几万行代码中出现Memory
 Leak的机率几乎是100%，而且要靠一般的手段是很难检测出这种程序错误的。它并不像“野指针”或是“数组越界”那么容易暴露出来（这些会让程序异常退出，而Memory Leak则不会）。当你发现你的服务器端的程序，每隔一个月（或是更长的时间）就把服务器上的内存全部耗尽时，让服务器有规律地每过几个月就当机一次，那么你的程序可能是中了Memory
 Leak了，同时，你会发现在数十万行代码中寻找这种Memory Leak无异于大海捞针。
于是，正如《黑客帝国II》中描述的那样，当你的程序越来越大，越来越复杂的时候，你会发现程序就越来越不受你的控制，有一些让你内存出现问题乃至让你应用程序崩溃的变量，他们生存在系统的边缘，你怎么找也找不到，这种情况下，除了用程序测试程序，别无其它的方法。对于C/C++内存问题中的Memory
 Leak这种顶级杀手，那怕最牛的程序员再加上最牛的系统架构师也很难把其找出来，对此，我们只有依靠程序，用程序去寻找这种系统的BUG。这么让我们事半功倍。
在我们寻求解决内存问题的同时，让我们所感到幸运的时，目前，已经有许多小的软件可供我们选择，如[MallocDebug](http://www.cs.colorado.edu/homes/zorn/public_html/MallocDebug.html)，[Valgrind](http://developer.kde.org/~sewardj/)，[Kcachegrind](http://www.weidendorfers.de/kcachegrind/)，[dmalloc](http://dmalloc.com/)，NuMega，[BoundsCheck](http://www.compuware.com/products/devpartner/bounds/)，ParaSoft
，Insure++等等，在这里，我想向大家介绍的是Rational
公司（呵呵，应该是IBM了）的 Purify，这是我觉得最专业，也是最强大的内存检测工具。
Purify
所支持的操作系统有Windows 2000/XP Professional/NT、Sun Solaris、HP-UX、SGI-IRIX。我不知道其支不支持Linux，但在其网站上，我并没有看到这样的信息，但又听别人说他支持，所以在这里我不敢断言它不支持，想想要做UNIX下的软件能有不支持Linux的吗？可能很少吧。
下面，是我所使用的Purify的版本和运行Purify的操作系统：
> purify -version
Version 2003.06.00 Solaris 2
> uname -a
SunOS hostname 5.8 Generic_108528-11 sun4u sparc SUNW,Ultra-60
我会基于这个版本向你介绍Purify的强大功能。
## 二、Purify简介
在C/C++的软件开发中，没有任何一种工具可以让你的应用程序避免引入内存问题，但是我们可以使用诸如Purify这样的工具对已经做好了的程序进行内存问题的检查。Purify的强大之处是可以找到应用程序中全面的内存问题，并可以和GDB/DBX等调试器以配合使用，让你对你的内存错误一目了然。
Purify是一个Run-Time的工具，也就是说只有在程序运行过程中，根据程序的运行情况来查看在某种运行条件下程序是否有内存上的问题，它可以在一个非常复杂的程序中查找内存错误，包括那种多进程或多线程的程序，它也可以进行测试。
Purify对程序中的每一个内存操作都进行检测，并对精确报告内存出现错误的变量和语句，以提供出现错误原因的分析。Purify主要检测的是下面这些内存错误：
l数组内存是否越界读/写。
l是否使用了未初始化的内存。
l是否对已释放的内存进行读/写。
l是否对空指针进行读/写。
l内存漏洞。
在软件工程中，以我的经验而言，最好是在编码阶段时就使用Purify检测内存存问题，一直到交给测试人员测试。请相信我，在一个大型的C/C++软件产品中，即使检测出了内存问题，离真正地解决它还有一定的距离，所以为了让这个“距离”不算太远，最好还是在功能模块完成时就进行Purify的内存检测。
一般而言，在软件测试中，首要的是软件的功能测试，然后是反面案例测试，再而是压力测试。就我个人经验而言，使用内存检测工具的阶段应该是编码阶段、模块合并后、以及程序逻辑测试完成以后，直到产品发布前，也要做一个内存测试。
要使用Purify这个工具很简单，首先在你安装好了的Purify的目录上你会看到两个Shell脚本：purifyplus_setup.csh（对应于C-Shell）
 purifyplus_setup.sh（对应于标准Shell）。你先执行一下这两个脚本，以便让Purify设置好一些环境参数，如：
> source purifyplus_setup.csh
而对于你的程序而言，你需要这样使用：
> purify cc –g –o myProgram myProgram.c
> purify cc –g –c –o myLib.o myLib.c
就这么简单，然后你只要运行你的程序就行了，Purify会向你提交一份内存问题的报告清单，以供你分析。Purify使用的是OCI（Object
 Code Insertion）技术，它会在你的目标程序中插入一些它自己的函数，这些函数主要都是内存检测的语句，这些语句将会放置在程序中所有，内存操作之前，一旦在程序运行时发现内存问题，Purify所插入的这些语句就会向你报告。一般来说，所有的内存检测工具都是这样工作的。
当被Purify编译过的程序运行时，Purify会弹出一个图形界面的窗口，来向你报告内存问题。如下所示：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_purify1.jpg)
点击三角符号后将出现更为详细的信息：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/haoel/15190/o_purify2.jpg)
Purify在报告内存问题的时候，可以指出源程序中哪个地方出现内存问题，但对于内存泄漏而言，它只能指出出现问题的内存是哪一块，也就是指出内存是在哪里被分配的，而不是指出内存泄露是如何发生的。这是比较合乎情理的，所以，即使你知道那块内存发生了泄漏，你也不一定能找到究竟在什么时候发生的。当然，如果你让Purify配合GDB一起使用，那么要找到这种问题的根本原因，也不是什么困难的事情
