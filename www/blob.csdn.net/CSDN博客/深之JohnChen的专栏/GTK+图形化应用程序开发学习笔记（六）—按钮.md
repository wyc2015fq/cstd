# GTK+图形化应用程序开发学习笔记（六）—按钮 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 00:09:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2042


**GTK+图形化应用程序开发学习笔记（六）—按钮**

一、常用构件函数

最常用的构件是按钮构件、录入字段构件、列表框构件和复合框构件。所有建立按钮的函数返回指向GtkWidget的指针。GtkWidget指针能够调用对构件进行操作的通用函数。函数gtk_widget_show是通用函数的例子。

由于建立的构件是通用的构件，需要将它转换为正确的类型以适合调用更为专用的函数。如：建立按钮函数返回GtkWidget指针，但是专用的按钮子程序要求GtkButton指针。所有在调用专用的按钮函数以前，需要使用GTK_BUTTON宏将通用的GtK_Widget指针转换为GtKButton指针。

二、按钮

按钮时最简单的构件，因为它们的最常用功能是允许用户点击它们。“点击”事件是一次按下按钮和释放按钮的组合。通常点击按钮导致某些操作的完成，如保存文件或结束对话等。

按钮由容器派生而来，所有它们共享容器的许多特性。一个重要的特性是容纳其它构件的能力。按钮构件的正文实际是放在按钮构件内的标号构件。

建立按钮可以带标号或不带标号。函数gtk_button_new_with_label建立带标号的按钮。函数gtk_button_new建立不带子构件的按钮。函数返回指向GtkWidget的指针，可以使用GTK_BUTTON宏将它转换为GtkButton。

1．

|名称:：|gtk_button_newgtk_button_new_with_label|
|----|----|
|功能：|创建一个不带标号的按钮（gtk_button_new）创建一个带标号的按钮（gtk_button_new_with_label）|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_button_new(void);GtkWidget *gtk_button_new_with_label(const gchar *label);|
|参数：|label按钮正文|
|返回值：|新的按钮|






按钮构件能够发送若干种信号。按钮信号有：

pressed 按下按钮

released释放按钮

clichéd点击按钮，这是“按下”和“释放”操作的组合

enter鼠标移动到按钮上

leave鼠标离开按钮

事件可以使用户的操作引起的，也可以用下面的函数模拟事件的发生。但不经常使用模拟信号发生的函数。

pressed gtk_button_pressed(button)

releasedgtk_button_released(button)

clichédgtk_button_clicked(button)

entergtk_button_enter(button)

leavegtk_button_leave(button)

下面的短信息建立带按钮的窗口。当产生信号时，在控制台上显示每一种按钮信号。

#include <gtk/gtk.h>

void button_event(GtkWidget *,gpointer *);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

gtk_init(&argc,&argv); /*初始化*/

window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,G_CALLBACK(gtk_main_quit),NULL); /*登记窗体delete_event信号的回调函数*/

button=gtk_button_new_with_label(“Button”); /*创建带标号的按钮*/

gtk_signal_connect(GTK_OBJECT(button),”pressed”,GTK_SIGNAL_FUNC(button_event),”pressed”); /*登记按钮pressed信号的回调函数*/

gtk_signal_connect(GTK_OBJECT(button),”released”,GTK_SIGNAL_FUNC(button_event),”released”); /*登记按钮released信号的回调函数*/

gtk_signal_connect(GTK_OBJECT(button),”clicked”,GTK_SIGNAL_FUNC(button_event),”clicked”); /*登记按钮clicked信号的回调函数*/

gtk_signal_connect(GTK_OBJECT(button),”enter”,GTK_SIGNAL_FUNC(button_event),”enter”); /*登记按钮enter信号的回调函数*/

gtk_signal_connect(GTK_OBJECT(button),”leave”,GTK_SIGNAL_FUNC(button_event),”leave”); /*登记按钮leave信号的回调函数*/

gtk_container_add(GTK_CONTAINER(window),button); /*把按钮加入窗体*/

gtk_widget_show(button); /*显示按钮*/

gtk_widget_show(window); /*显示窗体*/

gtk_main(); 

return 0;

}

void button_event(GtkWidget *widget,gpointer *data) /*回调函数*/

