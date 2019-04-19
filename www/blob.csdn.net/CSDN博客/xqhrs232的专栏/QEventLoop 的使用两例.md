# QEventLoop 的使用两例 - xqhrs232的专栏 - CSDN博客
2018年07月11日 18:30:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：77
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/dbzhang800/article/details/6300519](https://blog.csdn.net/dbzhang800/article/details/6300519)
## 熟悉的陌生人
Qt 是事件驱动的，所以当你用Qt的时候，几乎时时刻刻和 QEventLoop 打交道、，只是你可能没有意识到：
- QCoreApplicaton::exec()
- QApplication::exec()
- QDialog::exec()
- QThread::exec()
- QDrag::exec()
- QMenu::exec()
- ...
在前面列出的这些常见函数的背后，都有各自的QEventLoop，可能是我们很少有机会想到自己显式使用QEventLoop的缘故吧，对这个类似乎总是有些陌生。
在[如何让 Qt 程序的 Sleep](http://blog.csdn.net/dbzhang800/archive/2011/04/03/6300425.aspx) 和[QDialog 模态对话框与事件循环](http://blog.csdn.net/dbzhang800/archive/2011/04/03/6300416.aspx) 两个短文中，我们可以看到 QEventLoop 的使用。那么？如何自己使用 QEventLoop 的，又有什么用呢？
## QEventLoop
Manual 中说的很简洁
At any time, you can create a QEventLoop object and call exec() on it to start a local event loop. From within the event loop, calling exit() will force exec() to return.
在任何时候，你都可以创建一个QEventLoop的对象，然后调用它的exec() 来开始一个局部的事件循环。
看Manual容易让人头大，那么，看例子吧：
### 让主线程等待100ms？
直接sleep一下行么，显然，如果你的用户不介意你的程序界面不响应用户操作，没问题！可是如果介意呢？
此时，开启一个局部的事件循环，让其执行100ms后自己退出，似乎很不错。写来看看：
QEventLoop eventloop;
QTimer::singleShot(100, &eventloop, SLOT(quit()));
eventloop.exec();- 创建事件循环
- 启动定时器，让其100ms后触发事件循环的quit()槽
- 启动事件循环
注：让主线程等待有其他方法，此处略过。
### 窗口一闪而过？
不少人遇到过这个问题：在一个槽函数内创建了一个窗口对象，却没有看到窗口弹出来，或者看到窗口一闪而过。比如：
void XXXX::slot1()
{
    QDialog dlg;
    dlg.show()
}
当然，大家都知道原因：因为到了后面的大括号处，dlg因为出作用域，会被析构掉。解决方法很简单，增大w的生存时间即可。比如：
- 将 dlg 作为类的成员，而不是函数的局部变量
- 将 dlg 前面添加 static，作为静态成员
- 将 dlg 用 new 分配到 heap 中
- ...
能否用 QEventLoop 来解决呢？答案是，可以
void XXXX::slot1()
{
    QDialog dlg;
    dlg.show()
    QEventLoop loop;
    connect(&dlg, SIGNAL(finished(int)), &loop, SLOT(quit()));
    loop.exec(QEventLoop::ExcludeUserInputEvents);
}
恩至此，问题解决。其实，这也是 QDialog::exec() 内部所做的事情，只不过此处不是模态对话框而已。
