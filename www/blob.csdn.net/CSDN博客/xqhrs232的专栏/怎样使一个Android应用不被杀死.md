# 怎样使一个Android应用不被杀死 - xqhrs232的专栏 - CSDN博客
2012年03月07日 16:41:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：492标签：[android																[service																[server																[system																[application																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/gracioushe/article/details/7235565](http://blog.csdn.net/gracioushe/article/details/7235565)
参考文章：
[http://blog.csdn.net/windskier/article/details/6560925](http://blog.csdn.net/windskier/article/details/6560925)
[http://blog.csdn.net/zmyde2010/article/details/6756368](http://blog.csdn.net/zmyde2010/article/details/6756368)
[http://blog.sina.com.cn/s/blog_514048cb0100wi2j.html](http://blog.sina.com.cn/s/blog_514048cb0100wi2j.html)
方法：
对于放在/system/app下的应用，需要在其Manifest.xml文件中设置persistent属性，如应用程序'Phone'的AndroidManifest.xml文件：
    <application android:name="PhoneApp"
android:persistent="true"
                 android:label="@string/dialerIconLabel"
                 android:icon="@drawable/ic_launcher_phone">
         ...
    </application>
设置后app提升为系统核心级别，任何情况下不会被kill掉, settings->applications里面也会屏蔽掉stop操作。
这样设置前的log:   Proc #19: adj=svc  /B 4067b028 255:com.xxx.xxx/10001 (started-services)
    # cat /proc/255/oom_adj
    4
设置后的log:  PERS #19: adj=core /F 406291f0 155:com.xxx.xxx/10001 (fixed)
    # cat /proc/155/oom_adj
     -12                #
这是CORE_SERVER_ADJ
注：init进程的oom_adj为-16（即SYSTEM_ADJ）： cat  /proc/1/oom_adj
在文件frameworks/base/services/java/com/android/server/am/ActivityManagerService.java中有以下的代码：
    final ProcessRecord addAppLocked(ApplicationInfo info) {
        ProcessRecord app = getProcessRecordLocked(info.processName, info.uid);
        if (app == null) {
            app = newProcessRecordLocked(null, info, null);
            mProcessNames.put(info.processName, info.uid, app);
            updateLruProcessLocked(app, true, true);
        }    
        if ((info.flags&(ApplicationInfo.FLAG_SYSTEM|ApplicationInfo.FLAG_PERSISTENT))
                == (ApplicationInfo.FLAG_SYSTEM|ApplicationInfo.FLAG_PERSISTENT)) {
app.persistent = true;
app.maxAdj = CORE_SERVER_ADJ;             // 这个常数值为-12。
        }    
        if (app.thread == null && mPersistentStartingProcesses.indexOf(app) < 0) {
            mPersistentStartingProcesses.add(app);
            startProcessLocked(app, "added application", app.processName);
        }    
        return app;
    }
可见要想成为core service （即app.maxAdj = CORE_SERVER_ADJ(-12)），应用程序需要FLAG_SYSTEM和FLAG_PERSISTENT两个标志，FLAG_SYSTEM指的是应用位于/system/app下，FLAG_PERSISTENT就是指persistent属性。
而对于frameworks/base/services/java/com/android/server/SystemServer.java，则调用
       ActivityManagerService.setSystemProcess();
把自己的 app.maxAdj 设置成SYSTEM_ADJ，即-16。
原理：
Android中的进程是托管的，当系统进程空间紧张的时候，会依照优先级自动进行进程的回收。由此带来三个问题：
    1）回收规则:  什么时候回收与回收哪一个？
    2）避免误杀:  如何阻止被回收？
    3）数据恢复与保存:  被回收了怎么办？
Android将进程分为6个等级,它们按优先级顺序由高到低依次是:
   1.前台进程( FOREGROUND_APP)
   2.可视进程(VISIBLE_APP )
   3. 次要服务进程(SECONDARY_SERVER )
   4.后台进程 (HIDDEN_APP)
   5.内容供应节点(CONTENT_PROVIDER)
   6.空进程(EMPTY_APP)
特征：
1.如果一个进程里面同时包含service和可视的activity，那么这个进程应该归于可视进程，而不是service进程。
2.另外，如果其他进程依赖于它的话，一个进程的等级可以提高。例如，一个A进程里的service被绑定到B进程里的组件上，进程A将总被认为至少和B进程一样重要。
3.系统中的phone服务被划分到前台进程而不是次要服务进程.
在android中，进程的oom_adj值也就代表了它的优先级。oom_adj值越高代表该进程优先级越低。文件/init.rc中有以下属性设置：
    setprop ro.FOREGROUND_APP_ADJ       0
    setprop ro.VISIBLE_APP_ADJ                     1
    setprop ro.SECONDARY_SERVER_ADJ   2
    setprop ro.HIDDEN_APP_MIN_ADJ   
