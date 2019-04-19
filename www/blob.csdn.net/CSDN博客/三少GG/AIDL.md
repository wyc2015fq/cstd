# AIDL - 三少GG - CSDN博客
2013年01月22日 23:35:41[三少GG](https://me.csdn.net/scut1135)阅读数：1380
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)

**官网介绍及示例：**
**[http://developer.android.com/guide/components/aidl.html#PassingObjects](http://developer.android.com/guide/components/aidl.html#PassingObjects)**
**什么是AIDL**
AIDL:Android Interface Definition Language,即Android接口描述语言。
Android系统中的进程之间不能[共享内存](http://baike.baidu.com/view/120892.htm)，因此，需要提供一些机制在不同进程之间进行数据通信。
为了使其他的应用程序也可以访问本应用程序提供的服务，Android系统采用了[远程过程调用](http://baike.baidu.com/view/431455.htm)（Remote Procedure Call，RPC）方式来实现。与很多其他的基于RPC的解决方案一样，Android使用一种接口定义语言（Interface Definition Language，IDL）来公开服务的接口。我们知道4个Android应用程序组件中的3个（Activity、Broadcast和Content
 Provider）都可以进行跨进程访问，另外一个Android应用程序组件Service同样可以。因此，可以将这种可以跨进程访问的服务称为AIDL（Android Interface Definition Language）服务。
## 建立AIDL服务的步骤
建立AIDL服务要比建立普通的服务复杂一些，具体步骤如下：
（1）在Eclipse Android工程的Java包目录中建立一个扩展名为aidl的文件。该文件的语法类似于Java代码，但会稍有不同。详细介绍见实例的内容。
（2）如果aidl文件的内容是正确的，ADT会自动生成一个Java接口文件（*.java）。
（3）建立一个服务类（Service的子类）。
（4）实现由aidl文件生成的Java接口。
（5）在AndroidManifest.xml文件中配置AIDL服务，尤其要注意的是，<action>标签中android:name的属性值就是客户端要引用该服务的ID，也就是Intent类的参数值。
### Android AIDL使用详解
1.什么是aidl:aidl是 Android Interface definition language的缩写，一看就明白，它是一种android内部进程通信接口的描述语言，通过它我们可以定义进程间的通信接口 
   icp:interprocess communication :内部进程通信
2.既然aidl可以定义并实现进程通信，那么我们怎么使用它呢？文档/android-sdk/docs/guide/developing/tools/aidl.html中对步骤作了详细描述：
--1.Create your .aidl file - This file defines an interface (YourInterface.aidl) that defines the methods and fields available to a client. 
创建你的aidl文件，我在后面给出了一个例子，它的aidl文件定义如下：写法跟java代码类似，但是这里有一点值得注意的就是它可以引用其它aidl文件中定义的接口，但是不能够引用你的java类文件中定义的接口
**[java]**[view
 plain](http://blog.csdn.net/stonecao/article/details/6425019#)[copy](http://blog.csdn.net/stonecao/article/details/6425019#)
- package com.cao.android.demos.binder.aidl;    
- import com.cao.android.demos.binder.aidl.AIDLActivity;  
- interface AIDLService {     
- void registerTestCall(AIDLActivity cb);     
- void invokCallBack();  
- }    
--2.Add the .aidl file to your makefile - (the ADT Plugin for Eclipse manages this for you). Android includes the compiler, called AIDL, in the tools/ directory. 
编译你的aidl文件，这个只要是在eclipse中开发，你的adt插件会像资源文件一样把aidl文件编译成java代码生成在gen文件夹下，不用手动去编译：编译生成AIDLService.java如我例子中代码
![](http://hi.csdn.net/attachment/201105/16/0_1305538961LZ0C.gif)
--3.Implement your interface methods - The AIDL compiler creates an interface in the Java programming language from your AIDL interface. This interface has an inner abstract class named Stub that inherits the interface (and implements a few additional methods
 necessary for the IPC call). You must create a class that extends YourInterface.Stub and implements the methods you declared in your .aidl file. 
实现你定义aidl接口中的内部抽象类Stub,public static abstract class Stub extends android.os.Binder implements com.cao.android.demos.binder.aidl.AIDLService
Stub类继承了Binder，并继承我们在aidl文件中定义的接口，我们需要实现接口方法，下面是我在例子中实现的Stub类：
**[java:showcolumns]**[view
 plain](http://blog.csdn.net/stonecao/article/details/6425019#)[copy](http://blog.csdn.net/stonecao/article/details/6425019#)
·········10········20········30········40········50········60········70········80········90········100·······110·······120·······130·······140·······150
- privatefinal AIDLService.Stub mBinder = new AIDLService.Stub() {  
- 
- @Override
- publicvoid invokCallBack() throws RemoteException {  
-         Log("AIDLService.invokCallBack");  
-         Rect1 rect = new Rect1();  
-         rect.bottom=-1;  
-         rect.left=-1;  
-         rect.right=1;  
-         rect.top=1;  
-         callback.performAction(rect);  
-     }  
- 
- 
- @Override
- publicvoid registerTestCall(AIDLActivity cb) throws RemoteException {  
-         Log("AIDLService.registerTestCall");  
-         callback = cb;  
-     }  
- };  
Stub翻译成中文是存根的意思，注意Stub对象是在被调用端进程，也就是服务端进程,至此，服务端aidl服务端得编码完成了。
--4.Expose your interface to clients - If you're writing a service, you should extend Service and override Service.onBind(Intent) to return an instance of your class that implements your interface. 
第四步告诉你怎么在客户端如何调用服务端得aidl描述的接口对象，doc只告诉我们需要实现Service.onBind(Intent)方法，该方法会返回一个IBinder对象到客户端，绑定服务时不是需要一个ServiceConnection对象么，在没有了解aidl用法前一直不知道它是什么作用，其实他就是用来在客户端绑定service时接收service返回的IBinder对象的：
**[java]**[view
 plain](http://blog.csdn.net/stonecao/article/details/6425019#)[copy](http://blog.csdn.net/stonecao/article/details/6425019#)
- AIDLService mService;  
- private ServiceConnection mConnection = new ServiceConnection() {  
- publicvoid onServiceConnected(ComponentName className, IBinder service) {  
-         Log("connect service");  
-         mService = AIDLService.Stub.asInterface(service);  
- try {  
-             mService.registerTestCall(mCallback);  
-         } catch (RemoteException e) {  
- 
-         }  
-     }  
- 
- 
- publicvoid onServiceDisconnected(ComponentName className) {  
-         Log("disconnect service");  
-         mService = null;  
-     }  
- };  
mService就是AIDLService对象，具体可以看我后面提供的示例代码，需要注意在客户端需要存一个服务端实现了的aidl接口描述文件，但是客户端只是使用该aidl接口，不需要实现它的Stub类，获取服务端得aidl对象后mService = AIDLService.Stub.asInterface(service);，就可以在客户端使用它了，对mService对象方法的调用不是在客户端执行，而是在服务端执行。
4.aidl中使用java类，需要实现Parcelable接口，并且在定义类相同包下面对类进行声明：
上面我定义了Rect1类
之后你就可以在aidl接口中对该类进行使用了
package com.cao.android.demos.binder.aidl;  
import com.cao.android.demos.binder.aidl.Rect1;
interface AIDLActivity {   
    void performAction(in Rect1 rect);   
}  
注意in/out的说明，我这里使用了in表示输入参数，out没有试过，为什么使用in/out暂时没有做深入研究。
5.aidl使用完整示例，为了清除说明aidl使用，我这里写了一个例子，例子参考了博客:
[http://blog.csdn.net/saintswordsman/archive/2010/01/04/5130947.aspx](http://blog.csdn.net/saintswordsman/archive/2010/01/04/5130947.aspx)
作出说明
例子实现了一个AIDLTestActivity，AIDLTestActivity通过bindservice绑定一个服务AIDLTestService，通过并获取AIDLTestActivity的一个aidl对象AIDLService，该对象提供两个方法，一个是registerTestCall注册一个aidl对象，通过该方法，AIDLTestActivity把本身实现的一个aidl对象AIDLActivity传到AIDLTestService，在AIDLTestService通过操作AIDLActivity这个aidl远端对象代理，使AIDLTestActivity弹出一个toast，完整例子见我上传的资源：
[http://download.csdn.net/source/3284820](http://download.csdn.net/source/3284820)
文章仓促而成，有什么疑问欢迎大家一起讨论
///////////////////////////////////
### [Android高手进阶教程(二十五)之---Android 中的AIDL!!!](http://blog.csdn.net/android_tutor/article/details/6427680)

大家好，好久不见，今天要给大家分享的是android aidl的使用。在Android中, 每个应用程序都可以有自己的进程. 在写UI应用的时候, 经常要用到Service. 在不同的进程中, 怎样传递对象呢? 显然, Java中不允许跨进程内存共享. 因此传递对象, 只能把对象拆分成操作系统能理解的简单形式, 以达到跨界对象访问的目的. 在J2EE中,采用RMI的方式, 可以通过序列化传递对象. 在Android中, 则采用AIDL的方式. 理论上AIDL可以传递Bundle,实际上做起来却比较麻烦.  
AIDL(AndRoid接口描述语言)是一种借口描述语言; 编译器可以通过aidl文件生成一段代码，通过预先定义的接口达到两个进程内部通信进程的目的. 如果需要在一个Activity中, 访问另一个Service中的某个对象, 需要先将对象转化成AIDL可识别的参数(可能是多个参数), 然后使用AIDL来传递这些参数, 在消息的接收端, 使用这些参数组装成自己需要的对象.   
AIDL的IPC的机制和COM或CORBA类似, 是基于接口的，但它是轻量级的。它使用代理类在客户端和实现层间传递值. 如果要使用AIDL, 需要完成2件事情: 1. 引入AIDL的相关类.; 2. 调用aidl产生的class.  
今天的两个实例用到两个Android工程,一个是AIDL的服务端另一个是客户端。
**服务端的实现步骤:**
首先看一下服务端，工程目录如下:
![](http://hi.csdn.net/attachment/201105/17/0_1305633259ja0j.gif)
首先创建IaidlServerService.aidl文件，代码如下(一个简单方法，另一个返回对象方法),当我们点击保存时会在gen目录下生成对应的java文件，如上图红色部分:
**[java]**[view
 plain](http://blog.csdn.net/android_tutor/article/details/6427680#)[copy](http://blog.csdn.net/android_tutor/article/details/6427680#)
- package com.chapter8.aidl;  
- import com.chapter8.aidl.Book;  
- interface IAIDLServerService {   
- 
-     String sayHello();  
- 
-     Book getBook();  
- }  
第二步:因为这个接口里有传递对象，所以对象要特殊处理一下，这里继承了Parcelable,Book.java代码如下:
如果大家不明的地方，请参见，下面连接的文章:
# [Android高手进阶教程(十七)之---Android中Intent传递对象的两种方法(Serializable,Parcelable)!](http://blog.csdn.net/Android_Tutor/archive/2010/07/16/5740845.aspx)
**[java]**[view
 plain](http://blog.csdn.net/android_tutor/article/details/6427680#)[copy](http://blog.csdn.net/android_tutor/article/details/6427680#)
- package com.chapter8.aidl;  
- import android.os.Parcel;  
- import android.os.Parcelable;  
- publicclass Book implements Parcelable {  
- 
- private String bookName;  
- privateint bookPrice;  
- 
- public Book(){  
- 
-     }  
- 
- public Book(Parcel parcel){  
-         bookName = parcel.readString();  
-         bookPrice = parcel.readInt();  
-     }  
- 
- public String getBookName() {  
- return bookName;  
-     }  
- publicvoid setBookName(String bookName) {  
- this.bookName = bookName;  
-     }  
- publicint getBookPrice() {  
- return bookPrice;  
-     }  
- publicvoid setBookPrice(int bookPrice) {  
- this.bookPrice = bookPrice;  
-     }  
- 
- publicint describeContents() {  
- return0;  
-     }  
- publicvoid writeToParcel(Parcel parcel, int flags) {  
-         parcel.writeString(bookName);  
-         parcel.writeInt(bookPrice);  
-     }  
- 
- publicstaticfinal Parcelable.Creator<Book> CREATOR = new Creator<Book>() {  
- public Book createFromParcel(Parcel source) {  
- 
- returnnew Book(source);  
-         }  
- public Book[] newArray(int size) {  
- returnnew Book[size];  
-         }  
-     };  
- }  
第三步:写一个与Book类对应的aidl，命名为Book.aidl,代码非常简单,代码如下:
**[java]**[view
 plain](http://blog.csdn.net/android_tutor/article/details/6427680#)[copy](http://blog.csdn.net/android_tutor/article/details/6427680#)
- parcelable Book;  
第四步:新建一个名为AidlServerService的Service.代码如下:
**[java]**[view
 plain](http://blog.csdn.net/android_tutor/article/details/6427680#)[copy](http://blog.csdn.net/android_tutor/article/details/6427680#)
- package com.chapter8.aidl;  
- import com.chapter8.aidl.IAIDLServerService.Stub;  
- import com.chapter8.aidl.IAIDLServerService;  
- import android.app.Service;  
- import android.content.Intent;  
- import android.os.IBinder;  
- import android.os.RemoteException;  
- publicclass AidlServerService extends Service {  
- @Override
- public IBinder onBind(Intent intent) {  
- return mBinder;  
-     }  
- /**
-      * 在AIDL文件中定义的接口实现。
-      */
- private IAIDLServerService.Stub mBinder = new Stub() {  
- 
- public String sayHello() throws RemoteException {  
- return"Hello";  
-         }  
- 
- public Book getBook() throws RemoteException {  
-             Book mBook = new Book();  
-             mBook.setBookName("Android应用开发");  
-             mBook.setBookPrice(50);  
- return mBook;  
-         }  
-     };  
- }  
第五步:在AndroidManifest.xml注册Service，代码如下:
**[java]**[view
 plain](http://blog.csdn.net/android_tutor/article/details/6427680#)[copy](http://blog.csdn.net/android_tutor/article/details/6427680#)
- <?xml version="1.0" encoding="utf-8"?>  
- <manifest xmlns:android="http://schemas.android.com/apk/res/android"
- package="com.chapter8.aidl"
-       android:versionCode="1"
-       android:versionName="1.0">  
-     <application android:icon="@drawable/icon" android:label="@string/app_name">  
-         <activity android:name="AidlServerActivity"
-                   android:label="@string/app_name">  
-             <intent-filter>  
-                 <action android:name="android.intent.action.MAIN" />  
-                 <category android:name="android.intent.category.LAUNCHER" />  
-             </intent-filter>  
-         </activity>  
-         <service android:name="AidlServerService"
-                  android:process=":remote">  
-             <intent-filter>  
-                 <action android:name="com.chapter8.aidl.IAIDLServerService"></action>  
-             </intent-filter>  
-         </service>  
-     </application>  
- </manifest>  
第六步:运行服务端工程，到设备上，好让客户端调用，服务端的Activity什么都没做.效果如下:
![](http://hi.csdn.net/attachment/201105/17/0_1305635303vU0v.gif)
**客户端的具体实现步骤:**
第一步:新建客户端工程，目录结构如下:
![](http://hi.csdn.net/attachment/201105/17/0_1305634735obop.gif)
第二步:引入Aidl文件以及用到的类，如上面的com.chapter8.aidl包。直接从服务端里代码copy过来就OK.
第三步:修改main.xml布局文件，增加一个按钮，代码如下:
**[java]**[view
 plain](http://blog.csdn.net/android_tutor/article/details/6427680#)[copy](http://blog.csdn.net/android_tutor/article/details/6427680#)
- <?xml version="1.0" encoding="utf-8"?>  
- <LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
-     android:orientation="vertical"
-     android:layout_width="fill_parent"
-     android:layout_height="fill_parent"
-     >  
- <TextView    
-     android:id="@+id/textview"
-     android:layout_width="fill_parent"
-     android:layout_height="wrap_content"
-     android:text="@string/hello"
-     />  
- <Button  
-     android:id="@+id/button"
-     android:layout_width="fill_parent"
-     android:layout_height="wrap_content"
-     android:text="调用AIDL服务"
-     />  
- </LinearLayout>  
第四步:修改AidlClientActivity.java代码如下:
**[java]**[view
 plain](http://blog.csdn.net/android_tutor/article/details/6427680#)[copy](http://blog.csdn.net/android_tutor/article/details/6427680#)
- package com.chapter8.aidlclient;  
- import com.chapter8.aidl.IAIDLServerService;  
- import android.app.Activity;  
- import android.content.ComponentName;  
- import android.content.Intent;  
- import android.content.ServiceConnection;  
- import android.os.Bundle;  
- import android.os.IBinder;  
- import android.os.RemoteException;  
- import android.view.View;  
- import android.view.View.OnClickListener;  
- import android.widget.Button;  
- import android.widget.TextView;  
- publicclass AidlClientActivity extends Activity {  
- 
- private TextView mTextView;  
- private Button mButton;  
- 
- private IAIDLServerService mIaidlServerService = null;  
- 
- private ServiceConnection mConnection = new ServiceConnection() {  
- 
- publicvoid onServiceDisconnected(ComponentName name) {  
-             mIaidlServerService = null;  
-         }     
- publicvoid onServiceConnected(ComponentName name, IBinder service) {  
-             mIaidlServerService = IAIDLServerService.Stub.asInterface(service);  
- //aidl通信
- try {  
-                 String mText = "Say hello: " + mIaidlServerService.sayHello() + "/n";  
-                 mText += "书名: " + mIaidlServerService.getBook().getBookName()+"/n";  
-                 mText += "价格: " + mIaidlServerService.getBook().getBookPrice();  
-                 mTextView.setText(mText);  
-             } catch (RemoteException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     };  
- 
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);        
- //初始化控件
-         mTextView = (TextView)findViewById(R.id.textview);  
-         mButton = (Button)findViewById(R.id.button);  
- //增加事件响应
-         mButton.setOnClickListener(new OnClickListener(){  
- publicvoid onClick(View v) {  
- //bindService
-                 Intent service = new Intent("com.chapter8.aidl.IAIDLServerService");  
-                 bindService(service, mConnection,BIND_AUTO_CREATE);                           
-             }  
- 
-         });  
-     }  
- 
- 
- }  
第五步：运行客户端工程，效果如下:
![](http://hi.csdn.net/attachment/201105/17/0_1305635259KAm2.gif)![](http://hi.csdn.net/attachment/201105/17/0_1305635269WqzM.gif)
Ok，上面就完整的作了一个Aidl开发的流程，有什么不对的地方希望大家多多指教，今天就讲到这里
///////////////////////
“AIDL服务只支持有限的数据类型，因此，如果用AIDL服务传递一些复杂的数据就需要做更一步处理。AIDL服务支持的数据类型如下：
Java的简单类型（int、char、boolean等）。不需要导入（import）。
String和CharSequence。不需要导入（import）。
List和Map。但要注意，List和Map对象的元素类型必须是AIDL服务支持的数据类型。不需要导入（import）。
AIDL自动生成的接口。需要导入（import）。
实现android.os.Parcelable接口的类。需要导入（import）。
其中后两种数据类型需要使用import进行导入，将在本章的后面详细介绍。
传递不需要import的数据类型的值的方式相同。传递一个需要import的数据类型的值（例如，实现android.os.Parcelable接口的类）的步骤略显复杂。除了要建立一个实现android.os.Parcelable接口的类外，还需要为这个类单独建立一个aidl文件，并使用parcelable关键字进行定义。”
///////////////////////////////////////////////////////////////////////////////////////
### [AIDL 无法自动生成aidl.java](http://blog.csdn.net/xiaominmincs/article/details/7991142)
分类： [个人小笔记](http://blog.csdn.net/xiaominmincs/article/category/1238288)2012-09-18
 12:35131人阅读[评论](http://blog.csdn.net/xiaominmincs/article/details/7991142#comments)(0)[收藏]()[举报](http://blog.csdn.net/xiaominmincs/article/details/7991142#report)
       在做进程间通信时，使用系统aidl文件。 我把这个文件夹（包括aidl文件）拷贝到项目， 居然不自动生成aidl.java.
1、解决方案： 在aidl包下、新建一个合格的aidl文件， 新建的aidl可以自动生成aidl.java。  再删除系统aidl.     再粘贴系统aidl 放到包里。他就会自动生成aidl.java了。
![](https://img-my.csdn.net/uploads/201209/18/1347943179_7911.jpg)
[http://www.cnblogs.com/sipher/archive/2012/08/23/2652196.html](http://www.cnblogs.com/sipher/archive/2012/08/23/2652196.html)
最近有一个问题，困扰了很久都没有解决，就是在使用aidl进行进程间通信时，我的开发环境无法自动在gen目录下生成java文件。
我用的是ubuntu的操作系统。而同样的source在其他同事的开发环境下都是可以的。
检查sdk开发包安装都没有问题，platform-tools文件夹下aidl的编译工具也安静的躺在那边，怎么就不工作呢？薪水太少？？
在android发布了4.1开发包以后，发现tools和platform-tools都有更新安装，于是想刚好趁这个机会看能不能把问题解决。
于是更新安装最新的tools和platform-tools。clean一下project，重新build，竟然报错了，之前一直都没有报错的。
第一感觉，完了！
之前aidl无法自动生成java，可以从同事那边拷贝来用。现在项目报错，gen下面的所有文件都无法自动生成！！！
难道要逼我重做开发环境？没办法。跟他拼了！！！于是在mac机上试做一个开发环境，安装最新版的SDK，同样的错误！！
看来问题已经严重到即使我重做开发环境也不会解决的程度。狭路相逢，只有解决掉他了。
报的错误是让我去看他workspace下的log。
 Failed to run com.android.ide.eclipse.adt.internal.build.AidlProcessor. Check workspace log for detail.
于是，打开workspace/.metadata/.log
!ENTRY com.android.ide.eclipse.adt 4 0 2012-08-23 11:09:52.961
!MESSAGE Failed to run com.android.ide.eclipse.adt.internal.build.AidlProcessor. Check workspace log for detail.
!STACK 0
java.lang.NullPointerException
    at com.android.ide.eclipse.adt.internal.build.AidlProcessor.doCompileFiles(AidlProcessor.java:113)
    at com.android.ide.eclipse.adt.internal.build.SourceProcessor.compileFiles(SourceProcessor.java:244)
    at com.android.ide.eclipse.adt.internal.build.builders.PreCompilerBuilder.build(PreCompilerBuilder.java:638)
    at org.eclipse.core.internal.events.BuildManager$2.run(BuildManager.java:629)
    at org.eclipse.core.runtime.SafeRunner.run(SafeRunner.java:42)
    at org.eclipse.core.internal.events.BuildManager.basicBuild(BuildManager.java:172)
    at org.eclipse.core.internal.events.BuildManager.basicBuild(BuildManager.java:203)
    at org.eclipse.core.internal.events.BuildManager$1.run(BuildManager.java:255)
    at org.eclipse.core.runtime.SafeRunner.run(SafeRunner.java:42)
    at org.eclipse.core.internal.events.BuildManager.basicBuild(BuildManager.java:258)
    at org.eclipse.core.internal.events.BuildManager.basicBuildLoop(BuildManager.java:311)
    at org.eclipse.core.internal.events.BuildManager.build(BuildManager.java:343)
    at org.eclipse.core.internal.events.AutoBuildJob.doBuild(AutoBuildJob.java:144)
    at org.eclipse.core.internal.events.AutoBuildJob.run(AutoBuildJob.java:242)
    at org.eclipse.core.internal.jobs.Worker.run(Worker.java:54)
于是到google上查找原因。有人有同样的问题，下面的解答中有下面这句话。
I had this exact issue happen to me today. It turns out this happens when there are source folders in your project that do not actually exist.
So verify in your project Java Build Path that all source folders actually exist. After removing a dead source folder link from there my project once again compiled
 the aidl.
于是试着去看看我项目中有没有这样的文件夹，赫然发现有一个本项目依存的另一个项目名称的文件夹，里面是空。
因为调试程式的原因，把本来jar文件的原始项目引进来，不知道怎么就生成了这么个文件夹。
情况不会比现在更糟糕了，于是也不管他会不会用到，直接删除。
报错不见了！！！gen文件夹下文件通通自动生成了。到此问题解决。
**官网介绍：**
**[http://developer.android.com/guide/components/aidl.html#PassingObjects](http://developer.android.com/guide/components/aidl.html#PassingObjects)**
