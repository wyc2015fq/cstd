# Android-Adding SystemService - xqhrs232的专栏 - CSDN博客
2012年05月16日 10:58:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：739标签：[service																[exception																[transactions																[application																[class																[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=transactions&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://processors.wiki.ti.com/index.php/Android-Adding_SystemService](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService)
This wiki page will demonstrate - "How to add system service to android framework". Example - "Adding a Bluetooth HID service" - taken as reference of understanding.This will also help to add support for more bluetooth profiles into android framework.
## Contents
[[hide]()]
- [1What is service?](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService#What_is_service.3F)
- [2Service layer](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService#Service_layer)
- [3Create service](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService#Create_service)
- [4Register service](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService#Register_service)
- [5Expose service](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService#Expose_service)
- [6Add [service].aidl for build](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService#Add_.5Bservice.5D.aidl_for_build)
- [7Using service](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService#Using_service)
- [8References](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService#References)
- [9Support](http://processors.wiki.ti.com/index.php/Android-Adding_SystemService#Support)
## What is service? 
As per the definition given at [http://developer.android.com/guide/topics/fundamentals/services.html](http://developer.android.com/guide/topics/fundamentals/services.html)
**A Service** is an application component that can perform long-running operations in the background and does not provide a user interface. Another application component can start a service and it will continue to run in the background even
 if the user switches to another application. Additionally, a component can bind to a service to interact with it and even perform interprocess communication (IPC). For example, a service might handle network transactions, play music, perform file I/O, or interact
 with a content provider, all from the background. 
## Service layer 
![Customservice.png](http://processors.wiki.ti.com/images/4/41/Customservice.png)
## Create service 
1. Add your code to frameworks/base/services/java/com/android/server/
/*TestService.java */
package com.android.server;
import android.content.Context;
import android.os.Handler;
import android.os.ITestService;
import android.os.Looper;
import android.os.Message;
import android.os.Process;
import android.util.Log;
public class TestService extends ITestService.Stub {
    private static final String TAG = "TestService";
    private TestWorkerThread mWorker;
    private TestWorkerHandler mHandler;
    private Context mContext;
    public TestService(Context context) {
		super();
		mContext = context;
		mWorker = new TestWorkerThread("TestServiceWorker");
		mWorker.start();
		Log.i(TAG, "Spawned worker thread");
   }
   
	public void setValue(int val) {
		Log.i(TAG, "setValue " + val);
		Message msg = Message.obtain();
		msg.what = TestWorkerHandler.MESSAGE_SET;
		msg.arg1 = val;
		mHandler.sendMessage(msg);
    }
	private class TestWorkerThread extends Thread{
		public TestWorkerThread(String name) {
    		super(name);
		}
		public void run() {
		    Looper.prepare();
		    mHandler = new TestWorkerHandler();
		    Looper.loop();
		}
	}
	
	private class TestWorkerHandler extends Handler {
		private static final int MESSAGE_SET = 0;
		@Override
		public void handleMessage(Message msg) {
		    try {
					if (msg.what == MESSAGE_SET) {
				    Log.i(TAG, "set message received: " + msg.arg1);
					}
	    		}catch (Exception e) {
					// Log, don't crash!
					Log.e(TAG, "Exception in TestWorkerHandler.handleMessage:", e);
    			}
		}
    }
}
## Register service 
- Register service in SystemServer.java 
/*
* go to function "@Override public void run()"
* ........ 
* Add following block after line "if (factoryTest != SystemServer.FACTORY_TEST_LOW_LEVEL) {"  
*/
try {
        Slog.i(TAG, "Test Service");
        ServiceManager.addService(“Test”, new TestService(context));
    } catch (Throwable e) {
        Slog.e(TAG, "Failure starting TestService Service", e);
    }
## Expose service 
- A service can expose set of functions that can be access by other process/application. Exposed functions are required to be declared in .aidl file at following location
frameworks/base/core/java/android/os/[server].aidl 
/*
* aidl file : frameworks/base/core/java/android/os/ITestService.aidl
* This file contains definitions of functions which are exposed by service 
*/
package android.os;
interface ITestService {
/**
* {@hide}
*/
	void setValue(int val);
}
## Add [service].aidl for build
/*
* open frameworks/base/Android.mk and add following line
*/
...
core/java/android/os/IPowerManager.aidl \
core/java/android/os/ITestService.aidl \
core/java/android/os/IRemoteCallback.aidl \
...
- Rebuild the framework/base or android system.Service is now ready to use by other application/process.
## Using service 
To use service 
- first get service handle using "ServiceManager.getService()" api 
- use service handle to call set of functions exposed by service 
Below is the sample code to use service. 
/*
* HelloServer.java
*/
package com.Test.helloserver;
import android.app.Activity;
import android.os.Bundle;
import android.os.ServiceManager;
import android.os.ITestService;
import android.util.Log;
public class HelloServer extends Activity {
    private static final String DTAG = "HelloServer";
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        ITestService om = ITestService.Stub.asInterface(ServiceManager.getService("Test"));
        try {
			    Log.d(DTAG, "Going to call service");
			    om.setValue(20);
			    Log.d(DTAG, "Service called succesfully");
        }
        catch (Exception e) {
			    Log.d(DTAG, "FAILED to call service");
    			e.printStackTrace();
        }
    }
}
## References 
- [http://developer.android.com/reference/android/app/Service.html](http://developer.android.com/reference/android/app/Service.html)
- [http://developer.android.com/guide/topics/fundamentals/services.html](http://developer.android.com/guide/topics/fundamentals/services.html)
- [http://www.opersys.com/blog/esc-india-2011-wrapup](http://www.opersys.com/blog/esc-india-2011-wrapup)
## Support 
For community support join [http://groups.google.com/group/rowboat](http://groups.google.com/group/rowboat)
For IRC #rowboat on irc.freenode.net 

