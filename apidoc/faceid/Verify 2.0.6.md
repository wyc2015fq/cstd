# Verify 2.0.6

### 版本

2.0.6

### 描述

此接口提供基于人脸比对的身份核实功能，支持有源比对（调用者提供姓名、身份证号、和待核实人脸图）和无源比对（直接比对待核实人脸图和参照人脸图）。待核实人脸图可以由FaceID MegLive SDK产品提供，也可以由detect接口获得，还可以直接提供未经过detect方法检测的人脸图片。

**注意：**本接口仅支持FaceID MegLive SDK 3.0以下的版本，SDK 3.0及以上版本请使用“App端身份验证服务”下的文档。

### 调用URL：

<https://api.megvii.com/faceid/v2/verify>

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障。

### 调用方法：

POST

### 权限

 仅当用户接入FaceID产品后，才能调用FaceID各Web API。接入FaceID的流程请咨询FaceID商务人员。

 若使用Verify进行有源比对，api_key需要有“FaceID有源比对”的权限，否则返回错误码403（AUTHORIZATION_ERROR：“No data source permission. ”）

### 参数

 

|                                        | **参数名**               | **类型**                                                     | **参数说明**                                                 |                                                              |                                                              |
| -------------------------------------- | ------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 必选                                   | api_key                  | String                                                       | 调用此API的api_key；                                         |                                                              |                                                              |
| api_secret                             | String                   | 调用此API的api_key的secret；                                 |                                                              |                                                              |                                                              |
| comparison_type                        | String                   | 确定本次比对为“有源比对”或“无源比对”。取值只为“1”或“0”，取其他值返回错误码400（BAD_ARGUMENTS）。“1”表示有源比对，FaceID将连接**第三方身份信息数据源（以下简称“数据源“）**获得比对人脸照。以下的“二选一”参数组里，必须使用“有源比对时”参数组，否则可能返回错误码400（MISSING_ARGUMENTS）。此外，如果没有“有源比对”的权限，也不能设置此参数为“1”，否则返回错误码 403（AUTHORIZATION_ERROR:“No data source permission.”）  “0”表示无源比对，FaceID将使用用户自己提供的照片（参数image_ref[x]）作为比对人脸照。以下的“二选一”参数集里，必须使用“无源比对时”参数组，否则可能返回错误码400（MISSING_ARGUMENTS）。 |                                                              |                                                              |                                                              |
| face_image_type                        | String                   | 确定待比对图片的类型。取值只为“meglive”、“facetoken”、“raw_image”、“meglive_flash” 四者之一，取其他值返回错误码400（BAD_ARGUMENTS）。“meglive”，表示本次比对照为从FaceID活体检测SDK MegLive 中生成的最优照片。以下的“比对图片 四选一”参数组里，必须使用“**配合MegLive SDK使用时**”这一组参数，否则可能返回错误码400（MISSING_ARGUMENTS）。 “facetoken”，表示本次比对时，用户已经调用了FaceID的detect方法从原始图片中检测出人脸，用一个facetoken表示，然后将这个facetoken作为待比对的人脸。以下的“比对图片 四选一””参数组里，必须使用“**调用detect后获得facetoken时**”这一组参数，否则可能返回错误码400（MISSING_ARGUMENTS）。 “raw_image”，表示本次比对时，用户直接上传一张可能包含人脸的图片作为待比对的图片，FaceID将先检测图中人脸然后将人脸与参照脸比对。以下的“比对图片 四选一””参数组里，必须使用“**直接上传一张照片时**”这一组参数，否则可能返回错误码400（MISSING_ARGUMENTS）“meglive_flash”，表示本次比对的照片为从 FaceID 炫彩活体 SDK MegLiveFlash 中生成的照片。以下的“四选一”参数组里，必须使用“配合MegLiveFlash SDK使用时”这一组参数，否则可能返回错误码400（MISSING_ARGUMENTS）。 |                                                              |                                                              |                                                              |
| 比对方法二选一                         | 有源比对时               | 必选                                                         | idcard_name                                                  | String                                                       | 需要核实身份对象的姓名。在有源比对时，verify方法需要该参数和idcard_number参数去从数据源获取参考人脸图像。 |
| idcard_number                          | String                   | 需要核实身份对象的身份证号，也就是一个18位长度的字符串。在有源比对时，verify方法需要该参数和idcard_name参数去从数据源获取参考人脸图像。 |                                                              |                                                              |                                                              |
| 可选                                   | image_ref[x]             | File                                                         | 多张由您自己提供的参照人脸照片。x表示此参数可重复多次，其中1<= x <= 3，即表示可以传最多三张参照人脸照片（参数分别为image_ref1, image_ref2, image_ref3）。在有源比对时使用这个参数，主要目的是让待验证照不但与通过idcard_name和idcard_number参数获得的数据源照比对，也和您从其他渠道获得的参考照片比对（如果您有）。如果在image_ref1、image_ref2、image_ref3中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)；如果这些图片中任一张中有多张脸，将返回错误码400（MULTIPLE_FACES）。 |                                                              |                                                              |
| 无源比对时                             | 必选                     | uuid                                                         | String                                                       | 无源比对时，用于标志本次识别对应的用户的id，要求不长于512字节，否则返回错误码400（BAD_ARGUMENTS）。建议您对来自同一用户的比对请求使用同样的id，这非常有利于您反查比对结果以及获得更好的监控、报表体验。 |                                                              |
| 必选                                   | image_ref[x]             | File                                                         | 多张由您自己提供的参照人脸照片。x表示此参数可重复多次，其中1<=x <= 3，即表示可以传最多三张参照人脸照片（参数分别为image_ref1, image_ref2, image_ref3）。如果在image_ref1、image_ref2、image_ref3中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)；如果这些图片中任一张中有多张脸，将返回错误码400（MULTIPLE_FACES）。 |                                                              |                                                              |
| 比对图片四选一                         | 配合MegLive SDK使用时    | 必选                                                         | delta                                                        | String                                                       | 在配合MegLive SDK使用时，用于校验上传数据的校验字符串，此字符串会由MegLive SDK直接返回。 |
| 必选                                   | image_best               | File                                                         | 此参数请传MegLive SDK返回的质量最佳的人脸图片。              |                                                              |                                                              |
| 可选                                   | image_env                | File                                                         | 用于假脸判定，请传MegLive SDK返回的用作云端假脸攻击判定的照片，FaceID将使用image_env进行假脸判定，完整返回face_genuineness对象中的所有字段。**注意：此参数需要MegLive SDK 2.4.1版本以及更新的版本配合支持，低于2.4.1版本的MegLive SDK不返回这张照片。** |                                                              |                                                              |
| 可选                                   | image_action[x]          | File                                                         | 此字段可以传输多个文件，均为MegLive SDK返回的活体检测图片，其名称也由SDK来确定。其中1<=x<=5，即表示可以传最多五个活体检测图(参数分别为image_action1, image_action2, …, image_action5) |                                                              |                                                              |
| 可选                                   | check_delta              | String                                                       | 用于校验delta是否已经使用过，此参数时仅可取值为“0”或“1”，默认值为“0”：“1”：当使用此参数且delta已使用过时，将返回错误信息400（DELTA_USED）“0”：忽略delta是否使用过 其他值：返回400（BAD_ARGUMENTS）。**注意：通过Delta的使用状态校验，能够有效的防范攻击者使用历史数据进行反复的比对验证攻击。** |                                                              |                                                              |
| 调用detect后获得facetoken时            | 必选                     | face_token                                                   | String                                                       | 使用detect接口获得的一个标示人脸的token。您只有在调用detect方法对待验证照进行人脸检测后，才能以这种方式调用verify方法。 |                                                              |
| 直接上传一张照片时                     | 必选                     | image                                                        | File                                                         | 待比对的人脸照片。当以这种方式调用verify方法时，该方法会首先检测出image中有几张人脸，并且判断最大的那张人脸的图像质量是否足够后续的比对。您可以通过设置可选参数fail_when_multiple_faces和face_quality_threshold来定义verify方法在人脸检测方面的行为，具体请见这两个参数的描述 |                                                              |
| 可选                                   | fail_when_multiple_faces | String                                                       | 对验证照作人脸检测时发现有多张脸，是否立即返回错误，或者取最大的一张脸继续比对。本参数取值只能是 “1” 或 "0" （缺省值为“1”）:“1”: 立即返回错误码400（MULTIPLE_FACES）“0”：取最大脸继续比对其他值：返回错误码400（BAD_ARGUMENTS） |                                                              |                                                              |
| 可选                                   | face_quality_threshold   | String                                                       | 验证照中（最大的一张）人脸图像质量分的阈值（缺省值为30）。验证照人脸图像质量低于该阈值就直接返回错误码400（LOW_QUALITY）。本参数只能传入0至100的整数，传入其他整数或非整数字符串均返回错误码400（BAD_ARGUMENTS）。 |                                                              |                                                              |
| 可选                                   | return_faces             | String                                                       | 返回人脸检测结果。本参数取值只能是“1”或“0”（缺省值为“0”）:“1”: 返回人脸检测结果(faces)“0”：不返回其他值：返回错误码400（BAD_ARGUMENTS）**注意：**只要本参数设置为1，即使调用返回错误码400（LOW_QUALITY）、400（MULTIPLE_FACES）或400（NO_FACE_FOUND），返回结果里依然会包含faces字段。 |                                                              |                                                              |
| 配合MegLiveFlash （炫彩活体）SDK使用时 | 必选                     | meglive_flash_result                                         | File                                                         | 在 MegLiveFlash（炫彩活体）成功时会生成并返回一个文件，文件内容包含： 用于初始化 SDK 的 meglive_biz_no 经过算法处理的图像 活体过程中的日志信息**注意：**如果该参数传入的内容发生损坏、非炫彩活体输出、或是在失败时的输出，则返回 400（BAD_ARGUMENTS）在输入有问题的参数文件时，亦会返回 400（MEGLIVE_BIZ_NO_USED）错误上传文件的大小一般在 50KB 左右。在调用 verify 接口后，可以通过 meglive_biz_no 在 get_biz_info 接口中请求到图像、活体失败信息。 |                                                              |
| 可选                                   | multi_oriented_detection | String                                                       | 对**image**参数和**image_ref[x]**参数启用图片旋转检测功能。当**image**参数或**image_ref[x]**参数中传入的图片未检测到人脸时，是否对图片尝试旋转90度、180度、270度后再检测人脸。本参数取值只能是 “1” 或 "0" （**缺省值为“0”**）:“1”：启用旋转检测（启用旋转检测后，会增加API的调用时间）“0”：不启用旋转检测其他值：返回错误码400（BAD_ARGUMENTS） **注意：**设置此参数为1可能会轻微增加误检人脸的概率，如果您明确您的业务场景里不存在非正向的人脸图片、或概率极低，建议勿设置此参数。 |                                                              |                                                              |

