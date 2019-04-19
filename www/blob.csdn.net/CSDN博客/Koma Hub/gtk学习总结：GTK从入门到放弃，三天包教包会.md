# gtk学习总结：GTK从入门到放弃，三天包教包会 - Koma Hub - CSDN博客
2019年01月05日 18:32:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：688
今天花了一些时间复习之前学习过的gtk，感觉东西光学不用忘得好快啊，在这做个之前学过的内容的备份，希望对大家有用，也希望大家不要从入门到放弃。文中有些代码例子可能来自《GTK从入门到精通》吧，我不记得了，或者叫《GTK中文教程》，忘记了，总之，这是一个学习笔记，希望不触及任何人的利益，起到共享学习之用。
**目录**
[编译方法](#%E7%BC%96%E8%AF%91%E6%96%B9%E6%B3%95)
[编译脚本](#%E7%BC%96%E8%AF%91%E8%84%9A%E6%9C%AC)
[01.一个简单的按钮](#01.%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E6%8C%89%E9%92%AE)
[02.两个按钮](#02.%E4%B8%A4%E4%B8%AA%E6%8C%89%E9%92%AE)
[03.vbox：一张图片+一个按钮](#03.vbox%EF%BC%9A%E4%B8%80%E5%BC%A0%E5%9B%BE%E7%89%87%2B%E4%B8%80%E4%B8%AA%E6%8C%89%E9%92%AE)
[04.一个简单的提示窗口：下一步+上一步+确定](#04.%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E6%8F%90%E7%A4%BA%E7%AA%97%E5%8F%A3%EF%BC%9A%E4%B8%8B%E4%B8%80%E6%AD%A5%2B%E4%B8%8A%E4%B8%80%E6%AD%A5%2B%E7%A1%AE%E5%AE%9A)
[05.多按钮：一个简单的键盘](#05.%E5%A4%9A%E6%8C%89%E9%92%AE%EF%BC%9A%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E9%94%AE%E7%9B%98)
[06.一个简单的画图draw界面](#06.%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E7%94%BB%E5%9B%BEdraw%E7%95%8C%E9%9D%A2)
[07.带图片的按钮](#07.%E5%B8%A6%E5%9B%BE%E7%89%87%E7%9A%84%E6%8C%89%E9%92%AE)
[08.多选按钮check button](#08.%E5%A4%9A%E9%80%89%E6%8C%89%E9%92%AEcheck%20button)
[09.单选按钮radio button](#09.%E5%8D%95%E9%80%89%E6%8C%89%E9%92%AEradio%20button)
[10.控制条scrollbar](#10.%E6%8E%A7%E5%88%B6%E6%9D%A1scrollbar)
[11.箭头按钮create_arrow_button](#11.%E7%AE%AD%E5%A4%B4%E6%8C%89%E9%92%AEcreate_arrow_button)
[12.标签label](#12.%E6%A0%87%E7%AD%BElabel)
[12.子窗口：触发事件打开窗口](#12.%E5%AD%90%E7%AA%97%E5%8F%A3%EF%BC%9A%E8%A7%A6%E5%8F%91%E4%BA%8B%E4%BB%B6%E6%89%93%E5%BC%80%E7%AA%97%E5%8F%A3)
[13.进度条progressbar](#13.%E8%BF%9B%E5%BA%A6%E6%9D%A1progressbar)
[14.窗口标尺gtk_hruler_new](#14.%E7%AA%97%E5%8F%A3%E6%A0%87%E5%B0%BAgtk_hruler_new)
[15.状态栏（条）gtk_statusbar_new](#15.%E7%8A%B6%E6%80%81%E6%A0%8F%EF%BC%88%E6%9D%A1%EF%BC%89gtk_statusbar_new)
[另一个例子](#%E5%8F%A6%E4%B8%80%E4%B8%AA%E4%BE%8B%E5%AD%90)
[16.文本输入gtk_entry_new：可见/不可见，可输入/不可输入](#16.%E6%96%87%E6%9C%AC%E8%BE%93%E5%85%A5gtk_entry_new%EF%BC%9A%E5%8F%AF%E8%A7%81%2F%E4%B8%8D%E5%8F%AF%E8%A7%81%EF%BC%8C%E5%8F%AF%E8%BE%93%E5%85%A5%2F%E4%B8%8D%E5%8F%AF%E8%BE%93%E5%85%A5)
[规定最大长度的gtk_entry_new_with_max_length](#%E8%A7%84%E5%AE%9A%E6%9C%80%E5%A4%A7%E9%95%BF%E5%BA%A6%E7%9A%84gtk_entry_new_with_max_length)
[输入文本执行一条命令](#%E8%BE%93%E5%85%A5%E6%96%87%E6%9C%AC%E6%89%A7%E8%A1%8C%E4%B8%80%E6%9D%A1%E5%91%BD%E4%BB%A4)
[17.文件选择：gtk_file_selection_new](#17.%E6%96%87%E4%BB%B6%E9%80%89%E6%8B%A9%EF%BC%9Agtk_file_selection_new)
[18.事件盒子gtk_event_box_new](#18.%E4%BA%8B%E4%BB%B6%E7%9B%92%E5%AD%90gtk_event_box_new)
[19.固定容器：gtk_fixed_new](#19.%E5%9B%BA%E5%AE%9A%E5%AE%B9%E5%99%A8%EF%BC%9Agtk_fixed_new)
[20.一个简单的frame：gtk_frame_new（不同于java的JFrame）](#20.%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84frame%EF%BC%9Agtk_frame_new%EF%BC%88%E4%B8%8D%E5%90%8C%E4%BA%8Ejava%E7%9A%84JFrame%EF%BC%89)
[gtk_aspect_frame_new：更多选项的frame](#gtk_aspect_frame_new%EF%BC%9A%E6%9B%B4%E5%A4%9A%E9%80%89%E9%A1%B9%E7%9A%84frame)
[21.滚动窗口：gtk_scrolled_window_new](#21.%E6%BB%9A%E5%8A%A8%E7%AA%97%E5%8F%A3%EF%BC%9Agtk_scrolled_window_new)
[又一个例子：滚动窗口中放按钮](#%E5%8F%88%E4%B8%80%E4%B8%AA%E4%BE%8B%E5%AD%90%EF%BC%9A%E6%BB%9A%E5%8A%A8%E7%AA%97%E5%8F%A3%E4%B8%AD%E6%94%BE%E6%8C%89%E9%92%AE)
[22.按钮盒子：button box](#22.%E6%8C%89%E9%92%AE%E7%9B%92%E5%AD%90%EF%BC%9Abutton%20box)
[显示动态图：直接引用GIF，用多张图片显示动图效果](#%E6%98%BE%E7%A4%BA%E5%8A%A8%E6%80%81%E5%9B%BE%EF%BC%9A%E7%9B%B4%E6%8E%A5%E5%BC%95%E7%94%A8GIF%EF%BC%8C%E7%94%A8%E5%A4%9A%E5%BC%A0%E5%9B%BE%E7%89%87%E6%98%BE%E7%A4%BA%E5%8A%A8%E5%9B%BE%E6%95%88%E6%9E%9C)
[24.画图接口：gdk_draw_arc，gdk_draw_rectangle](#24.%E7%94%BB%E5%9B%BE%E6%8E%A5%E5%8F%A3%EF%BC%9Agdk_draw_arc%EF%BC%8Cgdk_draw_rectangle)
[25.文本文件编辑gtk_text_view_new](#25.%E6%96%87%E6%9C%AC%E6%96%87%E4%BB%B6%E7%BC%96%E8%BE%91gtk_text_view_new)
[26.菜单/工具栏：gtk_menu_new](#26.%E8%8F%9C%E5%8D%95%2F%E5%B7%A5%E5%85%B7%E6%A0%8F%EF%BC%9Agtk_menu_new)
[27.工具栏/条：gtk_toolbar_new](#27.%E5%B7%A5%E5%85%B7%E6%A0%8F%2F%E6%9D%A1%EF%BC%9Agtk_toolbar_new)
[28.浮动的工具条的菜单](#28.%E6%B5%AE%E5%8A%A8%E7%9A%84%E5%B7%A5%E5%85%B7%E6%9D%A1%E7%9A%84%E8%8F%9C%E5%8D%95)
[29.一个文本编辑器：包含menu，toolbar，statusbar](#29.%E4%B8%80%E4%B8%AA%E6%96%87%E6%9C%AC%E7%BC%96%E8%BE%91%E5%99%A8%EF%BC%9A%E5%8C%85%E5%90%ABmenu%EF%BC%8Ctoolbar%EF%BC%8Cstatusbar)
[30.模拟90坦克游戏：在frame中移动一个图片，键盘控制走向](#30.%E6%A8%A1%E6%8B%9F90%E5%9D%A6%E5%85%8B%E6%B8%B8%E6%88%8F%EF%BC%9A%E5%9C%A8frame%E4%B8%AD%E7%A7%BB%E5%8A%A8%E4%B8%80%E4%B8%AA%E5%9B%BE%E7%89%87%EF%BC%8C%E9%94%AE%E7%9B%98%E6%8E%A7%E5%88%B6%E8%B5%B0%E5%90%91)
[31.pixmap：](#31.pixmap%EF%BC%9A)
[32.固定调节数值的按钮:gtk_spin_button_new_with_range](#32.%E5%9B%BA%E5%AE%9A%E8%B0%83%E8%8A%82%E6%95%B0%E5%80%BC%E7%9A%84%E6%8C%89%E9%92%AE%3Agtk_spin_button_new_with_range)
[33.分隔面板：gtk_hpanel_new，gtk_vpanel_new（未编译）](#33.%E5%88%86%E9%9A%94%E9%9D%A2%E6%9D%BF%EF%BC%9Agtk_hpanel_new%EF%BC%8Cgtk_vpanel_new%EF%BC%88%E6%9C%AA%E7%BC%96%E8%AF%91%EF%BC%89)
[34.会话窗口：gtk_button_new_from_stock](#34.%E4%BC%9A%E8%AF%9D%E7%AA%97%E5%8F%A3%EF%BC%9Agtk_button_new_from_stock)
[35.字体选择：gtk_font_selection_dialog_new](#35.%E5%AD%97%E4%BD%93%E9%80%89%E6%8B%A9%EF%BC%9Agtk_font_selection_dialog_new)
[36.几个画图的骚操作：gtk_drawing_area_new](#36.%E5%87%A0%E4%B8%AA%E7%94%BB%E5%9B%BE%E7%9A%84%E9%AA%9A%E6%93%8D%E4%BD%9C%EF%BC%9Agtk_drawing_area_new)
[先给出三个例子的效果：](#%E5%85%88%E7%BB%99%E5%87%BA%E4%B8%89%E4%B8%AA%E4%BE%8B%E5%AD%90%E7%9A%84%E6%95%88%E6%9E%9C%EF%BC%9A)
[代码1效果](#%E4%BB%A3%E7%A0%811%E6%95%88%E6%9E%9C)
[代码2效果](#%E4%BB%A3%E7%A0%812%E6%95%88%E6%9E%9C)
[代码3效果](#%E4%BB%A3%E7%A0%813%E6%95%88%E6%9E%9C)
[代码1](#%E4%BB%A3%E7%A0%811)
[代码2](#%E4%BB%A3%E7%A0%812)
[代码3](#%E4%BB%A3%E7%A0%813)
[最后寄语](#%E6%9C%80%E5%90%8E%E5%AF%84%E8%AF%AD)
# 编译方法
`$ gcc main.c `pkg-config --libs --cflags gtk+-3.0``
或者使用gtk-2.0。
## 编译脚本
```bash
#!/bin/bash
if [ $# -lt 1 ]; then
    echo "usage: $0 [main.c]"
    exit
fi
gcc -o a $1 `pkg-config --libs --cflags gtk+-2.0`
```
# 01.一个简单的按钮
```cpp
#include <gtk/gtk.h>
//#include "temp.c"
/* 这是一个回调函数。data 参数在本示例中被忽略。
 * 后面有更多的回调函数示例。*/
void hello( GtkWidget *widget, gpointer   data )
{
    g_print ("Hello World\n");//temp(data );
}
gint delete_event( GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
    /* 如果你的 "delete_event" 信号处理函数返回 FALSE，GTK 会发出 "destroy" 信号。
     * 返回 TRUE，你不希望关闭窗口。
     * 当你想弹出“你确定要退出吗?”对话框时它很有用。*/
    g_print ("delete event occurred\n");
    /* 改 TRUE 为 FALSE 程序会关闭。*/
    return FALSE;
}
/* 另一个回调函数 */
void destroy( GtkWidget *widget, gpointer   data )
{
    gtk_main_quit ();
}
int main( int   argc, char *argv[] )
{
    /* GtkWidget 是构件的存储类型 */
    GtkWidget *window;
    GtkWidget *button;
    
    /* 这个函数在所有的 GTK 程序都要调用。参数由命令行中解析出来并且送到该程序中*/
    gtk_init (&argc, &argv);
    
    /* 创建一个新窗口 */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    /* 当窗口收到 "delete_event" 信号 (这个信号由窗口管理器发出，通常是“关闭”
     * 选项或是标题栏上的关闭按钮发出的)，我们让它调用在前面定义的 delete_event() 函数。
     * 传给回调函数的 data 参数值是 NULL，它会被回调函数忽略。*/
    g_signal_connect (G_OBJECT (window), "delete_event",
        G_CALLBACK (delete_event), NULL);
    
    /* 在这里我们连接 "destroy" 事件到一个信号处理函数。  
     * 对这个窗口调用 gtk_widget_destroy() 函数或在 "delete_event" 回调函数中返回 FALSE 值
     * 都会触发这个事件。*/
    g_signal_connect (G_OBJECT (window), "destroy",
        G_CALLBACK (destroy), NULL);
    
    /* 设置窗口边框的宽度。*/
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
    /* 创建一个标签为 "Hello World" 的新按钮。*/
    button = gtk_button_new_with_label ("Hello World");
    
    /* 当按钮收到 "clicked" 信号时会调用 hello() 函数，并将NULL传给
     * 它作为参数。hello() 函数在前面定义了。*/
    g_signal_connect (G_OBJECT (button), "clicked",
        G_CALLBACK (hello), "1");
    
    /* 当点击按钮时，会通过调用 gtk_widget_destroy(window) 来关闭窗口。
     * "destroy" 信号会从这里或从窗口管理器发出。*/
   // g_signal_connect_swapped (G_OBJECT (button), "clicked",
   //      G_CALLBACK (gtk_widget_destroy),window);
    
    /* 把按钮放入窗口 (一个 gtk 容器) 中。*/
    gtk_container_add (GTK_CONTAINER (window), button);
    
    /* 最后一步是显示新创建的按钮和窗口 */
    gtk_widget_show (button);
    gtk_widget_show (window);
    
    /* 所有的 GTK 程序必须有一个 gtk_main() 函数。程序运行停在这里
     * 等待事件 (如键盘事件或鼠标事件) 的发生。*/
    gtk_main ();
    
    return 0;
}
```
# 02.两个按钮
```cpp
#include <gtk/gtk.h>
/* 我们新改进的回调函数。传递到该函数的数据将打印到标准输出(stdout)。*/
void callback( GtkWidget *widget, gpointer data )
{
       g_print ("%s ", (gchar *) data);
}
/* 另一个回调函数 */
gint delete_event( GtkWidget *widget, GdkEvent *event, gpointer data )
{
     gtk_main_quit ();
     return FALSE;
}
int main( int argc, char *argv[] )
{
/* GtkWidget 是构件的存储类型 */
      GtkWidget *window;
      GtkWidget *button;
      GtkWidget *box1;
      gtk_init (&argc, &argv);
/* 创建一个新窗口 */
      window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
/* 这是一个新的调用,设置窗口标题为"Hello Buttons!" */
      gtk_window_set_title (GTK_WINDOW (window), "Hello Buttons!");
/* 在这里我们为 delete_event 设置了一个处理函数来立即退出 GTK。*/
      g_signal_connect (G_OBJECT (window), "delete_event",
                        G_CALLBACK (delete_event), NULL);
/* 设置窗口边框的宽度。 */
      gtk_container_set_border_width (GTK_CONTAINER (window), 20);
/* 我们创建了一个组装盒。详情参见“组装”章节。我们看不见组装盒,它仅被作为排列构件的工具。*/
      box1 = gtk_hbox_new (FALSE, 1);
/* 把组装盒放入主窗口中。*/
      gtk_container_add (GTK_CONTAINER (window), box1);
/* 创建一个标签为 "Button 1" 的新按钮。*/
      button = gtk_button_new_with_label ("Hello");
/* 当按钮被按下时,我们调用 "callback" 函数,并将一个指向 "button 1" 的指针作为它的参数。*/
      g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), "hello");
/* 代替 gtk_container_add,我们把按钮放入不可见的组装盒,该组合盒已经组装进窗口中了。*/
      gtk_box_pack_start (GTK_BOX(box1), button, TRUE, TRUE, 0);
/* 总是记住这一步,它告诉 GTK 我们为这个按钮做的准备工作已经完成了,现在可以显示它了。*/
      gtk_widget_show (button);
/* 同样创建第二个按钮。*/
      button = gtk_button_new_with_label ("World");
      g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), "World");
      gtk_box_pack_start(GTK_BOX (box1), button, TRUE, TRUE, 0);
      gtk_widget_show (button);
/* 同样创建第3个按钮。*/
      button = gtk_button_new_with_label ("!");
      g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), "!");
      gtk_box_pack_start(GTK_BOX (box1), button, TRUE, TRUE, 0);
      gtk_widget_show (button);
      gtk_widget_show (box1);
      gtk_widget_show (window);
/* 停在这里等待事件发生。*/
      gtk_main ();
      return 0;
}
```
# 03.vbox：一张图片+一个按钮
```cpp
/* 创建SPLASH窗口 splash.c */
#include <gtk/gtk.h>
//主函数
int main (int argc,char* argv[])
{
   GtkWidget* window ;
   GtkWidget* vbox ;
   GtkWidget* image ;
   GtkWidget* button ;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_POPUP);//////////no boundary
   gtk_window_set_title(GTK_WINDOW(window),"Splash窗口");
   g_signal_connect(G_OBJECT(window),"destroy",
      G_CALLBACK(gtk_main_quit),NULL);
   //gtk_container_set_border_width(GTK_CONTAINER(window),20);
   //gtk_window_set_default_size(GTK_WINDOW(window),500,400);
   //gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
   vbox = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);
   image = gtk_image_new_from_file("b.png");
   gtk_box_pack_start(GTK_BOX(vbox),image,FALSE,FALSE,0);
   button = gtk_button_new_with_label("Splash窗口");
   g_signal_connect(G_OBJECT(button),"clicked",
      G_CALLBACK(gtk_main_quit),NULL);
   gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,0);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
# 04.一个简单的提示窗口：下一步+上一步+确定
```cpp
/* 每日提示 tips.c */
#include <gtk/gtk.h>
//XPM格式的图像数据，以C语言源代码形式存于文件中
static char * book_open_xpm[] = {
   "16 16 4 1",
   " c None s None",
   ". c black",
   "X c #808080",
   "o c white",
   " ",
   " .. ",
   " .Xo. ... ",
   " .Xoo. ..oo. ",
   " .Xooo.Xooo... ",
   " .Xooo.oooo.X. ",
   " .Xooo.Xooo.X. ",
   " .Xooo.oooo.X. ",
   " .Xooo.Xooo.X. ",
   " .Xooo.oooo.X. ",
   " .Xoo.Xoo..X. ",
   " .Xo.o..ooX. ",
   " .X..XXXXX. ",
   " ..X....... ",
   " .. ",
   " "};
//自定义提示信息
static gchar *info[5] = {
   "此软件用于测试每日提示功能的实现，如果你发现问题请及时回复。",
   "我们的目的是把GTK+2.0的大多数功能奉献给每一位自由软件爱好者和开发者。",
   "每一位Linux的支持者都会让我们增加一分信心，Linux最终仍是台式计算机操作系统。",
   "计算机软件技术是一种科学技术，它和人类历史上其他的科学技术一样，是允许每一人自由使用的。",
   "当前你测试完此程序后，请设法把它附加到你创作的软件当中去，这是你成功的第一步。"
};
static GtkWidget *window; //主窗口
static GtkWidget *frame; //框架
static GtkWidget *pre_button; //上一提示按钮
static GtkWidget *next_button; //下一提示按钮
static GtkWidget *label; //提示信息内容标签
static GtkWidget *title; //框架的标题
gint current_info = 0 ;//当前提示信息计数
GtkWidget* create_title (GtkWidget *data)
{ //创建框架控件的标题
   GtkWindow *title;
   GtkWidget *hbox;
   GtkWidget *image;
   GtkWidget *label;
   GdkPixmap *pixmap;
   GdkBitmap *mask;
   pixmap = gdk_pixmap_create_from_xpm_d(
                  data->window,&mask,
                  >K_WIDGET(data)->style->white,
                  book_open_xpm);
   image = gtk_image_new_from_pixmap(pixmap,NULL);
   label = gtk_label_new("新的标题");
   hbox = gtk_hbox_new(FALSE,0);
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,2);
   gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,2);
   return hbox ;
}
GtkWidget* create_button (gchar* stockid,gchar* title)
{ //创建带图像的按钮
   GtkWidget *button;
   GtkWidget *image;
   GtkWidget *label;
   GtkWidget *hbox;
   image = gtk_image_new_from_stock(stockid,GTK_ICON_SIZE_MENU);
   label = gtk_label_new(title);
   hbox = gtk_hbox_new(FALSE,0);
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,3);
   gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,3);
   button = gtk_button_new();
   gtk_container_add(GTK_CONTAINER(button),hbox);
   return button;
}
void pre_info (GtkButton *button,gpointer data)
{ //上一提示
   gint i ;
   i = current_info - 1 ;
   if(i == -1) return ;
   if(i == 0 ) gtk_widget_set_sensitive(pre_button,FALSE);
   current_info = i ;
   gtk_widget_set_sensitive(next_button,TRUE);
   gtk_label_set_text(GTK_LABEL(label),info[current_info]);
}
void next_info (GtkButton *button,gpointer data)
{ //下一提示
   gint i ;
   i = current_info + 1 ;
   if(i == 5) return ;
   if(i == 4) gtk_widget_set_sensitive(next_button,FALSE);
   current_info = i ;
   gtk_widget_set_sensitive(pre_button,TRUE);
   gtk_label_set_text(GTK_LABEL(label),info[current_info]);
}
int main ( int argc , char* argv[])
{
   GtkWidget *hbox ;
   GtkWidget *vbox;
   GtkWidget *bbox;
   GtkWidget *button;
   GtkWidget *image;
   GtkWidget *title;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   g_signal_connect(G_OBJECT(window),"delete_event",
         G_CALLBACK(gtk_main_quit),NULL);
   gtk_window_set_title(GTK_WINDOW(window),"每日提示");
   gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window),10);
   gtk_widget_realize(window);
   vbox = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);
   hbox = gtk_hbox_new(FALSE,0);
   gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,5);
   image = gtk_image_new_from_file("l.bmp");
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
   frame = gtk_frame_new(NULL);
   title = create_title(window);
   gtk_frame_set_label_widget(GTK_FRAME(frame),title);
   gtk_box_pack_start(GTK_BOX(hbox),frame,TRUE,TRUE,5);
   label = gtk_label_new(NULL);
   gtk_label_set_text(GTK_LABEL(label),info[0]);
   gtk_label_set_line_wrap(GTK_LABEL(label),TRUE);
   gtk_container_add(GTK_CONTAINER(frame),label);
   bbox = gtk_hbutton_box_new();
   gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_END);
   gtk_box_set_spacing(GTK_BOX(bbox),5);
   gtk_box_pack_start(GTK_BOX(vbox),bbox,FALSE,FALSE,5);
   button = gtk_check_button_new_with_label("每次启动时显示");
   gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,5);
   pre_button = create_button(GTK_STOCK_GO_BACK,"上一提示");
   gtk_widget_set_sensitive(pre_button,FALSE);
   g_signal_connect(G_OBJECT(pre_button),"clicked",
         G_CALLBACK(pre_info),NULL);
   gtk_box_pack_start(GTK_BOX(bbox),pre_button,FALSE,FALSE,5);
   next_button = create_button(GTK_STOCK_GO_FORWARD,"下一提示");
   g_signal_connect(G_OBJECT(next_button),"clicked",
         G_CALLBACK(next_info),NULL);
   gtk_box_pack_start(GTK_BOX(bbox),next_button,FALSE,FALSE,5);
   button = gtk_button_new_from_stock(GTK_STOCK_OK);
   g_signal_connect(G_OBJECT(button),"clicked",
         G_CALLBACK(gtk_main_quit),NULL);
   gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,5);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
效果：
![](https://img-blog.csdnimg.cn/20190105161921461.PNG)
# 05.多按钮：一个简单的键盘
```cpp
#include <gtk/gtk.h>
/* 我们的回调。
* 传到这个函数的数据被打印到标准输出 */
void callback( GtkWidget *widget, gpointer data )
{
    g_print ("%s", (char *) data);
}
/* 这个回调退出程序 */
gint delete_event( GtkWidget *widget, GdkEvent *event, gpointer data )
{
    gtk_main_quit ();
    return FALSE;
}
int main( int argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *table;
    gtk_init (&argc, &argv);
/* 创建一个新窗口 */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
/* 设置窗口标题 */
    gtk_window_set_title (GTK_WINDOW (window), "Rong Tao's Keyboard");
/* 为 delete_event 设置一个立即退出 GTK 的处理函数。 */
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
/* 设置窗口的边框宽度。 */
    gtk_container_set_border_width (GTK_CONTAINER (window), 20);
/* 创建一个 2x2 的表 */
    table = gtk_table_new (4, 20, TRUE);
/* 将表放进主窗口 */
    gtk_container_add (GTK_CONTAINER (window), table);
/* 创建第一个按钮 */
    button = gtk_button_new_with_label ("Q");
/* 当这个按钮被点击时,我们调用 "callback" 函数,并将一个指向"button 1"的指针作为它的参数 */
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "q");
/* 将 button 1 插入表的左上象限(quadrant) */
    gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 2, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("W");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "w");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 2, 4, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("E");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "e");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 4, 6, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("R");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "r");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 6, 8, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("T");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "t");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 8, 10, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("Y");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "y");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 10, 12, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("U");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "u");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 12, 14, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("I");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "i");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 14, 16, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("O");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "o");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 16, 18, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("P");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "p");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 18, 20, 0, 1);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("A");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "a");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 3, 1, 2);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("S");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "s");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 3, 5, 1, 2);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("D");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "d");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 5, 7, 1, 2);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("F");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "f");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 7, 9, 1, 2);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("G");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "g");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 9, 11, 1, 2);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("H");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "h");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 11, 13, 1, 2);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("J");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "j");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 13, 15, 1, 2);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("K");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "k");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 15, 17, 1, 2);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("L");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "l");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 17, 19, 1, 2);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("'");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "'");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 2, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("Z");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "z");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 2, 4, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("X");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "x");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 4, 6, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("C");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "c");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 6, 8, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("V");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "v");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 8, 10, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("B");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "b");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 10, 12, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("N");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "n");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 12, 14, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("M");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "m");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 14, 16, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label (",");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) ",");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 16, 18, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label (".");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) ".");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 18, 20, 2, 3);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label ("Enter");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) "\n");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 2, 6, 3, 4);
    gtk_widget_show (button);
/*********************/
    button = gtk_button_new_with_label (" ");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (callback), (gpointer) " ");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 6, 16, 3, 4);
    gtk_widget_show (button);
/* 创建"Quit"按钮 */
    button = gtk_button_new_with_label ("Quit");
/* 当这个按钮被点击时,我们调用 "delete_event" 函数接着程序就退出了 */
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (delete_event), NULL);
/* 将退出按钮插入表的下面两个象限 */
    gtk_table_attach_defaults (GTK_TABLE (table), button, 16, 19, 3, 4);
    gtk_widget_show (button);
    gtk_widget_show (table);
    gtk_widget_show (window);
    gtk_main ();
    return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105162128295.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 06.一个简单的画图draw界面
```cpp
/* 绘图软件 brush.c */
#include <gtk/gtk.h>
static GtkWidget *window = NULL;
static GdkPixmap *pixmap = NULL;
static gboolean
scribble_configure_event (GtkWidget *widget,GdkEventConfigure *event,gpointer data)
{
   if (pixmap)
   g_object_unref (G_OBJECT (pixmap));
   pixmap = gdk_pixmap_new (widget->window,
   widget->allocation.width,
   widget->allocation.height,-1);
   gdk_draw_rectangle (pixmap,widget->style->white_gc,
                        TRUE,0,0,widget->allocation.width,
                        widget->allocation.height);
   return TRUE;
}
static gboolean scribble_expose_event (GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
   gdk_draw_drawable (widget->window,
                        widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                        pixmap,
                        event->area.x, event->area.y,
                        event->area.x, event->area.y,
                        event->area.width, event->area.height);
   return FALSE;
}
static void draw_brush (GtkWidget *widget, gdouble x, gdouble y)
{
   GdkRectangle update_rect;
   update_rect.x = x - 3;
   update_rect.y = y - 3;
   update_rect.width = 6;
   update_rect.height = 6;
   gdk_draw_rectangle (pixmap,
                        widget->style->black_gc,
                        TRUE,
                        update_rect.x, update_rect.y,
                        update_rect.width, update_rect.height);
                        gdk_window_invalidate_rect (widget->window,
                        &update_rect,
                        FALSE);
}
static gboolean scribble_button_press_event (GtkWidget *widget,GdkEventButton *event,gpointer data)
{
   if (pixmap == NULL)
      return FALSE;
   if (event->button == 1)
      draw_brush (widget, event->x, event->y);
   return TRUE;
}
static gboolean scribble_motion_notify_event (GtkWidget *widget,GdkEventMotion *event,gpointer data)
{
   int x, y;
   GdkModifierType state;
   if (pixmap == NULL)
      return FALSE;
   gdk_window_get_pointer (event->window, &x, &y, &state);
   if (state & GDK_BUTTON1_MASK)
      draw_brush (widget, x, y);
   return TRUE;
}
static gboolean checkerboard_expose (GtkWidget *da, GdkEventExpose *event,gpointer data)
{
   gint i, j, xcount, ycount;
   GdkGC *gc1, *gc2;
   GdkColor color;
#define CHECK_SIZE 10
#define SPACING 2
   gc1 = gdk_gc_new (da->window);
   color.red = 30000;
   color.green = 0;
   color.blue = 30000;
   gdk_gc_set_rgb_fg_color (gc1, &color);
   gc2 = gdk_gc_new (da->window);
   color.red = 65535;
   color.green = 65535;
   color.blue = 65535;
   gdk_gc_set_rgb_fg_color (gc2, &color);
   xcount = 0;
   i = SPACING;
   while (i < da->allocation.width)
   {
      j = SPACING;
      ycount = xcount % 2;
      while (j < da->allocation.height)
      {
         GdkGC *gc;
         if (ycount % 2)
            gc = gc1;
         else
            gc = gc2;
         gdk_draw_rectangle (da->window, gc, TRUE,
                              i, j, CHECK_SIZE, CHECK_SIZE);
         j += CHECK_SIZE + SPACING;
         ++ycount;
      }
      i += CHECK_SIZE + SPACING;
      ++xcount;
   }
   g_object_unref (G_OBJECT (gc1));
   g_object_unref (G_OBJECT (gc2));
   return TRUE;
}
int main (int argc,char* argv[])
{
   GtkWidget *frame;
   GtkWidget *vbox;
   GtkWidget *da;
   GtkWidget *label;
   gtk_init(&argc,&argv);
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "绘图软件");
   g_signal_connect (G_OBJECT(window), "delete_event",
         G_CALLBACK (gtk_main_quit), NULL);
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
   vbox = gtk_vbox_new (FALSE, 8);
   gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
   gtk_container_add (GTK_CONTAINER (window), vbox);
   label = gtk_label_new (NULL);
   gtk_label_set_markup (GTK_LABEL (label),"<u>绘图区域</u>");
   gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
   frame = gtk_frame_new (NULL);
   gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
   gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);
   da = gtk_drawing_area_new ();
   gtk_widget_set_size_request (da, 100, 100);
   gtk_container_add (GTK_CONTAINER (frame), da);
   g_signal_connect (da, "expose_event",
         G_CALLBACK (scribble_expose_event), NULL);
   g_signal_connect (da, "configure_event",
         G_CALLBACK (scribble_configure_event), NULL);
   g_signal_connect (da, "motion_notify_event",
         G_CALLBACK (scribble_motion_notify_event), NULL);
   g_signal_connect (da, "button_press_event",
         G_CALLBACK (scribble_button_press_event), NULL);
   gtk_widget_set_events (da, gtk_widget_get_events (da)
                        | GDK_LEAVE_NOTIFY_MASK
                        | GDK_BUTTON_PRESS_MASK
                        | GDK_POINTER_MOTION_MASK
                        | GDK_POINTER_MOTION_HINT_MASK);
   gtk_widget_show_all (window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/20190105162545885.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 07.带图片的按钮
```cpp
/* 带图像的按钮 button.c */
#include <gtk/gtk.h>
void on_button_clicked (GtkWidget* button,gpointer data)
{
   //g_print("按钮 %s ",(gchar*)data);
   g_print("Button %s is pressed.\n",(gchar*)data);
   //g_print("被按了一下． \n");
}
//创建自己按钮的函数
GtkWidget* create_button1 (void)
{
   GtkWidget* box;
   GtkWidget* image;
   GtkWidget* label;
   GtkWidget* button;
   char* title = "文件家";
   image = gtk_image_new_from_file("dir2.jpg");
   label = gtk_label_new(title);
   box = gtk_vbox_new(FALSE,2);
   gtk_container_set_border_width(GTK_CONTAINER(box),5);
   gtk_box_pack_start(GTK_BOX(box),image,FALSE,FALSE,3);
   gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,3);
   gtk_widget_show(image);
   gtk_widget_show(label);
   button = gtk_button_new();
   gtk_container_add(GTK_CONTAINER(button),box);
   gtk_widget_show(box);
   return button ;
}
GtkWidget* create_button2 (void)
{
   GtkWidget* box;
   GtkWidget* image;
   GtkWidget* label;
   GtkWidget* button;
   char* title = "蝙蝠侠";
   image = gtk_image_new_from_file("batman.jpg");
   label = gtk_label_new(title);
   box = gtk_hbox_new(FALSE,2);
   gtk_container_set_border_width(GTK_CONTAINER(box),5);
   gtk_box_pack_start(GTK_BOX(box),image,FALSE,FALSE,3);
   gtk_box_pack_start(GTK_BOX(box),label,FALSE,FALSE,3);
   gtk_widget_show(image);
   gtk_widget_show(label);
   button = gtk_button_new();
   gtk_container_add(GTK_CONTAINER(button),box);
   gtk_widget_show(box);
   return button ;
}
//主函数
int main (int argc, char *argv[])
{
   GtkWidget* window;
   GtkWidget* box;
   GtkWidget* button1;
   GtkWidget* button2;
   GtkWidget* button3;
   GtkWidget* button4;
   gchar* title = "带图像和快捷键的按钮";
   gchar* b1 = "Red apple";//"红苹果"
   gchar* b2 = "Small mushroom";//"小蘑菇"
   gchar* b3 = "Short key";//"快捷键"
   gchar* b4 = "OK";//"确认"
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window),title);
   gtk_container_set_border_width(GTK_CONTAINER(window),20);
   g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
   box = gtk_hbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),box);
   button1 = create_button1();
   g_signal_connect(G_OBJECT(button1),"clicked",
   G_CALLBACK(on_button_clicked),(gpointer)b1);
   gtk_box_pack_start(GTK_BOX(box),button1,FALSE,FALSE,5);
   button2 = create_button2();
   g_signal_connect(G_OBJECT(button2),"clicked",
   G_CALLBACK(on_button_clicked),(gpointer)b2);
   gtk_box_pack_start(GTK_BOX(box),button2,FALSE,FALSE,5);
   button3 = gtk_button_new_with_mnemonic("快捷键(_H)");
   g_signal_connect(G_OBJECT(button3),"clicked",
   G_CALLBACK(on_button_clicked),(gpointer)b3);
   gtk_box_pack_start(GTK_BOX(box),button3,FALSE,FALSE,5);
   button4 = gtk_button_new_from_stock(GTK_STOCK_OK);
   g_signal_connect(G_OBJECT(button4),"clicked",
   G_CALLBACK(on_button_clicked),(gpointer)b4);
   gtk_box_pack_start(GTK_BOX(box),button4,FALSE,FALSE,5);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE ;
}
```
![](https://img-blog.csdnimg.cn/20190105163003703.PNG)
# 08.多选按钮check button
```cpp
/* 多项选择窗口 checkbutton.c */
#include <gtk/gtk.h>
gboolean isbold = FALSE ;
gboolean isitli = FALSE ;
gboolean isuline = FALSE ;
gboolean iscolor = FALSE ;
void on_check_clicked (GtkWidget* check,gint data)
{
   switch((int)data)
   {
      case 1:
         isbold = !isbold;
         break;
      case 2:
         isitli = !isitli;
         break;
      case 3:
         isuline = !isuline;
         break;
      case 4:
         iscolor = !iscolor;
         break;
   }
}
void on_button_clicked (GtkWidget *button,gpointer data)
{
   g_print("字体配置为：");
   if(isbold)
      g_print("粗体 ");
   if(isitli)
      g_print("斜体 ");
   if(isuline)
      g_print("下划线 ");
   if(iscolor)
      g_print("彩色 ");
   if( !isbold && !iscolor && !isuline && !isitli )
      g_print("正常，无任何选项");
   g_print("\n");
}
//主函数
int main (int argc, char *argv[])
{
   GtkWidget* window;
   GtkWidget* frame;
   GtkWidget* box;
   GtkWidget* button;
   GtkWidget* box1;
   GtkWidget* check1;
   GtkWidget* check2;
   GtkWidget* check3;
   GtkWidget* check4;
   char* title = "多项选择窗口";
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window),title);
   gtk_container_set_border_width(GTK_CONTAINER(window),20);
   g_signal_connect(G_OBJECT(window),"destroy",
         G_CALLBACK(gtk_main_quit),NULL);
   box = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),box);
   frame = gtk_frame_new("字体选项：");
   gtk_box_pack_start(GTK_BOX(box),frame,FALSE,FALSE,5);
   box1 = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(frame),box1);
   gtk_container_set_border_width(GTK_CONTAINER(box1),10);
   gtk_widget_show(box);
   check1 = gtk_check_button_new_with_label(" 粗体 ");
   g_signal_connect(G_OBJECT(check1),"released",
         G_CALLBACK(on_check_clicked),(gpointer)1);
   gtk_box_pack_start(GTK_BOX(box1),check1,FALSE,FALSE,3);
   check2 = gtk_check_button_new_with_label(" 斜体 ");
   g_signal_connect(G_OBJECT(check2),"released",
         G_CALLBACK(on_check_clicked),(gpointer)2);
   gtk_box_pack_start(GTK_BOX(box1),check2,FALSE,FALSE,3);
   check3 = gtk_check_button_new_with_label(" 下划线 ");
   g_signal_connect(G_OBJECT(check3),"released",
         G_CALLBACK(on_check_clicked),(gpointer)3);
   gtk_box_pack_start(GTK_BOX(box1),check3,FALSE,FALSE,3);
   check4 = gtk_check_button_new_with_label(" 彩色 ");
   g_signal_connect(G_OBJECT(check4),"released",
         G_CALLBACK(on_check_clicked),(gpointer)4);
   gtk_box_pack_start(GTK_BOX(box1),check4,FALSE,FALSE,3);
   button = gtk_button_new_from_stock(GTK_STOCK_OK);
   g_signal_connect(G_OBJECT(button),"clicked",
         G_CALLBACK(on_button_clicked),NULL);
   g_signal_connect_swapped(G_OBJECT(button),"clicked",
         G_CALLBACK(gtk_widget_destroy),window);
   gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE ;
}
```
![](https://img-blog.csdnimg.cn/20190105163246908.PNG)
# 09.单选按钮radio button
这里给出两个例子
```cpp
#include <glib.h>
#include <gtk/gtk.h>
gint close_application( GtkWidget *widget, GdkEvent *event, gpointer data )
{
   gtk_main_quit ();
   return FALSE;
}
int main( int argc, char *argv[] )
{
   GtkWidget *window = NULL;
   GtkWidget *box1;
   GtkWidget *box2;
   GtkWidget *button;
   GtkWidget *separator;
   GSList *group;
   gtk_init (&argc, &argv);
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   g_signal_connect (G_OBJECT (window), "delete_event",
                     G_CALLBACK (close_application), NULL);
   gtk_window_set_title (GTK_WINDOW (window), "radio buttons");
   gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  
   box1 = gtk_vbox_new (FALSE, 0);
   gtk_container_add (GTK_CONTAINER (window), box1);
   gtk_widget_show (box1);
   box2 = gtk_hbox_new (FALSE, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
   gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);
   gtk_widget_show (box2);
   button = gtk_radio_button_new_with_label (NULL, "Leonvel");
   gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);
   gtk_widget_show (button);
   group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));
   button = gtk_radio_button_new_with_label (group, "Rong");
   gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
   gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);
   gtk_widget_show (button);
   button = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (button),
                                                         "Tao");
   gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);
   gtk_widget_show (button);
   separator = gtk_hseparator_new ();
   gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 0);
   gtk_widget_show (separator);
   box2 = gtk_vbox_new (FALSE, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
   gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, TRUE, 0);
   gtk_widget_show (box2);
   button = gtk_button_new_with_label ("close");
   g_signal_connect_swapped (G_OBJECT (button), "clicked",
                             G_CALLBACK (close_application), window);
   gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);
                       GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
   gtk_widget_grab_default (button);
   gtk_widget_show (button);
   gtk_widget_show (window);
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105163529358.PNG)
第二个例子
```cpp
/* 单项选择窗口 radiobutton.c */
#include <gtk/gtk.h>
static gchar* red = " 红色 " ;
static gchar* green = " 绿色 " ;
static gchar* yellow = " 黄色 " ;
static gchar* blue = " 蓝色 " ;
void on_radio_clicked (GtkWidget* radio,gint data)
{
   g_print("你选择的颜色是：");
   switch((int)data)
   {
      case 1:
         g_print("%s",red); break;
      case 2:
         g_print("%s",green); break;
      case 3:
         g_print("%s",yellow); break;
      case 4:
         g_print("%s",blue); break;
   }
   g_print("\n");
}
//主函数
int main (int argc, char *argv[])
{
   GtkWidget* window;
   GtkWidget* frame;
   GtkWidget* box;
   GtkWidget* box1;
   GtkWidget* button1;
   GtkWidget* radio1;
   GtkWidget* radio2;
   GtkWidget* radio3;
   GtkWidget* radio4;
   GSList* group;
   gchar* title = "单项选择窗口" ;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window),title);
   gtk_container_set_border_width(GTK_CONTAINER(window),10);
   g_signal_connect(G_OBJECT(window),"destroy",
   G_CALLBACK(gtk_main_quit),NULL);
   box = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),box);
   frame = gtk_frame_new("请选择一种颜色：");
   gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_ETCHED_OUT);
   gtk_box_pack_start(GTK_BOX(box),frame,FALSE,FALSE,5);
   box1 = gtk_hbox_new(FALSE,10);
   gtk_container_set_border_width(GTK_CONTAINER(box1),10);
   gtk_container_add(GTK_CONTAINER(frame),box1);
   radio1 = gtk_radio_button_new_with_label(NULL,red);//红色
   g_signal_connect(G_OBJECT(radio1),"released",
         G_CALLBACK(on_radio_clicked),(gpointer)1);
   gtk_box_pack_start(GTK_BOX(box1),radio1,FALSE,FALSE,5);
//创建多选按钮组
   group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio1));
   radio2 = gtk_radio_button_new_with_label(group,green);//绿色
   g_signal_connect(G_OBJECT(radio2),"released",
         G_CALLBACK(on_radio_clicked),(gpointer)2);
   gtk_box_pack_start(GTK_BOX(box1),radio2,FALSE,FALSE,5);
   group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio2));
   radio3 = gtk_radio_button_new_with_label(group,yellow);//黄色
   g_signal_connect(G_OBJECT(radio3),"released",
         G_CALLBACK(on_radio_clicked),(gpointer)3);
   gtk_box_pack_start(GTK_BOX(box1),radio3,FALSE,FALSE,5);
   group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio3));
   radio4 = gtk_radio_button_new_with_label(group,blue);//蓝色
   g_signal_connect(G_OBJECT(radio4),"released",
         G_CALLBACK(on_radio_clicked),(gpointer)4);
   gtk_box_pack_start(GTK_BOX(box1),radio4,FALSE,FALSE,5);
   button1 = gtk_button_new_from_stock(GTK_STOCK_OK);
   g_signal_connect(G_OBJECT(button1),"clicked",
         G_CALLBACK(gtk_main_quit),NULL);
   gtk_box_pack_start(GTK_BOX(box),button1,FALSE,FALSE,5);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE ;
}
```
![](https://img-blog.csdnimg.cn/20190105163644850.PNG)
# 10.控制条scrollbar
```cpp
#include <gtk/gtk.h>
GtkWidget *hscale, *vscale;
void cb_pos_menu_select( GtkWidget *item, GtkPositionType pos )
{
/* 设置两个比例构件的比例值的显示位置 */
   gtk_scale_set_value_pos (GTK_SCALE (hscale), pos);
   gtk_scale_set_value_pos (GTK_SCALE (vscale), pos);
}
void cb_update_menu_select( GtkWidget *item, GtkUpdateType policy )
{
/* 设置两个比例构件的更新方式 */
   gtk_range_set_update_policy (GTK_RANGE (hscale), policy);
   gtk_range_set_update_policy (GTK_RANGE (vscale), policy);
}
void cb_digits_scale( GtkAdjustment *adj )
{
/* 设置 adj->value 圆整的小数位数 */
   gtk_scale_set_digits (GTK_SCALE (hscale), (gint) adj->value);
   gtk_scale_set_digits (GTK_SCALE (vscale), (gint) adj->value);
}
void cb_page_size( GtkAdjustment *get, GtkAdjustment *set )
{
/* 将示例调整对象的 page size 和 page increment size 设置
* 为"Page Size"比例构件指定的值 */
   set->page_size = get->value;
   set->page_increment = get->value;
/* 设置调整对象的值并使它引发一个 "changed" 信号,以重新配置所有
* 已经连接到这个调整对象的构件。 */
   gtk_adjustment_set_value (set, CLAMP (set->value,
             set->lower, (set->upper - set->page_size)));
}
void cb_draw_value( GtkToggleButton *button )
{
/* 根据复选按钮的状态设置在比例构件上是否显示比例值 */
   gtk_scale_set_draw_value (GTK_SCALE (hscale), button->active);
   gtk_scale_set_draw_value (GTK_SCALE (vscale), button->active);
}
/* 方便的函数 */
GtkWidget *make_menu_item (gchar *name, GCallback callback, gpointer data)
{
   GtkWidget *item;
   item = gtk_menu_item_new_with_label (name);
   g_signal_connect (G_OBJECT (item), "activate", callback, data);
   gtk_widget_show (item);
   return item;
}
void scale_set_default_values( GtkScale *scale )
{
   gtk_range_set_update_policy (GTK_RANGE (scale),
   GTK_UPDATE_CONTINUOUS);
   gtk_scale_set_digits (scale, 1);
   gtk_scale_set_value_pos (scale, GTK_POS_TOP);
   gtk_scale_set_draw_value (scale, TRUE);
}
/* 创建示例窗口 */
void create_range_controls( void )
{
   GtkWidget *window;
   GtkWidget *box1, *box2, *box3;
   GtkWidget *button;
   GtkWidget *scrollbar;
   GtkWidget *separator;
   GtkWidget *opt, *menu, *item;
   GtkWidget *label;
   GtkWidget *scale;
   GtkObject *adj1, *adj2;
/* 标准的创建窗口代码 */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
   gtk_window_set_title (GTK_WINDOW (window), "RT's range controls");
   box1 = gtk_vbox_new (FALSE, 0);
   gtk_container_add (GTK_CONTAINER (window), box1);
   gtk_widget_show (box1);
   box2 = gtk_hbox_new (FALSE, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
   gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);
   gtk_widget_show (box2);
/* value, lower, upper, step_increment, page_increment, page_size */
/* 注意,page_size 值只对滚动条构件有区别,并且,你实际上能取得的最高值
* 是(upper - page_size)。 */
   adj1 = gtk_adjustment_new (0.0, 0.0, 101.0, 0.1, 1.0, 1.0);
   vscale = gtk_vscale_new (GTK_ADJUSTMENT (adj1));
   scale_set_default_values (GTK_SCALE (vscale));
   gtk_box_pack_start (GTK_BOX (box2), vscale, TRUE, TRUE, 0);
   gtk_widget_show (vscale);
   box3 = gtk_vbox_new (FALSE, 10);
   gtk_box_pack_start (GTK_BOX (box2), box3, TRUE, TRUE, 0);
   gtk_widget_show (box3);
/* 重新使用同一个调整对象 */
   hscale = gtk_hscale_new (GTK_ADJUSTMENT (adj1));
   gtk_widget_set_size_request (GTK_WIDGET (hscale), 200, -1);
   scale_set_default_values (GTK_SCALE (hscale));
   gtk_box_pack_start (GTK_BOX (box3), hscale, TRUE, TRUE, 0);
   gtk_widget_show (hscale);
/* 再次重用同一个调整对象 */
   scrollbar = gtk_hscrollbar_new (GTK_ADJUSTMENT (adj1));
/* 注意,这导致当滚动条移动时,比例构件总是连续更新 */
   gtk_range_set_update_policy (GTK_RANGE (scrollbar),
   GTK_UPDATE_CONTINUOUS);
   gtk_box_pack_start (GTK_BOX (box3), scrollbar, TRUE, TRUE, 0);
   gtk_widget_show (scrollbar);
   box2 = gtk_hbox_new (FALSE, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
   gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);
   gtk_widget_show (box2);
/* 用一个复选按钮控制是否显示比例构件的值 */
   button = gtk_check_button_new_with_label("Display value on scale widgets");
   gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
   g_signal_connect (G_OBJECT (button), "toggled",
                     G_CALLBACK (cb_draw_value), NULL);
   gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);
   gtk_widget_show (button);
   box2 = gtk_hbox_new (FALSE, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
/* 用一个选项菜单以改变显示值的位置 */
   label = gtk_label_new ("Scale Value Position:");
   gtk_box_pack_start (GTK_BOX (box2), label, FALSE, FALSE, 0);
   gtk_widget_show (label);
   opt = gtk_option_menu_new ();
   menu = gtk_menu_new ();
   item = make_menu_item ("Top", G_CALLBACK (cb_pos_menu_select),
                          GINT_TO_POINTER (GTK_POS_TOP));
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
   item = make_menu_item ("Bottom", G_CALLBACK (cb_pos_menu_select),
                          GINT_TO_POINTER (GTK_POS_BOTTOM));
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
   item = make_menu_item ("Left", G_CALLBACK (cb_pos_menu_select),
                           GINT_TO_POINTER (GTK_POS_LEFT));
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
   item = make_menu_item ("Right", G_CALLBACK (cb_pos_menu_select),
                           GINT_TO_POINTER (GTK_POS_RIGHT));
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
   gtk_option_menu_set_menu (GTK_OPTION_MENU (opt), menu);
   gtk_box_pack_start (GTK_BOX (box2), opt, TRUE, TRUE, 0);
   gtk_widget_show (opt);
   gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);
   gtk_widget_show (box2);
   box2 = gtk_hbox_new (FALSE, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
/* 另一个选项菜单,这里是用于设置比例构件的更新方式 */
   label = gtk_label_new ("Scale Update Policy:");
   gtk_box_pack_start (GTK_BOX (box2), label, FALSE, FALSE, 0);
   gtk_widget_show (label);
   opt = gtk_option_menu_new ();
   menu = gtk_menu_new ();
   item = make_menu_item ("Continuous", G_CALLBACK (cb_update_menu_select),
                           GINT_TO_POINTER (GTK_UPDATE_CONTINUOUS));
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
   item = make_menu_item ("Discontinuous", G_CALLBACK (cb_update_menu_select),
                          GINT_TO_POINTER (GTK_UPDATE_DISCONTINUOUS));
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
   item = make_menu_item ("Delayed", G_CALLBACK (cb_update_menu_select),
                          GINT_TO_POINTER (GTK_UPDATE_DELAYED));
   gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
   gtk_option_menu_set_menu (GTK_OPTION_MENU (opt), menu);
   gtk_box_pack_start (GTK_BOX (box2), opt, TRUE, TRUE, 0);
   gtk_widget_show (opt);
   gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);
   gtk_widget_show (box2);
   box2 = gtk_hbox_new (FALSE, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
/* 一个水平比例构件,用于调整示例比例构件的显示小数位数。 */
   label = gtk_label_new ("Scale Digits:");
   gtk_box_pack_start (GTK_BOX (box2), label, FALSE, FALSE, 0);
   gtk_widget_show (label);
   adj2 = gtk_adjustment_new (1.0, 0.0, 5.0, 1.0, 1.0, 0.0);
   g_signal_connect (G_OBJECT (adj2), "value_changed", 
                     G_CALLBACK (cb_digits_scale), NULL);
   scale = gtk_hscale_new (GTK_ADJUSTMENT (adj2));
   gtk_scale_set_digits (GTK_SCALE (scale), 0);
   gtk_box_pack_start (GTK_BOX (box2), scale, TRUE, TRUE, 0);
   gtk_widget_show (scale);
   gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);
   gtk_widget_show (box2);
   box2 = gtk_hbox_new (FALSE, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
/* 以及,最后一个水平比例构件用于调整滚动条的 page size。 */
   label = gtk_label_new ("Scrollbar Page Size:");
   gtk_box_pack_start (GTK_BOX (box2), label, FALSE, FALSE, 0);
   gtk_widget_show (label);
   adj2 = gtk_adjustment_new (1.0, 1.0, 101.0, 1.0, 1.0, 0.0);
   g_signal_connect (G_OBJECT (adj2), "value_changed",
                     G_CALLBACK (cb_page_size), adj1);
   scale = gtk_hscale_new (GTK_ADJUSTMENT (adj2));
   gtk_scale_set_digits (GTK_SCALE (scale), 0);
   gtk_box_pack_start (GTK_BOX (box2), scale, TRUE, TRUE, 0);
   gtk_widget_show (scale);
   gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);
   gtk_widget_show (box2);
   separator = gtk_hseparator_new ();
   gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 0);
   gtk_widget_show (separator);
   box2 = gtk_vbox_new (FALSE, 10);
   gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
   gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, TRUE, 0);
   gtk_widget_show (box2);
   button = gtk_button_new_with_label ("Quit");
   g_signal_connect_swapped (G_OBJECT (button), "clicked",
                             G_CALLBACK (gtk_main_quit), NULL);
   gtk_box_pack_start (GTK_BOX (box2), button, TRUE, TRUE, 0);
                        GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
   gtk_widget_grab_default (button);
   gtk_widget_show (button);
   gtk_widget_show (window);
}
int main( int argc, char *argv[] )
{
   gtk_init (&argc, &argv);
   create_range_controls ();
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105164026359.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 11.箭头按钮create_arrow_button
```cpp
#include <gtk/gtk.h>
/* 用指定的参数创建一个箭头构件并将它组装到按钮中 */
GtkWidget *create_arrow_button( GtkArrowType arrow_type, GtkShadowType shadow_type )
{
   GtkWidget *button;
   GtkWidget *arrow;
   button = gtk_button_new ();
   arrow = gtk_arrow_new (arrow_type, shadow_type);
   gtk_container_add (GTK_CONTAINER (button), arrow);
   gtk_widget_show (button);
   gtk_widget_show (arrow);
   return button;
}
int main( int argc, char *argv[] )
{
/* GtkWidget 是构件的存储类型 */
   GtkWidget *window;
   GtkWidget *button;
   GtkWidget *box;
/* 初始化 */
   gtk_init (&argc, &argv);
/* 创建一个新窗口 */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "Arrow Buttons");
/* 对所有的窗口都这样做是一个好主意 */
   g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
/* 设置窗口的边框的宽度 */
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
/* 建一个组装盒以容纳箭头/按钮 */
   box = gtk_hbox_new (FALSE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (box), 2);
   gtk_container_add (GTK_CONTAINER (window), box);
/* 组装、显示所有的构件 */
   gtk_widget_show (box);
   button = create_arrow_button (GTK_ARROW_UP, GTK_SHADOW_IN);
   gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 3);
   button = create_arrow_button (GTK_ARROW_DOWN, GTK_SHADOW_OUT);
   gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 3);
   button = create_arrow_button (GTK_ARROW_LEFT, GTK_SHADOW_ETCHED_IN);
   gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 3);
   button = create_arrow_button (GTK_ARROW_RIGHT, GTK_SHADOW_ETCHED_OUT);
   gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 3);
   gtk_widget_show (window);
/* 进入主循环,等待用户的动作 */
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105164240201.PNG)
# 12.标签label
```cpp
#include <gtk/gtk.h>
int main( int argc, char *argv[] )
{
   static GtkWidget *window = NULL;
   GtkWidget *hbox;
   GtkWidget *vbox;
   GtkWidget *frame;
   GtkWidget *label;
/* 初始化 */
   gtk_init (&argc, &argv);
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
   gtk_window_set_title (GTK_WINDOW (window), "Label");
   vbox = gtk_vbox_new (FALSE, 5);
   hbox = gtk_hbox_new (FALSE, 5);
   gtk_container_add (GTK_CONTAINER (window), hbox);
   gtk_box_pack_start (GTK_BOX (hbox), vbox, FALSE, FALSE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (window), 5);
   frame = gtk_frame_new ("Normal Label");
   label = gtk_label_new ("This is a Normal label");
   gtk_container_add (GTK_CONTAINER (frame), label);
   gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
   frame = gtk_frame_new ("Multi-line Label");
   label = gtk_label_new ("This is a Multi-line label.\nSecond line\n" \
                          "Third line");
   gtk_container_add (GTK_CONTAINER (frame), label);
   gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
   frame = gtk_frame_new ("Left Justified Label");
   label = gtk_label_new ("This is a Left-Justified\n" \
                          "Multi-line label.\nThird line");
   gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);
   gtk_container_add (GTK_CONTAINER (frame), label);
   gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
   frame = gtk_frame_new ("Right Justified Label");
   label = gtk_label_new ("This is a Right-Justified\nMulti-line label.\n" \
                          "Fourth line, (j/k)");
   gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_RIGHT);
   gtk_container_add (GTK_CONTAINER (frame), label);
   gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
   vbox = gtk_vbox_new (FALSE, 5);
   gtk_box_pack_start (GTK_BOX (hbox), vbox, FALSE, FALSE, 0);
   frame = gtk_frame_new ("Line wrapped label");
   label = gtk_label_new ("This is an example of a line-wrapped label. It " \
                           /* 用一段较长的空白字符来测试空白的自动排列 */
                          "should not be taking up the entire" \
                          "width allocated to it, but automatically " \
                          "wraps the words to fit. " \
                          "The time has come, for all good men, to come to " \
                          "the aid of their party. " \
                          "The sixth sheik's six sheep's sick.\n" \
                          "It supports multiple paragraphs correctly, " \
                          "and correctly adds "\
                          "many extra spaces. ");
   gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
   gtk_container_add (GTK_CONTAINER (frame), label);
   gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
   frame = gtk_frame_new ("Filled, wrapped label");
   label = gtk_label_new ("This is an example of a line-wrapped, filled label. " \
                          "It should be taking "\
                          "up the entire width allocated to it. " \
                          "Here is a sentence to prove "\
                          "my point. Here is another sentence. "\
                          "Here comes the sun, do de do de do.\n"\
                          " This is a new paragraph.\n"\
                          " This is another newer, longer, better " \
                          "paragraph. It is coming to an end, "\
                          "unfortunately.");
   gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_FILL);
   gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
   gtk_container_add (GTK_CONTAINER (frame), label);
   gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
   frame = gtk_frame_new ("Underlined label");
   label = gtk_label_new ("This label is underlined!\n"
                          "This one is underlined in quite a funky fashion");
   gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);
   gtk_label_set_pattern (GTK_LABEL (label),
                          "_________________________ _ _________ _ ________ _______ ___");
   gtk_container_add (GTK_CONTAINER (frame), label);
   gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
   gtk_widget_show_all (window);
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105164508468.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 12.子窗口：触发事件打开窗口
```cpp
#include <gtk/gtk.h>
gint delete_event( GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
    g_print ("delete event occurred\n");
    /* 改 TRUE 为 FALSE 程序会关闭。*/
    return FALSE;
}
/* 另一个回调函数 */
void destroy( GtkWidget *widget, gpointer   data )
{
    gtk_main_quit ();
}
void hello( GtkWidget *widget, gpointer   data )
{
    g_print ("Hello World\n");
}
void New_window( GtkWidget *widget, gpointer   data )
{
    GtkWidget *window;
    GtkWidget *button;
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
    button = gtk_button_new_with_label ("Hello world !");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (hello), "1");
    gtk_container_add (GTK_CONTAINER (window), button);
    gtk_widget_show (button);
    gtk_widget_show (window);
    g_print ("Hello World\n");//temp(data );
}
int main( int   argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;
    
    gtk_init (&argc, &argv);
    
    /* 创建一个新窗口 */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
    button = gtk_button_new_with_label ("Open hello world");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (New_window), "1");
    gtk_container_add (GTK_CONTAINER (window), button);
    gtk_widget_show (button);
    gtk_widget_show (window);
    gtk_main ();
    
    return 0;
}
```
# 13.进度条progressbar
```cpp
#include <gtk/gtk.h>
typedef struct _ProgressData {
   GtkWidget *window;
   GtkWidget *pbar;
   int timer;
   gboolean activity_mode;
} ProgressData;
/* 更新进度条,这样就能够看到进度条的移动 */
gint progress_timeout( gpointer data )
{
   ProgressData *pdata = (ProgressData *)data;
   gdouble new_val;
   if (pdata->activity_mode)
   {
       gtk_progress_bar_pulse (GTK_PROGRESS_BAR (pdata->pbar));
   } else {
/* 使用在调整对象中设置的取值范围计算进度条的值 */
       new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (pdata->pbar)) + 0.01;
       if (new_val > 1.0)
          new_val = 0.0;
/* 设置进度条的新值 */
       gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pdata->pbar), new_val);
   }
/* 这是一个 timeout 函数,返回 TRUE,这样它就能够继续被调用 */
   return TRUE;
}
/* 回调函数,切换在进度条你的滑槽上的文本显示 */
void toggle_show_text( GtkWidget *widget, ProgressData *pdata )
{
   const gchar *text;
   text = gtk_progress_bar_get_text (GTK_PROGRESS_BAR (pdata->pbar));
   if (text && *text)
      gtk_progress_bar_set_text (GTK_PROGRESS_BAR (pdata->pbar), "");
   else
      gtk_progress_bar_set_text (GTK_PROGRESS_BAR (pdata->pbar), "some text");
}
/* 回调函数,切换进度条的活动模式 */
void toggle_activity_mode( GtkWidget *widget, ProgressData *pdata )
{
   pdata->activity_mode = !pdata->activity_mode;
   if (pdata->activity_mode)
       gtk_progress_bar_pulse (GTK_PROGRESS_BAR (pdata->pbar));
   else
       gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pdata->pbar), 0.0);
}
/* 回调函数,切换进度条的移动方向 */
void toggle_orientation( GtkWidget *widget, ProgressData *pdata )
{
 switch (gtk_progress_bar_get_orientation (GTK_PROGRESS_BAR (pdata->pbar))) {
   case GTK_PROGRESS_LEFT_TO_RIGHT:
      gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (pdata->pbar),
                                     GTK_PROGRESS_RIGHT_TO_LEFT);
      break;
   case GTK_PROGRESS_RIGHT_TO_LEFT:
      gtk_progress_bar_set_orientation (GTK_PROGRESS_BAR (pdata->pbar),
                                     GTK_PROGRESS_LEFT_TO_RIGHT);
      break;
   default:; // 什么也不做
 }
}
/* 清除分配的内存,删除定时器(timer) */
void destroy_progress( GtkWidget *widget, ProgressData *pdata)
{
   gtk_timeout_remove (pdata->timer);
   pdata->timer = 0;
   pdata->window = NULL;
   g_free (pdata);
   gtk_main_quit ();
}
int main( int argc, char *argv[])
{
   ProgressData *pdata;
   GtkWidget *align;
   GtkWidget *separator;
   GtkWidget *table;
   GtkWidget *button;
   GtkWidget *check;
   GtkWidget *vbox;
   gtk_init (&argc, &argv);
/* 为传递到回调函数中的数据分配内存 */
   pdata = g_malloc (sizeof (ProgressData));
   pdata->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_resizable (GTK_WINDOW (pdata->window), TRUE);
   g_signal_connect (G_OBJECT (pdata->window), "destroy",
                     G_CALLBACK (destroy_progress), pdata);
   gtk_window_set_title (GTK_WINDOW (pdata->window), "GtkProgressBar");
   gtk_container_set_border_width (GTK_CONTAINER (pdata->window), 10);
   vbox = gtk_vbox_new (FALSE, 5);
   gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
   gtk_container_add (GTK_CONTAINER (pdata->window), vbox);
   gtk_widget_show (vbox);
/* 创建一个居中对齐的对象 */
   align = gtk_alignment_new (0.5, 0.5, 0, 0);
   gtk_box_pack_start (GTK_BOX (vbox), align, FALSE, FALSE, 5);
   gtk_widget_show (align);
/* 创建进度条 */
   pdata->pbar = gtk_progress_bar_new ();
   gtk_container_add (GTK_CONTAINER (align), pdata->pbar);
   gtk_widget_show (pdata->pbar);
/* 加一个定时器(timer),以更新进度条的值 */
   pdata->timer = gtk_timeout_add (200, progress_timeout, pdata);
   separator = gtk_hseparator_new ();
   gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, FALSE, 0);
   gtk_widget_show (separator);
/* 行数、列数、同质性(homogeneous) */
   table = gtk_table_new (3, 2, FALSE);
   gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, TRUE, 0);
   gtk_widget_show (table);
/* 添加一个复选按钮,以选择是否显示在滑槽里的文本 */
   check = gtk_check_button_new_with_label ("Show text");
   gtk_table_attach (GTK_TABLE (table), check, 0, 1, 0, 1,
                     GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
   g_signal_connect (G_OBJECT (check), "clicked", G_CALLBACK (toggle_show_text), pdata);
   gtk_widget_show (check);
/* 添加一个复选按钮,切换活动状态 */
   check = gtk_check_button_new_with_label ("Activity mode");
   gtk_table_attach (GTK_TABLE (table), check, 0, 1, 1, 2,
                     GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
   g_signal_connect (G_OBJECT (check), "clicked", G_CALLBACK (toggle_activity_mode), pdata);
   gtk_widget_show (check);
/* 添加一个复选按钮,切换移动方向 */
   check = gtk_check_button_new_with_label ("Right to Left");
   gtk_table_attach (GTK_TABLE (table), check, 0, 1, 2, 3, 
                     GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
   g_signal_connect (G_OBJECT (check), "clicked",
                     G_CALLBACK (toggle_orientation), pdata);
   gtk_widget_show (check);
/* 添加一个按钮,用来退出应用程序 */
   button = gtk_button_new_with_label ("close");
   g_signal_connect_swapped (G_OBJECT (button), "clicked",
                             G_CALLBACK (gtk_widget_destroy), pdata->window);
   gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
/* 将按钮设置为能缺省的构件 */
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
/* 将缺省焦点设置到这个按钮上,使之成为缺省按钮,只要按回车键
* 就相当于点击了这个按钮 */
//译者注: 能缺省的构件在获取焦点后成为缺省构件,用户按方向键等可以切换焦点。
   gtk_widget_grab_default (button);
   gtk_widget_show (button);
   gtk_widget_show (pdata->window);
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105165003304.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 14.窗口标尺gtk_hruler_new
```cpp
#include <gtk/gtk.h>
#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS(i)->x
#define XSIZE 600
#define YSIZE 400
/* 当点击"close"按钮时,退出应用程序 */
gint close_application( GtkWidget *widget, GdkEvent *event, gpointer data )
{
   gtk_main_quit ();
   return FALSE;
}
/* 主函数 */
int main( int argc, char *argv[] ) 
{
   GtkWidget *window, *table, *area, *hrule, *vrule;
/* 初始化,创建主窗口 */
   gtk_init (&argc, &argv);
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (close_application), NULL);
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
/* 创建一个组装表,绘图区和标尺放在里面 */
   table = gtk_table_new (3, 2, FALSE);
   gtk_container_add (GTK_CONTAINER (window), table);
   area = gtk_drawing_area_new ();
   gtk_widget_set_size_request (GTK_WIDGET (area), XSIZE, YSIZE);
   gtk_table_attach (GTK_TABLE (table), area, 1, 2, 1, 2,
                     GTK_EXPAND|GTK_FILL, GTK_FILL, 0, 0);
   gtk_widget_set_events (area, GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);
/* 水平标尺放在顶部。鼠标移动穿过绘图区时,一个
* motion_notify_event 被传递给标尺的相应的事件处理函数 */
   hrule = gtk_hruler_new ();
   gtk_ruler_set_metric (GTK_RULER (hrule), GTK_PIXELS);
   gtk_ruler_set_range (GTK_RULER (hrule), 0, 50, 0, 20);
   g_signal_connect_swapped (G_OBJECT (area), "motion_notify_event",
                          G_CALLBACK (EVENT_METHOD (hrule, motion_notify_event)), hrule);
   gtk_table_attach (GTK_TABLE (table), hrule, 1, 2, 0, 1,
                     GTK_EXPAND|GTK_SHRINK|GTK_FILL, GTK_FILL, 0, 0);
/* 垂直标尺放在左边。当鼠标移动穿过绘图区时,一个
* motion_notify_event 被传递到标尺相应的事件处理函数中 */
   vrule = gtk_vruler_new ();
   gtk_ruler_set_metric (GTK_RULER (vrule), GTK_PIXELS);
   gtk_ruler_set_range (GTK_RULER (vrule), 0, YSIZE, 10, YSIZE );
   g_signal_connect_swapped (G_OBJECT (area), "motion_notify_event",
                     G_CALLBACK (EVENT_METHOD (vrule, motion_notify_event)), vrule);
   gtk_table_attach (GTK_TABLE (table), vrule, 0, 1, 1, 2,
                     GTK_FILL, GTK_EXPAND|GTK_SHRINK|GTK_FILL, 0, 0);
/* 现在显示所有的构件 */
   gtk_widget_show (area);
   gtk_widget_show (hrule);
   gtk_widget_show (vrule);
   gtk_widget_show (table);
   gtk_widget_show (window);
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105165438805.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 15.状态栏（条）gtk_statusbar_new
```cpp
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib.h>
GtkWidget *status_bar;
void push_item( GtkWidget *widget, gpointer data )
{
   static int count = 1;
   char buff[20];
   g_snprintf (buff, 20, "Item %d", count++);
   gtk_statusbar_push (GTK_STATUSBAR (status_bar), GPOINTER_TO_INT (data), buff);
   return;
}
void pop_item( GtkWidget *widget, gpointer data )
{
   gtk_statusbar_pop (GTK_STATUSBAR (status_bar), GPOINTER_TO_INT (data));
   return;
}
int main( int argc, char *argv[] )
{
   GtkWidget *window;
   GtkWidget *vbox;
   GtkWidget *button;
   gint context_id;
   gtk_init (&argc, &argv);
/* 创建新窗口 */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_size_request (GTK_WIDGET (window), 200, 100);
   gtk_window_set_title (GTK_WINDOW (window), "GTK Statusbar Example");
   g_signal_connect (G_OBJECT (window), "delete_event",
                     G_CALLBACK (exit), NULL);
   vbox = gtk_vbox_new (FALSE, 1);
   gtk_container_add (GTK_CONTAINER (window), vbox);
   gtk_widget_show (vbox);
   status_bar = gtk_statusbar_new ();
   gtk_box_pack_start (GTK_BOX (vbox), status_bar, TRUE, TRUE, 0);
   gtk_widget_show (status_bar);
   context_id = gtk_statusbar_get_context_id( GTK_STATUSBAR (status_bar), "Statusbar example");
   button = gtk_button_new_with_label ("push item");
   g_signal_connect (G_OBJECT (button), "clicked",
                     G_CALLBACK (push_item), GINT_TO_POINTER (context_id));
   gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 2);
   gtk_widget_show (button);
   button = gtk_button_new_with_label ("pop last item");
   g_signal_connect (G_OBJECT (button), "clicked",
                     G_CALLBACK (pop_item), GINT_TO_POINTER (context_id));
   gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 2);
   gtk_widget_show (button);
/* 将窗口最后显示,让整个窗口一次性出现在屏幕上. */
   gtk_widget_show (window);
   gtk_main ();
   return 0;
}
```
## 另一个例子
```cpp
/* 状态栏 status.c */
#include <gtk/gtk.h>
static GtkWidget *statusbar;
gint i = 0 ;
void on_push_clicked (GtkButton* button,gpointer data)
{
   char message[1024];
   i++ ;
   sprintf(message,"这是输入的第 %d 条消息．",i);
   gtk_statusbar_push(GTK_STATUSBAR(statusbar),i,message);
}
void on_pop_clicked (GtkButton* button,gpointer data)
{
   if (i<0) return ;
   gtk_statusbar_pop(GTK_STATUSBAR(statusbar),i);
   i--;
}
void on_popped (GtkStatusbar* statusbar,guint id,const gchar*
text)
{
   if(!statusbar->messages)
   i = 0;
}
int main ( int argc , char* argv[])
{
   GtkWidget *window;
   GtkWidget *vbox,*hbox;
   GtkWidget *button;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   g_signal_connect(G_OBJECT(window),"delete_event",
      G_CALLBACK(gtk_main_quit),NULL);
   gtk_window_set_title(GTK_WINDOW(window),"状态栏");
  // gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window),20);
   vbox = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);
   statusbar = gtk_statusbar_new();
   g_signal_connect(G_OBJECT(statusbar),"text-popped",
      G_CALLBACK(on_popped),NULL);
   gtk_box_pack_start(GTK_BOX(vbox),statusbar,FALSE,FALSE,5);
   hbox = gtk_hbox_new(FALSE,0);
   gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
   button = gtk_button_new_with_label("推入");
   g_signal_connect(G_OBJECT(button),"clicked",
      G_CALLBACK(on_push_clicked),NULL);
   gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,5);
   button = gtk_button_new_with_label("弹出");
   g_signal_connect(G_OBJECT(button),"clicked",
      G_CALLBACK(on_pop_clicked),NULL);
   gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,5);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/20190105175420680.PNG)
# 16.文本输入gtk_entry_new：可见/不可见，可输入/不可输入
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
void enter_callback( GtkWidget *widget, GtkWidget *entry )
{
   const gchar *entry_text;
   entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
   printf("Entry contents: %s\n", entry_text);
}
void entry_toggle_editable( GtkWidget *checkbutton, GtkWidget *entry )
{
   gtk_editable_set_editable (GTK_EDITABLE (entry),
                              GTK_TOGGLE_BUTTON (checkbutton)->active);
}
void entry_toggle_visibility( GtkWidget *checkbutton, GtkWidget *entry )
{
   gtk_entry_set_visibility (GTK_ENTRY (entry),
                             GTK_TOGGLE_BUTTON (checkbutton)->active);
}
int main( int argc, char *argv[] )
{
   GtkWidget *window;
   GtkWidget *vbox, *hbox;
   GtkWidget *entry;
   GtkWidget *button;
   GtkWidget *check;
   gint tmp_pos;
   gtk_init (&argc, &argv);
/* 创建一个新窗口 */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_size_request (GTK_WIDGET (window), 200, 100);
   gtk_window_set_title (GTK_WINDOW (window), "GTK Entry");
   //g_signal_connect (G_OBJECT (window), "destroy",
   //                  G_CALLBACK (gtk_main_quit), NULL);
   g_signal_connect (G_OBJECT (window), "delete_event",
                     G_CALLBACK (exit), NULL);
   g_signal_connect_swapped (G_OBJECT (window), "delete_event",
                             G_CALLBACK (gtk_widget_destroy), window);
   vbox = gtk_vbox_new (FALSE, 0);
   gtk_container_add (GTK_CONTAINER (window), vbox);
   gtk_widget_show (vbox);
   entry = gtk_entry_new ();
   gtk_entry_set_max_length (GTK_ENTRY (entry), 50);
   g_signal_connect (G_OBJECT (entry), "activate", G_CALLBACK (enter_callback), entry);
   gtk_entry_set_text (GTK_ENTRY (entry), "hello");
   tmp_pos = GTK_ENTRY (entry)->text_length;
   gtk_editable_insert_text (GTK_EDITABLE (entry), " world", -1, &tmp_pos);
   gtk_editable_select_region (GTK_EDITABLE (entry), 0, GTK_ENTRY (entry)->text_length);
   gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
   gtk_widget_show (entry);
   hbox = gtk_hbox_new (FALSE, 0);
   gtk_container_add (GTK_CONTAINER (vbox), hbox);
   gtk_widget_show (hbox);
   check = gtk_check_button_new_with_label ("Editable");
   gtk_box_pack_start (GTK_BOX (hbox), check, TRUE, TRUE, 0);
   g_signal_connect (G_OBJECT (check), "toggled",
                     G_CALLBACK (entry_toggle_editable), entry);
   gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
   gtk_widget_show (check);
   check = gtk_check_button_new_with_label ("Visible");
   gtk_box_pack_start (GTK_BOX (hbox), check, TRUE, TRUE, 0);
   g_signal_connect (G_OBJECT (check), "toggled",
                     G_CALLBACK (entry_toggle_visibility), entry);
   gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
   gtk_widget_show (check);
   button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
   g_signal_connect_swapped (G_OBJECT (button), "clicked",
                             G_CALLBACK (gtk_widget_destroy), window);
   gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
                       GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
   gtk_widget_grab_default (button);
   gtk_widget_show (button);
   gtk_widget_show (window);
   gtk_main();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105170041489.PNG)
## 规定最大长度的gtk_entry_new_with_max_length
```cpp
#include<gtk/gtk.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
GtkWidget *_nx, *_nz, *_dx, *_dz, *_npd, *_mm;
GtkWidget *_vel, *_epsilu, *_deta, *_shot, *_snap;
GtkWidget *_favg, *_fs, *_ds, *_ns, *_zs;
gint nx, nz, npd, mm, fs, ds, ns, zs;
gfloat dx, dz, favg;
void debug(GtkWidget *window,gpointer data)
{  
    gtk_main_quit();
}
 
void write_txt(GtkWidget *widget,gpointer data)
{
    const char*str_nx=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_nx));
    const char*str_nz=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_nz));
    const char*str_dx=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_dx));
    const char*str_dz=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_dz));
    const char*str_npd=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_npd));
    const char*str_mm=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_mm));
    const char*str_vel=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_vel));
    const char*str_epsilu=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_epsilu));
    const char*str_deta=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_deta));
    const char*str_favg=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_favg));
    const char*str_fs=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_fs));
    const char*str_ns=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_ns));
    const char*str_ds=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_ds));
    const char*str_zs=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_zs));
    const char*str_shot=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_shot));
    const char*str_snap=gtk_entry_get_text(GTK_ENTRY((GtkWidget *)_snap));
    nx=strtod(str_nx,NULL);
    nz=strtod(str_nz,NULL);
    dx=strtod(str_dx,NULL);
    dz=strtod(str_dz,NULL);
    npd=strtod(str_npd,NULL);
    mm=strtod(str_mm,NULL);
    favg=strtod(str_favg,NULL);
    fs=strtod(str_fs,NULL);
    ns=strtod(str_ns,NULL);
    ds=strtod(str_ds,NULL);
    zs=strtod(str_zs,NULL);
}
int main(int argc,char* argv[]){
 
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *table;
   
    gtk_init(&argc,&argv);
       
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"RT's VTI model");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    //gtk_window_set_default_size(GTK_WINDOW(window),400,400);
    g_signal_connect(GTK_OBJECT(window),"delete_event",GTK_SIGNAL_FUNC(debug),NULL);
     
    table=gtk_table_new(13,6,FALSE);
    gtk_container_add(GTK_CONTAINER(window),table);
    label=gtk_label_new("Model parameter:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,1,2,0,1);
/***   nx  ***/
    label=gtk_label_new("nx=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,1,2);
    _nx=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_nx), "100");
    gtk_table_attach_defaults(GTK_TABLE(table),_nx,1,2,1,2);
