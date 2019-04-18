# Android异步处理技术(一) - weixin_33985507的博客 - CSDN博客
2018年07月20日 11:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 异步处理技术(一)
## 写在前面
本文章为《Android高级进阶》的读书笔记,看过的朋友可以出门转去别的更深度的文章了，没看过的可以当作知识点来看一下,欢迎给意见进行知识交流。表达能力有限，不喜勿喷。
## 安卓定义的三种线程：
- 
### 主线程：
主线程就是我们熟悉的UI线程，谨记只有UI线程才成操作控件更新空间。想子线程任务完成更新界面的方法现在很多了，传统的使用handler，高大上的就是用rxJava等异步操作库。
- 
### Binder线程：
Binder线程可能很多人用的不多，它是用于不同的`进程`之间的通信，其实每一个进程都会有一个线程池来用出来别的`进程`发送过来的消息，比如：系统服务，Intents，ContentProviders，Service等，典型的用例就是应用使用Binder线程提供一个service给其他进程通过AIDL接口进行绑定。
- 
### 后台线程：
这个你要是不知道什么意思就过分了，它其实就是我们一直说的子线程，创建出来内容时空的，需要我们自己添加任务后start。不过其实要是上升到Linux层面主线程跟子线程是一样的东西，只是Android框架里面添加了主线程跟后台线程的概念，限制了后台线程能力，确保UI是线程安全的。
## HandlerThread
HandlerThread是一个自带Looper跟MessageQueue的线程，也就是说这个线程是一个能够实现异步操作的子线程。而且它会一直等待消息进行处理。下面它的基本用法：
```
HandlerThread handlerThread = new HandlerThread("HandlerThread");
handlerThread.start();
mHandler = new Handler(handlerThread.getLooper()){
    @Override 
    public void handlerMessage(Message msg){
        super.handlerMessage(msg);
        //处理收到的消息
    }
};
```
也是因为HandlerThread自带Looper，所以HandlerThread里面的任务是按顺序执行的，吞吐量也会受到一定的影响，不会这个是我认为实现异步的比较好理解的一种方式了。
## AsyncQueryHandler
AsyncQueryHandler主要用在ContnetProvider里面，用于执行Create，Read，Update，delete操作的`工具类`，AsyncQueryHandler主要封装了ContentResolver，HandlerThread，Handler等实现了对ContentProvider的异步操作，原理如图所示：
![10884654-9400d751da2aa679.png](https://upload-images.jianshu.io/upload_images/10884654-9400d751da2aa679.png)
AsyncQueryHandler.png
（图片不好找，自己照着画了一个）
通常使用AsyncQueryHandler会跟着ContentProvider一起使用，AsyncQueryHandler封装了如下四种方法来操作ContentProvider：
```
final void startDelete(...);
final void startInsert(...);
final void startQuery(...);
final void startUpdate(...);
```
相对的AsyncQueryHandler有相应的回调函数，可以得到上面四个方法的返回结果：
```
@Override
protected void onDeleteComplete(...){...}
@Override
protected void onUpdateComplete(...){...}
@Override
protected void onInsertComplete(...){...}
@Override
protected void onQueryComplete(...){...}
```
## IntentService
service我们熟悉的生命周期函数都是运行在主线程中的，本身并不是一个异步处理的技术。为了service里面可以使用子线程处理耗时任务，Android引入了IntentService，service的一个子类，具有跟service一样的生命周期，同时也提供着后台线程异步处理任务的机制，这一点跟上面讲到的HandlerThread十分的相似，IntentService会后台顺序执行所有任务，与我们遇到的一些后台下载场景十分的契合。
我们可以使用一个Intent类型的参数启动IntentService的异步执行，如果IntentService已经启动了，新的Intent将会进入队列进行等待，如果没有运行将会启动一个新的IntentService。当IntentService运行完所有的后台任务后，会自动结束自己的生命周期，不需要开发者手动结束，这是我觉得比较好的地方。
IntentService本身是一个抽象类，因此继承它需要实现它的抽象方法onHandleIntent来处理后台的业务逻辑，同时需要在构造方法中调用super（String name）传入子类的名字，示例代码如下：
```
public class SimpleIntentService extends IntentService{
    public SimpleIntentService(){
        super(SimpleIntentService.class.getName());
        setIntentRedelivery(true);
    }
    @Override
    protected void onHandleIntent(Intent intent){
        //此处写后台逻辑
    }
}
上面的setIntentRedelivery方法如果设置为true，那么IntentService的onStartCommand方法将会返回START_REDELIVER_INTENT。这时要是onHandleIntent调用之前进程死掉了，那么进程将会重新启动，intent也会重新投递。
使用IntentService最重要的一点就是，大家记得在AndroidManifest.xml文件里注册声明。
其实大家如果有兴趣的话可以查看下IntentService的源码，里面其实是通过HandlerThread来实现的，套了一层service的马甲，源码有点多，就贴几句：
    public abstract class IntentService extends Service {
        ...
        private final class ServiceHandler extends Handler{
            ...
            @Override
            public void handleMessage(Message msg){
                onHandleIntent((Intent)msg.obj);
                stopSelf(msg.arg1);
            }
        }
        @Override
        public void onCreate(){
            ...
            HandlerThread thread  = new HandlerThread("IntentService["++"mName"]");
            thread.start();
            mServiceLooper = thread.getLooper();
            mServiceHandler = new ServiceHandler(mServiceLooper);
        }
        @Override
        public void onStart(Intent intent,int startId){
            Message msg = mServiceHandler.obtainMessage();
            msg.arg1 = startId;
            msg.obj = intent;
            mServiceHandler.sendMessage(msg);
        }
        ...
    }
```
## 总结
本文先总结了Android里面三种异步处理的技术，这只是其一，后面还会总结书中提到的另外三种技术：Loader，AsyncTask，Executor Framework。如开头提到的文中都是些看《Android高级进阶》一本的知识点总结，写的都是自己比较浅显的理解，书中有更详细的篇幅介绍，感兴趣的朋友可以找来看看，里面全是干货。最后欢迎大家交流共同进步。
