# GTK+图形化应用程序开发学习笔记（五）—组装盒、组合表、固定容器构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月17日 23:56:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2768


**GTK+图形化应用程序开发学习笔记（五）—组装盒、组合表、固定容器构件**

一、组装盒

组装盒(GtkBox)也称为组合构件。使用组装盒可以将多个构件放在一个容器中。容器可以把组装盒看作是一个构件。不像按钮构件，组装盒在屏幕上时看不见的。事实上，它们是同时跟踪若干子构件的不可见容器。组装盒分为纵向组装盒和横向组装盒。纵向组装盒在垂直方向堆积构件，横向组装盒沿水平方向堆积构件。

使用gtk_vbox_new函数建立纵向组装盒，使用gtk_hbox_new函数建立横向组装盒。在建立组装盒以后，可以使用gtk_box_pack_start函数或gtk_box_pack_end函数将构件放到组装盒中。

为了显示构件，必须将构件放入组装盒中，并将组装盒放在容器内。这等效于使容器具有容纳多个构件的能力。虽然组装盒没有任何可视的部分，但是必须使容器和组装盒成为可视。因为可视性影响加到容器内的构件。

2．

|名称:：|gtk_hbox_newgtk_vbox_new|
|----|----|
|功能：|创建一个横向组装盒（gtk_hbox_new）创建一个纵向组装盒（gtk_vbox_new）|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_hbox_new(gboolean homogeneous,gint spacing);GtkWidget *gtk_vbox_new(gboolean homogeneous,gint spacing);|
|参数：|homogeneous子构件是否具有同样的大小spacing子构件间的距离|
|返回值：|新的组装盒|






建立组装盒的函数取两个参数，用来确定组装盒在屏幕上的外观。

第一个参数称为homogeneous.用这一参数来确定所有增加的构件具有同样的大小。例如，在一个横向组装盒中建立5个按钮，分别带有标号：fred、joe、sue、karen和Bartholomew the Great。如果homogeneous参数为FALSE，组装盒建立5个不同大小的按钮，正好在按钮上容纳各自的标号的正文。如果homogeneous参数为TRUE，组装盒计算最大的构件大小，然后按照最大的构件的大小对所有按钮分配同样大小的空间。

第二个参数称为spacing。用它来确定插入组装盒的构件之间的空间。将spacing参数设置为0表示在插入的构件之间不留空间。

3．

|名称:：|gtk_box_pack_startgtk_box_pack_end|
|----|----|
|功能：|将构件放到组装盒中|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_box_pack_start(GtkBox *box,GtkWidget *child,gboolean expend,gboolean fill,guint padding);void gtk_box_pack_end(GtkBox *box,GtkWidget *child,gboolean expend,gboolean fill,guint padding);|
|参数：|box组装盒的名称child 子构件的名称expend构件周围是否还有可扩充的空间fill构件是否需要充分利用构件周围空间padding构件周围要保留多少个填充的像元|
|返回值：|无|








函数gtk_box_pack_start将构件放在顶部（对于纵向组装盒）或左边（对于横向组装盒。函数gtk_box_pack_end将构件放在底部（对于纵向组装盒）或右边（对于横向组装盒）。

gtk_box_paxk_start和gtk_box_pack_end有5个参数，其中3个参数用来确定如何将构件放在组装盒内。

expand参数表示在所有构件加入组装盒以后，构件周围是否还有可扩充的空间。如果用homogenous格式建立组装盒，则忽略这一参数。因为homegeneous参数表示构件在组装盒内使用同样大小的空间。

fill参数表示构件是否需要充分利用构件周围空间。将此参数设置为TRUE允许构件稍稍扩大一点，以充分利用组装盒分配给它的空间。将此参数设置为FALSE强制构件只使用它需要的空间。多余的空间围绕构件的周围分布。

padding参数表示在构件周围要保留多少个填充的像元。多数情况下将它的值设置为0。


#include <gtk/gtk.h>

void PackNewButton(Gtk Widget *box,char *szLabel);

int main(int argc,char *argv[ ])

{

GtkWidget *window; /*定义窗体*/

GtkWidget *box;/*定义组装盒*/

gtk_init(&argc,&argv); /*初始化GTK+库*/

window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL); /*登记destroy信号，调用退出函数*/

