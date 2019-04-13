
# MTK8127添加一个新的camera驱动 - 嵌入式Linux - CSDN博客

2015年11月03日 16:06:19[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：2088


简单总结一下
mtk 的all in one 这个文档台强大了，如果第一次配置摄像头，可以找这个文档来看。

# １、kernel层添加
+mediatek/custom/mid713l_lp_lvds/kernel/imgsensor/bf3703_yuv/　驱动文件夹
+bf3703yuv_CameraCustomized.h
+bf3703yuv_Camera_Sensor_para.h
+bf3703yuv_Sensor.c
+bf3703yuv_Sensor.h

# ２、hal层添加
mediatek/custom/mt8127/hal/
在这层里面有时候没有需要添加的问价，那是因为我们的摄像头是yuv的摄像头，自带了ＩＳＰ功能

# ３、驱动结构添加
mediatek/custom/common/kernel/imgsensor/src/kd_sensorlist.h
+UINT32 BF3703_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc);
+\#if defined(BF3703_YUV)
+    {BF3703_SENSOR_ID, SENSOR_DRVNAME_BF3703_YUV, BF3703_YUV_SensorInit},
+\#endif
# ４、定义ＩＤ和drv name
mediatek/custom/common/kernel/imgsensor/inc/kd_imgsensor.h
+\#define BF3703_SENSOR_ID            0x3703
+\#define SENSOR_DRVNAME_BF3703_YUV   "bf3703yuv"
# ５、hal层ＩＤ和driver　name的衔接
mediatek/custom/common/hal/imgsensor/src/sensorlist.cpp
+\#if defined(BF3703_YUV)
+    YUV_INFO(BF3703_SENSOR_ID, SENSOR_DRVNAME_BF3703_YUV, NULL),
+\#endif
# ６、Camera Sensor宏配置
mediatek/config/mid713l_lp_lvds/ProjectConfig.mk
＋CUSTOM_HAL_IMGSENSOR=bf3920_yuv
＋CUSTOM_HAL_SUB_IMGSENSOR=bf3703_yuv
＋CUSTOM_KERNEL_IMGSENSOR=bf3920_yuv
＋CUSTOM_KERNEL_SUB_IMGSENSOR=bf3703_yuv
**宏配置用git diff 显示如下　上面写的这几个并不完整**

```python
# User space image sensor driver. Define  project used all image sensors. The value is combination of CUSTOM_HAL_MAIN_IMGSENSOR, CUSTOM_HAL_MAIN_BACKUP_IMGSENSOR, CUSTOM_HAL_SUB_IMGSENSOR, and CUSTOM_HAL_SUB_BACKUP_IMGSENSOR
-CUSTOM_HAL_IMGSENSOR=ov5647_raw ov2659_yuv
+CUSTOM_HAL_IMGSENSOR=s5k5e2ya_mipi_raw ov2659_yuv
 
 # = CUSTOM_HAL_MAIN_LENS +CUSTOM_HAL_MAIN_BACKUP_LENS + CUSTOM_HAL_SUB_LENS + CUSTOM_HAL_SUB_BACKUP_LENS
 CUSTOM_HAL_LENS=fm50af sensordrive dummy_lens
@@ -61,7 +61,7 @@ CUSTOM_HAL_MAIN_BACKUP_IMGSENSOR=
 CUSTOM_HAL_MAIN_BACKUP_LENS=sensordrive
 
 # User space image sensor  driver: Main camera (rear camera) used sensor related tuning, setting and calibration information.Value is used main sensor name.
-CUSTOM_HAL_MAIN_IMGSENSOR=ov5647_raw
+CUSTOM_HAL_MAIN_IMGSENSOR=s5k5e2ya_mipi_raw
 
 # lens driver config for main camera
 CUSTOM_HAL_MAIN_LENS=fm50af
@@ -117,7 +117,7 @@ CUSTOM_KERNEL_GYROSCOPE=mpu6050gy
 CUSTOM_KERNEL_HEADSET=accdet
 
 # Kernel space image sensor driver. Define  project used all image sensors .The value is combination of CUSTOM_KERNEL_MAIN_IMGSENSOR, CUSTOM_KERNEL_MAIN_BACKUP_IMGSENSOR, CUSTOM_KERNEL_SUB_IMGSENSOR, and CUSTOM_KERNEL_SUB_BACKUP_IMG
-CUSTOM_KERNEL_IMGSENSOR=ov5647_raw ov2659_yuv
+CUSTOM_KERNEL_IMGSENSOR=s5k5e2ya_mipi_raw ov2659_yuv
 
 # key pad driver to report key event
 CUSTOM_KERNEL_KPD=kpd
@@ -140,7 +140,7 @@ CUSTOM_KERNEL_MAIN_BACKUP_IMGSENSOR=
 CUSTOM_KERNEL_MAIN_BACKUP_LENS=
 
 # Kernel space image sensor driver:Main camera (rear camera) used sensor driver.Value is used main sensor name.
-CUSTOM_KERNEL_MAIN_IMGSENSOR=ov5647_raw
+CUSTOM_KERNEL_MAIN_IMGSENSOR=s5k5e2ya_mipi_raw
```


