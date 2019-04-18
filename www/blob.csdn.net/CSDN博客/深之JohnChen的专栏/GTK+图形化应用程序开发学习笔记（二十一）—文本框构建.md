# GTK+图形化应用程序开发学习笔记（二十一）—文本框构建 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 23:58:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：5027


**GTK+图形化应用程序开发学习笔记（二十一）—文本框构建**

文本框也叫多行文本输入框，是gtk+较常用的构件，也是较复杂的一个构件。它被分为以下几部分：

[GtkTextView](http://developer.gnome.org/doc/API/2.0/gtk/GtkTextView.html)：代表了窗口中可见的文本框，用来显示GtkTextBuffer.

[GtkTextBuffer](http://developer.gnome.org/doc/API/2.0/gtk/GtkTextBuffer.html)：文本框正文的缓冲区，文本框文字的插入、删除都是对这一类变量进行操作。

[GtkTextIter](http://developer.gnome.org/doc/API/2.0/gtk/gtk-GtkTextIter.html)：保存文字在buffer中位置的结构。

[GtkTextMark](http://developer.gnome.org/doc/API/2.0/gtk/GtkTextMark.html)：A position in the buffer preserved across buffer modifications 

[GtkTextTag](http://developer.gnome.org/doc/API/2.0/gtk/GtkTextTag.html)：用来给指定的文字添加一些标记，改变指定区域的文字的显示效果。如：字体的颜色、大小的改变。

[GtkTextTagTable](http://developer.gnome.org/doc/API/2.0/gtk/GtkTextTagTable.html)：是GtkTextTag标记的集合表。

一、创建文本框构件

GtkTextView,它代表了窗口中可见的文本框，如果只是简单使用基本用不到除了创建以外的函数。有两种方法可以创建文本框。一种是用默认的缓冲区创建文本框，用函数gtk_text_view_new。另一种是指定缓冲区来创建文本框，用函数gtk_text_view_new_with_buffer。

1．

|名称:：|gtk_text_view_new|
|----|----|
|功能：|创建文本框构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_text_view_new(void);|
|参数：|无|
|返回值：|新的文本框构件|





gtk_text_view_new创建一个新的缓冲区，如果你没有在创建之前调用函数gtk_text_view_set_buffer来设置这个缓冲区，那么系统将创建一个空的默认的缓冲区，我们可以用gtk_text_view_get_buffer来得到这个缓冲区。如果你用你自己的缓冲区来创建文本框可以用gtk_text_view_new_with_buffer.

2．

|名称:：|gtk_text_view_new_with_buffer|
|----|----|
|功能：|创建树构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_text_view_new_with_buffer(GtkTextBuffer *buffer);|
|参数：|buffer缓冲区|
|返回值：|新的文本框构件|





gtk_text_view_new_with_buffer用指定的缓冲区创建一个新的文本框，我们用buffer来指定这个缓冲区，一个缓冲区能够被许多的构件共享。buffer可以为NULL，功能和gtk_text_view_new函数一样。

3．

|名称:：|gtk_text_view_set_buffer|
|----|----|
|功能：|设置文本框的缓冲区|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_text_view_set_buffer(GtkTextView *text_view,GtkTextBuffer *buffer);|
|参数：|text_view文本框buffer缓冲区|
|返回值：|无|







4．

|名称:：|gtk_text_view_get_buffer|
|----|----|
|功能：|获得文本框的缓冲区|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkTextBuffer* gtk_text_view_get_buffer (GtkTextView *text_view);|
|参数：|text_view文本框|
|返回值：|文本框缓冲区|





我们可以用gtk_text_view_new创建默认缓冲区的文本框，用gtk_text_view_get_buffer来得到这个缓冲区。下面就是程序例子。


#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *text_view;

GtkTextBuffer *buffer;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_size_request(window,200,150);

g_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

text_view=gtk_text_view_new();/*创建文本框构件*/

buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));/*获得文本框缓冲区*/

gtk_container_add(GTK_CONTAINER(window),text_view);

gtk_widget_show(view);

gtk_widget_show(window);

gtk_main();

}

程序运行后得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070711092005.png)

二、创建文本框的缓冲区

5．

|名称:：|gtk_text_buffer_new|
|----|----|
|功能：|创建文本框的缓冲区|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|[GtkTextBuffer](http://developer.gnome.org/doc/API/2.0/gtk/GtkTextBuffer.html)* gtk_text_buffer_new([GtkTextTagTable](http://developer.gnome.org/doc/API/2.0/gtk/GtkTextTagTable.html) *table);|
|参数：|table文本构件的标签盒|
|返回值：|文本框缓冲区|




gtk_text_buffer_new用来创建文本构件的缓冲区。它需要指定文本构件的标签盒。我们也可以让它为空，让系统创建默认的文件构件标签盒。然后用gtk_text_buffer_get_tag_table函数来获得标签盒。

6．

|名称:：|gtk_text_buffer_get_tag_table|
|----|----|
|功能：|获得文本框的标签盒|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkTextTagTable* gtk_text_buffer_get_tag_table(GtkTextBuffer *buffer);|
|参数：|buffer文本构件的缓冲区|
|返回值：|文本框构件的标签盒|







下面是用指定缓冲区创建文本框的例子。

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *text_view;

GtkTextBuffer *buffer;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_size_request(window,200,150);

g_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

buffer=gtk_text_buffer_new(NULL);/*创建文本框的缓冲区*/

text_view=gtk_text_view_new_with_buffer(buffer);/*创建文本框构件*/

gtk_container_add(GTK_CONTAINER(window),text_view);

gtk_widget_show(view);

gtk_widget_show(window);

gtk_main();

}

三、文本框文字的插入和删除

对文本框文字的插入和删除之前都需要得到当前buffer中开始位置、结束位置的ITER，

我们可以用gtk_text_buffer_get_bounds来实现这一点。

7．

|名称:：|gtk_text_buffer_get_bounds|
|----|----|
|功能：|获得文本框的标签盒|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_text_buffer_get_bounds(GtkTextBuffer *buffer,GtkTextIter *start, GtkTextIter *end);|
|参数：|buffer文本构件的缓冲区start文本框文字开始位置的iterend文本框文字结束位置的iter|
|返回值：|无|






缓冲区开始和结束位置的iter分别通过star和end参数得到。下面我们就可以对文本框构件的正文进行操作了。

8．

|名称:：|gtk_text_buffer_insert|
|----|----|
|功能：|向缓冲区插入文字|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_text_buffer_insert(GtkTextBuffer *buffer,GtkTextIter *iter,const gchar *text,gint len);|
|参数：|buffer文本构件的缓冲区iter插入的位置text插入的文本len插入的文本长度|
|返回值：|无|






start、end分别为文本框文字开始位置和结束位置的iter。len为要插入文本的长度，它可以为-1，表示插入text的所有内容。

下面是插入文字的程序例子：

#include <gtk/gtk.h>

GtkWidget *text_view;

GtkTextBuffer *buffer;

GtkTextIter *Iter;

void button_event(GtkWidget *widget,gpointer *data);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

GtkWidget *box;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_size_request(window,200,150);

g_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_vbox_new(FALSE,0);

gtk_widget_show(box);

text_view=gtk_text_view_new();/*创建文本框构件*/

gtk_widget_set_request(text_view,200,120);

gtk_container_add(GTK_CONTAINER(window),box);

gtk_box_pack_start(GTK_BOX(box),text_view,FALSE,FALSE,0);

buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

gtk_widget_show(text_view);

button=gtk_button_new_with_label(“确定”);

gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);

gtk_signal_connect(GTK_OBJECT(button),”clicked”,GTK_SIGNAL_FUNC(button_event),NULL);

gtk_widget_show(window);

gtk_main();

}

