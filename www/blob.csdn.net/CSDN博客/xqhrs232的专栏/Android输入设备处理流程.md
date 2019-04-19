# Android输入设备处理流程 - xqhrs232的专栏 - CSDN博客
2011年09月07日 09:56:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：858
原文地址::[http://hi.baidu.com/viataa/blog/item/5a179f8fe349876f9e2fb4af.html](http://hi.baidu.com/viataa/blog/item/5a179f8fe349876f9e2fb4af.html)
Android系统硬件的访问是通过HAL(Hardware Abstract Layer)来进行的，HAL就起到来Hardware adapter的作用。对于输入设备，如KeyPad、TouchPanel等，Android用EventHub具体的对其进行了封装。
输入设备驱动程序对用户空间应用程序提供一些设备文件，这些设备文件放在/dev/input里面。
**1. EventHub扫描/dev/input下所有设备文件，并打开它们。**
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
int EventHub::scan_dir(const char *dirname)
{
    char devname[PATH_MAX];
    char *filename;
    DIR *dir;
    struct dirent *de;
    dir = opendir(dirname);
    if(dir == NULL)
        return -1;
    strcpy(devname, dirname);
    filename = devname + strlen(devname);
    *filename++ = '/';
    while((de = readdir(dir)))
    {
        if(de->d_name[0] == '.' &&
           (de->d_name[1] == '\0' ||
            (de->d_name[1] == '.' && de->d_name[2] == '\0')))
            continue;
        strcpy(filename, de->d_name);
        open_device(devname);
    }
    closedir(dir);
    return 0;
}open_device(devname)
{
    open(...)
    ioctl(...)
}
EventHub对外提供了一个函数用于从输入设备文件中读取数据。
bool EventHub::getEvent(int32_t* outDeviceId, int32_t* outType,
        int32_t* outScancode, int32_t* outKeycode, uint32_t *outFlags,
        int32_t* outValue, nsecs_t* outWhen)
{
 ...
    while(1)
    {
 
        // First, report any devices that had last been added/removed.
        if (mClosingDevices != NULL)
        {
            device_t* device = mClosingDevices;
            LOGV("Reporting device closed: id=0x%x, name=%s\n",
                 device->id, device->path.string());
            mClosingDevices = device->next;
            *outDeviceId = device->id;
            if (*outDeviceId == mFirstKeyboardId) *outDeviceId = 0;
            *outType = DEVICE_REMOVED;
            delete device;
            return true;
        }
        
        if (mOpeningDevices != NULL)
        {
            device_t* device = mOpeningDevices;
            LOGV("Reporting device opened: id=0x%x, name=%s\n",
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
 
        if (pollres <= 0)
        {
            if (errno != EINTR) 
            {
                LOGW("select failed (errno=%d)\n", errno);
                usleep(100000);
            }
            continue;
        }
 
        for(i = 1; i < mFDCount; i++) 
        {
            if(mFDs[i].revents) 
            {
                LOGV("revents for %d = 0x%08x", i, mFDs[i].revents);
                if(mFDs[i].revents & POLLIN)
                {
                    res = read(mFDs[i].fd, &iev, sizeof(iev));
                    if (res == sizeof(iev))
                    {
                        LOGV("%s got: t0=%d, t1=%d, type=%d, code=%d, v=%d",
                             mDevices[i]->path.string(),
                             (int) iev.time.tv_sec, (int) iev.time.tv_usec,
                             iev.type, iev.code, iev.value);
                        *outDeviceId = mDevices[i]->id;
                        if (*outDeviceId == mFirstKeyboardId) *outDeviceId = 0;
                        *outType = iev.type;
                        *outScancode = iev.code;
                        if (iev.type == EV_KEY) 
                        {
                            err = mDevices[i]->layoutMap->map(iev.code, outKeycode, outFlags);
                            LOGV("iev.code=%d outKeycode=%d outFlags=0x%08x err=%d\n",
                                iev.code, *outKeycode, *outFlags, err);
                            if (err != 0)
                            {
                                *outKeycode = 0;
                                *outFlags = 0;
                            }
                        }
                        else
                        {
                            *outKeycode = iev.code;
                        }
                        *outValue = iev.value;
                        *outWhen = s2ns(iev.time.tv_sec) + us2ns(iev.time.tv_usec);
                        return true;
                    }
                    else
                    {
                        if (res<0)
                        {
                            LOGW("could not get event (errno=%d)", errno);
                        }
                        else
                        {
                            LOGE("could not get event (wrong size: %d)", res);
                        }
                        continue;
                    }
                }
            }
        }
 ...
}
对于按键事件，调用mDevices[i]->layoutMap->map进行映射。映射实际是由 KeyLayoutMap::map完成的，KeyLayoutMap类里读取配置文件qwerty.kl，由配置文件qwerty.kl决定键值的映射 关系。你可以通过修改./development/emulator/keymaps/qwerty.kl来改变键值的映射关系。
**2. JNI函数**
在frameworks/base/services/jni/com_android_server_KeyInputQueue.cpp文件 中，向JAVA提供了函数android_server_KeyInputQueue_readEvent，用于读取输入设备事件。
static jboolean
android_server_KeyInputQueue_readEvent(JNIEnv* env,
                                       jobject clazz,
                                       jobject event)
{
    gLock.lock();
    sp<EventHub> hub = gHub;
    if (hub == NULL)
    {
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
**3. 事件中转线程**
在frameworks/base/services/java/com/android/server/KeyInputQueue.java里创建了一个线程，它循环的读取事件，然后把事件放入事件队列里。
Thread mThread = new Thread("InputDeviceReader")
{
    public void run()
    {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_DISPLAY);
        try
        {
            RawInputEvent ev = new RawInputEvent();
            while (true)
            {
                InputDevice di;
                readEvent(ev); 
                send = preprocessEvent(di, ev);
                addLocked(di, curTime, ev.flags, ..., me);
            }
        }
    }
};
注：readEvent就是调用了JNI的android_server_KeyInputQueue_readEven函数
**4. 输入事件分发线程**
在frameworks/base/services/java/com/android/server/WindowManagerService.java里创建了一个输入事件分发线程，它负责把事件分发到相应的窗口上去。
在WindowManagerService构造函数中调用
mInputThread = new InputDispatcherThread();
mInputThread.start();
InputDispatcherThread的实现代码如下：
private final class InputDispatcherThread extends Thread
{
    // Time to wait when there is nothing to do: 9999 seconds.
    static final int LONG_WAIT=9999*1000;
    public InputDispatcherThread()
    {
        super("InputDispatcher");
    }
    @Override
    public void run()
    {
        ***while (true) process();***
    }
    private void process()
    {
        android.os.Process.setThreadPriority(
            android.os.Process.THREAD_PRIORITY_URGENT_DISPLAY);
        ... ...
        while (true)
        {
            long curTime = SystemClock.uptimeMillis();
            // Retrieve next event, waiting only as long as the next
            // repeat timeout.  If the configuration has changed, then
            // don't wait at all -- we'll report the change as soon as
            // we have processed all events.
            QueuedEvent ev = mQueue.getEvent(
                (int)((!configChanged && curTime < nextKeyTime)
                        ? (nextKeyTime-curTime) : 0));
            ... ...
            try
            {
                if (ev != null)
                {
                    switch (ev.classType)
                    {
                    case RawInputEvent.CLASS_KEYBOARD:
                        ...
                        **dispatchKey**((KeyEvent)ev.event, 0, 0);
                        mQueue.recycleEvent(ev);
                        break;
                    case RawInputEvent.CLASS_TOUCHSCREEN:
                        //Slog.i(TAG, "Read next event " + ev);
                        **dispatchPointer**(ev, (MotionEvent)ev.event, 0, 0);
                        break;
                    case RawInputEvent.CLASS_TRACKBALL:
                        ***dispatchTrackball***(ev, (MotionEvent)ev.event, 0, 0);
                        break;
                    case RawInputEvent.CLASS_CONFIGURATION_CHANGED:
                        configChanged = true;
                        break;
                    default:
                        mQueue.recycleEvent(ev);
                    break;
                    }
                } else if (configChanged)
                {
                    configChanged = false;
                    sendNewConfiguration();
                }
                else if (lastKey != null)
                {
                    curTime = SystemClock.uptimeMillis();
                    ... ...
                    ***dispatchKey***(newEvent, 0, 0);
                }
                else
                {
                    curTime = SystemClock.uptimeMillis();
                    lastKeyTime = curTime;
                    nextKeyTime = curTime + LONG_WAIT;
                }
            }
            catch (Exception e)
            {
                ... ...
            }
        }
    }
