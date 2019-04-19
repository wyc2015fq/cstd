# Android GPS架构分析之二 - xqhrs232的专栏 - CSDN博客
2011年12月01日 22:49:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1123
原文地址::[http://www.51x86.com/chongzi865458/blog/11-10/232233_27807.html](http://www.51x86.com/chongzi865458/blog/11-10/232233_27807.html)
介绍完了主体代码结构以及重要的数据结构后，下面来看看gps的定位服务（LocationManager）的启动过程。我总是喜欢追本溯源地从源头去认识事物。因为“人之初，性本善”，从事物的本性去认识事物。
LocationManager 这项服务是在SystemServer.java 中启动的，也就是系统启动之后，这个服务就已经启动了：
systemServer.java [framework\base\services\java\com\android\server]
在SystemServer.java的init2函数中启动了一个线程来注册Android的诸多服务，如：Bluetooth
 Service，NetworkManagement Service，Notification Manager等，当然也包括Location
 Service。
SystemServer.java [frameworks\base\services\java\com\android\server]
```
public static final void init2() {
        Slog.i(TAG, "Entered the Android system server!");
        Thread thr = new ServerThread();
        thr.setName("android.server.ServerThread");
        thr.start();
}
```
在ServerThread线程的run函数中LocationManager服务的代码段如下：
```
2.1版本
try {
                Log.i(TAG, "Location
 Manager");
                ServiceManager.addService(Context.LOCATION_SERVICE, new LocationManagerService(context));
            } catch (Throwable e) {
                Log.e(TAG, "Failure
 starting Location Manager", e);
            }
2.2的代码中代码段如下形式：
       try {
                Slog.i(TAG, "Location Manager");
                location = new LocationManagerService(context);
                ServiceManager.addService(Context.LOCATION_SERVICE, location);
            } catch (Throwable e) {
                Slog.e(TAG, "Failure starting Location
 Manager
```
在run函数的后半部分，是服务对系统的反馈，就是systemReady()函数。LocationManager服务的反馈函数如下：
`if (locationF != null) locationF.systemReady();`
其中的locationF 是LocationManagerService的final类型，就是一旦赋值，不能更改。
`final LocationManagerService locationF = location;`
哇！locationManager这项服务的反馈机制只在2.2的代码里面才有啊。2.1中的反馈机制中并没有locationManager（当然有其他的服务反馈）。
而在2.1版本中LocationManagerService的构造函数如下：
LocationManagerService.java [frameworks\base\services\java\com\android\server]
```
public LocationManagerService(Context context) {
        super();
        mContext = context;
        Thread thread = new Thread(null, this, "LocationManagerService");
        thread.start();
        if (LOCAL_LOGV) {
            Log.v(TAG, "Constructed
 LocationManager Service");
        }
    }
```
2.2版本
```
public LocationManagerService(Context context) {
        super();
mContext = context;
        if (LOCAL_LOGV) {
            Slog.v(TAG, "Constructed LocationManager
 Service");
        }
    }
```
2.1是在构造函数的时候就启动一个自身服务线程。见构造函数。
2.2是在反馈机制中通过systemReady函数启动自身服务线程。如下：
```
void systemReady() {
        // we defer starting up the service until the system is ready 
        Thread thread = new Thread(null, this, "LocationManagerService");
        thread.start();
    }
```
通过线程run函数，调用initialize函数:
```
public void run()
    {
        Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
        Looper.prepare();
        mLocationHandler = new LocationWorkerHandler();
        initialize();
        Looper.loop();
}
```
```
```
initialize函数
LocationManagerService.java[frameworks\base\services\java\com\android\server]
```
private void initialize() {
        // Create a wake lock, needs to be done before calling loadProviders() below
        PowerManager powerManager = (PowerManager) mContext.getSystemService(Context.POWER_SERVICE);
        mWakeLock = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, WAKELOCK_KEY);
        // Load providers
        loadProviders();
```
```
...
```
initialize函数中最重要的就是loadProviders函数了，该函数调用loadProvidersLocked，然后loadProvidersLocked函数又调用_loadProvidersLocked函数。为什么要这么折腾呢?
先来看一部分的_loadProvidersLocked函数：
```
private void _loadProvidersLocked() {
        // Attempt to load "real" providers first
        if (GpsLocationProvider.isSupported()) {
            // Create a gps location provider
            GpsLocationProvider gpsProvider = new GpsLocationProvider(mContext, this);
            mGpsStatusProvider = gpsProvider.getGpsStatusProvider();
            mNetInitiatedListener = gpsProvider.getNetInitiatedListener();
            addProvider(gpsProvider);
            mGpsLocationProvider = gpsProvider;
        }
```
`        ...`
注意这个if语句，狠重要，因为在这个语句中得到了HAL层的GPS接口GpsInterface。就是通过调用GpsLocationProvider的isSupported()函数才调用到gps.cpp[hardware/libhardware_legacy/gps]中的gps_get_interface()。这个isSupported函数才是第一个吃螃蟹的人。（而不是JNI层的init函数，这个下面会提到)。
GpsLocationProvider.cpp [frameworks\base\location\java\com\android\internal\location]
```
public static boolean isSupported() {
        return native_is_supported();
    }
```
然而isSupported只有一句话，果然是高手，一击必中。然后就调用native方法，也就是JNI层定义的方法。native_is_supported函数对于JNI层是android_location_GpsLocationProvider_is_supported方法。
android_location_GpsLocationProvider.cpp [frameworks\base\core\jni]
```
static jboolean android_location_GpsLocationProvider_is_supported(JNIEnv* env, jclass
 clazz) {
    if (!sGpsInterface)
        sGpsInterface = gps_get_interface();
    return (sGpsInterface != NULL);
}
```
前面已经提到JNI起到承上启下的作用，gps_get_interface函数属于HAL层的调用，在文件gps.cpp中。
gps.cpp [hardware\libhardware_legacy\gps]
```
const GpsInterface*
gps_get_interface()
{
    if (sGpsInterface == NULL)
         gps_find_hardware();
    return sGpsInterface;
}
```
然后通过gps_find_hardware函数去得到gps接口，下面只模拟器中的gpsinterface。
```
static void
gps_find_hardware( void )
{
#ifdef HAVE_QEMU_GPS_HARDWARE
    if (qemu_check()) {
        sGpsInterface = gps_get_qemu_interface();
        if (sGpsInterface) {
            LOGD("using QEMU GPS Hardware emulation\n");
            return;
        }
    }
#endif
#ifdef HAVE_GPS_HARDWARE
    sGpsInterface = gps_get_hardware_interface();
#endif
    if (!sGpsInterface)
        LOGD("no GPS hardware on this device\n");
}
```
gps_qemu.c [hardware\libhardware_legacy\gps]
```
const GpsInterface* gps_get_qemu_interface()
{
    return &qemuGpsInterface;
}
```
qemuGpsInterface的整体实现就在文件gps_qemu.c中。
```
static const GpsInterface
 qemuGpsInterface = {
    qemu_gps_init,
    qemu_gps_start,
    qemu_gps_stop,
    qemu_gps_cleanup,
    qemu_gps_inject_time,
    qemu_gps_inject_location,
    qemu_gps_delete_aiding_data,
    qemu_gps_set_position_mode,
    qemu_gps_get_extension,
};
```
```
```
--------------------------------------------------------------------------------
在底层得到gps的接口之后， if
 (GpsLocationProvider.isSupported())（在文件LocationManagerService.java中调用）语句得到true，然后进行下一步操作，在这里new了一个GpsLocationProvider对象。代码如下：
