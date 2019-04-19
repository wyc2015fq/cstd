# Qt Event - 三少GG - CSDN博客
2012年12月18日 18:13:35[三少GG](https://me.csdn.net/scut1135)阅读数：1085
个人分类：[流媒体基础](https://blog.csdn.net/scut1135/article/category/934830)
### Qt Event   http://blog.163.com/huangwei820410@126/blog/static/240704922009610104424725/
1. Qt事件
Qt程序是事件驱动的, 程序的每个动作都是由幕后某个事件所触发. Qt事件的类型很多, 常见的qt的事件如下:
键盘事件: 按键按下和松开.
鼠标事件: 鼠标移动,鼠标按键的按下和松开.
拖放事件: 用鼠标进行拖放.
滚轮事件: 鼠标滚轮滚动.
绘屏事件: 重绘屏幕的某些部分.
定时事件: 定时器到时.
焦点事件: 键盘焦点移动.
进入和离开事件: 鼠标移入widget之内,或是移出.
移动事件: widget的位置改变.
大小改变事件: widget的大小改变.
显示和隐藏事件: widget显示和隐藏.
窗口事件: 窗口是否为当前窗口.
还有一些非常见的qt事件,比如socket事件,剪贴板事件,字体改变,布局改变等等.
Qt的事件和Qt中的signal不一样. 后者通常用来"使用"widget, 而前者用来"实现" widget. 比如一个按钮, 我们使用这个按钮的时候, 我们只关心他clicked()的signal, 至于这个按钮如何接收处理鼠标事件,再发射这个信号,我们是不用关心的. 但是如果我们要重载一个按钮的时候,我们就要面对event了. 比如我们可以改变它的行为,在鼠标按键按下的时候(mouse press event) 就触发clicked()的signal而不是通常在释放的( mouse release event)时候.
2. 事件产生和处理流程
2.1 事件的产生
事件的两种来源:
一种是系统产生的;通常是window system把从系统得到的消息,比如鼠标按键,键盘按键等, 放入系统的消息队列中. Qt事件循环的时候读取这些事件,转化为QEvent,再依次处理.
一种是由Qt应用程序程序自身产生的.程序产生事件有两种方式, 一种是调用QApplication::postEvent(). 例如QWidget::update()函数,当需要重新绘制屏幕时,程序调用update()函数,new出来一个paintEvent,调用QApplication::postEvent(),将其放入Qt的消息队列中,等待依次被处理. 另一种方式是调用sendEvent()函数. 这时候事件不会放入队列, 而是直接被派发和处理, QWidget::repaint()函数用的就是这种方式.
// 自定义事件的时候讲述: 需要注意的时, 这两个函数的使用方法不大一样, 一个是new, 一个是....
2.2 事件的调度
两种调度方式,一种是同步的, 一种是异步.
Qt的事件循环是异步的,当调用QApplication::exec()时,就进入了事件循环. 该循环可以简化的描述为如下的代码:
while ( !app_exit_loop )
{
   while( !postedEvents ) { processPostedEvents() }
   while( !qwsEvnts ){ qwsProcessEvents();   }
   while( !postedEvents ) { processPostedEvents() }
}
先处理Qt事件队列中的事件, 直至为空. 再处理系统消息队列中的消息, 直至为空, 在处理系统消息的时候会产生新的Qt事件, 需要对其再次进行处理.
调用QApplication::sendEvent的时候, 消息会立即被处理,是同步的. 实际上QApplication::sendEvent()是通过调用QApplication::notify(), 直接进入了事件的派发和处理环节.
2.3 事件的派发和处理
首先说明Qt中事件过滤器的概念. 事件过滤器是Qt中一个独特的事件处理机制, 功能强大而且使用起来灵活方便. 通过它, 可以让一个对象侦听拦截另外一个对象的事件. 事件过滤器是这样实现的: 在所有Qt对象的基类: QObject中有一个类型为QObjectList的成员变量,名字为eventFilters,当某个QObjec (qobjA)给另一个QObject (qobjB)安装了事件过滤器之后, qobjB会把qobjA的指针保存在eventFilters中. 在qobjB处理事件之前,会先去检查eventFilters列表,
 如果非空, 就先调用列表中对象的eventFilter()函数. 一个对象可以给多个对象安装过滤器. 同样, 一个对象能同时被安装多个过滤器, 在事件到达之后, 这些过滤器以安装次序的反序被调用. 事件过滤器函数( eventFilter() ) 返回值是bool型, 如果返回true, 则表示该事件已经被处理完毕, Qt将直接返回, 进行下一事件的处理; 如果返回false, 事件将接着被送往剩下的事件过滤器或是目标对象进行处理.
Qt中,事件的派发是从QApplication::notify() 开始的, 因为QAppliction也是继承自QObject, 所以先检查QAppliation对象, 如果有事件过滤器安装在qApp上, 先调用这些事件过滤器. 接下来QApplication::notify() 会过滤或合并一些事件(比如失效widget的鼠标事件会被过滤掉, 而同一区域重复的绘图事件会被合并). 之后,事件被送到reciver::event() 处理.
同样, 在reciver::event()中, 先检查有无事件过滤器安装在reciever上. 若有, 则调用之. 接下来,根据QEvent的类型, 调用相应的特定事件处理函数. 一些常见的事件都有特定事件处理函数, 比如:mousePressEvent(), focusOutEvent(),  resizeEvent(), paintEvent(), resizeEvent()等等. 在实际应用中, 经常需要重载这些特定事件处理函数在处理事件. 但对于那些不常见的事件, 是没有相对应的特定事件处理函数的.
 如果要处理这些事件, 就需要使用别的办法, 比如重载event() 函数, 或是安装事件过滤器.
事件派发和处理的流程图如下:
 2.4 事件的转发
对于某些类别的事件, 如果在整个事件的派发过程结束后还没有被处理, 那么这个事件将会向上转发给它的父widget, 直到最顶层窗口. 如图所示, 事件最先发送给QCheckBox, 如果QCheckBox没有处理, 那么由QGroupBox接着处理, 如果QGroupBox没有处理, 再送到QDialog, 因为QDialog已经是最顶层widget, 所以如果QDialog不处理, QEvent将停止转发.       如何判断一个事件是否被处理了呢? Qt中和事件相关的函数通过两种方式相互通信. QApplication::notify(),
 QObject::eventFilter(), QObject::event() 通过返回bool值来表示是否已处理. “真”表示已经处理, “假”表示事件需要继续传递. 另一种是调用QEvent::ignore() 或 QEvent::accept() 对事件进行标识. 这种方式只用于event() 函数和特定事件处理函数之间的沟通. 而且只有用在某些类别事件上是有意义的, 这些事件就是上面提到的那些会被转发的事件, 包括: 鼠标, 滚轮, 按键等事件.
3. 实际运用
根据对Qt事件机制的分析, 我们可以得到5种级别的事件过滤,处理办法. 以功能从弱到强, 排列如下:
3.1 重载特定事件处理函数.
最常见的事件处理办法就是重载象mousePressEvent(), keyPressEvent(), paintEvent() 这样的特定事件处理函数. 以按键事件为例, 一个典型的处理函数如下:
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
3.2重载event()函数.
通过重载event()函数,我们可以在事件被特定的事件处理函数处理之前(象keyPressEvent())处理它. 比如, 当我们想改变tab键的默认动作时,一般要重载这个函数. 在处理一些不常见的事件(比如:LayoutDirectionChange)时,evnet()也很有用,因为这些函数没有相应的特定事件处理函数. 当我们重载event()函数时, 需要调用父类的event()函数来处理我们不需要处理或是不清楚如何处理的事件.
下面这个例子演示了如何重载event()函数, 改变Tab键的默认动作: (默认的是键盘焦点移动到下一个控件上. )
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
3.3 在Qt对象上安装事件过滤器.
安装事件过滤器有两个步骤: (假设要用A来监视过滤B的事件)
首先调用B的installEventFilter( const QOject *obj ), 以A的指针作为参数. 这样所有发往B的事件都将先由A的eventFilter()处理.
然后, A要重载QObject::eventFilter()函数, 在eventFilter() 中书写对事件进行处理的代码.
用这种方法改写上面的例子: (假设我们将CodeEditor 放在MainWidget中)
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
3.4 给QAppliction对象安装事件过滤器.
一旦我们给qApp(每个程序中唯一的QApplication对象)装上过滤器,那么所有的事件在发往任何其他的过滤器时,都要先经过当前这个eventFilter(). 在debug的时候,这个办法就非常有用, 也常常被用来处理失效了的widget的鼠标事件,通常这些事件会被QApplication::notify()丢掉. ( 在QApplication::notify() 中, 是先调用qApp的过滤器, 再对事件进行分析, 以决定是否合并或丢弃)
4.5 继承QApplication类,并重载notify()函数.
Qt是用QApplication::notify()函数来分发事件的.想要在任何事件过滤器查看任何事件之前先得到这些事件,重载这个函数是唯一的办法. 通常来说事件过滤器更好用一些, 因为不需要去继承QApplication类. 而且可以给QApplication对象安装任意个数的事
**关于QT事件(event)的简单介绍**
　　　事件(event)是有系统或者Qt本身在不同的时刻发出的。当用户按下[鼠标](http://diy.21tx.com/mouse/)，敲下[键盘](http://diy.21tx.com/mouse/)，或者是窗口需要重新绘制的时候，都会发出一个相应的事件。一些事件是在对用户操作做出响应的时候发出，如键盘事件等；另一些事件则是由系统自动发出，如计时器事件。
　　一般来说，使用Qt编程时，我们并不会把主要精力放在事件上，因为在Qt中，需要我们关心的事件总会发出一个信号。比如，我们关心的是QPushButton的鼠标点击，但我们不需要关心这个鼠标点击事件，而是关心它的clicked()信号。这与其他的一些框架不同：在[Swing](http://dev.21tx.com/java/applet/)中，你所要关心的是JButton的ActionListener这个点击事件。
　　Qt的事件很容易和信号槽混淆。这里简单的说明一下，signal由具体对象发出，然后会马上交给由connect函数连接的slot进行处理；而对于事件，Qt使用一个事件队列对所有发出的事件进行维护，当新的事件产生时，会被追加到事件队列的尾部，前一个事件完成后，取出后面的事件进行处理。但是，必要的时候，Qt的事件也是可以不进入事件队列，而是直接处理的。并且，事件还可以使用“事件过滤器”进行过滤。总的来说，如果我们使用组件，我们关心的是信号槽；如果我们自定义组件，我们关心的是事件。因为我们可以通过事件来改变组件的默认操作。比如，如果我们要自定义一个QPushButton，那么我们就需要重写它的鼠标点击事件和键盘处理事件，并且在恰当的时候发出clicked()信号。
　　还记得我们在main函数里面创建了一个QApplication对象，然后调用了它的exec()函数吗？其实，这个函数就是开始Qt的事件循环。在执行exec()函数之后，程序将进入事件循环来监听应用程序的事件。当事件发生时，Qt将创建一个事件对象。Qt的所有事件都继承于
 QEvent类。在事件对象创建完毕后，Qt将这个事件对象传递给QObject的event()函数。event()函数并不直接处理事件，而是按照事件对象的类型分派给特定的事件处理函数(eventhandler)。
　　在所有组件的父类QWidget中，定义了很多事件处理函数，如keyPressEvent()、keyReleaseEvent()、mouseDoubleClickEvent()、mouseMoveEvent ()、mousePressEvent()、mouseReleaseEvent()等。这些函数都是protected virtual的，也就是说，我们应该在子类中重定义这些函数。下面来看一个例子。
`#include`
`#include`
`#include`
`#include`
`class``　EventLabel　:　public　QLabel`
`{`
`protected:`
`　　　　　　　　void　mouseMoveEvent(QMouseEvent　*event);`
`　　　　　　　　void　mousePressEvent(QMouseEvent　*event);`
`　　　　　　　　void　mouseReleaseEvent(QMouseEvent　*event);`
`};`
`void``　EventLabel::mouseMoveEvent(QMouseEvent　*event)`
`{`
`　　　　　　　　this->setText(QString("`
# `Move:``　(%1,　%2)`
`")`
`　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　.arg(QString::number(event->x()),　QString::number(event->y())));`
`}`
`void``　EventLabel::mousePressEvent(QMouseEvent　*event)`
`{`
`　　　　　　　　this->setText(QString("`
# Press:　(%1,　%2)
`")`
`　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　.arg(QString::number(event->x()),　QString::number(event->y())));`
`}`
`void``　EventLabel::mouseReleaseEvent(QMouseEvent　*event)`
`{`
`　　　　　　　　QString　msg;`
`　　　　　　　　msg.sprintf("```
# Release:　(%d,　%d)
`",`
`　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　event->x(),　event->y());`
`　　　　　　　　this->setText(msg);`
`}`
`int``　main(int　argc,　char　*argv[])`
`{`
`　　　　　　　　QApplication　app(argc,　argv);`
`　　　　　　　　EventLabel　*label　=　new　EventLabel;`
`　　　　　　　　label->setWindowTitle("MouseEvent　Demo");`
`　　　　　　　　label->resize(300,　200);`
`　　　　　　　　label->show();`
`　　　　　　　　return　app.exec();`
`}`
　　这里我们继承了QLabel类，重写了mousePressEvent、mouseMoveEvent和MouseReleaseEvent三个函数。我们并没有添加什么功能，只是在[鼠标](http://diy.21tx.com/mouse/)按下(press)、鼠标移动(move)和鼠标释放(release)时把坐标显示在这个Label上面。注意我们在mouseReleaseEvent函数里面有关QString的构造。我们没有使用arg参数的方式，而是使用C语言风格的sprintf来构造QString对象，如果你对C语法很熟悉(估计很多[C++](http://dev.21tx.com/language/c/)+程序员都会比较熟悉的吧)，那么就可以在Qt中试试熟悉的C格式化写法啦！
event filter是什么？
EventFilter即所谓事件过滤器，在Qt中是一个比较重要的概念，它的功能是把所有事件在到达watchee（被监控者）之前全部传递给另一个watcher（监控者），由watcher先行处理并决定是否继续传递该事件，如果继续传递， 则事件将回传给watchee来处理。 可能很多人已经知道怎么用event filter处理事件了，不过你也别嫌本文太简单， 毕竟总是不断的有新人提出类似的问题， 就当是复习一下吧。
**event filter的常见应用场合**
用来处理热键 －－ 比如一个界面上可以由用户热键来触发的多个按钮。 由于只有得到焦点的控件才能获得键盘的事件， 如果不用event filter就需要给每个button都加上键盘事件的处理， 还要在button里去访问兄弟button的指针，逻辑非常混乱。如果由主窗体做各个按钮的eventFilter， 则只需要在主窗体里去处理键盘事件就好，而且主窗体可以很容易的访问到各个button的指针，很方便。
用来代替派生和重写虚函数 －－ Qt里的键盘鼠标事件基本上都是以虚函数的方式来处理，要想重写虚函数则必须派生一个子类，这样的话如果只是一个简单的事件处理也去派生子类代价未免大了些，这时候就值得用用eventFilter。比如我的MDI界面想在每个子窗体关闭的时候做一些统一的操作，一般的做法是处理子窗体的closeEvent。但显然给每个子窗体都去派生个子类太不现实，最好的方法是把mainwindow作为子窗体的eventFilter去处理CloseEvent事件。
这里只举了两个例子， 相信聪明的同学们能在自己的程序中找到适合eventFilter发挥的位置。
**如何使用event filter处理事件？**
在前面的一篇blog中， 作者bug介绍了[Qt中事件传递的顺序](http://www.cuteqt.com/blog/?p=38)（如果没看过的先来扫扫盲），其中就提到了两个级别的event filter， 一个是为QApplication类安装一个eventFilter, 另一个是针对某个控件来安装eventFilter。 本质上这两个级别的event filter调用的都是QObject类提供的API：“installEventFilter”。
event filter的使用有点曲折， 有两个步骤要做。一是要调用watchee的installEventFilter以watcher指针为参数
> 
watchee->installEventFilter(watcher);
二是在watcher的类中实现bool eventFilter(QObject*, QEvent*) 这个虚函数，在此函数中处理事件。
class KeyPressEater : public QObject
{
Q_OBJECT
...
protected:
bool eventFilter(QObject *obj, QEvent *event);
};
bool KeyPressEater::eventFilter(QObject *obj, QEvent *event)
{
if (event->type() == QEvent::KeyPress) {
QKeyEvent *keyEvent = static_cast(event);
qDebug("Ate key press %d", keyEvent->key());
return true;
} else {
// standard event processing
return QObject::eventFilter(obj, event);
}
}
上面的例子代码是直接从Qt文档里抄的， 呵呵， 其实文档说的实在是很明白， 不知道为啥还是有很多人不懂这个怎么用。
其中几点需要注意的地方：
－ watchee和watcher必须都是从QObject派生的子类实例
－ watcher的类必须是一个自定义的子类（因为需要重新实现虚函数， 用现成的类是不行的。）
－ watcher类的eventFilter函数返回true时表示该事件处理完毕，不再继续传递；返回false表示该事件仍然传递
－ eventFilter的实现的最后必须调用watcher基类的eventFilter函数以传递事件。如果不调的话watcher的所有事件都将丢失。
－ 注意eventFilter的声明必须和文档里的一模一样， 写的时候注意返回值、大小写和参数类型
**其他**
在Qt中还有一些应用程序级别的eventFilter函数， 是和平台相关的，如x11EventFilter可以截获程序得到的所有X事件。相应的Windows平台下用winEventFilter… 大家不要被函数的名字给误导了， 这些函数并不是全局级别的，充其量只能拿到应用程序内部的事件，想做系统级别的事件过滤还是差着档次…
