# 【GTK】单文件实现任务管理器_Linux下GTK界面编程 - CD's Coding - CSDN博客





2015年01月06日 13:58:00[糖果天王](https://me.csdn.net/okcd00)阅读数：1530








Linux下一般来说提到界面编程

JAVA自然是Eclipse

C++就大概是Qt了吧

那么C呢？ 于是我去学了学UnixC和GTK，先放个可用代码放着吧~

啊对了，配环境，没错配环境



```
sudo apt-get install vim #使用vim来编写代码，当然您可以使用任何自己喜欢的编辑器。 
　　sudo apt-get install build-essential #这将安装gcc/g++/gdb/make 等基本编程工具。 
　　sudo apt-get install gnome-core-devel #这将安装 libgtk2.0-dev libglib2.0-dev 等开发相关的库文件。 
　　sudo apt-get install pkg-config #用于在编译GTK程序时自动找出头文件及库文件位置。 
　　sudo apt-get install devhelp #这将安装 devhelp GTK文档查看程序。 
　　sudo apt-get install libglib2.0-doc libgtk2.0-doc #这将安装 gtk/glib 的API参考手册及其它帮助文档。 
　　sudo apt-get install glade libglade2-dev #这将安装基于GTK的界面构造程序。
```
有的获取不到要去官网下载下来tar.gz自己解压缩安装哦~






解压小贴士：


tar –xvf file.tar //解压 tar包

tar -xzvf file.tar.gz //解压tar.gz

tar -xjvf file.tar.bz2   //解压 tar.bz2

tar –xZvf file.tar.Z   //解压tar.Z

unrar e file.rar //解压rar

unzip file.zip //解压zip



东西都有了之后呢~


看看效果？

![](https://img-blog.csdn.net/20150106140200734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150106140203713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150106140206350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150106140209969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



效果看完了咱——


就来看看代码研读研读吧~



```cpp
//   UnixC Based on GTK 
//   Compile Method: #gcc cdr –o cdos.c `pkg-config --cflags --libs gtk+-2.0
//   Exec Method: ./cdr

// Author :okcd00 @ CSDN_2014
#include "cdhd.h" 
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sys/stat.h>
#include <gdk/gdkkeysyms.h>

#define DENSITY 100  		//No. Density of Graph
#define PMAX    100   		//No. maxNum of Process

enum
{
	NAME_COLUMN,
	PID_COLUMN,	
	STATUS_COLUMN,
	CPU_COLUMN,	
	MEMORY_COLUMN,
	NP_COLUMNS
};

enum
{
	MOD_COLUMN,
	DEPEND_COLUMN,
	NM_COLUMNS
};


//Declare of GtkWidget
GtkWidget *vbox;
GtkWidget *hbox;
GtkWidget *label;
GtkWidget *frame;
GtkWidget *window;
GtkWidget *menubar;
GtkWidget *notebook;
GtkWidget *main_vbox;
GtkWidget *mem_label;
GtkWidget *swap_label;
GtkWidget *info_label;
GtkWidget *status_bar;
GtkWidget *ptree_view;
GtkWidget *mtree_view;
GtkWidget *cpu_draw_area;
GtkWidget *mem_draw_area;
GtkWidget *scrolled_window;
GtkWidget *prefresh_button , *pdelete_button; //Refresh & Kill Button

// Global Varities
gint cpu_graph[DENSITY];					//CPU Graphic Density
gint mem_graph[DENSITY];					//RAM Graphic Density
gdouble rate = 0;							//cpu Use_Rate
gdouble total = 0;					        //Curr. No.Process
gdouble fuser = 0;							//Curr. CPUinfo
gdouble ftotal = 0;							//Curr. CPUtotal
gdouble pfuser[PMAX];					   	//No. MaxProcess
GString   *info;
GdkPixmap *cgraph = NULL;					//pixmap_Pointer_CPU
GdkPixmap *mgraph = NULL;					//pixmap_Pointer_RAM

//Others
GtkAccelGroup *accel_group;
GtkListStore *process_store;
GtkItemFactory *item_factory;
GtkCellRenderer *renderer;//Show each colTitle
GtkTreeViewColumn *column;//tree view's No.Column

//函数声明
void about (GtkWidget *, gpointer);
void destroy_window (GtkWidget *, gpointer);
void sys_shutdown(GtkWidget *, gpointer);
void sys_reboot (GtkWidget *, gpointer);
void sys_halt (GtkWidget *, gpointer);
void about_author (GtkWidget *, gpointer);

gboolean handle_timeout (gpointer data);
gboolean load_graph_refresh (GtkWidget *widget);
gboolean get_cpu_rate (gpointer data);

void create_sys_status_page (void);
void create_process_page (void);
void create_sys_info_page (void);

void get_status_info (void);
void get_process_info (GtkListStore *);
void get_cpu_info (GString *);
void get_os_info (GString *);

void draw_cpu_load_graph (void);
void draw_mem_load_graph (void);

void prefresh_button_clicked (gpointer data);
void pdelete_button_clicked (gpointer data);
void mrefresh_button_clicked (gpointer data);
void mdelete_button_clicked (gpointer data);

gboolean cpu_configure_event (GtkWidget *, GdkEventConfigure *, gpointer);
gboolean cpu_expose_event (GtkWidget *, GdkEventExpose *, gpointer);
gboolean mem_configure_event (GtkWidget *, GdkEventConfigure *, gpointer);
gboolean mem_expose_event (GtkWidget *, GdkEventExpose *, gpointer);

gchar ABt[]="About us";
gchar ABc[]="     __<20125209>Chendian_&_<20125216>Xutianyuan_&_<20125212>Yangyeyu__     ";
void show_dialog (gchar *, gchar *);


static GtkItemFactoryEntry menu_items[] = {
		{"/_File", NULL, NULL, 0, "<Branch>"},
		{"/File/Quit", "<CTRL>Q", destroy_window, 0, "<Item>"},
		{"/_Option", NULL, NULL, 0, "<Branch>"},
		{"/Option/Shutdown", "<CTRL>U", sys_shutdown, 0 ,"<Item>"},
		{"/Option/Reboot", "<CTRL>R", sys_reboot, 0 ,"<Item>"},
		{"/Option/Halt", "<CTRL>H",  sys_halt, 0 ,"<Item>"},
		{"/_About",NULL,NULL,0,"<Branch>"},
		{"/About/AboutUs","<CTRL>B", about, 0, "<Item>"},
};

static gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);//No.Menu

int main (int argc, char **argv)
{

  gtk_set_locale ();//Set LocalEnvir Vars
  gtk_init (&argc, &argv);

  memset (cpu_graph, 50, sizeof (cpu_graph));//Alloc 50size
  memset (mem_graph, 50, sizeof (mem_graph));//Alloc 50size
  memset (pfuser, 0 ,sizeof (pfuser));

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "okcd00's Linux System Moniter");//Title
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 300);//Default Window_Size
  gtk_window_set_resizable (GTK_WINDOW (window), FALSE);//Lock the Window_Size
  g_signal_connect(G_OBJECT(window), "destroy",
  			G_CALLBACK(destroy_window), NULL);

  main_vbox =gtk_vbox_new (FALSE, 0);
  gtk_widget_show (main_vbox);
  gtk_container_add (GTK_CONTAINER (window), main_vbox);
  accel_group = gtk_accel_group_new ();
  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR,"<main>", accel_group);//Create Menu
  gtk_item_factory_create_items (item_factory, nmenu_items,menu_items, NULL);
  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
  menubar = gtk_item_factory_get_widget (item_factory, "<main>");
  gtk_widget_show (menubar);
  gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, FALSE, 0);

  notebook = gtk_notebook_new ();//New a notebook
  gtk_widget_show (notebook);
  gtk_box_pack_start (GTK_BOX (main_vbox), notebook, FALSE, FALSE, 0);


  status_bar = gtk_statusbar_new ();//New a status_bar
  gtk_widget_show (status_bar);
  gtk_statusbar_set_has_resize_grip (GTK_STATUSBAR (status_bar),TRUE); //Can be Resized
  gtk_box_pack_start (GTK_BOX (main_vbox), status_bar, TRUE, FALSE, 0);


  gtk_timeout_add (2000, (GtkFunction)handle_timeout, NULL);
  gtk_timeout_add (1000, (GtkFunction)load_graph_refresh, NULL); //graph refresh per second
  gtk_timeout_add (1000, (GtkFunction)get_cpu_rate, NULL); //CPUrate refresh per second

  create_process_page ();
  create_sys_status_page ();
  create_sys_info_page ();

  gtk_widget_show_all (window);
  gtk_main ();

  return 0;
}

