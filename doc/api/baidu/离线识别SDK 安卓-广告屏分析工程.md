# 离线识别SDK 安卓-广告屏分析工程



# 版本日志

| 日期       | 版本 | 更新说明               |
| :--------- | :--- | :--------------------- |
| 2019.01.10 | V1.0 | 人脸广告屏分析方案初版 |

# 1. 简介

您好，欢迎使用百度人脸广告屏解决方案。

## 1.1 方案概述

此方案提供完整的线下广告智能投放、效果监测等一整套解决方案，颠覆传统的粗放投放模式，实现品效合一、数据驱动，赋能广告主科学制定投放决策，助力线下媒体提升点位价值。

## 1.2 功能介绍

### 1.2.1 受众检测与识别

离线识别SDK，通过分析线下媒体的摄像头拍摄的视频图像，利用人脸检测、质量检测、人脸识别等功能，为视频中出现的每个受众建立唯一的受众ID。同时，离线识别SDK可以对检测到的每个受众人脸进行抠图，通过与在线API配合，将抠出的人脸图上传云端，建立人脸库，可以实现对受众的管理和运营。

### 1.2.2 受众画像分析

离线识别SDK，通过对线下媒体的摄像头拍摄的视频图像进行处理，实时分析得到视频中出现单个或多个受众的画像。画像内容包括：受众ID、年龄、性别、称谓、表情、眼镜、观看时长、关注度。结合地理位置、时间等信息，建立完备的受众画像，实现线下广告投放精细化和个性化。

### 1.2.3 投放效果分析

离线识别SDK，通过对指定时段内线下媒体的摄像头拍摄的视频图像进行分析，监测该时段内，线下媒体所在区域的人流热度，智能分析受众的动作和行为，并进行语义化解析，形成多维度投放效果报表，帮助广告主改善广告内容，优化投放决策。

## 1.3 名词解释

受众画像中，称谓由年龄和性别的结果而产生，具体产生标准为：

**表1 称谓产生标准**

