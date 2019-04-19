# QT QSqlQuery QSqlQueryModel - xqhrs232的专栏 - CSDN博客
2018年05月17日 11:59:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：69
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.sina.com.cn/s/blog_6c97abf10102vnud.html](http://blog.sina.com.cn/s/blog_6c97abf10102vnud.html)
SQL执行操作
**QSqlQuery提供了对数据库记录的Select、Insert、Update、Delete操作。**
SELECT操作：
> 
QSqlQuery query;
**query.exec("SELECT name, salary FROM employee WHERE salary > 50000");**
while (query.next()) {
QString name =query.value(0).toString();
int salary = query.value(1).toInt();
qDebug() << name << salary;
}
通过QSqlQuery::next() 下一条
QSqlQuery::previous() 上一条
QSqlQuery::first() 第一条
QSqlQuery::last() 最后一条
QSqlQuery::seek()，定位任意一条记录的位置。
列数、字段数：query.record().count()   
行数，记录数 query.size();
sql="SELECT*FROMsys";
query.exec(sql);
rec=query.record();
intnumRows;
while(query.next())
{
if(db.driver()->hasFeature(QSqlDriver::QuerySize))
{
                //驱动支持返回记录数numRows=query.size();
}
else{
                  //驱动不支持返回记录数，只能循环查找query.last();
numRows=query.at()+1;
}
}
值：query.value(0)
INSERT操作：
> 
//单一插入数据
QSqlQuery query;
query.prepare("INSERT INTO employee (id, name, salary) "
"VALUES (:id, :name, :salary)");
query.bindValue(":id", 1001);
query.bindValue(":name", "Thad Beaumont");
query.bindValue(":salary", 65000);
query.exec();
**//批量插入数据**
QSqlQuery query;
query.prepare("insert into myTable values (?, ?)");
**QVariantList ints;**
ints << 1 << 2 << 3 << 4;
query.addBindValue(ints);
QVariantList names;
names << "Harald" << "Boris" << "Trond" << QVariant(QVariant::String);
query.addBindValue(names);
**if (!query.execBatch())**
qDebug() << query.lastError();
UPDATE操作：
> 
QSqlQuery query;
query.prepare("UPDATE employee SET salary = ? WHERE id = 1003");
query.bindValue(0, 70000);
query.exe();
DELETE操作：
> 
QSqlQuery query;
query.exec("DELETE FROM employee WHERE id = 1007");
事务处理：
> 
QSqlDatabase::database().transaction();
QSqlQuery query;
query.exec("SELECT id FROM employee WHERE name = 'Torild Halvorsen'");
if (query.next()) {
int employeeId = query.value(0).toInt();
query.exec("INSERT INTO project (id, name, ownerid) "
"VALUES (201, 'Manhattan Project', "
+ QString::number(employeeId) + ")");
}
QSqlDatabase::database().commit();
如果数据库引擎支持事务处理，则函数QSqlDriver::hasFeature(QSqlDriver::Transactions)将返回 真。
可以通过调用QSqlDatabase::transaction()来初始化一个事务处理。之后执行你想在该事务处理的工作。
完了再执行QSqlDatabase::commit()来提交事务处理或QSqlDatabase::rollback()取消事务处理。
这里在举个QSqlDriver::hasFeature(QSqlDriver::QuerySize)例子，可以较快的统计查询记录行数。
> 
QSqlQuery query;
int numRows;
query.exec("SELECT name, salary FROM employee WHERE salary > 50000");
QSqlDatabase defaultDB = QSqlDatabase::database();
if (defaultDB.driver()->hasFeature(QSqlDriver::QuerySize)) {
numRows = query.size();
} else {
// this can be very slow
query.last();
numRows = query.at() + 1;
}
存储过程：
**AsciiToInt()是数据库中的一个存储过程。**
但我在网上以前好像看过说是SQL Server中的存储过程是通过"EXEC"完成的，而不是"CALL"，这里我不确定！留下一个疑问吧~
> 
QSqlQuery query;
query.prepare("**CALL** AsciiToInt(?, ?)");
query.bindValue(0, "A");
query.bindValue(1, 0, QSql::Out);
int rowNum = query.at();
        //获取query所指向的记录在结果集中的编号
    int columnNum = query.record().count();
//获取每条记录中属性（即列）的个数
        int fieldNo = query.record().indexOf("name");
//获取"name"属性所在列的编号，列从左向右编号，最左边的编号为0
        int id = query.value(0).toInt();
//获取id属性的值，并转换为int型
       QString name = query.value(fieldNo).toString();
//获取name属性的值
[http://www.cnblogs.com/bennylam/archive/2010/03/30/1700761.html](http://www.cnblogs.com/bennylam/archive/2010/03/30/1700761.html)
## [QT操作数据库](http://www.cnblogs.com/bennylam/archive/2010/03/30/1700761.html)
2．完全解决数据库存储中文和Qt程式显示数据库中文及中文字符串的问题 
(1)数据库和表及表里的字符相关字段（varchar, char, text等）都要使用gbk_chinese_ci这种方式，不这样做也能，但这样做，会省非常多麻烦。 
(2)重新编译Qt的MySQL驱动，需要修改src / sql / drivers / mysql / qsql_mysql.cpp文件。要修改的部分如下：第一百零八行的codec函数   
static QTextCodec* codec(MYSQL* mysql) 
{ 
return QTextCodec::codecForName("GBK");//增加部分 
#if MYSQL_VERSION_ID >= 32321 
   QTextCodec* heuristicCodec = QTextCodec :: codecForName ( mysql_character_set_name ( mysql ) ); 
if (heuristicCodec) 
   return heuristicCodec; 
#endif 
return QTextCodec::codecForLocale(); 
} 
(3)然后，重新编译qt的mysql驱动在Qt程式main函数中app后面开头处加入下面三句，加上这三个是省得不必要的麻烦   
QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK")); 
QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK")); 
QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK")); 
(4)在Qt数据库连接后，运行"SET NAMES ’UTF8’"语句或"SET NAMES ’GBK’"。   
**db = QSqlDatabase::addDatabase("QMYSQL"); db.setHostName("localhost"); db.setDatabaseName("yourdatabase"); db.setUserName("yourusername"); db.setPassword("yourpassword"); db.open(); db.exec("SET NAMES ’UTF8’"); **
经过以上四步，就能在Qt程式中直接使用中文字符串，而且能直接使用中文字符串出入于程式和数据库之间 
**3．连接MySql数据库**
QSqlDatabase db = QSqlDatabase::**addDatabase("QMYSQL");**
db.setHostName("localhost"); 
db.setDatabaseName("test"); 
db.setUserName("root"); 
db.setPassword("******"); 
bool ok = db.open();
链接**QSLite数据库：**
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); 
4．SELECT操作 
QSqlQuery query; 
int numRows; 
query.exec("SELECT * FROM scores ORDER BY id ASC"); 
while (query.next()) 
{ 
QString name = query.value(0).toString();//query.value()的参数只能是index 
int salary =query.value(1).toInt(); 
label = setText(QString::number(salary)+name);//测试查询数据的行数 
if (db.driver()->hasFeature(QSqlDriver::QuerySize)) 
{ 
numRows = query.size(); 
} 
else 
{ 
   // this can be very slow 
query.last(); 
numRows = query.at() + 1; 
} 
} 
5．INSERT操作 
（1）直接插入（一条记录） 
QSqlQuery query; 
**query.exec("INSERT INTO employee (id, name, salary) VALUES (1001, ’Thad Beaumont’, 65000)"); （2）通过“:字段”操作 QSqlQuery query; query.prepare("INSERT INTO employee (id, name, salary) VALUES (:id, :name, :salary)"); query.bindValue(":id", 1001); query.bindValue(":name", "Thad Beaumont"); query.bindValue(":salary", 65000); query.exec(); (3)通过“？”操作 QSqlQuery query; query.prepare("INSERT INTO employee (id, name, salary) VALUES (?, ?, ?)"); query.addBindValue(1001); query.addBindValue("Thad Beaumont"); query.addBindValue(65000); query.exec(); 6．UPDATE操作**
QSqlQuery query; 
**query.exec("UPDATE employee SET salary = 70000 WHERE id = 1003"); 7．DELETE操作**
QSqlQuery query; 
**query.exec("DELETE FROM employee WHERE id = 1007"); 8.transaction()操作 有时为了执行一系列复杂的操作，使用QSqlDatabase::transaction()能加快速度 **//database为QSqlDatabase对象 
if(database.driver()->hasFeature(QSqlDriver::Transactions)) **//测试数据库是否支持Transactions， **{ 
**database.transaction();**
query.exec("SELECT name FROM scores WHERE id=19"); 
if(query.next()) 
{ 
      //str = query.value(0).toString(); 
      str = "中国"; 
      query.exec("INSERT INTO scores(name) VALUES(’"+str+"’)"); 
} 
**database.commit();**
} 
注意，如果str为中文字符串，在SQL语句中需要标明’str’（用单引号括起），如果是英文或数字字符串，能不加单引号(指的是包含在双引号内的SQL语句) 
**9．使用SQL Model类 QSqlQueryModel——一个只读的读取数据库数据的模型 QSqlTableModel——一个可读写的单一表格模型，能不用写SQL语句 QSqlRelationalTableModel——QSqlTableModel的一个子类 这些类都继承于QAbstractTableModel，而他们又都继承于QAbstractItemModel （1） QSqlQueryModel的使用**
QSqlQueryModel querymodel; 
querymodel.setQuery("SELECT * FROM scores ORDER BY id ASC"); 
for (num=0;numquerymodel.rowCount();num++) 
{ 
str += **QString::number(querymodel.record(num).value("id").toInt()); **str += " "; 
str +=** querymodel.record(num).value("name").toString(); **//**注意这里的value()参数能是index（索引）也能是字段名，**前面QSqlQuery的value()参数只能是index 
str += "＼n"; 
} 
label->setText(str); 
**（2） QSqlTableModel的使用 ① 读取数据**
QSqlTableModel tablemodel;
tablemodel.setTable("scores"); 
**tablemodel.setFilter("id > 10");**
tablemodel.setSort(0,Qt::DescendingOrder); 
**tablemodel.select();**
for (num=0;numtablemodel.rowCount();num++) 
{ 
str += **QString::number(tablemodel.record(num).value("id").toInt());**
str += " "; 
str += **tablemodel.record(num).value(1).toString();**
str += "＼n"; 
} 
label->setText(str); 
**② 修改数据**
QSqlTableModel tablemodel; 
tablemodel.setTable("scores"); 
tablemodel.setFilter("id > 10"); 
tablemodel.setSort(0,Qt::DescendingOrder); 
tablemodel.select(); 
for (num=0;numtablemodel.rowCount();num++) 
{ 
**QSqlRecord record = tablemodel.record(num); record.setValue("name",record.value("name").toString()+"2"); tablemodel.setRecord(num,record);**
} 
if(tablemodel.submitAll()) 
label->setText("修改成功！"); 
else 
label->setText("修改失败！"); 
**或能用setData()来修改，代码如下：**
QSqlTableModel tablemodel; 
tablemodel.setTable("scores"); 
tablemodel.setFilter("id > 10"); 
tablemodel.setSort(0,Qt::DescendingOrder); 
tablemodel.select(); 
**tablemodel.setData(tablemodel.index(2,1),"data修改");**
if(tablemodel.submitAll()) 
    label->setText("修改成功！"); 
else 
    label->setText("修改失败！"); 
**③ 删除数据 tablemodel.removeRows(row, 5);**
//removeRows()**第一个参数为要删除的第一行的行数，第二个参数为要删除的总行数；**
tablemodel.submitAll(); 
**//注意，利用QSqlTableModel修改或删除数据，最后都要使用submitAll()执行更改 ④ 插入数据**
QSqlRecord record = tablemodel.record(); 
**record.setValue("name","插入的"); tablemodel.insertRecord(2,record);**
//注意，此处插入利用insertRecord（）函数，**该函数第一个参数为插入到tablemodel的第几行，第二个参数为记录，注意这里的记录一 定要和tablemodel中的记录匹配，故QSqlRecord record = tablemodel.record();另外，插入操作不用submitAll（），因为，insertRecord（）返回bool值。 10．使用QTableView **开头处要使用#include 
QTableView *view = new QTableView(); 
view->setModel(&model); 
view->setEditTriggers(QAbstractItemView::NoEditTriggers); 
view->show();
