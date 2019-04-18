# QListView和QListWidget - 深之JohnChen的专栏 - CSDN博客

2019年04月07日 23:06:34[byxdaz](https://me.csdn.net/byxdaz)阅读数：37


**QListView和QListWidget区别**

QListView是基于Model，而QListWidget是基于Item。这是它们的本质区别。

往QListView中添加条目需借助QAbstractListModel:

如：

    MainWindow::MainWindow(QWidget *parent) :

    QMainWindow(parent),

    ui(new Ui::MainWindow)

{

    ui->setupUi(this);

    QStringListModel* slm = new QStringListModel(this);

    QStringList* sl = new QStringList();

    sl->append("asdfsadfsa");

    sl->append("asdfsadfsa");

    sl->append("asdfsadfsa");

    slm->setStringList(*sl);

    ui->listView->setModel(slm);

    delete sl;

}

而在QListWidget中添加条目可以直接additem

如：

     QListWidget list;

  list.setViewMode(QListWidget::IconMode );

  list.setResizeMode(QListWidget::Adjust);

  list.setMovement(QListWidget::Static);

  for(int i =0; i < 100 ; i ++){

  QListWidgetItem *item = new QListWidgetItem(&list);

  QStyle::StandardPixmap sp = (QStyle::StandardPixmap)(i % 57);

  item->setData(Qt::DecorationRole, qApp->style()->standardPixmap(sp).scaled(QSize(16,16), Qt::KeepAspectRatio, Qt::SmoothTransformation) );

  item->setData(Qt::DisplayRole,QObject::tr("Item %1").arg(i));

  }

**QListView自定义Item**

实现方法大概有这么三种：

1、如果使用QListWidget的话，我们直接调用 setItemWidget 即可。

    void setItemWidget(QListWidgetItem *item, QWidget *widget)

2、QListWidget和QListView都可以通过调用 setIndexWidget 实现。但是该方法只适合做静态数据的显示，不适合做一些插入、更新、删除操作的数据显示。

   void QAbstractItemView::setIndexWidget(const QModelIndex &index, QWidget *widget)

3、我们针对QListView实现自己的ItemDelegate。本文就是采用该种方法，重写ItemDelegate的paint函数。

QListView实现自己的ItemDelegate方式实现自定义item，参考文章
[https://blog.csdn.net/a844651990/article/details/84324560](https://blog.csdn.net/a844651990/article/details/84324560)

setItemWidget实现自定义item（实现复杂的列表，包含多列），参考文章

[https://blog.csdn.net/mingzznet/article/details/17202779](https://blog.csdn.net/mingzznet/article/details/17202779)

