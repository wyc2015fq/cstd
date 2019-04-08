# 离线识别SDK 安卓-人脸通行工程



# 版本日志

| 版本   | 日期       | 说明                                      |
| :----- | :--------- | :---------------------------------------- |
| v1.1.0 | 2019.03.14 | 1、增加几款结构光镜头支持；2、已知bug修复 |
| V1.0.0 | 2019.01.10 | 人脸通行示例工程初版                      |

# 1、方案介绍

## 1.1 方案概述

**闸机/门禁**是人脸识别技术的重点应用场景，人脸门禁/闸机在校园、景区、社区、展会等业务场景中应用广泛，为了便于开发者根据自己的场景化业务需求快速进行开发，提高人脸SDK/API的调用开发效率，我们基于场景化开发者对于人脸通行业务上的需求，为广大开发者提供基于业务层考虑的示例代码封装。您所需要的功能，都可以在我们提供的示例工程中快速找到该功能的实现模块并参考我们的实现方案进行开发使用。（注：本方案结合**人脸离线识别SDK**进行示例工程的开发）

> 人脸离线识别SDK包含人脸检测、活体检测、人脸比对等功能，SDK一经授权激活，可完全在无网络环境下工作，所有数据皆可在设备本地运行处理，可根据业务需求进行灵活的上层业务开发，核心能力分布如下图所示。

