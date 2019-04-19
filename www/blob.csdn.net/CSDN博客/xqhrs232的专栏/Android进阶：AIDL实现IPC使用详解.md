# Android进阶：AIDL实现IPC使用详解 - xqhrs232的专栏 - CSDN博客
2012年02月27日 22:40:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：533标签：[android																[service																[interface																[button																[eclipse插件																[import](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse插件&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.eefocus.com/bbs/article_867_192532.html](http://www.eefocus.com/bbs/article_867_192532.html)
**使用AIDL设计远程接口(Designing a Remote Interface Using AIDL)**
由于每个应用程序都运行在自己的进程空间，并且可以从应用程序UI运行另一个服务进程，而且经常会在不同的进程间传递对象。在Android平台，一个进程通常不能访问另一个进程的内存空间，所以要想对话，需要将对象分解成操作系统可以理解的基本单元，并且有序的通过进程边界。
通过代码来实现这个数据传输过程是冗长乏味的，Android提供了AIDL工具来处理这项工作。
AIDL (Android Interface Definition Language)是一种IDL 语言，用于生成可以在Android设备上两个进程之间进行进程间通信(IPC)的代码。如果在一个进程中（例如Activity）要调用另一个进程中（例如Service）对象的操作，就可以使用AIDL生成可序列化的参数。
AIDL IPC机制是面向接口的，像COM或Corba一样，但是更加轻量级。它是使用代理类在客户端和实现端传递数据。
**使用AIDL实现IPC(Implementing IPC Using AIDL)**
使用AIDL实现IPC服务的步骤是：
1.         创建.aidl文件-该文件（YourInterface.aidl）定义了客户端可用的方法和数据的接口。
2.         在makefile文件中加入.aidl文件-（Eclipse中的ADT插件提供管理功能）Android包括名为AIDL的编译器，位于tools/文件夹。
3.         实现接口-AIDL编译器从AIDL接口文件中利用Java语言创建接口，该接口有一个继承的命名为Stub的内部抽象类（并且实现了一些IPC调用的附加方法），要做的就是创建一个继承于YourInterface.Stub的类并且实现在.aidl文件中声明的方法。
4.         向客户端公开接口-如果是编写服务，应该继承Service并且重载Service.onBind(Intent) 以返回实现了接口的对象实例
**创建.aidl文件(Create an .aidl File)**
AIDL使用简单的语法来声明接口，描述其方法以及方法的参数和返回值。这些参数和返回值可以是任何类型，甚至是其他AIDL生成的接口。重要的是必须导入所有非内置类型，哪怕是这些类型是在与接口相同的包中。下面是AIDL能支持的数据类型：
l  Java编程语言的主要类型 (int, boolean等) — 不需要 import 语句。
l  以下的类 (不需要import 语句):
**n  String**
n  **List** -列表中的所有元素必须是在此列出的类型，包括其他AIDL生成的接口和可打包类型。List可以像一般的类（例如List<String>）那样使用，另一边接收的具体类一般是一个ArrayList，这些方法会使用List接口。
n  **Map** - Map中的所有元素必须是在此列出的类型，包括其他AIDL生成的接口和可打包类型。一般的maps（例如Map<String,Integer>）不被支持，另一边接收的具体类一般是一个HashMap，这些方法会使用Map接口。
n  **CharSequence** -该类是被TextView和其他控件对象使用的字符序列。
l  通常引引用方式传递的其他AIDL生成的接口，必须要import 语句声明
l  实现了Parcelable protocol 以及按值传递的自定义类，必须要import 语句声明。
**实现接口(Implementing the Interface)**
         AIDL生成了与.aidl文件同名的接口，如果使用Eclipse插件，AIDL会做为编译过程的一部分自动运行（不需要先运行AIDL再编译项目），如果没有插件，就要先运行AIDL。
         生成的接口包含一个名为Stub的抽象的内部类，该类声明了所有.aidl中描述的方法，Stub还定义了少量的辅助方法，尤其是asInterface()，通过它或以获得IBinder（当applicationContext.bindService()成功调用时传递到客户端的onServiceConnected()）并且返回用于调用IPC方法的接口实例
         要实现自己的接口，就从YourInterface.Stub类继承，然后实现相关的方法（可以创建.aidl文件然后实现stub方法而不用在中间编译，Android编译过程会在.java文件之前处理.aidl文件）。
         这个例子实现了对IRemoteService接口的调用，这里使用了匿名对象并且只有一个getPid()接口。
> 
> ![](http://www.linuxidc.com/upload/2011_12/111215090750321.gif)
         这里是实现接口的几条说明：
l  不会有返回给调用方的异常
l  默认IPC调用是同步的。如果已知IPC服务端会花费很多毫秒才能完成，那就不要在Activity或View线程中调用，否则会引起应用程序挂起（Android可能会显示“应用程序未响应”对话框），可以试着在独立的线程中调用。
l  AIDL接口中只支持方法，不能声明静态成员。
**向客户端暴露接口(Exposing Your Interface to Clients)**
在完成了接口的实现后需要向客户端暴露接口了，也就是发布服务，实现的方法是继承 Service，然后实现以Service.onBind(Intent)返回一个实现了接口的类对象。下面的代码片断表示了暴露IRemoteService接口给客户端的方式。
- **public****class** RemoteService **extends** Service {  
- 
- ...   
- 
- @Override
- 
- **public** IBinder onBind(Intent intent) {  
- 
- // Select the interface to return.  If your service only implements
- 
- // a single interface, you can just return it here without checking
- 
- // the Intent.
- 
- **if** (IRemoteService.**class**.getName().equals(intent.getAction())) {  
- 
- **return** mBinder;  
- 
-         }   
- 
- **if** (ISecondary.**class**.getName().equals(intent.getAction())) {  
- 
- **return** mSecondaryBinder;  
- 
-         }   
- 
- **return****null**;  
- 
-     }   
- 
- 
- 
- /**
- 
-      * The IRemoteInterface is defined through IDL
- 
-      */
- 
- **private****final** IRemoteService.Stub mBinder = **new** IRemoteService.Stub() {  
- 
- **public****void** registerCallback(IRemoteServiceCallback cb) {  
- 
- **if** (cb != **null**) mCallbacks.register(cb);  
- 
-         }  
- 
- **public****void** unregisterCallback(IRemoteServiceCallback cb) {  
- 
- **if** (cb != **null**) mCallbacks.unregister(cb);  
- 
-         }  
- 
-     };  
- 
- 
- 
- /**
- 
-      * A secondary interface to the service.
- 
-      */
- 
- **private****final** ISecondary.Stub mSecondaryBinder = **new** ISecondary.Stub() {  
- 
- **public****int** getPid() {  
- 
- **return** Process.myPid();  
- 
-         }  
- 
- **public****void** basicTypes(**int** anInt, **long** aLong, **boolean** aBoolean,  
- 
- **float** aFloat, **double** aDouble, String aString) {  
- 
-         }  
- 
-     };  
- 
- 
- 
- }  
使用可打包接口传递参数Pass by value Parameters using Parcelables
如果有类想要能过AIDL在进程之间传递，这一想法是可以实现的，必须确保这个类在IPC的两端的有效性，通常的情形是与一个启动的服务通信。
这里列出了使类能够支持Parcelable的4个步骤：【译者注：原文为5，但列表为4项，疑为作者笔误】
1. 使该类实现Parcelabel接口。
2. 实现public void writeToParcel(Parcel out) 方法，以便可以将对象的当前状态写入包装对象中。
3. 增加名为CREATOR的构造器到类中，并实现Parcelable.Creator接口。
4. 最后，但同样重要的是，创建AIDL文件声明这个可打包的类（见下文），如果使用的是自定义的编译过程，那么不要编译此AIDL文件，它像C语言的头文件一样不需要编译。
AIDL会使用这些方法的成员序列化和反序列化对象。
这个例子演示了如何让Rect类实现Parcelable接口。
import Android.os.Parcel;
import Android.os.Parcelable;
public final class Rect implements Parcelable {
public int left;
public int top;
public int right;
public int bottom;
public static final Parcelable.Creator CREATOR = new Parcelable.Creator() {
public Rect createFromParcel(Parcel in) {
return new Rect(in);
}
public Rect[] newArray(int size) {
return new Rect[size];
}
};
public Rect() {
}
private Rect(Parcel in) {
readFromParcel(in);
}
public void writeToParcel(Parcel out) {
out.writeInt(left);
out.writeInt(top);
out.writeInt(right);
out.writeInt(bottom);
}
public void readFromParcel(Parcel in) {
left = in.readInt();
top = in.readInt();
right = in.readInt();
bottom = in.readInt();
}
}
这个是Rect.aidl文件。
序列化Rect类的工作相当简单，对可打包的其他类型的数据可以参见Parcel类。
调用IPC方法(Calling an IPC Method)
这里给出了调用远端接口的步骤：
1. 声明.aidl文件中定义的接口类型的变量。
2. 实现ServiceConnection
3. 调用Context.bindService()，传递ServiceConnection的实现
4. 在ServiceConnection.onServiceConnected()方法中会接收到IBinder对象，调用YourInterfaceName.Stub.asInterface((IBinder)service)将返回值转换为YourInterface类型
5. 调用接口中定义的方法。应该总是捕获连接被打断时抛出的DeadObjectException异常，这是远端方法唯一的异常。
6. 调用Context.unbindService()断开连接
这里是几个调用IPC服务的提示：
l 对象是在进程间进行引用计数
l 可以发送匿名对象作为方法参数
以下是演示调用AIDL创建的服务，可以在ApiDemos项目中获取远程服务的示例。
public static class Binding extends Activity {
/** The primary interface we will be calling on the service. */
IRemoteService mService = null;
/** Another interface we use on the service. */
ISecondary mSecondaryService = null;
Button mKillButton;
TextView mCallbackText;
private boolean mIsBound;
/**
* Standard initialization of this activity. Set up the UI, then wait
* for the user to poke it before doing anything.
*/
@Override
protected void onCreate(Bundle savedInstanceState) {
super.onCreate(savedInstanceState);
setContentView(R.layout.remote_service_binding);
// Watch for button clicks.
Button button = (Button)findViewById(R.id.bind);
button.setOnClickListener(mBindListener);
button = (Button)findViewById(R.id.unbind);
button.setOnClickListener(mUnbindListener);
mKillButton = (Button)findViewById(R.id.kill);
mKillButton.setOnClickListener(mKillListener);
mKillButton.setEnabled(false);
mCallbackText = (TextView)findViewById(R.id.callback);
mCallbackText.setText("Not attached.");
}
/**
* Class for interacting with the main interface of the service.
*/
private ServiceConnection mConnection = new ServiceConnection() {
public void onServiceConnected(ComponentName className, IBinder service) {
// This is called when the connection with the service has been
// established, giving us the service object we can use to
// interact with the service. We are communicating with our
// service through an IDL interface, so get a client-side
// representation of that from the raw service object.
mService = IRemoteService.Stub.asInterface(service);
mKillButton.setEnabled(true);
mCallbackText.setText("Attached.");
// We want to monitor the service for as long as we are
// connected to it.
try {
mService.registerCallback(mCallback);
} catch (RemoteException e) {
// In this case the service has crashed before we could even
// do anything with it; we can count on soon being
// disconnected (and then reconnected if it can be restarted)
// so there is no need to do anything here.
}
// As part of the sample, tell the user what happened.
Toast.makeText(Binding.this, R.string.remote_service_connected,
Toast.LENGTH_SHORT).show();
}
public void onServiceDisconnected(ComponentName className) {
// This is called when the connection with the service has been
// unexpectedly disconnected -- that is, its process crashed.
mService = null;
mKillButton.setEnabled(false);
mCallbackText.setText("Disconnected.");
// As part of the sample, tell the user what happened.
Toast.makeText(Binding.this, R.string.remote_service_disconnected,
Toast.LENGTH_SHORT).show();
}
};
/**
* Class for interacting with the secondary interface of the service.
*/
private ServiceConnection mSecondaryConnection = new ServiceConnection() {
public void onServiceConnected(ComponentName className, IBinder service) {
// Connecting to a secondary interface is the same as any
// other interface.
mSecondaryService = ISecondary.Stub.asInterface(service);
mKillButton.setEnabled(true);
}
public void onServiceDisconnected(ComponentName className) {
mSecondaryService = null;
mKillButton.setEnabled(false);
}
};
private OnClickListener mBindListener = new OnClickListener() {
public void onClick(View v) {
// Establish a couple connections with the service, binding
// by interface names. This allows other applications to be
// installed that replace the remote service by implementing
// the same interface.
bindService(new Intent(IRemoteService.class.getName()),
mConnection, Context.BIND_AUTO_CREATE);
bindService(new Intent(ISecondary.class.getName()),
mSecondaryConnection, Context.BIND_AUTO_CREATE);
mIsBound = true;
mCallbackText.setText("Binding.");
}
};
private OnClickListener mUnbindListener = new OnClickListener() {
public void onClick(View v) {
if (mIsBound) {
// If we have received the service, and hence registered with
// it, then now is the time to unregister.
if (mService != null) {
try {
mService.unregisterCallback(mCallback);
} catch (RemoteException e) {
// There is nothing special we need to do if the service
// has crashed.
}
}
// Detach our existing connection.
unbindService(mConnection);
unbindService(mSecondaryConnection);
mKillButton.setEnabled(false);
mIsBound = false;
mCallbackText.setText("Unbinding.");
}
}
};
private OnClickListener mKillListener = new OnClickListener() {
public void onClick(View v) {
// To kill the process hosting our service, we need to know its
// PID. Conveniently our service has a call that will return
// to us that information.
if (mSecondaryService != null) {
try {
int pid = mSecondaryService.getPid();
// Note that, though this API allows us to request to
// kill any process based on its PID, the kernel will
// still impose standard restrictions on which PIDs you
// are actually able to kill. Typically this means only
// the process running your application and any additional
// processes created by that app as shown here; packages
// sharing a common UID will also be able to kill each
// other's processes.
Process.killProcess(pid);
mCallbackText.setText("Killed service process.");
} catch (RemoteException ex) {
// Recover gracefully from the process hosting the
// server dying.
// Just for purposes of the sample, put up a notification.
Toast.makeText(Binding.this,
R.string.remote_call_failed,
Toast.LENGTH_SHORT).show();
}
}
}
};
// ----------------------------------------------------------------------
// Code showing how to deal with callbacks.
// ----------------------------------------------------------------------
/**
* This implementation is used to receive callbacks from the remote
* service.
*/
private IRemoteServiceCallback mCallback = new IRemoteServiceCallback.Stub() {
/**
* This is called by the remote service regularly to tell us about
* new values. Note that IPC calls are dispatched through a thread
* pool running in each process, so the code executing here will
* NOT be running in our main thread like most other things -- so,
* to update the UI, we need to use a Handler to hop over there.
*/
public void valueChanged(int value) {
mHandler.sendMessage(mHandler.obtainMessage(BUMP_MSG, value, 0));
}
};
private static final int BUMP_MSG = 1;
private Handler mHandler = new Handler() {
@Override public void handleMessage(Message msg) {
switch (msg.what) {
case BUMP_MSG:
mCallbackText.setText("Received from service: " + msg.arg1);
break;
default:
super.handleMessage(msg);
}
}
};
}
