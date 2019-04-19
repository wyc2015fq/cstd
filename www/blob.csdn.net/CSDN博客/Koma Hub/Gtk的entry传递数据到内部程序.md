# Gtk的entry传递数据到内部程序 - Koma Hub - CSDN博客
2017年09月07日 19:45:57[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：315
                
需要两个程序，其中main函数在gtk程序中，而在这个main函数中调用外界函数，从而实现对外界函数的可视化。
   这里需要注意，当交换数据大于1时，再利用
```cpp
g_signal_connect(G_OBJECT(word),"clicked",G_CALLBACK(write_txt),NULL);
```
进行数据传递过程则只能传递一个gpointer，仔细考虑一下，则可以用全局变量进行数据传递，如下的entry_nx和entry_nz。
```cpp
// # #########
// # gtk主程序
// # #########
#include<gtk/gtk.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "model_VTI.c"
GtkWidget *entry_nx, *entry_nz;
gint nx = 0, nz = 0;
void debug(GtkWidget *window,gpointer data)
{
        gtk_main_quit();
}
void write_txt(GtkWidget *widget,gpointer data)
{
        // 从entey中获得字符串
        constchar*str_nx=gtk_entry_get_text(GTK_ENTRY((GtkWidget*)entry_nx));
        constchar*str_nz=gtk_entry_get_text(GTK_ENTRY((GtkWidget*)entry_nz));
        // 字符串转换为整形
        nx=strtod(str_nx,NULL);
        nz=strtod(str_nz,NULL);
        // 传递给外界函数
        model(nx,nz);
}
//主函数
int main(int argc,char* argv[])
{
        GtkWidget *window;
        GtkWidget *word;
        GtkWidget *label;
        GtkWidget *table;
 
        gtk_init(&argc,&argv);
 
        window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window),"VTImodel");
        gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(window),400,400);
        g_signal_connect(GTK_OBJECT(window),"delete_event",
                                   GTK_SIGNAL_FUNC(debug),NULL);
 
        table=gtk_table_new(3,2,FALSE);
        // 插入一个table
        gtk_container_add(GTK_CONTAINER(window),table);
        // 插入一个label，下同
        word=gtk_label_new("nx=");
        // 规定label在table中的位置
        gtk_table_attach_defaults(GTK_TABLE(table),word,0,1,0,1);
        // 从键盘输入一个entry，下同
        entry_nx=gtk_entry_new_with_max_length(10);
        // 将entry放入table中，下同
        gtk_table_attach_defaults(GTK_TABLE(table),entry_nx,1,2,0,1);
        word=gtk_label_new("nz=");
        gtk_table_attach_defaults(GTK_TABLE(table),word,0,1,1,2);
        entry_nz=gtk_entry_new_with_max_length(10);
        gtk_table_attach_defaults(GTK_TABLE(table),entry_nz,1,2,1,2);
        // 新建一个button，运行按钮
        word=gtk_button_new_with_label("RUN");
        // 点击按钮时运行对应函数
        gtk_table_attach_defaults(GTK_TABLE(table),word,1,2,2,3);
        gtk_widget_show_all(window);
        gtk_main();
        return 0;
}
```
```cpp
// # #########
// #  外界程序
// # #########
void model(int nx, int nz)
{
        // 进行对应的外界函数运算
        printf("nx=%d\nnz=%d\n",nx,nz);
}
```
![](https://img-blog.csdn.net/20170907194532974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
