# 离线识别SDK 安卓-SDK-接口说明



# 版本日志

| 版本   | 日期       | 更新说明                                                     |
| :----- | :--------- | :----------------------------------------------------------- |
| v2.0.1 | 2019.03.14 | 1、接口设计优化；2、增加几款结构光镜头支持；3、已知bug修复   |
| v2.0.0 | 2019.01.10 | 1、优化生活照模型精度及速度 2、优化检测模型及策略 3、优化接口设计 4、优化活体检测速度 |
| v1.1.0 | 2018.09.03 | 1、增加离线证件照特征抽取模型，可有效处理芯片照、证件照比对需求 2、增加离线人脸属性模型，支持性别、年龄等属性分析 3、增加多线程支持 4、增加离线激活支持，可导入授权文件无网激活 5、增加对奥比中光Astra Pro深度图镜头模组支持 6、增加对华捷艾米深度镜头模组支持 7、其他细节优化及已知bug修复 |
| v1.0.1 | 2018.08.03 | 1、修复设备指纹发生变化bug 2、替换近红外活体模型，优化效果 3、修复批量注册人脸到人脸库，失败问题 4、修复注册、图片人脸检测、视频返回图片抽取特征失败，经常出现检测不到人脸问题 |
| v1.0.0 | 2018.06.29 | 初版，包括离线人脸采集、离线活体检测、离线对比识别、离线人脸库管理等功能 |

# 目录

```
1、FaceAuth-鉴权接口
   1.1 鉴权-在线授权
   1.2 鉴权-SDK 压缩文件，本地鉴权
   1.3 开启底层Log输出
   1.4 设置Anakin核数
2、FaceDetect-检测接口
   2.1 检测对齐模型加载
   2.2 质量检测模型加载
   2.3 配置信息加载
   2.4 YUV图片转换ARGB
   2.5 人脸框检测
   2.6 人脸跟踪-最大人脸接口
   2.7 人脸跟踪-第一个人脸接口
   2.8 人脸跟踪-多人脸检测
   2.9 人脸图像质量检测
   2.10 检测方法类型设置
   2.11 人脸图片信息清理
3、FaceLive-活体接口
   3.1 活体模型加载
   3.2 人脸静默活体检测-RGB可见光
   3.3 人脸静默活体检测-NIR近红外或Depth深度图
4、FaceFeature-特征接口
   4.1 特征模型加载
   4.2 人脸特征提取
   4.3 人脸特征比对
5、FaceAttributes-属性接口
   5.1 属性情绪模型加载
   5.2 人脸属性检测
   5.3 人脸表情检测
6、人脸信息实体类
   6.1 基础信息实体类
   6.2 扩展信息实体类
```

# 1、FaceAuth鉴权接口

## 1.1 鉴权-在线授权

说明：用户通过申请授权码，在线授权，激活设备

```
void initLicenseOnLine(final Context context, final String licenseKey, final AuthCallback callback)
```

| 参数名     | 含义                                                         |
| :--------- | :----------------------------------------------------------- |
| context    | 当前上下文                                                   |
| licenseKey | AIPE 鉴权码                                                  |
| callback   | 鉴权结果 void onResponse(int code, String response) code 0 : 成功；code 1 加载失败 response 结果信息 |

## 1.2 鉴权-SDK 压缩文件，本地鉴权

说明：用户通过申请鉴权文件，存储在SD 卡根目录下，离线鉴权，激活设备

```
void initLicenseOffLine(final Context context, final AuthCallback callback)
```

| 参数名   | 含义                                                         |
| :------- | :----------------------------------------------------------- |
| context  | 当前上下文                                                   |
| callback | 鉴权结果 void onResponse(int code, String response) code 0 : 成功；code 1 加载失败 response 结果信息 |

## 1.3 开启底层Log输出

说明：用于Debug 时候输出LOG 详细信息

```
void setActiveLog(BDFaceLogInfo logInfo)
```

| 参数名        | 含义                                                         |
| :------------ | :----------------------------------------------------------- |
| BDFaceLogInfo | 底层log 打印 BDFACE_LOG_VALUE_MESSAGE, // 打印输出值日志 BDFACE_LOG_ERROR_MESSAGE, // 打印输出错误日志 BDFACE_LOG_ALL_MESSAGE, // 打印所有日志 |

## 1.4 设置Anakin核数

说明：根据开发板类型，设置加速对Cpu 核数依赖，调整参数，提高性能

```
void setAnakinThreadsConfigure(int flagsThreads ,int flagsCluster)
```

