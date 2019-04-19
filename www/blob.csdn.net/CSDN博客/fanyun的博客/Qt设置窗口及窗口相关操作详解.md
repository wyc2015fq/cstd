# Qt设置窗口及窗口相关操作详解 - fanyun的博客 - CSDN博客
2016年12月30日 17:20:50[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1263

     我们在Qt中创建窗口对象，在使用Qt的Ui设计器时，只需要拖动实现窗口的大小，有些时候我们不希望使用Qt的Ui设计器，那么创建的窗口默认显示的效果不符合我们的实际期待的效果，这个时候就需要通过其他方式来实现窗口显示效果的设置了，实际上Qt给我们提供了很多设置控制窗口的方法，如下面这些控制窗口大小常用的函数：
void setMinimumSize ( const QSize & )
virtual void setMinimumSize ( int minw, int minh )
void setMaximumSize ( const QSize & )
virtual void setMaximumSize ( int maxw, int maxh )
void setMinimumWidth ( int minw )
void setMinimumHeight ( int minh )
void setMaximumWidth ( int maxw )
void setMaximumHeight ( int maxh )
QSize size() const
void resize(int w, int h)
void resize(const QSize &)
未设置之前的效果：
![](https://img-blog.csdn.net/20170404093513532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(1)第一种设置窗口的方式：
```cpp
MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
        QToolBar *bar = this->addToolBar("Tools");
        QActionGroup *group = new QActionGroup(bar)
                …
        //(1)Set the window default size
        this->resize( QSize( 800, 600 ));
}
```
(2)第二种设置窗口的方式：
使用重载QWidget的一个虚函数方式来实现：
virtualQSize sizeHint () const
具体实现如下：
```cpp
//(2)Set the window default size
QSize MainWindow::sizeHint() const
 {
     return QSize( 800, 600 );
 }
```
设置后的效果：
![](https://img-blog.csdn.net/20170404093714811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       备注：在我们的实际应用中，如果遇到主窗口中有多个子窗口的情形，那么有时候设置会达不到预期的效果，这个时候可以先设置子窗口的布局和大小，在设置主窗口的大小。
