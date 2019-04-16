# 安卓开发40：使用startForeground让android服务前台运行 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年09月22日 18:53:52[initphp](https://me.csdn.net/initphp)阅读数：2486
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









最近在使用android 4.1系统的时候，发现在手机休眠一段时间后（1-2小时），后台运行的服务被强行kill掉，有可能是系统回收内存的一种机制，要想避免这种情况可以通过startForeground让服务前台运行，当stopservice的时候通过stopForeground去掉。

以下是android官方描述：

## Running a Service in the Foreground


A foreground service is a service that's considered to be something the user is actively aware of and thus not a candidate for the system to kill when low on memory. A foreground service must provide a notification for the status bar, which is placed under
 the "Ongoing" heading, which means that the notification cannot be dismissed unless the service is either stopped or removed from the foreground.


For example, a music player that plays music from a service should be set to run in the foreground, because the user is explicitly aware of its operation. The notification in the status bar might indicate the current song and allow the user to launch an activity
 to interact with the music player.


To request that your service run in the foreground, call `[startForeground()](http://developer.android.com/reference/android/app/Service.html#startForeground%28int,%20android.app.Notification%29)`.
 This method takes two parameters: an integer that uniquely identifies the notification and the `[Notification](http://developer.android.com/reference/android/app/Notification.html)` for
 the status bar. For example:
Notification notification =newNotification(R.drawable.icon, getText(R.string.ticker_text),System.currentTimeMillis());Intent notificationIntent =newIntent(this,ExampleActivity.class);PendingIntent pendingIntent =PendingIntent.getActivity(this,0, notificationIntent,0);
notification.setLatestEventInfo(this, getText(R.string.notification_title),
        getText(R.string.notification_message), pendingIntent);
startForeground(ONGOING_NOTIFICATION, notification);

To remove the service from the foreground, call `[stopForeground()](http://developer.android.com/reference/android/app/Service.html#stopForeground%28boolean%29)`.
 This method takes a boolean, indicating whether to remove the status bar notification as well. This method does *not* stop the service. However, if you stop the service while it's still running in the foreground, then the notification is also removed.

**Note:** The methods `[startForeground()](http://developer.android.com/reference/android/app/Service.html#startForeground%28int,%20android.app.Notification%29)` and `[stopForeground()](http://developer.android.com/reference/android/app/Service.html#stopForeground%28boolean%29)` were
 introduced in Android 2.0 (API Level 5). In order to run your service in the foreground on older versions of the platform, you must use the previous`setForeground()` method—see
 the `[startForeground()](http://developer.android.com/reference/android/app/Service.html#startForeground%28int,%20android.app.Notification%29)` documentation
 for information about how to provide backward compatibility.


For more information about notifications, see [Creating Status Bar Notifications](http://developer.android.com/guide/topics/ui/notifiers/notifications.html).

要想实现需求，我们只需要在onStartCommand里面调用 startForeground，然后再onDestroy里面调用stopForeground即可！ 

实际情况就譬如手机里面的音乐播放器一样，不管手机如何休眠，只要开始播放音乐了，就不会kill掉这个服务，一旦停止播放音乐，服务就可能被清掉。



