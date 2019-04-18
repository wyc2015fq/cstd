# GTK+图形化应用程序开发学习笔记（二十）—分栏列表构建 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 23:49:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3529


**GTK+图形化应用程序开发学习笔记（二十）—分栏列表构建**

分栏列表构件（GtkCList）是较常用的构件，它是多列列表构件，可以非常方便的处理数千列的信息。每一列都可以有一个标题，而且可以是活动的。你还可以将函数绑定到列选择上。

一、创建分栏列表构件

创建GtkCList构件的方法和创建其他构件的方法是类似的。

1．

|名称:：|gtk_clist_new|
|----|----|
|功能：|创建分栏列表构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_clist_new(gint columus);|
|参数：|columus分栏列表的列数|
|返回值：|新的分栏列表构件|





columus就是分栏列表的列数。注意分栏列表没有自己的滚动条，如果要提供滚动条功能，应该将分栏列表构件放在一个滚动窗口构件中。

现在创建完的分栏列表构件，外观和文本框一样，我们需要设置分栏列表构件的列标题。

2．

|名称:：|gtk_clist_set_column_title|
|----|----|
|功能：|设置分栏列表构件的列标题|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_clist_set_column_title(GtkCList *clist,gint column,gchar *title);|
|参数：|clist分栏列表构件column标题的编号title列标题|
|返回值：|无|






clist是就要设置的分栏列表构件，column就标题的编号，也是标题在分栏按钮中的位置0为第一个位置。

在设置完分栏列表构件的列标题后还要调用gtk_clist_column_titles_show函数把他显示出来。

3．

|名称:：|gtk_clist_column_titles_show|
|----|----|
|功能：|显示分栏列表构件的列标题|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_clist_column_titles_show(GtkCList *clist);|
|参数：|clist分栏列表构件|
|返回值：|无|





好了下面我们就可以建立没有内容的分栏列表构件了。

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *clist;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,GTK_SIGNAL_FUNC(gtk_main_quit),NULL);

gtk_widget_set_size_request(window,200,150);

gtk_container_set_border_width(GTK_CONTAINER(window),10);

clist=gtk_clist_new(3);

gtk_clist_set_column_title(GTK_CLIST(clist),0,”姓名”);

gtk_clist_set_column_title(GTK_CLIST(clist),1,”姓别”);

gtk_clist_set_column_title(GTK_CLIST(clist),2,”年龄”);

gtk_clist_column_titles_show(GTK_CLIST(clist));

gtk_container_add(GTK_CONTAINER(window),clist);

gtk_widget_show(clist);

gtk_widget_show(window);

gtk_main();

}

程序运行后得到如下界面：



![](http://blog.chinaunix.net/photo/22935_070627205402.png)

还有一些函数可以对分栏列表的列标题进行操作，下面我们分别进行介绍。

4．

|名称:：|gtk_clist_column_titles_hide|
|----|----|
|功能：|隐藏分栏列表构件的列标题|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_clist_column_titles_show(GtkCList *clist);|
|参数：|clist分栏列表构件|
|返回值：|无|





创建分栏列表构件时自动创建响应的标题按钮。标题一般处于分栏窗口的顶部，它可以是能对鼠标点击的按钮，也可以仅仅是不会作任何响应的标签。下面的四个函数可以设置这些标签的状态。

5．

|名称:：|gtk_clist_column_title_activegtk_clist_column_title_passivegtk_clist_column_titles_activegtk_clist_column_titles_passive|
|----|----|
|功能：|激活/停用分栏列表构件的列标题按钮|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_clist_column_titles_active(GtkCList *clist,gint column);|
|参数：|clist分栏列表构件column列标题编号|
|返回值：|无|







活动标题就是可以对用户动作响应的按钮标题，被动标题仅仅是一个标签。前两个函数激活或停用指定列的标题按钮，后两个激或或禁用整个分栏列表构件的按钮标题。

有时候不是所有的标题都是文本。GtkCList构件为我们提供的标题按钮实际上能够和所有的构件结合起来使用，例如，它可以和pixmap构件结合起来，在上面显示一副图片。使用下面得用函数可以为标题按钮设置构件。

6．

|名称:：|gtk_clist_set_column_widget|
|----|----|
|功能：|为标题按钮设置构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_clist_set_column_twidget(GtkCList *clist,gint column,GtkWidget *widget);|
|参数：|clist分栏列表构件column列标题编号widget要设置的构件|
|返回值：|无|






下面我们来看看上面几个函数的作用：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *clist;

GtkWidget *image;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,GTK_SIGNAL_FUNC(gtk_main_quit),NULL);

