# QT/QTableView - u013366022的专栏 - CSDN博客
2016年08月22日 10:48:53[slitaz](https://me.csdn.net/u013366022)阅读数：2453

QTableWidget是QT对话框设计中常用的显示数据表格的控件。
学习QTableWidget就要首先看看QTableView控件（控件也是有”家世“的！就像研究人一样一样的），因为QTableWidget继承于类QTableView。
两者主要区别是QTableView可以使用自定义的数据模型来显示内容（也就意味着使用时先要通过setModel来绑定数据源），而QTableWidget则只能使用标准的数据模型。
QTableWidget单元格数据是QTableWidgetItem对象来实现的（即就是不需要数据源，单元格内的信息需要逐个填充即可）。
这主要由于QTableView类中有setModel成员函数，而到了QTableWidget类中，该成员函数变成了私有。
使用QTableWidget就离不开QTableWidgetItem。QTableWidgetItem用来表示表格中的其中一个单元格，整个表格都需要用逐个单元格对象QTableWidgetItem构建起来。
【2】QTableWidget控件属性
（1）禁止编辑表格
在默认情况下，表格里的字符是可以更改的。
比如双击一个单元格，就可以修改原来的内容，如果想禁止用户的这种操作，让这个表格对用户只读，可以这样：
```
1 ui.qtablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
```
（2）设置表格为选择整行
```
1 /*设置表格为整行选中*/
2 ui.qtablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
```
（3）设置单个选中和多个选中
单个选中意味着每次只可以选中一个单元格，多个就是相当于可以选择”一片“那种模式。
```
1 /*设置允许多个选中*/ 
2     ui.qtablewidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
```
（4）表格表头的显示与隐藏
对于水平或垂直方向的表头，如果不想显示可以用以下方式进行（隐藏/显示）设置：
```
1 ui.qtablewidget->verticalHeader()->setVisible(true);  
2 ui.qtablewidget->horizontalHeader()->setVisible(false);
```
（5）设置具体单元格中字体的对齐方式
```
1 ui.qtablewidget->item(0, 0)->setTextAlignment(Qt::AlignHCenter);
```
（6）设置具体单元格中字体格式
```
1 ui.qtablewidget->item(1, 0)->setBackgroundColor(QColor(0,60,10));   
2 ui.qtablewidget->item(1, 0)->setTextColor(QColor(200,111,100)); 
3 ui.qtablewidget->item(1, 0)->setFont(QFont("Helvetica"));
```
（7）设置具体单元格的值
```
1 ui.qtablewidget->setItem(1, 0, new QTableWidgetItem(str));
```
（8）把QTableWidgetItem对象内容转换为QString
```
1 QString str =ui.qtablewidget->item(0, 0)->data(Qt::DisplayRole).toString();
```
（9）具体单元格中添加控件
```
1 QComboBox *comBox = new QComboBox(); 
2 comBox->addItem("F"); 
3 comBox->addItem("M"); 
4 ui.qtablewidget->setCellWidget(0,3,comBox);
```
（11）合并单元格
```
1 //合并单元格的效果
2 ui.qtablewidget->setSpan(2, 2, 3, 2);
3 //第一个参数：要改变的单元格行数
4 //第二个参数：要改变的单元格列数
5 //第三个参数：需要合并的行数
6 //第四个参数：需要合并的列数
```
（12）具体单元格中插入图片
```
ui.qtablewidget->setItem(3, 2, new QTableWidgetItem(QIcon("images/music.png"), "Music"));
```
（13）设置显示网格
```
ui.qtablewidget->setShowGrid(true);//显示表格线
```
（14）设置滚动条
```
ui.qtablewidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
```
（15）设置列标签
```
1     //初始化界面
 2     QStringList  HStrList;
 3     HStrList.push_back(QString("name"));
 4     HStrList.push_back(QString("id"));
 5     HStrList.push_back(QString("age"));
 6     HStrList.push_back(QString("sex"));
 7     HStrList.push_back(QString("department"));
 8     
 9 
10     //设置行列数(只有列存在的前提下，才可以设置列标签)
11     int HlableCnt = HStrList.count();
12     ui.qtablewidget->setRowCount(10);
13     ui.qtablewidget->setColumnCount(HlableCnt);
14 
15     //设置列标签
16     ui.qtablewidget->setHorizontalHeaderLabels(HStrList);
```
（16）设置行和列的大小设为与内容相匹配
```
1 ui.qtablewidget->resizeColumnsToContents();  
2 ui.qtablewidget->resizeRowsToContents();
```
（17）设置字体
```
ui.qtablewidget->setFont(font);   //设置字体
```
（18）获取某一单元格的内容
```
1 QString strText = ui.qtablewidget->item(0, 0)->text();
```
在Qt的开发过程中，时常会用到表单（QTableWidget）这个控件，网上的资料不少，但是都是最基本的，有一些比较经常遇到的问题也说得不太清楚。所以，今天就在这里总结一下！
  以下为个人模拟Windows资源管理器的一个表单
**一、设置表单样式**
  table_widget->setColumnCount(4); //设置列数
  table_widget->horizontalHeader()->setDefaultSectionSize(150); 
  table_widget->horizontalHeader()->setClickable(false); //设置表头不可点击（默认点击后进行排序）
  //设置表头内容
  QStringList header;
  header<<tr("name")<<tr("last modify time")<<tr("type")<<tr("size");
  table_widget->setHorizontalHeaderLabels(header);
  //设置表头字体加粗
  QFont font = this->horizontalHeader()->font();
  font.setBold(true);
  table_widget->horizontalHeader()->setFont(font);
 
  table_widget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
  table_widget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  table_widget->verticalHeader()->setDefaultSectionSize(10); //设置行高
  table_widget->setFrameShape(QFrame::NoFrame); //设置无边框
  table_widget->setShowGrid(false); //设置不显示格子线
  table_widget->verticalHeader()->setVisible(false); //设置垂直头不可见
  table_widget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
  table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
  table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
  table_widget->horizontalHeader()->resizeSection(0,150); //设置表头第一列的宽度为150
  table_widget->horizontalHeader()->setFixedHeight(25); //设置表头的高度
  table_widget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
  table_widget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
  //设置水平、垂直滚动条样式
  table_widget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
  "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
  "QScrollBar::handle:hover{background:gray;}"
  "QScrollBar::sub-line{background:transparent;}"
  "QScrollBar::add-line{background:transparent;}");
  table_widget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
  "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
  "QScrollBar::handle:hover{background:gray;}"
  "QScrollBar::sub-line{background:transparent;}"
  "QScrollBar::add-line{background:transparent;}");
  好了，样式设置完成，效果如下所示：
