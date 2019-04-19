# android触摸屏坐标手工校准/调整成功 (ZZ) - xqhrs232的专栏 - CSDN博客
2011年10月21日 22:14:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1824
原文地址::[http://hi.baidu.com/hexi1985/blog/item/7d40e6fad3dcd0d0b48f3196.html](http://hi.baidu.com/hexi1985/blog/item/7d40e6fad3dcd0d0b48f3196.html)
1.简述
android 的坐标转换处理：
This implementation is a linear transformation using 7 parameters 
(a, b, c, d, e, f and s) to transform the device coordinates (Xd, Yd) 
into screen coordinates (Xs, Ys) using the following equations:
s*Xs = a*Xd + b*Yd + c
s*Ys = d*Xd + e*Yd + f
Xs,Ys：LCD坐标
Xd,Yd：触摸屏坐标
在编译好了的ANDROID根文件系统的system/etc/pointercal这个文件内，存放着7个数，
这7个数就是对应上面公式的a,b,c,d,e,f,s
比如我原来的：(如果表格没对齐，请拷到记事本里面看)
+----+-----+--------+------+--+--------+----+
| a | b | c     | d    |e |   f    | s |
+----+-----+--------+------+--+--------+----+
|-411|37818|-3636780|-51325|39|47065584|6553|
+----+-----+--------+------+--+--------+----+
2.处理说明：
“system/etc/pointercal”这个文件是被java程序读取使用的，文件目录：
f rameworks/base/services/java/com/android/server/InputDevice.java
---注：我用的是koolu的源码(http://git.koolu.org/)，官方的源码请自行搜索。
该文件的第32行定义了：static final String CALIBRATION_FILE = "/system/etc/pointercal";
这个CALIBRATION_FILE变量在第237行被使用于打开该文件：
FileInputStream is = new FileInputStream(CALIBRATION_FILE);
后面的代码就是从这个文件里读取那7个数据，用于上层函数的坐标转换。
所以只要根据该公式通过手工计算出那7个值，就可以准确的进行触摸操作了。
3.计算
计算前需要取得4个触摸屏的坐标，我们取LCD 4个对角的坐标，因为只有这4个坐标知道确切的LCD坐标。
要取坐标必须打开内核中触摸屏的调试代码，启动后在console上使用dmesg命令来跟踪取得。
我是在一个角上用笔点一下，再用dmesg调出记录，然后记录下来。如下是我的320x240屏记录的结果：
坐标轴里小括号()里面的是调试信息给的触摸屏坐标，中括号[]里的是对应的LCD坐标。
x坐标
/|\
|(X:870, Y:140)   (X:870, Y:890)
| [320,0]          [320,240]
|
|
|
+-------------------------------> y坐标
(X:120, Y:140)    (X:120, Y:890)
     [0,0]             [0,240]
*LCD 分辨率：320 x 240 ，也是LCD坐标的最大值
             x     y
设定s=65536
将那4个坐标代入那个公式，可以得出8个方程组
        0 = a*120 + b*140 + c
        0 = d*120 + e*140 + f
        0 = a*120 + b*890 + c
65536*240 = d*120 + e*890 + f
65536*320 = a*870 + b*140 + c
        0 = d*870 + e*140 + f
65536*320 = a*870 + b*890 + c
65536*240 = d*870 + e*890 + f
解方程组后就可得：
a = 0
b = 20971
c = -2935940
d = 27962
e = 0
f = -3355440
s = 65535
然后用工具打开andriod的“system/etc/pointercal”文件，把这几个数输进去，用空格分开，
注意不要改文件结尾的两个字节0x00 0x0A,我是用Ghex工具打开的，在右边窗口中进行输入，在编辑里可以切换插入和覆盖模式。
此时启动试试。
////////////////////////////
我的屏到这个步骤后触摸操作的左右变成了上下，上下变成了左右操作，
处理方法：把a,b,c值和d,e,f值分别对调。
再启动试试。。
还是不行，左右是正确的，上下是反的。
处理方法：
把Y坐标再反向的公式：
Ys‘ = 320 - Ys = 320 - (a*Xd + b*Yd + c)/s
重新计算a,b,c的值，就OK了。
现在我点击计算器的按钮等操作都很准了，指哪打哪。
前面发的那个视频里的坐标是在内核里调的坐标，不是很准。
这是手工调的方法，如果要用程序校准的话可以用tslib，有时间再玩下。
注：我用的开发板是mini2440，LCD屏是320x240
二，Android 事件处理机制 
    android 事件的传入是从EventHub开始的，EventHub是 事件的抽象结构，维护着系统设备的运行情况（设备文件放在/dev/input里），设备类型包括Keyboard、TouchScreen、TraceBall。它在系统启动的时候会通过 open_device方法将系统提供的输入设备都增加到这个抽象结构中，并维护一个所有输入设备的文件描述符，如果输入设备是键盘的话还会读取 /system/usr/keylayout/目录下对应键盘设备的映射文件（修改./development/emulator/keymaps/qwerty.kl来改变键值的映射关系），另外getEvent方法是对EventHub中的设备文件描述符使用poll操作等侍驱动层事件的发生，如果发生的事件是键盘事件，则调用Map函数按照映射文件转换成相应的键值并将扫描码和键码返回给KeyInputQueue.
frameworks/base/services/jni/com_android_server_KeyInputQueue.cpp 
    根据事件的类型以及事件值进行判断处理，从而确定这个事件对应的设备状态是否发生了改变并相应的改变对这个设备的描述结构InputDevice。 
Windowmanager会创建一个线程（InputDispatcherThread），在这个线程里从事件队列中读取发生的事件 （QueuedEvent ev = mQueue.getEvent（）），并根据读取到事件类型的不同分成三类（KEYBOARD、TOUCHSCREEN、TRACKBALL），分别进 行处理，例如键盘事件会调用dispatchKey((KeyEvent)ev.event, 0, 0)以将事件通过Binder发送给具有焦点的窗口应用程序，然后调用 mQueue.recycleEvent(ev)继续等侍键盘事件的发生;如果是触摸屏事件则调用dispatchPointer(ev,
 (MotionEvent)ev.event, 0, 0)，这里会根据事件的种类（UP、DOWN、MOVE、OUT_SIDE等）进行判断并处理，比如Cancel或将事件发送到具有权限的指定的窗口中 去;
    移植方案 
Android本身并不带触摸屏校准。Android获取到的数据就是驱动上报的原始数据。 
方案一 : 移植TSLIB，通过TSLIB产生 pointercal 校准参数文件。 
方案二 : 从Android框架层获取OnTouch事件产生 pointercal 校准参数文件 
方案一: 数据的校准在驱动中完成。 即把 pointercal 的参数数据通过某种方式(sysfs)传递给驱动程序进行校准。 
方案二: 驱动上报原始点，原始点在框架层拦截后进行校验处理。 
[TSLIB移植过程](http://loovle.javaeye.com/admin/blogs/663454)
修改源码以适应android的文件结构。 
设定Android.mk 编译选项，生成库即应用。 
etc/ts.conf module_raw input 
src/ts_config.c #define TS_CONF "/system/etc/ts.conf" 
src/ts_load_module.c 
char *plugin_directory="/system/lib/ts/plugins/"; 
tests/fbutils.c 
char *defaultfbdevice = "/dev/graphics/fb0"; 
COPY ts.conf 到 /system/etc/ts.conf 
init.rc. mkdir /data/etc/pointercal 
通过 ts_calibrate 产生pointercal 数据文件。 
框架内获取参数文件制作APK 应用，仿效ts_calibrate采点并计算出各参数，产生 pointercal框架内实现触摸屏校准在 InputDevive.java 中 拦截触摸屏原始数据进行pointercal参数校验后再分发驱动内实现触摸屏校准
在init.rc 中添加event，在触摸屏加载后把 pointercal参数输送给驱动。 
    结果-效果 
实现细节: 
    扩展init - proper_serivce 系统支持的属性权限，对自定义的特殊系统属性进行权限开放。 
    使用自定义系统属性在 init.rc 中 on property 事件中处理pointercal的读写权限。 
    使用自定义系统属性 触摸屏校准程序.apk 和 InputDevice.java 中的输入事件的同步。 
（在触摸屏校准期间 inputDevice 在输入事件中不能采用算法。 校准程序完成有inputDevice重新启用校准算法） 
    模拟器中至今无法进入 device.absX/Y != null 的代码， 需要了解以下 inputDevice 被调用的步骤。 
三，触摸屏的时间流程: 
驱动层: 
C代码
/*
* Touchscreen absolute values
*
* These parameters are used to help the input layer discard out of
* range readings and reduce jitter etc.
*
* o min, max:- indicate the min and max values your touch screen returns
* o fuzz:- use a higher number to reduce jitter
*
* The default values correspond to Mainstone II in QVGA mode
*
* Please read
* Documentation/input/input-programming.txt for more details.
*/
static int abs_x[3] = {350, 3900, 5};
module_param_array(abs_x, int, NULL, 0);
MODULE_PARM_DESC(abs_x, "Touchscreen absolute X min, max, fuzz");
static int abs_y[3] = {320, 3750, 40};
module_param_array(abs_y, int, NULL, 0);
MODULE_PARM_DESC(abs_y, "Touchscreen absolute Y min, max, fuzz");
static int abs_p[3] = {0, 150, 4};
module_param_array(abs_p, int, NULL, 0);
MODULE_PARM_DESC(abs_p, "Touchscreen absolute Pressure min, max, fuzz");
/*
* 对设备进行初始化设置
*/
    set_bit(EV_ABS, wm->input_dev->evbit);
    set_bit(ABS_X, wm->input_dev->absbit);
    set_bit(ABS_Y, wm->input_dev->absbit);
    set_bit(ABS_PRESSURE, wm->input_dev->absbit);
    input_set_abs_params(wm->input_dev, ABS_X, abs_x[0], abs_x[1],
                 abs_x[2], 0);
    input_set_abs_params(wm->input_dev, ABS_Y, abs_y[0], abs_y[1],
                 abs_y[2], 0);
    input_set_abs_params(wm->input_dev, ABS_PRESSURE, abs_p[0], abs_p[1],
                 abs_p[2], 0);
/*
* 事件发生时，提供原始点
*/
input_report_abs(wm->input_dev, ABS_X, data.x & 0xfff);
        input_report_abs(wm->input_dev, ABS_Y, data.y & 0xfff);
        input_report_abs(wm->input_dev, ABS_PRESSURE, data.p & 0xfff);
/*
* 提供给驱动外查询input_dev 的接口
* struct input_absinfo info;
* ioctl(fd, EVIOCGABS(axis), &info)
* src file: evDev.c
*/
if ((_IOC_NR(cmd) & ~ABS_MAX) == _IOC_NR(EVIOCGABS(0))) {
                t = _IOC_NR(cmd) & ABS_MAX;
                abs.value = dev->abs[t];
                abs.minimum = dev->absmin[t];
                abs.maximum = dev->absmax[t];
                abs.fuzz = dev->absfuzz[t];
                abs.flat = dev->absflat[t];
Android 底层驱动 
EventHub.cpp
static const char *device_path = "/dev/input";
openPlatformInput(void)
scan_dir(device_path);
    open_device(devname);
      fd = open(deviceName, O_RDWR);
/*
* 对外接口，getEvent， 
* inotify 监控device_path目录， 使用poll机制轮询 inotify 和各个输入设备的可用状态。 解析事件或输入信息，放入各个传出参数中。
*/
bool EventHub::getEvent(int32_t* outDeviceId, int32_t* outType,
        int32_t* outScancode, int32_t* outKeycode, uint32_t *outFlags,
        int32_t* outValue, nsecs_t* outWhen)
JNI 部分： com_android_server_KeyInputQueue.cpp. 提供接口
static JNINativeMethod gInputMethods[] = {
    /* name, signature, funcPtr */
    { "readEvent", "(Landroid/view/RawInputEvent;)Z",
            (void*) android_server_KeyInputQueue_readEvent },
    { "getDeviceClasses", "(I)I",
        (void*) android_server_KeyInputQueue_getDeviceClasses },
    { "getDeviceName", "(I)Ljava/lang/String;",
        (void*) android_server_KeyInputQueue_getDeviceName },
    { "getAbsoluteInfo", "(IILcom/android/server/InputDevice$AbsoluteInfo;)Z",
        (void*) android_server_KeyInputQueue_getAbsoluteInfo },
    { "getSwitchState", "(I)I",
        (void*) android_server_KeyInputQueue_getSwitchState },
    { "getSwitchState", "(II)I",
        (void*) android_server_KeyInputQueue_getSwitchStateDevice },
    { "getScancodeState", "(I)I",
        (void*) android_server_KeyInputQueue_getScancodeState },
    { "getScancodeState", "(II)I",
        (void*) android_server_KeyInputQueue_getScancodeStateDevice },
    { "getKeycodeState", "(I)I",
        (void*) android_server_KeyInputQueue_getKeycodeState },
    { "getKeycodeState", "(II)I",
        (void*) android_server_KeyInputQueue_getKeycodeStateDevice },
    { "hasKeys", "([I[Z)Z",
        (void*) android_server_KeyInputQueue_hasKeys },
};
Cpp代码
EventHub.cpp
static const char *device_path = "/dev/input";
openPlatformInput(void)
scan_dir(device_path);
    open_device(devname);
      fd = open(deviceName, O_RDWR);
/*
* 对外接口，getEvent， 
* inotify 监控device_path目录， 使用poll机制轮询 inotify 和各个输入设备的可用状态。 解析事件或输入信息，放入各个传出参数中。
*/
bool EventHub::getEvent(int32_t* outDeviceId, int32_t* outType,
        int32_t* outScancode, int32_t* outKeycode, uint32_t *outFlags,
        int32_t* outValue, nsecs_t* outWhen)
JNI 部分： com_android_server_KeyInputQueue.cpp. 提供接口
static JNINativeMethod gInputMethods[] = {
    /* name, signature, funcPtr */
    { "readEvent", "(Landroid/view/RawInputEvent;)Z",
            (void*) android_server_KeyInputQueue_readEvent },
    { "getDeviceClasses", "(I)I",
        (void*) android_server_KeyInputQueue_getDeviceClasses },
    { "getDeviceName", "(I)Ljava/lang/String;",
        (void*) android_server_KeyInputQueue_getDeviceName },
    { "getAbsoluteInfo", "(IILcom/android/server/InputDevice$AbsoluteInfo;)Z",
        (void*) android_server_KeyInputQueue_getAbsoluteInfo },
    { "getSwitchState", "(I)I",
        (void*) android_server_KeyInputQueue_getSwitchState },
    { "getSwitchState", "(II)I",
        (void*) android_server_KeyInputQueue_getSwitchStateDevice },
    { "getScancodeState", "(I)I",
        (void*) android_server_KeyInputQueue_getScancodeState },
    { "getScancodeState", "(II)I",
        (void*) android_server_KeyInputQueue_getScancodeStateDevice },
    { "getKeycodeState", "(I)I",
        (void*) android_server_KeyInputQueue_getKeycodeState },
    { "getKeycodeState", "(II)I",
        (void*) android_server_KeyInputQueue_getKeycodeStateDevice },
    { "hasKeys", "([I[Z)Z",
        (void*) android_server_KeyInputQueue_hasKeys },
};
java service 部分： KeyInputQueue.java. 循环查询输入设备信息或目录状态并处理 
Thread mThread = new Thread("InputDeviceReader") {
        public void run() {
            android.os.Process.setThreadPriority(
                    android.os.Process.THREAD_PRIORITY_URGENT_DISPLAY);
            try {
                RawInputEvent ev = new RawInputEvent();
                while (true) {
                    InputDevice di;
                    // block, doesn't release the monitor
                    readEvent(ev);
                    boolean send = false;
                    boolean configChanged = false;
......
//检测到新设备后
if (ev.type == RawInputEvent.EV_DEVICE_ADDED) {
                        synchronized (mFirst) {
                            di = newInputDevice(ev.deviceId);
                            mDevices.put(ev.deviceId, di);
                            configChanged = true;
                        }
                    }
//对触摸屏设备
InputDevice.AbsoluteInfo absX;
        InputDevice.AbsoluteInfo absY;
        InputDevice.AbsoluteInfo absPressure;
        InputDevice.AbsoluteInfo absSize;
        if ((classes&RawInputEvent.CLASS_TOUCHSCREEN) != 0) {
            absX = loadAbsoluteInfo(deviceId, RawInputEvent.ABS_X, "X");
            absY = loadAbsoluteInfo(deviceId, RawInputEvent.ABS_Y, "Y");
            absPressure = loadAbsoluteInfo(deviceId, RawInputEvent.ABS_PRESSURE, "Pressure");
            absSize = loadAbsoluteInfo(deviceId, RawInputEvent.ABS_TOOL_WIDTH, "Size");
        } else {
            absX = null;
            absY = null;
            absPressure = null;
            absSize = null;
        }
        return new InputDevice(deviceId, classes, name, absX, absY, absPressure, absSize);
我们对触摸屏的数据修订是在 InputDevice.java 中基于 absX, absY, absPressure != null 的状态下的，当绝对原始点数据从驱动报上来之后，传递到InputDevice.java 经过我们的校准后再dispatch出去到windowManager -> activity 。 这样就是起到了校准效果。
四，需要注意的补助说明 
EventHub 中有使用IOCTL 对触摸屏的EVIOCGABS(axis)进行了采样，取出内容struct
C代码
input_absinfo info;
struct input_absinfo {
    __s32 value;
    __s32 minimum;
    __s32 maximum;
    __s32 fuzz;
    __s32 flat;
    __s32 resolution;
};
#define EVIOCGABS(abs)        _IOR('E', 0x40 + abs, struct input_absinfo)        /* get abs value/limits */
取出的是触摸屏的最大值，最小值等，这些值在com_android_server_KeyInputQueue.cpp 中会把这些值传递给InputDevice.cpp中使用，即把报上来的位置通过算法进行计算成绝对坐标值
C代码
scaledX = ((scaledX-device.absX.minValue)
                                / device.absX.range) * w;