void about(GtkWidget *widget, gpointer data)
{
	show_dialog(ABt,ABc);
}

void destroy_window (GtkWidget *widget, gpointer data)//QUIT 
{
	gtk_main_quit ();
}


void sys_shutdown (GtkWidget *widget, gpointer data)//Shotdown
{
	system ("shutdown -r now");
}

void sys_reboot (GtkWidget *widget, gpointer data)//Restart the PC 
{
	system ("reboot");
}

void sys_halt (GtkWidget *widget, gpointer data)//Halt
{
	system ("halt");
}




gboolean handle_timeout (gpointer data)//a series of operations by time
{
  gint page_num;

  page_num = gtk_notebook_get_current_page (GTK_NOTEBOOK (notebook));//Get Curr Page
  switch (page_num) {
		case 0:
			gtk_list_store_clear (process_store);//Clear treeview
			get_process_info (process_store);//Re-read Process_info
			break;
		default:
			break;
	}
	return TRUE;
}



gboolean load_graph_refresh (GtkWidget *widget)
{
  draw_cpu_load_graph ();
  draw_mem_load_graph ();
  return TRUE;
}


gboolean get_cpu_rate ( gpointer data)//Get CPU UseRate 
{
  int fd,i;
  gchar buffer[256];
  gchar *cpu_time[9];
  gchar *tmp;
  gchar *delim =  " ";
  gdouble cuser;
  gdouble ctotal;


  fd = open ("/proc/stat", O_RDONLY);//ReadFile
  read (fd, buffer, sizeof (buffer));
  close (fd);
  tmp = strstr (buffer, "cpu0");//Buffer ended with "cpu0"
  tmp--;
  *tmp = '\0';
  cpu_time[0] = strtok (buffer, delim);//split
  for (i = 1; i < 9 ; i++)  cpu_time[i] = strtok (NULL, delim);
  cuser = atoi (cpu_time[1]);//Change str to long
  ctotal = (cuser + atoi (cpu_time[2]) + atoi (cpu_time[3]) + atoi (cpu_time[4]));
  total = ctotal - ftotal;
  rate = (cuser - fuser) / total;
  fuser = cuser;
  ftotal = ctotal;
  return TRUE;
}