`GpsLocationProvider gpsProvider = new GpsLocationProvider(mContext, this);`
注意GpsLocationProvider构造函数里面的两个参数：mContext,
 this。下面来看看GpsLocationProvider的构造函数的前面几句：
```
public GpsLocationProvider(Context
 context, ILocationManager locationManager) {
        mContext = context;
        mLocationManager = locationManager;
        mNIHandler = new GpsNetInitiatedHandler(context, this);
```
`       ...`
`}`
    在GpsLocationProvider类里面的成员变量mLocationManager是构造函数的第二个参数，就是说是LocationManagerService对象。这一点在这里先明确。
接着看_loadProvidersLocked函数。
```
private void _loadProvidersLocked() {
        // Attempt to load "real" providers first
        if (GpsLocationProvider.isSupported()) {
            // Create a gps location provider
            GpsLocationProvider gpsProvider = new GpsLocationProvider(mContext, this);
            mGpsStatusProvider = gpsProvider.getGpsStatusProvider();
            mNetInitiatedListener = gpsProvider.getNetInitiatedListener();
            addProvider(gpsProvider);
            mGpsLocationProvider = gpsProvider;
        }
        // create a passive location provider, which is always enabled
        PassiveProvider passiveProvider = new PassiveProvider(this);
        addProvider(passiveProvider);
        mEnabledProviders.add(passiveProvider.getName());
        // initialize external network location and geocoder services
        Resources resources = mContext.getResources();
        String serviceName = resources.getString(
                com.android.internal.R.string.config_networkLocationProvider);
        if (serviceName != null) {
            mNetworkLocationProvider =
                new LocationProviderProxy(mContext, LocationManager.NETWORK_PROVIDER,
                        serviceName, mLocationHandler);
            addProvider(mNetworkLocationProvider);
        }
        serviceName = resources.getString(com.android.internal.R.string.config_geocodeProvider);
        if (serviceName != null) {
            mGeocodeProvider = new GeocoderProxy(mContext, serviceName);
        }
        updateProvidersLocked();
    }
```
在构造完GpsLocationProvider之后将其add到全局变量ArrayList mProviders中，备以后调用。
在2.2中采取了一种PassiveProvider的类，而在2.1中是通过LocationProviderProxy代理类的方式。2.1中LocationProviderProxy作为GpsLocationProvider的代理作用在LocationManagerService中，而2.2中的PassiveProvider感觉这个类是个空壳。。。。。。。。有待研究。
然后启动了nerwork
 location和geocoder 两个service。但是可惜的是这两个服务都无法启动，因为他们是通过配置文件conifg.xml[framework\base\core\res\res\values]得到服务的名字，然后启动服务的。但是在这个配置文件中，两个服务的名字都是null。
