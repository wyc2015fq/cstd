# GTK+图形化应用程序开发学习笔记（十九）—按钮盒构件、笔记本构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 23:47:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2249


**GTK+图形化应用程序开发学习笔记（十九）—按钮盒构件、笔记本构件**

一、按钮盒构件

按钮盒构件(GtkButtonBox) 可以很方便地快速布置一组按钮。它有水平和垂直两种样式。函数gtk_hbutton_box_new创建水平按钮盒构件。函数gtk_vbutton_box_new创建垂直按钮盒构件。

1．

|名称:：|gtk_hbutton_box_newgtk_vbutton_box_new|
|----|----|
|功能：|创建按钮盒构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_hbutton_box_new( void );GtkWidget *gtk_vbutton_box_new( void );|
|参数：|无|
|返回值：|新的按钮盒构件|






创建完按钮盒构件就可以将按钮放置在按钮盒中了。放置按扭用通用容器函数gtk_container_add来完成。

函数gtk_hbutton_box_set_spacing_default和gtk_vbutton_box_set_spacing_default可以改变按钮的间距。

函数gtk_hbutton_box_get_spacing_default和gtk_vbutton_box_get_spacing_default可以获得按钮的间距。

2．

|名称:：|gtk_hbutton_box_set_spacing_defaultgtk_vbutton_box_set_spacing_default|
|----|----|
|功能：|设置按钮间距|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_hbutton_box_set_spacing_default( gint spacing );void gtk_vbutton_box_set_spacing_default( gint spacing );|
|参数：|spacing间距|
|返回值：|无|








3．

|名称:：|gtk_hbutton_box_get_spacing_defaultgtk_vbutton_box_get_spacing_default|
|----|----|
|功能：|取得按钮间距|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gint gtk_hbutton_box_get_spacing_default( void );gint gtk_vbutton_box_get_spacing_default( void );|
|参数：|无|
|返回值：|按钮间距|







函数gtk_hbutton_box_set_layout_default和gtk_vbutton_box_set_layout_default可以设置按扭构件的布局。

函数gtk_hbutton_box_get_layout_default和gtk_vbutton_box_get_layout_default取得按钮构件布局。

4．

|名称:：|gtk_hbutton_box_set_layout_default gtk_vbutton_box_set_layout_default|
|----|----|
|功能：|设置按钮盒的布局|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_hbutton_box_set_layout_default( GtkButtonBoxStyle layout );void gtk_vbutton_box_set_layout_default( GtkButtonBoxStyle layout );|
|参数：|layout 按钮盒的布局|
|返回值：|无|






layout参数可以取以下值：

GTK_BUTTONBOX_DEFAULT_STYLE

GTK_BUTTONBOX_SPREAD

GTK_BUTTONBOX_EDGE

GTK_BUTTONBOX_START

GTK_BUTTONBOX_END 

5．

|名称:：|gtk_hbutton_box_get_layout_defaultgtk_vbutton_box_get_layout_default|
|----|----|
|功能：|获得按钮盒的布局|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkButtonBoxStyle gtk_hbutton_box_get_layout_default( void );GtkButtonBoxStyle gtk_vbutton_box_get_layout_default( void );|
|参数：|无|
|返回值：|按钮盒的布局|







下面是程序例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button_box;

GtkWodget *button;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_size_request(window,300,50);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

button_box=gtk_hbutton_box_new();/*创建按钮盒构件*/

gtk_hbutton_box_set_spacing_default(5);/*设置按钮间距*/

gtk_hbutton_box_set_layout_default(GTK_BUTTONBOX_SPREAD);/*设置按钮盒布局*/

gtk_container_add(GTK_CONTAINER(window),button_box);/*将按钮盒构件加入窗体*/

gtk_widget_show(button_box);

button=gtk_button_new_with_label(“打开”);

gtk_container_add(GTK_CONTAINER(button_box),button);/*将按钮加入按钮盒构件*/

gtk_widget_show(button);

button=gtk_button_new_with_label(“关闭”);

gtk_container_add(GTK_CONTAINER(button_box),button);

gtk_widget_show(button);

button=gtk_button_new_with_label(“帮助”);

gtk_container_add(GTK_CONTAINER(button_box),button);

gtk_widget_show(button);

gtk_widget_show(window);

gtk_main();

}

程序运行后显示如下界面：