| 参数名       | 含义                                   |
| :----------- | :------------------------------------- |
| flagsThreads | 大核个数（建议：3288传入2，3399传入4） |
| flagsCluster | 小核个数（建议：传入0）                |

# 2、FaceDetect检测接口

## 2.1 检测对齐模型加载

说明：检测模型加载，目前支持可见光模型，近红外检测模型（非必要参数，可以为空），对齐模型

```
void initModel(final Context context, final String visModel,final String nirModel,final String alignModel, final Callback callback)
```

| 参数名     | 含义                                                         |
| :--------- | :----------------------------------------------------------- |
| context    | 上下文context                                                |
| visModel   | 可见光图片检测模型                                           |
| nirModel   | 红外图片检测模型（非必要参数，可以为空）                     |
| alignModel | 对齐类型                                                     |
| callback   | 模型加载结果 void onResponse(int code, String response) code 0 : 成功；code 1 加载失败 response 结果信息 |

## 2.2 质量检测模型加载

说明：质量检测模型加载，判断人脸遮挡信息，光照信息，模糊信息，模型包含模糊模型，遮挡信息，作用于质量检测接口

```
void initQuality(final Context context, final String blurModel, final String occlurModel, final Callback callback)
```

| 参数名      | 含义                                                         |
| :---------- | :----------------------------------------------------------- |
| context     | 上下文context                                                |
| blurModel   | 模糊检测模型                                                 |
| occlurModel | 遮挡检测模型                                                 |
| callback    | 鉴权结果 void onResponse(int code, String response) code 0 : 成功；code 1 加载失败 response 结果信息 |

## 2.3 配置信息加载

说明：检测最小人脸，是否开启内部质量检测，检测或者追踪时间间隔等配置

```
void loadConfig(BDFaceSDKConfig config)
```

| 参数名 | 含义         |
| :----- | :----------- |
| config | 参数配置实体 |

## 2.4 YUV图片转换ARGB

说明：摄像头回调数据格式转化，YUV 转为ARGB 用于检测

```
int getDataFromYUVimg(byte[] dataYUV, int[] imageData, int width, int height, int angle, int flip)
```

| 参数名    | 含义                            |
| :-------- | :------------------------------ |
| dataYUV   | YUV 图片字节数                  |
| imageData | RGBA 图片返回值                 |
| width     | 图片宽                          |
| height    | 图片高                          |
| angle     | 原图的旋转角度                  |
| flip      | 原图是否镜像（0 非镜像 1 镜像） |

## 2.5 人脸框检测

说明：人脸框检测，每一帧图片都会检测，返回基本人脸信息，可以人脸框绘制

```
FaceInfo[] detect(int[] imageData, int height, int width,int minFaceSize)
```

| 参数名      | 含义                   |
| :---------- | :--------------------- |
| imageData   | ARGB图片像素点         |
| height      | 图片高                 |
| width       | 图片宽                 |
| minFaceSize | 需要检测的最小人脸尺寸 |

| 返回                                                         |
| :----------------------------------------------------------- |
| 成功则返回 FaceInfo[] 数组，包含：人脸框width，人脸angle，人脸框中心坐标x,y，人脸可信度mConf |

```
	           if (faceDetect == null) {
                    Toast.makeText(mContext,
                            "未初始化检测模型", Toast.LENGTH_SHORT).show();
                    return;
                }
                faceDetect.setDetectMethodType(FaceDetect.DetectType.DETECT_VIS);
                FaceInfo[] faceInfos = faceDetect.detect(
                        liveVisImg.data, liveVisImg.height, liveVisImg.width, 50);
                if (faceInfos != null && faceInfos.length > 0) {
                    FaceInfo faceInfo = faceInfos[0];
                    StringBuilder info = new StringBuilder();
                    info.append("face info length:").append(faceInfos.length)
                            .append(" face_id:").append(faceInfo.face_id)
                            .append(" width:").append(faceInfo.mWidth)
                            .append(" score:").append(faceInfo.mConf);
                    Toast.makeText(mContext,
                            info,
                            Toast.LENGTH_SHORT).show();
                    Log.e("handler", info.toString());
                } else {
                    String info = " face info length 0";
                    Toast.makeText(mContext,
                            info, Toast.LENGTH_SHORT).show();
                    Log.e("handler", info);
                }
```

## 2.6 人脸跟踪-最大人脸接口

说明：人脸跟踪检测，追踪图片中最大人脸信息，接口包含检测和跟踪功能，返回基本人脸信息和72 关键点，可以绘制人脸框，描绘眼耳鼻嘴关键点，也可作用于后续活体，特征抽取入参。

