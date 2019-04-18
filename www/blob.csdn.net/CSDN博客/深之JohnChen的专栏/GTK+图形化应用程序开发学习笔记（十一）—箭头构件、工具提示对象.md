# GTK+图形化应用程序开发学习笔记（十一）—箭头构件、工具提示对象 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 22:37:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1945


**GTK+图形化应用程序开发学习笔记（十一）—箭头构件、工具提示对象**

一、箭头构件

箭头构件(GtkArrow)和标签构件一样，不能引发信号。在许多应用程序中，常用于创建带箭头的按钮。箭头构件有几种不同方向，也有几种不同的风格。

用gtk_arrow_new函数来创建一个箭头构件。

1．
|名称:：|gtk_arrow_new|
|----|----|
|功能：|创建箭头构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_arrow_new(GtkArrowType arrow_type,GtkShadowType shadow_type);|
|参数：|arrow_type箭头指向哪个方向shadow_type箭头的投影类型|
|返回值：|新的箭头构件|






arrow_type参数指示箭头指向哪个方向，可取下列值：

GTK_ARROW_UP向上

GTK_ARROW_DOWN向下

GTK_ARROW_LEFT向左

GTK_ARROW_RIGHT向右

shadow_type参数指明箭头的投影类型，可取下列值：

GTK_SHADOW_IN

GTK_SHADOW_OUT(却省值)

GTK_SHADOW_ETCHED_IN

GTK_SHADOW_ETCHED_OUT

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

GtkWidget *arrow;

GtkWidget *box;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_hbox_new(FALSE,0);

gtk_container_add(GTK_CONTAINER(window),box);

button=gtk_button_new();/*创建不带标签的按钮*/

arrow=gtk_arrow_add(GTK_ARROW_UP,GTK_SHADOW_OUT);/*创建按纽构件*/

gtk_container_add(GTK_CONTAINER(button),arrow);/*将箭头构件加入按纽*/

gtk_box_pack_start(GTK_BOX(box),button,FALSE,TRUE,0);/*将按纽加入组装盒*/

gtk_widget_show(arrow);

gtk_winget_show(button);

button=gtk_button_new();

arrow=gtk_arrow_add(GTK_ARROW_DOWN,GTK_SHADOW_OUT);

gtk_container_add(GTK_CONTAINER(button),arrow);

gtk_box_pack_start(GTK_BOX(box),button,FALSE,TRUE,0);

gtk_widget_show(arrow);

gtk_winget_show(button);

gtk_widget_show(box);

gtk_widget_show(window);

gtk_main();

}

程序运行后得到如图界面：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/1633652368669687500.png)

2．
|名称:：|gtk_arrow_set|
|----|----|
|功能：|修改箭头构件的属性|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_arrow_set(GtkArrow *arrow, GtkArrowType arrow_type,GtkShadowType shadow_type);|
|参数：|arrow_type箭头指向哪个方向shadow_type箭头的投影类型|
|返回值：|无|














二、工具提示对象

工具提示对象（GtkTooltips）就是当鼠标指针移到按纽或其他构件上并停留几秒时，弹出的文本串。工具提示对象很容易使用，同样工具提示构件也不能触发信号。

可以使用gtk_tooltips_new函数创建工具提示对象。

3．
|名称:：|gtk_tooltips_new|
|----|----|
|功能：|创建工具提示对象|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkTooltips *gtk_tooltips_new(void);|
|参数：|无|
|返回值：|新的工具提示对象|






创建工具提示对象后，要把它与某个构件联系起来，使它发挥提示的作用。gtk_tooltips_set_tip函数可以达到这个目的。

4．
|名称:：|gtk_tooltips_set_tip|
|----|----|
|功能：|创建工具提示对象|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void *gtk_tooltips_set_tip(GtkTooltips *tooltip, GtkWidget *widget, const gchar *tip_text,const gchar *tip_private);|
|参数：|tooltip 工具提示对象widget希望弹出工具提示的构件tip_text要弹出的文本正文tip_private作为标识符的文本串|
|返回值：|无|








tooltip是已经创建的工具提示对象，widget是希望弹出工具提示的构件，tip_text是要弹出的文本正文。tip_private是作为标识符的文本串，当用GtkTipsQuery实现上下文敏感的帮助时要引用该标识符。目前，你可以把它设置为NULL。

下面是程序例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

GtkWidget *tooltip;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

button=gtk_button_new_with_label(“button”);

gtk_container_add(GTK_CONTAINER(window),button);

tooltip=gtk_tooltips_new();

gtk_tooltips_set_tip(tooltip,button,”hello”,NULL);

gtk_widget_show(button);

gtk_widget_show(window);

gtk_main();

}



