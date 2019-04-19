# Android的Camera架构介绍 - xqhrs232的专栏 - CSDN博客
2011年06月17日 13:45:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：850标签：[android																[callback																[java																[class																[interface																[jni](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/miliang1688/archive/2011/01/17/6146241.aspx](http://blog.csdn.net/miliang1688/archive/2011/01/17/6146241.aspx)
第一部分 Camera概述
Android 的Camera包含取景器（viewfinder）和拍摄照片的功能 。目前Android发布版的Camera程序虽然功能比较简单，但是其程序的架构分成客户端和服务器两个部分，它们建立在Android的进程间通讯Binder的结构上。
以开源 的Android为例，Camera的代码主要在以下的目录中：
Camera的JAVA程序的路径：
packages/apps/Camera/src/com/android/camera/
在其中Camera.java 是主要实现的文件
Camera的JAVA本地调用部分（JNI）：
frameworks/base/core/jni/android_hardware_Camera.cpp 
这部分内容编译成为目标是libandroid_runtime.so 。
主要的头文件在以下的目录中：
frameworks/base/include/ui/ 
Camera底层库在以下的目录中：
frameworks/base/libs/ui/ 
这部分的内容被编译成库libui.so 。
Camera服务部分：
frameworks/base/camera/libcameraservice/ 
这部分内容被编译成库libcameraservice.so 。
为了实现一个具体功能的Camera，在最底层还需要一个硬件相关的Camer库（例如通过调用video for linux驱动程序和Jpeg编码程序实现）。这个库将被Camera的服务库libcameraservice.so 调用。
第二部分 Camera的接口与架构 
2.1 Camera的整体框架图 
Camera的各个库之间的结构可以用下图的表示：
在 Camera系统的各个库中，libui.so位于核心的位置，它对上层的提供的接口主要是Camera类，类 libandroid_runtime.so通过调用Camera类提供对JAVA的接口，并且实现了android.hardware.camera 类。 libcameraservice.so是Camera的服务器程序，它通过继承libui.so的类实现服务器的功能，并且与libui.so中的另外一部分内容则通过进程间通讯（即Binder机制）的方式进行通讯。
libandroid_runtime.so和libui.so两个库是公用的,其中除了Camera还有其他方面的功能。
Camera部分的头文件在frameworks/base/include/ui/ 目录中，这个目录是和libmedia.so库源文件的目录frameworks/base/libs/ui/ 相对应的。
Camera主要的头文件有以下几个： 
■ICameraClient.h
■Camera.h
■ICamera.h 
■ICameraService.h
■CameraHardwareInterface.h 
在这些头文件Camera.h提供了对上层的接口，而其他的几个头文件都是提供一些接口类（即包含了纯虚函数的类），这些接口类必须被实现类继承才能够使用。
整个Camera在运行的时候，可以大致上分成Client和Server两个部分，它们分别在两个进程中运行，它们之间使用Binder机制实现进程间通讯。这样在客户端调用接口，功能则在服务器中实现，但是在客户端中调用就好像直接调用服务器中的功能，进程间通讯的部分对上层程序不可见。
从框架结构上来看，ICameraService.h、ICameraClient.h和ICamera.h三个类定义了MeidaPlayer的接口和架构，ICameraService.cpp和Camera.cpp两个文件用于Camera架构的实现，Camera的具体功能在下层调用硬件相关的接口来实现。
从Camera的整体结构上，类Camera是整个系统核心，ICamera类提供了Camera主要功能的接口，在客户端方面调用，CameraService是Camera服务，它通过调用实际的Camera硬件接口来实现功能。事实上，图中红色虚线框的部分都是Camera程序的框架部分，它主要利用了Android的系统的Binder机制来完成通讯。蓝色的部分通过调用Camera硬件相关的接口完成具体的Camera服务功能，其它的部分是为上层的JAVA程序提供JNI接口。在整体结构上，左边可以视为一个客户端，右边是一个可以视为服务器，二者通过Android的
 Bimder来实现进程间的通讯。
2.2 头文件ICameraClient.h
2.2 头文件ICameraClient.h 
ICameraClient.h用于描述一个Camera客户端的接口，定义如下所示： 
class ICameraClient: public IInterface 
{ 
public: 
DECLARE_META_INTERFACE(CameraClient); 
virtual void shutterCallback() = 0; 
virtual void rawCallback(const sp<IMemory>& picture) = 0; 
virtual void jpegCallback(const sp<IMemory>& picture) = 0; 
virtual void frameCallback(const sp<IMemory>& frame) = 0; 
virtual void errorCallback(status_t error) = 0; 
virtual void autoFocusCallback(bool focused) = 0; 
}; 
class BnCameraClient: public BnInterface<ICameraClient> 
{ 
public: 
virtual status_t onTransact( uint32_t code, 
const Parcel& data, 
Parcel* reply, 
uint32_t flags = 0); 
}; 
在定义中，ICameraClient 类继承IInterface，并定义了一个Camera客户端的接口，BnCameraClient 继承了BnInterface<ICameraClient>，这是为基于Android的基础类Binder机制实现在进程通讯而构建的。根据BnInterface类模版的定义BnInterface<ICameraClient>类相当于双继承了BnInterface和 ICameraClient。
IcameraClient这个类的主要接口是几个回调函数shutterCallback、rawCallback和jpegCallback等，它们在相应动作发生的时候被调用。作为Camera的“客户端”，需要自己实现几个回调函数，让服务器程序去“间接地”调用它们。
2.3 头文件Camera.h
2.3 头文件Camera.h 
Camera.h是Camera对外的接口头文件，它被实现Camera JNI的文件android_hardware_Camera.cpp所调用。Camera.h最主要是定义了一个Camera类：
class Camera : public BnCameraClient, public IBinder::DeathRecipient 
{ 
public: 
staticsp<Camera>connect(); 
~Camera(); 
voiddisconnect(); 
status_tgetStatus() { return mStatus; } 
status_tsetPreviewDisplay(const sp<Surface>& surface); 
status_tstartPreview(); 
voidstopPreview(); 
status_tautoFocus(); 
status_ttakePicture(); 
status_tsetParameters(const String8& params); 
String8 getParameters() const; 
voidsetShutterCallback(shutter_callback cb, void *cookie); 
voidsetRawCallback(frame_callback cb, void *cookie); 
voidsetJpegCallback(frame_callback cb, void *cookie); 
voidsetFrameCallback(frame_callback cb, void *cookie); 
voidsetErrorCallback(error_callback cb, void *cookie); 
voidsetAutoFocusCallback(autofocus_callback cb, void *cookie); 
// ICameraClient interface 
virtual voidshutterCallback(); 
virtual voidrawCallback(const sp<IMemory>& picture); 
virtual voidjpegCallback(const sp<IMemory>& picture); 
virtual voidframeCallback(const sp<IMemory>& frame); 
virtual voiderrorCallback(status_t error); 
virtual voidautoFocusCallback(bool focused); 
//…… 
}
从接口中可以看出Camera类刚好实现了一个Camera的基本操作，例如播放（startPreview）、停止（stopPreview）、暂停（takePicture）等。在Camera类中connect()是一个静态函数，它用于得到一个Camera的实例。在这个类中，具有设置回调函数的几个函数：setShutterCallback、setRawCallback和setJpegCallback等，这几个函数是为了提供给上层使用，上层利用这几个设置回调函数，这些回调函数在相应的回调函数中调用，例如使用setShutterCallback设置的回调函数指针被
 shutterCallback所调用。
在定义中，ICameraClient 类双继承了IInterface和IBinder::DeathRecipient，并定义了一个Camera客户端的接口，BnCameraClient 继承了BnInterface<ICameraClient>，这是为基于Android的基础类Binder机制实现在进程通讯而构建的。事实上，根据BnInterface类模版的定义BnInterface<ICameraClient>类相当于双继承了 BnInterface和ICameraClient。这是Android一种常用的定义方式。
继承了DeathNotifier类之后，这样当这个类作为IBinder使用的时候，当这个Binder即将Died的时候被调用其中的binderDied函数。继承这个类基本上实现了一个回调函数的功能。
2.4 头文件ICamera.h
2.4 头文件ICamera.h 
ICamera.h描述的内容是一个实现Camera功能的接口，其定义如下所示： 
class ICamera: public IInterface 
{ 
public: 
DECLARE_META_INTERFACE(Camera); 
virtual void disconnect() = 0; 
virtual status_tsetPreviewDisplay(const sp<ISurface>& surface) = 0; 
virtual void setHasFrameCallback(bool installed) = 0; 
virtual status_tstartPreview() = 0; 
virtual void stopPreview() = 0; 
virtual status_tautoFocus() = 0; 
virtual status_ttakePicture() = 0; 
virtual status_tsetParameters(const String8& params) = 0; 
virtual String8 getParameters() const = 0; 
}; 
class BnCamera: public BnInterface<ICamera> 
{ 
public: 
virtual status_tonTransact( uint32_t code, 
const Parcel& data, 
Parcel* reply, 
uint32_t flags = 0); 
};
在camera类中，主要定义Camera的功能接口，这个类必须被继承才能够使用。值得注意的是，这些接口和Camera类的接口有些类似，但是它们并没有直接的关系。事实上，在Camera类的各种实现中，一般都会通过调用ICamera类的实现类来完成。
2.5 头文件ICameraService .h
2.5 头文件ICameraService .h 
ICameraService.h用于描述一个Camera的服务，定义方式如下所示： 
class ICameraService : public IInterface 
{ 
public: 
DECLARE_META_INTERFACE(CameraService); 
virtual sp<ICamera> connect(const sp<ICameraClient>& cameraClient) = 0; 
}; 
class BnCameraService: public BnInterface<ICameraService> 
{ 
public: 
virtual status_t onTransact( uint32_t code, 
const Parcel& data, 
Parcel* reply, 
uint32_t flags = 0); 
}; 
由于具有纯虚函数， ICameraService 以及BnCameraService必须被继承实现才能够使用，在ICameraService 只定义了一个connect()接口，它的返回值的类型是sp<ICamera>，这个ICamera 是提供实现功能的接口。注意，ICameraService只有连接函数connect()，没有断开函数，断开的功能由ICamera接口来提供。
2.6 头文件CameraHardwareInterface.h
2.6 头文件CameraHardwareInterface.h 
CameraHardwareInterface.h定义的是一个Camera底层的接口，这个类的实现者是最终实现Camera的。
CameraHardwareInterface 定以Camera硬件的接口，如下所示： 
class CameraHardwareInterface : public virtual RefBase { 
public: 
virtual ~CameraHardwareInterface() { } 
virtual sp<IMemoryHeap> getPreviewHeap() const = 0; 
virtual status_t startPreview(preview_callback cb, void* user) = 0; 
virtual voidstopPreview() = 0; 
virtual status_t autoFocus(autofocus_callback, 
void* user) = 0; 
virtual status_t takePicture(shutter_callback, 
raw_callback, 
jpeg_callback, 
void* user) = 0; 
virtual status_t cancelPicture(bool cancel_shutter, 
bool cancel_raw, 
bool cancel_jpeg) = 0; 
virtual status_t setParameters(const CameraParameters& params) = 0; 
virtual CameraParametersgetParameters() const = 0; 
virtual void release() = 0; 
virtual status_t dump(int fd, const Vector<String16>& args) const = 0; 
};
使用C语言的方式导出符号： 
extern "C" sp<CameraHardwareInterface> openCameraHardware();
在程序的其他地方，使用openCameraHardware()就可以得到一个 CameraHardwareInterface，然后调用 CameraHardwareInterface的接口完成Camera的功能。
第三部分 Camera的主要实现分析
第三部分 Camera的主要实现分析
3.1 JAVA程序部分
在packages/apps/Camera/src/com/android/camera/ 
目录的Camera.java文件中，包含了对Camera的调用
在Camera.java中包含对包的引用： 
import android.hardware.Camera.PictureCallback; 
import android.hardware.Camera.Size;
在这里定义的Camera类继承了活动Activity类，在它的内部，包含了一个 android.hardware.Camera 
public class Camera extends Activity implements View.OnClickListener, SurfaceHolder.Callback {
android.hardware.Camera mCameraDevice; 
}
对Camera功能的一些调用如下所示： 
mCameraDevice.takePicture(mShutterCallback, mRawPictureCallback, mJpegPictureCallback);
mCameraDevice.startPreview(); 
mCameraDevice.stopPreview();
startPreview、stopPreview 和takePicture等接口就是通过JAVA本地调用（JNI）来实现的。
frameworks/base/core/java/android/hardware/ 目录中的Camera.java文件提供了一个JAVA类：Camera。 
public class Camera { 
} 
public void setParameters(Parameters params) { 
Log.e(TAG, "setParameters()"); 
//params.dump(); 
native_setParameters(params.flatten()); 
}
在这个类当中，大部分代码使用JNI调用下层得到，例如：
再者，例如以下代码： 
public final void setPreviewDisplay(SurfaceHolder holder) { 
setPreviewDisplay(holder.getSurface()); 
} 
private native final void setPreviewDisplay(Surface surface);
两个setPreviewDisplay 参数不同，后一个是本地方法，参数为Surface 类型，前一个通过调用后一个实现，但自己的参数以SurfaceHolder 为类型。
3.2 Camera的JAVA本地调用部分
3.2 Camera的JAVA本地调用部分 
Camera的JAVA本地调用（JNI）部分在frameworks/base/core/jni/ 目录的android_hardware_Camera.cpp 中的文件中实现。
android_hardware_Camera.cpp之中定义了一个JNINativeMethod（JAVA本地调用方法）类型的数组gMethods，如下所示： 
static JNINativeMethod camMethods[] = { 
{"native_setup","(Ljava/lang/Object;)V",(void*)android_hardware_Camera_native_setup },
{"native_release","()V",(void*)android_hardware_Camera_release }, 
{"setPreviewDisplay","(Landroid/view/Surface;)V",(void *)android_hardware_Camera_setPreviewDisplay },
{"startPreview","()V",(void *)android_hardware_Camera_startPreview }, 
{"stopPreview", "()V", (void *)android_hardware_Camera_stopPreview }, 
{"setHasPreviewCallback","(Z)V",(void *)android_hardware_Camera_setHasPreviewCallback },
{"native_autoFocus","()V",(void *)android_hardware_Camera_autoFocus }, 
{"native_takePicture", "()V", (void *)android_hardware_Camera_takePicture }, 
{"native_setParameters","(Ljava/lang/String;)V",(void *)android_hardware_Camera_setParameters },
{"native_getParameters", "()Ljava/lang/String;",(void*)android_hardware_Camera_getParameters }
}; 
JNINativeMethod的第一个成员是一个字符串，表示了JAVA本地调用方法的名称，这个名称是在JAVA程序中调用的名称；第二个成员也是一个字符串，表示JAVA本地调用方法的参数和返回值；第三个成员是JAVA本地调用方法对应的C语言函数。
register_android_hardware_Camera 函数将gMethods注册为的类"android/media/Camera"，其主要的实现如下所示。
int register_android_hardware_Camera(JNIEnv *env) 
{ 
// Register native functions 
return AndroidRuntime::registerNativeMethods(env, "android/hardware/Camera", 
camMethods, NELEM(camMethods)); 
} 
"android/hardware/Camera"对应JAVA的类android.hardware.Camera。 
3.3 Camera本地库libui.so
3.3 Camera本地库libui.so 
frameworks/base/libs/ui/中的Camera.cpp文件用于实现Camera.h提供的接口，其中一个重要的片段如下所示 
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
其中最重要的一点是binder = sm->getService(String16("media.camera"));;这个调用用来得到一个名称为"media.camera" 的服务，这个调用返回值的类型为IBinder，根据实现将其转换成类型ICameraService使用。
一个函数 connect的实现 如下所示： 
sp<Camera> Camera::connect() 
{ 
sp<Camera> c = new Camera(); 
const sp<ICameraService>& cs = getCameraService(); 
if (cs != 0) { 
c->mCamera = cs->connect(c); 
} 
if (c->mCamera != 0) { 
c->mCamera->asBinder()->linkToDeath(c); 
c->mStatus = NO_ERROR; 
} 
return c; 
}
connect通过调用getCameraService得到一个 ICameraService，再通过 ICameraService的cs->connect(c)得到一个 ICamera类型的指针。 调用connect将得到一个 Camera的指针,正常情况下Camera的成员 mCamera已经初始化完成。
一个具体的函数startPreview 如下所示： 
status_t Camera::startPreview() 
{ 
return mCamera->startPreview(); 
} 
这些操作可以直接对 mCamera来进行，它是ICamera类型的指针。
其他一些函数的实现也与setDataSource类似。
libmedia.so中的其他一些文件与头文件的名称相同，它们是：
frameworks/base/libs/ui/ICameraClient.cpp
frameworks/base/libs/ui/ICamera.cpp
frameworks/base/libs/ui/ICameraService.cpp 
在此处，BnCameraClient和BnCameraService类虽然实现了onTransact()函数，但是由于还有纯虚函数没有实现，因此这个类都是不能实例化的。
ICameraClient.cpp中的BnCameraClient在别的地方也没有实现；而ICameraService.cpp中的BnCameraService类在别的地方被继承并实现，继承者实现了Camera服务的具体功能。
3.4 Camera服务libcameraservice.so
frameworks/base/camera/libcameraservice/ 用于实现一个Camera的服务，这个服务是继承ICameraService的具体实现。
在这里的Android.mk文件中，使用宏USE_CAMERA_STUB决定是否使用真的Camera，如果宏为真，则使用 CameraHardwareStub.cpp和FakeCamera.cpp构造一个假的Camera，如果为假则使用 CameraService.cpp构造一个实际上的Camera服务。
CameraService.cpp是继承BnCameraService 的实现，在这个类的内部又定义了类Client，CameraService::Client继承了BnCamera。在运作的过程中 CameraService::connect()函数用于得到一个CameraService::Client，在使用过程中，主要是通过调用这个类的接口来实现完成Camera的功能，由于CameraService::Client本身继承了BnCamera类，而BnCamera类是继承了 ICamera，因此这个类是可以被当成ICamera来使用的。
CameraService和CameraService::Client两个类的结果如下所示：
class CameraService : public BnCameraService 
{ 
class Client : public BnCamera {}; 
wp<Client>mClient; 
}
在CameraService中的一个静态函数instantiate()用于初始化一个Camera服务，寒暑如下所示： 
void CameraService::instantiate() { 
defaultServiceManager()->addService( 
String16("media.camera"), new CameraService()); 
}
事实上，CameraService::instantiate()这个函数注册了一个名称为"media.camera"的服务，这个服务和Camera.cpp中调用的名称相对应。
Camera整个运作机制是：在Camera.cpp中可以调用ICameraService的接口，这时实际上调用的是 BpCameraService，而BpCameraService又通过Binder机制和BnCameraService实现两个进程的通讯。而 BpCameraService的实现就是这里的CameraService。因此，Camera.cpp虽然是在另外一个进程中运行，但是调用 ICameraService的接口就像直接调用一样，从connect()中可以得到一个ICamera类型的指针，真个指针的实现实际上是
 CameraService::Client。
而这些Camera功能的具体实现，就是CameraService::Client所实现的了，其构造函数如下所示： 
CameraService::Client::Client(const sp<CameraService>& cameraService, 
const sp<ICameraClient>& cameraClient) : 
mCameraService(cameraService), mCameraClient(cameraClient), mHardware(0) 
{ 
mHardware = openCameraHardware(); 
mHasFrameCallback = false; 
}
构造函数中，调用openCameraHardware()得到一个CameraHardwareInterface类型的指针，并作为其成员mHardware。以后对实际的Camera的操作都通过对这个指针进行。这是一个简单的直接调用关系。
事实上，真正的Camera功能己通过实现CameraHardwareInterface类来完成。在这个库当中 CameraHardwareStub.h和CameraHardwareStub.cpp两个文件定义了一个桩模块的接口，在没有Camera硬件的情况下使用，例如在仿真器的情况下使用的文件就是CameraHardwareStub.cpp和它依赖的文件FakeCamera.cpp。
CameraHardwareStub类的结构如下所示： 
class CameraHardwareStub : public CameraHardwareInterface { 
class PreviewThread : public Thread { 
}; 
};
在类CameraHardwareStub当中，包含一个线程类PreviewThread，这个线程用于处理PreView，即负责刷新取景器的内容。实际的Camera硬件接口通常可以通过对v4l2 捕获驱动的调用来实现，同时还需要一个JPEG编码程序将从驱动中取出的数据编码成JPEG文件。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/miliang1688/archive/2011/01/17/6146241.aspx](http://blog.csdn.net/miliang1688/archive/2011/01/17/6146241.aspx)