注：以上参数名标红表示为本次更新新增字段

### 返回值说明

API返回的为一个JSON字符串。所有的浮点数——除特殊说明——小数点后至多保留三位

| 字段             | 类型   | 说明                                                         |
| ---------------- | ------ | ------------------------------------------------------------ |
| request_id       | String | 用于区分每一次请求的唯一的字符串。此字符串可以用于后续数据反查。除非发生404（API_NOT_FOUND )错误，此字段必定返回。 |
| result_faceid    | Object | 有源比对时，数据源人脸照片与待验证人脸照的比对结果。此字段只在接口被成功调用时返回。result_faceid对象包含如下字段："**confidence**"：比对结果的置信度，Float类型，取值［0，100］，数字越大表示两张照片越可能是同一个人。“**thresholds**”：一组用于参考的置信度阈值，Object类型，包含四个字段，均为Float类型、取值［0，100］：“**1e-3**”：误识率为千分之一的置信度阈值；“**1e-4**”：误识率为万分之一的置信度阈值；“**1e-5**”：误识率为十万分之一的置信度阈值; “**1e-6**”：误识率为百万分之一的置信度阈值。**请注意：阈值不是静态的，每次比对返回的阈值不保证相同，所以没有持久化保存阈值的必要，更不要将当前调用返回的confidence与之前调用返回的阈值比较。****关于阈值选择，以下建议仅供参考：**阈值选择主要参考两个因素：业务对安全的要求和对用户体验的要求。严格的阈值对应更高的安全度，但是比对通过率会下降，因此更容易出现用户比对多次才通过的情况，用户体验会有影响；较松的阈值带来一次通过率会提升，用户体验更好，但是出现非同一个人的概率会增大，安全性会有影响。请按业务需求偏好慎重选择。“1e-3”阈值是较松的阈值。如果confidence低于“1e-3”阈值，我们不建议认为是同一个人；如果仅高于“1e-3”阈值，勉强可以认为是同一个人。这个阈值主要针对对安全性要求较低的场景（比如在分项业务有独立密码保护的情况下刷脸登陆app），或者原则上安全性要求高、但在一个具体流程里如果发生安全事故后果不严重的场景（比如“转账”场景安全性要求高、但是当前转账的金额很小）“1e-5”、“1e－6”阈值都是较严格的阈值，一般来说置信度大于“1e-5”阈值就可以相当明确是同一个人。我们建议使用“1e-5”到关键的、高安全级别业务场景中，比如大额度的借款或者转账。“1e-6”则更加严格，适用于比较极端的场景。“1e-4”阈值的严格程度介于上述两项之间。 如果对阈值选择存疑，请咨询FaceID商务或技术支持团队。 |
| result_ref[y]    | Object | 用户上传的参考人脸照片（对应参数image_ref[y]）与待验证人脸照的比对结果。至多有三组，分别为result_ref1、result_ref2、result_ref3，这取决于image_ref[y]的个数。此对象包含的字段与result_faceid的一致，请参考对应的描述。此字段只在接口被成功调用时返回。 |
| id_exceptions    | Object | 本对象返回身份相关的异常情况，如身份证号码是否曾被冒用来攻击FaceID活体检测、数据源人像照片是否存在质量不佳等问题。调用者可通过此对象增进对比对结果的解读。**本对象仅在有源比对时（comparison_type == 1)返回，**返回包含如下字段：**"id_attacked"：**Int类型，判别身份证号码是否曾被冒用来攻击FaceID活体检测，**取值1表示曾被攻击、取值0表示未被攻击**。攻击形态包括但不限于戴面具、换人攻击、软件3D合成人脸等手段。若被判别为“是”，则有可能这个身份证号码目前存在被利用的风险。注意：判别为“是”不意味身份持有者本人参与攻击，有可能其身份被他人盗用而本人无感知。**"id_photo_monochrome"：**Int类型，数据源人像照片的色彩判断，**取值1表示是黑白照片、取值0表示是彩色照片**。数据源存在一部分人像照片是黑白的现象，黑白的照片会影响比对质量，一般来说将降低比对分数。本字段表达这样的异常。 |
| time_used        | Int    | 整个请求所花费的时间，单位为毫秒。除非发生404（API_NOT_FOUND )错误，此字段必定返回。 |
| faces            | Object | 当验证照为未经detect方法检测过的照片、且return_faces参数为1时，返回本字段。本字段包含了一张或多张脸的人脸检测信息，包括人脸位置和人脸图像质量。此字段只在接口被成功调用时返回，请见参数return_faces的说明了解相关的出错情况。faces对象包含一系列人脸检测结果，每个结果包含如下字段："**quality**"：Float类型，表示检测出的一张人脸图像的质量，取值［0，100］，越高越好。“**quality_threshold**”：Float类型，取值［0，100］，表示FaceId建议的人脸图像质量阈值，大于此阈值可以认定图像质量足够完成比对。"**rect**"：这是一个Object，表示检测出的一张人脸在原始图像中的包围盒。它包含left、top、width、height四个Float类型字段来表示坐标，均取值［0，1］，小数点后数字不限3位。left、top分别表示此包围盒的左上角相对于原始图像左上角的位置的比例，width、height分别表示此包围盒相对原始图像的宽、高的比例，比如 left =0.5、top=0.5、width =0.5、height＝0.5 表示该包围盒的左上角是图像正中心，右下角是图像的右下角。**orientation**：Int类型，本字段仅在参数 multi_oriented_detection为“1”时才返回，表示人脸框的朝向，目前仅取0、90、180或270，单位为度，按顺时针方向。以人脸来说，0度表示通常意义上的正向、90度为头顶朝右下巴朝左、180度表示上下颠掉、270度表示头顶朝左下巴朝右。按此角度逆时针旋转图片即可获得正向的人像照。 |
| face_genuineness | Object | 该字段表示待比对的脸的真实性。“真实的人脸”是指待比对的人脸图像是真实人脸的拍摄，而不是戴面具的脸、通过软件人工合成的脸、或是屏幕翻拍回放的脸。本字段返回真实性检查结果以及用作参考的相关阈值。**仅当待比对照为来自MegLive模块时（face_image_type == "meglive" OR "meglive_flash"）才返回此对象**。本对象包括如下字段：**synthetic_face_confidence**：Float类型，取值［0，1］，表示人脸照片为软件合成脸的置信度。**synthetic_face_threshold**：Float类型，取值［0，1］，表示人脸照片为软件合成脸的置信度阈值。 如果synthetic_face_confidence < synthetic_face_threshold，可以认为人脸不是软件合成脸。**mask_confidence**：Float类型，取值［0，1］，表示人脸照片为面具的置信度。**mask_threshold**：Float类型，取值［0，1］，表示人脸照片为面具的置信度阈值。 如果mask_confidence < mask_threshold，可以认为人脸不是面具。**screen_replay_confidence**：Float类型，取值［0，1］，表示人脸照片为屏幕翻拍的置信度。**注意：此字段只有在调用时传入了image_env参数才返回。****screen_replay_threshold**：Float类型，取值［0，1］，表示人脸照片为屏幕翻拍的置信度阈值。 如果screen_replay_confidence < screen_replay_threshold，可以认为人脸不是屏幕翻拍。**注意：此字段只有在调用时传入了image_env参数才返回。（face_image_type == "meglive_flash" 不返回此字段）** **face_replaced**：Int类型，只取值0或1。0表示未检测出换脸攻击，1表示检测出了换脸攻击。注意：换脸攻击的检测需要MegLive SDK 2.4.3以上版本配合，仅当在移动端上使用的活体SDK版本**不低于2.4.3**或为炫彩活体时，才返回该字段，否则不返回该字段。 |
| error_message    | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