![QTableWidget详解（样式、右键菜单、表头塌陷、多选等）](http://s9.sinaimg.cn/mw690/a6fb6cc9gdcb857550c68&690)
问题一：鼠标点击的选项会出现虚框，在Qt官网找到一篇博客专门介绍的，直接上代码！
  （1）实现如下一个类
  #include "no_focus_delegate.h"
  void NoFocusDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
  {
     QStyleOptionViewItem itemOption(option);
     if (itemOption.state & QStyle::State_HasFocus)
     {
        itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
     }
     QStyledItemDelegate::paint(painter, itemOption, index);
  }
  （2）表格构造中添加如下代码
   table_widget->setItemDelegate(new NoFocusDelegate());
![QTableWidget详解（样式、右键菜单、表头塌陷、多选等）](http://s8.sinaimg.cn/mw690/a6fb6cc9gdcb8988176b7&690)
   OK，虚线边框去除
问题二：当表格只有一行的时候，则表头会出现塌陷问题
![QTableWidget详解（样式、右键菜单、表头塌陷、多选等）](http://s5.sinaimg.cn/mw690/a6fb6cc9gdcb955da2c04&690)
   摸索了很长时间，才得以解决：
   //点击表时不对表头行光亮（获取焦点） 
   table_widget->horizontalHeader()->setHighlightSections(false); 
** 二、多选并获取所选行**
this->setSelectionMode(QAbstractItemView::ExtendedSelection);  //设置多选（可以Ctral+A全选Ctral+Shift多选）获取所选行号：
bool TableWidget::getSelectedRow(QSet&set_row)
{
    QList items = this->selectedItems();
    int item_count = items.count();
    if(item_count <= 0)
    {
        return false;
    }
    for(int i=0; i
    {
        //获取选中的行
        int item_row = this->row(items.at(i));
        set_row.insert(item_row);
    }
    return  true;
}
**三、操作表单（添加、删除行等）**
  （1）动态插入行
  int row_count = table_widget->rowCount(); //获取表单行数
  table_widget->insertRow(row_count); //插入新行
  QTableWidgetItem *item = new QTableWidgetItem();
  QTableWidgetItem *item1 = new QTableWidgetItem();
  QTableWidgetItem *item2 = new QTableWidgetItem();
  QTableWidgetItem *item3 = new QTableWidgetItem();
  //设置对应的图标、文件名称、最后更新时间、对应的类型、文件大小
  item->setIcon(icon); //icon为调用系统的图标，以后缀来区分   
  item->setText(name);
  item1->setText(last_modify_time);
  item2->setText(type); //type为调用系统的类型，以后缀来区分
  item3->setText(size);
  table_widget->setItem(row_count, 0, item);
  table_widget->setItem(row_count, 1, item1);    
  table_widget->setItem(row_count, 2, item2);
  table_widget->setItem(row_count, 3, item3);
  //设置样式为灰色
  QColor color("gray");
  item1->setTextColor(color);
  item2->setTextColor(color);
  item3->setTextColor(color);
  （2）在指定位置插入行
  其实跟（1）相似，（1）的前提是获取到表格行数
  table_widget->insertRow(row); //插入新行 row为插入的位置
**四、单击表头触发的事件**
  （1）连接表头的信号和槽
  connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));
 （2）实现槽函数
  void TableWidget::onHeaderClicked(int column)
  {
      //column为所点击的表头的某列
  }
**五、打开某行进行编辑**
  既然模拟Window那么就模仿的像一点，Windows可以修改名称，那么Qt也必然可以实现
![QTableWidget详解（样式、右键菜单、表头塌陷、多选等）](http://s14.sinaimg.cn/mw690/a6fb6cc9gdcb87055c5ed&690)
   //获得当前节点并获取编辑名称
   QTableWidgetItem *item = table_widget->item(edit_row, 0); //edit_row为想要编辑的行号
   table_widget->setCurrentCell(edit_row, 0);
   table_widget->openPersistentEditor(item); //打开编辑项
   table_widget->editItem(item);
   //关闭编辑项
   table_widget->closePersistentEditor(item);
![QTableWidget详解（样式、右键菜单、表头塌陷、多选等）](http://s1.sinaimg.cn/mw690/a6fb6cc9gdcb891e6dfc0&690)
   OK，重命名完成，！
**六、右键菜单**
  （1）创建菜单、菜单项
   void TableWidget::createActions()
  {
    //创建菜单项
    pop_menu = new QMenu();
    action_name = new QAction(this);
    action_size = new QAction(this);
    action_type = new QAction(this);
    action_date = new QAction(this);
    action_open = new QAction(this);   
    action_download = new QAction(this);
    action_flush = new QAction(this);
    action_delete = new QAction(this);
    action_rename = new QAction(this);
    action_create_folder = new QAction(this);
    action_open->setText(QString("打开"));
    action_download->setText(QString("下载"));
    action_flush->setText(QString("刷新"));
    action_delete->setText(QString("删除"));
    action_rename->setText(QString("重命名"));
    action_create_folder->setText(QString("新建文件夹"));
    action_name->setText(QString("名称"));
    action_size->setText(QString("大小"));
    action_type->setText(QString("项目类型"));
    action_date->setText(QString("修改日期"));
    //设置快捷键
    action_flush->setShortcut(QKeySequence::Refresh);
    //设置文件夹图标
    action_create_folder->setIcon(icon);
    QObject::connect(action_create_folder, SIGNAL(triggered()), this, SLOT(createFolder()));
}  
  （2）重新实现contextMenuEvent
  void TableWidget::contextMenuEvent(QContextMenuEvent *event)
  {
    pop_menu->clear(); //清除原有菜单
    QPoint point = event->pos(); //得到窗口坐标
    QTableWidgetItem *item = this->itemAt(point);
    if(item != NULL)
    {
       pop_menu->addAction(action_download);
       pop_menu->addAction(action_flush);
       pop_menu->addSeparator();
       pop_menu->addAction(action_delete);
       pop_menu->addAction(action_rename);
       pop_menu->addSeparator();
       pop_menu->addAction(action_create_folder);
       sort_style = pop_menu->addMenu("排序");
       sort_style->addAction(action_name);
       sort_style->addAction(action_size);
       sort_style->addAction(action_type);
       sort_style->addAction(action_date);
       //菜单出现的位置为当前鼠标的位置
       pop_menu->exec(QCursor::pos());
       event->accept();
    } 
 }
  OK,大功告成！
![QTableWidget详解（样式、右键菜单、表头塌陷、多选等）](http://s13.sinaimg.cn/mw690/a6fb6cc9gdcb91f43547c&690)
**七、信号**
  void cellActivated(int row, int column)
  void cellChanged(int row, int column)
  void cellClicked(int row, int column)
  void cellDoubleClicked(int row, int column)
  void cellEntered(int row, int column)
  void cellPressed(int row, int column)
  void itemActivated(QTableWidgetItem *item)
  void itemChanged(QTableWidgetItem *item)
  void itemClicked(QTableWidgetItem *item)
  void itemDoubleClicked(QTableWidgetItem *item)
  void itemEntered(QTableWidgetItem *item)
  void itemPressed(QTableWidgetItem *item)
  void itemSelectionChanged()
  void currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
  void currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
  
    关于界面的文件（夹）图标和类型如何获取？对于文件而言，不同扩展名的文件至少也有100种以上，如果图标和类型固定写死的话必不可行，所以，这里提供以下两种方式获取。
- [Qt之QFileIconProvider（获取文件图标、类型）](http://blog.sina.com.cn/s/blog_a6fb6cc90101dx99.html).
- [Qt之QFileIconProvider续（获取文件图标、类型）](http://blog.sina.com.cn/s/blog_a6fb6cc90101h2ew.html). 
更多关于QTableView的资料请参考：
- [Qt之模型/视图（实时更新数据）](http://blog.sina.com.cn/s/blog_a6fb6cc90101hhse.html).
- [Qt之QTableView](http://blog.sina.com.cn/s/blog_a6fb6cc90101i8it.html).
    以上都是在接触Qt以来总结的一些小经验，希望对大家有用！不积跬步无以至千里，不积小流无以成江河。。。
