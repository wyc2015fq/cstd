# Qt窗口组件实现动态背景功能 - fanyun的博客 - CSDN博客
2017年10月28日 21:03:23[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1101
Qt窗口组件实现动态背景实际上就是使用QMovie类的动态播放功能，测试代码如下：
```cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMovie>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    QMovie *iconShow = new QMovie(":/icon/iconshow.gif");
    ui->label->setMovie(iconShow);
    iconShow->start();
}
MainWindow::~MainWindow()
{
    delete ui;
}
```
显示效果如图1所示：
![](https://img-blog.csdn.net/20171028210517906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                                                           图1 