/***   dx  ***/
    label=gtk_label_new("dx=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,2,3,1,2);
    _dx=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_dx), "5");
    gtk_table_attach_defaults(GTK_TABLE(table),_dx,3,4,1,2);
/***   npd  ***/
    label=gtk_label_new("npd=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,4,5,1,2);
    _npd=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_npd), "20");
    gtk_table_attach_defaults(GTK_TABLE(table),_npd,5,6,1,2);
/***   nz  ***/
    label=gtk_label_new("nz=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,2,3);
    _nz=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_nz), "100");
    gtk_table_attach_defaults(GTK_TABLE(table),_nz,1,2,2,3);
/***   dz  ***/
    label=gtk_label_new("dz=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,2,3,2,3);
    _dz=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_dz), "5");
    gtk_table_attach_defaults(GTK_TABLE(table),_dz,3,4,2,3);
/***   mm  ***/
    label=gtk_label_new("mm=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,4,5,2,3);
    _mm=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_mm), "4");
    gtk_table_attach_defaults(GTK_TABLE(table),_mm,5,6,2,3);
    label=gtk_label_new("Input Filename:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,1,2,3,4);
/***   vel  ***/
    label=gtk_label_new("vel:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,4,5);
    _vel=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_vel), "vel.dat");
    gtk_table_attach_defaults(GTK_TABLE(table),_vel,1,4,4,5);
