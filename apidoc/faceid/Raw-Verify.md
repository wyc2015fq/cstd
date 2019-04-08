# Raw-Verify

### 描述

用于Raw-ValidateSideFace或Raw-ValidateVideo API之后的活体结果验证、人脸比对、攻击判断等，并相应的返回结果。

仅当appkey有网页验证API服务的视频验证、或自拍验证权限时，才可以调用此API，否则返回错误码 403（AUTHORIZATION_ERROR:Denied）。

### 调用URL

  <https://api.megvii.com/faceid/lite/raw/verify>

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障

### 调用方法

POST

### 参数

| 必选／可选                                 | 参数                     | 类型        | 参数说明                                                     |                                                              |
| :----------------------------------------- | :----------------------- | :---------- | :----------------------------------------------------------- | ------------------------------------------------------------ |
| 必选                                       | api_key                  | String      | 调用此API的api_key；                                         |                                                              |
| 必选                                       | api_secret               | String      | 调用此API的api_key的secret；                                 |                                                              |
| 必选                                       | token                    | String      | 仅接受是Raw-ValidateSideFace API返回的token_side_face，或Raw-ValidateVideo API返回的token_video作为参数。 |                                                              |
| 可选                                       | biz_no                   | String      | 用于标志一次验证流程，不超过128字符。如果要使用此参数，**强烈建议对一次验证流程中调用的API（比如Raw-GetRandomNumber、Raw-ValidateVideo、Raw-Verify）使用同一个biz_no，对不同的验证流程使用不同的biz_no**。 |                                                              |
| 必选                                       | comparison_type          | String      | 设定本次核身服务的类型，目前支持的比对类型为“有源比对”（取值“1”）或“无源比对”（取值“0”）。传递其他值调用将识别，返回错误码400（BAD_ARGUMENTS）。“1”表示有源比对，表示将与第三方数据源照片比对，同时也可以与自己提供的照片（参数image_ref[x]）比对；**请注意：如果appkey没有有源比对的权限，设置此值将返回错误码 403（AUTHORIZATION_ERROR:Denied）；**“0”表示无源比对，表示将与自己提供的照片（参数image_ref[x]）比对。**请注意：如果appkey没有无源比对的权限，设置此值将返回错误码 403（AUTHORIZATION_ERROR:Denied）。** |                                                              |
| 可选                                       | liveness_preferences     | String      | 本参数可选，表示一系列可以放松或加强活体检测的特别的选项。目前可用的选项有：“none”：表示没有特别的选项，**此为默认值。**"selfie_no_metadata_check"：（仅双自拍验证流程时生效）表示不限定照片格式为JPG、也不校验照片的元信息。"video_strict"：（仅当使用视频验证时有效）表示针对上传的视频进行相对严格的活体检测，此设置会提高安全性，但在一定程度上影响通过率。设置其他值，均会返回400（BAD_ARGUMENTS）。 |                                                              |
| 二选一：有源比对时（comparison_type ＝ 1） | 必选                     | idcard_name | String                                                       | 需要核实身份对象的姓名，使用UTF-8编码。                      |
| 必选                                       | idcard_number            | String      | 需要合适身份对象的身份证号，也就是一个18位长度的字符串。     |                                                              |
| 可选                                       | image_ref[x]             | File        | 多张由您提供的参照人脸照片。x表示此参数可重复多次，其中1 <= x <= 3，即表示可以传最多二张参照人脸照片（参数分别为image_ref1、image_ref2、image_ref3）。如果在image_ref[x]中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)；如果这些图片中任一张中有多张脸，将返回错误码400（MULTIPLE_FACES）。 |                                                              |
| 二选一：无源比对时（comparison_type ＝ 0） | 必选                     | uuid        | String                                                       | 如果用户不使用数据源的数据进行比对，则上传此字段，用于标志本次识别对应的用户的唯一ID，要求不长于512字节。建议您对来自同一用户的比对请求使用同样的ID，这非常有利于您反查验证结果以及获得更好的监控报表体验。 |
| 必选                                       | image_ref[x]             | File        | 多张由您自己提供的参照人脸照片。x表示此参数可重复多次，其中1<=x <= 3，即表示可以传最多三张参照人脸照片（参数分别为image_ref1、image_ref2、image_ref3）。如果在image_ref[x]中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)；如果这些图片中任一张中有多张脸，将返回错误码400（MULTIPLE_FACES）。 |                                                              |
| 可选                                       | multi_oriented_detection | String      | 决定对于image_ref[x]参数对应的图片，当检测不出人脸时，是否旋转90度、180度、270度后再检测人脸。本参数取值只能是 “1” 或 "0" （**缺省值为“0”**）:“1”：要旋转检测；“0”：不旋转；其他值：返回错误码400（BAD_ARGUMENTS） **请注意：设置此参数为1可能会轻微增加误检人脸的概率，如果您明确您的业务场景里不存在非正向的人脸图片、或概率极低，建议勿设置此参数。** |                                                              |

