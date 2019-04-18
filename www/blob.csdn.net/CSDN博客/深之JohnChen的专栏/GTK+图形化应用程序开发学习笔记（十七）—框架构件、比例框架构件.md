# GTK+图形化应用程序开发学习笔记（十七）—框架构件、比例框架构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 23:32:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3280


**GTK+图形化应用程序开发学习笔记（十七）—框架构件、比例框架构件**

一、框架构件

框架构件（GtkFrame）可以用于在盒子中封装一个或一组构件，框架本身还可以有一个标签。标签的位置和风格可以灵活改变。

我们用gtk_frame_set_label函数创建框架构件。

１．
|名称:：|gtk_frame_new|
|----|----|
|功能：|创建框架构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_frame_new( const gchar *label );|
|参数：|label框架构件的标签|
|返回值：|新的框架构件|





label为框架构件的标签。缺省设置时，标签放在框架的左上角。传递NULL时，框架不显示标签。

下面是创建框架的程序例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *frame;

GtkWidget *button;

GtkWidget *box;

GSList *group=NULL;/*定义组*/

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_size_request(window,100,80);

gtk_signal_connect(GTK_BOX(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

grame=gtk_frame_new(“性别”); /*创建框架构件*/

gtk_container_add(GTK_CONTAINER(window),frame);/*将框架构件加入窗体*/

gtk_widget_show(frame);

box=gtk_vbox_new(FALSE,0);/*创建组装框*/

gtk_container_add(GTK_CONTAINER(frame),box);/*将组装框加入框架构件*/

gtk_widget_show(box);

button=gtk_radio_button_new_with_label(group,”男”);/*创建按钮*/

group=gtk_radio_button_group(GTK_RADIO_BUTTON(button));/*将按钮加入组装框*/

gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);

gtk_widget_show(button);

button=gtk_radio_button_new_with_label(group,”女”);

group=gtk_radio_button_group(GTK_RADIO_BUTTON(button));

gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);

gtk_widget_show(button);

gtk_widget_show(window);

gtk_main();

}

程序运行后得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070612162241.png)

标签文本可以用gtk_frame_set_label函数改变。

2．
|名称:：|gtk_frame_set_label|
|----|----|
|功能：|修改框架构件的标签|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_frame_set_label( GtkFrame *frame,const gchar *label );|
|参数：|frame框架构件label标签|
|返回值：|无|







标签的位置可以用gtk_frame_set_label_align

3．
|名称:：|gtk_frame_set_label_align|
|----|----|
|功能：|修改框架构件标签的位置|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_frame_set_label_align( GtkFrame *frame,gfloat xalign,gfloat yalign );|
|参数：|frame框架构件xalign标签在框架构件上部水平线上的位置yalign目前还没有使用|
|返回值：|无|







xalign和yalign参数取值范围介于0.0和1.0之间。xalign指定标签在框架构件上部水平线上的位置。yalign参数目前还没有使用。xalign的缺省值是0.0，它将标签放在框架构件的左上角处。

gtk_frame_set_label_align函数可以改变框架的轮廓框风格。

4．
|名称:：|gtk_frame_set_shadow_type|
|----|----|
|功能：|修改框架构件的轮廓框风格|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_frame_set_shadow_type( GtkFrame *frame,GtkShadowType type);|
|参数：|frame框架构件type框架的轮廓框风格|
|返回值：|无|







　Type参数可以取以下值：

　GTK_SHADOW_NONE

　GTK_SHADOW_IN

　GTK_SHADOW_OUT

　GTK_SHADOW_ETCHED_IN (缺省值)

　GTK_SHADOW_ETCHED_OUT



二、比例框架构件

比例框架构件(GtkAspectFrame) 和框架构件差不多，差别在于它会保持子构件的长宽比例，如果需要，还会在构件中增加额外的可用空间。

创建比例构件用函数gtk_aspect_frame_new。

5

|名称:：|gtk_aspect_frame_new|
|----|----|
|功能：|创建比例框架构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_aspect_frame_new( const gchar *label,gfloat xalign,gfloat yalign,gfloat ratio,gint obey_child);|
|参数：|label框架构件的标签xalign子构件在比例框架中的横向位置yalign子构件在比例框架中的纵向位置ratio比例框架的长宽比例obey_child|
|返回值：|新的比例框架构件|







xalign和yalign参数取值范围介于0.0和1.0之间。xalign指定标签在框架构件上部水平线上的位置。0.0为左对齐，1.0为右对齐。yalign指定标签在框架构件上部垂直线上的位置。

ratio为比例框架的长宽比例。

如果o b e y _ c h i l d参数设置为T R U E，子构件的长宽比例会和它所请求的理想长宽比例相匹配。否则，比例值由r a t i o参数指定。

我们将上面的程序例子中的：

grame=gtk_frame_new(“性别”); /*创建框架构件*/

换为：

grame= gtk_aspect_frame_new (“性别”,0.1,0.1,3,FALSE); /*创建比例框架构件*/

编译后得到如下构件：

![](http://blog.chinaunix.net/photo/22935_070612162233.png)


6、

|名称:：|gtk_frame_new|
|----|----|
|功能：|修改比例框架构件选项|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_aspect_frame_set( GtkAspectFrame *aspect_frame,gfloat xalign,gfloat yalign,gfloat ratio,gint obey_child);|
|参数：|aspect_frame比例构件xalign子构件在比例框架中的横向位置yalign子构件在比例框架中的纵向位置ratio比例框架的长宽比例obey_child|
|返回值：|无|

