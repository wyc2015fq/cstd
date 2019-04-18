# GTK+图形化应用程序开发学习笔记（九）—菜单 - 深之JohnChen的专栏 - CSDN博客

2008年12月18日 22:26:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3310


**GTK+图形化应用程序开发学习笔记（九）—菜单**

一、建立菜单

菜单由菜单条（GtkMenuBar）和从菜单条或其他菜单下拉的菜单（GtkMenu）组成。在建立菜单条以前必须先建立应用程序的纵向组合框。

建立菜单分为下面几个步骤：

（1）建立菜单条（GtkMenuBar），并把它加到纵向组合框内。函数gtk_menu_bar_new建立GtkMenuBar构件。

1．

|名称:：|gtk_menu_bar_new|
|----|----|
|功能：|创建菜单条|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget *gtk_menu_bar_new(void);|
|参数：|无|
|返回值：|新的菜单条|





下面是建立菜单和将它放入纵向组合框的代码：

menuber=gtk_menu_bar_new();

gtk_box_pack_start(GTK_BOX(vbox),menubar,FALSE,TRUE,0);

gtk_widget_show(menubar);

（2）建立后的菜单条是空的，还需要增加菜单项。用gtk_menu_item_new_with_label建立带标号的菜单项。如：建立标号为“File”的菜单项用下面的语句：

meanFile=gtk_menu_item_new_item_new_with_label(“File”);

2．

|名称:：|gtk_menu_item_new_with_label|
|----|----|
|功能：|创建菜单项|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|GtkWidget* gtk_menu_item_new_with_label(const gchar *label);|
|参数：|label菜单项标号|
|返回值：|新的菜单项|





（3）在建立菜单项后，需要将它加到前面建立的菜单条中。gtk_menu_bar_append函数能够实现上面的功能。我们把上面的菜单项加到菜单条里面的代码如下：

gtk_menu_bar_append(GTK_MENU_BAR(menubar),MenuFile);

注意gtk_menu_bar_append和下面的gtk_menu_bar_append实际上都是宏，gtk_menu_bar_append的定义是这样如下：

#define gtk_menu_bar_append(menu,child)gtk_menu_shell_append((GtkMenuShell *)(menu),(child))


下面我们给出gtk_nenu_shell_append函数的原型：

3、

|名称:：|gtk_menu_shell_append|
|----|----|
|功能：|把菜单项加入菜单|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_menu_shell_append(GtkMenuShell *menu,GtkWidget *child);|
|参数：|menu菜单child菜单项|
|返回值：|无|






于是上面的代码也可以改成：

gtk_menu_shell_append(GTK_MENU_SHELL(menubar),MenuFile);



到这里我们就可以编写如下图的菜单了



![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/1633652362083750000.png)


/*menu1.h*/

#include <gtk/gtk.h>

GtkWidget *CreateMenuItem(GtkWidget *MenuBar,char *test);

int main(int argc,char *argv[ ])

{

GtkWidget *window;/*定义窗体*/

GtkWidget *MenuBar;/*定义菜单条*/

GtkWidget *box;/*定义组合框*/

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_usize(window,400,200);/*设置窗体大小*/

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_vbox_new(FALSE,0);/*创建纵向组合框*/

gtk_container_add(GTK_CONTAINER(window),box);/*把组合框加入窗体*/

MenuBar=gtk_menu_bar_new();/*创建菜单条*/

gtk_box_pack_start(GTK_BOX(box),MenuBar,FALSE,TRUE,0);/*把菜单条加入组合框*/

CreateMenuItem(MenuBar,”Fiel”);/*调用创建菜单项函数*/

CreateMenuItem(MenuBar,”Edit”);

CreateMenuItem(MenuBar,”View”);

CreateMenuItem(MenuBar,”Insert”);

CreateMenuItem(MenuBar,”Tool”);

gtk_widget_show(box);

gtk_widget_show(MenuBar);

gtk_widget_show(window);

gtk_main();

}

