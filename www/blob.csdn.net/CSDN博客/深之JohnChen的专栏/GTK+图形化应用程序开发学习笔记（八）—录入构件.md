# GTK+图形化应用程序开发学习笔记（八）—录入构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 22:20:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4093


**GTK+图形化应用程序开发学习笔记（八）—录入构件**

一、录入构件

录入构件(GtkEntry)是单行录入字段，用它来录入和显示正文信息。录入构件由可编辑构件派生而来，它是更复杂的正文构件的简化形式。录入构件比按钮构件或标号构件复杂。与按钮构件或标号构件相比，它具有更多的功能。

可以用gtk_entry_new函数或者gtk_entry_new_with_max_length函数建立录入构件.其中函数gtk_entry_new_with_max_length能够设置录入到构件中的最长的字符数。

1．

|名称:：|gtk_entry_new gtk_entry_new_with_max_length|
|----|----|
|功能：|创建录入构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_entry_new(void); GtkWidget *gtk_entry_new_with_max_length(gint max);|
|参数：|max够设置录入到构件中的最长的字符数|
|返回值：|新的录入构件|







2．

|名称:：|gtk_entry_get_text|
|----|----|
|功能：|获得录入构件正文|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|const gchar *gtk_entry_get_text(GtkEntry *entry);|
|参数：|entry录入构件名|
|返回值：|指向录入构件正文的指针|






构件中的正文可以用gtk_entry_get_text函数提取。它的返回值是指向字符串数据的指针，但是不能通过该指针修改数据。

下面是录入构件的例子：用户输入姓名，按”Enter”按钮后屏幕上打印用户输入。



![](http://blog.chinaunix.net/photo/22935_070523222637.png)


/*entry1.c*/

#include <gtk/gtk.h>

void button_event(GtkWidget *,gpointer *);

GtkWidget *name;

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

GtkWidget *box;

GtkWidget *label;

gtk_init_chrck(&argc,&argv); /*初始化GTK+工具库*/

window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,G_CALLBACK(gtk_main_quit),NULL); /*登记窗体delete_event信号*/

box=gtk_vbox_new(FALSE,0);/*创建组合框*/

label=get_label_new(“name:”);/*创建标号*/

gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,5);/*把标号加入组合框*/

gtk_widget_show(label);/*显示标号*/

name=gtk_entry_new();/*创建录入构件*/

gtk_box_pack_start(GTK_BOX(box),labe,FALSE,FALSE,5);/*把录入构件加入组合框*/

gtk_widget_show(name);/*显示录入构件*/

button=gtk_button_new_with_label(“enter”);/*创建按钮*/

gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);/*把按钮加入组合框*/

gtk_signal_connect(GTK_OBJECT(button),”pressed”,GTK_SIGNAL_FUNC(button_event),”enter”);

gtk_widget_show(button);/*显示按钮*/

gtk_container_add(GTK_CONTAINER(window),box);/*把组合框加入窗体*/

gtk_widget_show(box);/*显示组合框*/

gtk_widget_show(window);/*显示窗体*/

gtk_main();

return 0;

}

void button_event(GtkWidget *widget,gpointer *data)

{

const gchar *uname;

uname=(gchar *)malloc(sizeof(gchar));

uname=gtk_entry_get_text(GTK_ENTRY(name));/*获得录入构件正文*/

g_print(“Name:%s/n”,uname);

}

