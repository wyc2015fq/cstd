# GTK+图形化应用程序开发学习笔记（十三）—微调构件 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 23:22:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1678


**GTK+图形化应用程序开发学习笔记（十三）—微调构件**

微调按纽构件(GtkSpinButton)通常用于让用户从一个取值范围内选择一个值。它由一个文本输入框和旁边的向上和向下两个按钮组成。点击某一个按钮会让文本输入框内的数值大

小在一定范围内改变。文本输入框也可以直接进行编辑。

微调按钮构件允许其中的数值没有小数位或具有指定的小数位，并且数值可以按一种可

配置的方式增加或减小。在按钮较长时间呈按下状态时，构件的数值会根据工具按下时间的

长短加速变化。

微调按钮用一个调整对象来维护该按钮能够取值的范围。微调按钮构件因此而具有了很

强大的功能。

在创建微调按纽前需要先创建微调对象。函数gtk_adjustment_new可以创建微调对象。

1．

|名称:：|gtk_adjustment_new|
|----|----|
|功能：|创建微调对象|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkObject *gtk_adjustment_new(gflost value, gfloat lower, gflost upper, gfloat step_increment, gfloat page_increment, gfloat page_size);|
|参数：|value微调按钮构件的初值lower构件允许的最小值upper构件允许的最大值step_increment当鼠标左键按下时构件一次增加/减小的值page_increment当鼠标右键按下时构件一次增加/减小的值page_size没有用到|
|返回值：|微调对象|










创建好微调对象就可以创建微调按纽构件了，我们用gtk_spin_button_new函数来创建。

2．

|名称:：|gtk_spin_button_new|
|----|----|
|功能：|创建微调按纽构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_spin_button_new(GtkAdjustment *adjustment, gfloat clim_rate, guint digits);|
|参数：|adjustment微调对象clim_rate构件变化的加速度digits显示数值的小数位|
|返回值：|新的微调按纽构件|







其中clim_rate参数是介于0.0和1.0间的值，指明构件数值变化的加速度。参数digits指定要显示的值的小数位数。

下面是创建微调按纽的例子：

#include <gtk/gtk.h>

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *spin;

GtkWidget *box;

GtkWidget *label;

GtkObject *adjustment;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_hbox_new(FALSE,10); /*创建组装盒*/

gtk_container_add(GTK_CONTAINER(window),box); /*将组装盒加入窗体*/

adjustment=gtk_adjustment_new(2007,1900,2100,1,1,0); /*创建微调对象*/

spin=gtk_spin_button_new(GTK_ADJUSTMENT(adjustment),0.5,0); /*创建微调按纽*/

gtk_box_pack_start(GTK_BOX(box),spin,TRUE,TRUE,5); /*将微调按纽加入组装盒*/

gtk_widget_show(spin); 

label=gtk_label_new(“年”);

gtk_box_pack_start(GTK_BOX(box),label,TRUE,TRUE,0);

gtk_widget_show(label);

gtk_widget_show(box);

gtk_widget_show(window);

gtk_main();

}

程序运行后会得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070609224802.png)

3．

|名称:：|gtk_spin_button_configure|
|----|----|
|功能：|设置微调按纽构件|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void *gtk_spin_button_configure(GtkSpinButton *spin_button, GtkAdjustment *adjustment, gfloat climb_rate, guint digits);|
|参数：|spin_button微调按纽构件adjustment微调对象clim_rate构件变化的加速度digits显示数值的小数位|
|返回值：|无|








创建微调按纽构件之后，还可以用下面的函数对其重新配置，其中spin_button参数就是要重新配置的微调按纽构件。

4．

|名称:：|gtk_spin_button_set_adjustmentadjustment|
|----|----|
|功能：|设置/获取微调按纽构件使用的调整对象|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_spin_button_set_adjustment( GtkSpinButton *spin_button, GtkAdjustment *adjustment ); /*设置*/GtkAdjustment adjustment(GtkSpinButton *spin_button ); /*获得*/|
|参数：|spin_button微调按纽构件adjustment微调对象|
|返回值：|调整对象|









5．

|名称:：|gtk_spin_button_set_digits|
|----|----|
|功能：|修改显示数值的小数位|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_spin_button_set_digits( GtkSpinButton *spin_button,guint digits) ;|
|参数：|spin_button微调按纽构件digits显示数值的小数位|
|返回值：|无|






6．
|名称:：|gtk_spin_button_set_value|
|----|----|
|功能：|修改当前显示构件的数值|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_spin_button_set_value( GtkSpinButton *spin_button, gfloat value );|
|参数：|spin_button微调按纽构件value当前显示构件的数值|
|返回值：|无|







7．
|名称:：|gtk_spin_button_get_value_as_floatgtk_spin_button_get_value_as_int|
|----|----|
|功能：|获得微调按纽构件的当前值|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|gfloat gtk_spin_button_get_value_as_float( GtkSpinButton *spin_button );gint gtk_spin_button_get_value_as_int( GtkSpinButton *spin_button );|
|参数：|spin_button微调按纽构件|
|返回值：|微调按纽构件的当前值|






微调按纽构件的当前值可以以整数或浮点数的形式获得。

gfloat gtk_spin_button_get_value_as_float以浮点数的形式获得微调按纽构件的当前值。

gint gtk_spin_button_get_value_as_int以整型的形式获得微调按纽构件的当前值。

我们对上面的程序稍加改动得到如下程序：

#include <gtk/gtk.h>

GtkWidget *spin;

void button_event(GtkWidget *widget, gpointer *data);

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *box;

GtkWidget *label;

GtkObject *adjustment;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_hbox_new(FALSE,10); /*创建组装盒*/

gtk_container_add(GTK_CONTAINER(window),box); /*将组装盒加入窗体*/

adjustment=gtk_adjustment_new(2007,1900,2100,1,1,0); /*创建微调对象*/

spin=gtk_spin_button_new(GTK_ADJUSTMENT(adjustment),0.5,0); /*创建微调按纽*/

gtk_box_pack_start(GTK_BOX(box),spin,TRUE,TRUE,5); /*将微调按纽加入组装盒*/

gtk_widget_show(spin); 

label=gtk_label_new(“年”);

gtk_box_pack_start(GTK_BOX(box),label,TRUE,TRUE,0);

gtk_widget_show(label);

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

gint year;

year=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin));/*获得微调按纽的当前值*/

g_print(“Year:%d”,year);

}

程序运行后会得到如下界面：

![](http://blog.chinaunix.net/photo/22935_070609224810.png)

当用户按“确定”按纽时相应的年份就会输出到屏幕上。



8．
|名称:：|gtk_spin_button_set_wrap|
|----|----|
|功能：|限制微调按纽构件的文本输入|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_spin_button_set_wrap( GtkSpinButton *spin_button,gboolean wrap );|
|参数：|spin_button微调按纽构件wrap|
|返回值：|无|





gtk_spin_button_set_wrap函数设置是否让微调按纽构件在upper和lower 之间循环。也就是当达到最大值后再向上调整回到最小值，当达到最小值后再向下调整变为最大值。