void create_sys_status_page ()//create_sys_status_page
{
  vbox = gtk_vbox_new (FALSE, 0);//Create colBox
  gtk_widget_show (vbox);
  gtk_container_add (GTK_CONTAINER (notebook), vbox);//Add it!

  frame = gtk_frame_new ("CPU");//CreateFrame
  gtk_widget_show (frame);
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);//Add it!

  cpu_draw_area = gtk_drawing_area_new ();//Add DrawPad
  gtk_widget_show (cpu_draw_area);
  gtk_drawing_area_size (GTK_DRAWING_AREA (cpu_draw_area), 400, 100);//Set DrawArea
  gtk_container_add (GTK_CONTAINER (frame), cpu_draw_area);

  g_signal_connect (cpu_draw_area, "expose_event",
  					G_CALLBACK (cpu_expose_event), NULL);
  g_signal_connect (cpu_draw_area, "configure_event",
  					G_CALLBACK (cpu_configure_event), NULL);


  frame = gtk_frame_new ("Memory");//CreateFrame
  gtk_widget_show (frame);
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 10);
  
  mem_draw_area = gtk_drawing_area_new ();
  gtk_widget_show (mem_draw_area);
  gtk_drawing_area_size (GTK_DRAWING_AREA (mem_draw_area), 400, 100);//Set DrawArea
  gtk_container_add (GTK_CONTAINER (frame), mem_draw_area);
  g_signal_connect (mem_draw_area, "expose_event",
  					G_CALLBACK (mem_expose_event), NULL);
  g_signal_connect (mem_draw_area, "configure_event",
  					G_CALLBACK (mem_configure_event), NULL);


  mem_label = gtk_label_new ("");//RAM Label
  gtk_widget_show (mem_label);
  gtk_box_pack_start (GTK_BOX (vbox), mem_label, FALSE, FALSE, 0);

  swap_label = gtk_label_new ("");//Exchange Label
  gtk_widget_show (swap_label);
  gtk_box_pack_start (GTK_BOX (vbox), swap_label, FALSE, FALSE, 10);

  label = gtk_label_new ("Status");//Add Title 
  gtk_widget_show (label);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
		  gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 1), label);
}


