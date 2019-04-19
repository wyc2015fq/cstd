# Qt数据库：（三）利用QSqlQuery类执行SQL语句（一） - xqhrs232的专栏 - CSDN博客
2018年05月27日 15:52:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：166
原文地址::[https://blog.csdn.net/liuligui5200/article/details/46966199](https://blog.csdn.net/liuligui5200/article/details/46966199)
SQL即结构化查询语言，是关系数据库的标准语言。前面已经提到，在Qt中利用QSqlQuery类实现了执行SQL语句。需要说明，我们这里只是Qt教程，而非专业的数据库教程，所以我们不会对数据库中的一些知识进行深入讲解，下面只是对最常用的几个知识点进行讲解。
我们下面先建立一个工程，然后讲解四个知识点，分别是：
一，操作SQL语句返回的结果集。
二，在SQL语句中使用变量。
三，批处理操作。
四，事务操作。
我们新建Qt4 Gui Application工程，我这里工程名为data2，然后选中QtSql模块，Base class选QWidget。工程建好后，添加C++ Header File ，命名为connection.h ，更改其内容如下：
#ifndefCONNECTION_H#defineCONNECTION_H#include<QMessageBox>#include<QSqlDatabase>#include<QSqlQuery>#include<QtDebug>#include<QtGui>#include<qdebug.h>staticboolcreatConnection(){QSqlDatabasedb=QSqlDatabase::addDatabase("QSQLITE");//添加数据库驱动db.setDatabaseName(":memory:");//数据库连接命名if(!db.open())//打开数据库{qDebug("openisfalse");returnfalse;}else{qDebug("openisOK");}QSqlQueryquery(db);//以下执行相关QSL语句boolbuscess=query.exec("createtableperson(idintprimarykey,firstnamevarchar(20),lastnamevarchar(20))");//新建person表，id设置为主键，一个firstname项，还有一个lastname项query.exec("insertintopersonvalues(101,'Danny','Young')");query.exec("insertintopersonvalues(102,'Christine','Holand')");query.exec("insertintopersonvalues(103,'Lars','Gordon')");query.exec("insertintopersonvalues(104,'Roberto','Robitaille')");query.exec("insertintopersonvalues(105,'Maria','Papadopoulos')");//向表中插入5条记录if(!buscess){qDebug("tableiserror");}else{qDebug("tableissucess");}returntrue;}#endif//CONNECTION_H
然后更改main.cpp的内容如下：
#include<QtGui/QApplication>#include"widget.h"#include"connection.h"#include<QtSql>#include<QtDebug>#include<qdebug.h>#include<QSqlQuery>intmain(intargc,char*argv[]){QApplicationa(argc,argv);if(!creatConnection())return1;Widgetw;w.show();returna.exec();}
可以看到，我们是在主函数中打开数据库的，而数据库连接用一个函数完成，并单独放在一个文件中，这样的做法使得主函数很简洁。我们今后使用数据库时均使用这种方法。我们打开数据库连接后，新建了一个人员表，并在其中插入了几条记录。
101 "Danny""Young"
102 "Christine" "Holand"
103 "Lars""Gordon"
104 "Roberto""Robitaille"
105 "Maria""Papadopoulos" 
表中的一行就叫做一条记录，一列是一个属性。这个表共有5条记录，id、firstname和lastname三个属性。程序中的“id int primary key”表明id属性是主键，也就是说以后添加记录时，必须有id项。
下面我们打开widget.ui文件，在设计器中向界面上添加一个Push Button ，和一个Spin Box 。将按钮的文本改为“查询”，然后进入其单击事件槽函数，更改如下。
voidWidget::on_pushButton_clicked(){QSqlQueryquery;query.exec("select*fromperson");while(query.next()){qDebug()<<query.value(0).toInt()<<query.value(1).toString()<<query.value(2).toString();}}
我们在widget.cpp中添加头文件：
#include <QSqlQuery>#include <QtDebug>
然后运行程序，单击“查询”按钮，效果如下：
![](https://img-blog.csdn.net/20150720114809589?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到在输出窗口，表中的所有内容都输出出来了。这表明我们的数据库连接已经成功建立了。
一，操作SQL语句返回的结果集。
在上面的程序中，我们使用query.exec(“select * from person”);来查询出表中所有的内容。其中的SQL语句“select * from person”中“*”号表明查询表中记录的所有属性。而当query.exec(“select * from person”);这条语句执行完后，我们便获得了相应的执行结果，因为获得的结果可能不止一条记录，所以我们称之为结果集。
结果集其实就是查询到的所有记录的集合，而在QSqlQuery类中提供了多个函数来操作这个集合，需要注意这个集合中的记录是从0开始编号的。最常用的有：
seek(int n) ：query指向结果集的第n条记录。
first() ：query指向结果集的第一条记录。
last() ：query指向结果集的最后一条记录。
next() ：query指向下一条记录，每执行一次该函数，便指向相邻的下一条记录。
previous() ：query指向上一条记录，每执行一次该函数，便指向相邻的上一条记录。
record() ：获得现在指向的记录。
value(int n) ：获得属性的值。其中n表示你查询的第n个属性，比方上面我们使用“select * from person”就相当于“select id, firstname，lastname from person”，那么value(0)返回id属性的值，value(1)返回firstname属性的值，value(2)返回lastname属性的值。该函数返回QVariant类型的数据，关于该类型与其他类型的对应关系，可以在帮助中查看QVariant。
at() ：获得现在query指向的记录在结果集中的编号。
需要说明，当刚执行完query.exec(“select * from person”);这行代码时，query是指向结果集以外的，我们可以利用query.next()，当第一次执行这句代码时，query便指向了结果集的第一条记录。当然我们也可以利用seek(0)函数或者first()函数使query指向结果集的第一条记录。但是为了节省内存开销，推荐的方法是，在query.exec(“select * from person”);这行代码前加上query.setForwardOnly(true);这条代码，此后只能使用next()和seek()函数。
下面将“查询”按钮的槽函数更改如下：
voidWidget::on_pushButton_clicked(){QSqlQueryquery;query.exec("select*fromperson");//while(query.next())//{//qDebug()<<query.value(0).toInt()<<query.value(1).toString()<<query.value(2).toString();//}qDebug()<<"execnext()";if(query.next())//开始就先执行一次next()函数，那么query指向结果集的第一条记录{introwNum=query.at();//获取query所指向的记录在结果集中的编号intcolumnNum=query.record().count();//获取每条记录中属性（即列）的个数intfieldNo1=query.record().indexOf("firstname");//获取firstname属性所在列的编号，列从左向右编号，最左边的编号为0intfieldNo2=query.record().indexOf("lastname");//获取lastname属性所在列的编号，列从左向右编号，最左边的编号为0intid=query.value(0).toInt();//获取id属性的值，并转换为int型QStringfirstname=query.value(1).toString();//获取firstname属性的值QStringlastname=query.value(2).toString();//获取lastname属性的值qDebug()<<"rowNumis:"<<rowNum//将结果输出<<"columnNumis:"<<columnNum<<"fieldNo1is:"<<fieldNo1<<"fieldNo2is:"<<fieldNo2<<"idis:"<<id<<"firstnameis:"<<firstname<<"lastnameis:"<<lastname;}qDebug()<<"execseek(2):";if(query.seek(2))//定位到结果集中编号为2的记录，即第三条记录，因为第一条记录的编号为0{qDebug()<<"rowNumis:"<<query.at()<<"idis:"<<query.value(0).toInt()<<"firstnameis:"<<query.value(1).toString()<<"lastnameis:"<<query.value(2).toString();}qDebug()<<"execlast():";if(query.last())//定位到结果集中最后一条记录{qDebug()<<"rowNumis:"<<query.at()<<"idis:"<<query.value(0).toInt()<<"firstnameis:"<<query.value(1).toString()<<"lastnameis:"<<query.value(2).toString();}}
然后在widget.cpp文件中添加头文件。
#include <QSqlRecord>
运行程序，结果如下：
![](https://img-blog.csdn.net/20150720134725941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
个人分类：[数据库知识](https://blog.csdn.net/liuligui5200/article/category/3268749)