gtk_widget_set_size_request(window,200,150);

gtk_container_set_border_width(GTK_CONTAINER(window),10);

clist=gtk_clist_new(3);

gtk_clist_set_column_title(GTK_CLIST(clist),0,”姓名”);

gtk_clist_set_column_title(GTK_CLIST(clist),1,”姓别”);

gtk_clist_set_column_title(GTK_CLIST(clist),2,”年龄”);

gtk_clist_column_title_passive(GTK_CLIST(clist),1);/*设置编号为1的列标题为不活动的列标题*/

image=gtk_image_new_from_file(“1”); /*创建图片构件，大家可以把参数改为自己的图片文件名*/

gtk_clist_set_column_widget(GTK_CLIST(clist),0,image);/*设置标题按钮构件*/

gtk_clist_column_titles_show(GTK_CLIST(clist));

gtk_container_add(GTK_CONTAINER(window),clist);

gtk_widget_show(clist);

gtk_widget_show(window);

gtk_main();

}

运行上面程序得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070627205426.png)

我们还可以用gtk_clist_set_shadow_type来设置分栏列表构件的边框类型。

7．

|名称:：|gtk_clist_set_shadow_type|
|----|----|
|功能：|设置分栏列表构件的边框类型|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_clist_set_shadow_type(GtkCList *clist,GtkShadowType border);|
|参数：|clist分栏列表构件border边框类型|
|返回值：|无|





边框类型可以取下面的值：

GTK_SHADOW_NONE

GTK_SHADOW_IN

GTK_SHADOW_OUT

GTK_SHADOW_ETCHED_IN

GTK_SHADOW_ETCHED_OUT

二、向列表中添加/删除行

可以用三种方法添加行。即在前面、后面和指定位子加入行。

8．

|名称:：|gtk_clist_prependgtk_clist_appendgtk_clist_insert|
|----|----|
|功能：|向列表中添加行|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_clist_prepend(GtkCList *clist, gchar *text[ ]);gtk_clist_append(GtkCList *clist, gchar *text[ ]);gtk_clist_insert(GtkCList *clist, gint row, gchar text[ ]);|
|参数：|clist分栏列表构件row列标题编号text文本数组|
|返回值：|无|









有两种方式在列表中删除行，gtk_clist_remove可以删除指定的行，gtk_clist_clear可以删除所有的行。

9．

|名称:：|gtk_clist_removegtk_clist_clear|
|----|----|
|功能：|向列表中添加/删除行|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_clist_remove(GtkCList *clist, gint row);gtk_clist_clear(GtkCList *clist);|
|参数：|clist分栏列表件row行编号|
|返回值：|无|






还有两个很方便的函数可以用在当列表中要发生很大变化时。因为GtkCList在发生变化时要重绘自身，所以当列表中内容变化较大时，频繁重绘会让屏幕不停闪烁。最好的办法是先将列表“冻结”，然后更新列表，最后将其“解冻”。

10．

|名称:：|gtk_clist_freezegtk_clist_thaw|
|----|----|
|功能：|冻结/解冻列表|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_clist_freeze(GtkCList *clist);gtk_clist_thaw(GtkCList *clist);|
|参数：|clist分栏列表构件|
|返回值：|无|






下面我们就向列表中插入几行数据：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *clist;

gchar *text1[3]={“刘备”,”男”,”23”}; /*定义列表项数据*/

