# QListView和QListWidget的区别 - xqhrs232的专栏 - CSDN博客
2018年01月05日 16:23:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1839
原文地址::[http://blog.csdn.net/libaineu2004/article/details/34416191](http://blog.csdn.net/libaineu2004/article/details/34416191)
相关文章
1、QListView和QListWidget----[http://blog.csdn.net/libaineu2004/article/details/34416191](http://blog.csdn.net/libaineu2004/article/details/34416191)
2、3.4.2　示例5：ListView的应用----[http://book.51cto.com/art/201203/323382.htm](http://book.51cto.com/art/201203/323382.htm)
3、急求大牛们，QT中怎么向listview中添加内容，用代码怎么实现？----[http://bbs.csdn.net/topics/370213093](http://bbs.csdn.net/topics/370213093)

文章来源：[http://blog.csdn.net/leaf6094189/article/details/5803763](http://blog.csdn.net/leaf6094189/article/details/5803763)
QListView是基于Model，而QListWidget是基于Item。这是它们的本质区别。
往QListView中添加条目需借助QAbstractListModel:
如：
    MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringListModel* slm =new QStringListModel(this);
    QStringList* sl =new QStringList();
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
我们可以通过信号和槽的方式对列表控件的选项进行响应，例如
connect(list, SIGNAL(currentTextChanged(QString)),　this, SLOT(setText(QString)));
总之，QListWidget是继承QListView，QListView是基于model的，需要自己来建模（例如建立QStringListModel、QSqlTableModel等），保存数据，这样就大大降低了数据冗余，提高了程序的效率，但是需要我们对数据建模有一定了解，初学者就先不要考虑了，而QListWidget是一个升级版本的QListView，它已经自己为我们建立了一个数据存储模型（QListWidgetItem），操作方便，直接调用addItem即可添加项目（ICON，文字）。
# 们，QT中怎么向listview中添加内容用码实现？
