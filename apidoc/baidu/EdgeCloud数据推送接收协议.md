# EdgeCloud数据推送接收协议

## 接入说明

用于开发者使用**EdgeCloud**提供的数据推送服务来开发自己的个性化业务系统。 在**EdgeCloud平台**抓拍机管理填写用于接收数据的**URL地址**，该URL地址应为一个可达地址。

## 数据推送消息格式

平台以HTTP POST请求形式向集成商平台注册地址推送数据，推送数据相关信息以JSON串的形式置于HTTP请求中的body部分。 集成商平台在接收数据时，会接收到数据的密文消息。

## 密文格式

在加密传输时，存在signature、timestamp、nonce、active_key、encrypted_data字段， signature 为body部分的消息摘要、timestamp为秒时间戳、nonce用于摘要计算的随机字符串、active_key为设备激活码、encrypted_data字段表示加密传输的数据。 如：

```
{
    "active_key":"4VPK-QSON-SREB-1E1Y",
    "timestamp":1543205510,
    "signature":"signature",
    "nonce":"xxx",
    "encrypted_data":"xxx"
}
```

## 加解密方案

- 平台基于AES算法提供加解密技术
- AES密钥计算为 AESKey=MD5(设备序列号) ，得到16字节的小写字符串。
- 设备序列号是在**抓拍机端云协同平台**添加抓拍机时填写的，在抓拍机管理控制台可以找到activeKey和设备序列号的对应关系，开发者可以在程序中保存这对映射关系，通过回调数据中active_key字段进行对应。 例如：设备序列号=1234 AES密钥 = md5(1234,16) = 52d04dc20036dbd8
- AES采用ECB模式，秘钥长度为16个字节（128位），数据采用PKCS5填充； PKCS#5：将数据填充到8字节的整数倍，定义待加密的内容Buf的字节长度为X，在Buf的尾部填充(8-X%8)个字节，填充的数据是 8 - (X % 8)，填充后的字节长度是 X + (8 - (X% 8))。 具体详见：https://tools.ietf.org/html/rfc2898
- 平台的加密数据部分为encrypted_data= Base64_Encode( AES_Encrypt[data] )，即数据本身的字节串作为AES加密的明文，再以AES算法对明文进行加密生成密文，最后对密文进行BASE64的编码操作生成加密消息体。
- 对加密消息体的解密流程为：
- 首先进行加密消息体的BASE64解码操作，aesData=Base64_Decode(encrypted_data)；
- 对获取的解码内容以AES算法进行解密操作，获取明文部分，plainData=AES_Decrypt(aesData)，解密中使用的秘钥为由MD5(active_key对应的设备序列号,16)计算得来。

## 摘要计算方案

> 为了验证消息体的合法性，每一条平台推送消息中包含了消息签名，接收平台可用以验证消息体的真实性，密文消息对消息体整体进行摘要计算。具体做法如下：signature=md5(sort(active_key + timestamp + nonce + encrypted_data)))，即按active_key、encrypted_data（密文数据）、nonce(8字节随机字符串)、timestamp（秒时间戳）的顺序以字节为单位拼接为一个整体，计算其md5值消息摘要(32字节小写字符串)。验证时，取出active_key、encrypted_data 、nonce 、timestamp计算出消息的md5值，与收到的signature进行比较，即可知道是否存在消息篡改。

## 推送数据说明

推送的数据是由开发者在**EdgeCloud平台**抓拍机配置管理中进行配置的输出结果。目前配置支持：

- 质量过滤
- 质量过滤
- 活体过滤
- 属性分析
- 年龄、颜值打分、表情、脸型、性别、是否带眼镜、种族
- 查找人脸库
- 设置后将返回人脸在指定人脸库和组中查询到的用户信息

推送结果说明：encrypted_data解密后的JSON格式

