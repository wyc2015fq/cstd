# 为什么 bindService 能和 Activity 的生命周期联动？ - 我的学习笔记 - CSDN博客





2018年12月14日 23:41:18[zouzhiheng](https://me.csdn.net/u011330638)阅读数：56








# 前言

之前做一道面试题：startService 和 bindService 有什么不同？为什么 bindService 能和 Activity 的生命周期联动？

前一个问题可以很快回答出来：生命周期不同，结束方式不同，交互方式不同。

后一个问题也能很快想到应该是 Activity 在销毁的时候顺带把 Service 销毁了。那么为什么 startService 不行呢？具体是怎么实现的呢？如果不对源码研究一番，似乎无法给出让人信服的回答，于是就有了这篇文章。

# 启动和绑定的区别

无论是启动 Activity，还是 Service，基本的流程都是 Context -> ActivtityManagerService -> 某些中间类（Activity 是 ActivityStarter、ActivityStack 等，Service 是 ActiveServices） -> ActivityThread。具体的代码流程比较长，而且很多和本文要探讨的主题无关，因此这里不会详细分析启动或绑定的流程，而只会保留与本文相关的部分源码。

## startService

从 ContextImpl 的 startService 方法开始说起：

```
class ContextImpl extends Context {

    @Override
    public ComponentName startService(Intent service) {
        warnIfCallingFromSystemProcess();
        return startServiceCommon(service, false, mUser);
    }
    
    private ComponentName startServiceCommon(Intent service, boolean requireForeground,
            UserHandle user) {
        try {
            // 检查 Intent
            validateServiceIntent(service);
            service.prepareToLeaveProcess(this);
            // 启动 Service
            ComponentName cn = ActivityManager.getService().startService(
                mMainThread.getApplicationThread(), service, service.resolveTypeIfNeeded(
                            getContentResolver()), requireForeground,
                            getOpPackageName(), user.getIdentifier());
            // 检查结果
            if (cn != null) {
                ...
            }
            return cn;
        } catch (RemoteException e) {
            throw e.rethrowFromSystemServer();
        }
    }
    
}
```

ActivityManager.getService() 返回的就是 AMS 本身，而 AMS 只起到一个中转的作用，除了一些参数判断之外，AMS 直接调用了 ActiveServices 的 startServiceLocked：

```
public final class ActiveServices {

    final ActivityManagerService mAm;

    ComponentName startServiceLocked(IApplicationThread caller, Intent service, String resolvedType,
            int callingPid, int callingUid, boolean fgRequired, String callingPackage, final int userId)
            throws TransactionTooLargeException {

        // 和 Activity 同样有一个 Record 记录对应的组件
        ServiceRecord r = res.record;
        
        ... // 主要是检查，发现错误则抛出异常，或返回 null 等结果

        // 设置 ServiceRecord 的数据域
        r.lastActivity = SystemClock.uptimeMillis();
        r.startRequested = true;
        r.delayedStop = false;
        r.fgRequired = fgRequired;
        r.pendingStarts.add(new ServiceRecord.StartItem(r, false, r.makeNextStartId(),
                service, neededGrants, callingUid));
                
        ... // 主要是检查，发现错误则抛出异常，或返回 null 等结果

        // 启动
        ComponentName cmp = startServiceInnerLocked(smap, service, r, callerFg, addToStarting);
        return cmp;
    }
}
```

好了，startService 的流程暂时分析到这里，后面也没什么特别的，最后还是会由 ActivityThread 来创建 Service 对象，回调相关的生命周期方法等。

## bindService

下面看 bindService 的实现：

```
class ContextImpl extends Context {

    final @NonNull LoadedApk mPackageInfo;
    private final @Nullable IBinder mActivityToken;

    @Override
    public boolean bindService(Intent service, ServiceConnection conn,
            int flags) {
        warnIfCallingFromSystemProcess();
        return bindServiceCommon(service, conn, flags, mMainThread.getHandler(),
                Process.myUserHandle());
    }
    
    private boolean bindServiceCommon(Intent service, ServiceConnection conn, int flags, Handler
            handler, UserHandle user) {
        ...
        IServiceConnection sd;
        if (mPackageInfo != null) {
            sd = mPackageInfo.getServiceDispatcher(conn, getOuterContext(), handler, flags);
        } else {
            throw new RuntimeException("Not supported in system context");
        }
        try {
            // 这个 token 是 Activity 启动时创建的，对应于 Activity 的 mToken 成员
            IBinder token = getActivityToken();
            ...
            int res = ActivityManager.getService().bindService(
                mMainThread.getApplicationThread(), getActivityToken(), service,
                service.resolveTypeIfNeeded(getContentResolver()),
                sd, flags, getOpPackageName(), user.getIdentifier());
            ...
        } catch (RemoteException e) {
            ...
        }
    }
    
    
    @Override
    public IBinder getActivityToken() {
        return mActivityToken;
    }
    
}
```

可以看到，相比 startService，bindService 还在 ContextImpl 执行的时候就已经显示出了它的不同，除了会获取 Activity 的 token 之外，还有一个很关键的调用是 LoadedApk 的 getServiceDispatcher 方法：

```
public final class LoadedApk {

    private final ArrayMap<Context, ArrayMap<ServiceConnection, LoadedApk.ServiceDispatcher>> mServices
        = new ArrayMap<>();

    public final IServiceConnection getServiceDispatcher(ServiceConnection c,
            Context context, Handler handler, int flags) {
        synchronized (mServices) {
            LoadedApk.ServiceDispatcher sd = null;
            ArrayMap<ServiceConnection, LoadedApk.ServiceDispatcher> map = mServices.get(context);
            if (map != null) {
                sd = map.get(c);
            }
            if (sd == null) {
                sd = new ServiceDispatcher(c, context, handler, flags);
                if (map == null) {
                    map = new ArrayMap<>();
                    mServices.put(context, map); // 记录
                }
                map.put(c, sd); // 记录 ServiceConnection
            } else {
                sd.validate(context, handler);
            }
            return sd.getIServiceConnection();
        }
    }
    
}
```

ServiceDispatcher 可以忽略，主要关注 mServices 这个成员，它记录了即将绑定到 Activity 的 ServiceConnection。

接着看后面的绑定流程，AMS 同样跳过，直接看 ActiveServices 的实现：

```
public final class ActiveServices {

    final ActivityManagerService mAm;
    final ArrayMap<IBinder, ArrayList<ConnectionRecord>> mServiceConnections = new ArrayMap<>();

    int bindServiceLocked(IApplicationThread caller, IBinder token, Intent service,
            String resolvedType, final IServiceConnection connection, int flags,
            String callingPackage, final int userId) throws TransactionTooLargeException {
        
        // 获取应用进程的信息
        final ProcessRecord callerApp = mAm.getRecordForAppLocked(caller);
        if (callerApp == null) {
            throw new SecurityException(
                    "Unable to find app for caller " + caller
                    + " (pid=" + Binder.getCallingPid()
                    + ") when binding service " + service);
        }

        // 获取绑定的 Activity 信息
        ActivityRecord activity = null;
        if (token != null) {
            activity = ActivityRecord.isInStackLocked(token);
            if (activity == null) {
                Slog.w(TAG, "Binding with unknown activity: " + token);
                return 0;
            }
        }

        // 获取 ServiceRecord
        ServiceRecord s = res.record;

        boolean permissionsReviewRequired = false;

        // 启动 Activity，成功启动后再启动 Service
        if (mAm.mPermissionReviewRequired) {
            if (mAm.getPackageManagerInternalLocked().isPermissionsReviewRequired(
                    s.packageName, s.userId)) {

                RemoteCallback callback = new RemoteCallback(
                        new RemoteCallback.OnResultListener() {
                    @Override
                    public void onResult(Bundle result) {
                        synchronized(mAm) {
                            final long identity = Binder.clearCallingIdentity();
                            try {
                                ...
                                if (...) {
                                    try {
                                        // 启动 Service
                                        bringUpServiceLocked(...);
                                    } catch (RemoteException e) {
                                        /* ignore - local call */
                                    }
                                } else {
                                    ...
                                }
                            } finally {
                                ...
                            }
                        }
                    }
                });

                final Intent intent = new Intent(Intent.ACTION_REVIEW_PERMISSIONS);
                // 注意 callback
                intent.putExtra(Intent.EXTRA_REMOTE_CALLBACK, callback);
                
                // 启动 Activity，成功启动后回调 callback 
                mAm.mHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        mAm.mContext.startActivityAsUser(intent, new UserHandle(userId));
                    }
                });
            }
        }

        final long origId = Binder.clearCallingIdentity();

        try {
            ...
            // 注意参数 activity(ActivityRecord)
            ConnectionRecord c = new ConnectionRecord(b, activity,
                    connection, flags, clientLabel, clientIntent);

            // connection 参数为 IServiceConnection
            IBinder binder = connection.asBinder();
            
            // 让 ActivityRecord 记录 connections 信息
            if (activity != null) {
                if (activity.connections == null) {
                    activity.connections = new HashSet<ConnectionRecord>();
                }
                activity.connections.add(c);
            }

            // 让 ServiceRecord 记录 connections 信息
            ArrayList<ConnectionRecord> clist = s.connections.get(binder);
            if (clist == null) {
                clist = new ArrayList<ConnectionRecord>();
                s.connections.put(binder, clist);
            }
            clist.add(c);

            // 让自身的成员变量 mServiceConnections 记录 connections 信息
            clist = mServiceConnections.get(binder);
            if (clist == null) {
                clist = new ArrayList<ConnectionRecord>();
                mServiceConnections.put(binder, clist);
            }
            clist.add(c);
            
            ...

            if ((flags&Context.BIND_AUTO_CREATE) != 0) { // 如果设置了绑定后自动启动
                s.lastActivity = SystemClock.uptimeMillis();
                // 启动 Service
                if (bringUpServiceLocked(s, service.getFlags(), callerFg, false,
                        permissionsReviewRequired) != null) {
                    return 0;
                }
            }

            if (s.app != null && b.intent.received) { // Service 已经在运行中，直接回调 onServiceConnected 即可
                // Service is already running, so we can immediately
                // publish the connection.
                try {
                    // 回调 onServiceConnected
                    c.conn.connected(s.name, b.intent.binder, false);
                } catch (Exception e) {
                    ...
                }

                ...
            } else if (!b.intent.requested) { 
                // 绑定 onBind，内部调用了 scheduleBindService
                requestServiceBindingLocked(s, b.intent, callerFg, false);
            }

        } finally {
            Binder.restoreCallingIdentity(origId);
        }

        return 1;
    }
    
}
```

可以看到，相比 startService，bindService 在启动 Service 之前做了一些额外的工作：
- 通知 LoadedApk 记录 ServiceConnection
- 根据 ActivityToken 获取 ActivityRecord
- 添加 ConnectionRecord 到 ActivityRecord 及 ServiceRecord 上

上面就是 startService 和 bindService 在源码实现上的主要区别了，下面开始分析 Activity 的 finish 方法，看看 Service 是怎么随着 Activity 的销毁而销毁的。

# Service 是怎么随着 Activity 的销毁而销毁的？

和启动流程类似，finish 的执行流程为 Activity -> ActivityManager -> ActivitiyStack -> ActivityThread，因为代码量有些大，而且前三步和本文关系不大，因此这里直接看 ActivityThread 的实现即可：

```
public final class ActivityThread {

    final ArrayMap<IBinder, ActivityClientRecord> mActivities = new ArrayMap<>();

    private void handleDestroyActivity(IBinder token, boolean finishing,
            int configChanges, boolean getNonConfigInstance) {
        // 回调生命周期方法
        ActivityClientRecord r = performDestroyActivity(token, finishing,
                configChanges, getNonConfigInstance);
        if (r != null) {
            // 清理 window 资源
            cleanUpPendingRemoveWindows(r, finishing);
            // 删除 DecorView
            WindowManager wm = r.activity.getWindowManager();
            View v = r.activity.mDecor;
            if (v != null) {
                IBinder wtoken = v.getWindowToken();
                if (r.activity.mWindowAdded) {
                    if (r.mPreserveWindow) {
                        r.window.clearContentView();
                    } else {
                        wm.removeViewImmediate(v);
                    }
                }
                // 清除记录，这个记录可以参考 ViewRootImpl 的 setView 方法
                if (wtoken != null && r.mPendingRemoveWindow == null) {
                    WindowManagerGlobal.getInstance().closeAll(wtoken,
                            r.activity.getClass().getName(), "Activity");
                } else if (r.mPendingRemoveWindow != null) {
                    WindowManagerGlobal.getInstance().closeAllExceptView(token, v,
                            r.activity.getClass().getName(), "Activity");
                }
                r.activity.mDecor = null;
            }
            
            if (r.mPendingRemoveWindow == null) {
                WindowManagerGlobal.getInstance().closeAll(token,
                        r.activity.getClass().getName(), "Activity");
            }

            // 使用 Base Context 执行最后的清理步骤
            Context c = r.activity.getBaseContext();
            if (c instanceof android.app.ContextImpl) {
                ((ContextImpl) c).scheduleFinalCleanup(
                        r.activity.getClass().getName(), "Activity");
            }
        }
        // 通知 AMS
        if (finishing) {
            try {
                ActivityManager.getService().activityDestroyed(token);
            } catch (RemoteException ex) {
                throw ex.rethrowFromSystemServer();
            }
        }
        mSomeActivitiesChanged = true;
    }
}
```

可以看到，在 ActivityThread 中，Activity 的销毁流程共有 4 步：
- 回调 onPause、onStop、onDestroy 等生命周期方法
- 关闭 Window、移除 DecorView、清理 WindowManager 的记录
- 调用 ContextImpl 执行最后的清理步骤
- 通知 AMS Activity 已被销毁

Service 的解绑逻辑就隐藏在 ContextImpl 里面，下面看它的实现：

```
class ContextImpl extends Context {

    final @NonNull ActivityThread mMainThread;
    final @NonNull LoadedApk mPackageInfo;

    // 回调 ActivityThread
    final void scheduleFinalCleanup(String who, String what) {
        mMainThread.scheduleContextCleanup(this, who, what);
    }

    // ActivityThread 最终又回调了该方法
    final void performFinalCleanup(String who, String what) {
        //Log.i(TAG, "Cleanup up context: " + this);
        mPackageInfo.removeContextRegistrations(getOuterContext(), who, what);
    }
    
}
```

可以看到，ContextImpl 只是起到了一个中转的作用，最终是交给 LoadedApk 执行的。从方法 removeContextRegistrations 的名字可以推测出，它的作用是清理注册到 Context 上的资源：

```
public final class LoadedApk {

    private final ArrayMap<Context, ArrayMap<BroadcastReceiver, ReceiverDispatcher>> mReceivers
        = new ArrayMap<>();
    private final ArrayMap<Context, ArrayMap<BroadcastReceiver, LoadedApk.ReceiverDispatcher>> mUnregisteredReceivers
        = new ArrayMap<>();
    private final ArrayMap<Context, ArrayMap<ServiceConnection, LoadedApk.ServiceDispatcher>> mServices
        = new ArrayMap<>();
    private final ArrayMap<Context, ArrayMap<ServiceConnection, LoadedApk.ServiceDispatcher>> mUnboundServices
        = new ArrayMap<>();

    public void removeContextRegistrations(Context context,
            String who, String what) {
        final boolean reportRegistrationLeaks = StrictMode.vmRegistrationLeaksEnabled();
        synchronized (mReceivers) {
            // 获取注册到 Context 上的 BroadcastReceiver
            ArrayMap<BroadcastReceiver, LoadedApk.ReceiverDispatcher> rmap =
                    mReceivers.remove(context);
            if (rmap != null) {
                // 遍历，逐个注销
                for (int i = 0; i < rmap.size(); i++) {
                    LoadedApk.ReceiverDispatcher rd = rmap.valueAt(i);
                    // 打印异常信息
                    IntentReceiverLeaked leak = new IntentReceiverLeaked(
                            what + " " + who + " has leaked IntentReceiver "
                            + rd.getIntentReceiver() + " that was " +
                            "originally registered here. Are you missing a " +
                            "call to unregisterReceiver()?");
                    leak.setStackTrace(rd.getLocation().getStackTrace());
                    Slog.e(android.app.ActivityThread.TAG, leak.getMessage(), leak);
                    if (reportRegistrationLeaks) {
                        StrictMode.onIntentReceiverLeaked(leak);
                    }
                    // 通知 AMS 注销 BroadcastReceiver
                    try {
                        ActivityManager.getService().unregisterReceiver(
                                rd.getIIntentReceiver());
                    } catch (RemoteException e) {
                        throw e.rethrowFromSystemServer();
                    }
                }
            }
            mUnregisteredReceivers.remove(context);
        }

        synchronized (mServices) {
            // 获取绑定到 Context 上的 ServiceConnection
            ArrayMap<ServiceConnection, LoadedApk.ServiceDispatcher> smap =
                    mServices.remove(context);
            if (smap != null) {
                // 遍历，逐个解绑
                for (int i = 0; i < smap.size(); i++) {
                    LoadedApk.ServiceDispatcher sd = smap.valueAt(i);
                    // 打印异常信息
                    ServiceConnectionLeaked leak = new ServiceConnectionLeaked(
                            what + " " + who + " has leaked ServiceConnection "
                            + sd.getServiceConnection() + " that was originally bound here");
                    leak.setStackTrace(sd.getLocation().getStackTrace());
                    Slog.e(android.app.ActivityThread.TAG, leak.getMessage(), leak);
                    if (reportRegistrationLeaks) {
                        StrictMode.onServiceConnectionLeaked(leak);
                    }
                    // 通知 AMS 解绑 ServiceConnection
                    try {
                        // 具体实现参考 
                        ActivityManager.getService().unbindService(
                                sd.getIServiceConnection());
                    } catch (RemoteException e) {
                        throw e.rethrowFromSystemServer();
                    }
                    sd.doForget();
                }
            }
            mUnboundServices.remove(context);
        }
    }
    
}
```

果然，removeContextRegistrations 的作用就是把注册/绑定到 Context 上的 BroadcastReceiver、ServiceConnection 给注销/解绑，并抛出异常信息，告诉用户应该主动地注销/解绑。unbindService、unregisterReceiver 的流程忽略，无非还是从相关的列表中删除一些记录（比如 activity.connections），并通知 ActivityThread 执行最后的注销逻辑。

# 总结

分析完上面的代码后，现在可以自信地给出这道面试题的答案了：
- bindService 方法执行时，LoadedApk 会记录 ServiceConnection 信息
- Activity 执行 finish 方法时，会通过 LoadedApk 检查 Activity 是否存在未注销/解绑的 BroadcastReceiver 和 ServiceConnection，如果有，那么会通知 AMS 注销/解绑对应的 BroadcastReceiver 和 Service，并打印异常信息，告诉用户应该主动执行注销/解绑的操作



