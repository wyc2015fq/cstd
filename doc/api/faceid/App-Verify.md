# App-Verify

### 版本

3.0.0

### 描述

此接口用于将FaceID MegLiveStill SDK 所获得的数据进行上传，并获取活体验证、人脸比对、攻击防范等结果信息。

**注意：**本接口仅支持FaceID MegLiveStill SDK 3.0及以上版本的数据，FaceID MegLive SDK 3.0以下版本请使用“人脸验证API”中的“Verify 2.0.6”下的文档。

### 调用URL：

<https://api.megvii.com/faceid/v3/sdk/verify>

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障。

### 调用方法：

POST

### 权限

仅当用户接入FaceID产品后，才能调用FaceID各Web API。接入FaceID的流程请咨询FaceID商务人员。

### 参数

| 必选/可选 | 参数         | 类型   | 参数说明                                                     |
| :-------- | :----------- | :----- | :----------------------------------------------------------- |
| 必选      | sign         | String | 调用此API客户的签名，具体的签名产生方式请查阅”App-鉴权说明“文档。 |
| 必选      | sign_version | String | 签名算法版本号，请传递：hmac_sha1                            |
| 必选      | biz_token    | String | 通过”App-GetBizToken“ API接口获取到的biz_token               |
| 必选/可选 | meglive_data | File   | 若在App-GetBizToken时，使用的活体类型参数为meglive 或者 still（即：liveness_type = meglive 或 liveness_type = still），则此参数为**必选**参数。此参数需要上传由FaceID MegLiveStill SDK 3.0及以上版本生成的数据，内容包括活体验证过程中的数据，和采集到的人脸数据。请不要对数据包做任何调整，直接提交接口即可。若在App-GetBizToken时，使用的活体类型参数为raw_image（即：liveness_type = raw_image），则此参数为**可选**参数，API会返回在App-GetBizToken时传递的带人脸图片的比对结果。 |

### 返回值说明

| 参数           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| request_id     | String | API 调用的流水号                                             |
| biz_no         | String | 传入的业务流水号，原封不动地返回。                           |
| time_used      | int    | 整个请求所花费的时间，单位为毫秒。此字段必定返回。           |
| result_code    | int    | 表示本次验证的结果状态码。可结合result_code和result_message字段知晓具体的结果及原因：1000系列状态码表示比对完成，验证通过，经过比对，是同一个人2000系列状态码表示比对完成，活体验证通过，但待比对照片与权威数据照片，或参考照片中的至少一张，不是同一个人3000系列状态码表示在比对的时候，权威数据调用情况3100系列状态码表示权威数据调用出错（不计费）4000系列状态码表示验证过程的操作结果，如果在验证过程结果，则返回对应的结果4100系列状态码表示云端活体判断未通过4200系列状态码表示SDK活体图像采集失败其他结果，请预留处理方案，对于未来可能的错误，我们可能持续增加错误码。**注：**9000系列的返回优先级高于其他错误码。 |
| result_message | String | 可通过此字段信息知晓具体的原因。具体见：result_code & result_message 对照表； |
| verification   | Json   | 当“verbose=1”，且“result状态码为1000、2000系列”时，本字段才会返回：**"idcard"：**活体采集图片与权威数据的比对结果。"**confidence**"： Float类型，取值［0，100］， 数字越大表示两张照片越可能是同一个人。“**thresholds**”：一组用于参考的置信度阈值，Object类型，包含三个字段，均为Float类型、取值［0，100］：“**1e-3**”：误识率为千分之一的置信度阈值“**1e-4**”：误识率为万分之一的置信度阈值“**1e-5**”：误识率为十万分之一的置信度阈值“**1e-6**”：误识率为百万分之一的置信度阈值**ref1**：活体采集人像与上传的image_ref1的比对结果（仅当get_biz_token中存在image_ref1时返回，返回结果同idcard字段）**ref2**：活体采集人像与上传的image_ref2的比对结果（仅当get_biz_token中存在image_ref2时返回，返回结果同idcard字段） |
| attack_result  | Json   | 当“result状态码非4200系列”时，本字段才会返回：**"score"：**Float类型，取值［0,1]。代表攻击的分数，分数越高表明攻击的可能性越大**"threshold"：**Float类型，取值［0,1]。代表攻击的阈值**"result"：**Bool类型，取值**True**或者**False**。代表云端攻击判断的结果，False代表不是攻击，True代表是攻击。**注：**当在App-GetBizToken中设置参数“force_compare=1”时，则该API会忽略云端的活体判断，用户需要自行通过此字段来判断是否是活体。 云端采用默认策略是判断score >= threshold时，代表此次可能是攻击。 |
| images         | Json   | 一组照片列表，后续会根据采集的照片增加对应的照片字段。image_best：活体照片，base64编码。 |
| error          | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节，否则此字段不存在。 |

### 返回值示例

正确请求返回示例

```
{
   "time_used":1448,
   "verification":{
      "idcard":{
         "confidence":86.63057,
         "thresholds":{
            "1e-3":62.168713,
            "1e-5":74.39926,
            "1e-4":69.31534,
            "1e-6":78.038055
         }
      }
   },
   "attack_result":{
      "score":0.26,
      "threshold":0.5,
      "result":False
   },
   "request_id":"1531397565,39b19451-393c-4fc4-8fae-6dc74b2b00d7",
   "images":{
      "image_best":"xxxxxxxxxxx"
   },
   "biz_no":"",
   "result_message":"SUCCESS",
   "result_code":1000
}
```

