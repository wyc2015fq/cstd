# Qt的Mysql数据库表操作（2） - fanyun的博客 - CSDN博客
2016年12月31日 22:13:45[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1276
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
﻿﻿
Qt的[MySQL](http://lib.csdn.net/base/mysql)[数据库](http://lib.csdn.net/base/mysql)表操作（1）中，我们讲了QSqlDatabase，QSqlQuery等类的常见使用方法，接下来，我们主要讲其它几个常用数据库操作类的使用：
        QSqlTableModel类继承至QSqlQueryModel类,该类提供了一个可读写单张SQL表的可编辑数据模型，功能：修改，插入，删除，查询，和排序
1、常用函数
[QVariant](http://write.blog.csdn.net/qvariant.html) headerData (int section,[Qt::Orientation](http://write.blog.csdn.net/qt.html#Orientation-enum) orientation,int role =
 Qt::DisplayRole ) const  获取水平头或垂直头标题
bool setHeaderData ( int section,[Qt::Orientation](http://blog.csdn.net/reborntercel/article/details/qt.html#Orientation-enum) orientation,const [QVariant](http://blog.csdn.net/reborntercel/article/details/qvariant.html) & value,int role =
 Qt::EditRole ) 设置水平头或垂直头标题
int rowCount ( const [QModelIndex](http://write.blog.csdn.net/qmodelindex.html) & parent=QModelIndex() ) const // 返回行数
int columnCount ( const [QModelIndex](http://write.blog.csdn.net/qmodelindex.html) &index =QModelIndex() ) const // 返回列数
virtual bool removeColumns ( int column,int count, const QModelIndex & parent = QModelIndex() )//model->removeColumns (0)删除第一列
bool QSqlTableModel::submitAll ()，//提交所有被修改的数据，然后修改的数据被保存在数据库中
void QSqlTableModel::revertAll ()  //撤销所有的修改，如果数据库已经被提交了修改，就不能通过撤销修改改回来了
virtual void revertRow ( int row ) //恢复指定行的改变
void QSqlTableModel::setFilter ( constQString & filter )  //筛选，按照字符串filter对数据库进行筛选，相当于SQL中的WHERE语句
bool QSqlTableModel::select ()  //在筛选和排序的条件下，将数据库中符合要求的在mode表格中显示出来
void QSqlTableModel::setSort ( int column,Qt::SortOrder order ) //排序操作。按照列和Qt::SortOrder排序。Qt::SortOrder有升序和降序
bool insertRow ( int row, const QModelIndex& parent = QModelIndex() )  //插入行
bool insertColumn ( int column, const[QModelIndex](http://write.blog.csdn.net/qmodelindex.html) & parent =QModelIndex() ) // 插入列
model->setEditStrategy(QSqlTableModel::OnManualSubmit);  //设置保存策略为手动提交
2、结合其它组件的数据库操作实例
**[cpp]**[view plain](http://blog.csdn.net/fanyun_01/article/details/66483835#)[copy](http://blog.csdn.net/fanyun_01/article/details/66483835#)[print](http://blog.csdn.net/fanyun_01/article/details/66483835#)[?](http://blog.csdn.net/fanyun_01/article/details/66483835#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2304040/fork)
- void UserManager::BuildCmtable()  
- {  
- 
-     model = new QSqlTableModel;  
-     model->setTable("admin");  
-     model->setEditStrategy(QSqlTableModel::OnManualSubmit);  
- //    QString str ='1,5';
- //    model->setFilter(tr("LIMIT '%1'").arg(str));
-     model->select(); //选取整个表的所有行
-     ui->tableView->setModel(model);  
- 
-     ui->tableView->setColumnWidth(0, 150);  
-     ui->tableView->setColumnWidth(1, 150);  
-     ui->tableView->setColumnWidth(2, 150);  
-     ui->tableView->setColumnWidth(3, 150);  
-     ui->tableView->setColumnWidth(4, 150);  
-     ui->tableView->setColumnWidth(5, 150);  
-     QSqlQuery query;  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
3、利用数据模型实现增删改查，排序
**[cpp]**[view plain](http://blog.csdn.net/fanyun_01/article/details/66483835#)[copy](http://blog.csdn.net/fanyun_01/article/details/66483835#)[print](http://blog.csdn.net/fanyun_01/article/details/66483835#)[?](http://blog.csdn.net/fanyun_01/article/details/66483835#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2304040/fork)
- //新增用户
- void UserManager::Adduser()  
- {  
- int rowNum = model->rowCount(); //获得表的行数
- int id = 10;  
-     model->insertRow(rowNum); //添加一行
-     model->setData(model->index(rowNum,0),id);  
-     model->submitAll(); //可以直接提交
- }  
- 
- //删除用户
- void UserManager::Delline()  
- {  
- int curRow = ui->tableView->currentIndex().row();  
- //获取选中的行
-     model->removeRow(curRow);  
- //删除该行
- int ok = QMessageBox::warning(this,tr("删除当前行！"),tr("你确定删除当前行吗？"),QMessageBox::Yes,QMessageBox::No);  
- if(ok == QMessageBox::No)  
-     {  
-        model->revertAll(); //如果不删除，则撤销
-     }  
- else{  
-        model->submitAll(); //否则提交，在数据库中删除该行
-     }  
- }  
- 
- //修改用户信息
- void UserManager::Modefy()  
- {  
-     model->database().transaction(); //开始事务操作
- if (model->submitAll()) {  
-         model->database().commit(); //提交
-     } else {  
-         model->database().rollback(); //回滚
-         QMessageBox::warning(this, tr("tableModel"),tr("数据库错误：%1").arg(model->lastError().text()));  
-     }  
- }  
- 
- //查找用户
- void UserManager::Search()  
- {  
- 
-    QString name;  
- model->setFilter(QObject::tr("name = '%1'").arg(name));
-    model->select(); //显示结果
- }  
- 
- //用户信息降序
- void UserManager::Jsort()  
- {  
-     model->setSort(0,Qt::DescendingOrder);  
-     model->select();  
- }  
- //用户信息升序
- void UserManager::Ssort()  
- {  
-     model->setSort(0,Qt::AscendingOrder); //id属性，即第0列，升序排列
-     model->select();  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
4.综合实例演示效果如下图1所示：
![](https://img-blog.csdn.net/20170326221920014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                  图1  后台测试数据库
