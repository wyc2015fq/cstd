
# 分析第一个Qt程序代码 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:28:25[seven-soft](https://me.csdn.net/softn)阅读数：1746个人分类：[Qt																](https://blog.csdn.net/softn/article/category/6274094)



在《[使用C++来创建界面](http://c.biancheng.net/cpp/html/3055.html)》一节中我们创建一个
 Qt Widgets 工程，共包含了4个文件，现在请大家把 mainwindow.cpp 和 mainwindow.h 删除，保留主文件 main.cpp 和工程文件即可。
把下面的代码复制到 main.cpp：\#include<QApplication>
\#include<QMainWindow>
\#include<QLabel>
intmain(intargc,char*argv[])
{
QApplicationapp(argc,argv);
QMainWindoww;
w.setWindowTitle("Widgets演示");//设置标题
w.resize(300,140);//设置客户区大小
QLabellabel("C语言中文网",&w);
label.setGeometry(100,50,160,30);
w.show();
returnapp.exec();
}

运行结果：
![](http://c.biancheng.net/cpp/uploads/allimg/151212/1-151212104G4420.png)
下面我们来分析一下这段简单的代码。
1) 第1~3行代码引入对应的头文件。在Qt中，头文件设计的非常规范，大部分情况下一个类对应一个头文件，类名和头文件名相同，以字母“Q”开头。一个控件对应一个类，要想使用控件，必须包含对应的头文件。
Qt 中的头文件被分门别类地放到了不同的文件夹。对于 MinGW 版本，头文件位于安装目录下的 \5.5\mingw492_32\include\ 文件夹中，如下图所示：
![](http://c.biancheng.net/cpp/uploads/allimg/151212/1-15121211542W36.png)
根据语法，包含头文件时应该带上目录，例如 <QLabel> 位于 QtWidgets 中，要写作`#include
 <QtWidgets/QLabel>`，但我们并没有这样做，因为在工程文件 WidgetsDemo.pro 中已经包含了 core 和 gui 模块。后续我们会讲解工程文件和 Qt 模块。
2) Qt 程序的入口函数和 C++ Console 程序一样，都是 main() 函数；参数也是一样的。不了解的读者请猛击：[C语言main()函数详解](http://c.biancheng.net/cpp/html/725.html)
3) 第7行创建了一个 QApplication 对象。QApplication 类主要用来管理程序的生命周期（包括初始化设置和销毁工作），开启事件循环。对于任何一个 Qt GUI 程序，必须有且仅有一个 QApplication
 对象，但可以存在零个或多个窗口。
main() 函数一般以创建 QApplication 对象开始，后面才是实际业务的代码。
4) 第9行代码中，QMainWindow 表示主窗口，它是一个顶级窗口。关于窗口，我们将在下节详细介绍。
5) 第13行创建了一个文本控件，用来显示字符串。第一个参数指定要显示的字符串，第二个参数指定父窗口。
读者要注意第二个参数。在 Qt 中，一个控件可以有父辈，这个父辈可以是一个窗口，也可以是一个容器控件。父辈有管理控件的作用，当父辈被销毁时，子控件也都被销毁，释放内存。后续会详细介绍。
6) 第14行设置 Label 控件的几何尺寸，100 为左边距，50 为上边距，160 为控件宽度，30为控件高度，单位都是像素（px）。边距是相对父辈 w 来说的。后续会详细介绍坐标系统。
7) 第15行用来显示控件。控件被创建后只是分配了内存，并不可见，还需要调用 show() 函数显示出来。w 是 label 的父辈，父辈调用 show() 函数也会显示所有子控件，所以 label 就不需要再调用 show()
 了。
8) exec() 使程序进入消息循环，等待可能发生的事件，例如鼠标点击、键盘输入等。这里 main() 把控制权转交给 Qt，由 Qt 完成事件处理工作，程序退出时 exec() 的值就会返回。在 exec() 中，Qt
 接受并处理用户和系统的事件，把它们传递给适当的窗口控件。
关于消息循环，在《[Windows编程](http://c.biancheng.net/cpp/windows/)》中已经进行了详细介绍，不了解的读者请自行脑补。