```
FaceInfo[] trackMaxFace(int[] imageData, int height, int width)
```

| 参数名    | 含义           |
| :-------- | :------------- |
| imageData | ARGB图片像素点 |
| height    | 图片高         |
| width     | 图片宽         |

| 返回                                                         |
| :----------------------------------------------------------- |
| 成功则返回 FaceInfo[] 数组，包含：人脸框width，人脸angle，人脸框中心坐标x,y，人脸可信度mConf，72个关键点landmark，人脸face_id，三个姿态角度headPose，动作活体数据is_live，用户根据自己需求选择 |

```
                if (faceDetect == null) {
                    Toast.makeText(mContext,
                            "未初始化检测模型", Toast.LENGTH_SHORT).show();
                    return;
                }
                faceDetect.setDetectMethodType(FaceDetect.DetectType.DETECT_VIS);
                FaceInfo[] faceInfos = faceDetect.trackMaxFace(
                        liveVisImg.data, liveVisImg.height, liveVisImg.width
                );
                if (faceInfos != null && faceInfos.length > 0) {
                    FaceInfo faceInfo = faceInfos[0];
                    StringBuilder info = new StringBuilder();
                    info.append("face info length:").append(faceInfos.length)
                            .append(" face_id:").append(faceInfo.face_id)
                            .append(" width:").append(faceInfo.mWidth)
                            .append(" score:").append(faceInfo.mConf);
                    if (faceInfo.occlu != null && faceInfo.occlu.length == 7) {
                        info.append("face occlusion: ")
                                .append(" L eye:").append(faceInfo.occlu[0])
                                .append(" R eye:").append(faceInfo.occlu[1])
                                .append(" nose:").append(faceInfo.occlu[2])
                                .append(" mouth:").append(faceInfo.occlu[3])
                                .append(" L contour:").append(faceInfo.occlu[4])
                                .append(" R contour:").append(faceInfo.occlu[5])
                                .append(" chin contour:").append(faceInfo.occlu[6]);
                        info.append("face blur: ").append(faceInfo.blur);
                        info.append("face illum: ").append(faceInfo.illum);
                    }
                    Toast.makeText(mContext,
                            info,
                            Toast.LENGTH_SHORT).show();
                    Log.e("handler", info.toString());
                } else {
                    String info = " face info length 0";
                    Toast.makeText(mContext,
                            info, Toast.LENGTH_SHORT).show();
                    Log.e("handler", info);
                }
                faceDetect.clearTrackedFaces();
```

## 2.7 人脸跟踪-第一个人脸接口

说明：人脸跟踪检测，追踪图片中第一个人脸信息，接口包含检测和跟踪功能，返回基本人脸信息和72 关键点，可以绘制人脸框，描绘眼耳鼻嘴关键点，也可作用于后续活体，特征抽取入参。

```
FaceInfo[] trackFirstFace(int[] imageData, int height, int width) 
```

| 参数名    | 含义           |
| :-------- | :------------- |
| imageData | ARGB图片像素点 |
| height    | 图片高         |
| width     | 图片宽         |

| 返回                                                         |
| :----------------------------------------------------------- |
| 成功则返回 FaceInfo[] 数组，包含：人脸框width，人脸angle，人脸框中心坐标x,y，人脸可信度mConf，72个关键点landmark，人脸face_id，三个姿态角度headPose，动作活体数据is_live，用户根据自己需求选择 |

## 2.8 人脸跟踪-多人脸检测

说明：人脸跟踪检测，追踪图片中多个人脸信息，通过参数num 配置，接口包含检测和跟踪功能，返回基本人脸信息和72 关键点，可以绘制人脸框，描绘眼耳鼻嘴关键点，也可作用于后续活体，特征抽取入参。

```
FaceInfo[] track(int[] imageData, int height, int width, int num)
```

| 参数名    | 含义             |
| :-------- | :--------------- |
| imageData | ARGB图片像素点   |
| height    | 图片高           |
| width     | 图片宽           |
| num       | 最大跟踪人脸个数 |

| 返回                                                         |
| :----------------------------------------------------------- |
| 成功则返回 FaceInfo[] 数组，包含：人脸框width，人脸angle，人脸框中心坐标x,y，人脸可信度mConf，72个关键点landmark，人脸face_id，三个姿态角度headPose，动作活体数据is_live，用户根据自己需求选择 |

## 2.9 人脸图像质量检测

