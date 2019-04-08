# 离线识别SDK 安卓-人证核验工程



# 版本日志

| 版本   | 日期       | 说明                                    |
| :----- | :--------- | :-------------------------------------- |
| v1.1.0 | 2019.03.14 | 1、增加几款镜头模组支持；2、已知bug修复 |
| V1.0.0 | 2019.01.10 | 人证核验示例工程初版                    |

# 1、方案介绍

## 1.1 方案概述

人证核验方案，用于常见的1：1身份核验场景，用于证明用户的身份是否符实（即证明你是你）。常见的核验方式为以下几种：

- 身份证信息 vs 本人身份：常见操作为终端设备上存在身份证读卡器，用于读取身份证内部的芯片照，再与本人人脸信息进行1：1比对，确定当前待核验用户为证件本人。如交通卡口验证、身份核验机等。
- 系统ID信息 vs 本人身份：系统中记录的指定ID信息，通过人脸方式进行本人身份核验，如访客管理、会员验证、支付验证等。

核验终端设备，常置于无人看守场景，需要确保用户不可作弊，则在本人人脸获取过程中，增加**活体检测**能力，确保是**真人操作**。不同类型技术方案的活体检测，需要依赖不同的前端镜头模组，则衍生出多种硬件设备适配方案。

此示例工程旨在提供通用的人证核验功能及配套设置，并提供一定的软硬件搭配方案，帮助开发者快速完成人证核验的业务原型搭建，及效果评估。

## 1.2 适用场景

此示例工程所提供的方案，可广泛应用于1：1身份核验场景，用于满足用户**是真人且是本人**的业务诉求。

- **人证核验机**
- **自助柜机**
- **交通卡口身份核验**
- **酒店前台核验机**
- **社区门禁**
- **访客管理**
- **......**

## 1.3 功能介绍

### 1.3.1 设备激活