GtkWidget *CreateMenuItem(GtkWidget *MenuBar,char *test)

{

GtkWidget *MenuItem；

MenuItem=gtk_menu_item_new_with_label(test);/*创建菜单项*/

gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),MenItem);/*把菜单项加入菜单条，注意我们用gtk_menu_shell_append是为了menu2.c程序的方便*/

gtk_widget_show(MenuItem);

return MenuItem;

}



（4）建立子菜单。

到这里我们已经建立了一个简单的菜单条，但大多数菜单每个菜单向都包括一个子菜单。这席菜单只有在下拉菜单时才能看到。如：当选择“File”菜单项时，会在下面弹出一个子菜单。其中包括诸如“New”、“Open”、和“Save”等选项。

Gtk+的做法是先建立包含“New”、“Open”、“Save”等选项的新菜单，再用gtk_menu_item_set_submenu函数将新菜单和“File”菜单项联系在一起。

4、

|名称:：|gtk_menu_new|
|----|----|
|功能：|建立菜单|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_menu_append(GtkMenu menu,GtkWidget child);|
|参数：|menu菜单条child菜单项|
|返回值：|无|







(5)建立子菜单项，并把它加入到子菜单。建立子菜单项和建立父菜单项的函数都是gtk_menu_item_new_with_label.而把子菜单项加入到子菜单可以用gtk_menu_append.也可以用gtk_menu_shell_append函数。因为在gtk_menu_append函数内部也是调用gtk_menu_shell_append函数的。下面是gtk_menu_bar_append函数的定义。

#define gtk_menu_bar_append(menu,child)gtk_menu_shell_append((GtkMenuShell *)(menu),(child))



6）建立子菜单于父菜单项的联系

|名称:：|gtk_menu_item_set_submenu|
|----|----|
|功能：|建立子菜单和父菜单项的联系|
|头文件：|#include <gtk/gtk.h>|
|函数原形：|void gtk_menu_item_set_submenu(GtkMenuItem *menu_item,GtkWidget *submenu);|
|参数：|menu_item父菜单项submenu子菜单|
|返回值：|无|









下面是建立菜单的例子：

/*menu2.c*/

#include <gtk/gtk.h>

GtkWidget *CreateMenuItem(GtkWidget *MenuBar,char *test);

GtkWidget *CreateMenu(GtkWidget *MenuItem);

int main(int argc,char *argv[ ])

{

GtkWidget *window;/*定义窗体*/

GtkWidget *MenuBar;/*定义菜单条*/

GtkWidget *box;/*定义组合框*/

GtkWidget *MenuItemFile;

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_usize(window,400,200);/*设置窗体大小*/

gtk_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_vbox_new(FALSE,0);/*创建纵向组合框*/

gtk_container_add(GTK_CONTAINER(window),box);/*把组合框加入窗体*/

MenuBar=gtk_menu_bar_new();/*创建菜单条*/

gtk_box_pack_start(GTK_BOX(box),MenuBar,FALSE,TRUE,0);/*把菜单条加入组合框*/

MenuItemFile=CreateMenuItem(MenuBar,”Fiel”);/*调用创建菜单项函数*/

CreateMenu(MenuItemFile);/*调用创建子菜单函数*/

CreateMenuItem(MenuBar,”Edit”);

CreateMenuItem(MenuBar,”View”);

CreateMenuItem(MenuBar,”Insert”);

CreateMenuItem(MenuBar,”Tool”);

gtk_widget_show(box);

gtk_widget_show(MenuBar);

gtk_widget_show(window);

gtk_main();

}

GtkWidget *CreateMenuItem(GtkWidget *MenuBar,char *test)

{

GtkWidget *MenuItem；

MenuItem=gtk_menu_item_new_with_label(test);/*创建菜单项*/

gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),MenItem);/*把菜单项加入菜单条，注意我们用gtk_menu_shell_append是为了程序的方便*/

