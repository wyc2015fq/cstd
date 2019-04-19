# linux Gtk基本内容学习 - BlueSky - CSDN博客
2015年09月30日 20:50:18[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：547
**CTK命名规则**
- 一般以gtk开头
- gtk_widget_代表这个函数应用于GtkWidget及所有的子类。gtk_container_代表这个函数可以应用于GtkContainer及所有子类。
- gtk_***_new，代表创建一个控件并且返回它的标号（编号），一般是返回指针地址；
- gtk_AAA_get_BBB，从AAA类型的控件的标识获得控件的BBB属性的值
- gtk_AAA_set_BBB，设置置顶的标识的类型为AAA的控件的BBB的属性的值。
g_singnal_connect(window,”destroy”,gtk_main_qiut,NULL) ：gtk程序退出的时候进程也结束 
gtk_widget_show(widget);  :显示组件 
gtk_widget_hide(widget) :隐藏 
gtk_window_set_resizable(window,resizable) ; 设置是否可以缩放 ，resizable为TRUE/FALSE 
gtk_window_set_position(window,GTK_WIN_POS_CENTER_ALWAYS) 显示在屏幕中央 
gtk_window_maximize(window) 最大化 
gtk_container_add(container,widget) 从container继承。比如将子控件widget添加到window中，window中只能有一个控件。
## 盒子布局
生成GtkBox控件来进行存放其他的控件 
gtk_box_new(int orientation , int spacing) ; 创建一个盒子布局，orientation方向：GTK_ORIENTATION_HORIZONTAL（水平的） 、 GTK_ORIENTATION_VERTICAL（垂直的）两个可选值；spacing：单元格之间的间隙。 
gtk_box_pack_start(int box , int child , boolean expand , boolean fill , int padding) ; 往盒子中添加其他元素。child被添加的控件，后三个参数一般设为FALSE,FALSE,0 即可。
## 网格布局
GtkGrid控件来进行存放需要布局的控件 
gtk_grid_new() ； 
gtk_grid_attach(grid , child , int left ,int right , int width , int height) : 将子控件child附加到grid中，表格行列动态增加。left、top为子控件的横纵编号（编号从0开始），width 、heigth为占用的列数、行数。**即是用left、top定位，用width、heigth设定占用几行几列。**
## 固定布局
gtk_fixed_new(); 创建一个固定布局容器，返回指针 
void gtk_fixed_put(fixed ,int x , int y) ; 把控件widget加入固定布局fixed中，左上角坐标为(x,y) 
这种方法很少用，因为无法自动适应屏幕大小。