![img](https://ai.bdstatic.com/file/72933F6797C243E3853BA1C1F406447F)

示例工程中提供两种工程授权激活方式，皆是基于设备维度的授权：

- 在线激活：只需手动填写激活码，便可快速完成联网激活（仅需操作一次）
- 离线激活：在设备完全不可联网的情况下，可将授权文件置于SD卡中，工程会自动寻找授权文件并完成激活。

### 1.3.2 功能首页

![img](https://ai.bdstatic.com/file/4F4C486FF92E449695DC170B27EAC38E)

工程首页中，提供几种核心功能的入口和功能说明，方便快速进行演示操作和业务逻辑验证。

### 1.3.3 人证比对演示

![img](https://ai.bdstatic.com/file/1F80C7B6573B4772838C42EE24968068)

模拟真实场景下，人脸图片与证件照图片对比的业务流程，特征抽取默认使用「证件照模型」，以处理对比过程中的证件照图片特征抽取的要求（证件照图片普遍像素较低）。配套使用的活体检测功能，需要在参数设置中单独选择对应的已经适配的镜头。

### 1.3.4 活体检测演示

![img](https://ai.bdstatic.com/file/F633CA42213D43DAA855F2A3FD12E86E)

用于体验和测试活体检测效果，界面内会持续显示实时的活体检测结果，可用于评估硬件及算法在真实场景下的真实表现。具体的活体模式可以在参数设置中按需选择。

例如选择RGB+NIR活体模式，界面内则会实时显示两种图像回显，用于反馈实时的视频流画面。同时显示活体判断结果、各项检测耗时、图片预览等。可直观地用于模拟真实应用状态下的活体效果。

### 1.3.5 参数设置

**人脸采集质量参数**

![img](https://ai.bdstatic.com/file/F89150EC5D3E495D963DB9B994F04545)

如开启此项，则在人脸检测过程中，将会增加实时的人脸质量检测，包括模糊度判断、遮挡判断、光照判断。如实时图片帧不满足这几项中的任何一项阈值，则此图片帧将会被舍弃，不会送到活体步骤判断。

开启此项后，由于处理任务增加，性能开销也会对应增大。

点击「开启质量检测」，则可开启此项。

![img](https://ai.bdstatic.com/file/34787E67C72B4CF5B2A2324CE8EB0D9C)

开启质量检测后，则可以自定义三个检测项的阈值，小于此阈值的图片帧将会被过滤掉。

**活体模式设置**

![img](https://ai.bdstatic.com/file/5D8BC3911C7246C9845A956DC092693C)

工程中提供四种活体模式配置，可根据业务需要自由选择。详细活体策略介绍，请参考 [业务策略说明](http://ai.baidu.com/docs#/Face-Offline-SDK-Strategies/top)

**识别相似度阈值**

![img](https://ai.bdstatic.com/file/254AD50517044DC182E4452FC5658226)

用于设置对比相似度的阈值，实际对比得分超过已设置的阈值，则可判断为同一人，推荐阈值80分或90分。

**活体阈值设置**

![img](https://ai.bdstatic.com/file/96128DBDDC484287A25FD84F0FD0EB58)

三种活体的阈值设定，超过此阈值则判断为活体通过。

> 注意：为业务安全性考虑，如果选择多种活体方式，则每一项活体得分都超过阈值，才会判断活体通过。

**最小检测人脸设置**

![img](https://ai.bdstatic.com/file/0AE2F966FC0C436382377791A5A3F8AC)

最小检测人脸是指：视频流中可以检测到的人脸的最小面积，相同镜头焦距情况下，此数值设置的越小，人脸越容易被检索到，对应的可识别距离也就越大。

此数值通常和分辨率一同配合使用。数值可调整范围为50px至200px之间，需要同时考虑分辨率大小因素影响。

**人脸检测角度设置**

![img](https://ai.bdstatic.com/file/1E05092B2278492F97CB972ECBF2E7C1)

此项设置用于管理：从视频流中实际检测人脸的方向，分为0、90、180、270四个角度。选择固定方向后，则只有人脸在此方向角度才会被检测到（RGB）

**摄像头预览旋转角度设置**

![img](https://ai.bdstatic.com/file/40E6B3AD91B144FE89BB7CF3748A1A8B)

此项设置用于管理：摄像头输出视频流的实际预览界面角度，一经设置，将会作用于所有类型的回显画面（RGB、NIR、Depth回显）

可用于纠正摄像头原本输出图像中，人脸并没有水平朝上的问题，避免人脸检测不到的现象。

## 1.4 授权激活

详细说明请参考 [授权说明](http://ai.baidu.com/docs#/Face-Offline-SDK-Activation/top)

## 1.5 硬件选型

详细说明请参考 [设备选型](http://ai.baidu.com/docs#/Face-Offline-SDK-Equipment/top)

# 2、集成指南

## 2.1 准备工作

### 2.1.1 注册开发者

1. STEP1：点击百度AI开放平台导航右侧的控制台，页面跳转到百度云登录界面，登录完毕后，将会进入到百度云后台，点击「控制台」进入百度云控制台页面；您也可以在官网直接点击免费试用，登录完毕后将自动进入到百度云控制台。
2. STEP2：使用百度账号完成登录，如您还未持有百度账户，可以点击此处注册百度账户。
3. STEP3：进入百度云欢迎页面，填写企业和个人基本信息，注册完毕，至此成为开发者。(如您之前已经是百度云用户或百度开发者中心用户，STEP3 可略过)
4. STEP4：进入百度云控制台，找到人工智能相关服务面板。
5. STEP5：点击进入「[人脸识别](https://console.bce.baidu.com/ai/?fromai=1#/ai/face/overview/index)」
6. STEP6：点击进入「[离线SDK管理](https://console.bce.baidu.com/ai/#/ai/face/offline/index)」

### 2.1.2 设备激活

首次运行示例工程，会弹出激活窗口。1为设备号，自动读取；2为序列号，手动输入。点击激活（激活需要联网，采用的https请求，https要求设备系统时间跟请求服务器时间差距不大，否则请求授权服务器会失败），激活以后使用不需要在连接网络，一个序列号绑定一台设备，卸载应用后重新安装可能需要重新激活，可以使用同一个序列号。可以在你的控制台查看设备号对应的序列号。

SDK的激活界面如下图所示：

![img](https://ai.baidu.com/file/D08196F89C5A47478E61AF94A5BBE5D9)

## 2.2 代码结构

### 2.2.1 核心库介绍

facelibrary是SDK的依赖库。

- lib目录为动态库so和jar包
- assets目录为模型文件
- java目录为用户组管理、人脸SDK操作、视频流、图片等操作辅助类

![img](https://ai.bdstatic.com/file/BA5D157D9CBA43978554C4ADE6C0A969)

### 2.2.2 示例代码介绍

| model名称                         | 功能说明                                                     |
| :-------------------------------- | :----------------------------------------------------------- |
| FaceIdCompareActivity             | 人证对比，包括选择证件照图片和视频流实时采集人脸进行人脸识别比对，根据比对的结果分数确认是否为同一个人，核验是否通过。 |
| LivenessSettingActivity           | 活体类型设置，分为无活体、RGB活体、RGB+NIR活体、RGB+Depth活体、RGB+NIR+Depth活体。默认为不使用活体。示例工程里面进行活体设置后，后续的人脸注册、人脸1：1，1：n等操作需选择相应Activity。无活体和rgb活体需要使用单目usb摄像头，rgb+ir活体需要使用rgb+ir双目摄像头（如迪威泰、视派尔双目摄像头，具体推荐型号详见上文设备选型介绍）。RGB+Depth需要使用奥比中光mini/mini-s/Pro-S、或华捷艾米摄像头，且目前只能使用RGB+Depth活体功能。RGB+NIR+Depth的活体硬件设备适配还在开发中。 |
| CameraPreviewAngleActivity        | 摄像头预览角度设置，可以根据实际预览的情况进行摄像头预览画面角度的旋转调整，包括0,90,180,270 |
| FaceMinValueSetActivity           | 最小检测人脸设置，此数值设置的越小，人脸越容易被检索到，对应的可识别距离也就越大，可调整数值范围为50px~200px |
| FaceQualitySetActivity            | 人脸采集质量参数设置，包括模糊度，遮挡和光照的设置。开启质量检测会增加耗时，性能也会下降，默认情况下是不开启质量检测。 |
| FaceRecogniseSetActivity          | 人脸识别相似度阈值设置，默认值是90。根据次阈值可以判读人脸是否识别成功。 |
| MainActivity                      | 示例主界面入口                                               |
| FaceSettingActivity               | 参数设置界面入口                                             |
| FaceTrackAngleSetActivity         | 人脸检测角度设置，用于调整实际送去人脸检测的图片的角度，包括0,90,180,270。SDK只能识别人脸朝上的人脸。 |
| LivenesThresholdSettingActivity   | 活体阈值设置，包括RGB活体阈值，NIR活体阈值，Depth活体阈值。默认值是0.9,活体分数达到0.9左右可以判断活体通过 |
| IminectVideoTrackActivity         | 华捷艾米摄像头视频预览实时人脸活体检测特征提取               |
| OrbbecMiniVideoTrackActivity      | 奥比中光mini摄像头视频预览实时人脸活体检测特征提取           |
| OrbbecProVideoTrackSecondActivity | 奥比中光Pro摄像头视频预览实时人脸活体检测特征提取            |
| RgbNirVideoTrackActivity          | 双目近红外摄像头视频预览实时人脸活体检测特征提取             |
| RgbVideoTrackActivity             | 单目RGB可见光摄像头视频预览实时人脸活体检测特征提取          |

以上所述文件位置如下图所示：

![img](https://ai.bdstatic.com/file/FE03B9EE89684E328C4DBCD564DE40B2)

## 2.3 开始集成

接下来我们详细介绍一下集成步骤。

### 2.3.1 人脸库集成

1. 把facelibrary库添加到自己的工程中：（1）`settings.gradle` 添加`‘:facelibrary’`；（2）app->build.gradle->dependencies->compile project(":facelibrary")。
2. 根据需要把app里面的示例代码添加到自己的工程。

### 2.3.2 SDK初始化

**采用默认参数进行初始化：**

![img](https://ai.bdstatic.com/file/1564E89AE84E4FB7BB985DD3C4BB9D60)

**如果需要设置SDK的具体参数：**

![img](https://ai.bdstatic.com/file/09C6CEED3B75476897704C8D20294846)

**详细参数设置参考如下表格所示：**

| 参数                | 名称                                                   | 默认值 | 取值范围   |
| :------------------ | :----------------------------------------------------- | :----- | :--------- |
| **brightnessValue** | 图片爆光度                                             | 40f    | 50~255     |
| **blurnessValue**   | 图像模糊度                                             | 0.7f   | 0~1.0f     |
| **occlusionValue**  | 人脸遮挡阀值                                           | 0.5f   | 0~1.0f     |
| **headPitchValue**  | 低头抬头角度                                           | 15     | 0~45       |
| **headYawValue**    | 左右角度                                               | 15     | 0~45       |
| **headRollValue**   | 偏头角度                                               | 15     | 0~45       |
| **minFaceSize**     | 最小人脸检测值，小于此值的人脸将检测不出来，最小值为50 | 50     | 50~200     |
| **notFaceSize**     | 人脸置信度                                             | 0.8f   | 0~1.0f     |
| **isCheckBlur**     | 是否检测人脸模糊度                                     | False  | True/Flase |
| **isOcclusion**     | 是否检测人脸遮挡                                       | False  | True/Flase |
| **isIllumination**  | 是否检测人脸曝光度                                     | False  | True/Flase |

### 2.3.3 按设备授权

SDK初始化的时候会检测设备授权，如果没有授权，会弹出授权框，填入在平台上创建的序列号。授权成功SDK才可初始化成功。

### 2.3.4 设置Anakin核数说明

SDK在授权初始化模型之前会进行Anakin大小核数的设置，共有两处地方需要修改。 在3399板子上运行，需要设置为4个大核0个小核。在3288板子的上运行，需要设置为2个大核0小核。 示例代码中默认的是是设置为4个大核0个小核的，用户需要根据自己板子情况进行修改，参考下图：

![img](https://ai.bdstatic.com/file/FCDD885226B543259CD470A9546BD4C6)

![img](https://ai.bdstatic.com/file/68F1CA98E78143E9B40CAA1EEEE016ED)

### 2.3.5 人脸采集质量参数设置

**模型介绍**

人脸质量阈值用于判断人脸的质量，包括模糊度，遮挡和光照。当质量检测通过之后，才进行活体的检测特征抽取比对识别。

![img](https://ai.bdstatic.com/file/0094762C42FB40FFB75B1C4465A2BCF0)

**注意事项**

温馨提示：开启质量检测会增加耗时，性能也会下降，默认是不开启质量检测。

### 2.3.6 无感知活体检测模式

**活体介绍**

详见文档**1.3.2**部分

**使用方法**

| model名称                   | 功能说明                                                     |
| :-------------------------- | :----------------------------------------------------------- |
| **LivenessSettingActivity** | 活体类型设置，分为无活体、RGB活体、RGB+NIR活体、RGB+Depth活体、RGB+NIR+Depth活体。默认为不使用活体。示例工程里面进行活体设置后，后续的人脸注册、人脸1：1，1：n等操作需选择相应Activity。无活体和rgb活体需要使用单目usb摄像头，rgb+ir活体需要使用rgb+ir双目摄像头（如迪威泰、视派尔双目摄像头，具体推荐型号详见上文设备选型介绍）。RGB+Depth需要使用奥比中光mini/mini-s/Pro-S、或华捷艾米摄像头，且目前只能使用RGB+Depth活体功能。RGB+NIR+Depth的活体硬件设备适配还在开发中。 |

温馨提示：业务流程中，只能使用一种活体设置。

### 2.3.7 人脸识别相似度阈值（FaceRecogniseSetActivity）

人脸识别相似度阈值用于判断是否为同一个人，识别是否通过，包括可见光特征，证件照特征。可以根据此阈值来判断人脸是否识别成功，核验是否通过。

![img](https://ai.bdstatic.com/file/89320F8658614B0EB336E10EE5FAE083)

### 2.3.8 活体阈值设置（LivenesThresholdSettingActivity）

活体阈值用于判断是否活体通过，每种活体模型的阈值单独设定，可以根据次阈值来判断活体的通过率和拒绝率。通常情况下，真人的活体得分极大接近于1，非活体极大接近于0.0，阈值设定视安全性而定。

![img](https://ai.bdstatic.com/file/DABB5921214540768788534CBD598D0A)

### 2.3.9 最小检测人脸设置(FaceMinValueSetActivity)

最小检测人脸是指：视频流中可以检测到的人脸的最小面积，此数值设置的越小，人脸越容易被检索到，对应的可识别距离也就越大。此数值通常和分辨率一同配合使用。数值可调整范围为50px至200px之间，需要同时考虑分辨率大小因素影响。

### 2.4.0 人脸检测角度设置（FaceTrackAngleSetActivity）

此项设置用于管理：从视频流中实际检测人脸的方向，分为0、90、180、270四个角度。选择固定方向后，则只有人脸在此方向角度才会被检测到（RGB）。

**注意事项**

温馨提示：SDK只检测人脸朝上的人脸，需要确保送去检测的人脸角度是正的，朝上的。

### 2.4.1 摄像头预览旋转角度设置（CameraPreviewAngleActivity）

此项设置用于管理：摄像头输出视频流的实际预览界面角度，分为0、90、180、270四个角度。一经设置，将会作用于所有类型的回显画面（RGB、NIR、Depth回显）可用于纠正摄像头原本输出图像中，人脸并没有水平朝上的问题，避免人脸检测不到的现象。

### 2.4.2 人证对比

**2.4.2.1 从相册里选择两张图片进行对比**

此种方法无需使用人脸采集及活体等功能。

**注意事项**

> 温馨提示：SDK只检测人脸朝上的人脸。

**2.4.2.2 从视频流中采集两张人脸图片进行对比**

此种方式的人脸图片需要从视频流中实时采集，如果为无人值守情况，还需配备活体检测以保障业务安全。`FaceIdCompareActivity`根据活体策略选择相应的实现，开发者可以根据实际使用的硬件进行选择。

1）获取人脸，可选择一下5种方式返回人脸

- RgbLivessDetectActivity：无活体或RGB活体（活体检测成功后，返回检测到的人脸）
- RgbNirLivessDetectActivity：进行RGB+NIR活体成功后返回检测到RGB人脸
- OrbbecMiniVideoTrackActivity：进行RGB+Depth活体成功后返回检测到RGB人脸（奥比中光mini镜头）
- OrbbecProVideoTrackSecondActivity：进行RGB+Depth活体成功后返回检测到RGB人脸（奥比中光Pro镜头）
- IminectVideoTrackActivity：进行RGB+Depth活体成功后返回检测到RGB人脸（华捷艾米镜头）

![img](https://ai.bdstatic.com/file/0B1A11214AD6484B8B7C188C922295F2)

2）根据返回人脸抽取特征

![img](https://ai.bdstatic.com/file/2BF5AA03BBD74D38B67B16A9874DC0CB)

3）比对两张人脸图片

![img](https://ai.bdstatic.com/file/CEA1B26422BE496E8A414AEBCFA0E8A0)

### 2.4.3 活体检测（视频流）

您可以根据实际硬件选择活体策略，有下面几种实现：

- RgbVideoTrackActivity 无活体或rgb活体(单目RGB镜头)
- RgbNirVideoTrackActivity rgb+ir活体(双目近红外镜头)
- OrbbecMiniVideoTrackActivity rgb+depth活体(奥比中光mini镜头)
- OrbbecProVideoTrackSecondActivity rgb+depth活体(奥比中光Pro镜头)
- IminectVideoTrackActivity rgb+depth活体(华捷艾米镜头)

开发者可以根据活体策略和实际使用的硬件（连接的摄像头）选择相应的实现。

**2.4.3.1 RgbVideoTrackActivity**

1）初始化视频流检测

![img](https://ai.bdstatic.com/file/A132A14673024FC593DE4D49D6D6F0CF)

2）选择摄像头类型

![img](https://ai.bdstatic.com/file/6DD0BAE0ED194C08829981380C176D41)

3）设置视频流人脸检测回调，在回调进行比对

![img](https://ai.bdstatic.com/file/F26E8535A87144CF9A549419B8859043)

4）活体检测

![img](https://ai.bdstatic.com/file/D2B3A856656C4BA09E33124639CC9D2F)

5）特征抽取

![img](https://ai.bdstatic.com/file/1838564B4ECD4E118BCDCC70697101AD)

**2.4.3.2 RgbNirVideoTrackActivity**

1）初始化视频流：RGB+NIR双目摄像头通过系统api出来的视频流都是yuv420数据，需要区分出RGB还是NIR数据。通过选取一点数量点的取平均值比较，大的为RGB，小的为NIR。

![img](https://ai.bdstatic.com/file/84A1227A78B04BA58D4D02C5CB1DF35C)

2）人脸活体检测

![img](https://ai.bdstatic.com/file/39FC9BF426ED44E0B9A2EC9AD52D67C3)

3）特征抽取（与RgbVideoTrackActivity实现相同）

**2.4.3.3 OrbbecMiniVideoTrackActivity**

1）初始化视频流+人脸检测+活体：采用奥比中光双目摄像头，Depth数据为16位数据

![img](https://ai.bdstatic.com/file/FFF933E7F4834E8A929EDAFDB57B4D2D)

2）活体检测回调

![img](https://ai.bdstatic.com/file/080F77360E04462FBCA2510A24C3D960)

3）特征抽取（与RgbVideoTrackActivity实现相同）

**2.4.3.4 IminectVideoTrackActivity**

1）初始化视频流+人脸检测+活体：采用华捷艾米双目深度摄像头，Depth数据为16位数据

![img](https://ai.bdstatic.com/file/D99B356F8B4A4DB58353D953DC1829CC)

2）活体检测回调（与OrbbecMiniVideoTrackActivity实现相同）

3）特征抽取（与RgbVideoTrackActivity实现相同）

## 2.4 核心类

### 2.4.1 FaceSDKManager

- 功能：**负责授权激活、初始检测类FaceDetector、FaceFeature、FaceLiveness**
- 位于：`com.baidu.aip.manager`

### 2.4.2 FaceDetector

- 功能：**人脸检测封装类，包含人脸检测功能初始化、人脸检测接口调用**
- 位于：`com.baidu.aip.manager`

### 2.4.3 FaceFeature

- 功能：**人脸特征抽取封装类，包含人脸特征抽取功能初始化、人脸特征抽取接口调用**
- 位于：`com.baidu.aip.manager`

### 2.4.4 FaceLiveness

- 功能：**人脸活体相关操作封装类，包含人脸rgb、ir、depth活体检测**
- 位于：`com.baidu.aip.manager`

### 2.4.5 FaceEnvironment

- 功能：**人脸配置相关操作封装类，包含人脸检测的相关参数及默认值**
- 位于：`com.baidu.aip.manager`

### 2.4.6 FaceDetectManager

- 功能：**人脸图片和视频人脸检测封装类，可以接受CameraImageSource和FileImageSource源**
- 位于：`com.baidu.aip.face`

### 2.4.7 FaceAttribute

- 功能：**人脸属性相关操作封装类，包含年龄，性别，表情等**
- 位于：`com.baidu.aip.manager`

### 2.4.8 FacefeaturesImage

- 功能：**人脸特征抽取封装类，包含人脸特征抽取功能初始化、人脸特征抽取接口调用**
- 位于：`com.baidu.aip.manager`

# 3、常见问题

**Q：提示硬件指纹变化，导致激活失效？**
A：v1.0.1版本以上的SDK，已经修复了硬件指纹变化问题。但由于硬件本身的多样化原因，可能某些情况下，仍会导致指纹变化。

**Q：哪些情况可能导致指纹变化？**
A：刷机、更换硬件设备将会导致指纹变化。但安卓系统升级、APP卸载重装、恢复系统出厂值并不会导致硬件指纹变化。

**Q：同一台设备可以被多个序列号多次激活么？**
A：可以。一台设备可以被多个序列号激活，没有限制。

**Q：调用getFeature接口对图片进行特征提取，经常会出现特征提取失败的情况，错误码为6。**
A：主要可能为在人脸检测的过程中没有检测到人脸，建议调整设置下人脸的大小`set_min_face_size`的值。

**Q：视频流人脸检测和图片检测是否可以同时或间隔进行？**
A：人脸SDK为单例，同一时间只能进行一个图像源。另外进行人脸检测具体追踪功能，视频流进行检测时，不能插入其他图像帧。想`VideoMatchImageActivity`、`RgbIrVideoMatchImageActivity`、`OrbbecVideoMatchImageActivity`，需要先把图片进行人脸检测后，在把打开视频流检测。中间需要使用`FaceSDKManager.getInstance().getFaceDetector().clearTrackedFaces();`清除数据

**Q：人脸检测检测不到人脸？**
A：人脸SDK检测需要传入检测的人脸图片是人脸朝上，预览和实际传给SDK检测的图片方向不一定相同，需要把实际检测的数据转成（**argb->bitmap**）图片，显示确定人脸是否朝上。

**Q：如何调整人脸检测识别距离，以及调节检测的最小人脸？**
A：主要方法有三种，详情如下：

1. 调整`FaceDetector`初始化时最小检测人脸大小（FaceEnvironment VALUE_MIN_FACE_SIZE = 100;），可选范围为：50~200（50*50px-200*200px），最小检测人脸越小，能检测到人脸越小。最小检测人脸越小，性能消耗越大。
2. 调整人脸检测传入的图像分辨率，分辨率越大，能检测越越远。鉴于目前端设备性能，建议选择640*480，1280*720。分辨率越大，性能消耗越大。
3. 选择更大焦距的摄像头，相当于把人脸拉近。同样距离，大焦距的镜头，图像视觉越小，人脸占比越大。通常USB摄像头为3mm、6mm焦距。对性能影响小，调整人脸检测距离明显。

**Q：人脸检测返回值问题？**
A：一般反馈OK(0)表示检测到合格的人脸，当传入检测数据间隔时间较长上，超过了追踪的时间，会返回DATA_HIT_LAST(9)。所有返回9也是检测到了合格的人脸，如下所示：

```
public static enum ErrCode {
    OK,
    PITCH_OUT_OF_DOWN_MAX_RANGE,
    PITCH_OUT_OF_UP_MAX_RANGE,
    YAW_OUT_OF_LEFT_MAX_RANGE,
    YAW_OUT_OF_RIGHT_MAX_RANGE,
    POOR_ILLUMINATION,
    NO_FACE_DETECTED,
    DATA_NOT_READY,
    DATA_HIT_ONE,
    DATA_HIT_LAST,
    IMG_BLURED,
    OCCLUSION_LEFT_EYE,
    OCCLUSION_RIGHT_EYE,
    OCCLUSION_NOSE,
    OCCLUSION_MOUTH,
    OCCLUSION_LEFT_CONTOUR,
    OCCLUSION_RIGHT_CONTOUR,
    OCCLUSION_CHIN_CONTOUR,
    FACE_NOT_COMPLETE,
    UNKNOW_TYPE;

    private ErrCode() {
    }
}
```

**Q：授权失败？**
A：一个序列号只能对应一台设备，一个设备可以绑定多个序列号，测试期间的序列号有使用时间，会过期，过期后需要到AI平台上进行延期，正式使用的序列号是永久授权的。授权不过，人脸SDK将无法返回正确结果。

**Q：so加载问题？**
A：很多开发者反馈找不到so库，原因是前面只提供了`armeabi-v7a`的库，但开发者基本加了其他第三方的库`arm64-v8a`、`armeabi`、`armeabi-v7a`和`x86`等都加进去了。so的加载原理是先加载当前CPU对应的so库，比如64位的手机会先加载`arm64-v8a`，只有在没有`arm64-v8a`目录才会去其他目录（如`armeabi-v7a`）下找，所有就算只留个空`arm64-v8a`目录也不行，因为这样他只会在`arm64-v8a`目录下找，这就要求每个目录下的so齐全一致。同时也不能把`armeabi-v7a`里面的so拷到其他目录，不要看名字一样。同时加入`arm64-v8a`和`armeabi-v7a`库。这样会导致打出来的包大不少。所以如果觉的包太大，只留`armeabi-v7a`是可以，他兼容其他cpu架构。**注意：aar里面可能包含so，注意检查。**