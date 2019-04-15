# Qt深入浅出（十八）MySql的使用 - qq769651718的专栏 - CSDN博客












2018年02月24日 00:42:56[吓人的猿](https://me.csdn.net/qq769651718)阅读数：151








# MySql的使用

## 1 使用QSqlDataBase
- 
Qt数据中的管理类为QSqlDataBase，没有继承任何基类，需要在pro文件中添加QT += sql。


代码如下：

```cpp
#include <QSqlDatabase>
#include <QDebug>
#include <QCoreApplication>
int main(int argc, char**argv)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("TESTDB");
    db.setUserName("root");
    db.setPassword("root");
    bool ok = db.open();
    qDebug() << "open db:" << ok << endl;
    db.close();
    return 0;
}
```

​	这个时候会报错：

```language-py
QSqlDatabase: mysql driver not loaded
QSqlDatabase: available drivers: QSQLITE QMYSQL QMYSQL3 QODBC QODBC3 QPSQL QPSQL7
```

​	这是因为Qt默认没有安装mysql驱动，需要将libmysql.dll拷贝到Qt的安装路径中，这个libmysql.dll可以到官网[http://dev.mysql.com/downloads/connector/c/](http://dev.mysql.com/downloads/connector/c/) 下载，下载32位还是64位的要根据编译工具来判断，例如32位的MinGW就下载32位的mysql驱动。

​	将libmysql.dll拷贝到如`C:\Qt\Qt5.7.0\5.7\mingw53_32\bin`目录下。


- 
SQL语句使用使用QSqlQuery


```cpp
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
int main(int argc, char**argv)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("TESTDB");
    db.setUserName("root");
    db.setPassword("root");
    if(!db.open())
    {
        qDebug() << db.lastError().text() << endl;
    }
​
​
    QSqlQuery sql(db);
​
    if(!sql.exec("SELECT * FROM test_tb11")) //exec返回true代表执行SQL语句成功
    {
        qDebug() << sql.lastError().text() << endl;
    }
​
    while(sql.next()) //获取下一条记录，如果没有下一条则返回false。
    {
        qDebug() << sql.value("name").toString() << endl
                  << sql.value("age").toString() << endl
                    << sql.value("sub_date").toString() << endl;
    }
​
    if(!sql.exec("DELETE FROM test_tb WHERE name = \'Zhangsan\'"))
    {
        qDebug() << sql.lastError().text() << endl;
    }
​
    if(!sql.exec("UPDATE test_tb SET age=99 WHERE name = \'Lisi\'"))
    {
        qDebug() << sql.lastError().text() << endl;
    }
​
​
    db.close();
    return 0;
}
```


- 
事务常用函数


```cpp
bool QSqlDatabase::transaction();
bool QSqlDatabase::commit();
bool QSqlDatabase::rollback();
```



## 2 使用QSqlTableModel

​	使用QSqlTableModel，可以非常智能的来管理数据库，不需要填写sqlquery语句。



### 2.1 在QTableView上显示

​	QSqlTableModel无法单独显示，它也仅仅是数据模型，但是通过QTableView来显示。

​	Widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
class QTableView;
class QSqlTableModel;
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    void initMySql();
private:
    QTableView* _view;
    QSqlTableModel* _sqlModel;
​
​
};
​
#endif // WIDGET_H
```

​	Widget.cpp

```cpp
#include "widget.h"
#include <QDebug>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlTableModel>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vBox = new QVBoxLayout;
    this->setLayout(vBox);
    initMySql();
​
    _view = new QTableView;
    vBox->addWidget(_view);
    _view->setModel(_sqlModel);
​
}
​
​
void Widget::initMySql()
{
    /*连接数据库*/
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("TESTDB");
    db.setUserName("root");
    db.setPassword("root");
    if(!db.open())
    {
        qDebug() << "db open failed" << endl;
    }
    /*创建数据库模型，链接db对象*/
    _sqlModel = new QSqlTableModel(this, db);
    _sqlModel->setTable("test_tb"); //选择TESTDB数据库中的表格test_tb
    _sqlModel->select(); //相当于“SELECT * FROM test_tb;”
}
​
Widget::~Widget()
{
​
}
```

​	以上例子通过QSqlTableModel将数据库TESTDB.test_tb中的数据全部显示到QTableView上，并且可以通过QTableView来修改内容。





### 2.2 手动提交

​	上面的例子我们发现经过QTableView修改的并不能实时更新到数据库中，其实我们可以自己手动提交，通过setEditStrategy设置数据库提交的策略：

```cpp
[virtual] void QSqlTableModel::setEditStrategy(EditStrategy strategy);
/*
* EditStrategy提交的策略，是个枚举类型，具体请看Qt 
*/
```

​	设置为手动提交后，可以使用submitall来提交:

```cpp
[slot] bool QSqlTableModel::submitAll();
```



以下例子通过添加一个按钮来提交，在17.2.1的基础上进行修改widget.cpp

```cpp
#include "widget.h"
#include <QDebug>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include <QHBoxLayout>
#include <QPushButton>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vBox = new QVBoxLayout;
    this->setLayout(vBox);
​
    initMySql();
    _view = new QTableView;
    vBox->addWidget(_view);
    _view->setModel(_sqlModel);
