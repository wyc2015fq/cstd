# X Window开发编程 - u013366022的专栏 - CSDN博客
2015年04月13日 19:25:33[slitaz](https://me.csdn.net/u013366022)阅读数：744
开始编程前先要有一个好的开发工具 可以快速的查找函数与宏定义及结构定义
我推荐vim
然后是要随时准备看man 基本上x开发不难 但是看man理解每个函数
那对以后的开发会更快
下面先开始vim相关的配置
1 创建/usr/include目录下的定义索引
cd /usr/include
ctags *
2 创建 /usr/include/X11目录下的定义索引
cd /usr/include/X11
ctags *
3 编辑你的~/.vimrc
vi ~/.vimrc 增加下面两行
set path+=.,./include,../include,/usr/include,/usr/include/**,/usr/X11R6/include,/usr/local/include
set tags=./tags,../tags,../../tags,/usr/include/tags,/usr/include/X11/tags
4 快速的使用vim的一些技巧
在x开发中 可能会看到很多结构或是宏定义
这时可以在你想查找的定义上使用
ctrl-w-]键跳转到它的定义处
:q退出定义窗口
ctrl-P 单词完成
快速的make与改错
在vim下输入:
:mak
然后使用
cn或cp浏览错误
编译
gcc -L/usr/X11R6/lib -lX11 filename.c -o filename.o
Xwindows开发学习
第一个程序
hello world
每个Xwindows程序都由三个部分组成
1 Desplay 这个就是X服务器
2 Win 程序的窗口 可能有一个或多个
3 gc 图形上下文 如果你想在程序窗口中输出内容时 你就需要到这个东西
每个程序的流程就是
1 打开显示
2 创建窗口
3 创建GC
4 选择窗口关心的事件
5 映射窗口 这时在屏幕上显示程序界面
6 进程事件循环读事件并处理
下面就是hello world的流程
代码:
#include ≶X11/Xlib.h>
#include ≶X11/Xutil.h>
#include ≶X11/Xos.h>
#include ≶X11/Xatom.h>
#define DUMP_ERR printf
int main()
{
Display *display;
int screennum;
int width;
int height;
const char *strdraw = “hello world”;
Window win;
GC gc;
//打开显示 没有指明打开哪个显示时使用环境变量中的DESPLAY
display = XOpenDisplay(NULL);
if( !display ){
DUMP_ERR(”call XOpenDisplay(%s) fail/n”,XDisplayName(NULL));
return 1;
}
//得到系统当前的屏幕 (如果使用虚拟屏幕的话 一般会有几个)
// get default screen
screennum = DefaultScreen(display);
//根据屏幕的大小决定窗口的大小
width = DisplayWidth(display,screennum)/4;
height = DisplayHeight(display,screennum)/4;
//创建窗口
// create window
win = XCreateSimpleWindow(display,
RootWindow(display,screennum),
0,0,width,height,3,
BlackPixel(display,screennum),
WhitePixel(display,screennum));
//选择窗口关心的事件
// select event
XSelectInput(display,win,
ExposureMask|KeyPressMask
|ButtonPressMask|StructureNotifyMask);
//创建GC(图形上下文)
// create gc
{
unsigned long valuemask = 0;
XGCValues values;
gc = XCreateGC(display,win,valuemask,&values);
}
// 映射窗口 只有map后窗口才会在屏幕上显示
// show window
XMapWindow(display,win);
//进入事件循环
// event loop
while(1){
XEvent event;
XNextEvent(display,&event); // 读一个事件
switch(event.type){
case Expose: // expose window 显示事件
if( event.xexpose.count != 0) break;
XDrawString(display,win,gc,10,10,
strdraw,strlen(strdraw));
break;
// 按键事件
case ButtonPress:
case KeyPress:
XFreeGC(display,gc);
XCloseWindow(win);
XCloseDisplay(display);
return 0;
default:
break;
}
}
}
上面是一个最基本的例子 基本上除了hello world外什么事也不做
下面开始介绍在X上画图的例子
上面说过GC 画图就是在gc上输出内容
每个gc都有属性 可以通过XChangeGC修改或是通过其它修改gc性能的函数
如
XSetLineAttributes修改
这两个的效果是一样的
代码:
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#define DUMP_ERR printf
static void draw(Display *display,Window win,GC gc,int width,int height);
int main()
{
Display *display;
int screennum;
int width;
int height;
Atom protocols;
const char *strdraw = “hello world”;
Window win;
GC gc;
display = XOpenDisplay(NULL);
if( !display ){
DUMP_ERR(”call XOpenDisplay(%s) fail/n”,XDisplayName(NULL));
return 1;
}
// get default screen
screennum = DefaultScreen(display);
width = DisplayWidth(display,screennum)/2;
height = DisplayHeight(display,screennum)/2;
// create window
win = XCreateSimpleWindow(display,
RootWindow(display,screennum),
0,0,width,height,3,
BlackPixel(display,screennum),
WhitePixel(display,screennum));
//增加这个代码是捕获程序退出事件
protocols = XInternAtom(display, “WM_DELETE_WINDOW”, True);
XSetWMProtocols(display, win, &protocols, 1);
// select event
XSelectInput(display,win,
ExposureMask|KeyPressMask
|ButtonPressMask|StructureNotifyMask);
// create gc
{
unsigned long valuemask = 0;
XGCValues values;
gc = XCreateGC(display,win,valuemask,&values);
}
// show window
XMapWindow(display,win);
// event loop
while(1){
XEvent event;
XNextEvent(display,&event);
switch(event.type){
case Expose: // expose window
if( event.xexpose.count != 0) break;
draw(display,win,gc,width,height);
//XDrawString(display,win,gc,10,10,
//strdraw,strlen(strdraw));
break;
case ConfigureNotify: // when the window’s size change
width = event.xconfigure.width;
height = event.xconfigure.height;
break;
case ClientMessage :
if(event.xclient.data.l[0] == protocols){
DUMP_ERR(”recv destroy notify/n”);
XFreeGC(display,gc);
XDestroyWindow(display, win);
XCloseDisplay(display);
}
return 0;
default:
break;
}
}
}
// 使用changeGC修改属性的代码 只有valuemask被设置的属性才会根据XCGValues里面的值进行修改
static void set_gc_values(Display *display,GC gc)
{
XGCValues values;
values.line_width = 2;
values.line_style = LineSolid;
values.cap_style = CapRound;
values.join_style = JoinRound;
XChangeGC(display,gc,GCLineWidth|GCLineStyle,//|GCCapStyle|GCFillStyle,
&values);
}
// 画画的代码
static void draw(Display *display,Window win,GC gc,int width,int height)
{
int i,x,y;
unsigned int ling_width = 0;
int line_style = LineSolid;// 是实线
int cap_style = CapRound; // 端点类型是圆的
int join_style = JoinRound;// 接点类型是圆的
XPoint points[5] = {
{400,200},
{600,200},
{450,300},
{500,150},
{550,300},
};
// draw point
for(i = 0;i< width;i++)
XDrawPoint(display,win,gc,i,10);
// draw line
y = 30;
for(i = 0;i<10;i+=2){
// int XSetLineAttributes(Display *display, GC gc, unsigned int
// line_width, int line_style, int cap_style, int join_style);
XSetLineAttributes(display,gc,i,line_style,cap_style,join_style);
// int XDrawLine(Display *display, Drawable d, GC gc, int x1, int y1, int
//x2, int y2);
XDrawLine(display,win,gc,10,y,width-10,y);
y += 20;
}
set_gc_values(display,gc);
XDrawRectangle(display,win,gc,10,y,50,50);
XDrawRectangle(display,win,gc,100,y,200,200);
XDrawArc(display,win,gc,100,y,200,200,0,90*64);
XFillArc(display,win,gc,100,y,200,200,90*64,120*64);
XFillPolygon(display,win,gc,points,5,Complex,EvenOddRule);
}
