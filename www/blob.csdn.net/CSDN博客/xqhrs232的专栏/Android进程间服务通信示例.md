# Android进程间服务通信示例 - xqhrs232的专栏 - CSDN博客
2011年03月14日 11:31:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：616
原文地址::[http://codroider.net/forum.php?mod=viewthread&tid=276&page=1](http://codroider.net/forum.php?mod=viewthread&tid=276&page=1)
两个进程间的Service需要进行通信，则需要把对象序列化后进行互相发送。
Android提供了一个 AIDL (Android接口定义语言)工具来处理序列化和通信。这种情况下Service需要以aidl文件的方式提供服务接口，AIDL工具将生成一个相应的java接口，并且在生成的服务接口中包含一个功能调用的stub服务桩类。Service的实现类需要去继承这个 stub服务桩类。Service的onBind方**返回实现类的对象，之后你就可以使用它了.现在来实现一个HelloWorld级的服务以及在Activity中使用这个服务。步骤如下:
1.编写aidl文件,在这个文件中提供了服务所提供的方法的接口,文件的扩展名为aidl,在我们的示例中命名为IDataTimeService.aidl文件的内容为如下所示:
JAVA代码：
- package com.services.demo2; 
- // Declare the interface. 
- interface IDataTimeService { 
- String getCurrentDataTime(); 
- } 
*复制代码*
2.编写服务的实现类,在这个示例中为DataTimeService.java,内容如下:
JAVA代码：
- public class DataTimeService extends Service{ 
- public static final String BROADCAST_ACTION = "com.services.demo2.DataTimeServiceEvent";
- /** 
- 使用上面定义的字符串常量创建一个Intent的实例,如果服务在运行的过程中发生的事件都可以通过Service的 
- sendBroadcast(broadcast)方法来发送一个广播消息通知Activity,然后有Activity注册的接收者进行处理; 
- */ 
- private Intent broadcast = new Intent(BROADCAST_ACTION); 
- sendBroadcast(broadcast); 
- private final IDataTimeService.Stub binder = new IDataTimeService.Stub() { 
- public String getCurrentDataTime() { 
- return (getCurrentDataTimeImpl()); 
- } 
- }; 
- synchronized private String getCurrentDataTimeImpl() { 
- Date date=new Date(); 
- return (date.toLocaleString()); 
- } 
- @Override 
- public IBinder onBind(Intent intent) { 
- return (binder); 
- } 
- } 
*复制代码*
3.现在编写一个Activity,来消费这个服务:
JAVA代码：
- public class ServicesDemo2 extends Activity { 
- private IDataTimeService service = null; 
- private Intent serviceIntent = null; 
- private WebView browser; 
- private ServiceConnection svcConn = new ServiceConnection() { 
- public void onServiceConnected(ComponentName className, IBinder binder) { 
- service = IDataTimeService.Stub.asInterface(binder); 
- 
- browser.postDelayed(new Runnable() { 
- public void run() { 
- updateForecast(); 
- } 
- }, 1000); 
- } 
- 
- public void onServiceDisconnected(ComponentName className) { 
- service = null; 
- } 
- }; 
- 
- @Override 
- public void onCreate(Bundle savedInstanceState) { 
- super.onCreate(savedInstanceState); 
- setContentView(R.layout.main); 
- 
- browser = (WebView) findViewById(R.id.webkit); 
- serviceIntent = new Intent(this, DataTimeServiceImpl.class); 
- //绑定一个服务 
- bindService(serviceIntent, svcConn, BIND_AUTO_CREATE); 
- } 
- 
- @Override 
- public void onResume() { 
- super.onResume(); 
- /** 
- 注册广播消息的接收者,在android中服务与Activity的通信依靠广播发送消息,然后由Activity注册的接收者 
- 接收到消息,进行相应的处理这里使用DataTimeServiceImpl.BROADCAST_ACTION来过滤,表明它只接收 
- DataTimeService中的调用sendBroadcast(broadcast)发送的广播消息; 
- */
- registerReceiver(receiver, new IntentFilter( 
- DataTimeServiceImpl.BROADCAST_ACTION)); 
- } 
- 
- @Override 
- public void onPause() { 
- super.onPause(); 
- unregisterReceiver(receiver); 
- } 
- 
- @Override 
- public void onDestroy() { 
- super.onDestroy(); 
- unbindService(svcConn); 
- } 
- 
- private void updateForecast() { 
- try { 
- String page = service.getForecastPage(); 
- if (page != null) { 
- browser.loadDataWithBaseURL(null, page, "text/html", "UTF-8", 
- null); 
- browser.postDelayed(new Runnable() { 
- public void run() { 
- updateForecast(); 
- } 
- }, 1000); 
- } 
- } catch (final Throwable t) { 
- svcConn.onServiceDisconnected(null); 
- } 
- } 
- 
- 
- //接收者实例 
- private BroadcastReceiver receiver = new BroadcastReceiver() { 
- public void onReceive(Context context, Intent intent) { 
- runOnUiThread(new Runnable() { 
- public void run() { 
- updateForecast(); 
- } 
- }); 
- } 
- }; 
- } 
*复制代码*