注：以上标红部分表示为本次更新新增字段

### 返回值示例

正确请求返回示例

```
{
   "request_id":"1461740007,71eeb124-08f0-47b3-8fc8-ac048cfa1372",
   "result_faceid":{
      "confidence":68.918,
      "thresholds":{
         "1e-3":64,
         "1e-4":69,
         "1e-5":74,
         "1e-6":79.9
      }
   },
   "result_ref1":{
      "confidence":68.918,
      "thresholds":{
         "1e-3":64,
         "1e-4":69,
         "1e-5":74,
         "1e-6":79.9
      }
   },
   "id_exceptions":{
      "id_attacked":0,
      "id_photo_monochrome":0
   },
   "faces":[
      {
         "quality":38.221,
         "quality_threshold":30.1,
         "rect":{
            "left":0.18,
            "top":0.18,
            "width":0.596,
            "height":0.596
         },
         "orientation":90
      }
   ],
   "face_genuineness":{
      "synthetic_face_confidence":0.88,
      "synthetic_face_threshold":0.5,
      "mask_confidence":0.32,
      "mask_threshold":0.5,
      "screen_replay_confidence":0.0,
      "screen_replay_threshold":0.5,
      "face_replaced":0
   },
   "time_used":1020
}
```

 

请求失败返回示例

