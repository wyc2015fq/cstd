# Qt深入浅出（五）信号与槽机制

2018年02月24日 00:20:23 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：1629



​        信号和槽是一种高级接口，应用于对象之间的通信，它是QT的核心特性，也是QT区别于其它工具包的重要地方。

​	信号和槽是QT自行定义的一种通信机制，它独立于标准的 C++ 语言，是标准C++的扩展。因此要正确的处理信号和槽，必须借助一个称为 moc（Meta Object Compiler）的QT工具，一般叫做元对象编辑器，该工具是一个C++ 预处理程序，它为高层次的事件处理自动生成所需要的附加代码。





## 1 初步认识信号

​	当某个信号对其客户或所有者发生的内部状态发生改变，信号被一个对象发射。例如QPushButton对象被鼠标点击后，发射出信号clicked()。

​	信号的声明是在头文件中进行的，QT 的 signals 关键字指出进入了信号声明区，随后即可声明信号，信号没有调用权限，不需要在申明的时候指定(public、protected、private)等权限。



- **信号只申明不实现**

  从形式上讲，信号的声明与普通的C++函数是一样的，但是信号却没有函数体定义，另外，信号的返回类型都是void，不要指望能从信号返回什么有用信息。

  

- **信号可以通过关键字emit来发射**

  

  ```cpp
  emit clicked();
  ```

  

  ## 2 初步认识槽函数

  ​	槽函数可以像普通的 C++ 成员函数，可以被正常调用，与普通函数的不同之处是信号可以与槽函数相关联。当与其关联的信号被发射时，这个槽就会被调用。

  ​	槽函数与其它普通函数一样，它们也有存取权限。槽函数也分为三种类型，即 public slots、private slots 和 protected slots。

  ​	public slots：在这个区内声明的槽意味着任何对象都可将信号与之相连接。

  ​	protected slots：在这个区内声明的槽意味着当前类及其子类可以将信号与之相连接。

  ​	private slots：在这个区内声明的槽意味着只有类自己可以将信号与之相连接。



## 3 信号与槽的链接

​	通过调用 QObject 对象的 connect 函数来将某个对象的信号与另外一个对象的槽函数相关联，这样当发射者发射信号时，接收者的槽函数将被调用。

该函数的定义如下：

```cpp
static QMetaObject::Connection QObject::connect(const QObject *sender, const QMetaMethod &signal, const QObject *receiver, const QMetaMethod &method, Qt::ConnectionType type = Qt::AutoConnection) 
```

​	这个函数的作用就是将发射者 sender 对象中的信号 signal 与接收者 receiver 中的 member 槽函数联系起来。

- 当指定信号 signal 时必须使用 Qt 的宏 SIGNAL()包装，当指定槽函数时必须使用宏 SLOT()包装，例如：

```cpp
QWidget widget;   
QPushButton pb;
QObject::connect(&pb, SIGNAL(clicked()), &widget, SLOT(close()));
```



- 一个信号可以跟多个槽函数链接，那么，当这个信号被发射时，这些槽将会一个接一个地执行，但是它们执行的顺序将会是随机的、不确定的，我们不能人为地指定哪个先执行、哪个后执行。
- 多个信号也可以和一个槽函数链接，那么只要其中一个信号被发射，都能调用该槽函数。

- 在非多线程对象之间，使用是以直接链接方式，即只有当所有的槽返回以后发射函数才返回。多线程之间对象的信号槽链接，是以队列的方式链接，即发射函数后不需要等待槽返回。

  

## 4 信号槽断开链接

​	当信号与槽没有必要继续保持关联时，我们可以使用 disconnect 函数来断开连接。其定义如下：

```cpp
[static] bool QObject::disconnect(const QObject *sender, const char *signal, const QObject *receiver, const char *method)
```



​	例如：

```cpp
QWidget widget;   
QPushButton pb;
QObject::disconnect(&pb, SIGNAL(clicked()), &widget, SLOT(close()));
```







## 5 自定义信号与槽

​	原生c++对象之间的通信很麻烦，自定义信号与槽进行通信是Qt中最常用的开发技术，当是自定义信号与槽函数的时候，需要注意这些细节。

1. 只有继承至QObject的派生类才能使用信号槽。

1. 必需在头文件添加宏Q_OBJECT。 

1. 关键字signals:指定声明信号区域。
2. 关键字[public protect private] slot:指定申明槽函数区域。

例如：

```cpp
class A :public QObject
{
    Q_OBJECT
public:
    A();
signals:
    void mysignal(int);
public slots:
    void myslot();
​
};
```



需要注意的是：有些开发者喜欢在cpp中定义一个class，但是Q_OBJECT 这个宏只能在头文件中才能被展开。