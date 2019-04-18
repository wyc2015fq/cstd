# [Android] 输入系统（一） - TaigaComplex求职中 - 博客园







# [[Android] 输入系统（一）](https://www.cnblogs.com/TaigaCon/p/4749450.html)





Android输入系统是人与机器交互最主要的手段。我们通过按键或者触碰屏幕，会先经由linux产生中断，进行统一的处理过后，转换成Android能识别的事件信息，然后Android的输入系统去获取事件，分发给上层用户程序进行处理。

下面在细分一下输入事件在Android系统中的流程：

![wpsC0C1.tmp](https://images0.cnblogs.com/blog/421096/201508/220259148475616.png)

从图上能看到，输入事件有四个处理的地方：
- InputReaderThread
- InputDispatcherThread
- WindowInputEventReceiver
- handleReceiverCallback





上面四个地方按功能来划分，其中：
- InputReaderThread负责从输入设备中获取事件，事件加入inboundQueue队列。
- InputDispatcherThread负责把inboundQueue中的事件信息取出，并且从系统中获取该事件所需要分发到的目标（窗口），把事件与目标分别整合成分发项，把分发项加入outboundQueue。另外，这里还是事件的分发端，负责把outboundQueue中的事件取出，通过InputChannel进行分发。分发完成后把该事件入waitQueue。
- WindowInputEventReceiver是事件的接收端。事件会在这里被onTouch这类回调函数处理
- handleReceiveCallback用于接收处理过后的反馈信息，事件在WindowInputEventReceiver端被处理成功或者失败，将会通过InputChannel返回Handled或者UNHandled消息。handleReceiveCallback接收到消息后将会对waitQueue中的事件进行出队列处理。





# InputManager

InputManager用于启动InputReaderThread与InputDispatcherThread，会在system_server初始化的时候被创建并且调用InputManager的start方法启动这两个线程。

InputManager的构造函数如下：
InputManager::InputManager(
        const sp<EventHubInterface>& eventHub,
        const sp<InputReaderPolicyInterface>& readerPolicy,
        const sp<InputDispatcherPolicyInterface>& dispatcherPolicy) {
    mDispatcher = new InputDispatcher(dispatcherPolicy);
    mReader = new InputReader(eventHub, readerPolicy, mDispatcher);
    initialize();
}
可以看到构造了InputDispatcher与InputReader两个类，这两个类是功能类，分别为InputDispatcherThread与InputReaderThread提供功能。另外，在构建InputReader的时候，把mDispatcher传递了进去，用于构建QueueInputListener。在这里可以提前说明一下这个成员的作用：把输入事件添加到inboundQueue。





构造函数最后调用了initialize，构建InputReaderThread、InputDispatcherThread。
void InputManager::initialize() {
    mReaderThread = new InputReaderThread(mReader);
    mDispatcherThread = new InputDispatcherThread(mDispatcher);
}
InputManager的start用于启动InputReaderThread与InputDispatcherThread这两个线程。
status_t InputManager::start() {
    status_t result = mDispatcherThread->run("InputDispatcher", PRIORITY_URGENT_DISPLAY);
    if (result) {
        ALOGE("Could not start InputDispatcher thread due to error %d.", result);
        return result;
    }

    result = mReaderThread->run("InputReader", PRIORITY_URGENT_DISPLAY);
    if (result) {
        ALOGE("Could not start InputReader thread due to error %d.", result);

        mDispatcherThread->requestExit();
        return result;
    }

    return OK;
}






![](https://images0.cnblogs.com/blog2015/421096/201508/230554584258157.jpg)









# InputReaderThread

InputReaderThread是用来从输入设备中读取输入事件的，首先看一下该线程的threadLoop函数
bool InputReaderThread::threadLoop() {
    mReader->loopOnce();
    return true;
}




mReader即在构建InputReaderThread时传进来的InputReader，负责实现读取输入事件所需要的各种功能。InputReader::loopOnce用于读取一次输入事件。其中，读取一次包含三个主要动作：
- 获取输入事件
- 处理输入事件
- 输入数据flush


void InputReader::loopOnce() {
    size_t count = mEventHub->getEvents(timeoutMillis, mEventBuffer, EVENT_BUFFER_SIZE);

    { // acquire lock
            processEventsLocked(mEventBuffer, count);
    }


    mQueuedListener->flush();
}

### 1. 获取输入事件getEvents

几乎所有与输入有关的事件都会从这里获得。其中包含:
- EPOLL_ID_INORIFY.输入设备打开或者删除的事件
- EPOLL_ID_WAKE.管道发送过来的模拟事件
- EPOLL_IN.按键，触摸这类实际操作事件





EPOLL_ID_INOTIFY，用于监控某个目录（子目录）下是否有新增或者删除文件，在这里用于监视/dev/input，这个是输入设备文件所在的目录，如果有新增设备，则会在该目录内创建新文件；如果删除设备，则该目录的相应文件会被删除。


            if (eventItem.data.u32 == EPOLL_ID_INOTIFY) {
                if (eventItem.events & EPOLLIN) {
                    mPendingINotify = true;
                } else {
                    ALOGW("Received unexpected epoll event 0x%08x for INotify.", eventItem.events);
                }
                continue;
            }

            ......

        if (mPendingINotify && mPendingEventIndex >= mPendingEventCount) {
            mPendingINotify = false;
            readNotifyLocked();
            deviceChanged = true;
        }


status_t EventHub::readNotifyLocked() {
            if(event->mask & IN_CREATE) {
                openDeviceLocked(devname);
            } else {
                ALOGI("Removing device '%s' due to inotify event\n", devname);
                closeDeviceByPathLocked(devname);
            }
}






EPOLL_ID_WAKE，EventHub有维护一个pipe，当pipe的写入端按照适当格式写入时间后，getEvents可以通过pipe的读取端获取这个虚拟事件


            if (eventItem.data.u32 == EPOLL_ID_WAKE) {
                if (eventItem.events & EPOLLIN) {
                    ALOGV("awoken after wake()");
                    awoken = true;
                    char buffer[16];
                    ssize_t nRead;
                    do {
                        nRead = read(mWakeReadPipeFd, buffer, sizeof(buffer));
                    } while ((nRead == -1 && errno == EINTR) || nRead == sizeof(buffer));
                } else {
                    ALOGW("Received unexpected epoll event 0x%08x for wake read pipe.",
                            eventItem.events);
                }
                continue;
            }




EPOLL_IN，用于监控设备文件的输入状态，当我们按键或者触摸设备时，我们就能获得EPOLL_IN状态，从而到该设备读取输入事件
            if (eventItem.events & EPOLLIN) {
                int32_t readSize = read(device->fd, readBuffer,
                        sizeof(struct input_event) * capacity);
                        event->when = now;
                        event->deviceId = deviceId;
                        event->type = iev.type;
                        event->code = iev.code;
                        event->value = iev.value;
                        event += 1;
                        capacity -= 1;
                    }




监听事件用的是epoll_wait，由于epoll_wait一次能获取的事件可能会有多个，所以一次的getEvents需要对所获得的每个事件都进行上述代码的打包操作，最后返回事件数组。
int pollResult = epoll_wait(mEpollFd, mPendingEventItems, EPOLL_MAX_EVENTS, timeoutMillis);




![](https://images0.cnblogs.com/blog2015/421096/201508/230538023638445.jpg)







### 2. 处理输入事件processEventsLocked

由getEvents获得的事件数组会在这个函数内进行处理，其中事件数组中的事件大致可以分为两类，在这个函数将他们分开处理
- 按键、触摸事件
- 设备增加、删除事件


void InputReader::processEventsLocked(const RawEvent* rawEvents, size_t count) {
    for (const RawEvent* rawEvent = rawEvents; count;) {
        int32_t type = rawEvent->type;
        size_t batchSize = 1;
        if (type < EventHubInterface::FIRST_SYNTHETIC_EVENT) {
            int32_t deviceId = rawEvent->deviceId;
            while (batchSize < count) {
                if (rawEvent[batchSize].type >= EventHubInterface::FIRST_SYNTHETIC_EVENT
                        || rawEvent[batchSize].deviceId != deviceId) {
                    break;
                }
                batchSize += 1;
            }
#if DEBUG_RAW_EVENTS
            ALOGD("BatchSize: %d Count: %d", batchSize, count);
#endif
            processEventsForDeviceLocked(deviceId, rawEvent, batchSize);
        } else {
            switch (rawEvent->type) {
            case EventHubInterface::DEVICE_ADDED:
                addDeviceLocked(rawEvent->when, rawEvent->deviceId);
                break;
            case EventHubInterface::DEVICE_REMOVED:
                removeDeviceLocked(rawEvent->when, rawEvent->deviceId);
                break;
            case EventHubInterface::FINISHED_DEVICE_SCAN:
                handleConfigurationChangedLocked(rawEvent->when);
                break;
            default:
                ALOG_ASSERT(false); // can't happen
                break;
            }
        }
        count -= batchSize;
        rawEvent += batchSize;
    }
}





在处理按键、触摸事件时，会根据他们设备的类型调用不同的process函数进行处理。对于触摸事件，基本上只是进行赋值，而按键事件则需要通过映射，把从设备文件读取进来的值转换成Android上层能统一处理的按键事件。
void InputReader::processEventsForDeviceLocked(int32_t deviceId,
        const RawEvent* rawEvents, size_t count) {
    InputDevice* device = mDevices.valueAt(deviceIndex);
    device->process(rawEvents, count);
}


void KeyboardInputMapper::process(const RawEvent* rawEvent) {
    switch (rawEvent->type) {
    case EV_KEY: {
            if (getEventHub()->mapKey(getDeviceId(), scanCode, usageCode, &keyCode, &flags)) {
                keyCode = AKEYCODE_UNKNOWN;
                flags = 0;
            }
            processKey(rawEvent->when, rawEvent->value != 0, keyCode, scanCode, flags);
        }
        break;
    }
}







上面的mapKey对按键进行了映射处理，processKey用于区分按键的按下或者松开。在processKey的最后，会把事件打包成NotifyKeyArgs，然后通过QueueInputListener把事件push进mArgQueue。由于这里是一个事件数组，所以mArgQueue是必须的。
void KeyboardInputMapper::processKey(nsecs_t when, bool down, int32_t keyCode,
        int32_t scanCode, uint32_t policyFlags) {

    if (down) {
    ...
    } else {
    ...
    }
    NotifyKeyArgs args(when, getDeviceId(), mSource, policyFlags,
            down ? AKEY_EVENT_ACTION_DOWN : AKEY_EVENT_ACTION_UP,
            AKEY_EVENT_FLAG_FROM_SYSTEM, keyCode, scanCode, newMetaState, downTime);
    getListener()->notifyKey(&args);
}


void QueuedInputListener::notifyKey(const NotifyKeyArgs* args) {
    mArgsQueue.push(new NotifyKeyArgs(*args));
}






![](https://images0.cnblogs.com/blog2015/421096/201508/230538351134584.jpg)





### 3. 输入数据flush

在事件数组都push进mArgQueue之后，就需要把mArgQueue队列给推送出去进行下一步的操作，mQueuedListener->flush();就是负责进行队列的推送。还记得我们最开始说的”在构建InputReader的时候，把mDispatcher传递了进去，用于构建QueueInputListener”,我们这里的flush最终就是调用了InputDispatcher的notifyKey
void QueuedInputListener::flush() {
    size_t count = mArgsQueue.size();
    for (size_t i = 0; i < count; i++) {
        NotifyArgs* args = mArgsQueue[i];
        args->notify(mInnerListener);
        delete args;
    }
    mArgsQueue.clear();
}

void NotifyKeyArgs::notify(const sp<InputListenerInterface>& listener) const {
    listener->notifyKey(this);
}



![](https://images0.cnblogs.com/blog2015/421096/201508/230539064726965.jpg)





以notifyKey为例，其目的实际上是把事件队列加入mInboundQueue，但是在入mInboundQueue队列之前，调用了interceptKeyBeforeQueueing，该函数通过jni，调用到PhoneWindowManager的interceptKeyBeforeQueueing。而在入了mInboundQueue队列后，就会调用wake函数去唤醒InputDispatcherThread。下一步就是InputDispatcherThread的工作了。
void InputDispatcher::notifyKey(const NotifyKeyArgs* args) {
    mPolicy->interceptKeyBeforeQueueing(&event, /*byref*/ policyFlags);
    needWake = enqueueInboundEventLocked(newEntry);
    if (needWake) {
        mLooper->wake();
    }
}






# InputDispatcherThread

InputDispatcherThread是用来进行事件分发的线程。内部也是调用InputDispatcher来实现所需要的功能。
bool InputDispatcherThread::threadLoop() {
    mDispatcher->dispatchOnce();
    return true;
}




每次分发，调用的都是dispatchOnce，其内部调用dispatchOnceInnerLocked进行分发后，线程会调用pollOnce进入睡眠，等待下次InputReaderThread的wake操作
void InputDispatcher::dispatchOnce() {
    dispatchOnceInnerLocked(&nextWakeupTime);
    mLooper->pollOnce(timeoutMillis);
}





分发的过程可以大概分成以下几个步骤：
- 从mInboundQueue的队列头取出事件
- 特殊事件的处理，如POLICY_FLAG_PASS_TO_USER这类事件能直接发送到用户，类似于电量不足的这类事件：当电量低于20%时，直接往上层发送事件，而不用知道当前是在哪个Activity
- 一般事件的处理，进行分发


void InputDispatcher::dispatchOnceInnerLocked(nsecs_t* nextWakeupTime) {
    mPendingEvent = mInboundQueue.dequeueAtHead();

    // Poke user activity for this event.
    if (mPendingEvent->policyFlags & POLICY_FLAG_PASS_TO_USER) {
        pokeUserActivityLocked(mPendingEvent);
    }

    switch (mPendingEvent->type) {
    case EventEntry::TYPE_KEY: {
        done = dispatchKeyLocked(currentTime, typedEntry, &dropReason, nextWakeupTime);
        break;
    }
    }
}





分发事件，肯定需要知道事件要分发到哪里，即分发的目标窗口，不过目标窗口可能不止一个。
bool InputDispatcher::dispatchKeyLocked(nsecs_t currentTime, KeyEntry* entry,
        DropReason* dropReason, nsecs_t* nextWakeupTime) {
    int32_t injectionResult = findFocusedWindowTargetsLocked(currentTime,
            entry, inputTargets, nextWakeupTime);
    
    // Dispatch the key.
    dispatchEventLocked(currentTime, entry, inputTargets);
    return true;
}





由于可能存在多个目标窗口，所以需要对每个目标窗口都进行事件分发
void InputDispatcher::dispatchEventLocked(nsecs_t currentTime,
        EventEntry* eventEntry, const Vector<InputTarget>& inputTargets) {
    for (size_t i = 0; i < inputTargets.size(); i++) {
            prepareDispatchCycleLocked(currentTime, connection, eventEntry, &inputTarget);
    }
}





在分发前的准备，就是把事件入outboundQueue队列，不过请注意，这里的队列不同于inboundQueue，因为outboundQueue是窗口相关的，窗口跟InputDispatcherThread间建立起一个连接（connection），该outboundQueue就是connection的成员。
void InputDispatcher::prepareDispatchCycleLocked(nsecs_t currentTime,
        const sp<Connection>& connection, EventEntry* eventEntry, const InputTarget* inputTarget) {
    // Not splitting.  Enqueue dispatch entries for the event as is.
    enqueueDispatchEntriesLocked(currentTime, connection, eventEntry, inputTarget);
}


void InputDispatcher::enqueueDispatchEntriesLocked(nsecs_t currentTime,
        const sp<Connection>& connection, EventEntry* eventEntry, const InputTarget* inputTarget) {
    bool wasEmpty = connection->outboundQueue.isEmpty();

    // Enqueue dispatch entries for the requested modes.
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
            InputTarget::FLAG_DISPATCH_AS_HOVER_EXIT);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
            InputTarget::FLAG_DISPATCH_AS_OUTSIDE);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
            InputTarget::FLAG_DISPATCH_AS_HOVER_ENTER);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
            InputTarget::FLAG_DISPATCH_AS_IS);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
            InputTarget::FLAG_DISPATCH_AS_SLIPPERY_EXIT);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
            InputTarget::FLAG_DISPATCH_AS_SLIPPERY_ENTER);

    // If the outbound queue was previously empty, start the dispatch cycle going.
    if (wasEmpty && !connection->outboundQueue.isEmpty()) {
        startDispatchCycleLocked(currentTime, connection);
    }
}