说明：单个原子方法质量检测方法（也可以配置BDFaceSDKConfig 中的 isCheckBlur 等三个参数，在trackMaxFace 和 trackFirstFace 方法内部生效）

```
int imgQuality(int[] imageData, int height, int width, int[] landmarks,float[] bluriness, int[] illum, float[] occlusion, int[] nOccluPart)
```

| 参数名     | 含义                            |
| :--------- | :------------------------------ |
| imageData  | 图片像素点                      |
| height     | 图片高                          |
| width      | 图片宽                          |
| landmark   | 人脸72个关键点FaceInfo.landmark |
| bluriness  | 模糊结果                        |
| illum      | 光照结果                        |
| occlusion  | 遮挡结果                        |
| nOccluPart | 遮挡部位                        |

| 返回         |
| :----------- |
| 成功则返回 1 |

```
                if (faceDetect == null) {
                    Toast.makeText(mContext,
                            "未初始化检测模型", Toast.LENGTH_SHORT).show();
                    return;
                }
                faceDetect.setDetectMethodType(FaceDetect.DetectType.DETECT_VIS);
                FaceInfo[] faceInfos = faceDetect.trackMaxFace(
                        liveVisImg.data, liveVisImg.height, liveVisImg.width);
                if (faceInfos != null && faceInfos.length > 0) {
                    FaceInfo faceInfo = faceInfos[0];
                    // 质量检测通过加入人脸列表
                    float[] bluriness = new float[1];
                    int[] illum = new int[1];
                    float[] occlusion = new float[7];
                    int[] nOccluPart = new int[1];
                    faceDetect.imgQuality(liveVisImg.data, liveVisImg.height,
                            liveVisImg.width, faceInfo.landmarks, bluriness, illum, occlusion, nOccluPart);
                    StringBuilder info = new StringBuilder();
                    info.append("face occlusion: ")
                            .append(" L eye:").append(occlusion[0])
                            .append(" R eye:").append(occlusion[1])
                            .append(" nose:").append(occlusion[2])
                            .append(" mouth:").append(occlusion[3])
                            .append(" L contour:").append(occlusion[4])
                            .append(" R contour:").append(occlusion[5])
                            .append(" chin contour:").append(occlusion[6]);
                    info.append("face blur: ").append(bluriness[0]);
                    info.append("face illum: ").append(illum[0]);
                    Toast.makeText(mContext,
                            info,
                            Toast.LENGTH_SHORT).show();
                    Log.e("handler", info.toString());
                } else {
                    String info = " face info length 0";
                    Toast.makeText(mContext,
                            info, Toast.LENGTH_SHORT).show();
                    Log.e("handler", info);
                }
                faceDetect.clearTrackedFaces();
```

## 2.10 检测方法类型设置

说明：检测方法类型设置，建议FACEBOX_VIS 可见光检测

```
void setDetectMethodType(DetectType detectMethodType)
```

| 参数名           | 含义                                                         |
| :--------------- | :----------------------------------------------------------- |
| detectMethodType | 图片检测类型(目前两种支持FACEBOX_VIS和FACEBOX_NIR，建议FACEBOX_VIS 可见光检测) |

## 2.11 人脸图片信息清理

说明：清空trackMaxFace 和 trackFirstFace 接口检测缓存数据，在注册或者识别成功之后，可以调用该接口

```
void clearTrackedFaces()
```

# 3、FaceLive活体接口

## 3.1 活体模型加载

说明：静默活体检测模型初始化，可见光活体模型，近红外活体模型，深度活体模型初始化

```
void initModel(final Context context,final String visModel,final String nirModel,final String depthModel,final Callback callback)
```

| 参数名     | 含义                                                         |
| :--------- | :----------------------------------------------------------- |
| context    | 上下文context                                                |
| visModel   | 可见光图片活体模型                                           |
| nirModel   | 红外图片活体模型                                             |
| depthModel | 深度图活体模型                                               |
| callback   | 模型加载结果 void onResponse(int code, String response) code 0 : 成功；code 1 加载失败 response 结果信息 |

## 3.2 人脸静默活体检测-RGB可见光

说明：可见光静默活体分值检测，返回0-1结果，建议超过0.9 为活体

```
float silentLive(LiveType liveType, int[] imageData, int height, int width, int[] landmarks)
```

| 参数名    | 含义                                                     |
| :-------- | :------------------------------------------------------- |
| liveType  | FaceLive.LiveType.LIVEID_VIS 可见光图像静默活体检测<br / |
| imageData | ARGB图片像素点                                           |
| height    | 图片高                                                   |
| width     | 图片宽                                                   |
| landmark  | 人脸72个关键点FaceInfo.landmark                          |

