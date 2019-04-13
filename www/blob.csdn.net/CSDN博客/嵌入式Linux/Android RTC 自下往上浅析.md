
# Android RTC 自下往上浅析 - 嵌入式Linux - CSDN博客

2015年10月23日 17:22:48[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：2292


1.首先搞清楚RTC在kernel内的作用: linux系统有两个时钟：一个是由主板电池驱动的“Real Time Clock”也叫做RTC或者叫CMOS时钟，硬件时钟。当操作系统关机的时候，用这个来记录时间，但是对于运行的系统是不用这个时间的。
另一个时间是 “System clock”也叫内核时钟或者软件时钟，是由软件根据时间中断来进行计数的，内核时钟在系统关机的情况下是不存在的，所以，当操作系统启动的时候，内核时钟是要读取RTC时间来进行时间同步。并且在系统关机的时候将系统时间写回RTC中进行同步。
 如前所述，Linux内核与RTC进行互操作的时机只有两个：
1) 内核在启动时从RTC中读取启动时的时间与日期；
2) 内核在需要时将时间与日期回写到RTC中。 系统启动时，内核通过读取RTC来初始化内核时钟,又叫墙上时间，该时间放在xtime变量中。
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7802502#)[copy](http://blog.csdn.net/crycheng/article/details/7802502#)

The current time of day (the wall time) is defined in kernel/timer.c:
structtimespec xtime;
The timespec data structure is defined in <linux/time.h> as:
structtimespec {
time_ttv_sec;/* seconds */
longtv_nsec;/* nanoseconds */
};

最有可能读取RTC设置内核时钟的位置应该在arch/arm/kernel/time.c里的time_init函数内.time.c为系统的时钟驱动部分.
time_init函数会在系统初始化时,由init/main.c里的start_kernel函数内调用. ARM架构的time_init代码如下:
/* arch/arm/kernel/time.c */
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7802502#)[copy](http://blog.csdn.net/crycheng/article/details/7802502#)

void__init time_init(void)
{
system_timer = machine_desc->timer;
system_timer->init();
\#ifdef CONFIG_HAVE_SCHED_CLOCK
sched_clock_postinit();
\#endif
}

2.RTC结构部分
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7802502#)[copy](http://blog.csdn.net/crycheng/article/details/7802502#)

