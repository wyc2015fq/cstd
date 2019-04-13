
# Android Alarm自上而下 调试浅析 - 嵌入式Linux - CSDN博客

2015年10月23日 17:24:50[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：929


1.为了创建一个新的Alarm，使用set方法并指定一个Alarm类型、触发时间和在Alarm触发时要调用的Intent。如果你设定的Alarm发生在过去，那么，它将立即触发。
这里有4种Alarm类型。你的选择将决定你在set方法中传递的时间值代表什么，是特定的时间或者是时间流逝：
❑ RTC_WAKEUP
在指定的时刻（设置Alarm的时候），唤醒设备来触发Intent。
❑ RTC
在一个显式的时间触发Intent，但不唤醒设备。
❑ ELAPSED_REALTIME
从设备启动后，如果流逝的时间达到总时间，那么触发Intent，但不唤醒设备。流逝的时间包括设备睡眠的任何时间。注意一点的是，时间流逝的计算点是自从它最后一次启动算起。
❑ ELAPSED_REALTIME_WAKEUP
从设备启动后，达到流逝的总时间后，如果需要将唤醒设备并触发Intent。

2.Alarm 调用流程，alarm的流程实现了从上层应用一直到下面driver的调用流程，下面简单阐述：
点击Clock 应用程序，然后设置新闹钟，会调到  Alarms.java  里面的

**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

publicstaticlongsetAlarm(Context context, Alarm alarm) {
ContentValues values = createContentValues(alarm);
ContentResolver resolver = context.getContentResolver();
resolver.update(
ContentUris.withAppendedId(Alarm.Columns.CONTENT_URI, alarm.id),
values, null, null);
longtimeInMillis = calculateAlarm(alarm);
if(alarm.enabled) {
// Disable the snooze if we just changed the snoozed alarm. This
// only does work if the snoozed alarm is the same as the given
// alarm.
// TODO: disableSnoozeAlert should have a better name.
disableSnoozeAlert(context, alarm.id);
// Disable the snooze if this alarm fires before the snoozed alarm.
// This works on every alarm since the user most likely intends to
// have the modified alarm fire next.
clearSnoozeIfNeeded(context, timeInMillis);
}
setNextAlert(context);
returntimeInMillis;
}

然后这里面也会调用到

**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

publicstaticvoidsetNextAlert(final Context context) {
final Alarm alarm = calculateNextAlert(context);
if(alarm != null) {
enableAlert(context, alarm, alarm.time);
}else{
disableAlert(context);
}
}
calculateNextAlert(context);   //new 一个新的alarm
然后继续调用到
private static void enableAlert(Context context, final Alarm alarm,final long atTimeInMillis)
其中am.set(AlarmManager.RTC_WAKEUP, atTimeInMillis, sender);//这里是RTC_WAKEUP, 这就保证了即使系统睡眠了，都能唤醒，闹钟工作（android平台关机闹钟好像不行）
然后就调用到了AlarmManager.java 里面方法
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

publicvoidset(inttype,longtriggerAtTime, PendingIntent operation) {
try{
mService.set(type, triggerAtTime, operation);
}catch(RemoteException ex) {
}
}

然后就调用到了AlarmManagerService.java  里面方法
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

publicvoidset(inttype,longtriggerAtTime, PendingIntent operation) {
setRepeating(type, triggerAtTime, 0, operation);
}

然后继续调用
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

publicvoidsetRepeating(inttype,longtriggerAtTime,longinterval,
PendingIntent operation) {
.....
synchronized (mLock) {
Alarm alarm =newAlarm();
alarm.type = type;
alarm.when = triggerAtTime;
alarm.repeatInterval = interval;
alarm.operation = operation;
// Remove this alarm if already scheduled.
removeLocked(operation);
if(localLOGV) Slog.v(TAG,"set: "+ alarm);
intindex = addAlarmLocked(alarm);
if(index == 0) {
setLocked(alarm);
}
}
}

然后就调用到
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

privatevoidsetLocked(Alarm alarm)
{
......
set(mDescriptor, alarm.type, alarmSeconds, alarmNanoseconds);//mDescriptor  这里的文件是 /dev/alarm
.....
}
这里就调用到jni了
private native void set(int fd, int type, long seconds, long nanoseconds);
这就调用到了com_android_server_AlarmManagerService.cpp 里面
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

staticJNINativeMethod sMethods[] = {
/* name, signature, funcPtr */
{"init","()I", (void*)android_server_AlarmManagerService_init},
{"close","(I)V", (void*)android_server_AlarmManagerService_close},
{"set","(IIJJ)V", (void*)android_server_AlarmManagerService_set},
{"waitForAlarm","(I)I", (void*)android_server_AlarmManagerService_waitForAlarm},
{"setKernelTimezone","(II)I", (void*)android_server_AlarmManagerService_setKernelTimezone},
};
set 对应的是android_server_AlarmManagerService_set， 具体是
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

staticvoidandroid_server_AlarmManagerService_set(JNIEnv* env, jobject obj, jint fd, jint type, jlong seconds, jlong nanoseconds)
{
\#if HAVE_ANDROID_OS
structtimespec ts;
ts.tv_sec = seconds;
ts.tv_nsec = nanoseconds;
intresult = ioctl(fd, ANDROID_ALARM_SET(type), &ts);
if(result < 0)
{
LOGE("Unable to set alarm to %lld.%09lld: %s\n", seconds, nanoseconds, strerror(errno));
}
\#endif
}
然后ioctl 就调用到了alarm-dev.c

**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

staticlongalarm_ioctl(structfile *file, unsignedintcmd, unsignedlongarg)
{
intrv = 0;
unsignedlongflags;
structtimespec new_alarm_time;
structtimespec new_rtc_time;
structtimespec tmp_time;
enumandroid_alarm_type alarm_type = ANDROID_ALARM_IOCTL_TO_TYPE(cmd);
uint32_t alarm_type_mask = 1U << alarm_type;
printk(">>%s cmd == %d\n",__FUNCTION__,cmd);
if(alarm_type >= ANDROID_ALARM_TYPE_COUNT)
return-EINVAL;
if(ANDROID_ALARM_BASE_CMD(cmd) != ANDROID_ALARM_GET_TIME(0)) {
if((file->f_flags & O_ACCMODE) == O_RDONLY)
return-EPERM;
if(file->private_data == NULL &&
cmd != ANDROID_ALARM_SET_RTC) {
spin_lock_irqsave(&alarm_slock, flags);
if(alarm_opened) {
spin_unlock_irqrestore(&alarm_slock, flags);
return-EBUSY;
}
alarm_opened = 1;
file->private_data = (void*)1;
spin_unlock_irqrestore(&alarm_slock, flags);
}
}
switch(ANDROID_ALARM_BASE_CMD(cmd)) {
caseANDROID_ALARM_CLEAR(0):
spin_lock_irqsave(&alarm_slock, flags);
pr_alarm(IO,"alarm %d clear\n", alarm_type);
alarm_try_to_cancel(&alarms[alarm_type]);
if(alarm_pending) {
alarm_pending &= ~alarm_type_mask;
if(!alarm_pending && !wait_pending)
wake_unlock(&alarm_wake_lock);
}
alarm_enabled &= ~alarm_type_mask;
spin_unlock_irqrestore(&alarm_slock, flags);
break;
caseANDROID_ALARM_SET_OLD:
caseANDROID_ALARM_SET_AND_WAIT_OLD:
if(get_user(new_alarm_time.tv_sec, (int__user *)arg)) {
rv = -EFAULT;
gotoerr1;
}
new_alarm_time.tv_nsec = 0;
gotofrom_old_alarm_set;
caseANDROID_ALARM_SET_AND_WAIT(0):
caseANDROID_ALARM_SET(0):
if(copy_from_user(&new_alarm_time, (void__user *)arg,
sizeof(new_alarm_time))) {
rv = -EFAULT;
gotoerr1;
}
from_old_alarm_set:
spin_lock_irqsave(&alarm_slock, flags);
pr_alarm(IO,"alarm %d set %ld.%09ld\n", alarm_type,
new_alarm_time.tv_sec, new_alarm_time.tv_nsec);
alarm_enabled |= alarm_type_mask;
alarm_start_range(&alarms[alarm_type],
timespec_to_ktime(new_alarm_time),
timespec_to_ktime(new_alarm_time));
spin_unlock_irqrestore(&alarm_slock, flags);
if(ANDROID_ALARM_BASE_CMD(cmd) != ANDROID_ALARM_SET_AND_WAIT(0)
&& cmd != ANDROID_ALARM_SET_AND_WAIT_OLD)
break;
/* fall though */
caseANDROID_ALARM_WAIT:
spin_lock_irqsave(&alarm_slock, flags);
pr_alarm(IO,"alarm wait\n");
if(!alarm_pending && wait_pending) {
wake_unlock(&alarm_wake_lock);
wait_pending = 0;
}
spin_unlock_irqrestore(&alarm_slock, flags);
rv = wait_event_interruptible(alarm_wait_queue, alarm_pending);
if(rv)
gotoerr1;
spin_lock_irqsave(&alarm_slock, flags);
rv = alarm_pending;
wait_pending = 1;
alarm_pending = 0;
spin_unlock_irqrestore(&alarm_slock, flags);
break;
caseANDROID_ALARM_SET_RTC:
if(copy_from_user(&new_rtc_time, (void__user *)arg,
sizeof(new_rtc_time))) {
rv = -EFAULT;
gotoerr1;
}
rv = alarm_set_rtc(new_rtc_time);
spin_lock_irqsave(&alarm_slock, flags);
alarm_pending |= ANDROID_ALARM_TIME_CHANGE_MASK;
wake_up(&alarm_wait_queue);
spin_unlock_irqrestore(&alarm_slock, flags);
if(rv < 0)
gotoerr1;
break;
caseANDROID_ALARM_GET_TIME(0):
switch(alarm_type) {
caseANDROID_ALARM_RTC_WAKEUP:
caseANDROID_ALARM_RTC:
getnstimeofday(&tmp_time);
break;
caseANDROID_ALARM_ELAPSED_REALTIME_WAKEUP:
caseANDROID_ALARM_ELAPSED_REALTIME:
tmp_time =
ktime_to_timespec(alarm_get_elapsed_realtime());
break;
caseANDROID_ALARM_TYPE_COUNT:
caseANDROID_ALARM_SYSTEMTIME:
ktime_get_ts(&tmp_time);
break;
}
if(copy_to_user((void__user *)arg, &tmp_time,
sizeof(tmp_time))) {
rv = -EFAULT;
gotoerr1;
}
break;
default:
rv = -EINVAL;
gotoerr1;
}
err1:
returnrv;
}

alarm.c  里面实现了 alarm_suspend  alarm_resume 函数
就是如果系统没有suspend的时候，设置闹钟并不会往rtc 芯片的寄存器上写数据，因为不需要唤醒系统，所以闹钟数据时间什么的就通过上层写到设备文件/dev/alarm
里面就可以了，AlarmThread 会不停的去轮寻下一个时间有没有闹钟，直接从设备文件 /dev/alarm 里面读取
第二种，系统要是进入susupend的话，alarm 的alarm_suspend  就会写到下层的rtc芯片的寄存器上去， 然后即使系统suspend之后，闹钟通过rtc 也能唤醒系统。
这里就调用到了interface.c 里面   //这里面 int rtc_set_alarm(struct rtc_device *rtc, struct rtc_wkalrm *alarm) 差不多 也是跟下面一样
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

intrtc_set_time(structrtc_device *rtc,structrtc_time *tm)
{
....
err = rtc->ops->set_time(rtc->dev.parent,tm);
....
}

然后set_time 就看到具体的是那个RTC芯片，这边我们是rtc-hym8563.c
然后就到了
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

staticinthym8563_i2c_set_regs(structi2c_client *client, u8 reg, u8constbuf[], __u16 len)
{
intret;
ret = i2c_master_reg8_send(client, reg, buf, (int)len, RTC_SPEED);
returnret;
}

到此，闹钟时间就已经写到rtc 芯片的寄存器里面，第二个参数就是寄存器的名字，后面的buf就是要写入的时间，rtc芯片是额外供电的，所以系统suspend之后，系统kernel都关了，但是rtc里面还有电，寄存器里面数据还是有的（掉电就会丢失数据），所以闹钟到了，通过硬件中断机制就可以唤醒系统。
3.下面是系统唤醒之后，闹钟怎么工作的流程，简单阐述
**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

privateclassAlarmThread extends Thread
{
publicAlarmThread()
{
super("AlarmManager");
}
publicvoidrun()
{
while(true)
{
intresult = waitForAlarm(mDescriptor);//这里调用jni调用static jint android_server_AlarmManagerService_waitForAlarm，主要还是对 /dev/alarm  操作
....
Alarm alarm = it.next();
try{
if(localLOGV) Slog.v(TAG,"sending alarm "+ alarm);
alarm.operation.send(mContext, 0,
mBackgroundIntent.putExtra(
Intent.EXTRA_ALARM_COUNT, alarm.count),
mResultReceiver, mHandler);
....
}
}
}

staticjint android_server_AlarmManagerService_waitForAlarm(JNIEnv* env, jobject obj, jint fd)
{
\#if HAVE_ANDROID_OS
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
\#endif
}
系统没有suspend的话直接走下面流程，如果suspend的话会被RTC唤醒，然后还是走下面的流程
AlarmManagerService  里面有个AlarmThread  会一直轮询 /dev/alarm文件，如果打开失败就直接返回，成功就会做一些动作，比如查找时间最近的
alarm，比如睡眠被闹钟唤醒的时候，这边就发一个intent出去，然后在AlarmReceiver.java里面弹出里面会收到就会调用下面的
context.startActivity(alarmAlert);
然后弹出alarm  这个界面
Class c = AlarmAlert.class;
其中public class AlarmAlert extends AlarmAlertFullScreen  所以系统睡眠之后被alarm唤醒弹出的alarm就是这边start的

**[cpp]**[view
 plain](http://blog.csdn.net/crycheng/article/details/7804813#)[copy](http://blog.csdn.net/crycheng/article/details/7804813#)

publicclassAlarmReceiver extends BroadcastReceiver {
/** If the alarm is older than STALE_WINDOW, ignore.  It
is probably the result of a time or timezone change */
privatefinalstaticintSTALE_WINDOW = 30 * 60 * 1000;
@Override
publicvoidonReceive(Context context, Intent intent) {
.........
Intent alarmAlert =newIntent(context, c);
alarmAlert.putExtra(Alarms.ALARM_INTENT_EXTRA, alarm);
alarmAlert.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK
| Intent.FLAG_ACTIVITY_NO_USER_ACTION);
context.startActivity(alarmAlert);
........
}

到这里alarm 就显示出来了

