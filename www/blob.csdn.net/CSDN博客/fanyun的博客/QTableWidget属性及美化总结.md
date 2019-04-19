# QTableWidget属性及美化总结 - fanyun的博客 - CSDN博客
2017年10月25日 15:01:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3532
(1).  QTableWidget创建及属性设置
tableWidget = new QTableWidget(this);
tableWidget->setRowCount(10);
tableWidget->setColumnCount(3);
//tableWidget->setFrameShape(QFrame::NoFrame);//设置边框
//tableWidget->setShowGrid(false); //设置不显示格子线
//QPalette pll = tableWidget->palette();
//pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
//tableWidget->setPalette(pll); 
//tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:lightblue;color: black;padding-left: 4px;border: 1px solid#6c6c6c;}");
tableWidget->horizontalHeader()->setDefaultSectionSize(150);
tableWidget->horizontalHeader()->setClickable(false);//设置表头不可点击（默认点击后进行排序）
(2).  QTableWidget设置表头内容
QStringList header;
header<<tr("Index")<<tr("Name")<<tr("Time");
tableWidget->setHorizontalHeaderLabels(header);
(3).  QTableWidget设置表头字体加粗
QFont font = tableWidget->horizontalHeader()->font();
font.setBold(true);
tableWidget->horizontalHeader()->setFont(font);
tableWidget->horizontalHeader()->setStretchLastSection(true);//设置充满表宽度
tableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
tableWidget->verticalHeader()->setDefaultSectionSize(10);//设置行距
tableWidget->setFrameShape(QFrame::NoFrame);//设置无边框
tableWidget->setShowGrid(false); //设置不显示格子线
tableWidget->verticalHeader()->setVisible(false);//设置垂直头不可见
tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);//可多选（Ctrl、Shift、 Ctrl+A都可以）
tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择行为时每次选择一行
tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑
tableWidget->horizontalHeader()->resizeSection(0,20);//设置表头第一列的宽度为150
tableWidget->horizontalHeader()->resizeSection(2,50);//设置表头第一列的宽度为150
tableWidget->horizontalHeader()->setFixedHeight(25);//设置表头的高度
//tableWidget->setStyleSheet("background:rgb(56,56,56);alternate-background-color:rgb(48,51,55);selection-background-color:qlineargradient(x1:0,y1:0, x2:0, y2:1, stop:0 rgb(56,56,56),stop:1 rgb(76,76,76));"); //设置选中背景色
tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(46,46,46),stop:1rgb(66,66,66));color: rgb(210,210,210);;padding-left: 4px;border: 1px solid#383838;}"); //设置表头背景色
tableWidget->setAlternatingRowColors(true);
tableWidget->setFocusPolicy(Qt::NoFocus);//去除选中虚线框
(4).  设置水平、垂直滚动条样式
//tableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:red;height:10px;}"
//"QScrollBar::handle{background:lightgray; border:2px solid transparent;border-radius:5px;}"
//"QScrollBar::handle:hover{background:gray;}"
//"QScrollBar::sub-line{background:transparent;}"
//"QScrollBar::add-line{background:transparent;}");
//tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:red;width: 10px;}"
//"QScrollBar::handle{background:lightgray; border:2px solid transparent;border-radius:5px;}"
//"QScrollBar::handle:hover{background:gray;}"
//"QScrollBar::sub-line{background:transparent;}"
// "QScrollBar::add-line{background:transparent;}");
/*QPalette pal;
pal.setColor(QPalette::Base, QColor(255, 0,0));
pal.setColor(QPalette::AlternateBase,QColor(0, 255, 0));
tableWidget->setPalette(pal);*/
tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
tableWidget->setAlternatingRowColors(true);
tableWidget->setShowGrid(false);
(5).将表格变为禁止编辑
  tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  （参数含义：QAbstractItemView.NoEditTriggers--不能对表格内容进行修改
              QAbstractItemView.CurrentChanged--任何时候都能对单元格修改
              QAbstractItemView.DoubleClicked--双击单元格
              QAbstractItemView.SelectedClicked--单击已选中的内容 
              QAbstractItemView.EditKeyPressed--
              QAbstractItemView.AnyKeyPressed--按下任意键就能修改
              QAbstractItemView.AllEditTriggers--以上条件全包括）
(6).设置表格为整行选择
  tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
  （参数含义：AbstractItemView.SelectItems--选中单个单元格
              QAbstractItemView.SelectRows--选中一行
              QAbstractItemView.SelectColumns--选中一列）
(7).单个选中和多个选中的设置
 tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);  //设置为可以选中多个目标
 （参数含义：QAbstractItemView.NoSelection--不能选择
             QAbstractItemView.SingleSelection--选中单个目标
             QAbstractItemView.MultiSelection--选中多个目标
  QAbstractItemView.ExtendedSelection/QAbstractItemView.ContiguousSelection 的区别不明显，主要功能是正常情况下是单选，但按下Ctrl或Shift键后，可以多选）
(8).表格表头的显示与隐藏
  对于水平或垂直方法的表头，可以用以下方式进行 隐藏/显示 的设置：
  tableWidget->verticalHeader()->setVisible(false);   //隐藏列表头  
  tableWidget->horizontalHeader()->setVisible(false); //隐藏行表头  
  注意：需要 #include <QHeaderView>
