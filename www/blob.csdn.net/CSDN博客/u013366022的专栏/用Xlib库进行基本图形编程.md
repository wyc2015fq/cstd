# 用Xlib库进行基本图形编程 - u013366022的专栏 - CSDN博客
2015年04月13日 18:54:11[slitaz](https://me.csdn.net/u013366022)阅读数：925
目录
1、前言
2、X Window系统的客户服务器模式
3、GUI编程－同步化的编程模型
4、基本的Xlib概念
        1、X Display
        2、GC－图形上下文
        3、对象句柄
        4、Xlib 结构体的内存分配
        5、事件
5、编译给予Xlib的程序
6、打开和关闭一个连接到X服务器的连接
7、检查关于Display的基本信息
8、创建一个简单的窗口－我们的“hello world”程序
9、在窗口中绘画
        1、分配图形上下文(GC)
        2、基本绘图－点，线，框，圆...
10、X事件
        1、使用事件遮罩给事件型别注册
        2、接收事件－撰写事件循环
        3、暴露事件
        4、获得用户输入
                1、鼠标按钮点击和释放事件
                2、鼠标移动事件
                3、鼠标指针进入和离开事件
                4、键盘焦点
                5、键盘按下和释放事件
        5、X事件－完整的例子
11、处理文本和字体
        1、字体结构
        2、载入字体
        3、把字体赋给图形上下文
        4、在窗口中绘出文本
12、窗口阶层
        1、根，父和子窗口
        2、事件传播
13、和窗口管理器交互
        1、窗口属性
        2、设置窗口名和图标名
        3、设置最佳窗口尺寸
        4、设置窗口管理器的杂项
        5、设置应用程序的图标
14、简单窗口操作
        1、映射和取消映射窗口
        2、在屏幕上移动窗口
        3、缩放窗口
        4、改变窗口的堆叠次序－提高会放低
        5、标识会取消标识窗口
        6、获得窗口的信息
15、使用颜色来绘出彩虹
        1、色表
        2、分配和释放色表
        3、分配和释放单个颜色
        4、用颜色绘画
16、X Bitmap和Pixmap
        1、什么是X Bitmap？什么又是X Pixmap？
        2、从文件中载入Bitmap
        3、在窗口中画出Bitmap
        4、创建Pixmap
        5、在窗口中画出Pixmap
        6、释放Pixmap
17、改变鼠标光标
        1、创建和释放鼠标光标
        2、设置窗口的鼠标光标
前言
该教程是“可能会有”的关于在X Window环境下进行图形化编程的教程的第一个系列。其自
身是用处不大的。一个真正的X程序员一般使用一个更高层次的抽象，比如用Motif(或者它
的的免费版本，lesstiff)，GTK，QT和类似的库。然而，我们需要从一个地方开始入手。
不仅如此，知道表象之下的事情是如何工作的决不会是坏的主意。
在读完本教程后，你可能能够些简单的图形程序，但是那不会是一个有良好用户界面的程序
。对于这样的程序，也许就要用到上述的库中的某一个。X window系统开发之初有一个最大的目标－灵活性。想法是这样的东西看上如如何一回事，
东西如何工作的又是另外一回事。因而，底层提供在画窗口，处理用户输入，允许使用颜色
画图形(或者黑白屏幕)等动作中需要的工作。就这点决定了把系统分为两个部分。客户决定
作什么，而服务器实际在屏幕上画图并读出用户输入以发给客户进行处理。
这个模型正好和人们在客户和服务器中所习惯的行为相反。在我们的例子，用户坐在由服务
器控制的机器旁边，而客户可能运行于一个远程的机器上。服务器控制屏幕，鼠标和键盘。
客户可能连接到了服务器，发出画一个(或者多个)窗口的请求，并要求服务器把任何用户发
送给这些窗口的输入给他。因而，几个客户可能连接到了同一个X服务器－一个可能在运行e
mail软件，一个可能在运行WWW浏览器，等等。当由用户发送输入给某些窗口时，服务器向
控制这些窗口的客户发送消息以供处理。客户决定对输入作什么，并给服务器发送请求来在
窗口中绘图。
整个会话过程是用X消息协议执行的。该协议最初时由TCP/IP协议包执行的，允许客户运行
于任何和服务器连接在相同网络上的机器上。后来，X服务器被扩展为允许客户运行在本地
机器上更优的访问服务器(注意到X协议消息可能有几百KB那么大)，比如使用共享内存，或
者使用Unix域sockets(一个在Unix系统上的两个进程间创建逻辑通道的方法)。
**3.GUI编程－同步化的编程模型**不同于包含某种顺序化执行内质的传统的计算机程序。GUI程序通常使用同步化的编程模型
，也被称为“事件驱动编程”。这个意味着程序大部分时候时闲着的，等待由X服务器发送
的事件，然后根据这些事件作出反应。事件可能时”用户在点x，y处按下第一个按钮“，或
者时”你控制的串口需要重画“。为了程序能够响应用户输入以及刷新请求，它需要在一个
相当短的时间内处理每个事件(比如作为一个大体的规则，小于200毫秒)。
这也意味着程序可能不执行在处理过程也许需要很长时间的事件的操作(例如打开一个连接
到远程服务器的网络连接，或者连接到数据库服务器，甚至执行一个大文件的拷贝)。而是
，它需要同步化的执行所有这些操作。这可能通过使用各种同步模型来执行长时间的操作，
或者通过用不同的进程或者线程来执行他们。
因而GUI程序看上去大概是这样：
   1、执行初始化
   2、连接到X服务器
   3、执行X相关的初始化工作
   4、在没有结束之前：
        1、接收来自于X服务器的下一个事件
        2、处理事件，也许向X服务器发送多种绘画请求
        3、如果事件是退出消息，跳出循环
   5、关闭连接到X服务器的连接
   6、执行清理操作
**4.基本的Xlib概念**为了消除程序事件实现X协议层的需求，一个称为‘Xlib’的库被创造出来。该库给程序提
供了一个对任何X服务器非常底层的访问。因为协议是标准化的，客户使用Xlib的任何一种
实现都可以和和X服务器通话。这些可能在今天看来没什么大用，但回到使用字符模式终端
和专有方法在屏幕上绘图的日子，这是一个很大的突破。事实上，你将注意到咱爱瘦客户，
窗口终端服务器，等等周围进行的各种虚伪的骗局。他们在今天实现X协议在80年代晚期已
经能够作的事情。另外一方面，X universe在玩一个关于CUA(共通用户感观，一个由IBM
制造的概念，指的是对所有程序使用共通的观感以使得用户能够更加轻松)的catch-up游戏。
没有共通的感观是X window系统创造者的哲学。明显，它有许多在今天看来显然的缺陷。
X Display
使用Xlib的最大的概念是X display。这是一个代表我们和一个给定X服务器打开的连接的结
构体。它隐藏了服务器的消息队列，客户将要发送给服务器的请求队列。在Xlib中，这个结
构体被命名为‘Display’。当我们打开一个连接到X服务器的连接的时候，库返回一个指向
这种结构体的指针。然后，我们把这个指针提供给任何需要发送消息给X服务器或者从这个
服务器接收消息的Xlib函数。
GC －图形上下文
当我们执行各种绘出(图形，文本，等)操作的时候，我们可能要指定几个选项以控制数据怎
么被绘出 － 前景色和背景色是什么，线的边缘如何连接，在绘出文本的时候使用何种字体
，等。为了避免给每个绘出函数提供n多参数，一个类型为‘GC’的图形上下文结构被启用
。我们在这个结构中设置各种绘出选项，并且把指向这个结构的指针传递给每个绘出函数。
这个是相当方便的，因为我们通常需要用相同选项执行好几个绘出请求。因而，我们初始化
图形上下文，设置所需的选项，并把这个GC结构传递给所有的绘出函数。
对象句柄
当各种对象被创造出来给X服务器使用 － 例如窗口，绘画区域和光标 － 相关的函数返回
一个句柄。这是实际存在于X服务器的内存中的对象的标识符。我们能够在后面通过把这些
句柄提供给各种Xlib函数来操纵这些对象。服务器保存了这些句柄和它们管理的对象之间的
映射。Xlib提供各种型别定义给这些对象(窗口，光标，色表等等)，它们实际上最终映射为
简单的整数。我们在定义保存这些句柄的变量的时候仍然应当使用这些型别名－为了有更好
的可移植性。
为Xlib结构体分配内存
在Xlib的接口中使用了各种结构型别。他们中的一些直接由用户分配内存。其他的使用Xli
b函数分配。这使得库能够恰当的初始化这些结构。这非常方便，因为这些结构倾向于包含
很多变量，使得对于差劲点的程序员非常难于初始化它们。记住－Xlib尝试着尽可能的了灵
活，而且这意味着他也是尽可能的复杂。由缺省值使得初学X的程序员能够使用这个库，而
不打扰有经验的程序员在n多选项中作调整的可能。对于释放内存，由两种方法完成。在我们
分配内存的情况－我们用相同方法释放它们(也就是使用free()来释放由malloc()分配的内
存)。在我们用某Xlib函数分配的时候，或者我们使用返回动态分配的内存的Xlib查询方法
的时候－我们使用XFree()函数来释放这些内存块。
事件
型别‘XEvent’的结构被用来传递从X服务器接收来的事件。Xlib支持很大数量的事件型别
。XEvent结构包含接收事件的类型，以及与该事件相关的数据(例如事件产生的屏幕位置，
与事件相关的鼠标按钮，和‘redraw’事件相关的屏幕区域，等)。读取事件的数据的方法
和事件类型有关。因而，XEvent结构包含一个C语言对于所有可能事件型别的联合(如果你不
确知C的联合是什么，该是查查你的C语言手册的时候...)。因而，我们能够有一个XExpose
事件，XButton事件，XMotion事件，等。**5.编译基于Xlib的程序**编译基于Xlib的需要把他们和Xlib库进行链接。这是通过使用如下的编译命令行来完成的：
cc prog.c -o prog -lX11
如果编译器抱怨它找不到X11库，尝试加上‘-L’标志，像这样:
cc prog.c -o prog -L/usr/X11/lib -lX11
或者也许是这样(对于用X11的release 6的系统)：
cc prog.c -o prog -L/usr/X11R6/lib -lX11
在SunOs 4系统上，X库被放置于/usr/openwin/lib：
cc prog.c -o prog -L/usr/openwin/lib -lX11
**6.打开和关闭连接到X服务器的连接**X程序首先需要打开连接到X服务器的连接。在我们完成这件工作的时候，我们需要指定运行
X服务器的机器的地址，以及display号码。X window系统能够支持全部连接于同一个机器的
好几个display。然而，通常只有一个这样的display，它的display号是‘0’。如果我们想
要连接到本地display(也就是我们客户程序所运行的机器的display)，我们可以指定displa
y为’:0‘。要连接到地址为”simey“的机器的第一个display，我们能够使用地址”simey
:0“。这儿是连接是如何被打开的：
#include <X11/Xlib.h>   /* defines common Xlib functions and structs. */
.
.
*/* this variable will contain the pointer to the Display structure */*
*/* returned when opening a connection.                             */*
Display* display;
*/* open the connection to the display "simey:0". */*
display = XOpenDisplay("simey:0");
if (display == NULL){
    fprintf(stderr, "Cannot connect to X server %s**\n**", "simey:0");
    exit (-1);
}
注意，对于X程序来说检查系统变量‘DISPLAY’是否被定义了是很常见的，而且如果是的话
，使用它的内容作为XOpenDisplay()函数的参数。
当程序完成了它的使命并且需要关闭连接到X服务器的连接的时候，它如下动作：
XCloseDisplay(display);
这将导致所有由程序创造的窗口(如果还有剩下的话)自动被服务器关闭，而且为了客户的利
益任何留在服务器上的资源－将被释放。注意这将不会导致我们的客户程序终止－我们使用
普通的exit()函数来完成。
**7.检查关于Display的基本信息**一旦我们打开了一个连接到X服务器的连接，我们应当检查有关它的一些基本信息：他有什
么样的屏幕，尺寸是多少(宽和高)，它支持多少颜色(黑白？灰度？256色？更多？)，以及
等等。我们将展示一些作一些这样检查的代码片段，以及在使用中解释每个函数的注释。我
们假定‘display’是一个指向‘Display’的结构的指针，由前面对XOpenDisplay()的调用
返回的。
*/* this variable will be used to store the "default" screen of the  */*
*/* X server. usually an X server has only one screen, so we're only */*
*/* interested in that screen.                                       */*
int screen_num;
*/* these variables will store the size of the screen, in pixels.    */*
int screen_width;
int screen_height;
*/* this variable will be used to store the ID of the root window of our */*
*/* screen. Each screen always has a root window that covers the whole   */*
*/* screen, and always exists.                                           */*
Window root_window;
*/* these variables will be used to store the IDs of the black and white */*
*/* colors of the given screen. More on this will be explained later.    */*
unsignedlong white_pixel;
unsignedlong black_pixel;
*/* check the number of the default screen for our X server. */*
screen_num = DefaultScreen(display);
*/* find the width of the default screen of our X server, in pixels. */*
screen_width = DisplayWidth(display, screen_num);
*/* find the height of the default screen of our X server, in pixels. */*
screen_height = DisplayHeight(display, screen_num);
*/* find the ID of the root window of the screen. */*
root_window = RootWindow(display, screen_num);
*/* find the value of a white pixel on this screen. */*
white_pixel = WhitePixel(display, screen_num);
*/* find the value of a black pixel on this screen. */*
black_pixel = BlackPixel(display, screen_num);
有各种其他的宏来得到关于屏幕的更多信息，你可以从任何Xlib参考书中得到它们。还有和
这些宏完成相同功能的函数(例如XWhitePixel，它和WhitePixel干一样的事情)。**8、创建一个简单的窗口－我们的“hello world”程序**在我们获得了一些有关我们的屏幕的基本信息之后，我们可以开始创建我们第一个窗口。Xl
ib提供数个函数来创建新窗口，其中的一个是XCreateSimpleWindow()。这个函数或者少量
几个决定窗口的大小和位置等的参数。这有一个这些参数的完整列表：
Display* display
    指向display结构的指针
Window parent
    窗口的父窗口
int x
    窗口左上角的点的x坐标
int y
    窗口左上角的点的x坐标
unsigned int width
    窗口的宽度
unsigned int height
    窗口的宽度
unsigned int border_width
    窗口边框的宽度
unsigned long border
    窗口边框的颜色
unsigned long background
    窗口的背景
让我们创建一个简单窗口，它的宽是屏幕宽的1/3，高是屏幕高的1/3，背景色是白的，边框
颜色是黑色的，而且边框宽2象素。窗口将会放置在屏幕的左上角。
*/* this variable will store the ID of the newly created window. */* Window win; */* these variables will store the window's width and height. */*int win_width; int win_height; */* these variables will store the window's location. */*int win_x; int win_y; */* calculate the window's width and height. */* win_width = DisplayWidth(display, screen_num) / 3; win_height = DisplayHeight(display, screen_num) / 3; */* position of the window is top-left corner - 0,0. */* win_x = win_y = 0; */* create the window, as specified earlier. */* win = XCreateSimpleWindow(display,                           RootWindow(display, screen_num),                           win_x, win_y,                           win_width, win_height,               win_border_width, BlackPixel(display, screen_num),                           WhitePixel(display, screen_num));
我们创造了窗口的事实并不意味着它会被画在屏幕上。缺省的，新创建的窗口不会被映射于
屏幕之上 － 它们是不可见的。为了使得我们的窗口可见，我们使用XMapWindow()函数，如
下：
XMapWindow(win);
要看我们至今积累写出的所有代码，看看simple-window.c程序。你将看到至今没有解释的
两个另外的函数 － XFlush()和XSync()函数用来清除仍未发送给X服务器的请求 － 很想用
来清除标准输出的fflush()函数。XSync()函数也清除所有仍未发送给X服务器的消息，而且
等待X服务器结束处理所有这些请求。在一个通常的程序中，这将不会是必要的(你可以看到
为什么在我们开始写一个普通的X程序的时候)，但对于现在我们把它放在那儿。尝试着有和
去掉这些函数调用来编译程序，以观察它们行为上的不同点。
**9.在窗口中绘图**在窗口中绘图能够通过使用各种图形函数来完成 － 画点，线，圆，矩形，等。为了能够在
窗口中绘图，我们首先需要定义几种通用的绘图参数 － 线宽使用多少的，绘图的颜色是什
么，等。这个是用图形上下文(GC)来完成的。
分配图形上下文(GC)
如我所说，图形上下文给出几个用于绘图函数的属性。因此，我们定义一个图形上下文。我
们能够在一个窗口中使用多余一个的图形上下文，以达到用多种风格(不同的颜色，线宽，
等)绘图。分配一个新的GC是通过使用XCreateGC()函数来完成的，如下(在这个代码片段中
，我们假定“display”是一个只想Display结构的指针，而起“win”是先前创建的窗口的I
D)：
*/* this variable will contain the handle to the returned graphics context. */* GC gc; */* these variables are used to specify various attributes for the GC. */**/* initial values for the GC. */* XGCValues values = CapButt | JoinBevel; */* which values in 'values' to check when creating the GC. */*unsignedlong valuemask = GCCapStyle | GCJoinStyle; */* create a new graphical context. */* gc = XCreateGC(display, win, valuemask, &values); if (gc < 0){     fprintf(stderr, "XCreateGC: **\n**"); }
注意“valuesmask”和“values”的角色。因为图形上下文有n多属性，并且我们不想定义
它们中的一些，我们需要能够告诉XCreateGC()哪些属性是我们想要设置的。这就是“value
smask”变量的用处。我们然后使用“values”变量来指定我们在“valuesmask”中定义的
属性的值。因而，对于每个在“values”中使用的常量，我们将使用在“valuesmask”中相
应的常量。在此例中，我们用两个属性定义图形上下文：
   1、当在画多部分的线的时候，线应该以‘Bevelian’风格连接起来。
   2、线的终点将被直的画出来(与以圆角结束线相对，如果它的宽度大于一个象素)。
这个GC的剩余属性将由它们的缺省值设定。
一旦我们创建了一个图形上下文，我们能够在绘图函数中使用它。我们还能够各种函数修改
它的参数。这儿有几个例子：
*/* change the foreground color of this GC to white. */* XSetForeground(display, gc, WhitePixel(display, screen_num)); */* change the background color of this GC to black. */* XSetBackground(display, gc, BlackPixel(display, screen_num)); */* change the fill style of this GC to 'solid'. */* XSetFillStyle(display, gc, FillSolid); */* change the line drawing attributes of this GC to the given values. */**/* the parameters are: Display structure, GC, line width (in pixels), */**/* line drawing  style, cap (line's end) drawing style, and lines     */**/* join style.                                                        */* XSetLineAttributes(display, gc, 2, LineSolid, CapRound, JoinRound);
要获关于在图形上下文中有的各种属性的完整信息，参考XCreateGC()的手册页。我们将在
我们的教程中仅仅使用几个简单的属性，以避搞得过度复杂了。
基本绘图－点，线，框，圆...
在我们创建了GC之后，我们能够使用这个GC在窗口上用一套Xlib函数绘画了，这些函数合成
为“基本绘图函数”。废话不多说了，让我们看看它们是如何使用的吧。我们假定”gc“是
先前初始化了的GC，而且‘win’包含了先前创建的窗口的句柄。
*/* draw a pixel at position '5,60' (line 5, column 60) of the given window. */* XDrawPoint(display, win, gc, 5, 5); */* draw a line between point '20,20' and point '40,100' of the window. */* XDrawLine(display, win, gc, 20, 20, 40, 100); */* draw an arc whose center is at position 'x,y', its width (if it was a     */**/* full ellipse) is 'w', and height is 'h'. Start the arc at angle 'angle1'  */**/* (angle 0 is the hour '3' on a clock, and positive numbers go              */**/* counter-clockwise. the angles are in units of 1/64 of a degree (so 360*64 */**/* is 360 degrees).                                                          */*int x = 30, y = 40; int h = 1, w = 45; int angle1 = 0, angle2 = 2.109; XDrawArc(display, win, gc, x-(w/2), y-(h/2), w, h, angle1, angle2); */* now use the XDrawArc() function to draw a circle whose diameter */**/* is 15 pixels, and whose center is at location '50,100'.         */* XDrawArc(display, win, gc, 50-(15/2), 100-(15/2), 15, 15, 0, 360*64); */* the XDrawLines() function draws a set of consecutive lines, whose     */**/* edges are given in an array of XPoint structures.                     */**/* The following block will draw a triangle. We use a block here, since  */**/* the C language allows defining new variables only in the beginning of */**/* a block.                                                              */*{*/* this array contains the pixels to be used as the line's end-points. */*     XPoint points[] = {{0, 0},       {15, 15},       {0, 15},       {0, 0}};     */* and this is the number of pixels in the array. The number of drawn */**/* lines will be 'npoints - 1'.                                       */*int npoints = sizeof(points)/sizeof(XPoint);     */* draw a small triangle at the top-left corner of the window. */**/* the triangle is made of a set of consecutive lines, whose   */**/* end-point pixels are specified in the 'points' array.       */*     XDrawLines(display, win, gc, points, npoints, CoordModeOrigin);   }*/* draw a rectangle whose top-left corner is at '120,150', its width is */**/* 50 pixels, and height is 60 pixels.                                  */* XDrawRectangle(display, win, gc, 120, 150, 50, 60); */* draw a filled rectangle of the same size as above, to the left of the  */**/* previous rectangle. note that this rectangle is one pixel smaller than */**/* the previous line, since 'XFillRectangle()' assumes it is filling up   */**/* an already drawn rectangle. This may be used to draw a rectangle using */**/* one color, and later to fill it using another color.                   */* XFillRectangle(display, win, gc, 60, 150, 50, 60);
但愿你跟上了我的进度。我们还将提到更多的一些使用上差不多的函数。例如，XFillArc(
)和XDrawArc()带有相同的参数，但是只画出弧的内部(像XFillRectangle()函数所作的和用
XDrawRectangle()函数画出的矩形一样)。还有一个填充多边形内部的XFillPolygon()函数
。它和XDrawLines()基本上有相同的参数。然而，如果数组的最后一个点和第一个点处于不
同的位置，XFillPolygon()函数自动添加一条”virtual“线，连接这两个点。两个函数的
另外一个不同点就是XFillPolygon()带另外一个参数，shape。它用来帮助X服务器优化它的
行为。你能够在手册页上学到这些。对于这些函数还有复数版本，名字为XFillArcs()和XFi
llRectangles()。
完成这些绘画的程序的源代码位于文件simple-drawing.c中。
**10.X 事件**
在Xlib程序中，所有的事情都是被事件驱动的。事件绘图有时是对事件－一个”暴露的“事
件－的反应。如果程序窗口被隐藏的一部分重又暴露了(例如窗口从另外一个窗口后面升上
来了)，X服务器将发送一个”暴露的“事件让程序知道它应当重新画处窗口的这个部分。用
户输入(按键，鼠标移动，等)也是作为一套事件被接收的。
1>.使用事件遮罩给事件型别注册
在程序创建了一个窗口(或者几个窗口)之后，它应当告诉X服务器它想让这个窗口接收什么
型别的事件。缺省的，没有事件发送给程序。它可能注册各种鼠标(也被称为”指针“)事件
，键盘事件，暴露事件等等。这是用于优化服务器和客户之间的连接(也就是，为什么要发
送给程序(那可能是运行于地球的另外一边的)它不感兴趣的事件的？)。
在Xlib中，我们使用XSelectInput()函数来注册事件。这个函数接收3个参数 － display结
构，窗口的ID，以及它想要收到的事件型别的遮罩。窗口ID这个参数使得我们能够为不同的
窗口注册接收不同型别的事件。这儿是我们如何给ID为‘win’的窗口注册”暴露”事件的
：
XSelectInput(display, win, ExposureMask);
ExposureMask 是定义在头文件“X.h”中的常量。如果我们想要注册好几种事件型别，我们
用逻辑或进行连接，如下：
XSelectInput(display, win, ExposureMask | ButtonPressMask);
遮注册了“暴露”事件以及鼠标按钮在给定窗口按下的事件。你应当注意到一个遮罩有可能
代表了好几种事件子型别。
注意：一个常见的蹩脚程序员所作的是在它们的程序中添加代码来处理新的事件型别，而忘
记了在调用XSelectInput()中添加这些事件的遮罩。这样的程序员然后坐下来花数个小时调
试它们的程序，奇怪于“为什么我的程序没注意到我释放鼠标？？”，最后只是发现它们忘
记了只注册鼠标按下事件，而不是鼠标释放事件。
2>.接收事件－撰写事件循环在我们为感兴趣的事件型别注册了之后，我们需要进入接收事件和处理它们的循环。有好几
种撰写这样的循环的办法，但基本的循环是这样的：
*/* this structure will contain the event's data, once received. */* XEvent an_event; */* enter an "endless" loop of handling events. */* while (1){     XNextEvent(display, &an_event);     switch (an_event.type) ｛       case Expose:         */* handle this event type... */*         .         .         break;       default: */* unknown event type - ignore it. */*break;     }}
XNextEvent()函数取得从X服务器发送来的下一个事件。如果没有事件在等待，它阻塞在那
知道接收到了一个。当它返回了，事件的数据被放置给函数的第二个参数XEvent变量中。之
后，变量的“type”域指定了我们得到的事件的型别。事件的型别是Expose告诉我们窗口的
一部分布需要重画。在我们处理了事件之后，我们回过头来继续等待下一个要处理的。明显
，我们需要给用户某种终止程序的途径。如我们即将看到的，这通常是通过处理“quit”事
件来完成那个的。
3>.暴露事件“暴露”事件是程序可能接收的最基本的事件中的一个。它在一下情况中将发送给我们：
    * 覆盖我们一部分窗口的窗口被移开了，暴露我们窗口的部分(或者全部)。
    * 我们的窗口从其他窗口后面升上来了
    * 我们的窗口第一次映射
    * 我们的窗口被取消标识了。
