# GTK+图形化应用程序开发学习笔记（四）—容器、构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月17日 23:50:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3940


**GTK+图形化应用程序开发学习笔记（四）—容器、构件**

一、构件

上一章我们介绍了如果创建一个窗体，但应用程序仅有一个窗体是不够的我们必须在窗体上添加一些东西（如：按钮，文本框等）。这些东西被称为构件，而我们添加的构件的载体被称为容器（如：主窗体）。

构件（GtkWidget）有一些统一的函数对其操作，通常情况下我们定义一个GtkWidget类型的构件指针。再根据不同的需要进行转换。

添加一个构建分5个步骤。

1、建立构件，并取得GtkWidget指针。

2、对构件设置回呼函数。一些构件不能有回调函数；一些构件不需要回调函数；但是，许多构件用回调函数与用户进行对话。

3、设置构件的特性。例如，对按钮设置标号，或使标号左对齐等。有时可以用建立构件的函数设置特性。

4、将构件加到容器中。除非构件是顶层窗口，所有其他构件都必须将它们加到可视容器内才能显示。

5、使构件成为可见。使构件成为可见通常被最后，因为它改变了窗口的性质。

二、构件的敏感性

构件可以是敏感或不敏感的，不敏感的构件不能对输入进行响应。一般不敏感的构件是灰色的。用gtk_widget_set_sensitive 函数改变构件的敏感性。

1．
|名称:：|gtk_widget_set_sensitive|
|----|----|
|功能：|改变构件的敏感性|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_widget_set_sensitive(GtkWidget *widget,gboolean setting);|
|参数：|widget构件gboolean构件的敏感性|
|返回值：|无|




构件缺省是敏感的。只有构件的所有容器是敏感的，构件才能是敏感的。也就是，可以通过设置容器的敏感性来让整个容器内的构件敏感（或不敏感）。

请看下面的程序例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

button=gtk_button_new_with_label(“button”);

gtk_container_add(GTK_CONTAINER(window),button);

gtk_widget_set_sensitive(button,FALSE);

gtk_widget_show(button);

gtk_widget_show(window);

gtk_main();

}

运行程序后会得到如下界面，可以看到按纽是灰色的，并且不可以用。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081217/22935_070523223010633651548201562500.png)

三、构件的类型转换

在GTK中，所有构件的存储形式都是GtkWidget，但是许多函数都需要指向某种构件类型（比如GtkButton）的指针作为参数。虽然所有构件都是从GtkWidget派生而来的，但是编译器并不理解这种派生和继承关系。为此，GTK引进了一套类型转换系统。这些系统都是通过宏实现的。这些宏测试给定数据项的类型转换能力，并实现类型转换。

下面几个宏我们以后会经常遇到：

GTK_WIDGET(winget)

GTK_OBJECT(object

GTK_SIGNAL_FUNC(function)

GTK_CONTAINER(window)

GTK_BOX(box)

四、统一构件函数

2．

|名称:：|gtk_widget_destroy|
|----|----|
|功能：|删除一个构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|voidgtk_widget_destroy(GtkWidget *widget);|
|参数：|widget构件|
|返回值：|无|





gtk_widget_destroy函数可以删除一个构件，它非常简单，我们这里就不再介绍了。

3．

|名称:：|gtk_widget_hide|
|----|----|
|功能：|隐藏一个构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_widget_hide (GtkWidget *widget);|
|参数：|widget构件|
|返回值：|无|





gtk_widget_hide函数可以使构件变得不可见，当实际上构件还存在，你还可以调用gtk_widget_show重新显示他们。

4．

|名称:：|gtk_widget_set_size_requestgtk_widget_get_size_request|
|----|----|
|功能：|设置/获得构件大小|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_widget_set_size_request (GtkWidget *widget, gint *width, gint *height);void gtk_widget_get_size_request (GtkWidget *widget, gint *width, gint *height);|
|参数：|widget构件width构件宽度height构件长度|
|返回值：|无|









gtk_widget_set_size_request可以设置构件的大小，gtk_widget_get_size_request函数可以获得构件的大小。

如：设置一个窗体为长500宽400。

gtk_widget_set_size_request(window,500,400);

五、容器

容器（GtkContainer）可以被看成是一个构件的载体，各种构件通过放置在容器中而被显现出来。我们知道顶层窗口可以作为容器，然而还有许多构件也可以作为容器，如按钮。事实上，在建立带标号的按钮构件时，在按钮构件中已经放入了标号构件。

对于容器只能容纳一个构件。为什么是这样，如果你增加第二个按钮，只有第一个按钮是可见的。绕过这一限制的方法是使用组合框或组合表。


六、统一容器函数


5．
|名称:：|gtk_container_add|
|----|----|
|功能：|向容器里添加构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_container_add(GtkContainer *container, GtkWidget *widget);|
|参数：|container容器widget构件|
|返回值：|无|






gtk_container_add可以向容器里添加构件，但要注意一个容器只能容纳一个构件。

6．
|名称:：|gtk_container_remove|
|----|----|
|功能：|从容器里移出构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_container_remove(GtkContainer *container, GtkWidget *widget);|
|参数：|container容器widget构件|
|返回值：|无|






gtk_container_remove函数和gtk_container_add函数的功能正好相反。移出的构件并不消失，还可以添加到容器里。

7．
|名称:：|**gtk_container_set_border_width ****gtk_container_get_border_width**|
|----|----|
|功能：|设置/获得容器边缘大小|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_container_get_border_width (GtkContainer *container, [guint](http://developer.gnome.org/doc/API/2.0/glib/glib-Basic-Types.html#guint) border_width);guint gtk_container_get_border_width (GtkContainer *container);|
|参数：|container容器border_width容器边缘大小|
|返回值：|无|







gtk_container_set_border_width可以设置容器边缘大小，gtk_container_get_border_width可以得到容器边缘的大小。


