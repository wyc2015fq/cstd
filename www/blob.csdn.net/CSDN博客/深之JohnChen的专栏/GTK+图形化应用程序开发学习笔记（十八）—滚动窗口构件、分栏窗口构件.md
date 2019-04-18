# GTK+图形化应用程序开发学习笔记（十八）—滚动窗口构件、分栏窗口构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 23:44:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3584


**GTK+图形化应用程序开发学习笔记（十八）—滚动窗口构件、分栏窗口构件**

一、滚动窗口构件

滚动窗口构件(GtkScrolledWindow)用于创建一个可滚动区域，并将其他构件放入其中。可以在滚动窗口中插入任何其他构件，在其内部的构件不论尺寸大小都可以通过滚动条访问到。

1．

|名称:：|gtk_scrolled_window_new|
|----|----|
|功能：|创建滚动窗口构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_scrolled_window_new( GtkAdjustment *hadjustment,GtkAdjustment *vadjustment );|
|参数：|hadjustmentvadjustment|
|返回值：|新的滚动窗口构件|







第一个参数是水平方向的调整对象，第二个参数是垂直方向的调整对象。它们总是设置为NULL。

2．

|名称:：|gtk_scrolled_window_set_policy|
|----|----|
|功能：|设置滚动条出现的方式|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_scrolled_window_set_policy( GtkScrolledWindow *scrooled_window, GtkPolicyType hscrollbar_policy, GtkPolicyType vscrollbar_policy );|
|参数：|hscrollbar_policy水平滚动条出现的方式vscrollbar_policy垂直滚动条的方式|
|返回值：|无|








滚动条的方式取值可以为GTK_POLICY_AUTOMATIC或GTK_POLICY_ALWAYS。当要求滚动条根据需要自动出现时,可设为GTK_POLICY_AUTOMATIC；若设为GTK_POLICY_ALWAYS，滚动条会一直出现在滚动窗口构件上。

3．

|名称:：|gtk_scrolled_window_add_with_viewport|
|----|----|
|功能：|将子构件添加到滚动窗口构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_scrolled_window_add_with_viewport(GtkScrolledWindow *scrolled_window, GtkWidget *child);|
|参数：|scrolled_window滚动窗口构件child子构件|
|返回值：|无|





下面是滚动窗口构件的程序例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *scrolled;

GtkWidget *text;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_size_request(window,200,200);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

scrolled=gtk_scrolled_window_new(NULL,NULL); /*创建滚动窗口构件*/

gtk_container_add(GTK_CONTAINER(window),scrolled);/*将滚动窗口构件加入窗体*/

gtk_widget_show(scrolled);/*显示滚动窗口构件*/

text=gtk_text_view_new();/*创建文本视图构件*/

gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled),text);/*将文本视图构件加入滚动窗口*/

gtk_widget_show(text);

gtk_widget_show(window);

gtk_main();

}

在上面的程序中我们用到了GtkTextView，关于GtkTextView我将在后面介绍。

程序运行后得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070614214431.png)

二、分栏窗口构件

分栏窗口构件（GtkPanedWindow）可以把窗口分为两部分，两部分的尺寸由用户控制，它们之间有一个凹槽，上面有一个把柄，用户可以拖动此手柄改变两部分的比例。

分栏窗口构件分为GtkHPaned（水平分栏窗口构件）和GtkVPaned（垂直分栏窗口构件）。其中水平分栏窗口构件用函数gtk_hpaned_new函数创建，垂直分栏窗口构件用gtk_vpaned_new函数创建。

4．

|名称:：|gtk_hpaned_newgtk_vpaned_new|
|----|----|
|功能：|创建分栏窗口构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_hpaned_new (void); /*水平分栏窗口构件* /GtkWidget *gtk_vpaned_new (void); /*垂直分栏窗口构件* /|
|参数：|无|
|返回值：|新的分栏窗口构件|






创建了分栏窗口构件后，可以在它的两边添加子构件。我们用gtk_hpaned_new

和gtk_vpaned_new函数完成这些功能。

5．

|名称:：|gtk_hpaned_newgtk_vpaned_new|
|----|----|
|功能：|创建分栏窗口构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_hpaned_new (void); /*水平分栏窗口构件* /GtkWidget *gtk_vpaned_new (void); /*垂直分栏窗口构件* /|
|参数：|无|
|返回值：|新的分栏窗口构件|






gtk_paned_add1() 将子构件添加到分栏窗口构件的左边或顶部。gtk_paned_add2() 将子构件添加到分栏窗口构件的右边或下部。

6．

|名称:：|gtk_paned_set_handle_size|
|----|----|
|功能：|设置分栏构件的手柄尺寸|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_paned_set_handle_size( GtkPaned *paned,guint16 size);|
|参数：|paned分栏窗口构件size手柄尺寸|
|返回值：|无|








7．

|名称:：|gtk_paned_set_gutter_size|
|----|----|
|功能：|设置两部分之间的凹槽的尺寸|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_paned_set_gutter_size( GtkPaned *paned,guint16 size);|
|参数：|paned分栏窗口构件size凹槽的尺寸|
|返回值：|无|






下面我们结合滚动窗口，在分栏构件窗口两面分别加入一个分栏窗口。

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *scrolled;

GtkWidget *text;

GtkWidget *paned;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_size_request(window,200,200);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

paned=gtk_hpaned_new();/*创建纵向分栏窗口构件*/

gtk_container_add(GTK_CONTAINER(window),paned);/*将分栏窗口构件加入窗体*/

gtk_widget_show(paned);

scrolled=gtk_scrolled_windwo_new(NULL,NULL);/*创建滚动窗口构件*/

gtk_paned_add1(GTK_PANED(paned),scrolled);/*将滚动窗口构件加入到分栏窗口构件的左窗口*/

gtk_widget_show(scrolled);

text=gtk_text_view_new( );

gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled),text);

gtk_widget_show(text);

scrolled=gtk_scrolled_windwo_new(NULL,NULL);

gtk_paned_add2(GTK_PANED(paned),scrolled);

gtk_widget_show(scrolled);

text=gtk_text_view_new( );

gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled),text);

gtk_widget_show(text);

gtk_widget_show(windwo);

gtk_main();

}

程序运行后得到如下窗体：

![](http://blog.chinaunix.net/photo/22935_070614214440.png)

