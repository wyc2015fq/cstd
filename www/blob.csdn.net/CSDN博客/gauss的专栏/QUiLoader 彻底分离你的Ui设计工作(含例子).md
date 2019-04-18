# QUiLoader:彻底分离你的Ui设计工作(含例子) - gauss的专栏 - CSDN博客
2013年01月03日 17:41:45[gauss](https://me.csdn.net/mathlmx)阅读数：353
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
原文链接:[QUiLoader:彻底分离你的Ui设计工作(含例子)](http://www.cuteqt.com/blog/?p=516)
Trolltech提供的界面所见即所得工具designer极大的简化了开发者花费在UI布局上的时间。但一部分喜欢在终端上开发的工程师认为拖拖拉拉这种伎俩实在是”雕虫小技，壮夫不为“也。即使专门的界面设计师为他设计.ui文件的话，他也嫌来来回回的重新编译麻烦。乐于自己通过QLayout手写代码进行设计。
那么QUiLoader这个工具，则可以真正做到程序师和界面师做到“鸡犬相闻，不相往来”, .ui文件不需要重新编译，就可以被程序加载，生成新的外观。
我们来看QUiLoader的实用方法
QUiLoader loader;
QFile file(“cuteqt.ui”);
file.open(QFile::ReadOnly);
QWidget *formWidget = loader.load(&file, this);
通过load函数即可根据.ui文件创建出一个对应的QWidget对象,后面的this指针是QWidget *类型的，你可以用其它窗体去作为formWidget的父窗体。父窗体被显示的时候，该formWidget窗体就会被显示，当然更好的方法是用一个QLayout来管理formWidget,因为父窗体可能不止一个子窗体。
如果你还想对cuteqt.ui中的那些子窗体信号作处理，那么可以用qFindChild去找到它们。
ui_setButton = qFindChild<QPushButton*>(this, “setButton”);
ui_clearButton = qFindChild<QPushButton*>(this, “clearButton”);
ui_textEdit = qFindChild<QTextEdit*>(this, “textEdit”);
比如你可以用
connect(ui_setButton,SIGNAL(clicked()),this,SLOT（setButtonClicked()));处理ui_setButton的clicked()信号。
但拜元对象机制所赐，还有一劳永逸的方法
QMetaObject::connectSlotsByName(this);该函数会将this的所有子对象都连接上一个槽(slot)，槽的命名规范为
on_对象名_信号名，比如上面的setButton的clicked()，将会生成一个on_setButton_clicked()的槽，你只要实现该槽函数就可以了。
怎样，动态加载.ui文件的方法是不是很简单？而且这样做的好处至少有两个
1. 前期的设计会更准确，程序师和界面师只要协商好有哪些对象就可以分头工作了，彼此互不依赖
2. 界面可以独立于应用发布
下面是我的一个小例子。设计了两个ui,界面分别为
![snap1](http://www.cuteqt.com/blog/wp-content/uploads/2009/06/snap1.png)![snap2](http://www.cuteqt.com/blog/wp-content/uploads/2009/06/snap2.png)
在想用界面1的时候，只要 cp cuteqt.ui.1 cuteqt.ui 然后运行./uiloader即可
同理,在想用界面2的时候，只要 cp cuteqt.ui.2 cuteqt.ui 然后运行./uiloader即可
这中间不需要改动你的代码～
#include <QtGui/QtGui>
#include <QtUiTools/QtUiTools>
int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    QUiLoader uiLoader;
    QFile file("..//CellDialog.ui");
    QWidget *cellDialog = uiLoader.load(&file);
    if(cellDialog)
    {
        cellDialog->show();
    }
    else
    {
        QMessageBox::information(NULL,"error","ui  loader error");
    }
    return app.exec();
}
在.pro文件中需要加上
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += uitools
# Input
FORMS += CellDialog.ui
SOURCES += uiloader.cpp
