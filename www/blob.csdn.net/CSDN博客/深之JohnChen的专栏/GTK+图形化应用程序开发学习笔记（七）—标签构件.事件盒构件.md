# GTK+图形化应用程序开发学习笔记（七）—标签构件.事件盒构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 13:01:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2281


**GTK+图形化应用程序开发学习笔记（七）—标签构件.事件盒构件**

**一、标签构件**

**　　标签构件（GtkLabel）是GTK中最常见的构件，它是静态的不可编辑的字段。在屏幕上，常常用标号说明其他字段。在按钮上设置标签用来说明按钮，或者放在其他字段的旁边对该字段提供说明。它不能自身引发信号。**

**用gtk_label_new创建标签构件：**

**1．**

|**名称:：**|**gtk_label_new**|
|----|----|
|**功能：**|**创建标签构件**|
|**头文件：**|**#include <gtk/gtk.h>**|
|**函数原形：**|**GtkWidget gtk_label_new(char *str);**|
|**参数：**|**str标签构件正文**|
|**返回值：**|**新的标签构件**|




**下面就是创建标号的程序例子：**

**/*label1.c*/**

**#include <gtk/gtk.h>**


**int main(int argc,char *argv[])**

**{**

**GtkWidget *window;**

**GtkWidget *label;**


**gtk_init_chech(&argc,&argv);**

**window=gtk_window_new(GTK_WINDOW_TOPLEVEL);**

**gtk_signal_connect(GTK_OBJECT(window),”delete_event”,G_CALLBACK(gtk_main_quit),NULL);**

**label=gtk_label_new(“This is a label”);/*建立标号*/**

**gtk_widget_show(label);**


**gtk_container_add(GTK_CONTAINER(window),label);**

**gtk_widget_show(window);**

**gtk_main();**

**return 0;**

**}**

**下图是运行后的界面：**




**![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/1633652344299062500.png)**

**创建标签构件后，要改变标签内的文本用gtk_label_set_text函数。**

**2．**

|**名称:：**|**gtk_label_set_text**|
|----|----|
|**功能：**|**改变标签构件的正文**|
|**头文件：**|**#include <gtk/gtk.h>**|
|**函数原形：**|**void gtk_label_set_text(GtkLabel *label,char *str);**|
|**参数：**|**label要修改的标签构件****str签构件的正文**|
|**返回值：**|**无**|





**第一个参数是前面创建的标签构件，第而个参数是标签的正文。新的标签正文会根据需要自动调整。在正文中放置换行符，可以创建多行标签。**

**gtk_label_get函数可以取得标签的当前正文。**

**3．**

|**名称:：**|**gtk_label_get**|
|----|----|
|**功能：**|**获得标签构件的正文**|
|**头文件：**|**#include <gtk/gtk.h>**|
|**函数原形：**|**void gtk_label_get(GtkLabel *label,char **str);**|
|**参数：**|**label要修改的标签构件****str签构件的正文**|
|**返回值：**|**无**|





**第一个参数是前面创建的标签构件，第而个参数是要返回的正文字符串。**



**下面我们结合上面的选择按钮编写一个如下图所示的程序：当用户点击选择按钮，上面的tabel显示相应的内容。**


**![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/2633652344299375000.png)![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/3633652344299687500.png)![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/4633652344300000000.png)**

**下面是代码：**

**/*tabel2.c*/**

**#include <gtk/gtk.h>**

**void button_event(GtkWidget *,gpointer *);**

**GtkWidget *label;**

**int main(int argc,char *argv[ ])**

**{**

**GtkWidget *window;**

**GtkWidget *button;**

**GtkWidget *box;**

**GSList *group=NULL;**

**gtk_init_chrck(&argc,&argv); /*初始化GTK+工具库*/**

**window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/**

**gtk_signal_connect(GTK_OBJECT(window),”delete_event”,G_CALLBACK(gtk_main_quit),NULL); /*登记窗体delete_event信号*/**

**box=gtk_vbox_new(FALSE,0);/*创建组合框*/**

**label=get_label_new(“Please check!”);/*创建标号*/**

**gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,15);/*把标号加入组合框*/**

**button=gtk_radio_button_new_with_label(group,”Radio Button1”);/*创建按钮*/**

**group=gtk_radio_button_group(GTK_RADIO_BUTTON(button));/*把按钮加到组中*/**

**gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);/*把按钮加入组合框*/**

**gtk_signal_connect(GTK_OBJECT(button),”pressed”,GTK_SIGNAL_FUNC(button_event),”You check button1”);**

**gtk_widget_show(button);/*显示按钮*/**

**button=gtk_radio_button_new_with_label(group,”Radio Button2”);**

**group=gtk_radio_button_group(GTK_RADIO_BUTTON(button));**

**gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);**