```
{
   "error_message" : "NO_SUCH_ID_NUMBER",
   "request_id" : "1461740007,71eeb124-08f0-47b3-8fc8-ac048cfa1372",
   "time_used" : 4
}
```



### Verify API特有的ERROR_MESSAGE



| **HTTP 状态代码** | **错误信息**                            | **说明**                                                     | **是否计费**                      |
| ----------------- | --------------------------------------- | ------------------------------------------------------------ | --------------------------------- |
| 400               | IMAGE_ERROR_UNSUPPORTED_FORMAT: <param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。<param>为 data_source、 facetoken、 image_ref1、 image_ref2、 image_ref3、 image中的一个。当<param>取值为data_source时，表示用户的姓名和身份证号已经通过了验证，但数据源返回的图片无法解析、或者数据源没有照片。**注意：这对应Verify 1.0 API里的错误码 400（IDCARD_NO_PHOTO）** 当<param>取其他值时对应相应的参数。 | 是（仅当<param>为 data_source时） |
| 400               | NO_FACE_FOUND: <param>                  | 从数据源获得的身份证图像、用户传递的image图像，或是用户传递image_ref图像中没有找到人脸。请注意：<param>只会有一项，即第一个满足条件的名称。当有源比对时，如果数据源没有指定人的图像、或者图像里没有检测到人脸，参数<param>值为data_source；当image中没有人脸时，参数<param>值为image当image_ref1、image_ref2、image_ref3中没有人脸时，参数<param>分别返回各自的参数名。 | 是（仅当<param>为 data_source时） |
| 400               | NO_SUCH_ID_NUMBER                       | 有源比对时，数据源中没有此身份证号码的记录。                 | 是                                |
| 400               | ID_NUMBER_NAME_NOT_MATCH                | 有源比对时，数据源中存在此身份证号码，但与提供的姓名不匹配。 | 是                                |
| 400               | INVALID_IMAGE_SIZE: <param>             | 客户上传的图像太大。具体是指图像存储尺寸超过2MB、或者像素尺寸的长或宽超过4096像素。<param>对应图像太大的那个参数的名称，为image_ref1、 image_ref2、 image_ref3、 image中的一个。请注意：如果图片存储尺寸大于2MB，会触发错误413(Request Entity Too Large)。 | 否                                |
| 400               | MULTIPLE_FACES: <param>                 | 参数<param>对应的图像中发现有多张脸。<param>为image_ref1、image_ref2、image_ref3、image中的一个。仅当fail_when_multiple_faces取值为1时，<param>才可能取值为image。请注意：<param>只会有一项，即第一个满足条件的名称。 | 否                                |
| 400               | LOW_QUALITY                             | 参数image对应的图像中的人脸图像质量低于阈值face_quality_threshold。请注意：image参数对应的图像中可能存在多张脸，这里指的“人脸”是指其中的面积最大的脸。image发现多张脸可能导致400（MULTIPLE_FACES）的错误，请参考该错误码的说明。 | 否                                |
| 400               | INVALID_FACE_TOKEN: <token>             | 使用face token作为待验证图时，所传的face_token不存在。       | 否                                |
| 400               | INVALID_NAME_FORMAT                     | 有源比对时，idcard_name参数字符数过多（多于32字符）、或者使用错误的编码（姓名要求以UTF-8编码）。 | 否                                |
| 400               | INVALID_IDCARD_NUMBER                   | 有源比对时，idcard_number参数不是正确的身份证号码格式。身份证号码必定为18位数字，且最后一位可以是X（大小写均可）。 | 否                                |
| 400               | DATA_VALIDATION_ERROR                   | 配合MegLive SDK使用时，delta 参数的校验数据与上传的图像无法一一对应，或者图像上传不完整。 | 否                                |
| 400               | DATA_SOURCE_ERROR                       | 有源比对时，verify方法调用数据源发生错误，一般来说是数据源出错。出现此错误时建议停止业务，并立即联系FaceID客服或商务，待确定数据源正常后再开启业务。 | 否                                |
| 400               | DELTA_USED                              | 配合MegLive SDK使用时，系统检验出delta已被使用过。此错误仅当客户设置了可选参数check_delta且delta确实被使用过时才触发错误返回，其他情况下不触发此错误信息 | 否                                |
| 400               | MEGLIVE_BIZ_NO_USED                     | 配合MegLiveFlash SDK使用时，需要通过一个 meglive_biz_no 进行算法初始化。如果使用重复的 meglive_biz_no ，并将生成的结果上传至 verify 接口调用，则会返回此错误。 | 否                                |

