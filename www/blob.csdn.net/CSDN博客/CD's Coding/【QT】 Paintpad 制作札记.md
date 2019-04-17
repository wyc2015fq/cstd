# 【QT】 Paintpad 制作札记 - CD's Coding - CSDN博客





2013年12月08日 15:54:01[糖果天王](https://me.csdn.net/okcd00)阅读数：970
个人分类：[作业](https://blog.csdn.net/okcd00/article/category/1727537)









                
1、无法用"UTF-8"编码

用记事本打开，另存为，更改编码为UTF-8，保存原位置，覆盖确定。

2、No such file or directory

在Paint.pro里增加行：


QT+=widgets


3、'setCodecForTr' is not a member of 'QTextCodec'

Reason：QT5中删除了此项函数。

相见CSDN-Blog：

[](http://blog.csdn.net/dbzhang800/article/details/7325698)[QTextCodec中的setCodecForTr等终于消失了
 (Qt5)](http://blog.csdn.net/dbzhang800/article/details/7325698)：

[http://blog.csdn.net/dbzhang800/article/details/7325698](http://blog.csdn.net/dbzhang800/article/details/7325698)

[](http://blog.csdn.net/dbzhang800/article/details/7542672)[Qt5与中文](http://blog.csdn.net/dbzhang800/article/details/7542672)： 

[http://blog.csdn.net/dbzhang800/article/details/7542672](http://blog.csdn.net/dbzhang800/article/details/7542672)


4、QT5茫茫多的奇葩问题

切换到QT4，原来QT和QtCreator的版本是不同的……各种失败之后，QtCreator2.6.0为QT4.8.4，Installing...

5、卸载程序时删除尽了当前文件夹，全文件丢失，上网也找不到了……Failed

6、重下载5.1.1，回归原始问题

上网查询发现No such file or directory的另一解决方案及原理：Qt5的命名空间更改了，有许多原来在gui、core里的头文件被迁移到了widgets里，所以进行更改


#include<QtWidgets\QMainWindow>#include<QtWidgets\QScrollArea>#include<QtWidgets\QComboBox>#include<QtWidgets\QLabel>


事实证明，和


QT+=coregui
改成




QT+=coreguiwidgets
的效果是一样的……



7、'QVariant(Qt::GlobalColor)' is private







又一个被移除了的可怜孩纸……

把Qt::white等颜色更改为QColor(Qt::white);  记得加上#include<QColor>//现在依然在QtGui内




8、undefined reference to _imp___ZN8QPrinterC1ENS_11PrinterModeE'

一怒之下直接删除QPrinter模块，终于可以构建运行了

后记：啊啊啊原来的代码在哪里T^T，我知道解决方法了：

使用到打印相关的类，Qt5单独放到了QtPrintSupport模块中，所以得加头文件#include <QtPrintSupport>，在.pro中要加QT += printsupport




Exec Success，now editing...

9、界面中插入Gif



```cpp
#include "widget.h"
#include "ui_widget.h"
#include <QLabel>
#include <QMovie>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie("D:/Project/Qt/testclass/2.gif");
    ui->label->setMovie(movie);
    movie->start();
}

Widget::~Widget()
{
    delete ui;
}
```

```cpp
#include <QtGui/QApplication>
#include <QLabel>
#include <QMovie>

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);

    QLabel *label = new QLabel();
    QMovie *movie = new QMovie("D:/Project/Qt/firstQT/2.gif");
    label->setMovie(movie);
    movie->start();
    label->show();

    return app.exec();
}
```
























