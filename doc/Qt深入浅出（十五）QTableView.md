# Qt深入浅出（十五）QTableView - qq769651718的专栏 - CSDN博客












2018年02月24日 00:39:18[吓人的猿](https://me.csdn.net/qq769651718)阅读数：15710








# QTableView

​	表格视图控件QTableView，需要和QStandardItemModel, 配套使用，这套框架是基于MVC设计模式设计的，M(Model)是QStandardItemModel数据模型，不能单独显示出来。V(view)是指QTableView视图，要来显示数据模型，C(controllor)控制在Qt中被弱化，与View合并到一起。

## 1 QTableView简单应用

​	QStandardItmeModel表格的数据模型，那么这个模型需要填上每一行每一列的数据，就像execl表格一样。

​	widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QTableView>
​
class Widget : public QTableView //继承至QTableView
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
};
​
#endif // WIDGET_H
```

​	widget.cpp

```cpp
#include "widget.h"
#include <QStandardItemModel>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QTableView(parent)
{
   QStandardItemModel* model = new QStandardItemModel(this);
   model->setItem(0, 0, new QStandardItem("张三")); 
   model->setItem(0, 1, new QStandardItem("3"));
   model->setItem(0, 2, new QStandardItem("男"));
   this->setModel(model);
}
​
Widget::~Widget()
{
​
}
```

​	以上代码实现了在model中添加一条数据，然后通过setModel函数设置view的数据模型为model，显示出来，如图：

![](https://img-blog.csdn.net/20180224003905251)

## 2 修改行列字段名

​	修改字段名可以使用`QStandardItemModel::setHeaderData`，但是在这之前你需要调用`QStandardItemModel::setColumnCount`和`QStandardItemModel::setRowCount`，例如：

```cpp
Widget::Widget(QWidget *parent)
    : QTableView(parent)
{
   QStandardItemModel* model = new QStandardItemModel(this);
   /*设置列字段名*/
   model->setColumnCount(3);
   model->setHeaderData(0,Qt::Horizontal, "姓名");
   model->setHeaderData(1,Qt::Horizontal, "年龄");
   model->setHeaderData(2,Qt::Horizontal, "性别");
​
   /*设置行字段名*/
   model->setRowCount(3);
   model->setHeaderData(0,Qt::Vertical, "记录一");
   model->setHeaderData(1,Qt::Vertical, "记录二");
   model->setHeaderData(2,Qt::Vertical, "记录三");   
      
   /*设置一条数据*/   
   model->setItem(0, 0, new QStandardItem("张三"));
   model->setItem(0, 1, new QStandardItem("3"));
   model->setItem(0, 2, new QStandardItem("男"));
   this->setModel(model);
}
```



## 3 移除数据

​	移除数据的常用函数有：

```cpp
/*移除某行数据*/
bool QAbstractItemModel::removeRow(int row, const QModelIndex &parent = QModelIndex()) 
/*移除某列数据*/
bool QAbstractItemModel::removeColumn(int column, const QModelIndex &parent = QModelIndex())
```

例如：

```cpp
model->removeRow(0);//移除第0行数据
model->removeColumn(0);//移除第0列数据
```



​	

## 4 插入数据


- 
插入一行数据


```cpp
void QStandardItemModel::insertRow(int row, const QList<QStandardItem *> &items)
/*
* row 表示从第几行插入数据
* items 表示要插入的数据QStandardItem对象
*/
```


- 
例如：


```cpp
QList<QStandardItem*> list;
list << new QStandardItem("王五") << new QStandardItem("22") << new QStandardItem("男");
model->insertRow(0, list); //在第0行插入一条记录
```



## 5 数据变更信号处理

​	当QStandardModel中的数据被更改的时候，会发射出dataChanged信号:

```cpp
[signal] void QAbstractItemModel::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());
​
/*
*topLeft bottomRight这两索引指的是表格中被更改数据的区域，如果只有一个数据被更改，那么topLeft等于bottomRight
*/
​
```

例如：

widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QTableView>
class QStandardItemModel;
class Widget : public QTableView
{
    Q_OBJECT
public slots:
    void dataChangedSlot(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());
public:
    Widget(QWidget *parent = 0);
    QStandardItemModel* _model;
    ~Widget();
};
​
#endif // WIDGET_H
​
```

widget.cpp

