# Android应用程序发送广播（sendBroadcast）的过程分析（1） - weixin_33985507的博客 - CSDN博客
2011年09月05日 01:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
       前面我们分析了Android应用程序注册广播接收器的过程，这个过程只完成了万里长征的第一步，接下来它还要等待ActivityManagerService将广播分发过来。ActivityManagerService是如何得到广播并把它分发出去的呢？这就是本文要介绍的广播发送过程了。
        广播的发送过程比广播接收器的注册过程要复杂得多了，不过这个过程仍然是以ActivityManagerService为中心。广播的发送者将广播发送到ActivityManagerService，ActivityManagerService接收到这个广播以后，就会在自己的注册中心查看有哪些广播接收器订阅了该广播，然后把这个广播逐一发送到这些广播接收器中，但是ActivityManagerService并不等待广播接收器处理这些广播就返回了，因此，广播的发送和处理是异步的。概括来说，广播的发送路径就是从发送者到ActivityManagerService，再从ActivityManagerService到接收者，这中间的两个过程都是通过Binder进程间通信机制来完成的，因此，希望读者在继续阅读本文之前，对Android系统的Binder进程间通信机制有所了解，具体可以参考[Android进程间通信（IPC）机制Binder简要介绍和学习计划](http://shyluo.blog.51cto.com/5725845/964526)一文。
        本文继续以[Android系统中的广播（Broadcast）机制简要介绍和学习计划](http://shyluo.blog.51cto.com/5725845/966167)一文中所开发的应用程序为例子，并且结合上文[Android应用程序注册广播接收器（registerReceiver）的过程分析](http://shyluo.blog.51cto.com/5725845/966376)的内容，一起来分析Android应用程序发送广播的过程。
        回顾一下[Android系统中的广播（Broadcast）机制简要介绍和学习计划](http://shyluo.blog.51cto.com/5725845/966167)一文中所开发的应用程序的组织架构，MainActivity向ActivityManagerService注册了一个CounterService.BROADCAST_COUNTER_ACTION类型的计数器服务广播接收器，计数器服务CounterService在后台线程中启动了一个异步任务（AsyncTask），这个异步任务负责不断地增加计数，并且不断地将当前计数值通过广播的形式发送出去，以便MainActivity可以将当前计数值在应用程序的界面线程中显示出来。
        计数器服务CounterService发送广播的代码如下所示：
- publicclass CounterService extends Service implements ICounterService {     
-     ......    
- 
- publicvoid startCounter(int initVal) {     
-         AsyncTask<Integer, Integer, Integer> task = new AsyncTask<Integer, Integer, Integer>() {         
- @Override
- protected Integer doInBackground(Integer... vals) {     
-                 ......     
-             }     
- 
- @Override
- protectedvoid onProgressUpdate(Integer... values) {     
- super.onProgressUpdate(values);     
- 
- int counter = values[0];     
- 
-                 Intent intent = new Intent(BROADCAST_COUNTER_ACTION);     
-                 intent.putExtra(COUNTER_VALUE, counter);     
- 
-                 sendBroadcast(intent);     
-             }     
- 
- @Override
- protectedvoid onPostExecute(Integer val) {     
-                 ......    
-             }     
- 
-         };     
- 
-         task.execute(0);         
-     }     
- 
-     ......   
- }   
        在onProgressUpdate函数中，创建了一个BROADCAST_COUNTER_ACTION类型的Intent，并且在这里个Intent中附加上当前的计数器值，然后通过CounterService类的成员函数sendBroadcast将这个Intent发送出去。CounterService类继承了Service类，Service类又继承了ContextWrapper类，成员函数sendBroadcast就是从ContextWrapper类继承下来的，因此，我们就从ContextWrapper类的sendBroadcast函数开始，分析广播发送的过程。
        在继承分析广播的发送过程前，我们先来看一下广播发送过程的序列图，然后按照这个序图中的步骤来一步一步分析整个过程。
![](http://blog.51cto.com/attachment/201208/151152474.png)
 Step 1. ContextWrapper.sendBroadcast
        这个函数定义在frameworks/base/core/java/android/content/ContextWrapper.java文件中：
- publicclass ContextWrapper extends Context {   
-     Context mBase;   
- 
-     ......   
- 
- @Override
- publicvoid sendBroadcast(Intent intent) {   
-         mBase.sendBroadcast(intent);   
-     }   
- 
-     ......   
- 
- }   
        这里的成员变量mBase是一个ContextImpl实例，这里只简单地调用ContextImpl.sendBroadcast进一行操作。
         Step 2. ContextImpl.sendBroadcast
         这个函数定义在frameworks/base/core/java/android/app/ContextImpl.java文件中：
- class ContextImpl extends Context {   
-     ......   
- 
- @Override
- publicvoid sendBroadcast(Intent intent) {   
-         String resolvedType = intent.resolveTypeIfNeeded(getContentResolver());   
- try {   
-             ActivityManagerNative.getDefault().broadcastIntent(   
-                 mMainThread.getApplicationThread(), intent, resolvedType, null,   
-                 Activity.RESULT_OK, null, null, null, false, false);   
-         } catch (RemoteException e) {   
-         }   
-     }   
- 
-     ......   
- 
- }   
        这里的resolvedType表示这个Intent的MIME类型，我们没有设置这个Intent的MIME类型，因此，这里的resolvedType为null。接下来就调用ActivityManagerService的远程接口ActivityManagerProxy把这个广播发送给ActivityManagerService了。
        Step 3. ActivityManagerProxy.broadcastIntent
        这个函数定义在frameworks/base/core/java/android/app/ActivityManagerNative.java文件中：
- class ActivityManagerProxy implements IActivityManager   
- {   
-     ......   
- 
- publicint broadcastIntent(IApplicationThread caller,   
-         Intent intent, String resolvedType,  IIntentReceiver resultTo,   
- int resultCode, String resultData, Bundle map,   
-         String requiredPermission, boolean serialized,   
- boolean sticky) throws RemoteException   
-     {   
-         Parcel data = Parcel.obtain();   
-         Parcel reply = Parcel.obtain();   
-         data.writeInterfaceToken(IActivityManager.descriptor);   
-         data.writeStrongBinder(caller != null ? caller.asBinder() : null);   
-         intent.writeToParcel(data, 0);   
-         data.writeString(resolvedType);   
-         data.writeStrongBinder(resultTo != null ? resultTo.asBinder() : null);   
-         data.writeInt(resultCode);   
-         data.writeString(resultData);   
-         data.writeBundle(map);   
-         data.writeString(requiredPermission);   
-         data.writeInt(serialized ? 1 : 0);   
-         data.writeInt(sticky ? 1 : 0);   
-         mRemote.transact(BROADCAST_INTENT_TRANSACTION, data, reply, 0);   
-         reply.readException();   
- int res = reply.readInt();   
-         reply.recycle();   
-         data.recycle();   
- return res;   
-     }   
- 
-     ......   
- 
- }   
         这里的实现比较简单，把要传递的参数封装好，然后通过Binder驱动程序进入到ActivityManagerService的broadcastIntent函数中。
         Step 4. ctivityManagerService.broadcastIntent
         这个函数定义在frameworks/base/services/java/com/android/server/am/ActivityManagerService.java文件中：
- publicfinalclass ActivityManagerService extends ActivityManagerNative   
- implements Watchdog.Monitor, BatteryStatsImpl.BatteryCallback {   
-     ......   
- 
- publicfinalint broadcastIntent(IApplicationThread caller,   
-             Intent intent, String resolvedType, IIntentReceiver resultTo,   
- int resultCode, String resultData, Bundle map,   
-             String requiredPermission, boolean serialized, boolean sticky) {   
- synchronized(this) {   
-             intent = verifyBroadcastLocked(intent);   
- 
- final Proce***ecord callerApp = getRecordForAppLocked(caller);   
- finalint callingPid = Binder.getCallingPid();   
- finalint callingUid = Binder.getCallingUid();   
- finallong origId = Binder.clearCallingIdentity();   
- int res = broadcastIntentLocked(callerApp,   
-                 callerApp != null ? callerApp.info.packageName : null,   
-                 intent, resolvedType, resultTo,   
-                 resultCode, resultData, map, requiredPermission, serialized,   
-                 sticky, callingPid, callingUid);   
-             Binder.restoreCallingIdentity(origId);   
- return res;   
-         }   
-     }   
- 
-     ......   
- }   
        这里调用broadcastIntentLocked函数来进一步处理。
         Step 5. ActivityManagerService.broadcastIntentLocked
         这个函数定义在frameworks/base/services/java/com/android/server/am/ActivityManagerService.java文件中：
- publicfinalclass ActivityManagerService extends ActivityManagerNative   
- implements Watchdog.Monitor, BatteryStatsImpl.BatteryCallback {   
-     ......   
- 
- privatefinalint broadcastIntentLocked(Proce***ecord callerApp,   
-             String callerPackage, Intent intent, String resolvedType,   
-             IIntentReceiver resultTo, int resultCode, String resultData,   
-             Bundle map, String requiredPermission,   
- boolean ordered, boolean sticky, int callingPid, int callingUid) {   
-         intent = new Intent(intent);   
- 
-         ......   
- 
- // Figure out who all will receive this broadcast.  
-         List receivers = null;   
-         List<BroadcastFilter> registeredReceivers = null;   
- try {   
- if (intent.getComponent() != null) {   
-                 ......   
-             } else {   
-                 ......   
-                 registeredReceivers = mReceiverResolver.queryIntent(intent, resolvedType, false);   
-             }   
-         } catch (RemoteException ex) {   
-             ......   
-         }   
- 
- finalboolean replacePending =   
-             (intent.getFlags()&Intent.FLAG_RECEIVER_REPLACE_PENDING) != 0;   
- 
- int NR = registeredReceivers != null ? registeredReceivers.size() : 0;   
- if (!ordered && NR > 0) {   
- // If we are not serializing this broadcast, then send the  
- // registered receivers separately so they don't wait for the  
- // components to be launched.  
-             BroadcastRecord r = new BroadcastRecord(intent, callerApp,   
-                 callerPackage, callingPid, callingUid, requiredPermission,   
-                 registeredReceivers, resultTo, resultCode, resultData, map,   
-                 ordered, sticky, false);   
-             ......   
- boolean replaced = false;   
- if (replacePending) {   
- for (int i=mParallelBroadcasts.size()-1; i>=0; i--) {   
- if (intent.filterEquals(mParallelBroadcasts.get(i).intent)) {   
-                         ......   
-                         mParallelBroadcasts.set(i, r);   
-                         replaced = true;   
- break;   
-                     }   
-                 }   
-             }   
- 
- if (!replaced) {   
-                 mParallelBroadcasts.add(r);   
- 
-                 scheduleBroadcastsLocked();   
-             }   
- 
-             registeredReceivers = null;   
-             NR = 0;   
-         }   
- 
-         ......   
- 
-     }   
- 
-     ......   
- }   
    这个函数首先是根据intent找出相应的广播接收器：
- // Figure out who all will receive this broadcast.  
-    List receivers = null;   
-    List<BroadcastFilter> registeredReceivers = null;   
- try {   
- if (intent.getComponent() != null) {   
-         ......   
- } else {   
-     ......   
-     registeredReceivers = mReceiverResolver.queryIntent(intent, resolvedType, false);   
- }   
-    } catch (RemoteException ex) {   
- ......   
-    }   
     回忆一下前面一篇文章[Android应用程序注册广播接收器（registerReceiver）的过程分析](http://shyluo.blog.51cto.com/5725845/966376)中的Step 6（ActivityManagerService.registerReceiver）中，我们将一个filter类型为BROADCAST_COUNTER_ACTION类型的BroadcastFilter实例保存在了ActivityManagerService的成员变量mReceiverResolver中，这个BroadcastFilter实例包含了我们所注册的广播接收器，这里就通过mReceiverResolver.queryIntent函数将这个BroadcastFilter实例取回来。由于注册一个广播类型的接收器可能有多个，所以这里把所有符合条件的的BroadcastFilter实例放在一个List中，然后返回来。在我们这个场景中，这个List就只有一个BroadcastFilter实例了，就是MainActivity注册的那个广播接收器。