你应当注意背后隐藏的假设 － 我们窗口的内容在被其他窗口遮盖时候丢失了。你可能奇怪
X服务器为什么不保存这些内容。答案是 － 为了节省内存。毕竟，窗口在display上的数量
在给定时间是非常巨大的，而且保存它们的所有内容可能需要很多的内存(例如，大小为400
*400象素的256色位图占据160KB的内存存储。现在想想20个窗口，比这个数字要大得多)。
事实上，在特殊情况下有告诉X服务器保存窗口内容的办法，我们将在后面看到。
当我们得到一个“暴露”事件的时候，我们应当从XEvent结构的"xexpose“成员处取出事件
的数据(在我们的代码例子中用”an_event.xexpose“引用它)。它包含几个有趣的域：
count
    在服务器中的事件队列等待的其他暴露事件的数量。这个可能在我们一次接连得到好几
个的时候有用 － 我们通常将避免重画知道我们得到它们的最后一个(也就是知道count为0
的时候)。
Window window
    暴露事件被发送的窗口的ID(如果我们的程序在几个窗口中注册事件)。
int x, y
    需要重画的窗口区域的从窗口左上角开始的x和y坐标(象素为单位)。
int width, height
    需要重画的窗口区域的宽和高(象素为单位)。
在我们的演示程序中，我们将倾向于忽略提供的区域，而仅仅重画整个屏幕。然而，这是非
常没有效率的，而且我们将尝试在后面演示一些仅仅画出相关屏幕部分的技术。
作为例子，这是我们将如何横跨我们的窗口画一条线，每当我们接收到”暴露“事件的时候
。假设这个’case‘是事件循环switch语句的一部分。
  case Expose:     */* if we have several other expose events waiting, don't redraw. */**/* we will do the redrawing when we receive the last of them.    */*     if (an_event.xexpose.count > 0)break;     */* ok, now draw the line... */*     XDrawLine(display, win, gc, 0, 100, 400, 100);     break;