/***   epsilu  ***/
    label=gtk_label_new("epsilu:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,5,6);
    _epsilu=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_epsilu), "epsilu.dat");
    gtk_table_attach_defaults(GTK_TABLE(table),_epsilu,1,4,5,6);
/***   deta  ***/
    label=gtk_label_new("deta:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,6,7);
    _deta=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_deta), "deta.dat");
    gtk_table_attach_defaults(GTK_TABLE(table),_deta,1,4,6,7);
    label=gtk_label_new("ShotParas:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,1,2,7,8);
/***   ns  ***/
    label=gtk_label_new("ns=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,8,9);
    _ns=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_ns), "1");
    gtk_table_attach_defaults(GTK_TABLE(table),_ns,1,2,8,9);
/***   favg  ***/
    label=gtk_label_new("favg=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,2,3,8,9);
    _favg=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_favg), "20");
    gtk_table_attach_defaults(GTK_TABLE(table),_favg,3,4,8,9);
/***   fs  ***/
    label=gtk_label_new("fs=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,9,10);
    _fs=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_fs), "1");
    gtk_table_attach_defaults(GTK_TABLE(table),_fs,1,2,9,10);
/***   ds  ***/
    label=gtk_label_new("ds=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,2,3,9,10);
    _ds=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_ds), "0");
    gtk_table_attach_defaults(GTK_TABLE(table),_ds,3,4,9,10);
