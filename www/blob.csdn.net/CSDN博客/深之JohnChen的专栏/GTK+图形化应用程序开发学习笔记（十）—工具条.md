# GTK+图形化应用程序开发学习笔记（十）—工具条 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 22:33:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1944


 GTK+图形化应用程序开发学习笔记（十）—工具条

工具条（GtkToolbar）为用户提供常用命令的简便菜单。工具条通常包含带有表示命令的图标的按钮，但也能包含其他构件。工具条在软件中应用很平凡，特别是编辑类的软件如office.

下面我就介绍如何建立一个工具条，大家看完本节就可以建立如下的工具条了。




![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/3633652364717812500.png)

1．

|名称:：|gtk_toolbar_new|
|----|----|
|功能：|创建工具条|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|Gtk_Widget* gtk_toolbar_new(void);|
|参数：|无|
|返回值：|新的工具条|






gtk_toolbar_new用来建立工具条，这个函数很简单，它没有任何参数。

用gtk_toolbar_new函数建立的工具条是还没有任何按钮的工具条，我们还要在上面填加一些带图标的按钮。我们用gtk_toolbar_append_item添加按钮。

注意gtk_toolbar_append_item会自动去生产带图标的按钮，并不需要程序员预先创建按钮。

2．

|名称:：|gtk_toolbar_append_item|
|----|----|
|功能：|添加带图标的按钮|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|Gtk_Widget* gtk_toolbar_append_item(GtkToolbar *toolbar, const char *text,const char *tooltip_text, const char *tooltip_private_text, GtkWidget *widget, GtkSignalFunc callback, gpointer userdata);|
|参数：|toolbar工具条text按钮正文tooptip_text工具提示正文tooptip_private_text用于[GtkTipsQuery](http://developer.gnome.org/doc/API/2.0/gtk/GtkTipsQuery.html)widget放在按钮上的图标callback回调函数userdata传递给回调函数的附加参数|
|返回值：|添加后的按钮|











看起来有点恐怖，其实并不复杂。下面我们分别介绍函数的参数。

toolbar是待加入按钮的工具条，也就是我们用gtk_toolbar_new建立的空白工具条了。

text是工具条按钮的正文，也就是上图的“电灯”、“笑脸”之类的。它通常显示在图标的下面。

tooptip_text是按钮的提示，当用户将鼠标移动到上面时显示提示。

tooptip_private_text参数我们暂时先不管它。

widget方在按钮上的图标了，它通常是一个图片构件。或是一个像元图。

像元图是使影像构件，建立像元图的一种方式是从图形编辑软件（如GIMP）中建立它。建立像元图很麻烦，要写一大堆的代码。并且初学者建立的都会很难看。不过感兴趣的朋友可以去看一看。我建议大家用图片构件的方式。我们稍后介绍怎样建立一个图片构件。

callback就是按钮的回调函数了，当用户点击按钮时gtk_toolbar_append_item调用callback函数。

userdata 是传递给回调函数的附加参数。你可以把一些有用的信息通过这个参数传递给回调函数。

哈哈，好了现在我们就可以建立自己的工具条了。

对了，忘记告诉大家怎样建立一个图片构件了。首先要准备几张图标了，如果手头上没有可以去/usr/share文件夹里面找，然后把它放在和程序同一文件夹下面。

3、

|名称:：|gtk_image_new_from_file|
|----|----|
|功能：|创建图片构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|Gtk_Widget* gtk_image_new_from_file(char *file);|
|参数：|file图片名|
|返回值：|新的图片构件|





file就是你想创建构件的图片文件了。

下面我们开始编程：

#inclue <gtk/gtk.h>

void ButtonEvent(GtkWidget *widget,gpointer *data);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *box;/*定义组装盒*/

GtkWidget *toolbar;/*定义工具条*/

GtkWidget *image;/*定义图片构件*/

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_usize(window,200,100);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_vbox_new(FALSE,0);/*创建组装盒*/

toolbar=gtk_toolbar_new();/*创建工具条*/

gtk_box_pack_start(GTK_BOX(box),toolbar,FALSE,FALSE,TRUE,5);/*把工具条加入组装盒*/

image=gtk_image_new_from_file(“1”);/*创建图片构件，大家可以把参数改为自己的图片文件名*/

gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),“电灯”, “电灯”,NULL,image,(GtkSignalFunc)ButtonEvent, “电灯”);/*创建工具条里的按钮*/

image=gtk_image_new_from_file(“2”);

gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),“笑脸”, “笑脸”,NULL,image,(GtkSignalFunc)ButtonEvent, “笑脸”);



image=gtk_image_new_from_file(“3”);

gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),“音乐”, “音乐”,NULL,image,(GtkSignalFunc)ButtonEvent, “音乐”);

image=gtk_image_new_from_file(“4”);

gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),“相册”, “相册”,NULL,image,(GtkSignalFunc)ButtonEvent, “相册”);

image=gtk_image_new_from_file(“5”);

gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),“帮助”, “帮助”,NULL,image,(GtkSignalFunc)ButtonEvent, “帮助”);

gtk_container_add(GTK_CONTAINER(window),box);

gtk_widget_show(toolbar);

gtk_widget_show(box);

gtk_widget_show(window);

gtk_main();

}

void ButtonEvent(GtkWidget *widget,gpointer *data) /*回调函数*/

{

g_print(“%s/n”,data);

}


