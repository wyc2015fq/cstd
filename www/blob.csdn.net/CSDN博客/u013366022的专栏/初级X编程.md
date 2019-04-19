# 初级X编程 - u013366022的专栏 - CSDN博客
2015年04月13日 19:25:23[slitaz](https://me.csdn.net/u013366022)阅读数：365
# 绪论
本教程是有关X窗口编程的"would-be"系列教程的第一部。单方面来说，这个教程是没用的，因为一个真正的X窗口程序员通常会使用抽象级更高的库，例如Modif（或者是它的自由版本lesstiff）,GTK，QT或者其它类似的库。但...也许我们应该从某个更易于学习理解的地方开始。因为，知道它们到底是如何工作的应该永远不是个坏主意吧。
读过这个教程后，读者应该能够编写非常简单的X窗口图形程序，但不会有具体的应用是用这样的方法来写的，对于那些情况，应该用上面提到的那些抽象级更高的库。
# X窗口系统的客户/服务器模式
当初开发X窗口系统的主要目的只有一个，那就是灵活性。这个灵活性的意思就是说一件东西虽然看起来是在这工作，但却实际上是工作在很远的地方。因此，较低等级的实现部分就必须提供绘制窗口，处理用户输入，画画，使用颜色等工作的工具。在这个要求下，决定了系统被分成了两部分，客户端和服务器端。客户端决定做什么，服务器端执行真正的绘图和接受用户的输入并把它发给客户端。
这种模式与我们一般习惯的客户端和服务器端的概念是正好相反的。在我们的情况下，用户就坐在服务器端控制的机器前，而客户端这时却是运行在远程主机上。服务器端控制着显示屏，鼠标和键盘。一个客户端也许正连接着服务器端，要求给它画一个窗口（或者是一堆），并要求服务器端把用户对它的窗口的输入传给它。结果，好几个客户端可能连接到了一个服务器端上-有的在运行一个电子邮件软件，有的在运行一个网页浏览器等。当用户输入了指令给窗口，服务器端就会把指令打包成事件传给控制那个窗口的客户端，客户端根据接受到的事件决定干什么然后发送请求让服务器端去画什么。
以上介绍的会话都是通过X消息协议传输的。该协议是实现在TCP/IP协议上的，它允许在一个网络里的客户端访问这个网络里的任何服务器端。最后，X服务器端可以和客户端运行在同一台机器上以获得性能优化（注意，一个X协议事件可能会达到上百KB），例如使用共享内存，或者使用Unix域socket(在一个Unix系统的两个进程间创建一个本地通道进行通信的方法)。
# 图形用户接口（GUI）编程-异步编程模式
不像我们通常的令人愉快的程序，一个GUI程序通常使用异步编程模式，也就是下面要介绍的"事件驱动编程"。这个"事件驱动编程"的意思是说程序通常都处于空闲状态，等待从X服务器发来的事件，等收到了事件，才根据事件做相应的事情。一个事件可能是"用户在屏幕某处x,y点击了鼠标左键"，或者是"你控制的窗口需要被重画"。因为程序要回应用户的请求，同时还需要刷新自己的请求队列，因此需要程序尽可能使用较短的事件来处理一个事件（例如，作为一条公认的准则，不能超过200毫秒）。
这也暗示着当然存在需要程序处理很长时间才能完成的事件（例如一个到远程服务器的网络连接，或者是连接一个数据库，或者是不幸的要处理一个超大文件的复制工作）。这都要求程序使用异步方式来处理而不是通常的同步方式。这时候就应该采用各种各样的异步编程方法来进行这些耗时的工作了，或者干脆把它们交给一个线程或进程来进行。
根据以上的说明，一个GUI程序就应该像以下的方式来工作：
- 进行初始化工作
- 连接X服务器
- 进行与X相关的初始化工作
- 进行循环
- 从X服务器那里接受下一个事件
- 根据收到的事件发送各种绘图指令给X服务器
- 如果事件是个退出事件，结束循环
- 关闭与X服务器的连接
- 进行资源释放工作
# Xlib的基本思想
X协议是非常复杂的，为了大家不用再辛辛苦苦把时间浪费在实现它上面，就有了一个叫"Xlib"的库。这个库提供了访问任何X服务器的非常底层的手段。因为X协议已经被标准化了，理论上客户程序使用任何Xlib的实现都可以访问任何X服务器。在今天，这看起来可能很琐碎，但如果回到那个使用字符终端和专有绘图方法的时代，这应该是一个很大的突破吧。实际上，你很快发现围绕瘦客户机，窗口终端服务器等领域会有许多多么令人兴奋的事情。
## X显示
使用XLib的基本思想就是X显示。它代表了一个打开的到X服务器的连接的结构。它隐藏了一个保存有从X服务器来的事件的队列，和一个保存客户程序准备发往服务器的请求队列。在Xlib里，这个结构被命名为显示"Display"。当我们打开了一个到X服务器的连接，库就会返回一个指向这个结构的指针。然后，我们就可以使用这个指针来使用Xlib里各种各样的函数。
## GC - 图形上下文
当我们进行各种绘图操作（图形，文本等）的时候，我们也许会使用许多参数来指定如何绘制，前景，背景，使用什么颜色，使用什么字体等等，等等。为了避免为每个绘图函数设置数量惊人的参数，我们使用一个叫"GC"的图形上下文结构。我们在这个结构里设置各种绘图参数，然后传给绘图函数就行了。这应当是一个非常方便的方法吧，尤其当我们在进行一连串操作中使用相同的参数时。
## 对象句柄
当X服务器为我们创建了各种各样的对象的时候 - 例如窗口，绘图区和光标 - 相应的函数就会返回一个句柄。这是一个存在在X服务器空间中的对象的一个标识-而不是在我们的应用程序的空间里。在后面我们就可以使用Xlib的函数通过句柄来操纵这些对象。X服务器维护了一个实际对象到句柄的映射表。Xlib提供了各种类型来定义这些对象。虽然这些类型实际上只是简单的整数，但我们应该继续使用这些类型的名字 - 理由是为了可移植。
## Xlib结构的内存分配
Xlib的接口使用了各种类型的结构。有些可以由用户直接来分配内存，有些则只能使用专门的Xlib库函数来分配。在使用库来分配的情况，库会生成有适当初始参数的结构。这对大家来说是非常方便的，指定初始值对于不太熟练的程序员来说是非常头疼的。记住-Xlib想要提供非常灵活的功能，这也就意味着它也会变得非常复杂。提供初始值设置的功能将会帮助那些刚开始使用X的程序员们，同时不会干扰那些高高手们。
在释放内存时，我们使用与申请的同样方法来释放（例如，使用free()来释放malloc()申请的内存）。所以，我们必须使用XFree()来释放内存。
## 事件
一个叫"XEvent"的结构来保存从X服务器那里接受到的事件。Xlib提供了非常大量的事件类型。XEvent包括事件的类型，以及与事件相关的数据（例如在屏幕什么地方生成的事件，鼠标键的事件等等），因此，要根据事件类型来读取相应的事件里的数据。这时，XEvent结构使用c语言里的联合来保存可能的数据（如果你搞不清楚c的联合是怎么回事，那你就得花点时间再读读你的教科书了）。结果，我们就可能受到XExpose事件，一个XButton事件，一个XMotion事件等等。
# 编译基于Xlib的程序
编译基于Xlib的程序需要与Xlib库连接。可以使用下面的命令行：
cc prog.c -o prog -lX11
如果编译器报告找不到X11库，可以试着加上"-L"标志，像这样：
cc prog.c -o prog -L/usr/X11/lib -lX11
或者这样（针对使用X11的版本6）
cc prog.c -o prog -L/usr/X11R6/lib -lX11
在SunOs 4 系统上，X的库被放到了 /usr/openwin/lib
cc prog.c -o prog -L/usr/openwin/lib -lX11
等等，具体情况具体分析
# 打开，关闭到一个X服务器的连接
一个X程序首先要打开到X服务器的连接。我们需要指定运行X服务器的主机的地址，以及显示器编号。X窗口允许一台机器开多个显示。然而，通常只有一个编号为"0"的显示。如果我们想要连接本地的显示（例如进行显示的机器同时又是客户程序运行的机器），我们可以直接使用":0"来连接。现在我们举例，连接一台地址是"simey"的机器的显示，我们可以使用地址"simey:0",下面演示如何进行连接
```
#include <X11/Xlib.h>   /* defines common Xlib functions and structs. */
.
.
/* this variable will contain the pointer to the Display structure */
/* returned when opening a connection.                             */
Display* display;
/* open the connection to the display "simey:0". */
display = XOpenDisplay("simey:0");
if (display == NULL) {
    fprintf(stderr, "Cannot connect to X server %s/n", "simey:0");
    exit (-1);
}
```
注意，通常要为X程序检查是否定义了环境变量"DISPLAY"，如果定义了，可以直接使用它来作为XOpenDisplay()函数的连接参数。
当程序完成了它的工作且需要关闭到X服务器的连接，它可以这样做：
XCloseDisplay(display);
这会使X服务器关闭所有为我们创建的窗口以及任何在X服务器上申请的资源被释放。当然，这并不意味着我们的客户程序的结束。
# 检查一个显示的相关基本信息
一旦我们打开了一个到X服务器的连接，我们应该检查与它相关的一些基本信息：它有什么样的屏幕，屏幕的尺寸（长和宽），它支持多少颜色（黑色和白色？灰度级？256色？或更多），等等。我们将演示一些有关的操作。我们假设变量"display"指向一个通过调用XOpenDisplay()获得的显示结构。
```
/* this variable will be used to store the "default" screen of the  */
/* X server. usually an X server has only one screen, so we're only */
/* interested in that screen.                                       */
int screen_num;
/* these variables will store the size of the screen, in pixels.    */
int screen_width;
int screen_height;
/* this variable will be used to store the ID of the root window of our */
/* screen. Each screen always has a root window that covers the whole   */
/* screen, and always exists.                                           */
Window root_window;
/* these variables will be used to store the IDs of the black and white */
/* colors of the given screen. More on this will be explained later.    */
unsigned long white_pixel;
unsigned long black_pixel;
/* check the number of the default screen for our X server. */
screen_num = DefaultScreen(display);
/* find the width of the default screen of our X server, in pixels. */
screen_width = DisplayWidth(display, screen_num);
/* find the height of the default screen of our X server, in pixels. */
screen_height = DisplayHeight(display, screen_num);
/* find the ID of the root window of the screen. */
root_window = RootWindow(display, screen_num);
/* find the value of a white pixel on this screen. */
white_pixel = WhitePixel(display, screen_num);
/* find the value of a black pixel on this screen. */
black_pixel = BlackPixel(display, screen_num);
```
还有很多其它的宏来帮助我们获取显示的信息，你可以在Xlib里的参考里找到。另外还有很多相当的函数可以完成相同的工作。
# 创建一个基本的窗口 - 我们的"Hello world"程序
在我们获得一些窗口的基本信息之后，我们就可以开始创建我们的第一个窗口了。Xlib支持好几个函数来创建窗口，它们其中的一个是XCreateSimpleWindow（）。这个函数使用很少的几个参数来指定窗口的尺寸，位置等。以下是它完整的参数列表：
- `Display* display`
指向显示结构的指针- `Window parent`
新窗口的父窗口的ID。- `int x`
窗口的左上X坐标（单位为屏幕像素）- `int y`
窗口的左上Y坐标（单位为屏幕像素）- `unsigned int width`
窗口的宽度（单位为屏幕像素）- `unsigned int height`
窗口的高度（单位为屏幕像素）- `unsigned int border_width`
窗口的边框宽度（单位为屏幕像素）- `unsigned long border`
用来绘制窗口边框的颜色- `unsigned long background`
用来绘制窗口背景的颜色
让我们创建一个简单的窗口，它的宽度是屏幕宽的1/3，高度是屏幕高的1/3，背景色是白色，边框是黑色，边框的宽度是2个像素。该窗口将会被放置到屏幕的左上角。
```
/* this variable will store the ID of the newly created window. */
Window win;
/* these variables will store the window's width and height. */
int win_width;
int win_height;
/* these variables will store the window's location. */
int win_x;
int win_y;
/* calculate the window's width and height. */
win_width = DisplayWidth(display, screen_num) / 3;
win_height = DisplayHeight(display, screen_num) / 3;
/* position of the window is top-left corner - 0,0. */
win_x = win_y = 0;
/* create the window, as specified earlier. */
win = XCreateSimpleWindow(display,
                          RootWindow(display, screen_num),
                          win_x, win_y,
                          win_width, win_height,
                          win_border_width, BlackPixel(display, screen_num),
                          WhitePixel(display, screen_num));
```
事实上我们创建窗口并不意味着它将会被立刻显示在屏幕上，在缺省情况下，新建的窗口将不会被映射到屏幕上-它们是不可见的。为了能让我们创建的窗口能被显示到屏幕上，我们使用函数XMapWindow()：
`XMapWindow(win);`
如果想察看目前为止我们所举的例子的代码，请参看源程序[simple-window.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/simple-window.c)。你将会发现两个新的函数
 - XFlush() 和XSync()。函数XFlush()刷新所有处于等待状态的请求到X服务器 - 非常像函数fflush()刷新所有的内容到标准输出。XSync()也刷新所有处于等待状态的请求，接着等待X服务器处理完所有的请求再继续。在一个一般的程序里这不是必须的（据此你可以发现我们什么时候只是写一个一般的程序），但我们现在把它们提出来，尝试在有或没有这些函数的情况下程序不同的行为。
# 在窗口里绘制
在窗口里绘图可以使用各种绘图函数 - 画点，线，圈，矩形，等等。为了能在一个窗口里绘图，我们首先需要定义各种参数 - 如线的宽度，使用什么颜色，等等。这都需要使用一个图形上下文（GC）。
## 申请一个图形上下文（GC）
如我们已经提到的，一个图形上下文定义一些参数来使用绘图函数。因此，为了绘制不同的风格，我们可以在一个窗口里使用多个图形上下文。使用函数XCreateGC()可以申请到一个新的图形上下文，如以下例（在这段代码里，我们假设"display"指向一个显示结构，"win"是当前创建的一个窗口的ID）：
```
/* this variable will contain the handle to the returned graphics context. */
GC gc;
/* these variables are used to specify various attributes for the GC. */
/* initial values for the GC. */
XGCValues values = CapButt | JoinBevel;
/* which values in 'values' to check when creating the GC. */
unsigned long valuemask = GCCapStyle | GCJoinStyle;
/* create a new graphical context. */
gc = XCreateGC(display, win, valuemask, &values);
if (gc < 0) {
    fprintf(stderr, "XCreateGC: /n");
}
```
注意，应该考虑一下变量"valuemask"和"values"的角色。因为一个图形上下文有数量惊人的属性，而且通常我们只需要设置里面的一部分，所以我们需要告诉XCreateGC()什么属性是我们需要设置的，这也就是变量"valuemask"的作用。我们接着就可以通过"values"来指定真正的值。在这个例子里，我们定义了图形上下文里的两个属性：
1 当绘制一个多重部分的线时，线在连接时使用"Bevelian"风格
2 一条线的终端被画直而不是圆形
其它未指定的属性GC将会使用缺省值。
一旦我们创建了一个图形上下文，我们就可以在各种绘图函数里用它，我们也可以为了适应别的函数来变更它的属性。
```
/* change the foreground color of this GC to white. */
XSetForeground(display, gc, WhitePixel(display, screen_num));
/* change the background color of this GC to black. */
XSetBackground(display, gc, BlackPixel(display, screen_num));
/* change the fill style of this GC to 'solid'. */
XSetFillStyle(display, gc, FillSolid);
/* change the line drawing attributes of this GC to the given values. */
/* the parameters are: Display structure, GC, line width (in pixels), */
/* line drawing  style, cap (line's end) drawing style, and lines     */
/* join style.                                                        */
XSetLineAttributes(display, gc, 2, LineSolid, CapRound, JoinRound);
```
如果你想了解全部的图形上下文的属性设定方法，请参考函数XCreateGC()的用户文档。我们在这里为了避免过于复杂只使用了一小部分非常简单的属性。
## 绘图的基本元素 - 点，线，矩形，圆...
在我们创建了一个GC后，我们就可以通过GC在一个窗口里使用一系列的Xlib函数，这个函数的集合被称为"绘图的基本元素"。为了简便，让我们通过例子来看一看它们是怎么工作的。这里我们假设"gc"是一个前面创建好的GC，"win"是一个已经创建好的窗口的句柄。
```
/* draw a pixel at position '5,60' (line 5, column 60) of the given window. */
XDrawPoint(display, win, gc, 5, 5);
/* draw a line between point '20,20' and point '40,100' of the window. */
XDrawLine(display, win, gc, 20, 20, 40, 100);
/* draw an arc whose center is at position 'x,y', its width (if it was a     */
/* full ellipse) is 'w', and height is 'h'. Start the arc at angle 'angle1'  */
/* (angle 0 is the hour '3' on a clock, and positive numbers go              */
/* counter-clockwise. the angles are in units of 1/64 of a degree (so 360*64 */
/* is 360 degrees).                                                          */
int x = 30, y = 40;
int h = 15, w = 45;
int angle1 = 0, angle2 = 2.109;
XDrawArc(display, win, gc, x-(w/2), y-(h/2), w, h, angle1, angle2);
/* now use the XDrawArc() function to draw a circle whose diameter */
/* is 15 pixels, and whose center is at location '50,100'.         */
XDrawArc(display, win, gc, 50-(15/2), 100-(15/2), 15, 15, 0, 360*64);
/* the XDrawLines() function draws a set of consecutive lines, whose     */
/* edges are given in an array of XPoint structures.                     */
/* The following block will draw a triangle. We use a block here, since  */
/* the C language allows defining new variables only in the beginning of */
/* a block.                                                              */
  {
    /* this array contains the pixels to be used as the line's end-points. */
    XPoint points[] = {
      {0, 0},
      {15, 15},
      {0, 15},
      {0, 0}
    };
    /* and this is the number of pixels in the array. The number of drawn */
    /* lines will be 'npoints - 1'.                                       */
    int npoints = sizeof(points)/sizeof(XPoint);
    /* draw a small triangle at the top-left corner of the window. */
    /* the triangle is made of a set of consecutive lines, whose   */
    /* end-point pixels are specified in the 'points' array.       */
    XDrawLines(display, win, gc, points, npoints, CoordModeOrigin);
  }
/* draw a rectangle whose top-left corner is at '120,150', its width is */
/* 50 pixels, and height is 60 pixels.                                  */
XDrawRectangle(display, win, gc, 120, 150, 50, 60);
/* draw a filled rectangle of the same size as above, to the left of the  */
/* previous rectangle. note that this rectangle is one pixel smaller than */
/* the previous line, since 'XFillRectangle()' assumes it is filling up   */
/* an already drawn rectangle. This may be used to draw a rectangle using */
/* one color, and later to fill it using another color.                   */
XFillRectangle(display, win, gc, 60, 150, 50, 60);
```
如果你觉得已经抓住使用这些函数的要点，那我们后面的说明就会变得简单。我们将提到其它一些使用相同方法的函数。例如，XFillArc()使用与XDrawArc()相同的参数，但它只画一个圆的内部（相似的，XFillRectangle()只画一个矩形区的内部）。另外还有一个函数XFillPolygon()负责填充一个多边形的内部区域。它使用的参数差不多与XDrawLines()相同。但是要注意，如果提供在矩阵里的最后一个参数所代表的点与第一个点不在同一个位置上，函数XFillPolygon()会添加一条虚拟的线来连接那两个点。这两个函数的另外一个不同是，XFillPolygon()使用一个附加的参数，形状，这个参数可以帮助X服务器来优化它的绘图工作。你可以在手册里找到详细的内容。以上的函数还存在它们的复数绘制版本，命名为XFillArcs()和XFillRectangles()。
以上的说明请参看程序 [simple-drawing.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/simple-drawing.c)
# X 事件
在一个Xlib程序里，所有的动作都是被事件驱动的。针对事件"expose"的反应是在屏幕上画些什么。如果程序窗口的一部分被遮住，然后又露出来了（例如一个窗口遮住了另一个窗口），X服务器将会发送一个"expose"事件来让程序知道它的窗口的一部分应该被重新绘制。用户的输入（按下键盘，鼠标移动等）也是被做成一系列的事件。
## 使用事件面具来注册事件类型
一个程序在创建一个窗口（也可以是好几个）之后，它就应该告诉X服务器为那个窗口它希望接受哪些事件。在缺省情况下，没有事件会发给程序。程序可以注册很多鼠标事件（也可以叫"指针"），键盘事件，暴露事件等等。这么做完全是为了优化服务器-到-客户端的通信（例如，实在是没什么理由向地球另一端的程序发送它不感兴趣的东西）。
在Xlib里，我们使用函数XSelectInput()来注册要接受的事件。该函数接受3个参数 - 显示结构，一个窗口ID，和一个它想要接受的事件类型的面具。参数窗口ID允许我们为不同的窗口注册接受不同类型的事件。下面的例子展示了我们为窗口ID为"win"的窗口注册"expose"事件：
`XSelectInput(display, win, ExposureMask);`
ExposureMask在头文件"X.h"中被定义，如果我们想注册更多的事件类型，我们可以使用逻辑"or"，如下：
`XSelectInput(display, win, ExposureMask | ButtonPressMask);`
这样就即注册了事件"expose"也注册了一个鼠标按键事件。你应该注意到一个面具可以描述多种事件类型。
**注意：一个经常出现的程序臭虫就是给程序添加了处理新的类型的事件的代码，却完全不记得在函数XSelectInput()里注册所追加的事件类型。这时候，程序员就可能会苦恼的在电脑前坐上个把小时去调试他的程序，疑惑"为什么我的程序不去注意我已经松开了按钮？？？"，最后发现自己只注册了按钮按下的事件却没有注册松开的事件。**
## 接收事件 - 编写事件循环
我们在注册了感兴趣的事件类型后，我们应该进入事件循环并且处理它们。有许多方法来实现事件循环，但比较一般且简单的如下：
```
/* this structure will contain the event's data, once received. */
XEvent an_event;
/* enter an "endless" loop of handling events. */
while (1) {
    XNextEvent(display, &an_event);
    switch (an_event.type) {
      case Expose:
        /* handle this event type... */
        .
        .
        break;
      default: /* unknown event type - ignore it. */
        break;
    }
}
```
函数XNextEvent从X服务器那里取得新的事件。如果没有，它就会处于阻塞状态直到接受到了一个事件。函数返回后，事件的数据就会被放到第二个类型为XEvent的参数里。前面取得的事件变量的"type"域指明了该事件的类型。Expose是一个告诉我们窗口的一部分需要重画的事件的类型。在处理过这个事件后，我们就返回去取得下一个要处理的事件。很明显，我们应该提供给用户一些方法去结束程序。一般发个"quit"事件就行了。
## 暴露事件
暴露事件是程序最经常收到的事件中的一个。它会在以下几种情况下出现：
- 一个遮住我们的窗口的窗口被移走了，我们的窗口又重新露出来了。
- 我们的窗口被其它窗口打开了。
- 我们的窗口第一次被映射到屏幕上。
- 我们的窗口从最小化中恢复到打开状态。
你应该已经注意到这里有一个隐藏的假设 - 当窗口被遮住时被遮住的内容就丢失了。你也许会提出疑问为什么X服务器不保存那些内容。答案只有一个 - 节省内存。在某一个时刻，屏幕上可能会有大量的窗口，保存它们的内容将会需要非常大量的内存（例如，一个256色的分辨率为400*400的bitmap图片需要至少160KB的内存来保存它。现在考虑一下有20个窗口的情况，这其中一些可能会有更大的尺寸）。实际上，确实有方法来告诉X服务器在特殊情况下保存窗口的内容，我们会在稍后介绍。
当我们取得了一个"expose"事件，我们应该从XEvent结构的"xexpose"成员中取得事件数据（在我们的例程里，它是"an_event.xexpose"）。另外那个结构还包括一些有趣的域：
- `count`
在服务器的事件队列里还有多少暴露事件。这在我们获得了多个暴露事件时非常有用 - 我们通常避免执行重画工作直到确定它是最后一个暴露事件的时候（如直到是0为止）。- `Window window`
发送该重画事件的窗口的ID（我们的程序为多个窗口注册了事件的时候）。- `int x, y`
从窗口的左上算起，需要被重画的区域的左上坐标。- `int width,height`
需要被重画区域的宽高。
在我们的演示程序中，我们无视了那个需要被重画的区域，而是重画了整个窗口，这是非常低效的，我们在后面将会演示一些只重画需要重画的区域的技术。
以下是一个例子，演示我们收到任何"expose"事件时如何在一个窗口中画一条直线。这是其中的事件循环的case段的代码：
```
case Expose:
    /* if we have several other expose events waiting, don't redraw. */
    /* we will do the redrawing when we receive the last of them.    */
    if (an_event.xexpose.count > 0)
        break;
    /* ok, now draw the line... */
    XDrawLine(display, win, gc, 0, 100, 400, 100);
    break;
```
# 获取用户输入
就目前来说，用户的输入主要从两个地方过来 - 鼠标和键盘。有各种各样的事件帮助我们来获取用户的输入 - 一个键盘上的键被按下了，一个键盘上的键被松开了，鼠标光标离开了我们的窗口，鼠标光标进入了我们的窗口等等。
## 鼠标按键事件和松开事件
我们为我们的窗口处理的第一个事件是鼠标按钮时间。为了注册一个这样的事件类型，我们将追加以下的面具
- `ButtonPressMask`
通知我们窗口中的任何一个鼠标键按下动作- `ButtonReleaseMask`
通知我们窗口中的任何一个鼠标键松开动作
在我们的事件循环中通过switch来检查以下的事件类型
`ButtonPress`在我们的窗口上一个鼠标键被按下了`ButtonRelease`在我们的窗口上一个鼠标键被松开了
在事件结构里，通过"an_event.xbutton"来获得事件的类型，另外它还包括下面这些有趣的内容：
`Window window`事件发送的目标窗口的ID（如果我们为多个窗口注册了事件）`int x, y`从窗口的左上坐标算起，鼠标键按下时光标在窗口中的坐标`int button`鼠标上那个标号的按钮被按下了，值可能是Button1，Button2，Button3`Time time`事件被放进队列的时间。可以被用来实现双击的处理
下面的例子，将演示我们如何在鼠标的位置画点，无论我们何时收到编号为1的按钮的"鼠标按下"的事件时我们画一个黑点，收到编号为2的按钮的"鼠标按下"的事件时我们擦掉那个黑点（例如画一个白点）。我们假设现在有两个GC，gc_draw使用下面的代码
```
case ButtonPress:
    /* store the mouse button coordinates in 'int' variables. */
    /* also store the ID of the window on which the mouse was */
    /* pressed.                                               */
    x = an_event.xbutton.x;
    y = an_event.xbutton.y;
    the_win = an_event.xbutton.window;
    /* check which mouse button was pressed, and act accordingly. */
    switch (an_event.xbutton.button) {
        case Button1:
            /* draw a pixel at the mouse position. */
            XDrawPoint(display, the_win, gc_draw, x, y);
            break;
        case Button2:
            /* erase a pixel at the mouse position. */
            XDrawPoint(display, the_win, gc_erase, x, y);
            break;
        default: /* probably 3rd button - just ignore this event. */
            break;
    }
    break;
```
## 鼠标光标的进入和离开事件
另一个程序通常会感兴趣的事件是，有关鼠标光标进入一个窗口的领域以及离开那个窗口的领域的事件。有些程序利用该事件来告诉用户程序现在在焦点里面。为了注册这种事件，我们将会在函数XSelectInput()里注册几个面具。
`EnterWindowMask`通知我们鼠标光标进入了我们的窗口中的任意一个`LeaveWindowMask`通知我们鼠标光标离开了我们的窗口中的任意一个
我们的事件循环中的分支检查将检查以下的事件类型
`EnterNotify`鼠标光标进入了我们的窗口`LeaveNotify`鼠标光标离开了我们的窗口
这些事件类型的数据结构通过例如"an_event.xcrossing"来访问，它还包含以下有趣的成员变量：
`Window window`事件发送的目标窗口的ID（如果我们为多个窗口注册了事件）`Window subwindow`在一个进入事件中，它的意思是从那个子窗口进入我们的窗口，在一个离开事件中，它的意思是进入了那个子窗口，如果是"none"，它的意思是从外面进入了我们的窗口。`int x, y`从窗口的左上坐标算起，事件产生时鼠标光标在窗口中的坐标`int mode`鼠标上那个标号的按钮被按下了，值可能是Button1，Button2，Button3`Time time`事件被放进队列的时间。可以被用来实现双击的处理`unsigned int state`这个事件发生时鼠标按钮（或是键盘键）被按下的情况 - 如果有的话。这个成员使用按位或的方式来表示
`Button1Mask``Button2Mask``Button3Mask``Button4Mask``ShiftMask``LockMask``ControlMask``Mod1Mask``Mod2Mask``Mod3Mask``Mod4Mask``Bool focus`当值是True的时候说明窗口获得了键盘焦点，False反之
## 键盘焦点
在屏幕上同时会有很多窗口，但同一时间只能有一个窗口获得键盘的使用。X服务器是如何知道哪一个窗口可以发送键盘事件呢？这个是通过使用键盘焦点来实现的。在同一时间只能有一个窗口获得键盘焦点。Xlib函数里存在函数允许程序让指定窗口获得键盘焦点。用户通常使用窗口管理器来为窗口设置焦点（通常是点击窗口的标题栏）。一旦我们的窗口获得了键盘焦点，每个键的按下和松开都将引起服务器发送事件给我们的程序（如果已经注册了这些事件的类型）。
## 键盘键按下和松开事件
如果我们程序控制的窗口获得了键盘焦点，它就可以接受按键的按下和松开事件。为了注册这些事件的类型，我们就需要通过函数XSelectInput()来注册下面的面具。
`KeyPressMask`通知我们的程序什么时候按键被按下了`KeyPressMask`通知我们的程序什么时候按键被松开了
我们的事件循环中的分支检查将检查以下的事件类型
`Window window`事件发送的目标窗口的ID（如果我们为多个窗口注册了事件）`unsigned int keycode`被按下（或松开）的键的编码。这是一些X内部编码，应该被翻译成一个键盘键符号才能方便使用，将会在下面介绍。`int x, y`从窗口的左上坐标算起，事件产生时鼠标光标在窗口中的坐标`Time time`事件被放进队列的时间。可以被用来实现双击的处理`unsigned int state`这个事件发生时鼠标按钮（或是键盘键）被按下的情况 - 如果有的话。这个成员使用按位或的方式来表示
`Button1Mask``Button2Mask``Button3Mask``Button4Mask``ShiftMask``LockMask``ControlMask``Mod1Mask``Mod2Mask``Mod3Mask``Mod4Mask`
如我们前面所提到的，按键编码对我们来说是没有什么意义的，它是由连接着X服务器的键盘产生的硬件级编码并且是与某个型号的键盘相关的。为了能解释到底是哪个按键产生的事件，我们把它翻译成已经被标准化了的按键符号。我们可以使用函数XKeycodeToKeysym()来完成这个翻译工作。该函数使用3个参数：一个显示的指针，要被翻译的键盘编码，和一个索引（我们在这里使用"0"）。标准的Xlib键编码可以参考文件"X11/keysymdef.h"。在下面的例子里我们使用函数XkeycodeToKeysym来处理按键操作，我们讲演示如何以以下顺序处理按键事件：按"1"键将会在鼠标的当前位置下画一个点。按下"DEL"键将擦除那个点。按任何字母键（a至z,大写或小写）将在标准输出里打印。其它的按键将会被无视。假设下面的"case"段代码是在一个消息循环中。
```
case KeyPress:
    /* store the mouse button coordinates in 'int' variables. */
    /* also store the ID of the window on which the mouse was */
    /* pressed.                                               */
    x = an_event.xkey.x;
    y = an_event.xkey.y;
    the_win = an_event.xkey.window;
    {
        /* translate the key code to a key symbol. */
        KeySym key_symbol = XKeycodeToKeysym(display, an_event.xkey.keycode, 0);
        switch (key_symbol) {
            case XK_1:
            case XK_KP_1: /* '1' key was pressed, either the normal '1', or */
                          /* the '1' on the keypad. draw the current pixel. */
                XDrawPoint(display, the_win, gc_draw, x, y);
                break;
            case XK_Delete: /* DEL key was pressed, erase the current pixel. */
                XDrawPoint(display, the_win, gc_erase, x, y);
                break;
            default:  /* anything else - check if it is a letter key */
		if (key_symbol >= XK_A && key_symbol <= XK_Z) {
		    int ascii_key = key_symbol - XK_A + 'A';
		    printf("Key pressed - '%c'/n", ascii_key);
		}
		if (key_symbol >= XK_a && key_symbol <= XK_z) {
		    int ascii_key = key_symbol - XK_a + 'a';
		    printf("Key pressed - '%c'/n", ascii_key);
		}
                break;
        }
    }
    break;
```
你将会发现键盘键符号到物理键编码的转换的方法，程序应该小心的处理各种可能出现的情况。同时我们假设字母键的符号值是连续的。
# X事件 - 一个完整的例子
我们将给一个完整的处理事件的例子[events.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/events.c)。给程序创建一个窗口，在上面进行一些绘画工作，然后进入一个事件循环。如果它获得了一个暴露事件
 - 它重画整个窗口。如果它获得一个鼠标左键事件，它在鼠标光标出画一个黑点。如果鼠标的中间键被按下了，它在鼠标光标下画一个白点（例如擦出那个点）。应该注意这个图形是改变是如何被处理的。它对使用适当的颜色来绘制并不是很有效。我们需要对颜色的变化作一下记录，这样在下一个暴露事件来到时我们可以用正确的颜色来绘制。我们使用了一个（1000*1000）的巨大矩阵来保存像素。刚开始，所有的单元都被置成0。当画了一个点的时候，我们将该单元置成1。如果该点被画成白色，我们将该单元置成-1。我们不能仅仅把黑色置成0，否则我们刚开始画的将被误删掉。最后，用户按了键盘上任意的按钮，程序将退出。
当运行这个程序时，你也许会注意到移动的事件经常会漏画点。如果鼠标移动的很快，我们将收不到所有的事件。结果，如果我们要处理这种情况，我们就需要记住上一次收到事件时的鼠标位置，然后应该画一条线在两点之间。一般绘图程序都是这么做的。
## 字体结构
为了支持灵活的字体，定义了一个字体结构，类型XFontStruct。该结构被用来包含一个字体的信息，被用来帮助一些函数处理字体的选择和文本绘图。
## 装载一个字体
作为绘制文本的第一步，我们使用字体装载函数，例如XLoadQueryFont()。该函数要求X服务器装载给定名字的字体。如果字体被发现了，服务器装载那个字体，返回一个XFontStruct结构的指针。如果字体没有被发现，或者是装载失败了，返回一个值NULL。每个字体也许会有两个名字。一个是长字符串，指明了字体的全部属性（字体族，字体尺寸，斜体/黑体/是否有下划线等等）。另一个是短的别名，为各自的服务器所配置。作为一个例子，我们尝试装载"*-helvetica-*-12-*"字体：
```
/* this pointer will hold the returned font structure. */
XFontStruct* font_info;
/* try to load the given font. */
char* font_name = "*-helvetica-*-12-*";
font_info = XLoadQueryFont(display, fond_name);
if (!font_info) {
    fprintf(stderr, "XLoadQueryFont: failed loading font '%s'/n", font_name);
}
```
## 给一个图形上下文分配字体
在我们装载了字体后，我们需要把它分配给一个GC。假设一个叫"gc"的GC已经存在了，下面是我们如何做：
```
XSetFont(display, gc, font_info->fid);
```
"fid"领域是一个XFontStruct结构用来为各种请求识别各种装载的字体。
## 在一个窗口中绘制文本
我们一旦为我们的GC装载了字体，我们就可以使用例如函数XDrawString()，在我们的窗口里绘制文本。该函数可以在窗口里的一个给定位置里绘制文本。给定的位置将是从被绘制的文本的左下算起，下面是它的例子：
```
/* assume that win_width contains the width of our window, win_height        */
/* contains the height of our window, and 'win' is the handle of our window. */
/* some temporary variables used for the drawing. */
int x, y;
/* draw a "hello world" string on the top-left side of our window. */
x = 0;
y = 0;
XDrawString(display, win, gc, x, y, "hello world", strlen("hello world"));
/* draw a "middle of the road" string in the middle of our window. */
char* text_string = "middle of the road";
/* find the width, in pixels, of the text that will be drawn using */
/* the given font.                                                 */
int string_width = XTextWidth(font_info, text_string, strlen(text_string));
/* find the height of the characters drawn using this font.        */
int fond_height = font_info->ascent + font_info->descent;
x = (win_width - string_width) / 2;
y = (win_width - font_height) / 2;
XDrawString(display, win, gc, x, y, "hello world", strlen("hello world"));
```
以下的说明应该可以使程序更清楚：
- 函数XTextWidth()被用来预测字符串的长度，当它使用指定字体进行绘制时。它被用来检查那里是开始那里是结束使它看起来占据着窗口的中央
- 一个字体的两个名字为"ascent"和"descent"的属性用来指定字体的高。基本上，一个字体的字符是画在一条假象的横线上的。一些字符被画在横线上面，一些画在下面。最高的字符是被画在"font_info->ascent"线上的，最低的部分则在"font_info->descent"线下面。因此，这两个值得和指明了字体的高度。
上面的源程序可以参考文件[simple-text.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/simple-text.c)
# 窗口们的组织体系
当窗口们被显示在X服务器上时，它们通常按照一定组织体系来排序 - 每个窗口可以有子窗口，每个子窗口又可以有自己的子窗口。让我们来查看这个组织体系的一些特性，看看它们是如何来影响例如绘画和事件等处理。
## 根窗口，父窗口和子窗口
每一个屏幕上都有一个根窗口。根窗口总是占据整个屏幕尺寸。这个窗口无法被销毁，改变尺寸或者图标化。当一个应用程序创建了一些窗口，它先创建至少一个顶层窗口。在被映射到屏幕上后，这个窗口成为一个根窗口的直接子窗口。这个窗口在被映射到屏幕上之前，窗口管理器被告知什么发生了，然后，窗口管理器获得特权成为新顶层窗口的"父亲"。这通常被用来增加一个会包含新窗口的窗口和绘制框架，标题栏，系统菜单等。
一旦一个顶层窗口（当然它实际上不是一个顶层窗口，因为窗口管理器已经成为它的父窗口了）被创建了，应用程序可以在它里面创建它的子窗口。一个子窗口只能在它的父窗口里显示 - 如果试图把它移动到外面，出去的部分将被父窗口的边框给切掉。任何窗口都可以包含一个以上的子窗口，在这种情况下，这些子窗口将被放置在应用的内部栈上。当一个顶层窗口被打开 - 它的所有子窗口也将随着它被打开。
以下例子演示如何在一个给定的叫"win"的窗口里打开一个子窗口。
Lets see how to create a child window inside a given window 'win'.
```
/* this variable will store the handle of the newly created child window. */
Window child_win;
/* these variables will store the window's width and height. */
int child_win_width = 50;
int child_win_height = 30;
/* these variables will store the window's location.         */
/* position of the child window is top-left corner of the    */
/* parent window. - 0,0.                                     */
int child_win_x = 0;
int child_win_y = 0;
/* create the window, as specified earlier. */
child_win = XCreateSimpleWindow(display,
                                win,
                                child_win_x, child_win_y,
                                child_win_width, child_win_height,
                                child_win_border_width,
                                BlackPixel(display, screen_num),
                                WhitePixel(display, screen_num));
```
## 事件传递
先前我们已经讨论了事件传递 - 如果一个窗口收到了一个它不处理的事件 - 它就把该事件发到它的父窗口去。如果那个父窗口也不处理该事件 - 那个父窗口就把该事件发到它的父窗口上去，接下来依此类推。这种行为对一个简单的Xlib程序是没什么用的，但对于抽象级更高的绘图库是有用的。这些抽象级更高的绘图库通常把某个特定窗口的事件联系到一个函数上去。在这种情况下，发送事件到特定的窗口并用适当的函数来处理就非常有用。
# 与窗口管理器进行交互
在我们察看了如何创建和绘制窗口之后，我们回过头来看一下我们的窗口是如何与它们的环境 - 整个屏幕和其它窗口进行交互的。首先，我们的程序需要与窗口管理器进行交互。窗口管理器有责任装饰被绘制的窗口（例如增加框架，一个图标化的按钮，一个系统菜单，一个标题栏），同时在窗口被图标化时绘制图标。它还管理屏幕里的窗口排列顺序以及其它可管理的任务。我们需要给它各种提示以让它以我们需要的方式来对待我们的窗口。
## 窗口属性
许多与窗口管理器交流的参数都通过叫"properties"的数据来传递。X服务器把这些属性贴到各种窗口上，同时把它们存储成一种可以被各种架构的系统所能读取的格式（记住，一个X客户程序可能运行在一台远程主机上）。属性可以是各种类型 - 数字，字符串，等等。大部分的窗口管理器提示函数使用文本属性。一个叫XStringListToTextProperty()的函数可以把C语言的字符串转换成X文本属性，转换后的结果就可以传给各色Xlib函数。以下是一个例子:
```
/* This variable will store the newly created property. */
XTextProperty window_title_property;
/* This is the string to be translated into a property. */
char* window_title = "hello, world";
/* translate the given string into an X property. */
int rc = XStringListToTextProperty(&window_title,
                                   1,
                                   &window_title_property);
/* check the success of the translation. */
if (rc == 0) {
    fprintf(stderr, "XStringListToTextProperty - out of memory/n");
    exit(1);
}
```
函数XStringListToTextProperty()接收一个C字符串矩阵（在我们的例子里只有一个）和一个指向XTextProperty型变量的指针为参数，合并C字符串里的属性把值传到XTextProperty型变量里。成功时它返回一个非0值，失败时返回0（例如，没有足够的内存来完成操作）。
## 设置窗口名字和图标名字
我们需要做的第一件事就是给我们的窗口设置名字。使用函数XSetWMName()。窗口管理器也许会把这个名字显示在窗口标题栏或是在任务栏上。该函数接受3个参数：一个指向显示的指针，一个窗口句柄，和一个包含有我们设置的名字的XTextProperty变量。下面是我们如何做的：
```
/* assume that window_title_property is our XTextProperty, and is */
/* defined to contain the desired window title.                   */
XSetWMName(display, win, &window_title_property);
```
为了设置我们的窗口的图标化名字，我们将用相同的方式使用函数XSetWMIconName()。
```
/* this time we assume that icon_name_property is an initialized */
/* XTextProperty variable containing the desired icon name.      */
XSetWMIconName(display, win, &icon_name_property);
```
## 设置满意的窗口尺寸
在各种情况下，我们希望让窗口管理器知道我们指定的窗口尺寸以及只允许用户在我们的限定下改变窗口尺寸。例如，一个终端窗口（像xterm），我们总是要求我们的窗口可以包含全部的行和列，因此我们就不能从中间截断我们的显示。在其它情况下，我们不希望我们的窗口可以被改变尺寸（像绝大部分的对话框窗口），等等。我们可以依赖窗口管理器的这个尺寸信息，虽然它可能被简单的忽视掉。我们首先需要创建一个数据结构来包含该信息，填充必要的数据，然后使用函数XSetWMNormalHints()。下面是如何操作：
```
/* pointer to the size hints structure. */
XSizeHints* win_size_hints = XAllocSizeHints();
if (!win_size_hints) {
    fprintf(stderr, "XAllocSizeHints - out of memory/n");
    exit(1);
}
/* initialize the structure appropriately. */
/* first, specify which size hints we want to fill in. */
/* in our case - setting the minimal size as well as the initial size. */
win_size_hints->flags = PSize | PMinSize;
/* next, specify the desired limits.                             */
/* in our case - make the window's size at least 300x200 pixels. */
/* and make its initial size 400x250.                            */
win_size_hints->min_width = 300;
win_size_hints->min_height = 200;
win_size_hints->base_width = 400;
win_size_hints->base_height = 250;
/* pass the size hints to the window manager. */
XSetWMNormalHints(display, win, win_size_hints);
/* finally, we can free the size hints structure. */
XFree(win_size_hints);
```
请查看你的手册来获取尺寸提示的完整信息。
## 设置各种窗口管理器提示
使用函数XSetWMHints()还可以设置许多其它的窗口管理器提示。该函数使用一个XWMHints结构来传递参数给窗口管理器。下面是例子：
```
/* pointer to the WM hints structure. */
XWMHints* win_hints = XAllocWMHints();
if (!win_hints) {
    fprintf(stderr, "XAllocWMHints - out of memory/n");
    exit(1);
}
/* initialize the structure appropriately. */
/* first, specify which hints we want to fill in. */
/* in our case - setting the state hint as well as the icon position hint. */
win_hints->flags = StateHint | IconPositionHint;
/* next, specify the desired hints data.                         */
/* in our case - make the window's initial state be iconized,    */
/* and set the icon position to the top-left part of the screen. */
win_hints->initial_state = IconicState;
win_hints->icon_x = 0;
win_hints->icon_y = 0;
/* pass the hints to the window manager. */
XSetWMHints(display, win, win_hints);
/* finally, we can free the WM hints structure. */
XFree(win_hints);
```
请查阅手册以获取全部选项的详细信息。
## 设置一个程序的图标
在用户图标化了我们的程序的时候，为了让窗口管理器能为我们的程序设置一个图标，我们使用上面提到的函数XSetWMHints。但是，首先我们需要创建一个包含有图标数据的像素图。X服务器使用像素图来操作图片，将在后面介绍它的详细使用。在这里，我们只是向你展示如何为你的程序设置图标。我们假设你已经得到了一个X bitmap格式的图标文件。教程为了方便提供了一个图标文件["icon.bmp"](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/icon.bmp) ,下面是代码：
```
/* include the definition of the bitmap in our program. */
#include "icon.bmp";
/* pointer to the WM hints structure. */
XWMHints* win_hints;
/* load the given bitmap data and create an X pixmap containing it. */
Pixmap icon_pixmap = XCreateBitmapFromData(display,
                                           win,
                                           icon_bitmap_bits,
                                           icon_bitmap_width,
                                           icon_bitmap_height);
if (!icon_pixmap) {
    fprintf(stderr, "XCreateBitmapFromData - error creating pixmap/n");
    exit(1);
}
/* allocate a WM hints structure. */
win_hints = XAllocWMHints();
if (!win_hints) {
    fprintf(stderr, "XAllocWMHints - out of memory/n");
    exit(1);
}
/* initialize the structure appropriately. */
/* first, specify which size hints we want to fill in. */
/* in our case - setting the icon's pixmap. */
win_hints->flags = IconPixmapHint;
/* next, specify the desired hints data.           */
/* in our case - supply the icon's desired pixmap. */
win_hints->icon_pixmap = icon_pixmap;
/* pass the hints to the window manager. */
XSetWMHints(display, win, win_hints);
/* finally, we can free the WM hints structure. */
XFree(win_hints);
```
你可以使用程序例如"xpaint"来创建使用X bitmap格式的文件。
我们提供文件[simple-wm-hints.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/simple-wm-hints.c)来总结这一节，这段程序包括创建一个窗口，设置窗口管理器提示为在上面显示，以及一个简单的事件循环。它允许用户调整参数以察看提示是如何影响程序的行为的。这可以帮助你了解X程序的可移植性。
# 简单窗口操作
对我们的窗口，我们可以做更多的一些事情。例如，改变它们的尺寸，打开或关闭它们，图标化它们等。Xlib提供了一系列函数来完成上面提到的功能。
## 映射和解除一个窗口的映射
首先我们对窗口作的一对操作是映射它到屏幕上去和解除它的映射。映射一个窗口的操作将会使一个窗口显示在屏幕上，如我们在简单窗口程序例子里所看到的。解除映射操作将会把窗口从屏幕里移除出去（虽然作为一个逻辑结点它仍然在X服务器里）。这个可以提供产生窗口被隐藏（映射解除）和再显示（映射）的效果。例如，我们的程序里有一个对话框，我们不需要每次在需要它显示的时候都重新创建一个窗口，我们只是以映射解除的状态创建一次，在用户需要的时候简单的把它映射到屏幕上去就行了。这比每一次都创建它和销毁它要快多了，当然，这需要在客户端和服务器端同时使用更多的内存。
你应该还记得映射操作是使用函数XMapWindow()。映射解除操作是使用函数XUnmapWindow()，下面是如何使用它们：
```
/* make the window actually appear on the screen. */
XMapWindow(display, win);
/* make the window hidden. */
XUnmapWindow(display, win);
```
除非整个窗口被其它窗口给覆盖了，一个暴露事件将在映射操作后发给应用程序。
## 在屏幕移动一个窗口
我们想做的另一个操作是在屏幕里移动窗口。使用函数XMoveWindow()可以完成这个操作。它接受窗口的新坐标，使用的方法和函数XCreateSimpleWindow()是一样的。一下是调用的例子：
```
/* move the window to coordinates x=400 and y=100. */
XMoveWindow(display, win, 400, 100);
```
注意当窗口移动的时候，窗口的部分可能后被遮住或被重新暴露，这样我们就可能会收到暴露事件。
## 改变窗口尺寸
接下来我们要做的是改变一个窗口的尺寸。使用函数XResizeWindow()可以完成这个操作：
```
/* resize the window to width=200 and height=300 pixels. */
XResizeWindow(display, win, 200, 300);
```
我们可以合并移动和改变尺寸操作为一个操作，使用函数XMoveResizeWindow()：
```
/* move the window to location x=20 y=30, and change its size */
/* to width=100 and height=150 pixels.                        */
XMoveResizeWindow(display, win, 20, 30, 100, 150);
```
## 改变窗口们的栈顺序 - 提升和降低
到目前为止我们已经改变了一个单独窗口的许多属性。接下来我们将看看窗口之间的属性。其中一个就是它们的栈属性。也就是说，窗口是如何在屏幕上排列的。最前面的窗口我们说它是在栈顶，最后面的窗口我们说它是在栈底。下面演示我们如何改变窗口的栈顺序：
```
/* move the given window to the top of the stack. */
XRaiseWindow(display, win1);
/* move the given window to the bottom of the stack. */
XLowerWindow(display, win1);
```
## 图标化和恢复一个窗口
在这里我们将要讲解的最后一个操作就是如何把一个窗口变换成图标状态和恢复它。使用函数XIconifyWindow()来把一个窗口变换成图标状态，使用函数XMapWindow()来恢复它。为了帮助理解为什么图标化函数没有一个对应的反函数，我们必须理解当一个窗口被图标化时，实际发生的事情是那个窗口被解除映射了，而它的图表被映射了。结果，如果想使哪个窗口在出现，我们只需要简单的映射它一下就行了。图标实际上是另一个窗口，只不过它与我们的窗口有非常强的联系关系。下面演示如何图标化一个窗口并恢复它：
```
/* iconify our window. Make its icon window appear on the same */
/* screen as our window (assuming we created our window on the */
/* default screen).                                            */
XIconifyWindow(display, win, DefaultScreen(display));
/* de-iconify our window. the icon window will be automatically */
/* unmapped by this operation.                                  */
XMapWindow(display, win);
```
## 获得一个窗口的信息
与可以为窗口设置许多属性相同，我们也可以要求X服务器提供这些属性的值。例如，我们可以检查窗口现在在屏幕里什么位置，当前尺寸，是否被映射了等等。函数XGetWindowAttributes()可以帮助我们获取那些信息：
```
/* this variable will contain the attributes of the window. */
XWindowAttributes win_attr;
/* query the window's attributes. */
Status rc = XGetWindowAttributes(display, win, &win_attr);
```
结构体XWindowAttributes包含了很多数据域，下面是它的一部分：
- `int x, y;`
窗口的位置，相对于它的父窗口。- `int width, height;`
窗口的宽和高（单位，像素）。- `int border_width`
窗口的边框宽度- `Window root;`
根窗口，也就是我们的窗口在那个窗口里被显示了。
这个函数有些问题，就是它返回的是相对于父窗口的位置。这对一些窗口的操作（例如XMoveWindow）是没有什么意义的。为了解决这个问题，我们需要使用两步的操作。首先，我们找出窗口的父窗口的ID。然后我们在使用它来确定窗口相对于屏幕的坐标。我们使用两个前面没有介绍的函数来完成这个计算，XQueryTree()和XTranslateCoordinates()。这两个函数的功能超出了我们的需要，所以我们只关注我们需要的：
```
/* these variables will eventually hold the translated coordinates. */
int screen_x, screen_y;
/* this variable is here simply because it's needed by the          */
/* XTranslateCoordinates function below. For its purpose, see the   */
/* manual page.                                                     */
Window child_win;
/* this variable will store the ID of the parent window of our window. */
Window parent_win;
/* this variable will store the ID of the root window of the screen    */
/* our window is mapped on.                                            */
Window root_win;
/* this variable will store an array of IDs of the child windows of    */
/* our window.                                                         */
Window* child_windows;
/* and this one will store the number of child windows our window has. */
int num_child_windows;
/* finally, make the query for the above values. */
XQueryTree(display, win,
           &root_win,
           &parent_win,
           &child_windows, &num_child_windows);
/* we need to free the list of child IDs, as it was dynamically allocated */
/* by the XQueryTree function.                                            */
XFree(child_windows);
/* next, we make the coordinates translation, from the coordinates system */
/* of the parent window, to the coordinates system of the root window,    */
/* which happens to be the same as that of the screen, since the root     */
/* window always spans the entire screen size.                            */
/* the 'x' and 'y' values are those previously returned by the            */
/* XGetWindowAttributes function.                                         */
XTranslateCoordinates(display, parent_win, root_win,
                      win_attr.x, win_attr.y, &screen_x, &screen_y,
                      &child_win);
/* at this point, screen_x and screen_y contain the location of our original */
/* window, using screen coordinates.                                         */
```
你可以看到Xlib有时候会让我们处理问题时变得很麻烦。
以上的内容可以参考例子[window-operations.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/window-operations.c) 程序。
# 使用颜色来绘制彩虹
到目前为止，我们的绘制操作都只使用了黑白两色。现在我们就看看如何使用丰富的颜色来绘制。
## 颜色映射
首先，是没有完全足够的颜色的。屏幕控制器同时只能支持有限的颜色。因此，一个应用程序不能只是要求使用颜色“轻紫红”就盼望这个颜色能被支持。每个应用分配它自己所需要的颜色，如果全部的16或256色入口都已经在使用了，下一个颜色的分配就会失败。
结果，就介绍使用“一个颜色映射”。一个颜色映射是一个与屏幕控制器同时可以支持的颜色数相同的表。每个表中的节点都为每种颜色包含一个RGB(红，绿和蓝)。当一个应用想在屏幕上绘制时，它并不指定使用什么颜色，而是指定使用映射表里那一个节点，因此，改变表里某个节点的值将会改变程序绘制的颜色。
为了能让程序员使用他想要的颜色来绘制，提供了颜色映射分配函数。你可以要求分配一个颜色映射节点来对应一个RGB值，然后一个节点的索引值返回给你。如果表满了，这个操作将会失败。你可以接下来要求一个相近的颜色来满足你的需要。这意味着一个相近的颜色将会被绘制到屏幕上去。
在当今的X服务器使用的现代显示器上，一般都可以支持上百万的颜色，上面那个限制也许看起来挺傻的，但是记住还有很多古旧的显示卡和显示器在被使用。使用颜色映射，你可以不必考虑服务器的屏幕硬件细节就可以使用它们。在一个支持上百万的显示器上，任何颜色的分配请求都应该会成功。在一个职能支持有限颜色的显示器上可能会使用一个相近颜色来代替你的要求，这可能不好看，但你的程序仍然能工作。
## 分配和释放颜色映射
当你使用Xlib绘制的时候，你可以选择屏幕的标准颜色映射，或者为你的窗口分配一个新的颜色映射。在后一种情况下，每次鼠标移动到你的窗口上时，你窗口的颜色映射都将替换缺省的屏幕映射，然后你就会看到屏幕花一下其它的窗口颜色也改变了。实际上，这和你在使用“-install”选项运行X应用时效果一样。
系统定义了宏DefaultColormap来获取屏幕的标准颜色映射：
```
Colormap screen_colormap = DefaultColormap(display, DefaultScreen(display));
```
上面的调用将会返回第一个屏幕的缺省颜色映射的句柄（再多余的提醒一下，一个X服务器可以同时支持数个不同的屏幕，每个屏幕都可以有自己的资源）。
另一个选项，分配一个颜色映射，像下面这样工作：
```
/* first, find the default visual for our screen. */
Visual* default_visual = DefaultVisual(display, DefaultScreen(display));
/* this creates a new color map. the number of color entries in this map */
/* is determined by the number of colors supported on the given screen.  */
Colormap my_colormap = XCreateColormap(display,
                                       win,
                                       default_visual,
                                       AllocNone);
```
注意，window参数是用来只允许X服务器为指定屏幕分配颜色映射。我们接下来可以使用分配来的颜色映射给同一个屏幕里的任意一个窗口使用。
## 分配和释放颜色节点
一旦我们获得了颜色映射的访问，我们就可以开始分配颜色。使用函数XAllocNameColor()和XAllocClor()来完成这个工作。首先函数XAllocNameColor()获得颜色的名字（例如"红"，"蓝"，"棕"等等）然后获得能使用的实际相近颜色。函数XAllocColor()访问RGB颜色。两个函数都使用XColor结构，它有以下的一些数据域：
- `unsigned long pixel`
颜色映射节点的索引。- `unsigned short red`
RGB颜色值的红色部分。- `unsigned short green`
RGB颜色值的绿色部分。- `unsigned short blue`
RGB颜色值的蓝色部分。
下面是使用的例子：
```
/* this structure will store the color data actually allocated for us. */
XColor system_color_1, system_color_2;
/* this structure will store the exact RGB values of the named color.  */
/* it might be different from what was actually allocated.             */
XColor exact_color;
/* allocate a "red" color map entry. */
Status rc = XAllocNamedColor(display,
                             screen_colormap,
                             "red",
                             &system_color_1,
                             &exact_color);
/* make sure the allocation succeeded. */
if (rc == 0) {
    fprintf(stderr,
            "XAllocNamedColor - allocation of 'red' color failed./n");
}
else {
    printf("Color entry for 'red' - allocated as (%d,%d,%d) in RGB values./n",
           system_color_1.red, system_color_1.green, system_color_1.blue);
}
/* allocate a color with values (30000, 10000, 0) in RGB. */
system_color_2.red = 30000;
system_color_2.green = 10000;
system_color_2.blue = 0;
Status rc = XAllocColor(display,
                        screen_colormap,
                        &system_color_2);
/* make sure the allocation succeeded. */
if (rc == 0) {
    fprintf(stderr,
            "XAllocColor - allocation of (30000,10000,0) color failed./n");
}
else {
    /* do something with the allocated color... */
    .
    .
}
```
## 使用一个眼色绘制
我们在分配了希望的颜色之后，我们可以使用它们绘制文本或图形。为了达到目的，我们需要把获得的颜色设置给一些GC（图形上下文）作为前景色和背景色，然后使用设置好的GC来进行绘制。使用函数XSetForeground()和XSetBackground()来进行，如下：
```
/* use the previously defined colors as the foreground and background  */
/* colors for drawing using the given GC. assume my_gc is a handle to */
/* a previously allocated GC.                                         */
XSetForeground(display, my_gc, screen_color_1.pixel);
XSetForeground(display, my_gc, screen_color_2.pixel);
```
如你所见，这个是个使用的例子。实际的绘制工作使用我们以前介绍的绘图函数。注意，为了使用各种颜色完成绘制工作，我们可以使用两种方法。我们可以在调用绘图函数前改变GC的值，也可以使用代表不同颜色的GC。由你自己根据情况使用哪种方法。注意，使用多个GC降消耗X服务器额外的资源，但这样可以使你的代码显的更紧凑。
作为使用颜色绘制的例子，请察看例子程序[color-drawing.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/color-drawing.c) 。这是程序[simple-drawing.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/simple-drawing.c) 的一个拷贝，我们只是添加了颜色的部分在里面。
# X Bitmaps和Pixmaps
一个被称为多媒体的程序所有作的一件事情就是显示图片。在X的世界里，使用bitmaps和pixmaps来实现这个功能。在为我们的程序设置图标的介绍里我们已经使用了它们。现在让我们进一步对它们进行研究，看看在一个窗口里是如何绘制它们的。
在进入之前有一点需要注意，Xlib不能处理许多现在流行的图片格式，例如gif，jpeg或tiff。这些被留给了应用程序（或者是一些图形处理库）来转换成X服务器可以接受的x bitmaps和x pixmaps。
## 什么是一个X Bitmap？和X Pixmap？
一个Xbitmap是一个有X窗口系统定义的双色图形格式。在保存在一个文件里的时候，bitmap数据看起来就像一段C源程序。它包括定义图片宽高的变量，一个包含比特值得矩阵（矩阵的尺寸=宽*高），和一个可选的热点位置（将会在后面的鼠标光标的部分进行解释）。
一个X pixmap是一个X窗口系统在内存里保存图像的格式。该格式可以储存黑色和白色的图片（例如X bitmaps）也可以保存带颜色的图片。这实际上是X协议唯一能支持的图片格式，任何图片如果想被显示在屏幕上前都要先被转换成这种格式。
实际上，一个X pixmap应该被认为是一个没有被绘制到屏幕上的窗口。许多在窗口上的图形操作也可以工作于X pixmap，只不过使用X pixmap ID来代替窗口ID。事实上，如果你查看手册，你会发现所有的这些函数都是接受一个叫"可画"参数而不是一个窗口参数。因为这两种类型都是可画的，它们都可以被用在例如函数XDrawArc()，XDrawText()等等。
## 从一个文件里读取一个Bitmap
在图标的程序里，我们已经看过如何从一个文件里把一个Bitmap装载到内存里。前面我们使用的方法是使用C预编译器"#include"来进行，下面我们看看如何直接从文件里读取。
```
/* this variable will contain the ID of the newly created pixmap.    */
Pixmap bitmap;
/* these variables will contain the dimensions of the loaded bitmap. */
unsigned int bitmap_width, bitmap_height;
/* these variables will contain the location of the hot-spot of the   */
/* loaded bitmap.                                                    */
int hotspot_x, hotspot_y;
/* this variable will contain the ID of the root window of the screen */
/* for which we want the pixmap to be created.                        */
Window root_win = DefaultRootWindow(display);
/* load the bitmap found in the file "icon.bmp", create a pixmap      */
/* containing its data in the server, and put its ID in the 'bitmap'  */
/* variable.                                                          */
int rc = XReadBitmapFile(display, root_win,
                         "icon.bmp",
                         &bitmap_width, &bitmap_height,
                         &bitmap,
                         &hotspot_x, &hotspot_y);
/* check for failure or success. */
switch (rc) {
    case BitmapOpenFailed:
        fprintf(stderr, "XReadBitmapFile - could not open file 'icon.bmp'./n");
        break;
    case BitmapFileInvalid:
        fprintf(stderr,
                "XReadBitmapFile - file '%s' doesn't contain a valid bitmap./n",
                "icon.bmp");
        break;
    case BitmapNoMemory:
        fprintf(stderr, "XReadBitmapFile - not enough memory./n");
        break;
    case BitmapSuccess:
        /* bitmap loaded successfully - do something with it... */
        .
        .
        break;
}
```
注意对于给定的bitmap参数"root_win"什么作用也不起 - 读取的bitmap并不与这个窗口相联系。这个窗口句柄只是被用来指明bitmap所使用的屏幕。这是非常重要的，bitmap必须支持与屏幕相同数量的颜色，这样它才能发挥作用。
## 在一个窗口里绘制图形
一旦我们获得了从bitmap里生成的pixmap的句柄，我们就可以使用函数XCopyPlane()把它绘制到窗口里。这个函数可以帮助我们指定什么（一个窗口，甚至另一个pixmap）可以画到这个pixmap上去。
```
/* draw the previously loaded bitmap on the given window, at location   */
/* 'x=100, y=50' in that window. we want to copy the whole bitmap, so   */
/* we specify location 'x=0, y=0' of the bitmap to start the copy from, */
/* and the full size of the bitmap, to specify how much of it to copy.  */
XCopyPlane(display, bitmap, win, gc,
          0, 0,
          bitmap_width, bitmap_height,
          100, 50,
          1);
```
如你所见，我们可以只拷贝制定的矩形区而不是整个pixmap。另外还需要注意的是函数XCopyPlane的最后一个参数（那个结尾的"1"）。该参数指定了那个平面被从源里拷贝出来。对于bitmaps，我们通常只拷贝平面1。到了下面我们讨论颜色深度的时候你就能确切的明白为什么这么做。
## 创建一个Pixmap
有时我们需要创建一个没有初始化的pixmap，这样我们可以接下来在它上面绘制。这对图像绘制程序是非常有用的。另外，这对读取各种格式的图像数据也是非常有用的。
```
/* this variable will store the handle of the newly created pixmap. */
Pixmap pixmap;
/* this variable will contain the ID of the root window of the screen */
/* for which we want the pixmap to be created.                        */
Window root_win = DefaultRootWindow(display);
/* this variable will contain the color depth of the pixmap to create. */
/* this 'depth' specifies the number of bits used to represent a color */
/* index in the color map. the number of colors is 2 to the power of   */
/* this depth.                                                         */
int depth = DefaultDepth(display, DefaultScreen(display));
/* create a new pixmap, with a width of 30 pixels, and height of 40 pixels. */
pixmap = XCreatePixmap(display, root_win, 30, 40, depth);
/* just for fun, draw a pixel in the middle of this pixmap. */
XDrawPoint(display, pixmap, gc, 15, 20);
```
## 在一个窗口里绘制一个Pixmap
我们在获得了一个pixmap的句柄后，我们就可以使用它在窗口里绘制，使用函数XCopyArea()。
```
/* draw the previously loaded bitmap on the given window, at location   */
/* 'x=100, y=50' in that window. we want to copy the whole bitmap, so   */
/* we specify location 'x=0, y=0' of the bitmap to start the copy from, */
/* and the full size of the bitmap, to specify how much of it to copy.  */
XCopyPlane(display, bitmap, win, gc,
          0, 0,
          bitmap_width, bitmap_height,
          100, 50,
          1);
```
如你所见，我们可以拷贝指定的矩形区域而不是整个pixmap。
另外一个需要被强调注意的是 - 可以在同一个屏幕上创建不同深度的pixmap。当我们进行拷贝作业时（往一个窗口上拷贝pixmap等等），我们应该保证源和目标是用相同的深度。如果两个的深度不一样，操作将会失败。除非我们使用前面介绍的函数XCopyPlane()可以完成这个操作。在那样一种情况下，我们拷贝指定的平面到窗口上去，实际上指定了每一个被拷贝的颜色位。这个操作可以制作许多特殊的效果，但这超出了本文的范围。
## 释放一个Pixmap
最后，当我们完成了对一个pixmap的操作，我们应该释放它所占的资源。使用函数XFreePixmap()。
```
/* free the pixmap with the given ID. */
XFreePixmap(display, pixmap);
```
在释放一个pixmap之后 - 我们绝对不能再访问它。
作为总结这一章，看一下程序[draw-pixmap.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/draw-pixmap.c) 。
# 改变鼠标光标
我们经常看到改变鼠标光标的程序（经常被称为X光标）。例如，一个正在埋头工作的程序经常会把光标变成沙漏，提示用户需要等待才能处理新的请求。如果没有这么个提示方法，用户会认为程序已经卡住了。下面让我们看看如何为我们的窗口改变鼠标光标。
## 创建和销毁鼠标光标
系统提供了两个方法来创建光标。其中一个是使用系统预定义的形状，由Xlib支持。另一个是有程序提供一个bitmap来显示。
使用前一种方法时，我们使用一个特殊的字体名字"cursor"和相应的函数XCreateFontCursor()。该函数接受一个形状指示然后返回一个代表生成的光标的句柄。文件``
列出了系统支持的光标类型，下面的是其中的三个：
- `XC_arrow`
X服务器显示的普通光标。- `XC_pencil`
一个笔状的光标。- `XC_watch`
一个表状沙漏
使用这些符号来创建光标是非常简单的：
```
#include <X11/cursorfont.h>    /* defines XC_watch, etc. */
/* this variable will hold the handle of the newly created cursor. */
Cursor watch_cursor;
/* create a sand watch cursor. */
watch_cursor = XCreateFontCursor(display, XC_watch);
```
另一种创建鼠标光标的方法时使用一对pixmaps。一个pixmap定义了光标的形状，另一个是个面具，来指定前一个的什么内容被显示。其它的内容将变成透明的。创建一个那样的光标使用函数XCreatePixmapCursor()。下面的例子里，我们将使用"icon.bmp"来创建光标。我们将假设它已经被装载到内存里去了，并已经被转换成pixmap，返回的句柄被保存到"bitmap"变量里。我们希望它是完全透明的。也就是说，只有黑色颜色的部分会被确实画在屏幕上。为了实现这个效果，我们将会既用它来做光标pixmap且做面具pixmap。希望你能明白为什么这样...
```
/* this variable will hold the handle of the newly created cursor. */
Cursor icon_cursor;
/* first, we need to define foreground and background colors for the cursor. */
XColor cursor_fg, cursor_bg;
/* access the default color map of our screen. */
Colormap screen_colormap = DefaultColormap(display, DefaultScreen(display));
/* allocate black and while colors. */
Status rc = XAllocNamedColor(display,
                             screen_colormap,
                             "black",
                             &cursor_fg,
                             &cursor_fg);
if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - cannot allocate 'black' ??!!??/n");
    exit(1);
}
Status rc = XAllocNamedColor(display,
                             screen_colormap,
                             "white",
                             &cursor_bg,
                             &cursor_bg);
if (rc == 0) {
    fprintf(stderr, "XAllocNamedColor - cannot allocate 'white' ??!!??/n");
    exit(1);
}
/* finally, generate the cursor. make the 'hot spot' be close to the */
/* top-left corner of the cursor - location (x=5, y=4). */
icon_cursor = XCreatePixmapCursor(display, bitmap, bitmap,
				  &cursor_fg, &cursor_bg,
                                  5, 4);
```
上面需要说明的是参数"hot spot"。当我们定义了一个光标，我们需要指明光标里的哪一个像素用来生成各种鼠标事件。通常，我们根据习惯来指定一个看起来像"hot spot"的点。例如一个箭头形状的光标，我们就会选择箭头尖为"hot spot"。
最后，我们不需要再使用光标时，我们可以使用函数XFreeCursor()来释放它的资源：
```
XFreeCursor(display, icon_cursor);
```
## 设置一个窗口的鼠标光标
我们在创建了光标后，就可以告诉X服务器把它贴到我们的任何窗口上去。使用函数XDefineCursor()，X服务器就会在每一次光标移进指定的窗口时改变光标的形状。稍候我们可以使用函数XUndefinCursor()来撤销刚才的指定。这样，鼠标再移进指定的窗口时就会使用缺省的光标。
```
/* attach the icon cursor to our window. */
XDefineCursor(display, win, icon_cursor);
/* detach the icon cursor from our window. */
XUndefineCursor(display, win);
```
作为例子，请查看程序[cursor.c](http://users.actcom.co.il/~choo/lupg/tutorials/xlib-programming/cursor.c) 。
[http://blog.csdn.net/kaku_you/archive/2003/03/31/15437.aspx](http://blog.csdn.net/kaku_you/archive/2003/03/31/15437.aspx)
[http://blog.csdn.net/kaku_you/archive/2003/07/07/15438.aspx](http://blog.csdn.net/kaku_you/archive/2003/07/07/15438.aspx)