注：以上标红部分表示为本次更新新增字段

### 通用的ERROR_MESSAGE

 

| **HTTP 状态代码** | **错误信息**                 | **说明**                                                     | **是否计费** |
| ----------------- | ---------------------------- | ------------------------------------------------------------ | ------------ |
| 403               | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。<reason>有："Limit reached." : （这个api_key对当前API的调用量达到上限。仅当api_key为测试key时才有可能发生） "Denied." : （没有权限调用当前API）"No data source permission." : （仅为FaceID Verify API存在，表示使用“有源比对”的方式调用FaceID Verify API，但是没有“有源比对”的权限） | 否           |
| 403               | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。                                             | 否           |
| 400               | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           | 否           |
| 400               | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） | 否           |
| 413               | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 | 否           |
| 404               | API_NOT_FOUND                | 所调用的API不存在。                                          | 否           |
| 500               | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系FaceID客服或商务。 | 否           |

### 调用示例

有源比对、使用MegLive的输出：

curl "https://api.megvii.com/faceid/v2/verify" -F api_key=<api_key>

-F api_secret=<api_secret>

-F comparison_type="1"

-F face_image_type="meglive"

-F "idcard_name=姓名" -F idcard_number=<18位身份号>

-F image_best=@image_best.jpg

-F image_env=@image_env.jpg

