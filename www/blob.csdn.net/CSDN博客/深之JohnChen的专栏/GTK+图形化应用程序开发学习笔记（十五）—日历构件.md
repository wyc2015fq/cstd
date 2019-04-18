# GTK+图形化应用程序开发学习笔记（十五）—日历构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 23:27:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1873


**GTK+图形化应用程序开发学习笔记（十五）—日历构件**

日历构件(GtkCalendar)显示一个月历视图，可以在上面方便地选择年份、月份和日期。

这样，如果要做与日期相关的编程，不再需要考虑复杂的历法问题。日历构件本身外观也很

漂亮，创建和使用都非常简单。

1．

|名称:：|gtk_calendar_new|
|----|----|
|功能：|创建日历构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_calendar_new();|
|参数：|无|
|返回值：|新的日历构件|





下面是创建日历的程序例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *calendar;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

calendar=gtk_calendar_new();

gtk_container_add(GTK_CONTAINER(window),calendar);

gtk_widget_show(calendar);

gtk_widget_show(window);

gtk_main();

}

程序运行后会得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070610211124.png)

有时候，需要同时对构件的外观和内容做很多的修改。这时候可能会引起构件的多次更，导致屏幕闪烁。可以在修改之前使用一个函数将构件“冻结”，然后在修改完成之后再用一个函数将构件“解冻”。这样，构件在整个过程中只做一次更新。

我们用gtk_calendar_freeze函数冻结构件，用gtk_calendar_thaw函数解冻构件。

2．

|名称:：|gtk_calendar_freezegtk_calendar_thaw|
|----|----|
|功能：|冻结/解冻日历构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_calendar_freeze( GtkCalendar *Calendar );void gtk_calendar_thaw ( GtkCalendar *Calendar );|
|参数：|calendar 日历构件|
|返回值：|无|






日历构件有几个选项，可以用来改变构件的外观和操作方式。使用gtk_calendar_diaplay_options函数可以改变这些选项.

3．

|名称:：|gtk_calendar_display_options|
|----|----|
|功能：|设置日历构件外观和操作方式|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_calendar_display_options( GtkCalendar *calendar, GtkCalendarDisplayOptions flags );|
|参数：|calendar日历构件flags选项|
|返回值：|无|






函数中的f l a g s参数可以将下面的五种选项中的一个或者多个用逻辑位或（ |）操作符组合起来：

GTK_CALENDAR_SHOW_HEADING：这个选项指定在绘制日历构件时，应该显示月份和年份。

GTK_CALENDAR_SHOW_DAY_NAMES：这个选项指定用三个字母的缩写显示每一天是星期几（比如M O N、T U E等）。

GTK_CALENDAR_NO_MONTH_CHANGE：这个选项指定用户不应该也不能够改变显示的月份。如果只想显示某个特定的月份，则可以使用这个选项。比如，如果在窗口上同时为一年的1 2个月分别设置一个日历构件时。

GTK_CALENDAR_SHOW_WEEK_NUMBERS：这个选项指定应该在构件的左边显示每一周在全年的周序号（一年是5 2个周，元月1日是第1周，1 2月3 1日是第5 2周）。

GTK_CALENDAR_WEEK_START_MONDAY：这个选项指定在日历构件中每一周是从星期一开始而不是从星期天开始。缺省设置是从星期天开始。此选项只影响日期在构件中从左到右的排列顺序。

当设置好日历构件后，我门可以通过gtk_calendar_select_month函数选择某一月份，通过gtk_calendar_select_day选择某一天。

4．
|名称:：|gtk_calendar_select_monthgtk_calendar_select_day|
|----|----|
|功能：|选择的日期构件的日期|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gint gtk_calendar_select_month( GtkCalendar *calendar,guint month, guint year );void gtk_calendar_select_day( GtkCalendar *calendar, guint day );|
|参数：|calendar日历构件month月份year年day日|
|返回值：|成功返回TRUE,错误返回FALSE|









gtk_calendar_select_month( )的返回值是一个布尔值，指示设置是否成功。如果设置一个非法值则会返回一个FALSE值。

