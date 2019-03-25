# Face SDK iOS开发文档

## 目录

```
1 简介
  1.1 功能介绍	
  1.2 兼容性	
  1.3 开发包说明	
2 集成指南	
  2.1 Sample示例	
  2.2 准备工作	
    2.2.1 申请license	
    2.2.2 下载SDK	
  2.3 运行示例工程	
    2.3.1自动配置授权信息集成	
    2.3.2 未使用自动配置授权信息的集成	
  2.4添加SDK到工程	
  2.5权限声明	
3 功能使用	
  3.1 活体识别	
  3.2人脸采集	
  3.3质量校验设置	
  3.4 界面定制说明	
  3.5接口设计说明	
    3.5.1 DetectRemindCode	
    3.5.2 FaceLivenessActionType	
    3.5.3 LivenessRemindCode	
  3.6创建实例	
  3.7 设置人脸功能控制参数	
  3.8 取得人脸图像采集功能接口	
  3.9 取得活体检测功能接口	
  3.10 设置人脸图像采集功能参数	
  3.11 设置人脸功能控制参数	
  3.12 活体动作设置	
4 常见问题	
```

# 1、简介

## 1.1 功能介绍

​	百度Face SDK IOS 版是一种面向 IOS 移动设备人脸技术开发包，此版SDK包含人脸检测、活体识别等功能。基于该方案，开发者可以轻松的构建包含人脸检测、采集和活体识别的应用。在您使用SDK之前，我们首先为您介绍一下SDK本身及相关人脸能力，以便您能更方便使用人脸服务。

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

- **系统**：支持iOS8以上系统。需要开发者通过Deployment Target 来保证支持系统的检测。
- **机型**：手机和平板皆可
- **网络**：支持 WIFI 及移动网络,移动网络支持使用 NET 网关及WAP 网关(CMWAP、CTWAP、UNIWAP、3GWAP)。

## 1.3 开发包说明##