4>.获得用户输入
传统上用户输入有两个来源 － 鼠标和键盘。存在多种事件型别来通知我们用户的输入 －
 键盘上的按键被按下，在键盘上释放按键，鼠标移动于我们的窗口之上，鼠标进入(或者离
开)我们的窗口等等。鼠标按钮点击和释放事件
我们将要处理的第一个事件型别是在我们窗口中鼠标按钮按下(或者按键放开)事件。为了注
册这样的一个事件，我们将添加一下遮罩中一个(或者更多)来在XSelectInput()函数中指定
事件型别：
ButtonPressMask
    Notify us of any button that was pressed in one of our windows.
ButtonReleaseMask
    Notify us of any button that was released over one of our windows.
The event types to be checked for in our event-loop switch, are any of the follo
wing:
ButtonPress
    A button was pressed over one of our windows.
ButtonRelease
    A button was released over one of our windows.
这些事件型别的事件结构是通过"an_event.xbutton“来访问的，并且包括一下有趣的域：
Window window
    鼠标事件发送给的窗口ID(如果我们的程序在几个窗口中注册了事件)。
int x, y
    在点击时，鼠标指针从窗口左上角为原点的x和y坐标(象素为单位)。
int button
    被点击的鼠标按钮的编号。可能是像Button1, Button2, Button3这样的值。