| 返回               |
| :----------------- |
| 成功则返回活体分值 |

```
                if (faceDetect == null) {
                    Toast.makeText(mContext,
                            "未初始化检测模型", Toast.LENGTH_SHORT).show();
                    return;
                }

                if (faceLive == null) {
                    Toast.makeText(mContext,
                            "未初始化活体模型", Toast.LENGTH_SHORT).show();
                    return;
                }
                faceDetect.setDetectMethodType(FaceDetect.DetectType.DETECT_VIS);

                FaceInfo[] faceInfos = faceDetect.trackMaxFace(
                        liveVisImg.data, liveVisImg.height, liveVisImg.width
                );

                if (faceInfos != null && faceInfos.length > 0) {
                    FaceInfo faceInfo = faceInfos[0];

                    float visScore = faceLive.silentLive(FaceLive.LiveType.LIVEID_VIS,
                            liveVisImg.data, liveVisImg.height, liveVisImg.width,
                            faceInfo.landmarks);
                    float nirScore = faceLive.silentLive(FaceLive.LiveType.LIVEID_NIR,
                            liveVisImg.dataByte, liveVisImg.height, liveVisImg.width,
                            faceInfo.landmarks);
                    float depthScore = faceLive.silentLive(FaceLive.LiveType.LIVEID_DEPTH,
                            liveDepthImg.dataByte, liveVisImg.height, liveVisImg.width,
                            faceInfo.landmarks);
                    StringBuilder builder = new StringBuilder();
                    builder.append("live-vis").append(visScore)
                            .append("  live-nir").append(nirScore)
                            .append("  live-depth").append(depthScore);
```

## 3.3 人脸静默活体检测-NIR近红外或Depth深度图

说明：近红外和深度静默活体分值检测，返回0-1结果，建议超过0.9 为活体；红外和深度为byte 字节数组

```
float silentLive(LiveType liveType, byte[] imageData, int height, int width, int[] landmarks)
```

| 参数名    | 含义                                                         |
| :-------- | :----------------------------------------------------------- |
| liveType  | FaceLive.LiveType.LIVEID_NIR近红外图像静默活体检测 FaceLive.LiveType.LIVEID_DEPTH 深度图静默活体检测 |
| imageData | 灰度图片像素点或深度图片像素点                               |
| height    | 图片高                                                       |
| width     | 图片宽                                                       |
| landmark  | 人脸72个关键点FaceInfo.landmark                              |

| 返回               |
| :----------------- |
| 成功则返回活体分值 |

# 4、FaceFeature特征接口

## 4.1 特征模型加载

说明：离线特征获取模型加载，目前支持可见光模型，近红外检测模型（非必要参数，可以为空），证件照模型；用户根据自己场景，选择相应场景模型

```
initModel(final Context context,final String idPhotoModel,final String visModel,final String nirModel,final Callback callback)
```

| 参数名       | 含义                                                         |
| :----------- | :----------------------------------------------------------- |
| context      | 上下文context                                                |
| idPhotoModel | 证件照图片模型                                               |
| visModel     | 可见光图片模型                                               |
| nirModel     | 红外图片模型（非必要参数，可以为空）                         |
| callback     | 模型加载结果 void onResponse(int code, String response) code 0 : 成功；code 1 加载失败 response 结果信息 |

## 4.2 人脸特征提取

说明：离线特征提取接口，通过featureType 提取不同图片特征数据，函数返回特征个数，特征存储在feature 参数中

```
float feature(FeatureType featureType, int[] imageData, int height, int width, int[] landmarks, byte[] feature)
```

| 参数名      | 含义                                                         |
| :---------- | :----------------------------------------------------------- |
| featureType | FeatureType.FEATURE_VIS生活照 FeatureType.FEATURE_ID_PHOTO证件照照 |
| imageData   | 图片像素点                                                   |
| height      | 图片高                                                       |
| width       | 图片宽                                                       |
| landmark    | 人脸72个关键点FaceInfo.landmark                              |
| feature     | 人脸特征 feature 数组，默认初始化512空字节                   |

| 返回                 |
| :------------------- |
| 成功则返回512 特征点 |

