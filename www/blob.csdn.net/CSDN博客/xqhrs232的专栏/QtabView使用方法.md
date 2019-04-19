# QtabView使用方法 - xqhrs232的专栏 - CSDN博客
2018年01月05日 18:04:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：739
原文地址::[http://blog.chinaunix.net/uid-20551900-id-3360638.html](http://blog.chinaunix.net/uid-20551900-id-3360638.html)
最近做项目用到Qt中的QTabView，这东西以前没用过，写下来做个记号。
QTabView在使用之前要设定模式
下面为TabView初始化
点击(此处)折叠或打开
- 
void Config::SetupTabview()
- {
- 
     tableModel = new QStandardItemModel();
- 
- //设置行
- 
     tableModel->setColumnCount(4);
- 
     tableModel->setHeaderData(0,Qt::Horizontal,tr("ID"));
- 
     tableModel->setHeaderData(1,Qt::Horizontal,tr("time"));
- 
     tableModel->setHeaderData(2,Qt::Horizontal,tr("data"));
- 
     tableModel->setHeaderData(3,Qt::Horizontal,tr("hex"));
- //tableModel->sort(0,Qt::DescendingOrder);
- 
     tableModel->setSortRole(1);
- 
- //设置每行大小
- 
     ui->tableView->setModel(tableModel);
- 
     ui->tableView->setColumnWidth(0,50);
- 
     ui->tableView->setColumnWidth(1,180);
- 
     ui->tableView->setColumnWidth(2,180);
- 
     ui->tableView->setColumnWidth(3,400);
- 
     ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
- 
     ui->tableView->verticalHeader()->hide();
- 
- }

下面为TabView每次添加数据，并排序，最新数据显示在第一行，此函数可放在定时器中调用，实现不据连续插入

点击(此处)折叠或打开
- 
void Config::UpdataTabview(unsigned int lines, QString hex)
- {
- 
    QString str = QString::number(lines);
- 
- //添加数据之前排序
- 
    tableModel->sort(0,Qt::AscendingOrder);
- 
- //添加数据
- 
    tableModel->setItem(lines, 0, new
 QStandardItem(str));
- 
    tableModel->setItem(lines, 1, new
 QStandardItem(CurTime->currentDateTime().toString("yyyy-MM-dd
 hh:mm:ss")));
- 
    tableModel->setItem(lines, 3, new
 QStandardItem(hex.toUpper()));
- 
- //添加数据后排序
- 
    tableModel->sort(0,Qt::AscendingOrder);
- }
- 