Time time
    事件发生的时间(毫秒为单位)。可能用于在程序中计算”双击“的情况(例如，如果鼠
标按钮在小于给定时间内被点击两次，就认定这个为双击)。
作为例子，这儿是我们如何每当接收到”鼠标按下“事件时，当按下的是第一个鼠标按钮的
时候在鼠标点击位置画一个黑点的，而是第二个的时候擦除该点(也就是画个白点)。我们假
定存在两个GC，gc_draw设置为前景色为黑，而gc_erase前景色为白。
假定一下'case’是事件循环的swtich语句的一部分。
  case ButtonPress:
    /* store the mouse button coordinates in 'int' variables. */
    /* also store the ID of the window on which the mouse was */
    /* pressed.                                               */
    x = an_event.xbutton.x;
    y = an_event.xbutton.y;
    the_win = an_event.xbutton.window;
    /* check which mouse button was pressed, and act accordingly. */
    switch (an_event.xbutton.button) {
        case Button1:
            /* draw a pixel at the mouse position. */
            XDrawPoint(display, the_win, gc_draw, x, y);
            break;
        case Button2:
            /* erase a pixel at the mouse position. */
            XDrawPoint(display, the_win, gc_erase, x, y);
            break;
        default: /* probably 3rd button - just ignore this event. */
            break;
    }
    break;