gchar *text2[3]={“关羽”,”男”,”22”};

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,GTK_SIGNAL_FUNC(gtk_main_quit),NULL);

gtk_widget_set_size_request(window,200,150);

gtk_container_set_border_width(GTK_CONTAINER(window),10);

clist=gtk_clist_new(3);

gtk_clist_set_column_title(GTK_CLIST(clist),0,”姓名”);

gtk_clist_set_column_title(GTK_CLIST(clist),1,”姓别”);

gtk_clist_set_column_title(GTK_CLIST(clist),2,”年龄”);

gtk_clist_prepend(GTK_CLIST(clist),text1);/*将列表项数据添加到列表*/

gtk_clist_prepend(GTK_CLIST(clist),text2);

gtk_clist_column_titles_show(GTK_CLIST(clist));

gtk_container_add(GTK_CONTAINER(window),clist);

gtk_widget_show(clist);

gtk_widget_show(window);

gtk_main();

}

程序运行后得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070627205443.png)

三、列表设置

可以用gtk_clist_set_column_justification函数设置列的对齐方式.

11．

|名称:：|gtk_clist_set_column_justification|
|----|----|
|功能：|设置列的对齐方式|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_clist_set_column_justification( GtkCList *clist, gint column,GtkJustification justification );|
|参数：|clist分栏列表构件column列标题编号justification对齐方式|
|返回值：|无|







GtkJustification参数类型可取以下值：

GTK_JUSTIFY_LEFT：列中的文本左对齐。

GTK_JUSTIFY_RIGHT：列中的文本右对齐。

GTK_JUSTIFY_CENTER：列中的文本居中对齐。

GTK_JUSTIFY_FILL：文本使用列中所有可用的空间。

创建构件时单元格的宽度是依据它们的标题确定的，而单元格的高度的却省值是当前字体的高度。因此在多数情况下这不一定符合要求，gtk_clist_set_column_width函数设置单元格的宽度。gtk_clist_set_column_width函数可以设置单元格的高度。

12．

|名称:：|gtk_clist_set_column_widthgtk_clist_set_row_height|
|----|----|
|功能：|设置单元格的高度和宽度|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_clist_set_column_width( GtkCList *clist,gint column,gint width );void gtk_clist_set_row_height( GtkCList *clist,gint height );|
|参数：|clist分栏列表构件column列标题编号width单元格的宽度height单元格的高度|
|返回值：|无|









注意，单元格宽度和高度都是以像素度量，而不是以字母度量的。缺省值是当前字体的宽度和高度。



13．

|名称:：|gtk_clist_set_shif|
|----|----|
|功能：|设置单元格内水平和垂直方向上的缩进|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_clist_set_shift( GtkCList *clist,gint row,gint column,gint vertical,gint horizontal);|
|参数：|clist分栏列表构件row行编号column列编号vertical水平缩进horizontal垂直缩进|
|返回值：|无|








gtk_clist_set_column_with函数可以用于设置单元格内水平和垂直方向上的缩进，缩进值是以像素度量的整数值，它可以是正数也可以是负数。下面是我们应用上面两个函数的程序例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *clist;

gchar *text1[3]={“刘备”,”男”,”23”}; /*定义列表项数据*/

gchar *text2[3]={“关羽”,”男”,”22”};

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,GTK_SIGNAL_FUNC(gtk_main_quit),NULL);

gtk_widget_set_size_request(window,200,150);

gtk_container_set_border_width(GTK_CONTAINER(window),10);

clist=gtk_clist_new(3);

gtk_clist_set_column_title(GTK_CLIST(clist),0,”姓名”);

gtk_clist_set_column_title(GTK_CLIST(clist),1,”姓别”);

gtk_clist_set_column_title(GTK_CLIST(clist),2,”年龄”);

gtk_clist_prepend(GTK_CLIST(clist),text1);/*将列表项数据添加到列表*/

gtk_clist_prepend(GTK_CLIST(clist),text2);

gtk_clist_set_column_justification(GTK_CLIST(clist),0,GTK_JUSTIFY_CENTER);/*设置列0的对齐方式为居中对齐*/

