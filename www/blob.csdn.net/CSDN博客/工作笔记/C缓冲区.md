# C缓冲区 - 工作笔记 - CSDN博客





2015年03月22日 18:22:03[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5569








原文引自：

[http://www.cnblogs.com/octobershiner/archive/2011/12/06/2278492.html](http://www.cnblogs.com/octobershiner/archive/2011/12/06/2278492.html)

[http://www.cnblogs.com/octobershiner/archive/2011/12/08/2281240.html](http://www.cnblogs.com/octobershiner/archive/2011/12/08/2281240.html)

C语言可以用指针直接操作内存地址，这是他的一个优势，但是也是安全最容易出问题的地方。我们知道C语言是没有内存保护的，它假定C语言负责内存的安全。过去的经验告诉我，Objective-C语言也是这样的，相反java就提供了内存保护的机制。详细的就不多说了。说一下缓冲区的来历。

   在linux上写程序，了解操作系统的话，我们都知道，会涉及到系统调用。有时我们看不到，直接使用C标准库已经实现好的函数，这时候往往我们调用C标准库中的函数，比如getc()，实际上在标准库实现getc的过程中就会进行系统的调用。

   常见的会用到系统调用的函数是C库中的文件读写函数，比如一些以f开头的函数。

   现在我们看一下，在没有缓冲区的情况下,利用gcc编译程序并运行，从文件读入一个字符的过程：

用户态程序调用 getc(FILE* stream)  --->
 GNU C库 getc  ---->系统调用 -----> GNU C返回代码---->用户态getc返回 --->读入成功

   其中红色文字部分表示比较耗时，代价比较高。试想如果我们读入10000个字符，那么这样一个耗时操作就要执行1000次。

   所以，C中引入了缓冲区，缓冲区实际上是一块内存空间，这样当进行系统调用的时候，就不只读一个字符了，C标准库中stdio.h中定义了BUFSIZE就是缓冲区的大小，即每次读入数据的大小。这样结构就发生了变化：

  用户态程序调用 getc(FILE* stream)  --->  GNU C库 getc  ---->系统调用 ----->读入缓冲区----> GNU C返回代码---->用户态getc返回
 --->读入成功

  这样当试图读取第二个字符的时候，就免去了红色的部分，而是用缓冲区取代了原来的系统调用部分，大大减少了系统调用的次数。提高程序的效率。

  但是这样就引发了一个很多同学在学习C的过程中经常遇到的问题

  /////那就是getchar清空缓冲区的例子，后续会更新这个例子



 ------技术的分割线

 缓冲区溢出带来的问题

 下面就是我学到的内容了，首先先了解一下C语言函数进栈的操作。



![](http://pic002.cnblogs.com/images/2011/321923/2011120620574876.png)  图片来自哈工大计算机学院王彦老师课件

    那么，怎样使缓冲区溢出呢，简单的说就是用大于BUFSIZE的数据去填充一个BUFSIZ的缓冲区，由于C没有内存保护，并不会禁止程序的编译和执行，所以“子程序变量”中溢出的字符就会覆盖返回地址，邪恶的同学应该想到攻击的办法了。

    假如我们有一段攻击代码，只要我们将返回地址改写为攻击代码的位置就可以在函数返回时跳转到我们期待的代码了，那么linux下常见的攻击代码就是shell code ，想学习shell code的朋友可以参照 这篇文章，写的很棒[http://blog.csdn.net/sepnic/article/details/6158424](http://blog.csdn.net/sepnic/article/details/6158424)

     但是实际中，由于linux本身的一些安全特性，shell code的地址预测的难度增加了，所以现在常用的方法是nop填充，即在shell code 的前面加入空语句，这样指针就会顺序的执行，自然的跳到shell code的位置，加大了命中shell code的范围。

     缓冲区是一把双刃剑，极大的优化了程序的同时，也有各种各样的问题，接下来会给大家介绍一些常用的函数，就体现了这些，效率的同时也隐含了很多的问题。


与缓冲区相关的操作的常见的使我们的字符的输入输出操作，其实在接触过操作系统和编译原理之后，感觉计算机其实就是在处理一串又一串的字符串。今天给大家介绍get家族的几个函数。

      很多最初使用C语言进行字符串操作，并且认识到缓冲区问题的一般都是从类似下面的程序开始的：



![](http://pic002.cnblogs.com/images/2011/321923/2011120818521244.png)

![](http://pic002.cnblogs.com/images/2011/321923/2011120818524847.png)

    很多初学者会问，按下回车，为什么没等第二次输入程序就结束了，这就是缓冲区的原因了。这里需要了解一下getchar函数，这也是今天第一个要给大家介绍的stdio家族中getchar。



**（1）   int  getchar(void)**

    函数每次从stdin缓冲区读入一个字符遇到回车返回，并且回车也被存入缓冲区，所以getchar可以用来吃掉回车符。

    函数返回读到的第一个字符的ASC码值，如果失败则返回-1

    其定义为宏定义，即 #define getchar()         getc(stdin)

    因为是宏定义函数，所以getchar不支持指针的引用。

    现在解释跳过第二个输入的原因，假如我们第一次输入a按回车，这时候我们getchar实际上将a字符和回车放入了缓冲区中，并返回第一个字符的值，然后第二个getchar函数直接从缓冲区继续去字符，拿到了回车，没有需要用户再输入。为什么会有这种机制，可以参考

[http://www.cnblogs.](http://www.cnblogs.com/octobershiner/archive/2011/12/06/2278492.html)[com/octobershiner/archive/2011/12/06/2278492.html](http://www.cnblogs.com/octobershiner/archive/2011/12/06/2278492.html)

     所以很多人把getchar用来吃掉回车符，起到类似清空缓冲区的原因。清空缓冲区还可以使用fflush函数，但是这个函数不是C标准库中的函数，有时候是无效的。

   刚才说到了getchar的定义，其实就是getc函数的一种特殊的情况，下面介绍getc函数。

**（2）   int  getc(FILE* stream)**

   也是采用的宏定义，所以不支持函数指针调用。

  #define getc(_stream) (--(_stream)->_cnt >= 0 ? 0xff & *(_stream)->_ptr++ : _filbuf(_stream))

    getc从指定的流中读取一个字符，刚才的getchar实际上就是getc(stdin) ,stdin是标准输入流，C在stdio.h中定义了三个流，也可以理解为是缓冲区。

   #define stdin (&_iob[0])       //标准输入，一般指向键盘
   #define stdout (&_iob[1])     //标准输出
   #define stderr (&_iob[2])      //错误流

   基本实现过程

![](http://pic002.cnblogs.com/images/2011/321923/2011120819241426.png)

 在这里再补充两个函数，简单带过，因为getch和getche不是C标准的库函数。

**（3）   int  getch(void)**

从命令行读取一个字符，不显示在命令行，很多人用来模拟“按任意键继续的效果”

**（4）   int  getche(void)**

从命令行中读取一个字符，显示在，命令行，与getch一样不是标准C函数库中的函数，在Windows平台下包含与conio.h中，在linux下与之相对的是curses.h库，但是在cygwin模拟的UNIX和ubuntu linux下gcc编译器均无法使用。

本文的最后介绍一下gets函数，他从流中读取字符串直到遇到换行符或者EOF或者遇到错误.

**（5）   char*  gets(char* buffer)**

   从stdin流中读取字符串，直至接受到换行符或EOF时停止，并将读取的结果存放在buffer指针所指向的字符数组中。换行符不作为读取串的内容，读取的换行符被转换为null值，并由此来结束字符串。



![](http://pic002.cnblogs.com/images/2011/321923/2011120820092958.png)



其实gets这个函数还是有很多安全性的问题需要注意的，比如他要一直等EOF或者换行符才会结束，所以当程序无法预知可能存在的输入串长度时，是非常危险的，上一篇文章提到过缓冲区溢出的攻击，那么这里就是一个漏洞，我们不可能和攻击者比buffer的大小，及时我们定义很大很大的缓冲区，攻击者却能输入一个更长的字符串。

   其中VS 2005中提供了一个新的函数gets_s函数，带有安全性的gets，但是显然这不是标准库中所定义的。

   ========f的分割线=================

   stdio.h中还包含了与gets和getc想对的文件操作函数，fgets和fgetc

   char *fgets(char *s, int n, FILE *stream); //  n表示一次读入数据的长度，从stream中读取存入到s串中，遇到换行和结束符则结束

   int fgetc(FILE * stream);

﻿﻿



