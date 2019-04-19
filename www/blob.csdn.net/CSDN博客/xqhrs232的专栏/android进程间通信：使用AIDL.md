# android进程间通信：使用AIDL - xqhrs232的专栏 - CSDN博客
2011年01月10日 13:22:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：870标签：[android																[descriptor																[service																[interface																[button																[callback](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=descriptor&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/saintswordsman/archive/2010/01/04/5130947.aspx](http://blog.csdn.net/saintswordsman/archive/2010/01/04/5130947.aspx)
  欢迎阅读本文，你能关注本文，你知道你需要进程间通信、需要AIDL（以及Binder），那么可以默认你对这些概念已经有了一些了解，你（大致）知道它们是什么，它们有什么用，所以为了节约大家的眼力和时间，在此我不复制粘贴网上泛滥的博客或者翻译冗长的android文档。
      关于AIDL的介绍在文档：docs/guide/developing/tools/aidl.html
      关于IBinder的介绍在文档：docs/reference/android/os/IBinder.html
      以及Binder：docs/reference/android/os/Binder.html
      在后文中，我将以我自己的理解向你介绍相关的概念。以我目前粗浅的经验，应用程序使用AIDL的地方，几乎都和Service有关，所以你也需要知道一些关于Service的知识。日后得闲我也会继续写一些关于Service的贴。
      本文将以一个例子来和你分享使用AIDL的基础技能，这个例子里有：
1、一个类mAIDLActivity，继承Activity。里面有三个按钮，text分别为StartService，StopService，CallbackTest。
2、一个类mAIDLService，继承Service。为了充分展示ADIL的功能，它做以下工作：当用户点击CallbackTest按钮时，从mAIDLActivity调用mAIDLService中的Stub 对象的一个方法invokCallBack()，而这个方法又会调用mAIDLActivity中Stub 对象的一个方法performAction()，这个方法在屏幕上显示一个toast。没什么意义，只是展示一下AIDL如何使用。
3、两个AIDL文件：forService.aidl和forActivity.aidl。对应名字，在Service和Activity中分别有对象需要用到它们定义的接口。
4、相关XML文件，略过。关于manifest中Service的语法，见docs/guide/topics/manifest/service-element.html。你也可以简单地在<application></application>中加入
     <service android:name=".mAIDLService" android:process=":remote"> </service>
开发环境为Eclipse。
拣重要的先说，来看看aidl文件的内容：
文件：forActivity.aidl
package com.styleflying.AIDL;
interface forActivity {
 void performAction();
 }   
文件：forService.aidl
package com.styleflying.AIDL;
import com.styleflying.AIDL.forActivity;
interface forService {
 void registerTestCall(forActivity cb);
 void invokCallBack();
}   
这两个文件和Java文件放置的地方一样，看包名。
在Eclipse中它们将被自动编译为forActivity.java和forService.java，它们存放在gen目录下。为了方便手头无法演练的读者，代码贴上，不用细看。
文件forActivity.java：
/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: D://workspace//AIDLTest//src//com//styleflying//AIDL//forActivity.aidl
 */
package com.styleflying.AIDL;
import java.lang.String;
import android.os.RemoteException;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Binder;
import android.os.Parcel;
public interface forActivity extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.styleflying.AIDL.forActivity
{
private static final java.lang.String DESCRIPTOR = "com.styleflying.AIDL.forActivity";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an forActivity interface,
 * generating a proxy if needed.
 */
public static com.styleflying.AIDL.forActivity asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = (android.os.IInterface)obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.styleflying.AIDL.forActivity))) {
return ((com.styleflying.AIDL.forActivity)iin);
}
return new com.styleflying.AIDL.forActivity.Stub.Proxy(obj);
}
public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_performAction:
{
data.enforceInterface(DESCRIPTOR);
this.performAction();
reply.writeNoException();
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.styleflying.AIDL.forActivity
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
public android.os.IBinder asBinder()
{
return mRemote;
}
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
public void performAction() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_performAction, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
}
static final int TRANSACTION_performAction = (IBinder.FIRST_CALL_TRANSACTION + 0);
}
public void performAction() throws android.os.RemoteException;
}
文件forService.java：
/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: D://workspace//AIDLTest//src//com//styleflying//AIDL//forService.aidl
 */
package com.styleflying.AIDL;
import java.lang.String;
import android.os.RemoteException;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Binder;
import android.os.Parcel;
public interface forService extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.styleflying.AIDL.forService
{
private static final java.lang.String DESCRIPTOR = "com.styleflying.AIDL.forService";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an forService interface,
 * generating a proxy if needed.
 */
public static com.styleflying.AIDL.forService asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = (android.os.IInterface)obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.styleflying.AIDL.forService))) {
return ((com.styleflying.AIDL.forService)iin);
}
return new com.styleflying.AIDL.forService.Stub.Proxy(obj);
}
public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_registerTestCall:
{
data.enforceInterface(DESCRIPTOR);
com.styleflying.AIDL.forActivity _arg0;
_arg0 = com.styleflying.AIDL.forActivity.Stub.asInterface(data.readStrongBinder());
this.registerTestCall(_arg0);
reply.writeNoException();
return true;
}
case TRANSACTION_invokCallBack:
{
data.enforceInterface(DESCRIPTOR);
this.invokCallBack();
reply.writeNoException();
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.styleflying.AIDL.forService
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
public android.os.IBinder asBinder()
{
return mRemote;
}
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
public void registerTestCall(com.styleflying.AIDL.forActivity cb) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeStrongBinder((((cb!=null))?(cb.asBinder()):(null)));
mRemote.transact(Stub.TRANSACTION_registerTestCall, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
public void invokCallBack() throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
mRemote.transact(Stub.TRANSACTION_invokCallBack, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
}
static final int TRANSACTION_registerTestCall = (IBinder.FIRST_CALL_TRANSACTION + 0);
static final int TRANSACTION_invokCallBack = (IBinder.FIRST_CALL_TRANSACTION + 1);
}
public void registerTestCall(com.styleflying.AIDL.forActivity cb) throws android.os.RemoteException;
public void invokCallBack() throws android.os.RemoteException;
}
两段代码差不多，前面基本一样，从后面看，最后跟着我们在AIDL中自定义的方法，没有实现。两个文件各定义一个了接口，这两个接口分别会在Activity和Service中使用，在那里我们将实现自定义的方法。两个接口中都定义了一个抽象类Stub，实现所在的接口。Stub中又有一个类Proxy。Stub中有一个static的asInterface()方法，里面有很多return语句，在mAIDLActivity中调用它时，它返回一个新创建的内部类Proxy对象。
这个Stub对我们来说很有用，它继承了Binder。Binder有什么用呢？一个类，继承了Binder，那么它的对象就可以被远程的进程使用了(前提是远程进程获取了这个类的对象【对象的引用】，至于如如何获得看下文)，在本例中就是说，如果一个Service中有一个继承了Stub的类的对象，那么这个对象中的方法就可以在Activity中使用，对Activity也是这样。至于Binder的细节，网上有很多贴介绍，看不明白也不影响我们完成这个例子。
再看mAIDLActivity.java:
package com.styleflying.AIDL;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
public class mAIDLActivity extends Activity {
 private static final String TAG = "AIDLActivity";
 private Button btnOk;
    private Button btnCancel;
    private Button btnCallBack;
    private void Log(String str) {
        Log.d(TAG, "------ " + str + "------");
        }
    private forActivity mCallback = new forActivity.Stub() {
  public void performAction() throws RemoteException
  {
   Toast.makeText(mAIDLActivity.this, "this toast is called from service", 1).show();
  }
  };
 forService mService;
 private ServiceConnection mConnection = new ServiceConnection() {
  public void onServiceConnected(ComponentName className,
    IBinder service) {
   mService = forService.Stub.asInterface(service);
   try {
    mService.registerTestCall(mCallback);}
   catch (RemoteException e) {
   }
   }
  public void onServiceDisconnected(ComponentName className) {
   Log("disconnect service");
   mService = null;
   }
  };
    @Override
    public void onCreate(Bundle icicle) {
     super.onCreate(icicle);
        setContentView(R.layout.main);
        btnOk = (Button)findViewById(R.id.btn_ok);
        btnCancel = (Button)findViewById(R.id.btn_cancel);
        btnCallBack = (Button)findViewById(R.id.btn_callback);
        btnOk.setOnClickListener(new OnClickListener() {
         public void onClick(View v) {
             Bundle args = new Bundle();
                Intent intent = new Intent(mAIDLActivity.this, mAIDLService.class);
                intent.putExtras(args);
                bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
                startService(intent);
                }
            });
        btnCancel.setOnClickListener(new OnClickListener() {
         public void onClick(View v) {
             unbindService(mConnection);
             //stopService(intent);
             }
            });
        btnCallBack.setOnClickListener(new OnClickListener() {
   @Override
   public void onClick(View v)
   {
    try
    {
     mService.invokCallBack();
    } catch (RemoteException e)
    {
     // TODO Auto-generated catch block
     e.printStackTrace();
    }
   }
  });
        }
} 
很短，相信大家很容易看明白。注意mConnection，它的onServiceConnected()中有一句mService = forService.Stub.asInterface(service);给mService赋值了，这个mService是一个forService，而service是onServiceConnected()传进来的参数，onServiceConnected()会在连接Service的时候被系统调用，这个service参数的值来自哪里呢？看mAIDLService.java:
package com.styleflying.AIDL;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.util.Log;
public class mAIDLService extends Service {
    private static final String TAG = "AIDLService";  
    private forActivity callback;
    private void Log(String str) {
        Log.d(TAG, "------ " + str + "------");
    }
    @Override
    public void onCreate() {
        Log("service create");
    }
    @Override
    public void onStart(Intent intent, int startId) {
        Log("service start id=" + startId);
    }
    @Override
    public IBinder onBind(Intent t) {
        Log("service on bind");
        return mBinder;
    }
    @Override
    public void onDestroy() {
        Log("service on destroy");
        super.onDestroy();
    }
    @Override
    public boolean onUnbind(Intent intent) {
        Log("service on unbind");
        return super.onUnbind(intent);
    }
    public void onRebind(Intent intent) {
        Log("service on rebind");
        super.onRebind(intent);
    }
    private final forService.Stub mBinder = new forService.Stub() {
  @Override
  public void invokCallBack() throws RemoteException
  {
   callback.performAction();
  }
  @Override
  public void registerTestCall(forActivity cb) throws RemoteException
  {
   callback = cb;
  }
    };
} 
注意onBind()，它的返回类型为IBinder，返回了一个mBinder，看看mBinder的定义：
    private final forService.Stub mBinder = new forService.Stub() {
        @Override
        public void invokCallBack() throws RemoteException
        {
            callback.performAction();
         }
        @Override
        public void registerTestCall(forActivity cb) throws RemoteException
        {
            callback = cb;
        }
       };
