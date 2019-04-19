# 关于Android 添加系统级(java)服务和调用的编写实现说明 - xqhrs232的专栏 - CSDN博客
2012年05月11日 13:52:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2338标签：[java																[android																[service																[frameworks																[string																[import](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=frameworks&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/jinliang_890905/article/details/7320234](http://blog.csdn.net/jinliang_890905/article/details/7320234)
相关网贴
1.Android系统自带的Service原理与使用----[http://www.61ic.com/Mobile/Android/201102/29421.html](http://www.61ic.com/Mobile/Android/201102/29421.html)
2.Android 支持的系统级服务表  ----[http://tigerszdf.blog.163.com/blog/static/45955133201082042931710/](http://tigerszdf.blog.163.com/blog/static/45955133201082042931710/)
3.Android系统服务一览----[http://www.360doc.com/content/11/0305/14/3639038_98345450.shtml](http://www.360doc.com/content/11/0305/14/3639038_98345450.shtml)
关于Android 添加系统级(java)服务和调用的编写实现说明：
	本文当主要描述Android添加java 系统级服务的编写实现。
一.添加java 系统级服务：
      目前网站上介绍的比较多，但是没有很详细的步骤和流程。现总结如下：
	以 AlarmManagerService为例
	Java层服务顾名思义即为从 Java层提供的服务，它与 C++层的服务不同之处在于其服务进程的运行由系统（ ServiceManager）统一维护。在文件 frameworks /base /services /java /com /android /server /SystemServer.java 中我们可以看到以下代码：
AlarmManagerService alarm = new AlarmManagerService (context );
ServiceManager .addService (Context .ALARM_SERVICE , alarm );
         这个操作在系统启动时完成，由 ServiceManager 负责创建服务进程并运行之。
        在目录/frameworks/base/core/java中IAlarmManager.aidl文件.
	在目录frameworks/base/services/java/com/android/server中AlarmManagerService的aidl的实现类，
	在目录frameworks/base/services/java/com/android/server增加 service的实现文件.
二.实验测试添加java 系统级服务步骤：
      1.增加aidl文件
	在目录/frameworks/base/core/java/android/app下增加中IDvbService.aidl.
package android.app;
interface IDvbService {
	int countTest(in int testvalue);
}
	2.在目录frameworks/base/services/java/com/android/server中增加DvbManagerService实现IDvbService.aidl类
package com.android.server;
import android.content.Context;
import android.util.Slog;
import android.app.IDvbService;
class DvbManagerService extends IDvbService.Stub {
 
    private static final String TAG = "DvbManagerService";
    private static final boolean localLOGV = false;
    private final Context mContext;
    
    public DvbManagerService(Context context) {
        super();
        mContext = context;
        Slog.w(TAG, "DvbManagerService");
    }
    
    public int countTest(int value){
    	return value*2;
    }
    
}
      3.在文件 frameworks /base /services /java /com /android /server /SystemServer.java 类中run（）方法中增加
 		Slog.i(TAG, "Alarm DvbManagerService");
            DvbManagerService dvb = new DvbManagerService(context);//dvb
            Slog.i(TAG, "Alarm DvbManagerService dvb ="+dvb);
            ServiceManager.addService(Context.DVB_SERVICE, dvb);
 	4.在文件/frameworks/base/core/java/android/content/Context.java中增加
	public static final String DVB_SERVICE = "dvb";//dvb
	5.在文件/frameworks/base/core/java/android/app中增加DvbManager.java
package android.app;
import android.content.Context;
import android.content.Intent;
import android.os.RemoteException;
import android.os.ServiceManager;
public class DvbManager{
	//dvb
    private final IDvbService mService;
    /**
     * package private on purpose
     */
    DvbManager(IDvbService service) {
        mService = service;
    }
    
    public int count(int value) {
        try {
        	return mService.countTest(value);
        } catch (RemoteException ex) {
        }
        return -1;
    }
    
}
	6.在文件/frameworks/base/core/java/android/app/ContextImpl.java中
新增加属性
    private static DvbManager sDvbManager;//dvb
在getSystemService(String name)方中增加
else if (DVB_SERVICE.equals(name)) {//dvb
	        return getDvbManager();
	    } 
另外增加方法
    private DvbManager getDvbManager() {//dvb
    	Log.e(TAG, "getDvbManager sSync = "+sSync);
    	Log.e(TAG, "getDvbManager sDvbManager = "+sDvbManager);
        synchronized (sSync) {
            if (sDvbManager == null) {
                IBinder b = ServiceManager.getService(DVB_SERVICE);
            	Log.e(TAG, "getDvbManager IBinder = "+b);
                IDvbService service = IDvbService.Stub.asInterface(b);
            	Log.e(TAG, "getDvbManager IDvbService = "+service);
                sDvbManager = new DvbManager(service);
            }
        }
        return sDvbManager;
    }
	7.在/frameworks/base/Android.mk中
LOCAL_SRC_FILES += \
下增加
	core/java/android/app/IDvbService.aidl \
	8.在应用调用如下：
	导入import android.app.DvbManager;
 
	用法
     DvbManager dvb=(DvbManager)getSystemService(DVB_SERVICE);
	        int count = dvb.count(9);
	        Toast.makeText(
	        		TVMainActivity.this,"dvb.count(9)  back = "+count,
	                Toast.LENGTH_SHORT).show();
	
	9.编译如下:
（1）首先编译 frameworks
. build/envsetup.sh 
choosecombo 
make -j4  framework
编译后会生成framework.jar
（2）然后编译frameworks/base/services/java的service
make -j4  frameworks/base/services/java
编译后会生成services.jar
（3）编译应用
因为服务是自定义的，所有必须在自定义的sdk中编译才能使用。 
//====================================================================
备注：
 1》可以参考系统自带的AlarmManagerService，然后自己照着来一个就基本能成了
 2》不知道怎么编译集成有自己增加的服务接口的SDK包啊？！----要不然APK里面想用自己的服务接口只能把APK写到image镜像里面去了
