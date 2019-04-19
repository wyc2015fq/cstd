# android AppWidgetProvider 定时刷新问题 - xqhrs232的专栏 - CSDN博客
2012年04月02日 14:18:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：645
原文地址:: [http://blog.csdn.net/maydie1989/article/details/6460163](http://blog.csdn.net/maydie1989/article/details/6460163)
相关网帖
1.AppWidgetProvider使用问题----[http://www.eoeandroid.com/thread-50955-1-1.html](http://www.eoeandroid.com/thread-50955-1-1.html)
2.关于 Android widget 配置文件里updatePeriodMillis的问题----[http://blog.sina.com.cn/s/blog_7db89cd50100qu3m.html](http://blog.sina.com.cn/s/blog_7db89cd50100qu3m.html)
AppWidgetProvider是专门用来制作android桌面组件的东东
本来想做一个小时钟
但是在定时刷新的时候遇到了问题
很多资料和书上的方法是
在onUpdate()方法中
用Intent请求Service
    Intent intent = new Intent(context, UpdateService.class);
    context.startService(intent);
在service中的onStart()或者onReceive()方法中来实现更新界面
而在AndroidManifest.xml中定义一个receiver
    <receiver
      android:label="@string/app_name"
      android:name=".DeskMain">
            <intent-filter>  
                <action android:name="android.appwidget.action.APPWIDGET_UPDATE" />  
            </intent-filter>  
     <meta-data android:name="android.appwidget.provider"
        android:resource="@xml/my_widget_provider" >
      </meta-data>
    </receiver>
DeskMain是你的AppWidgetProvider类 @xml/my_widget_provider是提供UI信息的配置文件 比如宽高
android.appwidget.action.APPWIDGET_UPDATE 书上的解释是 当AppWidget更新时会收到的系统广播信息
但是用eclipse填写xml的时候这个选项已经没有了 于是上网查了一下
在 appwidget-provider 用 android:updatePeriodMillis 这种方式已经在SKD1.5以后被废了
解决的办法找了一下 大概总结了几种
1.在onUpdate()中用自己的Timer和TimerTask解决
[http://www.eoeandroid.com/thread-50955-1-1.html](http://www.eoeandroid.com/thread-50955-1-1.html)
2.用Thread
3.Intent intent=new Intent(context ,WidgetService.class);
        PendingIntent refreshIntent=PendingIntent.getService(context, 0, intent, 0);
        AlarmManager alarm=(AlarmManager)context.getSystemService(Context.ALARM_SERVICE);
        alarm.setRepeating(AlarmManager.RTC, 0, 1000, refreshIntent);//每秒1次
        context.startService(intent);     
service类↓
[java:nogutter:collapse][view
 plain](http://blog.csdn.net/maydie1989/article/details/6460163#)[copy](http://blog.csdn.net/maydie1989/article/details/6460163#)[print](http://blog.csdn.net/maydie1989/article/details/6460163#)[?](http://blog.csdn.net/maydie1989/article/details/6460163#)
- publicclass WidgetService extends Service  
- {  
- 
- @Override
- public IBinder onBind(Intent intent)  
-     {         
- returnnull;  
-     }  
- 
- @Override
- publicvoid onStart(Intent intent, int startId)  
-     {     
- super.onStart(intent, startId);  
-         RemoteViews rv = new RemoteViews(this.getPackageName(), R.layout.app);  
-         rv.setTextViewText(R.id.av1, new Date().toLocaleString());  
-         ComponentName cn = new ComponentName(this, DeskMain.class);  
-         AppWidgetManager am = AppWidgetManager.getInstance(this);  
-         am.updateAppWidget(cn, rv);  
-     }  
- }  
AppWidgetProvider类↓
[java:nogutter:collapse][view
 plain](http://blog.csdn.net/maydie1989/article/details/6460163#)[copy](http://blog.csdn.net/maydie1989/article/details/6460163#)[print](http://blog.csdn.net/maydie1989/article/details/6460163#)[?](http://blog.csdn.net/maydie1989/article/details/6460163#)
- publicclass DeskMain extends AppWidgetProvider  
- {  
- 
- @Override
- publicvoid onUpdate(Context context, AppWidgetManager appWidgetManager,  
- int[] appWidgetIds)  
-     {  
-         Log.i("syso", "update3...................");  
-         Intent intent=new Intent(context ,WidgetService.class);  
-         PendingIntent refreshIntent=PendingIntent.getService(context, 0, intent, 0);  
-         AlarmManager alarm=(AlarmManager)context.getSystemService(Context.ALARM_SERVICE);  
-         alarm.setRepeating(AlarmManager.RTC, 0, 1000, refreshIntent);  
-         context.startService(intent);        
-     }  
- 
- @Override
- publicvoid onReceive(Context context, Intent intent)  
-     {  
-         Log.i("syso", "receive...................");  
- super.onReceive(context, intent);  
-     }  
AndroidManifest.xml
[xhtml][view
 plain](http://blog.csdn.net/maydie1989/article/details/6460163#)[copy](http://blog.csdn.net/maydie1989/article/details/6460163#)[print](http://blog.csdn.net/maydie1989/article/details/6460163#)[?](http://blog.csdn.net/maydie1989/article/details/6460163#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <manifestxmlns:android="http://schemas.android.com/apk/res/android"
- package="com.app"
- android:versionCode="1"
- android:versionName="1.0">
- <uses-permissionandroid:name="android.permission.ACCESS_FINE_LOCATION"></uses-permission>
- 
- 
- <applicationandroid:icon="@drawable/icon"android:label="@string/app_name"android:screenOrientation="portrait"android:permission="android.permission.ACCESS_FINE_LOCATION">
- <activityandroid:name=".Main"
- android:label="@string/app_name">
- <!--  -->
- <receiver
- android:label="@string/app_name"
- android:name=".DeskMain">
- <intent-filter>
- <actionandroid:name="android.appwidget.action.APPWIDGET_UPDATE"/>
- </intent-filter>
- <meta-dataandroid:name="android.appwidget.provider"
- android:resource="@xml/my_widget_provider">
- </meta-data>
- </receiver>
- <serviceandroid:name="WidgetService"/>
- 
- </application>
- <uses-sdkandroid:minSdkVersion="7"/>
- 
- </manifest>
layout下的app.xml UI布局                                          xml下的my_widget_provider.xml 提供了UI宽高信息
[java:nogutter:collapse][view
 plain](http://blog.csdn.net/maydie1989/article/details/6460163#)[copy](http://blog.csdn.net/maydie1989/article/details/6460163#)[print](http://blog.csdn.net/maydie1989/article/details/6460163#)[?](http://blog.csdn.net/maydie1989/article/details/6460163#)
- <?xml version="1.0" encoding="utf-8"?>  
- <LinearLayout  
-   xmlns:android="http://schemas.android.com/apk/res/android"
-   android:layout_width="fill_parent"
-   android:layout_height="fill_parent" android:orientation="horizontal" android:gravity="center">  
-     <ImageView android:src="@drawable/icon" android:layout_height="wrap_content" android:layout_width="wrap_content" android:id="@+id/imageView1"></ImageView>  
- 
-     <TextView   
-      android:gravity="center" android:text="TextView" android:id="@+id/av1" android:layout_width="wrap_content" android:layout_height="wrap_content"></TextView>  
- </LinearLayout>  
[xhtml:nogutter:collapse][view
 plain](http://blog.csdn.net/maydie1989/article/details/6460163#)[copy](http://blog.csdn.net/maydie1989/article/details/6460163#)[print](http://blog.csdn.net/maydie1989/article/details/6460163#)[?](http://blog.csdn.net/maydie1989/article/details/6460163#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <appwidget-providerxmlns:android="http://schemas.android.com/apk/res/android"
- android:minWidth="95dip"
- android:minHeight="73dip"
- android:initialLayout="@layout/app"/>

