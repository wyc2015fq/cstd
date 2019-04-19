# android service - xqhrs232的专栏 - CSDN博客
2011年03月12日 10:34:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：637标签：[android																[service																[application																[playback																[networking																[thread](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=networking&t=blog)](https://so.csdn.net/so/search/s.do?q=playback&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/wtao158/archive/2010/01/08/5149721.aspx](http://blog.csdn.net/wtao158/archive/2010/01/08/5149721.aspx)
作为一款多任务操作系统，如果不能运行后台服务，显然说不过去，Android 当然提供了运行后台程序的方法。而且非常简单易用，只不过有一些小问题需要注意，这个主题分为两部分，第一部分是如何实现一个Service以及他的生命周期，第二部分是对于一个个后台服务应该注意的事项。我们开始吧！
创建服务类
所谓的服务，在Android里被称做 Service，只要继承 android.app.Service 这个抽象类，并且实现其中几个方法就可以了。
public class RoidingService extends android.app.Service {}里边必须实现的一个方法是 onBind(Intent intent) ，他具体是做什么的我们下边讲。还有两个重要的回调函数需要覆盖，onCreate() 和 onDestroy()。跟 Actitivty 类似，在创建和销毁 Service 时回调这两个函数，达到初始化或退出前保存状态。
服务的生命周期
有了 Service 类我们如何启动他呢，有两种方法：
Context.startService() 
Context.bindService() 
在同一个应用任何地方调用 startService() 方法就能启动 Service 了，然后系统会回调 Service 类的 onCreate() 以及 onStart() 方法。这样启动的 Service 会一直运行在后台，直到 Context.stopService() 或者 selfStop() 方法被调用。另外如果一个 Service 已经被启动，其他代码再试图调用 startService() 方法，是不会执行 onCreate() 的，但会重新执行一次 onStart() 。
另外一种 bindService() 方法的意思是，把这个 Service 和调用 Service 的客户类绑起来，如果调用这个客户类被销毁，Service 也会被销毁。用这个方法的一个好处是，bindService() 方法执行后 Service 会回调上边提到的 onBind() 方发，你可以从这里返回一个实现了 IBind 接口的类，在客户端操作这个类就能和这个服务通信了，比如得到 Service 运行的状态或其他操作。如果 Service 还没有运行，使用这个方法启动 Service 就会 onCreate()
 方法而不会调用 onStart()。
与 Service 通信并且让它持续运行
如果我们想保持和 Service 的通信，又不想让 Service 随着 Activity 退出而退出呢？你可以先 startService() 然后再 bindService() 。当你不需要绑定的时候就执行 unbindService() 方法，执行这个方法只会触发 Service 的 onUnbind() 而不会把这个 Service 销毁。这样就可以既保持和 Service 的通信，也不会随着 Activity 销毁而销毁了。
提高 Service 优先级
Android 系统对于内存管理有自己的一套方法，为了保障系统有序稳定的运信，系统内部会自动分配，控制程序的内存使用。当系统觉得当前的资源非常有限的时候，为了保 证一些优先级高的程序能运行，就会杀掉一些他认为不重要的程序或者服务来释放内存。这样就能保证真正对用户有用的程序仍然再运行。如果你的 Service 碰上了这种情况，多半会先被杀掉。但如果你增加 Service 的优先级就能让他多留一会，我们可以用 setForeground(true) 来设置 Service 的优先级。
为什么是 foreground ? 默认启动的 Service 是被标记为 background，当前运行的 Activity 一般被标记为 foreground，也就是说你给 Service 设置了 foreground 那么他就和正在运行的 Activity 类似优先级得到了一定的提高。当让这并不能保证你得 Service 永远不被杀掉，只是提高了他的优先级。
有一个方法可以给你更清晰的演示，进入 $SDK/tools 运行命令
返回的一大堆东西，观察 oom_adj 的值，如果是大于 8 一般就是属于 backgroud 随时可能被干掉，数值越小证明优先级越高，被干掉的时间越晚。你看phone的程序是 -12 说明电话就是电话，其他什么都干了了，也的能接电话对吧。另外还有一个 -100 的，更邪乎因为是 system 如果他也完蛋了，你得系统也就挂了，嘿嘿。
用其他方式启动 Service
其实不光能从 Activity 中启动 Service ，还有一个很有用的方法是接收系统的广播，这就要用到 Receiver 。在 Mainfest 文件中配置你得 Receiver 能接收什么样的广播消息，那么即使你得程序没有显示给用户，你的 Service 也能启动。你要做的就是继承 android.content.BroadcastReceiver ，然后实现 onReceive(Context context, Intent intent) 方法，就可以启动你得 Service 了。这里不能 bindService
 因为一个 Receiver 是一个短暂存在的对象，所以 bind 是没有什么意义的。
资源消耗
大家都说 G1 的电池太不抗用，这个问题其实我看来跟多是软件的问题。1150毫安的电池不算大，但也不算小了，考虑到 500mhz 的 CPU 还是非常耗电的。因为一个 Service 要长时间后台运行，所以如果你得 Service 太过于消耗资源那电池更用不了多久了。
对于这个问题我有一点点考虑，和大家分享一下。因为一般 Service 都会启动另外的线程不断循环作一些操作，循环频率不易太高。也不要做太过于耗费资源的操作，特别是CPU资源，因为后台 Service 用户看不到，会比较莫名奇妙。具体可以结合 top 以及 logcat 监测使用情况。LOG中如果虚拟机频繁的 GC 应该也说明程序还有很大改进的余地。因为GC 也是很耗费CPU的。可能这些不光 Service 应该注意，只要是移动设备都应该考虑，才能给你的用户最佳的体验。
原文参见：[http://www.roiding.com/index.php/archives/187](http://www.roiding.com/index.php/archives/187)
A Service is an application component that runs in the background,not interacting with the user, for an indefinite period of time. Each service class must have a corresponding declaration in its package's AndroidManifest.xml. Services can be started with
 Context.startService() and Context.bindService(). Note that services, like other application objects, run in the main thread of their hosting process. This means that, if your service is going to do any CPU intensive (such as MP3 playback) or blocking (such
 as networking) operations, it should spawn its own thread in which to do that work.
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wtao158/archive/2010/01/08/5149721.aspx](http://blog.csdn.net/wtao158/archive/2010/01/08/5149721.aspx)

