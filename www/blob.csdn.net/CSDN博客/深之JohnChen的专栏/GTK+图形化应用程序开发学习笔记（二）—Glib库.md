# GTK+图形化应用程序开发学习笔记（二）—Glib库 - 深之JohnChen的专栏 - CSDN博客

2008年12月17日 23:41:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2450


**GTK+图形化应用程序开发学习笔记（二）—Glib库**


在学习GTK+之前我们需要先学习一下 glib的有关知识。因为我们将会在以后的学习中遇到这些知识。由于本笔记不是专门介绍glib的，所以下面的介绍不会太详细。

**一、什么是glib库**

glib库是linux平台下最常用的c语言函数库，它具有很好的可移植性和实用性。glib是gtk+库和gnome的基础。glib的各种实用程序具有一致的接口。glib为许多标准的、常用的C语言结构提供了相应的替代物。它的编码风格是半面向对象，标识符加了一个前缀“g”，这也是一种通行的命名约定。使glib库的程序都应该包含glib的头文件glib.h。如果程序已经包含了gtk .h则不需要再包含glib.h。

**二、glib的类型定义**

glib的类型定义不是使用C的标准类型，它自己有一套类型系统。它们比常用的C语言的类型更丰富，也更安全可靠。引进这套系统是为了多种原因。

以下是glib基本类型定义：

整数类型：gint8、guint8、gint16、guint16、gint32、guint32、gint64、guint64。其中gint8是8位的整数，guint8是8位的无符号整数，其他依此类推。这些整数类型能够保证大小。不是所有的平台都提供64位整型，如果一个平台有这些， glib会定义G_HAVE_GINT64。

整数类型gshort、glong、gint和short、long、int完全等价。

布尔类型gboolean：它可使代码更易读，因为普通C没有布尔类型。Gboolean可以取两个值：TRUE和FALSE。实际上FALSE定义为0，而TRUE定义为非零值。

字符型gchar和char完全一样，只是为了保持一致的命名。

浮点类型gfloat、gdouble和float、double完全等价。

指针gpointer对应于标准C的void *，但是比void *更方便。

指针gconst pointer对应于标准C的const void *（注意，将const void *定义为const gpointer是行不通的）。

**三、glib的宏**

glib除了定义一些在c程序中常见的宏外，还定义了一些用于类型转换的宏。如

**GINT_TO_POINTER(p)**

**GPOINTER_TO_INT(p)**

**GUINT_TO_POINTER(p)**

**GPOINTER_TO_UINT(p)**

这些宏允许在一个指针中存储一个整数，但在一个整数中存储一个指针是不行的。如果

**要实现的话，必须在一个长整型中存储指针。**

例如，我们想把一个int型整数转换为pointer型数，我们

**四、内存管理**

glib用自己的g_变体包装了标准的malloc( )和free( )，即g_malloc() 和g_free( )。它们有以下几个小优点：

**• g_malloc()总是返回gpointer，而不是char *，所以不必转换返回值。**

**•如果低层的malloc ( )失败，g_malloc ( )将退出程序，所以不必检查返回值是否是NULL。**

**• g_malloc() 对于分配0字节返回NULL。**

**• g_free()忽略任何传递给它的NULL指针。**



**1．**
|**名称:：**|**g_malloc**|
|----|----|
|**功能：**|**分配内存空间**|
|**头文件：**|**#include <glib.h>**|
|**函数原形：**|**gpointer g_malloc(gulong size);**|
|**参数：**|gulong空间的大小|
|**返回值：**|**指向空间的指针**|





**2．**
|**名称:：**|**g_free**|
|----|----|
|**功能：**|**释放内存空间**|
|**头文件：**|**#include <glib.h>**|
|**函数原形：**|**void g_free(gpointer mem);**|
|**参数：**|**mem志向空间的指针**|
|**返回值：**|**无**|






**五、出错处理函数**

**3．**

|**名称:：**|**g_strerror**|
|----|----|
|**功能：**|**出错处理函数**|
|**头文件：**|**#include <glib.h>**|
|**函数原形：**|**gchar *g_strerror(gint errnum);**|
|**参数：**|**errnum给定的错误代码**|
|**返回值：**|**一条对应于给定错误代码的错误字符串信息**|

