# QT之窗口 - LC900730的博客 - CSDN博客
2017年08月20日 15:34:30[lc900730](https://me.csdn.net/LC900730)阅读数：192
## QWidget
基础窗口部件QWidget类是所有用户的界面对象的基类，窗口和控件都是直接或间接继承自QWidget。
### Widget
窗口部件(widget)简称部件，是QT中建立界面的主要元素,QT 
中把没有嵌入到其他部件的称为窗口。一般窗口都有边框，标题栏，窗口是指程序的整体界面，可以包含标题栏、菜单栏、工具栏、关闭按钮、最大最小化；
### 控件
指的是按钮、复选框、文本框、表格、进度条等这些组成程序的基本元素。一个程序可以有多个窗口，一个窗口也可以有多个控件。(但是其实控件有时候就是窗口)
```
QWidget *widget=new QWidget();
//没有指定父窗口
QLabel *label=new QLabel();
label->setWindowTitle("我是label");
label->setText("label1:我是个窗口");
//指定了父窗口为widget
QLabel *label2=new QLabel(widget);
label2->setText("label2:不是独立窗口，只是widget子部件");
```
## 窗口类型
QWidget的构造函数中有2个函数：QWidget *parent=0；和Qt::WindowFlags f=0。parent指的是父窗口部件，默认为0表示没有父亲窗口，而后面的f参数是Qt::WindowFlags类型，是Qt::WindowType枚举类型或者组合，0表示是Qt::Widget，默认的类型。
QMainWindow、QWidget、QDialog三个类就是用来创建窗口的，可以直接使用也可以继承后在使用。
QMainWindow窗口可以包含菜单栏、工具栏、状态栏、标题栏，是最常见的 窗口形式，可以作为GUI程序主要窗口；
QDialog是对话框窗口的基类，对话框主要用来执行短期任务，或与用户进行互动，可以是模态的也可以是非模态的。QDialog没有菜单栏、工具栏、状态栏等。
如果是主窗口就用QMainWindow，如果是对话框就用QDialog；如果不确定，或有可能作为顶级窗口，也有可能嵌入到其他窗口中，那么使用QWidget。
窗口和控件都继承QWidget，如果不为控件指定父对象，它就会被当做窗口处理。
因此：窗口和控件没有绝对区别，控件也可以成为窗口，这取决于其有没有父对象。