/***   zs  ***/
    label=gtk_label_new("zs=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,4,5,9,10);
    _zs=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_zs), "1");
    gtk_table_attach_defaults(GTK_TABLE(table),_zs,5,6,9,10);
    label=gtk_label_new("Output Filename:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,1,2,10,11);
/***   shot  ***/
    label=gtk_label_new("shot=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,11,12);
    _shot=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_shot), "shot.dat");
    gtk_table_attach_defaults(GTK_TABLE(table),_shot,1,4,11,12);
/***   snap  ***/
    label=gtk_label_new("snap=");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,12,13);
    _snap=gtk_entry_new_with_max_length(10);
    gtk_entry_set_text (GTK_ENTRY (_snap), "snap.dat");
    gtk_table_attach_defaults(GTK_TABLE(table),_snap,1,4,12,13);
    label=gtk_label_new("Please ensure all parameters input accuratlly !");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,10,13,14);
/***   run button  ***/
    button=gtk_button_new_with_label("RUN");
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(write_txt),NULL);
    gtk_table_attach_defaults(GTK_TABLE(table),button,5,6,12,13);
 
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
```
## 输入文本执行一条命令
```cpp
/* 命令工具 command.c */
#include <gtk/gtk.h>
static GtkWidget *entry;
void on_button_clicked (GtkButton* button,gpointer data)
{
   const gchar* command;
   command = gtk_entry_get_text(GTK_ENTRY(entry));
   system(command);
}
int main ( int argc , char* argv[])
{
   GtkWidget *window;
   GtkWidget *hbox;
   GtkWidget *label;
   GtkWidget *button;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   g_signal_connect(G_OBJECT(window),"delete_event",
         G_CALLBACK(gtk_main_quit),NULL);
   gtk_window_set_title(GTK_WINDOW(window),"命令窗口");
   //gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
   hbox = gtk_hbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),hbox);
   label = gtk_label_new("输入命令：");
   gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,5);
   entry = gtk_entry_new();
   gtk_box_pack_start(GTK_BOX(hbox),entry,TRUE,TRUE,5);
   button = gtk_button_new_with_label("执行");
   g_signal_connect(G_OBJECT(button),"clicked",
         G_CALLBACK(on_button_clicked),NULL);
   gtk_box_pack_start(GTK_BOX(hbox),button,FALSE,FALSE,5);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/20190105171232915.PNG)