conifg.xml [framework\base\core\res\res\values]
```
<</span>!-- Component name of the
 service providing network location support. -->
    <</span>string name="config_networkLocationProvider">@null<</span>/string>
    <</span>!-- Component name of the
 service providing geocoder API support. -->
    <</span>string name="config_geocodeProvider">@null<</span>/string>
```
其实这也导致了，在调用GetFromLocationName和GetFromLocation两个函数时提示“Service not Available”，这个google Android 2.2的bug。
_loadProvidersLocked函数的最后一句是调用updateProvidersLocked函数，仍然在LocationManagerServic.java文件中。
LocationManagerServic.java
```
private void updateProvidersLocked() {
        for (int i = mProviders.size() - 1; i >= 0; i--) {
            LocationProviderInterface p = mProviders.get(i);
            boolean isEnabled = p.isEnabled();
            String name = p.getName();
            boolean shouldBeEnabled = isAllowedBySettingsLocked(name);
            if (isEnabled && !shouldBeEnabled) {
                updateProviderListenersLocked(name, false);
            } else if (!isEnabled && shouldBeEnabled) {
                updateProviderListenersLocked(name, true);
            }
        }
    }
```
   从上面_loadProvidersLocked函数的代码来看，在mProviders这个ArrayList中有两个元素（这一点未求证），一个是gpsProvider，另一个是passiveProvider。gpsProvider是GpsLocationProvider类型的，它的isEnabled函数返回的是false，因为它并没有被enable。而passiveProvider是PassiveProvider类型，它总是enable的。所以gpsProvider会调用else语句中的updateProviderListenersLocked(name, true)函数。我们主要分析这个else语句，对于passiveProvider不做分析。