void create_process_page ()
{
  int i;
  gchar *col_name[5] = { "NAME", "PID" , "STATUS", "CPU" , "MEMORY"};

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox);
  gtk_container_add (GTK_CONTAINER (notebook), vbox);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_size_request (scrolled_window, 300, 300);
  gtk_widget_show (scrolled_window);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);


//Process info 
  process_store = gtk_list_store_new (NP_COLUMNS,
				G_TYPE_STRING,
				G_TYPE_STRING,
				G_TYPE_STRING,
				G_TYPE_STRING,
				G_TYPE_STRING);


  ptree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (process_store));
  //Add tree_view Show ProcessTree

  g_object_unref (G_OBJECT (process_store));

  gtk_widget_show (ptree_view);
  gtk_container_add (GTK_CONTAINER (scrolled_window), ptree_view);//Tree in scrolled_window


  for (i = 0; i < 5; i++) {
  		renderer = gtk_cell_renderer_text_new ();//Add cell_renderer_text to Show Text
  		column = gtk_tree_view_column_new_with_attributes (col_name[i],
							renderer,"text",i,NULL);
	    gtk_tree_view_append_column (GTK_TREE_VIEW (ptree_view), column);
  }


  get_process_info (process_store); //Getinfo and show

  hbox = gtk_hbox_new (FALSE, 0);//add parreling box
  gtk_widget_show (hbox);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, FALSE, 0);//Add to colBox

  prefresh_button = gtk_button_new ();//Create Refresh Button 
  gtk_widget_show (prefresh_button);
  gtk_widget_set_size_request (prefresh_button, 70, 30);//Set Button Size
  gtk_button_set_label (GTK_BUTTON (prefresh_button), "refresh");//SetText
  g_signal_connect (G_OBJECT (prefresh_button),"clicked",
  								G_CALLBACK(prefresh_button_clicked),
								NULL);//Function
  gtk_box_pack_start (GTK_BOX (hbox), prefresh_button, TRUE, FALSE, 0);//Add to hBox


  pdelete_button = gtk_button_new ();//Create Kill Button
  gtk_widget_show (pdelete_button);
  gtk_widget_set_size_request (pdelete_button, 70, 30);
  gtk_button_set_label (GTK_BUTTON (pdelete_button), "kill");
  g_signal_connect (G_OBJECT (pdelete_button),"clicked",
  								G_CALLBACK(pdelete_button_clicked),
								NULL);//Func:pdelete_button_clicked
  gtk_box_pack_start (GTK_BOX (hbox), pdelete_button, TRUE, FALSE, 0);//Add to hBox




  label = gtk_label_new ("Processes");
  gtk_widget_show (label);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
 		 gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), 0), label);
}