**gtk_signal_connect(GTK_OBJECT(button),”pressed”,GTK_SIGNAL_FUNC(button_event),”You check button2”);**

**gtk_widget_show(button);**

**gtk_container_add(GTK_CONTAINER(window),box);/*把组合框加入窗体*/**

**gtk_widget_show(label);/*显示标号*/**

**gtk_widget_show(box);/*显示组合框*/**

**gtk_widget_show(window);/*显示窗体*/**

**gtk_main();**

**return 0;**

**}**

**void button_event(GtkWidget *widget,gpointer *data)**

**{**

**gtk_label_set(GTK_LABEL(lable),(char *)data);**

**}**


4．

|名称:：|gtk_label_set_justify|
|----|----|
|功能：|调整标签构件正文的对齐方式|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_label_set_justify(GtkLabel *label,GtkJustification jtype);|
|参数：|label要修改的标签构件jtype对齐方式|
|返回值：|无|





jtype的值可以是：

GTK_JUSTIFY_LEFT左对齐

GTK_JUSTIFY_RIGHT右对齐

GTK_JUSTIFY_CENTER居中对齐

GTK_JUSTIFY_FILL充满



标签构件文本会自动换行。用gtk_label_set_line_wrap函数激活自动换行。

5．
|名称:：|gtk_label_set_line_wrap­|
|----|----|
|功能：|激活自动换行|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_label_set_line_wrap(GtkLabel *label,gboolean wrap);|
|参数：|label要修改的标签构件wrap是否自动换行|
|返回值：|无|





wrap参数可取TRUE或FALSE，对应于自动换行和不自动换行。


可以使用gtk_label_set_pattern函数对标签正文加下划线。

6．
|名称:：|gtk_label_set_pattern|
|----|----|
|功能：|对标签正文加下划线|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_label_set_pattern(GtkLabel *label,const gchar *pattern);|
|参数：|label要修改的标签构件pattern下划线外观|
|返回值：|无|





pattern参数指定下划线的外观。它由一串下划线和空格组成。下划线指示标签的相应字符应该加一个下划线。

下面我们来练习上面的三个函数。

#include <gtk/gtk.h>


int main(int argc,char *argv[])

{

GtkWidget *window;

GtkWidget *label;


gtk_init (&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_usize(window,100,100);

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,G_CALLBACK(gtk_main_quit),NULL);


label=gtk_label_new(“This is a label”);/*建立标号*/

gtk_widget_set_usize(label,10,10);/*设置标号尺寸*/

gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_CENTER);/*设置标号对齐方式为居中对齐*/

gtk_label_set_line_wrap(GTK_LABEL(label),TRUE);/*打开自动换行*/

gtk_label_set_pattern(GTK_LABEL(label),”_ __ _”);/*添加下划线*/

gtk_widget_show(label);


gtk_container_add(GTK_CONTAINER(window),label);

gtk_widget_show(window);

gtk_main();

return 0;

}

运行程序会得到如下结果： 

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/5633652344300312500.png)

二、事件盒构件

事件盒构件（GtkEventBox）在屏幕上什么也不画，也不能单独对事件做出响应。它的功能是为一些没有X窗口的构件（如label）提供X窗口。使其可以接收信号。它另外一个作用是剪裁这些构件。它的功能有点像Dreamweaver里的焦点。

使一个没有X窗体的构件响应信号分为3步。

一、创建事件盒构件。我们用gtk_event_box_new来创建事件盒构件。

1．
|名称:：|gtk_event_box_new|
|----|----|
|功能：|创建事件盒构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_event_box_new( void );|
|参数：|无|
|返回值：|新的事件盒构件|





二、将要响应事件的构件加入事件盒构件。

gtk_container_add( GTK_CONTAINER(event_box), child_widget );

三、为事件盒构件绑定一个事件响应。

gtk_signal_connect (GTK_OBJECT(event_box), "button_press_event", GTK_SIGNAL_FUNC (gtk_main_quit), NULL);

下面就是应用事件盒构件的例子。

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *event_box;

GtkWidget *label;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,GTK_SIGNAL_FUNC(gtk_main_quit),NULL);

gtk_container_set_border_width(GTK_CONTAINER(window),10);

event_box=gtk_event_box_new();/*创建事件盒构件*/

gtk_container_add(GTK_CONTAINER(window),event_box);

gtk_widget_show(event_box);

label_gtk_label_new(“Click here to quit”);

gtk_container_add(GTK_CONTAINER(event_box),label);/*将label构件加入事件盒构件*/

gtk_widget_show(label);

gtk_signal_connect(GTK_OBJECT(event_box),”button_press_event”,GTK_SIGNAL_FUNC(gtk_main_quit),NULL);/* 为事件盒构件绑定一个事件响应*/

gtk_widget_show(window);

gtk_main();

}

程序运行后得到如下窗体，当用户点击label程序退出。


![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/6633652344300625000.png)

