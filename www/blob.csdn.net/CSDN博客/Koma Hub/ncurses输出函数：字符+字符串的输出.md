# ncurses输出函数：字符+字符串的输出 - Koma Hub - CSDN博客
2018年06月20日 22:22:23[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：177
- Copyright(C) `NCURSES Programming HOWTO`
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#%E8%BE%93%E5%87%BA%E5%87%BD%E6%95%B0)输出函数
在curses 函数中有三类输出函数，它们分别是：
- `addch()`系列：将单一的字符打印到屏幕上，可以附加加字符修饰参数的一类函数。
- `printw()`系列：和printf()一样的具有格式化输出的一类函数。
- `addstr()`系列：打印字符串的一类函数。
这几类函数可以交替使用。关键是要了解各类函数的具体应用环境。让我们来看看这些函数。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#addch%E7%B3%BB%E5%88%97%E5%87%BD%E6%95%B0)`addch()`系列函数
`addch()`函数用于在当前光标位置输入单个字符，并将光标右移一位。你可以使用这个函数输出一个字符，并给其添加修饰。在后面的章节中我们将会对其做出详细的介绍。如果一个字符关联有修饰效果（比如：粗体、反色等等），那么当curses 输出这个字符的同时就会应用相关的修饰。
给单个字符关联属性有两种方法：使用`修饰宏`（通过`“或”`运算符）修饰。这些修饰宏可以在头文件`ncurses.h` 中找到。比如，你想输出一个具有`加粗（BOLD）`和`加下划线（UNDERLINE）`效果的字符变量ch，可以使用下面这种方法：
addch(ch | A_BOLD | A_UNDERLINE);
使用`attrset()`、`attron()`、`attroff()`修饰函数。简而言之，它们将当前的修饰关联于给定的窗口。一旦设置完成，则在相应窗口中输出的字符都会被修饰，直到关闭窗口。这些函数将在文字修饰一章介绍。另外，curses 提供了一些可以在字符模式下作简单图形的特殊字符。你可以用它们绘制表格、水平、垂直线条等等，这些特殊字符都是以`ACS_`作为开头声明的宏并保存在头文件ncurses.h 里。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#mvaddch-waddch-%E5%92%8Cmvwaddch%E5%87%BD%E6%95%B0)`mvaddch()`, `waddch()` 和`mvwaddch()`函数
`mvaddch()`用于将光标移动到指定位置输出字符。因而，下面的函数调用：
move(row,col); /*将光标移动到row 所指定的行和col 所指定的列。*/
addch(ch);
可以用以下的等价函数取代：
mvaddch(row,col,ch);
`waddch()`函数和`addch()`函数类似。不同的是，`waddch()`函数是将字符输出到指定窗口的指定坐标处。（注：`addch()`将字符输出到标准输出stdscr 上。） 同样的`mvwaddch()`函数是把光标移动到指定窗口中的指定位置处输出字符。 现在，我们熟悉了一些基本的输出函数。但是，如果我们要输出字符串，像这样一个一个的输出字符是很烦人的。幸好，ncurses 为我们提供了像`printf()`和`puts()`一样方便的函数。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#printw%E7%B3%BB%E5%88%97%E5%87%BD%E6%95%B0)`printw()`系列函数
这些函数的用法和我们熟悉的`printf()`函数相似，但增加了可以在屏幕任意位置输出的功能。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#printw%E5%87%BD%E6%95%B0%E5%92%8Cmvprintw%E5%87%BD%E6%95%B0)`printw()`函数和`mvprintw()`函数
这两个函数的绝大部分用法和`printf()`函数相同。`mvprintw()`函数将光标移动到指定的位置，然后打印内容。如果你想先移动光标，再调用`printw()`函数，也就是说先调用`move()`函数，然后调用`printw()`函数。我不知道为什么有些人非要使用这两个函数代替一个函数，当然决定权在你手里。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#wprintw-%E5%87%BD%E6%95%B0%E5%92%8Cmvwprintw-%E5%87%BD%E6%95%B0)`wprintw()` 函数和`mvwprintw` 函数
这两个函数和以上两个函数类似。区别在于这两个函数将在指定的窗口输出内容，而以上两个函数将内容输出到标准输出stdscr 上。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#vwprintw%E5%87%BD%E6%95%B0)`vwprintw()`函数
这个函数和`vprintf()`相似，用于打印变量表中所对应的变量。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84printw%E5%87%BD%E6%95%B0%E7%9A%84%E4%BD%BF%E7%94%A8%E4%BE%8B%E5%AD%90)一个简单的`printw()`函数的使用例子
- 例：一个简单的`printw()`函数的使用例子：
/*Compile: gcc main.c -lncurses*/
#include<ncurses.h>/* ncurses.h 已经包含了stdio.h */
#include<string.h>intmain()
{
    char mesg[]="Just a string";        /* 将要被打印的字符串*/int row,col;                        /* 存储行号和列号的变量，用于指定光标位置*/initscr();                          /* 进入curses 模式*/getmaxyx(stdscr,row,col);           /* 取得stdscr（标准输出设备）的行数和列数*/mvprintw(row/2,(col-strlen(mesg))/2,"%s",mesg);
                                        /*在屏幕的正中打印字符串mesg*/mvprintw(row/2,0,"This screen has %d rows and %d columns\n",row,col);
    printw("Try resizing your window(if possible)and then run this program again");
    refresh();
    getch();
    endwin();
    return0;
}
上面这个程序展示了使用`printw()`系列函数输出字符是多么简单。你只需要修改要输出的行列坐标和要打印的信息，它就会按你的需要输出。这个程序引入了一个新函数`getmaxyx()`，这是一个定义在`ncurses.h` 中的宏，可以给出指定窗口的行列数目。`getmaxyx()`是通过修改所给变量实现这一功能的。`getmaxyx()`不是一个函数，因此不能以指针作为参数，只能通过两个整型变量来实现。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#addstr%E7%B3%BB%E5%88%97%E5%87%BD%E6%95%B0)`addstr()`系列函数
`addstr()`函数用于在指定窗口输出字符串，如同连续使用`addch()`函数来输出指定字符串中的每个字符。实际上，也就是所有`addstr()`系列输出函数的事实。`addstr()`系列函数还包括`mvaddstr()`、`mvwaddstr()`和`waddstr()`，它们有着相同的curses 命名规则和调用方法（如`mvaddstr()` 函数就是分别调用了`move()`和`addstr()`函数）。这个函数集中，还有一个特殊函数`addnstr()`，它需要一个整型参数`n`，用来打印字符串中的前`n` 个字符。如果这个参数是负数，`addnstr()`将会打印整个字符串。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-05-io-01-output.md#%E6%8F%90%E9%86%92)提醒
所有这些函数中使用坐标时，先给定`y` 坐标，再给定`x` 坐标，也就是先行后列。因为计算机字符模式是逐行显示的。很多初学者因为数学上的使用习惯，而错误的先`x` 后`y`。如果你进行了过多行列坐标的操作，想想看将屏幕分割成几个不同的窗口，然后独立处理每一个单独窗口的情形。
