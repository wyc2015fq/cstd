# GTK+图形化应用程序开发学习笔记（十二）—对话框构件、标尺构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 22:44:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1714


**GTK+图形化应用程序开发学习笔记（十二）—对话框构件、标尺构件**

一、对话框构件

对话框构件非常简单，事实上它仅仅是一个预先组装了几个构件到里面的窗口。对话框的定义如下：

struct GtkDialog{

GtkWindow window; 

GtkWidget *vbox;

GtkWidget *action_area;

};

从上面可以看到，对话框只是简单地创建一个窗口，并在顶部组装一个G t k V B o x，然后在G t k V B o x中组装一个分隔线，再加一个称为“活动区”的G t k H B o x。我们可以在该“活动区”添加一些按纽。

对话框用gtk_dialog_new创建。

1．

|名称:：|gtk_dialog_new|
|----|----|
|功能：|创建对话框构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_dialog_new(void);|
|参数：|无|
|返回值：|新的对话框构件|












#include <gtk/gtk.h>

void button_event(GtkWidget *widget, gpointer *data);

void destroy(GtkWidget *widget, gpointer *data);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

button=gtk_button_new_with_label(“Exit”);

gtk_container_add(GTK_CONTAINER(window),button);

gtk_signal_connect(GTK_OBJECT(button),”clicked”,G_CALLBACK(button_event),NULL);

gtk_widget_show(button);

gtk_widget_show(window);

gtk_main();

}

void button_event(GtkWidget *widget, gpointer *data)

{

GtkWidget *button;

GtkWidget *dialog;

dialog=gtk_dialog_new( );

button=gtk_button_new_with_label(“Yes”);

gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),button,TRUE,TRUE,0);

gtk_signal_connect(GTK_OBJECT(button),”clicked”,G_CALLBACK(gtk_main_quit),NULL);

gtk_widget_show(button);

button=gtk_button_new_with_label(“No”);

gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),button,TRUE,TRUE,0);

gtk_signal_connect(GTK_OBJECT(button),”clicked”,G_CALLBACK(destroy),dialog);

gtk_widget_show(button);

gtk_widget_show(dialog);

}

void destroy(GtkWidget *widget, gpointer *data)

{

gtk_widget_destroy(GTK_WIDGET(data));

}

程序运行后会得到如下界面：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/1633652389497343750.png)

当用户点击按纽会得到如下对话框。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/2633652389497656250.png)

如果用户电击Yes程序退出，如果用户点击No对话框消失。

二、标尺构件

标尺构件(GtkRuler）一般用于在给定窗口中指示鼠标指针的位置。一个窗口可以有一个横跨整个窗口宽度的水平标尺和一个占据整个窗口高度的垂直标尺。标尺上有一个小三角形的指示器标出鼠标指针相对于标尺的精确位置。

有两种标尺构件：G t k H R u l e r（水平）和G t k V R u l e r（垂直）。用gtk_hruler_new创建水平标尺构件,用gtk_vruler­_new创建垂直标尺构件。

2．

|名称:：|gtk_hruler_newgtk_vruler_new|
|----|----|
|功能：|创建标尺构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_hruler_new(void); /*水平标尺*/GtkWidget *gtk_vruler_new(void); /*垂直标尺*/|
|参数：|无|
|返回值：|新的标尺构件|






3．

|名称:：|gtk_ruler_set_metric|
|----|----|
|功能：|设置标尺构件的度量单位|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_ruler_set_metric(GtkRuler *ruler, GtkMetricType metric);|
|参数：|ruler标尺构件metric度量单位|
|返回值：|无|






一旦创建了标尺，我们就能指定它的度量单位。标尺的度量单位可以是GTK_PIXELS, GTK_INCHES, GTK_CENTIMETERS. 标尺的度量单位用gtk_ruler_set_metric设置。默认的度量单位是GTK_PIXELS.

4．

|名称:：|gtk_ruler_set_range|
|----|----|
|功能：|设置标尺构件的位置和跨度|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_ruler_set_range(GtkRuler *ruler, gfloat lower, gfloat upper, gfloat position, gfloat max_size);|
|参数：|ruler标尺构件lower表示的开始upper标尺的结束position标尺的指针指示器的初始位置max_size显示最大可能数值|
|返回值：|无|








创建完标尺还要对标尺的跨度和指示器初始位置进行设置，我们可以用gtk_ruler_set_range函数进行设置。

下面是创建标尺构件的程序例子：

#include <gtk/gtk.h>

int main(int argc, char *argv[ ])

{

GtkWidget *window;

GtkWidget *box;

GtkWidget *hrule;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,GTK_SIGNAL_FUNC(gtk_main_quit), NULL);

gtk_container_set_border_width(GTK_CONTAINER(window),10);

gtk_widget_set_usize(window,400,100);

box=gtk_vbox_new(FALSE,0);

hrule=gtk_hruler_new();/*创建标尺*/

gtk_ruler_set_metric(GTK_RULER(hrule),GTK_PIXELS);/*设置标尺单位*/

gtk_ruler_set_range(GTK_RULER(hrule),0,10,0,10);/*设置标尺跨度，指示器初始位置*/

gtk_box_pack_start(GTK_BOX(box), hrule, FALSE,FALSE,0);

gtk_container_add(GTK_CONTAINER(window),box);

gtk_widget_show(box);

gtk_widget_show(hrule);

gtk_widget_show(window);

gtk_main();

}

程序运行后得到如下界面：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/3633652389497968750.png)