```
                if (faceDetect == null) {
                    Toast.makeText(mContext,
                            "未初始化检测模型", Toast.LENGTH_SHORT).show();
                    return;
                }

                if (faceFeature == null) {
                    Toast.makeText(mContext,
                            "未初始化特征模型", Toast.LENGTH_SHORT).show();
                    return;
                }
                faceDetect.setDetectMethodType(FaceDetect.DetectType.DETECT_VIS);

                FaceInfo[] faceInfos = faceDetect.trackMaxFace(
                        featureIdPImg.data, featureIdPImg.height, featureIdPImg.width
                );

                byte[] feature1 = new byte[512];
                if (faceInfos != null && faceInfos.length > 0) {
                    FaceInfo faceInfo = faceInfos[0];
                    float length = faceFeature.feature(FaceFeature.FeatureType.FEATURE_VIS,
                            featureIdPImg.data, featureIdPImg.height, featureIdPImg.width, faceInfo.landmarks, feature1);
                    Log.e("handler", "VIS feature 1 length " + length);
                }
                faceDetect.clearTrackedFaces();

                FaceInfo[] faceInfos2 = faceDetect.trackMaxFace(
                        featureVisImg.data, featureVisImg.height, featureVisImg.width
                );

                byte[] feature2 = new byte[512];
                if (faceInfos2 != null && faceInfos2.length > 0) {
                    FaceInfo faceInfo2 = faceInfos2[0];
                    float length = faceFeature.feature(FaceFeature.FeatureType.FEATURE_VIS,
                            featureVisImg.data, featureVisImg.height, featureVisImg.width, faceInfo2.landmarks, feature2);
                    Log.e("handler", "VIS feature 2 length " + length);
                }
                faceDetect.clearTrackedFaces();

                float result = faceFeature.featureCompare(FaceFeature.FeatureType.FEATURE_VIS,
                        feature1, feature2);
```

## 4.3 人脸特征比对

说明：离线特征比对结果，分值为0--100 之间

```
float featureCompare(FeatureType featureType, byte[] feature1, byte feature2)
```

| 参数名      | 含义                                                         |
| :---------- | :----------------------------------------------------------- |
| featureType | FeatureType.FEATURE_VIS生活照 FeatureType.FEATURE_ID_PHOTO证件照照 |
| feature1    | 特征1                                                        |
| feature2    | 特征2                                                        |

| 返回                          |
| :---------------------------- |
| 成功则返回 比对结果（0--100） |

# 5、FaceAttributes属性接口

## 5.1 属性情绪模型加载

说明：人脸属性（年龄，性别，戴眼镜等），情绪（喜怒哀乐）模型初始化

```
void initModel(final Context context,final String atttibuteModel,final String emotionModel,final Callback callback)
```

| 参数名         | 含义                                                         |
| :------------- | :----------------------------------------------------------- |
| context        | 上下文context                                                |
| atttibuteModel | 属性模型                                                     |
| emotionModel   | 情绪模型                                                     |
| callback       | 模型加载结果 void onResponse(int code, String response) code 0 : 成功；code 1 加载失败 response 结果信息 |

## 5.2 人脸属性检测

说明：获取人脸属性信息，包含年龄，表情，种族，性别，是否佩戴眼镜

```
BDFaceSDKAttribute attribute(int[] imageData, int height, int width, int[] landmarks)
```

| 参数名    | 含义                            |
| :-------- | :------------------------------ |
| imageData | ARGB图片像素点                  |
| height    | 图片高                          |
| width     | 图片宽                          |
| landmark  | 人脸72个关键点FaceInfo.landmark |

| 返回                          |
| :---------------------------- |
| 成功则返回 BDFaceSDKAttribute |

```
                if (faceDetect == null) {
                    Toast.makeText(mContext,
                            "未初始化检测模型", Toast.LENGTH_SHORT).show();
                    return;
                }

                if (faceAttributes == null) {
                    Toast.makeText(mContext,
                            "未初始化属性模型", Toast.LENGTH_SHORT).show();
                    return;
                }

                faceDetect.setDetectMethodType(FaceDetect.DetectType.DETECT_VIS);

                FaceInfo[] faceInfos = faceDetect.trackMaxFace(
                        liveVisImg.data, liveVisImg.height, liveVisImg.width);

                if (faceInfos != null && faceInfos.length > 0) {
                    FaceInfo faceInfo = faceInfos[0];
                    BDFaceSDKAttribute attribute = faceAttributes.attribute(liveVisImg.data,
                            liveVisImg.height, liveVisImg.width,
                            faceInfo.landmarks);

                    BDFaceSDKEmotions emotions = faceAttributes.emotions(liveVisImg.data,
                            liveVisImg.height, liveVisImg.width,
                            faceInfo.landmarks);

                    final StringBuilder builder = new StringBuilder();
                    if (attribute != null) {
                        builder.append(" attribute:");
                        builder.append(attribute.age);
                        builder.append("    ").append(attribute.emotion);
                        builder.append("    ").append(attribute.gender);
                        builder.append("    ").append(attribute.glasses);
                    }
                    if (emotions != null) {
                        builder.append("\n emotion:");
                        builder.append("    ").append(emotions.emotion);
                        builder.append("    ").append(emotions.expression_conf);
                    }
                    Log.e("handler", "attribute  " + builder.toString());

                }
                faceDetect.clearTrackedFaces();
```

