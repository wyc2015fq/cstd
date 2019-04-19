# ncurses窗口机制：wprintw(), wrefresh() - Koma Hub - CSDN博客
2018年06月20日 22:02:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：147
个人分类：[GUI																[ncurses](https://blog.csdn.net/Rong_Toa/article/category/7746858)](https://blog.csdn.net/Rong_Toa/article/category/7517387)
- Copyright(C) `NCURSES Programming HOWTO`
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-04-window.md#%E7%AA%97%E5%8F%A3%E6%9C%BA%E5%88%B6%E7%AE%80%E4%BB%8B)窗口机制简介
在我们面对数以万计的curses 函数之前，让我们先了解一下窗口机制。关于窗口机制的详细内容我们会在以后的章节中详细介绍，本章只介绍窗口的基本概念。 窗口是由curses系统定义的一个假想的屏幕，并不像Windows 平台上的窗口那样具有边框。当curses 初始化的时候，它会默认创建一个叫做stdscr 的窗口。这个窗口的屏幕一般是80列，25 行（根据显示环境的不同，可能会出现不同的大小）。如果只是执行简单的任务，比如打印几个字符串、输入一些数据等等，这样的单窗口程序完全可以满足你的需要。当然，你也可以通过窗口系统的函数创建你自己的窗口。 举个例子，如果你调用以下函数：
printw(“Hi! There!”);
refresh();
它会在stdscr 窗口上的当前光标位置输出`“Hi! There!”`。同样，调用`refresh()`函数，只更新stdscr 上的输出内容。 例如，你已经建立了一个叫做win 的窗口并要输出以上内容，只需在以上的输出函数前添加w 前缀就可以了。当然，函数中的参数也要做出相应的变化（要指明你所要显示信息的窗口）：
wprintw(win, "Hi There !!!");
wrefresh(win);
你将在这份文档的其余部分看到这些函数有相同的命名规则。每个相关函数都对应有三个甚至更多的处理函数。
printw(string); /* 在stdscr 的当前光标位置打印字符串string */
mvprintw(y, x, string); /* 将字符串string 打印在坐标(y,x)处*/
wprintw(win, string); /* 在窗口win 的当前光标位置打印字符串string */
mvwprintw(win, y, x, string);
/* 将光标移动到窗口win 的(y,x)坐标处然后打印字符串string */
由此可以看出，没有`w` 前缀的函数在调用时被扩展成以stdscr 作为当前窗口的函数。
