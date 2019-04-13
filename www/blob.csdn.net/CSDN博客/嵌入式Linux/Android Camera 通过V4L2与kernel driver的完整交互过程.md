
# Android Camera 通过V4L2与kernel driver的完整交互过程 - 嵌入式Linux - CSDN博客

2015年05月04日 11:00:25[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：786


Android Camera 通过V4L2与kernel driver的完整交互过程
之前在Android
 Camera 的执行流程http://blog.chinaunix.net/uid-26765074-id-3499537.html这篇文章中已经详细介绍了
Android Cameraapp到调用open打开camera
 设备的完成过程，但是只是到此为止，并未对较底层的操作控制等做任何分析，
今天就要进入这个环节了
![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/30.gif)![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/30.gif)
这里就直接从open那个方法开始了，先说一下这个open是怎么被调用的
实例化Camera Hal接口 hardware，hardware调用initialize()进入HAL层打开Camear驱动。
CameraHardwareInterface中initialize()定义在以下路径:frameworks/base/services/camera/libcameraservice/CameraHardwareInterface.h
代码如下：
status_t initialize(hw_module_t *module)
{
LOGI("Opening camera %s", mName.string());
int
 rc = module->methods->open(module, mName.string(),(hw_device_t **)&mDevice);
if (rc != OK) {
LOGE("Could not open camera %s: %d", mName.string(), rc);
return rc;
}
\#ifdef OMAP_ENHANCEMENT_CPCAM
initHalPreviewWindow(&mHalPreviewWindow);
initHalPreviewWindow(&mHalTapin);
initHalPreviewWindow(&mHalTapout);
\#else
initHalPreviewWindow();
\#endif
return rc;
}
此处通过module->method->open()方法真正打开Camera设备,
其中module的定义在以下路径:frameworks/base/services/camera/libcameraservice/CameraService.h
class CameraService :
public BinderService<CameraService>,
public BnCameraService
{
class Client : public BnCamera
{
public:
......private:
.....
};
camera_module_t
 *mModule;
};
此处还必须找到camera_module_t的定义,以更好的理解整个运行流程,通过追根溯源找到了camera_module_t定义,
camera_module_t的定义在以下路径:hardware/libhardware/include/hardware/camera.h
typedef struct camera_module {
hw_module_t common;
int (*get_number_of_cameras)(void);
int (*get_camera_info)(int camera_id, struct camera_info *info);
} camera_module_t;
其中包含get_number_of_cameras方法和get_camera_info方法用于获取camera
 info
另外hw_module_t common;这个选项十分重要,此处应重点关注,因为是使用hw_module_t结构体中的open()方法打开设备文件的
继续找到hw_module_t
 结构体的定义.在以下路径:hardware/libhardware/include/hardware/hardware.h

typedef struct hw_module_t{
/**tag
 must be initializedtoHARDWARE_MODULE_TAG*/
uint32_t tag;

/**major
 version numberforthe module*/
uint16_t version_major;

/**minor
 version number of the module*/
uint16_t version_minor;

/**Identifier
 of module*/
constchar*id;

/**Name
 of this module*/
constchar*name;

/**Author/owner/implementor
 of the module*/
constchar*author;

/**Modules
 methods*/
struct hw_module_methods_t*methods;

/**module's
 dso*/
void*dso;

/**paddingto128
 bytes,reservedforfuture
 use*/
uint32_t reserved[32-7];

}hw_module_t;
typedef struct hw_module_methods_t{
/**Open
 a specific device*/
int(*open)(conststruct
 hw_module_t*module,constchar*id,
struct hw_device_t**device);

}hw_module_methods_t;
这里的这个open方法才是我们真正调用的open方法，那么这里只是定义，他又是在哪里实现的呢？？
这个问题我们还是需要往前面追溯啊，当然这在上一篇文章中也没有提到，不卖关子了，其实重点就是我们调用了这个module->methods->open(module,
 mName.string(),(hw_device_t **)&mDevice)方法
但是这个module是在哪里初始化的呢？看看CameraService类里的这个方法吧，路径：frameworks/base/services/camera/libcameraservice/CameraService.cpp
void CameraService::onFirstRef()
{
BnCameraService::onFirstRef();

if(hw_get_module(CAMERA_HARDWARE_MODULE_ID,
(consthw_module_t**)&mModule)<0){
LOGE("Could not load camera HAL module");
mNumberOfCameras=0;
}
else{
mNumberOfCameras=mModule->get_number_of_cameras();
if(mNumberOfCameras>MAX_CAMERAS){
LOGE("Number of cameras(%d) > MAX_CAMERAS(%d).",
mNumberOfCameras,MAX_CAMERAS);
mNumberOfCameras=MAX_CAMERAS;
}
for(inti=0;i<mNumberOfCameras;i++){
setCameraFree(i);
}
}
}
不错，这个module正是通过这个hw_get_module方法获得的，其实他是通过方法中的CAMERA_HARDWARE_MODULE_ID作为flag最终找到已经定义好的module，那么这个已经定义好的module又在哪呢，是什么样子的呢？
这里我就直接放在这里，不在拐弯抹角了，方法路径：hardware/ti/omap4xxx/camera/CameraHal_Module.cpp
staticintcamera_device_open(consthw_module_t*module,constchar*name,
hw_device_t**device);
staticintcamera_device_close(hw_device_t*device);
staticintcamera_get_number_of_cameras(void);
staticintcamera_get_camera_info(intcamera_id,struct
 camera_info*info);

static struct hw_module_methods_t camera_module_methods={
open:camera_device_open
};

camera_module_t HAL_MODULE_INFO_SYM={
common:{
tag:HARDWARE_MODULE_TAG,
version_major:1,
version_minor:0,
id:CAMERA_HARDWARE_MODULE_ID,
name:"TI OMAP CameraHal Module",
author:"TI",
methods:&camera_module_methods,
dso:NULL,/*remove compilation warnings*/
reserved:{0},/*remove compilation warnings*/
},
get_number_of_cameras:camera_get_number_of_cameras,
get_camera_info:camera_get_camera_info,
};
这里还是很关键的，通过id：CAMERA_HARDWARE_MODULE_ID作为识别码找到这个module，get
 module完成任务，大家可以看到，这个定义好的module实现了methods中的open方法，
实现了camera_get_number_of_cameras和camera_get_camera_info，当然还包括了其他一些变量的初始化
这里开始我们找到了我们真正需要的open方法，万里长征走完一大步了，现在就去看看这个open方法干了些什么吧
/*open device handletoone
 of the cameras
*
*assume camera service will keep singleton ofeachcamera
*so thisfunctionwill always only be called once per camera instance
*/

intcamera_device_open(consthw_module_t*module,constchar*name,
hw_device_t**device)
{
intrv=0;
intnum_cameras=0;
intcameraid;
ti_camera_device_t*camera_device=NULL;
camera_device_ops_t*camera_ops=NULL;
android::CameraHal*camera=NULL;
android::CameraProperties::Properties*properties=NULL;

android::Mutex::Autolock lock(gCameraHalDeviceLock);

CAMHAL_LOGI("camera_device open");

if(name!=NULL){
cameraid=atoi(name);
num_cameras=gCameraProperties.camerasSupported();

if(cameraid>num_cameras)
{
LOGE("camera service provided cameraid out of bounds, "
"cameraid = %d, num supported = %d",
cameraid,num_cameras);
rv=-EINVAL;
goto fail;
}

if(gCamerasOpen>=MAX_SIMUL_CAMERAS_SUPPORTED)
{
LOGE("maximum number of cameras already open");
rv=-ENOMEM;
goto fail;
}

camera_device=(ti_camera_device_t*)malloc(sizeof(*camera_device));
if(!camera_device)
{
LOGE("camera_device allocation fail");
rv=-ENOMEM;
goto fail;
}

camera_ops=(camera_device_ops_t*)malloc(sizeof(*camera_ops));
if(!camera_ops)
{
LOGE("camera_ops allocation fail");
rv=-ENOMEM;
goto fail;
}

memset(camera_device,0,sizeof(*camera_device));
memset(camera_ops,0,sizeof(*camera_ops));

camera_device->base.common.tag=HARDWARE_DEVICE_TAG;
camera_device->base.common.version=0;
camera_device->base.common.module=(hw_module_t*)(module);
camera_device->base.common.close=camera_device_close;
camera_device->base.ops=camera_ops;

camera_ops->set_preview_window=camera_set_preview_window;
\#ifdef OMAP_ENHANCEMENT_CPCAM
camera_ops->set_buffer_source=camera_set_buffer_source;
\#endif
camera_ops->set_callbacks=camera_set_callbacks;
camera_ops->enable_msg_type=camera_enable_msg_type;
camera_ops->disable_msg_type=camera_disable_msg_type;
camera_ops->msg_type_enabled=camera_msg_type_enabled;
camera_ops->start_preview=camera_start_preview;
camera_ops->stop_preview=camera_stop_preview;
camera_ops->preview_enabled=camera_preview_enabled;
camera_ops->store_meta_data_in_buffers=camera_store_meta_data_in_buffers;
camera_ops->start_recording=camera_start_recording;
camera_ops->stop_recording=camera_stop_recording;
camera_ops->recording_enabled=camera_recording_enabled;
camera_ops->release_recording_frame=camera_release_recording_frame;
camera_ops->auto_focus=camera_auto_focus;
camera_ops->cancel_auto_focus=camera_cancel_auto_focus;
camera_ops->take_picture=camera_take_picture;
camera_ops->cancel_picture=camera_cancel_picture;
camera_ops->set_parameters=camera_set_parameters;
camera_ops->get_parameters=camera_get_parameters;
camera_ops->put_parameters=camera_put_parameters;
camera_ops->send_command=camera_send_command;
camera_ops->release=camera_release;
camera_ops->dump=camera_dump;
\#ifdef OMAP_ENHANCEMENT_CPCAM
camera_ops->reprocess=camera_reprocess;
camera_ops->cancel_reprocess=camera_cancel_reprocess;
\#endif

*device=&camera_device->base.common;

//--------TI specific stuff--------

camera_device->cameraid=cameraid;

if(gCameraProperties.getProperties(cameraid,&properties)<0)
{
LOGE("Couldn't get camera properties");
rv=-ENOMEM;
goto fail;
}

camera=new
 android::CameraHal(cameraid);

if(!camera)
{
LOGE("Couldn't create instance of CameraHal class");
rv=-ENOMEM;
goto fail;
}

if(properties&&(camera->initialize(properties)!=android::NO_ERROR))
{
LOGE("Couldn't initialize camera instance");
rv=-ENODEV;
goto fail;
}

gCameraHals[cameraid]=camera;
gCamerasOpen++;
}

return rv;

fail:
if(camera_device){
free(camera_device);
camera_device=NULL;
}
if(camera_ops){
free(camera_ops);
camera_ops=NULL;
}
if(camera){
delete camera;
camera=NULL;
}
*device=NULL;
return rv;
}
看看这么长的代码，open的任务还是比较中的，没办法，能者多劳嘛，红色部分是最重点的部分
从这里可以知道，这里就像一个控制中心，上传调用到这里被分发出去，实现各自的操作，我们就以startPreview为例进行分析
intcamera_start_preview(struct camera_device*device)
{
intrv=-EINVAL;
ti_camera_device_t*ti_dev=NULL;

LOGV("%s",__FUNCTION__);

if(!device)
return rv;

ti_dev=(ti_camera_device_t*)device;

rv=gCameraHals[ti_dev->cameraid]->startPreview();

return rv;
}
这里每open一个device就会相应的创建并且初始化一个CameraHal 对象，定义在：hardware/ti/omap4xxx/camera/CameraHal.cpp
并且把这个对象保存在gCameraHals这个数组中，正因为这样这里camera_start_preview才可以通过这个数据检索对象调用方法
现在我们就看看这个startPreview()方法是怎样实现的
/**
@brief Start preview mode.

@param none
@return NO_ERROR Camera switchedtoVF mode
@todo Updatefunctionheader with the different errors that are possible

*/
status_t CameraHal::startPreview(){
LOG_FUNCTION_NAME;

//When tunnelingisenabled during VTC,startPreview happensin2 steps:
//When the application sends the command CAMERA_CMD_PREVIEW_INITIALIZATION,
//cameraPreviewInitialization()iscalled,whichinturn causes the CameraAdapter
//tomove from loadedtoidle state.Andwhen the application calls startPreview,
//the CameraAdapter moves from idletoexecuting state.
//
//Ifthe application calls startPreview()without sending the command
//CAMERA_CMD_PREVIEW_INITIALIZATION,thenthefunctioncameraPreviewInitialization()
//ANDstartPreview()are executed.Inother words,ifthe application calls
//startPreview()without sending the command CAMERA_CMD_PREVIEW_INITIALIZATION,
//thenthe CameraAdapter moves from loadedtoidletoexecuting stateinone shot.
status_t ret=cameraPreviewInitialization();

//The flag mPreviewInitializationDoneissettotrueat theendof thefunction
//cameraPreviewInitialization().Therefore,ifeverything goes alright,thenthe
//flag will beset.Sometimes,thefunctioncameraPreviewInitialization()may
//return prematurelyifall the resources arenotavailableforstarting preview.
//Forexample,ifthe previewwindowisnotset,thenit would return NO_ERROR.
//Under such circumstances,one should return from startPreview as wellandshould
//notcontinue execution.Thatiswhy,we check the flagandnotthe return value.
if(!mPreviewInitializationDone)return ret;

//Once startPreviewiscalled,thereisno needtocontinuetoremember whether
//thefunctioncameraPreviewInitialization()was called earlierornot.Andso
//the flag mPreviewInitializationDoneisreset here.Plus,this preserves the
//current behavior of startPreview under the circumstances where the application
//calls startPreview twiceormore.
mPreviewInitializationDone=false;

///Enable the display adapterifpresent,actual overlay enable happens when we post the buffer
if(mDisplayAdapter.get()!=NULL){
CAMHAL_LOGDA("Enabling display");
intwidth,height;
mParameters.getPreviewSize(&width,&height);

\#ifPPM_INSTRUMENTATION||PPM_INSTRUMENTATION_ABS
ret=mDisplayAdapter->enableDisplay(width,height,&mStartPreview);
\#else
ret=mDisplayAdapter->enableDisplay(width,height,NULL);
\#endif

if(ret!=NO_ERROR){
CAMHAL_LOGEA("Couldn't enable display");

//FIXME:At this stage mStateSwitchLockislockedandunlockissupposedtobe called
//only from mCameraAdapter->sendCommand(CameraAdapter::CAMERA_START_PREVIEW)
//below.But this will never happen because of gotoerror.Thus atnext
//startPreview()callCameraHAL will be deadlocked.
//Needtorevisit mStateSwitch lock,fornowjust abort the process.
CAMHAL_ASSERT_X(false,
"At this stage mCameraAdapter->mStateSwitchLock is still locked, "
"deadlock is guaranteed");

gotoerror;
}

}

///Send START_PREVIEW commandtoadapter
CAMHAL_LOGDA("Starting CameraAdapter preview mode");

ret=mCameraAdapter->sendCommand(CameraAdapter::CAMERA_START_PREVIEW);

if(ret!=NO_ERROR){
CAMHAL_LOGEA("Couldn't start preview w/ CameraAdapter");
gotoerror;
}
CAMHAL_LOGDA("Started preview");

mPreviewEnabled=true;
mPreviewStartInProgress=false;
return ret;

error:

CAMHAL_LOGEA("Performing cleanup after error");

//Doall the cleanup
freePreviewBufs();
mCameraAdapter->sendCommand(CameraAdapter::CAMERA_STOP_PREVIEW);
if(mDisplayAdapter.get()!=NULL){
mDisplayAdapter->disableDisplay(false);
}
mAppCallbackNotifier->stop();
mPreviewStartInProgress=false;
mPreviewEnabled=false;
LOG_FUNCTION_NAME_EXIT;

return ret;
}
在我的理解看来上面标注的部分是这个方法的关键，这个地方可是会让初次研究这里的人晕头转向的，因为我就在这里犯了错误，走岔道了，下面会说明到底是怎么走岔道的：
先说一下吧，这里调用mCameraAdapter对象的sendCommand方法
status_t BaseCameraAdapter::sendCommand(CameraCommands
 operation,intvalue1,intvalue2,intvalue3,intvalue4){
status_t ret=NO_ERROR;
struct timeval*refTimestamp;
BuffersDescriptor*desc=NULL;
CameraFrame*frame=NULL;

LOG_FUNCTION_NAME;

switch(operation){
case：
...............
case CameraAdapter::CAMERA_START_PREVIEW:
{
CAMHAL_LOGDA("Start Preview");
if ( ret == NO_ERROR )
{
ret = setState(operation);
}
if ( ret == NO_ERROR )
{
ret = startPreview();
}
if ( ret == NO_ERROR )
{
ret = commitState();
}else{
ret |= rollbackState();
}
break;
}
}
}
status_t BaseCameraAdapter::setState(CameraCommands
 operation)
{
status_t ret=NO_ERROR;

LOG_FUNCTION_NAME;

constchar*printState=getLUTvalue_translateHAL(operation,CamCommandsLUT);

mLock.lock();

switch(mAdapterState)
{

caseINTIALIZED_STATE:
............

caseLOADED_PREVIEW_STATE:

switch(operation)
{

caseCAMERA_START_PREVIEW:
CAMHAL_LOGDB("Adapter state switch LOADED_PREVIEW_STATE->PREVIEW_STATE event = %s",
printState);
mNextState=PREVIEW_STATE;
break;
}
}
}
status_t BaseCameraAdapter::startPreview()
{
status_t ret=NO_ERROR;

LOG_FUNCTION_NAME;

LOG_FUNCTION_NAME_EXIT;

return ret;
}
就是这里了，所以我用可很醒目的颜色标注出来，很多人会想当然的理解，你不是调用了startPreview方法嘛，那就是他了啊！可是这里为什么什么动作都没做呢？？
于是就卡在这里不知所措了，那个纠结啊![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/11.gif)![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/11.gif)
现在就来解开这个谜团吧！！！
这个我们还是要往前追溯了，追溯到哪里呢？？那就从这里开始吧
mCameraAdapter->sendCommand(CameraAdapter::CAMERA_START_PREVIEW);
这个方式是在CameraHal的startPreview()方法中被调用的
所以我要知道这个mCameraAdapter对象原型是什么啊，他从哪里而来，原来他是CameraHal这个类的一个成员，定义在：hardware/ti/omap4xxx/camera/inc/CameraHal.h
CameraAdapter *mCameraAdapter;
这里必须打破砂锅追到底，找到CameraAdapter 这个类的定义，他的定义同样这这个.h文件中
/**
*CameraAdapter interfaceclass
*Concrete classes derive from thisclassandprovide implementations basedonthe specific camera h/w interface
*/

classCameraAdapter:publicFrameNotifier,publicvirtual RefBase
{
protected:
enum AdapterActiveStates{
INTIALIZED_ACTIVE=1<<0,
LOADED_PREVIEW_ACTIVE=1<<1,
PREVIEW_ACTIVE=1<<2,
LOADED_CAPTURE_ACTIVE=1<<3,
CAPTURE_ACTIVE=1<<4,
BRACKETING_ACTIVE=1<<5,
AF_ACTIVE=1<<6,
ZOOM_ACTIVE=1<<7,
VIDEO_ACTIVE=1<<8,
LOADED_REPROCESS_ACTIVE=1<<9,
REPROCESS_ACTIVE=1<<10,
};
public:
typedef struct
{
CameraBuffer*mBuffers;
uint32_t*mOffsets;
intmFd;
size_t mLength;
size_t mCount;
size_t mMaxQueueable;
}BuffersDescriptor;

enum CameraCommands
{
CAMERA_START_PREVIEW=0,
CAMERA_STOP_PREVIEW=1,
CAMERA_START_VIDEO=2,
CAMERA_STOP_VIDEO=3,
CAMERA_START_IMAGE_CAPTURE=4,
CAMERA_STOP_IMAGE_CAPTURE=5,
CAMERA_PERFORM_AUTOFOCUS=6,
CAMERA_CANCEL_AUTOFOCUS=7,
CAMERA_PREVIEW_FLUSH_BUFFERS=8,
CAMERA_START_SMOOTH_ZOOM=9,
CAMERA_STOP_SMOOTH_ZOOM=10,
CAMERA_USE_BUFFERS_PREVIEW=11,
CAMERA_SET_TIMEOUT=12,
CAMERA_CANCEL_TIMEOUT=13,
CAMERA_START_BRACKET_CAPTURE=14,
CAMERA_STOP_BRACKET_CAPTURE=15,
CAMERA_QUERY_RESOLUTION_PREVIEW=16,
CAMERA_QUERY_BUFFER_SIZE_IMAGE_CAPTURE=17,
CAMERA_QUERY_BUFFER_SIZE_PREVIEW_DATA=18,
CAMERA_USE_BUFFERS_IMAGE_CAPTURE=19,
CAMERA_USE_BUFFERS_PREVIEW_DATA=20,
CAMERA_TIMEOUT_EXPIRED=21,
CAMERA_START_FD=22,
CAMERA_STOP_FD=23,
CAMERA_SWITCH_TO_EXECUTING=24,
CAMERA_USE_BUFFERS_VIDEO_CAPTURE=25,
\#ifdef OMAP_ENHANCEMENT_CPCAM
CAMERA_USE_BUFFERS_REPROCESS=26,
CAMERA_START_REPROCESS=27,
\#endif
\#ifdef OMAP_ENHANCEMENT_VTC
CAMERA_SETUP_TUNNEL=28,
CAMERA_DESTROY_TUNNEL=29,
\#endif
CAMERA_PREVIEW_INITIALIZATION=30,
};

enum CameraMode
{
CAMERA_PREVIEW,
CAMERA_IMAGE_CAPTURE,
CAMERA_VIDEO,
CAMERA_MEASUREMENT,
CAMERA_REPROCESS,
};

enum AdapterState{
INTIALIZED_STATE=INTIALIZED_ACTIVE,
LOADED_PREVIEW_STATE=LOADED_PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
PREVIEW_STATE=PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
LOADED_CAPTURE_STATE=LOADED_CAPTURE_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
CAPTURE_STATE=CAPTURE_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
BRACKETING_STATE=BRACKETING_ACTIVE|CAPTURE_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
AF_STATE=AF_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
ZOOM_STATE=ZOOM_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
VIDEO_STATE=VIDEO_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
VIDEO_AF_STATE=VIDEO_ACTIVE|AF_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
VIDEO_ZOOM_STATE=VIDEO_ACTIVE|ZOOM_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
VIDEO_LOADED_CAPTURE_STATE=VIDEO_ACTIVE|LOADED_CAPTURE_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
VIDEO_CAPTURE_STATE=VIDEO_ACTIVE|CAPTURE_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
AF_ZOOM_STATE=AF_ACTIVE|ZOOM_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
BRACKETING_ZOOM_STATE=BRACKETING_ACTIVE|ZOOM_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
LOADED_REPROCESS_STATE=LOADED_REPROCESS_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
LOADED_REPROCESS_CAPTURE_STATE=LOADED_REPROCESS_ACTIVE|LOADED_CAPTURE_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
REPROCESS_STATE=REPROCESS_ACTIVE|CAPTURE_ACTIVE|PREVIEW_ACTIVE|INTIALIZED_ACTIVE,
};


public:

///Initialzes
 the camera adapter creates any resources required
virtualintinitialize(CameraProperties::Properties*)=0;

virtualintsetErrorHandler(ErrorNotifier*errorNotifier)=0;

//Message/Frame
 notification APIs
virtual void enableMsgType(int32_t msgs,
frame_callback callback=NULL,
event_callback eventCb=NULL,
void*cookie=NULL)=0;
virtual void disableMsgType(int32_t msgs,void*cookie)=0;
virtual void returnFrame(CameraBuffer*frameBuf,CameraFrame::FrameType
 frameType)=0;
virtual void addFramePointers(CameraBuffer*frameBuf,void*buf)=0;
virtual void removeFramePointers()=0;

//APIstoconfigure
 Camera adapterandgetthe current parameterset
virtualintsetParameters(constCameraParameters&params)=0;
virtual void getParameters(CameraParameters&params)=0;

//Registers
 callbackforreturning image buffers backtoCameraHAL
virtualintregisterImageReleaseCallback(release_image_buffers_callback
 callback,void*user_data)=0;

//Registers
 callback,which signals a completed image capture
virtualintregisterEndCaptureCallback(end_image_capture_callback
 callback,void*user_data)=0;

//APItosend
 a commandtothe camera
virtual status_t sendCommand(CameraCommands operation,intvalue1=0,intvalue2=0,intvalue3=0,intvalue4=0)=0;

virtual~CameraAdapter(){};

//Retrieves
 the current Adapter state
virtual AdapterState getState()=0;

//Retrieves
 thenextAdapter state
virtual AdapterState getNextState()=0;

//Receive
 orientation events from CameraHal
virtual void onOrientationEvent(uint32_t orientation,uint32_t
 tilt)=0;

//Rolls
 the state machine backtoINTIALIZED_STATE from the current state
virtual status_t rollbackToInitializedState()=0;

//Retrieves
 the current Adapter state-forinternal use(notlocked)
virtual status_t getState(AdapterState&state)=0;
//Retrieves
 thenextAdapter state-forinternal
 use(notlocked)
virtual status_t getNextState(AdapterState&state)=0;

protected:
//The
 first two methods will trytoswitch the adapter state.
//EverycalltosetState()should
 be followed by a corresponding
//calltocommitState().Ifthe
 state switch fails,thenit will
//getresettothe
 previous state via rollbackState().
virtual status_t setState(CameraCommands operation)=0;
virtual status_t commitState()=0;
virtual status_t rollbackState()=0;
};
看一下我标出的这是红色部分啊，为什么我要把它们标注成红色呢？？
懂C++面向对象思想应该都知道virtual这个关键字是干什么的，
如果一个类的方法被定义为virtual，如果该类的子类实现了同样的方法，则这个方法被调用的时候，会忽略父类的实现，而直接调用子类的实现，前提是方法名，包括变量类型，个数必须一致
那么这里有没有类继承了CameraAdapter 这个类，并且实现了其中的一些虚拟函数呢？？答案是肯定的，我可是吃了苦头才发现的![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/11.gif)![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/11.gif)
不过也是赖自己，这是只有定义是没有实现的，肯定是由子类来实现这是方法的，不，还是不赖自己，让我吃苦头的是这里是双层继承的，双层继承啊，我怎么知道![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/11.gif)![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/11.gif)
不卖关子了，那么谁继承了CameraAdapter 这个类呢？ 先给路径：hardware/ti/omap4xxx/camera/inc/BaseCameraAdapter.h
classBaseCameraAdapter:publicCameraAdapter
{

public:

BaseCameraAdapter();
virtual~BaseCameraAdapter();

///Initialzes the camera adapter creates any resources required
virtual status_t initialize(CameraProperties::Properties*)=0;

virtualintsetErrorHandler(ErrorNotifier*errorNotifier);

//Message/Frame notification APIs
virtual void enableMsgType(int32_t msgs,frame_callback callback=NULL,event_callback eventCb=NULL,void*cookie=NULL);
virtual void disableMsgType(int32_t msgs,void*cookie);
virtual void returnFrame(CameraBuffer*frameBuf,CameraFrame::FrameType frameType);
virtual void addFramePointers(CameraBuffer*frameBuf,void*y_uv);
virtual void removeFramePointers();

//APIstoconfigure Camera adapterandgetthe current parameterset
virtual status_t setParameters(constCameraParameters&params)=0;
virtual void getParameters(CameraParameters&params)=0;

//APItosend a commandtothe camera
virtual status_t sendCommand(CameraCommands operation,intvalue1=0,intvalue2=0,intvalue3=0,intvalue4=0);

virtual status_t registerImageReleaseCallback(release_image_buffers_callback callback,void*user_data);

virtual status_t registerEndCaptureCallback(end_image_capture_callback callback,void*user_data);

//Retrieves the current Adapter state
virtual AdapterState getState();
//Retrieves thenextAdapter state
virtual AdapterState getNextState();

//Rolls the state machine backtoINTIALIZED_STATE from the current state
virtual status_t rollbackToInitializedState();

protected:
//The first two methods will trytoswitch the adapter state.
//EverycalltosetState()should be followed by a corresponding
//calltocommitState().Ifthe state switch fails,thenit will
//getresettothe previous state via rollbackState().
virtual status_t setState(CameraCommands operation);
virtual status_t commitState();
virtual status_t rollbackState();

//Retrieves the current Adapter state-forinternal use(notlocked)
virtual status_t getState(AdapterState&state);
//Retrieves thenextAdapter state-forinternal use(notlocked)
virtual status_t getNextState(AdapterState&state);

//-----------Interface that needstobe implemented by deriving classes--------------------

//Should be implmented by deriving classesinordertostart image capture
virtual status_t takePicture();

//Should be implmented by deriving classesinordertostart image capture
virtual status_t stopImageCapture();

//Should be implmented by deriving classesinordertostart temporal bracketing
virtual status_t startBracketing(intrange);

//Should be implemented by deriving classesinordertostop temporal bracketing
virtual status_t stopBracketing();

//Should be implemented by deriving classesinodertoinitiate autoFocus
virtual status_t autoFocus();

//Should be implemented by deriving classesinodertoinitiate autoFocus
virtual status_t cancelAutoFocus();

//Should be called by deriving classesinordertodosome bookkeeping
virtual status_t startVideoCapture();

//Should be called by deriving classesinordertodosome bookkeeping
virtual status_t stopVideoCapture();

//Should be implemented by deriving classesinordertostart camera preview
virtual status_t startPreview();

//Should be implemented by deriving classesinordertostop camera preview
virtual status_t stopPreview();

//Should be implemented by deriving classesinordertostart smooth zoom
virtual status_t startSmoothZoom(inttargetIdx);

//Should be implemented by deriving classesinordertostop smooth zoom
virtual status_t stopSmoothZoom();

//Should be implemented by deriving classesinordertostop smooth zoom
virtual status_t useBuffers(CameraMode mode,CameraBuffer*bufArr,intnum,size_t length,unsignedintqueueable);

//Should be implemented by deriving classesinorder queue a released bufferinCameraAdapter
virtual status_t fillThisBuffer(CameraBuffer*frameBuf,CameraFrame::FrameType frameType);

//APItogetthe frame size requiredtobe allocated.This sizeisusedtooverride the size passed
//by camera service when VSTAB/VNFisturnedONforexample
virtual status_t getFrameSize(size_t&width,size_t&height);

//APItogetrequired data frame size
virtual status_t getFrameDataSize(size_t&dataFrameSize,size_t bufferCount);

//APItogetrequired picture buffers size with the current configurationinCameraParameters
virtual status_t getPictureBufferSize(size_t&length,size_t bufferCount);

//Should be implemented by deriving classesinordertostart face detection
//(ifsupported)
virtual status_t startFaceDetection();

//Should be implemented by deriving classesinordertostop face detection
//(ifsupported)
virtual status_t stopFaceDetection();

virtual status_t switchToExecuting();

virtual status_t setupTunnel(uint32_t SliceHeight,uint32_t EncoderHandle,uint32_t width,uint32_t height);

virtual status_t destroyTunnel();

virtual status_t cameraPreviewInitialization();

//Receive orientation events from CameraHal
virtual void onOrientationEvent(uint32_t orientation,uint32_t tilt);

//---------------------Interface ends-----------------------------------

status_t notifyFocusSubscribers(CameraHalEvent::FocusStatus status);
status_t notifyShutterSubscribers();
status_t notifyZoomSubscribers(intzoomIdx,bool targetReached);
status_t notifyMetadataSubscribers(sp<CameraMetadataResult>&meta);

//Send the frametosubscribers
status_t sendFrameToSubscribers(CameraFrame*frame);

//Resets the refCountforthis particular frame
status_t resetFrameRefCount(CameraFrame&frame);

//A couple of helper functions
void setFrameRefCount(CameraBuffer*frameBuf,CameraFrame::FrameType frameType,intrefCount);
intgetFrameRefCount(CameraBuffer*frameBuf,CameraFrame::FrameType frameType);
intsetInitFrameRefCount(CameraBuffer*buf,unsignedintmask);
staticconstchar*getLUTvalue_translateHAL(intValue,LUTtypeHAL LUT);

.................
.................
}
这里我只列出了一部分代码，不过大家清楚了，BaseCameraAdapter继承CameraAdapter，不过这里还没完呢，看看这个类中定义的方法
那么多的virtual方法，后来自己才发现的，他还是被别的类继承了，而且其中的很多方法被子类重新实现了
所以实现上上面调用的startPreview方法其实不是BaseCameraAdapter.cpp中实现的那个startPreview方法
那挺调用的startPreview方法在哪里呢，自然是继承了BaseCameraAdapter 类的那个子类实现的startPreview
现在就把这个罪魁祸首拉上来，先看定义：hardware/ti/omap4xxx/camera/inc/V4LCameraAdapter/V4LCameraAdapter.h
/**
*Classwhich completely abstracts the camera hardware interaction from camera hal
*TODO:Needtolist down here,all the message types that will be supported by thisclass
Needtoimplement BufferProvider interfacetouse AllocateBuffer of OMXifneeded
*/
classV4LCameraAdapter:publicBaseCameraAdapter
{
public:

/*--------------------Constant declarations----------------------------------------*/
staticconstint32_t MAX_NO_BUFFERS=20;

///@remarks OMX Camera has six ports-buffer input,timeinput,preview,image,video,andmeta data
staticconstintMAX_NO_PORTS=6;

///Fivesecondtimeout
staticconstintCAMERA_ADAPTER_TIMEOUT=5000*1000;

public:

V4LCameraAdapter(size_t sensor_index);
~V4LCameraAdapter();


///Initialzes the camera adapter creates any resources required
virtual status_t initialize(CameraProperties::Properties*);

//APIstoconfigure Camera adapterandgetthe current parameterset
virtual status_t setParameters(constCameraParameters&params);
virtual void getParameters(CameraParameters&params);

//API
virtual status_t UseBuffersPreview(CameraBuffer*bufArr,intnum);
virtual status_t UseBuffersCapture(CameraBuffer*bufArr,intnum);

static status_t getCaps(constintsensorId,CameraProperties::Properties*params,V4L_HANDLETYPE handle);

protected:

//----------Parentclassmethod
 implementation------------------------------------//看看人家这里说的很清楚，这是父类的方法
virtual status_t startPreview();
virtual status_t stopPreview();
virtual status_t takePicture();
virtual status_t stopImageCapture();
virtual status_t autoFocus();
virtual status_t useBuffers(CameraMode mode,CameraBuffer*bufArr,intnum,size_t
 length,unsignedintqueueable);
virtual status_t fillThisBuffer(CameraBuffer*frameBuf,CameraFrame::FrameType
 frameType);
virtual status_t getFrameSize(size_t&width,size_t&height);
virtual status_t getPictureBufferSize(size_t&length,size_t
 bufferCount);
virtual status_t getFrameDataSize(size_t&dataFrameSize,size_t
 bufferCount);
virtual void onOrientationEvent(uint32_t orientation,uint32_t
 tilt);
//-----------------------------------------------------------------------------


private:

classPreviewThread:publicThread{
V4LCameraAdapter*mAdapter;
public:
PreviewThread(V4LCameraAdapter*hw):
Thread(false),mAdapter(hw){}
virtual void onFirstRef(){
run("CameraPreviewThread",PRIORITY_URGENT_DISPLAY);
}
virtual bool threadLoop(){
mAdapter->previewThread();
//loopuntilwe needtoquit
returntrue;
}
};

//Usedforcalculation of the average frame rate during preview
status_t recalculateFPS();

char*GetFrame(int&index);

intpreviewThread();

public:

private:
//capabilities data
staticconstCapPixelformat mPixelformats[];
staticconstCapResolution mPreviewRes[];
staticconstCapFramerate mFramerates[];
staticconstCapResolution mImageCapRes[];

//camera defaults
staticconstchar DEFAULT_PREVIEW_FORMAT[];
staticconstchar DEFAULT_PREVIEW_SIZE[];
staticconstchar DEFAULT_FRAMERATE[];
staticconstchar DEFAULT_NUM_PREV_BUFS[];

staticconstchar DEFAULT_PICTURE_FORMAT[];
staticconstchar DEFAULT_PICTURE_SIZE[];
staticconstchar DEFAULT_FOCUS_MODE[];
staticconstchar*DEFAULT_VSTAB;
staticconstchar*DEFAULT_VNF;

static status_t insertDefaults(CameraProperties::Properties*,V4L_TI_CAPTYPE&);
static status_t insertCapabilities(CameraProperties::Properties*,V4L_TI_CAPTYPE&);
static status_t insertPreviewFormats(CameraProperties::Properties*,V4L_TI_CAPTYPE&);
static status_t insertPreviewSizes(CameraProperties::Properties*,V4L_TI_CAPTYPE&);
static status_t insertImageSizes(CameraProperties::Properties*,V4L_TI_CAPTYPE&);
static status_t insertFrameRates(CameraProperties::Properties*,V4L_TI_CAPTYPE&);
static status_t sortAscend(V4L_TI_CAPTYPE&,uint16_t);

status_t v4lIoctl(int,int,void*);
status_t v4lInitMmap(int&);
status_t v4lInitUsrPtr(int&);
status_t v4lStartStreaming();
status_t v4lStopStreaming(intnBufferCount);
status_t v4lSetFormat(int,int,uint32_t);
status_t restartPreview();


intmPreviewBufferCount;
intmPreviewBufferCountQueueable;
intmCaptureBufferCount;
intmCaptureBufferCountQueueable;
KeyedVector<CameraBuffer*,int>mPreviewBufs;
KeyedVector<CameraBuffer*,int>mCaptureBufs;
mutable Mutex mPreviewBufsLock;
mutable Mutex mCaptureBufsLock;
mutable Mutex mStopPreviewLock;

CameraParameters mParams;

bool mPreviewing;
bool mCapturing;
Mutex mLock;

intmFrameCount;
intmLastFrameCount;
unsignedintmIter;
nsecs_t mLastFPSTime;

//variables holding the estimated framerate
float mFPS,mLastFPS;

intmSensorIndex;

//protected by mLoc
大家看到了V4LCameraAdapter 又继承了BaseCameraAdapter，双层继承，实现了父类的一些方法
所有这里算是媳妇熬着婆了，终于找到了我们想要的startPreview
不过看到终于进入了V4LCameraAdapter 这个类，我知道，离成功已经很近了，V4L2就是直接去和driver谈判的
那么我们就看看V4LCameraAdapter 这个类中的startPreview方法吧，路径：ardware/ti/omap4xxx/camera/V4LCameraAdapter/V4LCameraAdapter.cpp
status_t V4LCameraAdapter::startPreview()
{
status_t ret=NO_ERROR;

LOG_FUNCTION_NAME;
Mutex::Autolock lock(mPreviewBufsLock);

if(mPreviewing){
ret=BAD_VALUE;
gotoEXIT;
}

for(inti=0;i<mPreviewBufferCountQueueable;i++){

mVideoInfo->buf.index=i;
mVideoInfo->buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
mVideoInfo->buf.memory=V4L2_MEMORY_MMAP;

ret=v4lIoctl(mCameraHandle,VIDIOC_QBUF,&mVideoInfo->buf);
if(ret<0){
CAMHAL_LOGEA("VIDIOC_QBUF Failed");
gotoEXIT;
}
nQueued++;
}

ret=v4lStartStreaming();

//Createandstart
 preview threadforreceiving buffers from V4L Camera
if(!mCapturing){
mPreviewThread=new PreviewThread(this);
CAMHAL_LOGDA("Created preview thread");
}
不错，这条语句就是我一直找寻的，真是众里寻他千百度，蓦然回首，那句就在灯火阑珊处
这样，其他的事情就全部由v4l2去做了，这些过程会单独分一章去学习
还有就是上面绿的部分，同样要分一章学习，很重要
待续。。。。。。。。。。。