## 5.3 人脸表情检测

说明：获取人脸表情信息，包含生气，开心，厌恶，害怕，惊讶等

```
BDFaceSDKEmotions emotions(int[] imageData, int height, int width, int[] landmarks) 
```

| 参数名    | 含义                            |
| :-------- | :------------------------------ |
| imageData | ARGB图片像素点                  |
| height    | 图片高                          |
| width     | 图片宽                          |
| landmark  | 人脸72个关键点FaceInfo.landmark |

| 返回                        |
| :-------------------------- |
| 成功则返回BDFaceSDKEmotions |

## 6、人脸信息实体类

## 6.1 基础信息实体类

```
/**
 * 人脸信息实体类
 */

public class FaceInfo {
    public float mWidth; // rectangle width
    public float mAngle; // rectangle tilt angle [-45 45] in degrees
    public float mCenter_y;   // rectangle center y
    public float mCenter_x;  // rectangle center x
    public float mConf; // face detection score

    public int[] landmarks;
    public int face_id;
    public float[] headPose;
    public int[] is_live;

    public float illum = 0f;
    public float blur = 0f;
    public float[] occlu;

    public FaceInfo(float width, float angle, float y, float x, float conf) {
        mWidth = width;
        mAngle = angle;
        mCenter_y = y;
        mCenter_x = x;
        mConf = conf;
        landmarks = null;
        face_id = 0;
    }

    public FaceInfo(float width, float angle, float y, float x, float conf, int track_id, int[] ldmks) {
        mWidth = width;
        mAngle = angle;
        mCenter_y = y;
        mCenter_x = x;
        mConf = conf;
        landmarks = ldmks;
        face_id = track_id;
    }

    public FaceInfo(float width, float angle, float y, float x, float conf,
                    int track_id, int[] ldmks, float[] pose, int[] livestatus,
                    float illum,float blur,float[] occlu
    ) {
        mWidth = width;
        mAngle = angle;
        mCenter_y = y;
        mCenter_x = x;
        mConf = conf;
        landmarks = ldmks;
        face_id = track_id;
        headPose = pose;
        is_live = livestatus;
        this.illum = illum;
        this.blur = blur;
        this.occlu = occlu;
    }
}
```

## 6.2 扩展信息实体类

