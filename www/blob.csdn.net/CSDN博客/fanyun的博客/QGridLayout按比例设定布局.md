# QGridLayout按比例设定布局 - fanyun的博客 - CSDN博客
2016年09月30日 09:01:02[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：7576
如何使用代码控制QGridLayout中的窗体比例呢？
有两个函数可以用上：
QGridLayout::setColumnStretch(列码, 比例值);
QGridLayout::setRowStretch(行码, 比例值);
关于QGridLayout，关键是要理解其中函数addwidget（）中各个参数的含义
### void QGridLayout::addWidget ( [QWidget](http://hi.baidu.com/fc/editor/qwidget.html) * *widget*, int *row*, int *column*, [Qt::Alignment](http://hi.baidu.com/fc/editor/qt.html#AlignmentFlag-enum)*alignment* = 0 )
### void QGridLayout::addWidget ( [QWidget](http://hi.baidu.com/fc/editor/qwidget.html) * *widget*, int *fromRow*, int *fromColumn*, int *rowSpan*, int *columnSpan*,[Qt::Alignment](http://hi.baidu.com/fc/editor/qt.html#AlignmentFlag-enum)*alignment* = 0 )
   第一个的布局的参数的意思依次为：QGridLayoutName,行号，列号。
   第二个的布局的参数的意思依次为：QGridLayoutName,行号，列号，行宽，列宽
-> 下面是需要实现的比例效果图：
第一行和第二行是1:1，第一列和第二列是1:2
![](https://img-blog.csdn.net/20160817182845751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
具体使用方法见示例代码：
```cpp
#include <QApplication>  
#include <QGridLayout>  
#include <QList>  
#include <QLabel>  
#include <QPalette>  
#include <QString>  
void setlayout(QGridLayout * , QWidget * , QList<QLabel*> *);  
int main(int argc, char *argv[])  
{  
    QApplication a(argc, argv);  
// 初始化变量  
    QPalette myPalette;  
    myPalette.setColor(QPalette::Window, QColor(0, 255, 0));  
QList<QLabel *> * labelList = new QList<QLabel*>;  
    for(int i = 0; i < 3; ++i){  
        QLabel * temp1 = new QLabel("Label Num: " + QString::number(i + 1, 10));  
 temp1 -> setAutoFillBackground(true);  
        temp1->setPalette(myPalette);  
        labelList->push_back(temp1);  
}
QGridLayout * layout = new QGridLayout();  
    QWidget * testWidget = new QWidget();  
// 布局处理  
    setlayout(layout , testWidget, labelList); 
int result = a.exec(); 
// 程序指针delete处理  
    // 先删除小的，嵌套在大窗体中的小窗体  
    while(!labelList->isEmpty()){  
 QLabel * temp = labelList -> last();  
        labelList -> removeLast();  
        delete temp;  
}
delete labelList;   // 删除label列表  
    // 然后删除窗口布局  
    delete layout;  
    // 最后删除窗口  
    delete testWidget;  // 运行widget的析构时，它会将关联的layout析构，还有放在这个layout内的窗体析构 
  return  result;  
}  
void setlayout(QGridLayout * layout, QWidget * testWidget, QList<QLabel*> *labelList)  
{  
    // 放置窗口，设置窗口布局  
testWidget -> setLayout(layout);  
    layout -> addWidget(labelList->at(0), 0, 0);  
    layout -> addWidget(labelList->at(1), 1, 0); 
layout -> addWidget(labelList ->at(2), 0, 1, 2, 1); 
// 列比   第0列与第1列之比为 1:2  
    layout -> setColumnStretch(0, 1);  
    layout -> setColumnStretch(1, 2);  
// 行比   第0行与第1行之比为1:1  
    layout -> setRowStretch(0, 1);  
    layout -> setRowStretch(1, 1); 
testWidget -> show();  
 
    return;  
}
```
