# Detect API

### 版本号

1.0.1

\---------------------------------
2017-02-22， V1.0.1
\---------------------------------

**API和能力升级：**

- 1）新增对image参数图片的旋转检测能力，详情见新增的multi_oriented_detection参数描述。

 

**简介**

此接口检测一张照片中的人脸，并且将检测出的人脸保存到FaceID平台里，便于后续的人脸比对。

因为FaceID Verify接口假设待比对的照片必须是有且只有一张人脸的正面大头照，对于不使用FaceID MegLive活体检查模块获得大头照的用户， Detect接口提供了从一张图片里直接获得大头照的方式。

### 调用URL：

<https://api.faceid.com/faceid/v1/detect>

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障。

### 调用方法:

POST

### 参数

|      | 参数名                   | 类型   | 参数说明                                                     |
| :--- | :----------------------- | :----- | :----------------------------------------------------------- |
| 必选 | api_key                  | String | 用于验证客户身份的API Key； 对于每一个客户此字段不会变更，相当于用户名。 |
| 必选 | api_secret               | String | 用于验证客户身份的API Secret； 对于每一个客户可以申请变更此字段，相当于密码。 |
| 必选 | image                    | File   | 通过非MegLive的途径获得的真人人脸照片。                      |
| 可选 | multi_oriented_detection | String | 对**image**参数启用图片旋转检测功能。当**image**参数中传入的图片未检测到人脸时，是否对图片尝试旋转90度、180度、270度后再检测人脸。本参数取值只能是 “1” 或 "0" （**缺省值为“0”**）:“1”：启用旋转检测（启用旋转检测后，会增加API的调用时间）“0”：不启用旋转检测其他值：返回错误码400（BAD_ARGUMENTS） **注意：**设置此参数为1可能会轻微增加误检人脸的概率，如果您明确您的业务场景里不存在非正向的人脸图片、或概率极低，建议勿设置此参数。 |

### 返回值

API返回的为一个JSON字符串。

| 字段              | 类型   | 说明                                                         |
| :---------------- | :----- | :----------------------------------------------------------- |
| request_id        | String | 用于区分每一次请求的唯一的字符串。                           |
| time_used         | Float  | 整个请求所花费的时间，单位为毫秒。                           |
| faces             | Array  | 在照片中所有找到的人脸，都以Json的形式返回，并组成一个Array。 |
| quality           | Float  | 每一个人脸都会有一个质量判断的分数。                         |
| quality_threshold | Float  | 表示人脸质量基本合格的一个阈值，超过该阈值的人脸适合用于人脸比对。 |
| rect              | Json   | 图片中人脸框的位置，会用一个Json表示。                       |
| left              | Float  | 人脸框的左上角（以图片宽度的比例给出）的x坐标。              |
| top               | Float  | 人脸框的左上角（以图片高度的比例给出）的y坐标。              |
| width             | Float  | 人脸框的宽度（以图片宽度的比例给出）。                       |
| height            | Float  | 人脸框的高度（以图片高度的比例给出）。                       |
| token             | String | 用于标示每一个人脸的唯一字符串，可以用于verify上的face_token。 |
| orientation       | Int    | 本字段仅在参数 multi_oriented_detection为“1”时才返回，表示人脸框的朝向，目前仅取0、90、180或270，单位为度，按顺时针方向。以人脸来说，0度表示通常意义上的正向、90度为头顶朝右下巴朝左、180度表示上下颠掉、270度表示头顶朝左下巴朝右。按此角度逆时针旋转图片即可获得正向的人像照。 |

### 返回值示例

正确请求返回示例

```
{
   "time_used":320,
   "request_id":"1457432550,b70ab3a8-ee37-4f90-a2bd-007e23a970e2",
   "faces":[
      {
         "quality":38.22176746384912,
         "quality_threshold":30.1,
         "rect":{
            "left":0.18,
            "top":0.18,
            "width":0.5966667,
            "height":0.5966667
         },
         "orientation":90,
         "token":"Rihc25px-qjfYWBq5MRdy2HaE7FWKSaEj-J2qLEyMLY="
      }
   ]
}
```

请求失败返回示例:

```
{
   "time_used":0,
   "request_id":"1457432550,b70ab3a8-ee37-4f90-a2bd-007e23a970e2",
   "error":"IMAGE_ERROR_UNSUPPORTED_FORMAT"
} 
```

 

### 错误信息

| HTTP 状态代码 | 错误信息                       | 说明                                                         |
| :------------ | :----------------------------- | :----------------------------------------------------------- |
| 400           | MISSING_ARGUMENTS:key          | 缺少某个必要参数。                                           |
| 400           | IMAGE_ERROR_UNSUPPORTED_FORMAT | 上传的图像无法正确解析，其可能不是一个图像文件。             |
| 400           | INVALID_IMAGE_SIZE:<param>     | 上传的图像像素尺寸的长或宽超过4096像素。此处param为image。   |
| 400           | BAD_ARGUMENTS:<key>            | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 403           | AUTHORIZATION_ERROR            | 客户身份信息验证不通过。                                     |
| 403           | CONCURRENCY_LIMIT_EXCEEDED     | 并发数超过限制。                                             |
| 413           | Request Entity Too Large       | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                  | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR                 | 服务器内部错误，当此类错误发生时请再次请求，如果一直出现此类错误，请及时联系客服。 |

 

 

### 调用示例

curl "https://api.faceid.com/faceid/v1/detect" –F api_key=<api_key> -F api_secret=<api_secret> –F image=@customer.jpg 

 

 

 

 