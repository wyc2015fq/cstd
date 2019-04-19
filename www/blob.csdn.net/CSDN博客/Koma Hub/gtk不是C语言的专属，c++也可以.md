# gtk不是C语言的专属，c++也可以 - Koma Hub - CSDN博客
2018年03月14日 20:15:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：525
原来以为gtk只能是C语言用，c++的话还是用Qt的GUI，今天无聊试了一下，原来c++也可以用gtk。
Demo.cpp内容个如下：
```
#include<iostream>
#include<string>
#include<cstring>
#include<cmath>
#include<complex>
#include<algorithm>
#include<vector>
#include<utility>
#include<cstddef> /*size_t*/
#include<bitset>
#include<assert.h>
#include<cstdlib>//abort()
#include<list>
#include<map>
#include<deque>
#include<set>
#include<iterator>
#include<gtk/gtk.h>
using namespace std;
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
int main(int argc, char*argv[])
/* Rong Tao 2018.03.14 */ 
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *table;
    GtkWidget *box;
    gchar *folder16 = "folder16.png";
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
    box = xpm_label_box (folder16, NULL);
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
    box = xpm_label_box (folder16, NULL);
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
    box = xpm_label_box (folder16, NULL);
    gtk_container_add (GTK_CONTAINER (button), box);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(select_file),_deta);
    gtk_table_attach_defaults(GTK_TABLE(table),button,7,8,3,4);
    gtk_widget_show_all(window);
    gtk_main ();
    return 0;
}
```
Makefile内容如下：
```
lgtk=`pkg-config --cflags --libs gtk+-2.0`
ALL:
	g++ Demo.cpp $(lgtk) -w
	./a.out
```
make：
```
$ make
g++ Demo.cpp `pkg-config --cflags --libs gtk+-2.0` -w
./a.out
```
运行结果：
![](https://img-blog.csdn.net/20180314201354853)
![](https://img-blog.csdn.net/20180314201457218)
另附folder16.png![](https://img-blog.csdn.net/20180314201518624)
