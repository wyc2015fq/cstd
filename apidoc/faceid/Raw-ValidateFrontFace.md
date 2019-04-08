# Raw-ValidateFrontFace

### 描述

验证上传用户自拍正脸照片，验证成功的正脸将保存在系统里。

仅当appkey有使用网页验证API服务的自拍验证权限时，才可以调用此API，否则返回错误码 403（AUTHORIZATION_ERROR:Denied）。

### 调用URL

<https://api.megvii.com/faceid/lite/raw/validate_front_face> 

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障

### 调用方法

POST

### 参数

| 必选／可选 | 参数       | 类型   | 参数说明                                                     |
| :--------- | :--------- | :----- | :----------------------------------------------------------- |
| 必选       | api_key    | String | 调用此API的api_key。                                         |
| 必选       | api_secret | String | 调用此API的api_key的secret。                                 |
| 可选       | biz_no     | String | 用于标志一次验证流程，不超过128字符。如果要使用此参数，**强烈建议对一次验证流程中调用的API（比如Raw-ValidateFrontFace、Raw-ValidateSideFace、Raw-Verify）使用同一个biz_no，对不同的验证流程使用不同的biz_no**。 |
| 必选       | selfie     | File   | 用户的自拍正面照片。**注：**要求上传用户的原始自拍照片，不要对图片进行额外的加工处理。特别是在微信客户端中，默认是启用压缩图片的，请注意上传原始图片。 |

### 返回值说明

| 字段             | 类型   | 说明                                                         |
| :--------------- | :----- | :----------------------------------------------------------- |
| request_id       | String | 用于区分每一次请求的唯一的字符串。此字符串可以用于后续数据反查。此字段必定返回。 |
| time_used        | Int    | 整个请求所花费的时间，单位为毫秒。此字段必定返回。           |
| biz_no           | String | **此参数仅当调用时设置了biz_no参数才返回，但是如果设置了则即使API调用失败也返回，**值与传入的biz_no保持完全一致。 |
| token_front_face | String | **本字段仅调用成功才返回，**传出的token仅供Raw-ValidateSideFace API调用。本字段不超过1024字节。**注意：**Token有效期为24小时 |
| error_message    | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

###  

### 成功

```
{
 "time_used": 544,
 "token_front_face": "c57eec288cdb45a8f17c67b2eaaaa271",
 "request_id": "1490025471,ce073fb2-9fb9-4169-aef0-3b1ecf8a6874",
 "biz_no": "3149525e-2c24-4862-8e9f-92040595f0a4"
}
```

 

### 失败

```
{
 "time_used": 19,
 "biz_no": "3149525e-2c24-4862-8e9f-92040595f0a4",
 "error_message": "IMAGE_ERROR_UNSUPPORTED_FORMAT",
 "request_id": "1490025636,2b1b4399-19a1-4ef1-9944-e14e7d4222be"
}
```

 

### 错误码列表

#### Raw-ValidateFrontFace特有的 ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                       | 说明                                                         |
| :------------ | :----------------------------- | :----------------------------------------------------------- |
| 400           | IMAGE_ERROR_UNSUPPORTED_FORMAT | 自拍图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400           | NO_FACE_FOUND                  | 自拍图像没有检测到人脸。                                     |
| 400           | MULTIPLE_FACES                 | 自拍图像检测到多张人脸，且因大小差异不特别明显，无法确定目标脸。 |
| 400           | FACE_BLURRED                   | 人脸太模糊，不适宜比对。                                     |
| 400           | FACE_POOR_LIGHT                | 人脸区过亮或过暗，不适宜比对。                               |
| 400           | FACE_NOT_FRONT                 | 自拍图像中的人脸非正面朝向。正面朝向是指：面部无明显的左右上下倾斜，上下不颠倒。 |
| 400           | FACE_NOT_PROPER_POSITION       | 人脸位置不合适。理想情况是人脸在画面中央，人脸包围盒的长宽分别是画面长宽的三分之一。如果实际情况偏离理想情况太多，就会触发错误，比如人脸太小或太大或位置太偏，以致于不适宜后续的活体判定和人脸比对。 |

 

#### 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 403           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有：Denied. （没有权限调用当前API） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。                                             |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 长度过长;不允许的输入值。） |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系 FaceID 客服或商务。 |