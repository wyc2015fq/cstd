# ncurses初始化函数：raw(),cbreak(),echo(),noecho(),keypad(),halfdelay() - Koma Hub - CSDN博客
2018年06月20日 21:56:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：364
个人分类：[GUI																[ncurses](https://blog.csdn.net/Rong_Toa/article/category/7746858)](https://blog.csdn.net/Rong_Toa/article/category/7517387)
- Copyright(C) `NCURSES Programming HOWTO`
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-03-initial.md#%E5%88%9D%E5%A7%8B%E5%8C%96%E5%87%BD%E6%95%B0)初始化函数
我们现在知道在程序中调用`initscr()`函数，屏幕就会初始化并进入curses 模式。本章我们会介绍其它的初始化函数，这些函数可以根据我们自己的要求在初始化后定制curses 会话（curses session）的功能及模式。例如：`终端模式`（terminal mode）、`彩色显示模式`（colormode）、`鼠标操作模式`（mouse mode）等。当然，我们还可以定制`混合模式`。这章就让我们来讨论这些在`initscr()`函数之后调用的初始化函数。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-03-initial.md#raw%E5%87%BD%E6%95%B0%E5%92%8Ccbreak%E5%87%BD%E6%95%B0)`raw()`函数和`cbreak()`函数
通常情况下，终端驱动程序会缓冲用户输入的字符，直到遇到换行符或回车符后，这些字符才可以被使用。但是大多数程序要求字符在输入时就可以被使用。`raw()`和`cbreak()`两个函数都可以禁止行缓冲（line buffering）。`区别`是：在`raw()`函数模式下，处理挂起（CTRLZ）、中断或退出（CTRLC）等控制字符时，将直接传送给程序去处理而不产生终端信号；而在`cbreak()`模式下，控制字符将被终端驱动程序解释成其它字符。就我个人而言，比较喜欢使用`raw()`函数，因为用它可以做一些一般用户无法进行的控制操作。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-03-initial.md#echo%E5%87%BD%E6%95%B0%E5%92%8Cnoecho%E5%87%BD%E6%95%B0)`echo()`函数和`noecho()`函数
这两个函数用来控制是否将从键盘输入的字符显示在终端上。调用`noecho()`函数禁止输入的字符出现在屏幕上。也许程序员希望用户在进行控制操作时，需要屏蔽掉控制字符（如组合键操作），或调用`getch()`函数读取键盘字符时，不想显示输入的字符（如在控制台输入登陆密码）。大多数的交互式应用程序在初始化时会调用`noecho()`函数，用于在进行控制操作时不显示输入的控制字符。这两个函数给予程序员很大的灵活性，使程序员可以在窗口中的任意地方，实现输入字符的显示和屏蔽，而不需要刷新屏幕。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-03-initial.md#keypad%E5%87%BD%E6%95%B0)`keypad()`函数
这个函数允许使用功能键，例如：F1、F2、方向键等功能键。几乎所有的交互式程序都需要使用功能键，因为绝大多数用户界面主要用方向键进行操作。使用`keypad(stdscr,TURE)`就为“标准屏幕”（`stdscr`）激活了功能键。你将会在以后的章节中学习到如何使用功能键。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-03-initial.md#halfdelay%E5%87%BD%E6%95%B0)`halfdelay()`函数
`halfdelay()`函数会启用半延时模式（halfdelaymode）。和`cbreak()`函数一样，当程序需要用户输入字符时，输入的每个字符都是可用的。给`halfdelay()`传递一个整型参数（以0.1 秒为单位），它就会在这个参数时间内等待输入。如果没有有效的输入，则返回`ERR`。 一般来说，这个函数在需要等待输入的程序中会用到。如果用户没有在规定时间内给出有效输入，程序就可以去处理其它事情。最常见例子就是在输入密码时做出超时响应。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-03-initial.md#%E5%85%B6%E5%AE%83%E7%9A%84%E5%88%9D%E5%A7%8B%E5%8C%96%E5%87%BD%E6%95%B0)其它的初始化函数
上面提到的函数用来定制curses 在初始化后的行为，因此这些函数只能在整个curses 会话的开始部分（即初始化时）调用，而不能在程序的其它地方调用。
## [](https://github.com/Rtoax/Knowledge/blob/master/GUI/ncurses/ncurses-03-initial.md#%E7%A4%BA%E4%BE%8B%E7%A8%8B%E5%BA%8F)示例程序
让我们写一个程序用来说明这些函数的用法。
#include<ncurses.h>/*Compile: gcc main.c -lncurses*/intmain()
{
    int ch;
    initscr();              /* 开始curses 模式*/raw();                  /* 禁用行缓冲*/keypad(stdscr, TRUE);   /* 开启功能键响应模式*/noecho();               /* 当执行getch()函数的时候关闭键盘回显*/printw("Type any character to see it in bold\n");
    ch = getch();           /* 如果没有调用raw()函数，                               我们必须按下enter 键才可以将字符传递给程序*/if(ch == KEY_F(1))      /* 如果没有调用keypad（）初始化，将不会执行这条语句*/printw("F1 Key pressed");
                            
    else/* 如果没有使用noecho() 函数，一些控制字符将会被打印到屏幕上*/
    { 
        printw("The pressed key is ");
        attron(A_BOLD);
        printw("%c", ch);
        attroff(A_BOLD);
    }
    refresh();              /* 将缓冲区的内容打印到显示器上*/getch();                /* 等待用户输入*/endwin();               /* 结束curses 模式*/return0;
}
这个程序很简单，但还是有一些在前面的章节没有介绍的函数。`getch()`函数用来取得用户输入的字符，它等价于通常的`getchar()`函数，只是我们在调用`getchar()`函数时要禁止行缓冲以避免在输入完成后按enter 键。在后面的章节中我们将讨论到输入函数。`attron()`和`attroff()`函数分别作为修饰的开头和结尾，修饰其间输出的字符。例如在本例中加粗（即使用`A_BOLD`作为函数参数）用户输入的字符。在后面的章节中我们也将会详细讨论这些函数。