# 17.文件选择：gtk_file_selection_new
```cpp
#include <gtk/gtk.h>
/* 获得文件名,并将它打印到控制台(console)上 */
void file_ok_sel( GtkWidget *w, GtkFileSelection *fs )
{
   g_print ("Filename:%s\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
}
int main( int argc, char *argv[] )
{
   GtkWidget *filew;
   gtk_init (&argc, &argv);
/* 创建一个新的文件选择构件 */
   filew = gtk_file_selection_new ("File selection");
   g_signal_connect (G_OBJECT (filew), "destroy", G_CALLBACK (gtk_main_quit), NULL);
/* 为 ok_button 按钮设置回调函数,连接到 file_ok_sel function 函数 */
   g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button), "clicked",
                     G_CALLBACK (file_ok_sel), filew);
/* 为 cancel_button 设置回调函数,销毁构件 */
   g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button), "clicked",
                             G_CALLBACK (gtk_widget_destroy), filew);
/* 设置文件名,比如这个一个文件保存对话框,我们给了一个缺省文件名 */
   gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew), "penguin.png");
   gtk_widget_show (filew);
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105170415408.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
给另一个例子，选择一个VTI介质模型参数
```cpp
#include <gtk/gtk.h>
#include <stdlib.h>
GtkWidget *file,*_vel,*_deta,*_epsilu;
void file_OK( GtkWidget *w, gpointer *data )
{
    gtk_entry_set_text (GTK_ENTRY (data),
                   gtk_file_selection_get_filename (GTK_FILE_SELECTION (file)));
    gtk_widget_destroy (file);
}
void select_file(GtkWidget *w,gpointer *data)
{
    file= gtk_file_selection_new ("File selection");
    g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (file)->ok_button), "clicked",
                     G_CALLBACK (file_OK), data);
    g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (file)->cancel_button), "clicked",
                             G_CALLBACK (gtk_widget_destroy), NULL);
    gtk_widget_show (file);
}
/* 创建一个新的横向盒,它包含一个图像和一个标签,并返回这个盒。*/
GtkWidget *xpm_label_box( gchar *xpm_filename, gchar *label_text )
{
   GtkWidget *box;
   GtkWidget *label;
   GtkWidget *image;
/* 为图像和标签创建盒 */
   box = gtk_hbox_new (FALSE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (box), 10);
/* 创建一个图像 */
   image = gtk_image_new_from_file (xpm_filename);
/* 为按钮创建一个标签 */
   label = gtk_label_new (label_text);
/* 把图像和标签组装到盒子里 */
   gtk_box_pack_start (GTK_BOX (box), image, TRUE, TRUE, 3);
   gtk_box_pack_start (GTK_BOX (box), label, TRUE, TRUE, 3);
   gtk_widget_show (image);
   gtk_widget_show (label);
   return box;
}
int main( int argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *table;
    GtkWidget *box;
    gtk_init (&argc, &argv);
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"RT's VTI model");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    g_signal_connect(GTK_OBJECT(window),"delete_event",GTK_SIGNAL_FUNC(gtk_main_quit),NULL);
     
    table=gtk_table_new(3,8,TRUE);
    gtk_container_add(GTK_CONTAINER(window),table);
    label=gtk_label_new("Input Filename:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,2,0,1);
/*******************  vel *******************/
    label=gtk_label_new("vel:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,1,2);
    _vel=gtk_entry_new_with_max_length(100);
    gtk_entry_set_text (GTK_ENTRY (_vel), "vel.dat");
    gtk_table_attach_defaults(GTK_TABLE(table),_vel,1,7,1,2);
    button=gtk_button_new();
    box = xpm_label_box ("pic/dir2.jpg", NULL);
    gtk_container_add (GTK_CONTAINER (button), box);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(select_file),_vel);
    gtk_table_attach_defaults(GTK_TABLE(table),button,7,8,1,2);
/*******************  epsilu *******************/
    label=gtk_label_new("epsilu:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,2,3);
    _epsilu=gtk_entry_new_with_max_length(100);
    gtk_entry_set_text (GTK_ENTRY (_epsilu), "epsilu.dat");
    gtk_table_attach_defaults(GTK_TABLE(table),_epsilu,1,7,2,3);
    //button=gtk_button_new_with_label("...");
    button=gtk_button_new();
    box = xpm_label_box ("pic/dir2.jpg", NULL);
    gtk_container_add (GTK_CONTAINER (button), box);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(select_file),_epsilu);
    gtk_table_attach_defaults(GTK_TABLE(table),button,7,8,2,3);
/*******************  deta *******************/
    label=gtk_label_new("deta:");
    gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,3,4);
    _deta=gtk_entry_new_with_max_length(100);
    gtk_entry_set_text (GTK_ENTRY (_deta), "deta.dat");
    gtk_table_attach_defaults(GTK_TABLE(table),_deta,1,7,3,4);
    //button=gtk_button_new_with_label("...");
    button=gtk_button_new();
    box = xpm_label_box ("pic/dir2.jpg", NULL);
    gtk_container_add (GTK_CONTAINER (button), box);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(select_file),_deta);
    gtk_table_attach_defaults(GTK_TABLE(table),button,7,8,3,4);
    gtk_widget_show_all(window);
    gtk_main ();
    return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105170535961.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 18.事件盒子gtk_event_box_new
```cpp
#include <stdlib.h>
#include <gtk/gtk.h>
int main( int argc, char *argv[] )
{
   GtkWidget *window;
   GtkWidget *event_box;
   GtkWidget *label;
   gtk_init (&argc, &argv);
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "Event Box");
   g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (exit), NULL);
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
/* 创建一个事件盒,然后将它加到顶级窗口上 */
   event_box = gtk_event_box_new ();
   gtk_container_add (GTK_CONTAINER (window), event_box);
   gtk_widget_show (event_box);
/* 创建一个长标签 */
   label = gtk_label_new ("Click here to quit, quit, quit, quit, quit");
   gtk_container_add (GTK_CONTAINER (event_box), label);
   gtk_widget_show (label);
/* 将标签剪裁短。 */
   gtk_widget_set_size_request (label, 110, 20);
/* 为它绑定一个动作 */
   gtk_widget_set_events (event_box, GDK_BUTTON_PRESS_MASK);
   g_signal_connect (G_OBJECT (event_box), "button_press_event", G_CALLBACK (exit), NULL);
/* 又是一个要有 X 窗口的东西... */
//译者注: 要设置鼠标式样也要有 X 窗口。gtk_widget_realize 使构件在没有 show 之前就形成 X 窗口。
   gtk_widget_realize (event_box);
   gdk_window_set_cursor (event_box->window, gdk_cursor_new (GDK_HAND1));
   gtk_widget_show (window);
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105171447785.PNG)
# 19.固定容器：gtk_fixed_new
```cpp
#include <gtk/gtk.h>
/* 我准备偷点懒,用一些全局变量储存固定容器里构件的位置。 */
gint x = 50;
gint y = 50;
/* 这个回调函数将按钮移动到固定容器的新位置。 */
void move_button( GtkWidget *widget, GtkWidget *fixed )
{
   x = (x + 30) % 300;
   y = (y + 50) % 300;
   gtk_fixed_move (GTK_FIXED (fixed), widget, x, y);
}
int main( int argc, char *argv[] )
{
/* GtkWidget 是构件的存储类型 */
   GtkWidget *window;
   GtkWidget *fixed;
   GtkWidget *button;
   gint i;
/* 初始化 */
   gtk_init (&argc, &argv);
/* 创建一个新窗口 */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "Fixed Container");
/* 为窗口的 "destroy" 事件设置一个信号处理函数 */
   g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
/* 设置窗口的边框宽度 */
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
/* 创建一个固定容器 */
   fixed = gtk_fixed_new ();
   gtk_container_add (GTK_CONTAINER (window), fixed);
   gtk_widget_show (fixed);
   for (i = 1 ; i <= 3 ; i++) {
/* 创建一个标签为"Press me"的新按钮 */
   button = gtk_button_new_with_label ("Press me");
/* 当按钮接收到 "clicked" 信号时,调用 move_button() 函数,并把这个固定
* 容器作为参数传给它 */
   g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (move_button), fixed);
/* 将按钮组装到一个固定容器的窗口中 */
   gtk_fixed_put (GTK_FIXED (fixed), button, i*10, i*10);
/* 最后一步是显示新建的构件 */
   gtk_widget_show (button);
   }
/* 显示窗口 */
   gtk_widget_show (window);
/* 进入事件循环 */
   gtk_main ();
   return 0;
}
```
点击按钮后，按钮位置发生变化，挺好玩的
![](https://img-blog.csdnimg.cn/20190105171848405.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 20.一个简单的frame：gtk_frame_new（不同于java的JFrame）
```cpp
#include <gtk/gtk.h>
int main( int argc, char *argv[] )
{
/* GtkWidget 是构件的存储类型 */
   GtkWidget *window;
   GtkWidget *frame;
/* 初始化 */
   gtk_init (&argc, &argv);
/* 创建一个新窗口 */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "Frame Example");
/* 在这里我们将 "destroy" 事件连接到一个回调函数 */
   g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
   gtk_widget_set_size_request (window, 300, 300);
/* 设置窗口的边框宽度 */
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
/* 创建一个框架 */
   frame = gtk_frame_new (NULL);
   gtk_container_add (GTK_CONTAINER (window), frame);
/* 设置框架的标签 */
   gtk_frame_set_label (GTK_FRAME (frame), "GTK Frame Widget");
/* 将标签定位在框架的右边 */
   gtk_frame_set_label_align (GTK_FRAME (frame), 1.0, 1.0);
/* 设置框架的风格 */
   gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_OUT);
   gtk_widget_show (frame);
/* 显示窗口 */
   gtk_widget_show (window);
/* 进入事件循环 */
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105172031136.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
## gtk_aspect_frame_new：更多选项的frame
```cpp
#include <gtk/gtk.h>
int main( int argc, char *argv[] )
{
   GtkWidget *window;
   GtkWidget *aspect_frame;
   GtkWidget *drawing_area;
   gtk_init (&argc, &argv);
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "Aspect Frame");
   g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
/* 创建一个比例框架,将它添加到顶级窗口中 */
   aspect_frame = gtk_aspect_frame_new ("2x1", /* label */
                                          0.5, /* center x */
                                          0.5, /* center y */
                                            2, /* xsize/ysize = 2 */
                                         FALSE /* ignore child's aspect */);
   gtk_container_add (GTK_CONTAINER (window), aspect_frame);
   gtk_widget_show (aspect_frame);
/* 添加一个子构件到比例框架中 */
   drawing_area = gtk_drawing_area_new ();
/* 要求一个 200×200 的窗口,但是比例框架会给出一个 200×100
* 的窗口,因为我们已经指定了 2×1 的比例值 */
   gtk_widget_set_size_request (drawing_area, 200, 200);
   gtk_container_add (GTK_CONTAINER (aspect_frame), drawing_area);
   gtk_widget_show (drawing_area);
   gtk_widget_show (window);
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105172357913.PNG)
# 21.滚动窗口：gtk_scrolled_window_new
```cpp
#include <stdio.h>
#include <gtk/gtk.h>
/* 创建一个"信息"列表 */
GtkWidget *create_list( void )
{
   GtkWidget *scrolled_window;
   GtkWidget *tree_view;
   GtkListStore *model;
   GtkTreeIter iter;
   GtkCellRenderer *cell;
   GtkTreeViewColumn *column;
   int i;
/* 创建一个新的滚动窗口(scrolled window),只有需要时,滚动条才出现 */
   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                   GTK_POLICY_AUTOMATIC,  GTK_POLICY_AUTOMATIC);
   model = gtk_list_store_new (1, G_TYPE_STRING);
   tree_view = gtk_tree_view_new ();
   gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), tree_view);
   gtk_tree_view_set_model (GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (model));
   gtk_widget_show (tree_view);
/* 在窗口中添加一些消息 */
   for (i = 0; i < 10; i++) {
      gchar *msg = g_strdup_printf ("Message #%d", i);
      gtk_list_store_append (GTK_LIST_STORE (model), &iter);
      gtk_list_store_set (GTK_LIST_STORE (model),
                          &iter,
                          0, msg,
                          -1);
      g_free (msg);
   }
   cell = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes ("Messages", cell, "text", 0, NULL);
   gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view),
                                GTK_TREE_VIEW_COLUMN (column));
   return scrolled_window;
}
/* 向文本构件中添加一些文本 - 这是当窗口被实例化(realized)时调用的回调函数。
* 我们也可以用 gtk_widget_realize 强行将窗口实例化,但这必须在它的层次关系
* 确定后(be part of a hierarchy)才行。 */
// 译者注: 构件的层次关系就是其 parent 被确定。将一个子构件加到一个容器中
// 时,其 parent 就是这个容器。层次关系被确定要求,其 parent 的 parent...也
// 确定了。顶级窗口可以不要 parent。只是我的经验理解。
void insert_text (GtkTextBuffer *buffer)
{
   GtkTextIter iter;
   gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
   gtk_text_buffer_insert (buffer, &iter,
                          "From: pathfinder@nasa.gov\n"
                          "To: mom@nasa.gov\n"
                          "Subject: Made it!\n"
                          "\n"
                          "We just got in this morning. The weather has been\n"
                          "great - clear but cold, and there are lots of fun sights.\n"
                          "Sojourner says hi. See you soon.\n"
                          " -Path\n", -1);
}
/* 创建一个滚动文本区域,用于显示一个"信息" */
GtkWidget *create_text( void )
{
   GtkWidget *scrolled_window;
   GtkWidget *view;
   GtkTextBuffer *buffer;
   view = gtk_text_view_new ();
   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   gtk_container_add (GTK_CONTAINER (scrolled_window), view);
   insert_text (buffer);
   gtk_widget_show_all (scrolled_window);
   return scrolled_window;
}
int main( int argc, char *argv[] )
{
   GtkWidget *window;
   GtkWidget *vpaned;
   GtkWidget *list;
   GtkWidget *text;
   gtk_init (&argc, &argv);
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "Paned Windows");
   g_signal_connect (G_OBJECT (window), "destroy",
   G_CALLBACK (gtk_main_quit), NULL);
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
   gtk_widget_set_size_request (GTK_WIDGET (window), 450, 400);
/* 在顶级窗口上添加一个垂直分栏窗口构件 */
   vpaned = gtk_hpaned_new ();
   gtk_container_add (GTK_CONTAINER (window), vpaned);
   gtk_widget_show (vpaned);
/* 在分栏窗口的两部分各添加一些构件 */
   list = create_list ();
   gtk_paned_add1 (GTK_PANED (vpaned), list);
   gtk_widget_show (list);
   text = create_text ();
   gtk_paned_add2 (GTK_PANED (vpaned), text);
   gtk_widget_show (text);
   gtk_widget_show (window);
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105172707879.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
## 又一个例子：滚动窗口中放按钮
```cpp
#include <stdio.h>
#include <gtk/gtk.h>
void destroy( GtkWidget *widget, gpointer data )
{
   gtk_main_quit ();
}
int main( int argc, char *argv[] )
{
   static GtkWidget *window;
   GtkWidget *scrolled_window;
   GtkWidget *table;
   GtkWidget *button;
   char buffer[32];
   int i, j;
   gtk_init (&argc, &argv);
/* 创建一个新的对话框窗口,滚动窗口就放在这个窗口上 */
   window = gtk_dialog_new ();
   g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
   gtk_window_set_title (GTK_WINDOW (window), "GtkScrolledWindow example");
   gtk_container_set_border_width (GTK_CONTAINER (window), 0);
   gtk_widget_set_size_request (window, 300, 300);
/* 创建一个新的滚动窗口。 */
   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 10);
/* 滚动条的出现方式可以是 GTK_POLICY_AUTOMATIC 或 GTK_POLICY_ALWAYS。
* 设为 GTK_POLICY_AUTOMATIC 将自动决定是否需要出现滚动条
* 而设为 GTK_POLICY_ALWAYS,将一直显示一个滚动条
* 第一个是设置水平滚动条,第二个是垂直滚动条 */
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
/* 对话框窗口内部包含一个 vbox 构件 */
   gtk_box_pack_start (GTK_BOX (GTK_DIALOG(window)->vbox), scrolled_window, TRUE, TRUE, 0);
   gtk_widget_show (scrolled_window);
/* 创建一个包含 10×10 个格子的表格 */
   table = gtk_table_new (10, 10, FALSE);
/* 设置 x 和 y 方向的行间间距为 10 像素 */
   gtk_table_set_row_spacings (GTK_TABLE (table), 10);
   gtk_table_set_col_spacings (GTK_TABLE (table), 10);
/* 将表格组装到滚动窗口中 */
   gtk_scrolled_window_add_with_viewport ( GTK_SCROLLED_WINDOW (scrolled_window), table);
   gtk_widget_show (table);
/* 简单地在表格中添加许多开关按钮以展示滚动窗口 */
   for (i = 0; i < 10; i++)
      for (j = 0; j < 10; j++) {
          sprintf (buffer, "button (%d,%d)\n", i, j);
          button = gtk_toggle_button_new_with_label (buffer);
          gtk_table_attach_defaults (GTK_TABLE (table), button, i, i+1, j, j+1);
          gtk_widget_show (button);
       }
/* 在对话框的底部添加一个"close"按钮 */
   button = gtk_button_new_with_label ("close");
   g_signal_connect_swapped (G_OBJECT (button), "clicked", G_CALLBACK (gtk_widget_destroy), window);
/* 让按钮能被缺省 */
   GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
   gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area), button, TRUE, TRUE, 0);
/* 将按钮固定为缺省按钮,只要按回车键就相当于点击了这个按钮 */
   gtk_widget_grab_default (button);
   gtk_widget_show (button);
   gtk_widget_show (window);
   gtk_main();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105172851135.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 22.按钮盒子：button box
```cpp
#include <gtk/gtk.h>
/* 用指定的参数创建一个按钮盒 */
GtkWidget *create_bbox( gint horizontal, char *title, gint spacing,
                        gint child_w, gint child_h, gint layout )
{
   GtkWidget *frame;
   GtkWidget *bbox;
   GtkWidget *button;
   frame = gtk_frame_new (title);
   if (horizontal)
      bbox = gtk_hbutton_box_new ();
   else
      bbox = gtk_vbutton_box_new ();
   gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);
   gtk_container_add (GTK_CONTAINER (frame), bbox);
/* 设置按钮盒的外观 */
   gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox), layout);
   gtk_box_set_spacing (GTK_BOX (bbox), spacing);