**g_strerror返回一条对应于给定错误代码的错误字符串信息，例如“ no such process”等。**



** 4.**

|**名称:：**|**g_error**|
|----|----|
|**功能：**|**显示消息**|
|**头文件：**|**#include <glib.h>**|
|**函数原形：**|**void g_error(gchar *format,…);**|
|**参数：**|**format出错信息**|
|**返回值：**|**无**|

**g_error ****函数显示应用软件中的严重错误。它在错误信息前加上了"** ERROR **"，并结束程序的运行。只当可能发生导致程序退出的错误才使用这一函数.**

**5．**

|**名称:：**|**g_warning**|
|----|----|
|**功能：**|**显示消息**|
|**头文件：**|**#include <glib.h>**|
|**函数原形：**|**void g_warning(gchar *format,…);**|
|**参数：**|**Format出错信息**|
|**返回值：**|**无**|

**g_warning当发生可恢复的错误而且程序能够继续运行时，函数g_warning在错误信息前加上 " ** WARNING ** "，。GTK+用这一函数向程序设计人员显示已经成功处理了的错误。**

六、与错误无关的显示消息函数

6．

|名称:：|g_message|
|----|----|
|功能：|显示消息|
|头文件：|#include <glib.h>|
|函数原形：|void g_message(gchar *format,…);|
|参数：|Format信息|
|返回值：|无|
g_message函数显示与错误无关的消息。在传递的字符串前打印 "message:"

7．

|名称:：|g_print|
|----|----|
|功能：|显示消息|
|头文件：|#include <glib.h>|
|函数原形：|void g_print(gchar *format,…);|
|参数：|format信息|
|返回值：|无|
g_print函数通常用于调试。你可以在开发过程中使用g_print函数，并在实际使用时撤消它的功能。

下面我们用上一节的程序验证着这些函数。

#include <gtk/gtk.h>

int main(int argc, char* argv[])

{

GtkWidget *window;

gtk_init(&argc,&argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_show(window);

g_print(“hello world”);

g_warning(“hello world”);

g_message(“hello world”);

//g_error(“hello world”);

gtk_main();

return FALSE;

}

程序的运行结果为：

#gcc –o base base.c `pkg-config –cflags –libs gtk+-2.0`

#./base

hello world

** (base:8850) :WARNIG ** : hello world

** Message: hello world

七、Timeout 函数

Timeout函数会隔一定时间(毫秒)就自动地调用一次。下面的函数用于添加一个Timeout函数。定时函数类似于回调函数（我们将在下节介绍）。


8．

|名称:：|g_timeout_add|
|----|----|
|功能：|添加一个Timeout函数|
|头文件：|#include <glib.h>|
|函数原形：|gint gtk_timeout_add(guint32 interval,GtkFunction function,gpointer data);|
|参数：|interval调用定时函数的时间间隔function要调用的函数定时函数data传递给定时函数的参数|
|返回值：|返回一个整形“标志“|

可以用下面的函数停止调用定时函数：

9．

|名称:：|g_timeout_remove|
|----|----|
|功能：|停止调用定时函数|
|头文件：|#include <glib.h>|
|函数原形：|void gtk_timeout_remove(gint tag);|
|参数：|tagg_timeout_add函数返回的标志|
|返回值：|无|

还可以让回调函数返回FA L S E或0来停止调用定时函数。也就是说，要想让函数继续调用，必须让它返回一个非0值或T R U E。

定期调用的回调函数声明应该是下面的形式：

gint timeout_callback( gpointer data );

下面是程序例子：

#include <gtk/gtk.h>

gint n=1;

gint function(gpointer data); 

int main(int argc, char* argv[])

{

GtkWidget *window;

gpointer data;

gint sign;

gtk_init(&argc,&argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_show(window);

sign=g_timeout_add(500,function,data);

gtk_main();

return FALSE;

}

gint function(gpointer data)

{

g_print(“hello”);

if(++n>5)

return 0;

}

程序每隔500毫秒输出一个hello.输出5个结束。


