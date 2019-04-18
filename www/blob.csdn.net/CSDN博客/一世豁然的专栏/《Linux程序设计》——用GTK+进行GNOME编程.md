# 《Linux程序设计》——用GTK+进行GNOME编程 - 一世豁然的专栏 - CSDN博客





2014年10月24日 15:49:19[一世豁然](https://me.csdn.net/Explorer_day)阅读数：3026








一、X视窗系统简介

 1、X视窗系统被分为硬件级和应用程序级组件，他们分别被称为X服务器和X客户端。这些组件使用X协议进行通信。




 2、X服务器

   1）、X服务器运行在用户的本地机器上，它在屏幕上完成底层的绘图操作。

   2）、X服务器直接与显卡交互，所以必须选择适合本机的X服务器，并配置好合适的分辨率、刷新率、颜色深度等。其配置文件名是xorg.conf或者Xfree86Config。





 3、X客户端

   1）、X客户端是以X视窗系统作为GUI的任何程序。它不需要与X服务器运行在同一机器上。





 4、X协议

   1）、X客户端和X服务器使用X协议进行通信，这使得客户端和服务端可以通过网络分离。





 5、Xlib库

   1）、Xlib是X客户端间接用于产生X协议消息的库。它提供一个非常底层的API供客户端在X服务器上绘制非常基本额元素，并影响最简单的输入。





 6、X工具包

   1）、X工具包是一个GUI库，X客户端可以利用它来极大地简化窗口、菜单和按钮等的创建。




 7、窗口管理器

   1）、负责定位屏幕上的窗口。

   2）、常用窗口管理器

      I、Metacity：GNOME桌面的默认窗口管理器。

      II、KWin：KDE桌面的默认窗口管理器。

      III、Openbox：旨在节约资源，用于较老的、较慢的系统。

      IV、Enlightenment：一个出色图形界面和效果的窗口管理器。




 8、创建GHI的其他方法——平台无关的窗口API







二、GTK+简介

 1、Glib类型系统

   1）、GTK+建立在一个可移植的C语言库Glib和GObject之上。


   2）、Glib和GObject提供了一组数据类型、函数和宏的标准替代集来进行内存管理和处理常见事物，从而实现跨平台开发。

   3）、Glib定义了一些常量：

**      #include<glib/gmacros.h>**

**           #define  FALSE  0**

**           #defien  TRUE  !FALSE**

       I、**gint、guint、gchar、guchar、glong、gulong、gfloat和gdouble**是标准C语言数据类型的简单替代。

       II、**gpointer与（void *）**同义。

       III、**gboolean**用于表示布尔类型的值，它是对int的一个包装。

       IV、**gint8、guint8、gint16、guint16、gint32和guint32**是保证字节长度额有符号和无符号类型。




 2、GTK+对象系统（略）




 3、GNOME介绍（略）




 4、安装GNOME/GTK+开发库（略）







三、事件、信号和回调函数

 1、GTK有属于它自己idea事件和时间监听器系统，他们被称为信号和可调函数。GTK+信号是当某件事发生时，由Gtkobject对象发出的。一个与信号连接，并且一旦信号被发

       出，它就会被调用的函数被称为回调函数。




 2、编写和链接回调函数

   1）、回调函数原型：

**void  a_callback_function(GTKWdget  *widget,  gpointer  usr_data);**

       I、第一个参数是指向发出信号的构建的指针。

       II、第二个参数是当你连接到回调函数时自己选择的一个任一指针。

   2）、链接回调函数

**gulong  g_signal_connect(gpointer  *object,  const  gchar  *name,  GCallback  func,  gpointer  user_data);**

      I、此函数传递的参数：构件、信号名、回调函数指针和你的任意指针。

      II、链接回调函数没有任何限制。你可以将多个信号连接到同一回调函数，也可以将多个回调函数链接到同一信号。







四、组装盒构件

 1、Gtkbox有两个主要的子类：

   1）、**GtkHBOX**是一个单行的横向组装盒构件。

   2）、**GtxVBOX**是一个单列的纵向组装盒构件。




 2、创建组装盒

** Gtkwidget*  gtk_hbox_new(gboolean  homogeneous,  gint  spacing);**

**    Gtkwidget*  gtk_vbox_new(gboolean  homogeneous,  gint  spacing);**

    I、homogeneous是一个布尔值，如果它被设置为TRUE，则强制盒中的每个构件都占据相同大小的空间，而不管每个构件的大小。

    II、spacing以像素为单位设置构件间的间距。




 3、添加构件

**   void  gtk_box_pack_start(Gtkbox  *box,  GtkWidget  *child,  gboolean  expand,  gboolean  fill,  guint  padding);**

**    void  gtk_box_pack_end(Gtkbox  *box,  GtkWidget  *child,  gboolean  expand,  gboolean  fill,  guint  padding);**

    I、gtk_box_pack_start向GtkHBOX的右边和GtxVBOX的底部增加构件，而gtk_box_pack_end向GtkHBOX的坐边和GtxVBOX的顶部增加构件。

    II、参数（P557  表16-1）







五、GTK+构件

 1、GtkWindow

   1）、GtkWindow是所有GTK+应用程序的基本元素。

   2）、GtkWindowAPI调用

**         GtkWidget*  gtk_window_new(GtkWindowType  type);**

**         void  gtk_window_set_title(GtkWindow *window,  const  gchar  *title);**

**         void  gtk_window_set_position(GtkWindow  *window,  GtkWindowPosition  position);**

**         void  gtk_window_set_default_size(GtkWindow  *window,  gint  width,  gint  height);**

**         void  gtk_window_resize(GtkWindow  *window,  gint  width,  gint  height);**

**         void  gtk_window_set_resizable(GtkWindow  *window,  gboolean  resizable);**

**         void  gtk_window_present(GtkWindow  *window);**

**         void  gtk_window_maximize(GtkWindow  *window);**

**         void  gtk_window_unmaximize(GtkWindow  *window);**

        I、gtk_window_new在内存中创建一个新的空白窗口。

        II、gtk_window_set_title函数通过向窗口管理器发出请求来改变标题栏文本。

        III、gtk_window_set_position控制窗口在屏幕上的初始位置。参数position取值（P560  表16-2）。

        IV、gtk_window_set_default_size按GTK+绘图单元设置屏幕中窗口的大小

        V、gtk_window_resize可以强制调制显示在屏幕窗口的大小。

        VI、gtk_window_set_resizable可以用来阻止用户拖拽窗口边框改变其大小，其参数resizable设为FALSE。

        VII、gtk_window_present可以确保窗口在屏幕上并且对用户是可见的，即它没有被最小化或隐藏。

        VIII、gtk_window_maximize强制最大化窗口。

        IX、gtk_window_minimize强制最小化窗口。




2、GtkEntry

   1）、GtkEntry是单行文本输入构件，它常用于输入简单的文本信息或主机名。它可被设置为使用星号来代替输入的字符。

   2）、GtkEntry函数

**   GtkWidget*  gtk_entry_new(void);**

**         GtkWidget*  gtk_entry_new_with_max_length(gint  max);**

**         void   gtk_entry_set_max_length(GtkEntry  *entry ,  gint  max);**

**         G_CONST_RETURN  gchar*  gtk_entry_get_text(GtkEntry  *entry);**

**         void  gtk_entry_set_text(GtkEntry  *entry,  const  gchar  *text);**

**         void  gtk_entry_append_text(GtkEntry  *entry,  const  gchar  *text);**

**         void  gtk_entry_prepend_text(GtkEntry  *entry,  const  gchar  *text);**

**         void  gtk_entry_set_visibility(GtkEntry  *entry,  gboolean  visible);**

**         void  gtk_entry_set_invisible_char(GtkEntry  *entry,  gchar   invch);**

**         void  gtk_entry_set_editable(GtkEntry  *entry,  gboolean  editable);**

        I、可以通过gtk_entry_new或固定最大文本输入长度的gtk_entry_new_with_max_length来创建一个GtkEntry。

        II、调用gtk_entry_get_text函数可以获取GtkEntry的内容，它将返回GtkEntry内部的一个const  char指针。

        III、可以通过_set_text、_append_text、_modift_text函数来手工设置或修改GtkEntry的内容。

        IV、gtk_entry_set_visibility函数将GtkEntry作为一个密码输入框使用，在显示字符的地方用星号代替，将其参数visible设为FALSE。

        V、不可见字符的替代符号可以用gtk_entry_set_invisible_char函数来改变。




3、GtkSpinButton

   1）、GtkSpinButton限制用户只能输入数字字符，可以为输入值设置上界和下界。

   2）、GtkSpinButton函数

**  GtkWidget*  gtk_spin_button_new(GtkAdiustment  *adjustment,  gdouble  climb_rate,  guint  digths);**

**         GtkWidget*  gtk_spin_button_new_with_range(gdouble  min,  gdouble  max,  gdouble  digits);**

**         void  gtk_spin_button_set_digits(GtkSpinButton  *spin_button,  guint  digits);**

**         void  gtk_spin_button_set_increments(GtkSpinButton  *spin_button,  gdouble  step,  gdouble  page);**

**         void  gtk_spin_button_set_range(GtkSpinButton  *spin_button,  gdouble  min,  gdouble  max);**

**         gdouble  gtk_spin_button_get_value(GtkSpinButton  *spin_button);**

**         gint  gtk_spin_button_get_value_as_int(GtkSpinButton  *spin_button);**

**         void  gtk_spin_button_set_value(GtkSpinButton  *spin_button,  gdouble  value);**

**         Gtkobject*  gtk_adjustment_new(gdouble  value,  gdouble  lower,  gdouble  upper,  gdouble  step_increment,  gdouble  page_increment,  gdouble  page_size);**


       I、要使用gtk_spin_button_new来创建一个GtkSpinButton，首先需要用gtk_adjustment_new创建一个GtkAdjustment对象。

       II、gtk_spin_button_new_with_range可以很方便的在创建GtkSpinButton时创建GtkAdjustment对象。

       III、gtk_spin_button_get_value可以很容易的读取到当前值。

       IV、gtk_spin_button_get_value_as_int可以获得一个整数值。




 4、GtkButton

   1）、GtkToggleButton

        I、GtkToggleButton和GtkButton几乎完全一样，但其还有重要区别：GtkToggleButton拥有状态，也就是说，它可以打开和关闭。

**   GtkWidget*  gtk_toggle_button_new(void);**

**          GtkWidget*  gtk_toggle_button_new_woth_label(const  gchar  *label);**

**          gboolean  gtk_toggle_button_get_active(GtkToogleButton  *tooggle_button);**

**          void  gtk_toggle_button_set_active(GtkToogleButton  *tooggle_button,  gboolean  is_active);**

          gtk_toggle_button_get_active和gtk_toggle_button_get_active可以用来读取和设置开关按钮的状态。一个TRUE返回值表明GtkToggleButton处于开状态。

   2）、GtkCheckButton

         I、GtkCheckButton显示带文本的复选框。

**      GtkWidget*  gtk_check_button_new(void);**

**           GtkWidget*  gtk_check_button_new_with_label(const  gchar  *label);    **

   3）、GtkRadioButton

         I、此按钮将同类型的按钮其他按钮分为一组，它允许一次只能从一组选项中选择一个。

** GtkWidget*  gtk_radio_button_new(GSList  *group);**

**           GtkWidget*  gtk_radio_button_new_from_widget(GtkRadioButton  *group);**

**           GtkWidget*  gtk_radio_button_new_with_label(GSList  *group,  const  gchar  *label);**

**           void  gtk_radio_button_group(GtkRadioButton  *radio_button,  GSList  *group);**

**           GSList*  gtk_radio_button_get_group(GtkRadioButton  *radio_butto**n);

   4）、GtkTreeView

         I、GtkTreeView构件族

              GtkTreeView：树和列表视图。

              GtkTreeViewColunmn：代表一个列表或树的列。

              GtkCellRenderer：控制绘图单元。

              GtkTreeModel：代表树和列的数据。

              前三个构成了视图，后一个是模型。







六、GNOME

 1、GNOME构件

   1）、GNOME库包含GNOME构件，它们扩展了GTK+，并且一些更容易使用的构件替换了GTK+中的部分构件。

   2）、在使用GNOME库之前，你必须在程序的开始初始化它。在纯GTK+程序中调用的是GTK_INIT，在此调用的是gnome_program_info。

**  GnomeProgram*  gnome_program_init(const  char  *app_id,  const  char  *app_version,  int  GnomeModuleInfo  *module_info,**

**                                                                                   intargc,  char  **argv,  const  char  *first_property_name, .  .  .);**




 2、GNOME菜单

  1、GNOMEEUIInfo结构

**      typedef  struct {**

**      GnomeUIInfoType  type;**

**      gchar  const  *label;**

**       gchar  const  *hint;**

**       gpointer  moreinfo;**

**       gpointer  user_data;**

**       gpointer  unused_data;**

**       GnomeUIPoxmapType  pixmap_type;**

**       gconstpointer  pixmap_info;**

**       guint  accelerator_key;**

**       GdkModifierType  ac_mods;**

**       GdkWidget  *widget;**

**       }GnomeUIInfo;**

      I、该结构中的第一项type定义了单元元素的类型，它可以使是个GnomeUIInfoType类型中的一个（P573  表16-3）。

      II、该结构中第二项和第三项定义了菜单项的文本和弹出提示。

      III、moreinfo的目的取决了type。对ITEM和TOGGLEITEM，它指向菜单项被激活调用的回调函数。对RADIOITEMS，它指向一个定义单选按钮组的GnomeUIInfo数据结构。

      IV、user_data是一个传递给回调函数的任一指针。

      V、pixmap_type和pixmap_info用于为菜单增加一个位图图标。

      VI、accelerator_key和ac_mods定义一个快捷键。

      VI、widget用于在内部保存由菜单创建函数指向的菜单项构件。




 2、GNOMEEUIInfo宏

   1）、第一组单独的菜单项：


**    #include<libgnomeui/libgnomeui.h>**

**        #define  GNOMEUIINFO_MENU_OPEN_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_SAVE_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_SAVE_AS_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_PRINT_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_PRINT_SETUP_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_CLOSE_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_EXIT_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_QUIT_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_CUT_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_COPY_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_PASTE_ITEM  (cb,  data)**

**        #define  GNOMEUIINFO_MENU_SELECT_ALL_ITEM  (cb,  data)**

** .  .  .  etc**


   2）、第二组用于定义顶层菜单定义，你只需传递参数即可：

**  #define  GNOMEUIINFO_MENU_FILE_TREE  (tree)**

**        #define  GNOMEUIINFO_MENU_EDIT_TREE  (tree)**

**       #define  GNOMEUIINFO_MENU_VIEW_TREE  (tree)**

**       #define  GNOMEUIINFO_MENU_SETTINGS_TREE  (tree)**

**       #define  GNOMEUIINFO_MENU_FILES_TREE  (tree)**

**       #define  GNOMEUIINFO_MENU_WINDOWS_TREE  (tree)**

**       #define  GNOMEUIINFO_MENU_HELP_TREE  (tree)**

**       #define  GNOMEUIINFO_MENU_GAME_TREE  (tree)**







八、对话框

 1、GtkDialog

   1）、GtkDialog窗口分为两个区域，一个放构件的内容，一个放底部的按钮。

**GtkWidget*  gtk_dialog_new_with_buttons(const  gchar  *title,  GtkWindoe  *parent,  GtkDialogFlags  flags,  const  gchar  *first_button_text, .  .  .);**

          I、此函数创建了一个完整的带有标题和按钮的对话框窗口。

          II、parent参数应指向应用程序的主窗口，这样GTK+才可以确保对话框是一直链接到主窗口的。

          III、flags参数决定了对话框可以拥有的属性组合：

**           GTK_DIALOG_MODAL;**

**               GTK_DIALOG_DESTROY_WITH_PARENT;**

**               GTK_DIALOG_NO_SEPAPATOR.**

          IV、其余参数是一个以NULL结尾的按钮和相应的相应代码列表




 2、模式对话框

        I、模式对话框强制用户首先相应，然后才能进行任何其他动作。它对用户将要做一件有严重后果的事，或报告错误和警告信息这些情况很有用。

        II、可以通过设置GTK_DIALOG_MODAL标记和调用gtk_widget_show函数，使一个对话框变为模式对话框。还可以调用gtk_dialog_run阻止程序的进一步执行，知道一个

              按钮被按下。

**       typedef  enum**

**              {**

**                    GTK_RESPONSE_NONE  =  -1;**

**                    GTK_RESPONSE_REJECT  =  -2;**

**                    GTK_RESPONSE_ACCEPT  =  -3;**

**                    GTK_RESPONSE_DELETE_EVEET  =  -4;**

**                    GTK_RESPONSE_OK  =  -5;**

**                    GTK_RESPONSE_CANCEL  =  -6;**

**                    GTK_RESPONSE_CLOSE  =  -7;**

**                    GTK_RESPONSE_YES  =  -8;**

**                    GTK_RESPONSE_NO  =  -9;**

**                    GTK_RESPONSE_APPLY  =  -10;**

**                    GTK_RESPONSE_HELP  =  -11;**

**               }GtkResponseType;**



 3、非模式对话框




 4、GtkMessageDialog

**  GtkWidget*  gtk_message_dialog_new(GtkWindow  *parent,  GtkDialogFlags  flags,  GtkMessageType  type, **

**                                                                                    GtkButtonsType  buttons,  const  gchar  *message_format,  .  .  .);**

           I、此函数创建了一个带有图标、标题和可配置按钮的完整对话框。

           II、参数type根据对话框的目的设置它的图标和标题。

           III、最常遇到的简单对话框有下面4种可能的类型值：

** GTK_MESSAGE_INFO;**

**                  GTK_MESSAGE_WARNING;**

**                  GTK_MESSAGE_QUESTION;**

**                  GTK_MESSAGE_ERROR;**

           IV、还可以选择一个GTK_MESSAGE_OTHER值，它用于前述对话框类型都不适用的情况。对于GtkMessageDialog，可以传递一个GtkButtonType而不是分别列出每个

                  按钮。（P580  表16-4）






