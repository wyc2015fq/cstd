# GTK+图形化应用程序开发学习笔记（十四）—组合框构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 23:25:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2066


**GTK+图形化应用程序开发学习笔记（十四）—组合框构件**

组合框（GtkCombo）是极为常见的构件，实际上它仅仅是其他构件的集合。从用户的观点来说，这个构件是由一个文本输入构件和一个下拉菜单组成的，用户可以从一个预先定义的列表里面选择一个选项，同时，用户也可以直接在文本框里面输入文本。

组合框构件有两个主要部分：一个输入框和一个列表。下面是组合框的结构

struct _GtkCombo

GtkHBox hbox;

GtkWidget *entry;

GtkWidget *button;

GtkWidget *popup;

GtkWidget *popwin;

GtkWidget *list;

};

用函数gtk_combo_new来创建一个组合框。

1．

|名称:：|gtk_combo_new|
|----|----|
|功能：|创建组合框|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_combo_new( void );|
|参数：|无|
|返回值：|新的组合框构件|





我们可以直接操纵组合框构件内部的文本输入构件，如：

gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),”请选择”);

下面是建立组合框的例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidgtk *window;

GtkWidgtk *combo;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

combo=gtk_combo_new( ); /*创建组合框*/

gtk_container_add(GTK_CONTAINER(window),combo);

gtk_widget_show(combo);

gtk_widget_show(window);

gtk_main( );

}

程序运行后会显示如下界面：

![](http://blog.chinaunix.net/photo/22935_070610211032.png)

![](http://blog.chinaunix.net/photo/22935_070610211047.png)

2．

|名称:：|g_list_append|
|----|----|
|功能：|追加字符串到链表|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GList *g_list_append( GList *glist, gpointer data );|
|参数：|glist链表gpointer字符串|
|返回值：|追加后的链表|













下面是建立一个链表的代码。

GList *glist=NULL;

glist = g_list_append(glist, "String 1");

glist = g_list_append(glist, "String 2");

glist = g_list_append(glist, "String 3");

glist = g_list_append(glist, "String 4");

建立好链表就可以把它加入组合框的下拉菜单了，我们用gtk_combo_set_popdown_strings完成这项工作。



3．

|名称:：|gtk_combo_set_popdown_strings|
|----|----|
|功能：|将链表加入组合框|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_combo_set_popdown_strings( GtkCombo *combo, GList *strings );|
|参数：|combo组合框strings链表|
|返回值：|无|







下面的我们建立一个带下拉菜单的组合框：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidgtk *window;

GtkWidgtk *combo;

GList *glist=NULL;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

combo=gtk_combo_new( ); /*创建组合框*/

glist=g_list_append(glist,”请选择”); /*添加字符串到链表

glist=g_list_append(glist,”数学”);

glist=g_list_append(glist,”语文”);

glist=g_list_append(glist,”外语”);

gtk_combo_set_popdown_strings(GTK_COMBO(combo),glist);

gtk_container_add(GTK_CONTAINER(window),combo);

gtk_widget_show(combo);

gtk_widget_show(window);

gtk_main( );

}

程序运行后会得到如下界面。

![](http://blog.chinaunix.net/photo/22935_070610211104.png)

![](http://blog.chinaunix.net/photo/22935_070610211114.png)

好了到这里我就把怎样建立组合框介绍完了，下面我们来介绍怎样取得组合框的文本。

组合框的文本可用gtk_entry_get_text来取得，具体方法如下：

char *string;

string=gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo)->entry));

