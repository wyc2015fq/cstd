# 第一个ncurses程序： hello world ！！！ - Koma Hub - CSDN博客
2018年06月20日 21:41:31[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：143
- Copyright(C) `NCURSES Programming HOWTO`
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-02-helloworld.md#%E7%AC%AC%E4%B8%80%E4%B8%AAncurses-hello-world-%E7%A8%8B%E5%BA%8F)第一个ncurses hello world 程序
#include<ncurses.h>/*Compile: gcc main.c -lncurses*/intmain()  
{  
    initscr();                 /* 初始化，进入NCURSES 模式*/printw("Hello World !!!"); /* 在虚拟屏幕上打印Hello, World!!! */refresh();                 /* 将虚拟屏幕上的内容写到显示器上，并刷新*/getchar();                 /* 等待用户输入*/endwin();                  /* 退出NCURSES 模式*/return0;  
} 
这个程序在显示器屏幕上打印`“Hello World !!!”`后等待用户按任意键退出。这个小程序展示了如何初始化并进入curses 模式、处理屏幕和退出curses 模式。让我们逐行的分析这个小程序:
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-02-helloworld.md#%E5%85%B3%E4%BA%8Einitscr-%E5%87%BD%E6%95%B0)关于`initscr()` 函数
`initscr()`函数将终端屏幕初始化为curses 模式。它用来清除屏幕上所有的字符，并等待下一部处理。所以在调用其它的curses 函数前，要先调用`initscr()`函数初始化屏幕。这个函数初始化了curses 系统并且为当前屏幕（也就是`“stdscr”`）和相关的数据结构分配内存。在以前的计算机上曾经出现过一个极端的例子：因为系统中的可用内存太小，以至于`initscr()`函数无法分配足够的内存给相关的数据结构，从而导致curses 系统初始化失败。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-02-helloworld.md#%E7%A5%9E%E7%A7%98%E7%9A%84refresh%E5%87%BD%E6%95%B0)神秘的`refresh()`函数
第二行的`printw()`函数用于把字符串`“Hello，World！！！”`输出到虚拟的屏幕上。这个函数在用法上和`printf()`函数很像。不同的是：`printw()`函数把字符串输出到`“stdscr”`的虚拟窗口坐标`（0,0）`上（从显示的结果来看，坐标`（0,0）`在屏幕的左上角上）。 现在该说说这个神秘的`refresh()`函数了。在我们使用`printw` 函数打印`“Hello World！！！”`时，实际上这个消息打印到了一个叫作`“stdscr”`的虚拟窗口上，没有被直接输出到屏幕上。`printw()`函数的作用是不断将一些显示标记和相关的数据结构写在虚拟显示器上，并将这些数据写入`stdscr` 的缓冲区内。为了显示这些缓冲区中的数据我们必须使用`refresh（）`函数告诉curses系统将缓冲区的内容输出到屏幕上。 通过这种机制程序员能够不断在虚拟屏幕上写数据。然后调用`refresh（）`函数让输出的操作看起来是一次完成的。因为`refresh（）`函数只核查窗口和数据中变动的部分，这种富有弹性的设计提供了一个高效的反馈机制。但是这有时很打击初学者的积极性。因为对于初学者来说忘记在输出后调用`refresh()`函数是很恼人的错误。不过不用担心，很多人都会犯这样的错误。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-02-helloworld.md#%E5%85%B3%E4%BA%8Eendwin%E5%87%BD%E6%95%B0)关于`endwin()`函数
最后，别忘了退出curses 模式。否则，在程序结束后你的终端可能会运转得不正常。`endwin()`函数释放了curses 子系统和相关数据结构占用的内存，使你能够正常返回控制台模式。这个函数必须是在你完成所有的curses 操作以后才可以调用。（译者注：如果你在`endwin()`函数后再调用其它的curses 的函数。很显然，那些语句不会执行。）说到这里，顺便提醒一下。如果你的程序不能正常地显示东西了。请务必看看`initscr()`函数和`endwin()`函数是不是在不该被调用的地方调用了。