--------------------------------------------------------------------------------
```
private void updateProviderListenersLocked(String provider, boolean enabled) {
        int listeners = 0;
        LocationProviderInterface p = mProvidersByName.get(provider);
        if (p == null) {
            return;
        }
        ArrayList<</span>Receiver> deadReceivers = null;  
        ArrayList<</span>UpdateRecord> records = mRecordsByProvider.get(provider);
        if (records != null) {
            final int N = records.size();
            for (int i=0; i<</span>N; i++) {
                UpdateRecord record = records.get(i);
                // Sends a notification message to the receiver
                if (!record.mReceiver.callProviderEnabledLocked(provider, enabled)) {
                    if (deadReceivers == null) {
                        deadReceivers = new ArrayList<</span>Receiver>();
                    }
                    deadReceivers.add(record.mReceiver);
                }
                listeners++;
            }
        }
        if (deadReceivers != null) {
            for (int i=deadReceivers.size()-1; i>=0; i--) {
                removeUpdatesLocked(deadReceivers.get(i));
            }
        }       
        if (enabled) {
 //enabled 的值是true
            p.enable();
            if (listeners > 0) {
                p.setMinTime(getMinTimeLocked(provider));
                p.enableLocationTracking(true);
            }
        } else {
            p.enableLocationTracking(false);
            p.disable();
        }
    }
```
我们只关注主体部分代码，就是在if(enabled)这个语句段里面，启动了gps的服务。
通过调用GpsLocationProvider类的enable和enableLocationTracking函数就把GPS的LocationManager服务启动起来了。下面对这两个函数进行分析。
首先是enable函数。
GpsLocationProvider.java
publicvoidenable(){
synchronized(mHandler){
            mHandler.removeMessages(ENABLE);
            Message m = Message.obtain(mHandler,ENABLE);
            m.arg1 = 1;
            mHandler.sendMessage(m);
}
}
对了，这个要提一点，在2.2中加入了一个ProviderHandler类（extends Handler），这个在2.1中是没有的，其实是换汤不换药的，对于函数调用的过程来说没有本质的改变。对于Handler的机制我还没有研究过。
publicvoid handleMessage(Message
 msg)
{
switch(msg.what){
caseENABLE:
if(msg.arg1 == 1){
                        handleEnable();
}else{
                        handleDisable();
}
break;
case ENABLE_TRACKING:
                    handleEnableLocationTracking(msg.arg1 == 1);
break;
                 ...
在handleMessage函数中，定义了各种message对应的处理函数。对于ENABLE消息还带有一个参数，enable函数里面带的参数值为1，所以调用handleEnable函数。
privatevoid handleEnable(){
if(DEBUG)Log.d(TAG,"handleEnable");
if(mEnabled)return;
mEnabled = native_init();
if(mEnabled){
if(mSuplServerHost !=null){
                native_set_agps_server(AGPS_TYPE_SUPL, mSuplServerHost, mSuplServerPort);
}
if(mC2KServerHost !=null){
                native_set_agps_server(AGPS_TYPE_C2K, mC2KServerHost, mC2KServerPort);
}
// run event listener thread while we are enabled
mEventThread =new GpsEventThread();
mEventThread.start();
}else{
Log.w(TAG,"Failed
 to enable location provider");
}
}
在handleEnable函数中中主要做了3件事，不过有一件事情没有做成。先来看看哪三件事：
1）调用了native的初始化方法对gps进行初始化，
2）试图启动agps服务，
3）并启动一个线程去监听事件。
先来说说它没有做成的第二件事，启动agps服务。其实在GpsLocationProvider类构造的时候就试图去读取agps的配置文件"/etc/gps.conf"，该文件里面储存着agps的服务器地址以及端口号，但是服务器地址以及端口号都是错误的，所以它基本上无法启动agps服务，而且对模拟器来说agps基本是个鸡肋。关于agps部分可能在以后的以后会提到。下面看它做成的第一和第三件事。
1)调用native方法native_init，就是JNI层的android_location_GpsLocationProvider_init方法，在文件andoird_location_GpsLocationProvider.cpp中。
static jboolean android_location_GpsLocationProvider_init(JNIEnv* env, jobject
 obj)
{
if(!sGpsInterface)
        sGpsInterface = gps_get_interface();
if(!sGpsInterface ||sGpsInterface->init(&sGpsCallbacks)!= 0)
returnfalse;
    ...
    return true;
}
在初始化函数中会去确认GpsInterface是否已经得到，如果没有得到那么通过gps_get_interface()方法再次去得到，正如其实前面提到的那样该接口已经在android_location_GpsLocationProvider_is_supported函数（第一个吃螃蟹的人）中得到了。然后在第二个if语句中调用初始化方法sGpsInterface->init。
android_location_GpsLocationProvider_init的后半部分，试图通过GpsInterface->get_extension方法去得到gps相关的扩展接口，可是在2.2的模拟器实现中并没有实现这个函数，在gps_qume.c中明显写着return
 NULL。
gps_qume.c
staticconstvoid*
qemu_gps_get_extension(constchar* name)
{
returnNULL;
}
--------------------------------------------------------------------------------
言归正传，分析sGpsInterface->init方法。
gps_qume.c
staticint
qemu_gps_init(GpsCallbacks* callbacks)
{
    GpsState* s = _gps_state;
if(!s->init)
gps_state_init(s);
if(s->fd <</span> 0)
return-1;
s->callbacks =*callbacks;
return 0;
}
在sGpsInterface->init中，也就是在qemu_gps_init方法，首先调用了gps_state_init，其次注册了回调函数，再说一次，这个回调函数就是在JNI层实现的，而且有JNI层传下来的函数。
staticvoid
gps_state_init( GpsState* state )
{
    state->init = 1;
    state->control[0]=-1;
    state->control[1]=-1;
    state->fd =-1;
    state->fd =qemu_channel_open(&state->channel,
                                   QEMU_CHANNEL_NAME,
                                   O_RDONLY );
if(state->fd <</span> 0){
        D("no gps emulation detected");
return;
}
    D("gps emulation will read from '%s' qemud channel", QEMU_CHANNEL_NAME );
if(socketpair(AF_LOCAL,SOCK_STREAM, 0, state->control )<</span> 0 ){
        LOGE("could not create thread control socket pair: %s",strerror(errno));
goto Fail;
}
if(pthread_create(&state->thread,NULL, gps_state_thread, state )!= 0 ){
        LOGE("could not create gps thread: %s",strerror(errno));
goto Fail;
}
    D("gps state initialized");
return;
Fail:
    gps_state_done( state );
}
在这个gps_state_init函数中，首先打开串口，然后建立socket通信，然后建立线程监听底层数据上报，分别对应于代码中黄低部分。
3）建立线程监听事件
mEventThread =new GpsEventThread();
mEventThread.start();
来看看GpsEventThread的run函数。
publicvoidrun(){
if(DEBUG)Log.d(TAG,"GpsEventThread
 starting");
// Exit as soon as disable() is called instead of waiting for the GPS to stop.
while(mEnabled){
// this will wait for an event from the GPS,
// which will be reported via reportLocation or reportStatus
native_wait_for_event();
}
if(DEBUG)Log.d(TAG,"GpsEventThread
 exiting");
}
}
run函数中还是需要调用native函数：JNI：android_location_GpsLocationProvider_wait_for_event函数。这个函数就是在一个while循环里面等待事件的触发（由回调函数触发），然后调用GpsLocationProvider类的数据上报函数（Location数据）。这个在后面还会讲到。
staticvoid android_location_GpsLocationProvider_wait_for_event(JNIEnv* env, jobject
 obj)
{
pthread_mutex_lock(&sEventMutex);
while(sPendingCallbacks == 0){
pthread_cond_wait(&sEventCond,&sEventMutex);
}
...
}

分析完了enable函数以后就轮到enableLocationTracking函数了。
GpsLocationProvider.java
```
public void enableLocationTracking(boolean enable) {
        synchronized (mHandler) {
            mHandler.removeMessages(ENABLE_TRACKING);
            Message m = Message.obtain(mHandler, ENABLE_TRACKING);
            m.arg1 = (enable ? 1 : 0);
            mHandler.sendMessage(m);
        }
    }
```
同样地，也采取Handler的方式。调用的是handleEnableLocationTracking函数。
```
private void handleEnableLocationTracking(boolean enable) {
        if (enable) {
            mTTFF = 0;
            mLastFixTime = 0;
            startNavigating();
        } else {
            mAlarmManager.cancel(mWakeupIntent);
            mAlarmManager.cancel(mTimeoutIntent);
            stopNavigating();
        }
    }
```
调用startNavigating函数。
```
private void startNavigating() {
        if (!mStarted) {
            if (DEBUG) Log.d(TAG, "startNavigating");
            mStarted = true;
            int positionMode;
            if (Settings.Secure.getInt(mContext.getContentResolver(),
                    Settings.Secure.ASSISTED_GPS_ENABLED, 1) != 0) {
                positionMode = GPS_POSITION_MODE_MS_BASED;
            } else {
                positionMode = GPS_POSITION_MODE_STANDALONE;
            }
            if (!native_start(positionMode, false, 1)) {
                mStarted = false;
                Log.e(TAG, "native_start
 failed in startNavigating()");
                return;
            }
```
`           ...`
在startNavigating函数中，最有作用的语句就是调用native方法native_start。调用到了JNI层的android_location_GpsLocationProvider_start函数。
android_location_GpsLocationProvider.cpp
--------------------------------------------------------------------------------
在startNavigating函数中，最有作用的语句就是调用native方法native_start。调用到了JNI层的android_location_GpsLocationProvider_start函数。
android_location_GpsLocationProvider.cpp
```
static jboolean android_location_GpsLocationProvider_start(JNIEnv* env, jobject
 obj, jint positionMode,
        jboolean singleFix, jint fixFrequency)
{
    int result = sGpsInterface->set_position_mode(positionMode, (singleFix ? 0 : fixFrequency));
    if (result) {
        return false;
    }
    return (sGpsInterface->start() == 0);
}
```
接下去就会调用sGpsInterface接口的实现gps_qemu.c中具体实现的函数。
```
static int
qemu_gps_start()
{
    GpsState* s = _gps_state;
    if (!s->init) {
        D("%s: called with uninitialized state !!", __FUNCTION__);
        return -1;
    }
    D("%s: called", __FUNCTION__);
    gps_state_start(s);
    return 0;
}
```
通过向底层发送命令，CMD_START来启动gps。其实这个所谓的底层就是在enable/init函数中启动的等待数据的线程。
```
static void
gps_state_start( GpsState* s )
{
    char cmd = CMD_START;
    int ret;
    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret <</span> 0 && errno == EINTR);
    if (ret != 1)
        D("%s: could not send CMD_START command: ret=%d: %s",
          __FUNCTION__, ret, strerror(errno));
}
```
数据监听线程
```
static void*
gps_state_thread( void* arg )
{ 
    ...
// now loop
    for (;;) {
     ...
   if (cmd == CMD_QUIT) {
   D("gps thread quitting on demand");
       goto Exit;
    }else
```
`   if (cmd == CMD_START) {`
```
if (!started) {
       D("gps thread starting  location_cb=%p",     state>callbacks.location_cb);
      started = 1;
  nmea_reader_set_callback( reader, state->callbacks.location_cb );
 } }
  else if (cmd == CMD_STOP) {
```
`...`
`}`
其实就是注册了一个回调函数，location_cb 这个回调函数就是对底层location数据上报的回调函数。
    在enableLocationTracking函数调用完成以后，基本上gps服务已经启动完成了，也就是LocationManagerService中的updateProvidersLocked函数的完成，也就是loadProviders函数的完成，也就是initialize函数的完成，也就是run函数的完成，也就是2.2中反馈机制systemReady的完成。
```
void systemReady() {
        // we defer starting up the service until the system is ready 
        Thread thread = new Thread(null, this, "LocationManagerService");
        thread.start();
    }
```
```
```
