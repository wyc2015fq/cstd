
# Qt窗口之QMainWindow、QDialog、QWidget -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:29:08[seven-soft](https://me.csdn.net/softn)阅读数：970


在 Qt 中，我们将窗口和控件统称为部件（Widget）。窗口是指程序的整体界面，可以包含标题栏、菜单栏、工具栏、关闭按钮、最小化按钮、最大化按钮等；控件是指按钮、复选框、文本框、表格、进度条等这些组成程序的基本元素。一个程序可以有多个窗口，一个窗口也可以有多个控件。
QWidget 是所有用户界面元素的基类，窗口和控件都是直接或间接继承自 QWidget，读者可以下载[Qt类继承图（提取密码为
 spv1）](http://pan.baidu.com/s/1kUtByIV)详细查看。
> 该图是 Qt 4.3 的类继承图，大部分和 Qt 5 相同，读者可以借此来了解 Qt 有哪些类、以及它们之间的关系。
一个GUI程序必须包含一个窗口。在创建 Qt Widgets 项目时，有一个步骤是让我们选择基类，如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151214/1-15121409350U10.png)
QMainWindow、QWidget、QDialog 三个类就是用来创建窗口的，可以直接使用也可以继承后再使用。
QMainWindow 窗口可以包含菜单栏、工具栏、状态栏、标题栏等，是最常见的窗口形式，可以作为GUI程序的主窗口。如下图所示：![](http://c.biancheng.net/cpp/uploads/allimg/151214/1-151214121R3J4.png)

QDialog 是对话框窗口的基类。对话框主要用来执行短期任务，或与用户进行互动，它可以是模态的也可以是非模态的。QDialog 没有菜单栏、工具栏、状态栏等，如下乳所示：![](http://c.biancheng.net/cpp/uploads/allimg/151214/1-15121413253QK.png)
如果是主窗口，就使用 QMainWindow；如果是对话框，就是用 QDialog；如果不确定，或有可能作为顶级窗口，也有可能嵌入到其他窗口中，那么使用 QWidget。
需要注意的是，窗口和控件都继承自 QWidget，如果不为控件指定父对象，它就会被作为窗口处理，这时 setWindowTitle() 和 setWindowIcon() 函数就会生效。请看下面的代码：\#include<QApplication>
\#include<QLabel>
\#include<QPushButton>
intmain(intargc,char*argv[])
{
QApplicationapp(argc,argv);
QLabellabel("这是Label控件");//没有父对象
label.setWindowTitle("Label控件作为顶级窗口");
label.setGeometry(100,50,360,160);
label.setAlignment(Qt::AlignCenter);
label.show();
QPushButtonbtn;//没有父对象
btn.setWindowTitle("PushButton控件作为顶级窗口");
btn.setText("这是个大大的按钮");
btn.setGeometry(480,50,360,160);
btn.show();
returnapp.exec();
}

运行代码，会在距离显示器 x=100, y=50 的位置弹出两个窗口，如下图所示：
![](http://c.biancheng.net/cpp/uploads/allimg/151214/1-151214135240491.png)
可以发现，窗口和控件没有绝对的区别，控件也可以成为窗口，这取决于它有没有父对象。