void create_sys_info_page (void)//Add Page
{
  vbox = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox);
  gtk_container_add (GTK_CONTAINER(notebook), vbox);

  frame = gtk_frame_new ("CPU Information:");
  gtk_widget_show (frame);
  gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 10);

  info = g_string_new ("");
  get_cpu_info (info);//info: Save CPUinfo

  info_label = gtk_label_new (info->str);//info_label Show infoString
  gtk_widget_show (info_label);
  gtk_container_add (GTK_CONTAINER(frame), info_label);

  frame = gtk_frame_new ("OS Information:");
  gtk_widget_show (frame);
  gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 30);

  info = g_string_new ("");
  get_os_info (info);//info: Save CPUinfo

  info_label = gtk_label_new (info->str);//info_label Show infoString
  gtk_widget_show (info_label);
  gtk_container_add (GTK_CONTAINER(frame), info_label);

  label = gtk_label_new ("SysInfo");
  gtk_widget_show (label);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook),
  		gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook),2), label);
}


//IMPORTANT <MOST> 2014.12.28 
void get_process_info (GtkListStore *store)
{
  DIR *dir;
  int fd, i,num = 0; 
  GtkTreeIter iter;
  struct dirent *entry;
  gdouble mem;
  gdouble srate;
  gdouble pcuser[PMAX];
  gchar *info[26];
  gchar buffer[128];
  gchar dir_buf[256];
  gchar *delim = " ";
  gchar mem_buffer[16];
  gchar rate_buffer[16];
  gchar state[2][10]={"running", "sleeping"};

  dir = opendir ("/proc");// Open/proc, dir: pointer to Return
  while ((entry = readdir (dir)) != NULL ) {
	if ((entry->d_name[0] >= '0') && (entry->d_name[0] <= '9')) {
		sprintf (dir_buf, "/proc/%s/stat", entry->d_name);//ReadFile
		fd = open (dir_buf, O_RDONLY);
		read (fd, buffer, sizeof (buffer));
		close (fd);

		info[0] =  strtok (buffer, delim);//Delim split
		for (i = 1; i < 26 ; i++) info[i] = strtok (NULL, delim);
		info[1]++;
		info[1] = strtok(info[1], ")");

		switch(info[2][0])
		{
		    case 'R': info[2] = state[0];break;
		    case 'S': info[2] = state[1];break;
		}

		pcuser[num] = atoi (info[13]);
		srate = (pcuser[num]-pfuser[num]) / (2 * total);//Percent Calc
                if(srate<0||srate>1){srate=0;}
		sprintf (rate_buffer, "%.2f%%",100 * srate);

		mem = atoi (info[22]);//Process' Using Percent
		mem = mem / (1024 * 1024);
		sprintf (mem_buffer, "%-.2f MB",mem);

		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter,
					NAME_COLUMN,info[1],
					PID_COLUMN,info[0],
					STATUS_COLUMN,info[2],
					CPU_COLUMN,rate_buffer,
					MEMORY_COLUMN,mem_buffer,
					-1);
		pfuser[num] = pcuser[num];
  		num = (num + 1 ) % PMAX;
		}
	}
  closedir (dir);
}




void get_cpu_info(GString *string)
{
  int i=0,j=0;
  FILE *file=NULL;
  gchar buffer[120];
  gchar cpu_info[3][88];
  gchar *tmp = NULL;

    file = fopen("/proc/cpuinfo", "r");
    while(!feof(file))
    {
        fgets(buffer, 120, file);
        if(tmp = strstr(buffer, "model name"))
        {
            tmp+=13;
            sprintf(cpu_info[i], "CPU %d:\t%s", i++, tmp);
        }
    }

    fclose(file);
    while(j<i) g_string_append (string, cpu_info[j++]);
    return;
}