![img](https://ai.bdstatic.com/file/8DA66CBB6D7B47C9B63B5B97C3B4CA1C)

# 2、集成指南

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

在弹出的框中输入授权标识，选择应用类型，应用系统，以及包名，详情请查看输入框右边提示

![img](https://ai.bdstatic.com/file/246891F450FE4845B26029798FFE5DA6)

### 2.2.2 下载SDK

![img](https://ai.bdstatic.com/file/50F8DDDD329F41D29FCC2EC348BE7B8B)

下载SDK分为自动配置授权信息（创建license后就可以选择为该应用，下载后SDK自动帮您配置授权，不用下载license拷贝到工程中，初始化参数licenseID,包名也帮您配置好了）和未配置授权信息两种方式：

![img](https://ai.bdstatic.com/file/880FA64A0E1640B99758D2F52C118C85)

## 2.3 运行示例工程

### 2.3.1 自动配置授权信息集成

如果您是通过自动配置授权信息下载的示例工程，只需配置好证书即可。查看下项目中的`FaceParameterConfig.h`文件，已经自动配置

![img](https://ai.bdstatic.com/file/54E23F1D92D44F25840CCE818EC2FF23)

配置好证书，即可运行。注意已经设置好的bundle id不要随意改动。

### 2.3.2 未使用自动配置授权信息的集成

打开或者新建一个项目。

右键点击项目，会出现一个添加菜单，在菜单中选择『Add Files to“此处是你的项目名字”…… 』,如下图所示：

![img](https://ai.bdstatic.com/file/32C6A8B891234D13B91DF07DBD5BC813)

## 2.4 添加SDK到工程##

1. 打开或者新建一个项目。
2. 右键点击项目，会出现一个添加菜单，在菜单中选择『Add Files to“此处是你的项目名字”…… 』,如下图所示：

![img](https://ai.bdstatic.com/file/3EC8946DDB604E709A154D93C3A2BECB)

1. 在添加文件弹出框里面选择申请到的license和SDK添加进来。如下图：

**注意：license为百度官方提供的。**

SDK包含下面三个文件:

- IDLFaceSDK.framework
- com.baidu.idl.face.faceSDK.bundle
- com.baidu.idl.face.model.bundle

![img](https://ai.bdstatic.com/file/F6A79051C29044138DFB9F5C1A245388)

1. 确认下Bundle Identifier 是否是申请license时填报的那一个，注意：license和Bundle Identifier是一一对应关系，填错了会导致SDK不能用。![img](https://ai.bdstatic.com/file/DA80FF80F0454FCDA754ED46CFCDC289)
2. 填写正确的FACE_LICENSE_ID。

**（即后台展示的LicenseID）**

在FaceParameterConfig.h文件里面填写拼接好的FACE_LICENSE_ID。

![img](https://ai.bdstatic.com/file/4C6919235B254AA5B4B7DC1C2939F82D)

1. 选择链接C++标准库。

![img](https://ai.bdstatic.com/file/0B648CE1F76546A39F2BD554CE845D93)

1. 如果没有使用pod管理第三方库的话，请在Build Setting >Linking > Other Linker Flags 上面加入 –ObjC 选项。如果用了pod请忽略，因为pod会自动添加上。

![img](https://ai.bdstatic.com/file/D08925EDE7724DBABB31463ECF4E187A)

## 2.5 权限声明

需要使用相机权限：编辑Info.plist文件，添加

Privacy- Camera Usage Description 的Key值，Value为使用相机时候的提示语，可以填写：『使用相机』。

![img](https://ai.bdstatic.com/file/914EE9DC8C40431F8191507E4CA55134)

# 3、功能使用

## 3.1 活体识别

使用IDLFaceLivenessManager类的:

```
(void)livenessStratrgyWithImage:(UIImage*)image previewRect:(CGRect)previewRectdetectRect:(CGRect)detectRect completionHandler:(LivenessStrategyCompletion)completion
```

方法进行活体识别功能。使用方法见下图：

![img](https://ai.bdstatic.com/file/8E867F2117A048AF970923BF76EB206D)

## 3.2 人脸采集

使用IDLFaceDetectionManager类的

```
(void)detectStratrgyWithImage:(UIImage*)image previewRect:(CGRect)previewRect  detectRect:(CGRect)detectRectcompletionHandler:(DetectStrategyCompletion)completion;
```

方法，来进行人脸采集。使用方法见下图：

![img](https://ai.bdstatic.com/file/AA4AA53B0B92496D81F085492D9EB270)

## 3.3 质量校验设置

![img](https://ai.bdstatic.com/file/153AE590D6B244B4BEA339D47C4FBD93)

## 3.4 界面定制说明

如果需要自定义UI界面，可以在FaceBaseViewController类的viewDidLoad方法里面进行修改。

## 3.5 接口设计说明

### 3.5.1 DetectRemindCode

| **类型名**                             | DetectRemindCode          |        |
| :------------------------------------- | :------------------------ | :----- |
| **名称**                               | 人脸检测返回类型          |        |
| **所属头文件**                         | IDLFaceDetectionManager.h |        |
| **类型说明**                           | Enum 枚举类型             |        |
| **枚举名**                             | **说明**                  | **值** |
| DetectRemindCodeOK                     | 正常                      | 0      |
| DetectRemindCodePitchOutofDownRange    | 头部偏低                  | 1      |
| DetectRemindCodePitchOutofUpRange      | 头部偏高                  | 3      |
| DetectRemindCodeYawOutofLeftRangeE     | 头部偏左                  | 3      |
| DetectRemindCodeYawOutofRightRange     | 头部偏右                  | 4      |
| DetectRemindCodePoorIllumination       | 光照不足                  | 5      |
| DetectRemindCodeNoFaceDetected         | 没有检测到人脸            | 6      |
| DetectRemindCodeImageBlured            | 图像模糊                  | 7      |
| DetectRemindCodeOcclusionLeftEye       | 左眼有遮挡                | 8      |
| DetectRemindCodeOcclusionRightEye      | 右眼有遮挡                | 9      |
| DetectRemindCodeOcclusionNose          | 鼻子有遮挡                | 10     |
| DetectRemindCodeOcclusionMouth         | 嘴巴有遮挡                | 11     |
| DetectRemindCodeOcclusionLeftContour   | 左脸颊有遮挡              | 12     |
| DetectRemindCodeOcclusionRightContour  | 右脸颊有遮挡              | 13     |
| DetectRemindCodeOcclusionChinCoutour   | 下颚有遮挡                | 14     |
| DetectRemindCodeTooClose               | 太近                      | 15     |
| DetectRemindCodeTooFar                 | 太远                      | 16     |
| DetectRemindCodeBeyondPreviewFrame     | 出框                      | 17     |
| DetectRemindCodeVerifyInitError        | 鉴权失败                  | 18     |
| DetectRemindCodeVerifyDecryptError     | 鉴权失败                  | 19     |
| DetectRemindCodeVerifyInfoFormatError  | 鉴权失败                  | 20     |
| DetectRemindCodeVerifyExpired          | 鉴权失败                  | 21     |
| DetectRemindCodeVerifyMissRequiredInfo | 鉴权失败                  | 22     |
| DetectRemindCodeVerifyInfoCheckError   | 鉴权失败                  | 23     |
| DetectRemindCodeVerifyLocalFileError   | 鉴权失败                  | 24     |
| DetectRemindCodeVerifyRemoteDataError  | 鉴权失败                  | 25     |
| DetectRemindCodeTimeout                | 超时                      | 26     |
| DetectRemindCodeConditionMeet          | 条件满足                  | 27     |

### 3.5.2 FaceLivenessActionType

| 类型名                              | FaceLivenessActionType |      |
| :---------------------------------- | :--------------------- | :--- |
| 名称                                | 活体检测命令类型       |      |
| 所属头文件                          | FaceSDKManager.h       |      |
| 类型说明                            | Enum 枚举类型          |      |
| 枚举名                              | 说明                   | 值   |
| FaceLivenessActionTypeLiveEye       | 眨眨眼                 | 0    |
| FaceLivenessActionTypeLiveMouth     | 张张嘴                 | 1    |
| FaceLivenessActionTypeLiveYawRight  | 向右摇头               | 2    |
| FaceLivenessActionTypeLiveYawLeft   | 向左摇头               | 3    |
| FaceLivenessActionTypeLivePitchUp   | 抬头                   | 4    |
| FaceLivenessActionTypeLivePitchDown | 低头                   | 5    |
| FaceLivenessActionTypeLiveYaw       | 摇头                   | 6    |
| FaceLivenessActionTypeNoAction      | 没有动作               | 7    |

### 3.5.3 LivenessRemindCode

| **类型名**                               | LivenessRemindCode       |        |
| :--------------------------------------- | :----------------------- | :----- |
| **名称**                                 | 活体检测提醒类型         |        |
| **所属头文件**                           | IDLFaceLivenessManager.h |        |
| **类型说明**                             | Enum 枚举类型            |        |
| **枚举名**                               | **说明**                 | **值** |
| LivenessRemindCodeOK                     | 正常                     | 0      |
| LivenessRemindCodePitchOutofDownRange    | 头部偏低                 | 1      |
| LivenessRemindCodePitchOutofUpRange      | 头部偏高                 | 2      |
| LivenessRemindCodeYawOutofLeftRange      | 头部偏左                 | 3      |
| LivenessRemindCodeYawOutofRightRange     | 头部偏右                 | 4      |
| LivenessRemindCodePoorIllumination       | 光照不足                 | 5      |
| LivenessRemindCodeNoFaceDetected         | 没有检测到人脸           | 6      |
| LivenessRemindCodeImageBlured            | 图像模糊                 | 7      |
| LivenessRemindCodeOcclusionLeftEye       | 左眼有遮挡               | 8      |
| LivenessRemindCodeOcclusionRightEye      | 右眼有遮挡               | 9      |
| LivenessRemindCodeOcclusionNose          | 鼻子有遮挡               | 10     |
| LivenessRemindCodeOcclusionMouth         | 嘴巴有遮挡               | 11     |
| LivenessRemindCodeOcclusionLeftContour   | 左脸颊有遮挡             | 12     |
| LivenessRemindCodeOcclusionRightContour  | 右脸颊有遮挡             | 13     |
| LivenessRemindCodeOcclusionChinCoutour   | 下颚有遮挡               | 14     |
| LivenessRemindCodeTooClose               | 太近                     | 15     |
| LivenessRemindCodeTooFar                 | 太远                     | 16     |
| LivenessRemindCodeBeyondPreviewFrame     | 出框                     | 17     |
| LivenessRemindCodeLiveEye                | 眨眨眼                   | 18     |
| LivenessRemindCodeLiveMouth              | 张大嘴                   | 19     |
| LivenessRemindCodeLiveYawRight           | 向左摇头                 | 20     |
| LivenessRemindCodeLiveYawLeft            | 向右摇头                 | 21     |
| LivenessRemindCodeLivePitchUp            | 向上抬头                 | 22     |
| LivenessRemindCodeLivePitchDown          | 向下低头                 | 23     |
| LivenessRemindCodeLiveYaw                | 摇摇头                   | 24     |
| LivenessRemindCodeSingleLivenessFinished | 完成一个活体动作         | 25     |
| LivenessRemindCodeVerifyInitError        | 鉴权失败                 | 26     |
| LivenessRemindCodeVerifyDecryptError     | 鉴权失败                 | 27     |
| LivenessRemindCodeVerifyInfoFormatError  | 鉴权失败                 | 28     |
| LivenessRemindCodeVerifyExpired          | 鉴权失败                 | 29     |
| LivenessRemindCodeVerifyMissRequiredInfo | 鉴权失败                 | 30     |
| LivenessRemindCodeVerifyInfoCheckError   | 鉴权失败                 | 31     |
| LivenessRemindCodeVerifyLocalFileError   | 鉴权失败                 | 32     |
| LivenessRemindCodeVerifyRemoteDataError  | 鉴权失败                 | 33     |
| LivenessRemindCodeTimeout                | 超时                     | 34     |
| LivenessRemindCodeConditionMeet          | 条件满足                 | 35     |

## 3.6 创建实例

- 方法：FaceSDKManager.instance
- 参数：无
- 返回：人脸功能管理器
- 说明：创建人脸功能管理器。

## 3.7 设置人脸功能控制参数

- 方法

```
 // 设置鉴权
(void)setLicenseID:(NSString )licenseID andLocalLicenceFile:(NSString)localLicencePath;
// 最小检测人脸阈值
(void)setMinFaceSize:(NSInteger)width;
// 截取人脸图片大小
(void)setCropFaceSizeWidth:(CGFloat)width;
// 人脸检测精度阀值
(void)setNotFaceThreshold:(CGFloat)th; 
// 人脸遮挡阀值
(void)setOccluThreshold:(CGFloat)thr;
// 亮度阀值
(void)setIllumThreshold:(NSInteger)thr;
// 图像模糊阀值
(void)setBlurThreshold:(CGFloat)thr;
// 头部姿态角度
(void)setEulurAngleThrPitch:(NSInteger)pitch yaw:(NSInteger)yawroll:(NSInteger)roll; 
// 是否进行人脸图片质量检测
(void)setIsCheckQuality:(BOOL)isCheck;
```

## 3.8 取得人脸图像采集功能接口

- 方法

  ```
  (void)detectWithImage:(UIImage *)image completion: (void (^)(FaceInfo *faceinfo, ResultCode resultCode))completion;
  ```

- 参数

image: 采集到的图像

- 返回

faceinfo: FaceInfo 人脸检测信息

resultCode: ResultCode 返回执行结果码

- 说明

 取得人脸图像采集功能接口。人脸图像采集接口完成，解析图片人脸信息，返回检测结果。

## 3.9 取得活体检测功能接口

- 方法

  ```
  (void)livenessWithImage:(UIImage )image completion:(void (^)(FaceInfo faceinfo, LivenessState* state, ResultCode resultCode))completion;
  ```

- 参数

image: 采集到的图像

liveAction: 活体动作类型

- 返回

aceinfo: FaceInfo 人脸检测信息

livenessState:LivenessState 活体状态码

resultCode: ResultCode 返回执行结果码

- 说明

取得活体检测功能接口。活体检测功能接口完成，解析图片人脸信息，返回活体检测结果。

## 3.10 设置人脸图像采集功能参数

- 方法

  ```
  (void)detectStratrgyWithImage:(UIImage *)image previewRect:(CGRect)previewRectdetectRect:(CGRect)detectRectcompletionHandler:(DetectStrategyCompletion)completion;
  ```

- 参数

previewRect 人脸图片大小，类型：Rect

detectRect 人脸检测区域大小，类型：Rect

completion 完成后返回照片和状态结果

- 说明

检测图片中的人脸信息，完成人脸图像采集，返回检测状态和结果。

## 3.11 设置人脸功能控制参数

- 方法

```
(void)livenessStratrgyWithImage:(UIImage *)imagepreviewRect:(CGRect)previewRect detectRect:(CGRect)detectRectcompletionHandler:(LivenessStrategyCompletion)completion;
```

- 参数

previewRect 人脸图片大小，类型：Rect

detectRect 人脸检测区域大小，类型：Rect

completion 完成后返回照片和状态结果

- 说明

检测图片中的人脸信息，完成人脸图像采集和人脸活体检测，返回检测状态和结果。

## 3.12 活体动作设置

- 方法

  ```
  (void)livenesswithList:(NSArray *)array order:(BOOL)ordernumberOfLiveness:(NSInteger)numberOfLiveness
  ```

- 参数

array: 活体动作列表

order: 是否按顺序进行活体动作

numberOfLiveness: 活体动作数目（array为nil是起作用）

- 返回

 无

- 说明

 活体动作设置

# 4、常见问题

**1、license文件有什么用，该放在什么地方？**

license文件需要申请，目的是作为sdk校验开发者的使用合法性，license文件放置位置 不对或未放置license文件会导致没法使用sdk，一般应先申请license文件，并把申请得到的license文件，放置在assets目录下面。****

**2、活体检测常见有那些动作？是否可配置？**

常见有6个动作，眨眼，张大嘴，向上抬头，向下低头，向左摇头，向右摇头等。 sdk提供FaceConfig参数设置类，如活体检测角度、光线，检测动作，检测动作数量等设置。

**3、license文件失效了，不能用了怎么办？**

License文件申请时候有期限，如过期会导致校验失效，需要在后台进行申请延期。