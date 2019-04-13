
# Qt自定义信号和槽 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:30:41[seven-soft](https://me.csdn.net/softn)阅读数：423


Qt 本身包含了很多信号和槽，为编程带来了很多方便，但在实际开发中这些是远远不够的，通常需要定义自己的信号和槽。信号和槽是 QObject 类实现的功能，我们必须直接或间接继承该类。
## 自定义槽
我们首先增加一个C++类。在项目名称上单击鼠标右键，选择“添加新文件”，弹出如下所示的对话框：![](http://c.biancheng.net/cpp/uploads/allimg/151218/1-15121Q52J1B4.png)
在”C++“类别中选择“C++ Class”，点击“Choose”，弹出如下的对话框：![](http://c.biancheng.net/cpp/uploads/allimg/151218/1-15121Q53024V1.png)
将新类命名为 MainWindow，并选择 QMainWindow 作为基类，点击“下一步”完成类的创建。
这时在项目中新增了两个文件，分别为 mainwindow.h 和 mainwindow.cpp。mainwindow.h 是头文件，对 MainWindow 类进行了声明，mainwindow.cpp 是实现文件，完成了各个成员函数的定义。
将 mainwindow.h 中的代码改为：\#ifndef MAINWINDOW_H
\#define MAINWINDOW_H
\#include <QMainWindow>
\#include <QLabel>
\#include <QPushButton>
\#include <QLineEdit>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
public slots:
    void showText();  //声明槽函数
private:
    QLineEdit *lineEdit;
    QPushButton *btn;
    QLabel *label;
};
\#endif // MAINWINDOW_H第1~2行是为了防止头文件重复包含。第一次包含头文件时会定义宏 MAINWINDOW_H，第二次包含时由于宏 MAINWINDOW_H 已经存在，就不会编译后面的代码，相当于只包含了一次头文件。不了解的读者请阅读《[C语言模块化编程](http://c.biancheng.net/cpp/u/c16/)》。
接下来声明了类 MainWindow。Q_OBJECT 是 Qt 中的宏，借助它才能实现信号和槽机制，继承 Qt 类时都要在类声明的开头添加 Q_OBJECT。
> 你也可不添加 Q_OBJECT，但那将无法实现信号和槽机制，并且有时会出现莫名其妙的错误，所以建议添加该宏。
第14~15行声明了一个槽函数。slots 是 Qt 关键字，Qt 对C++进行了扩展，增加了一些关键字。
再将 mainwindow.cpp 中的代码改为：\#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle("Widget工程");
    this->resize(325, 120);
    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(30, 20, 180, 36);
    lineEdit->setPlaceholderText("请输入文本");
    btn = new QPushButton("显示", this);
    btn->setGeometry(220, 20, 70, 36);
    label = new QLabel(this);
    label->setGeometry(30, 70, 250, 30);
    QObject::connect(btn, SIGNAL(clicked()), this, SLOT(showText()));
}
//实现槽函数
void MainWindow::showText(){
    label->setText(lineEdit->text());
}在构造函数中，我们创建了三个控件，并将它们的父对象都指定为 this，也就是 MainWindow 类的对象。接下来实现了槽函数，它的作用是设置 label 控件的文本。
最后将 main.cpp 中的代码改为：\#include <QApplication>
\#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}很明显，这里将 MainWindow 作为主窗口。父对象 w 调用 show() 函数，它的所有直接和间接子对象都会显示出来。