鼠标移动事件
与鼠标按下和释放事件类似，我们也可以得到各种鼠标移动事件的通知。这些能够划分为两
类。一类是按钮没有被按下时鼠标指针的移动，而第二类时当一个或者多个按钮被按下的时
候鼠标指针的移动(这有时被称为“鼠标托放操作”，或者仅仅“拖放”)。下面的事件遮罩
可以加到XSelectInput()的调用中以让我们的应用程序得到这些事件的通知。
指针移动遮罩
    当没有鼠标按钮被按下时，由程序控制的窗口中的一个的指针移动的事件
按钮移动遮罩
    当鼠标的一个(或者更多)的鼠标按钮被按下的时候指针移动的事件。
按钮1移动遮罩
    核按钮移动遮罩一样，只不过当第一个鼠标按钮被按下的时候。
鼠标2移动遮罩，鼠标3移动遮罩，鼠标4移动遮罩，鼠标5移动遮罩
    类似的，用于第二个鼠标按钮，或者第三，第四，第五。
在我们的事件循环swtich语句中要检查的事件型别，是以下的任何一个：
移动通知
    在我们需要得到这个消息通知的窗口中移动的鼠标指针。
这些事件型别的事件结构是以”an_event.xbutton“来访问的，并且包含一下有趣的域：
Window window
    鼠标移动事件发送给的窗口的ID(如果我们的应用程序给几个窗口注册了事件)。