##  

### 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。此字符串可以用于后续数据反查。此字段必定返回。 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。此字段必定返回。           |
| biz_no        | String | **此参数仅当调用时设置了biz_no参数才返回，但是如果设置了则即使API调用失败也返回，**值与传入的biz_no保持完全一致。 |
| liveness      | Object | 本字段结构体表示流程验证结果、活体检测的结果。本字段结构体包含两个字段：**procedure_validation**：流程验证的结果，它取如下值： "**PASSED"**：流程验证通过。"**VIDEO_SR_ERROR**"：语音识别结果与要求不符。**（仅当视频验证流程时）**"**VIDEO_NOT_SYNCHRONIZED**"：视频中唇语识别错误。**（仅当视频验证流程时）****"VIDEO_FACE_INCONSISTENT":** 视频过程中的人脸不一致"**SELFIE_INCONSISTENT**"：正脸侧脸照不一致 。**（仅双角度活体验证时）****"SELFIE_METADATA_INCONSISTENT"：**正侧脸照的照片不都为JPG编码、或者元信息不一致。**（仅双角度活体验证时，且liveness_preferences不为“selfie_no_metadata_check”；对此项的判定优先于对“SELFIE_INCONSISTENT”的判定。）****face_genuineness**：假脸攻击判定的结果，它取如下值：“**PASSED**”：不是假脸。“**MASK**”：面具攻击。“**SCREEN_REPLAY**”：屏幕翻拍。“**FACEGEN**”：软件合成脸。  **请注意：****一般仅在procedure_validation、face_genuineness 都为“PASSED”时，才认为活体认定通过。****如果procedure_validation不为PASSED，将不返回face_genuineness字段，也不会进行人脸比对。****随产品迭代，本结构体中各字段可能出现新的取值，或者下线已存在的取值，请在集成开发时务必保留灵活性。** |
| result_faceid | Object | **本字段仅在参数comparison_type == 1返回，**返回第三方数据源人脸照片与待验证人脸照的比对结果。此字段只在接口被成功调用时返回。result_faceid对象包含如下字段："**confidence**"：比对结果的置信度，Float类型，取值［0，100］，数字越大表示两张照片越可能是同一个人。“**thresholds**”：一组用于参考的置信度阈值，Object类型，包含四个字段，均为Float类型，取值［0，100］：“**1e-3**”：误识率为千分之一的置信度阈值；“**1e-4**”：误识率为万分之一的置信度阈值；“**1e-5**”：误识率为十万分之一的置信度阈值; “**1e-6**”：误识率为百万分之一的置信度阈值。**请注意：阈值不是静态的，每次比对返回的阈值不保证相同，所以没有持久化保存阈值的必要，更不要将当前调用返回的confidence与之前调用返回的阈值比较。****关于阈值选择，以下建议仅供参考：**阈值选择主要参考两个因素：业务对安全的要求和对用户体验的要求。严格的阈值对应更高的安全度，但是比对通过率会下降，因此更容易出现用户比对多次才通过的情况，用户体验会有影响；较松的阈值带来一次通过率会提升，用户体验更好，但是出现非同一个人的概率会增大，安全性会有影响。请按业务需求偏好慎重选择。“1e-3”阈值是较松的阈值。如果confidence低于“1e-3”阈值，我们不建议认为是同一个人；如果仅高于“1e-3”阈值，勉强可以认为是同一个人。这个阈值主要针对对安全性相对要求较低的场景（比如在分项业务有独立密码保护的情况下刷脸登陆app），或者原则上安全性要求高、但在一个具体流程里如果发生安全事故后果不严重的场景（比如“转账”场景安全性要求高、但是当前转账的金额很小）。“1e-5”、“1e-6”阈值都是较严格的阈值，一般来说置信度大于“1e-5”阈值就可以相当明确是同一个人。我们建议使用“1e-5”到关键的、高安全级别业务场景中，比如大额度的借款或者转账。“1e-6”则更加严格，适用于对安全性要求比较极端的场景。“1e-4”阈值的严格程度介于“1e-3”阈值与“1e-5”阈值之间。 如果对阈值选择存疑，请咨询FaceID商务或技术支持团队。 |
| result_ref[x] | Object | 本字段仅在设置了image_ref1、image_ref2或image_ref3时才返回、且只在接口被成功调用时返回。返回image_ref[x]与待验证人脸照的比对结果，分别为result_ref1、result_ref2、result_ref3。此对象包含的字段与result_faceid的一致，请参考对应的描述。 |
| id_exceptions | Object | **本对象仅在参数comparison_type == 1返回，**返回身份相关的异常情况，如身份证号码是否曾被冒用来攻击FaceID活体检测、身份证数据源的人像照片是否存在质量不佳等问题。可通过此字段增进对比对结果的解读。本对象包含如下字段：**"id_attacked"：**Int类型，判别身份证号码是否曾被冒用来攻击FaceID活体检测，**取值1表示曾被攻击、取值0表示未被攻击**。攻击形态包括但不限于戴面具、换人攻击、软件3D合成人脸等手段。若被判别为“是”，则有可能这个身份证号码目前存在被利用的风险。注意：判别为“是”不意味身份持有者本人参与攻击，有可能其身份被他人盗用而本人无感知。**"id_photo_monochrome"：**Int类型，数据源身份证人像照片的色彩判断，**取值1表示是黑白照片、取值0表示是彩色照片**。身份数据源存在一部分人像照片是黑白的现象，黑白的照片会影响比对质量，一般来说将降低比对分数。本字段表达这样的异常。 |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

