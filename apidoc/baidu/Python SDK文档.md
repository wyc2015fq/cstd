# Python SDK文档

# 简介

Hi，您好，欢迎使用百度人脸识别服务。

本文档主要针对Python开发者，描述百度人脸识别接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**；
- 如有疑问，进入[AI社区交流](http://ai.baidu.com/forum/topic/list/165)：http://ai.baidu.com/forum/topic/list/165

## 接口能力

| 接口名称 | 接口能力简要描述                                             |
| :------- | :----------------------------------------------------------- |
| 人脸检测 | 检测人脸并定位，返回五官关键点，及人脸各属性值               |
| 人脸比对 | 返回两两比对的人脸相似值                                     |
| 人脸查找 | 在一个人脸集合中找到找到相似的人脸，由一系列接口组成，包括人脸识别、人脸认证、人脸库管理相关接口（人脸注册、人脸更新、人脸删除、用户信息查询、组列表查询、组内用户列表查询、组间复制用户、组内删除用户） |

## 版本更新记录

| 上线日期   | 版本号 | 更新内容                       |
| :--------- | :----- | :----------------------------- |
| 2018.5.10  | 2.2.4  | 修复人脸V3问题                 |
| 2018.4.28  | 2.2.3  | 全面切换为人脸V3接口           |
| 2018.4.9   | 2.2.2  | 新增身份验证，在线活体检测接口 |
| 2018.01.12 | 2.1.0  | 新增M：N多人脸识别             |
| 2017.12.22 | 2.0.0  | SDK代码重构                    |
| 2017.5.11  | 1.0.0  | 人脸识别服务上线               |

# 快速入门

## 安装人脸识别 Python SDK

**人脸识别 Python SDK目录结构**

```
├── README.md
├── aip                   //SDK目录
│   ├── __init__.py       //导出类
│   ├── base.py           //aip基类
│   ├── http.py           //http请求
│   └── face.py //人脸识别
└── setup.py              //setuptools安装
```

**支持Python版本：2.7.+ ,3.+**

**安装使用Python SDK有如下方式**：

- 如果已安装pip，执行`pip install baidu-aip`即可。
- 如果已安装setuptools，执行`python setup.py install`即可。

## 新建AipFace

AipFace是人脸识别的Python SDK客户端，为使用人脸识别的开发人员提供了一系列的交互方法。

参考如下代码新建一个AipFace：

```
from aip import AipFace

""" 你的 APPID AK SK """
APP_ID = '你的 App ID'
API_KEY = '你的 Api Key'
SECRET_KEY = '你的 Secret Key'

client = AipFace(APP_ID, API_KEY, SECRET_KEY)
```

在上面代码中，常量`APP_ID`在百度云控制台中创建，常量`API_KEY`与`SECRET_KEY`是在创建完毕应用后，系统分配给用户的，均为字符串，用于标识用户，为访问做签名验证，可在AI服务控制台中的**应用列表**中查看。

**注意**：如您以前是百度云的老用户，其中`API_KEY`对应百度云的“Access Key ID”，`SECRET_KEY`对应百度云的“Access Key Secret”。

## 配置AipFace

如果用户需要配置AipFace的网络请求参数(一般不需要配置)，可以在构造AipFace之后调用接口设置参数，目前只支持以下参数：

| 接口                         | 说明                                           |
| :--------------------------- | :--------------------------------------------- |
| setConnectionTimeoutInMillis | 建立连接的超时时间（单位：毫秒                 |
| setSocketTimeoutInMillis     | 通过打开的连接传输数据的超时时间（单位：毫秒） |

# 接口说明

## 人脸检测

**人脸检测**：检测图片中的人脸并标记出位置信息;

```
image = "取决于image_type参数，传入BASE64字符串或URL字符串或FACE_TOKEN字符串"

imageType = "BASE64"

""" 调用人脸检测 """
client.detect(image, imageType);

""" 如果有可选参数 """
options = {}
options["face_field"] = "age"
options["max_face_num"] = 2
options["face_type"] = "LIVE"

""" 带参数调用人脸检测 """
client.detect(image, imageType, options)
```

**人脸检测 请求参数详情**

| 参数名称     | 是否必选 | 类型   | 默认值 | 说明                                                         |
| :----------- | :------- | :----- | :----- | :----------------------------------------------------------- |
| image        | 是       | string |        | 图片信息(**总数据大小应小于10M**)，图片上传方式根据image_type来判断 |
| image_type   | 是       | string |        | 图片类型 **BASE64**:图片的base64值，base64编码后的图片数据，需urlencode，编码后的图片大小不超过2M；**URL**:图片的 URL地址( 可能由于网络等原因导致下载图片时间过长)**；FACE_TOKEN**: 人脸图片的唯一标识，调用人脸检测接口时，会为每个人脸图片赋予一个唯一的FACE_TOKEN，同一张图片多次检测得到的FACE_TOKEN是同一个 |
| face_field   | 否       | string |        | 包括**age,beauty,expression,faceshape,gender,glasses,landmark,race,quality,facetype信息** 逗号分隔. 默认只返回face_token、人脸框、概率和旋转角度 |
| max_face_num | 否       | string | 1      | 最多处理人脸的数目，默认值为1，仅检测图片中面积最大的那个人脸；**最大值10**，检测图片中面积最大的几张人脸。 |
| face_type    | 否       | string |        | 人脸的类型 **LIVE**表示生活照：通常为手机、相机拍摄的人像图片、或从网络获取的人像图片等**IDCARD**表示身份证芯片照：二代身份证内置芯片中的人像照片 **WATERMARK**表示带水印证件照：一般为带水印的小图，如公安网小图 **CERT**表示证件照片：如拍摄的身份证、工卡、护照、学生证等证件图片 默认**LIVE** |

**人脸检测 返回数据参数详情**

| 字段              | 必选 | 类型   | 说明                                                         |
| :---------------- | :--- | :----- | :----------------------------------------------------------- |
| face_num          | 是   | int    | 检测到的图片中的人脸数量                                     |
| face_list         | 是   | array  | 人脸信息列表，具体包含的参数参考下面的列表。                 |
| +face_token       | 是   | string | 人脸图片的唯一标识                                           |
| +location         | 是   | array  | 人脸在图片中的位置                                           |
| ++left            | 是   | double | 人脸区域离左边界的距离                                       |
| ++top             | 是   | double | 人脸区域离上边界的距离                                       |
| ++width           | 是   | double | 人脸区域的宽度                                               |
| ++height          | 是   | double | 人脸区域的高度                                               |
| ++rotation        | 是   | int64  | 人脸框相对于竖直方向的顺时针旋转角，[-180,180]               |
| +face_probability | 是   | double | 人脸置信度，范围【0~1】，代表这是一张人脸的概率，0最小、1最大。 |
| +angel            | 是   | array  | 人脸旋转角度参数                                             |
| ++yaw             | 是   | double | 三维旋转之左右旋转角[-90(左), 90(右)]                        |
| ++pitch           | 是   | double | 三维旋转之俯仰角度[-90(上), 90(下)]                          |
| ++roll            | 是   | double | 平面内旋转角[-180(逆时针), 180(顺时针)]                      |
| +age              | 否   | double | 年龄 ，当**face_field包含age时返回**                         |
| +beauty           | 否   | int64  | 美丑打分，范围0-100，越大表示越美。当**face_fields包含beauty时返回** |
| +expression       | 否   | array  | 表情，当 **face_field包含expression时返回**                  |
| ++type            | 否   | string | **none**:不笑；**smile**:微笑；**laugh**:大笑                |
| ++probability     | 否   | double | 表情置信度，范围【0~1】，0最小、1最大。                      |
| +face_shape       | 否   | array  | 脸型，当**face_field包含faceshape时返回**                    |
| ++type            | 否   | double | **square**: 正方形 **triangle**:三角形 **oval**: 椭圆 **heart**: 心形 **round**: 圆形 |
| ++probability     | 否   | double | 置信度，范围【0~1】，代表这是人脸形状判断正确的概率，0最小、1最大。 |
| +gender           | 否   | array  | 性别，**face_field包含gender时返回**                         |
| ++type            | 否   | string | male:**男性** female:**女性**                                |
| ++probability     | 否   | double | 性别置信度，范围【0~1】，0代表概率最小、1代表最大。          |
| +glasses          | 否   | array  | 是否带眼镜，**face_field包含glasses时返回**                  |
| ++type            | 否   | string | **none**:无眼镜，**common**:普通眼镜，**sun**:墨镜           |
| ++probability     | 否   | double | 眼镜置信度，范围【0~1】，0代表概率最小、1代表最大。          |
| +race             | 否   | array  | 人种 **face_field包含race时返回**                            |
| ++type            | 否   | string | **yellow**: 黄种人 **white**: 白种人 **black**:黑种人 **arabs**: **阿拉伯人** |
| ++probability     | 否   | double | 人种置信度，范围【0~1】，0代表概率最小、1代表最大。          |
| +face_type        | 否   | array  | 真实人脸/卡通人脸 **face_field包含face_type时返回**          |
| ++type            | 否   | string | **human**: 真实人脸 **cartoon**: 卡通人脸                    |
| ++probability     | 否   | double | 人脸类型判断正确的置信度，范围【0~1】，0代表概率最小、1代表最大。 |
| +landmark         | 否   | array  | 4个关键点位置，左眼中心、右眼中心、鼻尖、嘴中心。**face_field包含landmark时返回** |
| +landmark72       | 否   | array  | 72个特征点位置 **face_field包含landmark时返回**              |
| +quality          | 否   | array  | 人脸质量信息。**face_field包含quality时返回**                |
| ++occlusion       | 否   | array  | 人脸各部分遮挡的概率，范围[0~1]，0表示完整，1表示不完整      |
| +++left_eye       | 否   | double | 左眼遮挡比例                                                 |
| +++right_eye      | 否   | double | 右眼遮挡比例                                                 |
| +++nose           | 否   | double | 鼻子遮挡比例                                                 |
| +++mouth          | 否   | double | 嘴巴遮挡比例                                                 |
| +++left_cheek     | 否   | double | 左脸颊遮挡比例                                               |
| +++right_cheek    | 否   | double | 右脸颊遮挡比例                                               |
| +++chin           | 否   | double | 下巴遮挡比例                                                 |
| ++blur            | 否   | double | 人脸模糊程度，范围[0~1]，0表示清晰，1表示模糊                |
| ++illumination    | 否   | double | 取值范围在[0~255], 表示脸部区域的光照程度 越大表示光照越好   |
| ++completeness    | 否   | int64  | 人脸完整度，0或1, 0为人脸溢出图像边界，1为人脸都在图像边界内 |
| +parsing_info     | 否   | string | 人脸分层结果 结果数据是使用gzip压缩后再base64编码 使用前需base64解码后再解压缩 **原数据格式为string 形如0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,…** |

**人脸检测 返回示例**

```
{
  "face_num": 1,
  "face_list": [
        {
            "face_token": "35235asfas21421fakghktyfdgh68bio",
            "location": {
                "left": 117,
                "top": 131,
                "width": 172,
                "height": 170,
                "rotation": 4
            },
            "face_probability": 1,
            "angle" :{
                 "yaw" : -0.34859421849251
                 "pitch" 1.9135693311691
                 "roll" :2.3033397197723
            }
            "landmark": [
                {
                    "x": 161.74819946289,
                    "y": 163.30244445801
                },
                ...
            ],
            "landmark72": [
                {
                    "x": 115.86531066895,
                    "y": 170.0546875
                }，
                ...
            ],
            "age": 29.298097610474,
            "beauty": 55.128883361816,
            "expression": {
                "type": "smile",
                "probability" : 0.5543018579483
            },
            "gender": {
                "type": "male",
                "probability": 0.99979132413864
            },
            "glasses": {
            "type": "sun",
                "probability": 0.99999964237213
            },
            "race": {
                "type": "yellow",
                "probability": 0.99999976158142
            },
            "face_shape": {
                "type": "triangle",
                "probability": 0.5543018579483
            }
            "quality": {
                "occlusion": {
                    "left_eye": 0,
                    "right_eye": 0,
                    "nose": 0,
                    "mouth": 0,
                    "left_cheek": 0.0064102564938366,
                    "right_cheek": 0.0057411273010075,
                    "chin": 0
                },
                "blur": 1.1886881756684e-10,
                "illumination": 141,
                "completeness": 1
            }
        }
    ]
}
```

**72个关键点分布图（对应landmark72个点的顺序，序号从0-71）：https://ai.bdstatic.com/file/52BC00FFD4754A6298D977EDAD033DA0

## 人脸搜索

- **1：N人脸搜索**：也称为1：N识别，在指定人脸集合中，找到最相似的人脸；
- **1：N人脸认证**：基于uid维度的1：N识别，由于uid已经锁定固定数量的人脸，所以检索范围更聚焦；

> **1：N人脸识别**与**1：N人脸认证**的差别在于：人脸搜索是在指定人脸集合中进行直接地人脸检索操作，而人脸认证是基于uid，先调取这个uid对应的人脸，再在这个uid对应的人脸集合中进行检索（因为每个uid通常对应的只有一张人脸，所以通常也就变为了1：1对比）；实际应用中，人脸认证需要用户或系统先输入id，这增加了验证安全度，但也增加了复杂度，具体使用哪个接口需要视您的业务场景判断。

```
image = "取决于image_type参数，传入BASE64字符串或URL字符串或FACE_TOKEN字符串"

imageType = "BASE64"

groupIdList = "3,2"

""" 调用人脸搜索 """
client.search(image, imageType, groupIdList);

""" 如果有可选参数 """
options = {}
options["quality_control"] = "NORMAL"
options["liveness_control"] = "LOW"
options["user_id"] = "233451"
options["max_user_num"] = 3

""" 带参数调用人脸搜索 """
client.search(image, imageType, groupIdList, options)
```

**人脸搜索 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 默认值 | 说明                                                         |
| :--------------- | :------- | :----- | :----- | :----------------------------------------------------------- |
| image            | 是       | string |        | 图片信息(**总数据大小应小于10M**)，图片上传方式根据image_type来判断 |
| image_type       | 是       | string |        | 图片类型 **BASE64**:图片的base64值，base64编码后的图片数据，需urlencode，编码后的图片大小不超过2M；**URL**:图片的 URL地址( 可能由于网络等原因导致下载图片时间过长)**；FACE_TOKEN**: 人脸图片的唯一标识，调用人脸检测接口时，会为每个人脸图片赋予一个唯一的FACE_TOKEN，同一张图片多次检测得到的FACE_TOKEN是同一个 |
| group_id_list    | 是       | string |        | 从指定的group中进行查找 用逗号分隔，**上限20个**             |
| quality_control  | 否       | string | NONE   | 图片质量控制 **NONE**: 不进行控制 **LOW**:较低的质量要求 **NORMAL**: 一般的质量要求 **HIGH**: 较高的质量要求 **默认 NONE** |
| liveness_control | 否       | string | NONE   | 活体检测控制 **NONE**: 不进行控制 **LOW**:较低的活体要求(高通过率 低攻击拒绝率) **NORMAL**: 一般的活体要求(平衡的攻击拒绝率, 通过率) **HIGH**: 较高的活体要求(高攻击拒绝率 低通过率) **默认NONE** |
| user_id          | 否       | string |        | 当需要对特定用户进行比对时，指定user_id进行比对。即人脸认证功能。 |
| max_user_num     | 否       | string |        | 查找后返回的用户数量。返回相似度最高的几个用户，默认为1，最多返回20个。 |

**人脸搜索 返回数据参数详情**

| 字段       | 必选 | 类型   | 说明                      |
| :--------- | :--- | :----- | :------------------------ |
| face_token | 是   | string | 人脸标志                  |
| user_list  | 是   | array  | 匹配的用户信息列表        |
| +group_id  | 是   | string | 用户所属的group_id        |
| +user_id   | 是   | string | 用户的user_id             |
| +user_info | 是   | string | 注册用户时携带的user_info |
| +score     | 是   | float  | 用户的匹配得分            |

**人脸搜索 返回示例**

```
{
  "face_token": "fid",
  "user_list": [
     {
        "group_id" : "test1",
        "user_id": "u333333",
        "user_info": "Test User",
        "score": 99.3
    }
  ]
}
```

## 人脸注册

用于从人脸库中新增用户，可以设定多个用户所在组，及组内用户的人脸图片，

典型应用场景：构建您的人脸库，如**会员人脸注册**，**已有用户补全人脸信息**等。

人脸库、用户组、用户、用户下的人脸**层级关系**如下所示：

```
|- 人脸库
   |- 用户组一
      |- 用户01
         |- 人脸
      |- 用户02
         |- 人脸
         |- 人脸
         ....
       ....
   |- 用户组二
   |- 用户组三
   |- 用户组四
   ....
```

**关于人脸库的设置限制**

- 每个开发者账号可以创建100个appid；
- **每个appid对应一个人脸库，且不同appid之间，人脸库互不相通**；
- 每个人脸库下，可以创建多个用户组，用户组（group）数量**没有限制**；
- 每个用户组（group）下，可添加最多**无限**张人脸，**无限**个uid；
- 每个用户（uid）所能注册的最大人脸数量**没有限制**；

为了保证识别效果，请控制注册人脸的质量（通过`/detect`人脸检测接口判断），具体参数可详见下表所示：

**质量判断**

可通过人脸检测接口，基于以下字段和对应阈值，进行质量检测的判断，以保证人脸质量符合后续业务操作要求。

| 指标           | 字段与解释                                                   | 推荐数值界限                                                 |
| :------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| **遮挡范围**   | **occlusion**（0~1），0为无遮挡，1是完全遮挡 含有多个具体子字段，表示脸部多个部位 通常用作判断头发、墨镜、口罩等遮挡 | left_eye : 0.6, #左眼被遮挡的阈值 right_eye : 0.6, #右眼被遮挡的阈值 nose : 0.7, #鼻子被遮挡的阈值 mouth : 0.7, #嘴巴被遮挡的阈值 left_check : 0.8, #左脸颊被遮挡的阈值 right_check : 0.8, #右脸颊被遮挡的阈值 chin_contour : 0.6, #下巴被遮挡阈值 |
| **模糊度范围** | **Blur**（0~1），0是最清晰，1是最模糊                        | 小于0.7                                                      |
| **光照范围**   | **illumination**（0~255） 脸部光照的灰度值，0表示光照不好 以及对应客户端SDK中，YUV的Y分量 | 大于40                                                       |
| **姿态角度**   | **Pitch**：三维旋转之俯仰角度[-90(上), 90(下)] **Roll**：平面内旋转角[-180(逆时针), 180(顺时针)] **Yaw**：三维旋转之左右旋转角[-90(左), 90(右)] | 分别小于20度                                                 |
| **人脸完整度** | **completeness**（0或1），0为人脸溢出图像边界，1为人脸都在图像边界内 | 视业务逻辑判断                                               |
| **人脸大小**   | 人脸部分的大小 建议长宽像素值范围：80*80~200*200             | 人脸部分不小于**100\*100**像素                               |

```
image = "取决于image_type参数，传入BASE64字符串或URL字符串或FACE_TOKEN字符串"

imageType = "BASE64"

groupId = "group1"

userId = "user1"

""" 调用人脸注册 """
client.addUser(image, imageType, groupId, userId);

""" 如果有可选参数 """
options = {}
options["user_info"] = "user's info"
options["quality_control"] = "NORMAL"
options["liveness_control"] = "LOW"

""" 带参数调用人脸注册 """
client.addUser(image, imageType, groupId, userId, options)
```

**人脸注册 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 默认值 | 说明                                                         |
| :--------------- | :------- | :----- | :----- | :----------------------------------------------------------- |
| image            | 是       | string |        | 图片信息(**总数据大小应小于10M**)，图片上传方式根据image_type来判断 |
| image_type       | 是       | string |        | 图片类型 **BASE64**:图片的base64值，base64编码后的图片数据，需urlencode，编码后的图片大小不超过2M；**URL**:图片的 URL地址( 可能由于网络等原因导致下载图片时间过长)**；FACE_TOKEN**: 人脸图片的唯一标识，调用人脸检测接口时，会为每个人脸图片赋予一个唯一的FACE_TOKEN，同一张图片多次检测得到的FACE_TOKEN是同一个 |
| group_id         | 是       | string |        | 用户组id（由数字、字母、下划线组成），长度限制128B           |
| user_id          | 是       | string |        | 用户id（由数字、字母、下划线组成），长度限制128B             |
| user_info        | 否       | string |        | 用户资料，长度限制256B                                       |
| quality_control  | 否       | string | NONE   | 图片质量控制 **NONE**: 不进行控制 **LOW**:较低的质量要求 **NORMAL**: 一般的质量要求 **HIGH**: 较高的质量要求 **默认 NONE** |
| liveness_control | 否       | string | NONE   | 活体检测控制 **NONE**: 不进行控制 **LOW**:较低的活体要求(高通过率 低攻击拒绝率) **NORMAL**: 一般的活体要求(平衡的攻击拒绝率, 通过率) **HIGH**: 较高的活体要求(高攻击拒绝率 低通过率) **默认NONE** |

**人脸注册 返回数据参数详情**

| 字段       | 必选 | 类型   | 说明                                           |
| :--------- | :--- | :----- | :--------------------------------------------- |
| log_id     | 是   | uint64 | 请求标识码，随机数，唯一                       |
| face_token | 是   | string | 人脸图片的唯一标识                             |
| location   | 是   | array  | 人脸在图片中的位置                             |
| +left      | 是   | double | 人脸区域离左边界的距离                         |
| +top       | 是   | double | 人脸区域离上边界的距离                         |
| +width     | 是   | double | 人脸区域的宽度                                 |
| +height    | 是   | double | 人脸区域的高度                                 |
| +rotation  | 是   | int64  | 人脸框相对于竖直方向的顺时针旋转角，[-180,180] |

**人脸注册 返回示例**

```
{
  "face_token": "2fa64a88a9d5118916f9a303782a97d3",
  "location": {
      "left": 117,
      "top": 131,
      "width": 172,
      "height": 170,
      "rotation": 4
  }
}
```

## 人脸更新

用于对人脸库中指定用户，更新其下的人脸图像。

> **说明：**针对一个uid执行更新操作，新上传的人脸图像将覆盖此uid原有所有图像。

> **说明：**执行更新操作，如果该uid不存在时，会返回错误。如果添加了action_type:replace,则不会报错，并自动注册该uid，操作结果等同注册新用户。

```
image = "取决于image_type参数，传入BASE64字符串或URL字符串或FACE_TOKEN字符串"

imageType = "BASE64"

groupId = "group1"

userId = "user1"

""" 调用人脸更新 """
client.updateUser(image, imageType, groupId, userId);

""" 如果有可选参数 """
options = {}
options["user_info"] = "user's info"
options["quality_control"] = "NORMAL"
options["liveness_control"] = "LOW"

""" 带参数调用人脸更新 """
client.updateUser(image, imageType, groupId, userId, options)
```

**人脸更新 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 默认值 | 说明                                                         |
| :--------------- | :------- | :----- | :----- | :----------------------------------------------------------- |
| image            | 是       | string |        | 图片信息(**总数据大小应小于10M**)，图片上传方式根据image_type来判断 |
| image_type       | 是       | string |        | 图片类型 **BASE64**:图片的base64值，base64编码后的图片数据，需urlencode，编码后的图片大小不超过2M；**URL**:图片的 URL地址( 可能由于网络等原因导致下载图片时间过长)**；FACE_TOKEN**: 人脸图片的唯一标识，调用人脸检测接口时，会为每个人脸图片赋予一个唯一的FACE_TOKEN，同一张图片多次检测得到的FACE_TOKEN是同一个 |
| group_id         | 是       | string |        | 更新指定groupid下uid对应的信息                               |
| user_id          | 是       | string |        | 用户id（由数字、字母、下划线组成），长度限制128B             |
| user_info        | 否       | string |        | 用户资料，长度限制256B                                       |
| quality_control  | 否       | string | NONE   | 图片质量控制 **NONE**: 不进行控制 **LOW**:较低的质量要求 **NORMAL**: 一般的质量要求 **HIGH**: 较高的质量要求 **默认 NONE** |
| liveness_control | 否       | string | NONE   | 活体检测控制 **NONE**: 不进行控制 **LOW**:较低的活体要求(高通过率 低攻击拒绝率) **NORMAL**: 一般的活体要求(平衡的攻击拒绝率, 通过率) **HIGH**: 较高的活体要求(高攻击拒绝率 低通过率) **默认NONE** |

**人脸更新 返回数据参数详情**

| 字段       | 必选 | 类型   | 说明                                           |
| :--------- | :--- | :----- | :--------------------------------------------- |
| log_id     | 是   | uint64 | 请求标识码，随机数，唯一                       |
| face_token | 是   | string | 人脸图片的唯一标识                             |
| location   | 是   | array  | 人脸在图片中的位置                             |
| +left      | 是   | double | 人脸区域离左边界的距离                         |
| +top       | 是   | double | 人脸区域离上边界的距离                         |
| +width     | 是   | double | 人脸区域的宽度                                 |
| +height    | 是   | double | 人脸区域的高度                                 |
| +rotation  | 是   | int64  | 人脸框相对于竖直方向的顺时针旋转角，[-180,180] |

**人脸更新 返回示例**

```
{
  "face_token": "2fa64a88a9d5118916f9a303782a97d3",
  "location": {
      "left": 117,
      "top": 131,
      "width": 172,
      "height": 170,
      "rotation": 4
  }
}
```

## 人脸删除

用于从人脸库中删除一个用户。

**人脸删除注意事项：**

- 删除的内容，包括用户所有图像和身份信息；
- 如果一个uid存在于多个用户组内，将会同时将从各个组中把用户删除
- 如果指定了group_id，则只删除此group下的uid相关信息

```
userId = "user1"

groupId = "group1"

faceToken = "face_token_23123"

""" 调用人脸删除 """
client.faceDelete(userId, groupId, faceToken);
```

**人脸删除 请求参数详情**

| 参数名称   | 是否必选 | 类型   | 说明                                                         |
| :--------- | :------- | :----- | :----------------------------------------------------------- |
| user_id    | 是       | string | 用户id（由数字、字母、下划线组成），长度限制128B             |
| group_id   | 是       | string | 用户组id（由数字、字母、下划线组成），长度限制128B           |
| face_token | 是       | string | 需要删除的人脸图片token，（由数字、字母、下划线组成）长度限制64B |

**人脸删除 返回数据参数详情**

| 字段   | 是否必选 | 类型   | 说明                   |
| :----- | :------- | :----- | :--------------------- |
| log_id | 是       | uint64 | 请求唯一标识码，随机数 |

**人脸删除 返回示例**

```
// 删除成功
{
  "error_code": 0,
  "log_id": 73473737,
}
// 删除发生错误
{
  "error_code": 223106,
  "log_id": 1382953199,
  "error_msg": "face is not exist"
}
```

## 用户信息查询

获取人脸库中某个用户的信息(user_info信息和用户所属的组)。

```
userId = "user1"

groupId = "group1"

""" 调用用户信息查询 """
client.getUser(userId, groupId);
```

**用户信息查询 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                               |
| :------- | :------- | :----- | :------------------------------------------------- |
| user_id  | 是       | string | 用户id（由数字、字母、下划线组成），长度限制128B   |
| group_id | 是       | string | 用户组id（由数字、字母、下划线组成），长度限制128B |

**用户信息查询 返回数据参数详情**

| 字段       | 必选 | 类型   | 说明                         |
| :--------- | :--- | :----- | :--------------------------- |
| log_id     | 是   | uint64 | 请求标识码，随机数，唯一     |
| user_list  | 是   | array  | 查询到的用户列表             |
| +user_info | 是   | string | 用户资料，被查询用户的资料   |
| +group_id  | 是   | string | 用户组id，被查询用户的所在组 |

**用户信息查询 返回示例**

```
{
  "user_list": [
      {
          "user_info": "user info ...",
          "group_id": "gid1"
      },
      {
          "user_info": "user info2 ...",
          "group_id": "gid2"
      }
  ]
}
```

## 获取用户人脸列表

用于获取一个用户的全部人脸列表。

```
userId = "user1"

groupId = "group1"

""" 调用获取用户人脸列表 """
client.faceGetlist(userId, groupId);
```

**获取用户人脸列表 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                               |
| :------- | :------- | :----- | :------------------------------------------------- |
| user_id  | 是       | string | 用户id（由数字、字母、下划线组成），长度限制128B   |
| group_id | 是       | string | 用户组id（由数字、字母、下划线组成），长度限制128B |

**获取用户人脸列表 返回数据参数详情**

| 字段        | 必选 | 类型   | 说明                     |
| :---------- | :--- | :----- | :----------------------- |
| log_id      | 是   | uint64 | 请求标识码，随机数，唯一 |
| face_list   | 是   | array  | 人脸列表                 |
| +face_token | 是   | string | 人脸图片的唯一标识       |
| +ctime      | 是   | string | 人脸创建时间             |

**获取用户人脸列表 返回示例**

```
{
  "face_list": [
      {
          "face_token": "fid1",
          "ctime": "2018-01-01 00:00:00"
      },
      {
          "face_token": "fid2",
          "ctime": "2018-01-01 10:00:00"
      }
  ]
}
```

## 获取用户列表

用于查询指定用户组中的用户列表。

```
groupId = "group1"

""" 调用获取用户列表 """
client.getGroupUsers(groupId);

""" 如果有可选参数 """
options = {}
options["start"] = 0
options["length"] = 50

""" 带参数调用获取用户列表 """
client.getGroupUsers(groupId, options)
```

**获取用户列表 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 默认值 | 说明                                               |
| :------- | :------- | :----- | :----- | :------------------------------------------------- |
| group_id | 是       | string |        | 用户组id（由数字、字母、下划线组成），长度限制128B |
| start    | 否       | string | 0      | 默认值0，起始序号                                  |
| length   | 否       | string | 100    | 返回数量，默认值100，最大值1000                    |

**获取用户列表 返回数据参数详情**

| 字段         | 必选 | 类型  | 说明       |
| :----------- | :--- | :---- | :--------- |
| user_id_list | 是   | array | 用户ID列表 |

**获取用户列表 返回示例**

```
{
  "user_id_list": [
      "uid1",
      "uid2"
  ]
}
```

## 复制用户

用于将已经存在于人脸库中的用户**复制到一个新的组**。

```
userId = "user1"

""" 调用复制用户 """
client.userCopy(userId);

""" 如果有可选参数 """
options = {}
options["src_group_id"] = "11111"
options["dst_group_id"] = "222222"

""" 带参数调用复制用户 """
client.userCopy(userId, options)
```

**复制用户 请求参数详情**

| 参数名称     | 是否必选 | 类型   | 说明                                             |
| :----------- | :------- | :----- | :----------------------------------------------- |
| user_id      | 是       | string | 用户id（由数字、字母、下划线组成），长度限制128B |
| src_group_id | 否       | string | 从指定组里复制信息                               |
| dst_group_id | 否       | string | 需要添加用户的组id                               |

**复制用户 返回数据参数详情**

| 字段   | 必选 | 类型 | 说明   |
| :----- | :--- | :--- | :----- |
| log_id | 是   | id   | log_id |

**复制用户 返回示例**

```
// 正确返回值
{
   "error_code": 0,
  "log_id": 3314921889,
}
// 发生错误时返回值
{
  "error_code": 216100,
  "log_id": 3111284097,
  "error_msg": "already add"
}
```

## 删除用户

用于将用户从某个组中删除。

```
groupId = "group1"

userId = "user1"

""" 调用删除用户 """
client.deleteUser(groupId, userId);
```

**删除用户 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                               |
| :------- | :------- | :----- | :------------------------------------------------- |
| group_id | 是       | string | 用户组id（由数字、字母、下划线组成），长度限制128B |
| user_id  | 是       | string | 用户id（由数字、字母、下划线组成），长度限制128B   |

**删除用户 返回数据参数详情**

| 字段   | 是否必选 | 类型   | 说明                   |
| :----- | :------- | :----- | :--------------------- |
| log_id | 是       | uint64 | 请求唯一标识码，随机数 |

**删除用户 返回示例**

```
// 正确返回值
{

  "error_code": 0,
  "log_id": 3314921889,
}
// 发生错误时返回值
{
  "error_code":  223103,
  "log_id": 815967402,
  "error_msg": "user is not exist"
}
```

## 创建用户组

用于创建一个空的用户组，如果用户组已存在 则返回错误。

```
groupId = "group1"

""" 调用创建用户组 """
client.groupAdd(groupId);
```

**创建用户组 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                               |
| :------- | :------- | :----- | :------------------------------------------------- |
| group_id | 是       | string | 用户组id（由数字、字母、下划线组成），长度限制128B |

**创建用户组 返回数据参数详情**

| 字段   | 必选 | 类型   | 说明                     |
| :----- | :--- | :----- | :----------------------- |
| log_id | 是   | uint64 | 请求标识码，随机数，唯一 |

**创建用户组 返回示例**

```
{

  "error_code": 0,
  "log_id": 3314921889,
}
// 发生错误时返回值
{
  "error_code":  223101,
  "log_id": 815967402,
  "error_msg": " group is already exist"
}
```

## 删除用户组

删除用户组下所有的用户及人脸，如果组不存在 则返回错误。

```
groupId = "group1"

""" 调用删除用户组 """
client.groupDelete(groupId);
```

**删除用户组 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                                               |
| :------- | :------- | :----- | :------------------------------------------------- |
| group_id | 是       | string | 用户组id（由数字、字母、下划线组成），长度限制128B |

**删除用户组 返回数据参数详情**

| 字段   | 必选 | 类型   | 说明                     |
| :----- | :--- | :----- | :----------------------- |
| log_id | 是   | uint64 | 请求标识码，随机数，唯一 |

**删除用户组 返回示例**

```
// 正确返回值
{

  "error_code":0,
  "log_id": 3314921889,
}
// 发生错误时返回值
{
  "error_code": 223100,
  "log_id": 815967402,
  "error_msg": " group is not exist"
}
```

## 组列表查询

用于查询用户组的列表。

```
""" 调用组列表查询 """
client.getGroupList();

""" 如果有可选参数 """
options = {}
options["start"] = 0
options["length"] = 50

""" 带参数调用组列表查询 """
client.getGroupList(, options)
```

**组列表查询 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 默认值 | 说明                            |
| :------- | :------- | :----- | :----- | :------------------------------ |
| start    | 否       | string | 0      | 默认值0，起始序号               |
| length   | 否       | string | 100    | 返回数量，默认值100，最大值1000 |

**组列表查询 返回数据参数详情**

| 字段          | 必选 | 类型  | 说明  |
| :------------ | :--- | :---- | :---- |
| group_id_list | 是   | array | group |

**组列表查询 返回示例**

```
{
    "group_id_list": [
        "gid1",
        "gid2"
    ]
}
```

## 身份验证

质量检测（可选）活体检测（可选）公安验证（必选）

```
image = "取决于image_type参数，传入BASE64字符串或URL字符串或FACE_TOKEN字符串"

imageType = "BASE64"

idCardNumber = "110233112299822211"

name = "张三"

""" 调用身份验证 """
client.personVerify(image, imageType, idCardNumber, name);

""" 如果有可选参数 """
options = {}
options["quality_control"] = "NORMAL"
options["liveness_control"] = "LOW"

""" 带参数调用身份验证 """
client.personVerify(image, imageType, idCardNumber, name, options)
```

**身份验证 请求参数详情**

| 参数名称         | 是否必选 | 类型   | 默认值 | 说明                                                         |
| :--------------- | :------- | :----- | :----- | :----------------------------------------------------------- |
| image            | 是       | string |        | 图片信息(**总数据大小应小于10M**)，图片上传方式根据image_type来判断 |
| image_type       | 是       | string |        | 图片类型 **BASE64**:图片的base64值，base64编码后的图片数据，需urlencode，编码后的图片大小不超过2M；**URL**:图片的 URL地址( 可能由于网络等原因导致下载图片时间过长)**；FACE_TOKEN**: 人脸图片的唯一标识，调用人脸检测接口时，会为每个人脸图片赋予一个唯一的FACE_TOKEN，同一张图片多次检测得到的FACE_TOKEN是同一个 |
| id_card_number   | 是       | string |        | 身份证号（真实身份证号号码）                                 |
| name             | 是       | string |        | utf8，姓名（真实姓名，和身份证号匹配）                       |
| quality_control  | 否       | string | NONE   | 图片质量控制 **NONE**: 不进行控制 **LOW**:较低的质量要求 **NORMAL**: 一般的质量要求 **HIGH**: 较高的质量要求 **默认 NONE** |
| liveness_control | 否       | string | NONE   | 活体检测控制 **NONE**: 不进行控制 **LOW**:较低的活体要求(高通过率 低攻击拒绝率) **NORMAL**: 一般的活体要求(平衡的攻击拒绝率, 通过率) **HIGH**: 较高的活体要求(高攻击拒绝率 低通过率) **默认NONE** |

**身份验证 返回数据参数详情**

| 参数   | 必须 | 类型   | 说明                                                         |
| :----- | :--- | :----- | :----------------------------------------------------------- |
| log_id | 是   | uint64 | 日志id                                                       |
| score  | 是   | float  | 与公安小图相似度可能性，用于验证生活照与公安小图是否为同一人，有正常分数时为[0~1]，推荐阈值0.8，超过即判断为同一人 |

**身份验证 返回示例**

```
{
  "score": 44.3,
}
```

## 语音校验码接口

此接口主要用于生成随机码，用于视频的语音识别校验使用，以判断视频的即时性，而非事先录制的，提升作弊的难度。

```
""" 调用语音校验码接口 """
client.videoSessioncode();

""" 如果有可选参数 """
options = {}
options["appid"] = "223245"

""" 带参数调用语音校验码接口 """
client.videoSessioncode(, options)
```

**语音校验码接口 请求参数详情**

| 参数名称 | 是否必选 | 类型   | 说明                         |
| :------- | :------- | :----- | :--------------------------- |
| appid    | 否       | string | 百度云创建应用时的唯一标识ID |

**语音校验码接口 返回数据参数详情**

| 字段       | 必选 | 类型   | 说明                            |
| :--------- | :--- | :----- | :------------------------------ |
| session_id | 是   | string | 语音校验码会话id                |
| code       | 是   | string | 语音验证码，数字形式，3~6位数字 |

**语音校验码接口 返回示例**

```
{
  "err_no": 0,
  "err_msg": "SUCCESS",
  "result": {
    "session_id": "S59faeeebb9111890355690",
    "code": "9940"
  },
  "timestamp": 1509617387,
  "cached": 0,
  "serverlogid": "0587756642"
}
```

## 在线活体检测

**接口能力**

- **人脸基础信息**：包括人脸框位置，人脸空间旋转角度，人脸置信度等信息。
- **人脸质量检测**：判断人脸的遮挡、光照、模糊度、完整度等质量信息。可用于判断上传的人脸是否符合标准。
- **基于图片的活体检测**：基于单张图片，判断图片中的人脸是否为二次翻拍（举例：如用户A用手机拍摄了一张包含人脸的图片一，用户B翻拍了图片一得到了图片二，并用图片二伪造成用户A去进行识别操作，这种情况普遍发生在金融开户、实名认证等环节）。此能力可用于H5场景下的一些人脸采集场景中，增加人脸注册的安全性和真实性。

```
result = client.faceverify([
    {
        'image': base64.b64encode(open('1.jpg', 'rb').read()),
        'image_type': 'BASE64',
    },
    {
        'image': base64.b64encode(open('2.jpg', 'rb').read()),
        'image_type': 'BASE64',
    }
])
```

**请求参数**

| 参数       | 是否必选 | 类型   | 说明                                                         |
| :--------- | :------- | :----- | :----------------------------------------------------------- |
| image      | 是       | string | 图片信息(总数据大小应小于10M)，图片上传方式根据image_type来判断； **可以上传同一个用户的1张、3张或8张图片来进行活体判断，注：后端会选择每组照片中的最高分数作为整体分数。** |
| image_type | 是       | string | 图片类型 **BASE64**:图片的base64值，base64编码后的图片数据，需urlencode，编码后的图片大小不超过2M； **URL**:图片的 URL地址( 可能由于网络等原因导致下载图片时间过长)； **FACE_TOKEN**: 人脸图片的唯一标识，调用人脸检测接口时，会为每个人脸图片赋予一个唯一的FACE_TOKEN，同一张图片多次检测得到的FACE_TOKEN是同一个。 |
| face_field | 否       | string | 包括**age,beauty,expression,faceshape,gender,glasses,landmark,race,quality,facetype信息**，逗号分隔，默认只返回face_token、活体数、人脸框、概率和旋转角度 |

**返回参数**

| 参数              | 类型 | 是否必须 | 说明                                                         |
| :---------------- | :--- | :------- | :----------------------------------------------------------- |
| face_liveness     | 是   | float    | 活体分数值                                                   |
| thresholds        | 是   | array    | 由服务端返回最新的阈值数据（随着模型的优化，阈值可能会变化），将此参数与返回的face_liveness进行比较，可以作为活体判断的依据。 frr_1e-4：万分之一误识率的阈值；frr_1e-3：千分之一误识率的阈值；frr_1e-2：百分之一误识率的阈值。误识率越低，准确率越高，相应的拒绝率也越高 |
| face_list         | 是   | array    | 每张图片的详细信息描述，如果只上传一张图片，则只返回一个结果。 |
| +face_token       | 是   | string   | 人脸图片的唯一标识                                           |
| +location         | 是   | array    | 人脸在图片中的位置                                           |
| ++left            | 是   | double   | 人脸区域离左边界的距离                                       |
| ++top             | 是   | double   | 人脸区域离上边界的距离                                       |
| ++width           | 是   | double   | 人脸区域的宽度                                               |
| ++height          | 是   | double   | 人脸区域的高度                                               |
| ++rotation        | 是   | int64    | 人脸框相对于竖直方向的顺时针旋转角，[-180,180]               |
| +face_probability | 是   | double   | 人脸置信度，范围【0~1】，代表这是一张人脸的概率，0最小、1最大。 |
| +angel            | 是   | array    | 人脸旋转角度参数                                             |
| ++yaw             | 是   | double   | 三维旋转之左右旋转角[-90(左), 90(右)]                        |
| ++pitch           | 是   | double   | 三维旋转之俯仰角度[-90(上), 90(下)]                          |
| ++roll            | 是   | double   | 平面内旋转角[-180(逆时针), 180(顺时针)]                      |
| +age              | 否   | double   | 年龄 ，当**face_field包含age时返回**                         |
| +beauty           | 否   | int64    | 美丑打分，范围0-100，越大表示越美。当**face_fields包含beauty时返回** |
| +expression       | 否   | array    | 表情，当 **face_field包含expression时返回**                  |
| ++type            | 否   | string   | **none**:不笑；**smile**:微笑；**laugh**:大笑                |
| ++probability     | 否   | double   | 表情置信度，范围【0~1】，0最小、1最大。                      |
| +face_shape       | 否   | array    | 脸型，当**face_field包含faceshape时返回**                    |
| ++type            | 否   | double   | **square**: 正方形 **triangle**:三角形 **oval**: 椭圆 **heart**: 心形 **round**: 圆形 |
| ++probability     | 否   | double   | 置信度，范围【0~1】，代表这是人脸形状判断正确的概率，0最小、1最大。 |
| +gender           | 否   | array    | 性别，**face_field包含gender时返回**                         |
| ++type            | 否   | string   | male:**男性** female:**女性**                                |
| ++probability     | 否   | double   | 性别置信度，范围【0~1】，0代表概率最小、1代表最大。          |
| +glasses          | 否   | array    | 是否带眼镜，**face_field包含glasses时返回**                  |
| ++type            | 否   | string   | **none**:无眼镜，**common**:普通眼镜，**sun**:墨镜           |
| ++probability     | 否   | double   | 眼镜置信度，范围【0~1】，0代表概率最小、1代表最大。          |
| +race             | 否   | array    | 人种 **face_field包含race时返回**                            |
| ++type            | 否   | string   | **yellow**: 黄种人 **white**: 白种人 **black**:黑种人 **arabs**: **阿拉伯人** |
| ++probability     | 否   | double   | 人种置信度，范围【0~1】，0代表概率最小、1代表最大。          |
| +face_type        | 否   | array    | 真实人脸/卡通人脸 **face_field包含facetype时返回**           |
| ++type            | 否   | string   | **human**: 真实人脸 **cartoon**: 卡通人脸                    |
| ++probability     | 否   | double   | 人脸类型判断正确的置信度，范围【0~1】，0代表概率最小、1代表最大。 |
| +landmark         | 否   | array    | 4个关键点位置，左眼中心、右眼中心、鼻尖、嘴中心。**face_field包含landmark时返回** |
| +landmark72       | 否   | array    | 72个特征点位置 **face_field包含landmark时返回**              |
| +quality          | 否   | array    | 人脸质量信息。**face_field包含quality时返回**                |
| ++occlusion       | 否   | array    | 人脸各部分遮挡的概率，范围[0~1]，0表示完整，1表示不完整      |
| +++left_eye       | 否   | double   | 左眼遮挡比例，[0-1] ， 1表示完全遮挡                         |
| +++right_eye      | 否   | double   | 右眼遮挡比例，[0-1] ， 1表示完全遮挡                         |
| +++nose           | 否   | double   | 鼻子遮挡比例，[0-1] ， 1表示完全遮挡                         |
| +++mouth          | 否   | double   | 嘴巴遮挡比例，[0-1] ， 1表示完全遮挡                         |
| +++left_cheek     | 否   | double   | 左脸颊遮挡比例，[0-1] ， 1表示完全遮挡                       |
| +++right_cheek    | 否   | double   | 右脸颊遮挡比例，[0-1] ， 1表示完全遮挡                       |
| +++chin           | 否   | double   | 下巴遮挡比例，，[0-1] ， 1表示完全遮挡                       |
| ++blur            | 否   | double   | 人脸模糊程度，范围[0~1]，0表示清晰，1表示模糊                |
| ++illumination    | 否   | double   | 取值范围在[0~255], 表示脸部区域的光照程度 越大表示光照越好   |
| ++completeness    | 否   | int64    | 人脸完整度，0或1, 0为人脸溢出图像边界，1为人脸都在图像边界内 |
| +parsing_info     | 否   | string   | 人脸分层结果 结果数据是使用gzip压缩后再base64编码 使用前需base64解码后再解压缩 **原数据格式为string 形如0,0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,…** |

- 返回示例

  ```
  {
    "thresholds": {
        "frr_1e-4": 0.05,  //万分之一误拒率的阈值
        "frr_1e-3": 0.3,   //千分之一误拒率的阈值
        "frr_1e-2": 0.9    //百分之一误拒率的阈值
    },
    "face_liveness": 0.05532243927,
    "face_list": [
        {
            "face_token": "df46f7c7db4aa09a093c26fb8d1a8d44",
            "location": {
                "left": 328.9026489,
                "top": 97.16340637,
                "width": 162,
                "height": 154,
                "rotation": 32
            },
            "face_probability": 1,
            "angle": {
                "yaw": 10.16196251,
                "pitch": 2.244354248,
                "roll": 33.82199097
            },
            "liveness": {
                "faceliveness": 0.004187555984,
                "livemapscore": 0.04492170034
            },
            "age": 23,
            "beauty": 20.23693275
        },
        {
            "face_token": "901d2c64274fccd687d311a6e6110a01",
            "location": {
                "left": 411.4876404,
                "top": 166.3593445,
                "width": 329,
                "height": 308,
                "rotation": 45
            },
            "face_probability": 0.9194830656,
            "angle": {
                "yaw": -1.716423035,
                "pitch": 7.344647408,
                "roll": 45.79914856
            },
            "liveness": {
                "faceliveness": 0.0001665892196,
                "livemapscore": 0.001787073661
            },
            "age": 23,
            "beauty": 12.6438179
        },
        {
            "face_token": "7d57e36981c48b4946eb97c8d838b02a",
            "location": {
                "left": 161.4559937,
                "top": 199.8726501,
                "width": 218,
                "height": 201,
                "rotation": -1
            },
            "face_probability": 1,
            "angle": {
                "yaw": -8.187754631,
                "pitch": 6.973727226,
                "roll": -1.25429821
            },
            "liveness": {
                "faceliveness": 0.02942637168,
                "livemapscore": 0.05532243927
            },
            "age": 23,
            "beauty": 8.20657444
        }
    ]
    }
  ```

## 人脸对比

**接口能力**

- **两张人脸图片相似度对比**：比对两张图片中人脸的相似度，并返回相似度分值；
- **多种图片类型**：支持**生活照**、**证件照**、**身份证芯片照**、**带网纹照**四种类型的人脸对比；
- **活体检测**：基于图片中的破绽分析，判断其中的人脸是否为**二次翻拍**（举例：如用户A用手机拍摄了一张包含人脸的图片一，用户B翻拍了图片一得到了图片二，并用图片二伪造成用户A去进行识别操作，这种情况普遍发生在金融开户、实名认证等环节。）；
- **质量检测**：返回模糊、光照等质量检测信息，用于辅助判断图片是否符合识别要求；

**业务应用**

用于比对多张图片中的人脸相似度并返回两两比对的得分，可用于判断两张脸是否是同一人的可能性大小。

典型应用场景：如**人证合一验证**，**用户认证**等，可与您现有的人脸库进行比对验证。

```
result = client.match([
    {
        'image': base64.b64encode(open('1.jpg', 'rb').read()),
        'image_type': 'BASE64',
    },
    {
        'image': base64.b64encode(open('2.jpg', 'rb').read()),
        'image_type': 'BASE64',
    }
])
```

**请求参数**

| 参数             | 必选 | 类型   | 说明                                                         |
| :--------------- | :--- | :----- | :----------------------------------------------------------- |
| image            | 是   | string | 图片信息(**总数据大小应小于10M**)，图片上传方式根据image_type来判断。 **两张图片通过json格式上传，格式参考表格下方示例** |
| image_type       | 是   | string | 图片类型 **BASE64**:图片的base64值，base64编码后的图片数据，需urlencode，编码后的图片大小不超过2M；**URL**:图片的 URL地址( 可能由于网络等原因导致下载图片时间过长)**；FACE_TOKEN**: 人脸图片的唯一标识，调用人脸检测接口时，会为每个人脸图片赋予一个唯一的FACE_TOKEN，同一张图片多次检测得到的FACE_TOKEN是同一个。 |
| face_type        | 否   | string | 人脸的类型 **LIVE**表示生活照：通常为手机、相机拍摄的人像图片、或从网络获取的人像图片等，**IDCARD**表示身份证芯片照：二代身份证内置芯片中的人像照片， **WATERMARK**表示带水印证件照：一般为带水印的小图，如公安网小图 **CERT**表示证件照片：如拍摄的身份证、工卡、护照、学生证等证件图片 默认LIVE |
| quality_control  | 否   | string | 图片质量控制 **NONE**: 不进行控制 **LOW**:较低的质量要求 **NORMAL**: 一般的质量要求 **HIGH**: 较高的质量要求 **默认 NONE** |
| liveness_control | 否   | string | 活体检测控制 **NONE**: 不进行控制 **LOW**:较低的活体要求(高通过率 低攻击拒绝率) **NORMAL**: 一般的活体要求(平衡的攻击拒绝率, 通过率) **HIGH**: 较高的活体要求(高攻击拒绝率 低通过率) **默认NONE** |

**返回参数**

| 参数名      | 必选 | 类型   | 说明           |
| :---------- | :--- | :----- | :------------- |
| score       | 是   | float  | 人脸相似度得分 |
| face_list   | 是   | array  | 人脸信息列表   |
| +face_token | 是   | string | 人脸的唯一标志 |

- 返回示例

  ```
  {
      "score": 44.3,
      "face_list": [  //返回的顺序与传入的顺序保持一致
          {
              "face_token": "fid1"
          },
          {
              "face_token": "fid2"
          }
      ]
  }
  ```

# 错误信息

## 错误返回格式

若请求错误，服务器将返回的JSON文本包含以下参数：

- **error_code**：错误码。
- **error_msg**：错误描述信息，帮助理解和解决发生的错误。

## 错误码

**服务端返回的错误码**

| 错误码 | 错误信息                                            | 描述                                                         |
| :----- | :-------------------------------------------------- | :----------------------------------------------------------- |
| 4      | Open api request limit reached                      | 集群超限额                                                   |
| 14     | IAM Certification failed                            | IAM鉴权失败，建议用户参照文档自查生成sign的方式是否正确，或换用控制台中ak sk的方式调用 |
| 17     | Open api daily request limit reached                | 每天流量超限额                                               |
| 18     | Open api qps request limit reached                  | QPS超限额                                                    |
| 19     | Open api total request limit reached                | 请求总量超限额                                               |
| 100    | Invalid parameter                                   | 无效参数                                                     |
| 110    | Access token invalid or no longer valid             | Access Token失效                                             |
| 111    | Access token expired                                | Access token过期                                             |
| 222001 | param[] is null                                     | 必要参数未传入                                               |
| 222002 | param[start] format error                           | 参数格式错误                                                 |
| 222003 | param[length] format error                          | 参数格式错误                                                 |
| 222004 | param[op_app_id_list] format error                  | 参数格式错误                                                 |
| 222005 | param[group_id_list] format error                   | 参数格式错误                                                 |
| 222006 | group_id format error                               | 参数格式错误                                                 |
| 222007 | uid format error                                    | 参数格式错误                                                 |
| 222008 | face_id format error                                | 参数格式错误                                                 |
| 222009 | quality_conf format error                           | 参数格式错误                                                 |
| 222010 | user_info format error                              | 参数格式错误                                                 |
| 222011 | param[uid_list] format error                        | 参数格式错误                                                 |
| 222012 | param[op_app_id] format error                       | 参数格式错误                                                 |
| 222013 | param[image] format error                           | 参数格式错误                                                 |
| 222014 | param[app_id] format error                          | 参数格式错误                                                 |
| 222015 | param[image_type] format error                      | 参数格式错误                                                 |
| 222016 | param[max_face_num] format error                    | 参数格式错误                                                 |
| 222017 | param[face_field] format error                      | 参数格式错误                                                 |
| 222018 | param[user_id] format error                         | 参数格式错误                                                 |
| 222019 | param[quality_control] format error                 | 参数格式错误                                                 |
| 222020 | param[liveness_control] format error                | 参数格式错误                                                 |
| 222021 | param[max_user_num] format error                    | 参数格式错误                                                 |
| 222022 | param[id_card_number] format error                  | 参数格式错误                                                 |
| 222023 | param[name] format error                            | 参数格式错误                                                 |
| 222024 | param[face_type] format error                       | 参数格式错误                                                 |
| 222025 | param[face_token] format error                      | 参数格式错误                                                 |
| 222026 | param[max_star_num] format error                    | 参数格式错误                                                 |
| 222201 | network not available                               | 服务端请求失败                                               |
| 222202 | pic not has face                                    | 图片中没有人脸                                               |
| 222203 | image check fail                                    | 无法解析人脸                                                 |
| 222204 | image_url_download_fail                             | 从图片的url下载 图片失败                                     |
| 222205 | network not availablel                              | 服务端请求失败                                               |
| 222206 | rtse service return fail                            | 服务端请求失败                                               |
| 222207 | match user is not found                             | 未找到匹配的用户                                             |
| 222208 | the number of image is incorrect                    | 图片的数量错误                                               |
| 222209 | face token not exist                                | face token不存在                                             |
| 222300 | add face fail                                       | 人脸图片添加失败                                             |
| 222301 | get face fail                                       | 获取人脸图片失败                                             |
| 222302 | system error                                        | 服务端请求失败                                               |
| 222303 | get face fail                                       | 获取人脸图片失败                                             |
| 223100 | group is not exist                                  | 操作的用户组不存在                                           |
| 223101 | group is already exist                              | 该用户组已存在                                               |
| 223102 | user is already exist                               | 该用户已存在                                                 |
| 223103 | user is not exist                                   | 找不到该用户                                                 |
| 223104 | group_list is too large                             | group_list包含组 数量过多                                    |
| 223105 | face is already exist                               | 该人脸已存在                                                 |
| 223106 | face is not exist                                   | 该人脸不存在                                                 |
| 223110 | uid_list is too large                               | uid_list包含数量过多                                         |
| 223111 | dst group is not exist                              | 目标用户组不存在                                             |
| 223112 | quality_conf format error                           | quality_conf格式不正确                                       |
| 223113 | face is covered                                     | 人脸有被遮挡                                                 |
| 223114 | face is fuzzy                                       | 人脸模糊                                                     |
| 223115 | face light is not good                              | 人脸光照不好                                                 |
| 223116 | incomplete face                                     | 人脸不完整                                                   |
| 223117 | app_list is too large                               | app_list包含app数量 过多                                     |
| 223118 | quality control error                               | 质量控制项错误                                               |
| 223119 | liveness control item error                         | 活体控制项错误                                               |
| 223120 | liveness check fail                                 | 活体检测未通过                                               |
| 223121 | left eye is occlusion                               | 质量检测未通过 左眼 遮挡程度过高                             |
| 223122 | right eye is occlusion                              | 质量检测未通过 右眼 遮挡程度过高                             |
| 223123 | left cheek is occlusion                             | 质量检测未通过 左脸 遮挡程度过高                             |
| 223124 | right cheek is occlusion                            | 质量检测未通过 右脸 遮挡程度过高                             |
| 223125 | chin contour is occlusion                           | 质量检测未通过 下巴遮挡程度过高                              |
| 223126 | nose is occlusion                                   | 质量检测未通过 鼻子遮挡程度过高                              |
| 223127 | mouth is occlusion                                  | 质量检测未通过 嘴巴 遮挡程度过高                             |
| 222350 | police picture is none or low quality               | 公安网图片不存在或 质量过低                                  |
| 222351 | id number and name not match or id number not exist | 身份证号与姓名不匹配或该 身份证号不存在                      |
| 222352 | name format error                                   | 身份证名字格式错误                                           |
| 222353 | id number format error                              | 身份证号码格式错误                                           |
| 222354 | id number not exist                                 | 公安库里不存在此身份证号                                     |
| 222355 | police picture not exist                            | 身份证号码正确，公安库里没有 对应的照片                      |
| 222360 | invalid name or id number                           | 身份证号码或名字非法（公安网校 验不通过）                    |
| 222901 | system busy                                         | 系统繁忙                                                     |
| 222902 | system busy                                         | 系统繁忙                                                     |
| 222903 | system busy                                         | 系统繁忙                                                     |
| 222904 | system busy                                         | 系统繁忙                                                     |
| 222905 | system busy                                         | 系统繁忙                                                     |
| 222906 | system busy                                         | 系统繁忙                                                     |
| 222907 | system busy                                         | 系统繁忙                                                     |
| 222908 | system busy                                         | 系统繁忙                                                     |
| 222909 | system busy                                         | 系统繁忙                                                     |
| 222910 | system busy                                         | 系统繁忙                                                     |
| 222911 | system busy                                         | 系统繁忙                                                     |
| 222912 | system busy                                         | 系统繁忙                                                     |
| 222913 | system busy                                         | 系统繁忙                                                     |
| 222914 | system busy                                         | 系统繁忙                                                     |
| 222915 | system busy                                         | 系统繁忙                                                     |
| 222916 | system busy                                         | 系统繁忙                                                     |
| 222361 | system busy                                         | 系统繁忙                                                     |