int x, y
    事件发生的时候，以窗口的左上角为原点鼠标指针所位于的x和y坐标(象素为单位)。
unsigned int state
    按钮(或者按键)在事件发生时按下的遮罩 － 如果有的话。改域是以下值的位或：
  # Button1Mask
  # Button2Mask
  # Button3Mask
  # Button4Mask
  # Button5Mask
  # ShiftMask
  # LockMask
  # ControlMask
  # Mod1Mask
  # Mod2Mask
  # Mod3Mask
  # Mod4Mask
  # Mod5Mask
    它们的名字是可以自明的，前五个是指被按下的鼠标按钮，而剩下的指的是被按下的“
特殊按键”(Mod1通常是‘ALT’或者‘META’键)。
Time time
    事件发生所处的事件(毫秒为单位)。
作为例子，以下的代码处理一个绘图程序的“绘图模式”，也就是说如果用户在鼠标1键按
下的时候移动了，那么我们在屏幕上“绘图”。注意代码有一个惯性：因为鼠标移动可能产
生许多事件，可能我们不会在每个鼠标移到的点都得到鼠标移动事件。我们的程序应当能够
处理这么一个情况。解决的一个办法可能是记住鼠标托过的上一个点，并在和新的鼠标指针
位置之间画直线。假定下面的‘case’是事件循环的switch语句的一部分。
  case MotionNotify:
    /* store the mouse button coordinates in 'int' variables. */
    /* also store the ID of the window on which the mouse was */
    /* pressed.                                               */
    x = an_event.xmotion.x;
    y = an_event.xmotion.y;
    the_win = an_event.xbutton.window;
    /* if the 1st mouse button was held during this event, draw a pixel */
    /* at the mouse pointer location.                                   */
    if (an_event.xmotion.state & Button1Mask) {
        /* draw a pixel at the mouse position. */
        XDrawPoint(display, the_win, gc_draw, x, y);
    }
    break;
