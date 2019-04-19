# RSS Reader实例开发之使用Service组件 - xqhrs232的专栏 - CSDN博客
2011年03月14日 15:06:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：598标签：[service																[rss																[ophone																[listview																[string																[timer](https://so.csdn.net/so/search/s.do?q=timer&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=listview&t=blog)](https://so.csdn.net/so/search/s.do?q=ophone&t=blog)](https://so.csdn.net/so/search/s.do?q=rss&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.ophonesdn.com/article/show/122](http://www.ophonesdn.com/article/show/122)
   到目前为止，我们已经实现了RSS Reader的基本功能，在这个OPhone应用程序中，我们使用Activity作为UI界面，使用SQLite数据库并封装为ContentProvider实现数据存储和查询。为了更进一步地优化RSS Reader应用程序的设计，我们将使用OPhone系统提供的另一种重要的组件——Service来封装RSS Reader的逻辑，使应用程序的结构更加清晰。
**使用Service组件**        Service组件是OPhone系统中定义的一类没有界面，在后台运行并提供服务的组件。例如，音乐播放器就使用了Service组件在后台播放音乐，这样，即使用户关闭了前台的Activity，也可以继续播放音乐。
使用Service组件的另一个好处是将应用程序的逻辑全部移到Service组件中，这样，Activity只需要把注意力放在UI逻辑上，通过调用Service组件，Activity不必关心业务逻辑。
       下面，我们就把RSS Reader的联网、XML解析、数据存取等复杂逻辑从Activity移到Service里。
要编写一个Service组件相当容易，从android.app.Service派生一个实现类即可：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- publicclass ReadingService extends Service {   
-     ...   
- }  
```java
public class ReadingService extends Service { ... }
```
       Service组件是由系统或Activity启动的，其生命周期主要对应onCreate()、onStart()和onDestroy()三个方法。Service组件被创建时，onCreate()方法被调用，这里可以编写初始化代码，每当Activity请求启动一个Service组件时，onStart()方法被调用，最后，当系统销毁Service组件时，onDestroy()方法被调用，这里可以编写清理资源的代码。
        需要注意的是，onStart()方法可能被多次调用，因此，只需初始化一次的代码需要放到onCreate()而不是onStart()方法。然后，我们就可以向ReadingService中添加若干公共方法：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- public BriefSubscription addSubscription(String url) { ... }   
- publicint getPreferenceOfExpires() { ... }   
- publicint getPreferenceOfFreq() { ... }   
- publicboolean getPreferenceOfUnreadOnly() { ... }   
- publicvoid markRead(long item_id) { ... }   
- publicvoid markUnread(long item_id) { ... }   
- public List<BriefItem> queryBriefItems(long sub_id, boolean unreadOnly) { ... }   
- public List<BriefSubscription> queryBriefSubscriptions() { ... }   
- publicvoid removeSubscription(String sub_id) { ... }   
- publicvoid storePreferences(boolean unreadOnly, int freq, int expires) { ... }  
```java
public BriefSubscription addSubscription(String url) { ... } public int getPreferenceOfExpires() { ... } public int getPreferenceOfFreq() { ... } public boolean getPreferenceOfUnreadOnly() { ... } public void markRead(long item_id) { ... } public void markUnread(long item_id) { ... } public List<BriefItem> queryBriefItems(long sub_id, boolean unreadOnly) { ... } public List<BriefSubscription> queryBriefSubscriptions() { ... } public void removeSubscription(String sub_id) { ... } public void storePreferences(boolean unreadOnly, int freq, int expires) { ... }
```
       把Activity中的相关逻辑代码移至相应的方法中即可。
         此外，Service组件也支持消息处理，因此，多线程和任务调度相关的逻辑也从MainActivity中移至ReadingService中，并添加删除过期Item的逻辑：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- privatefinal Handler handler = new Handler() {   
- @Override
- publicvoid handleMessage(Message msg) {   
- switch (msg.what) {   
- case MSG_TIMER:   
-             log.info("Message: MSG_TIMER");   
-             removeExpires();   
-             refreshFeeds();   
- break;   
-         }   
-     }   
- };  
```java
private final Handler handler = new Handler() { @Override public void handleMessage(Message msg) { switch (msg.what) { case MSG_TIMER: log.info("Message: MSG_TIMER"); removeExpires(); refreshFeeds(); break; } } };
```
       现在，应用程序的逻辑已经完全移至ReadingService中。下一步，我们需要在AndroidManifest.xml中添加ReadingService的声明：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- <service android:enabled="true" android:name=".service.ReadingService">   
- </service>  
```java
<service android:enabled="true" android:name=".service.ReadingService"> </service>
```
注意：Service的Class全名由AndroidManifest.xml中声明的package名称“org.expressme.wireless.reader”和Service的android:name组合而成。
**启动Service**
        Service的启动是通过Activity的startService()方法实现的，同样需要一个Intent实例：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {   
- super.onCreate(savedInstanceState);   
-     ...   
-     Intent intent = new Intent(this, ReadingService.class);   
-     ComponentName service = startService(intent);   
- }  
```java
@Override public void onCreate(Bundle savedInstanceState) { super.onCreate(savedInstanceState); ... Intent intent = new Intent(this, ReadingService.class); ComponentName service = startService(intent); }
```
        Activity无需知道Service当前是否已经启动。如果Service还没有启动，OPhone系统会创建Service，调用其onCreate()方法，再调用其onStart()方法。如果Service已经正在运行，OPhone系统会调用其onStart()方法，由于onStart()方法可能被多次调用，因此，Service组件要维护自己的内部状态，防止在onStart()方法中多次初始化。
**停止Service**
         停止Service与启动Service类似，也需要构造一个Intent实例，然后，通过stopService()方法停止Service：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- @Override
- protectedvoid onDestroy() {   
- super.onDestroy();   
-     Intent intent = new Intent(this, service.getClass());   
-     stopService(intent);   
- }  
```java
@Override protected void onDestroy() { super.onDestroy(); Intent intent = new Intent(this, service.getClass()); stopService(intent); }
```
       停止Service的方法一般由生命周期最长的Activity在其onDestroy()方法中调用，这样，Activity被销毁时，Service就停止了，能够及时释放系统资源。
**与Activity通信**         仅仅是启动和停止Service还远远不够。细心的读者可能发现了，启动ReadingService时，返回的不是ReadingService类的引用，而是ComponentName的实例。那么，我们在ReadingService中定义了若干个public方法，如何才能在Activity中调用呢？
          在OPhone系统中，要调用Service的public方法，需要通过Binder机制来实现，首先，Service组件本身要实现Binder机制，然后，Activity才能通过Binder连接到Service组件，并调用其public方法。
          因此，第一步是给ReadingService添加Binder支持。在ReadingService内部添加一个ReadingBinder的内部类声明，添加getService()方法并返回ReadingService的当前实例，然后，实例化并持有一个Binder的引用：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- publicclass ReadingBinder extends Binder {   
- public ReadingService getService() {   
- return ReadingService.this;   
-     }   
- }   
- privatefinal IBinder binder = new ReadingBinder();  
```java
public class ReadingBinder extends Binder { public ReadingService getService() { return ReadingService.this; } } private final IBinder binder = new ReadingBinder();
```
       下一步，覆写onBind()方法，返回binder实例：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- @Override
- public IBinder onBind(Intent intent) {   
- return binder;   
- }  
```java
@Override public IBinder onBind(Intent intent) { return binder; }
```
       现在，ReadingService组件就实现了Binder机制，下面，我们需要在Activity中添加一点代码，通过bindService()方法来绑定ReadingService的实例：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- // bind service: 
- Intent bindIntent = new Intent(this, ReadingService.class);   
- bindService(bindIntent, serviceConnection, Context.BIND_AUTO_CREATE);  
```java
// bind service: Intent bindIntent = new Intent(this, ReadingService.class); bindService(bindIntent, serviceConnection, Context.BIND_AUTO_CREATE);
```
        没错！绑定一个Service也是通过Intent完成的，同时需要提供一个ServiceConnection回调接口，用于接收Bind事件：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- private ServiceConnection serviceConnection = new ServiceConnection() {   
- publicvoid onServiceConnected(ComponentName className, IBinder service) {   
-             serviceBinder = ((ReadingService.ReadingBinder)service).getService();   
-             init();   
- ;   
-         }   
- publicvoid onServiceDisconnected(ComponentName className) {   
-             serviceBinder = null;   
-         }   
- };  
```java
private ServiceConnection serviceConnection = new ServiceConnection() { public void onServiceConnected(ComponentName className, IBinder service) { serviceBinder = ((ReadingService.ReadingBinder)service).getService(); init(); ; } public void onServiceDisconnected(ComponentName className) { serviceBinder = null; } };
```
        ServiceConnection回调接口用于接收Connected和Disconnected事件，请注意，bindService()方法是异步执行的，即bindService()返回后，并不能立刻获取到Service的实例，必须响应onServiceConnected()事件，在这个事件中获取Service的实例，然后执行一些初始化方法。
         绑定Service后，Activity就获得了Service实例的引用，我们将其保存在成员变量中，然后，在Activity的生命周期中，就可以随时调用Service的public业务方法了。
Activity结束时，还必须及时取消对Service的绑定，通过unbindService()方法实现：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- unbindService(serviceConnection);  
```java
unbindService(serviceConnection);
```
       对Service的绑定和取消应该分别对应Activity的onCreate()和onDestroy()事件，这样，能够保证Activity正确释放引用的资源。
**使用Broadcast广播**
        在Activity中调用Service的public方法很容易，例如，我们通过调用refresh()方法就可以请求ReadingService组件在后台开启新的异步任务来获取最新的RSS。当ReadingService获得了最新的RSS内容并写入数据库后，如何通知前台的MainActivity刷新当前显示的ListView呢？
直接调用MainActivity的某个notifyChanged()方法可不好，因为ReadingService很难获得MainActivity的引用，即使获得了，ReadingService不是运行在系统API层的，无法掌控MainActivity的状态，如果MainActivity已经处于销毁状态，则刷新UI可能引发应用程序崩溃。
        此外，直接调用还导致两个组件的紧密耦合，将来如果有其他Activity也需要得到该通知的话，则还需添加更多的代码，导致更紧密的耦合。
理想状态下，ReadingService应该只负责发出通知，不知道也不关心谁会接收到该消息，而MainActivity则应该只负责接收该通知，不知道也不关心谁发出的消息，这样，通过典型的Observer模式实现的广播，就可以让各个组件保持松耦合，还可以动态地加入接收者。
**发送广播**       OPhone系统已经提供了Observer模式的实现，即使用Broadcast广播一个Intent。下面，我们通过Broadcast机制来实现ReadingService和MainActivity之间的异步消息发送和接收的功能。
首先，我们需要定义ReadingService能够发出的消息类型，目前，RSS Reader应用一共支持以下3种消息类型：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- // 有新的RSS项: 
- publicstaticfinal String NOTIFY_NEW_ITEMS = ReadingService.class.getName() + ".NOTIFY_NEW_ITEMS";
   
- // 用户设置已更改: 
- publicstaticfinal String NOTIFY_PREF_CHANGED = ReadingService.class.getName() + ".NOTIFY_PREF_CHANGED";
   
- // 用户删除了一个订阅: 
- publicstaticfinal String NOTIFY_SUB_REMOVED = ReadingService.class.getName() + ".NOTIFY_SUB_REMOVED";  
```java
// 有新的RSS项: public static final String NOTIFY_NEW_ITEMS = ReadingService.class.getName() + ".NOTIFY_NEW_ITEMS"; // 用户设置已更改: public static final String NOTIFY_PREF_CHANGED = ReadingService.class.getName() + ".NOTIFY_PREF_CHANGED"; // 用户删除了一个订阅: public static final String NOTIFY_SUB_REMOVED = ReadingService.class.getName() + ".NOTIFY_SUB_REMOVED";
```
      注意到消息类型是String类型，因此，为了确保全局唯一，我们使用ReadingService的完整类名+自定义消息名称。
        现在，ReadingService可以在合适的时候发出通知消息。例如，当用户修改了设置后，ReadingService将首先保存用户设置，然后，发出NOTIFY_PERF_CHANGED消息：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- Intent intent = new Intent(NOTIFY_PREF_CHANGED);   
- sendBroadcast(intent);  
```java
Intent intent = new Intent(NOTIFY_PREF_CHANGED); sendBroadcast(intent);
```
       使用sendBroadcast()方法就可以发出广播消息，该方法定义在android.content.Context接口中，Service和Activity均继承并实现了该方法。
        如果我们希望能在消息中再附带一点数据，则需要将需要携带的数据放入Intent中，通过Intent的putExtra()方法可以放入String、int、boolean等常见数据类型，例如，当发现新的RSS项后，ReadingService将发送NOTIFY_NEW_ITEMS消息，并同时附上Subscription的ID值：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- Intent intent = new Intent(NOTIFY_NEW_ITEMS);   
- intent.putExtra(SubscriptionColumns._ID, sub_id);   
- sendBroadcast(intent);  
```java
Intent intent = new Intent(NOTIFY_NEW_ITEMS); intent.putExtra(SubscriptionColumns._ID, sub_id); sendBroadcast(intent);
```
**接收广播**        现在，ReadingService已经能够发出广播了，下一步需要做的，就是让MainActivity能够接收广播。
        要接收一个广播，首先需要创建一个BroadcastReceiver的实例，并覆写onReceive()方法用于处理广播：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- privatefinal BroadcastReceiver newItemsReceiver = new BroadcastReceiver() {   
- @Override
- publicvoid onReceive(Context context, Intent intent) {   
- // TODO... 
-     }   
- };  
```java
private final BroadcastReceiver newItemsReceiver = new BroadcastReceiver() { @Override public void onReceive(Context context, Intent intent) { // TODO... } };
```
     建议将BroadcastReceiver的实例定义为final类型。
       然后，在Activity的onCreate()方法中注册BroadcastReceiver的实例，以便能够接收到广播消息：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {   
- super.onCreate(savedInstanceState);   
-     ...   
- // 注册: 
-     IntentFilter filter = new IntentFilter(ReadingService.NOTIFY_NEW_ITEMS);   
-     registerReceiver(this.newItemsReceiver, filter);   
- }  
```java
@Override public void onCreate(Bundle savedInstanceState) { super.onCreate(savedInstanceState); ... // 注册: IntentFilter filter = new IntentFilter(ReadingService.NOTIFY_NEW_ITEMS); registerReceiver(this.newItemsReceiver, filter); }
```
     注意到registerReceiver方法除了传入BroadcastReceiver的实例外，还需要一个IntentFilter。顾名思义，IntentFilter就是根据消息类型来过滤接收到的Intent的。例如，上述代码指定的IntentFilter将过滤掉除NOTIFY_NEW_ITEMS之外的其他所有Intent，这样，该BroadcastReceiver接收到的广播消息就全部是NOTIFY_NEW_ITEMS，没有必要再根据Intent.getAction()来判断了。
       最后，不要忘记在Activity的onDestroy()方法中取消已注册的BroadcaseReceiver：
[view plain](http://www.ophonesdn.com/article/show/122#)[copy to clipboard](http://www.ophonesdn.com/article/show/122#)[print](http://www.ophonesdn.com/article/show/122#)[?](http://www.ophonesdn.com/article/show/122#)
- @Override
- protectedvoid onDestroy() {   
- super.onDestroy();   
-     unregisterReceiver(this.newItemsReceiver);   
- }  
```java
@Override protected void onDestroy() { super.onDestroy(); unregisterReceiver(this.newItemsReceiver); }
```
       有了Broadcast机制，我们就最大限度地分离了Service和Activity之间的通信逻辑，现在，RSS Reader已经在ReadingService中实现了广播，而MainActivity则接收广播消息。当MainActivity接收到NOTIFY_NEW_ITEMS后，菜单项Refresh将被设置为enable状态，用户就可以点击Refresh选项菜单来刷新ListView。
**关于作者**
        廖雪峰，精通Java/Java EE/Java ME/Android/Python/C#/Visual Basic开发，对开源框架有深入研究，著有《Spring 2.0核心技术与最佳实践》一书，其官方博客是[http://www.liaoxuefeng.com/](http://www.liaoxuefeng.com/)和[http://michael-liao.appspot.com/](http://michael-liao.appspot.com/)，可以通过[askxuefeng@gmail.com](mailto:askxuefeng@gmail.com)与之联系。

