# App-GetBizToken

### 版本

1.0.0

### 描述

此接口用于配置人脸比对的身份核实功能，支持有源比对（调用者提供姓名、身份证号、和待核实人脸图）和无源比对（直接比对待核实人脸图和参照人脸图）。客户通过服务器将本次活体相关的配置传到FaceID服务器，在验证无误后，返回本次业务的biz_token，用FaceID MegLiveStill SDK的初始化。

**注意：**本接口仅支持FaceID MegLiveStill SDK 3.0及以上的版本来获取biz_token进行初始化，FaceID MegLive SDK 3.0以下版本请使用“人脸验证API”中的“Verify 2.0.6”下的文档。

### 调用URL：

<https://api.megvii.com/faceid/v3/sdk/get_biz_token> 

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障。

### 调用方法：

POST

### 权限

仅当用户接入FaceID产品后，才能调用FaceID各Web API。接入FaceID的流程请咨询FaceID商务人员。

### 参数

| 必选／可选                                                   | 参数                     | 类型                  | 参数说明                                                     |                                                              |
| :----------------------------------------------------------- | :----------------------- | :-------------------- | :----------------------------------------------------------- | ------------------------------------------------------------ |
| **必选**                                                     | sign                     | String                | 调用此API客户的签名，具体的签名产生方式请查阅”App-鉴权说明“文档。 |                                                              |
| **必选**                                                     | sign_version             | String                | 签名算法版本，请传递：hmac_sha1                              |                                                              |
| **必选**                                                     | liveness_type            | String                | 活体验证流程的选择，目前仅取以下值：meglive：动作活体still：静默活体raw_image：不进行活体验证，仅使用上传的图片进行后续的比对 |                                                              |
| **必选**                                                     | comparison_type          | String                | 确定本次比对为“有源比对”或“无源比对”。取值只为“1”或“0”，取其他值返回错误码400（BAD_ARGUMENTS）。0：表示无源比对，FaceID将使用客户自己提供的照片（参数image_ref[x]）作为比对人脸照。1：表示有源比对，表示最终的用户照片将与第三方权威数据照片进行比对。此外，如果没有“有源比对”的权限，也不能设置此参数为“1”，否则返回错误码 403（AUTHORIZATION_ERROR:NO_DATA_SOURCE_PERMISSION） |                                                              |
| 比对方法二选一：有源比对（comparison_type = 1）              | 必选                     | idcard_name           | String                                                       | 需要核实身份对象的姓名，使用UTF-8编码。                      |
| 必选                                                         | idcard_number            | String                | 需要核实身份对象的身份证号，也就是一个18位长度的字符串。     |                                                              |
| 可选                                                         | image_ref1               | File                  | 由客户提供的参照人脸照片。 如果在image_ref1、image_ref2中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)； 如果这些图片中任一张中有多张脸，将选择最大人脸进行比对。 |                                                              |
| 可选                                                         | image_ref2               | File                  | 由客户提供的参照人脸照片。 如果在image_ref1、image_ref2中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)； 如果这些图片中任一张中有多张脸，将选择最大人脸进行比对。 |                                                              |
| 比对方法二选一：无源比对（comparison_type = 0）              | 必选                     | uuid                  | String                                                       | 如果用户不使用数据源的数据进行比对，则上传此字段，用于标志本次识别对应的用户的唯一ID，要求不长于128字节。建议您对来自同一用户的比对请求使用同样的ID，这非常有利于您反查验证结果以及获得更好的监控报表体验。 |
| 必选                                                         | image_ref1               | File                  | 由客户提供的参照人脸照片。 如果在image_ref1、image_ref2中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)； 如果这些图片中任一张中有多张脸，将选择最大人脸进行比对。 |                                                              |
| 可选                                                         | image_ref2               | File                  | 由客户提供的参照人脸照片。 如果在image_ref1、image_ref2中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)； 如果这些图片中任一张中有多张脸，将选择最大人脸进行比对。 |                                                              |
| 可选                                                         | biz_no                   | String                | “默认为空”。客户业务流水号，建议设置为您的业务相关的流水串号并且唯一，会在return时原封不动的返回给您的服务器，以帮助您确认对应业务的归属。此字段不超过128字节。 |                                                              |
| 可选                                                         | liveness_timeout         | String                | 超时时间，示用户进入活体识别流程后的超时时间，若未在规定时间完成操作，则本次活体失败。超时时间设定：单位 秒， ∈ [5, 60]动作活体时，设置每个动作的超时时间，默认 10静默活体时，设置照镜子的超时时间，默认 60 |                                                              |
| 活体类型控制三选一：动作活体时（liveness_type = meglive）    | 可选                     | liveness_action_count | String                                                       | 配置动作活体时的动作个数：默认3个动作；默认 3， ∈ [1,3]      |
| 活体类型控制三选一：静默活体时（liveness_type = still）      | 可选                     | eyes_close_detection  | String                                                       | 表示静默活体是否进行闭眼检测，取值如下：    0：默认值，不进行睁闭眼检测；1：开启睁闭眼检测，若用户全程闭眼，则活体检测失败。 |
| 活体类型控制三选一：无活体验证时（liveness_type = raw_image） | 必选                     | image                 | File                                                         | 待比对的人脸照片。当以这种方式调用时，该方法会首先检测出image中有几张人脸，并且判断最大的那张人脸的图像质量是否足够后续的比对。您可以通过设置可选参数fail_when_multiple_faces 和 face_quality_threshold来定义人脸检测方面的行为。 |
| 可选                                                         | fail_when_multiple_faces | String                | 对待比对照作人脸检测时发现有多张脸，是否立即返回错误，或者取最大的一张脸继续比对。本参数取值只能是 “1” 或 "0" （缺省值为“1”）:“1”: 立即返回错误码400（MULTIPLE_FACES）“0”：取最大脸继续比对 |                                                              |
| 可选                                                         | face_quality_threshold   | String                | 待比对照中（人脸面积最大的一张）人脸图像质量分的阈值（缺省值为30）。待比对照人脸图像质量低于该阈值就直接返回错误码400（LOW_QUALITY）。本参数只能传入0至100的整数，传入其他整数或非整数字符串均返回错误码400（BAD_ARGUMENTS）。 |                                                              |
| 可选                                                         | return_faces             | String                | 返回人脸检测结果。本参数取值只能是“1”或“0”（缺省值为“0”）:“1”: 返回人脸检测结果(faces字段)；“0”: 不返回人脸检测结果，只返回比对结果；其他值：返回错误码400（BAD_ARGUMENTS）**注意：**只要本参数设置为1，即使调用返回错误码400（LOW_QUALITY）、400（MULTIPLE_FACES）或400（NO_FACE_FOUND），返回结果里依然会包含faces字段。 |                                                              |
| 可选                                                         | verbose                  | String                | 表示返回数据的详细程度，取值如下：0：默认值，仅返回结论1：返回结论与摘要信息 |                                                              |
| 可选                                                         | security_level           | String                | 表示对比对结果的严格程度限制，请根据您的场景，选择安全规则，越严格，准确性要求越高，通过率也会相应下降。1：宽松2：常规（默认值）3：严格4：非常严格 |                                                              |
| 可选                                                         | force_compare            | String                | 表示云端算法判断为假脸后，是否依然进行比对结果0：默认值，不开启比对。云端判断为假脸，则直接返回结果，不执行比对，可以节省成本1：进行强制比对。云端判断为假脸后，但依然进行比对，此时result中将不返回假脸的结果，而返回比对的结果，风险情况您可以通过verification进行查看（需要设置verbose=1）。注：开启强制比对后，此条记录将被纳入计费逻辑。 |                                                              |
| 可选                                                         | multi_oriented_detection | String                | 决定对于image_ref[x]参数对应的图片，当检测不出人脸时，是否旋转90度、180度、270度后再检测人脸。本参数取值只能是 “1” 或 "0" ；1：默认值，要旋转检测0：不旋转其他值：返回错误码400（BAD_ARGUMENTS）  **请注意：设置此参数为1可能会轻微增加误检人脸的概率，如果您明确您的业务场景里不存在非正向的人脸图片、或概率极低，建议勿设置此参数。** |                                                              |