{

g_print(“Button event:%s/n”,data);

}

我们编译上面的程序：

$gcc –o button1 button1.c `pkg-config –cflags –libs gtk+-2.0

$./button1

下面是显示页面：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/1633651559647187500.png)

当用户点击Button然后移开，终端会显示如下信息：

Button event: enter

Button event: pressed

Button event: clicked

Button event: released

Button event: leave

当然这只是一个演示，在实际应用中也许你需要通过点击一个按钮来调用一个段具有一定功能的函数。

二、触发按钮

触发按钮（toggle button,GtkToggleButton）是GtkButton派生而来。它的外观类似GtkButton,但运行方式稍有不同。触发按钮与一种状态结合在一起。它可以按下和弹回。触发按钮的外观反映它的状态。在开始，触发按钮看上去和普通按钮一样。如果按下按钮，它就停留在下面。触发按钮需要再按一次才能弹回来。下面是触发按钮的两种状态：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/2633651559647500000.png)弹起状态![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/3633651559647812500.png)按下状态

可以使用gtk_toggle_button_new_with_labal函数建立带标号的触发按钮，或者使用gtk_toggle_button_new函数建立不带标号的触发按钮。因为触发按钮是由普通按钮派生而来的，所以所有可以用在普通按钮上的事件和函数都可以用在触发按钮上。并且触发按钮增加了一个信号”toggled(触发)”，当按钮的状态改变时它发送”触发”信号。

2．

|名称:：|gtk_toggle_button_newgtk_toggle_button_new_with_label|
|----|----|
|功能：|创建一个不带标号的触发按钮（gtk_toggle_button_new）创建一个带标号的触发按钮（gtk_toggle_button_new_with_label）|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_toggle_button_new(void);GtkWidget *gtk_toggle_button_new_with_labal(const gchar *label);|
|参数：|label触发按钮正文|
|返回值：|新的触发按钮|





















我们对button1程序稍加改动，并且添加了对“toggled”信号的处理。

/*button2.c*/

#include <gtk/gtk.h>

void button_event(GtkWidget *,gpointer *);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

gtk_init(&argc,&argv); /*初始化*/

window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,G_CALLBACK(gtk_main_quit),NULL); /*登记窗体delete_event信号的回调函数*/

button=gtk_toggle_button_new_with_label(“Button”); /*创建带标号的触发按钮*/

gtk_signal_connect(GTK_OBJECT(button),”pressed”,GTK_SIGNAL_FUNC(button_event),”pressed”); /*登记按钮pressed信号的回调函数*/

gtk_signal_connect(GTK_OBJECT(button),”released”,GTK_SIGNAL_FUNC(button_event),”released”); /*登记按钮released信号的回调函数*/

gtk_signal_connect(GTK_OBJECT(button),”clicked”,GTK_SIGNAL_FUNC(button_event),”clicked”); /*登记按钮clicked信号的回调函数*/

gtk_signal_connect(GTK_OBJECT(button),”enter”,GTK_SIGNAL_FUNC(button_event),”enter”); /*登记按钮enter信号的回调函数*/

gtk_signal_connect(GTK_OBJECT(button),”leave”,GTK_SIGNAL_FUNC(button_event),”leave”); /*登记按钮leave信号的回调函数*/

gtk_signal_connect(GTK_OBJECT(button),”toggle”,GTK_SIGNAL_FUNC(button_event),”toggle”); /*登记按钮toggle信号的回调函数*/

gtk_container_add(GTK_CONTAINER(window),button); /*把按钮加入窗体*/

gtk_widget_show(button); /*显示触发按钮*/

gtk_widget_show(window); /*显示窗体*/

gtk_main(); 

return 0;

}

void button_event(GtkWidget *widget,gpointer *data) /*回调函数*/

{

g_print(“Button event:%s/n”,data);

}

当用户点击Button然后移开，终端会显示如下信息：

Button event: enter

Button event: pressed

Button event: toggled

Button event: clicked

Button event: released

Button event: leave


可以看到与上面的区别是增加了Button event: toggled信息。这是由“触发”信号产生的。

三、检查按钮

检查按钮就是我们通常来说的复选框。检查按钮（check button,GtkCheckButton）由触发按钮（GtkToggleButton）派生而来。GtkCheckButton的表现类似GtkToggleButton。两者的主要差别是按钮在屏幕上的显示方式不同。两种构件完成同样的功能，选择哪一种按钮纯粹是用户爱好。下面是选择按钮的两种状态。

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/4.png)![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/5.png)

下面是创建检查按钮的函数：

3．

|名称:：|gtk_check_button_newgtk_check_button_new_with_label|
|----|----|
|功能：|创建一个不带标号的检查按钮（gtk_check_button_new）创建一个带标号的检查按钮（gtk_check_button_new_with_label）|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_check_button_new(void);GtkWidget *gtk_check_button_new_with_labal(const gchar *label);|
|参数：|label检查按钮正文|
|返回值：|新的检查按钮|










只要把button2.c中的button=gtk_toggle_button_new_with_label(“Check Button”); 

替换为：button=gtk_check_button_new_with_label(“Button”);就可创建如图所示的按钮

了。

四、选择按钮

选择就是我们通常来说的单选框。从外观来看，选择按钮（GtkRaddioButton）很像检查按钮。因为选择按钮是从检查按钮派生而来的。选择按钮和检查按钮的区别不大，对选择按钮来说，任何时候在一组按钮只可以选择一个按钮，点击一个按钮就释放前面选择的按钮，使点击的按钮成为被选择的按钮。

可以用gtk_radio_hutton_new_with_label函数建立带标号的GtkRadioButton，或者用gtk_radio_button_new函数建立不带标号的GtkRadioButton。建立GtkRadioButton只完成了工作的一半。因为选择按钮必须和组联系起来，使得任何时候只能选择一个按钮。

4．

|名称:：|gtk_radio_button_newgtk_radio_button_new_with_label|
|----|----|
|功能：|创建一个不带标号的选择按钮（gtk_radio_button_new）创建一个带标号的选择按钮（gtk_radio_button_new_with_label）|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_radio_button_new(GSList *group);GtkWidget *gtk_radio_button_new_with_labal(GSList *group,const gchar *label);|
|参数：|group所属组label选择按钮正文|
|返回值：|新的选择按钮|








在创建选择按钮的时候需要指定选择按钮的组，在使用组之前应该先把组指向NULL。否则会出现错误。

5．

|名称:：|gtk_radio_button_group|
|----|----|
|功能：|把选择按钮添加到组中|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GSList *gtk_group_button_group(GtkWidget *radio);|
|参数：|radio选择按钮|
|返回值：|组名|





每添加一个选择按钮后都得调用一次gtk_radio_button_group函数取得组，然后继续添加按钮。如果不这样做将导致不可预期的结果。

下面我们就创建2个按钮，并把它们加入到一个组中：

/*button4.c*/

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *button;

GtkWidget *box;

GSList *group=NULL;

gtk_init_chrck(&argc,&argv); /*初始化GTK+工具库*/

window=gtk_window_new(GTK_WINDOW_TOPLEVEL); /*创建窗体*/

gtk_signal_connect(GTK_OBJECT(window),”delete_event”,G_CALLBACK(gtk_main_quit),NULL); /*登记窗体delete_event信号*/

box=gtk_vbox_new(FALSE,0);/*创建组合框*/

button=gtk_radio_button_new_with_label(group,”Radio Button1”);/*创建按钮*/

group=gtk_radio_button_group(GTK_RADIO_BUTTON(button));/*把按钮加到组中*/

gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);/*把按钮加入组合框*/

gtk_widget_show(button);/*显示按钮*/

button=gtk_radio_button_new_with_label(group,”Radio Button2”);

group=gtk_radio_button_group(GTK_RADIO_BUTTON(button));

gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);

gtk_widget_show(button);

gtk_container_add(GTK_CONTAINER(window),box);/*把组合框加入窗体*/

gtk_widget_show(box);/*显示组合框*/

gtk_widget_show(window);/*显示窗体*/

gtk_main();

return 0;

}

我们编译上面的程序：

$gcc –o button4 button4.c `pkg-config –cflags –libs gtk+-2.0

$./button4

下图是运行之后的页面：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/6.png)