gtk_clist_set_column_width(GTK_CLIST(clist),0,50);/*设置列0的宽度为50*/

gtk_clist_set_shift(GTK_CLIST(clist),1,1,0,10);

gtk_clist_column_titles_show(GTK_CLIST(clist));

gtk_container_add(GTK_CONTAINER(window),clist);

gtk_widget_show(clist);

gtk_widget_show(window);

gtk_main();

}

下面是运行后的界面：

![](http://blog.chinaunix.net/photo/22935_070627205500.png)

我们可以gtk_clist_get_text来得到指定单元格的数据，可以用gtk_clist_set_text来修改这些数据。

14．

|名称:：|gtk_clist_get_textgtk_clist_set_text|
|----|----|
|功能：|查看/设置单元格中的文本|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gint gtk_clist_get_text( GtkCList *clist,gint row,gint column,gchar **text );void gtk_clist_set_text( GtkCList *clist,gint row,gint column,const gchar *text );|
|参数：|clist分栏列表构件text单元格文本|
|返回值：|无|








gtk_clist_set_text可以设置单元格的文本，函数可以用新文本来覆盖原有的文本，row和column用来指定单元的位置，单元格的横纵位置都是从0开始的。

gtk_clist_get_text取得指定单元格的数据，这些指针都是指向存储在构件内部的数据指针，所以不应该修改或释放。引用的数据没有必要将不感兴趣的数据全部读出。任何返回值指针（除了GtkCList构件）都可以是NULL。

四、信号

与其他构件一样，GtkCList有一些信号供我们使用。GtkCList构件是从容器构件GtkContainer派生的，它有容器所具有的一些信号，还有下面这些附加信号：

select_row：选中一行时引发，该信号传递以下信息，依次是GtkCList *clist、gint row、

gint column、GtkEventButton *event、gpointer data。该信号的回调函数形势如下：

void select_row_callback(GtkWidget *widget,gint row,gint column,GtkEventButton *event,gpointer data);

unselect_row：用户对一行取消选择，引发这个信号。传递的信息与上一个信号一样。

click_column：选中某一列时引发。传递以下信息： GtkCList *clist、gint column、gpointer data。

下面是程序例子：

#include <gtk/gtk.h>

void select_row_callback(GtkWidget *clist,gint row,gint column,GdkEventButton *event,gpointer data);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *clist;

gchar *text1[3]={“刘备”,”男”,”23”}; /*定义列表项数据*/

gchar *text2[3]={“关羽”,”男”,”22”};

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,GTK_SIGNAL_FUNC(gtk_main_quit),NULL);

gtk_widget_set_size_request(window,200,150);

gtk_container_set_border_width(GTK_CONTAINER(window),10);

clist=gtk_clist_new(3);

gtk_clist_set_column_title(GTK_CLIST(clist),0,”姓名”);

gtk_clist_set_column_title(GTK_CLIST(clist),1,”姓别”);

gtk_clist_set_column_title(GTK_CLIST(clist),2,”年龄”);

gtk_clist_prepend(GTK_CLIST(clist),text1);/*将列表项数据添加到列表*/

gtk_clist_prepend(GTK_CLIST(clist),text2);

gtk_signal_connect(GTK_OBJECT(clist),”select_row”,GTK_SIGNAL_FUNC(select_row_callback),NULL);/*捕捉select_row信号*/

gtk_clist_column_titles_show(GTK_CLIST(clist));

gtk_container_add(GTK_CONTAINER(window),clist);

gtk_widget_show(clist);

gtk_widget_show(window);

gtk_main();

}

void select_row_callback(GtkWidget *clist,gint row,gint column,GdkEventButton *event,gpointer data)/*设置select_row信号的回调函数*/

{

gchar *text=NULL;

gtk_clist_get_text(GTK_CLIST(clist),row,column,&text);

gprint(“%s/n”,text);

}

程序运行后当用户点击单元格，屏幕就打印相应的内容。



