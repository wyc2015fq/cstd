
# 使用C++来创建界面 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:24:17[seven-soft](https://me.csdn.net/softn)阅读数：2441


上节讲到，Qt 支持两种创建界面的方式：一种是使用C++代码，Qt 自诞生以来就支持；另一种是使用 QML，可以创建个性化的界面。
Qt 提供了多种工程类型，选择不同的工程类型将导致使用不同的方式来创建界面。本节先讲解C++的使用。
首次打开Qt，会出现欢迎界面，点击`New
 Project`按钮，或者在菜单栏点击`文件`，然后选择`新建文件或项目`，都会弹出向导对话框，如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120FQ94D22.png)
`Qt Widgets Application`和`Qt
 Console Application`工程将会使用C++代码，Console 用来创建控制台程序，Widgets 用来创建GUI程序。
`Qt Quick Application`、`Qt
 Quick Controls Application`和`Qt
 Canvas 3D Application`工程将会使用 QML 代码，Quick Control 主要比 Quick 多一些控件，Canvas 3D 用来创建3D动画。
选择`Qt
 Widgets Application`，点击`Choose...`按钮，弹出如下对话框：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120FRQ3209.png)
填写好项目名称和保存路径，点击`下一步`按钮，弹出工具选择对话框：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120FS0424T.png)
选中`Select
 all kits`即可。这里我使用的是 VS2010 版的Qt，你也可以使用其他编译器。点击`下一步`按钮：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120FS354563.png)
填写项目中要使用到的类的信息。简单起见，取消“创建界面”选项。单击`下一步`按钮，弹出“项目管理”对话框，点击`完成`按钮，完成项目的创建。
之后可以看到，IDE自动生成了四个文件：WidgetsDemo.pro 为工程文件，它是 qmake 产生的用于生成 makefile 的配置文件，它的名称和项目名称是一样的。我们暂时不用理会。
main.cpp 是主文件，里面就是一个 main() 函数，程序从该文件开始执行。
mainwindow.h 和 mainwindow.cpp 分别是 MainWindow 类的头文件和源文件。MainWindow 是我们在创建项目时指定的名字。
整个项目的示意图如下：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120G43116428.png)
现在点击`运行(Run)`按钮，或者按下`Ctrl+R`快捷键，就可以看到编译结果，如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120G43412M0.png)
这是一个最简单的 Qt Widget 程序。
## 显示“Hello World”
接下来，我们让程序显示"Hello World!"。打开 mainwindow.cpp，在构造函数 MainWindow() 中添加如下代码：[复制](http://c.biancheng.net/cpp/html/3055.html#)[纯文本](http://c.biancheng.net/cpp/html/3055.html#)[新窗口](http://c.biancheng.net/cpp/html/3055.html#)
//设置标题
this->setWindowTitle(tr("Widgets演示"));
//设置客户区大小
this->resize(300,120);
//设置背景颜色
QPalettepalette=this->palette();//获取调色板
palette.setColor(QPalette::Window,QColor(255,255,255));//设置为白色
this->setPalette(palette);//重新设置调色板
//添加QLabel控件
QLabel*label=newQLabel(this);//创建QLabel控件
setCentralWidget(label);//将当前控件至于中央
label->setText(tr("Hello World!"));//设置控件文本
label->setAlignment(Qt::AlignCenter);//文本水平和垂直都居中

运行结果为：
![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120G509434b.png)
如果你有Web开发经验，也可以在设置文本时使用简单的HTML。将上面第 13 行代码改为：label->setText(tr("<font color='red' face='微软雅黑' size='6'>Hello World!</font>"));
运行结果为：
![](http://c.biancheng.net/cpp/uploads/allimg/151207/1-15120G51551N5.png)

