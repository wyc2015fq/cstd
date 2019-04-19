# Qt的Mysql数据库表操作（1） - fanyun的博客 - CSDN博客
2016年12月31日 21:17:27[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1626
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
       Qt中的QSqlDatabase、QSqlQueryModel、QSqlRecord、QSqlTableModel 等类为我们提供了快速开发数据库的方式，支持多种数据库连接，例如oracle，db2，Mysql，Sqlserver等数据库，QSqlDatabase类实现了数据库连接的操作；QSqlQuery类用来执行SQL语句,对[数据库](http://lib.csdn.net/base/mysql)记录的Select、Insert、Update、Delete操作；QSqlRecord类
 封装数据库所有记录；QSqlTableModel 继承QSqlQueryModel类 ,该类提供了一个可读写单张SQL表的可编辑数据模型，功能：修改，插入，删除，查询，和排序等功能。
       那么如何连接Mysql数据库呢？我们一般使用静态方法addDatabas类实现数据库的连接：直接上代码：
```cpp
QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setPort(3306);
    db.setHostName("139.129.21.226");
    db.setDatabaseName("table");
    db.setUserName("root");
    db.setPassword("123abc");
 if(db.open())
         {
              qDebug()<<"success!";
              qDebug()<<db.driverName();
              QSqlQuery query("show tables from admin");//admin数据库里的所有表
         }else{
             qDebug()<<"failure";
         }
```
       一般情况下，我们安装Qt5以上版本，都自带了数据库驱动，只需要在程序里指明即可，有些情况下，会在连接时报缺该驱动，那么就需要要重写编译下对应的数据库驱动了，详细编译过程，请查相关资料，这里只讲操作，不再赘述。下面我们看下数据库的操作：
1、QSqlQuery 创建表：
```cpp
QSqlQuery query;  
    if(!query.exec("CREATE TABLE admin(id INTEGER PRIMARY KEY, name VARCHAR,passwdVARCHAR,create_time DATETIME,root VARCHAR, authority VARCHAR)"))  
    {  
        return query.lastError();  
    }
```
2、QSqlQuery 数据插入：
QSqlQuery query;  
query.exec("SELECT * FROM admin "); // 查询的结果可能不止一条记录，所以我们称之为结果集  
while (query.next())  
{  
    QString id = query.value(0).toInt (); //取第i条记录第1个字段(从0开始计数)的结果  
    int name = query.value(0).toString (); //取第i条记录第2个字段的结果  
}  
seek(int n) ：query指向结果集的第n条记录。指定当前的位置
first() ：query指向结果集的第一条记录。
last() ：query指向结果集的最后一条记录。
next() ：query指向下一条记录，每执行一次该函数，便指向相邻的下一条记录。
previous() ：query指向上一条记录，每执行一次该函数，便指向相邻的上一条记录。
record() ：获得现在指向的记录。
value(int n) ：获得属性的值。其中n表示你查询的第n个属性
int rowNum = query.at(); //获取query所指向的记录在结果集中的编号
int fieldNo =query.record().indexOf(“name”); //返回"name"的列号
int columnNum =query.record().count(); //获取每条记录中属性（即列）的个数
3、QSqlQuery 事务操作:
操作函数：transaction()，commit()提交，rollback()回滚
操作事务前，先判断该数据库是否支持事务操作。hasFeature是QSQLDriver类函数
if (QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions)){ ... } //   
插入一条记录，然后提交事务
```cpp
QSqlDatabase::database().transaction();  
QSqlQuery query;  
query.exec("SELECT id FROM admin WHERE id=12");  
if (query.next())  
{  
    query.exec("INSERT INTO T_STUDENT (id,name,root) VALUES (3,'lixiao',1)");  
}  
QSqlDatabase::database().commit();
```
4、综合示例：
```cpp
ui->tableWidget = new QTableWidget(6,6);
        //设置表格行标题的对齐方式
        ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
        ui->vlaout->addWidget(ui->tableWidget);
        //设置行表题
        QStringList rowLabels;
        rowLabels << "Line1" << "Line2" << "Line3" << "Line4"<< "Line5"<< "Line6";
        ui->tableWidget->setVerticalHeaderLabels(rowLabels);
        //设置每一行的高度
        for(int i = 0; i < 6; i++)
        ui->tableWidget->setRowHeight(i, 22);
        //自动调整最后一列的宽度使它和表格的右边界对齐
        ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
        //设置表格的选择方式
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
        //设置编辑方式
        ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
        //设置表格的内容
        for (int row = 0; row < 6; ++row)
       {
           QTableWidgetItem *item0, *item1, *item2, *item3, *item4 , *item5;
           item0 = new QTableWidgetItem;
           item1 = new QTableWidgetItem;
           item2 = new QTableWidgetItem;
           item3 = new QTableWidgetItem;
           item4 = new QTableWidgetItem;
           item5 = new QTableWidgetItem;
           QSqlQuery query1("select * from admin");//查询表的内容
           while (query1.next()) {
               QString id = query1.value(0).toString();//表第1个字段的内容
               QString name = query1.value(1).toString();//表第2个字段的内容
               QString passwd = query1.value(2).toString();//表第3个字段的内容
               QString create_time = query1.value(3).toString();//表第4个字段的内容
               QString root = query1.value(4).toString();//表第5个字段的内容
               QString authority = query1.value(5).toString();//表第6个字段的内容
               qDebug()<<id;
               qDebug()<<name;
               qDebug()<<passwd;
               qDebug()<<create_time;
               qDebug()<<root;
		       qDebug()<<authority;
           item0->setText(id);
           ui->tableWidget->setItem(row, 0, item0);
           item1->setText(name);
           ui->tableWidget->setItem(row, 1, item1);
           item2->setText(passwd);
           ui->tableWidget->setItem(row, 2, item2);
           item3->setText(create_time);
           ui->tableWidget->setItem(row, 3, item3);
           item4->setText(root);
           ui->tableWidget->setItem(row, 4, item4);
           item5->setText(authority);
           ui->tableWidget->setItem(row, 5, item5);
           }
       } ui->tableWidget->show();
```
如图1所示：
![](https://img-blog.csdn.net/20170326213441190?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                 图1   后台管理测试数据库表操作实例