void InputDispatcher::enqueueDispatchEntryLocked(
        const sp<Connection>& connection, EventEntry* eventEntry, const InputTarget* inputTarget,
        int32_t dispatchMode) {
    // Enqueue the dispatch entry.
    connection->outboundQueue.enqueueAtTail(dispatchEntry);
}




在准备完成后就会调用startDispatchCycleLocked进行事件分发，startDispatchCycleLocked这个函数的主体是一个while循环，在循环体内会执行下面三个主要步骤：
- 调用connection的inputPublisher来发出事件
- 把事件从outboundQueue队列中移除
- 把事件加入waitQueue队列，当事件在处理完成后返回，就会从waitQueue中删除该事件


void InputDispatcher::startDispatchCycleLocked(nsecs_t currentTime,
        const sp<Connection>& connection) {
    while (connection->status == Connection::STATUS_NORMAL
            && !connection->outboundQueue.isEmpty()) {
        DispatchEntry* dispatchEntry = connection->outboundQueue.head;

        switch (eventEntry->type) {
        case EventEntry::TYPE_KEY: {
            KeyEntry* keyEntry = static_cast<KeyEntry*>(eventEntry);

            // Publish the key event.
            status = connection->inputPublisher.publishKeyEvent(dispatchEntry->seq,
                    keyEntry->deviceId, keyEntry->source,
                    dispatchEntry->resolvedAction, dispatchEntry->resolvedFlags,
                    keyEntry->keyCode, keyEntry->scanCode,
                    keyEntry->metaState, keyEntry->repeatCount, keyEntry->downTime,
                    keyEntry->eventTime);
        }
        }

        // Re-enqueue the event on the wait queue.
        connection->outboundQueue.dequeue(dispatchEntry);
        traceOutboundQueueLengthLocked(connection);
        connection->waitQueue.enqueueAtTail(dispatchEntry);
        traceWaitQueueLengthLocked(connection);
    }
}