它是实现了我们在AIDL中定义的方法，这个mBinder最终返回给了mAIDLActivity中的mService，于是在mAIDLActivity中可以使用mBinder中的方法了。在mAIDLActivity中也有一个类似mBinder的对象，看看定义：   
        private forActivity mCallback = new forActivity.Stub() 
    {
        public void performAction() throws RemoteException
        {
            Toast.makeText(mAIDLActivity.this, "this toast is called from service", 1).show();
        }
      };
我们要在界面上显示一个toast，就是在这里实现的。这个对象，在mConnection的onServiceConnected()被调用时，通过调用mService（也就是远程的mAIDLService中的mBinder）的registerTestCall()，传递给了mAIDLService，于是在mAIDLService中可以调用performAction()了。
很啰嗦，只为了能把这个细节说清楚。请大家认真看，我尽量避免错别字、混乱的大小写和逻辑不清的语法，相信你会看明白。是不是很简单？再啰嗦一下，做一个大致总结，我们使用AIDL是要做什么呢：
让Acticity（或者说一个进程/一个类？）和Service（或者说远端进程/远端类/对象？）获取对方的一个Stub对象，这个对象在定义时实现了我们在AIDL中定义的方法，于是这些远程对象中的方法可以在本地使用了。如果这种使用（通信）是单向的，比如只是Activity需要通知Service做什么，那么只要Service中有一个Stub对象，并且传给Acticity就够了。
至于如何获得远程的Stub，参看上面的代码，看mConnection、registerTestCall、onRebind，它们展示了一种方法。
另外，有时候我们可能在一个类中有多个Stub对象，它们都要给远程交互的类的实例，这个时候可以考虑使用RemoteCallbackList<>（docs/reference/android/os/RemoteCallbackList.html）。
欢迎阅读、收藏本文。例子随手写的，功能只在演示AIDL的使用。您可以转载本文，但请勿盲目乱贴。不是我小气，我不权威，我怕它被贴到泛滥，以讹传讹，害了人。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/saintswordsman/archive/2010/01/04/5130947.aspx](http://blog.csdn.net/saintswordsman/archive/2010/01/04/5130947.aspx)

