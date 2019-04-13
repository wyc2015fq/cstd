
# Android Camera调用流程 - 嵌入式Linux - CSDN博客

2016年02月22日 18:19:23[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：19818



## 一个流程图画的非常好的文章
[http://blog.csdn.net/lushengchu_luis/article/details/11033095](http://blog.csdn.net/lushengchu_luis/article/details/11033095)
## 1、Packages/apps/到framework 打开Camera
./packages/apps/Camera/src/com/android/camera/Camera.java
进来第一个肯定是onCreate(Bundle icicle) {
这里是开始了一个Camera的线程
```python
//start camera opening process
```
```python
mCameraStartUpThread =
```
```python
new
```
```python
CameraStartUpThread();
CameraStarUpThread是一个类
```
```python
/// M: open camera process functions @{
```
```python
private
```
```python
class
```
```python
CameraStartUpThread
```
```python
extends
```
```python
Thread
```
```python
{
```
```python
//上面的注释就写着open camera process functions 打开相机进程的函数
```
```python
Util.openCamera(Camera.
```
```python
this
```
```python
, isPIPMode(getCurrentMode()));
```
```python
//这一句就是用Util这个类来打开相机进程了
```
```python
./packages/apps/Camera/src/com/android/camera/Util.java
```
```python
//这里面还有打开摄像头的函数openCamera
```
```python
public
```
```python
static
```
```python
void
```
```python
openCamera
```
```python
(Activity activity,
```
```python
boolean
```
```python
isPIP)
```
```python
throws
```
```python
CameraHardwareException, CameraDisabledException {
        Log.i(TAG,
```
```python
"openCamera begin isPIP = "
```
```python
+ isPIP);
```
```python
// Check if device policy has disabled the camera.
```
```python
DevicePolicyManager dpm = (DevicePolicyManager) activity.getSystemService(
                Context.DEVICE_POLICY_SERVICE);
```
```python
if
```
```python
(dpm.getCameraDisabled(
```
```python
null
```
```python
)) {
```
```python
throw
```
```python
new
```
```python
CameraDisabledException();
        }
```
```python
if
```
```python
(isPIP) {
            retryOpen(activity, OPEN_RETRY_COUNT, CameraHolder.instance().getBackCameraId());
            retryOpen(activity, OPEN_RETRY_COUNT, CameraHolder.instance().getFrontCameraId());
        }
```
```python
else
```
```python
{
```
```python
int
```
```python
currentCameraId = ((com.android.camera.Camera)activity).getCameraId();
            retryOpen(activity, OPEN_RETRY_COUNT, currentCameraId);
        }
        Log.i(TAG,
```
```python
"openCamera end"
```
```python
);
    }
```
```python
//这里面又会调用一个retryOpen函数
```
```python
private
```
```python
static
```
```python
CameraManager.CameraProxy
```
```python
retryOpen
```
```python
(Activity activity,
```
```python
int
```
```python
count,
```
```python
int
```
```python
cameraId) 
    //这个函数最终会调用open函数来打开摄像头
```
```python
try
```
```python
{
```
```python
if
```
```python
(activity
```
```python
instanceof
```
```python
ActivityBase) {
                    CameraProxy cameraProxy = CameraHolder.instance().open(cameraId);
```
```python
return
```
```python
cameraProxy;
                }
```
```python
else
```
```python
{
```
```python
return
```
```python
CameraHolder.instance().open(cameraId);
                }
     ........
     }
```
２. 这里面的CameraHolder.instance().open调用了下面的open函数
./packages/apps/Camera/src/com/android/camera/CameraHolder.java
```python
public
```
```python
CameraProxy
```
```python
open
```
```python
(
```
```python
int
```
```python
cameraId)
```
```python
throws
```
```python
CameraHardwareException {
        Log.i(TAG,
```
```python
"CameraHolder open cameraId = "
```
```python
+ cameraId);
        assertError(cameraId != UNKONW_ID);
```
```python
if
```
```python
(mMockCameraInfo ==
```
```python
null
```
```python
) {
```
```python
//这里又有一个open函数
```
```python
return
```
```python
getCameraProxyWrapper(cameraId).open();
        }
```
```python
else
```
```python
{
```
```python
if
```
```python
(mMockCamera ==
```
```python
null
```
```python
) {
```
```python
throw
```
```python
new
```
```python
RuntimeException();
            }
            getCameraProxyWrapper(cameraId).insertMockCameraProxy(mMockCamera[cameraId]);
```
```python
return
```
```python
mMockCamera[cameraId];
        }
    }
```
```python
public
```
```python
synchronized
```
```python
CameraProxy
```
```python
open
```
```python
()
```
```python
throws
```
```python
CameraHardwareException {
            Log.i(TAG,
```
```python
"CameraProxyWrapper open mCameraOpened = "
```
```python
+ mCameraOpened +
```
```python
" mCameraId = "
```
```python
+ mCameraId);
            assertError(!mCameraOpened);
```
```python
if
```
```python
(mCameraProxy ==
```
```python
null
```
```python
) {
```
```python
try
```
```python
{
                    Log.i(TAG,
```
```python
"open camera "
```
```python
+ mCameraId);
                    mCameraProxy = mCameraManager.cameraOpen(mCameraId);
```
```python
//现在又跑到一个mCameraManager.cameraOpen里面来了
```
```python
//看一下下面这个函数，这个函数就是app里面最终调用到framework里面的接口
```
```python
// Open camera synchronously. This method is invoked in the context of a
```
```python
// background thread.
```
```python
CameraProxy cameraOpen(
```
```python
int
```
```python
cameraId) {
```
```python
// Cannot open camera in mCameraHandler, otherwise all camera events
```
```python
// will be routed to mCameraHandler looper, which in turn will call
```
```python
// event handler like Camera.onFaceDetection, which in turn will modify
```
```python
// UI and cause exception like this:
```
```python
// CalledFromWrongThreadException: Only the original thread that created
```
```python
// a view hierarchy can touch its views.
```
```python
MMProfileManager.startProfileCameraOpen();
        mCamera = FrameworksClassFactory.openCamera(cameraId);
        MMProfileManager.stopProfileCameraOpen();
```
```python
if
```
```python
(mCamera !=
```
```python
null
```
```python
) {
            mParametersIsDirty =
```
```python
true
```
```python
;
```
```python
if
```
```python
(mParamsToSet ==
```
```python
null
```
```python
) {
                mParamsToSet = mCamera.getParameters();
            }
            mCameraProxy =
```
```python
new
```
```python
CameraProxy();
```
```python
return
```
```python
mCameraProxy;
        }
```
```python
else
```
```python
{
```
```python
return
```
```python
null
```
```python
;
        }
```
```python
//./packages/apps/Camera/src/com/mediatek/camera/FrameworksClassFactory.java
```
```python
//代码的路径　camera.open这里就是跑到framework里面去了
```
```python
public
```
```python
static
```
```python
ICamera
```
```python
openCamera
```
```python
(
```
```python
int
```
```python
cameraId) {
```
```python
if
```
```python
(MOCK_CAMERA) {
```
```python
return
```
```python
MockCamera.open(cameraId);
        }
```
```python
else
```
```python
{
            Camera camera = Camera.open(cameraId);
```
```python
if
```
```python
(
```
```python
null
```
```python
== camera) {
                Log.e(TAG,
```
```python
"openCamera:got null hardware camera!"
```
```python
);
```
```python
return
```
```python
null
```
```python
;
            }
```
```python
// wrap it with ICamera
```
```python
return
```
```python
new
```
```python
AndroidCamera(camera);
        }
    }
```
## ２、Framework　open到jni调用流程
还是open这个接口
代码路径：./frameworks/base/core/java/android/hardware/Camera.java
```python
public
```
```python
static
```
```python
Camera
```
```python
open
```
```python
(
```
```python
int
```
```python
cameraId) {
```
```python
if
```
```python
(!isPermissionGranted()) {
```
```python
return
```
```python
null
```
```python
;
        }
```
```python
return
```
```python
new
```
```python
Camera(cameraId);
    }
```
```python
//这里的return new Camera就是在下面进行了初始化
```
```python
//填充一些参数
```
```python
Camera(
```
```python
int
```
```python
cameraId) {
        mShutterCallback =
```
```python
null
```
```python
;
        mRawImageCallback =
```
```python
null
```
```python
;
        mJpegCallback =
```
```python
null
```
```python
;
        mPreviewCallback =
```
```python
null
```
```python
;
        mPreviewRawDumpCallback =
```
```python
null
```
```python
;
        mPostviewCallback =
```
```python
null
```
```python
;
        mUsingPreviewAllocation =
```
```python
false
```
```python
;
        mZoomListener =
```
```python
null
```
```python
;
        Looper looper;
```
```python
if
```
```python
((looper = Looper.myLooper()) !=
```
```python
null
```
```python
) {
            mEventHandler =
```
```python
new
```
```python
EventHandler(
```
```python
this
```
```python
, looper);
        }
```
```python
else
```
```python
if
```
```python
((looper = Looper.getMainLooper()) !=
```
```python
null
```
```python
) {
            mEventHandler =
```
```python
new
```
```python
EventHandler(
```
```python
this
```
```python
, looper);
        }
```
```python
else
```
```python
{
            mEventHandler =
```
```python
null
```
```python
;
        }
        String packageName = ActivityThread.currentPackageName();
        native_setup(
```
```python
new
```
```python
WeakReference<Camera>(
```
```python
this
```
```python
), cameraId, packageName);
    }
```
```python
//这个native_setup就是打开摄像头的关键了
```
jni的调用接口
看到这个native 函数说明这个函数是在jni接口，用的是c++代码实现的，so，我们继续找
private native final void native_setup(Object camera_this, int cameraId,
终于我们在./frameworks/base/core/jni/android_hardware_Camera.cpp
这里面找到了这个函数，然后我们看一下这个函数
staticJNINativeMethod camMethods[] = {
  {"getNumberOfCameras","()I",
    (void*)android_hardware_Camera_getNumberOfCameras },
  {"_getCameraInfo","(ILandroid/hardware/Camera$CameraInfo;)V",
    (void*)android_hardware_Camera_getCameraInfo },
  {"native_setup","(Ljava/lang/Object;ILjava/lang/String;)V",
    (void*)android_hardware_Camera_native_setup },//从这里可以看出来，实现这个函数的真正函数是android_hardware_Camera_native_setup//所以我们继续找一下这个函数在哪里{"native_release","()V",３. 下面就是打开摄像头的jni代码，app调用open的时候就会调用到这个位置
// connect to camera servicestaticvoidandroid_hardware_Camera_native_setup(JNIEnv*env, jobject thiz,
    jobject weak_this, jint cameraId, jstring clientPackageName)
{// Convert jstring to String16const char16_t*rawClientName=env->GetStringChars(clientPackageName,NULL);
    jsize rawClientNameLen=env->GetStringLength(clientPackageName);
    String16 clientName(rawClientName, rawClientNameLen);
    env->ReleaseStringChars(clientPackageName, rawClientName);
    sp<Camera>camera=Camera::connect(cameraId, clientName,
            Camera::USE_CALLING_UID);//不相关的代码就不加进来了//sp<Camera> camera = Camera::connect(cameraId, clientName,//这句代码应该是从camera jni连接到camera client的关键   上面有个clientName也说明了这点.......}３、Android camera Client和camera service调用流程
我只知道spCamera>是一个强指针引用，具体是多厉害百度一下更好
从client的代码里面
frameworks/av/camera/Camera.cpp
sp<Camera>Camera::connect(int cameraId, const String16&clientPackageName,
        int clientUid)
{returnCameraBaseT::connect(cameraId, clientPackageName, clientUid);//CameraBaseT::connect这个又是从哪里来的，而且都是c++这可是难倒了我//不太好办}//在./frameworks/av/include/camera/CameraBase.h:113://typedef CameraBase<TCam>         CameraBaseT;//里面有一个typedef 好了，那我们下一句就应该去找CameraBase<TCam>这个东西//找到在这个位置//./frameworks/av/camera/CameraBase.cpp:94:sp<TCam> CameraBase<TCam, TCamTraits>::connect(int cameraId,sp<TCam>CameraBase<TCam, TCamTraits>::connect(int cameraId,
                                               const String16&clientPackageName,
                                               int clientUid)
{
    ALOGV("%s: connect", __FUNCTION__);
    sp<TCam>c=newTCam(cameraId);
    sp<TCamCallbacks>cl=c;
    status_t status=NO_ERROR;//下一步就应该到getCameraService这里来了//这时候我们就要到cameraservice.java里面去看代码了const sp<ICameraService>&cs=getCameraService();if(cs!=0) {
        TCamConnectService fnConnectService=TCamTraits::fnConnectService;
        status=(cs.get()->*fnConnectService)(cl, cameraId, clientPackageName, clientUid,/*out*/c->mCamera);
    }if(status==OK&&c->mCamera!=0) {
        c->mCamera->asBinder()->linkToDeath(c);
        c->mStatus=NO_ERROR;
    }else{
        ALOGW("An error occurred while connecting to camera: %d", cameraId);
        c.clear();
    }returnc;
}为了查看它的调用流程，我加了一点日志在connect函数里面，加的日志代码如下
template <typename TCam, typename TCamTraits>
sp<TCam> CameraBase<TCam, TCamTraits>::connect(int cameraId,constString16& clientPackageName,
                                               int clientUid)
{
    ALOGD("%s: weiqifa connect 23", __FUNCTION__);
    sp<TCam> c =newTCam(cameraId);
    sp<TCamCallbacks> cl = c;
    status_t status = NO_ERROR;constsp<ICameraService>& cs = getCameraService();if(cs !=0) {
        TCamConnectService fnConnectService =TCamTraits::fnConnectService;status=(cs.get()->*fnConnectService)(cl, cameraId, clientPackageName, clientUid,/*out*/c->mCamera);ALOGD("%s: weiqifa connect 234", __FUNCTION__);
    //ALOGD("cs.get()->*fnConnectService %s: weiqifawe", *(cs.get()->*fnConnectService));
 }if(status == OK && c->mCamera !=0){c->mCamera->asBinder()->linkToDeath(c);
        c->mStatus = NO_ERROR;
    }else{
        ALOGW("An error occurred while connecting to camera: %d", cameraId);
        c.clear();
    }returnc;
}![这里写图片描述](https://img-blog.csdn.net/20151120172750573)[ ](https://img-blog.csdn.net/20151120172750573)
我们可以看到CameraBase: connect: weiqifa connect 234这句话已经打印出来了
然后我们也可以看到CameraService::connect X 打印出来了
然后我们也可以看到CameraClient: CameraClient::initialize X这个也打印出来了
上面的三个日志还是非常重要的
看一下cameraservice.cpp的connect函数，service是服务端，client是客户端，那么客户端的实现在服务端应该有一个对应的函数
status_t CameraService::connect(constsp<ICameraClient>& cameraClient,intcameraId,constString16& clientPackageName,intclientUid,
        .....switch(deviceVersion) {caseCAMERA_DEVICE_API_VERSION_1_0:
            client =newCameraClient(this, cameraClient,//new CameraClient 这个new了一个这个东西，那么CameraClient里面的东西就会被调用了clientPackageName, cameraId,
                    facing, callingPid, clientUid, getpid());./frameworks/av/services/camera/libcameraservice/api1/CameraClient.cpp:73:
status_t CameraClient::initialize(camera_module_t *module) {intcallingPid = getCallingPid();
    status_t res;////看到这里的时候我突然看到了open的函数//但是有个问题很奇怪这个函数是谁调用了呢？谁让他运行的？？？？？？LOG1("CameraClient::initialize E (pid %d, id %d)", callingPid, mCameraId);
    res = mHardware->initialize(&module->common);//这句还是比较关键的mHardware->initialize就是要获取硬件设备的一些东东啊//......LOG1("CameraClient::initialize X (pid %d, id %d)", callingPid, mCameraId);returnOK;./frameworks/av/services/camera/libcameraservice/device1/CameraHardwareInterface.h
status_t initialize(hw_module_t *module)
    {
        ALOGI("Opening camera %s", mName.string());//通过这几句话去打开设备intrc =module->methods->open(module, mName.string(),
                                       (hw_device_t **)&mDevice);if(rc != OK) {
            ALOGE("Could not open camera %s: %d", mName.string(), rc);returnrc;
        }
        initHalPreviewWindow();returnrc;
    }//日志里面也有这样的打印01-0122:23:00.999868191748I CameraClient: Opening camera0我们下一步就是要去找到hw_module_t这个结构体，找到了这个结构体就可以往下走
libcameraservice.so里面调用
voidCameraService::onFirstRef()
{
    LOG1("CameraService::onFirstRef weiqifa");
    BnCameraService::onFirstRef();if(hw_get_module(CAMERA_HARDWARE_MODULE_ID,
                (const hw_module_t**)&mModule)<0) {
        ALOGE("Could not load camera HAL module");
        mNumberOfCameras=0;
    }else{
        ALOGI("Loaded \"%s\" camera module", mModule->common.name);
        mNumberOfCameras=mModule->get_number_of_cameras();if(mNumberOfCameras>MAX_CAMERAS) {
            ALOGE("Number of cameras(%d) > MAX_CAMERAS(%d).",
                    mNumberOfCameras, MAX_CAMERAS);
            mNumberOfCameras=MAX_CAMERAS;
        }//for (int i = 0; i < mNumberOfCameras; i++) {for (int i=0; i<MAX_CAMERAS; i++) {// workaround for MATVLOG1("setCameraFree(%d)", i);
            setCameraFree(i);
        }if(mModule->common.module_api_version>=CAMERA_MODULE_API_VERSION_2_1) {
            mModule->set_callbacks(this);
        }
        CameraDeviceFactory::registerService(this);
    }
}hw_get_module(CAMERA_HARDWARE_MODULE_ID这个是加载硬件hardware层对应的东东，具体可以查看链接[hw_get_module获取硬件模块](http://blog.csdn.net/flydream0/article/details/7086273)
看一下CAMERA_HARDWARE_MODULE_ID
hardware/libhardware/include/hardware/camera_common.h:35:\#define CAMERA_HARDWARE_MODULE_ID"camera"看一下我们打印出来的日志
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/log$grep -wrn"CameraService::onFirstRef"./
./mobilelog/APLog_2010_0105_214945/main_log.boot:922:01-0522:49:08.208839194194DCameraService:CameraService::onFirstRef weiqifa
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/log$４、Android camera Hardware调用
hardware肯定是属于平台的特性，所以我们这里就要找到平台特性的东西了。
从libcameraservice下来
if(hw_get_module(CAMERA_HARDWARE_MODULE_ID,
                (consthw_module_t **)&mModule) <0) {
        ALOGE("Could not load camera HAL module");
        mNumberOfCameras =0;
    }hw_get_module这个东西就是获取hardware下面的内容的东西。具体的可以看一下链接
[hw_get_module](http://www.open-open.com/lib/view/open1325474381453.html)
然后我们就找到了这个地方
./mediatek/hardware/mtkcam/module/module.h
static
camera_moduleget_camera_module()
{
    camera_modulemodule = {common:{tag:HARDWARE_MODULE_TAG,module_api_version:CAMERA_MODULE_API_VERSION_2_1,hal_api_version:HARDWARE_HAL_API_VERSION,id:CAMERA_HARDWARE_MODULE_ID,name:"MediaTek Camera Module",author:"MediaTek",methods:get_module_methods(),dso:NULL,reserved:{0}, 
        },get_number_of_cameras:get_number_of_cameras,get_camera_info:get_camera_info,set_callbacks:set_callbacks, 
    };returnmodule;}里面的methods: get_module_methods（）这个就是我们hardware的open函数
open_device(hw_module_tconst*module,constchar* name, hw_device_t** device)
{returnNSCam::getCamDeviceManager()->open(module, name, device);
}statichw_module_methods_t*
get_module_methods()
{statichw_module_methods_t
    _methods =
    {
        open: open_device
    };return&_methods;
}open_device就会找到NScam：：getCamDeviceManager()->open
NSCam这个是一个命名空间，我看了网上的解释，更像是类引用，后面我再跟进去找到open的地方
./mediatek/hardware/mtkcam/devicemgr/CamDeviceManagerBase.cpp
status_t
CamDeviceManagerBase::
open(
    hw_module_tconst*module,charconst* name, 
    hw_device_t** device
)
{
    RWLock::AutoWLock _l(mRWLock);//returnopenDeviceLocked(module, name, device);
}这里调用到了openDeviceLocked　我们找到这个地方
./mediatek/hardware/mtkcam/devicemgr/CamDeviceManagerBase.openDevice.cpp
/******************************************************************************
 *
 ******************************************************************************/status_t
CamDeviceManagerBase::
openDeviceLocked(
    hw_module_tconst* module,charconst* name, 
    hw_device_t** device
)
{
    status_t status = OK;
    sp<ICamDevice> pDevice = NULL;
    int32_tconsti4OpenId = (name != NULL) ? ::atoi(name) : -1;//String8consts8ClientAppMode = queryClientAppMode();
    uint32_tconstversion = determineOpenDeviceVersionLocked(s8ClientAppMode, i4OpenId);//MY_LOGD("+ mOpenMap.size:%d mEnumMap.size:%d", mOpenMap.size(), mEnumMap.size());////  [1] check to see whether it's ready to open.if( OK != (status = validateOpenLocked(i4OpenId)) )
    {returnstatus;
    }////  [2] get platformIPlatform*constpPlatform = getPlatform();if( ! pPlatform )
    {
        MY_LOGE("No Platform");returnNAME_NOT_FOUND;
    }////  [3] create device based on device version.if( version == CAMERA_DEVICE_API_VERSION_1_0 )
    {
        pDevice = pPlatform->createCam1Device(s8ClientAppMode.string(), i4OpenId);
    }elseif( version >= CAMERA_DEVICE_API_VERSION_3_0 )
    {
        MY_LOGE("Unsupported version:0x%x >= CAMERA_DEVICE_API_VERSION_3_0", version);returnUNKNOWN_ERROR;
    }else{
        MY_LOGE("Unsupported version:0x%x", version);returnUNKNOWN_ERROR;
    }//if( pDevice ==0)
    {
        MY_LOGE("device creation failure");returnNO_MEMORY;
    }////  [4] open device successfully.{
        *device =const_cast<hw_device_t*>(pDevice->get_hw_device());//pDevice->set_hw_module(module);
        pDevice->set_module_callbacks(mpModuleCallbacks);
        pDevice->setDeviceManager(this);//attachDeviceLocked(pDevice);
    }//returnOK;
}