```cpp
#include "widget.h"
#include <QStandardItemModel>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QTableView(parent)
{
   _model = new QStandardItemModel(this);
   _model->setColumnCount(3);
   _model->setHeaderData(0,Qt::Horizontal, "姓名");
   _model->setHeaderData(1,Qt::Horizontal, "年龄");
   _model->setHeaderData(2,Qt::Horizontal, "性别");
​
   _model->setRowCount(3);
   _model->setHeaderData(0,Qt::Vertical, "记录一");
   _model->setHeaderData(1,Qt::Vertical, "记录二");
   _model->setHeaderData(2,Qt::Vertical, "记录三");
​
   _model->setItem(0, 0, new QStandardItem("张三"));
   _model->setItem(0, 1, new QStandardItem("3"));
   _model->setItem(0, 2, new QStandardItem("男"));
​
​
   connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(dataChangedSlot(QModelIndex,QModelIndex,QVector<int>)));
   this->setModel(_model);
}
​
void Widget::dataChangedSlot(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    qDebug() << _model->data(topLeft).toString() << endl;
​
}
​
Widget::~Widget()
{
​
}
```

​	

需要知道的是函数data可以获取想要的QStandardItem对象的索引：

```cpp
[pure virtual] QVariant QAbstractItemModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const
```

返回的QVariant对象是一种泛型变量，可以转换成QString、int、double等数据类型。

## 6 常用函数

```cpp
//默认显示行头，如果你觉得不美观的话，我们可以将隐藏        
tableview->verticalHeader()->hide();      
​
 //设置选中时为整行选中        
tableview->setSelectionBehavior(QAbstractItemView::SelectRows);         
   
 //设置表格的单元为只读属性，即不能编辑        
tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);  
​
//返回一个被选中的所有Item的索引,一般是去遍历这个链表进行处理
[virtual protected] QModelIndexList QTableView::selectedIndexes() const
```



​									

## 7 QStandardItem被点选信号

​	当QStandardItemModel中的某个QStandardItem被点选后，QStandardItemModel对象会发出一个信号:

```cpp
void QAbstractItemView::clicked(const QModelIndex &index);
/*
*返回被点选的Item的索引
*/
```





## 8 QItemDelegate代理

​	QTableView在处理信息显示编辑的时候比较单调，类似行编辑器，为了获得更多的灵性性，交互通过QItemDelegate执行。

​	下面通过派生一个SpinDelegate来实现一个整数旋转框的代理器。

​	一般我们要重写函数createEditor：

```cpp
[virtual] QWidget *QItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
/*
*QWidget *parent一般是指哪个窗口使用了这个代理,一般用来托管内存
* QStyleOptionViewItem &option 样式风格
* const QModelIndex &index 需要更改的Item索引
*/
```



​	spinDelegate.h

```cpp
#ifndef SPINDELEGATE_H
#define SPINDELEGATE_H
#include <QItemDelegate>
class SpinDelegate : public QItemDelegate
{
public:
    SpinDelegate(QObject *parent = Q_NULLPTR);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
​
#endif // SPINDELEGATE_H
```



​	spinDelegate.cpp

```cpp
#include "spindelegate.h"
#include <QSpinBox>
SpinDelegate::SpinDelegate(QObject *parent): QItemDelegate(parent)
{
​
}
QWidget* SpinDelegate::createEditor(QWidget *parent
           , const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1) // 返回下拉框
    {
       QSpinBox* box = new QSpinBox(parent);
       box->setMinimum(1);
       box->setMaximum(99);
       return box;
    }
    return QItemDelegate::createEditor(parent, option, index);
}
```



将widget.cpp中的构造器中修改如下函数如下：

```cpp
Widget::Widget(QWidget *parent)
    : QTableView(parent)
{
   QStandardItemModel* model = new QStandardItemModel(this);
   /*设置列字段名*/
   model->setColumnCount(3);
   model->setHeaderData(0,Qt::Horizontal, "姓名");
   model->setHeaderData(1,Qt::Horizontal, "年龄");
   model->setHeaderData(2,Qt::Horizontal, "性别");
​
   /*设置行字段名*/
   model->setRowCount(3);
   model->setHeaderData(0,Qt::Vertical, "记录一");
   model->setHeaderData(1,Qt::Vertical, "记录二");
   model->setHeaderData(2,Qt::Vertical, "记录三");   
      
   /*设置一条数据*/   
   model->setItem(0, 0, new QStandardItem("张三"));
   model->setItem(0, 1, new QStandardItem("3"));
   model->setItem(0, 2, new QStandardItem("男"));
   this->setModel(model);
   /*设置代理*/
   this->setItemDelegate(new SpinDelegate(this));
}
```






