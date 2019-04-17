# AsyncTask 、HandlerTread 和 IntentService 会与 Handler 有什么样的姻亲关系？ - DEVELOPER - CSDN博客





2018年03月26日 16:42:09[学术袁](https://me.csdn.net/u012827205)阅读数：82








`AsyncTask`、`HandlerTread` 和 `IntentService` 会与 `Handler`有什么样的姻亲关系？，上个月一口气把这四块内容通读了下，觉得神清气爽有种融会贯通的感觉。但是昨晚遇到一个问题在联想到这里的时候，又突然一片空白。所以，想着今天在细细通读下并做下记录。

## Handler 异步消息机制

看代码 
![这里写图片描述](https://img-blog.csdn.net/20180326140757948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
此时可以看到`Handler` 是定义在了`主线程` 中。 

由此，这里生出**两个问题**
- 
**请问为什么说，Handler 这种直接写在Activity中，就是可以的，并且不用写方法`Looper.prepare()` 和 `Looper.loop()` 。那到底Handler的执行原理是怎样的呢？？**

在App初始化的时候会执行`ActivityThread`的`main`方法，看代码 
![这里写图片描述](https://img-blog.csdn.net/20180326145834277?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到`Looper.prepare()` 和 `Looper.loop()` 都被依次调用！因此在主线程虽然不用再次调用这两个方法，也可以让Handler顺利运行。也从侧面反映了，使用`Handler`的时候是必须要有这两个方法进行开路的。 

看其执行源码探究原理，方法`Looper.prepareMainLooper()` 即 类`Looper`的 `Looper.prepare()` 被调用。进入类 `public final class Looper` 查看源码 
![这里写图片描述](https://img-blog.csdn.net/20180326142822804?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码中在使用`Looper.prepare()` 方法过程中，构建了`Looper` 对象。通过构造方法可以看到 
![这里写图片描述](https://img-blog.csdn.net/20180326142847993?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里在构建 Looper 实例的过程中，同时创建了类`MessageQueue` 的对象。 

大概意思就是：在App初始化的时候执行`ActivityThread`的`main`方法，main方法中调用`Looper.prepare()`的同时关联到了对象`MessageQueue`。 

而且我们还能看到，在App初始化的时候执行`ActivityThread`的`main`方法，main方法中也在后面调用`Looper.loop()` 方法。进入`loop()` 方法中，其实它就是一个死循环，有这样一行代码，`for (;;)` 。一直在消息队列中做消息的分发处理。 
![这里写图片描述](https://img-blog.csdn.net/20180326151207166?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

又调用了一个方法`msg.target.dispatchMessage(msg)` ， 而`msg.target` 通过上下文可以明确知道是指向 `Handler` 的引用。进入方法`dispatchMessage(msg)` 的源码，可以清晰的看到该方法回调了`Handler` 中的方法`HandleMessage(String msg)`
![这里写图片描述](https://img-blog.csdn.net/20180326151443236?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 
**请问我不定义在主线程中，可以吗？？**

当然是可以的，要看怎操作了！正规的编写方式是这样的无论主线程（只是在主线程中已经预先定义铺垫过了）或者子线程：


```python
Looper.prepare();//1必须滴
Handler mHandler = new Handler() {
      @Override
      public void handleMessage(Message msg) {
         if (msg.what == 110) {
             Log.i(TAG, "接收到handler消息...");
         }
      }
};
Looper.loop();//2必须滴
```

**总结：**说到Handler的异步消息原理，其实就是分析Handler、Looper和MessageQueue三者之间的关系。Looper中的有两个静态方法`Looper.prepare();` 和 `Looper.loop();` 。方法`Looper.prepare();` 执行的过程中关联到了`MessageQueue` 对象（构建了MessageQueue的实例），而在我们使用`mHandler.sendMessage()` 方法时便把消息（msg）塞入到了`messageQueue消息队列` 中进行或等待分发处理。而方法`Looper.loop();` 就是做对消息队列中的消息进行分发处理这个功能的。这样一个异步消息机制的`消息的产生-消息队列中的分发处理-消息的消费` 过程就完成了。

## AsyncTask  异步任务

对于`AsynTask`的异步任务其实就是使用`Handler + Runnable`来完成的任务功能。 

为证实这种方式的存在，通过源码分析。此处贴出`AsynTask` 的构造方法

```java
/**
     * Creates a new asynchronous task. This constructor must be invoked on the UI thread.
     *
     * @hide
     */
    public AsyncTask(@Nullable Looper callbackLooper) {
        mHandler = callbackLooper == null || callbackLooper == Looper.getMainLooper()
            ? getMainHandler()
            : new Handler(callbackLooper);

        mWorker = new WorkerRunnable<Params, Result>() {
            public Result call() throws Exception {
                mTaskInvoked.set(true);
                Result result = null;
                try {
                    Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
                    //noinspection unchecked
                    result = doInBackground(mParams);
                    Binder.flushPendingCommands();
                } catch (Throwable tr) {
                    mCancelled.set(true);
                    throw tr;
                } finally {
                    postResult(result);
                }
                return result;
            }
        };

        mFuture = new FutureTask<Result>(mWorker) {
            @Override
            protected void done() {
                try {
                    postResultIfNotInvoked(get());
                } catch (InterruptedException e) {
                    android.util.Log.w(LOG_TAG, e);
                } catch (ExecutionException e) {
                    throw new RuntimeException("An error occurred while executing doInBackground()",
                            e.getCause());
                } catch (CancellationException e) {
                    postResultIfNotInvoked(null);
                }
            }
        };
    }
```

代码中`WorkerRunnable` 通过`FutureTask` 的封装，而 `FutureTask`如果继续追踪的话会看到 `FutureTask`最终实现了接口`Runnable` 。也就是说`FutureTask` 其实就是一个线程任务。 
![这里写图片描述](https://img-blog.csdn.net/20180326172458646?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

继续深入 
![这里写图片描述](https://img-blog.csdn.net/20180326172530657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从代码中看到事实就是如此。这时，一些耗时操作在`result = doInBackground(mParams);` 中执行就说的通了，因为等同于在一个子线程中执行，而非UI线程。最终把`doInBackground()`执行的结果传递给`postResult(result);` ，进入`AsyncTask` 类的中方法 `postResult(result);`
![这里写图片描述](https://img-blog.csdn.net/20180326173042118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里来看，又回到了`Handler` 的异步消息机制中。类 `Message` 的实例通过`Handler`获取，并且类 `Message` 的实例中关联有`Handler` 的实例对象。通过方法`message.sendToTarget();` 其实就是在使用 `Handler` 进行发送异步信息，看代码
```java
/**
     * Sends this Message to the Handler specified by {@link #getTarget}.
     * Throws a null pointer exception if this field has not been set.
     */
    public void sendToTarget() {
        target.sendMessage(this);
    }
```

然后在`AsyncTask` 类中寻找到消费消息的方法，在这里

```java
private static class InternalHandler extends Handler {
        public InternalHandler(Looper looper) {
            super(looper);
        }

        @SuppressWarnings({"unchecked", "RawUseOfParameterizedType"})
        @Override
        public void handleMessage(Message msg) {
            AsyncTaskResult<?> result = (AsyncTaskResult<?>) msg.obj;
            switch (msg.what) {
                case MESSAGE_POST_RESULT:
                    // There is only one result
                    result.mTask.finish(result.mData[0]);
                    break;
                case MESSAGE_POST_PROGRESS:
                    result.mTask.onProgressUpdate(result.mData);
                    break;
            }
        }
    }
```

从代码中能分析到两点，1 方法 `result.mTask.onProgressUpdate(result.mData);` 可以进行UI操作。2 方法`result.mTask.finish(result.mData[0]);`也可以进行UI操作，并且这个方法中执行两个重要的方法逻辑，请看

```java
private void finish(Result result) {
        if (isCancelled()) {
            onCancelled(result);
        } else {
            onPostExecute(result);
        }
        mStatus = Status.FINISHED;
    }
```

到这里，基本逻辑已经分析完了！ 
**总结：**`AsynTask` 的实现是通过 `Handler+Runable` 。方法`doInBackground()`执行在子线程归`Runnable`线程管理，方法`onProgressUpdate()`、`onPreExecute()`和`onPostExecute()`都执行在Ui线程通过Handler的异步消息机制进行管理。 

从源码中还得出这样一个结论：对于抽象类`AsynTask` 的三个泛型参数`<Params, Progress, Result>` 第一个参数对应的是`protected abstract Result doInBackground(Params... params)`；第二个参数对应的是`protected void onProgressUpdate(Progress... values)` ；第三个参数对应的是 `protected void onPostExecute(Result result)` 。
## HandlerThread 线程间通信

对于`HandlerThread` 我们看一下它的继承关系 `public class HandlerThread extends Thread`；在此说明`HandlerThread` 实质上就是一个线程`Thread` 。 

贴一个简单使用

```java
public class MainActivity extends AppCompatActivity {

    private TextView tvNum;
    private int random = 1;
    HandlerThread ht;
    private final String TAG = "MainActivity.class";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tvNum = (TextView) findViewById(R.id.tvNum);

        ht = new HandlerThread("handlerthread");
        ht.start();

        final Handler _handler = new Handler(ht.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                random += 1;
                Log.e(TAG, "HandlerThread-" + random);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        tvNum.setText(String.valueOf(random));
                    }
                });
            }
        };


        _handler.sendEmptyMessageDelayed(0, 3 * 1000);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ht.quit();
    }
}
```

在我进行编写一个简单的代码逻辑中，我本想在方法`handlerMessage()` 方法中去更改UI。而结果是`Only the original thread that created a view hierarchy can touch its views<只有创建了视图层次结构的原始线程才能触及它的视图>.` 说明使用`HandlerThread` 配合`Handler` 使用的时候还不能在其中做UI操作。 

先看一下`HandlerThread` 的构造方法

```java
/**
     * Constructs a HandlerThread.
     * @param name
     * @param priority The priority to run the thread at. The value supplied must be from 
     * {@link android.os.Process} and not from java.lang.Thread.
     */
    public HandlerThread(String name, int priority) {
        super(name);
        mPriority = priority;
    }
```

很普通，就是起个线程名字、设置线程的优先级别。 

 沿着源码继续看下它的 `run()` 方法

```java
@Override
    public void run() {
        mTid = Process.myTid();
        Looper.prepare();
        synchronized (this) {
            mLooper = Looper.myLooper();
            notifyAll();
        }
        Process.setThreadPriority(mPriority);
        onLooperPrepared();
        Looper.loop();
        mTid = -1;
    }
```

首先能看到`synchronized` 说明该线程是线程安全的。ok，关键的代码显而易见 `Looper.prepare();` 和 `Looper.loop();` 还是 `Handler` 异步消息机制的代码风格。说明在`HandlerThread` 在调用方法`start()` 时候，`HandlerThread`线程已经为`Handler`创建了该线程的`Looper与MessageQueue`。其中`onLooperPrepared()` 只是一个未实现的空的方法，但是在这里我们可以做一些Looper的初始化操作，暂且不管他。 

 再看我刚才贴上的测试代码的片段

```java
final Handler _handler = new Handler(ht.getLooper()) {
            @Override
            public void handleMessage(Message msg) {
                random += 1;
                Log.e(TAG, "HandlerThread-" + random);
                        tvNum.setText(String.valueOf(random));
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                    }
                });
            }
        };
```

其中`final Handler _handler = new Handler(ht.getLooper())`这一行中的Handler构造方法中传入的是`ht.getLooper()`， 也就是说Handler中关联的Looper对象是从HandlerThread中拿到的。

```java
/**
     * Use the provided {@link Looper} instead of the default one.
     *
     * @param looper The looper, must not be null.
     */
    public Handler(Looper looper) {
        this(looper, null, false);
    }
```

好，回到重要环节。**`为什么 Handler 的构造方法中拿到 HandlerThread 的 Looper 引用最终会导致 Handler中的handleMessage()方法不能进行UI操作？？`**

结果很显然，对于`MessageQueue`队列中的 消息的处理，是通过方法`Looper.loop()` 方法，而在该方法中最终又调用了`msg.target.dispatchMessage(msg);` 对消息进行处理，也就是代码

```java
/**
     * Handle system messages here.
     */
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

也就是说这些所有的消息分发到处理过程，实质上的执行位置还是在`HandlerTread` 的 方法`run()` 中，一个非UI线程方法。 

而我们也可以通过`HandlerThread` 的方法`quit()` 进行直接有效的控制，即可以立即终止消息队列对消的分发执行。 
![这里写图片描述](https://img-blog.csdn.net/20180326213120973?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**总结：** HandlerTread 就是一个普通的Thread线程，只是该线程在执行的过程中为`Handler`创建了所需要的`Looper与MessageQueue`。然后通过`HandlerThread + Handler` 组合出了一个灵活多样化的线程间通信的处理方案。
## IntentService

```java
/**
 * IntentService is a base class for {@link Service}s that handle asynchronous
 * requests (expressed as {@link Intent}s) on demand.  Clients send requests
 * through {@link android.content.Context#startService(Intent)} calls; the
 * service is started as needed, handles each Intent in turn using a worker
 * thread, and stops itself when it runs out of work.
 *
 * <p>This "work queue processor" pattern is commonly used to offload tasks
 * from an application's main thread.  The IntentService class exists to
 * simplify this pattern and take care of the mechanics.  To use it, extend
 * IntentService and implement {@link #onHandleIntent(Intent)}.  IntentService
 * will receive the Intents, launch a worker thread, and stop the service as
 * appropriate.
 *
 * <p>All requests are handled on a single worker thread -- they may take as
 * long as necessary (and will not block the application's main loop), but
 * only one request will be processed at a time.
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For a detailed discussion about how to create services, read the
 * <a href="{@docRoot}guide/components/services.html">Services</a> developer
 * guide.</p>
 * </div>
 *
 * @see android.os.AsyncTask
 */
public abstract class IntentService extends Service {
    private volatile Looper mServiceLooper;
    private volatile ServiceHandler mServiceHandler;
    private String mName;
    private boolean mRedelivery;

    private final class ServiceHandler extends Handler {
        public ServiceHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            onHandleIntent((Intent)msg.obj);
            stopSelf(msg.arg1);
        }
    }

    /**
     * Creates an IntentService.  Invoked by your subclass's constructor.
     *
     * @param name Used to name the worker thread, important only for debugging.
     */
    public IntentService(String name) {
        super();
        mName = name;
    }

    /**
     * Sets intent redelivery preferences.  Usually called from the constructor
     * with your preferred semantics.
     *
     * <p>If enabled is true,
     * {@link #onStartCommand(Intent, int, int)} will return
     * {@link Service#START_REDELIVER_INTENT}, so if this process dies before
     * {@link #onHandleIntent(Intent)} returns, the process will be restarted
     * and the intent redelivered.  If multiple Intents have been sent, only
     * the most recent one is guaranteed to be redelivered.
     *
     * <p>If enabled is false (the default),
     * {@link #onStartCommand(Intent, int, int)} will return
     * {@link Service#START_NOT_STICKY}, and if the process dies, the Intent
     * dies along with it.
     */
    public void setIntentRedelivery(boolean enabled) {
        mRedelivery = enabled;
    }

    @Override
    public void onCreate() {
        // TODO: It would be nice to have an option to hold a partial wakelock
        // during processing, and to have a static startService(Context, Intent)
        // method that would launch the service & hand off a wakelock.

        super.onCreate();
        HandlerThread thread = new HandlerThread("IntentService[" + mName + "]");
        thread.start();

        mServiceLooper = thread.getLooper();
        mServiceHandler = new ServiceHandler(mServiceLooper);
    }

    @Override
    public void onStart(@Nullable Intent intent, int startId) {
        Message msg = mServiceHandler.obtainMessage();
        msg.arg1 = startId;
        msg.obj = intent;
        mServiceHandler.sendMessage(msg);
    }

    /**
     * You should not override this method for your IntentService. Instead,
     * override {@link #onHandleIntent}, which the system calls when the IntentService
     * receives a start request.
     * @see android.app.Service#onStartCommand
     */
    @Override
    public int onStartCommand(@Nullable Intent intent, int flags, int startId) {
        onStart(intent, startId);
        return mRedelivery ? START_REDELIVER_INTENT : START_NOT_STICKY;
    }

    @Override
    public void onDestroy() {
        mServiceLooper.quit();
    }

    /**
     * Unless you provide binding for your service, you don't need to implement this
     * method, because the default implementation returns null.
     * @see android.app.Service#onBind
     */
    @Override
    @Nullable
    public IBinder onBind(Intent intent) {
        return null;
    }

    /**
     * This method is invoked on the worker thread with a request to process.
     * Only one Intent is processed at a time, but the processing happens on a
     * worker thread that runs independently from other application logic.
     * So, if this code takes a long time, it will hold up other requests to
     * the same IntentService, but it will not hold up anything else.
     * When all requests have been handled, the IntentService stops itself,
     * so you should not call {@link #stopSelf}.
     *
     * @param intent The value passed to {@link
     *               android.content.Context#startService(Intent)}.
     *               This may be null if the service is being restarted after
     *               its process has gone away; see
     *               {@link android.app.Service#onStartCommand}
     *               for details.
     */
    @WorkerThread
    protected abstract void onHandleIntent(@Nullable Intent intent);
}
```

本来我还打算对`IntentService` 仔细的分析一下，深刻的了解一下她与`Handler` 之间的关系。结果看到她简单的源码与类注释之后，我发现其实她其实就裸露在你身边，只是你看不到身边的风景而已！ 

如果看住是的话 
```
IntentService is a base class for {@link Service}s that handle asynchronous requests (expressed as {@link Intent}s) on demand.  Clients send requests 

through {@link android.content.Context#startService(Intent)} calls; the 

service is started as needed, handles each Intent in turn using a worker 

thread, and stops itself when it runs out of work. 

*大概意思是：*`IntentService` 就是一个可以进行处理异步操作命令请求的继承自`Service` 的一个抽象类。`Clients` 通过调用`startService` 发送请求命令。`Service`是按需要启动的，使用一个工作线程来处理每个`Intent`，并直到完成所有的任务工作时，则停止。 

接下来我们快速分析一下 
`IntentService` 中定义了一个内部类`ServiceHander` 继承自 `Handler` 用来消费信息
```java
private final class ServiceHandler extends Handler {
        public ServiceHandler(Looper looper) {
            super(looper);
        }

        @Override
        public void handleMessage(Message msg) {
            onHandleIntent((Intent)msg.obj);
            stopSelf(msg.arg1);
        }
    }
```

而在`IntentService` 的生命周期的方法 `onCreate()` 中

```java
@Override
    public void onCreate() {
        // TODO: It would be nice to have an option to hold a partial wakelock
        // during processing, and to have a static startService(Context, Intent)
        // method that would launch the service & hand off a wakelock.

        super.onCreate();
        HandlerThread thread = new HandlerThread("IntentService[" + mName + "]");
        thread.start();

        mServiceLooper = thread.getLooper();
        mServiceHandler = new ServiceHandler(mServiceLooper);
    }
```

我们是不是看到什么了，对是`HandlerThread` ，鉴于上面对`HandlerThread` 的分析我们也应该明白了。其实`IntentService` 功能的形成就是使用了 `Service + HandlerThread` 的组合功能。当然，方法`onHandleIntent((Intent)msg.obj)`也是不能进行UI操作的。

好了，到此我们通过上面的内容可以总结到。对于Handler的使用范围还是很广泛的，通过不同的方式对Handler进行封装可以得到我们想要的逻辑设计。也就是说 Handler 的实现原理很重要，他骑着基石的作用！谢谢
```