​
​
    QHBoxLayout* hBox = new QHBoxLayout;
    vBox->addLayout(hBox);
    QPushButton* pb_sub = new QPushButton("提交");
    hBox->addWidget(pb_sub);
    /*按钮按下触发submitAll*/
    connect(pb_sub, SIGNAL(clicked()),  _sqlModel, SLOT(submitAll()));
​
}
​
​
void Widget::initMySql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("TESTDB");
    db.setUserName("root");
    db.setPassword("root");
    if(!db.open())
    {
        qDebug() << "db open failed" << endl;
    }
    _sqlModel = new QSqlTableModel(this, db);
    _sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置为手动提交
    _sqlModel->setTable("test_tb");
    _sqlModel->select();
  // _sqlModel->removeColumn(1); //不显示第二列
}
​
Widget::~Widget()
{
​
}
```





### 2.3 撤销修改

​	如果不想提交修改的内容，那么可以撤销操作，revertAll

```cpp
[slot] void QSqlTableModel::revertAll();
```





### 2.4 过滤器

​	如果只想显示特定的内容，那么可以使用QSqlTableModel的过滤器，相当是SQL中的WHERE子句：

```cpp
[virtual] void QSqlTableModel::setFilter(const QString &filter);
```



​	例如：

```cpp
_sqlModel->setFilter("age = 2");
```

### 2.5 排序

​	如果想要进排序可以使用QSqlTableModel::setSort,相当于SQL中的ORDER BY：

```cpp
[virtual] void QSqlTableModel::setSort(int column, Qt::SortOrder order)
```







### 2.6 添加一条记录

​	如果想添加一条记录，那么需要知道记录表格式，那么可以通过QSqlTableModel::record来获取, QSqlTableModel::insertRecord来插入数据：

```cpp
QSqlRecord QSqlTableModel::record() const;
bool QSqlTableModel::insertRecord(int row, const QSqlRecord &record);
/*
* 在row行插入数据，如果row为负数就在末尾插入一行
*/
```

例如

```cpp
void Widget::addSlot()
{
    QSqlRecord record = _model->record();
    _model->insertRecord(-1, record); //在末尾插入数据
}
```







### 2.7 删除记录

​	删除的时候一般要先获取到当前被选中的行：

```cpp
void Widget::deleteSlot()
{
    QItemSelectionModel* selectModel = _view->selectionModel();
    QModelIndexList list = selectModel->selectedIndexes();
    foreach(QModelIndex index, list)
    {
        _sqlModel->removeRow(index.row());
    }
 }
```



### 2.8 综合例子

Widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
class QTableView;
class QSqlTableModel;
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void addSlot();
    void deleteSlot();
private:
    void initMySql();
private:
    QTableView* _view;
    QSqlTableModel* _sqlModel;
​
​
};
​
#endif // WIDGET_H
​
```



Widget.cpp

```cpp
#include "widget.h"
#include <QDebug>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSqlRecord>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vBox = new QVBoxLayout;
    this->setLayout(vBox);
​
    initMySql();
    _view = new QTableView;
    vBox->addWidget(_view);
    _view->setModel(_sqlModel);
​
​
    QHBoxLayout* hBox = new QHBoxLayout;
    vBox->addLayout(hBox);
    QPushButton* pb_sub = new QPushButton("提交");
    QPushButton* pb_revert = new QPushButton("撤销");
    QPushButton* pb_insert = new QPushButton("插入");
    QPushButton* pb_delete = new QPushButton("删除");
    hBox->addWidget(pb_sub);
    hBox->addWidget(pb_revert);
    hBox->addWidget(pb_insert);
    hBox->addWidget(pb_delete);
    /*按钮按下触发submitAll*/
    connect(pb_sub, SIGNAL(clicked()),  _sqlModel, SLOT(submitAll()));
    connect(pb_revert, SIGNAL(clicked()), _sqlModel, SLOT(revertAll()));
    connect(pb_insert, SIGNAL(clicked()), this, SLOT(addSlot()));
    connect(pb_delete, SIGNAL(clicked()), this, SLOT(deleteSlot()));
​
}
​
​
void Widget::initMySql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("TESTDB");
    db.setUserName("root");
    db.setPassword("root");
    if(!db.open())
    {
        qDebug() << "db open failed" << endl;
    }
    _sqlModel = new QSqlTableModel(this, db);
    _sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置为手动提交
    _sqlModel->setTable("test_tb");
    _sqlModel->setFilter("age > 0");
    _sqlModel->setSort(1, Qt::AscendingOrder);
    _sqlModel->select();
  // _sqlModel->removeColumn(1); //不显示第二列
}
​
void Widget::addSlot()
{
    QSqlRecord record = _sqlModel->record();
    _sqlModel->insertRecord(-1, record); //在末尾插入数据
}
​
​
void Widget::deleteSlot()
{
    QItemSelectionModel* selectModel = _view->selectionModel();
    QModelIndexList list = selectModel->selectedIndexes();
    foreach(QModelIndex index, list)
    {
        _sqlModel->removeRow(index.row());
    }
 }
​
Widget::~Widget()
{
​
}
​
```






