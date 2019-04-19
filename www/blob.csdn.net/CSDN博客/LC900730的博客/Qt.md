# Qt - LC900730的博客 - CSDN博客
2017年06月20日 11:58:53[lc900730](https://me.csdn.net/LC900730)阅读数：211标签：[qt																[widget](https://so.csdn.net/so/search/s.do?q=widget&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[qt](https://blog.csdn.net/LC900730/article/category/6981788)
```
Widget::Widget(QWidget *parent):QWidget(parent)
{
    b1.setParent(this);
    b1.setText("close");
    b1.move(100,100);
    b2=new QPushButton(this);
    b2->setText("b2");
    connect(&b1,&QPushButton::pressed,this,&Widget::close);
}
Widget::~Widget(){
}
```
connect(&b1,&QPushButton::pressed,this,&Widget::close); 
/* 
&b1:信号发出者，指针类型； 
&PushButton::pressed:处理的信号   &发送者的类名::信号名字 
this:信号接收者 
&Widget::close:槽函数,信号处理函数   &接收者的类名::槽函数 
**/
自定义槽：普通函数的用法 
任意的成员函数，普通全局函数，静态函数 
槽函数和信号一致(参数，返回值) 
由于信号都是没有返回值，所以槽函数一定没有返回值;
父子窗口的切换 
项目文件 
CONFIG+=C++11 
Lambda表达式：匿名函数对象 
[](){};方括号代表开始 
QPushButton *b4； 
connect(b4,&QPushButton::released,[](){ 
qDebug<<”11111”; 
});
如果传递参数，那么[]就可以派上用场；如[b4] 
[=]：把外部所有局部变量、类中所有成员以值传递方式传递进来; 
    this:类中所有成员以值传递； 
    &：把外部所有局部变量，引用符号
操作按钮的时候，有些内存还没释放，所以如果&会出现乱码 
传递到里面的变量是只读的，如{ 
a=11；}就会报错，in lambda function assignment of read-only variable ‘a’; 
但是加上mutable 
所有的控件类都继承QWidget 
move(100,100)；相对于屏幕左上角，x往右递增，y往下递增;原点相当于窗口空白区域 
子窗口：坐标体系相对于父窗口
指定父对象后，直接或者间接继承自QObject 
子对象如果是动态分配的new，不需要手动释放delete
添加状态栏： 
    //状态栏 
    QStatusBar *status=statusBar(); 
    //核心控件 
    QLabel *label=new QLabel(this); 
    label->setText(“Normal text file”); 
    status->addWidget(label); 
    status->addWidget(new QLabel(“2”,this)); 
addWidget：c从左往右添加 
addPermanentWidget() ;//从右往左添加
浮动窗口： 
    QDockWidget *dock=new QDockWidget(this); 
    addDockWidget(Qt::RightDockWidgetArea,dock); 
    QTextEdit *textEdit1=new QTextEdit(dock); 
    dock->setWidget(textEdit1);
模态对话框与非模态：
```
#include "mainwindow.h"
#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<QDialog>
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMenuBar *mBar=menuBar();
    setMenuBar(mBar);
    QMenu *menu=mBar->addMenu("对话框");
    QAction *p1=menu->addAction("模态对话框");
    connect(p1,&QAction::triggered,[=](){
        dlg.exec();
        qDebug()<<"11111";
    });
    QAction *p2=menu->addAction("非模态对话框");
    connect(p2,&QAction::triggered,[=](){
        QDialog *p=new QDialog(this);
        p->setAttribute(Qt::WA_DeleteOnClose);
        p->show();
        //qDebug()<<"1111";
    });
}
MainWindow::~MainWindow()
{
}
```
### 标准对话框与文件对话框
```
QAction *p3=menu->addAction("关于对话框");
    connect(p3,&QAction::triggered,[=](){
        //QDialog dlg;
        QMessageBox::about(this,"about","关于qt");
    });
    QAction *p4=menu->addAction("问题对话框");
    connect(p4,&QAction::triggered,[=](){
        //QDialog dlg;
        QMessageBox::question(this,"question","are you ok?");
    });
//问题对话框有返回值
int ret=QMessageBox::question(this,"question","are you ok?");
switch(ret){
    case QMessage::Yes:
        qDebug()<<"yes";
        break;
    case QMessage::no:
        qDebug()<<"no";
        break;
}
```
//QMessage默认情况下是yes，no，如果自己指定则可以在第四个参数 
 int ret=QMessageBox::question(this,”question”,”are you ok?”,QMessageBox::Ok|QMessageBox::Cancel);
