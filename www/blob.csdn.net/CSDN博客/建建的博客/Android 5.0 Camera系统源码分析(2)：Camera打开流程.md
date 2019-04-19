# Android 5.0 Camera系统源码分析(2)：Camera打开流程 - 建建的博客 - CSDN博客
2017年08月02日 10:32:33[纪建](https://me.csdn.net/u013898698)阅读数：479
# 1. 前言
本文将分析[Android](http://lib.csdn.net/base/android)系统源码，从frameworks层到hal层，暂不涉及app层和kernel层。由于某些函数比较复杂，在贴出代码时会适当对其进行简化。本文属于自己对源码的总结，仅仅是贯穿代码流程，不会深入分析各个细节。欢迎联系讨论，QQ：1026656828
# 2. app层
从apk开始，简单列出各个入口函数
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- privatevoid initCamera()  
- {  
-     Camera mCamera = Camera.open();  
-     Camera.Parameters mParameters = mCamera.getParameters();  
-     mParameters.setPictureFormat(PixelFormat.JPEG);  
-     mCamera.setParameters(mParameters);  
-     mCamera.setPreviewDisplay(mSurfaceHolder);  
-     mCamera.startPreview();  
-     mCamera.takePicture(null, null , mJpegCallback);  
- }  
# 3. frameworks层
这里将重点介绍Camera.open函数，其余函数将在后续博文分析。先来看看Camera.open函数在frameworks层的实现，代码路径为: frameworks/base/core/java/android/hardware/Camera.java
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- publicstatic Camera open() {  
- if (!isPermissionGranted()) {  
- return null;  
-     }      
- int numberOfCameras = getNumberOfCameras();  
-     CameraInfo cameraInfo = new CameraInfo();  
- for (int i = 0; i < numberOfCameras; i++) {  
-         getCameraInfo(i, cameraInfo);  
- if (cameraInfo.facing == CameraInfo.CAMERA_FACING_BACK) {  
- returnnew Camera(i);  
-         }      
-     }      
- return null;  
- }      
第5行, 通过getNumberOfCameras函数来获取Camera的个数。从上一篇博文[CameraService的启动流程](http://blog.csdn.net/shell812/article/details/49424857)可以看出，这个信息保存在CameraService中。
第10行，需重点关注，构造一个Camera对象，并将它返回给app层。
### 3.1 getNumberOfCameras函数分析
getNumberOfCameras函数进入到CameraService获取Camera个数的流程如下:
![](https://img-blog.csdn.net/20160805160317552?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Camera.[Java](http://lib.csdn.net/base/java)调用的getNumberOfCameras函数是一个JNI接口，对应的函数是android_hardware_Camera.cpp里的android_hardware_Camera_getNumberOfCameras函数
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- static jint android_hardware_Camera_getNumberOfCameras(JNIEnv *env, jobject thiz)  
- {  
- return Camera::getNumberOfCameras();  
- }  
这里只是简单调用了Camera.cpp的getNumberOfCameras函数，Camera继承了CameraBase，该函数由它实现
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- template <typename TCam, typename TCamTraits>  
- int CameraBase<TCam, TCamTraits>::getNumberOfCameras() {  
- const sp<ICameraService> cs = getCameraService();  
- return cs->getNumberOfCameras();  
- }  
第3行, getCameraService函数用来获取ICameraService的Bp端，代码实现如下
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- constchar* kCameraServiceName = "media.camera";  
- 
- template <typename TCam, typename TCamTraits>  
- const sp<ICameraService>& CameraBase<TCam, TCamTraits>::getCameraService()  
- {     
- if (gCameraService.get() == 0) {  
-         sp<IServiceManager> sm = defaultServiceManager();  
-         sp<IBinder> binder;  
-         binder = sm->getService(String16(kCameraServiceName));         
-         gCameraService = interface_cast<ICameraService>(binder);  
-     }  
- return gCameraService;  
- }  
[android](http://lib.csdn.net/base/android)的Binder通讯机制
第1行, 获取的ServiceName为"media.camera"，结合上一篇博文[CameraService的启动流程](http://blog.csdn.net/shell812/article/details/49424857)可以看出Bn端的实现在CameraService.cpp
回到之前的getNumberOfCameras函数，在获取到ICameraService的Bp端后，就可以开始和Bn端通讯了。在第4行，当调用cs->getNumberOfCameras函数时，将会进入CameraService.cpp的getNumberOfCameras函数
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- int32_t CameraService::getNumberOfCameras() {  
- return mNumberOfCameras;  
- }  
代码很简单，返回上一篇博文讲到的，千辛万苦从hal层拿到的数据
### 3.2 Camera构造函数分析
回到最开始的Camera.open函数，在第10行，将会构造一个Camera对象
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- privateint cameraInitVersion(int cameraId, int halVersion) {  
-     ......  
-     Looper looper;  
- if ((looper = Looper.myLooper()) != null) {  
-         mEventHandler = new EventHandler(this, looper);  
-     } elseif ((looper = Looper.getMainLooper()) != null) {  
-         mEventHandler = new EventHandler(this, looper);  
-     } else {  
-         mEventHandler = null;  
-     }  
- 
-     String packageName = ActivityThread.currentPackageName();  
- 
- return native_setup(new WeakReference<Camera>(this), cameraId, halVersion, packageName);  
- }  
- 
- privateint cameraInitNormal(int cameraId) {  
- return cameraInitVersion(cameraId, CAMERA_HAL_API_VERSION_NORMAL_CONNECT);  
- }  
- 
- Camera(int cameraId) {  
- int err = cameraInitNormal(cameraId);  
-     ......  
- }  
第14行, native_setup同样是个JNI接口，对应android_hardware_Camera.cpp里的android_hardware_Camera_native_setup函数
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- static jint android_hardware_Camera_native_setup(JNIEnv *env, jobject thiz,  
-     jobject weak_this, jint cameraId, jint halVersion, jstring clientPackageName)  
- {  
-     camera = Camera::connect(cameraId, clientName, Camera::USE_CALLING_UID);  
- 
- #if 1 // defined(MTK_CAMERA_BSP_SUPPORT)
-     sp<JNICameraContext> context = new MtkJNICameraContext(env, weak_this, clazz, camera);  
- #else
-     sp<JNICameraContext> context = new JNICameraContext(env, weak_this, clazz, camera);  
- #endif
- }  
第4行, 调用了Camera.cpp的connect函数，同时返回一个Camera对象，保存在JNICameraContext当中
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- sp<Camera> Camera::connect(int cameraId, const String16& clientPackageName,  
- int clientUid)  
- {  
- return CameraBaseT::connect(cameraId, clientPackageName, clientUid);  
- }  
先来看看Camera和CameraBase的类定义
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- /* ---------- Camera.h ---------- */
- template <>  
- struct CameraTraits<Camera>  
- {  
-     ......  
- static TCamConnectService fnConnectService;  
- };  
- 
- class Camera : public CameraBase<Camera>  
- {  
-     ......  
- }  
- /* -------- CameraBase.h -------- */
- <pre name="code"class="cpp">template <typename TCam>  
- struct CameraTraits {  
- };  
- 
- template <typename TCam, typename TCamTraits = CameraTraits<TCam>>  
- class CameraBase  
- {  
-     ......  
- typedef CameraBase<TCam>    CameraBaseT;  
- }  
这里使用了C++模版，其实就是调用CameraBase::connect函数
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- CameraTraits<Camera>::TCamConnectService CameraTraits<Camera>::fnConnectService = &ICameraService::connect;  
- 
- template <typename TCam, typename TCamTraits>  
- sp<TCam> CameraBase<TCam, TCamTraits>::connect(int cameraId,  
- const String16& clientPackageName,  
- int clientUid)  
- {  
-     sp<TCam> c = new TCam(cameraId);  
-     sp<TCamCallbacks> cl = c;  
- 
- const sp<ICameraService>& cs = getCameraService();  
- if (cs != 0) {  
-         TCamConnectService fnConnectService = TCamTraits::fnConnectService;  
-         status = (cs.get()->*fnConnectService)(cl, cameraId, clientPackageName, clientUid,  
- /*out*/ c->mCamera);  
-     }  
- 
- return c;  
- }  
第1行, 将CameraTraits::fnConnectService赋为ICameraService::connect
第7行, 构造一个Camera对象
第10行, 获取ICameraService的Bp端
第13行, 从上面的解释可以看出，实际就是调用CameraService.cpp的connect函数
第17行, 将Camera对象返回给JNI层
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- status_t CameraService::connectHelperLocked(  
- /*out*/
-         sp<Client>& client,  
- /*in*/
- const sp<ICameraClient>& cameraClient,  
- int cameraId,  
- const String16& clientPackageName,  
- int clientUid,  
- int callingPid,  
- int halVersion,  
- bool legacyMode) {  
-     ......  
- int deviceVersion = getDeviceVersion(cameraId, &facing);  
- 
- switch(deviceVersion) {  
- case CAMERA_DEVICE_API_VERSION_1_0:  
-             client = new CameraClient(this, cameraClient,  
-                     clientPackageName, cameraId,  
-                     facing, callingPid, clientUid, getpid(), legacyMode);  
- break;  
- case CAMERA_DEVICE_API_VERSION_2_0:  
- case CAMERA_DEVICE_API_VERSION_2_1:  
- case CAMERA_DEVICE_API_VERSION_3_0:  
- case CAMERA_DEVICE_API_VERSION_3_1:  
- case CAMERA_DEVICE_API_VERSION_3_2:  
-             client = new Camera2Client(this, cameraClient,  
-                     clientPackageName, cameraId,  
-                     facing, callingPid, clientUid, getpid(), legacyMode);  
- break;  
-     }  
- 
-     status_t status = connectFinishUnsafe(client, client->getRemote());  
-     mClient[cameraId] = client;  
- }  
- 
- status_t CameraService::connect(  
- const sp<ICameraClient>& cameraClient,  
- int cameraId,  
- const String16& clientPackageName,  
- int clientUid,  
- /*out*/
-         sp<ICamera>& device) {  
-     ......  
-     sp<Client> client;  
-     status = connectHelperLocked(/*out*/client,  
-                                  cameraClient,  
-                                  cameraId,  
-                                  clientPackageName,  
-                                  clientUid,  
-                                  callingPid);  
-     device = client;  
- return OK;  
- }  
忽略细节之后connect函数就只是调用connectHelperLocked函数
第13行, 获取api版本信息，这个函数比较简单，不细说。这里的版本为CAMERA_DEVICE_API_VERSION_1_0
第15-30行, 根据不同的api版本选择构造CameraClient或Camera2Client，这里是CameraClient
第32行, 调用connectFinishUnsafe函数，实现如下
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- status_t CameraService::connectFinishUnsafe(const sp<BasicClient>& client,  
- const sp<IBinder>& remoteCallback) {  
-     status_t status = client->initialize(mModule);  
- }  
这里的client就是上一个函数的CameraClient，mModule就是在上一篇博文[CameraService的启动流程](http://blog.csdn.net/shell812/article/details/49424857)里提到的hal层的接口
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- status_t CameraClient::initialize(camera_module_t *module) {  
-     mHardware = new CameraHardwareInterface(camera_device_name);  
-     res = mHardware->initialize(&module->common);  
-     mHardware->setCallbacks(notifyCallback,  
-             dataCallback,  
-             dataCallbackTimestamp,  
-             (void *)(uintptr_t)mCameraId);  
- return OK;  
- }  
构造一个CameraHardwareInterface对象，并调用它的initalize函数，直接看initalize函数
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- status_t initialize(hw_module_t *module)  
- {  
-     module->methods->open(module, mName.string(), (hw_device_t **)&mDevice)  
-     initHalPreviewWindow();  
- }  
第4行, 从这里进入到了hal层，hal层主要对Camera硬件进行初始化，并将操作集保存在mDevice当中
# 4. hal层-基于MTK平台
hal层对Camera硬件进行初始化以及返回Device操作集的流程如下
![](https://img-blog.csdn.net/20160808180720077?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 4.1 open函数分析
这里再看一次module的定义
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- static
- hw_module_methods_t*  
- get_module_methods()  
- {  
- static
-     hw_module_methods_t  
-     _methods =  
-     {  
-         open: open_device  
-     };  
- 
- return  &_methods;  
- }  
- 
- static
- camera_module  
- get_camera_module()  
- {  
-     camera_module module = {  
-         common:{  
-              tag                    : HARDWARE_MODULE_TAG,  
-              #if (PLATFORM_SDK_VERSION >= 21)
-              module_api_version     : CAMERA_MODULE_API_VERSION_2_3,  
-              #else
-              module_api_version     : CAMERA_DEVICE_API_VERSION_1_0,  
-              #endif
-              hal_api_version        : HARDWARE_HAL_API_VERSION,  
-              id                     : CAMERA_HARDWARE_MODULE_ID,  
-              name                   : "MediaTek Camera Module",  
-              author                 : "MediaTek",  
-              methods                : get_module_methods(),  
-              dso                    : NULL,  
-              reserved               : {0},  
-         },  
-         get_number_of_cameras       : get_number_of_cameras,  
-         get_camera_info             : get_camera_info,  
-         set_callbacks               : set_callbacks,  
-         get_vendor_tag_ops          : get_vendor_tag_ops,  
-         #if (PLATFORM_SDK_VERSION >= 21)
-         open_legacy                 : open_legacy,  
-         #endif
-         reserved                    : {0},  
-     };  
- return  module;  
- };  
通过module->methods获取到的函数为open_device，
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- static
- int
- open_device(hw_module_t const* module, constchar* name, hw_device_t** device)  
- {  
- return  NSCam::getCamDeviceManager()->open(device, module, name);  
- }  
CamDeviceManagerImp 继承了 CamDeviceManagerBase。这里直接调用了CamDeviceManagerBase的open()
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- status_t  
- CamDeviceManagerBase::  
- openDeviceLocked(  
-     hw_device_t** device,  
-     hw_module_t const* module,  
-     int32_t const i4OpenId,  
-     uint32_t device_version  
- )  
- {  
- // [2] get platform
-     IPlatform*const pPlatform = getPlatform();  
-     pDevice = pPlatform->createCam1Device(s8ClientAppMode.string(), i4OpenId);  
-     *device = const_cast<hw_device_t*>(pDevice->get_hw_device());  
- }  
- 
- status_t  
- CamDeviceManagerBase::  
- open(  
-     hw_device_t** device,  
-     hw_module_t const* module,  
- charconst* name,  
-     uint32_t device_version  
- )  
- {  
- return  openDeviceLocked(device, module, i4OpenId, device_version);  
- }  
第11行, getPlatform函数用来加载libcam_platform.so，并获取PlatformEntry接口
第12行, 构造一个Cam1Device对象，并调用它的init函数
第13行, 获取camera device的操作集
### 4.2 getPlatform函数分析
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- static PlatformEntry g_PlatformEntry;  
- 
- IPlatform*  
- getHandleToPlatform()  
- {  
- return &g_PlatformEntry;  
- }  
- 
- IPlatform*  
- CamDeviceManagerBase::  
- getPlatform()  
- {  
- charconst szModulePath[] = "libcam_platform.so";  
- charconst szEntrySymbol[] = "getHandleToPlatform";  
- void* pfnEntry = NULL;  
-     IPlatform* pIPlatform = NULL;  
- 
-     mpLibPlatform = ::dlopen(szModulePath, RTLD_NOW);  
-     pfnEntry = ::dlsym(mpLibPlatform, szEntrySymbol);  
-     pIPlatform = reinterpret_cast<IPlatform*(*)()>(pfnEntry)();  
-     gIPlatform = pIPlatform;  
- 
- return gIPlatform;  
- }  
第18行, 加载libcam_platform.so
第19 20行, 获取getHandleToPlatform函数入口，并调用，最后返回PlatformEntry接口
### 4.3 pPlatform->createCam1Device函数分析
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- NSCam::Cam1Device*  
- createCam1Device(  
-     String8 const   s8ClientAppMode,  
-     int32_t const   i4OpenId  
- )  
- {  
-     NSCam::Cam1Device* pdev = NULL;  
- 
-     String8 const s8LibPath = String8::format("libcam.device1.so");  
- void *handle = ::dlopen(s8LibPath.string(), RTLD_GLOBAL);  
- 
-     String8 const s8CamDeviceInstFactory = String8::format("createCam1Device_Default");  
- void* pCreateInstance = ::dlsym(handle, s8CamDeviceInstFactory.string());  
-     pdev = reinterpret_cast<NSCam::Cam1Device* (*)(String8 const&, int32_t const)>  
-                     (pCreateInstance)(s8ClientAppMode, i4OpenId);  
- 
-     pdev->initialize();  
- }  
- 
- ICamDevice*  
- PlatformEntry::  
- createCam1Device(   
- charconst*         szClientAppMode,  
-     int32_t const       i4OpenId  
- )  
- {  
- return  ::createCam1Device(String8(szClientAppMode), i4OpenId);  
- }  
pPlatform->createCam1Device函数调用的是Cam1DeviceFactory.cpp里的createCam1Device函数
第10行, 加载libcam.device1.so
第12-15行, 获取createCam1Device_Default函数入口并调用
第17行, Cam1Device初始化
先来看createCam1Device_Default函数，以及类的继承关系
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- class Cam1DeviceBase : public Cam1Device  
- {  
- }  
- 
- class DefaultCam1Device : public Cam1DeviceBase  
- {  
- }  
- 
- NSCam::Cam1Device*  
- createCam1Device_Default(  
-     String8 const&          rDevName,   
-     int32_t const           i4OpenId  
- )  
- {  
- returnnew DefaultCam1Device(rDevName, i4OpenId);  
- }  
接着看DefaultCam1Device的构造函数
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- Cam1Device::  
- Cam1Device()  
- {     
-     ::memset(&mDevice, 0, sizeof(mDevice));  
-     mDevice.priv    = this;  
-     mDevice.common  = gHwDevice;  
-     mDevice.ops     = (camera_device_ops*)&mDeviceOps;  
-     mDeviceOps      = gCameraDevOps;  
- }    
- 
- Cam1DeviceBase::  
- Cam1DeviceBase(  
-     String8 const&          rDevName,  
-     int32_t const           i4OpenId  
- )  
-     : Cam1Device()  
-     , mDevName(rDevName)  
-     , mi4OpenId(i4OpenId)  
- {  
-     MY_LOGD("");  
- }  
- 
- DefaultCam1Device::  
- DefaultCam1Device(  
-     String8 const&          rDevName,  
-     int32_t const           i4OpenId  
- )  
-     : Cam1DeviceBase(rDevName, i4OpenId)  
- {  
- }   
删除了一些暂不关注的代码，DefaultCam1Device的构造函数会不断调用父类的构造函数，需要关注的是它的父类Cam1Device的构造函数。其中的gCameraDevOps结构体很重要，是Camera Device的操作集，预览、拍照、录像都是通过它来操作，来看下它的定义
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- static mtk_camera_device_ops const
- gCameraDevOps =  
- {  
-     #define OPS(name) name: camera_##name
- 
-     {  
-         OPS(set_preview_window),  
-         OPS(set_callbacks),   
-         OPS(enable_msg_type),  
-         OPS(disable_msg_type),  
-         OPS(msg_type_enabled),   
-         OPS(start_preview),  
-         OPS(stop_preview),   
-         OPS(preview_enabled),   
-         OPS(store_meta_data_in_buffers),  
-         OPS(start_recording),   
-         OPS(stop_recording),   
-         OPS(recording_enabled),  
-         OPS(release_recording_frame),  
-         OPS(auto_focus),   
-         OPS(cancel_auto_focus),  
-         OPS(take_picture),  
-         OPS(cancel_picture),   
-         OPS(set_parameters),   
-         OPS(get_parameters),   
-         OPS(put_parameters),   
-         OPS(send_command),  
-         OPS(release),  
-         OPS(dump)  
-     },  
-     OPS(mtk_set_callbacks),  
- 
-     #undef  OPS
- };  
回到createCam1Device函数，最后调用了pdev->initialize函数，这个函数过程比较复杂，在它的父类Cam1DeviceBase中实现
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- bool
- DefaultCam1Device::  
- onInit()  
- {  
- //  (1) power on sensor
- if( pthread_create(&mThreadHandle, NULL, doThreadInit, this) != 0 )  
-     {  
- goto lbExit;  
-     }  
- 
- //  (2) Open 3A
-     mpHal3a = NS3A::IHal3A::createInstance(  
-             NS3A::IHal3A::E_Camera_1,  
-             getOpenId(),  
-             LOG_TAG);  
- 
- //  (3) Init Base.
- if  ( ! Cam1DeviceBase::onInit() )  
-     {  
- goto lbExit;  
-     }  
- }  
- 
- status_t  
- Cam1DeviceBase::  
- initialize()  
- {  
-     onInit();  
- return  OK;    
- }  
initialize函数只是简单的回调了onInit函数，如注释所示，主要做了3件事情。其中(2)和(3)主要是初始化3A和CamClient，这两个这里暂时不会关注，所以暂时不进行分析。重点关注(1)，也就是doThreadInit函数
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- bool
- DefaultCam1Device::  
- powerOnSensor()  
- {  
-     IHalSensorList* pHalSensorList = IHalSensorList::get();  
-     mpHalSensor = pHalSensorList->createSensor(USER_NAME, getOpenId());  
-     sensorIdx = getOpenId();  
- if( !mpHalSensor->powerOn(USER_NAME, 1, &sensorIdx) )  
-     {  
-         MY_LOGE("sensor power on failed: %d", sensorIdx);  
- goto lbExit;  
-     }  
-     ......  
- }  
- 
- void*     
- DefaultCam1Device::  
- doThreadInit(void* arg)  
- {  
-     DefaultCam1Device* pSelf = reinterpret_cast<DefaultCam1Device*>(arg);  
-     pSelf->mRet = pSelf->powerOnSensor();  
-     pthread_exit(NULL);  
- return NULL;  
- }  
doThreadInit函数只是回调自身的了powerOnSensor函数，而powerOnSensor函数先调用pHalSensorList->createSensor函数创建一个HalSensor实例，然后再调用它的PowerOn函数来开始相关的硬件操作，来看powerOn的实现
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- MBOOL  
- HalSensor::  
- powerOn(  
- charconst* szCallerName,  
-     MUINT const uCountOfIndex,  
-     MUINT const*pArrayOfIndex  
- )  
- {  
-     ......  
-     ret = mpSeninfDrv->init();  
-     ret = mpSensorDrv->init(sensorDev);  
-     ret = setTgPhase(sensorDev, pcEn);  
-     ret = setSensorIODrivingCurrent(sensorDev);  
-     ret = mpSensorDrv->open(sensorDev);  
-     ......  
- }  
powerOn函数比较长，这里暂时只关注SensorDrv的init和open函数
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- MINT32  
- ImgSensorDrv::init(MINT32 sensorIdx)  
- {  
-     m_fdSensor = ::open("dev/kd_camera_hw", O_RDWR);  
- 
- //set driver
-     ret = ioctl(m_fdSensor, KDIMGSENSORIOC_X_SET_DRIVER,sensorDrvInit);  
- 
- //init resolution
-     pSensorResInfo[0] = &m_SenosrResInfo[0];  
-     pSensorResInfo[1] = &m_SenosrResInfo[1];  
-     ret = getResolution(pSensorResInfo);  
- 
- if(SENSOR_MAIN & sensorIdx ) {  
-         sensorDevId = SENSOR_MAIN;  
- 
-         FeatureParaLen = sizeof(MUINTPTR);  
-         ret = featureControl((CAMERA_DUAL_CAMERA_SENSOR_ENUM)sensorDevId, SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ,  (MUINT8*)&FeaturePara32,(MUINT32*)&FeatureParaLen);  
- 
-         FeatureParaLen = sizeof(pFeaturePara16);  
-         ret = featureControl((CAMERA_DUAL_CAMERA_SENSOR_ENUM)sensorDevId, SENSOR_FEATURE_GET_PERIOD,  (MUINT8*)pFeaturePara16,(MUINT32*)&FeatureParaLen);  
-     }  
-     ......  
- }  
- 
- MINT32  
- ImgSensorDrv::open(MINT32 sensorIdx)  
- {  
-     MINT32 err = SENSOR_NO_ERROR;  
- 
-     ......  
-     err = ioctl(m_fdSensor, KDIMGSENSORIOC_X_SET_CURRENT_SENSOR, &sensorIdx);  
-     err = ioctl(m_fdSensor, KDIMGSENSORIOC_T_OPEN);  
- 
- return err;  
- }  
这两个函数逻辑比较简单，就是通过ioctl进入到kernel层来对sensor硬件进行初始化和获取硬件相关的信息。kernel层的代码暂不分析
### 4.4 get_hw_device函数分析
回到4.1的openDeviceLocked函数，最后调用了pDevice->get_hw_device函数，并将它的返回值赋给*device
**[cpp]**[view
 plain](http://blog.csdn.net/eternity9255/article/details/52126392#)[copy](http://blog.csdn.net/eternity9255/article/details/52126392#)
- class Cam1Device : public ICamDevice  
- {  
- virtual hw_device_t const*      get_hw_device() const { return &mDevice.common; }  
- }  
这个函数很简单，就是获取4.3里面提到的mDevice，这个mDevice最终将被保存在frameworks层的CameraHardwareInterface.h的mDevice变量当中，以便日后访问
# 5. 总结
Camera打开流程的重点工作在4.3和4.4章节，也就是对Camera硬件进行初始化和将gCameraDevOps操作集返回给frameworks层。