###  

### 成功

```
{
  "time_used": 1943, 
  "id_exceptions": {
    "id_photo_monochrome": 0, 
    "id_attacked": 0
  }, 
  "liveness": {
    "procedure_validation": "PASSED", 
    "face_genuineness": "PASSED"
  }, 
  "result_faceid": {
    "confidence": 59.873, 
    "thresholds": {
      "1e-3": 65.3, 
      "1e-5": 76.5, 
      "1e-4": 71.8, 
      "1e-6": 79.9
    }
  }, 
  "request_id": "1490012180,eca42fde-ef75-4fb3-8a98-6299e5bf8c4b"
}
```

 

### 失败

```
{
  "time_used": 2,
  "biz_no": "3149525e-2c24-4862-8e9f-92040595f0a4",
  "error_message": "INVALID_TOKEN",
  "request_id": "1490026804,adcb6619-c8f0-49ae-a643-00dc77356184"
}
```

 

### 错误码列表

#### 特有的 ERROR_MESSAGE

 

| HTTP 状态代码 | 错误信息                               | 说明                                                         | 是否计费                          |
| :------------ | :------------------------------------- | :----------------------------------------------------------- | :-------------------------------- |
| 400           | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。<param>为 data_source、 image_ref1、 image_ref2、 image_ref3中的一个。当<param>取值为data_source时，表示数据源的图片无法解析、或者数据源没有照片；当<param>取其他值时对应相应的参数。 | 是（仅当<param>为 data_source时） |
| 400           | NO_FACE_FOUND:<param>                  | 参数<param>对应的图像没有检测到人脸。                        | 是（仅当<param>为 data_source时） |
| 400           | NO_SUCH_ID_NUMBER                      | 有源比对时，数据源中没有此身份证号码的记录。                 | 是                                |
| 400           | ID_NUMBER_NAME_NOT_MATCH               | 有源比对时，数据源中存在此身份证号码，但与提供的姓名不匹配。 | 是                                |
| 400           | INVALID_TOKEN                          | token不存在、过期、或格式错误、或不是同一个API Key调用的所返回的token | 否                                |
| 400           | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像太大。具体是指像素尺寸的长或宽超过4096像素。<param>对应图像太大的那个参数的名称。**请注意：如果图片存储尺寸大于2MB，会触发错误413(Request Entity Too Large)。** | 否                                |
| 400           | MULTIPLE_FACES:<param>                 | 参数<param>对应的图像检测到了多张人脸。                      | 否                                |
| 400           | INVALID_NAME_FORMAT                    | idcard_name参数字符数过多（多于32字符）、或者使用错误的编码（姓名要求以UTF-8编码）。 | 否                                |
| 400           | INVALID_IDCARD_NUMBER                  | idcard_number参数不是正确的身份证号码格式。身份证号码必定为18位数字，且最后一位可以是X（大小写均可）。 | 否                                |
| 400           | DATA_SOURCE_ERROR                      | 有源比对时，verify方法调用数据源发生错误，一般来说是数据源出错。出现此错误时建议停止业务，并立即联系FaceID客服或商务，待确定数据源正常后再开启业务。 | 否                                |

#### 通用的ERROR_MESSAGE

 

| HTTP 状态代码 | 错误信息                     | 说明                                                         | 是否计费 |
| :------------ | :--------------------------- | :----------------------------------------------------------- | :------- |
| 403           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               | 否       |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有：Denied. （没有权限调用当前API） | 否       |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。                                             | 否       |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           | 否       |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 长度过长;不允许的输入值。） | 否       |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          | 否       |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系 FaceID 客服或商务。 | 否       |