我们来看一下inputPublisher的publishKeyEvent的实现，最后也是调用socket的send接口来实现。
status_t InputPublisher::publishKeyEvent(
        uint32_t seq,
        int32_t deviceId,
        int32_t source,
        int32_t action,
        int32_t flags,
        int32_t keyCode,
        int32_t scanCode,
        int32_t metaState,
        int32_t repeatCount,
        nsecs_t downTime,
        nsecs_t eventTime) {

    InputMessage msg;
    msg.header.type = InputMessage::TYPE_KEY;
    msg.body.key.seq = seq;
    msg.body.key.deviceId = deviceId;
    msg.body.key.source = source;
    msg.body.key.action = action;
    msg.body.key.flags = flags;
    msg.body.key.keyCode = keyCode;
    msg.body.key.scanCode = scanCode;
    msg.body.key.metaState = metaState;
    msg.body.key.repeatCount = repeatCount;
    msg.body.key.downTime = downTime;
    msg.body.key.eventTime = eventTime;
    return mChannel->sendMessage(&msg);
}

status_t InputChannel::sendMessage(const InputMessage* msg) {
    do {
        nWrite = ::send(mFd, msg, msgLength, MSG_DONTWAIT | MSG_NOSIGNAL);
    } while (nWrite == -1 && errno == EINTR);
}
![](https://images0.cnblogs.com/blog2015/421096/201508/230540429883351.jpg)











总体的流程如下

![image](https://images0.cnblogs.com/blog/421096/201508/220259324566428.png)