box=gtk_vbox_new(FALSE,0); /*创建组装盒*/

PackNewButton(box,”Button1”); /*调用创建按钮函数*/

PackNewButton(box,”Button2”); 

PackNewButton(box,”Button3”); 

PackNewButton(box,”Button4”); 

PackNewButton(box,”Button5”); 

gtk_container_add(GTK_CONTAINER(window),box); /*把组装盒放入窗体*/

gtk_widget_show(box); /*使组装盒可视*/

gtk_widget_show(window); /*使窗体可视*/

gtk_main(); /*等待用户操作*/

}

void PackNewButton(GtkWidget *box,char *szLabel)

{

GtkWidget *button;/*定义按钮*/

button=gtk_button_new_with_label(szLabel); /*创建带标号的按钮*/

gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0); /*把按钮加入组装盒*/

gtk_widget_show(button); /*使按钮可视*/

}

下面我们来编译它：

$gcc –o box box.c `pkg-config –cflags –libs gtk+-2.0`

$./box

程序运行后会出现下面的图形：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/1.png)

本例涉及到了按钮的创建，大家暂时不必关注，我会在下面的章节介绍。

二、组合表

组合表允许将多个构件放入一个窗口内，这一点类似于组装盒。但是，组装盒在放置构件时只允许纵向或横向的二维控制。组合表则类似于HTML表；允许用行和列控制构件的放置，并且对象可以取多行或多列。

4．

|名称:：|gtk_table_new|
|----|----|
|功能：|创建组合表|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget* gtk_table_new(guint rows,guint columns,gboolean homogeneous);|
|参数：|rows表的行数columns表的列数homogeneous子构件是否具有同样的大小|
|返回值：|新的组合表|













rows 和columns表示建表所用的行数和列数。homogeneous的效果和在组装盒中homogeneous的使用效果相同。如果在建立组合表时它被设置为TRUE，表内的表框用最大构件的外框。如果它被设置为FALSE，则将每一列的宽度设置为本例中最大构件的宽度，每一行的高度设置为行中最大构件的高度。

行的排列从0~rows-1,列的排列从0~columns-1.当增加构件时，每个构件提供开始列、结束列、开始行和结束行。如果构件将被放在表的左上角，列的范围从0到1。行的范围从0到1。

5．

|名称:：|gtk_table_attachgtk_table_attch_defaults|
|----|----|
|功能：|将控件加到表中|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_table_attach(GtkTable *table,GtkWidget * child,guint left_attach,guint right_attach,guint top_attach,guintbotton_attach, GtkAttachOptions xoptions,GtkAttachOptions yoptions,guint xpadding,guint ypadding);void gtk_table_attach_defaults( GtkTable *table,GtkWidget * child,guint left_attach,guint right_attach,guint top_attach,guintbotton_attach);|
|参数：|table组合表名child子构件名left_attach横向起始位置right_attach横向结束位置top_attach 纵向起始位置botton_attach纵向结束位置xoptions横向选项yoptions纵向选项xpadding横向填充像元ypadding纵向填充像元|
|返回值：|无|


















两个函数都可以将构件加到表中。

table为表的名字，child和widget为构件的名字。

left_attach、right_attach、top_attach和botton_attach表示控件在表中的位置。

xoptions和yoptions可以用GTK_FILL、GTK_SHRINK和GTK_EXPAND,或者用3者的任意组合。其中GTK_FILL选项表示构件充分利用分配给它的空间进行扩展。GTK_SHRINK选项允许构件缩小到比原来分配的空间还小的空间。GTK_EXPAND选项使表扩展填满它插入的所有空间。

xpadding和ypadding表示围绕构件填充的像元数。

gtk_table_attach_defaults函数取比较少的参数，并对gtk_table_attach函数使用的xoptions、yoptions、xpadding和ypadding参数用缺省值来代替。对gtk_table_attach_defaults函数来说，xpadding和ypadding的缺省值为0；而xoptions和yoptions的却省值为（GTK_FILL|GTK_EXPAND）。

下面是应用组合表的程序例子：

/*table.c*/

#include <getk/gtk.h>

int main(int argc,char *argv[])

