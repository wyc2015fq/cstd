# Qt事件驱动机制原理及源码剖析 - fanyun的博客 - CSDN博客
2018年12月31日 17:52:28[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：441
        什么是Qt事件驱动？
        我们在写Qt工程类项目的时候都会发现，主程序里面都有这么一段代码：
```cpp
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
```
       实际上a.exec()便是Qt程序进入事件消息循环，具体我们进入代码内部看看：
```cpp
int QCoreApplication::exec()  
{  
    if (!QCoreApplicationPrivate::checkInstance("exec"))  
        return -1;  
    //获取线程数据  
    QThreadData *threadData = self->d_func()->threadData;  
    //判断是否在主线程创建  
    if (threadData != QThreadData::current()) {  
        qWarning("%s::exec: Must be called from the main thread", self->metaObject()->className());  
        return -1;  
    }  
    //判断eventLoop是否已经创建  
    if (!threadData->eventLoops.isEmpty()) {  
        qWarning("QCoreApplication::exec: The event loop is already running");  
        return -1;  
    }  
    threadData->quitNow = false;  
    QEventLoop eventLoop;  
    self->d_func()->in_exec = true;  
    //创建eventLoop  
    int returnCode = eventLoop.exec();  
    threadData->quitNow = false;  
    if (self) {  
        self->d_func()->in_exec = false;  
        //退出程序  
        emit self->aboutToQuit();  
        sendPostedEvents(0, QEvent::DeferredDelete);  
    }  
    return returnCode;  
}  
再来到qeventloop.cpp中。  
int QEventLoop::exec(ProcessEventsFlags flags)  
{  
    Q_D(QEventLoop);  
    if (d->threadData->quitNow)  
        return -1;  
    //已经调用过exec了。  
    if (d->inExec) {  
        qWarning("QEventLoop::exec: instance %p has already called exec()", this);  
        return -1;  
    }  
    d->inExec = true;  
    d->exit = false;  
    ++d->threadData->loopLevel;  
    //将事件类对象压入线程结构体中  
    d->threadData->eventLoops.push(this);  
    // remove posted quit events when entering a new event loop  
    // 这句不用翻译了把！  
    if (qApp->thread() == thread())  
        QCoreApplication::removePostedEvents(qApp, QEvent::Quit);  
#if defined(QT_NO_EXCEPTIONS)  
    while (!d->exit)  
        //这里才是关键,我们还要继续跟踪进去。  
        processEvents(flags | WaitForMoreEvents);  
#else  
    try {  
        while (!d->exit)  
            processEvents(flags | WaitForMoreEvents);  
    } catch (...) {  
        //如果使用了EXCEPTION，则继续对下一条时间进行处理。  
        qWarning("Qt has caught an exception thrown from an event handler. Throwing\n"  
                 "exceptions from an event handler is not supported in Qt. You must\n"  
                 "reimplement QApplication::notify() and catch all exceptions there.\n");  
        throw;  
    }  
#endif  
    //退出eventloop前，将时间对象从线程结构中取出。  
    QEventLoop *eventLoop = d->threadData->eventLoops.pop();  
    Q_ASSERT_X(eventLoop == this, "QEventLoop::exec()", "internal error");  
    Q_UNUSED(eventLoop); // --release warning  
 
    d->inExec = false;  
    --d->threadData->loopLevel;  
    //退出事件循环。  
    return d->returnCode;  
}  
 
来到了processEvents函数：  
bool QEventLoop::processEvents(ProcessEventsFlags flags)  
{  
    Q_D(QEventLoop);  
    //判断事件分派器是否为空。  
    if (!d->threadData->eventDispatcher)  
        return false;  
    if (flags & DeferredDeletion)  
        QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);  
    //调用不同平台下的事件分派器来处理事件。  
    return d->threadData->eventDispatcher->processEvents(flags);  
}  
processEvents是在QAbstractEventDispatcher类中定义的纯虚方法。在QEventDispatcherWin32类有processEvents的实现。  
bool QEventDispatcherWin32::processEvents(QEventLoop::ProcessEventsFlags flags)  
{  
    Q_D(QEventDispatcherWin32);  
    //内部数据创建。registerClass注册窗口类，createwindow创建窗体。  
    //注册socket notifiers，启动所有的normal timers  
    if (!d->internalHwnd)  
        createInternalHwnd();  
    d->interrupt = false;  
    emit awake();  
 
    bool canWait;  
    bool retVal = false;  
    do {  
        QCoreApplicationPrivate::sendPostedEvents(0, 0, d->threadData);  
        DWORD waitRet = 0;  
        HANDLE pHandles[MAXIMUM_WAIT_OBJECTS - 1];  
        QVarLengthArray<MSG> processedTimers;  
        while (!d->interrupt) {  
            DWORD nCount = d->winEventNotifierList.count();  
            Q_ASSERT(nCount < MAXIMUM_WAIT_OBJECTS - 1);  
            MSG msg;  
            bool haveMessage;  
            if (!(flags & QEventLoop::ExcludeUserInputEvents) && !d->queuedUserInputEvents.isEmpty()) {  
                // process queued user input events处理用户输入事件，放入队列中。  
                haveMessage = true;  
                msg = d->queuedUserInputEvents.takeFirst();  
            } else if(!(flags & QEventLoop::ExcludeSocketNotifiers) && !d->queuedSocketEvents.isEmpty()) {  
                // process queued socket events  处理socket事件，放入队列中。  
                haveMessage = true;  
                msg = d->queuedSocketEvents.takeFirst();  
            } else {  
                //从消息队列中取消息，同PeekMessage  
                haveMessage = winPeekMessage(&msg, 0, 0, 0, PM_REMOVE);  
                if (haveMessage && (flags & QEventLoop::ExcludeUserInputEvents)  
                    && ((msg.message >= WM_KEYFIRST  
                         && msg.message <= WM_KEYLAST)  
                        || (msg.message >= WM_MOUSEFIRST  
                            && msg.message <= WM_MOUSELAST)  
                        || msg.message == WM_MOUSEWHEEL)) {  
                    // queue user input events for later processing  
                    haveMessage = false;  
                    d->queuedUserInputEvents.append(msg);  
                }  
                if (haveMessage && (flags & QEventLoop::ExcludeSocketNotifiers)  
                    && (msg.message == WM_USER && msg.hwnd == d->internalHwnd)) {  
                    // queue socket events for later processing  
                    haveMessage = false;  
                    d->queuedSocketEvents.append(msg);  
                }  
            }  
            if (!haveMessage) {  
                // no message - check for signalled objects  
                for (int i=0; i<(int)nCount; i++)  
                    pHandles[i] = d->winEventNotifierList.at(i)->handle();  
                //注册signal--slot。  
                waitRet = MsgWaitForMultipleObjectsEx(nCount, pHandles, 0, QS_ALLINPUT, MWMO_ALERTABLE);  
                if ((haveMessage = (waitRet == WAIT_OBJECT_0 + nCount))) {  
                    // a new message has arrived, process it  
                    continue;  
                }  
            }  
            //事件队列中有事件需要处理。  
            if (haveMessage) {   
                //处理timer事件  
                if (msg.message == WM_TIMER) {  
                    // avoid live-lock by keeping track of the timers we've already sent  
                    bool found = false;  
                    for (int i = 0; !found && i < processedTimers.count(); ++i) {  
                        const MSG processed = processedTimers.constData()[i];  
                        found = (processed.wParam == msg.wParam && processed.hwnd == msg.hwnd && processed.lParam == msg.lParam);  
                    }  
                    if (found)  
                        continue;  
                    processedTimers.append(msg);  
                } else if (msg.message == WM_QUIT) {  
                    if (QCoreApplication::instance())  
                        QCoreApplication::instance()->quit();  
                    return false;  
                }  
                //消息分发处理。  
                if (!filterEvent(&msg)) {  
                    TranslateMessage(&msg);  
                    QT_WA({  
                        DispatchMessage(&msg);  
                    } , {  
                        DispatchMessageA(&msg);  
                    });  
                }  
            } else if (waitRet >= WAIT_OBJECT_0 && waitRet < WAIT_OBJECT_0 + nCount) {  
                //处理signal--slot  
                d->activateEventNotifier(d->winEventNotifierList.at(waitRet - WAIT_OBJECT_0));  
            } else {  
                // nothing todo so break  
                break;  
            }  
            retVal = true;  
        }  
        // still nothing - wait for message or signalled objects  
        QThreadData *ddata = d->threadData;  
        canWait = (!retVal  
                   && data->canWait  
                   && !d->interrupt  
                   && (flags & QEventLoop::WaitForMoreEvents));  
        if (canWait) {  
            DWORD nCount = d->winEventNotifierList.count();  
            Q_ASSERT(nCount < MAXIMUM_WAIT_OBJECTS - 1);  
            for (int i=0; i<(int)nCount; i++)  
                pHandles[i] = d->winEventNotifierList.at(i)->handle();  
            emit aboutToBlock();  
            waitRet = MsgWaitForMultipleObjectsEx(nCount, pHandles, INFINITE, QS_ALLINPUT, MWMO_ALERTABLE);  
            emit awake();  
            if (waitRet >= WAIT_OBJECT_0 && waitRet < WAIT_OBJECT_0 + nCount) {  
                d->activateEventNotifier(d->winEventNotifierList.at(waitRet - WAIT_OBJECT_0));  
                retVal = true;  
            }  
        }  
    } while (canWait);  
    return retVal;  
}
```
QT源码中事件处理过程中调用函数如下：
```cpp
QApplication::exec()
QCoreApplication::exec()   
QEventLoop::exec(ProcessEventsFlags )    
QEventLoop::processEvents(ProcessEventsFlags )
QEventDispatcherWin32::processEvents(QEventLoop::ProcessEventsFlags)
QT_WIN_CALLBACK QtWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
bool QETWidget::translateMouseEvent(const MSG &msg)   
bool QApplicationPrivate::sendMouseEvent(...)   
inline bool QCoreApplication::sendSpontaneousEvent(QObject *receiver, QEvent *event)   
bool QCoreApplication::notifyInternal(QObject *receiver, QEvent *event)   
bool QApplication::notify(QObject *receiver, QEvent *e)   
bool QApplicationPrivate::notify_helper(QObject *receiver, QEvent * e)   
bool QWidget::event(QEvent *event)
```
1.进入QApplication事件循环
```cpp
int main(int argc, char *argv[])     
{     
QApplication app(argc, argv);     
Widget window;  // Widget 继承自QWidget     
window.show();     
return app.exec(); // 进入Qpplication事件循环     
}
```
2.进入QCoreApplication事件循环
```cpp
int QApplication::exec()
{
#ifndef QT_NO_ACCESSIBILITY
    QAccessible::setRootObject(qApp);
#endif    
//简单的交给QCoreApplication来处理事件循环
    return QCoreApplication::exec();
}
```
3.进入QeventLoop事件队列循环
```cpp
int QCoreApplication::exec()
{
    if (!QCoreApplicationPrivate::checkInstance("exec"))
        return -1;
    //得到当前Thread数据  
    QThreadData *threadData = self->d_func()->threadData;
    if (threadData != QThreadData::current()) {
        qWarning("%s::exec: Must be called from the main thread", self->metaObject()->className());
        return -1;
    }
       //检查event loop是否已经创建 
    if (!threadData->eventLoops.isEmpty()) {
        qWarning("QCoreApplication::exec: The event loop is already running");
        return -1;
    }
    threadData->quitNow = false;
    QEventLoop eventLoop;
    self->d_func()->in_exec = true;
    self->d_func()->aboutToQuitEmitted = false;
       //委任QEventLoop 处理事件队列循环
    int returnCode = eventLoop.exec();
    threadData->quitNow = false;
    if (self) {
        self->d_func()->in_exec = false;
        if (!self->d_func()->aboutToQuitEmitted)
            emit self->aboutToQuit();
        self->d_func()->aboutToQuitEmitted = true;
        sendPostedEvents(0, QEvent::DeferredDelete);
    }
    return returnCode;
}
```
4.进入QEventLoop::processEvents
```cpp
int QEventLoop::exec(ProcessEventsFlags flags)
{
    Q_D(QEventLoop);  //访问QEventloop私有类实例d
    //we need to protect from race condition with QThread::exit
    QMutexLocker locker(&static_cast<QThreadPrivate *>(QObjectPrivate::get(d->threadData->thread))->mutex);
    if (d->threadData->quitNow)
        return -1;
  
    if (d->inExec) {
        qWarning("QEventLoop::exec: instance %p has already called exec()", this);
        return -1;
    }
    d->inExec = true;
    d->exit = false;
    ++d->threadData->loopLevel;
    d->threadData->eventLoops.push(this);
    locker.unlock();
  
    // remove posted quit events when entering a new event loop
    QCoreApplication *app = QCoreApplication::instance();
    if (app && app->thread() == thread())
        QCoreApplication::removePostedEvents(app, QEvent::Quit);
    //这里的实现代码不少，最为重要的是以下几行 
#if defined(QT_NO_EXCEPTIONS)
    while (!d->exit)
        processEvents(flags | WaitForMoreEvents | EventLoopExec);#else
    try {
        while (!d->exit)  //只要没有遇见exit，循环派发事件 
            processEvents(flags | WaitForMoreEvents | EventLoopExec);
    } catch (...) {
        qWarning("Qt has caught an exception thrown from an event handler. Throwing\n"
                 "exceptions from an event handler is not supported in Qt. You must\n"
                 "reimplement QApplication::notify() and catch all exceptions there.\n");
  
        // copied from below        locker.relock();
        QEventLoop *eventLoop = d->threadData->eventLoops.pop();
        Q_ASSERT_X(eventLoop == this, "QEventLoop::exec()", "internal error");
        Q_UNUSED(eventLoop); // --release warning
        d->inExec = false;
        --d->threadData->loopLevel;
  
        throw;
    }#endif
  
    // copied above    locker.relock();
    QEventLoop *eventLoop = d->threadData->eventLoops.pop();
    Q_ASSERT_X(eventLoop == this, "QEventLoop::exec()", "internal error");
    Q_UNUSED(eventLoop); // --release warning
    d->inExec = false;
    --d->threadData->loopLevel;
  
    return d->returnCode;
}
```
5.事件处理QEventDispatcherWin32::processEvents
```cpp
bool QEventLoop::processEvents(ProcessEventsFlags flags)
{
    Q_D(QEventLoop);
    if (!d->threadData->eventDispatcher)
        return false;
    if (flags & DeferredDeletion)
        QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
//将事件派发给与平台相关的QAbstractEventDispatcher子类 
    return d->threadData->eventDispatcher->processEvents(flags);  
}
```
6.将获取的事件打包为消息，传递给操作系统
     AbstractEventDispatcher的子类QEventDispatcherWin32获得用户的输入事件，并将其打包成message后，通过标准的Windows API传递给Windows OS。
```cpp
bool QEventDispatcherWin32::processEvents(QEventLoop::ProcessEventsFlags flags)
{
    Q_D(QEventDispatcherWin32);
  
    if (!d->internalHwnd)
        createInternalHwnd();
  
    d->interrupt = false;
    emit awake();
  
    bool canWait;
    bool retVal = false;
    bool seenWM_QT_SENDPOSTEDEVENTS = false;
    bool needWM_QT_SENDPOSTEDEVENTS = false;
    do {
        DWORD waitRet = 0;
        HANDLE pHandles[MAXIMUM_WAIT_OBJECTS - 1];
        QVarLengthArray<MSG> processedTimers;
        while (!d->interrupt) {
            DWORD nCount = d->winEventNotifierList.count();
            Q_ASSERT(nCount < MAXIMUM_WAIT_OBJECTS - 1);
  
            MSG msg;
            bool haveMessage;
  
            if (!(flags & QEventLoop::ExcludeUserInputEvents) && !d->queuedUserInputEvents.isEmpty()) {
                // process queued user input events
                haveMessage = true;
                msg = d->queuedUserInputEvents.takeFirst(); //从处理用户输入队列中取出一条事件，处理队列里面的用户输入事件
            } else if(!(flags & QEventLoop::ExcludeSocketNotifiers) && !d->queuedSocketEvents.isEmpty()) {
                // process queued socket events
                haveMessage = true;
                msg = d->queuedSocketEvents.takeFirst();  // 从处理socket队列中取出一条事件,处理队列里面的socket事件
            } else {
                haveMessage = PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
                if (haveMessage && (flags & QEventLoop::ExcludeUserInputEvents)
                    && ((msg.message >= WM_KEYFIRST
                         && msg.message <= WM_KEYLAST)
                        || (msg.message >= WM_MOUSEFIRST
                            && msg.message <= WM_MOUSELAST)
                        || msg.message == WM_MOUSEWHEEL
                        || msg.message == WM_MOUSEHWHEEL
                        || msg.message == WM_TOUCH
#ifndef QT_NO_GESTURES
                        || msg.message == WM_GESTURE
                        || msg.message == WM_GESTURENOTIFY#endif
                        || msg.message == WM_CLOSE)) {
                    // queue user input events for later processing
                    haveMessage = false;
                    d->queuedUserInputEvents.append(msg);  // 用户输入事件入队列，待以后处理                 }
                if (haveMessage && (flags & QEventLoop::ExcludeSocketNotifiers)
                    && (msg.message == WM_QT_SOCKETNOTIFIER && msg.hwnd == d->internalHwnd)) {
                    // queue socket events for later processing
                    haveMessage = false;
                    d->queuedSocketEvents.append(msg);     // socket 事件入队列，待以后处理                   }
            }
            if (!haveMessage) {
                // no message - check for signalled objects
                for (int i=0; i<(int)nCount; i++)
                    pHandles[i] = d->winEventNotifierList.at(i)->handle();
                waitRet = MsgWaitForMultipleObjectsEx(nCount, pHandles, 0, QS_ALLINPUT, MWMO_ALERTABLE);
                if ((haveMessage = (waitRet == WAIT_OBJECT_0 + nCount))) {
                    // a new message has arrived, process it
                    continue;
                }
            }
            if (haveMessage) {
#ifdef Q_OS_WINCE
                // WinCE doesn't support hooks at all, so we have to call this by hand :(
                (void) qt_GetMessageHook(0, PM_REMOVE, (LPARAM) &msg);#endif
  
                if (d->internalHwnd == msg.hwnd && msg.message == WM_QT_SENDPOSTEDEVENTS) {
                    if (seenWM_QT_SENDPOSTEDEVENTS) {
                        // when calling processEvents() "manually", we only want to send posted
                        // events once
                        needWM_QT_SENDPOSTEDEVENTS = true;
                        continue;
                    }
                    seenWM_QT_SENDPOSTEDEVENTS = true;
                } else if (msg.message == WM_TIMER) {
                    // avoid live-lock by keeping track of the timers we've already sent
                    bool found = false;
                    for (int i = 0; !found && i < processedTimers.count(); ++i) {
                        const MSG processed = processedTimers.constData()[i];
                        found = (processed.wParam == msg.wParam && processed.hwnd == msg.hwnd && processed.lParam == msg.lParam);
                    }
                    if (found)
                        continue;
                    processedTimers.append(msg);
                } else if (msg.message == WM_QUIT) {
                    if (QCoreApplication::instance())
                        QCoreApplication::instance()->quit();
                    return false;
                }
  
                if (!filterEvent(&msg)) {
                    TranslateMessage(&msg);   //将事件打包成message调用Windows API派发出去
                    DispatchMessage(&msg);    //分发一个消息给窗口程序。消息被分发到回调函数，将消息传递给windows系统，windows处理完毕，会调用回调函数 => section 7                 }
            } else if (waitRet < WAIT_OBJECT_0 + nCount) {
                d->activateEventNotifier(d->winEventNotifierList.at(waitRet - WAIT_OBJECT_0));
            } else {
                // nothing todo so break
                break;
            }
            retVal = true;
        }
  
        // still nothing - wait for message or signalled objects
        canWait = (!retVal
                   && !d->interrupt
                   && (flags & QEventLoop::WaitForMoreEvents));
        if (canWait) {
            DWORD nCount = d->winEventNotifierList.count();
            Q_ASSERT(nCount < MAXIMUM_WAIT_OBJECTS - 1);
            for (int i=0; i<(int)nCount; i++)
                pHandles[i] = d->winEventNotifierList.at(i)->handle();
  
            emit aboutToBlock();
            waitRet = MsgWaitForMultipleObjectsEx(nCount, pHandles, INFINITE, QS_ALLINPUT, MWMO_ALERTABLE | MWMO_INPUTAVAILABLE);
            emit awake();
            if (waitRet < WAIT_OBJECT_0 + nCount) {
                d->activateEventNotifier(d->winEventNotifierList.at(waitRet - WAIT_OBJECT_0));
                retVal = true;
            }
        }
    } while (canWait);
  
    if (!seenWM_QT_SENDPOSTEDEVENTS && (flags & QEventLoop::EventLoopExec) == 0) {
        // when called "manually", always send posted events
        QCoreApplicationPrivate::sendPostedEvents(0, 0, d->threadData);
    }
  
    if (needWM_QT_SENDPOSTEDEVENTS)
        PostMessage(d->internalHwnd, WM_QT_SENDPOSTEDEVENTS, 0, 0);
  
    return retVal;
}
```
8.将操作系统打包的事件解包、翻译为QApplication可识别的事件
```cpp
bool QETWidget::translateMouseEvent(const MSG &msg)     
{
     ......
     res = QApplicationPrivate::sendMouseEvent(target, &e, alienWidget, this, &qt_button_down,  qt_last_mouse_receiver);
}
```
9.根据事件类型发送事件
```cpp
bool QApplicationPrivate::sendMouseEvent(QWidget *receiver, QMouseEvent *event,
  QWidget *alienWidget, QWidget *nativeWidget,QWidget **buttonDown,  QPointer<QWidget> &lastMouseReceiver,bool spontaneous)
{
    ...
    if (spontaneous)
        result = QApplication::sendSpontaneousEvent(receiver, event);
    else
        result = QApplication::sendEvent(receiver, event);
       
    ...
      
     return result;
}
```
10.发送事件
```cpp
inline bool QCoreApplication::sendSpontaneousEvent(QObject *receiver, QEvent *event)
{ 
      //将event标记为自发事件
     //进一步调用 2-5 QCoreApplication::notifyInternal     
      if (event) 
          event->spont = true; 
      return self ? self->notifyInternal(receiver, event) : false; 
}
```
11.线程内事件的处理
```cpp
bool QCoreApplication::notifyInternal(QObject *receiver, QEvent *event)
{
        ...
    // 以下代码主要意图为Qt强制事件只能够发送给当前线程里的对象，也就是说receiver->d_func()->threadData应该等于QThreadData::current()。
    //注意，跨线程的事件需要借助Event Loop来派发
    QObjectPrivate *d = receiver->d_func();
    QThreadData *threadData = d->threadData;
    ++threadData->loopLevel;
    QT_TRY {
        returnValue = notify(receiver, event);
    } QT_CATCH (...) {
        --threadData->loopLevel;
        QT_RETHROW;
    }
    ...
    return returnValue;
}
```
12.事件的派发
    任何线程的任何对象的所有事件在发送时都会调用notify()函数。
```cpp
bool QCoreApplication::notify(QObject *receiver, QEvent *event)
{
    Q_D(QCoreApplication);
    // no events are delivered after ~QCoreApplication() has started
    if (QCoreApplicationPrivate::is_app_closing)
        return true;
    if (receiver == 0) {                        // serious error
        qWarning("QCoreApplication::notify: Unexpected null receiver");
        return true;
    }
#ifndef QT_NO_DEBUG
    d->checkReceiverThread(receiver);#endif
    return receiver->isWidgetType() ? false : d->notify_helper(receiver, event);
}
```
13.使用事件过滤器对发送的事件进行过滤处理
```cpp
bool QCoreApplicationPrivate::notify_helper(QObject *receiver, QEvent * event)
{
    // send to all application event filters
    if (sendThroughApplicationEventFilters(receiver, event))
        return true;
     // 向事件过滤器发送该事件，这里介绍一下Event Filters. 事件过滤器是一个接受即将发送给目标对象所有事件的对象。 
    //如代码所示它开始处理事件在目标对象行动之前。过滤器的QObject::eventFilter（）实现被调用，能接受或者丢弃过滤
    //允许或者拒绝事件的更进一步的处理。如果所有的事件过滤器允许更进一步的事件处理，事件将被发送到目标对象本身。
    //如果他们中的一个停止处理，目标和任何后来的事件过滤器不能看到任何事件。
    if (sendThroughObjectEventFilters(receiver, event))
        return true;
    return receiver->event(event);
}
```
14.事件派发至QObject的子类QWidget
```cpp
bool QWidget::event(QEvent *event)
{
    ...
    switch (event->type()) {
    case QEvent::MouseMove:
        mouseMoveEvent((QMouseEvent*)event);
        break;
    case QEvent::MouseButtonPress:
        #if 0
        resetInputContext();
 #endif
        mousePressEvent((QMouseEvent*)event);
        break;
        ...
}
```
15.QWidget调用预定义的槽函数执行相应的动作。
