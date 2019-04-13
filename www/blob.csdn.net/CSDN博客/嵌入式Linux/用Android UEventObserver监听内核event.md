
# 用Android UEventObserver监听内核event - 嵌入式Linux - CSDN博客

2015年12月31日 10:23:45[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1135


很多时候我们在框架上层需要知道内核中某些事件的状态，用设备的show属性是可以供上层来查询，但是这需要上层设定一个较耗资源的循环。如果用UEventObserver就可以监听内核event，它是android Java层利用uevent与获取Kernel层状态变化的机制。
（1）UEventObserver简介
`framework有如下模块使用UEventObserver的功能来提供服务：`电池状态：services/java/com/android/server/BatteryService.java，
耳机状态：services/java/com/android/server/HeadsetObserver.java，
DOCK状态：services/java/com/android/server/DockObserver.java，
USB状态：services/java/com/android/server/usb/UsbService.java。
它们全部继承自UEventObserver，先看看这个类的构造和原理：
./core/java/android/os/UEventObserver.java
|
[ native_setup(), next_event() ]
|/
./core/jni/android_os_UEventObserver.cpp
|
[ uevent_init(),uevent_next_event() ]
|/
/hardware/libhardware_legacy/uevent/uevent.c
|                                                                                         [userspace]
———————[socket]—————————————————————————–
|
|/                                                                                           [kernel]
socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT)
核心响应函数是：继承UEventObserver的类必须实现自己的public abstract void onUEvent(UEvent event)，在这里面进行获取状态后的处理。
（2）实例，在上层服务中监听摄像头的打开和关闭，并作相应处理。需要内核摄像头驱动中也要发出event事件才行，所以改动分为内核和上层两部分。
`A，内核摄像头驱动中的改动：`[cpp] view plaincopy
static atomic_t g_CamHWOpend;   //camera是否打开成功的标志
struct device* sensor_device = NULL;
static void set_camera_status()
{
char *envp[2];
int ret = atomic_read(&g_CamHWOpend)? 1 : 0;
if(ret)
envp[0] = “STATUS=OPEN”;
else
envp[0] = “STATUS=CLOSE”;
envp[1] = NULL;
kobject_uevent_env(&sensor_device->kobj, KOBJ_CHANGE, envp); //将evnp通过kobject上报
return;
}
在打开摄像头和关闭摄像头的函数中，成功打开和成功关闭的位置都要放置set_camera_status函数。
`B，上层某个服务中的改动：`导入类：
[java] view plaincopy
import android.os.UEventObserver;
在服务的onCreate函数中启动UEventObserver，启动的路径要根据uevent规则来匹配，具体要去目录下寻找：
[java] view plaincopy
m_CameraStatusObserver.startObserving(“DEVPATH=/devices/virtual/sensordrv/kd_camera_hw”);
UEventObserver型变量的创建：
[java] view plaincopy
private UEventObserver m_CameraStatusObserver = new UEventObserver(){
public void onUEvent(UEvent event){                       //完成抽象的核心响应函数
String status = event.get(“STATUS”);    //没有取特定长度字符串，直接取=前面的子串
Log.i(TAG,”camera app operation and mCameraOpenInSS is “+mCameraOpenInSS);
if( “OPEN”.equals(status)){
Log.i(TAG,”camera app open”);
//如果打开摄像头做。。处理
}
else if (“CLOSE”.equals(status)){
Log.i(TAG,”camera app close”);
//如果关闭摄像头做。。处理
}
}
};
在服务的onDestroy函数中加上停止监听：
[java] view plaincopy
m_CameraStatusObserver.stopObserving();
这样就完成了上层对底层设备状态的响应处理。
参考原文：参考原文：[http://blog.csdn.net/darkengine/article/details/7442359](http://blog.csdn.net/darkengine/article/details/7442359)

