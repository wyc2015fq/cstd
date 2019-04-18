# GTK+图形化应用程序开发学习笔记（十六）—文件选择构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 23:29:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2270


**GTK+图形化应用程序开发学习笔记（十六）—文件选择构件**

文件选择构件(GtkFileSelect)是一种种快速、简单的显示文件对话框的方法。它带有“确定”、“取消”、“帮助”按钮，可以极大地减少编程时间。

创建文件选择构件用函数gtk_file_selection_new.

1．

|名称:：|gtk_file_selection_new|
|----|----|
|功能：|创建文件选择构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_file_selection_new( gchar *title );|
|参数：|title文件选择构件标题|
|返回值：|新的文件选择构件|





要注意文件选择构件本身就是一个窗体，所以我们不需要也不能把它放入另外的一个窗体。通常情况下是用户点击某一按纽（如：打开文件）弹出该窗体，所以我们可以把创建文件构件的函数放在该按纽“clecked”信号的回调函数中。

2．

|名称:：|gtk_file_selection_set_filename|
|----|----|
|功能：|设置文件名|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_file_selection_set_filename( GtkFileSelection *filesel, gchar *filename );|
|参数：|filesel文件选择构件filename文件名|
|返回值：|无|







gtk_file_selection_set_filename函数设置文件名，例如，要在打开时指向指定目录，或者给定一个缺省文件名。

下面给出创建文件选择构件的程序例子：

#include <gtk/gtk.h>

GtkWidget *FileSelection;

void button_event(GtkWidget *widget, gpointer *data);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOWTOPLEVEL);

gtk_widget_set_size_request(window,100,100);/*调整窗口大小*/

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

button=gtk_button_new_with_label(“打开文件”);/*常见按纽*/

gtk_signal_connect(GTK_OBJECT(button),”clicked”,C_CALLBACK(button_event),NULL);

gtk_container_add(GTK_CONTAINER(window),button);

gtk_widget_show(button);

gtk_widget_show(window);

gtk_main();

}

void button_event(GtkWidget *widget,gpointer *data)

{

FileSelection=gtk_file_selection_new(“选择文件”);/*创建文件选择构件*/

gtk_file_selection_set_filename(GTK_FILE_SELECTION(FileSelection),”*.txt”);

gtk_widget_show(FileSelection);

}

程序运行后会得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070614214229.png)

当用户点击按纽时会出现如下界面：

![](http://blog.chinaunix.net/photo/22935_070612162157.png)

文件选择构件的结构如下：

typedef struct {

GtkWidget *dir_list;

GtkWidget *file_list;

GtkWidget *selection_entry;

GtkWidget *selection_text;

GtkWidget *main_vbox;

GtkWidget *ok_button;

GtkWidget *cancel_button;

GtkWidget *help_button;

GtkWidget *history_pulldown;

GtkWidget *history_menu;

GList*history_list;

GtkWidget *fileop_dialog;

GtkWidget *fileop_entry;

gchar*fileop_file;

gpointercmpl_state;


GtkWidget *fileop_c_dir;

GtkWidget *fileop_del_file;

GtkWidget *fileop_ren_file;


GtkWidget *button_area;

GtkWidget *action_area;

} GtkFileSelection;


在为文件选择构件的信号设置回调函数时，可能用到ok_button、cancel_button两个指针。如：打开一个文件我们需要对ok_button的”clicked”信号设置回调函数使它连接打开文件的函数。

我们也可以根据自己的需要在对该结构进行修改，添加或删除一些内部构件。


3．

|名称:：|gtk_file_selection_get_filename|
|----|----|
|功能：|获取用户输入或选中的文件名|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gchar *gtk_file_selection_get_filename( GtkFileSelection *filesel );|
|参数：|filesel文件选择构件|
|返回值：|文件名|





gtk_file_selection_get_filename函数可以获得用户输入或选中的文件，我们通常需要在ok_button的”clicked”信号设置的回调函数中调用它。

下面我们利用上面所学的知识，编写一个程序。当用户选择文件，并点击打开按纽时打印文件名。

#include <gtk/gtk.h>

GtkWidget *FileSelection;

void OpenFile(GtkWidget *widget,gpointer *data);

void button_event(GtkWidget *widget, gpointer *data);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOWTOPLEVEL);

gtk_widget_set_size_request(window,100,100);/*调整窗口大小*/

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

button=gtk_button_new_with_label(“打开文件”);/*常见按纽*/

gtk_signal_connect(GTK_OBJECT(button),”clicked”,C_CALLBACK(button_event),NULL);

gtk_container_add(GTK_CONTAINER(window),button);

gtk_widget_show(button);

gtk_widget_show(window);

gtk_main();

}

void button_event(GtkWidget *widget,gpointer *data)

{

FileSelection=gtk_file_selection_new(“选择文件”);/*创建文件选择构件*/

gtk_file_selection_set_filename(GTK_FILE_SELECTION(FileSelection),”*.txt”);

gtk_signal_connect(GTK_OBJECT(GTK_FILL_SELECTION(FileSelection)->ok_button),”clicked”,GTK_SIGNAL_FUNC(OpenFile),NULL);/*捕捉打开按纽的”clicked”信号*/

gtk_widget_show(FileSelection);

}

void OpenFile(GtkWidget *widget,gpointer *data)

{

g_print(“%s/n”,gtk_file_selection_get_filename(GTK_FILE_SELECTION(FileSelection));

/*输出选择的文件名*/

}



