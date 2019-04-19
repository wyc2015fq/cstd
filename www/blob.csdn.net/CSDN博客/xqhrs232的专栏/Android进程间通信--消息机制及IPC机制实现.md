# Android进程间通信--消息机制及IPC机制实现 - xqhrs232的专栏 - CSDN博客
2011年03月14日 11:24:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：641标签：[android																[service																[string																[action																[scheme																[server](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=scheme&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://myqdroid.blog.51cto.com/2057579/394189](http://myqdroid.blog.51cto.com/2057579/394189)
**一、概念及说明**
Android为了屏蔽进程的概念，利用不同的组件[Activity、Service]来表示进程之间的通信！
组件间通信的核心机制是Intent，通过Intent可以开启一个Activity或Service，不论这个Activity或Service是属于当前应用还是其它应用的！
**本文如有bug，请指出啊！！大家一同进步！！**
**                                                             谢谢！！**
Intent包含两部分：
1、目的[action]--要往哪里去
2、内容[category、data]--路上带了些啥，区分性数据或内容性数据
Intent类型：
1、显式--直接指定消息目的地，只适合同一进程内的不同组件之间通信
new Intent(this,Target.class)
2、隐式--AndroidMainifest.xml中注册，一般用于跨进程通信
new Intent(String action)
**二、实现-Intent简单进程间通信**
显式的Intent较为简单！
如何实现隐式Intent呢？
在AndroidManifest.xml文件中定义<activity>
说明：
1、一个<activity>包括：
零个或多个<intent-filter>
它主要是作为匹配的标准，能否匹配成功由<action>、<category>、<data>三个tag共同决定的。
2、一个<intent-filter>包括：
**一个或多个 <action>零个或多个 <category>**
指定<activity>的分类特征
eg:
<category android:name="android.intent.category.LAUNCHER" /> 
--说明该<activity>是该project运行的第一个界面
<category android:name="android.intent.category.HOME" /> 
--说明该<activity>可以作为Launcher的，即系统操作界面
<category android:name="android.intent.category.DEFAULT" />
 --缺省情况
**零个或一个 <data>**
-- 指定携带的数据的类型,使用MIME类型描述方式来描述
eg:
<data android:mimeType="video/mpeg" />
video/mpeg表示编码格式为mpeg的视频，
也可以使用通配符video/*表示任意格式的视频文件类型；
在查询ContentProvider时，可以使用
<data android:mimeType="vnd.android.cursor.dir/vnd.myq.note" />
查询上来的数据是多个记录
<data android:mimeType="vnd.android.cursor.item/vnd.myq.note" />
查询上来的数据是单个记录
如上设置，要重写SQLiteOpenHelper的getType(Uri uri)方法
eg:
 @Override
 public String getType(Uri uri) {
  final int match = sUriMatcher.match(uri) ;
  switch(match)
  {
  case NOTES :
  case LIVE_FOLDER_NOTES:
   return "vnd.android.cursor.dir/vnd.myq.note" ;
  case NOTES_ID :
   return "vnd.android.cursor.item/vnd.myq.note" ;
  default:
   throw new IllegalArgumentException("invalid uri : " + uri) ;
  }
 }
数据的URI由scheme（协议），host，port，path四部分:scheme://host:port/path
<data android:scheme="[http://localhost:8080/test.jsp](http://localhost:8080/test.jsp)" />
3、一个Intent对应多种匹配结果的处理说明
 一个intent有多个可匹配的处理组件，系统如何处理？
分响应消息的组件类型：
1）如果是service那么这些service都可以启动并处理消息。
2）如果是Activity则会弹出一个对话框让用户进行选择。
4、安全性问题
 如果不同进程间的组件可以通过隐式消息互相通信，那程序不是可以轻易调用到其他的程序或者系统中的一些敏感程序的组件，这样会不会很不安全呢？
其实Android在安全方面有一个统一，完备和轻便的安全策略模型。
简单一点说就是：权限设置问题
我们可以自己定义permission，然后在需要的组件处设置该permission，那么用户要想该组件，必须要配置该permission，否则访问失败的！
eg:
1、定义permission
<permission-group android:name="android.permission-group.MYQ_INFO"/> 
<permission
     android:name="com.myq.android.permission.DATETIME_SERVICE"
     android:permissionGroup="android.permission-group.MYQ_INFO"
     android:protectionLevel="normal"
     />
2、配置permission
<service android:name=".DateTimeService" android:permission="com.myq.android.permission.DATETIME_SERVICE">
   <intent-filter>
 <action android:name="com.myq.android.MultiProcessTest.DATETIMESERVICE_ACTION" />
   </intent-filter>
</service>
3、使用permission
<uses-permission android:name="com.myq.android.permission.DATETIME_SERVICE"/>
**三、IPC机制**
有了Intent这种基于消息的进程内或进程间通信模型，我们就可以通过Intent去开启一个Service，可以通过Intent跳转到另一个Activity，不论上面的Service或Activity是在当前进程还是其它进程内即不论是当前应用还是其它应用的Service或Activity，通过消息机制都可以进行通信！
但是通过消息机制实现的进程间通信，有一个弊端就是，如果我们的Activity与Service之间的交往不是简单的Activity开启Service操作，而是要随时发一些控制请求，那么必须就要保证Activity在Service的运行过程中随时可以连接到Service。
**eg**：音乐播放程序
后台的播放服务往往独立运行，以方便在使用其他程序界面时也能听到音乐。同时这个后台播放服务也会定义一个控制接口，比如播放，暂停，快进等方法，任何时候播放程序的界面都可以连接到播放服务，然后通过这组控制接口方法对其控制。
如上的需求仅仅通过Intent去开启Service就无法满足了！从而Android的显得稍微笨重的IPC机制就出现了，然而它的出现只适用于Activity与Service之间的通信，类似于远程方法调用，就像是C/S模式的访问，通过定义AIDL接口文件来定义一个IPC接口，Server端实现IPC接口，Client端调用IPC接口的本地代理。
由于IPC调用是同步的，如果一个IPC服务需要超过几毫秒的时间才能完成的话，你应该避免在Activity的主线程中调用，否则IPC调用会挂起应用程序导致界面失去响应。在 这种情况下，应该考虑单起一个线程来处理IPC访问。
两个进程间IPC看起来就象是一个进程进入另一个进程执行代码然后带着执行的结果返回。
IPC机制鼓励我们“尽量利用已有功能，利用IPC和包含已有功能的程序协作完成一个完整的项目”
IPC实现demo:
我的
project -- **MultiProcessTest**
package -- **com.myq.android.MultiProcessTest**
1、AIDL文件，我是放在package下,
**文件名称为：**
IDateTimeService.aidl
**文件内容为：**
package com.myq.android.MultiProcessTest ;
interface IDateTimeService 
{
 String getCurrentDateTime(in String format) ;
}
如果正确配置，会在gen下，生成同名的java文件
简单摘要：
//我们需要实现的类Stub
public interface **IDateTimeService** extends android.os.IInterface
{
...
public static abstract class** Stub**
extends android.os.Binder 
implements com.myq.android.MultiProcessTest.IDateTimeService
{
 ...
//获取实例的方法asInterface
public static com.myq.android.MultiProcessTest.IDateTimeService **asInterface**(android.os.IBinder obj)
{
  ...
}
 ...
}
//我们自己的业务方法，需要实现的
public java.lang.String **getCurrentDateTime**(java.lang.String format) throws android.os.RemoteException;
}
2、Service中实现IDateTimeService.Stub
eg:
package com.myq.android.MultiProcessTest;
import java.text.SimpleDateFormat;
import java.util.Date;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
public class DateTimeService extends Service {
 public static final String DATETIME_SERVICE_ACTION = "com.myq.android.MultiProcessTest.DATETIMESERVICE_ACTION" ;
 private static final String TAG = "--------DateTimeService-------" ;
 private  SimpleDateFormat sdf ;
 private final IDateTimeService.Stub stub = new IDateTimeService.Stub()
 {
  public String getCurrentDateTime(String format) throws RemoteException {
   return getCurrentDateTimeString(format) ;
  }
 } ;
 private synchronized String getCurrentDateTimeString(String format)
 {
     sdf = new SimpleDateFormat(format) ;
     final String temp = sdf.format(new Date()) ;
   Log.i(TAG,"getCurrentDateTimeString--" + Thread.currentThread() + "--" + temp) ;
   return temp ;
 }
 public IBinder onBind(Intent arg0) 
 {
  Log.i(TAG, "onBind--" + Thread.currentThread()) ;
  return stub;
 }
}
3、Client端代码实现
private ServiceConnection mServiceConn = new ServiceConnection()
 {
  public void onServiceConnected(ComponentName name, IBinder service) {
   mDateTimeService = IDateTimeService.Stub.asInterface(service) ;
  }
  public void onServiceDisconnected(ComponentName name) {
   mDateTimeService = null ;
  }
 } ;
说明：
网上的好多资料都没有涉及IPC调用的AIDL的具体说明！
它本质上是Server端和Client端都具有相同的AIDL文件，要位于相同的包下，即package的包名药一样，然后才能正确的通过proxy访问，否则client与server的aidl文件处于不同package会出错的。
aidl模型如下：
                |<--------------------aidl---------------------->|
 client端-->proxy  ----------parcel数据包-------- stub<---server端
从而proxy+parcel+stub构成了aidl.
只不过，proxy运行在客户进程，而stub运行在服务端进程。
当你通过aidl去访问服务端时，客户端会阻塞在proxy,服务端处理完后，通知proxy返回。
**四、附件及说明**
1、
附件是我测试所用的demo，我用的系统是ubuntu9，Android2.2版本
基本功能：
可以根据用户选择的不同输出格式输出当前系统的时间。
2、
运行顺序:
先运行Server端：MultiProcessTest
再运行Client端：MultiProcessTestClient
3、
注意点：
Server和Client端的AIDL文件必须要位于同package下，否则会出错
安全性问题实现，权限控制--定义、配置、使用
异步处理问题--Handler

