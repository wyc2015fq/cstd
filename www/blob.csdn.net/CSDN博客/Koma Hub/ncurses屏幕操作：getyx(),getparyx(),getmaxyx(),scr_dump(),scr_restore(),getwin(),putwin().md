# ncurses屏幕操作：getyx(),getparyx(),getmaxyx(),scr_dump(),scr_restore(),getwin(),putwin() - Koma Hub - CSDN博客
2018年06月22日 14:19:16[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：170
个人分类：[GUI																[ncurses](https://blog.csdn.net/Rong_Toa/article/category/7746858)](https://blog.csdn.net/Rong_Toa/article/category/7517387)
- Copyright(C) NCURSES Programming HOWTO
# [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-10-screen.md#%E5%B1%8F%E5%B9%95%E6%93%8D%E4%BD%9C)屏幕操作
在这一章我们将了解一些管理屏幕的函数。这些函数经常被用于编写基于控制台的游戏。顺便让我们写一些非常有意思的程序。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-10-screen.md#getyx%E7%B3%BB%E5%88%97%E5%87%BD%E6%95%B0)`getyx()`系列函数
`getyx()` 函数可以用来取得当前光标的位置。并把它存储在传递给它的两个变量中。`getyx()`是一个宏，所以不能向它传送变量的地址。你只能这样调用它：
/* win: 窗口的指针 * y, x: 光标坐标的y, x 值将被赋到这两个变量*/getyx(win, y, x);
`getparyx()`用于取得子窗口相对主窗口的起始坐标，它在更新子窗口时经常使用。当设计一个多级菜单时，如果用存储菜单坐标方法来处理，就变得非常困难。然而使用`getparyx()`函数找到该菜单的相对坐标的方案就显的比较简单。`getbegyx()`函数和`getmaxyx()`函数用于以同样的方式存储当前窗口的起始和结束坐标，可以高效地管理窗口和子窗口。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-10-screen.md#%E5%B1%8F%E5%B9%95%E8%BD%AC%E5%82%A8)屏幕转储
开发游戏的时候，通常存储和恢复屏幕是十分必要的。`scr_dump()`函数可以把当前屏幕的内容存入指定文件，即以文件名作为函数的参数（函数原型：`scr_dump(const char *file)` ——译者注）。而通过`scr_restore()`函数调用屏幕数据文件来恢复屏幕（ 函数原型：`scr_restore(constchar *file)` ——译者注）。在游戏设计中这两个函数可以用来快速切换游戏场景。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-10-screen.md#%E7%AA%97%E5%8F%A3%E8%BD%AC%E5%82%A8)窗口转储
窗口转储同屏幕转储的原理一样。getwin()函数（函数原型：`getwin(FILE * filep)` ——译者注）用来将窗口内容存储到一个指定的文件中。`putwin()`函数（函数原型：`putwin(WINDOW*win, FILE * filep)` ——译者注）则调用相应的文件来恢复窗口。`copywin()`可以将一个窗口拷贝到另一个窗口，即将源窗口矩形区域（由参数指定）中的内容复制到目标窗口指定的矩形区域（由参数指定）里。而最后的参数用来选择是否要覆盖目的窗口：如果参数为TRUE，那么就会覆盖目标窗口的内容；如果参数为FLASE，那么就会重写目的窗口中的内容。以下为函数的原型：
intcopywin(
    const WINDOW *src,  /* 源窗口指针*/
    WINDOW *dst,        /* 目的窗口指针*/int sminrow,        /* 源窗口所选矩形区域的最小行数*/int smincol,        /* 源窗口所选矩形区域的最小列数*/int dminrow,        /* 目的窗口所选矩形区域的最小行数*/int dmincol,        /* 目的窗口所选矩形区域的最小列数*/int dmaxrow,        /* 目的窗口所选矩形区域的最大行数*/int dmaxcol,        /* 目的窗口所选矩形区域的最大列数*/int over)           /* 是否覆盖目的窗口*/
