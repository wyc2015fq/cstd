# [Android] 消息处理机制 - weixin_33985507的博客 - CSDN博客
2016年06月06日 12:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
## 概述
Android 的消息处理机制主要是指 Handler 的运行机制以及 Handler 所附带的 MessageQueue 和 Looper 的工作流程。
在 Handler 创建完毕之后，就可以通过 Handler.post 方法将一个 Runnable 转换成一个 Message 对象，它会调用 MessageQueue 的 enqueueMessage() 将其放入消息队列中：
```
public final boolean post(Runnable r)
{
   return  sendMessageDelayed(getPostMessage(r), 0);
}
public final boolean sendMessageDelayed(Message msg, long delayMillis)
{
    if (delayMillis < 0) {
        delayMillis = 0;
    }
    return sendMessageAtTime(msg, SystemClock.uptimeMillis() + delayMillis);
}
public boolean sendMessageAtTime(Message msg, long uptimeMillis) {
    MessageQueue queue = mQueue;
    if (queue == null) {
        RuntimeException e = new RuntimeException(
                this + " sendMessageAtTime() called with no mQueue");
        Log.w("Looper", e.getMessage(), e);
        return false;
    }
    return enqueueMessage(queue, msg, uptimeMillis);
}
```
Looper 发现有新消息到来时，就会处理这个消息，最终消息中的 Runnable 或 Handler 的 handleMessage 方法会被调用。这样就切换到了创建 Handler 所在的线程中去执行了。
其工作流程图如下所示：
![1903766-0864d85a80072fbc.png](https://upload-images.jianshu.io/upload_images/1903766-0864d85a80072fbc.png)
## MessageQueue 的工作原理
MessageQueue 类主要包含两个操作：插入和读取，它通过一个单链表的数据结构来维护消息列表，在每个 Looper 中都持有一个 MessageQueue 对象。
enqueueMessage 方法主要就是往单链表中插入一条消息，我们来看一下 next 读取方法的代码：
```
Message next() {
    // Return here if the message loop has already quit and been disposed.
    // This can happen if the application tries to restart a looper after quit
    // which is not supported.
    final long ptr = mPtr;
    if (ptr == 0) {
        return null;
    }
    int pendingIdleHandlerCount = -1; // -1 only during first iteration
    int nextPollTimeoutMillis = 0;
    for (;;) {
        if (nextPollTimeoutMillis != 0) {
            Binder.flushPendingCommands();
        }
        nativePollOnce(ptr, nextPollTimeoutMillis);
        synchronized (this) {
            // Try to retrieve the next message.  Return if found.
            final long now = SystemClock.uptimeMillis();
            Message prevMsg = null;
            Message msg = mMessages;
            if (msg != null && msg.target == null) {
                // Stalled by a barrier.  Find the next asynchronous message in the queue.
                do {
                    prevMsg = msg;
                    msg = msg.next;
                } while (msg != null && !msg.isAsynchronous());
            }
            if (msg != null) {
                if (now < msg.when) {
                    // Next message is not ready.  Set a timeout to wake up when it is ready.
                    nextPollTimeoutMillis = (int) Math.min(msg.when - now, Integer.MAX_VALUE);
                } else {
                    // Got a message.
                    mBlocked = false;
                    if (prevMsg != null) {
                        prevMsg.next = msg.next;
                    } else {
                        mMessages = msg.next;
                    }
                    msg.next = null;
                    if (DEBUG) Log.v(TAG, "Returning message: " + msg);
                    msg.markInUse();
                    return msg;
                }
            } else {
                // No more messages.
                nextPollTimeoutMillis = -1;
            }
            // Process the quit message now that all pending messages have been handled.
            if (mQuitting) {
                dispose();
                return null;
            }
            // If first time idle, then get the number of idlers to run.
            // Idle handles only run if the queue is empty or if the first message
            // in the queue (possibly a barrier) is due to be handled in the future.
            if (pendingIdleHandlerCount < 0
                    && (mMessages == null || now < mMessages.when)) {
                pendingIdleHandlerCount = mIdleHandlers.size();
            }
            if (pendingIdleHandlerCount <= 0) {
                // No idle handlers to run.  Loop and wait some more.
                mBlocked = true;
                continue;
            }
            if (mPendingIdleHandlers == null) {
                mPendingIdleHandlers = new IdleHandler[Math.max(pendingIdleHandlerCount, 4)];
            }
            mPendingIdleHandlers = mIdleHandlers.toArray(mPendingIdleHandlers);
        }
        // Run the idle handlers.
        // We only ever reach this code block during the first iteration.
        for (int i = 0; i < pendingIdleHandlerCount; i++) {
            final IdleHandler idler = mPendingIdleHandlers[i];
            mPendingIdleHandlers[i] = null; // release the reference to the handler
            boolean keep = false;
            try {
                keep = idler.queueIdle();
            } catch (Throwable t) {
                Log.wtf(TAG, "IdleHandler threw exception", t);
            }
            if (!keep) {
                synchronized (this) {
                    mIdleHandlers.remove(idler);
                }
            }
        }
        // Reset the idle handler count to 0 so we do not run them again.
        pendingIdleHandlerCount = 0;
        // While calling an idle handler, a new message could have been delivered
        // so go back and look again for a pending message without waiting.
        nextPollTimeoutMillis = 0;
    }
}
```
尽管代码偏长，但可以看出 next 方法实际上是一个无限循环，直到 MessageQueue 中有消息则将其取出并删除，否则会一直阻塞在这里。
## Looper 的工作原理
Looper 在消息处理机制中负责不断从 MessageQueue 中查看是否有新消息，如果有的话则进行处理，否则就一直阻塞在哪里。
在初始化 Looper 时会创建一个 MessageQueue：
```
private Looper(boolean quitAllowed) {
    mQueue = new MessageQueue(quitAllowed);
    mThread = Thread.currentThread();
}
```
当调用 prepare 方法时，可当前线程初始化并绑定一个 Looper，可以看到不能重复调用 prepare 方法：
```
private static void prepare(boolean quitAllowed) {
    if (sThreadLocal.get() != null) {
        throw new RuntimeException("Only one Looper may be created per thread");
    }
    sThreadLocal.set(new Looper(quitAllowed));
}
```
最后我们会调用 loop 方法来开启消息循环，这也是 Looper 中最重要的一个方法，我们先列出整体代码，再来依次进行分析：
```
public static void loop() {
    final Looper me = myLooper();
    if (me == null) {
        throw new RuntimeException("No Looper; Looper.prepare() wasn't called on this thread.");
    }
    final MessageQueue queue = me.mQueue;
    // Make sure the identity of this thread is that of the local process,
    // and keep track of what that identity token actually is.
    Binder.clearCallingIdentity();
    final long ident = Binder.clearCallingIdentity();
    for (;;) {
        Message msg = queue.next(); // might block
        if (msg == null) {
            // No message indicates that the message queue is quitting.
            return;
        }
        // This must be in a local variable, in case a UI event sets the logger
        Printer logging = me.mLogging;
        if (logging != null) {
            logging.println(">>>>> Dispatching to " + msg.target + " " +
                    msg.callback + ": " + msg.what);
        }
        msg.target.dispatchMessage(msg);
        if (logging != null) {
            logging.println("<<<<< Finished to " + msg.target + " " + msg.callback);
        }
        // Make sure that during the course of dispatching the
        // identity of the thread wasn't corrupted.
        final long newIdent = Binder.clearCallingIdentity();
        if (ident != newIdent) {
            Log.wtf(TAG, "Thread identity changed from 0x"
                    + Long.toHexString(ident) + " to 0x"
                    + Long.toHexString(newIdent) + " while dispatching to "
                    + msg.target.getClass().getName() + " "
                    + msg.callback + " what=" + msg.what);
        }
        msg.recycleUnchecked();
    }
}
```
先取出与当前线程绑定的 Looper，并取出 Looper 中的 MessageQueue 消息队列，接着就进入了无限循环：
```
final Looper me = myLooper();
if (me == null) {
    throw new RuntimeException("No Looper; Looper.prepare() wasn't called on this thread.");
}
final MessageQueue queue = me.mQueue;
for (;;) {
```
后面的代码都是在无限循环之内，这里取出消息，如果为 null 则退出循环：
```
Message msg = queue.next(); // might block
    if (msg == null) {
        // No message indicates that the message queue is quitting.
        return;
     }
```
注意当 MessageQueue 没有消息时，是不会返回 null 的，只会一直循环等待消息。只有当调用 quit 方法退出时，才会返回 null。
```
public void quit() {
    mQueue.quit(false);
}
```
接下来会对消息进行处理：
```
msg.target.dispatchMessage(msg);
```
msg.target 就是发送该消息的 Handler，这里就会将线程切换到该 Handler 所在的线程去处理该 msg，这样就完成了线程的切换啦~
## Handler 的工作原理
在概述中我们已经讲过了，Handler.post 方法会将消息插入到 Looper 中的消息队列，开启循环后又会将该消息转发到 Handler 所在线程进行处理，那么我们就来看一下 dispatchMessage 方法：
```
public void dispatchMessage(Message msg) {
    if (msg.callback != null) {
        handleCallback(msg);
    } else {
        if (mCallback != null) {
            if (mCallback.handleMessage(msg)) {
                return;
            }
        }
        handleMessage(msg);
    }
}
```
msg.callback 是 post 方法中传递进去的 Runnable 参数，如果不为空，则：
```
private static void handleCallback(Message message) {
    message.callback.run();
}
```
如果为空，则判断 Handler 的 Callback 是否为空，这个 Callback 的定义如下：
```
public interface Callback {
    public boolean handleMessage(Message msg);
}
```
我们可以用如下这种方式来创建一个 Handler：
public Handler(Callback callback) {
this(callback, false);
}
实际上是与我们重写 Handler.handleMessage 方法差不多的，只是另一种使用 Handler 的方式而已。
## 结语
那么以上就是，Android 的消息处理机制了，这一部分的代码比较简单易懂，所以推荐大家都去看看。
## 参考资料
[Android中的Handler的具体用法](https://link.jianshu.com?t=http://txlong-onz.iteye.com/blog/934957)
[android的消息处理机制](https://link.jianshu.com?t=http://www.cnblogs.com/codingmyworld/archive/2011/09/14/2174255.html)
[《Android 开发艺术探索》](https://link.jianshu.com?t=https://book.douban.com/subject/26599538/)
