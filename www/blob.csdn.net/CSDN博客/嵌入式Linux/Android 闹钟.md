
# Android 闹钟 - 嵌入式Linux - CSDN博客

2015年10月12日 11:40:20[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：639个人分类：[Android																](https://blog.csdn.net/weiqifa0/article/category/1752017)



需求：新的平台要实现关机启动，所以要了解一下闹钟的机制
这个链接写得比较详细（我只是动手试了试，毕竟应用不是专长）：http://www.cnblogs.com/mengdd/p/3819806.html
１、AlarmManager　这个是闹钟的核心
里面有void set函数，void setInexactRepeating函数　通过这些函数来设置

源码链接：http://download.csdn.net/detail/weiqifa0/9173341
设置闹钟的代码：
package com.example.helloworld;
import java.util.Calendar;
import android.os.Bundle;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.Service;
import android.app.TimePickerDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TimePicker;
import android.widget.Toast;
public class AlarmTest extends Activity
{
Button setTime;
AlarmManager aManager;
Calendar currentTime = Calendar.getInstance();
@Override
public void onCreate(Bundle savedInstanceState)
{
super.onCreate(savedInstanceState);
setContentView(R.layout.activity_main);
setTime = (Button) findViewById(R.id.setTime);
aManager = (AlarmManager) getSystemService(
Service.ALARM_SERVICE);
setTime.setOnClickListener(new OnClickListener()
{
@Override
public void onClick(View source)
{
Calendar currentTime = Calendar.getInstance();
new TimePickerDialog(AlarmTest.this, 0,
new TimePickerDialog.OnTimeSetListener()
{
@Override
public void onTimeSet(TimePicker tp,
int hourOfDay, int minute)
{
Intent intent = new Intent(AlarmTest.this,
AlarmActivity.class);
PendingIntent pi = PendingIntent.getActivity(
AlarmTest.this, 0, intent, 0);
Calendar c = Calendar.getInstance();
c.set(Calendar.HOUR, hourOfDay);
c.set(Calendar.MINUTE, minute);
**aManager.set(AlarmManager.RTC_WAKEUP,**
**c.getTimeInMillis(), pi);  //这个是代码的核心部分**
Toast.makeText(AlarmTest.this, "设置闹钟成功"
, Toast.LENGTH_SHORT).show();
}
}, currentTime.get(Calendar.HOUR_OF_DAY), currentTime
.get(Calendar.MINUTE), false).show();
}
});
}
}

到时后就会去启动另一个Activity ,核心代码如下：
package com.example.helloworld;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.widget.Toast;
/**
* Description:
* <br/>��վ: <a href="http://www.crazyit.org">���Java����</a>
* <br/>Copyright (C), 2001-2014, Yeeku.H.Lee
* <br/>This program is protected by copyright laws.
* <br/>Program Name:
* <br/>Date:
* @author  Yeeku.H.Lee kongyeeku@163.com
* @version  1.0
*/
public class AlarmActivity extends Activity
{
MediaPlayer alarmMusic;
@Override
public void onCreate(Bundle savedInstanceState)
{
super.onCreate(savedInstanceState);
Toast.makeText(AlarmActivity.this, "设置闹钟时间到"
, Toast.LENGTH_SHORT).show();
new AlertDialog.Builder(AlarmActivity.this).setTitle("����")
.setMessage("１,２")
.setPositiveButton("ＯＫ", new OnClickListener()
{
@Override
public void onClick(DialogInterface dialog, int which)
{
// ֹͣ����
//alarmMusic.stop();
// �����Activity 　　//疯狂android里面显示的声音　简单的用法我把声音去掉了
AlarmActivity.this.finish();
}
}).show();
}
}

AndroidMainfest.xml的代码，每一个Activity都要在里面注册，我第一次写的时候没有注册，结果启动不成功
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
package="com.example.helloworld"
android:versionCode="1"
android:versionName="1.0" >
<uses-sdk
android:minSdkVersion="8"
android:targetSdkVersion="18" />
<application
android:allowBackup="true"
android:icon="@drawable/ic_launcher"
android:label="@string/app_name"
android:theme="@style/AppTheme" >
<activity
android:name="com.example.helloworld.AlarmTest"
android:label="@string/app_name" >
<intent-filter>
<action android:name="android.intent.action.MAIN" />
<category android:name="android.intent.category.LAUNCHER" />
</intent-filter>
</activity>
**<activity android:name=".AlarmActivity"/>****//刚开始忘记了这行代码**
</application>
</manifest>