失败请求返回示例

```
{
    "error": "AUTHORIZATION_ERROR: INVALID_SIGN"
}
```

### result_code & result_message 对照表

| result_code | result_message            | 含义解释                                                     | 是否计费 |
| :---------- | :------------------------ | :----------------------------------------------------------- | :------- |
| 1000        | SUCCESS                   | 待比对照片与权威数据照片或参考照对比均是同一个人             | 是       |
| 2000        | PASS_LIVING_NOT_THE_SAME  | 通过了活体检测，但是经过验证，待比对照片与权威数据照片，或参考照片中的至少一张，不是同一个人 | 是       |
| 3000        | NO_ID_CARD_NUMBER         | 权威数据错误，可能原因：权威数据无此身份证号、权威数据照片格式错误、权威数据照片中找不到人脸等可能 | 是       |
| 3000        | ID_NUMBER_NAME_NOT_MATCH  | 身份证号和姓名不匹配                                         | 是       |
| 3000        | NO_FACE_FOUND             | 权威数据错误，可能原因：权威数据无此身份证号、权威数据照片格式错误、权威数据照片中找不到人脸等可能 | 是       |
| 3000        | NO_ID_PHOTO               | 权威数据错误，可能原因：权威数据无此身份证号、权威数据照片格式错误、权威数据照片中找不到人脸等可能 | 是       |
| 3000        | PHOTO_FORMAT_ERROR        | 权威数据错误，可能原因：权威数据无此身份证号、权威数据照片格式错误、权威数据照片中找不到人脸等可能 | 是       |
| 3000        | DATA_SOURCE_ERROR         | 权威数据出现错误                                             | 否       |
| 4100        | FAIL_LIVING_FACE_ATTACK   | 未经过活体判断，可能的原因：是假脸攻击                       | 否       |
| 4100        | VIDEO_LACK_FRAMES         | 获取到的活体数据故障，请换一台手机重试                       | 否       |
| 4100        | FAIL_EYES_CLOSE_DETECTION | 未通过闭眼检测，活体失败                                     | 否       |
| 4200        | BIZ_TOKEN_DENIED          | 传入的 biz_token 不符合要求                                  | 否       |
| 4200        | AUTHENTICATION_FAIL       | 鉴权失败                                                     | 否       |
| 4200        | MOBILE_PHONE_NOT_SUPPORT  | 手机在不支持列表里                                           | 否       |
| 4200        | SDK_TOO_OLD               | SDK版本过旧，已经不被支持                                    | 否       |
| 4200        | MOBILE_PHONE_NO_AUTHORITY | 没有权限（运动传感器、存储、相机）                           | 否       |
| 4200        | USER_CANCELLATION         | 用户活体失败，原因可能如下：用户取消了、验证过程超时等原因； | 否       |
| 4200        | USER_TIMEOUT              | 用户活体失败，原因可能如下：用户取消了、验证过程超时等原因； | 否       |
| 4200        | VERIFICATION_FAILURE      | 用户活体失败，原因可能如下：用户取消了、验证过程超时等原因； | 否       |
| 4200        | UNDETECTED_FACE           | 用户活体失败，原因可能如下：用户取消了、验证过程超时等原因； | 否       |
| 4200        | ACTION_ERROR              | 用户活体失败，原因可能如下：用户取消了、验证过程超时等原因； | 否       |

### ERROR信息

| HTTP状态码 | 错误信息                               | 说明                                                         |
| :--------- | :------------------------------------- | :----------------------------------------------------------- |
| 400        | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法解析，有可能不是图像文件、或有数据破损。<param>为 image_ref1、 image_ref2中的一个。请注意：<param>只会有一项，即第一个满足条件的名称。 |
| 400        | MISSING_ARGUMENTS:<key>                | 缺少某个必选参数。                                           |
| 400        | BAD_ARGUMENTS:<key>                    | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长） |
| 400        | NO_FACE_FOUND:<param>                  | 表示上传的 image_ref 的图像中，没有检测到人脸。              |
| 400        | INVALID_IMAGE_SIZE:<param>             | 上传的图像太大，具体是指像素尺寸的长或宽超过4096像素。<param>为 image_ref1、 image_ref2中的一个。请注意：<param>只会有一项，即第一个满足条件的名称。 |
| 400        | MEGLIVE_DATA_ERROR                     | 上传的meglive_data数据包解析失败。请将SDK产生的meglive_data数据包直接传递到此API，任何对数据包的修改都会导致数据包解析失败的问题。 |
| 400        | MEGLIVE_DATA_BIZ_TOKEN_NOT_MATCH       | 上传的meglive_data包中的biz_token和传入参数biz_token不一致。 |
| 403        | AUTHORIZATION_ERROR:<reason>           | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。 <reason>取值：API_KEY_BE_DISCONTINUED：api_key被停用IP_NOT_ALLOWED：不允许访问的IP（预留设计）MORE_RETRY_TIMES：超过重试次数EXPIRED_SIGN：签名已过期INVALID_SIGN：无效签名其他可能的错误码，请预留处理方案 |
| 429        | CONCURRENCY_LIMIT_EXCEEDED             | 并发数超过限制                                               |
| 404        | API_NOT_FOUND                          | 所调用的API不存在。                                          |
| 413        | Request Entity Too Large               | 客户发送的请求大小超过了20MB限制或单张照片大小超过了2MB。该错误的返回格式为纯文本，不是json格式。 |
| 500        | INTERNAL_ERROR                         | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系 FaceID 客服或商务 |