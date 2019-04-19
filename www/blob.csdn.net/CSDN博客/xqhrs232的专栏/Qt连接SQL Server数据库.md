# Qt连接SQL Server数据库 - xqhrs232的专栏 - CSDN博客
2018年01月05日 11:11:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：115
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/qian_f/article/details/8927234](http://blog.csdn.net/qian_f/article/details/8927234)
相关文章
1、Qt连接SQL server数据库----[http://blog.csdn.net/ccsuxwz/article/details/72875376](http://blog.csdn.net/ccsuxwz/article/details/72875376)
2、Qt笔记(1)连接 SQL Server 数据库----[http://blog.51cto.com/hhuayuan/893415](http://blog.51cto.com/hhuayuan/893415)
3、win10操作系统的ODBC数据源配置sqlserver数据库----[http://blog.csdn.net/cuiyaoqiang/article/details/50708851](http://blog.csdn.net/cuiyaoqiang/article/details/50708851)
4、WIN10如何配置MysqlODBC数据源----[https://jingyan.baidu.com/article/a24b33cd030ec819fe002b04.html](https://jingyan.baidu.com/article/a24b33cd030ec819fe002b04.html)
前提：
SQL Server里已经建立了相应的数据库，有对应的表和数据。
步骤：
1. Qt里新建一个空工程，添加main.cpp文件。
2. 在工程文件（.pro文件）添加一行：QT += sql。
3. 该敲代码了，代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/qian_f/article/details/8927234#)[copy](http://blog.csdn.net/qian_f/article/details/8927234#)
- #include <QtGui>
- #include <QString>
- #include <QTextCodec>
- #include <QSqlDatabase>
- #include <QtSql>
- 
- /*连接数据库*/
- void OpenDatabase()  
- {  
-     QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");  
-     db.setDatabaseName(QString("DRIVER={SQL SERVER};"
- "SERVER=%1;"
- "DATABASE=%2;"
- "UID=%3;"
- "PWD=%4;").arg("QIAN-PC")  
-                        .arg("StuManager")  
-                        .arg("sa")  
-                        .arg("123456"));  
- if (!db.open())  
-     {  
- 
-         QMessageBox::warning(0, qApp->tr("Cannot open database"),  
-                 db.lastError().databaseText(), QMessageBox::Cancel);  
-     }  
- else
-     {  
-         qDebug()<<"Connect to Database Success!";  
-     }  
- 
- }  
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication a(argc, argv);  
- 
- /*设置编码格式*/
-     QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));  
- 
-     OpenDatabase();  
- 
-     QDialog *mainDialog = new QDialog;  
-     QTableView *tableView = new QTableView;  
-     QHBoxLayout *layout = new QHBoxLayout;  
-     layout->addWidget(tableView);  
-     mainDialog->setLayout(layout);  
- 
-     QSqlQueryModel *model = new QSqlQueryModel;  
-     model->setQuery(QObject::tr("select * from 教师"));  
-     tableView->setModel(model);  
- 
-     mainDialog->adjustSize();  
-     mainDialog->show();  
- 
- return a.exec();  
- }  
4. 看看运行结果吧。
![](https://img-blog.csdn.net/20130514191916982)
参考网址：[http://hhuayuan.blog.51cto.com/1630327/893415](http://hhuayuan.blog.51cto.com/1630327/893415)

