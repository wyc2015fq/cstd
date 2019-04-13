
# Android wakelock 自上而下浅析 - 嵌入式Linux - CSDN博客

2015年10月23日 17:25:56[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：622


Wake Lock是一种锁的机制, 只要有人拿着这个锁,系统就无法进入休眠,可以被用户态程序和内核获得. 这个锁可以是有超时的或者是没有超时的,超时的锁会在时间过去以后自动解锁. 如果没有锁了或者超时了, 内核就会启动休眠的那套机制来进入休眠.
PowerManager.WakeLock 有加锁和解锁两种状态，加锁的方式有两种，一种是永久的锁住，这样的锁除非显式的放开，是不会解锁的，所以这种锁用起来要非常的小心。第二种锁是超时锁，这种锁会在锁住后一段时间解锁。在创建了
 PowerManager.WakeLock 后，有两种机制，第一种是不计数锁机制，另一种是计数锁机制。可以通过 setReferenceCounted(boolean value) 来指定，一般默认为计数机制。这两种机制的区别在于，前者无论 acquire() 了多少次，只要通过一次 release()即可解锁。而后者正真解锁是在（ --count == 0 ）的时候，同样当 (count == 0) 的时候才会去申请加锁，其他情况 isHeld 状态是不会改变的。所以 PowerManager.WakeLock
 的计数机制并不是正真意义上的对每次请求进行申请／释放每一把锁，它只是对同一把锁被申请／释放的次数进行了统计再正真意义上的去操作。一下进行了永久锁的测试： 从测试我们可以看到使用计数和计数锁的区别。

(一).内核维护了:
1).两个链表，active_wake_locks[WAKE_LOCK_TYPE_COUNT]
active_wake_locks[0]维护的是suspend
 lock.
active_wake_locks[1]维护的是idle
 lock.
2).一个链表，inactive_locks来记录所有处于inactive状态的锁.
(二). 下面讲述应用层申请的锁怎么传到kernel下面的，来理解整个wakelock的框架。
比如/sys/power/wake_lock 下面的PowerManagerService的生成过程。
1). Android 提供了现成 android.os.PowerManager
 类 ,类中提供 newWakeLock(int flags, String tag)方法来取得相应层次的锁， 此函数的定义 frameworks/base/core/java/android/os/PowerManager.java
下面，应用程序在申请wake_lock时都会有调用。
实例：
PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
PowerManager.WakeLock wl = pm.newWakeLock
(PowerManager.SCREEN_DIM_WAKE_LOCK, “My Tag”);
wl.acquire();//申请锁这个里面会调用PowerManagerService里面acquireWakeLock()
***********************
wl.release(); //释放锁，显示的释放，如果申请的锁不在此释放系统就不会进入休眠。
2). frameworks层
> /frameworks/base/services/java/com/android/server/PowerManagerService.java

> 这个类是来管理所有的应用程序申请的wakelock。比如音视频播放器，camera等申请的wakelock都是通过这个类来管理的。

> static final String PARTIAL_NAME = "PowerManagerService"

> Power.acquireWakeLock(Power.PARTIAL_WAKE_LOCK,PARTIAL_NAME);

> 这个函数调用Power类里面的acquireWakeLock()，此时的PARTIAL_NAME作为参数传递到底层去。

> /frameworks/base/core/java/android/os/Power.java

> public static native void acquireWakeLock(int lock, String id);

> 注：native申明的方法在Power类中没有实现，其实现体在frameworks/base/core/jni/android_os_Power.cpp中，所 以调用Power类的acquireWakeLock()方法时会调用JNI下的实现方法。

3).JNI层的实现
> 路径：frameworks/base/core/jni/android_os_Power.cpp

> static void acquireWakeLock(JNIEnv *env, jobject clazz,

> jint lock, jstring idObj)

> {

> const char *id = env->GetStringUTFChars(idObj, NULL);

> acquire_wake_lock(lock, id);

> env->ReleaseStringUTFChars(idObj, id);

> }

> 注：在acquireWakeLock()中调用了 路径下hardware/libhardware_legacy/power/power.c下面的acquire_wake_lock(lock, id)

4).与kernel层的交互
> 在power.c下的acquire_wake_lock(lock, id)函数如下：

> int acquire_wake_lock(int lock, const char* id)

> {

> return write(fd, id, strlen(id));

> }

> 注： fd就是文件描述符，在此表示”/sys/power/wake_lock”

> id就是从PowerManagerService类中传下来的参数即：

> PARTIAL_NAME = "PowerManagerService"  到此就是通过文件系统来与kernel层交互的地方。
(三)。wakelocks调试：
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7815004#)[copy](http://blog.csdn.net/crycheng/article/details/7815004#)

staticlonghas_wake_lock_locked(inttype)
{
structwake_lock *lock, *n;
longmax_timeout = 0;

BUG_ON(type >= WAKE_LOCK_TYPE_COUNT);
list_for_each_entry_safe(lock, n, &active_wake_locks[type], link) {
if(lock->flags & WAKE_LOCK_AUTO_EXPIRE) {
longtimeout = lock->expires - jiffies;
if(timeout <= 0)
expire_wake_lock(lock);
elseif(timeout > max_timeout)
max_timeout = timeout;
}else
printk(">>%s lock.name == %s\n",__FUNCTION__,lock->name);
return-1;
}
returnmax_timeout;
}

在内核打印休眠时没释放锁的名称。
1.要想能够在休眠的时候唤醒，必须使用RTC_WAKEUP，并且赋予<uses-permission android:name="android.permission.WAKE_LOCK">  系统唤醒的权限。
否则，即使使用了RTC_WAKEUP也唤醒不了设备。

