# GTK+图形化应用程序开发学习笔记（三）—窗体 - 深之JohnChen的专栏 - CSDN博客

2008年12月17日 23:46:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2077


**GTK+图形化应用程序开发学习笔记（三）—窗体**

一、初始化GTK+ 

写GTK+程序需要调用gtk_init函数对GTK+ 库函数进行初始化。

1．

|名称:：|gtk_init|
|----|----|
|功能：|初始化GTK+库|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_init(int *argc,char ***argv);|
|参数：|argc指向主函数argc的指针argv指向主函数argv的指针|
|返回值：|无|






在你程序使用到GTK+工具库之前，必须对它进行初始化。gtk_init可以初始化GTK+工具库。gtk_init的参数的指向主函数argc,argv的指针，它可以改变一些不满足GTK+函数要求的命令行参数。

因为gtk_init函数没有返回值，所以如果在初始化过程中发生错误程序就会立即退出。

2．

|名称:：|gtk_init_check|
|----|----|
|功能：|初始化GTK+库|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gboolean gtk_init_check(int *argc,char ***argv);|
|参数：|argc指向主函数argc的指针argv指向主函数argv的指针|
|返回值：|成功返回TURE，出错返回FALSE。|






还有一个GTK+库初始化函数gtk_init_check，它的作用和gtk_init完全相同。唯一的区别是gtk_init_check有返回值，可以判断初始化是否成功。

二、建立窗口

GTK+的构件是GUI的组成部分。窗口、检查框、按钮和编辑字段都属于构件。通常将构件和窗口定义为指向GtkWidget结构的指针。在GTK+中，GtkWidget是用于所有构件和窗口的通用数据类型。

GTK+ 库进行初始化后，大多数应用建立一个主窗口。在GTK+中，主窗口常常被称为顶层窗口。顶层窗口不被包含在任何其他窗口内，所以它没有上层窗口。在GTK+ 中，构件具有父子关系，其中父构件是容器，而子构件则是包含在容器中的构件。顶层窗口没有父窗口，但可能成为其他构件的容器。

在GTK+中建立构件分两步：建立构件，然后使它可以看得见。gtk_window_new函数负责建立窗口。gtk_widget_show函数负责使它成为可见。

2.

