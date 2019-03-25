# 人脸检测基础版SDK

## 目录

```
1 简介	
  1.1 功能介绍	
  1.2 兼容性	
  1.3 开发包说明	
2 集成指南	
  2.1 Sample示例工程说明	
  2.2 准备工作	
    2.2.1 申请license	
    2.2.2 下载SDK	
  2.3 运行示例工程	
    2.3.1 运行自动配置授权信息的示例工程	
    2.3.2 运行未配置授权信息的示例工程	
  2.4添加SDK到工程	
  2.5权限声明	
3 功能使用	
  3.1人脸检测	
  3.2质量校验设置	
  3.3 界面定制说明	
    3.3.1 修改ui界面	
4 接口设计说明	
  4.1人脸功能管理器	
    4.1.1 创建实例	
    4.1.2人脸功能管理器初始化	
    4.1.3设置人脸功能控制参数	
    4.1.4取得人脸图像采集功能接口	
5常见问题	
```

# 1、简介

​	百度Face SDK Android 版是一种面向 Android 移动设备人脸技术开发包，此版SDK包含人脸检测、活体识别等功能，以aar包+动态链接库的形式发布。基于该方案，开发者可以轻松的构建包含人脸检测、采集和活体识别的应用。在您使用SDK之前，我们首先为您介绍一下SDK本身及相关人脸能力，以便您能更方便使用人脸服务。

## 1.1 功能介绍

此版SDK所包含的能力如下：

- **本地版活体检测**：通过让用户做出指定人脸配合式的交互动作，识别当前操作者是否为活体，此功能为离线使用，可设定指定动作是否使用及应用顺序。动作包含：眨眼、张嘴、左摇头，右摇头，摇摇头、向上抬头，向下低头七个。可有效抵御高清图片、3D建模、视频等攻击。
- **本地版人脸质量检测**：判断视频流中的图片帧中，哪些图片质量较佳，即人脸图像特征清晰（满足角度、姿态、光照、模糊度等校验）。
- **本地版人脸图像采集**：通过本地SDK能力，采集人脸图像，同时经过人脸质量检测，确保采集到的人脸图像符合各条件校验（角度、姿态、光照、模糊度等），为设备前端获取有效可分析人脸的主要功能。
- **离线授权**：SDK的授权判断，授权介质也称为license，在SDK使用中，需要通过license向授权服务器发起请求，判断SDK的使用合法性及使用有效期。

> 此版SDK**全部功能为离线版本，所有功能均本地化使用**，主要用于在客户端（Android）获取人脸，实际业务使用中，可以按照业务需要，配合在线API完成全流程的业务集成。