| 字段名            | 类型   | 是否必须 | 备注                                                         | 样例 |
| :---------------- | :----- | :------- | :----------------------------------------------------------- | :--- |
| active_key        | string | 是       | 激活码                                                       |      |
| device_code       | string | 是       | 设备序列号                                                   |      |
| request_time      | Int    | 是       | 抓拍机发送数据时间戳                                         |      |
| capture_time      | Int    | 是       | 抓拍机抓拍人脸时间戳                                         |      |
| receive_time      | Int    | 是       | EdgeCloud收到请求时间戳                                      |      |
| ext_info          | string | 是       | 抓拍机上传附加的扩展信息                                     |      |
| face_liveness     | double | 是       | 图片的活体打分 范围[0~1]                                     |      |
| image_url         | string | 是       | 图片URL地址                                                  |      |
| face              | json   | 是       | 人脸信息                                                     |      |
| +face_token       | string | 是       | 人脸唯一标识                                                 |      |
| +face_probability | double | 是       | 人脸置信度，范围0~1                                          |      |
| +location         | json   | 是       | 人脸在图片中的位置                                           |      |
| ++left            | double | 是       | 人脸区域离左边界的距离                                       |      |
| ++top             | double | 是       | 人脸区域离上边界的距离                                       |      |
| ++width           | double | 是       | 人脸区域的宽度                                               |      |
| ++height          | double | 是       | 人脸区域的高度                                               |      |
| ++rotation        | int    | 是       | 人脸框相对于竖直方向的顺时针旋转角，[-180,180]               |      |
| +angle            | json   | 是       | 人脸旋转角度参数                                             |      |
| ++pitch           | double | 是       | 三维旋转之俯仰角度[-90(上), 90(下)]                          |      |
| ++roll            | double | 是       | 平面内旋转角[-180(逆时针), 180(顺时针)]                      |      |
| ++yaw             | double | 是       | 三维旋转之左右旋转角[-90(左), 90(右)]                        |      |
| +age              | double | 否       | 年龄，属性分析选择**年龄**时返回                             |      |
| +beauty           | int    | 否       | 美丑打分，范围0-100，越大表示越美。 属性分析选择**颜值打分**时返回 |      |
| +expression       | json   | 否       | 表情，属性分析选择**表情**时返回                             |      |
| ++type            | string | 否       | none:不笑；smile:微笑；laugh:大笑                            |      |
| ++probability     | double | 否       | 表情置信度，范围0~1                                          |      |
| +face_shape       | json   | 否       | 脸型，属性分析选择**脸型**时返回                             |      |
| ++type            | string | 否       | square: 正方形 triangle:三角形 oval: 椭圆 heart: 心形 round: 圆形 |      |
| ++probability     | double | 否       | 脸型置信度，范围0~1                                          |      |
| +gender           | json   | 否       | 性别，属性分析选择**性别**时返回                             |      |
| ++type            | string | 否       | male:男性 female:女性                                        |      |
| ++probability     | double | 否       | 性别置信度，范围0~1                                          |      |
| +glasses          | json   | 否       | 是否带眼镜，属性分析选择**是否带眼镜**时返回                 |      |
| ++type            | string | 否       | none:无眼镜，common:普通眼镜，sun:墨镜                       |      |
| ++probability     | double | 否       | 是否带眼镜置信度，范围0~1                                    |      |
| +race             | json   | 否       | 种族，属性分析选择**种族**时返回                             |      |
| ++type            | string | 否       | yellow: 黄种人 white: 白种人 black:黑种人 arabs: 阿拉伯人    |      |
| ++probability     | double | 否       | 种族置信度，范围0~1                                          |      |
| +quality          | array  | 否       | 人脸质量信息。过滤配种中选择**质量过滤**是返回               |      |
| ++occlusion       | array  | 否       | 人脸各部分遮挡的概率，范围[0~1]， 0表示完整，1表示不完整     |      |
| +++left_eye       | double | 否       | 左眼遮挡比例                                                 |      |
| +++right_eye      | double | 否       | 右眼遮挡比例                                                 |      |
| +++nose           | double | 否       | 鼻子遮挡比例                                                 |      |
| +++mouth          | double | 否       | 嘴巴遮挡比例                                                 |      |
| +++left_cheek     | double | 否       | 左脸颊遮挡比例                                               |      |
| +++right_cheek    | double | 否       | 右脸颊遮挡比例                                               |      |
| +++chin_contour   | double | 否       | 下巴遮挡比例                                                 |      |
| ++blur            | double | 否       | 人脸模糊程度，范围[0~1]，0表示清晰，1表示模糊                |      |
| ++illumination    | double | 否       | 取值范围在[0~255], 表示脸部区域的光照程度 越大表示光照越好   |      |
| ++completeness    | int64  | 否       | 人脸完整度，0或1, 0为人脸溢出图像边界， 1为人脸都在图像边界内 |      |
| face_user         | json   | 否       | 设置**人脸搜索**配置是返回                                   |      |
| +group_id         | string | 否       | 用户所属的group_id                                           |      |
| +user_id          | string | 否       | 用户的user_id                                                |      |
| +user_info        | string | 否       | 注册用户时携带的user_info                                    |      |
| +score            | double | 否       | 用户的匹配得分 80分以上可以判断为同一人， 此分值对应万分之一误识率 |      |

示例：

```
{
    "active_key":"7OJL-HJOU-EAFW-GAG1",
    "device_code":"666",
    "request_time":1523429715000,
    "capture_time":1523429712000,
    "receive_time":1542958944875,
    "ext_info":"{\"name\":\"test\"}",
    "face_liveness":1,
    "image_url":"http://bj.bcebos.com/v1/ai-edgecloud/face/test/7OJL-HJOU-EAFW-GAG1/f66512822c20d62b4f61e5af0bde7e4a.png?authorization=bce-auth-v1%2Ff86a2044998643b5abc89b59158bad6d%2F2018-11-23T07%3A42%3A26Z%2F-1%2F%2F65245a0897a3a215813163388bf63dabf086a7e1b2c38a4c676e96fbaf8c5dc1",
    "face":{
        "face_token":"1177c7f81b687328df6fa5ccd5bef07d",
        "face_probability":1,
        "location":{
            "height":171,
            "left":53,
            "rotation":-3,
            "top":126,
            "width":184
        },
        "angle":{
            "pitch":7.21,
            "roll":-8.14,
            "yaw":12.22
        },
        "age":36,
        "beauty":63.24,
        "expression":{
            "probability":1,
            "type":"smile"
        },
        "gender":{
            "probability":1,
            "type":"male"
        },
        "glasses":{
            "probability":1,
            "type":"none"
        },
        "race":{
            "probability":1,
            "type":"yellow"
        },
        "quality":{
            "blur":0,
            "completeness":1,
            "illumination":159,
            "occlusion":{
                "chin_contour":0,
                "left_cheek":0,
                "left_eye":0,
                "mouth":0,
                "nose":0,
                "right_cheek":0.01,
                "right_eye":0
            }
        }
    },
    "face_user":{
        "group_id":"tuyi_baidu_face_pro",
        "score":97.827209472656,
        "user_id":"6ac6ad0f2cc94e8a92668f1cc85244ec",
        "user_info":"fc6aca7f-c6eb-4523-9054-3467ac2639c5tmp_21d33b992144a6c4d5a339ec101fdb6109018cda61512b9e.jpg"
    }
}
```

## 返回数据说明

开发者接收到数据后返回如下格式数据，然后进行异步进行业务处理。 Header设置：`Content-Type: application/json;charset=UTF-8`

```
{
	"code":0,
	"message":"success"
} 
```

code = 0 表示数据接收成功