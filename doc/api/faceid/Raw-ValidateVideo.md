# Raw-ValidateVideo

### 描述

通过语音识别、唇语识别的方式来进行自拍视频的活体验证。

此API必须在使用同一个API key调用的Raw-GetRandomNumber API返回的token_random_number作为参数，否则返回400（INVALID_TOKEN）。

仅当appkey有使用网页验证API服务的视频验证权限时，才可以调用此API，否则返回错误码 403（AUTHORIZATION_ERROR:Denied）。 

### 调用URL

<https://api.megvii.com/faceid/lite/raw/validate_video> 

 **注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障

### 调用方法

POST

### 参数

| 必选／可选 | 参数                | 类型   | 参数说明                                                     |
| :--------- | :------------------ | :----- | :----------------------------------------------------------- |
| 必选       | api_key             | String | 调用此API的api_key。                                         |
| 必选       | api_secret          | String | 调用此API的api_key的secret。                                 |
| 必选       | token_random_number | String | 调用Raw-GetRandomNumber API返回的token_random_number。       |
| 必选       | video               | File   | 需用户上传的视频，视频要求为ffmpeg所支持的格式及码率，视频时长为3~6秒，容量不大于20MB。 |
| 可选       | biz_no              | String | 用于标志一次验证流程，不超过128字符。如果使用此参数，**强烈建议对一次验证流程中调用的API（比如Raw-GetRandomNumber、Raw-ValidateVideo、Raw-Verify）使用同一个biz_no，对不同的验证流程使用不同的biz_no**。 |
| 可选       | return_image        | String | 此参数为可选参数，决定了是否返回从视频中截取的最佳质量图像：0（默认）：不需要图像1：需要返回最佳质量图 (仅当API调用成功后才返回) |

 

### 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。此字符串可以用于后续数据反查。此字段必定返回。 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。此字段必定返回。           |
| biz_no        | String | **此参数仅当调用时设置了biz_no参数才返回，但是如果设置了则即使API调用失败也返回，**值与传入的biz_no保持完全一致。 |
| token_video   | String | **本字段仅调用成功才返回，**返回的token仅供Raw-Verify API使用。本字段不超过1024字节。**注意：**Token有效期为24小时 |
| image_best    | String | **本字段仅调用成功才返回，返回为将会用于人脸比对的视频中的最佳质量的人脸照片**。image_return字段配置为0，或者没有配置，则不返回这个字段。image_return字段配置为1，并用Base64字符串返回，图像格式为JPEG。在异常情况下，该字段有可能返回null。 |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

###  

### 成功

```
{
  "time_used": 6206,
  "biz_no": "12345",
  "token_video": "68d32116d9ba2d2662812df30705dcb5",
  "request_id": "1491803871,dc23ec02-5ab5-48a4-8fe9-95f4eeb37699",
  "image_best": "data:image/jpeg;base64,..."

}
```

 

### 失败

```
{
  "time_used": 1,
  "error_message": "MISSING_ARGUMENTS: video",
  "request_id": "1491803987,5c52c7c6-e974-4c83-9f22-9d268d6e35b0"
}
```

 

### 错误码列表

#### Raw-ValidateVide API特有的 ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                 | 说明                                                         |
| :------------ | :----------------------- | :----------------------------------------------------------- |
| 400           | INVALID_TOKEN            | 参数所对应调用Raw-GetRandomNumber API的token_random_number不存在、或格式错误、或已过期、或并非通过一个API Key调用的Raw-GetRandomNumber API返回的token_random_number。 |
| 400           | VIDEO_FACE_NOT_FOUND     | 上传的视频中没有检测到人脸                                   |
| 400           | VIDEO_LOW_FACE_QUALITY   | 上传的视频中人脸质量太差                                     |
| 400           | VIDEO_INVALID_DURATION   | 上传的视频时长不对                                           |
| 400           | VIDEO_MULTIPLE_FACES     | 上传的视频中有多张人脸                                       |
| 400           | VIDEO_NO_AUDIO           | 上传视频中没有音轨，通常是由于手机的录音权限没有打开或者受到阻止 |
| 400           | VIDEO_UNSUPPORTED_FORMAT | 视频无法解析，有可能是ffmpeg不支持的格式或视频有破损         |

 

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