![img](https://ai.bdstatic.com/file/759B1B80C8BC45FF920D83A4CA382793)

**为了方便您的开发，我们已经为您准备了多种场景的示例工程，您可以根据业务需要，在后台进行直接下载，目前支持【人脸核身】【人脸闸机/门禁】【人脸登录/考勤】【多人脸识别】**,示例工程参考下图：

![img](https://ai.bdstatic.com/file/58B55D43422241A48C620551E7D10C6D)

## 1.2 兼容性

**系统**：支持 Android 4.0.3(API Level 15)及以上系统。需要开发者通过 minSdkVersion来保证支持系统的检测。

**机型**：手机和平板皆可

**构架**：支持 CPU架构平台【arm-v7，arm-64，x86】

**网络**：支持 WIFI 及移动网络,移动网络支持使用NET 网关及 WAP 网关(CMWAP、CTWAP、UNIWAP、3GWAP)。

## 1.3 开发包说明

| 文件/文件夹名         | 说明                                                         |
| :-------------------- | :----------------------------------------------------------- |
| /faceplatform-release | SDK lib 库相关代码的 aar                                     |
| /faceplatform-ui      | SDK的UI库，封装拍照裁剪等功能,以及各平台的so库。so包含以下几个平台如果关注包大小，请自行删减。/armeabi-v7a/arm64-v8a/x86， |
| FacePlatform/         | DEMO工程                                                     |

# 2、集成指南

​	本章将进行 Step-By-Step的讲解,如何快速的集成 人脸Sdk到现有应用中。一个完整的Demo 请参考开发包中的示例程序 FacePlatform。方案架构参考下图：

![img](https://ai.bdstatic.com/file/C6D66423DFD2426698599B876CDD6645)

## 2.1 Sample示例

- 把脸移入框内

  ![img](https://ai.bdstatic.com/file/7D59B41403F144E88D0C30086B190C73)

  

- 慢慢低头

  ![img](https://ai.bdstatic.com/file/8B6FB0605A05467590F443F290C8DBEA)

  

- 慢慢抬头

  

  ![img](https://ai.bdstatic.com/file/96353BF5796A493F85BD547476EB6496)

- 未检测到人脸

  

  ![img](https://ai.bdstatic.com/file/33391B2015894D1B82EC8028D57E6459)

- 向左缓慢摇头

  ![img](https://ai.bdstatic.com/file/C4CD829E0C06419AAB341B6AB1AFE25C)

  

## 2.2 准备工作

### 2.2.1 申请license

**人脸SDK License**：此license用于SDK离线功能使用，在您的申请人脸SDK的后台页面，全局->产品服务->人脸识别->客户端SDK申请

人脸控制台路径如下：

![img](https://ai.bdstatic.com/file/5C7C5F88B04A4C8D85972236FE616447)

点击客户端SDK管理，弹出如下图：创建应用（这里创建应用是为了使用离线SDK，上面创建应用为了使用人脸在线接口，如注册、识别等）

![img](https://ai.bdstatic.com/file/7C76C0FF58604E5D87597CB6580B69FE)

在弹出的框中输入授权标识，选择应用类型，应用系统，以及包名、MD5签名，详情请查看输入框右边提示

![img](https://ai.bdstatic.com/file/246891F450FE4845B26029798FFE5DA6)

### 2.2.2 下载SDK

![img](https://ai.bdstatic.com/file/881DF6D9264C42F487016E841B6491D3)

下载SDK分为自动配置授权信息（创建license后就可以选择为该应用，下载后SDK自动帮您配置授权，不用下载license拷贝到工程中，初始化参数licenseID,包名也帮您配置好了）和未配置授权信息两种方式：

![img](https://ai.bdstatic.com/file/FDA8A956CF264D739FBABF2260DC8E95)

## 2.3 运行示例工程

### 2.3.1 运行自动配置授权信息的示例工程

**该下载的示例工程，已经帮你改好了license和包名**

（1）Android Studio导入下载的示例工程

（2）配置打包签名文件，由于SDK运行时会校验签名文件的MD5是否和申请时填入一样。为了便于debug能正常使用SDK，需要配置打包签名文件。

（3）运行示例工程

![img](https://ai.bdstatic.com/file/39DD088B6BEC4FC297C5445AFA6DD0D4)

![img](https://ai.bdstatic.com/file/13F50F7335B649C58F48D253D688757B)

### 2.3.2 运行未配置授权信息的示例工程

（1）Android Studio导入下载的示例工程

（2）下载license拷贝到工程的assets目录

![img](https://ai.bdstatic.com/file/CD68CAB88A0D452194339FCC950F4668)

![img](https://ai.bdstatic.com/file/E360349A4534412D8F17770DE0FB430A)

（3）修改Config类

![img](https://ai.bdstatic.com/file/6E2D997D24BE46E3B820B0916DCCAD8A)

![img](https://ai.bdstatic.com/file/14F6FD0BFDCC4537B96CCECEE6BA850C)

（4）修改app.gradle和AndroidManifest里面的包名为申请license填入的包名，如上图安卓包名。

![img](https://ai.bdstatic.com/file/381D5DF95F9043A78C68A1E303322332)

![img](https://ai.bdstatic.com/file/7AAFCF3EAAE144A1878BECC24C57DE3E)

（5）配置打包签名文件，由于SDK运行时会校验签名文件的MD5是否和申请时填入一样。为了便于debug能正常使用SDK，需要配置打包签名文件。

![img](https://ai.bdstatic.com/file/D0E534CFACA2433B8639EF89DA708CE3)

![img](https://ai.bdstatic.com/file/EF3F7A9666E747578F48025FADE59FB9)

（6）运行示例工程。如果无法正常体验，请查看logcat日志。是否有**FaceSDK-License LICENSE_INFO_CHECK_ERROR **日志。如果有说明授权没有成功，可以查看本文档最后的常见问题进行解决。

## 2.4 添加SDK到工程

FaceSdk以androidstudio开发方式提供，以下介绍在android studio开发工具导入FaceSdk

（1）将开发包中的faceplatform-release库Copy 到工程根目录。

![img](https://ai.bdstatic.com/file/C23E0955A19849728315CE92BD2310C1)

（2）将开发包中的faceplatform-ui库Copy 到工程根目录。

（3）SDK提供的了开源的faceplatform-ui库，把活体检测和人脸图像采集功能等功能进行了封装，适配了主流机型机型。如果需要使用，请添加faceplatform-ui模块到的工程中。faceplatform-ui目录结构如下图

 ![img](https://ai.bdstatic.com/file/A22B3618EB134243ADD6A54691D28791)

（4）在build.gradle使用compile project引人faceplatform-ui库工程。

![img](https://ai.bdstatic.com/file/0D11491793D14B8D801E5BF64EB94E32)

（5）Setting.gradle中include faceplatfrom-ui和facepaltfrom-release

![img](https://ai.bdstatic.com/file/75114EE9EC0D44B5BB1F496188F95E03)

（6）从官网下载授权文件license，复制到app/src/main/assets目录下。

（7）申请的license已经和打包签名key进行了绑定（申请时用到了签名的md5，为了便于debug模式也能调用SDK的功能，需要把debug的key改成申请license的key。

​	a、把key拷贝到项目根目录下

​	b、主appbuild.gradle android 下面添加(修改)signingConfigs相关的配置。如下图。

![img](https://ai.bdstatic.com/file/A1538297CFF04D1A88D9251183FCCFA1)

## 2.5 权限声明

| 名称                                      | 用途                                   |
| :---------------------------------------- | :------------------------------------- |
| android.permission.INTERNET               | 允许应用联网,SDK联网授权。             |
| android.permission.READ_PHONE_STATE       | 获取用户手机的 IMEI,用来唯一的标识用户 |
| android.permission.CAMERA                 | 允许调用相机进行拍照                   |
| android.hardware.camera.autofocus         | 允许相机对焦                           |
| android.permission.WRITE_EXTERNAL_STORAGE | 图片裁剪临时存储                       |

# 3、功能使用

## 3.1 人脸检测

**（1） **调用FaceSDKManager.getIntance().initialize(context,Config.licenseID, Config.licenseFileName);

初始化SDK,demo中此代码在Demoapplication中。

(2) 初始化参数设置

![img](https://ai.bdstatic.com/file/363DB13EDBAC4667895E740686E366DD)

人脸旋转角度参考下图：

![img](https://ai.bdstatic.com/file/03D8FB257D664D01A5C927647190843F)

（3） FaceConfig config =FaceSDKManager.*getInstance*().getFaceConfig();

FaceConfig .setSound(boolean)相应的方法设置，提示音&提示语资源。(非必须)

（4）Activity中初始化相机可见示例TrackActivity中的initCamera中，主要用到2个类FaceDetectManager及CameraImageSource等。(该部分代码虽然在facesdk中，但是开源代码，可根据自身需要修改。)代码如：

![img](https://ai.bdstatic.com/file/3203334F80E0409F846E0600B937EEE1)

（5）在faceDetectManager的监听回调方法onDetectFace中，可以检测得到人脸信息FaceInfo,retCode为人脸检测值，，ImageFrame为相机返回的预览图片，一般为0的时候表示合格人脸图片，可用这时候的图片进行人脸注册、登录等事宜。亦可如后描述7在onTrack中取图片进行。

（6）sdk自带可适配前置、后置及usb摄像头，可在如下方法中进行调整。

![img](https://ai.bdstatic.com/file/9553E337111D4EC49DB2CEB2ED1C8D3D)

（7）多次调用采集的人脸图片也可保存在本地，如示例demo中得DetectActivity中的faceDetectManager的回调方法onTrack中，可以利用回调参数FaceFilter.TrackedModel model中进行model.cropFace获取回调的图片并进行保存（Bitmap face = model.cropFace();）。

（8） 以上为完成人脸检测采集，若需要调用在线API,可以使用第7部获取的bitmap可用于调用百度人脸云服务（如：注册、识别等。具体参见 https://ai.baidu.com/docs#/Face-API/top）。调用在线服务通常只需要传最佳人脸（bestimage0）

​	注意：调用在线api功能，为了上传人脸更快，可以把人脸图片压缩到200*200~300*300。如果调用在线活体功能，需要保证上传图片中人脸不小于100px*100px，长宽占图片的三分之一左右。

## 3.2 质量校验设置

com.baidu.idl.face.platform.FaceConfig类用于人脸检测参数设置。

| 参数               | 名称                                                   | 默认值 | 取值范围   |
| :----------------- | :----------------------------------------------------- | :----- | :--------- |
| brightnessValue    | 图片爆光度                                             | 40f    |            |
| blurnessValue      | 图像模糊度                                             | 0.5f   | 0~1.0f     |
| occlusionValue     | 人脸遮挡阀值                                           | 0.5f   | 0~1.0f     |
| headPitchValue     | 低头抬头角度                                           | 10     | 0~45       |
| headYawValue       | 左右角度                                               | 10     | 0~45       |
| headRollValue      | 偏头角度                                               | 10     | 0~45       |
| cropFaceValue      | 裁剪图片大小                                           | 400    |            |
| minFaceSize        | 最小人脸检测值 小于此值的人脸将检测不出来。最小值为80. | 200    |            |
| notFaceValue       | 人脸置信度                                             | 0.6f   | 0~1.0f     |
| isCheckFaceQuality | 是否检测人脸质量                                       | true   | true/flase |

## 3.3 界面定制说明

### 3.3.1 修改faceplatform_ui界面

（1）如果SDK自带的UI和您的APP不统一，您可以自行修改FaceDetectRoundView。

![img](https://ai.bdstatic.com/file/EC4F71B37E5F4BDCB5DEA72664E495F0)

（2）修改提示语音音频文件，有两种方式。

a、直接替换FaceUI工程raw下的mp3文件和string.xml。

b、FaceEnvironment 提供了setSoundId(FaceStatusEnum status, int soundId); 设置提示音资源。 FaceStatusEnum为不同的状态。soundId为资源文件所对应的resource id. //TODO 都支持哪些音频格式？ 和	setTipsId(FaceStatusEnumstatus, int tipsId); 设置提示语。

# 4、接口设计说明

## 4.1 人脸功能管理器

### 4.1.1创建实例

- 方法

 FaceSDKManager getInstance()

- 参数

 无

- 返回

 人脸功能管理器

- 说明

创建人脸功能管理器

### 4.1.2 人脸功能管理器初始化

- 方法

```
       public void initialize(final Context context, String licenseID, String licenseFileName)
```

- 参数

 context 上下文环境，licenseID 传入申请License时获取的应用名称+_face_android后缀

- 返回

 无

- 说明

 初始化人脸检测功能。进行人脸检测功能License鉴权验证。

### 4.1.3 设置人脸功能控制参数

- 方法

```
       void setFaceConfig(FaceConfig config)
```

- 参数

 config 人脸功能控制参数对象

- 返回

 无

- 说明

 设置人脸功能控制参数对象。

 FaceConfig对象参数：

 光照阀值

 图像模糊阀值

 人脸遮挡阀值

 头部姿态角度

 最小人脸检测阀值

 人脸检测精度阀值

 截取人脸图片大小

 进行活体检测的动作类型列表

 是否进行人脸图片质量检测

### 4.1.4 取得人脸图像采集功能接口

- 方法

```
    IDetectStrategy getDetectStrategyModule()
```

- 参数

 无

- 返回

 人脸图像采集功能接口

- 说明

 取得人脸图像采集功能接口。人脸图像采集接口完成，解析图片人脸信息，返回检测结果。

# 5、常见问题

（1）**license**文件有什么用，该放在什么地方？

license文件需要申请，目的是作为sdk校验开发者的使用合法性，license文件放置位置不对或未放置license文件会导致没法使用sdk，一般应先申请license文件，并把申请得到的license文件，放置在assets目录下面。

（2）**FaceSDK-LicenseLICENSE_INFO_CHECK_ERROR =license id**

licenseID为您申请时填appname+“_face_android”。如下图demo-turnstile-face-android为license里面的licenseID, **demo-turnstile-face-android1**为app运行时Config.licenseID，两者必须一致

![img](https://ai.bdstatic.com/file/15242FCB29314C2893AB200E31179470)

（3）**FaceSDK-LicenseLICENSE_INFO_CHECK_ERROR =signature md5**

md5不一致错误，签名的为license里面的md5，后面的为app运行时获取的签名文件的md5，这两个md5必须一致且区分大小写。![img](https://ai.bdstatic.com/file/A3D53F40DB154257B4742B917348BE55)

（4） **FaceSDK-License LICENSE_INFO_CHECK_ERROR =package name**

PackageName不一致错误。License里面的packagename为申请license时填的，需要保证和app里面的packagename一致。

（5） 在有些机型上出现特别卡或出现无响应？

SDK在**armeabi**上性能非常差，建议删掉其他so只留下armeabi-v7a，包括使用的其他第三方so。因为如果其他so有armeabi，根据android系统查找so的逻辑，在armeabi的机型上只会去该目录下查找so，而人脸SDK没有，就会出现找不到.so文件。

（6） **license** 文件失效了，不能用了怎么办？

license文件申请时候有期限，如过期会导致校验失效，需要在后台申请延期。