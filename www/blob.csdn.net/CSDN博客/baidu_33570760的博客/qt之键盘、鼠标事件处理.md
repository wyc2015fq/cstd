# qt之键盘、鼠标事件处理 - baidu_33570760的博客 - CSDN博客
2017年05月12日 15:20:33[carman_风](https://me.csdn.net/baidu_33570760)阅读数：329
个人分类：[qt](https://blog.csdn.net/baidu_33570760/article/category/6898155)
## 转载自：https://wizardforcel.gitbooks.io/qt-beginning/content/12.html
## 导语
事件是对各种应用程序需要知道的由应用程序内部或者外部产生的事情或者动作的通称。对于初学者，总会对Qt中信号和事件的概念混淆不清。其实，记住事件比信号更底层就可以了。比如说，我们用鼠标按下界面上的一个按钮，它会发射`clicked()`单击信号，但是，它怎么知道自己被按下的呢，那就是通过鼠标事件处理的。这里可以看到，鼠标事件比信号更底层。
在Qt中处理事件有多种方法，不过最常用的是重写Qt事件处理函数。这里我们就以鼠标事件和键盘事件为例来进行简单的介绍。
环境是：Windows 7 + Qt 4.8.1 +Qt Creator 2.4.1
## 目录
- 一、鼠标事件
- 二、键盘事件
## 正文
一、鼠标事件
1．新建Qt Gui应用，项目名称为`myEvent`，基类更改为`QWidget`，类名为`Widget`。
2．完成项目创建后，在设计模式向界面上拖入一个`Push Button`。
3．在`widget.h`文件添加鼠标按下事件处理函数声明：
```cpp
protected:
    void mousePressEvent(QMouseEvent *);
```
4．到`widget.cpp`文件中先添加头文件包含：
```cpp
#include <QMouseEvent>
```
然后在下面添加函数的定义：
```cpp
void Widget::mousePressEvent(QMouseEvent *e)
{
   ui->pushButton->setText(tr("(%1,%2)").arg(e->x()).arg(e->y()));
}
```
这里的`arg()`里的参数分别用来填充%1和%2处的内容，`arg()`是`QString`类中的一个静态函数，使用它就可以在字符串中使用变量了。其中`x()`和`y()`分别用来返回鼠标光标所在位置的`x`和`y`坐标值。这样，当鼠标在界面上点击时，按钮就会显示出当前鼠标的坐标值。效果如下图所示。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/9-1.png)
除了鼠标按下事件，还有鼠标释放、双击、移动、滚轮等事件，其处理方式与这个例子是相似的。
二、键盘事件
1．首先在`widget.h`中添加`protected`函数声明：
```cpp
void keyPressEvent(QKeyEvent *);
```
2．然后到`widget.cpp`中添加头文件包含：
```cpp
#include <QKeyEvent>
```
3．最后添加键盘按下事件处理函数的定义：
```cpp
void Widget::keyPressEvent(QKeyEvent *e)
{
   int x = ui->pushButton->x();
   int y = ui->pushButton->y();
   switch (e->key())
   {
   case Qt::Key_W : ui->pushButton->move(x, y-10); break;
   case Qt::Key_S : ui->pushButton->move(x, y+10); break;
   case Qt::Key_A : ui->pushButton->move(x-10, y); break;
   case Qt::Key_D : ui->pushButton->move(x+10, y); break;
   }
}
```
这里我们先获取了按钮的位置，然后使用`key()`函数获取按下的按键，如果是指定的W、S、A、D等按键时则移动按钮。所有的按键都在`Qt::Key`枚举变量中进行了定义，大家可以在帮助文档中进行查看。
## 结语
除了键盘按下事件，常用的还有键盘释放事件，这里就不再举例。如果想了解更多事件方面的知识，可以参考《Qt Creator快速入门》一书的第6章的内容。
            