![](http://blog.chinaunix.net/photo/22935_070614214451.png)

二、笔记本构件

笔记本构件(GtkNotebook) 是互相重叠的页面集合，每一页都包含不同的信息，且一次只有一个页面是可见的。该构件在G U I (图形用户接口)编程中很常用。要说明大量的相似信息，同时把它们分别显示时，使用这种构件是一个很好的方法。许多应用程序的“选项”对话框都使用了这个构件。

用函数gtk_notebook来创建笔记本构件。

6．

|名称:：|gtk_notebook_new|
|----|----|
|功能：|获得按钮盒的布局|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_notebook_new( void );|
|参数：|无|
|返回值：|新的笔记本构件|






一旦创建了笔记本构件，就可以使用一系列的函数操作该构件。下面将对它们进行分别讨论。

首先是定位页标签，用函数gtk_notebook_set_tab_pos来完成。

7．

|名称:：|gtk_notebook_set_tab_pos|
|----|----|
|功能：|定位页标签|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_notebook_set_tab_pos( GtkNotebook *notebook,GtkPositionType pos );|
|参数：|notebook笔记本构件pos标签位置|
|返回值：|无|








pos参数可以取以下几个值：

GTK_POS_LEFT 将标签页放在左边

GTK_POS_RIGHT 将标签页放在右边

GTK_POS_TOP 将标签页放在顶部

GTK_POS_BOTTOM 将标签页放在底部

它的缺省值是GTK_POS_TOP

下面看一下怎样向笔记本中添加页面。有三种方法向笔记本中添加页面。前两种方法是非常相似的。

8．

|名称:：|gtk_notebook_append_page gtk_notebook_prepend_pagegtk_notebook_prepend_page|
|----|----|
|功能：|在笔记本构件插入页面|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_notebook_append_page( GtkNotebook *notebook,GtkWidget *child,GtkWidget *tab_label );void gtk_notebook_prepend_page( GtkNotebook *notebook,GtkWidget *child,GtkWidget *tab_label );void gtk_notebook_insert_page( GtkNotebook *notebook,GtkWidget *child,GtkWidget *tab_label,gint position );|
|参数：|notebook 笔记本构件child子页面tab_label页面标签position插入位置|
|返回值：|无|















gtk_notebook_append_page函数按从前向后的顺序向笔记本构件插入页面，

gtk_notebook_prepend_page函数总是把页面插入到笔记本构件的第一个页面。

gtk_notebook_insert_page函数按指定把页面插入到笔记本构件。位置由position指定。

child 参数是放在笔记本上的子构件， tab_label是要添加的页面的标签。子构件必须分别创建，一般是一个容器构件，比如说组装盒、表格等构件。

position为要插入位置，第一页位置为0。

下面就是程序例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *notebook;

GtkWidget *box;

GtkWidget *label;

GtkWidget *lb;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_size_request(window,200,100);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

notebook=gtk_notebook_new();/*创建笔记本构件*/

gtk_container_add(GTK_CONTAINER(window),notebook);/*将笔记本构件加入窗体*/

gtk_widget_show(notebook);

box=gtk_hox_new(FALSE,0);/*创建组装盒构件*/

label=gtk_label_new(“常规”);/*创建标签构件*/

lb=gtk_label_new(“这是常规页面”);

gtk_notebook_append_page(GTK_NOTEBOOK(notebook),box,label);/*插入新页面*/

gtk_box_pack_start(GTK_BOX(box),lb,FALSE,FALSE,10);

gtk_widget_show(box);

gtk_widget_show(label);

gtk_widget_show(lb);

box=gtk_hox_new(FALSE,0);

label=gtk_label_new(“高级”);

lb=gtk_label_new(“这是高级页面”);

gtk_notebook_prepend_page(GTK_NOTEBOOK(notebook),box,label);

gtk_box_pack_start(GTK_BOX(box),lb,FALSE,FALSE,10);

gtk_widget_show(box);

gtk_widget_show(label);

gtk_widget_show(lb);

gtk_widget_show(window);

gtk_main();

}

程序运行后得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070614214501.png)

9．

|名称:：|gtk_notebook_remove_page|
|----|----|
|功能：|在笔记本构件中删除页面|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_notebook_remove_page( GtkNotebook *notebook,gint page_num );|
|参数：|notebook笔记本构件page_num子页面|
|返回值：|无|