![img](https://ai.bdstatic.com/file/EE6702C4B20A4A9FAAC00CC9E93E8FBF)

## 1.2 适用场景

**适用场景特点**

- **网络**：业务仅运行在局域网中，无法连接公有网络。如政府单位、金融保险、教育机构等。
- **安全**：处于业务安全考虑，人脸数据具有敏感性，离线局域网方案适用于数据安全要求较高的场景。
- **速度**：离线方案运行于离线嵌入式设备，处理性能相对服务器较弱，但网络延迟影响小。
- **稳定**：设备端运行离线人脸库，避免了网络抖动、机房故障带来的稳定性影响因素。

**典型场景举例**

- **智慧校园**：使用人脸识别系统结合传统校园一卡通，实现公寓门禁、食堂计费、浴室扣费等一些列刷脸功能。
- **智慧票务**：将人脸识别系统与票务系统关联，使用人脸完成用户注册、购票、检票等票务全流程，适用于景区、剧场等。
- **智慧社区**：方案适用于智慧社区等小型安防场景，集成与社区监控系统中完成人流统计、人脸通行等核心功能。
- **企业管理**：方案可用于智能企业管理，可在此基础上根据业务需求实现人脸考勤、人脸门禁、访客签到等功能。
- **智慧酒店**：利用人脸识别能力辅助完成客人实名入住登记、人脸门锁、账户计费等功能。
- **......**

## 1.3 名词解释

- **活体检测**：利用生物特征进行活体判断，这里主要利用人脸识别技术判断图像中是否为真人人脸，用户抵抗面具、高清图片等攻击。
- **阈值**： 阈值又叫临界值，是指一个效应能够产生的最低值或最高值，人脸活体和识别一般算法输出高于阈值才会判定人脸为活体或为同一个人。
- **嵌入式设备端**：嵌入式设备主要指配备在闸机中作为人脸分析前端的中小型设备（比服务器小），包括人脸抓拍机、人脸识别平板、人脸分析盒子等。
- **人脸服务端**：人脸识别服务器，分为百度云端识别服务器或本地私有化部署人脸服务器。
- **管理服务端**：对业务场景下的人脸库、通行记录等进行管理等终端。

## 1.4 功能介绍

本节主要介绍示例工程中的核心基础业务功能，如下图为示例工程的功能结构：

![img](https://ai.bdstatic.com/file/922175E97D72494186CD4FAAFA473E82)

### 1.4.1 人脸注册

人脸注册即通过设备连接的摄像头抓取的人脸图像进行人脸检测以及特征提取将用户的人脸图片及特征值录入人脸库的过程：

- **录入姓名**：示例工程当前设计的人脸数据库存储结构为“姓名”+“人脸图片”，用户需要输入姓名并配合完成人脸抓拍方可完成人脸的录入，即人脸注册。第一步进行姓名的录入，点击“下一步”箭头图标进入下一步人脸采集。

![img](https://ai.bdstatic.com/file/293152888C30476AAF2CCA57DB059B23)

- **人脸采集**：通过摄像头采集图像并通过算法自动完成对图片中人脸进行检测及特征提取，并会将人脸图像及对应人脸特征值存至数据库中，完成信息录入。

![img](https://ai.bdstatic.com/file/96506C65F9294BF497BA58E68E7AA36A)

- **注册结果**：注册结果通知包括“注册成功”或“注册超时”信息。

![img](https://ai.bdstatic.com/file/AEF327905FC44540BB891F516A7DC798)

### 1.4.2 人脸库管理

“人脸库管理”功能演示模块包含人脸列表管理、注册图片查看、人员查询、批量导入（批量注册）、批量删除功能。

- **人脸列表**：列表展示用户通过“人脸注册”功能或“批量导入（批量注册）”功能录入到人脸库的姓名信息及头像切图。

![img](https://ai.bdstatic.com/file/3D6DF26341F64F3D98149B541253F21F)

- **注册图片查看**：点击上图中头像可查看原始注册图像。
- **人员查询**：在搜索框中输入人员姓名，可查询筛选出对应人员。（Sample中仅实现了全字段匹配检索，模糊检索开发者可根据产品或项目需求自行实现）
- **批量导入（批量注册）**：点击人脸列表上方的“批量导入”进入批量注册功能，可实现人脸批量注册，请按照示例工程APP文案提示进行操作。
- **批量删除**：人脸库管理支持数据的批量删除，点击人脸列表上方的“批量删除”后点选要删除的图片或点击“全选”选择全部图片。删除操作会将对应人员的姓名信息及图像信息全部删除。

![img](https://ai.bdstatic.com/file/53CDBB71AA964CCDA909259FCBE0EA48)

- **数据库导入/导出**：示例工程提供了方便开发者直接进行数据库导入导出操作的功能，以节省开发者在不同机器上进行大数据量底库检索耗时测试时进行批量注册的时间，用户可在一次批量注册后导出数据库以供其他机器测试使用。

### 1.4.3 通行演示

通行演示功能为示例工程重点功能，为百度官方提供的闸机/门禁通行场景的参考业务实现，支持“万”级别底库人像检索，企业开发者可参考示例工程代码逻辑对自己的项目进行功能开发。该功能支持外接单目摄像头、双目红外摄像头、双目结构光摄像头，以及对应的活体校验逻辑。为方便开发者测试当前耗时情况，我们在界面上为用户提供了如下信息：

- 底库人脸数
- 人脸检测耗时
- 特征提取耗时
- 活体检测耗时
- 人脸检索耗时（1:N检索耗时）
- RGB活体分数
- 红外活体分数
- 深度（结构光）活体分数

![img](https://ai.bdstatic.com/file/E73935ECBE7C4A898FB1AC645EF8F25A)

### 1.4.4 设置

- **无感知活体阈值设置**：调节阈值以控制活体攻击防范程度，包括“可见光活体阈值”、“红外活体阈值”、“深度活体阈值”三项设置。
- **人脸识别阈值设置**：针对应用场景来调节人脸识别阈值，人脸对比分数高于阈值则认为是同一个人。阈值设置越高则识别准确率越高，但会造成召回率下降，即本来为同一个人的两张图片被判定为非同一个人的情况会增多。建议开发者根据业务场景进行适当的实验以确定最佳阈值。
- **无感知活体检测设置**：我们为开发者提供了四种活体方案，包括“不使用活体检测”、“单目活体（RGB）”、“红外双目活体（RGB+NIR）”、“结构光双目活体（RGB+Depth）”，其中结构光活体由于需要加载驱动需要选择对应连接的摄像头型号。

![img](https://ai.bdstatic.com/file/E13A9A8EB36043AA90652A0A1441EBC8)

- **摄像头预览旋转角度设置**：摄像头输出视频流的实际预览界面角度，一经设置，将会作用于回显画面上，用于纠正摄像头原本输出图像，避免出现图像中人头方向未朝上的情况，以确保能够检测到人脸。用户可通过查看“通行演示”模块中预览图像的实际展示情况做角度调整。

![img](https://ai.bdstatic.com/file/0B14F52FAA054994B74BAF6FBE866D35)

## 1.5 授权激活

人脸通行演示方案示例工程对开发者免费提供上层应用代码，但由于目前离线识别SDK采用阶梯收费标准，并在用户成功申请离线识别SDK后发放一定数量的试用序列号，用户使用试用序列号对本示例工程中的人脸SDK进行授权及设备激活后方可进行相关测试或开发。SDK相关授权或定价可参考 [[设备激活\]](http://ai.baidu.com/docs#/Face-Offline-SDK-Activation/top)

## 1.6 硬件选型

具体硬件设备选型请参考SDK技术文档中的 [[设备选型\]](http://ai.baidu.com/docs#/Face-Offline-SDK-Equipment/top)

## 1.7 方案组合

人脸闸机/门禁由于不同场景中业务需求存在一定差异，并且依据具体情况可能需要定制化开发，综合考虑人脸识别能力在服务器、嵌入式设备端不同的表现，我们梳理了如下几种不同的技术实现方案：

![img](https://ai.bdstatic.com/file/BE1140B89AA74686BF73853BD3965C06)

- **云端识别方案**：

![img](https://ai.bdstatic.com/file/6E1AB1486BDA4ACDB7D6134B2F24FA12)

- **嵌入式识别方案**：

![img](https://ai.bdstatic.com/file/9C4C50AF68624F61B8858FBB5AA1B50E)

- **私有化部署方案**：

![img](https://ai.bdstatic.com/file/62705CE5C1D7469CA8CDCCE2EC8FD807)

# 2、集成指南

## 2.1 准备工作

### 2.1.1 注册开发者

1. STEP1：点击百度AI 开放平台导航右侧的控制台，页面跳转到百度云登录界面，登录完毕后，将会进入到百度云后台，点击「控制台」进入百度云控制台页面；您也可以在官网直接点击免费试用，登录完毕后将自动进入到百度云控制台。
2. STEP2：使用百度账号完成登录，如您还未持有百度账户，可以点击此处注册百度账户。
3. STEP3：进入百度云欢迎页面，填写企业和个人基本信息，注册完毕，至此成为开发者。(如您之前已经是百度云用户或百度开发者中心用户，STEP3 可略过。)
4. STEP4：进入百度云控制台，找到人工智能相关服务面板。
5. STEP5：点击进入「[人脸识别](https://console.bce.baidu.com/ai/?fromai=1#/ai/face/overview/index)」
6. STEP6：点击进入「[离线SDK管理](https://console.bce.baidu.com/ai/#/ai/face/offline/index)」

### 2.1.2 设备激活

首次运行示例工程，会显示“设备激活”页面，激活方式分别是：在线激活、离线激活。

（1）在线激活：

1为设备指纹，自动读取；2为序列号，手动输入。点击激活（激活需要联网，采用的https请求，https要求设备系统时间跟请求服务器时间差距不大，否则请求授权服务器会失败），激活以后使用不需要在连接网络，一个序列号绑定一台设备，卸载应用后重新安装可能需要重新激活，可以使用同一个序列号。可以在你的控制台查看设备号对应的序列号。在线激活界面如下图所示：

![img](https://ai.bdstatic.com/file/CDF17B19F2F14EA5A44E9191D8DD246A)

（2）离线激活：

离线激活需要登录百度云控制台，根据设备指纹，下载离线激活文件License.zip，将该激活文件保存在手机的SD卡根目录下，点击“离线激活”，SDK会自动进行激活处理。

## 2.2 代码结构

### 2.2.1 核心库介绍

faceLibrary库主要用于存放SDK相关的动态库以及模型文件。

-- lib目录为动态库so和jar包

-- assets目录为模型文件

facesample主要是java代码，主要包含用户组管理、人脸SDK操作、视频流、图片等操作辅助类。

![img](https://ai.bdstatic.com/file/33D6D0B51F004C0DBEFE8D9145A88C71)

### 2.2.2 示例代码介绍

| 参数名                            | 含义                                                         |
| :-------------------------------- | :----------------------------------------------------------- |
| BatchImportActivity               | 提供批量注册人脸到人脸库。                                   |
| DBImportActivity                  | 数据库文件的导入导出功能。                                   |
| ImageDetailActivity               | 用于查看图片。                                               |
| IminectPassActivity               | 带RGB+Depth活体，视频流人脸实时检测，并和某个分组的人脸库进行1：n比对。需要使用RGB+Depth的双目摄像头。如华捷艾米双目摄像头。 |
| LicenseActivity                   | 用于初始化鉴权操作。                                         |
| OrbbecMiniPassActivity            | 带RGB+Depth活体，视频流人脸实时检测，并和某个分组的人脸库进行1：n比对。需要使用RGB+Depth的双目摄像头。如奥比中光mini双目摄像头。 |
| OrbbecProPassActivity             | 带RGB+Depth活体，视频流人脸实时检测，并和某个分组的人脸库进行1：n比对。需要使用RGB+Depth的双目摄像头。如奥比中光Pro双目摄像头。 |
| PassActivity                      | 手机RGB或外接RGB活体或带RGB+NIR活体，视频流人脸实时检测，并和某个分组的人脸库进行1：n比对。需要使用RGB+NIR的双目摄像头。 |
| RegisterActivity                  | 用于进行人脸注册。                                           |
| SettingActivity                   | 设置功能，用于摄像头配置以及SDK检测参数设置。                |
| SettingCameraActivity             | 无感知检测活体设置，用于设置是否检测活体，以及设置采用单目、双目活体。 |
| SettingCameraPreviewAngleActivity | 摄像头预览旋转角度设置，用于调整预览界面人脸是否摆正。       |
| SettingDetectActivity             | 人脸质量检测阈值设置。                                       |
| SettingFeatureActivity            | 人脸识别阈值设置。                                           |
| SettingLiveActivity               | 无感知活体阈值设置。                                         |
| UserActivity                      | 用户管理，主要用于用户的导入、查找、删除。                   |

以上所述文件位置如下图所示：

![img](https://ai.bdstatic.com/file/9E20618CAC0848328A66A27774E48368)

## 2.3 开始集成

接下来我们详细介绍一下集成步骤。

### 2.3.1 人脸库集成

1、把facelibrary库下的lib、asset目录下的so库、jar包以及模型文件添加到自己的工程中。

2、根据需要把app里面的示例代码添加到自己的工程。

### 2.3.2 按设备授权（授权过程中对模型进行初始化）

（1）在线激活：

```
FaceAuth faceAuth = new FaceAuth();
faceAuth.initLicenseOnLine(this, key, new AuthCallback() {
            @Override
            public void onResponse(final int code, final String response, String licenseKey) {
                if (code == 0) {
                    // 如果激活成功
                    // 初始化模型
                    FaceSDKManager.getInstance().initModel(mContext);
                    // 初始化数据库
                    DBManager.getInstance().init(getApplicationContext());
                    finish();
                } else {
                    ToastUtils.toast(mContext, code + "  " + response);
                }
            }
        });
```

初始化模型的相关代码如下：

```
public void initModel(final Context context) {
	// 加载人脸检测模型
    faceDetector.initModel(context, "enc_detect_vgg_anakin.lite.bin",
            "small_detect.model",
            "enc_net_deploy_stageI.anakin.bin", new FaceCallback() {
                @Override
                public void onResponse(int code, String response) {
                    ToastUtils.toast(context, code + "  " + response);
                }
            });
	// 设定人脸检测配置参数，该方法采取默认参数
    faceDetector.loadConfig(getFaceEnvironmentConfig());
	// 加载特征提取模型
    faceFeature.initModel(context, "enc_sphere_resnet34_128_580_pytorch_anakin.bin",
            "enc_fsp_fc128_160_pytorch_anakin.bin",
            "", new FaceCallback() {
                @Override
                public void onResponse(int code, String response) {
                    ToastUtils.toast(context, code + "  " + response);
                    ;
                }
            });
	// 加载活体检测模型
    faceLiveness.initModel(context, "enc_model_rgb_145_anakin.bin",
            "enc_nir_anakin.lite.bin",
            "enc_depth_anakin.lite.bin", new FaceCallback() {
                @Override
                public void onResponse(int code, String response) {
                    ToastUtils.toast(context, code + "  " + response);
                }
            });
}
```

如果需要设置SDK的具体参数：

```
FaceEnvironment	faceEnvironment = new FaceEnvironment();
faceEnvironment.setMinFaceSize(50);
faceEnvironment.setMaxFaceSize(-1);
faceEnvironment.setDetectInterval(200);      
faceEnvironment.setTrackInterval(500);
faceEnvironment.setNoFaceSize(0.5f);
faceEnvironment.setPitch(30);
faceEnvironment.setYaw(30);
faceEnvironment.setRoll(30);
faceEnvironment.setCheckBlur(true);
faceEnvironment.setOcclusion(true);
faceEnvironment.setIllumination(true);
faceEnvironment.setDetectMethodType(FaceDetect.DetectType.DETECT_VIS);
```

详细参数设置参考如下表格所示：

| 参数             | 名称                     | 默认值         |
| :--------------- | :----------------------- | :------------- |
| minFaceSize      | 检测最小人脸值           | 50             |
| maxFaceSize      | 检测最大人脸值           | -1（不做限制） |
| trackInterval    | 人脸跟踪，检测的时间间隔 | 500            |
| detectInterval   | 人脸跟踪，跟踪时间间隔   | 1000           |
| noFaceSize       | 非人脸阈值               | 0.5            |
| pitch            | 抬头低头角度             | 15             |
| yaw              | 左右摇头角度             | 15             |
| yaw              | 左右摇头角度             | 15             |
| roll             | 偏头角度                 | 15             |
| isCheckBlur      | 是否进行模糊检测         | false          |
| isIllumination   | 是否进行光照检测         | false          |
| isOcclusion      | 是否进行遮挡检测         | false          |
| detectMethodType | 图片检测类型             | VIS            |

（2）离线激活

相关代码如下：

```
faceAuth.initLicenseOffLine(this, new AuthCallback() {
            @Override
            public void onResponse(final int code, final String response, String licenseKey) {
                if (code == 0) {
                    GlobalSet.FACE_AUTH_STATUS = 0;
                    // 初始化人脸
                    FaceSDKManager.getInstance().initModel(mContext);
                    // 初始化数据库
                    DBManager.getInstance().init(getApplicationContext());
                    finish();
                } else {
                    ToastUtils.toast(mContext, code + "  " + response);
                }
            }
        });
```

### 2.3.3 人脸注册（RegisterActivity）

进入该功能之后首先需要录入注册人的姓名，姓名要求不能含有特殊符号，要求不能超过10个字符，填入成功之后才可进入视频流实时采集页面。在采集页面如果30s未检测到人脸，则显示人脸注册超时。采集页面将会执行人脸检测、人脸活体检测、特征值提取功能。当返回正常时，说明注册成功。并将注册成功的图片保存到本地。

下面是活体检测相关代码：

```
FaceInfo[] faceInfos = null;
    if (mRgbArray != null) {
		// 进行人脸检测
        faceInfos = FaceSDKManager.getInstance().getFaceDetector().trackMaxFace(mRgbArray, width, height);
    }
	// 保存相关图片信息
    LivenessModel livenessModel = new LivenessModel();
    livenessModel.setRgbDetectDuration(System.currentTimeMillis() - startTime);
    livenessModel.getImageFrame().setArgb(mRgbArray);
    livenessModel.getImageFrame().setWidth(width);
    livenessModel.getImageFrame().setHeight(height);
    livenessModel.setLiveType(type);

    if (faceInfos != null && faceInfos.length > 0) {
        livenessModel.setTrackFaceInfo(faceInfos);
        FaceInfo faceInfo = faceInfos[0];
        livenessModel.setFaceInfo(faceInfo);
        livenessModel.setLandmarks(faceInfo.landmarks);

        // 返回检测到人脸的信息，用于画人脸框
        if (faceDetectCallBack != null) {
            faceDetectCallBack.onFaceDetectCallback(true, (int) faceInfo.mWidth,
                    (int) faceInfo.mWidth, (int) faceInfo.mCenter_x, (int) faceInfo.mCenter_y,
                    width, height);
        }

        if (livenessCallBack != null) {
            livenessCallBack.onTip(1, "活体判断中");
        }

        Log.e("lth_id",""+curFaceID+"   "+livenessModel.getFaceInfo().face_id);

		// 活体检测，获取活体分值
        float rgbScore = 0;
        if ((type & MASK_RGB) == MASK_RGB) {
            startTime = System.currentTimeMillis();
            rgbScore = rgbLiveness(mRgbArray, width, height, faceInfos[0].landmarks);
            livenessModel.setRgbLivenessScore(rgbScore);
            livenessModel.setRgbLivenessDuration(System.currentTimeMillis() - startTime);
        }
        ......
    }
```

下面是人脸特征提取，并将相关信息保存起来的过程：

```
public void registFace(LivenessModel livenessModel) {
	// 人脸特征提取
    byte[] bytes = new byte[512];
    float length = FaceSDKManager.getInstance().getFaceFeature().extractFeature(
            livenessModel.getImageFrame().getArgb(),
            livenessModel.getImageFrame().getHeight(),
            livenessModel.getImageFrame().getWidth(),
            bytes,
            livenessModel.getLandmarks());
	// 如果特征值提取正常
    if (length == 128) {
        Feature feature = new Feature();
        feature.setCtime(System.currentTimeMillis());
        feature.setFeature(bytes);
        feature.setUserName(registNickName);
        final String uid = UUID.randomUUID().toString();
        feature.setUserId(uid);
        feature.setGroupId("0");
        
        int imgWidth = livenessModel.getImageFrame().getWidth();
        int imgHeight = livenessModel.getImageFrame().getHeight();
        Bitmap registBmp = registBmp = Bitmap.createBitmap(imgWidth,
                imgHeight, Bitmap.Config.ARGB_8888);
        registBmp.setPixels(livenessModel.getImageFrame().getArgb(), 0, imgWidth, 0, 0, imgWidth, imgHeight);
        
        // 保存图片到新目录中
        File facePicDir = FileUtils.getFacePicDirectory();
        // 保存抠图图片到新目录中
        File faceCropDir = FileUtils.getFaceCropPicDirectory();
        String picFile = "regist_" + uid + "_rgb.png";

        if (facePicDir != null) {
            File savePicPath = new File(facePicDir, picFile);
            if (FileUtils.saveFile(savePicPath, registBmp)) {
                Log.i(TAG, "图片保存成功");
                feature.setImageName(picFile);
            }
        }

        Bitmap cropBitmap = null;
        String cropImgName = null;
        // 人脸抠图
        FaceInfo faceInfo = livenessModel.getFaceInfo();
        if (faceInfo != null) {
            cropBitmap = ImageUtils.noBlackBoundImgCrop(faceInfo.landmarks,
                    livenessModel.getImageFrame().getHeight(), livenessModel.getImageFrame().getWidth(),
                    livenessModel.getImageFrame().getArgb());

            if (cropBitmap == null) {
                cropBitmap = registBmp;
            }

            cropImgName = "crop_" + picFile;
        }
        if (faceCropDir != null && cropBitmap != null) {
            File saveCropPath = new File(faceCropDir, cropImgName);
            if (FileUtils.saveFile(saveCropPath, cropBitmap)) {
                Log.i(TAG, "抠图图片保存成功");
                feature.setCropImageName(cropImgName);
            }
        }

		// 将人脸信息保存数据库
        if (FaceApi.getInstance().featureAdd(feature)) {
			// 注册成功，返回结果
            returnRegistResult(0, livenessModel, cropBitmap);
        }
        return;
    }
}
```

### 2.3.4 通行演示（1:N）

1:N 通行主要是通过视频流的形式，将摄像头采集到的人脸与人脸库中的N张人脸图片进行特征值比对。如果比对成功，则允许通行。

您可以根据实际硬件选择活体策略，有下面几种实现：

- PassActivity 无活体或rgb活体或rgb+ir活体
- OrbbecProPassActivity rgb+depth活体
- OrbbecMiniPassActivity rgb+depth活体
- IminectPassActivity rgb+depth活体

以PassActivity实现为例

1）跟据是否是红外摄像头选择相关view

```
// 如果是NIR摄像头，则用BinocularView加载
if (GlobalSet.getLiveStatusValue() == GlobalSet.LIVE_STATUS.RGN_NIR) {
        mBinocularView = new BinocularView(mContext);
        mBinocularView.setImageView(mImageTrack);
        mBinocularView.setLivenessCallBack(this);
        mCameraView.addView(mBinocularView, layoutParams);
} else {  // 如果是RGB摄像头，则用MonocularView加载
        mMonocularView = new MonocularView(mContext);
        mMonocularView.setImageView(mImageTrack);
        mMonocularView.setLivenessCallBack(this);
        mCameraView.addView(mMonocularView, layoutParams);
}
```

2）以RGB为例，在MonocularView中实现人脸检测、活体检测、特征值对比功能

```
// 开始调用人脸相关接口
private synchronized void checkData() {
    if (rgbData != null) {
        FaceSDKManager.getInstance().getFaceLiveness().setNirRgbInt(null);
        FaceSDKManager.getInstance().getFaceLiveness().setRgbInt(rgbData);
        FaceSDKManager.getInstance().getFaceLiveness().setIrData(null);
        FaceSDKManager.getInstance().getFaceLiveness().livenessCheck(mTrackImageWidth, mTrackImageHeight,
                GlobalSet.getLiveStatusValue() == GlobalSet.LIVE_STATUS.RGB ? 0X0001 : 0X0000);
        rgbData = null;
    }
}

// 实现人脸检测、活体检测
private boolean onLivenessCheck(int width, int height, int type) {
    boolean isLiveness = false;

    long startTime = System.currentTimeMillis();
    FaceInfo[] faceInfos = null;
    if (mRgbArray != null) {
		// 人脸检测，获取人脸信息
        faceInfos = FaceSDKManager.getInstance().getFaceDetector().trackMaxFace(mRgbArray, width, height);
    }
    LivenessModel livenessModel = new LivenessModel();
    livenessModel.setRgbDetectDuration(System.currentTimeMillis() - startTime);
    livenessModel.getImageFrame().setArgb(mRgbArray);
    livenessModel.getImageFrame().setWidth(width);
    livenessModel.getImageFrame().setHeight(height);
    livenessModel.setLiveType(type);

    if (faceInfos != null && faceInfos.length > 0
            && faceInfos[0].mConf >= GlobalSet.getDetectConfValue()) {
        livenessModel.setTrackFaceInfo(faceInfos);
        FaceInfo faceInfo = faceInfos[0];
        livenessModel.setFaceInfo(faceInfo);
        livenessModel.setLandmarks(faceInfo.landmarks);

        // 返回检测到人脸的事件
        if (faceDetectCallBack != null) {
            faceDetectCallBack.onFaceDetectCallback(true, (int) faceInfo.mWidth,
                    (int) faceInfo.mWidth, (int) faceInfo.mCenter_x, (int) faceInfo.mCenter_y,
                    width, height);
        }

        if (livenessCallBack != null) {
            livenessCallBack.onTip(1, "活体判断中");
        }

        if (currentTaskType == TASK_TYPE_ONETON && livenessModel.getFaceInfo().face_id == curFaceID) {
            return true;
        }

        float rgbScore = 0;
        if ((type & MASK_RGB) == MASK_RGB) {
            startTime = System.currentTimeMillis();
			// 活体检测
            rgbScore = rgbLiveness(mRgbArray, width, height, faceInfos[0].landmarks);
            livenessModel.setRgbLivenessScore(rgbScore);
            livenessModel.setRgbLivenessDuration(System.currentTimeMillis() - startTime);
        }
        
        if (livenessCallBack != null) {
            switch (currentTaskType) {
                case TASK_TYPE_ONETON:
                    filterFeature(livenessModel);
                    break;
                default:
                    break;
            }

        }
    } else {
        // 返回检测到人脸的事件
        if (faceDetectCallBack != null) {
            faceDetectCallBack.onFaceDetectCallback(false, 0,
                    0, 0, 0, 0, 0);
        }
        if (livenessCallBack != null) {
            livenessCallBack.onCallback(1, null);
            livenessCallBack.onTip(1, "未检测到人脸");
        }
    }
    livenessModel.setTrackFaceInfo(faceInfos);
    if (livenessCallBack != null) {
        livenessCallBack.onCanvasRectCallback(livenessModel);
    }
    return isLiveness;
}

// 特征值提取
public boolean onFeatureCheck(LivenessModel livenessModel) {
    if ((GlobalSet.getLiveStatusValue() == GlobalSet.LIVE_STATUS.NO) ||
            (GlobalSet.getLiveStatusValue() == GlobalSet.LIVE_STATUS.RGB
                    && livenessModel.getRgbLivenessScore() > GlobalSet.getLiveRgbValue())) {
        byte[] visFeature = new byte[512];
        long sTime = System.currentTimeMillis();
		// 特征值提取
        float length = FaceSDKManager.getInstance().getFaceFeature().extractFeature(
                livenessModel.getImageFrame().getArgb(),
                livenessModel.getImageFrame().getHeight(),
                livenessModel.getImageFrame().getWidth(),
                visFeature,
                livenessModel.getLandmarks());
        livenessModel.setFeatureDuration(System.currentTimeMillis() - sTime);
        if (length == 128) {
            livenessModel.setFeatureByte(visFeature);
            return true;
        }
    }
    return false;
}
// 将拿到的特征进行比对，并将结果返回
public void filterFeature(LivenessModel livenessModel) {
    if (onFeatureCheck(livenessModel)) {
        long sTime = System.currentTimeMillis();
		// 方法内部实现比对，并将结果存入数据库
        Feature feature = FaceSDKManager.getInstance().getFeature(
                FaceFeature.FeatureType.FEATURE_VIS, livenessModel.getFeatureByte(), livenessModel);
        livenessModel.setCheckDuration(System.currentTimeMillis() - sTime);
        Log.e("lth_sc", "" + livenessModel.getCheckDuration());
        if (feature != null) {
            curFaceID = livenessModel.getFaceInfo().face_id;
            livenessModel.setFeature(feature);
            livenessCallBack.onCallback(0, livenessModel);
            return;
        }
    }
    livenessCallBack.onCallback(1, livenessModel);
}
```

3）摄像头配置

```
public void onResume() {
    if (mCameraNum == 0) {
        Toast.makeText(context, "未检测到摄像头", Toast.LENGTH_LONG).show();
        return;
	} else {
        try {
            Camera.CameraInfo info = new Camera.CameraInfo();
            int numCameras = Camera.getNumberOfCameras();
            for (int i = 0; i < numCameras; i++) {
				// 获取摄像头信息，判断是前置摄像头还是后置摄像头
                Camera.getCameraInfo(i, info);
                if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                    mCamera = Camera.open(i);
                    break;
                }
            }
            // 如果没有前置摄像头，则打开默认的后置摄像头
            if (mCamera == null) {
                mCamera = Camera.open(0);
            }
            // 没有摄像头时，抛出异常
            if (mCamera == null) {
                throw new RuntimeException("Unable to open camera");
            }
			
			// 设置摄像头预览角度
            mCamera.setDisplayOrientation(previewAngle);
            mPreview.setCamera(mCamera, PREFER_WIDTH, PERFER_HEIGH);
            mCamera.setPreviewCallback(new Camera.PreviewCallback() {
                @Override
                public void onPreviewFrame(byte[] data, Camera camera) {
                    dealRgb(data);
                }
            });
        } catch (RuntimeException e) {
            // Log.e(TAG, e.getMessage());
        }
    }

    if (mTextureViewOne != null) {
        mTextureViewOne.setOpaque(false);
    }
}
```

### 2.3.5 人脸库管理

1）查找功能

通过姓名查找库中对应的此人照片

```
// 搜索按钮
if (v == mImageSearch) {
    String searchContent = mEditContent.getText().toString().trim();
    // 根据关键字查找人脸库
    UserInfoManager.getInstance().getFeatureInfo(searchContent, mUserInfoListener);
}
```

2）批量导入

本功能下支持批量导入人脸图片数据，用于将图片录入到人脸库中，即批量注册。该过程不进行活体检测，仅执行批量人脸特征值生成录入，每张图片提取一张最大人脸做为注册人脸，人脸图片较多时可能会耗时较长，请耐心等待！

```
// 检测人脸，提取人脸特征值
ret = FaceApi.getInstance().getFeature(argbImg, bytes,
        FaceFeature.FeatureType.FEATURE_VIS, environment, faceInfos);
FaceInfo faceInfo = faceInfos[0];
Log.e(TAG, "live_photo = " + ret);
```

PC通过USB连接本机，找到本机跟目录，该目录下"Import Faces"目录，该目录下包含"Faces List.txt" 及 "FacePictures"目录。请不要对此目录进行更改。批量导入时会判断此目录：

```
// 获取导入图片目录 /sdcard/Import Faces/FacePictures
File batchPicDir = FileUtils.getBatchFacePicDirectory();
String[] picFiles = batchPicDir.list();
if (picFiles == null || picFiles.length == 0) {
    if (mImportListener != null) {
        mImportListener.showToastMessage("导入图片的文件夹没有图片");
    }
    return;
}
```

"FacePictures"目录下放人脸注册图片，可通过PC拷贝放入。在"Faces List.txt" 中如下将姓名与相对应图片名称按照对应格式填入或自行编写脚本写入

|  姓名  |   图片名   |
| :----: | :--------: |
| 刘德华 | 刘德华.jpg |
| 岳云鹏 | 岳云鹏.jpg |
| 王大锤 |  WDC.png   |

如导入时库中已有相同则会过滤此条导入(本程序安装后自带100张图片的批量导入示例，可直接点击体验)，准备就绪，开始导入吧！

```
// 根据姓名查询数据库与文件中对应的姓名是否相等，如果相等，则直接过滤
List<Feature> listFeatures = DBManager.getInstance().queryFeatureByName(userName);
if (listFeatures != null && listFeatures.size() > 0) {
    String msg = "与之前图片对应的姓名相同";
    // 保存失败信息
    logBuilder.append(userName + "\t" + picFile + "\t"
            + "失败" + "\t" + msg + "\n");
```

批量导入过程中如有特征未能提取，检测失败的图片，请查看日志信息，如：

```
Log.e(TAG, picFile + "未检测到人脸，可能原因：人脸太小");
String msg = "未检测到人脸，可能原因：人脸太小"
        + "（必须大于最小检测人脸minFaceSize），"
        + "或者人脸角度太大，人脸不是朝上";
logBuilder.append(userName + "\t" + picFile + "\t"
        + "失败" + "\t" + msg + "\n");
```

3）批量删除

批量删除可自由选择删除任意数据，或者全选所有。

```
// 从数据库中删除
UserInfoManager.getInstance().batchRemoveFeatureInfo(mListFeatureInfo, mUserInfoListener,
        mSelectCount);
```

4）导入/导出数据库

本功能支持将批量导入人脸图片数据从手机内存导出到SD卡中，或者将SD卡的数据文件导入到手机内存当中。如下：其中outputDBSuccess()为导出监听，importDBSuccess()为导入监听,showErrMsg()为异常监听。

```
// 复制数据库到内存
public void copyDBFileToData() {
    String sqlPath = "/sdcard/bdface/import/bdface.db";
    String newPath = "/data/data/com.baidu.idl.face.demo/databases/bdface.db";
    if (FileUtils.copyFile(sqlPath, newPath)){
        if (mDBFileListener != null) {
            mDBFileListener.importDBSuccess();
        }
    } else {
        if (mDBFileListener != null) {
            mDBFileListener.showErrMsg();
        }
    }
}
// 复制数据库到SD卡
public void copyDBFileToSDCard() {
    String sqlPath = "/data/data/com.baidu.idl.face.demo/databases/bdface.db";
    String newPath = "/sdcard/bdface/output/bdface.db";
    if (FileUtils.copyFile(sqlPath, newPath)){
        if (mDBFileListener != null) {
            mDBFileListener.outputDBSuccess();
        }
    } else {
        if (mDBFileListener != null) {
            mDBFileListener.showErrMsg();
        }
    }
}
```

（1）导入数据库：将数据库文件放置SD卡bdface/import目录下，并命名为bdface.db，点击"导入数据库"按钮，完成导入功能。

（2）导出数据库：点击"导出数据库"按钮，数据库文件(bdface.db)将会导出到bdface/outout目录下。

### 2.3.6 设置

此功能中所有改动都是将值存入SP（PreferencesUtil），等待SDK中拿到的值来进行比较。

与设置的值相对应的key在 GlobalSet 类中列出。如：

```
// 质量参数阈值类型
public static final String TYPE_BLURR_THRESHOLD = "TYPE_BLURR_THRESHOLD";
public static final String TYPE_OCCLUSION_THRESHOLD = "TYPE_OCCLUSION_THRESHOLD";
public static final String TYPE_ILLUMINATION_THRESHOLD = "TYPE_ILLUMINATION_THRESHOLD";
```

1）无感知活体阀值设置

a.可见光活体（默认值0.8）

通过算法对可见光图像中人脸进行活体判断，当算法输出的人脸活体分数大于“阀值”是判定活体校验通过。

b.红外活体（默认值0.8）

通过算法对近红外图像中人脸进行活体判断，当算法输出的人脸活体分数大于“阀值”是判定活体校验通过。

c.深度活体（默认值0.8）

通过算法对深度图像中人脸进行活体判断，当算法输出的人脸活体分数大于“阀值”是判定活体校验通过。

2）人脸识别阀值设置

a.可见光特征（默认值90）

通过算法对两张人脸进行对比得出相似度，当相似度大于“阀值”时判定两张照片为同一个人。

b.证件照特征（默认值90）

通过算法对两张人脸进行比对并得出相似度，当相似度大于“阀值”时判定两张照片为同一个人。

3）无感知活体检测设置（四选一）

a.不使用活体检测

活体检测主要用于判断当前进行对比的用户是否为真人，但如果使用场景中有固定人员看守，则无需进行活体检测，直接采集人脸。因为国体检测虽然保障了安全性，但却会影响一定的通过率。

b.单目活体（RGB）

采用单目活体模式，即使用RGB（彩色）活体检测，此方式可有效防止照片翻拍，屏幕成像等攻击，可使用市面上通用的USB单目摄像头，要求设备分辨率大于1280*720。

c.红外双目活体（RGB+NIR）

采用RGB（彩色）+NIR（近红外）双目活体模式，基于近红外成像特点，此方式下可提高对屏幕成像攻击、高清图片攻击的拒绝率。要求近红外波段860nm。

d.结构光双目活体（RGB+Depth）

采用RGB（彩色）+Depth（深度）双目活体模式，基于深度传感器特点，此方式下可提高对屏幕、高清彩打图片、视频、模具攻击的拒绝率。（三种结构光摄像头可选：奥比中光 Astra Pro/Pro S，奥比中光 Astra MiNi/MiNi S，华捷艾米 A100S+MINI）。

```
if (GlobalSet.getStructuredLightValue() == GlobalSet.STRUCTURED_LIGHT.OBI_ASTRA_PRO) {
    intent = new Intent(this, OrbbecProPassActivity.class);
} else if (GlobalSet.getStructuredLightValue() == GlobalSet.STRUCTURED_LIGHT.OBI_ASTRA_MINI) {
    intent = new Intent(this, OrbbecMiniPassActivity.class);
} else if (GlobalSet.getStructuredLightValue() == GlobalSet.STRUCTURED_LIGHT.HUAJIE_AMY_MINI) {
    intent = new Intent(this, IminectPassActivity.class);
}
```

注：请设置与您设备相对应的方式！

4）摄像头预览旋转角度设置

此项设置用于管理：摄像头输出视频流的实际预览界面角度，一经设置，将会作用于所有类型的回显画面（RGB、NIR、Depth回显）可用于纠正摄像头原本输出图像中，人脸并没有水平朝上的问题，避免人脸检测不到的现象。旋转角度分为四种0,90,180,270。 （顺时针方向）

5）设备激活

同上2.3.2按设备授权（授权过程中对模型进行初始化）

## 2.4 核心类

### 2.4.1 FaceSDKManager

- 功能：负责初始检测类FaceDetector、FaceFeature、FaceLiveness、FaceEnvironment
- com.baidu.idl.sample.manager

### 2.4.2 FaceDetector

- 功能：人脸检测封装类，包含人脸检测功能初始化、人脸检测接口调用
- com.baidu.idl.sample.manager

### 2.4.3 FaceFeatures

- 功能：人脸特征抽取封装类，包含人脸特征抽取功能初始化、人脸特征抽取接口调用
- com.baidu.idl.sample.manager

### 2.4.4 FaceLiveness

- 功能：人脸活体相关操作封装类，包含人脸rgb、ir、depth活体检测
- com.baidu.idl.sample.manager

### 2.4.5 FaceEnvironment

- 功能：人脸配置相关操作封装类，包含人脸检测的相关参数及默认值
- com.baidu.idl.sample.manager