# Qt深入浅出（一） 从零开始写Qt应用

2018年02月24日 00:08:55 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：5605



## 1 Qt简单介绍



### 1.1 什么是Qt

​	Qt是1991年由奇趣科技开发的跨平台C++图形用户界面应用程序开发框架。它既可以开发GUI（Graphical User Interface）程序，也可用于开发非GUI程序，比如控制台程序。与windows上面的MFC是同一类东西。它提供给应用程序开发者建立艺术级的图形用户界面所需的所有功能，并且Qt很容易扩展。



​	Qt Creator是一个用于Qt开发的轻量级跨平台集成开发环境。Qt Creator可带来两大关键益处：提供首个专为支持跨平台开发而设计的IDE(integrated development environment)，并确保首次接触Qt框架的开发人员能迅速上手和操作。即使不开发Qt应用程序，Qt Creator也是一个简单易用且功能强大的IDE。



​	Qt真正指的是Qt库，并不是Qt Creator，官网上面的Qt版本号5.7.0也是指的是Qt库的版本，开发者也可以使用VS集成开发环境来开发基于Qt的应用程序。



### 1.2 优良的跨平台特性

​	Qt支持下列操作系统: Microsoft Windows 95/98， Microsoft Windows NT， Linux， Solaris， SunOS， HP-UX， Digital UNIX (OSF/1， Tru64)， Irix， FreeBSD， BSD/OS， SCO， AIX， OS390，QNX 等等。



​	Qt在各个平台上都封装了同一套对外的编程接口API(Application Program Interface),开发者可以在一个平台上编写Qt程序, 同时可以将不经过任何修改的代码放到其它平台中编译, 可以生成对应平台的应用程序.

 	简单来说，就是一份代码到处编译。











### 1.3 第一个Qt GUI应用

​	使用Qt Creator集成开发环境开发应用是通过qmake来组建项目，qmake与cmake类似，qmake语法更简单，cmake功能更强大。

​	qmake能够自动生成Makefile、Microsoft Visual Studio 专案文件。不管源代码是否是用Qt写的，都能使用qmake，因此qmake能用于很多软件的构建过程，qmake文件的后缀一般为.pro。



1. `新建qmake空项目，添加main.cpp源文件`

   ![img](https://img-blog.csdn.net/20180224000603773)

2. `编写.pro项目工程qmake文件，加上QT变量`

      ![img](https://img-blog.csdn.net/20180224000629745)

3. `main.cpp中添加QApplication头文件,并且实例化`

   ```
   #include <QApplication>
   int main(int argc, char** argv)
   {
       QApplication app(argc, argv); //每个Qt应用只能有一个QApplication对象，QApplication对象主要用来处理主程序控制流和主要设置。
   }
   ```

   

4. `添加QWidget头文件，实例化对象，调用QWidget对象的show函数显示`

   ```
   #include <QApplication>
   #include <QWidget>
   int main(int argc, char** argv)
   {
       QApplication app(argc, argv);
       QWidget widget; //Qt中每个窗口都是QWidget对象，或者其派生类的对象。
       widget.show();
   }
   ```

   

5. `调用QApplication::exec()消息循环, 接受来自桌面系统的消息`

   ```
   #include <QApplication>
   #include <QWidget>
   int main(int argc, char** argv)
   {
       QApplication app(argc, argv);
       QWidget widget;
       widget.show();
       return app.exec(); //消息循环，消息泵，阻塞接收桌面系统传递过来的消息。
   }
   ```

6. `设置窗口标题`

   ```
   #include <QApplication>
   #include <QWidget>
   int main(int argc, char** argv)
   {
       QApplication app(argc, argv);
       QWidget widget;
       widget.setWindowTitle("helloworld");
       widget.show();
       return app.exec();
   }
   ```

重要知识:

 	1、Qt中大部分的类都是继承至QObject，QObject是最基本的类, 通过帮助文档可查看继承关系。

 	2、QApplication用来管理界面应用的控制流以及主要的程序设置。在Qt中不管有多少的窗口，QApplication实例对象都只有一个。

​	3、QWidget 在Qt中是所有窗口类的基类。











## 2 认识窗口



### 2.1 窗口大小

​	Qt中每一个窗口都可以设置其窗口大小，以像素为单位。

```
void QWidget::resize(int w, int h) //设置窗口大小
QSize size() const   //获取窗口大小，返回的是QSize，QSize封装了宽高
```



### 2.2 窗口坐标系



​	Qt中每一个窗口都有一个坐标系统，默认的，窗口左上角为坐标原点，水平向右递增，水平向左递减，垂直向下递增，垂直向上递减。原点即为（0，0）点，以像素为单位增减。

​	如图：

​	![img](https://img-blog.csdn.net/20180224000652807)



### 2.3 窗口关系

​	普通窗口可以以顶级窗口方式显示，比如直接调用show函数来显示；也可以嵌套在其它窗口上来显示，这个时候被嵌套的窗口叫做父窗口，嵌套的窗口叫做子窗口。（注意：父子窗口是对象间的组合关系，不是父子类继承关系）

​	子窗口位置坐标参考的是父窗口的坐标系。

​	顶级窗口其父窗口就是桌面，桌面也可以认为是一个窗口。





- 设置父窗口

```
void QWidget::setParent(QWidget *parent)
```

例如：

```
#include <QApplication>
#include <QWidget>
#include <QPushButton>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget widget;
    QPushButton pb;
    pb.setParent(&widget);  //注意将按钮嵌套到widget窗口中的时候，必须在widget对象调用show函数之前，否则pb窗口不显示。
    widget.show();
    return app.exec();
}
```



- 设置窗口位置坐标

```
void QWidget::move(int w, int h)   //设置窗口位置，单位像素
void QWidget::setGeometry(int x, int y, int w, int h) //设置窗口几何属性，坐标与大小，单位像素
QPoint pos() const     //获取窗口位置，返回QPoint类对象，QPoint封装了x、y坐标
```

注意：窗口移动的作用点是该窗口的坐标原点，也就是窗口的左上角。







###  

### 2.4 使用信号槽

​	信号和槽用于两个QObject对象或者其派生类之间的通信，信号和槽机制是Qt的核心特征，也是Qt不同于其它开发框架的最突出的特征。

​	例如：QPushButton对象点击之后会发射出一个clicked()信号，我们现在要把这个信号传递给QWidget对象，并且让QWidget对象调用close()函数关闭窗口。

- 那么要使用到QObject的一个静态函数connect：

```
[static] QMetaObject::Connection QObject::connect(const QObject *sender, const QMetaMethod &signal, const QObject *receiver, const QMetaMethod &method, Qt::ConnectionType type = Qt::AutoConnection);

/*将sender对象的信号signal链接到receiver对象的槽函数method，并且返回一个QMetaObject::Connection对象，可以使用该对象调用disconnect函数来注销链接。
type一般选用默认参数即可。
*/
```



​	源码：

```
#include <QApplication>
#include <QWidget> 
#include <QPushButton>
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWidget widget;  
    QPushButton pb;
    pb.setParent(&widget);
    QObject::connect(&pb, SIGNAL(clicked()), &widget, SLOT(close()));
    widget.show();
    return app.exec();
}
```





- 信号作为connect的传入参数时候需要使用SIGNAL()这个宏函数来包装一下
- 槽函数作为connect的传入参数时候需要使用SLOT()这个宏函数来包装一下

初步认识就到这里，后面会再详细介绍Qt中的信号槽机制。