staticconststructrtc_class_ops hym8563_rtc_ops = {
.read_time  = hym8563_rtc_read_time,
.set_time   = hym8563_rtc_set_time,
.read_alarm = hym8563_rtc_read_alarm,
.set_alarm  = hym8563_rtc_set_alarm,
.ioctl      = hym8563_rtc_ioctl,
.proc       = hym8563_rtc_proc
};
staticint__devinit hym8563_probe(structi2c_client *client,conststructi2c_device_id *id)
{
intrc = 0;
u8 reg = 0;
structhym8563 *hym8563;
structrtc_device *rtc = NULL;
structrtc_time tm_read,tm= {
.tm_wday = 6,
.tm_year = 111,
.tm_mon = 0,
.tm_mday = 1,
.tm_hour = 12,
.tm_min = 0,
.tm_sec = 0,
};
if(!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
return-ENODEV;
hym8563 = kzalloc(sizeof(structhym8563), GFP_KERNEL);
if(!hym8563) {
return-ENOMEM;
}
gClient = client;
hym8563->client = client;
mutex_init(&hym8563->mutex);
wake_lock_init(&hym8563->wake_lock, WAKE_LOCK_SUSPEND,"rtc_hym8563");
INIT_WORK(&hym8563->work, hym8563_work_func);
i2c_set_clientdata(client, hym8563);
hym8563_init_device(client);
// check power down
hym8563_i2c_read_regs(client,RTC_SEC,®,1);
if(reg&0x80) {
dev_info(&client->dev,"clock/calendar information is no longer guaranteed\n");
hym8563_set_time(client, &tm);
}
hym8563_read_datetime(client, &tm_read);//read time from hym8563
if(((tm_read.tm_year < 70) | (tm_read.tm_year > 137 )) | (tm_read.tm_mon == -1) | (rtc_valid_tm(&tm_read) != 0))//if the hym8563 haven't initialized
{
hym8563_set_time(client, &tm);//initialize the hym8563
}
if(gpio_request(client->irq,"rtc gpio"))
{
dev_err(&client->dev,"gpio request fail\n");
gpio_free(client->irq);
gotoexit;
}
hym8563->irq = gpio_to_irq(client->irq);
gpio_pull_updown(client->irq,GPIOPullUp);
if(request_irq(hym8563->irq, hym8563_wakeup_irq, IRQF_TRIGGER_FALLING, client->dev.driver->name, hym8563) < 0)
{
printk("unable to request rtc irq\n");
gotoexit;
}
enable_irq_wake(hym8563->irq);
rtc = rtc_device_register(client->name, &client->dev,
&hym8563_rtc_ops, THIS_MODULE);
if(IS_ERR(rtc)) {
rc = PTR_ERR(rtc);
rtc = NULL;
gotoexit;
}
hym8563->rtc = rtc;
return0;
exit:
if(rtc)
rtc_device_unregister(rtc);
if(hym8563)
kfree(hym8563);
returnrc;
}

看这两个结构体，我认为就已经达到目的，第2个结构体是平台设备中的driver部分，也就是hym8563_probe,是个很重要的函数，在这里面，第1个结构体被顺利注册进rtc子系统。Rtc的所用到的结构体被定义在，LINUX/include/linux/rtc.h里面。
struct rtc_device这个结构体是核心部分，内核中就是靠它传递信息，不管在哪使用，都要靠它间接的调用底层信息。比如在alarm.c 中。
alarm_ioctl这个函数中，多次使用了rtc_set_time/rtc_get_time，这些函数虽然是定义在rtc目录下的interface.c 中，但实质还是rtc-hym8563.c中结构体 rtc_class_ops所指过去的函数。
也就是说在和内核层以上的交互是通过alarm-dev.c里面的alarm_ioctl及其余的函数交互，但是在这个文件里面的rtc_set_time/rtc_get_time操作是为了设置RTC时间等的操作是调用alarm.c里面的函数，但是alarm.c驱动本身和硬件没有关系，在这里屏蔽了RTC的硬件操作，比如HYM8563的时间I2C硬件驱动操作在rtc-HYM8563.c驱动里，只需要使用 rtc_class_ops进行注册就可以了，完整的实现了硬件对平台无关性的屏蔽。
那么我可以告诉你了，为什么多了一个alarm.c ，因为在android中它为了使得平台无关性提高，因此大量的增加过渡代码层，HAL就是这种性质的存在。alarm.c在用户空间中会多一个/dev/alarm 节点，而rtc-hym8563.c.c 会产生/dev/rtc这样的节点。
3.JNI层
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7802502#)[copy](http://blog.csdn.net/crycheng/article/details/7802502#)

namespaceandroid {
staticjint android_server_AlarmManagerService_setKernelTimezone(JNIEnv* env, jobject obj, jint fd, jint minswest)
{
structtimezone tz;
tz.tz_minuteswest = minswest;
tz.tz_dsttime = 0;
intresult = settimeofday(NULL, &tz);
if(result < 0) {
LOGE("Unable to set kernel timezone to %d: %s\n", minswest, strerror(errno));
return-1;
}else{
LOGD("Kernel timezone updated to %d minutes west of GMT\n", minswest);
}
return0;
}
staticjint android_server_AlarmManagerService_init(JNIEnv* env, jobject obj)
{
returnopen("/dev/alarm", O_RDWR);
}
staticvoidandroid_server_AlarmManagerService_close(JNIEnv* env, jobject obj, jint fd)
{
close(fd);
}
staticvoidandroid_server_AlarmManagerService_set(JNIEnv* env, jobject obj, jint fd, jint type, jlong seconds, jlong nanoseconds)
{
structtimespec ts;
ts.tv_sec = seconds;
ts.tv_nsec = nanoseconds;
intresult = ioctl(fd, ANDROID_ALARM_SET(type), &ts);
if(result < 0)
{
LOGE("Unable to set alarm to %lld.%09lld: %s\n", seconds, nanoseconds, strerror(errno));
}
}
staticjint android_server_AlarmManagerService_waitForAlarm(JNIEnv* env, jobject obj, jint fd)
{
intresult = 0;
do
{
result = ioctl(fd, ANDROID_ALARM_WAIT);
}while(result < 0 && errno == EINTR);
if(result < 0)
{
LOGE("Unable to wait on alarm: %s\n", strerror(errno));
return0;
}
returnresult;
}
staticJNINativeMethod sMethods[] = {
/* name, signature, funcPtr */
{"init","()I", (void*)android_server_AlarmManagerService_init},
{"close","(I)V", (void*)android_server_AlarmManagerService_close},
{"set","(IIJJ)V", (void*)android_server_AlarmManagerService_set},
{"waitForAlarm","(I)I", (void*)android_server_AlarmManagerService_waitForAlarm},
{"setKernelTimezone","(II)I", (void*)android_server_AlarmManagerService_setKernelTimezone},
};
intregister_android_server_AlarmManagerService(JNIEnv* env)
{
returnjniRegisterNativeMethods(env,"com/android/server/AlarmManagerService",
sMethods, NELEM(sMethods));
}
}/* namespace android */

其实在JNI层这里RTC就和其余的模块一样，直接去通过打开/关闭/设置/等待等来操作节点/dev/alarm和底层进行通信，不仔细解释。
**4、 framework层**
frameworks/base/services/java/com/android/server/AlarmManagerService.java
frameworks/base/core/java/android/app/AlarmManager.java
下面的是直接提供给app层的API接口，它是AlarmManagerService.java的一个封装。
这里只是简单的解释下service到底在此做什么了。
其实也没做什么，仅仅是把上面分析的JNI拿来在此调用一下而已。然后包装一下，将功能实现得更完美些。
5.App层
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7802502#)[copy](http://blog.csdn.net/crycheng/article/details/7802502#)

package android.app;
import android.content.Context;
import android.content.Intent;
import android.os.RemoteException;
import android.os.ServiceManager;
publicclassAlarmManager
{
publicstaticfinalintRTC_WAKEUP = 0;
publicstaticfinalintRTC = 1;
publicstaticfinalintELAPSED_REALTIME_WAKEUP = 2;
publicstaticfinalintELAPSED_REALTIME = 3;
privatefinal IAlarmManager mService;
AlarmManager(IAlarmManager service) {
mService = service;
}
publicvoidset(inttype,longtriggerAtTime, PendingIntent operation) {
try{
mService.set(type, triggerAtTime, operation);
}catch(RemoteException ex) {
}
}
publicvoidsetRepeating(inttype,longtriggerAtTime,longinterval,
PendingIntent operation) {
try{
mService.setRepeating(type, triggerAtTime, interval, operation);
}catch(RemoteException ex) {
}
}
publicstaticfinallongINTERVAL_FIFTEEN_MINUTES = 15 * 60 * 1000;
publicstaticfinallongINTERVAL_HALF_HOUR = 2*INTERVAL_FIFTEEN_MINUTES;
publicstaticfinallongINTERVAL_HOUR = 2*INTERVAL_HALF_HOUR;
publicstaticfinallongINTERVAL_HALF_DAY = 12*INTERVAL_HOUR;
publicstaticfinallongINTERVAL_DAY = 2*INTERVAL_HALF_DAY;
publicvoidsetInexactRepeating(inttype,longtriggerAtTime,longinterval,
PendingIntent operation) {
try{
mService.setInexactRepeating(type, triggerAtTime, interval, operation);
}catch(RemoteException ex) {
}
}
publicvoidcancel(PendingIntent operation) {
try{
mService.remove(operation);
}catch(RemoteException ex) {
}
}
publicvoidsetTime(longmillis) {
try{
mService.setTime(millis);
}catch(RemoteException ex) {
}
}
publicvoidsetTimeZone(String timeZone) {
try{
mService.setTimeZone(timeZone);
}catch(RemoteException ex) {
}
}
}

frameworks\base\core\java\android\app这个目录下，就是系统自带定时器的源代码，比如Alarms.java
 中：第一个导入的包就是 import android.app.AlarmManager。