/*gtk_button_box_set_child_size (GTK_BUTTON_BOX (bbox), child_w, child_h);*/
   button = gtk_button_new_from_stock (GTK_STOCK_OK);
   gtk_container_add (GTK_CONTAINER (bbox), button);
   button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
   gtk_container_add (GTK_CONTAINER (bbox), button);
   button = gtk_button_new_from_stock (GTK_STOCK_HELP);
   gtk_container_add (GTK_CONTAINER (bbox), button);
   return frame;
}
int main( int argc, char *argv[] )
{
   static GtkWidget* window = NULL;
   GtkWidget *main_vbox;
   GtkWidget *vbox;
   GtkWidget *hbox;
   GtkWidget *frame_horz;
   GtkWidget *frame_vert;
/* 初始化 */
   gtk_init (&argc, &argv);
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "Button Boxes");
   g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
   main_vbox = gtk_vbox_new (FALSE, 0);
   gtk_container_add (GTK_CONTAINER (window), main_vbox);
   frame_horz = gtk_frame_new ("Horizontal Button Boxes");
   gtk_box_pack_start (GTK_BOX (main_vbox), frame_horz, TRUE, TRUE, 10);
   vbox = gtk_vbox_new (FALSE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
   gtk_container_add (GTK_CONTAINER (frame_horz), vbox);
   gtk_box_pack_start (GTK_BOX (vbox),
   create_bbox (TRUE, "Spread (spacing 40)", 40, 85, 20, 
                GTK_BUTTONBOX_SPREAD), TRUE, TRUE, 0);
   gtk_box_pack_start (GTK_BOX (vbox), create_bbox (TRUE, "Edge (spacing 30)", 30, 85, 20,
                       GTK_BUTTONBOX_EDGE), TRUE, TRUE, 5);
   gtk_box_pack_start (GTK_BOX (vbox),
   create_bbox (TRUE, "Start (spacing 20)", 20, 85, 20, GTK_BUTTONBOX_START), TRUE, TRUE, 5);
   gtk_box_pack_start (GTK_BOX (vbox), create_bbox (TRUE, "End (spacing 10)", 10, 85, 20,
                       GTK_BUTTONBOX_END), TRUE, TRUE, 5);
   frame_vert = gtk_frame_new ("Vertical Button Boxes");
   gtk_box_pack_start (GTK_BOX (main_vbox), frame_vert, TRUE, TRUE, 10);
   hbox = gtk_hbox_new (FALSE, 0);
   gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
   gtk_container_add (GTK_CONTAINER (frame_vert), hbox);
   gtk_box_pack_start (GTK_BOX (hbox), create_bbox (FALSE, "Spread (spacing 5)", 5, 85, 20,
                       GTK_BUTTONBOX_SPREAD), TRUE, TRUE, 0);
   gtk_box_pack_start (GTK_BOX (hbox), create_bbox (FALSE, "Edge (spacing 30)", 30, 85, 20,
                       GTK_BUTTONBOX_EDGE), TRUE, TRUE, 5);
   gtk_box_pack_start (GTK_BOX (hbox), create_bbox (FALSE, "Start (spacing 20)", 20, 85, 20,
                       GTK_BUTTONBOX_START), TRUE, TRUE, 5);
   gtk_box_pack_start (GTK_BOX (hbox), create_bbox (FALSE, "End (spacing 20)", 20, 85, 20,
                       GTK_BUTTONBOX_END), TRUE, TRUE, 5);
   gtk_widget_show_all (window);
/* 进入事件循环 */
   gtk_main ();
   return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105173144117.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
23.在aspect_frame中显示一个图片
```cpp
#include <gtk/gtk.h>
#define pixmap GdkPixmap
#define pixbuf GdkPixmap
void hello( GtkWidget *widget, gpointer   data )
{
    g_print ("Hello World\n");//temp(data );
}
gint delete_event( GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
    g_print ("delete event occurred\n");
    return FALSE;
}
void destroy( GtkWidget *widget, gpointer   data )
{
    gtk_main_quit ();
}
int main( int   argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *draw;
    GtkWidget *frame;
    
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
    frame = gtk_aspect_frame_new ("image:",0.5, 0.1,1, FALSE);
    gtk_container_add (GTK_CONTAINER (window), frame);
    draw = gtk_drawing_area_new();
    gtk_widget_set_size_request (draw, 800, 600);
    draw = gtk_image_new_from_file("batman.jpg");
   // g_signal_connect (G_OBJECT (draw), "clicked", G_CALLBACK (expose_event), NULL);
    
    gtk_container_add (GTK_CONTAINER (frame), draw);
    
    gtk_widget_show_all (window);
    gtk_main ();
    
    return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105173419680.PNG)
## 显示动态图：直接引用GIF，用多张图片显示动图效果
```cpp
/* 简单的动画 gif.c */
#include <gtk/gtk.h>
static GtkWidget *ourgif ;
static gchar *bmpfile[4] = {"l.bmp","r.bmp","u.bmp","d.bmp" };
gint i = 0 ;
void change_bmp ()
{ //依次更改图像实现动态效果
   gtk_image_set_from_file(GTK_IMAGE(ourgif),bmpfile[i]);
   i++ ;
   if ( i == 4 ) i = 0 ;
}
int main ( int argc , char* argv[])
{
   GtkWidget *window;
   GtkWidget *vbox;
   GtkWidget *hbox;
   GtkWidget *label;
   GtkWidget *image;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   g_signal_connect(G_OBJECT(window),"delete_event",
         G_CALLBACK(gtk_main_quit),NULL);
   gtk_window_set_title(GTK_WINDOW(window),"简单的动画");
   //gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window),10);
   vbox = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);
   label = gtk_label_new("直接引用GIF动画");
   gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,5);
   image = gtk_image_new_from_file("a.gif");
   gtk_box_pack_start(GTK_BOX(vbox),image,FALSE,FALSE,5);
   label = gtk_label_new("四幅静态的图像");
   gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,5);
   hbox = gtk_hbox_new(FALSE,0);
   gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
   image = gtk_image_new_from_file("l.bmp");
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
   image = gtk_image_new_from_file("r.bmp");
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
   image = gtk_image_new_from_file("u.bmp");
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
   image = gtk_image_new_from_file("d.bmp");
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
   label = gtk_label_new("通过时钟控制的动画");
   gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,5);
   ourgif = gtk_image_new_from_file("p1.bmp");
   gtk_box_pack_start(GTK_BOX(vbox),ourgif,FALSE,FALSE,5);
   gtk_timeout_add(150,(GtkFunction)change_bmp,NULL);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/20190105173613147.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 24.画图接口：gdk_draw_arc，gdk_draw_rectangle
```cpp
#include <gtk/gtk.h>
static void destroy( GtkWidget *widget, gpointer data )
{
    gtk_main_quit ();
}
static gboolean my_expose( GtkWidget      *da,
                           GdkEventExpose *event,
                           gpointer        data )
{
    GdkGC *gc1, *gc2, *gc;  /*gc1为矩形，gc2为文字*/
    GdkColor color;
    gc1 = gdk_gc_new( da->window );
    color.red = 10000;
    color.green = 20000;
    color.blue = 30000;
    gdk_gc_set_rgb_fg_color(gc1, &color );
    gc2 = gdk_gc_new( da->window );
    color.red = 1;
    color.green = 2;
    color.blue = 3;
    gdk_gc_set_rgb_fg_color( gc2, &color );
    gc = gc1;
    gdk_draw_rectangle( da->window, gc, TRUE,//fill or not
                           50, 50, 100, 100 );  /*（fx、fz、dx、dz）*/
    gc = gc2;
    gdk_draw_arc( da->window, gc, TRUE, //fill or not
                      50, 50, 100, 100, 0, 270 * 64 );  /*（fx、fz、dx、dz、0、arc）*/
 
    PangoLayout *layout = gtk_widget_create_pango_layout( da, "This is a cube \nand a circle." );
    PangoFontDescription *fontdesc = pango_font_description_from_string( "Luxi Mono 8" );
    pango_layout_set_font_description( layout, fontdesc ); 
    gdk_draw_layout( da->window, gc1, 50, 155, layout );
    pango_font_description_free( fontdesc );
    g_object_unref( layout );
 
    g_object_unref( G_OBJECT(gc1) );
    g_object_unref( G_OBJECT(gc2) );
   
    return TRUE;
}
int main( int  argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *da; //drawing area
    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect( G_OBJECT (window), "destroy",
                G_CALLBACK (destroy), NULL );
    
    da = gtk_drawing_area_new ();
    /* set a minimum size */
    gtk_widget_set_size_request( da, 200, 200 );
    gtk_container_add( GTK_CONTAINER(window), da );
    g_signal_connect( da, "expose_event", G_CALLBACK(my_expose), NULL );
    gtk_widget_show_all( window ); //simply show all widgets
    gtk_main();
    
    return 0;
}
```
![](https://img-blog.csdnimg.cn/20190105173848956.PNG)
# 25.文本文件编辑gtk_text_view_new
```cpp
#include <gtk/gtk.h> 
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
GtkWidget *text_view; 
GtkTextBuffer *buffer; 
GtkTextIter *Iter; 
void button_event(GtkWidget *widget,gpointer *data) 
{
    const gchar *text={"How are you!\n"};
    GtkTextIter start,end; 
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffer),&start,&end);
    gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffer),&end,text,strlen(text));
}
 int main(int argc,char *argv[ ]) 
{ 
    GtkWidget *window;
    GtkWidget *button; 
    GtkWidget *box; 
    gtk_init(&argc,&argv); 
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL); 
    gtk_widget_set_size_request(window,400,300); 
    g_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL); 
    box=gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),box);  
    text_view=gtk_text_view_new();
    gtk_widget_set_size_request(text_view,400,250); 
    gtk_box_pack_start(GTK_BOX(box),text_view,FALSE,FALSE,0); 
    buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    button=gtk_button_new_with_label("sure");
    gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,5);
 
    gtk_signal_connect(GTK_OBJECT(button),"clicked",GTK_SIGNAL_FUNC(button_event),NULL); 
    gtk_widget_show_all(window); 
    gtk_main();
}
```
![](https://img-blog.csdnimg.cn/20190105174140450.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 26.菜单/工具栏：gtk_menu_new
```cpp
/* 添加菜单 menu.c */
#include <gtk/gtk.h>
void on_menu_activate (GtkMenuItem* item,gpointer data)
{
   //g_print("菜单项 %s 被激活\n",(gchar*)data);
   g_print("Menuitem %s is pressed.\n",(gchar*)data);
}
int main (int argc, char* argv[])
{
   GtkWidget* window;
   GtkWidget* box;
   GtkWidget* menubar;
   GtkWidget* menu;
   GtkWidget* editmenu;
   GtkWidget* helpmenu;
   GtkWidget* rootmenu;
   GtkWidget* menuitem;
   GtkAccelGroup* accel_group ;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_size_request(window,400,300); 
   gtk_window_set_title(GTK_WINDOW(window),"菜单测试程序");
   g_signal_connect(G_OBJECT(window),"destroy",
      G_CALLBACK(gtk_main_quit),NULL);
   accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(window),accel_group);// AccelGroup
   box = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),box);
   menu = gtk_menu_new();//文件菜单
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_NEW,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("新建"));
      (gpointer)("New"));
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_OPEN,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("打开"));
      (gpointer)("Open"));
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_SAVE,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("保存"));
      (gpointer)("Save"));
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_SAVE_AS,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("另存为"));
      (gpointer)("Save As"));
   menuitem = gtk_separator_menu_item_new();
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_QUIT,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("退出"));
      (gpointer)("Exit"));
   rootmenu = gtk_menu_item_new_with_label(" 文件 ");
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),menu);
   menubar = gtk_menu_bar_new();
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
   rootmenu = gtk_menu_item_new_with_label(" 编辑 ");
   editmenu = gtk_menu_new();//编辑菜单
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_CUT,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("剪切"));
      (gpointer)("Cut"));
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_COPY,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("复制"));
      (gpointer)("Copy"));
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_PASTE,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("粘贴"));
      (gpointer)("Paste"));
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_FIND,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("查找"));
      (gpointer)("Search"));
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),editmenu);
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
   rootmenu = gtk_menu_item_new_with_label(" 帮助 ");
   helpmenu = gtk_menu_new();
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_HELP,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("帮助"));
      (gpointer)("Help"));
   menuitem = gtk_menu_item_new_with_label(" 关于... ");
   gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_menu_activate),//(gpointer)("关于"));
   (gpointer)("About"));
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),helpmenu);
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
   gtk_box_pack_start(GTK_BOX(box),menubar,FALSE,FALSE,0);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/20190105174554907.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 27.工具栏/条：gtk_toolbar_new
```cpp
/* 添加工具条 toolbar.c*/
#include <gtk/gtk.h>
void on_button_clicked (GtkButton* button,gpointer data)
{
   g_print("你点击的按钮是 %s \n",(gchar*)data);
}
int main (int argc, char* argv[])
{
   GtkWidget* window;
   GtkWidget* box ;
   GtkWidget* toolbar ;
   GtkWidget* entry ;
   GtkWidget* label ;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_size_request(window,700,100); 
   gtk_window_set_title(GTK_WINDOW(window),"添加工具条");
   g_signal_connect(G_OBJECT(window),"destroy",
      G_CALLBACK(gtk_main_quit),NULL);
   box = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),box);
   toolbar = gtk_toolbar_new();
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_NEW,
      "新建一个文件","新建",
      GTK_SIGNAL_FUNC(on_button_clicked),
      (gpointer)("新建"),-1);
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_OPEN,
      "打开一个文件","打开",
      GTK_SIGNAL_FUNC(on_button_clicked),
      (gpointer)("打开"),-1);
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_SAVE,
      "保存当前文件","保存",
      GTK_SIGNAL_FUNC(on_button_clicked),
      (gpointer)("保存"),-1);
   gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));
   label = gtk_label_new(" 文件名：");
   gtk_toolbar_append_widget(GTK_TOOLBAR(toolbar),label,
      "这是一个标签","标签");
   entry = gtk_entry_new();
   gtk_toolbar_append_widget(GTK_TOOLBAR(toolbar),entry,
      "这是一个录入条","录入");
   gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_CUT,
      "剪切","剪切",
      GTK_SIGNAL_FUNC(on_button_clicked),
      (gpointer)("剪切"),-1);
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_COPY,
      "复制","复制",
      GTK_SIGNAL_FUNC(on_button_clicked),
      (gpointer)("复制"),-1);
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_PASTE,
      "粘贴","粘贴",
      GTK_SIGNAL_FUNC(on_button_clicked),
      (gpointer)("粘贴"),-1);
   gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_QUIT,
      "退出","退出",
      GTK_SIGNAL_FUNC(on_button_clicked),
      (gpointer)("退出"),-1);
   gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_ICONS);
   gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar),
      GTK_ICON_SIZE_SMALL_TOOLBAR);
   gtk_box_pack_start(GTK_BOX(box),toolbar,FALSE,FALSE,0);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/20190105174816342.PNG)
