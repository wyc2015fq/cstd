# android Service Activity三种交互方式（付源码） - xqhrs232的专栏 - CSDN博客
2011年03月14日 11:08:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2521标签：[android																[service																[import																[application																[thread																[encoding](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://yangguangfu.javaeye.com/blog/699306](http://yangguangfu.javaeye.com/blog/699306)

### android Service Binder交互通信实例
最下边有源代码：
android SDK提供了Service，用于类似*nix守护进程或者windows的服务。
Service有两种类型：
- 本地服务（Local Service）：用于应用程序内部 
- 远程服务（Remote Sercie）：用于android系统内部的应用程序之间 
前者用于实现应用程序自己的一些耗时任务，比如查询升级信息，并不占用应用程序比如Activity所属线程，而是单开线程后台执行，这样用户体验比较好。
后者可被其他应用程序复用，比如天气预报服务，其他应用程序不需要再写这样的服务，调用已有的即可。
### 编写不需和Activity交互的本地服务示例
本地服务编写比较简单。首先，要创建一个Service类，该类继承android的Service类。这里写了一个计数服务的类，每秒钟为计数器加一。在服务类的内部，还创建了一个线程，用于实现后台执行上述业务逻辑。
packagecom.easymorse;
importandroid.app.Service;
importandroid.content.Intent;
importandroid.os.IBinder;
importandroid.util.Log;
publicclassCountService extendsService {
private
booleanthreadDisable;
private
intcount;
    @Override
public
IBinder onBind(Intent intent) {
return
null;
    }
    @Override
public
voidonCreate() {
super
.onCreate();
new
Thread(new
Runnable() {
            @Override
public
voidrun() {
while
(!threadDisable) {
try
{
                        Thread.sleep(1000);
                    } catch
(InterruptedException e) {
                    }
                    count++
;
                    Log.v("
CountService"
, "Count is
"+
count);
                }
            }
        }).start();
    }
    @Override
public
voidonDestroy() {
super
.onDestroy();
this
.threadDisable =
true;
        Log.v("
CountService"
, "on destroy");
    }
public
intgetCount() {
return
count;
    }
}
 需要将该服务注册到配置文件AndroidManifest.xml中，否则无法找到：
<?
xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.easymorse"
android:versionCode="1"
android:versionName="1.0"
>
<application
android:icon
="@drawable/icon"android:label
="@string/app_name">
<activity
android:name
=".LocalServiceDemoActivity"
            android:label="@string/app_name">
<
intent-filter>
<
action android:name
="android.intent.action.MAIN"/>
<
category android:name
="android.intent.category.LAUNCHER"/>
</
intent-filter>
</
activity>
<service
android:name
="CountService"/>
</application>
<uses-sdk
android:minSdkVersion
="3"/>
</manifest/>
在Activity中启动和关闭本地服务。
packagecom.easymorse;
importandroid.app.Activity;
importandroid.content.Intent;
importandroid.os.Bundle;
publicclassLocalServiceDemoActivity extendsActivity {
/**Called when the activity is first created.
*/
    @Override
public
voidonCreate(Bundle savedInstanceState) {
super
.onCreate(savedInstanceState);
        setContentView(R.layout.main);
this
.startService(new
Intent(this
, CountService.class
));
    }
    @Override
protected
voidonDestroy() {
super
.onDestroy();
this
.stopService(new
Intent(this
, CountService.class
));
    }
}
可通过日志查看到后台线程打印的计数内容。
### 编写本地服务和Activity交互的示例
上面的示例是通过startService和stopService启动关闭服务的。适用于服务和activity之间没有调用交互的情况。如果之间需要传递参数或者方法调用。需要使用bind和unbind方法。
具体做法是，服务类需要增加接口，比如ICountService，另外，服务类需要有一个内部类，这样可以方便访问外部类的封装数据，这个内部类需要继承Binder类并实现ICountService接口。还有，就是要实现Service的onBind方法，不能只传回一个null了。
这是新建立的接口代码：
packagecom.easymorse;
publicinterfaceICountService {
public
abstractint
getCount();
}
修改后的CountService代码：
packagecom.easymorse;
importandroid.app.Service;
importandroid.content.Intent;
importandroid.os.Binder;
importandroid.os.IBinder;
importandroid.util.Log;
publicclassCountService extendsService implementsICountService {
private
booleanthreadDisable;
private
intcount;
private
ServiceBinder serviceBinder=
newServiceBinder();
public
classServiceBinder extendsBinder implementsICountService{
        @Override
public
intgetCount() {
return
count; 
        }
    } 
    @Override 
publicIBinder onBind(Intent intent) {
return
serviceBinder;
    }
    @Override
public
voidonCreate() {
super
.onCreate();
new
Thread(new
Runnable() {
            @Override
public
voidrun() {
while
(!threadDisable) {
try
{
                        Thread.sleep(1000);
                    } catch
(InterruptedException e) {
                    }
                    count++
;
                    Log.v("
CountService"
, "Count is
"+
count);
                }
            }
        }).start();
    }
    @Override
public
voidonDestroy() {
super
.onDestroy();
this
.threadDisable =
true;
        Log.v("
CountService"
, "on destroy");
    }
/*(non-Javadoc)
     * @see com.easymorse.ICountService#getCount()
*/
public
intgetCount() {
return
count;
    }
}
服务的注册也要做改动，AndroidManifest.xml文件：
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="com.easymorse"
android:versionCode="1"
android:versionName="1.0"
>
<application
android:icon
="@drawable/icon"android:label
="@string/app_name">
<activity
android:name
=".LocalServiceDemoActivity"
            android:label="@string/app_name">
<
intent-filter>
<
action android:name
="android.intent.action.MAIN"/>
<
category android:name
="android.intent.category.LAUNCHER"/>
</
intent-filter>
</
activity>
<service
android:name
="CountService">
<
intent-filter>
<
action android:name
="com.easymorse.CountService"/>
</
intent-filter>
</
service>
</application>
<uses-sdk
android:minSdkVersion
="3"/>
</manifest>
Acitity代码不再通过startSerivce和stopService启动关闭服务，另外，需要通过ServiceConnection的内部类实现来连接Service和Activity。
packagecom.easymorse;
importandroid.app.Activity;
importandroid.content.ComponentName;
importandroid.content.Intent;
importandroid.content.ServiceConnection;
importandroid.os.Bundle;
importandroid.os.IBinder;
importandroid.util.Log;
publicclassLocalServiceDemoActivity extendsActivity {
private
ServiceConnection serviceConnection =
newServiceConnection() {
        @Override
public
voidonServiceConnected(ComponentName name, IBinder service) {
            countService =
(ICountService) service;
            Log.v("
CountService"
, "on serivce connected, count is
"
+
countService.getCount());
        }
        @Override
public
voidonServiceDisconnected(ComponentName name) {
            countService =
null;
        }
    };
private
ICountService countService;
/**Called when the activity is first created.
*/
    @Override
public
voidonCreate(Bundle savedInstanceState) {
super
.onCreate(savedInstanceState);
        setContentView(R.layout.main);
this
.bindService(new
Intent("com.easymorse.CountService"),
this
.serviceConnection, BIND_AUTO_CREATE);
    }
    @Override
protected
voidonDestroy() {
          this
.unbindService(serviceConnection);
          super
.onDestroy();       //注意先后
    }
}
### 编写传递基本型数据的远程服务
上面的示例，可以扩展为，让其他应用程序复用该服务。这样的服务叫远程（remote）服务，实际上是进程间通信（RPC）。
这时需要使用android接口描述语言（AIDL）来定义远程服务的接口，而不是上述那样简单的java接口。扩展名为aidl而不是java。可用上面的ICountService改动而成ICountSerivde.aidl，eclipse会自动生成相关的java文件。
package com.easymorse;
interfaceICountService {
intgetCount();
}
编写服务（Service）类，稍有差别，主要在binder是通过远程获得的，需要通过桩（Stub）来获取。桩对象是远程对象的本地代理。
packagecom.easymorse;
importandroid.app.Service;
importandroid.content.Intent;
importandroid.os.IBinder;
importandroid.os.RemoteException;
importandroid.util.Log;
publicclassCountService extendsService {
private
booleanthreadDisable;
private
intcount;
private
ICountService.Stub serviceBinder =
newICountService.Stub() {
        @Override
public
intgetCount() throwsRemoteException {
return
count;
        }
    };
    @Override
public
IBinder onBind(Intent intent) {
return
serviceBinder;
    }
    @Override
public
voidonCreate() {
super
.onCreate();
new
Thread(new
Runnable() {
            @Override
public
voidrun() {
while
(!threadDisable) {
try
{
                        Thread.sleep(1000);
                    } catch
(InterruptedException e) {
                    }
                    count++
;
                    Log.v("
CountService"
, "Count is
"+
count);
                }
            }
        }).start();
    }
    @Override
public
voidonDestroy() {
super
.onDestroy();
this
.threadDisable =
true;
        Log.v("
CountService"
, "on destroy");
    }
}
配置文件AndroidManifest.xml和上面的类似，没有区别。
在Activity中使用服务的差别不大，只需要对ServiceConnection中的调用远程服务的方法时，要捕获异常。
privateServiceConnection serviceConnection
=newServiceConnection() {
    @Override
public
voidonServiceConnected(ComponentName name, IBinder service) {
        countService =
(ICountService) service;
try
{
            Log.v("
CountService"
, "on serivce connected, count is
"
+
countService.getCount());
        } catch
(RemoteException e) {
throw
newRuntimeException(e);
        }
    }
    @Override
public
voidonServiceDisconnected(ComponentName name) {
        countService =
null;
    }
};
这样就可以在同一个应用程序中使用远程服务的方式和自己定义的服务交互了。
如果是另外的应用程序使用远程服务，需要做的是复制上面的aidl文件和相应的包构到应用程序中，其他调用等都一样。
### 编写传递复杂数据类型的远程服务
远程服务往往不只是传递java基本数据类型。这时需要注意android的一些限制和规定：
- android支持String和CharSequence 
- 如果需要在aidl中使用其他aidl接口类型，需要import，即使是在相同包结构下； 
- android允许传递实现Parcelable接口的类，需要import； 
- android支持集合接口类型List和Map，但是有一些限制，元素必须是基本型或者上述三种情况，不需要import集合接口类，但是需要对元素涉及到的类型import；
- 非基本数据类型，也不是String和CharSequence类型的，需要有方向指示，包括in、out和inout，in表示由客户端设置，out表示由服务端设置，inout是两者均可设置。
这里将前面的例子中返回的int数据改为复杂数据类型：
packagecom.easymorse;
importandroid.os.Parcel;
importandroid.os.Parcelable;
publicclassCountBean implementsParcelable {
public
staticfinal
Parcelable.Creator<
CountBean>
CREATOR =newCreator<
CountBean>
() {
        @Override
public
CountBean createFromParcel(Parcel source) {
            CountBean bean =
newCountBean();
            bean.count =
source.readInt();
return
bean;
        }
        @Override
public
CountBean[] newArray(int
size) {
return
newCountBean[size];
        }
    };
public
intcount;
    @Override
public
voidwriteToParcel(Parcel dest, intflags) {
        dest.writeInt(this
.count);
    }
    @Override
public
intdescribeContents() {
return
0;
    }
}
然后，需要在相同包下建一个同名的aidl文件，用于android生成相应的辅助文件：
packagecom.easymorse;
parcelable CountBean;
这一步是android 1.5后的变化，无法通过adt生成aidl，也不能用一个比如全局的project.aidl文件，具体见：
> 
[http://www.anddev.org/viewtopic.php?p=20991](http://www.anddev.org/viewtopic.php?p=20991)
然后，需要在服务的aidl文件中修改如下：
packagecom.easymorse;
importcom.easymorse.CountBean;
interfaceICountService {
    CountBean getCount();
}
其他的改动很小，只需将CountService和调用CountService的部分修改为使用CountBean即可
