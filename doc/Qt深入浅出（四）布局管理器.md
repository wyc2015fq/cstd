# Qt深入浅出（四）布局管理器

2018年02月24日 00:18:43 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：484



​     布局管理器可以让程序员摆脱繁琐的窗口位置大小设置工作。并且布局管理器可以依据窗口大小变更布局控件的位置大小。

​	布局管理器基类为QLayout继承至QObject 和QLayoutItem，它是一个抽象基类，需要被进一步继承才能使用。它的派生类有QHBoxLayout、QVBoxLayout和QGridLayout。

​	每个窗口对象都可以设置一个布局管理器，来管理所有它的子窗口，由于布局管理器继承至QObject，那么布局管理的内存释放也可以托管给窗口对象，而所有添加到布局管理器中的窗口对象，它们的内存释放又都托管给这个布局管理器。

​	通过窗口对象的setLayout来设置布局管理器，如果事先已经设置了一个布局管理器，那么会替换掉这个布局管理器。

```cpp
void QWidget::setLayout(QLayout *layout)
```





​	你可以简单的理解布局管理器为窗口的一部分。



## 1 QHBoxLayout与QVBoxLayout

​	QHBoxLayout、QVBoxLayout它们都继承至QBoxLayout，QBoxLayout又继承至QLayout。

​	QHBoxLayout是水平布局管理器，可以将所有托管给它的窗口对象按照水平方向进行布局。

​	QVBoxLayout是垂直布局管理器，可以将所有托管给它的窗口对象按照垂直方向进行布局。





- 将窗口添加到布局管理器中

```cpp
void QBoxLayout::addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment())



/*



widget      窗口对象



stretch     伸缩系数



alignment   对齐方式Qt::Aligment是Qt中的一个枚举类型，具体请查看帮助文档



*/  
```

- 设置伸缩系数

```cpp
void QBoxLayout::setStretch(int index,int stretch)
```

- 添加类似弹簧的东西

```cpp
void QBoxLayout::addStretch(int stretch= 0)
```



- 设置布局管理中窗口的间隔

```cpp
void QBoxLayout::setSpacing(int spacing)  
```



- 布局管理器中添加间隙

```cpp
void QBoxLayout::addSpacing(int size);
```



- 设置布局管理器边界

```cpp
void QLayout::setMargin(int margin)
```

- 设置对其方式

```cpp
bool QLayout::setAlignment(QWidget *w, Qt::Alignment alignment)  
```



## 2 QGridLayout表格布局管理器

- 行伸缩系数

```cpp
void QGridLayout::setRowStretch(int row,int stretch)
```



- 列的伸缩系数

```cpp
void QGridLayout::setColumnStretch(int column,int stretch)
```



- 设置间隔   

```cpp
void QGridLayout::setSpacing(int spacing)   
```

 

- 设置行的最小高

```cpp
void QGridLayout::setRowMinimumHeight(int row, int minSize)
```

- 设置列的最小宽

```cpp
void QGridLayout::setColumnMinimumWidth(int column, int minSize)
```



- 设置对齐方式

```cpp
bool QLayout::setAlignment(QWidget *w, Qt::Alignment alignment)
```







## 3 嵌套布局管理器

​	有些时候，我们需要实现复杂的布局效果，单单使用一种布局管理器，无法完成，那么这个时候需要嵌套使用布局管理器。即布局管理器中添加布局管理器。



- QHBoxLayout与QVBoxLayout使用以下函数来嵌套其它布局管理器：

```cpp
void QBoxLayout::addLayout(QLayout *layout, int stretch = 0)
```

- QGridLayout使用以下函数来嵌套其它布局管理器：

```cpp
void QGridLayout::addLayout(QLayout *layout, int row, int column, Qt::Alignment alignment = Qt::Alignment())
```





## 4 获取布局之后窗口大小



​	使用布局管理器之后，如果想在构造函数中获取窗口大小，这个时候就不能使用size函数来获取，需要通过sizeHint函数来获取。

​	这是因为，size获取的是窗口实际大小，一般通过resize来设置，不设置的时候也有默认大小。

​	然而如果设置了布局管理器，那么窗口在显示的时候还会按照特定的布局策略重置窗口大小，而sizeHint获取到的是重置后的窗口大小。函数原型如下：

```cpp
virtual QSize sizeHint() const
```



注意：如果没有使用布局管理器，那么sizeHint返回的无效的值