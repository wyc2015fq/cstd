
# Android关机闹钟实现 - 嵌入式Linux - CSDN博客

2015年10月29日 19:07:15[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：9581


**时间转换网站：http://tool.chinaz.com/Tools/unixtime.aspx**

**１、apk层**
这个还是比较简单的，百度一下就可以看到apk的代码，我之前也有贴出来过还是看一下核心代码吧。
写好的apk（里面有Android.mk文件　加入system/app/下面进行编译）：[http://download.csdn.net/detail/weiqifa0/9237021](http://download.csdn.net/detail/weiqifa0/9237021)

```python
package com.example.helloworld;
import java.util.Calendar;
import android.os.Bundle;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.Service;
import android.app.TimePickerDialog;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
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
	public static final int POWER_OFF_WAKE_UP = 8;//用来设置关机启动的参数　平台这边已经设置好了
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Log.e("weiqifa", test());
		setTime = (Button) findViewById(R.id.setTime);
		aManager=(AlarmManager)AlarmTest.this.getSystemService(Service.ALARM_SERVICE);
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
							Intent intent = new Intent(AlarmTest.this,AlarmActivity.class);
							PendingIntent pi = PendingIntent.getActivity(AlarmTest.this, 0, intent, 0);
							Calendar c = Calendar.getInstance();
							c.set(Calendar.HOUR, hourOfDay);
							c.set(Calendar.MINUTE, minute);
							Log.v("weiqifa", "c.getTimeMillis()"+c.getTimeInMillis());
							//aManager.setExact(POWER_OFF_WAKE_UP,c.getTimeInMillis(), pi);
							aManager.setExact(AlarmManager.ELAPSED_REALTIME_WAKEUP,c.getTimeInMillis(), pi);
							Toast.makeText(AlarmTest.this, "设置闹钟成功", Toast.LENGTH_SHORT).show();
							Log.e("weiqifa", "set the clock success!");
						}
					}, currentTime.get(Calendar.HOUR_OF_DAY), currentTime
						.get(Calendar.MINUTE), false).show();
			}
		});
	}
	public native String  test();
	static {
		try{
			Log.i("JNI", "Trying to load libhelloworld.so");
			System.loadLibrary("helloWorld");
		}catch(UnsatisfiedLinkError ule){
			Log.e("JNI", "Warning : could not load the libhelloworld.so");
		}
	}
}</span>
```
好了关键就是　aManager.setExact(AlarmManager.ELAPSED_REALTIME_WAKEUP,c.getTimeInMillis(), pi);这句代码
**２、AlarmManager和AlarmManagerService**
他们一个是客户端一个是服务端　他们通过ipc　binder通信，我对这个还不是非常懂，但是有个网友总结的非常好
http://blog.csdn.net/jdsjlzx/article/details/20936709

**３、JNI　要想达到关机闹钟的功能就一定要调用JNI**
```python
public void setExact(int type, long triggerAtMillis, PendingIntent operation) {
    	 Log.i(TAG,"setExact"+type);
        setImpl(type, triggerAtMillis, WINDOW_EXACT, 0, operation, null);
    }</span>
```

```python
private void setImpl(int type, long triggerAtMillis, long windowMillis, long intervalMillis,
            PendingIntent operation, WorkSource workSource) {
            Log.i(TAG,"setImpl"+type);
        if (triggerAtMillis < 0) {
            /* NOTYET
            if (mAlwaysExact) {
                // Fatal error for KLP+ apps to use negative trigger times
                throw new IllegalArgumentException("Invalid alarm trigger time "
                        + triggerAtMillis);
            }
            */
            triggerAtMillis = 0;
        }
        try {
            mService.set(type, triggerAtMillis, windowMillis, intervalMillis, operation,
                    workSource);
        } catch (RemoteException ex) {
        }
    }
```
mService.set这个函数在服务端实现，所以要到AlarmManagerService里面去找到这个函数的实现，因为太长就不贴出来了

通过ＪＮＩ去设置闹钟,在AlarmManagerService里面有调用jni函数
```python
public void scheduleTimeTickEvent() {
            final long currentTime = System.currentTimeMillis();
            final long nextTime = 60000 * ((currentTime / 60000) + 1);
            // Schedule this event for the amount of time that it would take to get to
            // the top of the next minute.
            final long tickEventDelay = nextTime - currentTime;
            final WorkSource workSource = null; // Let system take blame for time tick events.
            set(ELAPSED_REALTIME, SystemClock.elapsedRealtime() + tickEventDelay, 0,
                    0, mTimeTickSender, true, workSource);
        }
```
```python
private native int init();
    private native void close(int fd);
    private native void set(int fd, int type, long seconds, long nanoseconds);
    private native int waitForAlarm(int fd);
    private native int setKernelTimezone(int fd, int minuteswest);</span>
```
里面的set(mDescriptor, 6, latestTime / 1000, (latestTime % 1000) * 1000 * 1000);调用到jni里面的函数。
看到这个native就知道这是一个jni函数了吧

**４、JNI通过ioctl去调用驱动里面的代码**
这里是在./frameworks/base/services/jni/com_android_server_AlarmManagerService.cpp里面

```python
static void android_server_AlarmManagerService_set(JNIEnv* env, jobject obj, jint fd, jint type, jlong seconds, jlong nanoseconds)
{
    struct timespec ts;
    ts.tv_sec = seconds;
    ts.tv_nsec = nanoseconds;
	ALOGE("weiqifa type=%d set fd[%d]alarm to %lld.%09lld: %s\n",type,fd, seconds, nanoseconds, strerror(errno));
	int result = ioctl(fd, ANDROID_ALARM_SET(type), &ts);
	if (result < 0)
	{
        ALOGE("Unable to set alarm to %lld.%09lld: %s\n", seconds, nanoseconds, strerror(errno));
    }
}
```
**５、驱动代码**
```python
static long alarm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int rv = 0;
	unsigned long flags;
	struct timespec new_alarm_time;
	struct timespec new_rtc_time;
	struct timespec tmp_time;
	struct rtc_time new_rtc_tm;
	struct rtc_device *rtc_dev;
	struct rtc_wkalrm pwron_alm;
	enum android_alarm_type alarm_type = ANDROID_ALARM_IOCTL_TO_TYPE(cmd);
	uint32_t alarm_type_mask = 1U << alarm_type;
	printk("%s cmd=[0x%x] ANDROID_ALARM_IOCTL_TO_TYPE(cmd)=[%d]\n",__func__,cmd,ANDROID_ALARM_IOCTL_TO_TYPE(cmd));
 	if (alarm_type >= ANDROID_ALARM_TYPE_COUNT &&
	    alarm_type != ANDROID_ALARM_POWER_ON &&
	    alarm_type != ANDROID_ALARM_POWER_ON_LOGO) {
		return -EINVAL;
	}
	
	if (ANDROID_ALARM_BASE_CMD(cmd) != ANDROID_ALARM_GET_TIME(0)) {
		if ((file->f_flags & O_ACCMODE) == O_RDONLY)
			return -EPERM;
		if (file->private_data == NULL &&
		    cmd != ANDROID_ALARM_SET_RTC) {
			spin_lock_irqsave(&alarm_slock, flags);
			if (alarm_opened) {
				spin_unlock_irqrestore(&alarm_slock, flags);
				return -EBUSY;
			}
			alarm_opened = 1;
			file->private_data = (void *)1;
			spin_unlock_irqrestore(&alarm_slock, flags);
		}
	}</span>
```
后面再调用到,mtk_rtc_hal.c里面
```python
void hal_rtc_set_alarm_time(struct rtc_time *tm) {
		u16 irqen;
		dump_stack();
		printk("weiqifa read tc time = %04d/%02d/%02d (%d) %02d:%02d:%02d\n",
		          tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
		          tm->tm_wday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		hal_rtc_xinfo("a = %d\n",(rtc_read(RTC_AL_MTH)& (RTC_NEW_SPARE3))|tm->tm_mon);
		hal_rtc_xinfo("b = %d\n",(rtc_read(RTC_AL_DOM)& (RTC_NEW_SPARE1))|tm->tm_mday);
		hal_rtc_xinfo("c = %d\n",(rtc_read(RTC_AL_HOU)& (RTC_NEW_SPARE_FG_MASK))|tm->tm_hour);
		rtc_write(RTC_AL_YEA, tm->tm_year);
		rtc_write(RTC_AL_MTH, (rtc_read(RTC_AL_MTH) & (RTC_NEW_SPARE3))|tm->tm_mon);
		rtc_write(RTC_AL_DOM, (rtc_read(RTC_AL_DOM) & (RTC_NEW_SPARE1))|tm->tm_mday);
		rtc_write(RTC_AL_HOU, (rtc_read(RTC_AL_HOU) & (RTC_NEW_SPARE_FG_MASK))|tm->tm_hour);
		rtc_write(RTC_AL_MIN, tm->tm_min);
		rtc_write(RTC_AL_SEC, tm->tm_sec);
		rtc_write(RTC_AL_MASK, RTC_AL_MASK_DOW);		/* mask DOW */
		rtc_write_trigger();
		irqen = rtc_read(RTC_IRQ_EN) | RTC_IRQ_EN_ONESHOT_AL;
		rtc_write(RTC_IRQ_EN, irqen);
		rtc_write_trigger();
	}
```
驱动的调用关系可以用dump_stack()来调试然后加载vmlinux可以定位到哪一行，

最后闹钟可以在手机关机的时候也能够响。
**６、问题**
通过上面的流程，基本上可以理通了整个思路，但是还是要修改一下代码，贴出git diff

```python
a/frameworks/base/services/java/com/android/server/AlarmManagerService.java
+++ b/frameworks/base/services/java/com/android/server/AlarmManagerService.java
@@ -673,6 +673,7 @@ class AlarmManagerService extends IAlarmManager.Stub {
 
             String setPackageName = null;
             long nowTime = System.currentTimeMillis();
+            Slog.d(TAG,"weiqifa nowTime="+nowTime+"  triggerAtTime="+triggerAtTime);
             if (triggerAtTime < nowTime) {
                 Slog.w(TAG, "power off alarm set time is wrong!");
                 return;
@@ -684,11 +685,12 @@ class AlarmManagerService extends IAlarmManager.Stub {
                 Alarm alarm = new Alarm(type, triggerAtTime, 0, 0, 0, interval, operation, workSource);
                 int index = addPoweroffAlarmLocked(alarm);
                 if (index == 0) {
+                                       Slog.w(TAG, "weiqifa==================1");
                     resetPoweroffAlarm(alarm);
                 }
             }
                 type = RTC_WAKEUP;
-
+                  Slog.w(TAG, "weiqifa==================2");
         }
         // /@}
 
@@ -2095,7 +2097,12 @@ class AlarmManagerService extends IAlarmManager.Stub {
             SystemProperties.set("persist.sys.bootpackage", "2"); // for
                                                                   // poweronofftest
             set(mDescriptor, 7, latestTime / 1000, (latestTime % 1000) * 1000 * 1000);
-        } else {
+        } else if(setPackageName.equals("com.example.helloworld")){
+               Slog.i(TAG, "mBootPackage = " + setPackageName + " set Prop 1");
+            SystemProperties.set("persist.sys.bootpackage", "1"); // for
+                                                                  // helloworld test clock
+            set(mDescriptor, 6, latestTime / 1000, (latestTime % 1000) * 1000 * 1000); 
+        }else {
             Slog.w(TAG, "unknown package (" + setPackageName + ") to set power off alarm");
         }
         // [Note] Power off Alarm -
(END)
```
这里要加上这个包的名字，要不然就设置不上去了。
apk里面的时间　是格林威治时间　这个可以网上百度一下
![](https://img-blog.csdn.net/20151030112356026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后apk的日志打印出来的时间，如下图
![](https://img-blog.csdn.net/20151030112427368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
kernel日志打印出来的时间如下图
![](https://img-blog.csdn.net/20151030112449162?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


