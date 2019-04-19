# Qt事件处理机制详解 - fanyun的博客 - CSDN博客
2017年07月30日 21:36:58[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：603
Qt程序是基于事件驱动的，在每个GUI主线程的main函数后面都有return a.exec();
实际上就是使Qt程序进入事件循环。
下面我们看看Qt exec()的源码：
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
        qWarning("Qt has caught an exception thrown from an event handler. Throwing/n"
                 "exceptions from an event handler is not supported in Qt. You must/n"
                 "reimplement QApplication::notify() and catch all exceptions there./n");
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
        QThreadData *data = d->threadData;
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
