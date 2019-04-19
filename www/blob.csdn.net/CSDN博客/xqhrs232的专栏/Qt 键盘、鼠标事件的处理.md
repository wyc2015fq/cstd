# Qt 键盘、鼠标事件的处理 - xqhrs232的专栏 - CSDN博客
2017年10月11日 11:03:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：146
原文地址::[http://blog.csdn.net/skc361/article/details/17930521](http://blog.csdn.net/skc361/article/details/17930521)
相关文章
1、[QT基础(二)----鼠标、键盘事件处理机制、信息拦截机制](http://blog.csdn.net/qianqin_2014/article/details/51234752)----[http://blog.csdn.net/qianqin_2014/article/details/51234752](http://blog.csdn.net/qianqin_2014/article/details/51234752)
事件是对各种应用程序需要知道的由应用程序内部或者外部产生的事情或者动作的通称。对于初学者，总会对Qt中信号和事件的概念混淆不清。其实，记住事件比信号更底层就可以了。比如说，我们用鼠标按下界面上的一个按钮，它会发射clicked()单击信号，但是，它怎么知道自己被按下的呢，那就是通过鼠标事件处理的。这里可以看到，鼠标事件比信号更底层。
    在Qt中处理事件有多种方法，不过最常用的是重写Qt事件处理函数。这里我们就以鼠标事件和键盘事件为例来进行简单的介绍。 
**一、鼠标事件**
1.新建Qt Gui应用，项目名称为“myEvent”，基类更改为QWidget，类名为Widget。
2.完成项目创建后，在设计模式向界面上拖入一个Push Button。
3.在widget.h文件添加鼠标按下事件处理函数声明：
protected:
void*mousePressEvent*(QMouseEvent*); 
4.到widget.cpp文件中先添加头文件包含：
#include<QMouseEvent>
然后在下面添加函数的定义：
voidWidget::*mousePressEvent*(QMouseEvent*e)
{
ui->pushButton->setText(tr("(%1,%2)").arg(e->x()).arg(e->y()));
}
这里的arg()里的参数分别用来填充%1和%2处的内容，arg()是QString类中的一个静态函数，使用它就可以在字符串中使用变量了。其中x()和y()分别用来返回鼠标光标所在位置的x和y坐标值。这样，当鼠标在界面上点击时，按钮就会显示出当前鼠标的坐标值。效果如下图所示。
除了鼠标按下事件，还有鼠标释放、双击、移动、滚轮等事件，其处理方式与这个例子是相似的。
**二、键盘事件**
1.首先在widget.h中添加protected函数声明：
void*keyPressEvent*(QKeyEvent*); 
2.然后到widget.cpp中添加头文件包含：
#include<QKeyEvent>
3.最后添加键盘按下事件处理函数的定义：
voidWidget::*keyPressEvent*(QKeyEvent*e)
{
intx=ui->pushButton->x();
inty=ui->pushButton->y();
switch(e->key())
{
caseQt::Key_W:ui->pushButton->move(x,y-10);break;
caseQt::Key_S:ui->pushButton->move(x,y+10);break;
caseQt::Key_A:ui->pushButton->move(x-10,y);break;
caseQt::Key_D:ui->pushButton->move(x+10,y);break;
}
}
这里我们先获取了按钮的位置，然后使用key()函数获取按下的按键，如果是指定的W、S、A、D等按键时则移动按钮。所有的按键都在Qt::Key枚举变量中进行了定义，大家可以在帮助文档中进行查看。 