# ７、时序配置
mediatek/custom/mid713l_lp_lvds/kernel/camera/camera/kd_camera_hw.c
+else if ((pinSetIdx==1) && currSensorName && (0 == strcmp(SENSOR_DRVNAME_BF3703_YUV, currSensorName)))
分别配置上电/下电.              原理图上的PIN脚收集如下.
Main Sensor 控制脚:
CMRST_MAIN     CMPDN_MAIN
VCAM_AF        AF_EN
VCAMD          VCAMA
VCAMIO
SCL            SDA
CMMCLK
MIPI-RDP0    MIPI-RDN0
MIPI-RDP1    MIPI-RDN1
MIPI-RDP2    MIPI-RDN2
MIPI-RDP3    MIPI-RDN3
MIPI-RCP     MIPI-RCN
F-LED+
F-LED1-
F-LED2-
# ８、Camera预览/拍照　存储照片的方向改变
mediatek/custom/mid713l_lp_lvds/hal/imgsensor/src/cfg_setting_imgsensor.cpp
+namespace NSCamCustomSensor {
+/*******************************************************************************
+* Image Sensor Orientation
+*******************************************************************************/
+SensorOrientation_T const&
+getSensorOrientation()
+{
+    static SensorOrientation_T const inst = {
+        u4Degree_0  : 0,   // 90  main sensor in degree (0, 90, 180, 270)
+        u4Degree_1  : 0,    // 270  sub  sensor in degree (0, 90, 180, 270)
+        u4Degree_2  : 90,   //  main2 sensor in degree (0, 90, 180, 270)
+    };
+    return inst;
+}


# ９、摄像头分层的相关代码
![](https://img-blog.csdn.net/20151106095249017?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```python
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ gedit mediatek/custom/mid713l_lp_lvds/kernel/camera/camera/kd_camera_hw.c
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls packages/apps/Camera
Camera/  Camera2/ 
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls packages/apps/Camera/
ext  MODULE_LICENSE_APACHE2  NOTICE  res  res_emulator  src
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls frameworks/base/core/java/android/hardware/Camera.java 
frameworks/base/core/java/android/hardware/Camera.java
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls frameworks/base/core/jni/android_hardware_Camera.cpp 
frameworks/base/core/jni/android_hardware_Camera.cpp
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls frameworks/av/include/camera/
camera2       Camera.h          CameraParameters.h  ICamera.h                ICameraRecordingProxyListener.h  ICameraServiceListener.h  IProCameraUser.h
CameraBase.h  CameraMetadata.h  ICameraClient.h     ICameraRecordingProxy.h  ICameraService.h                 IProCameraCallbacks.h     ProCamera.h
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls frameworks/av/camera/
Android.mk  CameraBase.cpp  CameraMetadata.cpp    ICameraClient.cpp  ICameraRecordingProxy.cpp          ICameraService.cpp          IProCameraCallbacks.cpp  MODULE_LICENSE_APACHE2  ProCamera.cpp
camera2     Camera.cpp      CameraParameters.cpp  ICamera.cpp        ICameraRecordingProxyListener.cpp  ICameraServiceListener.cpp  IProCameraUser.cpp       NOTICE                  tests
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls mediatek/frameworks-ext/av/include/camera/
MtkCamera.h  MtkCameraMMP.h  MtkCameraParameters.h  MtkCameraProfile.h
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls mediatek/frameworks-ext/av/camera/
MtkCamera.cpp  MtkCameraMMP.cpp  MtkCameraParameters.cpp  MtkCameraProfile.cpp
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls frameworks/av/services/camera/libcameraservice/
Android.mk  api1  api2  api_pro  CameraDeviceFactory.cpp  CameraDeviceFactory.h  CameraService.cpp  CameraService.h  common  device1  device2  device3  gui  MODULE_LICENSE_APACHE2  NOTICE  utils
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls system/core/include/system/
audio.h         audio_policy.h  camera.h        graphics.h      thread_defs.h   window.h        
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls system/core/include/system/camera.h 
system/core/include/system/camera.h
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls hardware/libhardware
Android.mk  CleanSpec.mk  hardware.c  include  MODULE_LICENSE_APACHE2  modules  NOTICE  tests
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls hardware/libhardware/include/hardware/camera.h 
hardware/libhardware/include/hardware/camera.h
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls mediatek/platform/mt8127/hardware/
aal/        Android.mk  audio/      bwc/        include/    jpeg/       m4u/        mtkcam/     pq/         vcodec/     
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$ ls mediatek/platform/mt8127/hardware/mtkcam/
acdk  Android.mk  core  custom  devicemgr  entry  hwutils  mtkcam.mk  v1
weiqifa@weiqifa-Inspiron-3847:~/weiqifa/new_tm100/tm100$
```

# 10、摄像头电压引脚定义
![](https://img-blog.csdn.net/20151124110059951)
VCAM_A ==AVDD
VCAM_D==DVDD
VCAM_A2==AFVDD
VCAM_D2==IOVDD

这些要对应摄像头的datesheet查看