# 28.浮动的工具条的菜单
```cpp
/* 浮动的工具条和菜单 handle.c */
#include <gtk/gtk.h>
//创建自己按钮的函数
GtkWidget* create_button (char *filename)
{
   GtkWidget* image;
   GtkWidget* button;
   image = gtk_image_new_from_file(filename) ;
   gtk_widget_show(image);
   button = gtk_button_new();
   gtk_container_add(GTK_CONTAINER(button),image) ;
   return button ;
}
//主函数
int main (int argc, char *argv[])
{
   GtkWidget* window;
   GtkWidget* vbox;
   GtkWidget* hbox;
   GtkWidget* box;
   GtkWidget* box1;
   GtkWidget* handle;
   GtkWidget* handle1;
   GtkWidget* menubar;
   GtkWidget* button1;
   GtkWidget* button2;
   GtkWidget* button3;
   GtkWidget* button4;
   GtkWidget* menu;
   GtkWidget* menuitem;
   GtkWidget* rootmenu;
   GtkTooltips* button_tips;
   char* title = "浮动的工具条和菜单";
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window),title);
   gtk_container_set_border_width(GTK_CONTAINER(window),5);
   g_signal_connect(G_OBJECT(window),"destroy",
      G_CALLBACK(gtk_main_quit),NULL);
   vbox = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);
   hbox = gtk_hbox_new(FALSE,0);
   gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,5);
   handle = gtk_handle_box_new();
   gtk_box_pack_start(GTK_BOX(hbox),handle,FALSE,FALSE,5);
   box = gtk_hbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(handle),box);
   button1 = create_button("win7.jpg") ;
   gtk_box_pack_start(GTK_BOX(box),button1,FALSE,FALSE,3);
   button2 = create_button("dir.jpg");
   gtk_box_pack_start(GTK_BOX(box),button2,FALSE,FALSE,3);
   button3 = create_button("save.gif");
   gtk_box_pack_start(GTK_BOX(box),button3,FALSE,FALSE,3);
   button4 = create_button("quit.gif");
   gtk_box_pack_start(GTK_BOX(box),button4,FALSE,FALSE,3);
   button_tips = gtk_tooltips_new();
   gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button1,
      "创建一个新文件","New");
   gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button2,
      "打开文件","Open");
   gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button3,
      "保存当前编辑的文件","Save");
   gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button4,
      "退出此程序","Quit");
   handle1 = gtk_handle_box_new();
   gtk_box_pack_start(GTK_BOX(hbox),handle1,FALSE,FALSE,5);
   box1 = gtk_hbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(handle1),box1);
   menubar = gtk_menu_bar_new();
   gtk_box_pack_start(GTK_BOX(box1),menubar,FALSE,FALSE,5);
   menu = gtk_menu_new();
   menuitem = gtk_menu_item_new_with_label("菜单项一");
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   menuitem = gtk_menu_item_new_with_label("菜单项二");
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   rootmenu = gtk_menu_item_new_with_label(" 文件 ");
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),menu);
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE ;
}
```
![](https://img-blog.csdnimg.cn/20190105175201726.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 29.一个文本编辑器：包含menu，toolbar，statusbar
```cpp
// A app //
///////////////////////////////////////////////////////////////////////////////////////////////////a
///////////////////////////////////////////////////////////////////////////////////////////////////a
///////////////////////////////////////////////////////////////////////////////////////////////////a
///////////////////////////////////////////////////////////////////////////////////////////////////a
/*a 回调函数文件 callbacks.c */
#include <gtk/gtk.h>
gboolean on_window_delete_event (GtkWidget* widget,
GdkEvent *event,
gpointer data)
{
   gtk_main_quit();
   return FALSE;
}
void on_file_new_activate (GtkMenuItem* menuitem, gpointer data)
{
   g_print("the file_new is clicked .\n");
}
void on_file_open_activate (GtkMenuItem* menuitem, gpointer data)
{
}
void on_file_save_activate (GtkMenuItem* menuitem, gpointer data)
{
}
void on_file_saveas_activate (GtkMenuItem* menuitem, gpointer data)
{
}
void on_file_exit_activate (GtkMenuItem* menuitem,gpointer data)
{
}
void on_edit_cut_activate (GtkMenuItem* menuitem,gpointer data)
{
}
void on_edit_copy_activate (GtkMenuItem* menuitem,gpointer data)
{
}
void on_edit_paste_activate (GtkMenuItem* menuitem,gpointer data)
{
}
void on_edit_selectall_activate (GtkMenuItem* menuitem,gpointer data)
{
}
void on_edit_find_activate (GtkMenuItem* menuitem,gpointer data)
{
}
void on_help_help_activate (GtkMenuItem* menuitem,gpointer data)
{
}
void on_help_about_activate (GtkMenuItem* menuitem,gpointer data)
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////a
///////////////////////////////////////////////////////////////////////////////////////////////////a
///////////////////////////////////////////////////////////////////////////////////////////////////a
///////////////////////////////////////////////////////////////////////////////////////////////////a
/* 界面代码文件 interface.c */
//#include <gtk/gtk.h>
//#include "callbacks.h"
GtkWidget* create_menu (GtkAccelGroup* accel_group,GtkWidget *window);
GtkWidget* create_toolbar (GtkWidget* window);
GtkWidget* create_window (void)
{
   GtkWidget* window;
   GtkWidget* text;
   GtkWidget* scrolledwin;
   GtkWidget* box;
   GtkWidget* statusbar;
   GtkWidget* menubar ;
   GtkWidget* toolbar ;
   GtkAccelGroup* accel_group ;
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window),"完整的应用程序窗口");
   gtk_window_set_default_size(GTK_WINDOW(window),400,300);
   accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(window),accel_group);
   box = gtk_vbox_new(FALSE,0);
   gtk_container_add (GTK_CONTAINER (window), box);
   menubar = create_menu(accel_group,window);
   gtk_box_pack_start(GTK_BOX(box),menubar,0,0,0);
   toolbar = create_toolbar(window);
   gtk_box_pack_start(GTK_BOX(box),toolbar,0,1,0);
   scrolledwin = gtk_scrolled_window_new(NULL,NULL);
   text = gtk_text_view_new();
   gtk_box_pack_start(GTK_BOX(box),scrolledwin,TRUE,TRUE,0);
   gtk_container_add(GTK_CONTAINER(scrolledwin),text);
   gtk_text_view_set_editable(GTK_TEXT_VIEW(text),TRUE);
   statusbar = gtk_statusbar_new();
   gtk_box_pack_start(GTK_BOX(box),statusbar,FALSE,FALSE,0);
   gtk_widget_show_all(window);
   return window;
}
GtkWidget* create_menu (GtkAccelGroup* accel_group,GtkWidget *window)
{
   GtkWidget* menubar;
   GtkWidget* menu;
   GtkWidget* editmenu;
   GtkWidget* helpmenu;
   GtkWidget* rootmenu;
   GtkWidget* menuitem;
   menu = gtk_menu_new();//文件菜单
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_NEW,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_file_new_activate),NULL);
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_OPEN,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_file_new_activate),NULL);
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_SAVE,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_SAVE_AS,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   menuitem = gtk_separator_menu_item_new();
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_QUIT,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(menu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_window_delete_event),NULL);
   rootmenu = gtk_menu_item_new_with_label(" 文件 ");
   gtk_widget_show(rootmenu);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),menu);
   menubar = gtk_menu_bar_new();
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
   rootmenu = gtk_menu_item_new_with_label(" 编辑 ");
   editmenu = gtk_menu_new();//编辑菜单
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_CUT,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_edit_cut_activate),NULL);
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_COPY,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_edit_copy_activate),NULL);
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_PASTE,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_edit_paste_activate),NULL);
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_FIND,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(editmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_edit_find_activate),NULL);
   gtk_widget_show(rootmenu);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),editmenu);
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
   rootmenu = gtk_menu_item_new_with_label(" 帮助 ");
   helpmenu = gtk_menu_new();
   menuitem = gtk_image_menu_item_new_from_stock
      (GTK_STOCK_HELP,accel_group);
   gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_help_help_activate),NULL);
   menuitem = gtk_menu_item_new_with_label(" 关于 ");
   gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu),menuitem);
   g_signal_connect(G_OBJECT(menuitem),"activate",
      G_CALLBACK(on_help_about_activate),NULL);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(rootmenu),helpmenu);
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar),rootmenu);
   return menubar ;
}
GtkWidget* create_toolbar (GtkWidget* window)
{
   GtkWidget* toolbar ;
   toolbar = gtk_toolbar_new();
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),
      GTK_STOCK_NEW,
      "创建一个新文件","新建",
      GTK_SIGNAL_FUNC(on_file_new_activate),
      window,-1);
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),
      GTK_STOCK_OPEN,
      "打开一个文件","打开",
      GTK_SIGNAL_FUNC(on_file_open_activate),
      toolbar,-1);
   gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),
      GTK_STOCK_SAVE,
      "保存当前文件","保存",
      GTK_SIGNAL_FUNC(on_file_save_activate),
      toolbar,-1);
   return toolbar;
}
///////////////////////////////////////////////////////////////////////////////////////////////////a
///////////////////////////////////////////////////////////////////////////////////////////////////a
///////////////////////////////////////////////////////////////////////////////////////////////////a
///////////////////////////////////////////////////////////////////////////////////////////////////a
/* 主函数文件 main.c*/
//#include <gtk/gtk.h>
//#include "callbacks.h"
//#include "interface.h"
int main (int argc , gchar* argv[])
{
   GtkWidget * window ;
   gtk_init(&argc,&argv);
   window = create_window();
   g_signal_connect(G_OBJECT(window),"delete_event",
      G_CALLBACK(on_window_delete_event),NULL);
   gtk_widget_show(window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/20190105175658477.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 30.模拟90坦克游戏：在frame中移动一个图片，键盘控制走向
```cpp
/* 坦克游戏 tank.c */
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
enum _FORWARD { LEFT, UP, RIGHT, DOWN } ;
typedef enum _FORWARD Forward; //定义方向类型
void move (Forward fw);
void key_press (GtkWidget* widget,GdkEventKey *event,gpointer data);
static gchar* tank_file[4] = {"u.bmp","r.bmp","d.bmp","l.bmp"};
static GtkWidget *fixed;
static GtkWidget *tank_image;
gint i = 75 ;
gint j = 75 ;
Forward forward = UP ;//定义方向
void move (Forward fw)
{
   switch(fw)
   {
      case UP :
         j = j - 5 ;
         if ( j < 0 ) j = 400 ;
         gtk_image_set_from_file(GTK_IMAGE(tank_image),tank_file[0]);
         gtk_fixed_move(GTK_FIXED(fixed),tank_image,i,j);
         break;
      case DOWN :
         j = j + 5;
         if ( j > 400 ) j = 0 ;
         gtk_image_set_from_file(GTK_IMAGE(tank_image),tank_file[2]);
         gtk_fixed_move(GTK_FIXED(fixed),tank_image,i,j);
         break;
      case LEFT :
         i = i - 5;
         if ( i < 0 ) i = 400 ;
         gtk_image_set_from_file(GTK_IMAGE(tank_image),tank_file[3]);
         gtk_fixed_move(GTK_FIXED(fixed),tank_image,i,j);
         break;
      case RIGHT :
         i = i + 5;
         if ( i > 400 ) i = 0 ;
         gtk_image_set_from_file(GTK_IMAGE(tank_image),tank_file[1]);
         gtk_fixed_move(GTK_FIXED(fixed),tank_image,i,j);
         break;
   }
}
void key_press (GtkWidget* widget,GdkEventKey *event,gpointer data)
{
   switch(event->keyval)
   {
      case GDK_Up :
         forward = UP;
         move(forward);
         break;
      case GDK_Down :
         forward = DOWN;
         move(forward);
         break;
      case GDK_Left :
         forward = LEFT;
         move(forward);
         break;
      case GDK_Right :
         forward = RIGHT;
         move(forward);
         break;
   }
}
//主函数
int main (int argc, char *argv[])
{
   GtkWidget* window;
   GtkWidget* vbox;
   GtkWidget* bbox;
   GtkWidget* sep;
   GtkWidget* frame;
   GtkWidget* button;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window),"坦克游戏");
   gtk_container_set_border_width(GTK_CONTAINER(window),10);
   g_signal_connect(G_OBJECT(window),"destroy",
      G_CALLBACK(gtk_main_quit),NULL);
   g_signal_connect(G_OBJECT(window),"key_press_event",
      G_CALLBACK(key_press),NULL);////keyboard input
   vbox = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);
   frame = gtk_frame_new(NULL);
   gtk_frame_set_shadow_type(GTK_FRAME(frame),GTK_SHADOW_ETCHED_OUT);
   fixed = gtk_fixed_new();
   gtk_fixed_set_has_window(GTK_FIXED(fixed),TRUE);
   gtk_widget_set_size_request(fixed,400,400);
   gtk_container_add(GTK_CONTAINER(frame),fixed);
   gtk_box_pack_start(GTK_BOX(vbox),frame,TRUE,TRUE,5);
   sep = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(vbox),sep,FALSE,FALSE,5);
   bbox = gtk_hbutton_box_new();
   gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_END);
   gtk_box_pack_start(GTK_BOX(vbox),bbox,FALSE,FALSE,5);
   button = gtk_button_new_from_stock(GTK_STOCK_QUIT);
   g_signal_connect(G_OBJECT(button),"clicked",
      G_CALLBACK(gtk_main_quit),NULL);
   gtk_box_pack_end(GTK_BOX(bbox),button,FALSE,FALSE,5);
   tank_image = gtk_image_new_from_file(tank_file[0]);
   gtk_fixed_put(GTK_FIXED(fixed),tank_image,i,j);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE ;
}
```
![](https://img-blog.csdnimg.cn/20190105180001684.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 31.pixmap：
网址：[https://developer.gimp.org/api/2.0/gdk-pixbuf/gdk-pixbuf-creating.html](https://developer.gimp.org/api/2.0/gdk-pixbuf/gdk-pixbuf-creating.html)
我暂时没找到一个pixmap的图像，等找到以后在更新
```cpp
/* 图像的直接引用 pixmap.c */
#include <gtk/gtk.h>
#include "a.h" //这里买年包含了一个pixmap图像
int main ( int argc , char* argv[])
{
   GtkWidget *window;
   GtkWidget *vbox;
   GtkWidget *frame;
   GtkWidget *image;
   GdkPixbuf *pixbuf;
   GdkPixbuf *pix1,*pix2,*pix3;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   g_signal_connect(G_OBJECT(window),"delete_event",
      G_CALLBACK(gtk_main_quit),NULL);
   gtk_window_set_title(GTK_WINDOW(window),"图像的直接引用");
   gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window),10);
   vbox = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);
   pixbuf = gdk_pixbuf_new_from_file("b.png",NULL);
   frame = gtk_frame_new("这是一幅完整的图像");
   image = gtk_image_new_from_pixbuf(pixbuf);
   gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,5);
   gtk_container_add(GTK_CONTAINER(frame),image);
   pix1 = gdk_pixbuf_new_subpixbuf(pixbuf,280,40,40,40);
   frame = gtk_frame_new("从上图中分割出来的一幅图像");
   image = gtk_image_new_from_pixbuf(pix1);
   gtk_container_add(GTK_CONTAINER(frame),image);
   gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,5);
   pix2 =gdk_pixbuf_new_from_inline(22400+24,pieces_inline,TRUE,NULL);
   frame = gtk_frame_new("内建的一幅图像");
   image = gtk_image_new_from_pixbuf(pix2);
   gtk_container_add(GTK_CONTAINER(frame),image);
   gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,5);
   pix3 = gdk_pixbuf_new_subpixbuf(pix2,120,20,20,20);
   frame = gtk_frame_new("从内建图像中分割出来的一幅图像");
   image = gtk_image_new_from_pixbuf(pix3);
   gtk_container_add(GTK_CONTAINER(frame),image);
   gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,5);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
# 32.固定调节数值的按钮:gtk_spin_button_new_with_range
```cpp
/* 滚动按钮 spin.c */
#include <gtk/gtk.h>
int main (int argc, char* argv[])
{
   GtkWidget* window;
   GtkWidget* vbox;
   GtkWidget* frame;
   GtkWidget* spin;
   GtkWidget* label;
   GtkWidget* vvbox;
   GtkWidget *hbox,*vbox1,*vbox2,*vbox3;
   GtkAdjustment* adj1;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window),"滚动按钮");
   g_signal_connect(G_OBJECT(window),"destroy",
            G_CALLBACK(gtk_main_quit),NULL);
   gtk_container_set_border_width(GTK_CONTAINER(window),10);
   vbox = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);
   frame = gtk_frame_new("日历");
   gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,5);
   hbox = gtk_hbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(frame),hbox);
   gtk_container_set_border_width(GTK_CONTAINER(hbox),10);
   vbox1 = gtk_vbox_new(TRUE,0);
   gtk_box_pack_start(GTK_BOX(hbox),vbox1,TRUE,TRUE,5);
   label = gtk_label_new("年：");
   gtk_box_pack_start(GTK_BOX(vbox1),label,FALSE,FALSE,2);
   spin = gtk_spin_button_new_with_range(2019,2100,1);
   gtk_box_pack_start(GTK_BOX(vbox1),spin,FALSE,FALSE,2);
   vbox2 = gtk_vbox_new(TRUE,0);
   gtk_box_pack_start(GTK_BOX(hbox),vbox2,TRUE,TRUE,5);
   label = gtk_label_new("月：");
   gtk_box_pack_start(GTK_BOX(vbox2),label,FALSE,FALSE,2);
   spin = gtk_spin_button_new_with_range(1,12,1);
   gtk_box_pack_start(GTK_BOX(vbox2),spin,FALSE,FALSE,2);
   vbox3 = gtk_vbox_new(TRUE,0);
   gtk_box_pack_start(GTK_BOX(hbox),vbox3,TRUE,TRUE,5);
   label = gtk_label_new("日：");
   gtk_box_pack_start(GTK_BOX(vbox3),label,FALSE,FALSE,2);
   spin = gtk_spin_button_new_with_range(1,31,1);
   gtk_box_pack_start(GTK_BOX(vbox3),spin,FALSE,FALSE,2);
   frame = gtk_frame_new(NULL);
   gtk_box_pack_start(GTK_BOX(vbox),frame,FALSE,FALSE,5);
   vvbox = gtk_vbox_new(FALSE,0);
   gtk_container_add(GTK_CONTAINER(frame),vvbox);
   gtk_container_set_border_width(GTK_CONTAINER(vvbox),10);
   label = gtk_label_new("第一个滚动按钮,\n显示整数,范围: 0 - 100");
   adj1 = (GtkAdjustment *) gtk_adjustment_new (50.0, 0.0, 100.0, 1.0,  5.0, 5.0);
   spin = gtk_spin_button_new(adj1,5,1);
   gtk_box_pack_start(GTK_BOX(vvbox),label,FALSE,FALSE,3);
   gtk_box_pack_start(GTK_BOX(vvbox),spin,FALSE,FALSE,3);
   label = gtk_label_new("第二个滚动按钮,\n显示浮点数,范围: 0.1 - 1.50");
   spin = gtk_spin_button_new_with_range(0,9.9,0.1);
   gtk_box_pack_start(GTK_BOX(vvbox),label,FALSE,FALSE,3);
   gtk_box_pack_start(GTK_BOX(vvbox),spin,FALSE,FALSE,3);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/2019010518090654.PNG)
# 33.分隔面板：gtk_hpanel_new，gtk_vpanel_new（未编译）
```cpp
/* 分隔面板 panel.c */
#include <gtk/gtk.h>
int main (int argc, char* argv[])
{
   GtkWidget *window;
   GtkWidget *panel;
   GtkWidget *panel1;
   GtkWidget *button1;
   GtkWidget *button2;
   GtkWidget *button3;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window),"分隔面板测试程序");
   g_signal_connect(G_OBJECT(window),"destroy",
         G_CALLBACK(gtk_main_quit),NULL);
   panel = gtk_hpanel_new();
   gtk_container_add(GTK_CONTAINER(window),panel);
   gtk_widget_show(panel);
   button1 = gtk_button_new_with_label("按钮一");
   gtk_panel_add1(GTK_PANEL(panel),button1);
   gtk_widget_show(button1);
   panel1 = gtk_vpanel_new();
   gtk_panel_add2(GTK_PANEL(panel),panel1);
   gtk_widget_show(panel1);
   button2 = gtk_button_new_with_label("按钮二");
   gtk_panel_add1(GTK_PANEL(panel1),button2);
   gtk_widget_show(button2);
   button3 = gtk_button_new_with_label("按钮三");
   gtk_panel_add2(GTK_PANEL(panel1),button3);
   gtk_widget_show(button3);
   gtk_widget_show(window);
   gtk_main();
   return FALSE;
}
```
# 34.会话窗口：gtk_button_new_from_stock
共四种：
GTK_STOCK_DIALOG_INFO， 
![](https://img-blog.csdnimg.cn/20190105181629800.PNG)
GTK_STOCK_DIALOG_ERROR，
![](https://img-blog.csdnimg.cn/20190105181643273.PNG)
GTK_STOCK_DIALOG_QUESTION，
![](https://img-blog.csdnimg.cn/20190105181656645.PNG)
GTK_STOCK_DIALOG_WARNING
![](https://img-blog.csdnimg.cn/20190105181708750.PNG)
```cpp
/* 消息对话框测试程序 message.c*/
#include <gtk/gtk.h>
static void on_button_clicked (GtkWidget* button,gint data)
{
   GtkWidget* dialog ;
   GtkMessageType type ;
   gchar *message;
   switch((int)data)
   {
      case 1 :
         message = "这是一个信息提示对话框．";
         type = GTK_MESSAGE_INFO ; break;
      case 2 :
         message = "这是一个错误提示对话框．";
         type = GTK_MESSAGE_ERROR ; break;
      case 3 :
         message = "这是一个问题提示对话框．";
         type = GTK_MESSAGE_QUESTION ; break;
      case 4 :
         message = "这是一个警告提示对话框．";
         type = GTK_MESSAGE_WARNING ; break;
   }
//
   dialog = gtk_message_dialog_new(NULL,
         GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
         type ,
         GTK_BUTTONS_OK,
         message);
   gtk_dialog_run(GTK_DIALOG(dialog));
   gtk_widget_destroy(dialog);
}
int main (int argc, char* argv[])
{
   GtkWidget* window;
   GtkWidget* frame;
   GtkWidget* box;
   GtkWidget* button1;
   GtkWidget* button2;
   GtkWidget* button3;
   GtkWidget* button4;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window),"创建消息框");
   g_signal_connect(G_OBJECT(window),"destroy",
         G_CALLBACK(gtk_main_quit),NULL);
   gtk_container_set_border_width(GTK_CONTAINER(window),20);
   frame = gtk_frame_new("四种消息对话框：");
   gtk_container_add(GTK_CONTAINER(window),frame);
   box = gtk_hbox_new(TRUE,0);
   gtk_container_add(GTK_CONTAINER(frame),box);
   gtk_container_set_border_width(GTK_CONTAINER(box),20);
   button1= gtk_button_new_from_stock(GTK_STOCK_DIALOG_INFO);
   gtk_box_pack_start(GTK_BOX(box),button1,FALSE,FALSE,5);
   g_signal_connect(G_OBJECT(button1),"clicked",
         G_CALLBACK(on_button_clicked),(gpointer)1);
   button2 = gtk_button_new_from_stock(GTK_STOCK_DIALOG_ERROR);
   gtk_box_pack_start(GTK_BOX(box),button2,FALSE,FALSE,5);
   g_signal_connect(G_OBJECT(button2),"clicked",
         G_CALLBACK(on_button_clicked),(gpointer)2);
   button3 = gtk_button_new_from_stock(GTK_STOCK_DIALOG_QUESTION);
   gtk_box_pack_start(GTK_BOX(box),button3,FALSE,FALSE,5);
   g_signal_connect(G_OBJECT(button3),"clicked",
         G_CALLBACK(on_button_clicked),(gpointer)3);
   button4 = gtk_button_new_from_stock(GTK_STOCK_DIALOG_WARNING);
   gtk_box_pack_start(GTK_BOX(box),button4,FALSE,FALSE,5);
   g_signal_connect(G_OBJECT(button4),"clicked",
         G_CALLBACK(on_button_clicked),(gpointer)4);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/20190105181609792.PNG)