(9).设置表头颜色
  QTableWidgetItem *columnHeaderItem0 = tableWidget->horizontalHeaderItem(0); //获得水平方向表头的Item对象  
  columnHeaderItem0->setFont(QFont("Helvetica")); //设置字体  
  columnHeaderItem0->setBackgroundColor(QColor(0,60,10)); //设置单元格背景颜色  
  columnHeaderItem0->setTextColor(QColor(200,111,30)); //设置文字颜色
  注意：需要 #include <QHeaderView>
(10).在单元格里加入控件：
   QComboBox *comBox = new QComboBox();
   comBox->addItem("Y");
   comBox->addItem("N");
   tableWidget->setCellWidget(0,2,comBox); 
(11).单元格中添加图片：
  tableWidget->setItem(row, 0, new QTableWidgetItem(QIcon(":/new/images/demo.ico"),tr("")));
(12).设置单元格字体颜色、背景颜色和字体字符：
  QTableWidgetItem *item = new QTableWidgetItem("Apple");
  item->setBackgroundColor(QColor(0,60,10));
  item->setTextColor(QColor(200,111,100));
  item->setFont(QFont("Helvetica"));
  tableWidget->setItem(0,3,item);
  另：如果需要对所有的单元格都使用这种字体，则可以使用  tableWidget->setFont(QFont("Helvetica"));
(13).设置单元格内文字的对齐方式
 水平对齐方式有：
 Constant Value Description
 Qt.AlignLeft 0x0001 Aligns with the left edge.
 Qt.AlignRight 0x0002 Aligns with the right edge.
 Qt.AlignHCenter 0x0004 Centers horizontally in the available space.
 Qt.AlignJustify 0x0008 Justifies the text in the available space.
 垂直对齐方式：
 Constant Value Description
 Qt.AlignTop 0x0020 Aligns with the top.
 Qt.AlignBottom 0x0040 Aligns with the bottom.
 Qt.AlignVCenter 0x0080 Centers vertically in the available space.
 如果两种都要设置，只要用 Qt.AlignHCenter |  Qt.AlignVCenter 的方式即可
(14).合并单元格：
  tableWidget->setSpan(0, 0, 3, 1)  # 其参数为： 要改变单元格的1行数、2列数，要合并的3行数、4列数
(15).设置单元格的大小
  首先，可以指定某个行或者列的大小
   tableWidget->setColumnWidth(3,200);
   tableWidget->setRowHeight(3,60);
还可以将行和列的大小设为与内容相匹配
   tableWidget->resizeColumnsToContents();
   tableWidget->resizeRowsToContents();
(16).获得单击单元格的内容
  通过实现 itemClicked (QTableWidgetItem *) 信号的槽函数，就可以获得鼠标单击到的单元格指针，进而获得其中的文字信息
connect(tableWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(getItem(QTreeWidgetItem*,int)));
//将itemClicked信号与函数getItem绑定
(17).QTableWidget要调整表格行宽主要涉及以下函数
  tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);//使列完全填充并平分
  tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);//行自适应宽度                                                                                                      
  tableWidget->resizeColumnsToContents(); //根据内容调整列宽
  tableWidget->resizeColumnToContents(int col);//根据内容自动调整给定列宽
  tableWidget->horizontalHeader()->setResizeMode//把给定列设置为给定模式
  主要模式有Stretch和Fixed
(18).添加表头内容：
  方法一：
  QStringList header;
  header<<""<<tr("1")<<tr("2")<<tr("3")<<tr("4)<<tr("5");
  方法二：
  tableWidget->setHorizontalHeaderLabels(QStringList() << tr("1")<<tr("2")<<tr("3")<<tr("4)<<tr("5"));
(19).清除：
  tableWidget->clear();//清除所有可见数据（包括表头），行还在
  tableWidget->clearContents();//只清除表中数据，不清除表头内容
  tableWidget->setRowCount(0)；//连行也清除掉
(20).其它使用细节总结
  int row = tableWidget->rowCount();//获取表格中当前总行数
  tableWidget->setRowCount(row+1);//添加一行
  tableWidget->removeRow(row);//清除已有的行列
  Int row1 = tableWidget->currentItem()->row();//当前选中行
  bool focus = tableWidget->isItemSelected(tableWidget->currentItem());//判断是否选中一行
  QString proName = tableWidget->item(row, col)->text();//获取某一格内容
  setShowGrid(true);//显示表格线
  verticalHeader()->setVisible(false);//隐藏左边垂直
  QHeaderView *headerView = horizontalHeader();
  headerView->setMovable(false);//去除表头的移动
  headerView->resizeSection(0,284);//设置第一列宽
  headerView->resizeSection(1,127);//设置第二列宽
  headerView->setResizeMode(QHeaderView::Fixed);//列表不能移动
  headerView->setClickable(false);//不响应鼠标单击
  setEditTriggers(QTableWidget::NoEditTriggers);//不能编辑
  setSelectionBehavior(QTableWidget::SelectRows);//一次选中一行
  setSelectionMode(QAbstractItemView::SingleSelection);//只能单选
  /*QScrollBar *scrollBar = horizontalScrollBar();
  scrollBar->hide();*/
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
  setVerticalScrollMode(QAbstractItemView::ScrollPerItem);//垂直滚动条按项移动
  setAutoScroll(false);//去掉自动滚动
(21).排序：
  tableWidget->sortByColumn(0, Qt::AscendingOrder);//顾名思义，该函数意思是将某列按升序/降序的方式排列
