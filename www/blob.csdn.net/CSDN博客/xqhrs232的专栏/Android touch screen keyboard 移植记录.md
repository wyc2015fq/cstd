# Android touch screen keyboard 移植记录 - xqhrs232的专栏 - CSDN博客
2011年09月07日 10:24:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：926标签：[android																[keyboard																[thread																[server																[report																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=report&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=keyboard&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/pottichu/article/details/5689479](http://blog.csdn.net/pottichu/article/details/5689479)
仅仅是作为记录：
Andorid 的 touchscreen 事件必须要有  BTN_TOUCH 才可以。
所以初始化的时候加上：
tablet_dev->keybit[BIT_WORD (BTN_TOUCH)] = BIT_MASK (BTN_TOUCH);
报事件的时候加上
input_report_key( input_dev , BTN_TOUCH , 1);    // pressed
input_report_key( input_dev , BTN_TOUCH , 0);    // realse
其他的就跟标准的 linux 没什么区别了。
另外 键值映射 需要修改
/system/usr/keylayout/qwerty.kl
文件
具体的按键代表什么意思：
需要看 
/usr/include/linux/input.h
文件。
另外转一篇文章：
暂时还没精力去看应用层。
Android事件处理
Init-----------zygote---------system-server-------------------windosmanager  ------------------------------------------------------------ UEventObserver
------------------------------------------------------------ InputDeviceRead
-------------------------------------------------------------InputDispatcher
-------------------------------------------------------------DisplayEventThr
-------------------------------------------------------------ActivityManager
EventHub：
而事件的传入是从EventHub开始的，EventHub是事件的抽象结构，维护着系统设备的运行情况，设备类型包括Keyboard、 TouchScreen、TraceBall。它在系统启动的时候会通过open_device方法将系统提供的输入设备都增加到这个抽象结构中，并维护一个所有输入设备的文件描述符，如果输入设备是键盘的话还会读取/system/usr/keylayout/目录下对应键盘设备的映射文件，另外 getEvent方法是对EventHub中的设备文件描述符使用poll操作等侍驱动层事件的发生，如果发生的事件是键盘事件，则调用Map函数按照映射文件转换成相应的键值并将扫描码和键码返回给KeyInputQueue。
KeyLayoutMap主要是读取键盘映射文件并将键盘扫描码和键码进行转换
frameworks/base/core/jni/server/ com_android_server_KeyInputQueue.cpp
EventHub和KeyinputQueue的JNI接口层
KeyinputQueue：
在线程InputDeviceReader中会根据事件的类型以及事件值进行判断处理，从而确定这个事件对应的设备状态是否发生了改变并相应的改变对这个设备的描述结构InputDevice。
getEvent：在给定时间段时看是否有事件发生，如果有的话返回true否则false。
Windowmanager：
(frameworks/base/services/java/com/android/server/windowmanagerservice.java)
进程Windowmanager会创建一个线程（InputDispatcherThread），在这个线程里从事件队列中读取发生的事件（QueuedEvent ev = mQueue.getEvent（）），并根据读取到事件类型的不同分成三类（KEYBOARD、TOUCHSCREEN、TRACKBALL），分别进行处理，例如键盘事件会调用dispatchKey((KeyEvent)ev.event, 0, 0)以将事件通过Binder发送给具有焦点的窗口应用程序，然后调用 mQueue.recycleEvent(ev)继续等侍键盘事件的发生;如果是触摸屏事件则调用dispatchPointer(ev,
 (MotionEvent)ev.event, 0, 0)，这里会根据事件的种类（UP、DOWN、MOVE、OUT_SIDE等）进行判断并处理，比如Cancel或将事件发送到具有权限的指定的窗口中去;
Android 输入事件流程
EventHub
EventHub对输入设备进行了封装。输入设备驱动程序对用户空间应用程序提供一些设备文件，这些设备文件放在/dev/input里面。
EventHub扫描/dev/input下所有设备文件，并打开它们。
C代码
   1. bool EventHub::openPlatformInput(void)  
   2. {  
   3. ...  
   4.     mFDCount = 1;  
   5.     mFDs = (pollfd *)calloc(1, sizeof(mFDs[0]));  
   6.     mDevices = (device_t **)calloc(1, sizeof(mDevices[0]));  
   7.     mFDs[0].events = POLLIN;  
   8.     mDevices[0] = NULL;  
   9.   
  10.     res = scan_dir(device_path);  
  11. ...  
  12.     return true;  
  13. }  
bool EventHub::openPlatformInput(void)
{
...
    mFDCount = 1;
    mFDs = (pollfd *)calloc(1, sizeof(mFDs[0]));
    mDevices = (device_t **)calloc(1, sizeof(mDevices[0]));
    mFDs[0].events = POLLIN;
    mDevices[0] = NULL;
    res = scan_dir(device_path);
...
    return true;
}
EventHub对外提供了一个函数用于从输入设备文件中读取数据。
C代码
   1. bool EventHub::getEvent(int32_t* outDeviceId, int32_t* outType,  
   2.         int32_t* outScancode, int32_t* outKeycode, uint32_t *outFlags,  
   3.         int32_t* outValue, nsecs_t* outWhen)  
   4. {  
   5.     ...  
   6.     while(1) {  
   7.   
   8.         // First, report any devices that had last been added/removed.  
   9.         if (mClosingDevices != NULL) {  
  10.             device_t* device = mClosingDevices;  
  11.             LOGV("Reporting device closed: id=0x%x, name=%s/n",  
  12.                  device->id, device->path.string());  
  13.             mClosingDevices = device->next;  
  14.             *outDeviceId = device->id;  
  15.             if (*outDeviceId == mFirstKeyboardId) *outDeviceId = 0;  
  16.             *outType = DEVICE_REMOVED;  
  17.             delete device;  
  18.             return true;  
  19.         }  
  20.         if (mOpeningDevices != NULL) {  
  21.             device_t* device = mOpeningDevices;  
  22.             LOGV("Reporting device opened: id=0x%x, name=%s/n",  
  23.                  device->id, device->path.string());  
  24.             mOpeningDevices = device->next;  
  25.             *outDeviceId = device->id;  
  26.             if (*outDeviceId == mFirstKeyboardId) *outDeviceId = 0;  
  27.             *outType = DEVICE_ADDED;  
  28.             return true;  
  29.         }  
  30.   
  31.         release_wake_lock(WAKE_LOCK_ID);  
  32.   
  33.         pollres = poll(mFDs, mFDCount, -1);  
  34.   
  35.         acquire_wake_lock(PARTIAL_WAKE_LOCK, WAKE_LOCK_ID);  
  36.   
  37.         if (pollres <= 0) {  
  38.             if (errno != EINTR) {  
  39.                 LOGW("select failed (errno=%d)/n", errno);  
  40.                 usleep(100000);  
  41.             }  
  42.             continue;  
  43.         }  
  44.   
  45.         for(i = 1; i < mFDCount; i++) {  
  46.             if(mFDs[i].revents) {  
  47.                 LOGV("revents for %d = 0x%08x", i, mFDs[i].revents);  
  48.                 if(mFDs[i].revents & POLLIN) {  
  49.                     res = read(mFDs[i].fd, &iev, sizeof(iev));  
  50.                     if (res == sizeof(iev)) {  
  51.                         LOGV("%s got: t0=%d, t1=%d, type=%d, code=%d, v=%d", 
  52.                              mDevices[i]->path.string(),  
  53.                              (int) iev.time.tv_sec, (int) iev.time.tv_usec, 
  54.                              iev.type, iev.code, iev.value);  
  55.                         *outDeviceId = mDevices[i]->id;  
  56.                         if (*outDeviceId == mFirstKeyboardId) *outDeviceId = 0; 
  57.                         *outType = iev.type;  
  58.                         *outScancode = iev.code;  
  59.                         if (iev.type == EV_KEY) {  
  60.                             err = mDevices[i]->layoutMap->map(iev.code, outKeycode, outFlags); 
  61.                             LOGV("iev.code=%d outKeycode=%d outFlags=0x%08x err=%d/n", 
  62.                                 iev.code, *outKeycode, *outFlags, err);  
  63.                             if (err != 0) {  
  64.                                 *outKeycode = 0;  
  65.                                 *outFlags = 0;  
  66.                             }  
  67.                         } else {  
  68.                             *outKeycode = iev.code;  
  69.                         }  
  70.                         *outValue = iev.value;  
  71.                         *outWhen = s2ns(iev.time.tv_sec) + us2ns(iev.time.tv_usec); 
  72.                         return true;  
  73.                     } else {  
  74.                         if (res<0) {  
  75.                             LOGW("could not get event (errno=%d)", errno); 
  76.                         } else {  
  77.                             LOGE("could not get event (wrong size: %d)", res); 
  78.                         }  
  79.                         continue;  
  80.                     }  
  81.                 }  
  82.             }  
  83.         }  
  84.     ...  
  85. }  
bool EventHub::getEvent(int32_t* outDeviceId, int32_t* outType,
        int32_t* outScancode, int32_t* outKeycode, uint32_t *outFlags,
        int32_t* outValue, nsecs_t* outWhen)
{
    ...
    while(1) {
        // First, report any devices that had last been added/removed.
        if (mClosingDevices != NULL) {
            device_t* device = mClosingDevices;
            LOGV("Reporting device closed: id=0x%x, name=%s/n",
                 device->id, device->path.string());
            mClosingDevices = device->next;
            *outDeviceId = device->id;
            if (*outDeviceId == mFirstKeyboardId) *outDeviceId = 0;
            *outType = DEVICE_REMOVED;
            delete device;
            return true;
        }
        if (mOpeningDevices != NULL) {
            device_t* device = mOpeningDevices;
            LOGV("Reporting device opened: id=0x%x, name=%s/n",
                 device->id, device->path.string());
            mOpeningDevices = device->next;
            *outDeviceId = device->id;
            if (*outDeviceId == mFirstKeyboardId) *outDeviceId = 0;
            *outType = DEVICE_ADDED;
            return true;
        }
        release_wake_lock(WAKE_LOCK_ID);
        pollres = poll(mFDs, mFDCount, -1);
        acquire_wake_lock(PARTIAL_WAKE_LOCK, WAKE_LOCK_ID);
        if (pollres <= 0) {
            if (errno != EINTR) {
                LOGW("select failed (errno=%d)/n", errno);
                usleep(100000);
            }
            continue;
        }
        for(i = 1; i < mFDCount; i++) {
            if(mFDs[i].revents) {
                LOGV("revents for %d = 0x%08x", i, mFDs[i].revents);
                if(mFDs[i].revents & POLLIN) {
                    res = read(mFDs[i].fd, &iev, sizeof(iev));
                    if (res == sizeof(iev)) {
                        LOGV("%s got: t0=%d, t1=%d, type=%d, code=%d, v=%d",
                             mDevices[i]->path.string(),
                             (int) iev.time.tv_sec, (int) iev.time.tv_usec,
                             iev.type, iev.code, iev.value);
                        *outDeviceId = mDevices[i]->id;
                        if (*outDeviceId == mFirstKeyboardId) *outDeviceId = 0;
                        *outType = iev.type;
                        *outScancode = iev.code;
                        if (iev.type == EV_KEY) {
                            err = mDevices[i]->layoutMap->map(iev.code, outKeycode, outFlags);
                            LOGV("iev.code=%d outKeycode=%d outFlags=0x%08x err=%d/n",
                                iev.code, *outKeycode, *outFlags, err);
                            if (err != 0) {
                                *outKeycode = 0;
                                *outFlags = 0;
                            }
                        } else {
                            *outKeycode = iev.code;
                        }
                        *outValue = iev.value;
                        *outWhen = s2ns(iev.time.tv_sec) + us2ns(iev.time.tv_usec);
                        return true;
                    } else {
                        if (res<0) {
                            LOGW("could not get event (errno=%d)", errno);
                        } else {
                            LOGE("could not get event (wrong size: %d)", res);
                        }
                        continue;
                    }
                }
            }
        }
    ...
}
对于按键事件，调用mDevices[i]->layoutMap->map进行映射。映射实际是由 KeyLayoutMap::map完成的，KeyLayoutMap类里读取配置文件qwerty.kl，由配置 文件 qwerty.kl 决定键值的映射关系。你可以通过修 改./development/emulator/keymaps/qwerty.kl来改变键值的映射关系。
JNI 函数
在frameworks/base/services/jni/com_android_server_KeyInputQueue.cpp文件中，向 JAVA提供了函数android_server_KeyInputQueue_readEvent，用于读 取输入设备事件。
C代码
   1. static jboolean  
   2. android_server_KeyInputQueue_readEvent(JNIEnv* env, jobject clazz,  
   3.                                           jobject event)  
   4. {  
   5.     gLock.lock();  
   6.     sp hub = gHub;  
   7.     if (hub == NULL) {  
   8.         hub = new EventHub;  
   9.         gHub = hub;  
  10.     }  
  11.     gLock.unlock();  
  12.   
  13.     int32_t deviceId;  
  14.     int32_t type;  
  15.     int32_t scancode, keycode;  
  16.     uint32_t flags;  
  17.     int32_t value;  
  18.     nsecs_t when;  
  19.     bool res = hub->getEvent(&deviceId, &type, &scancode, &keycode,  
  20.             &flags, &value, &when);  
  21.   
  22.     env->SetIntField(event, gInputOffsets.mDeviceId, (jint)deviceId);  
  23.     env->SetIntField(event, gInputOffsets.mType, (jint)type);  
  24.     env->SetIntField(event, gInputOffsets.mScancode, (jint)scancode);  
  25.     env->SetIntField(event, gInputOffsets.mKeycode, (jint)keycode);  
  26.     env->SetIntField(event, gInputOffsets.mFlags, (jint)flags);  
  27.     env->SetIntField(event, gInputOffsets.mValue, value);  
  28.     env->SetLongField(event, gInputOffsets.mWhen,  
  29.                         (jlong)(nanoseconds_to_milliseconds(when)));  
  30.   
  31.     return res;  
  32. }  
static jboolean
android_server_KeyInputQueue_readEvent(JNIEnv* env, jobject clazz,
                                          jobject event)
{
    gLock.lock();
    sp hub = gHub;
    if (hub == NULL) {
        hub = new EventHub;
        gHub = hub;
    }
    gLock.unlock();
    int32_t deviceId;
    int32_t type;
    int32_t scancode, keycode;
    uint32_t flags;
    int32_t value;
    nsecs_t when;
    bool res = hub->getEvent(&deviceId, &type, &scancode, &keycode,
            &flags, &value, &when);
    env->SetIntField(event, gInputOffsets.mDeviceId, (jint)deviceId);
    env->SetIntField(event, gInputOffsets.mType, (jint)type);
    env->SetIntField(event, gInputOffsets.mScancode, (jint)scancode);
    env->SetIntField(event, gInputOffsets.mKeycode, (jint)keycode);
    env->SetIntField(event, gInputOffsets.mFlags, (jint)flags);
    env->SetIntField(event, gInputOffsets.mValue, value);
    env->SetLongField(event, gInputOffsets.mWhen,
                        (jlong)(nanoseconds_to_milliseconds(when)));
    return res;
}
readEvent调用hub->getEvent读了取事件，然后转换成JAVA的结构。
事件中转线程
在frameworks/base/services/java/com/android/server/KeyInputQueue.java 里创建了一个线程，它循环的读取事件，然后把事件放入事件队列里。
Java代码
   1. Thread mThread = new Thread("InputDeviceReader") {  
   2.         public void run() {  
   3.             android.os.Process.setThreadPriority(  
   4.                     android.os.Process.THREAD_PRIORITY_URGENT_DISPLAY);  
    5.   
   6.             try {  
   7.                 RawInputEvent ev = new RawInputEvent();  
   8.                 while (true) {  
   9.                     InputDevice di;  
   10.   
  11.                     readEvent(ev);  
   12.   
  13.                     send = preprocessEvent(di, ev);  
  14.                     addLocked(di, curTime, ev.flags, ..., me);  
   15.                 }  
   16.         }  
   17.     };  
Thread mThread = new Thread("InputDeviceReader") {
        public void run() {
            android.os.Process.setThreadPriority(
                    android.os.Process.THREAD_PRIORITY_URGENT_DISPLAY);
            try {
                RawInputEvent ev = new RawInputEvent();
                while (true) {
                    InputDevice di;
                    readEvent(ev);
                    send = preprocessEvent(di, ev);
                    addLocked(di, curTime, ev.flags, ..., me);
                }
        }
    };
输入事件分发线程
在frameworks/base/services/java/com/android/server/WindowManagerService.java里创建了一个输入事件分发线程，它负责把事件分发到相应的窗口上去。
Java代码
   1. mQueue.getEvent  
   2. dispatchKey/dispatchPointer/dispatchTrackball  
mQueue.getEvent
dispatchKey/dispatchPointer/dispatchTrackball
按键,触摸屏流程分析
按键触摸屏流程分析:
WindowManagerService类的构造函数
WindowManagerService()
  mQueue = new KeyQ();
因为 WindowManagerService.java (frameworks/base/services/java/com/android/server)中有：  
private class KeyQ extends KeyInputQueue
KeyQ 是抽象类  KeyInputQueue 的实现，所以 new KeyQ类的时候实际上在 KeyInputQueue 类中创建了
一个线程  InputDeviceReader 专门用来冲设备读取按键事件，代码：
Thread mThread = new Thread("InputDeviceReader") {
  public void run()
  {
        在循环中调用：readEvent(ev);
    ...
    send = preprocessEvent(di, ev);
        实际调用的是 KeyQ 类的 preprocessEvent 函数
    ...
    int keycode = rotateKeyCodeLocked(ev.keycode);
      int[] map = mKeyRotationMap;
      for (int i=0; i<N; i+=2)
      {
        if (map[i] == keyCode)
          return map[i+1];
      } //
    addLocked(di, curTime, ev.flags,RawInputEvent.CLASS_KEYBOARD,newKeyEvent(di, di.mDownTime, curTime, down,keycode, 0, scancode,...));
      QueuedEvent ev = obtainLocked(device, when, flags, classType, event);
  }
}
readEvent() 实际上调用的是 com_android_server_KeyInputQueue.cpp (frameworks/base/services/jni)中的：
static jboolean android_server_KeyInputQueue_readEvent(JNIEnv* env, jobject clazz,jobject event)
  bool res = hub->getEvent(&deviceId, &type, &scancode, &keycode,&flags, &value, &when);
调用的是 EventHub.cpp (frameworks/base/libs/ui)中的：
bool EventHub::getEvent(int32_t* outDeviceId, int32_t* outType,
        int32_t* outScancode, int32_t* outKeycode, uint32_t *outFlags,
        int32_t* outValue, nsecs_t* outWhen)
在函数中调用了读设备操作：res = read(mFDs[i].fd, &iev, sizeof(iev));
在构造函数 WindowManagerService()调用 new KeyQ() 以后接着调用了：
  mInputThread = new InputDispatcherThread();      
  ...    
  mInputThread.start();
来启动一个线程  InputDispatcherThread
run()
  process();
    QueuedEvent ev = mQueue.getEvent(...)
因为WindowManagerService类中： final KeyQ mQueue;
所以实际上 InputDispatcherThread 线程实际上从  KeyQ 的事件队列中读取按键事件。
switch (ev.classType)
  case RawInputEvent.CLASS_KEYBOARD:
    ...
    dispatchKey((KeyEvent)ev.event, 0, 0);
    mQueue.recycleEvent(ev);
    break;
  case RawInputEvent.CLASS_TOUCHSCREEN:
    //Log.i(TAG, "Read next event " + ev);
    dispatchPointer(ev, (MotionEvent)ev.event, 0, 0);
    break;
===============================================================
KeyInputQueue.java (frameworks/base/services/java/com/android/server):
的线程  Thread mThread = new Thread("InputDeviceReader") 本地调用：
readEvent(ev);读取按键。readEvent 调用的是文件：
com_android_server_KeyInputQueue.cpp (frameworks/base/services/jni)中的函数：
static jboolean android_server_KeyInputQueue_readEvent(JNIEnv* env, jobject clazz,
                                          jobject event)
android_server_KeyInputQueue_readEvent中有：
hub = new EventHub;
bool res = hub->getEvent(&deviceId, &type, &scancode, &keycode,
            &flags, &value, &when);
hub->getEvent 调用的是
EventHub.cpp (frameworks/base/libs/ui) 文件中的函数：
bool EventHub::getEvent(int32_t* outDeviceId, int32_t* outType,
        int32_t* outScancode, int32_t* outKeycode, uint32_t *outFlags,
        int32_t* outValue, nsecs_t* outWhen)
读取按键。
class RefBase::weakref_impl : public RefBase::weakref_type
在系统启动后，android 会通过
static const char *device_path = "/dev/input";
bool EventHub::openPlatformInput(void)
  res = scan_dir(device_path);
通过下面的函数打开设备。
int EventHub::open_device(const char *deviceName)
{
  ...
  fd = open(deviceName, O_RDWR);
  ...
  mFDs[mFDCount].fd = fd;
  mFDs[mFDCount].events = POLLIN;
  ...
  ioctl(mFDs[mFDCount].fd, EVIOCGNAME(sizeof(devname)-1), devname);
  ...
  const char* root = getenv("ANDROID_ROOT");
  snprintf(keylayoutFilename, sizeof(keylayoutFilename),
                 "%s/usr/keylayout/%s.kl", root, tmpfn);
  ...
  device->layoutMap->load(keylayoutFilename);
  ...
}
打开设备的时候，如果 device->classes&CLASS_KEYBOARD 不等于 0 表明是键盘。
常用输入设备的定义有：
enum {
        CLASS_KEYBOARD      = 0x00000001, //键盘
        CLASS_ALPHAKEY      = 0x00000002, //
        CLASS_TOUCHSCREEN   = 0x00000004, //触摸屏
        CLASS_TRACKBALL     = 0x00000008  //轨迹球
    };
打开键盘设备的时候通过上面的  ioctl 获得设备名称，命令字 EVIOCGNAME 的定义在文件：
kernel/include/linux/input.h 中。
#define EVIOCGNAME(len)   _IOC(_IOC_READ, 'E', 0x06, len) /* get device name */
在内核键盘驱动文件 drivers/input/keyboard/pxa27x_keypad.c 中定义了设备名称：pxa27x-keypad
static struct platform_driver pxa27x_keypad_driver = {
    .probe        = pxa27x_keypad_probe,
    .remove        = __devexit_p(pxa27x_keypad_remove),
    .suspend    = pxa27x_keypad_suspend,
    .resume        = pxa27x_keypad_resume,
    .driver        = {
        .name    = "pxa27x-keypad",
        .owner    = THIS_MODULE,
    },
};
ANDROID_ROOT 为环境变量，在android的命令模式下通过 printenv 可以知道它为： system
所以 keylayoutFilename 为：/system/usr/keylayout/pxa27x-keypad.kl
pxa27x-keypad.kl 定义了按键映射，具体内容如下：
－－－－－－－－－－－－－－－－－－－－－－
# NUMERIC KEYS 3x4
key 2   1
key 3   2
key 4   3
key 5   4
key 6   5
key 7   6
key 8   7
key 9   8
key 10  9
key 11  0
key 83  POUND
key 55  STAR
# FUNCTIONAL KEYS
key 231  MENU        WAKE_DROPPED
key 192  BACK           WAKE_DROPPED
key 193  HOME       WAKE
key 107  DEL        WAKE
key 102  CALL        WAKE_DROPPED
key 158  ENDCALL     WAKE_DROPPED
key 28   DPAD_CENTER     WAKE
key 115  VOLUME_UP
key 114  VOLUME_DOWN
－－－－－－－－－－－－－－－－－－－－－－
如果没有定义键盘映射文件，那么默认使用系统的 /system/usr/keylayout/qwerty.kl
可以修改 /system/usr/keylayout/qwerty.kl 文件改变Android公司的按键映射。
device->layoutMap->load(keylayoutFilename) 调用的是文件：
KeyLayoutMap.cpp (frameworks/base/libs/ui)中的函数：
status_t KeyLayoutMap::load(const char* filename)通过解析 pxa27x-keypad.kl
把按键的映射关系保存在 ：KeyedVector<int32_t,Key> m_keys; 中。
当获得按键事件以后调用：
status_t KeyLayoutMap::map(int32_t scancode, int32_t *keycode, uint32_t *flags)
由映射关系 KeyedVector<int32_t,Key> m_keys 把扫描码转换成andorid上层可以识别的按键。
