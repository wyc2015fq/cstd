# Android实现程序前后台切换效果 - u013366022的专栏 - CSDN博客
2014年06月03日 09:52:08[slitaz](https://me.csdn.net/u013366022)阅读数：613
在介绍程序实现之前，我们先看下Android中Activities和Task的基础知识。
　　我们都知道，一个Activity 可以启动另一个Activity,即使这个Activity是定义在别一个应用程序里的，比如说，想要给用户展示一个地图的信息，现在已经有一个Activity可以做这件事情，那么现在你的Activity需要做的就是将请求信息放进一个Intent对象里，并且将这个Intent对象传递给startActivity(),那么地图就可显示出来了，但用户按下Back键之后，你的Activity又重新出现在屏幕上。
　　对用户来讲，显示地图的Activity和你的Activity好像在一个应用程序中的，虽然是他们是定义在其他的应用程序中并且运行在那个应有进程中。Android将你的Activity和借用的那个Activity被放进一个Task中以维持用户的体验。那么Task是以栈的形式组织起来一组相互关联的Activity,栈中底部的Activity就是开辟这个Task的，通常是用户在应用程序启动器中选择的Activity。栈的顶部的Activity是当前正在运行的Activity--用户正在交互操作的Activity。
　　当一个Activity启动另一个Activity时，新启动的Activity被压进栈中，成为正在运行的Activity。旧的Activity仍然在栈中。当用户按下BACK键之后，正在运行的Activity弹出栈，旧的Activity恢复成为运行的Activity。栈中包含对象，因此如果一个任务中开启了同一个Activity子类的的多个对象——例如，多个地图浏览器——则栈对每一个实例都有一个单独的入口。栈中的Activity不会被重新排序，只会被、弹出。Task是一组Activity实例组成的栈，不是在manifest文件里的某个类或是元素，所以无法设定一个Task的属性而不管它的Activity，一个Task的所有属性值是在底部的Activity里设置的，这就需要用于Affinity。关于Affinity这里不再详述，大家可以查询文档。
　　一个Task里的所有Activity作为一个整体运转。整个Task（整个Activity堆栈）可以被推到前台或被推到后台。假设一个正在运行的Task中有四个Activity——正在运行的Activity下面有三个Activity，这时用户按下HOME键，回到应有程序启动器然后运行新的应用程序（实际上是运行了一个新的Task），那么当前的Task就退到了后台，新开启的应用程序的root Activity此时就显示出来了，一段时间后，用户又回到应用程序器，又重新选择了之前的那个应用程序（先前的那个Task），那么先前的那个Task此时又回到了前台了，当用户按下BACK键时，屏幕不是显示刚刚离开的那个新开启的那个应用程序的Activity，而是被除回到前台的那个Task的栈顶Activity，将这个Task的下一个Activity显示出来。
 上述便是Activity和Task一般的行为，但是这个行为的几乎所有方面都是可以修改的。Activity和Task的关系，以及Task中Activity的行为，是受启动该Activity的Intent对象的标识和在manifest文件中的Activity的<Activity>元素的属性共同影响的。
　　以上是关于Activity和Task的描述。
　　在开发Android项目时，用户难免会进行程序切换，在切换过程中，程序将进入后台运行，需要用时再通过任务管理器或是重新点击程序或是通过点击信息通知栏中的图标返回原来的界面。这种效果类似于腾讯QQ的效果，打开QQ后显示主界面，在使用其他的程序时，QQ将以图标的形式显示在信息通知栏里，如果再用到QQ时再点击信息通知栏中的图标显示QQ主界面。
　　先看下本示例实现效果图：
![](http://images.cnblogs.com/cnblogs_com/hanyonglu/2012-04-15_163817.png)![](http://images.cnblogs.com/cnblogs_com/hanyonglu/2012-04-15_164123.png)
　　在上图第二个图中，我们点击时将会返回到的原来的Activity中。 
　　当我们的程序进入后台运作时，在我们的模拟器顶部将以图标形式出现，如下图：
![](http://images.cnblogs.com/cnblogs_com/hanyonglu/2012-04-15_170428.png)
　　对于这种效果一般的做法是在Activity中的onStop()方法中编写相应代码，因为当Activity进入后台时将会调用onStop()方法，我们可以在onStop()方法以Notification形式显示程序图标及信息，其中代码如下所示：
```
@Override
    protected void onStop() {
    // TODO Auto-generated method stub
    　　super.onStop();
    　　Log.v("BACKGROUND", "程序进入后台");
    　　showNotification();
    }
```
　　以上的showNotification()方法就是Notification。
　　然后点击信息通知栏的Notification后再返回到原来的Activity。
　　当然，我们也可以捕捉HOME键，在用户按下HOME键时显示Notification， 以下是代码示例：
```
// 点击HOME键时程序进入后台运行
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // TODO Auto-generated method stub
        // 按下HOME键
        if(keyCode == KeyEvent.KEYCODE_HOME){
            // 显示Notification
            notification = new NotificationExtend(this);
            notification.showNotification();
            moveTaskToBack(true);                
 
            return true;
        }
        
        return super.onKeyDown(keyCode, event);
    }
```
 　　这里的NotificationExtend是对显示Notification的一个封装，类中的代码如下：
```
package com.test.background;
import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.graphics.Color;
/**
 * Notification扩展类
 * @Description: Notification扩展类
 * @File: NotificationExtend.java
 * @Package com.test.background
 * @Author Hanyonglu
 * @Date 2012-4-13 下午02:00:44
 * @Version V1.0
 */
public class NotificationExtend {
    private Activity context;
    
    public NotificationExtend(Activity context) {
        // TODO Auto-generated constructor stub
        this.context = context;
    }
    
    // 显示Notification
    public void showNotification() {
        // 创建一个NotificationManager的引用
        NotificationManager notificationManager = (
                NotificationManager)context.getSystemService(
                        android.content.Context.NOTIFICATION_SERVICE);
        
        // 定义Notification的各种属性
        Notification notification = new Notification(
                R.drawable.icon,"阅读器", 
                System.currentTimeMillis());
        // 将此通知放到通知栏的"Ongoing"即"正在运行"组中
        notification.flags |= Notification.FLAG_ONGOING_EVENT;
        // 表明在点击了通知栏中的"清除通知"后，此通知自动清除。
        notification.flags |= Notification.FLAG_AUTO_CANCEL
        notification.flags |= Notification.FLAG_SHOW_LIGHTS;
        notification.defaults = Notification.DEFAULT_LIGHTS;
        notification.ledARGB = Color.BLUE;
        notification.ledOnMS = 5000;
                
        // 设置通知的事件消息
        CharSequence contentTitle = "阅读器显示信息"; // 通知栏标题
        CharSequence contentText = "推送信息显示，请查看……"; // 通知栏内容
        
        Intent notificationIntent = new Intent(context,context.getClass());
        notificationIntent.setAction(Intent.ACTION_MAIN);
        notificationIntent.addCategory(Intent.CATEGORY_LAUNCHER);
        PendingIntent contentIntent = PendingIntent.getActivity(
        context, 0, notificationIntent,PendingIntent.FLAG_UPDATE_CURRENT);
        notification.setLatestEventInfo(
        context, contentTitle, contentText, contentIntent);
        // 把Notification传递给NotificationManager
        notificationManager.notify(0, notification);
    }
    
    // 取消通知
    public void cancelNotification(){
        NotificationManager notificationManager = (
                NotificationManager) context.getSystemService(
                        android.content.Context.NOTIFICATION_SERVICE);
        notificationManager.cancel(0);
    }
}
```
　　这里需要在配置文件中设置每个Activity以单任务运行，否则，每次返回原Activity时会新增加一个Activity，而不会返回到原Activity。
　　在使用FLAG_ACTIVITY_NEW_TASK控制标识时也会出现不会返回到原Activity的现象。如果该标识使一个Activity开始了一个新的Task，然后当用户按了HOME键离开这个Activity，在用户按下BACK键时将无法再返回到原Activity。一些应用(例如Notification)总是在一个新的Task里打开Activity，而从来不在自己的Task中打开，所以它们总是将包含FLAG_ACTIVITY_NEW_TASK的Intent传递给startActivity()。所以如果有一个可以被其他的东西以这个控制标志调用的Activity，请注意让应用程序有独立的回到原Activity的方法。 代码如下：
```
<activity android:name="ShowMessageActivity"
             android:launchMode="singleTask"></activity>
```
 　　这里需要注意的是：
　　<activity>下的launchMode属性可以设置四种启动方式： 
- standard (默认模式) 
- singleTop 
- singleTask 
- singleInstance 
　　这四个模式有以下的几个不同点：
　　1. 响应Intent时Activity将被装入哪个task。
　　对于standard和singleTop模式，由产生该Intent(调用startActivity())的task持有该Activity——除非Intent对象里含有FLAG_ACTIVITY_NEW_TASK标志，那么就会寻找一个新的task。 
　　相反的，singTask和singleInstance模式，总是标志Activity为task的root Activity，开启这样的活动会新建一个task，而不是装入某个正在运行的任务。 
　　2. 一个Activity是否可以有多个实例。
　　一个standard或者singleTop属性的Activity可以实例化多次，他们可以属于多个不同的task，而且一个task也可以含有相同Activity的多个实例。 
　　相反的，singleTask或者singleInstance属性的Activity只能有一个实例（单例），因为这些Activity是位于task的底部，这种限制意味着同一设备的同一时刻该task只能有一个实例。
　　3. 实例是否能允许在它的task里有其他的Activity。
　　一个singleInstance属性的Activity是它所在的task里仅有的一个Activity，如果他启动了另一个Activity，那个Activity会被加载进一个不同的task而无视它的启动模式——就如Intent里有FLAG_ACTIVITY_NEW_TASK标识一样。在其他的方面，singleInstance和singleTask一样的。 
　　其他三个模式允许有多个Activity在一个task里，一个singleTask属性的Activity总是一个task里的root Activity，但是他可以启动另外的Activity并且将这个新的Activity装进同一个task里，standard和singleTop属性的Activity可以出现在task的任何位置。 
　　4. 是否创建一个新的Activity实例来处理一个新的Intent。
　　对于默认的standard方式，将会生成新的实例来处理每一个新的Intent。每个实例处理一个新的Intent。
　　对singleTop模式，如果一个已经存在的实例在目标task的栈顶，那么就重用这个实例来处理这个新的Intent，如果这个实例存在但是不在栈顶，那就不重用他，而是重新创建一个实例来处理这个新的Intent并且将这个实例压入栈。 
　　例如现在有一个task堆栈ABCD，A是root Activity，D是栈顶Activity，现在有一个启动D的Intent来了，如果D是默认的standard方法，那么就会创建一个新的实例来处理这个Intent，所以这个堆栈就变为ABCDD，然而如果D是singleTop方式，这个已经存在的栈顶的D就会来处理这个Intent，所以堆栈还是ABCD。 
　　如果另外一种情况，到来的Intent是给B的，不管B是standard还是singleTop（因为现在B不在栈顶），都会创建一个新的实例，所以堆栈变为ABCDB。 
　　如上所述，一个"singleTask"或"singleInstance"模式的activity只会有一个实例，这样它们的实例就会处理所有的新intent。一个"singleInstance" activity总是在栈里的最上面
(因为它是task里的唯一的activity)， 这样它总是可以处理一个intent。而一个"singleTask" activity在栈里可以有或没有其他activity在它上面。如果有的话，它就不能对新到的intent进行处理，intent将被丢弃。(即使intent被丢弃，它的到来将使task来到前台，并维持在那里。) 
　　当一个已有的Activity被请求去处理一个新的Intent时，Intent对象会通过onNewIntent()的调用传递给这个活动。(传递进来的原始的Intent对象可以通过调用getIntent()获取)。  
　　注意，当创建一个新的Activity的实例来处理一个新收到的Intent时，用户可以按BACK键回到上一个状态（上一个Activity）。但是使用一个已有的Activity实例操作新收到的Intent时，用户不能通过按下BACK键回到这个实例在接受到新Intent之前的状态。 
　　呵呵，不好意思，扯得有点多了，我们继续看我们的程序。
　　在这里，如果是对一个Activity实现时可以这样实现，如果有多个Activity，我们就需要在每个Activity里重写onKeyDown事件并捕捉用户是否按下HOME键。
　　为了实现方便，我们可以使用一个Service专门用于监听程序是否进入后台或前台工作，如果程序进入后台运行就显示Notification，这样不管程序中有多少个Activity就可以很方便的实现程序前后如切换。 
　　为此，我在程序中新添加了一个AppStatusService 类，目的是监听程序是否在前后台运行，如果在后台运行则显示信息提示。
　　代码如下：
```
package com.test.service; 
import java.util.List;
import com.test.background.MainActivity;
import com.test.background.NotificationExtend;
import com.test.background.R;
import com.test.util.AppManager;
import android.app.ActivityManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.IBinder;
import android.util.Log;
/**
 * 监听程序是否在前后台运行Service
 * @Description: 监听程序是否在前后台运行Service
 * @FileName: AppStatusService.java 
 * @Package com.test.service
 * @Author Hanyonglu
 * @Date 2012-4-13 下午04:13:47 
 * @Version V1.0
 */
public class AppStatusService extends Service{
    private static final String TAG = "AppStatusService"; 
    private ActivityManager activityManager; 
    private String packageName;
    private boolean isStop = false;
    
    @Override
    public IBinder onBind(Intent intent) {
        // TODO Auto-generated method stub
        return null;
    }
    
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        // TODO Auto-generated method stub
        activityManager = (ActivityManager) this.getSystemService(Context.ACTIVITY_SERVICE); 
        packageName = this.getPackageName(); 
        System.out.println("启动服务");
        
        new Thread() { 
            public void run() { 
                try { 
                    while (!isStop) { 
                        Thread.sleep(1000); 
                        
                        if (isAppOnForeground()) { 
                            Log.v(TAG, "前台运行");
                        } else { 
                            Log.v(TAG, "后台运行");
                            showNotification();
                        } 
                    } 
                } catch (Exception e) { 
                    e.printStackTrace(); 
                } 
            } 
        }.start(); 
        
        return super.onStartCommand(intent, flags, startId);
    }
    
    /**
     * 程序是否在前台运行
     * @return
     */
    public boolean isAppOnForeground() { 
        // Returns a list of application processes that are running on the device 
        List<RunningAppProcessInfo> appProcesses = activityManager.getRunningAppProcesses(); 
        if (appProcesses == null) return false; 
        
        for (RunningAppProcessInfo appProcess : appProcesses) { 
            // The name of the process that this object is associated with. 
            if (appProcess.processName.equals(packageName) 
                    && appProcess.importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) { 
                return true; 
            } 
        } 
        
        return false; 
    } 
    
    @Override
    public void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        System.out.println("终止服务");
        isStop = true;
    }
    
    // 显示Notification
    public void showNotification() {
        // 创建一个NotificationManager的引用
        NotificationManager notificationManager = (
                NotificationManager)getSystemService(
                        android.content.Context.NOTIFICATION_SERVICE);
        
        // 定义Notification的各种属性
        Notification notification = new Notification(
                R.drawable.icon,"阅读器", 
                System.currentTimeMillis());
        // 将此通知放到通知栏的"Ongoing"即"正在运行"组中
        notification.flags |= Notification.FLAG_ONGOING_EVENT;
        // 点击后自动清除Notification
        notification.flags |= Notification.FLAG_AUTO_CANCEL;
        notification.flags |= Notification.FLAG_SHOW_LIGHTS;
        notification.defaults = Notification.DEFAULT_LIGHTS;
        notification.ledARGB = Color.BLUE;
        notification.ledOnMS = 5000;
                
        // 设置通知的事件消息
        CharSequence contentTitle = "阅读器显示信息"; // 通知栏标题
        CharSequence contentText = "推送信息显示，请查看……"; // 通知栏内容
        
        Intent notificationIntent = new Intent(AppManager.context,AppManager.context.getClass());
        notificationIntent.setAction(Intent.ACTION_MAIN);
        notificationIntent.addCategory(Intent.CATEGORY_LAUNCHER);
        PendingIntent contentIntent = PendingIntent.getActivity(
                AppManager.context, 0, notificationIntent,PendingIntent.FLAG_UPDATE_CURRENT);
        notification.setLatestEventInfo(
                AppManager.context, contentTitle, contentText, contentIntent);
        // 把Notification传递给NotificationManager
        notificationManager.notify(0, notification);
    }
}
```
　　在这里为了在信息提示栏里点击后能够返回到原来的Activity，需要在AppManager里记下我们当前的Activity。
　　最后，希望转载的朋友能够尊重作者的劳动成果，加上转载地址：[http://www.cnblogs.com/hanyonglu/archive/2012/04/15/2450551.html](http://www.cnblogs.com/hanyonglu/archive/2012/04/15/2450551.html)  谢谢。 
　　示例下载：[点击下载](http://files.cnblogs.com/hanyonglu/AndroidFile/MyAppExchange.rar)
　　完毕。^_^ 