![img](https://ai.bdstatic.com/file/A2D9F61A51BD4B3C8E5536B41232120F)

- 观看行为：系统检测到人脸后，该人脸头部上下（Pitch）、左右（Yaw）偏转角度在一定范围之内，视为一次观看行为。其中，上下（Pitch）、左右（Yaw）偏转角度的默认范围均为-15°~15°，此范围可根据实际情况进行调整。
- 观看时长：受众所有观看行为的持续时间，单位为秒。
- 关注度：观看时长/检测时长*100%
- 触达人数：从摄像头视频流中检测出的人脸，经过去重后，对应的受众数量。
- 观看人数：触达人数中有观看行为的受众数量。
- 检测时长：系统检测到人脸并进行人脸跟踪的全部持续时间，单位为秒。
- 关注度：观看时长/检测时长*100%
- 深度观看人数：观看度大于某个值的受众数量，默认值为50%，可根据实际情况进行调整。
- 点位热度：衡量广告媒体点位的流量热度，其计算公式为：**点位热度= 触达人数\*0.3 + 观看人数\*0.3 + 深度观看人数\*0.4**

## 1.4 应用场景

### 1.4.1 线下媒体广告投放

无网状态离线运行，精准分析广告触达人数、观看人数、收视率等指标，建立流量漏斗模型，利用数据帮助广告主科学优化投放策略。

### 1.4.2 广告精准营销

获取线下媒体屏前受众画像，打通线上线下数据，基于画像完成千人千面的广告精准投放。

### 1.4.3 新零售智能会员服务

零售店、酒店、商场等多种新零售场景，打通广告营销系统与会员管理系统，借助会员信息促进广告智能投放，利用广告投放吸引会员注册。

## 1.5 方案优势

- 多人脸场景：提供基于多人脸场景的各项人脸检测和识别功能，能够同时支持对4-8人进行处理。
- 离线/在线通用：能够在离线环境完成各项人脸检测和识别功能，也支持与在线API联动，以建立更大的受众库。
- 业务功能自定义：可以自定义观看行为等业务功能的标准，极大拓展跨场景的适用性。

# 2、集成指南

## 2.1 准备工作

在正式集成前，需要做一些准备工作，完成一些账号、应用及配置，具体如下：

### 2.1.1 注册开发者

- STEP1：点击百度AI开放平台导航右侧的控制台，页面跳转到百度云登录界面，登录完毕后，将会进入到百度云后台，点击「控制台」进入百度云控制台页面；您也可以在官网直接点击免费试用，登录完毕后将自动进入到百度云控制台。
- STEP2：使用百度账号完成登录，如您还未持有百度账户，可以点击此处注册百度账户。
- STEP3：进入百度云欢迎页面，填写企业和个人基本信息，注册完毕，至此成为开发者。（如您之前已经是百度云用户或百度开发者中心用户，STEP3 可略过。)
- STEP4：进入百度云控制台，找到人工智能相关服务面板。
- STEP5：点击进入「[人脸识别](https://console.bce.baidu.com/ai/?fromai=1#/ai/face/overview/index)」模块。

### 2.1.2 获取序列号

点击“离线识别SDK管理”选项，进入离线SDK管理页面。在此页面，用户将获得五个免费试用的序列号，每个序列号经过激活后，可以在一台设备上使用一个离线SDK。用户可以点击“下载SDK”按钮，下载离线识别SDK。

完成授权请参考 [激活教程](http://ai.baidu.com/docs#/Face-Offline-SDK-Activation/top)

## 2.2 集成逻辑

### 2.2.1 阈值选择

1. 观看行为定义

| 头部偏转方向  | 阈值     |
| :------------ | :------- |
| 上下（Pitch） | -15°~15° |
| 左右（Pitch） | -15°~15° |

1. 遮挡检测

| 指标         | 说明                                  | 阈值 |
| :----------- | :------------------------------------ | :--- |
| left_eye     | 取值范围[0~1]，0为无遮挡，1是完全遮挡 | 0.6  |
| right_eye    | 取值范围[0~1]，0为无遮挡，1是完全遮挡 | 0.6  |
| nose         | 取值范围[0~1]，0为无遮挡，1是完全遮挡 | 0.6  |
| mouth        | 取值范围[0~1]，0为无遮挡，1是完全遮挡 | 0.6  |
| left_cheek   | 取值范围[0~1]，0为无遮挡，1是完全遮挡 | 0.6  |
| right_cheek  | 取值范围[0~1]，0为无遮挡，1是完全遮挡 | 0.6  |
| chin_contour | 取值范围[0~1]，0为无遮挡，1是完全遮挡 | 0.6  |

1. 质量检测

| 指标         | 说明                                                         | 阈值 |
| :----------- | :----------------------------------------------------------- | :--- |
| illumination | 取值范围[0~255]，脸部光照的灰度值，0表示光照不好。 以及对应客户端SDK中，YUV的Y分量。 | 40   |
| blurdegree   | 取值范围[0~1]，0是最清晰，1是最模糊。                        | 0.6  |
| completeness | 0或1，0为人脸溢出图像边界，1为人脸都在图像边界内。           | 0    |

## 2.3 安卓集成说明

### 2.3.1 工程配置

1. 修改包名app->build.gradle->android->defaultConfig -> 您申请license时填的包名

```
defaultConfig {
        applicationId ""
        minSdkVersion 15
        targetSdkVersion 26
        versionCode 1
        versionName "1.0"

        ndk {
            moduleName "facesdk"
            ldLibs "log"
            abiFilters "armeabi-v7a" , "x86", "arm64-v8a" // "armeabi", "x86", "arm64-v8a"
        }
    }


<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="">
```

1. 配置权限

```
<uses-permission android:name="android.permission.CAMERA" />
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.MOUNT_UNMOUNT_FILESYSTEMS" />
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
<uses-permission android:name="android.hardware.camera.autofocus" />
<uses-permission android:name="android.permission.WRITE_SETTINGS" />
<uses-permission android:name="android.permission.READ_PHONE_STATE" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.GET_TASKS" />
```

### 2.3.2 FaceAuth鉴权接口

1. 在线鉴权

备注：鉴权时需手动输入序列号，在有网络的情况下进行鉴权

```
/*
 * 初始化鉴权,鉴权方式:通过AIPE 序列码在线激活鉴权
 *  @param context
 * @param licenseKey
 * @param callback
 */
public void initLicenseOnLine(final Context context, final String licenseKey, final AuthCallback callback)
```

- 方法功能：初始化鉴权，进行远程网络鉴权
- 方法参数：

| 字段名称   | 含义                                              | 参考值                                           | 类型         | 必传 |
| :--------- | :------------------------------------------------ | :----------------------------------------------- | :----------- | :--- |
| context    | 当前上下文                                        | context 上下文，传当前的 application 即可        | Context      | Y    |
| licenseKey | 申请的鉴权码                                      | 申请的序列码                                     | String       | Y    |
| callback   | 鉴权结果 void Response(int code, String response) | code：0：成功；1：加载失败； response : 结果信息 | AuthCallback | Y    |

- 接口调用：

```
/*
 *  鉴权接口调用
 */
mFaceAuth = new FaceAuth();
mFaceAuth.initLicenseOnLine(this, licenseKey, new AuthCallback() {
    @Override
    public void onResponse(int code, String response, String licenseKey) { 
    }
});
```

1. 离线鉴权

```
/*
 * 初始化鉴权,鉴权方式:通过AIPE 离线线激活鉴权
 *
 * @param context
 * @param callback
 */
public void initLicenseOffLine(final Context context, final AuthCallback callback) 
```

- 方法功能：进行离线鉴权
- 方法参数：

| 字段名称 | 含义                                              | 参考值                                           | 类型         | 必传 |
| :------- | :------------------------------------------------ | :----------------------------------------------- | :----------- | :--- |
| context  | 当前上下文                                        | context上下文，传当前的 application 即可         | Context      | Y    |
| callback | 鉴权结果 void Response(int code, String response) | code：0：成功；1：加载失败； response : 结果信息 | AuthCallback | Y    |

- 接口调用：

```
mFaceAuth = new FaceAuth();
faceAuth.initLicenseOffLine(this, new AuthCallback() {
    @Override
    public void onResponse(final int code, final String response, String licenseKey) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                etKey.setText("");
                Toast.makeText(FaceAipeAuthActivity.this,
                        code + "  " + response, Toast.LENGTH_SHORT).show();
            }
        });
        if (code == 0) {
            PreferencesUtil.putString("activate_key", licenseKey);
            BDFaceSDKManager.getInstance().initSDK(FaceAipeAuthActivity.this);
            finish();
        }
    }
});
```

### 2.3.3 初始化SDK

```
mFaceSDKManager = BDFaceSDKManager.getInstance();
```

### 2.3.4 人脸检测

```
public void track(int[] img, int rows, int cols, int imgType, int maxTrackObjNum)
```

- 方法功能：实现人脸检测，获取活体分值及人脸属性
- 方法参数：

| 字段名称       | 含义             | 参考值               | 类型  | 必传 |
| :------------- | :--------------- | :------------------- | :---- | :--- |
| img            | ARGB 格式的像素  | new int[rows * cpls] | int[] | Y    |
| rows           | 图像的高度       | Height               | int   | Y    |
| cols           | 图像的宽度       | Width                | int   | Y    |
| imgtype        | 图像色彩空间格式 | FaceSDK.imgType.ARGB | emum  | Y    |
| maxTrackObjNum | 最大检测人脸数   | 默认值为16           | int   | Y    |

- 接口调用：

```
/*
 * 实现人脸检测，获取活体分数及人脸属性
 */
private void decodeRun() {
    if (mYuvimg != null) {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                ARGBdata = new int[mYuvimg.width * mYuvimg.height];
                // 将YUV颜色编码格式数据转为ARGB色彩模式数据，输出ARGB色彩模式数据
                FaceSDK.getARGBFromYUVimg(mYuvimg.data, ARGBdata, mYuvimg.width,
                        mYuvimg.height, mYuvimg.angle, mYuvimg.flip);
                mFaceSDKManager.faceTrack(handler, ARGBdata, mYuvimg.height, mYuvimg.width);
                frame_stack_counter--;
            }
        };
        handler.post(runnable);
    }
}
```

### 2.3.5 Face Info(检测到的监播信息)

1. 检测到的总人数，观看广告的总人数，所有观看的总时长

```
public int getTotalPeople()//检测到的总人数
public int getViewPeople()//观看广告的总人数
public long getTotalViewTime()//所有人观看的总时长(ms)
```

- 接口调用：

```
/*
 *  检测后的数据
 */
FaceInfo faceInfo1=new FaceInfo();
//  检测到的总人数
int totalPeople = faceInfo1.getTotalPeople();
//  观看广告的总人数
int viewPeople = faceInfo.getViewPeople();
//  观看广告的总时长
long totalViewTime = faceInfo1.getTotalViewTime();
```

### 2.3.6 BDfaceInfoManager(检测到的用户信息)

1. 获取深度观看人数

```
/*
 * 获取深度观看人数
 *
 * @return
 */
public int getDepPerson()
```

- 接口调用：

```
BDFaceInfoManager.getInstance().getDepPerson()
```

1. 获取点位热度

```
/*
 * 点位热度
 *
 * @return
 */
public double getPointHeatdegree(FaceInfo faceInfo)
```

- 接口调用

```
BDFaceInfoManager.getInstance().getPointHeatdegree(faceInfo)
```

1. 获取男性人数

```
/*
 * 男性人数
 *
 * @return
 */
public int getMaleCount()
```

- 接口调用

```
BDFaceInfoManager.getInstance().getMaleCount() 
```

1. 获取女性人数

```
/*
 * 女性人數
 *
 * @return
 */
public int getFemaleCount()
```

- 接口调用

```
BDFaceInfoManager.getInstance().getFemaleCount()
```

1. 观看行为检测（姿态角过滤）

```
public void setAttitudeAngle(int lookUp, int lowHead, int leftDeviation, int rightDeviation)
```

- 方法功能：设置观看行为的条件（姿态角度属于该范围内，即为观看媒体屏幕）
- 方法参数：

| 字段名称       | 含义   | 值              | 类型 | 必传 |
| :------------- | :----- | :-------------- | :--- | :--- |
| LookUp         | 抬头值 | 负值 默认为 -15 | int  | Y    |
| lowHead        | 低头值 | 正值 默认为 15  | int  | Y    |
| leftDeviation  | 左偏值 | 负值 默认为 -15 | int  | Y    |
| rightDeviation | 右偏值 | 正值 默认为 15  | int  | Y    |

- 接口调用

```
BDFaceInfoManager.getInstance().setAttitudeAngle(-15,15,-15,15)
```

### 2.3.6 人脸基本信息

1. faceID

```
// faceID
public int getFaceID()
```

- 接口调用：

```
/*
 *  遍历 bdFacePersonList
 */
for (BDFacePerson person : bdFacePersonList)
// Face ID 
person.getFaceID();
```

1. 年龄

```
public float getAge()
```

- 接口调用：

```
/*
*  @param  person 于 Face ID 调用一致
*/
person.getAge()
```

1. 性别

```
public int getGender()
```

- 接口调用：

```
/*
*  @param  person 于 Face ID 调用一致
*  返回值为 0 男性
* 返回值为 1 女性
*/
person.getGender() == 0 ? "女" : person.getGender() == 1 ? "男" : "婴儿"
```

1. 是否戴眼镜

```
public int getGlasses()
```

- 接口调用

```
*
*  @param  person 于 Face ID 调用一致
*/
person.getGlasses() == 0 ? "无" : "有"
```

1. 情绪

```
public FaceEmotion getEmotion()
```

- 接口调用

```
/*
*  @param  person 于 Face ID 调用一致
* 
*/
person.getEmotion().expression == 0 
? "生气" : (person.getEmotion().expression == 1
        ? "恶心 " : (person.getEmotion().expression == 2
        ? "恐惧 " : (person.getEmotion().expression == 3
        ? "开心 " : (person.getEmotion().expression == 4
        ? "伤心 " : (person.getEmotion().expression == 5
        ? "惊讶 " : "中性")))))
```

1. 观看时长

```
 public long getViewTime()
```

- 接口调用

```
/**
 * 返回观看所有时间
 *
 * @return
 */
public long getViewTime() 

for (BDFacePerson person : bdFacePersonList) {
    person.getViewTime();
}
```

1. 关注度

```
public int getGazePrecent()
```

- 接口调用

```
/*
 *  @param  person 于 Face ID 调用一致
 */
person.getGazePrecent()
```