使用gtk_calendar_select_day ( )函数，如果d a y参数指定的日期是合法的，会在日历构件中选中该日期。

好了我们要体验一下上面两个函数的作用：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *calendar;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

calendar=gtk_calendar_new();

gtk_calendar_display_options(GTK_CALENDAR(calendar),GTK_CALENDAR_SHOW_HEADING|GTK_CALENDAR_SHOW_DAY_NAMES|GTK_CALENDAR_SHOW_WEEK_NUMBERS|GTK_CALENDAR_WEEK_START_MONDAY);

gtk_calendar_select_month(GTK_CALENDAR(calendar),8,1985);

gtk_calendar_select_day(GTK_CALENDAR(calendar),8);

gtk_container_add(GTK_CONTAINER(window),calendar);

gtk_widget_show(calendar);

gtk_widget_show(window);

gtk_main();

}

程序运行后会显示如下窗体：

![](http://blog.chinaunix.net/photo/22935_070610211134.png)



除了可以选中一个日期以外，在一个月中可以有任意个日期被“标记”。被“标记”的日期会在日历构件中高亮显示。函数gtk_calendar_mark_day用于标记日期，函数gtk_calendar_unmark_day用于取消标记。注意，在月份和年份变化时，被标记的日期是不会变化的。

4．
|名称:：|gtk_calendar_mark_daygtk_calendar_unmark_daygtk_calendar_clear_marks|
|----|----|
|功能：|标记/取消标记/清除标记|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gint gtk_calendar_mark_day( GtkCalendar *calendar,guint day);gint gtk_calendar_unmark_day( GtkCalendar *calendar,guint day);void gtk_calendar_clear_marks( GtkCalendar *calendar);|
|参数：|calendar日历构件day日|
|返回值：|成功返回TRUE,错误返回FALSE|










5．
|名称:：|gtk_calendar_get_date|
|----|----|
|功能：|取得当前选中的年/月/日|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_calendar_get_date( GtkCalendar *calendar,guint *year,guint *month,guint *day );|
|参数：|calendar日历构件month月份year年day日|
|返回值：|无|









gtk_calendar_get_date可以取得当前选中的年/月/日。在使用这个函数时，需要先声明几个guint类型的变量—传递给函数的year、month和day参数。所需要的返回值就存放在这几个变量中。如果将某一个参数设置为NULL，则不返回该值。

#include <gtk/gtk.h>

GtkWidget *calendar;

void button_event(GtkWidget *widget, gpointer *data);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *box;

GtkWidget *button;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_vbox_new(FALSE,10);/*建立组装盒*/

gtk_container_add(GTK_CONTAINER(window),box);/*将组装盒加入窗体*/

calendar=gtk_calendar_new();/*建立日历构件

gtk_box_pack_start(GTK_BOX(box),calendar,TRUE,TRUE,5);/*将日历构件加入组装盒*/

gtk_widget_show(calendar);/*显示日历构件*/

button=gtk_button_new_with_label(“确定”);

gtk_box_pack_start(GTK_BOX(box),button,TRUE,TRUE,0);

gtk_signal_connect(GTK_OBJECT(button),”clicked”,GTK_SIGNAL_FUNC(button_event),NULL);

gtk_widget_show(button);

gtk_widget_show(box);

gtk_widget_show(window);

gtk_main();

}

void button_event(GtkWidget *widget,gpointer *data)

{

guint year;

guint month;

guint day;

gtk_calendar_get_data(GTK_CALENDAR(calendar),&year,&month,&day);/*取得选择的年月日*/

g_print(“Year:%d Month:%d Day:%d/n”,year,month,day);

}

程序运行后得到如下窗体，当点击确定时程序打印选择的年月日。

![](http://blog.chinaunix.net/photo/22935_070610211142.png)

日历构件还有一些信号它们是：

month_changed/*选择月份变化* /

day_selected /*选择日期变化* /

day_selected_double_click /*选中日期并以鼠标双击* /

prev_month /*选择前一月* /

next_month /*选择下一月* /

prev_year /*选择前一年* /

next_year /*选择下一年* /