gtk_widget_show(MenuItem);

return MenuItem;

}

GtkWidget *CreateMenu(GtkWidget *MenuItem)

{

GtkWidget *Menu;/*定义子菜单*/

Menu=gtk_menu_new();/*创建子菜单*/

CreateMenuItem(Menu,”New”);/*调用创建菜单项函数*/

CreateMenuItem(Menu,”Open”);

CreateMenuItem(Menu,”Save”);

CreateMenuItem(Menu,”Exit”);

gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuItem),Menu);/*把父菜单项于子菜单联系起来*/

gtk_widget_show(Menu);

}

程序运行后出现下面的界面：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20081218/2633652362085000000.png)


到这里我们已经把怎样建立下拉菜单介绍完了。接下来我们菜单的信号处理.

二、菜单的信号处理

菜单项具有表示菜单项被选择的“activate（激活）”信号。对这个信号建立相应的回调函数，应用程序就能对用户选择的菜单选项作出响应了。

下面的例子对”exit”菜单项进行信号处理，当用户点击”exit”时程序退出。

/*menu3.c*/

#include <gtk/gtk.h>

GtkWidget *CreateMenuItem(GtkWidget *MenuBar,char *test);

GtkWidget *CreateMenu(GtkWidget *MenuItem);

int main(int argc,char *argv[ ])

{

GtkWidget *window;/*定义窗体*/

GtkWidget *MenuBar;/*定义菜单条*/

GtkWidget *box;/*定义组合框*/

GtkWidget *MenuItemFile;/*定义文件子菜单*/

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_usize(window,400,200);/*设置窗体大小*/

g_signal_connect(GTK_OBJECT(window),”destroy”,G_CALLBACK(gtk_main_quit),NULL);

box=gtk_vbox_new(FALSE,0);/*创建纵向组合框*/

gtk_container_add(GTK_CONTAINER(window),box);/*把组合框加入窗体*/

MenuBar=gtk_menu_bar_new();/*创建菜单条*/

gtk_box_pack_start(GTK_BOX(box),MenuBar,FALSE,TRUE,0);/*把菜单条加入组合框*/

MenuItemFile=CreateMenuItem(MenuBar,”Fiel”);/*调用创建菜单项函数*/

CreateMenu(MenuItemFile);/*调用创建子菜单函数*/

CreateMenuItem(MenuBar,”Edit”);

CreateMenuItem(MenuBar,”View”);

CreateMenuItem(MenuBar,”Insert”);

CreateMenuItem(MenuBar,”Tool”);

gtk_widget_show(box);

gtk_widget_show(MenuBar);

gtk_widget_show(window);

gtk_main();

}

GtkWidget *CreateMenuItem(GtkWidget *MenuBar,char *test)

{

GtkWidget *MenuItem；

MenuItem=gtk_menu_item_new_with_label(test);/*创建菜单项*/

gtk_menu_shell_append(GTK_MENU_SHELL(MenuBar),MenItem);/*把菜单项加入菜单条，注意我们用gtk_menu_shell_append是为了程序的方便*/

gtk_widget_show(MenuItem);

return MenuItem;

}

GtkWidget *CreateMenu(GtkWidget *MenuItem)

{

GtkWidget *Menu;/*定义子菜单*/

GtkWidget *Exit;/*定义exit子菜单项*/

Menu=gtk_menu_new();/*创建子菜单*/

CreateMenuItem(Menu,”New”);/*调用创建菜单项函数*/

CreateMenuItem(Menu,”Open”);

CreateMenuItem(Menu,”Save”);

Exit=CreateMenuItem(Menu,”Exit”);

g_signal_connect(GTK_OBJECT(Exit),”activate”,G_CALLBACK(gtk_menu_quit)mNULL);

gtk_menu_item_set_submenu(GTK_MENU_ITEM(MenuItem),Menu);/*把父菜单项于子菜单联系起来*/

gtk_widget_show(Menu);

}