-F image_action1=@image_action1.jpg

-F image_action2=@image_action2.jpg

-F image_action3=@image_action3.jpg

-F "delta=<delta>"

 

有源比对、使用Face token：

curl "https://api.megvii.com/faceid/v2/verify" -F api_key=<api_key>

-F api_secret=<api_secret>

-F comparison_type="1"

-F face_image_type="facetoken"

-F "idcard_name=姓名" -F idcard_number=<18位身份号>

-F face_token=<facetoken>

 

有源比对、使用未detect过的人脸照片：

curl "https://api.megvii.com/faceid/v2/verify" -F api_key=<api_key>

-F api_secret=<api_secret>

-F comparison_type="1"

-F face_image_type="raw_image"

-F "idcard_name=姓名" -F idcard_number=<18位身份号>

-F image=<待验证人脸照>

 

无源比对、使用MegLive的输出：

curl "https://api.megvii.com/faceid/v2/verify" -F api_key=<api_key>

-F api_secret=<api_secret>

-F comparison_type="0"

-F face_image_type="meglive"

-F "uuid=<用户的在您的系统中的id>"

-F image_ref1=@image_ref1.jpg

-F image_best=@image_best.jpg

-F image_env=@image_env.jpg

-F "delta=<delta.txt>"

 

无源比对、使用Face token：

curl "https://api.megvii.com/faceid/v2/verify" -F api_key=<api_key>

-F api_secret=<api_secret>

-F comparison_type="0"

-F face_image_type="facetoken"

-F "uuid=<用户的在您的系统中的id>"

-F image_ref1=@image_ref1.jpg

-F face_token=<facetoken>

 

无源比对、使用未detect过的人脸照片：

curl "https://api.megvii.com/faceid/v2/verify" -F api_key=<api_key>

-F api_secret=<api_secret>

-F comparison_type="0"

-F face_image_type="raw_image"

-F "uuid=<用户的在您的系统中的id>"

-F image_ref1=@image_ref1.jpg

-F image=<待验证人脸照>

 

无源比对、使用MegLiveFlash的输出：

curl "https://api.megvii.com/faceid/v2/verify" -F api_key=<api_key>

-F api_secret=<api_secret>

-F comparison_type="0"

-F face_image_type="megliveflash"

-F "uuid=<用户的在您的系统中的id>"

-F image_ref1=@image_ref1.jpg

-F megliveflash_result_content=@content_file

 

 