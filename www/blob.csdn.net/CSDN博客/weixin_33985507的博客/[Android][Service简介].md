# [Android][Service简介] - weixin_33985507的博客 - CSDN博客
2017年11月06日 14:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
# 1.什么是Service
Service 是一个可以在后台执行长时间运行操作而不提供用户界面的应用组件。服务可由其他应用组件启动，而且即使用户切换到其他应用，服务仍将在后台继续运行。 此外，组件可以绑定到服务，以与之进行交互，甚至是执行进程间通信 (IPC)。上面的这句话是官方的解释，这句化的第一句很明显的阐述了Service的使用场景，也就是说在不需要与用户进行交互（用户点击屏幕等触发事件的行为都是交互行为）的情景，以及某个操作需要在后台长时间运行的情况。只要满足这两个情况之一，都可以考虑使用Service。
此外，必须注意的是Service必须要被其他组件（Activity等组件）启动才能运行。
```
Intent mIntent = new Intent(getApplicationContext(), AboutService.class);
getApplicationContext().startService(mIntent);
```
但是，我测试通过ApplicationContext启动服务，它也确实能够启动，这里冒昧下个结论，只要有Context的地方，都可以通过Context启动服务。
服务在其托管进程的主线程中运行，它既不创建自己的线程，也不在单独的进程中运行（除非另行指定）。 这意味着，如果服务将执行任何 CPU 密集型工作或阻止性操作（例如 MP3 播放或联网）（可以理解为耗时的操作），则应在服务内创建新线程来完成这项工作。通过使用单独的线程，可以降低发生“应用无响应”(ANR) 错误的风险，而应用的主线程仍可继续专注于运行用户与 Activity 之间的交互。其实这句话可以简单的理解为服务是在主线程中运行，因此如果服务中有耗时任务应该放到子线程中去执行。
# 2.Service按启动方式分类
|类别|区别|注意|
|----|----|----|
|startService|当应用组件（如 Activity）通过调用 startService() 启动服务时，服务即处于“启动”状态。一旦启动，服务即可在后台无限期运行，即使启动服务的组件已被销毁也不受影响。 已启动的服务通常是执行单一操作，而且不会将结果返回给调用方。例如，它可能通过网络下载或上传文件。 操作完成后，服务会自行停止运行。|1. 因为这种方式启动的服务一旦启动即可在后台无限期运行，即使启动服务的组件被销毁了也不会影响这个服务,所以为了不引起内存溢出等问题，不要忘了在不需要该服务的时候要记得停止该服务。服务可以使用 stopSelf() 自行停止运行，或由其他组件通过调用 stopService() 停止。2.通过startService启动的服务是无法与其他组件进行交互的。3.通过startService启动的服务，是有可能被系统杀死的。仅当内存过低且必须回收系统资源以供具有用户焦点的 Activity 使用时，Android 系统才会强制停止服务。4.如果服务已启动并要长时间运行，则系统会随着时间的推移降低服务在后台任务列表中的位置，而服务也将随之变得非常容易被终止；如果服务是启动服务，则您必须将其设计为能够妥善处理系统对它的重启。 如果系统终止服务，那么一旦资源变得再次可用，系统便会重启服务（不过这还取决于从 onStartCommand() 返回的值）。|
|bindService|当应用组件通过调用 bindService() 绑定到服务时，服务即处于“绑定”状态。绑定服务提供了一个客户端-服务器接口，允许组件与服务进行交互、发送请求、获取结果，甚至是利用进程间通信 (IPC) 跨进程执行这些操作。 仅当与另一个应用组件绑定时，绑定服务才会运行。 多个组件可以同时绑定到该服务，但全部取消绑定后，该服务即会被销毁。|1.仅当与另一个应用组件绑定时，绑定服务才会运行.也就是说通过bindService启动服务必须要绑定一个应用组件才能运行。而且当该被绑定的组件销毁的时候，该服务也会被销毁。2.通过bindService方式启动的服务是可以与其他组件进行交互的。如需与 Activity 和其他应用组件中的服务进行交互，或者需要通过进程间通信 (IPC) 向其他应用公开某些应用功能，则应创建绑定服务。3.仅当内存过低且必须回收系统资源以供具有用户焦点的 Activity 使用时，Android 系统才会强制停止服务。如果将服务绑定到具有用户焦点的 Activity，则它不太可能会终止；如果将服务声明为在前台运行，则它几乎永远不会终止。4.从 Android 5.0（API 级别 21）开始，如果使用隐式 Intent 调用 bindService()，系统会引发异常。这是官方的原话，但是经过测试不管是bindService()还是startService()，只要是隐式启动的Intent没有调用setPackage设置app的包名，都会报错。5.多个客户端可同时绑定到一个服务。不过，只有在第一个客户端绑定时，系统才会调用服务的 onBind()方法来检索 IBinder。系统随后无需再次调用 onBind()，便可将同一 IBinder 传递至任何其他绑定的客户端。|
为了确保应用的安全性，请始终使用显式 Intent 启动或绑定 Service，且不要为服务声明 Intent 过滤器。启动哪个服务存在一定的不确定性，而如果对这种不确定性的考量非常有必要，则可为服务提供 Intent 过滤器并从 Intent 中排除相应的组件名称，但随后必须使用 setPackage() 方法设置 Intent 的软件包(setPackage设置的是app的包名，而不是该服务的路径)，这样可以充分消除目标服务的不确定性。这句话的意思是为了确保安全，要使用显示Intent的方法来启动或者绑定服务，而且一般不要为服务声明Intent过滤器。（一般隐式启动服务是需要使用到Intent过滤器设置action）使用隐式 Intent 启动服务存在安全隐患（如果未设置android:exported="false"那么其他应用可以调用该应用的服务，这确实不安全），因为无法确定哪些服务将响应 Intent，且用户无法看到哪些服务已启动（这里在filter里的例子测试了一下，在AndroidManifest将两个服务的过滤器里的action设置为相同的，多次测试结果是只有先设置过滤的服务会被启动，而另一个服务未被启动。但是这句话的意思是不能确定哪个服务被启动，所以不清楚是不是先设置过滤的服务一定会被启动，不清楚是不是设置了相同Action的服务只能有一个被启动）。隐式启动一点要设置setPackage() 否则会报错。
```
//通过ApplicationContext来启动服务
//        IllegalArgumentException: Service Intent must be explicit
//        经过查找相关资料，发现是因为Android5.0中service的intent一定要显性声明，当这样绑定的时候不会报错。
//  隐式启动
//        Intent mIntent = new Intent();
//        mIntent.setAction("xxx");
//  startService(mIntent);
        //显示启动
        Intent mIntent = new Intent(this, AboutService.class);
        startService(mIntent);
```
# 3.服务的生命周期
![3831664-cdbe74fbc0be6c3b.png](https://upload-images.jianshu.io/upload_images/3831664-cdbe74fbc0be6c3b.png)
服务生命周期。左图显示了使用 startService() 所创建的服务的生命周期，右图显示了使用 bindService() 所创建的服务的生命周期。
服务生命周期（从创建到销毁）可以遵循两条不同的路径：
•   启动服务
该服务在其他组件调用 startService() 时创建，然后无限期运行，且必须通过调用 stopSelf() 来自行停止运行。此外，其他组件也可以通过调用stopService() 来停止服务。服务停止后，系统会将其销毁。
•   绑定服务
该服务在另一个组件（客户端）调用 bindService() 时创建。然后，客户端通过 IBinder 接口与服务进行通信。客户端可以通过调用unbindService() 关闭连接。多个客户端可以绑定到相同服务，而且当所有绑定全部取消后，系统即会销毁该服务。 （服务不必自行停止运行。）
这两条路径并非完全独立。也就是说，您可以绑定到已经使用 startService() 启动的服务。例如，可以通过使用 Intent（标识要播放的音乐）调用 startService() 来启动后台音乐服务。随后，可能在用户需要稍加控制播放器或获取有关当前播放歌曲的信息时，Activity 可以通过调用 bindService()绑定到服务。在这种情况下，除非所有客户端均取消绑定，否则 stopService() 或 stopSelf() 不会实际停止服务。
![3831664-e187b1ce634a2174.png](https://upload-images.jianshu.io/upload_images/3831664-e187b1ce634a2174.png)
允许绑定的已启动服务的生命周期
当一个服务既bindService又startService的时候（先bindService还是先startService并不会有什么影响，），如果这时候所有的客户端都调用unbindService(),那么服务会调用onUnbind()方法，但是服务还是会继续运行，只有当调用stopService()的时候，服务才会调用onDestroy()方法，进入死亡状态。
当一个服务既bindService又startService的时候（先bindService还是先startService并不会有什么影响），如果先调用stopService()，这时候服务还是会继续运行，只有当所以客户端都调用unbindService()，这时候服务就会先调用onUnbind()，然后调用onDestroy()方法，进入死亡状态。
上面的图我认为是有错误的，应该分两种情况，也就是上面描述的两种情况，是先解绑所以服务还是先调用stopService。
![3831664-9706d6f4efe50c1a.png](https://upload-images.jianshu.io/upload_images/3831664-9706d6f4efe50c1a.png)
image.png
# 4.前台服务
前台服务被认为是用户主动意识到的一种服务，因此在内存不足时，系统也不会考虑将其终止。 前台服务必须为状态栏提供通知，放在“正在进行”标题下方，这意味着除非服务停止或从前台移除，否则不能清除通知。
例如，应该将通过服务播放音乐的音乐播放器设置为在前台运行，这是因为用户明确意识到其操作。 状态栏中的通知可能表示正在播放的歌曲，并允许用户启动 Activity 来与音乐播放器进行交互。
要请求让服务运行于前台，请调用 startForeground()。要从前台移除服务，请调用 stopForeground()。stopForeground此方法采用一个布尔值，指示是否也移除状态栏通知。 此方法不会停止服务。 但是，如果您在服务正在前台运行时将其停止，则通知也会被移除。
![3831664-38e3adc0253e4483.png](https://upload-images.jianshu.io/upload_images/3831664-38e3adc0253e4483.png)
image.png
具体例子请查看com.tan.lgy.testservice.ForegroundService
# 5.源码地址
[Servicel例子](https://link.jianshu.com?t=http://download.csdn.net/download/lgywsdy/10105782)
[Service客户端（测试远程服务）](https://link.jianshu.com?t=http://download.csdn.net/download/lgywsdy/10105785)
# 6.参考文章
[https://developer.android.google.cn/guide/components/services.html#CreatingStartedService](https://link.jianshu.com?t=https://developer.android.google.cn/guide/components/services.html#CreatingStartedService)
[https://developer.android.google.cn/guide/components/aidl.html](https://link.jianshu.com?t=https://developer.android.google.cn/guide/components/aidl.html)
[Android开发中文网址](https://link.jianshu.com?t=https://developer.android.google.cn/index.html)