{

GtkWidget *window; /*定义窗体*/

GtkWidget *table; /*定义组合表*/

GtkWidget *button; /*定义按钮*/

gtk_init(&argc,&argv); /*初始化*/

window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL); /*登记destroy信号函数*/

table=gtk_table_new(4,4,FALSE); /*创建组合表*/

button=gtk_button_new_with_label(“button1”); /*创建按钮*/

gtk_table_attach(GTK_TABLE(table),button,0,1,0,1,GTK_FILL,GTK_FILL,0,0);/*把按钮加入组合表*/

gtk_widget_show(button); /*显示按钮*/

button=gtk_button_new_with_label(“button1”);

gtk_table_attach(GTK_TABLE(table),button,1,2,1,2,GTK_FILL,GTK_FILL,0,0);

gtk_widget_show(button);

button=gtk_button_new_with_label(“button1”);

gtk_table_attach(GTK_TABLE(table),button,2,3,2,3,GTK_FILL,GTK_FILL,0,0);

gtk_widget_show(button);

gtk_container_add(GTK_CONTAINER(window),table); /*把组合表添加到窗体上*/

gtk_widget_show(table); /*显示组合表*/

gtk_widget_show(window); /*显示窗体*/

gtk_main(); /*等待用户操作*/

return FALSE;

}

$gcc –o table table.c `pkg-config –cflags –libs gtk+-2.0`

$./table

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/2.png)

这里没有给出参数的所用情况的程序，大家可以把相应的参数换一下，体会各个参数的作用。

三、固定容器构件

GtkFixed（固定容器构件）允许将构件放在窗口的固定位置，这个位置是相对与窗口的左上角的。构件的位置可以动态改变。

使用GtkFixed为构件定位，在大多数情况下都是不可取的。因为当用户调整窗口尺寸时，勾结不能适应窗口的尺寸变化。当然，你可以在窗口尺寸变化的时候采取行动，调整构件的位置和大小。

gtk_fixed_new函数用于创建新的固定容器构件。

6．

|名称:：|gtk_fixed_new|
|----|----|
|功能：|创建固定容器构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtiWidget *gtk_fixed_new(void);|
|参数：|无|
|返回值：|新的固定容器构件|





gtk_fixed_put函数将构件放在由x,y指定的位置。

7．

|名称:：|gtk_fixed_put|
|----|----|
|功能：|将构件放入固定容器构件的指定位置|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_fixed_put(GtkFixed *fixed,GtkWidget *widget,gint16 x,gint16 y);|
|参数：|fixed固定容器容器构件widget要放在固定容器构件中的构件x放置的横坐标y放置的纵坐标|
|返回值：|无|







gtk_fixed_move函数将指定构件移动到新位置。

8．

|名称:：|gtk_fixed_move|
|----|----|
|功能：|移动固定容器构件里的构件到指定位子|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_fixed_move(GtkFixed *fixed,GtkWidget *widget,gint16 x,gint16 y);|
|参数：|fixed固定容器构件widget要放在固定容器构件中的构件x放置的横坐标y放置的纵坐标|
|返回值：|无|







下面是固定构件的程序例子：

#include <gtk/gtk.h>

GtkWidget *window;/*定义窗体*/

GtkWidget *fixed;/*定义固定容器构件*/

GtkWidget *button;/*定义按钮*/

gint function(gpointer data);

gpointer data;

gint sign;

int x=0;

int y=0;

int main(int argc,char *argv[ ])

{

gtk_init(&argc,&argv);/*初始化*/

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);/*创建窗体*/

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

fixed=gtk_fixed_new();/*创建固定容器构件*/

button=gtk_button_new_with_label(“button”);/*创建按钮*/

gtk_fixed_put(GTK_FIXED(fixed),button,10,10);/*将按纽放在固定容器构件的指定位置*/

sign=g_timeout_add(600,function,data);/*创建定时器*/

gtk_container_add(GTK_CONTAINER(window),fixed);/*把固定容器构件放入窗体*/

gtk_widget_show(fixed);

gtk_widget_show(button);

gtk_widget_show(window);

gtk_main();

}

gint function(gpointer data)

{

gtk_fixed_move(GTK_FIXED(fixed),button,++x,++y);/*移动按纽*/

return TRUE;

}

程序运行后得到如下的窗体：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/3.png)

窗体每隔600毫秒便会扩大一次。


