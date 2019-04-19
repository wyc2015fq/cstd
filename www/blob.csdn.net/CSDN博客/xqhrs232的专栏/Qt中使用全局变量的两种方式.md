# Qt中使用全局变量的两种方式 - xqhrs232的专栏 - CSDN博客
2018年05月28日 17:24:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：456
原文地址::[https://blog.csdn.net/u014546553/article/details/78558925](https://blog.csdn.net/u014546553/article/details/78558925)
相关文章
1、QT 使用全局变量的方法----[https://blog.csdn.net/guoqianqian5812/article/details/49913371](https://blog.csdn.net/guoqianqian5812/article/details/49913371)
2、QT编程之——使用全局变量----[https://blog.csdn.net/zhenyu5211314/article/details/26210353](https://blog.csdn.net/zhenyu5211314/article/details/26210353)
1、使用static关键字：
头文件声明：声明为public类型变量
mainwindow.h
[cpp][view plain](https://blog.csdn.net/u014546553/article/details/78558925#)[copy](https://blog.csdn.net/u014546553/article/details/78558925#)
- #ifndef MAINWINDOW_H
- #define MAINWINDOW_H
- #include <QMainWindow>
- namespace Ui {  
- class MainWindow;  
- }  
- class MainWindow : public QMainWindow  
- {  
-     Q_OBJECT  
- public:  
- explicit MainWindow(QWidget *parent = 0);  
-     ~MainWindow();     
- staticint a;      
- static QString c;  
- private:  
-     Ui::MainWindow *ui;  
- };  
- #endif // MAINWINDOW_H
![](https://img-blog.csdn.net/20171117112834759)
源文件定义：注意这里的变量定义，一定要写在函数的外面。
mainwindow.cpp
[cpp][view plain](https://blog.csdn.net/u014546553/article/details/78558925#)[copy](https://blog.csdn.net/u014546553/article/details/78558925#)
- #include "mainwindow.h"
- #include "ui_mainwindow.h"
- #include <QtMath>
- #include <qwt_plot.h>
- #include <qwt_plot_curve.h> //是包含QwtPointSeriesData类的头文件
- #include <qwt_plot_grid.h>
- 
- int MainWindow::a = 100;  
- QString MainWindow::c = "clue";  
- 
- MainWindow::MainWindow(QWidget *parent) :  
-     QMainWindow(parent),  
-     ui(new Ui::MainWindow)  
- {  
- 
-     qDebug()<<"a="<< a;  
- 
-     ui->textBrowser->setText(c);  
- //..........................后面代码省略
![](https://img-blog.csdn.net/20171117113736982)
调用方式：在函数里面调用全局变量
![](https://img-blog.csdn.net/20171117113928872)
2、使用extern关键字：
cglobal.h  (声明写在类和命名控件的外面)
[cpp][view plain](http://blog.csdn.net/caoshangpa/article/details/51104022#)[copy](http://blog.csdn.net/caoshangpa/article/details/51104022#)
- #ifndef CGLOBAL_H
- #define CGLOBAL_H
- externint testValue;  
- #endif // CGLOBAL_H
cglobal.cpp  (在函数外面定义变量)
[cpp][view plain](http://blog.csdn.net/caoshangpa/article/details/51104022#)[copy](http://blog.csdn.net/caoshangpa/article/details/51104022#)
- #include "cglobal.h"
- 
- int testValue=1;  
调用方式
[cpp][view plain](http://blog.csdn.net/caoshangpa/article/details/51104022#)[copy](http://blog.csdn.net/caoshangpa/article/details/51104022#)
- #include "cglobal.h"
- #include <QDebug>
- 
- qDebug()<<testValue;  
- testValue=2;  
- qDebug()<<testValue;  
墙裂推荐第一种使用static关键字的全局变量，第二种会破坏封装性。
