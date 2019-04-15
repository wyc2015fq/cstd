# Qt深入浅出（三）Qt的内存管理机制

2018年02月24日 00:16:34 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：525



## 4 内存管理机制

- 一般我们不直接用QWidget来实例化对象，用它的派生类来生成对象，main函数中一般不写太多代码，都是在派生类的构造函数中进行窗口的初始化、布局、设置、其子窗口设置等等。

例如:

main.cpp

```
#include <QApplication>
#include "widget.h"
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Widget widget;
    widget.show();
    return app.exec();
}
```

widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
public:
    Widget(QWidget *parent = 0); //继承QWidget或者其派生类的时候，一般要实现带一个QWidget*参数的构造函数，用来指定其父窗口的对象。

};

#endif
```



widget.cpp

```
#include "widget.h"
#include <QPushButton>
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(400, 300);
    this->move(100, 100);
}
```







- 构造函数中要用new来申请堆窗口对象，不能使用栈对象, 因为栈对象生命周期只在构造函数内，构造函数执行完成后，栈对象就被销毁了，于是看不到该窗口。

 以下是错误的写法：

```
#include "widget.h"
#include <QPushButton>
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(400, 300);
    this->move(100, 100);
    QPushButton pb;
    pb.setParent(this);
}
```

以下是正确写法：

```cpp
#include "widget.h"
#include <QPushButton>
Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->resize(400, 300);
    this->move(100, 100);
    QPushButton pb* = new QPushButton;
    pb->setParent(this);
}
```







- 我们看到以上正确代码只有new而没有delete，难道是课件代码写错了？我是否还要在析构函数里delete呢？

  ​	其实是Qt有一套回收内存的机制，子窗口可以通过指定父窗口，来托管子窗口内存的释放，而父窗口又通过它的父窗口来销毁对象，顶级窗口一般在main函数中实例化为可自动销毁的栈对象，子窗口无需进行额外的释放。

  ​	我们可以通过派生一个子窗口类，然后分别在父子窗口析构函数里打印信息，来观察子窗口的释放过程。

  

- 是不是只有窗口类对象有这套管理机制，还是所有的Qt类都可以使用这套管理机制？

  ​	所有的QObject派生类都可以通过指定父对象进行内存托管，QWidget继承至QObject，所以它能使用这套管理机制，后面我们还会接触到其它的QObject派生类对象，例如QGraphicsItem，它也可以使用这套内存管理机制。

- 那么所有的子对象只能在父对象销毁的时候才能一起销毁，势必造成内存持续占用，能不能我们自己来销毁呢？

  ​	当然，开发者可以使用delete来释放QObject对象，但是不推荐这么使用，QObject对象远比普通的类要复杂的多，QObject底层实现在我们不知道的情况下，正进行着一些工作。如果这个时候使用delete强制释放，会造成不可预知的错误。

  ​	一般我们会使用deleteLater函数，进行安全释放，它会等待对象所有事情都处理完毕后才释放。

  ```cpp
  [slot] void QObject::deleteLater();
  ```

  