鼠标指针进入和离开事件
另一个应用程序可能感兴趣的事件型别，是鼠标指针进入或者离开程序控制的窗口。一些程
序使用这些事件来向用户展示应用程序现在在焦点状态。为了注册这么一个事件型别，我们
将把下面的一个(或者多个)遮罩添加到我们给XSelectInput()函数指定的事件型别中：
EnterWindowMask
    当鼠标指针进入我们控制的任何窗口时通知我们。
LeaveWindowMask
    当鼠标指针离开我们控制的任何窗口时通知我们。
要在我们的事件循环swtich中检查的事件型别是以下的这些：
EnterNotify
    鼠标指针刚刚进入了我们控制的窗口
LeaveNotify
    鼠标指针刚刚离开了我们控制的窗口
这些事件型别的事件结构是通过“an_event.xcrossing"来访问的，并且包含以下有趣的域
：
Window window
    鼠标事件发送给的窗口的ID(如果我们的程序给几个程序注册了事件)。
Window subwindow
    鼠标进入到我们的窗口(在EnterNotify事件中)，或者鼠标指针移出到的子窗口ID(在Le
aveNotify事件中)，或者两者均否，如果鼠标从我们的窗口外边移入。
int x, y
    事件产生的时候，鼠标指针以窗口的左上角为原点的x和y坐标(象素为单位)。
int mode
    鼠标指针点击的编号。可能是如Button1, Button2, Button3这样的值。
Time time
    事件发生的时间(毫秒为单位)。可能用于在程序中计算”双击“的情况(例如，如果鼠
标按钮在小于给定时间内被点击两次，就认定这个为双击)。
unsigned int state
    A mask of the buttons (or keys) held down during this event - if any. This f
ield is a bitwise OR of any of the following:
  # Button1Mask
  # Button2Mask
  # Button3Mask
  # Button4Mask
  # Button5Mask
  # ShiftMask
  # LockMask
  # ControlMask
  # Mod1Mask
  # Mod2Mask
  # Mod3Mask
  # Mod4Mask
  # Mod5Mask
    Their names are self explanatory, where the first 5 refer to mouse buttons t
hat are being pressed, while the rest refer to various "special keys" that are b
eing pressed (Mod1 is usually the 'ALT' key or the 'META' key).
Bool focus
    如果窗口拥有键盘焦点，设置为真。否则反之。
键盘焦点屏幕上有许多窗口，但是仅仅有一个键盘和他附着。X服务器如何知道键盘输入是发送给哪
个窗口的呢？这个是通过键盘焦点来完成的。在给定的时间，屏幕上只有一个窗口能够有键
盘焦点。存在Xlib函数来使得程序给某个窗口设置键盘焦点。用户通常能够使用窗口管理器
来安排键盘焦点(通常是通过点击所需窗口的标题栏)。一旦我们的窗口拥有了键盘焦点，每
个键的按下和放开都将导致事件发送给我们的程序(如果它注册了这些事件类型...)。
键盘按下和释放事件
如果由我们程序控制的窗口当前保有键盘焦点，它能够接收键的按下和释放事件。为了注册
这些事件，下面的遮罩要加到XSelectInput()的调用中去：
KeyPressMask
    Notify our program when a key was pressed while any of its controlled window
s had the keyboard focus.
KeyPressMask
    Notify our program when a key was released while any of its controlled windo
ws had the keyboard focus.
The event types to be checked for in our event-loop switch, are any of the follo
wing:
KeyPress
    A key was just pressed on the keyboard while any of our windows had the keyb
