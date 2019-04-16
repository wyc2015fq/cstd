# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之70---面向照相机的开发 - 我相信...... - CSDN博客





2010年08月26日 10:08:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1407








这里主要讲述一下面向手机中照相机的应用开发。ICamera能够使应用访问手机中的照相机传感器，配置照相机的快照和录像模式，并提供多种的记录和编码的方式（图14-9）。



**图14-9：ICamera的****体系结构**



使用ICamera 的应用可以实现取景，以类似JPEG格式编码和存储相片，以类似MPEG4格式编码和存储录像，对照相机的对比度，缩放，聚焦，白平衡等属性进行设定，监听并处理与照相机相关的各种异步事件得等，其状态机示意图如14-10所示。



**图14-10：ICamera的****状态机**

使用ICamera时，首先通过AEECLSID_CAMERA创建接口：

ISHELL_CreateInstance(pme->a.m_pIShell,AEECLSID_CAMERA,(void **)&pme->m_pICamera);

然后注册回调函数来接收Camera的事件,

ICAMERA_RegisterNotify(pme->pICamera, CApp_CameraNotify,pme);

接下来，设定取景模式和显示区域：

ICAMERA_SetParm(pme->pICamera, CAM_PARM_PREVIEW_TYPE,CAM_PREVIEW_SNAPSHOT, 0);

ICAMERA_SetDisplaySize(pme->m_pICamera, &pme->m_sizeDisplay);

ICAMERA_SetSize(pme->m_pICamera, &pme->m_sizeFrame);

就编码方式而言，照相时可以指定JPEG方式，录像时可以指定MPEG4和AAC的音频编码

ICAMERA_SetVideoEncode(pme->m_pICamera,AEECLSID_JPEG, 0);

ICAMERA_SetVideoEncode(pme->m_pICamera,AEECLSID_MPEG4, 0);

ICAMERA_SetAudioEncode(pme->m_pICamera,AEECLSID_AAC, 0);

开始和停止取景可以调用ICAMERA_Preview()和ICamera_Stop()完成，照相和录像的中的异步事件主要包含在数据结构AEECameraNotify中：

typedef struct AEECameraNotify

{

ICamera *pCam;

int16nCmd;

int16nSubCmd;

int16nStatus;

int16nReserved;

void *pData;

uint32dwSize;

} AEECameraNotify;

其中nStatus定义了类似CAM_STATUS_XXX的状态代码，nCmd 定义了指令代码（例如 CAM_CMD_START），nSubCmd 包含了子指令的代码（例如CAM_CMD_START, 子指令的代码包含了类似CAM_MODE_XXX等模式信息），pData 包含了与事件相关的数据，dwSize 是指这些数据的长度。 表14-3介绍了一些与取景相关的事件解释。
|事件组合|事件解释|
|----|----|
|{CAM_CMD_START, CAM_MODE_PREVIEW,CAM_STATUS_START}|开始取景，ICamera接口对象由READY转换成PREVIEW状态|
|{CAM_CMD_START, CAM_MODE_PREVIEW,CAM_STATUS_FRAME}|ICAMERA_GetFrame()可以获得当前的这帧图像|
|{CAM_CMD_START, CAM_MODE_PREVIEW,CAM_STATUS_DONE}|取景结束，ICamera接口对象从PREVIEW转换成状态READY|
|{CAM_CMD_START, CAM_MODE_PREVIEW,CAM_STATUS_ABORT, CAM_EXXX_}|取景时异常退出|

**表**** 14-3****：照相机取景模式中****的事件**

ICamera接口对象在取景时的状态转移时序如图14-11所示。



**图14-11：ICamera**取景时的状态转移时序



下面是在使用ICamera的录像模式中处理帧事件的示例：

static void MyApp_CameraNotify(MyApp *pme, AEECameraNotify * pn){

switch (pn->nStatus){

case CAM_STATUS_START:

// Preview has begun...

break;

case CAM_STATUS_FRAME:

{

IBitmap * pFrame;

AEEBitmapInfo bi;

// Get the frame.

ICAMERA_GetFrame(pme->m_pICamera, &pFrame));

// Get the bitmap info.

IBITMAP_GetInfo(pFrame, &bi, sizeof(bi));

// Display the frame at (0, 0) location of the screen

IDISPLAY_BitBlt(pme, 0, 0, bi.cx, bi.cy, pFrame, 0, 0, AEE_RO_COPY);

IBITMAP_Release(pFrame);

break;

}

case CAM_STATUS_DONE:

// ICAMERA_Stop() stopprf preview operation stopped.

break;

case CAM_STATUS_ABORT:

// Preview got aborted.

break;

}

}

如果希望完成一个照相的操作，首先要设定照片的存储位置，然后执行照相操作即可。

ICAMERA_SetMediaData(pme->m_pICamera, &md);

ICAMERA_RecordSnapshot(pme->m_pICamera);

原始图像是以JPEG格式保存的，ICamera接口对象照相时的状态转移时序如图14-12所示。



**图14-12：ICamera执行**照相操作的状态转移时序

在录像的时候，ICamera 接口对象会捕获原始的图像帧和相关的音频数据，然后发送帧的相关事件给BREW应用，同时对帧和音频数据以MPEG4/AAC的格式进行编码。使用时，同样先要指定录像的存储位置，然后执行录像操作，录像完毕时终止录像操作。

ICAMERA_SetMediaData(pme->m_pICamera, &md);

ICAMERA_RecordMovie(pme->m_pICamera);

ICAMERA_Stop(pme->m_pICamera);

ICamera接口对象录像时的状态转移时序如图14-13所示。



**图14-13：ICamera执行**录像操作的状态转移时序