void get_os_info (GString *string)//Get OSinfo
{
	int fd,i;
      gchar buffer[128];
      gchar *tmp,*start,*stop;
	gchar *os_info[8];
	gchar *delim = " ";

	fd = open ("/proc/version", O_RDONLY);//Read OS infoFile
	read(fd, buffer, sizeof (buffer));

	start = buffer;
	stop = strstr (buffer, "#");
	stop--;
	stop--;
	os_info[0] = strtok (buffer, delim);
	for (i = 1; i < 8; i++) {
		os_info[i] = strtok (NULL, delim);

	}

	g_string_append (string, "Os\t\t\t\t:");
	g_string_append (string, os_info[0]);
	g_string_append_c (string, '\n');
	g_string_append (string, "Release\t\t:");
    g_string_append (string, os_info[2]);
    g_string_append_c (string, '\n');
    g_string_append (string, "Domain\t\t:");
    g_string_append (string, os_info[3]);
  	g_string_append_c (string, '\n');
    g_string_append (string, "Gcc version\t:");
  	g_string_append (string, os_info[6]);
	g_string_append_c (string, ' ');
    g_string_append (string, os_info[7]);
    g_string_append_c (string, '\n');
}


void draw_cpu_load_graph ()
{
    if (cgraph == NULL)
        return;
   int i;
   gchar buffer[64];
   gint width,height,current,step;
   guint context_id;

   sprintf (buffer, "CPU using rate:%.2f%%",100 * rate);
   context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (status_bar),"cpu");

   gtk_statusbar_push (GTK_STATUSBAR (status_bar), context_id, buffer);

   //Clear PastGraph
   gdk_draw_rectangle (GDK_DRAWABLE (cgraph), window->style->white_gc, TRUE, 0, 0,
  										cpu_draw_area->allocation.width,
  										cpu_draw_area->allocation.height);
   width = cpu_draw_area->allocation.width;//Get Size
   height = cpu_draw_area->allocation.height;

   current = (int)(rate*(double)height);//Adjust Height
   cpu_graph[DENSITY-1] = height - current;  //Move Direction
   for ( i = 0 ; i < DENSITY - 1 ; i ++) {
	  	cpu_graph[i] = cpu_graph[i+1];
   }

   step = width/DENSITY;//stepsets
    GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(cgraph));
    GdkColor color;
    gdk_color_parse("#FFFF00", &color);
    gdk_gc_set_foreground(gc, &color);

   for (i = DENSITY - 1; i >= 1 ; i -- ) {
 		gdk_draw_line (cgraph, gc, i * step, cpu_graph[i],	
		 				(i - 1) * step, cpu_graph[i-1]  );
   }

   gtk_widget_queue_draw (cpu_draw_area);	//ReDraw
}

void draw_mem_load_graph ()
{
  if (mgraph == NULL) return;
  int fd,i;
  gint width,height,current,step;
  gint mem_total,mem_free;
  gchar buffer[540];
  gchar mem_tmp[19][20];
  gchar *start,*stop,*tmp;
  GString *string;

  fd = open ("/proc/meminfo", O_RDONLY);
  read (fd, buffer, sizeof (buffer));
  stop = buffer;
  close(fd);
  for (i = 0; i < 19; i++) 
  {
  	start = strstr (stop, ":");
  	start++;
  	stop = strstr (start, "kB");
  	stop--;
  	tmp = mem_tmp[i];
  	while (start != stop) *tmp++ = *start++;
  	*tmp = '\0';
  }
  mem_total = atoi (mem_tmp[0]);
  mem_free = atoi (mem_tmp[1]);


  string = g_string_new ("");//Refresh info_Num
  g_string_append (string, "Total Memory :");
  g_string_append (string, mem_tmp[0]);
  g_string_append (string, " kB");
  g_string_append (string, "   ");
  g_string_append (string, "Free Memory  :");
  g_string_append (string, mem_tmp[1]);
  g_string_append (string, " kB");
  g_string_append_c (string, '\n');
  gtk_label_set_text (GTK_LABEL (mem_label), string->str);
  g_string_free (string, TRUE);


  string = g_string_new ("");
  g_string_append (string, "Total Swap   :");
  g_string_append (string, mem_tmp[11]);
  g_string_append (string, " kB");
  g_string_append (string, "   ");
  g_string_append (string, "Free Swap    :");
  g_string_append (string, mem_tmp[12]);
  g_string_append (string, " kB");
  g_string_append_c (string, '\n');
  gtk_label_set_text (GTK_LABEL (swap_label), string->str);
  g_string_free (string, TRUE);


   //ClearGraph
  gdk_draw_rectangle (mgraph, window->style->white_gc, TRUE, 0, 0,
  									mem_draw_area->allocation.width,
  									mem_draw_area->allocation.height);

  width = mem_draw_area->allocation.width;//GetSize
  height = mem_draw_area->allocation.height;
  current = ((float)(mem_total-mem_free)/(float)mem_total) * height;//Adjust Size
  mem_graph[DENSITY-1] = height - current; //Move Dir 
  for ( i = 0 ; i < DENSITY - 1 ; i ++) {
	  	mem_graph[i] = mem_graph[i+1];
  }
  step = width / DENSITY;

    GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(mgraph));
    GdkColor color;
    gdk_color_parse("#0000EE", &color);
    gdk_gc_set_foreground(gc, &color);

  for (i = DENSITY - 1; i >= 1 ; i -- ) {
  		gdk_draw_line (mgraph, gc, i * step,
  	  						mem_graph[i], (i - 1) * step, mem_graph[i-1]);
  }
  gtk_widget_queue_draw ( mem_draw_area);
}