```
// 图片信息类
public class BDFaceSDKImageInfo {
    public int height;                              // 图片高度
    public int widht;                              // 图片宽度
    public int[] data;                            // 图片数据
    public BDFaceSDKCommon.BDFaceImageType type;  // 图片格式

    public BDFaceSDKImageInfo(int height, int widht, int[] data, int type) {
        this.height = height;
        this.widht = widht;
        this.data = data;
        this.type = BDFaceSDKCommon.BDFaceImageType.values()[type];
    }
}

public class BDFaceSDKConfig {
    /**
     * 最小人脸检测大小 建议50
     */
    public int minFaceSize = 50;

    /**
     * 最大人脸检测大小 建议-1(不做限制)
     */
    public int maxFaceSize = -1;

    /**
     * 人脸跟踪，检测的时间间隔 默认 500ms
     */
    public int trackInterval = 0;

    /**
     * 人脸跟踪，跟踪时间间隔 默认 1000ms
     */
    public int detectInterval = 0;

    /**
     * 人脸置信度阈值，建议值0.5
     */
    public float noFaceSize = 0.5f;

    /**
     * 人脸姿态角 pitch,yaw,roll
     */
    public int pitch;
    public int yaw;
    public int roll;

    /**
     * 质量检测模糊，遮挡，光照，默认不做质量检测
     */
    public boolean isCheckBlur = true;
    public boolean isOcclusion = true;
    public boolean isIllumination = true;

    /**
     * 检测图片类型，可见光或者红外
     */
    public FaceDetect.DetectType detectMethodType
            = FaceDetect.DetectType.DETECT_VIS;
}

// 人脸属性
public class BDFaceSDKAttribute {
    public int age;                                    // 年龄
    public BDFaceSDKCommon.BDFaceRace race;            // 种族
    public BDFaceSDKCommon.BDFaceEmotion emotion;      // 表情
    public BDFaceSDKCommon.BDFaceGlasses glasses;      // 戴眼镜状态
    public BDFaceSDKCommon.BDFaceGender gender;        // 性别

    public BDFaceSDKAttribute(int age, int race, int emotion, int glasses, int gender) {
        this.age = age;
        this.race = BDFaceSDKCommon.BDFaceRace.values()[race];
        this.emotion = BDFaceSDKCommon.BDFaceEmotion.values()[emotion];
        this.gender = BDFaceSDKCommon.BDFaceGender.values()[gender];
        this.glasses = BDFaceSDKCommon.BDFaceGlasses.values()[glasses];
    }
}

// 人脸情绪
public class BDFaceSDKEmotions {
    public BDFaceSDKCommon.BDFaceEmotionEnum emotion;
    public float expression_conf;
    public float[] expression_conf_list;

    public BDFaceSDKEmotions(int emotion, float expression_conf, float[] expression_conf_list) {
        this.emotion = BDFaceSDKCommon.BDFaceEmotionEnum.values()[emotion];
        this.expression_conf = expression_conf;
        this.expression_conf_list = expression_conf_list;
    }
}
    // 图像类型
    public enum BDFaceImageType {
        BD_FACE_IMAGE_TYPE_RGB,         // rgb图像
        BD_FACE_IMAGE_TYPE_BGR,         // bgr图像
        BD_FACE_IMAGE_TYPE_RGBA,        // rgba图像
        BD_FACE_IMAGE_TYPE_BGRA,        // bgra图像
        BD_FACE_IMAGE_TYPE_GRAY,        // 灰度图
        BD_FACE_IMAGE_TYPE_DEPTH,       // 深度图
        BD_FACE_IMAGE_TYPE_YUV422,      // YUV422图像
        BD_FACE_IMAGE_TYPE_YUV_411,     // YUV 411图像
        BD_FACE_IMAGE_TYPE_YUV_420,     // YUV 411图像
        BD_FACE_IMAGE_TYPE_YUV_YUYV,    // YUV YUYV图像
        BD_FACE_IMAGE_TYPE_YUV_YU12,    // YUV YU12图像
        BD_FACE_IMAGE_TYPE_YUV_NV12,    // YUV NV12图像
        BD_FACE_IMAGE_TYPE_YUV_NV21,    // YUV NV21图像
    }

    // 表情类型
    public enum BDFaceEmotion {
        BDFACE_EMOTION_NEUTRAL,         // 中性表情
        BDFACE_EMOTION_SMILE,           // 微笑
        BDFACE_EMOTION_BIG_SMILE,       // 大笑
    }

    // 情绪
    public enum BDFaceEmotionEnum {
        BDFACE_EMOTIONS_ANGRY,          // 生气
        BDFACE_EMOTIONS_DISGUST,        // 恶心
        BDFACE_EMOTIONS_FEAR,           // 害怕
        BDFACE_EMOTIONS_HAPPY,          // 开心
        BDFACE_EMOTIONS_SAD,            // 伤心
        BDFACE_EMOTIONS_SURPRISE,       // 惊讶
        BDFACE_EMOTIONS_NEUTRAL,        // 无情绪
    };

    // 人脸属性种族
    public enum BDFaceRace {
        BDFACE_RACE_YELLOW,       // 黄种人
        BDFACE_RACE_WHITE,        // 白种人
        BDFACE_RACE_BLACK,        // 黑种人
        BDFACE_RACE_INDIAN,       // 印度人
    }

    // 戴眼镜状态
    public enum BDFaceGlasses {
        BDFACE_NO_GLASSES,   // 无眼镜
        BDFACE_GLASSES,      // 有眼镜
        BDFACE_SUN_GLASSES,  // 墨镜
    }

    // 性别
    public enum BDFaceGender {
        BDFACE_GENDER_FEMALE, // 女性
        BDFACE_GENDER_MALE,   // 男性
    }
    /**
     * log种类枚举
     */
    public enum BDFaceLogInfo {
        BDFACE_LOG_VALUE_MESSAGE,   // 打印输出值日志
        BDFACE_LOG_ERROR_MESSAGE,   // 打印输出错误日志
        BDFACE_LOG_ALL_MESSAGE,     // 打印所有日志
    };
```