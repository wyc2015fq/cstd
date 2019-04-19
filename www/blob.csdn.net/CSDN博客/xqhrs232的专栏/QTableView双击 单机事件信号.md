# QTableView双击 单机事件信号 - xqhrs232的专栏 - CSDN博客
2018年06月13日 14:52:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1170
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://qimo601.iteye.com/blog/1546267](http://qimo601.iteye.com/blog/1546267)
相关文章
1、QT QTableView 获取单击选中行的内容----[https://blog.csdn.net/wojiuguowei/article/details/71294434](https://blog.csdn.net/wojiuguowei/article/details/71294434)
2、Qt: QTableView如何获取（行）选中、行切换信息----[https://blog.csdn.net/u012790503/article/details/76099587](https://blog.csdn.net/u012790503/article/details/76099587)
双击QTableView的行，获取该行数据
代码如下,请注意参数类型匹配
Cpp代码  ![收藏代码](http://qimo601.iteye.com/images/icon_star.png)
- connect(dataTabView_, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotRowDoubleClicked(const QModelIndex &)));  
- 
- 
- void ABED::rowDoubleClicked(const QModelIndex index)  
- {  
- //通过Model获取一行
- 
-     QModelIndex index = ui->tableView->currentIndex();  
- if (index.isValid())  
-     {  
- <span style="white-space: pre;">    </span>//也可以通过自定义的Model中获取
-         QSqlRecord record = model->record(index.row());  
-         QString value = record.value("xxxxxxx").toString();  
-          .................  
-     }  
- }  
//设定选择行为，按行选择
tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