void button_event(GtkWidget *widget,gpointer *data)

{

const gchar *text=”How are you!”;

GtkTextIter start,end;

gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer),&start,&end);/*获得缓冲区开始和结束位置的Iter*/

gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer),&start,text,strlen(text));/*插入文本到缓冲区*/

}

程序运行后得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070711092012.png)

当用户点击确定按扭后得到：

![](http://blog.chinaunix.net/photo/22935_070711092021.png)

9．

|名称:：|gtk_text_buffer_delete|
|----|----|
|功能：|缓冲区内容删除|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_text_buffer_delete(GtkTextBuffer *buffer,GtkTextIter *start,GtkTextIter *end);|
|参数：|buffer文本构件的缓冲区start文本框文字开始位置的iterend文本框文字结束位置的iter|
|返回值：|无|







gtk_text_buffer_delete函数可将缓冲区内容删除。start、end分别为文本框文字开始位置和结束位置的iter.

四、文本框缓冲区文本的获得和设置

我们可以用函数gtk_text_buffer_get_text来获得文本框缓冲区的文本，用gtk_text_buffer_set_text来设置它。

10．

|名称:：|gtk_text_buffer_get_text|
|----|----|
|功能：|文本框缓冲区文本的获得|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gchar* gtk_text_buffer_get_text(GtkTextBuffer *buffer,const GtkTextIter *start,const GtkTextIter *end,gboolean include_hidden_chars);|
|参数：|buffer文本构件的缓冲区start文本框文字开始位置的iterend文本框文字结束位置的iterinclude_hidden_chars|
|返回值：|文本框缓冲区文本|






gtk_text_buffer_get_text返回文本框缓冲区文本，include_hidden_chars设置是否包含看不见的文本。

下面是文本框缓冲区文本的获得的程序例子：

#include <gtk/gtk.h>

GtkWidget *text_view;

GtkTextBuffer *buffer;

GtkTextIter *Iter;

void button_event(GtkWidget *widget,gpointer *data);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

GtkWidget *box;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_size_request(window,200,150);

g_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_vbox_new(FALSE,0);

gtk_widget_show(box);

text_view=gtk_text_view_new();/*创建文本框构件*/

gtk_widget_set_request(text_view,200,120);

gtk_container_add(GTK_CONTAINER(window),box);

gtk_box_pack_start(GTK_BOX(box),text_view,FALSE,FALSE,0);

buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

gtk_widget_show(text_view);

button=gtk_button_new_with_label(“确定”);

gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);

gtk_signal_connect(GTK_OBJECT(button),”clicked”,GTK_SIGNAL_FUNC(button_event),NULL);

gtk_widget_show(window);

gtk_main();

}

void button_event(GtkWidget *widget,gpointer *data)

{

gchar *text;

GtkTextIter start,end;

gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer),&start,&end);/*获得缓冲区开始和结束位置的Iter*/

const GtkTextIter s=start,e=end;

text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer),&s,&e,FALSE);/*获得文本框缓冲区文本*/

g_print(“%s/n”,text);

}

当用户点击确定按钮，文本框的内容将会打印到屏幕上。

11．

|名称:：|gtk_text_buffer_set_text|
|----|----|
|功能：|文本框缓冲区文本的设置|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_text_buffer_set_text(GtkTextBuffer *buffer,const gchar *text, gint len);|
|参数：|buffer文本构件的缓冲区start文本框文字开始位置的iterend文本框文字结束位置的itertext要设置的文本|
|返回值：|无|






gtk_text_buffer_set_text会删除原有缓冲区内容，然后用text的内容代替。