void prefresh_button_clicked (gpointer data)
{
  gtk_list_store_clear (process_store);
  get_process_info (process_store);
}

void pdelete_button_clicked (gpointer data)
{
  GtkTreeSelection *selection;
  GtkTreeModel *model;
  GtkTreeIter iter;
  gchar *pid;
  pid_t pid_num;

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(ptree_view));//Get Selection
  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		gtk_tree_model_get (model, &iter, PID_COLUMN, &pid, -1);//Get PID
  		pid_num = atoi (pid);

	    if(kill (pid_num, SIGTERM) == -1 ) {
			gchar *title = "ERROR";
			gchar *content = "Termination Failed,Check UID";
			show_dialog (title, content);
		}
	    gtk_list_store_clear (process_store);
	    get_process_info (process_store);
  }
}


gboolean cpu_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
  if (cgraph) {
  		g_object_unref (cgraph);
  }

   //Pixmap Create
  cgraph = gdk_pixmap_new (widget->window,
  					widget->allocation.width, widget->allocation.height,-1);

  //RePaint in Pixmap
  gdk_draw_rectangle (cgraph, widget->style->white_gc, TRUE, 0, 0,
					widget->allocation.width, widget->allocation.height);
  return TRUE;
}

gboolean cpu_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  gdk_draw_drawable (widget->window,
				widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		     	cgraph,
		     	event->area.x, event->area.y,
		     	event->area.x, event->area.y,
		      event->area.width, event->area.height);
  return TRUE;
}


//Graph Refreshing
gboolean mem_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
  if (mgraph) {
  		g_object_unref (mgraph);
  }

  mgraph = gdk_pixmap_new (widget->window,
  					widget->allocation.width, widget->allocation.height,-1);

  gdk_draw_rectangle (mgraph, widget->style->white_gc, TRUE, 0, 0,
					widget->allocation.width, widget->allocation.height);
  return TRUE;
}

gboolean mem_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  gdk_draw_drawable (widget->window,
			widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		     	mgraph,
		     	event->area.x, event->area.y,
		     	event->area.x, event->area.y,
		      event->area.width, event->area.height);
  return TRUE;
}



void show_dialog (gchar *title, gchar *content)
{
	GtkWidget *dialog;
	GtkWidget *label;
    dialog = gtk_dialog_new_with_buttons (title,
                                         GTK_WINDOW (window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_STOCK_CLOSE,
                                         GTK_RESPONSE_NONE,
                                         NULL);
	gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
	g_signal_connect_swapped (dialog,
                             "response",
                             G_CALLBACK (gtk_widget_destroy),
                             dialog);

	label = gtk_label_new (content);
	gtk_widget_show (label);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),
                      label);
	gtk_widget_show (dialog);
}
```







