# Qt入门教程系列之四     信号槽机制详解 - DumpDoctorWang的博客 - CSDN博客





2018年08月14日 19:29:28[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：522








# 一、简介

在GUI编程中，当我们改变一个控件(比如Widget)时，通常把改变通知到UI中的另一个控件。更一般地，希望任何类型的对象能够彼此通信。 例如，如果用户单击“关闭”按钮，我们可能希望调用窗口的close()函数。信号槽机制是用来在两个对象(两个实例)之间通信。信号槽机制是Qt的核心特性之一，也是Qt和其他框架的差别。

其他框架使用回调实现这种通信。 回调是指向函数的指针，因此如果您希望处理函数通知您某些事件，则将指针传递给处理函数的另一个函数（回调）。 然后，处理函数在适当时调用回调。 虽然确实存在使用此方法的成功框架，但回调可能不直观，并且可能在确保回调参数的类型正确性方面存在问题。

# 二、信号和槽

在Qt中，有一种替代回调的技术：信号和槽。发生特定事件时会发出信号。 Qt的控件有许多预定义的信号，但可以继承控件，并向它们添加我们自定义的信号。 槽是响应于特定信号而被调用的函数。 Qt的控件有许多预定义的槽，但通常的做法是继承控件并添加自定义的槽，以便可以处理感兴趣的信号。

![](http://doc.qt.io/qt-5/images/abstract-connections.png)

上图就是信号槽简单的示例。

信号槽机制是类型安全的：信号的签名必须与槽的签名匹配(签名匹配就是函数的返回类型，参数类型匹配；一个槽的参数可能比它收到的信号更少，因为它可以忽略参数)。 **信号和槽松散耦合：发出信号的类既不知道也不关心哪个槽接收信号。** Qt的信号和槽机制确保如果你将信号连接到槽，将在适当的时间使用信号的参数调用槽。 信号和槽可以采用任何类型的任意数量的参数。 它们完全是类型安全的。

从QObject或其子类（例如，QWidget）继承的所有类都可以包含信号和槽。它不知道或关心是否有任何东西正在接收它发出的信号，这是真正的信息封装。

槽可用于接收信号，但它们也是普通的成员函数。就像一个对象不知道是否有任何东西接收到它的信号一样，一个槽也不知道它是否有任何信号连接到它。

可以将任意数量的信号连接到单个插槽，也可以根据需要将信号连接到任意数量的槽。甚至可以将信号直接连接到另一个信号。 （每当发射第一个信号时，这将立即发出第二个信号。）

信号和槽共同构成了一个强大的信号槽机制。

### 信号(Signal)

当对象的内部状态发生更改时，会发出信号。比如按钮被点击，就会发出"clicked()"这个信号。信号是公有函数，可以从任何地方发出，但建议只从定义信号及其子类的类中发出信号。

发出信号时(关键字为emit，例如emit clicked();)，通常会立即执行与其连接的槽，就像正常的函数调用一样。发生这种情况时，信号和槽机制完全独立于任何GUI事件循环。所有槽返回后，执行emit语句后的代码。使用队列连接时情况略有不同;在这种情况下，emit关键字后面的代码将立即被执行，并且槽将在稍后执行(相当于是异步执行)。

如果多个槽连接到一个信号，则在发出信号时，槽将按照它们连接的顺序依次执行。

信号由moc自动生成，不得在.cpp文件中实现，永远不会有返回值（即使用void）。

关于参数的说明：我们的经验表明，如果信号和槽不使用特殊的参数，则它们的重用性更好。如果QScrollBar::valueChanged()使用特殊类型，例如QScrollBar::Range，则它只能连接到专门为QScrollBar设计的槽。

### 槽(Slot)

当一个信号被发射时(emit)，会调用连接到信号的槽。槽是普通的C++函数，可以像调用普通函数一样调用槽。和普通函数的区别就是信号可以连接到它们。

由于槽是普通的成员函数，因此它们在直接调用时遵循普通的C++规则。**但是，作为槽，它们可以通过连接信号由任何组件调用，而不管其访问级别如何。**这意味着从任意类的实例发出的信号可以导致在其他类的实例中调用私有槽。

还可以将插槽定义为虚拟插槽，我们发现这些插槽在实践中非常有用。

与回调相比，信号槽机制稍微慢一些，因为它们提供了更大的灵活性，在实际应用的速度差异是微不足道的。

请注意，定义了signals或slots变量的其他库在与基于Qt的应用程序一起编译时可能会导致编译器警告和错误。

# 三、一个小例子

一个普通的C++计数类如下

```cpp
class Counter
{
public:
    Counter() { m_value = 0; }

    int value() const { return m_value; }
    void setValue(int value);

private:
    int m_value;
};
```

假设有一个需求是当Counte的value变化时，需要通知另外类A的一个实例a，当然，可以在setValue里面调用a的成员函数，不过，这样A类和Counter类就不相互独立了。且如果要通知n个类A的实例，难道要在setValue里调用n个实例的成员函数？所以我们需要用到信号槽机制。

打开Qt新建项目Signal-Slot。主类的名字取为SignalSlot。向项目中添加一个继承QObject的计数类Counter和类A

![](https://img-blog.csdn.net/20180814150902508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

向counter.h中添加成员函数value，信号valueChanged和槽setValue

```cpp
#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>

class Counter : public QObject
{
    Q_OBJECT
public:
    explicit Counter(QObject *parent = nullptr);
    int value();

signals:
    //值改变了的信号
    void valueChanged(int newValue);

public slots:
    // 槽
    void setValue(int value);

private:
    int m_value;
};

#endif // COUNTER_H
```

要让一个类支持Qt的信号槽机制，必须在类的开始写上宏Q_OBJECT，且直接或者间接继承QObject。要让Counter类在值变化时发出信号，setValue的实现如下

```cpp
void Counter::setValue(int value)
{
    if (value != m_value) {
        m_value = value;
        emit valueChanged(value);//发出信号
    }
}
```

整个counter.cpp如下

```cpp
#include "counter.h"

Counter::Counter(QObject *parent) : QObject(parent)
{
    m_value = 0;
}

int Counter::value()
{
    return m_value;
}

void Counter::setValue(int value)
{
    if (value != m_value) {
        m_value = value;
        emit valueChanged(value);//发出信号
    }
}
```

接下来，向A中添加一个槽receiveNewValue，参数要和要连接的信号相同，也可以没有参数；添加属性id；修改a的构造函数。a.h如下

```cpp
#ifndef A_H
#define A_H

#include <QObject>

class A : public QObject
{
    Q_OBJECT
public:
    explicit A(int id, QObject *parent = nullptr);

signals:

public slots:
    void receiveNewValue();
    void receiveNewValue(int newValue);

private:
    int m_id;
};

#endif // A_H
```

a.cpp如下

```cpp
#include "a.h"
#include <QDebug>

A::A(int id, QObject *parent) : QObject(parent)
{
    m_id = id;
}

void A::receiveNewValue()
{
    qDebug()<<"class A:"<<m_id<<"---value changed"<<endl;
}

void A::receiveNewValue(int newValue)
{
    qDebug()<<"class A:"<<m_id<<"---value changed to:"<<newValue<<endl;
}
```

此时，转到main.cpp中，添加如下测试代码

```cpp
#include "signalslot.h"
#include <QApplication>

#include<QObject>
#include "counter.h"
#include "a.h"

void testCounter(){
    Counter counter;
    A a1(1),a2(2),a3(3);
    // 连接counter的valueChanged(int)与a1的receiveNewValue()
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a1,
            SLOT(receiveNewValue()));
    // 连接counter的valueChanged(int)与a1的receiveNewValue(int)
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a1,
            SLOT(receiveNewValue(int)));
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a2,SLOT(receiveNewValue()));
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a2,SLOT(receiveNewValue(int)));
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a3,SLOT(receiveNewValue()));
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a3,SLOT(receiveNewValue(int)));
    counter.setValue(1000);
    counter.setValue(2000);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SignalSlot w;
    w.show();

    testCounter();
    return a.exec();
}
```

运行程序，可以看到A中的valueReceived函数被调用了

![](https://img-blog.csdn.net/20180814163541385?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**恭喜你！你已经学会了信号槽机制的高级技术，自定义信号和槽！ **

# **四、信号与槽的关联**

信号与槽的连接使用connect函数连接，想必大家已经知道了。信号槽机制还有如下特点
- 一个信号可以连接多个类中的槽
- 一个信号可以连接同一个槽多次
- 一个槽可以被多个信号连接
- 可以使用disconnect取消连接

**切记：避免重复关联信号与槽，尽量在构造函数内关联信号和槽，或者在connect传入参数Qt::UniqueConnection。**若重复连接信号和槽，会导致槽被调用多次。

### **一个信号可以**连接**多个类中的槽，只要信号与槽的参数匹配**

向项目添加一个类B，并添加槽receiveNewValue。b.h如下

```cpp
#ifndef B_H
#define B_H

#include <QObject>

class B : public QObject
{
    Q_OBJECT
public:
    explicit B(QObject *parent = nullptr);
    void emitSignal();

signals:
    void BbWhat(int value);

public slots:
    void receiveNewValue(int newValue);
};

#endif // B_H
```

b.cpp如下

```cpp
#include "b.h"
#include <QDebug>

B::B(QObject *parent) : QObject(parent)
{

}

void B::emitSignal()
{
    emit BbWhat(1111111);
}

void B::receiveNewValue(int newValue)
{
    qDebug()<<"class B---value changed to:"<<newValue<<endl;
}
```

在main.cpp包含头文件“b.h”；添加函数one2many，如下；并在main函数中调用

```cpp
void one2many(){
    Counter counter;
    A a(1);
    B b;

    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a,SLOT(receiveNewValue(int)));
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&b,SLOT(receiveNewValue(int)));

    counter.setValue(2333);
}
```

运行程序 

![](https://img-blog.csdn.net/20180814163603286?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### **一个信号可以**连接**同一个槽多次**

在main.cpp中添加函数duplicate，如下；并在main.cpp中调用

```cpp
void duplicate(){
    Counter counter;
    A a(1);

    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a,SLOT(receiveNewValue(int)));
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a,SLOT(receiveNewValue(int)));
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a,SLOT(receiveNewValue(int)));

    counter.setValue(23333);
}
```

运行，结果如下

![](https://img-blog.csdn.net/20180814164246362?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以发现a. receiveNewValue只被调用了3次。要避免重复关联，可在connect函数里传入参数**Qt::UniqueConnection，**如下

```cpp
void duplicate(){
    Counter counter;
    A a(1);

    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a,SLOT(receiveNewValue(int)));
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a,SLOT(receiveNewValue(int)),Qt::UniqueConnection);
    QObject::connect(&counter,SIGNAL(valueChanged(int)),&a,SLOT(receiveNewValue(int)),Qt::UniqueConnection);

    counter.setValue(23333);
}
```

再次运行，可以发现a. receiveNewValue只被调用了一次。connect函数有五个参数，最后一个参数是type，type类型是Qt::ConnectionType，是一个枚举。Qt::ConnectionType的取值如下
|Constant|Value|Description|
|----|----|----|
|Qt::AutoConnection|0|（默认）如果receiver和sender在同一个线程，则使用Qt::DirectConnection。 否则，使用Qt ::QueuedConnection。 连接类型在发出信号时确定。|
|Qt::DirectConnection|1|发出信号时立即调用槽。 槽在发出信号的线程中执行。|
|Qt::QueuedConnection|2|当控制权返回到receiver线程的事件循环时，将调用该槽。 槽在receiver的线程中执行。|
|Qt::BlockingQueuedConnection|3|与Qt::QueuedConnection相同，发出信号的线程会阻塞，直到槽执行完。如果receiver和sender在同一个线程，则不得使用此参数，否则应用程序将死锁。|
|Qt::UniqueConnection|0x80|这是一个可以使用按位或(|)与上述任何一种连接类型组合的标志。 当设置Qt::UniqueConnection时，如果连接已经存在（即，如果相同的信号已连接到同一对对象的相同槽），QObject::connect()将失败。|

### **一个槽可以被多个信号连接**

在main.cpp添加如下many2one，如下；并在main.cpp中调用

```cpp
void many2one(){
     Counter counter;
     A a(1);
     B b;

     QObject::connect(&counter,SIGNAL(valueChanged(int)),&a,SLOT(receiveNewValue(int)));
     QObject::connect(&b,SIGNAL(BbWhat(int)),&a,SLOT(receiveNewValue(int)));

     counter.setValue(11111);
     b.emitSignal();
}
```

运行，结果如下

![](https://img-blog.csdn.net/20180814165329605?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### **使用disconnect取消信号与槽之间的连接**

在main.cpp添加函数test_disconnect。到这里为止，想必读者不熟悉disconnect函数，那么该如何查找资料呢？一是百度；二是参考Qt的文档。在函数test_disconnect输入“QObject::disconnect”，然后把光标移动到disconnect上，按F1，就会自动跳转到Qt的帮助文档。如下

![](https://img-blog.csdn.net/20180814170718751?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 帮助文档里，首先是函数的定义，然后是说明文档。若读者的英语足够好，可直接看英文文档；如不够好，多看看就知道了。**test_disconnect函数就留给读者去完成**。一定要完成啊，不然怎么进步。

# 五、预定义信号的使用

Qt中的控件很多已经预定义好了信号，我们只需要自定义槽，并在构造函数中关联信号与槽。比如，在本项目中，需要在UI中添加一个按钮，然后使用qDebug()输出按钮被点击。后面讲控件的时候会提到这些预定义信号的使用。

![](https://img-blog.csdn.net/20180814182238556?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 六、信号槽机制的高级用法

说是高级，其实没有啥很高级的。
- 获取信号的发送者：[QObject::sender](http://doc.qt.io/qt-5/qobject.html#sender)()
- Qt5中 connect函数新的参数类型
- 使用Lambda表达式来作为槽
- 使用第三方库时，如何使用信号和槽 

### 获取信号的发送者：[QObject::sender](http://doc.qt.io/qt-5/qobject.html#sender)()

如果在由信号关联的槽中调用，则返回指向发送信号的对象的指针; 否则它返回0，也就是空指针。该指针仅在调用[QObject::sender](http://doc.qt.io/qt-5/qobject.html#sender)()的槽中有效。

如果sender被销毁，或者槽与sender的信号断开连接，则此函数返回的指针将变为无效。

警告：此功能违反了面向对象的模块化原则。 但是，当许多信号连接到单个槽时，访问sender可能很有用。

警告：如上所述，当通过Qt::DirectConnection从与该对象的线程不同的线程调用槽时，此函数的返回值无效。 请勿在此类场景中使用此功能。

### Qt5中 connect函数新的参数类型

在Qt4中，connect函数的示例如下

```cpp
QObject::connect(&counter,SIGNAL(valueChanged(int)),&a,SLOT(receiveNewValue(int)));
```

 在Qt5中，可以这样写

```cpp
QObject::connect(&counter,Counter::valueChanged,&a,&A::receiveNewValue);
```

不过上面这一行编译通不过，因为receiveNewValue函数有重载，connect函数不知道应该选择哪一个。上面这种能编译过的条件是：信号和槽都必须没有重载，也就是在类中没有同名函数。比如，下面这行就能编译过

```cpp
QObject::connect(&counter,Counter::valueChanged,&b,&B::receiveNewValue);
```

### 使用Lambda表达式来作为槽

观察如下代码

```cpp
void test_lambda(){
    Counter c;
    QObject::connect(&c,//发送者
                     &Counter::valueChanged, //信号，这里必须使用函数指针的形式
                     &c,//接收者，非空指针
                     [=](int value)->void { qDebug() << value <<endl;}//槽，一个lambda表达式
    );
    c.setValue(2333);
}
```

执行的结果是输出“2333”。lambda表达式看起来很奇怪，不过并不难懂，也很好用，大家可自行查找lambda的博客。

### 使用第三方库时，如何使用信号和槽 

可以将Qt与第三方信号/槽机制一起使用。 甚至可以在同一个项目中使用这两种机制。 只需将以下行添加到qmake项目（.pro）文件即可。
`CONFIG += no_keywords`
它告诉Qt不要定义moc关键字signals，slots和emit，因为这些名称将被第三方库使用，例如 Boost。 然后只需将源码中Qt moc关键字的所有使用替换为相应的Qt宏Q_SIGNALS（或Q_SIGNAL），Q_SLOTS（或Q_SLOT）和Q_EMIT。

参考链接：

[http://doc.qt.io/qt-5/signalsandslots.html](http://doc.qt.io/qt-5/signalsandslots.html)

[https://blog.csdn.net/qq_40194498/article/details/79647356](https://blog.csdn.net/qq_40194498/article/details/79647356)



