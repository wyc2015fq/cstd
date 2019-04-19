# Qt入门－列表框QListWidget类 - xqhrs232的专栏 - CSDN博客
2018年01月05日 17:18:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1384
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/xgbing/article/details/7768226](http://blog.csdn.net/xgbing/article/details/7768226)
相关文章
1、Qt学习: QListWidget程序示例----[http://blog.51cto.com/13119855/1970275](http://blog.51cto.com/13119855/1970275)
2、Qt学习——QListWidget控件的使用----[http://blog.csdn.net/gdutlyp/article/details/50370536](http://blog.csdn.net/gdutlyp/article/details/50370536)
版权声明：###########################原创文章，转载请注明来自:http://blog.csdn.net/xgbing
    QListWidget类也是GUI中常用的类，它从QListView下派生：
**[cpp]**[view
 plain](http://blog.csdn.net/xgbing/article/details/7768226#)[copy](http://blog.csdn.net/xgbing/article/details/7768226#)
- class Q_GUI_EXPORT QListWidget : public QListView  
- {  
-     Q_OBJECT  
常用方法和属性：
（1）addItem
void addItem ( const QString & label )
void addItem ( QListWidgetItem * item )
void addItems ( const QStringList & labels )
添加一项或多项。
（2）
int count () const
包含隐藏的列表项在内的列表项总数。
（3）currentItem
QListWidgetItem * currentItem () const
返回当前选择的项。
（4）currentRow
int currentRow () const
返回当前选择的项的序号。
（5）insertItem
void insertItem ( int row, QListWidgetItem * item )
void insertItem ( int row, const QString & label )
void insertItems ( int row, const QStringList & labels )
插入新项到列表框。
（6）
QListWidgetItem * item ( int row ) const
返回序号为row处的列表项。
（7）removeItemWidget
void removeItemWidget ( QListWidgetItem * item )
删除指定项item。
（8）selectedItems
QList<QListWidgetItem *> selectedItems () const
返回当前选择的项。
（9）
void setCurrentItem ( QListWidgetItem * item )
设置当前选择的项为item。
（10）
void setCurrentRow ( int row )
设置当前选择项。
（11）
void setSortingEnabled ( bool enable )
设置排序功能是否启用。
（12）
void sortItems ( Qt::SortOrder order = Qt::AscendingOrder )
排序列表框，参数的值可以是：
Qt::AscendingOrder          0                  The items are sorted ascending e.g. starts with 'AAA' ends with 'ZZZ' in Latin-1 locales
Qt::DescendingOrder       1                  The items are sorted descending e.g. starts with 'ZZZ' ends with 'AAA' in Latin-1 locales
（13）
QListWidgetItem * QListWidget::takeItem ( int row )
删除一项。
示例：
Window.h
**[cpp]**[view
 plain](http://blog.csdn.net/xgbing/article/details/7768226#)[copy](http://blog.csdn.net/xgbing/article/details/7768226#)
- #ifndef __WINDOW_H__
- #define __WINDOW_H__
- 
- #include <QMainWindow>
- #include <QPushButton>
- #include <QLineEdit>
- #include <QLayout>
- #include <QLabel>
- #include <QComboBox>
- #include <QMessageBox>
- #include <QDialog>
- #include <QListWidget>
- 
- 
- class Window : public QMainWindow  
- {  
-     Q_OBJECT  
- 
- public:  
-     Window(QWidget *parent = NULL):QMainWindow(parent)  
-     {  
- 
-         listWidget = new QListWidget(this);  
- new QListWidgetItem(tr("Oak"), listWidget);  
- new QListWidgetItem(tr("Fir"), listWidget);  
- new QListWidgetItem(tr("Pine"), listWidget);  
- 
- 
-         QLabel *lbl = new QLabel(tr("select text:"));  
-         edt = new QLineEdit;  
- 
-         QVBoxLayout *leftLayout = new QVBoxLayout;  
- 
-         leftLayout->addWidget(listWidget);  
-         leftLayout->addWidget(lbl);  
-         leftLayout->addWidget(edt);  
- 
- //
-         edt_add = new QLineEdit;  
-         QPushButton *add_btn = new QPushButton(tr("Add"));  
-         QPushButton *del_btn = new QPushButton(tr("Del"));  
-         QPushButton *quit_btn = new QPushButton(tr("Quit"));  
- 
-         QVBoxLayout *rightLayout = new QVBoxLayout;  
-         rightLayout->addWidget(edt_add);  
-         rightLayout->addWidget(add_btn);  
-         rightLayout->addWidget(del_btn);  
-         rightLayout->addStretch();  
-         rightLayout->addWidget(quit_btn);  
- 
-         QHBoxLayout *mainLayout = new QHBoxLayout;  
-         mainLayout->addLayout(leftLayout);  
-         mainLayout->addLayout(rightLayout);  
- 
-         QWidget *mainWidget = new QWidget;  
-         mainWidget->setLayout(mainLayout);  
- 
-         setCentralWidget(mainWidget);  
- 
-         connect(listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged ()));  
-         connect(add_btn, SIGNAL(clicked()), this, SLOT(on_add()));  
-         connect(del_btn, SIGNAL(clicked()), this, SLOT(on_del()));  
-         connect(quit_btn, SIGNAL(clicked()), this, SLOT(close()));  
-     }  
- 
- private:  
-     QListWidget *listWidget;  
-     QLineEdit *edt, *edt_add;  
- 
- private slots:  
- void itemSelectionChanged()  
-     {  
-         edt->setText(listWidget->currentItem()->text());  
-     }  
- 
- void on_add()  
-     {  
-         listWidget->insertItem(listWidget->currentRow() + 1, edt_add->text());  
-     }  
- 
- void on_del()  
-     {  
-         listWidget->takeItem(listWidget->currentRow());  
-     }  
- 
- 
- };  
- 
- 
- #endif
main.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/xgbing/article/details/7768226#)[copy](http://blog.csdn.net/xgbing/article/details/7768226#)
- #include <QApplication>
- #include <QDialog>
- #include "Window.h"
- 
- 
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication a(argc, argv);  
-     Window *mainWindow = new Window;  
- 
- 
- 
-     mainWindow->resize(300, 300);  
-     mainWindow->setWindowTitle(QWidget::tr("Qt Test"));  
-     mainWindow->show();  
- 
- return a.exec();  
- }  
编译运行，界面如下：
![](https://img-my.csdn.net/uploads/201207/21/1342839509_1026.JPG)
QListWidget的项可以是带图片的显示，示例：
**[cpp]**[view
 plain](http://blog.csdn.net/xgbing/article/details/7768226#)[copy](http://blog.csdn.net/xgbing/article/details/7768226#)
- QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);  
- configButton->setIcon(QIcon(":/images/config.png"));  
- configButton->setText(tr("Configuration"));  
- configButton->setTextAlignment(Qt::AlignHCenter);  
- configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);  
- 
- QListWidgetItem *updateButton = new QListWidgetItem(contentsWidget);  
- updateButton->setIcon(QIcon(":/images/update.png"));  
- updateButton->setText(tr("Update"));  
- updateButton->setTextAlignment(Qt::AlignHCenter);  
- updateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);  
- 
- QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);  
- queryButton->setIcon(QIcon(":/images/query.png"));  
- queryButton->setText(tr("Query"));  
- queryButton->setTextAlignment(Qt::AlignHCenter);  
- queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);  
如图：
![](https://img-my.csdn.net/uploads/201207/22/1342966301_8762.JPG)