|名称:：|gtk_window_new|
|----|----|
|功能：|建立窗口|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget * gtk_window_new(|
|参数：||
|返回值：||






3.

|名称:：|gtk_widget_show|
|----|----|
|功能：|显示窗口|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gtk_widget_show( GtkWidget *window)|
|参数：||
|返回值：||







三、GTK+的事件循环

对GTK+进行初始化并将窗口和构件置于屏幕以后，程序就调用get_main函数等待某种事件的执行。

4.

|名称:：|gtk_main|
|----|----|
|功能：|等待事件的发生|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_main(void);|
|参数：|无|
|返回值：|无|





我们在来看看第一章的程序。也许大家就会理解上面函数的作用了。

/*base1.c*/

#include <gtk/gtk.h>

int main(int argc, char* argv[])

{

GtkWidget *window;

gtk_init(&argc,&argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_show(window);

gtk_main();

return FALSE;

}

$ gcc -o base1 base1.c 'pkg-config --cflags --libs gtk+-2.0'

$./base1

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081217/22935_070523223010.png)

程序开始定义了一个窗体，然后用函数gtk_init初始化GTK+库。用函数gtk_window_new创建一个窗体，用get_widget_show显示该窗体。程序最后调用gtk_main()进入主循环，等待各种事件的发生.

注意该程序不能正常退出，原因是程序没有回呼函数。关于回呼函数我们将会在下面介绍。



四、结束应用程序

5.

|名称:：|gtk_main_quit|
|----|----|
|功能：|结束应用程序|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_main_quit(void);|
|参数：|无|
|返回值：|无|





gtk_main_quit函数可以结束程序

**五、回调函数**

由于程序必须能够对用户的操作做出相应，在基于GUI的程序设计中信号是必要的。移动鼠标，按下按钮，敲入正文或者关闭窗口，将给应用软件的回呼函数提供信号。信号可能需要应用软件来加以处理。如：字处理软件有使字体变黑的按钮。如果用户按下了按钮，就需要调用使字体变黑的程序。与此类型，如果用户关闭了主窗口，在实际关闭窗口以前要进行某些处理（如保存文件，清除等）。

在GTK+中经常产生各种信号，多数情况下信号被忽略。以按钮构件为例，应用软件有专门用于按钮的信号。当用户按下鼠标或释放鼠标按钮时，当用户电机鼠标时，或者当鼠标移过按钮或离开按钮时都产生各自的信号。应用程序可以忽略掉一些信号，只对感兴趣的事件加以处理。

当需要对信号进行处理时，需要用GTK+登记回调函数，并将它和构件联系在一起。构件可以登记回调函数，回调函数可与多个构件联系在一起。

6.
|名称:：|g_signal_connect|
|----|----|
|功能：|信号登记函数|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gulong g_signal_connect(gpointer *object,const gchar *name,GCallback func,gpointer data);|
|参数：|object发出信号的控件name信号名称func回调函数（对信号要采取的动作）data传给回调函数的数据|
|返回值：||







g_signal_connect用于登记一个GTK+信号，其功能有点像普通信号登记函数signal.当某个空间发出信号，程序就会去执行由g_signal_connect登记的回调函数。

下面我们把上面的程序稍微改动一下，使它可以正常退出。

/*base2.c*/

#include <gtk/gtk.h>

int main(int argc, char* argv[])

{

GtkWidget *window;

gtk_init(&argc,&argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

g_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);

gtk_widget_show(window);

gtk_main();

return FALSE;

}

$ gcc -o base2 base2.c 'pkg-config --cflags --libs gtk+-2.0'

$./base2

其中destroy为GTK+最基本信号之一，当关闭窗口时，发出该信号。还有一个是delete_event, 当将要关闭窗口时，发出该信号。

程序中添加了gtk_signal_connect函数，当用户关闭窗口时gtk_signel_connect函数调用gtk_main_quit函数来关闭程序。

大家也可以编写回调函数，在回调函数里结束程序。这样做的好处是当用户试图退出一个程序时，程序可以提示你是否真的退出。

自己编写回调函数的程序例子：

/*base2.c*/

#include <gtk/gtk.h>

ginit destroy(GtkWidget *,gpointer)

int main(int argc, char* argv[])

{

GtkWidget *window;

gtk_init(&argc,&argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

g_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(destroy),NULL);

gtk_widget_show(window);

gtk_main();

return 0;

}

ginit destroy(GtkWidget *widget,gpointer gdata)

{

g_print(“Quitting!/n”);

gtk_main_quit();

return(FALSE);

}

$ gcc -o base3 base3.c 'pkg-config --cflags --libs gtk+-2.0'

$./base3

注意，当关闭窗口时，将在启动应用程序的控制台上显示Quitting消息。这是由回调函数显示的。

从上面的程序可以看到，g_signal_connect对应的回调函数形式为ginit destroy(GtkWidget *widget,gpointer gdata)有两个参数。GTK+还有一个信号登记函数。g_signal_connect_swapped.，它的作用和gtk_signal_connect相同。不同的是g_signal_connect_swapped.对应的回调函数只有一个参数，形式为ginit destroy(GtkWidget *widget)。是因为GTK+有一些只接收一个参数的函数(比如gtk_widget_destroy)。



7.
|名称:：|g_signal_connect_swapped|
|----|----|
|功能：|信号登记函数|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gulong g_signal_connect_swapped(gpointer *object,const gchar *name,GCallback func,gpointer *widget);|
|参数：|object发出信号的控件name信号名称func回调函数（对信号要采取的动作）winget传给回调函数的数据|
|返回值：||



















**六、其它窗体函数**

在上面我们已经介绍了怎样去建立一个窗体，下面我们来介绍其它的窗体函数。

8、
|名称:：|gtk_window_set_title|
|----|----|
|功能：|修改窗体标题|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_window_set_title(GtkWindow *window,const gchar *title);|
|参数：|window窗体名title窗体标题|
|返回值：|无|





gtk_window_set_title函数可以修改程序的标题。窗口的标题会出现在标题栏中。在X窗体系统中，标题栏被窗体管理器管理，并由程序员指定。标题应该帮助用户区分当前窗体与其它窗体。

如果我们想把一个程序的标题修改为“Main Window”.我们可在程序中加入

gtk_window_set_title(GTK_WINDOW(window),”Main Window”);

9.
|名称:：|gtk_window_set_resizablegtk_window_get_resizable|
|----|----|
|功能：|获得/修改窗体的伸缩属性|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_window_set_resizable(GtkWindow *window,gboolean resizable);gboolean gtk_window_get_resizable(GtkWindow *window);|
|参数：|window窗体名resizable窗体是否可以伸缩|
|返回值：|无（ gtk_window_set_resizable）如果可以伸缩为TRUE，如果不可以伸缩为FALSE（ gtk_window_get_resizable）|









gtk_window_get_resizable可以获得窗体的伸缩属性，系统默认窗体是可伸缩的。gtk_window_get_resizable有一个返回值，如果可以伸缩为TRUE，如果不可以伸缩为FALSE。

gtk_window_set_resizable 可以修改窗体的伸缩属性，由第二参数指定。

如果我们想把一个窗体指定为不可伸缩的我们可以在程序中添加：

gtk_window_set_resizable(GTK_WINDOW(window),FALSE);

，它通常在回调函数中被调用。下面我们介绍回调函数。

