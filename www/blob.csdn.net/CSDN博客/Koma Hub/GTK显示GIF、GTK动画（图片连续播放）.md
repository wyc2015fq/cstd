# GTK显示GIF、GTK动画（图片连续播放） - Koma Hub - CSDN博客
2017年10月12日 20:35:30[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：267
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
   image = gtk_image_new_from_file(bmpfile[0]);
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
   image = gtk_image_new_from_file(bmpfile[1]);
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
   image = gtk_image_new_from_file(bmpfile[2]);
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
   image = gtk_image_new_from_file(bmpfile[3]);
   gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);
   label = gtk_label_new("通过时钟控制的动画");
   gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,5);
   ourgif = gtk_image_new_from_file("");
   gtk_box_pack_start(GTK_BOX(vbox),ourgif,FALSE,FALSE,5);
   gtk_timeout_add(90,(GtkFunction)change_bmp,NULL);
   gtk_widget_show_all(window);
   gtk_main();
   return FALSE;
}
```
编译与运行：
```
-bash-4.1$ gcc -o a GIF_moviePic.c `pkg-config --cflags --libs gtk+-2.0`
-bash-4.1$ ./a
```
