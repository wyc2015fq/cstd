# Qt多页面切换之QTabWidget - fanyun的博客 - CSDN博客
2016年10月30日 20:49:54[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4857
**[cpp]**[view
 plain](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)[copy](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)
[print](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)[?](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)
- //mydialog.h
- #ifndef MYDIALOG_H
- #define MYDIALOG_H
- #include <QDialog>
- class QTabWidget;  
- class MyDialog : public QDialog  
- {  
-   Q_OBJECT  
- public:  
- explicit MyDialog(QWidget *parent = 0);  
- signals:  
- public slots:  
- private:  
-   QTabWidget *tabWidget;  
- };  
- #endif // MYDIALOG_H
**[cpp]**[view
 plain](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)[copy](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)
[print](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)[?](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)
- //mydialog.cpp
- #include "mydialog.h"
- #include <QtGui/QTabWidget>
- #include <QtGui/QHBoxLayout>
- #include <QtGui/QVBoxLayout>
- #include <QtGui/QLabel>
- #include <QtGui/QLineEdit>
- #include <QtGui/QPushButton>
- #include <QtGui/QWidget>
- #include <QtGui/QIcon>
- MyDialog::MyDialog(QWidget *parent) :  
-   QDialog(parent)  
- {  
-   tabWidget = new QTabWidget();  
- 
- //新建第一个页面的部件
-   QWidget *widget = new QWidget();  
-   QLineEdit *lineEdit = new QLineEdit();  
-   QPushButton *pushButton = new QPushButton("Test");  
-   QVBoxLayout *vLayout = new QVBoxLayout();  
-   vLayout->addWidget(lineEdit);  
-   vLayout->addWidget(pushButton);  
-   widget->setLayout(vLayout);  
- 
- 
- //新建第二个页面的部件
-   QLabel *label = new QLabel("Hello Qt");  
- 
- //新建第三个页面的部件
-   QPushButton *pushButton3 = new QPushButton("Click Me");  
- 
- //向QTabWidget中添加第一个页面
-   QIcon icon1(":/new/icon/images/1.ico");  
-   tabWidget->addTab(widget, icon1, "Tab1");  
- 
- //向QTabWidget中添加第二个页面
-   QIcon icon2(":/new/icon/images/2.ico");  
-   tabWidget->addTab(label, icon2, "Tab2");  
- 
- //向QTabWidget中添加第三个页面
-   QIcon icon3(":/new/icon/images/3.ico");  
-   tabWidget->addTab(pushButton3, icon3, "Tab3");  
- 
-   QHBoxLayout *layout = new QHBoxLayout();  
-   layout->addWidget(tabWidget);  
- 
- this->setLayout(layout);  
- this->resize(300, 100);  
- this->setWindowTitle("QTabWidgetDemo");  
- }  
**[cpp]**[view
 plain](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)[copy](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)
[print](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)[?](http://blog.csdn.net/qiurisuixiang/article/details/6916603#)
- //main.cpp
- #include <QtGui/QApplication>
- #include "mydialog.h"
- int main(int argc, char *argv[])  
- {  
-   QApplication a(argc, argv);  
-   MyDialog dialog;  
-   dialog.show();  
- return a.exec();  
- }  
资源文件截图：
![](http://hi.csdn.net/attachment/201110/29/0_1319862253sAd8.gif)
程序运行截图：
![](http://hi.csdn.net/attachment/201110/29/0_13198623349RbM.gif)