### 返回值说明

| 字段       | 类型   | 说明                                                         |
| :--------- | :----- | :----------------------------------------------------------- |
| request_id | String | 用于区分每一次请求的唯一的字符串。此字符串可以用于后续数据反查。此字段必定返回。 |
| time_used  | Int    | 整个请求所花费的时间，单位为毫秒。此字段必定返回。           |
| biz_token  | String | 字符串，调用SDK时传入的信息。**注：**biz_token唯一且只能使用一次，且有效期为1小时。 |
| error      | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### 返回值示例

正确请求返回示例

```
{
   "biz_token":"1531397038,f0b9eea0-6a2d-486c-a1ba-42fdf843e8cc",
   "time_used":18,
   "request_id":"1531397037,c1223c5b-c2f6-4bb7-8cbb-8993497ddbe0"
}
```

失败请求返回示例

```
{
    "error": "BAD_ARGUMENTS: sign_version"
}
```

### ERROR信息

| HTTP 状态代码 | 错误信息                               | 说明                                                         |
| :------------ | :------------------------------------- | :----------------------------------------------------------- |
| 400           | MISSING_ARGUMENTS: <key>               | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS: <key>                   | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长） |
| 400           | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法解析，有可能不是图像文件、或有数据破损。<param>为 image_ref1、 image_ref2中的一个。请注意：<param>只会有一项，即第一个满足条件的名称。 |
| 400           | NO_FACE_FOUND:<param>                  | 表示上传的 image_ref 的图像中，没有检测到人脸。<param>为 image_ref1、 image_ref2中的一个。请注意：<param>只会有一项，即第一个满足条件的名称。 |
| 400           | INVALID_IMAGE_SIZE<param>              | 客户上传的图像太大，具体是指像素尺寸的长或宽超过4096像素。<param>为 image_ref1、 image_ref2中的一个。请注意：<param>只会有一项，即第一个满足条件的名称。 |
| 400           | LOW_QUALITY                            | image图质量太差                                              |
| 400           | MULTIPLE_FACES                         | image图片中的有多张人脸                                      |
| 403           | AUTHENTICATION_ERROR                   | 无效签名                                                     |
| 403           | AUTHORIZATION_ERROR: <reason>          | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。<reason>取值：API_KEY_BE_DISCONTINUED：api_key被停用IP_NOT_ALLOWED：不允许访问的IP（预留设计）LIMIT_REACHED：这个api_key对当前API的调用量达到上限。仅当api_key为测试keyMORE_RETRY_TIMES：比对重试次数达到上限NO_VERIFY_PERMISSION：没有无源比对的权限，但是本次请求依然尝试调用了。NO_DATA_SOURCE_PERMISSION：没有有源比对的权限，但是本次请求依然尝试调用了。 DENIED：无权限调用当前APIEXPIRED_SIGN：签名已过期INVALID_SIGN：无效签名其他可能的错误码，请预留处理方案。 |
| 403           | CONCURRENCY_LIMIT_EXCEEDED             | 并发数超过限制                                               |
| 404           | API_NOT_FOUND                          | 所调用的API不存在。                                          |
| 413           | Request Entity Too Large               | 客户发送的请求大小超过了20MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 500           | INTERNAL_ERROR                         | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系 FaceID 客服或商务 |