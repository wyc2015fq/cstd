# Android Service学习之本地服务 - xqhrs232的专栏 - CSDN博客
2012年03月07日 15:53:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：363标签：[service																[android																[button																[binding																[class																[application](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=binding&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://android.blog.51cto.com/268543/527314](http://android.blog.51cto.com/268543/527314)
相关网帖
1.[Android Service学习笔记](http://www.cnblogs.com/feisky/archive/2010/06/14/1758336.html) ----[http://www.cnblogs.com/feisky/archive/2010/06/14/1758336.html](http://www.cnblogs.com/feisky/archive/2010/06/14/1758336.html)
2.[Android Service生命周期及用法！](http://blog.csdn.net/android_tutor/article/details/5789203)----[http://blog.csdn.net/android_tutor/article/details/5789203](http://blog.csdn.net/android_tutor/article/details/5789203)
Service是在一段不定的时间运行在后台，不和用户交互应用组件。每个Service必须在manifest中 通过<service>来声明。可以通过contect.startservice和contect.bindserverice来启动。
    Service和其他的应用组件一样，运行在进程的主线程中。这就是说如果service需要很多耗时或者阻塞的操作，需要在其子线程中实现。
    service的两种模式（startService()/bindService()不是完全分离的）：
- **本地服务** Local Service 用于应用程序内部。
  它可以启动并运行，直至有人停止了它或它自己停止。在这种方式下，它以调用Context.startService()启动，而以调用Context.stopService()结束。它可以调用Service.stopSelf() 或 Service.stopSelfResult()来自己停止。不论调用了多少次startService()方法，你只需要调用一次stopService()来停止服务。
  用于实现应用程序自己的一些耗时任务，比如查询升级信息，并不占用应用程序比如Activity所属线程，而是单开线程后台执行，这样用户体验比较好。 
- **远程服务** Remote Service 用于android系统内部的应用程序之间。
  它可以通过自己定义并暴露出来的接口进行程序操作。客户端建立一个到服务对象的连接，并通过那个连接来调用服务。连接以调用Context.bindService()方法建立，以调用 Context.unbindService()关闭。多个客户端可以绑定至同一个服务。如果服务此时还没有加载，bindService()会先加载它。
  可被其他应用程序复用，比如天气预报服务，其他应用程序不需要再写这样的服务，调用已有的即可。
**生命周期**
    Service的生命周期并不像Activity那么复杂，它只继承了onCreate(),onStart(),onDestroy()三个方法，当我们第一次启动Service时，先后调用了onCreate(),onStart()这两个方法，当停止Service时，则执行onDestroy()方法，这里需要注意的是，如果Service已经启动了，当我们再次启动Service时，不会在执行onCreate()方法，而是直接执行onStart()方法。
    而启动service，根据onStartCommand的返回值不同，有两个附加的模式：
    1. START_STICKY 用于显示启动和停止service。
    2. START_NOT_STICKY或START_REDELIVER_INTENT用于有命令需要处理时才运行的模式。
服务不能自己运行，需要通过调用Context.startService()或Context.bindService()方法启动服务。这两个方法都可以启动Service，但是它们的使用场合有所不同。
    1. 使用startService()方法启用服务，调用者与服务之间没有关连，即使调用者退出了，服务仍然运行。
    如果打算采用Context.startService()方法启动服务，在服务未被创建时，系统会先调用服务的onCreate()方法，接着调用onStart()方法。
    如果调用startService()方法前服务已经被创建，多次调用startService()方法并不会导致多次创建服务，但会导致多次调用onStart()方法。
    采用startService()方法启动的服务，只能调用Context.stopService()方法结束服务，服务结束时会调用onDestroy()方法。
    2. 使用bindService()方法启用服务，调用者与服务绑定在了一起，调用者一旦退出，服务也就终止，大有“不求同时生，必须同时死”的特点。
    onBind()只有采用Context.bindService()方法启动服务时才会回调该方法。该方法在调用者与服务绑定时被调用，当调用者与服务已经绑定，多次调用Context.bindService()方法并不会导致该方法被多次调用。
    采用Context.bindService()方法启动服务时只能调用onUnbind()方法解除调用者与服务解除，服务结束时会调用onDestroy()方法。
看看官方给出的比较流程示意图：
![&#20004;&#31181;&#27604;&#36739;](http://android.blog.51cto.com/attachment/201103/004355525.png)650) this.width=650;" align=middle border=0 isImg="true" http: blog.51cto.com viewpic.php?refimg="
 + this.src)" vmlBg="http://android.blog.51cto.com/attachment/201103/004355525.png" vmlInitiated="true">
    官方文档告诉我们，一个service可以同时start并且bind，在这样的情况，系统会一直保持service的运行状态如果service已经start了或者BIND_AUTO_CREATE标志被设置。如果没有一个条件满足，那么系统将会调用onDestory方法来终止service.所有的清理工作（终止线程，反注册接收器）都在onDestory中完成。
**拥有service的进程具有较高的优先级**
    官方文档告诉我们，Android系统会尽量保持拥有service的进程运行，只要在该service已经被启动(start)或者客户端连接(bindService)到它。当内存不足时，需要保持，拥有service的进程具有较高的优先级。
1． 如果service正在调用onCreate,onStartCommand或者onDestory方法，那么用于当前service的进程则变为前台进程以避免被killed。
2． 如果当前service已经被启动(start)，拥有它的进程则比那些用户可见的进程优先级低一些，但是比那些不可见的进程更重要，这就意味着service一般不会被killed.
3． 如果客户端已经连接到service (bindService),那么拥有Service的进程则拥有最高的优先级，可以认为service是可见的。
4． 如果service可以使用startForeground(int, Notification)方法来将service设置为前台状态，那么系统就认为是对用户可见的，并不会在内存不足时killed。
如果有其他的应用组件作为Service,Activity等运行在相同的进程中，那么将会增加该进程的重要性。
**本地service**
**1.不需和Activity交互的本地服务**
publicclass LocalService
extends Service {
privatestaticfinal String TAG = 
"LocalService"; 
        @Override 
public IBinder onBind(Intent intent) {
                Log.i(TAG, "onBind"); 
return
null; 
        } 
        @Override 
publicvoid onCreate() {
                Log.i(TAG, "onCreate"); 
super.onCreate(); 
        } 
        @Override 
publicvoid onDestroy() {
                Log.i(TAG, "onDestroy"); 
super.onDestroy(); 
        } 
        @Override 
publicvoid onStart(Intent intent,
int startId) { 
                Log.i(TAG, "onStart"); 
super.onStart(intent, startId);
        } 
}
Activity:
publicclass ServiceActivity
extends Activity { 
        @Override 
protected
void onCreate(Bundle savedInstanceState) { 
super.onCreate(savedInstanceState);
                setContentView(R.layout.servicedemo); 
                ((Button) findViewById(R.id.startLocalService)).setOnClickListener(
new View.OnClickListener(){
                                        @Override 
publicvoid onClick(View view) { 
// TODO Auto-generated method stub
startService(new Intent("com.demo.SERVICE_DEMO"));
                                        }  
                                }); 
                ((Button) findViewById(R.id.stopLocalService)).setOnClickListener(
new View.OnClickListener(){
                                        @Override 
publicvoid onClick(View view) { 
// TODO Auto-generated method stub
stopService(new Intent("com.demo.SERVICE_DEMO"));
                                        } 
                                }); 
        } 
}
在AndroidManifest.xml添加：
<service
android:name=".LocalService">
<intent-filter>
<actionandroid:name="com.demo.SERVICE_DEMO"/>
<category
android:name="android.intent.category.default"/>
</intent-filter>
</service>
否则启动服务时会提示new Intent找不到"com.demo.SERVICE_DEMO"。
    对于这类不需和Activity交互的本地服务，是使用startService/stopService的最好例子。
    运行时可以发现第一次startService时，会调用onCreate和onStart，在没有stopService前，无论点击多少次startService，都只会调用onStart。而stopService时调用onDestroy。再次点击stopService，会发现不会进入service的生命周期的，即不会再调用onCreate，onStart和onDestroy。
    而onBind在startService/stopService中没有调用。
**2.本地服务和Activity交互**
    对于这种case，官方的sample(APIDemo\app.LocalService)是最好的例子:
/** 
* This is an example of implementing an application service that runs locally 
* in the same process as the application.    The {@link LocalServiceController} 
* and {@link LocalServiceBinding} classes show how to interact with the 
* service. 
* 
* <p>Notice the use of the {@link NotificationManager} when interesting things 
* happen in the service.    This is generally how background services should 
* interact with the user, rather than doing something more disruptive such as 
* calling startActivity(). 
*/
publicclass LocalService
extends Service { 
private NotificationManager mNM; 
/** 
         * Class for clients to access.    Because we know this service always 
         * runs in the same process as its clients, we don't need to deal with 
         * IPC. 
         */
publicclass LocalBinder 
extends Binder {
LocalService getService() {
return LocalService.this;
                } 
        } 
        @Override 
publicvoid onCreate() {
                mNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
// Display a notification about us starting.    We put an icon in the status bar.
                showNotification(); 
        } 
        @Override 
publicint onStartCommand(Intent intent,
int flags, int startId) {
                Log.i("LocalService", 
"Received start id " + startId + ": " + intent);
// We want this service to continue running until it is explicitly
// stopped, so return sticky.
return START_STICKY; 
        } 
        @Override 
publicvoid onDestroy() {
// Cancel the persistent notification.
                mNM.cancel(R.string.local_service_started); 
// Tell the user we stopped.
                Toast.makeText(this, R.string.local_service_stopped, Toast.LENGTH_SHORT).show();
        } 
        @Override 
        public IBinder onBind(Intent intent) {
             return mBinder;
        } 
// This is the object that receives interactions from clients.    See
// RemoteService for a more complete example.
privatefinal IBinder mBinder = 
new LocalBinder();
/** 
         * Show a notification while this service is running. 
         */
privatevoid showNotification() {
// In this sample, we'll use the same text for the ticker and the expanded notification
                CharSequence text = getText(R.string.local_service_started); 
// Set the icon, scrolling text and timestamp
                Notification notification = new Notification(R.drawable.stat_sample, text,
                                System.currentTimeMillis()); 
// The PendingIntent to launch our activity if the user selects this notification
                PendingIntent contentIntent = PendingIntent.getActivity(this, 0,
new Intent(this, LocalServiceController.class), 0);
// Set the info for the views that show in the notification panel.
                notification.setLatestEventInfo(this, getText(R.string.local_service_label),
                                             text, contentIntent); 
// Send the notification.
// We use a layout id because it is a unique number.    We use it later to cancel.
                mNM.notify(R.string.local_service_started, notification); 
        } 
}
   这里可以发现onBind需要返回一个IBinder对象。也就是说和上一例子LocalService不同的是，
1. 添加了一个public内部类继承Binder，并添加getService方法来返回当前的Service对象；
2. 新建一个IBinder对象——new那个Binder内部类；
3. onBind方法返还那个IBinder对象。
Activity:
/** 
* <p>Example of binding and unbinding to the {@link LocalService}. 
* This demonstrates the implementation of a service which the client will 
* bind to, receiving an object through which it can communicate with the service.</p>
*/
publicclass LocalServiceBinding
extends Activity { 
privateboolean mIsBound;
private LocalService mBoundService;
        @Override 
      protected
void onCreate(Bundle savedInstanceState) { 
super.onCreate(savedInstanceState);
                setContentView(R.layout.local_service_binding); 
// Watch for button clicks.
                Button button = (Button)findViewById(R.id.bind); 
                button.setOnClickListener(mBindListener); 
                button = (Button)findViewById(R.id.unbind); 
                button.setOnClickListener(mUnbindListener); 
        } 
private ServiceConnection mConnection =
new ServiceConnection() { 
public void onServiceConnected(ComponentName className, IBinder service) {
// This is called when the connection with the service has been
// established, giving us the service object we can use to
// interact with the service.    Because we have bound to a explicit
// service that we know is running in our own process, we can
// cast its IBinder to a concrete class and directly access it.
                        mBoundService = ((LocalService.LocalBinder)service).getService();
// Tell the user about this for our demo.
                        Toast.makeText(LocalServiceBinding.this, R.string.local_service_connected,
                                        Toast.LENGTH_SHORT).show(); 
                } 
public void onServiceDisconnected(ComponentName className) { 
// This is called when the connection with the service has been
// unexpectedly disconnected -- that is, its process crashed.
// Because it is running in our same process, we should never
// see this happen.
                        mBoundService = null; 
                        Toast.makeText(LocalServiceBinding.this, R.string.local_service_disconnected,
                                        Toast.LENGTH_SHORT).show(); 
                } 
        }; 
private OnClickListener mBindListener =
new OnClickListener() { 
public
void onClick(View v) { 
// Establish a connection with the service.    We use an explicit
// class name because we want a specific service implementation that
// we know will be running in our own process (and thus won't be
// supporting component replacement by other applications).
bindService(new Intent(LocalServiceBinding.this,    
                                        LocalService.class),
mConnection, Context.BIND_AUTO_CREATE);
                        mIsBound = true; 
                } 
        }; 
private OnClickListener mUnbindListener =
new OnClickListener() { 
public
void onClick(View v) { 
if (mIsBound) { 
// Detach our existing connection.
unbindService(mConnection);
                                mIsBound = false;
                        } 
                } 
        }; 
}
    明显看出这里面添加了一个名为ServiceConnection类，并实现了onServiceConnected(从IBinder获取Service对象)和onServiceDisconnected(set Service to null)。
    而bindService和unbindService方法都是操作这个ServiceConnection对象的。
AndroidManifest.xml里添加:
<service
android:name=".app.LocalService"
/>
<activity
android:name=".app.LocalServiceBinding"android:label="@string/activity_local_service_binding">
 <intent-filter>
<actionandroid:name="android.intent.action.MAIN"/>
<categoryandroid:name="android.intent.category.SAMPLE_CODE"/>
</intent-filter>
</activity>
这里没什么特别的，因为service没有需要什么特别的action，所以只是声明service而已，而activity和普通的没差别。
运行时，发现调用次序是这样的：
**bindService:**
1.LocalService : onCreate
2.LocalService : onBind
3.Activity: onServiceConnected
**unbindService: 只是调用onDestroy**
可见，onStart是不会被调用的，而onServiceDisconnected没有调用的原因在上面代码的注释有说明。
**介绍onStartCommand()需要用到的几个常量 (引自官方文档)**
**START_NOT_STICKY**
If the system kills the service after onStartCommand() returns, *do not* recreate the service, unless there are pending intents to deliver. This is the safest option to avoid running your service when not necessary and when your application can simply restart any unfinished jobs.
- **START_STICKY**
If the system kills the service after onStartCommand() returns, recreate the service and call onStartCommand(), but
*do not* redeliver the last intent. Instead, the system calls onStartCommand() with a null intent, unless there were pending intents to start the service, in which case, those intents are delivered. This is suitable for media players (or similar services)
 that are not executing commands, but running indefinitely and waiting for a job.
- **START_REDELIVER_INTENT**
If the system kills the service after onStartCommand() returns, recreate the service and call onStartCommand() with the last intent that was delivered to the service. Any pending intents are delivered in turn. This is suitable for services that are actively
 performing a job that should be immediately resumed, such as do wnloading a file.
**Running a Service in the Foreground**
    具体内容查看官方文档，主要是使用 startForeground() 和 stopForeground()方法。