# 35.字体选择：gtk_font_selection_dialog_new
```cpp
/* 选择字体 fontsel.c */
#include <gtk/gtk.h>
void on_font_select_ok (GtkWidget *button,GtkFontSelectionDialog *fs)
{
   GtkWidget *dialog;
   gchar message[1024];
   gchar *s = gtk_font_selection_dialog_get_font_name(fs);
   sprintf(message,"你选择的字体是：%s \n",s);
   dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            message,
            NULL);
   g_free(s);
   gtk_widget_destroy(GTK_WIDGET(fs));
   gtk_dialog_run(GTK_DIALOG(dialog));
   gtk_widget_destroy(dialog);
}
void on_button_clicked (GtkWidget *button,gpointer userdata)
{
   GtkWidget* dialog ;
   dialog = gtk_font_selection_dialog_new("请选择一种字体：");
   gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
   gtk_signal_connect(GTK_OBJECT(dialog),"destroy",
         GTK_SIGNAL_FUNC(gtk_widget_destroy),&dialog);
   gtk_signal_connect(GTK_OBJECT(GTK_FONT_SELECTION_DIALOG(dialog)->ok_button),
         "clicked",GTK_SIGNAL_FUNC(on_font_select_ok),
         GTK_FONT_SELECTION_DIALOG(dialog));
   gtk_signal_connect_object(GTK_OBJECT(GTK_FONT_SELECTION_DIALOG(dialog)->cancel_button),
         "clicked",GTK_SIGNAL_FUNC(gtk_widget_destroy),
         GTK_OBJECT(dialog));
   gtk_widget_show(dialog);
}
int main ( int argc , char* argv[])
{
   GtkWidget *window;
   GtkWidget *button;
   gtk_init(&argc,&argv);
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   g_signal_connect(G_OBJECT(window),"delete_event",
         G_CALLBACK(gtk_main_quit),NULL);
   gtk_window_set_title(GTK_WINDOW(window),"字体选择功能实现");
   gtk_window_set_default_size(GTK_WINDOW(window),500,100);
   //gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window),40);
   button = gtk_button_new_with_label("按下此按钮来选择字体");
   g_signal_connect(G_OBJECT(button),"clicked",
         G_CALLBACK(on_button_clicked),NULL);
   gtk_container_add(GTK_CONTAINER(window),button);
   gtk_widget_show(button);
   gtk_widget_show(window);
   gtk_main();
   return FALSE;
}
```
![](https://img-blog.csdnimg.cn/20190105182057535.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# 36.几个画图的骚操作：gtk_drawing_area_new
## 先给出三个例子的效果：
### 代码1效果
![](https://img-blog.csdnimg.cn/20190105182827962.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
### 代码2效果
![](https://img-blog.csdnimg.cn/20190105182839208.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
### 代码3效果
![](https://img-blog.csdnimg.cn/20190105182852303.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
### 代码1
```cpp
/* 绘图软件 brush.c */
#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
static GtkWidget *window = NULL;
static GdkPixmap *pixmap = NULL;
char FN[250]={"layer.txt"};
int nx=300;
int nz=300;
static gboolean scribble_configure_event (GtkWidget *widget,GdkEventConfigure *event,gpointer data)
{
   if (pixmap)
   g_object_unref (G_OBJECT (pixmap));
   pixmap = gdk_pixmap_new (widget->window,
   widget->allocation.width,
   widget->allocation.height,-1);
   gdk_draw_rectangle (pixmap,widget->style->white_gc,
                        TRUE,0,0,widget->allocation.width,
                        widget->allocation.height);
   return TRUE;
}
static gboolean scribble_expose_event (GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
   gdk_draw_drawable (widget->window,
                        widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                        pixmap,
                        event->area.x, event->area.y,
                        event->area.x, event->area.y,
                        event->area.width, event->area.height);
   return FALSE;
}
static void draw_brush (GtkWidget *widget, gdouble x, gdouble y)
{
   GdkRectangle update_rect;
   update_rect.x = x - 3;
   update_rect.y = y - 3;
   update_rect.width = 6;
   update_rect.height = 6;
   gdk_draw_rectangle (pixmap,
                        widget->style->black_gc,
                        TRUE,
                        update_rect.x, update_rect.y,
                        update_rect.width, update_rect.height);
                        gdk_window_invalidate_rect (widget->window,
                        &update_rect,
                        FALSE);
}
static gboolean scribble_button_press_event (GtkWidget *widget,GdkEventButton *event,gpointer data)
{
   if (pixmap == NULL)
      return FALSE;
   if (event->button == 1)
   {
      draw_brush (widget, event->x, event->y);
      printf("x=%.2f, z=%.2f\n", event->x, event->y*5.0);
   }
   return TRUE;
}
static gboolean scribble_motion_notify_event (GtkWidget *widget,GdkEventMotion *event,gpointer data)
{
   int x, y;
   GdkModifierType state;
   if (pixmap == NULL)
      return FALSE;
   gdk_window_get_pointer (event->window, &x, &y, &state);
   if (state & GDK_BUTTON1_MASK)
      draw_brush (widget, x, y);
   return TRUE;
}
static gboolean checkerboard_expose (GtkWidget *da, GdkEventExpose *event,gpointer data)
{
   gint i, j, xcount, ycount;
   GdkGC *gc1, *gc2;
   GdkColor color;
#define CHECK_SIZE 10
#define SPACING 2
   gc1 = gdk_gc_new (da->window);
   color.red = 30000;
   color.green = 0;
   color.blue = 30000;
   gdk_gc_set_rgb_fg_color (gc1, &color);
   gc2 = gdk_gc_new (da->window);
   color.red = 65535;
   color.green = 65535;
   color.blue = 65535;
   gdk_gc_set_rgb_fg_color (gc2, &color);
   xcount = 0;
   i = SPACING;
   while (i < da->allocation.width)
   {
      j = SPACING;
      ycount = xcount % 2;
      while (j < da->allocation.height)
      {
         GdkGC *gc;
         if (ycount % 2)
            gc = gc1;
         else
            gc = gc2;
         gdk_draw_rectangle (da->window, gc, TRUE,
                              i, j, CHECK_SIZE, CHECK_SIZE);
         j += CHECK_SIZE + SPACING;
         ++ycount;
      }
      i += CHECK_SIZE + SPACING;
      ++xcount;
   }
   g_object_unref (G_OBJECT (gc1));
   g_object_unref (G_OBJECT (gc2));
   return TRUE;
}
void writeTXT( GtkWidget *widget, gpointer data )
{
       FILE *fp=fopen(FN,"w");
       fprintf(fp,"rongtao\n");
       fclose(fp);
       g_print ("Write txt done.\n ");
       gtk_main_quit ();
}
int main (int argc,char* argv[])
{
   GtkWidget *frame;
   GtkWidget *vbox;
   GtkWidget *da;
   GtkWidget *label;
   GtkWidget *button;
   gtk_init(&argc,&argv);
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "绘图软件");
   g_signal_connect (G_OBJECT(window), "delete_event",
         G_CALLBACK (gtk_main_quit), NULL);
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);
   vbox = gtk_vbox_new (FALSE, 8);
   gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
   gtk_container_add (GTK_CONTAINER (window), vbox);
   label = gtk_label_new (NULL);
   gtk_label_set_markup (GTK_LABEL (label),"<u>绘图区域</u>");
   gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
   frame = gtk_frame_new (NULL);
   gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
   gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);
   da = gtk_drawing_area_new ();
   gtk_widget_set_size_request (da, nx, nz);
   gtk_container_add (GTK_CONTAINER (frame), da);
   g_signal_connect (da, "expose_event",
         G_CALLBACK (scribble_expose_event), NULL);
   g_signal_connect (da, "configure_event",
         G_CALLBACK (scribble_configure_event), NULL);
   g_signal_connect (da, "motion_notify_event",
         G_CALLBACK (scribble_motion_notify_event), NULL);
   g_signal_connect (da, "button_press_event",
         G_CALLBACK (scribble_button_press_event), NULL);
   gtk_widget_set_events (da, gtk_widget_get_events (da)
                           | GDK_LEAVE_NOTIFY_MASK
                           | GDK_BUTTON_PRESS_MASK
                           | GDK_POINTER_MOTION_MASK
                           | GDK_POINTER_MOTION_HINT_MASK);
      button = gtk_button_new_with_label ("Hello");
      g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (writeTXT), NULL);
      gtk_box_pack_start (GTK_BOX(vbox), button, TRUE, TRUE, 0);
   gtk_widget_show_all (window);
   gtk_main();
   return FALSE;
}
```
### 代码2
```cpp
#include <gtk/gtk.h>
static GdkPixmap *pixmap = NULL;
GdkGC *my_gc_red;
GdkColor color;
int my_configure_event(GtkWidget *widget , GdkEventConfigure *event)
{
      if(pixmap)
          g_object_unref(pixmap);
      pixmap = gdk_pixmap_new(widget->window , 
                  widget->allocation.width , 
                  widget->allocation.height , 
                  -1);
      gdk_draw_rectangle(pixmap , widget->style->white_gc , 
                  TRUE , 0 , 0 , 
                  widget->allocation.width , 
                  widget->allocation.height);
      my_gc_red = gdk_gc_new(widget->window);
      color.red = 30000;
      color.green = 0;
      color.blue = 30000;
      gdk_gc_set_rgb_fg_color(my_gc_red , &color);    
      return 0;
}
int my_expose_event(GtkWidget *widget , GdkEventExpose *event , gpointer data)
{
      gdk_draw_drawable(widget->window , 
              widget->style->fg_gc[GTK_WIDGET_STATE(widget)] ,
              pixmap , 0 , 0 , 0 , 0 , 
              widget->allocation.width , 
              widget->allocation.height);    
      return 0;
}
int my_draw(GtkWidget *widget , double x , double y , int type)
{
      switch(type){
          case 1:
              gdk_draw_rectangle(pixmap , 
                      widget->style->black_gc , 
                      FALSE ,     
                      x , y , 10 , 10);
              gtk_widget_queue_draw_area(widget , 
                          x , y , 10 , 10);
              break;
          case 2:
              gdk_draw_rectangle(pixmap , 
                      my_gc_red , 
                      TRUE ,     
                      x , y , 15 , 15);
              gtk_widget_queue_draw_area(widget , 
                          x , y , 15 , 15);
              break;
          case 3:
              gdk_draw_line(pixmap , my_gc_red , 
                      x , y , x + 15 , y - 15);
              gtk_widget_queue_draw_area(widget , 
                      x , y - 15 , 15 , 15);
              break;
          default:
              printf("....\n");
              break;
      }
    
      return 0;
}
int my_button_press_event(GtkWidget *widget , GdkEventButton *event)
{
      my_draw(widget , event->x , event->y , event->button);    
      printf("x=%lf y=%lf\n" , event->x , event->y);
      return 0;    
}
int my_clear(GtkWidget *widget , GtkWidget *area)
{
      gdk_draw_rectangle(pixmap , 
              area->style->white_gc , 
              TRUE , 0 , 0 , 
              area->allocation.width , 
              area->allocation.height);
      gtk_widget_queue_draw(area);
      return 0;
}
int my_quit(GtkWidget *widget , gpointer data)
{
      gtk_main_quit();
    
      return 0;
}
int main(int argc , char *argv[])
{
      GtkWidget *window;
      GtkWidget *drawing_area;
      GtkWidget *vbox;
      GtkWidget *bt_clear;
      GtkWidget *bt_quit;
      gtk_init(&argc , &argv);
    
      window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_widget_set_name(window , "Test drawing_area");
      gtk_widget_set_size_request(window , 300 , 200);
      gtk_container_set_border_width(GTK_CONTAINER(window) , 2);
      gtk_window_set_resizable(GTK_WINDOW(window) , FALSE);
      vbox = gtk_vbox_new(FALSE , 0);
      gtk_container_add(GTK_CONTAINER(window) , vbox);
      gtk_widget_show(vbox);
    
      g_signal_connect(G_OBJECT(window) , "destroy" , 
              G_CALLBACK(my_quit) , NULL);
      drawing_area = gtk_drawing_area_new();
      gtk_box_pack_start(GTK_BOX(vbox) , drawing_area , TRUE , TRUE , 0);
      gtk_widget_show(drawing_area);
      g_signal_connect(G_OBJECT(drawing_area) , "configure_event" , 
              G_CALLBACK(my_configure_event) , NULL);
      g_signal_connect(G_OBJECT(drawing_area) , "expose_event" , 
              G_CALLBACK(my_expose_event) , NULL);
      g_signal_connect(G_OBJECT(drawing_area) , "button_press_event" , 
              G_CALLBACK(my_button_press_event) , NULL);
      gtk_widget_set_events(drawing_area , GDK_BUTTON_PRESS_MASK);
    
      bt_clear = gtk_button_new_with_label("clear");
      gtk_box_pack_start(GTK_BOX(vbox) , bt_clear , FALSE , FALSE , 0);
      g_signal_connect(G_OBJECT(bt_clear) , "clicked" , 
              G_CALLBACK(my_clear) , drawing_area);
      gtk_widget_show(bt_clear);
    
      bt_quit = gtk_button_new_with_label("quit");
      gtk_box_pack_start(GTK_BOX(vbox) , bt_quit , FALSE , FALSE , 0);
      g_signal_connect(G_OBJECT(bt_quit) , "clicked" , 
              G_CALLBACK(my_quit) , window);
      gtk_widget_show(bt_quit);
      gtk_widget_show(window);
      gtk_main();
      return 0;
}
```
### 代码3
```cpp
/* GTK - The GIMP Toolkit
* Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*/
/*
* Modified by the GTK+ Team and others 1997-2000.   See the AUTHORS
* file for a list of people on the GTK+ Team.   See the ChangeLog
* files for a list of changes.   These files are distributed with
* GTK+ at ftp://ftp.gtk.org/pub/gtk/. 
*/
//#include <config.h>
#include <stdio.h>
#include "gtk/gtk.h"
/* Backing pixmap for drawing area */
static GdkPixmap *pixmap = NULL;
/* Information about cursor */
static gint cursor_proximity = TRUE;
static gdouble cursor_x;
static gdouble cursor_y;
/* Unique ID of current device */
static GdkDevice *current_device;
/* Erase the old cursor, and/or draw a new one, if necessary */
static void
update_cursor (GtkWidget *widget,   gdouble x, gdouble y)
{
   static gint cursor_present = 0;
   gint state = !current_device->has_cursor && cursor_proximity;
   if (pixmap != NULL)
     {
       if (cursor_present && (cursor_present != state ||
         x != cursor_x || y != cursor_y))
{
    gdk_draw_drawable (widget->window,
         widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
         pixmap,
         cursor_x - 5, cursor_y - 5,
         cursor_x - 5, cursor_y - 5,
         10, 10);
}
       cursor_present = state;
       cursor_x = x;
       cursor_y = y;
       if (cursor_present)
{
    gdk_draw_rectangle (widget->window,
          widget->style->black_gc,
          TRUE,
          cursor_x - 5, cursor_y -5,
          10, 10);
}
     }
}
/* Create a new backing pixmap of the appropriate size */
static gint
configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
   if (pixmap)
     g_object_unref (pixmap);
   pixmap = gdk_pixmap_new(widget->window,
      widget->allocation.width,
      widget->allocation.height,
      -1);
   gdk_draw_rectangle (pixmap,
         widget->style->white_gc,
         TRUE,
         0, 0,
         widget->allocation.width,
         widget->allocation.height);
   return TRUE;
}
/* Refill the screen from the backing pixmap */
static gint
expose_event (GtkWidget *widget, GdkEventExpose *event)
{
   gdk_draw_drawable (widget->window,
        widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
        pixmap,
        event->area.x, event->area.y,
        event->area.x, event->area.y,
        event->area.width, event->area.height);
   return FALSE;
}
/* Draw a rectangle on the screen, size depending on pressure,
    and color on the type of device */
static void
draw_brush (GtkWidget *widget, GdkInputSource source,
      gdouble x, gdouble y, gdouble pressure)
{
   GdkGC *gc;
   GdkRectangle update_rect;
   switch (source)
     {
     case GDK_SOURCE_MOUSE:
       gc = widget->style->dark_gc[GTK_WIDGET_STATE (widget)];
       break;
     case GDK_SOURCE_PEN:
       gc = widget->style->black_gc;
       break;
     case GDK_SOURCE_ERASER:
       gc = widget->style->white_gc;
       break;
     default:
       gc = widget->style->light_gc[GTK_WIDGET_STATE (widget)];
     }
   update_rect.x = x - 10 * pressure;
   update_rect.y = y - 10 * pressure;
   update_rect.width = 20 * pressure;
   update_rect.height = 20 * pressure;
   gdk_draw_rectangle (pixmap, gc, TRUE,
         update_rect.x, update_rect.y,
         update_rect.width, update_rect.height);
   gtk_widget_queue_draw_area (widget,
          update_rect.x, update_rect.y,
          update_rect.width, update_rect.height);
   gdk_window_process_updates (widget->window, TRUE);
}
static guint32 motion_time;
static void
print_axes (GdkDevice *device, gdouble *axes)
{
   int i;
  
   if (axes)
     {
       g_print ("%s ", device->name);
      
       for (i=0; i<device->num_axes; i++)
             g_print ("%g ", axes[i]);
       g_print ("\n");
     }
}
static gint
button_press_event (GtkWidget *widget, GdkEventButton *event)
{
   current_device = event->device;
   cursor_proximity = TRUE;
   if (event->button == 1 && pixmap != NULL)
     {
       gdouble pressure = 0.5;
       print_axes (event->device, event->axes);
       gdk_event_get_axis ((GdkEvent *)event, GDK_AXIS_PRESSURE, &pressure);
       draw_brush (widget, event->device->source, event->x, event->y, pressure);
      
       motion_time = event->time;
     }
   update_cursor (widget, event->x, event->y);
   return TRUE;
}
static gint
key_press_event (GtkWidget *widget, GdkEventKey *event)
{
   if ((event->keyval >= 0x20) && (event->keyval <= 0xFF))
     printf("I got a %c\n", event->keyval);
   else
     printf("I got some other key\n");
   return TRUE;
}
static gint
motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
   GdkTimeCoord **events;
   int n_events;
   int i;
   current_device = event->device;
   cursor_proximity = TRUE;
   if (event->state & GDK_BUTTON1_MASK && pixmap != NULL)
     {
       if (gdk_device_get_history (event->device, event->window, 
       motion_time, event->time,
       &events, &n_events))
{
    for (i=0; i<n_events; i++)
      {
        double x = 0, y = 0, pressure = 0.5;
        gdk_device_get_axis (event->device, events[i]->axes, GDK_AXIS_X, &x);
        gdk_device_get_axis (event->device, events[i]->axes, GDK_AXIS_Y, &y);
        gdk_device_get_axis (event->device, events[i]->axes, GDK_AXIS_PRESSURE, &pressure);
        draw_brush (widget,   event->device->source, x, y, pressure);
        print_axes (event->device, events[i]->axes);
      }
    gdk_device_free_history (events, n_events);
}
       else
{
    double pressure = 0.5;
    gdk_event_get_axis ((GdkEvent *)event, GDK_AXIS_PRESSURE, &pressure);
    draw_brush (widget,   event->device->source, event->x, event->y, pressure);
}
       motion_time = event->time;
     }
   if (event->is_hint)
     gdk_device_get_state (event->device, event->window, NULL, NULL);
   print_axes (event->device, event->axes);
   update_cursor (widget, event->x, event->y);
   return TRUE;
}
/* We track the next two events to know when we need to draw a
    cursor */
static gint
proximity_out_event (GtkWidget *widget, GdkEventProximity *event)
{
   cursor_proximity = FALSE;
   update_cursor (widget, cursor_x, cursor_y);
   return TRUE;
}
static gint
leave_notify_event (GtkWidget *widget, GdkEventCrossing *event)
{
   cursor_proximity = FALSE;
   update_cursor (widget, cursor_x, cursor_y);
   return TRUE;
}
void
input_dialog_destroy (GtkWidget *w, gpointer data)
{
   *((GtkWidget **)data) = NULL;
}
void
create_input_dialog (void)
{
   static GtkWidget *inputd = NULL;
   if (!inputd)
     {
       inputd = gtk_input_dialog_new ();
       g_signal_connect (inputd, "destroy",
            G_CALLBACK (input_dialog_destroy), &inputd);
       g_signal_connect_swapped (GTK_INPUT_DIALOG (inputd)->close_button,
            "clicked",
            G_CALLBACK (gtk_widget_hide),
            inputd);
       gtk_widget_hide (GTK_INPUT_DIALOG (inputd)->save_button);
       gtk_widget_show (inputd);
     }
   else
     {
       if (!GTK_WIDGET_MAPPED(inputd))
            gtk_widget_show(inputd);
       else
            gdk_window_raise(inputd->window);
     }
}
void
quit (void)
{
   gtk_main_quit ();
}
int
main (int argc, char *argv[])
{
   GtkWidget *window;
   GtkWidget *drawing_area;
   GtkWidget *vbox;
   GtkWidget *button;
   gtk_init (&argc, &argv);
   current_device = gdk_device_get_core_pointer ();
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_widget_set_name (window, "Test Input");
      gtk_widget_set_size_request(window , 300 , 320);
      gtk_container_set_border_width(GTK_CONTAINER(window) , 2);
      gtk_window_set_resizable(GTK_WINDOW(window) , FALSE);
   vbox = gtk_vbox_new (FALSE, 0);
   gtk_container_add (GTK_CONTAINER (window), vbox);
   gtk_widget_show (vbox);
   g_signal_connect (window, "destroy",
       G_CALLBACK (quit), NULL);
   /* Create the drawing area */
   drawing_area = gtk_drawing_area_new ();
   gtk_widget_set_size_request (drawing_area, 200, 200);
   gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);
   gtk_widget_show (drawing_area);
   /* Signals used to handle backing pixmap */
   g_signal_connect (drawing_area, "expose_event",
       G_CALLBACK (expose_event), NULL);
   g_signal_connect (drawing_area, "configure_event",
       G_CALLBACK (configure_event), NULL);
   /* Event signals */
   g_signal_connect (drawing_area, "motion_notify_event",
       G_CALLBACK (motion_notify_event), NULL);
   g_signal_connect (drawing_area, "button_press_event",
       G_CALLBACK (button_press_event), NULL);
   g_signal_connect (drawing_area, "key_press_event",
       G_CALLBACK (key_press_event), NULL);
   g_signal_connect (drawing_area, "leave_notify_event",
       G_CALLBACK (leave_notify_event), NULL);
   g_signal_connect (drawing_area, "proximity_out_event",
       G_CALLBACK (proximity_out_event), NULL);
   gtk_widget_set_events (drawing_area, GDK_EXPOSURE_MASK
     | GDK_LEAVE_NOTIFY_MASK
     | GDK_BUTTON_PRESS_MASK
     | GDK_KEY_PRESS_MASK
     | GDK_POINTER_MOTION_MASK
     | GDK_POINTER_MOTION_HINT_MASK
     | GDK_PROXIMITY_OUT_MASK);
   /* The following call enables tracking and processing of extension
      events for the drawing area */
   gtk_widget_set_extension_events (drawing_area, GDK_EXTENSION_EVENTS_ALL);
   GTK_WIDGET_SET_FLAGS (drawing_area, GTK_CAN_FOCUS);
   gtk_widget_grab_focus (drawing_area);
   /* .. And create some buttons */
   button = gtk_button_new_with_label ("Input Dialog");
   gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
   g_signal_connect (button, "clicked",
       G_CALLBACK (create_input_dialog), NULL);
   gtk_widget_show (button);
   button = gtk_button_new_with_label ("Quit");
   gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
   g_signal_connect_swapped (button, "clicked",
        G_CALLBACK (gtk_widget_destroy),
        window);
   gtk_widget_show (button);
   gtk_widget_show (window);
   gtk_main ();
   return 0;
}
```
# 最后寄语
希望在座各位在学习的道路上越走越远，完成从入门到放弃的质的转变。


