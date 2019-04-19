# Android Camera Framework Stream(二) - xqhrs232的专栏 - CSDN博客
2011年06月17日 13:34:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：906标签：[android																[stream																[interface																[jni																[null																[descriptor](https://so.csdn.net/so/search/s.do?q=descriptor&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/miliang1688/archive/2011/01/17/6146309.aspx](http://blog.csdn.net/miliang1688/archive/2011/01/17/6146309.aspx)
接下来，我们通过对流程的步步分析来将camera整体串接起来:
1． 首先则看看camera.java的 onCreate函数入口，针对android的所有应用，onCreate函数入口作为跟踪和了解应用架构的首选。
@Override
public void onCreate(Bundle icicle) {
super.onCreate(icicle);
devlatch = new CountDownLatch(1);
CountDownLatch()关于这个类，可以简单的理解为它是用来线程之间的等待处理，当然这里采用的计数为1，则可以简单理解为一个计数开关来控制调用了tlatch.await()函数的进程，方式就是将devlatch的计数减为0(countDown() )。
这里启动了一个线程用来打开camera服务，而打开过程则比较费时(一般在2s左右)，故单独启用一个线程避免应用线程阻塞。
Thread startPreviewThread = new Thread(new Runnable() {
CountDownLatch tlatch = devlatch;
public void run() {
try {
mStartPreviewFail = false;
ensureCameraDevice();
// Wait for framework initialization to be complete before
// starting preview
try {
tlatch.await();
} catch (InterruptedException ie) {
mStartPreviewFail = true;
}
startPreview();
} catch (CameraHardwareException e) {
// In eng build, we throw the exception so that test tool
// can detect it and report it
if ("eng".equals(Build.TYPE)) {
throw new RuntimeException(e);
}
mStartPreviewFail = true;
}
}
});
startPreviewThread.start();
在这里，需要跟进ensureCameraDevice();该函数，可以看到其实现为：
private void ensureCameraDevice() throws CameraHardwareException {
if (mCameraDevice == null) {
mCameraDevice = CameraHolder.instance().open();
mInitialParams = mCameraDevice.getParameters();
}
}
当前mCameraDevice()实例为null,则会调用CameraHolder.instance().open()函数来创建mCameraDevice对象实例。
private android.hardware.Camera mCameraDevice;
跟进CameraHolder.instance().open()，进入到了CameraHolder类中：
public synchronized android.hardware.Camera open()
throws CameraHardwareException {
Assert(mUsers == 0);
if (mCameraDevice == null) {
try {
mCameraDevice = android.hardware.Camera.open();
} catch (RuntimeException e) {
Log.e(TAG, "fail to connect Camera", e);
throw new CameraHardwareException(e);
}
mParameters = mCameraDevice.getParameters();
} else {
……
下面大概介绍下我对CameraHolder的理解：
1、CameraHolder对mCameraDevice实例进行短暂的保留(keep()函数中可以设定这个保留时长,一般默认为3000ms)，避免用户在短暂退出camera又重新进入时，缩短camera启动时长(正如之前所说，打开CameraDevice时间较长)
2、CameraHolder并有一个关键的计数mUsers用来保证open()和release()的配套调用，避免多次重复释放或者打开(上层应用的保护措施之一)。
2． 第一步的完成，进而跳转到了android.hardware.Camera类中的open()函数接口调用。
public static Camera open() {
return new Camera();
}
静态函数，也就可以通过类名直接调用，open()函数中去创建一个Camera的实例。
Camera() {
mShutterCallback = null;
mRawImageCallback = null;
mJpegCallback = null;
mPreviewCallback = null;
mPostviewCallback = null;
mZoomListener = null;
Looper looper;
if ((looper = Looper.myLooper()) != null) {
mEventHandler = new EventHandler(this, looper);
} else if ((looper = Looper.getMainLooper()) != null) {
mEventHandler = new EventHandler(this, looper);
} else {
mEventHandler = null;
}
native_setup(new WeakReference<Camera>(this));
}
在Camera构造函数中有这个关键的一步,最开始的一些callback可以认为它们最终被底层调用到(至于具体流程后面会讲到)。EventHandler和Looper我们暂时跳过，知道它是消息处理就行了。最后也就是最为关键的函数接口调用：native_setup
private native final void native_setup(Object camera_this);
典型的native函数接口声明，说明并非camera类的本地函数实现，也就意味着会通过JNI(Java Native Interface)调用对用C++文件中的函数接口。
3． 通过代码搜索，或者如果你清楚JNI文件路径也可以去该路径下找。
其实这边有个小技巧，虽然不一定都通用，但可以试试看：
java类的package名往往可以作为寻找相应JNI文件的途径：
package android.hardware;
则就可以通过android.hardware.camera.cpp来寻找(其实还是归咎于android的规范命名规则)。
跳转到android_hardware_Camera.cpp中寻找native_setup()所对应的JNI函数接口：
static JNINativeMethod camMethods[] = {
{ "native_setup",
"(Ljava/lang/Object;)V",
(void*)android_hardware_Camera_native_setup },
{ "native_release",
"()V",
(void*)android_hardware_Camera_release },
{ "setPreviewDisplay",
"(Landroid/view/Surface;)V",
(void *)android_hardware_Camera_setPreviewDisplay },
……
而camMethods[]在什么时候映射的那？继续看：
int register_android_hardware_Camera(JNIEnv *env) {
…..
// Register native functions
return AndroidRuntime::registerNativeMethods(env, "android/hardware/Camera",
camMethods,NELEM(camMethods));
}
最终在AndroidRuntime.cpp中被调用：
REG_JNI(register_android_hardware_Camera),
说明如果我们自己要添加JNI接口实现的话，这些地方也需要添加相应的代码(具体在AndroidRuntime.cpp的细节我没深看，也不做介绍)。
简单介绍： JNINativeMethod的第一个成员是一个字符 串，表示了JAVA本地调用方法的名称，这个名称是在JAVA程序中调用的名称；第二个成员也是一个字符串，表示JAVA本地调用方法的参数和返回值；第三个成员是JAVA本地调用方法对应的C语言函数。
跟进观察android_hardware_Camera_native_setup()函数的实现：
// connect to camera service
static void android_hardware_Camera_native_setup(JNIEnv *env, jobject thiz, jobject weak_this)
{
sp<Camera> camera = Camera::connect();
if (camera == NULL) {
jniThrowException(env, "java/lang/RuntimeException",
"Fail to connect to camera service");
return;
}
….
}
初步可以认为Camera::connect()的函数调用时返回了一个Camera 的实例对象。
4． 通过上述的跟进流程来到了针对上层应用而言最为直接的类：camera.cpp：
对Camera::connect函数的调用如下：
sp<Camera> Camera::connect()
{
LOGV("connect");
sp<Camera> c = new Camera();
const sp<ICameraService>& cs = getCameraService();
if (cs != 0) {
c->mCamera = cs->connect(c);
}
if (c->mCamera != 0) {
c->mCamera->asBinder()->linkToDeath(c);
c->mStatus = NO_ERROR;
} else {
c.clear();
}
return c;
}
首先是创建一个camera对象实例，然后通过调用getCameraService()去取得ICameraService的服务实例：
// establish binder interface to camera service
const sp<ICameraService>& Camera::getCameraService()
{
Mutex::Autolock _l(mLock);
if (mCameraService.get() == 0) {
sp<IServiceManager> sm = defaultServiceManager();
sp<IBinder> binder;
do {
binder = sm->getService(String16("media.camera"));
if (binder != 0)
break;
LOGW("CameraService not published, waiting...");
usleep(500000); // 0.5 s
} while(true);
if (mDeathNotifier == NULL) {
mDeathNotifier = new DeathNotifier();
}
binder->linkToDeath(mDeathNotifier);
mCameraService = interface_cast<ICameraService>(binder);
}
LOGE_IF(mCameraService==0, "no CameraService!?");
return mCameraService;
}
这边就涉及到了ServiceManager()对服务的管理，在这之前Camera的服务已经注册到了ServiceManager中，我们可以通过服务字串(media.camera)来获得camera service(其本质得到的是CameraService的实例对象，虽然通过类型上溯转换成父类ICameraService，对ICameraService对象的函数调用本质是调用到了CameraService的函数实现)。
在得到camera service后，返回之前的步骤：当得到的cs即cameraservice实例存在时，通过调用cs->connect(c)去得到ICamera实例，并赋值给了camera实例的一个类成员ICamera mCamera：
if (cs != 0) {
c->mCamera = cs->connect(c);
5． 接下来则涉及到ICamraService的相关调用关系，其实这个地方需要去弄清楚一些函数接口的实现在具体哪些文件中，因为存在较多的虚函数。
继续流程，上一步走到了cs->connect()，也就是ICameraService的connect()函数接口。
class ICameraService : public IInterface
{
public:
enum {
CONNECT = IBinder::FIRST_CALL_TRANSACTION,
};
public:
DECLARE_META_INTERFACE(CameraService);
virtual sp<ICamera> connect(const sp<ICameraClient>& cameraClient) = 0;
};
可以发现该connect()接口为一个纯虚函数，需要ICameraService的子类对该接口进行实现，从而对connect()的调用则会映射到ICameraService子类的具体实现。
关于ICameraService的实例问题，目前暂时跳过(后面马上就会讲到)，简单认为这个时候会调用到其一个子类的实现：
class BpCameraService: public BpInterface<ICameraService>
{
public:
BpCameraService(const sp<IBinder>& impl)
: BpInterface<ICameraService>(impl)
{
}
// connect to camera service
virtual sp<ICamera> connect(const sp<ICameraClient>& cameraClient)
{
Parcel data, reply;
data.writeInterfaceToken(ICameraService::getInterfaceDescriptor());
data.writeStrongBinder(cameraClient->asBinder());
remote()->transact(BnCameraService::CONNECT, data, &reply);
return interface_cast<ICamera>(reply.readStrongBinder());
}
};
BpCameraService为代理类，其主要用途为Binder通讯机制即进程间的通讯(Client/Service)，最终还是会调用BnCameraService的具体实现，即：
status_t BnCameraService::onTransact(
uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
switch(code) {
case CONNECT: {
CHECK_INTERFACE(ICameraService, data, reply);
sp<ICameraClient> cameraClient
= interface_cast<ICameraClient>(data.readStrongBinder());
sp<ICamera> camera = connect(cameraClient);
reply->writeStrongBinder(camera->asBinder());
return NO_ERROR;
} break;
default:
return BBinder::onTransact(code, data, reply, flags);
}
}
而BnCameraService(为实现类)类继承于ICameraService，并且也并没有对connect()纯虚函数进行了实现，同样意味着其实该调用的实质是BnCameraService的子类实现。
毕竟虚函数的调用没有实例肯定是没有意义的，说明我们需要找到对connect()纯虚函数的实现子类即继承于BnCameraService。
6． 结合上面所述，可以寻找到了继承于BnCameraService的子类CameraService.cpp：
这时虽然找到了CameraService该类，但是你肯定会问到该类实例的创建在什么地方哪？再后头看CameraService启动注册的地方：
int main(int argc, char** argv)
{
sp<ProcessState> proc(ProcessState::self());
sp<IServiceManager> sm = defaultServiceManager();
LOGI("ServiceManager: %p", sm.get());
AudioFlinger::instantiate();
MediaPlayerService::instantiate();
CameraService::instantiate();
AudioPolicyService::instantiate();
ProcessState::self()->startThreadPool();
IPCThreadState::self()->joinThreadPool();
}
这个main函数位于main_mediaserver.cpp中，而mediaserver是在系统开始的时候就启动起来的server端（MediaServer，在系统启动时由init所启动，具可参考init.rc文件），进而将相关的服务也创建了实例。
跟进CameraService::instantiate()函数实现，可以发现：
void CameraService::instantiate() {
defaultServiceManager()->addService(
String16("media.camera"), new CameraService());
}
创建了一个CameraService实例 ，并给定了CameraService的服务字串为”media.camera”，而之前在通过ServiceManager获取CameraService的时候，所调用的接口为binder = sm->getService(String16("media.camera"));，两者保持了一样的字符串。
if (mCameraService.get() == 0) {
sp<IServiceManager> sm = defaultServiceManager();
sp<IBinder> binder;
do {
binder = sm->getService(String16("media.camera"));
if (binder != 0)
break;
LOGW("CameraService not published, waiting...");
usleep(500000); // 0.5 s
} while(true);
if (mDeathNotifier == NULL) {
mDeathNotifier = new DeathNotifier();
}
binder->linkToDeath(mDeathNotifier);
mCameraService = interface_cast<ICameraService>(binder);
}
结合上述分析，此处的binder对象其实为CameraService类实例(多态类型转换)。
interface_cast<ICameraService>(binder)宏映射，需要展开：
template<typename INTERFACE>
inline sp<INTERFACE> interface_cast(const sp<IBinder>& obj)
{
return INTERFACE::asInterface(obj);
}
INTERFACE::asInterface(obj);宏映射，继续展开可得：
sp<I##INTERFACE> I##INTERFACE::asInterface(const sp<IBinder>& obj) /
{ /
sp<I##INTERFACE> intr; /
if (obj != NULL) { /
intr = static_cast<I##INTERFACE*>( /
obj->queryLocalInterface( /
I##INTERFACE::descriptor).get()); /
if (intr == NULL) { /
intr = new Bp##INTERFACE(obj); /
} /
} /
return intr; /
} 
(其上的宏展开都是在IMPLEMENT_META_INTERFACE(CameraService, "android.hardware.ICameraService");中实现的)
此处又创建了一个BpCameraService(new Bp##INTERFACE)对象并将binder对象(obj)传入到BpCameraService的构造函数中。
虽然获取的时候通过多态将CameraService实例转换成了BnCameraService 也进一步解释了为什么ICameraService子类BnCameraservice中的connect函数实质会调用到CameraService中函数实现了。
于是就调用到了CameraService的connect函数接口：
sp<ICamera> CameraService::connect(const sp<ICameraClient>& cameraClient) {
…..
// create a new Client object
client = new Client(this, cameraClient, callingPid);
mClient = client;
if (client->mHardware == NULL) {
client = NULL;
mClient = NULL;
return client;
}
…..
}
创建了一个Client实例对象，并将该实例对象赋值给CameraSevice的类成员mClient,方便其实函数接口对Client的调用。
在这之前需要提及它的一个内部类Client，该类才是最为关键的函数实现,CameraService的一些接口都会调用到其Client实例的具体函数。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/miliang1688/archive/2011/01/17/6146309.aspx](http://blog.csdn.net/miliang1688/archive/2011/01/17/6146309.aspx)