oard focus.
KeyRelease
    A key was just released on the keyboard while any of our windows had the key
board focus.
The event structure for these event types is accessed as "an_event.xkey", and co
ntains the following interesting fields:
Window window
    The ID of the window this button event was sent for (in case our application
 registered for events on several windows).
unsigned int keycode
    The code of the key that was pressed (or released). This is some internal X
code, that should be translated into a key symbol, as will be explained below.
int x, y
    The x and y coordinates (in pixels) from the top-left of the window, of the
mouse pointer, when the event was generated.
Time time
    time (in millisecond) the event took place in. May be used to calculate "dou
ble-click" situations by an application (e.g. if the mouse button was clicked tw
o times in a duration shorter than a given amount, assume this was a double-clic
k).
unsigned int state
    A mask of the buttons (or modifier keys) held down during this event - if an
y. This field is a bitwise OR of any of the following:
  # Button1Mask
  # Button2Mask
  # Button3Mask
  # Button4Mask
  # Button5Mask
  # ShiftMask
  # LockMask
  # ControlMask
  # Mod1Mask
  # Mod2Mask
  # Mod3Mask
  # Mod4Mask
  # Mod5Mask
    Their names are self explanatory, where the first 5 refer to mouse buttons t
hat are being pressed, while the rest refer to various "special keys" that are b
eing pressed (Mod1 is usually the 'ALT' key or the 'META' key).
如果我们提到过的，按键代码于其本身是相当没有意义的，并且是受到了附着于运行X服务
器的机器的键盘设备的影响。为了真正使用这些代码，我们要把他们翻译为按键符号，它们
是标准的。我们能用XKeycodeToKeysym()函数来完成翻译工作。这个函数需要3个参数：只
想display的指针，要翻译的按键代码，和一个索引(我们将用‘0’来作这个参数)。标准Xl
ib案件代码可以在”X11/keysymdef.h“中找到。作为一个使用键按下事件和XKeycodeToKey
sym函数的例子，我们将展示如何处理这种的按键：按下‘1’将导致鼠标当前所在的点涂黑
。按下DEL键将导致点的擦除(使用‘gc_erase’GC)。按下任何字母(a到z，大写或者小写)
将使得他们显示在标准输出上。任何其他键的按下都被忽略。假定下面的‘case’是事件循
环的switch语句的一部分。
  case KeyPress:
    /* store the mouse button coordinates in 'int' variables. */
    /* also store the ID of the window on which the mouse was */
    /* pressed.                                               */
    x = an_event.xkey.x;
    y = an_event.xkey.y;
    the_win = an_event.xkey.window;
    {
        /* translate the key code to a key symbol. */
        KeySym key_symbol = XKeycodeToKeysym(display, an_event.xkey.keycode, 0);
        switch (key_symbol) {
            case XK_1:
            case XK_KP_1: /* '1' key was pressed, either the normal '1', or */
                          /* the '1' on the keypad. draw the current pixel. */
                XDrawPoint(display, the_win, gc_draw, x, y);
                break;
            case XK_Delete: /* DEL key was pressed, erase the current pixel. */
                XDrawPoint(display, the_win, gc_erase, x, y);
                break;
            default:  /* anything else - check if it is a letter key */
                if (key_symbol >= XK_A && key_symbol <= XK_Z) {
                    int ascii_key = key_symbol - XK_A + 'A';
                    printf("Key pressed - '%c'\n", ascii_key);
                }
                if (key_symbol >= XK_a && key_symbol <= XK_z) {
                    int ascii_key = key_symbol - XK_a + 'a';
                    printf("Key pressed - '%c'\n", ascii_key);
                }
                break;
        }
    }
    break;
如你所见，按键符号以某种方法与键盘上的屋里按键对应，因而你应当谨慎以恰当检查到所
有的可能情况(如我们在上面例子中对‘1’所作的那样)。我们假定字母键有连续的符号值
，否则范围检查的技巧与按键符号到ASCII代码之间的翻译将不能正常工作。
X Events - A Complete Example
X Events - 一个完整的例子
作为一个处理事件的例子，我们将展示events.c程序。这个程序创建一个窗口，在其上作一
些绘画，然后进入事件循环。如果它得到一个暴露时间 － 它重画整个窗口。如果它得到一
个左键按下(或者移动)事件，它用白色画在鼠标指针下面的点(也就是说擦除这个点)。应当
注意这些画面的改变是怎么被处理的。仅仅用恰当的颜色画出点是不够的。我们需要注意到
颜色的改变，因而下一个暴露事件我们将能够再用合适的颜色画点。就这个目的，我们使用
一个巨大的(1000乘1000)数组代表窗口的点。初始时，数组中的所有的元素被初始化为‘0
’。当以黑色画点的时候，我们设置相应的数组元素为‘1’。当以白色画点的时候我们设
置相应的数组元素为‘－1’。我们不能仅仅把他们重新设置为‘0’，否则我们原来画在屏
幕上的东西总是会被擦除。最终，当用户按下键盘的任意键，程序退出。
当运行这个程序时，你应注意到移动事件常常漏掉点。 如果鼠标快速从一个点移到另一个
点，我们将不会在某个鼠标指针移过的点处得到移动事件。因而，如果我们故意好好处理这
些间隙，我们也许需要记住上次移动事件发生的位置，然后再那个地点和下一个移动事件的
地点之间画一条线。这就是绘画程序通常干的事情。
