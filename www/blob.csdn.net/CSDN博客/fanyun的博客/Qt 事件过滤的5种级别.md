# Qt 事件过滤的5种级别 - fanyun的博客 - CSDN博客
2018年03月25日 22:19:05[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：278
以下按照事件过滤等级由强到弱分为5级：
1级、继承QApplication类,并重载notify()函数.
        Qt 是用QApplication::notify()函数来分发事件的.想要在任何事件过滤器查看任何事件之前先得到这些事件,重载这个函数是唯一的办法. 通常来说事件过滤器更好用一些, 因为不需要去继承QApplication类. 而且可以给QApplication对象安装任意个数的事。
2级、给QAppliction对象安装事件过滤器.
   一旦我们给qApp(每个程序中唯一的QApplication对象)装上过滤器,那么所有的事件在发往任何其他的过滤器时,都要先经过当前这个 eventFilter(). 在debug的时候,这个办法就非常有用, 也常常被用来处理失效了的widget的鼠标事件,通常这些事件会被QApplication::notify()丢掉. ( 在QApplication::notify() 中, 是先调用qApp的过滤器, 再对事件进行分析, 以决定是否合并或丢弃)
3级、在Qt对象上安装事件过滤器.
   安装事件过滤器有两个步骤: (假设要用A来监视过滤B的事件)
   首先调用B的installEventFilter( const QOject *obj ), 以A的指针作为参数. 这样所有发往B的事件都将先由A的eventFilter()处理.
然后, A要重载QObject::eventFilter()函数, 在eventFilter() 中书写对事件进行处理的代码.
用这种方法改写上面的例子: (假设我们将CodeEditor 放在MainWidget中)
```cpp
MainWidget::MainWidget()
{
    CodeEditor * ce = new CodeEditor( this, “code editor”);
    ce->installEventFilter( this );
}
 
bool MainWidget::eventFilter( QOject * target , QEvent * event )
{
   if( target == ce )
   {
       if( event->type() == QEvent::KeyPress )
       {
             QKeyEvent *ke = (QKeyEvent *) event;
             if( ke->key() == Key_Tab )
             {
                ce->insertAtCurrentPosition('\t');
                return true;
             }
      }
   }
   return false;
}
```
4级、重载event()函数.
      通过重载event()函数,我们可以在事件被特定的事件处理函数处理之前(象keyPressEvent())处理它. 比如, 当我们想改变tab键的默认动作时,一般要重载这个函数. 在处理一些不常见的事件(比如:LayoutDirectionChange)时,evnet()也很有用,因为这些函数没有相应的特定事件处理函数. 当我们重载event()函数时, 需要调用父类的event()函数来处理我们不需要处理或是不清楚如何处理的事件.
下面这个例子演示了如何重载event()函数, 改变Tab键的默认动作: (默认的是键盘焦点移动到下一个控件上. )
```cpp
bool CodeEditor::event(QEvent * event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = (QKeyEvent *) event;
        if (keyEvent->key() == Key_Tab)
        {
            insertAtCurrentPosition('\t');
            return true;
        }
    }
    return QWidget::event(event);
}
```
5级、重载特定事件处理函数.
      最常见的事件处理办法就是重载象mousePressEvent(), keyPressEvent(), paintEvent() 这样的特定事件处理函数. 以按键事件为例, 一个典型的处理函数如下:
```cpp
void imageView::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
    case Key_Plus:
        zoomIn();
        break;
    case Key_Minus:
        zoomOut();
        break;
    case Key_Left:
        // …
    default:
        QWidget::keyPressEvent(event);
    }
}
```
**从assistant上获取的qt event表述：**
        1.qt的event通过QObject通知另一个qobject，event用一个QEvent类表示，它是所有event事件的基类，特殊event如鼠标event用QEvent的子类
如QMouseEvent类表示。
   2.QWidget重载了event()函数，并在这里把所有的event转发给相应的event处理函数，如mousePressEvent(),mouseReleaseEvent().
       3.qt还可以设置一个QObject去监视另一个QObject的event，这个功能通过eventfilter()实现的（installEventFilter()函数）。
     4.对于event的截取，我们可以1)重载特定的eventhandler，2)重载QObject的event(),3)安装eventfilter，4)在QApplication安装eventfilter
5.重载QApplication的notify(),此时event还没有转发给eventfilter，可以监视所有的event。