$gcc –o entry1 entry1.c `pkg-config –cflags –libs gtk+-2.0

$./entry1

当用户在录入构件里输入root,然后点击按钮后屏幕上显示Name：root



构件中的正文可以用下列3种方式来修改：

1、使用gtk_entry_prepend_text函数在字段的开始插入正文。

2、使用gtk_entry_append_text函数在字段的最后插入正文。

3、使用gtk_entry_set_text函数设置字段的正文，但它清除字段的原有值。



3．

|名称:：|gtk_entry_prepend_text|
|----|----|
|功能：|修改录入构件正文|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_entry_prepend_text(GtkEntry *entry,const gchar *text);void gtk_entry_append_text(GtkEntry *entry,const gchar *text);void gtk_entry_set_text(GtkEntry *entry,const gchar *text);|
|参数：|entry录入构件名text指向录入构件正文的指针|
|返回值：|无|







我们程序entry1中加入下面的代码，就可以得到如图所示的程序。

gtk_entry_prepend_text(GTK_ENTRY(name),”Please input your name!”);



![](http://blog.chinaunix.net/photo/22935_070523222644.png)



4．

|名称:：|gtk_entry_set_visibility|
|----|----|
|功能：|是否显示录入构件正文|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_entry_set_visibility(GtkEntry *entry,gboolean visible);|
|参数：|entry录入构件visible是否显示正文|
|返回值：|无|






函数gtk_entry_set_visibility确定用户是否可以看到输入字段的正文。在多数情况下，用户应该能够看到正文。但如口令字段，就不适合在用户录入时加以显示。

我们对entry1程序稍加改动。添加了用户口令的录入。下面是程序：

/*entry3.c*/

#include <gtk/gtk.h>

void button_event(GtkWidget *,gpointer *);

GtkWidget *name;

GtkWidget *passwd;

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

GtkWidget *box;

GtkWidget *label;

gtk_init(&argc,&argv); /*初始化GTK+工具库*/

window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,G_CALLBACK(gtk_main_quit),NULL); /*登记窗体delete_event信号*/

box=gtk_vbox_new(FALSE,0);/*创建组合框*/

label=get_label_new(“name:”);/*创建标号*/

gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,5);/*把标号加入组合框*/

gtk_widget_show(label);/*显示标号*/

name=gtk_entry_new();/*创建录入构件*/

gtk_box_pack_start(GTK_BOX(box),name,FALSE,FALSE,5);/*把录入构件加入组合框*/

gtk_widget_show(name);/*显示录入构件*/

label=get_label_new(“passwd:”);/*创建标号*/

gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,5);/*把标号加入组合框*/

gtk_widget_show(label);/*显示标号*/

passwd=gtk_entry_new();/*创建录入构件*/

gtk_entry_set_visibility(GTK_ENTRY(passwd),FALSE);/*设置录入构件正文为不可视*/

gtk_box_pack_start(GTK_BOX(box),passwd,FALSE,FALSE,5);/*把录入构件加入组合框*/

gtk_widget_show(passwd);/*显示录入构件*/

button=gtk_button_new_with_label(“enter”);/*创建按钮*/

gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);/*把按钮加入组合框*/

gtk_signal_connect(GTK_OBJECT(button),”pressed”,GTK_SIGNAL_FUNC(button_event),”enter”);

gtk_widget_show(button);/*显示按钮*/

gtk_container_add(GTK_CONTAINER(window),box);/*把组合框加入窗体*/

gtk_widget_show(box);/*显示组合框*/

gtk_widget_show(window);/*显示窗体*/

gtk_main();

return 0;

}

void button_event(GtkWidget *widget,gpointer *data)

{

const gchar *uname;

const gchar *upasswd;

uname=(gchar *)malloc(sizeof(gchar));

upasswd=(gchar *)malloc(sizeof(gchar));

uname=gtk_entry_get_text(GTK_ENTRY(name));/*获得用户名*/

upasswd=gtk_entry_get_text(GTK_ENTRY(passwd));/*获得密码*/

g_print(“Name:%s/n”,uname);

g_print(“Passwe:%s/n”,upasswd);

}

$gcc –o entry3 entry3.c `pkg-config –cflags –libs gtk+-2.0

$./entry3

编译程序会得到如下界面：



![](http://blog.chinaunix.net/photo/22935_070523222733.png)



录入构件和我们下面介绍的正文构件都是从GTKEditable派生而来的。因而它们都支持一些快捷键，包括标准的剪切、复制和粘贴的快捷键。下面是录入构件支持的一些快捷键。

移动快捷键：

Ctrl + A 全选

Ctrl + X 剪切到剪切板

Ctrl + C 复制到剪切板

Ctrl + V 从剪切板粘贴

