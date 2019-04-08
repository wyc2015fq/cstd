文档中心/Detect API

## 版本

3.0

## 描述

传入图片进行人脸检测和人脸分析。

可以检测图片内的所有人脸，对于每个检测出的人脸，会给出其唯一标识 face_token，可用于后续的人脸分析、人脸比对等操作。对于正式 API Key，支持指定图片的某一区域进行人脸检测。

本 API 支持对检测到的人脸直接进行分析，获得人脸的关键点和各类属性信息。对于试用 API Key，最多只对人脸框面积最大的 5 个人脸进行分析，其他检测到的人脸可以使用 Face Analyze API 进行分析。对于正式 API Key，支持分析所有检测到的人脸。

### 关于 face_token

如果您需要将检测出的人脸用于后续的分析、比对等操作，建议将对应的 face_token 添加到 FaceSet 中。如果一个 face_token 在 72 小时内没有存放在任一 FaceSet 中，则该 face_token 将会失效。

如果对同一张图片进行多次人脸检测，同一个人脸得到的 face_token 是不同的。

### 图片要求

图片格式：JPG(JPEG)，PNG
图片像素尺寸：最小 48*48 像素，最大 4096*4096 像素
图片文件大小：2 MB
最小人脸像素尺寸： 系统能够检测到的人脸框为一个正方形，正方形边长的最小值为图像短边长度的 48 分之一，最小值不低于 48 像素。 例如图片为 4096*3200 像素，则最小人脸像素尺寸为 66*66 像素。

### 更新日志

2017 年 10 月 18 日：提供 106 点人脸关键点 landmark。

2017 年 9 月 19 日：提供皮肤状态检测 skinstatus

2017 年 8 月 16 日：提供嘴部状态检测 mouthstatus；提供颜值分析 beauty；提供视线估计 eyegaze。

2017 年 6 月 7 日：算法重大升级；提供情绪识别 emotion。

2017 年 3 月 28 日：支持 base64 编码的图片。

2017 年 3 月 9 日：人脸检测算法升级；提供了眼睛状态检测 eyestatus，替换是否戴眼镜 glass 的检测；人脸模糊检测升级；提供了人种检测。

## 调用URL

<https://api-cn.faceplusplus.com/facepp/v3/detect>

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。其中 calculate_all 和 face_rectangle 参数只有正式 API Key 才能使用，试用 API Key 无法使用。

## 请求参数

| 是否必选                        | 参数名            | 类型                                                         | 参数说明                                                     |
| ------------------------------- | ----------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 必选                            | api_key           | String                                                       | 调用此API的API Key                                           |
| 必选                            | api_secret        | String                                                       | 调用此API的API Secret                                        |
| 必选（三选一）                  | image_url         | String                                                       | 图片的 URL。注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用 image_file 或 image_base64 参数直接上传图片。 |
| image_file                      | File              | 一个图片，二进制文件，需要用post multipart/form-data的方式上传。 |                                                              |
| image_base64                    | String            | base64 编码的二进制图片数据如果同时传入了 image_url、image_file 和 image_base64 参数，本API使用顺序为 image_file 优先，image_url 最低。 |                                                              |
| 可选                            | return_landmark   | Int                                                          | 是否检测并返回人脸关键点。合法值为：2 检测。返回 106 个人脸关键点。1检测。返回 83 个人脸关键点。0不检测注：本参数默认值为 0 |
| 可选                            | return_attributes | String                                                       | 是否检测并返回根据人脸特征判断出的年龄、性别、情绪等属性。合法值为：none不检测属性genderagesmilingheadposefacequalityblureyestatusemotionethnicitybeautymouthstatuseyegazeskinstatus希望检测并返回的属性。需要将属性组成一个用逗号分隔的字符串，属性之间的顺序没有要求。关于各属性的详细描述，参见下文“返回值”说明的 "attributes" 部分。注：在此参数中的传入参数smiling，对应在返回值的attributes中参数名为smile。在使用时请注意。注：本参数默认值为 none |
| 可选（仅正式 API Key 可以使用） | calculate_all     | Int                                                          | 是否检测并返回所有人脸的人脸关键点和人脸属性。如果不使用此功能，则本 API 只会对人脸面积最大的五个人脸分析人脸关键点和人脸属性。合法值为：1是0否注：本参数默认值为 0 |
| 可选（仅正式 API Key 可以使用） | face_rectangle    | String                                                       | 是否指定人脸框位置进行人脸检测。如果此参数传入值为空，或不传入此参数，则不使用此功能。本 API 会自动检测图片内所有区域的所有人脸。如果使用正式 API Key 对此参数传入符合格式要求的值，则使用此功能。需要传入一个字符串代表人脸框位置，系统会根据此坐标对框内的图像进行人脸检测，以及人脸关键点和人脸属性等后续操作。系统返回的人脸矩形框位置会与传入的 face_rectangle 完全一致。对于此人脸框之外的区域，系统不会进行人脸检测，也不会返回任何其他的人脸信息。参数规格：四个正整数，用逗号分隔，依次代表人脸框左上角纵坐标（top），左上角横坐标（left），人脸框宽度（width），人脸框高度（height）。例如：70,80,100,100 |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| ------------- | ------ | ------------------------------------------------------------ |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| faces         | Array  | 被检测出的人脸数组，具体包含内容见下文。注：如果没有检测出人脸则为空数组 |
| image_id      | String | 被检测的图片在系统中的标识。                                 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### faces 数组中单个元素的结构

| 字段           | 类型   | 说明                                                         |
| -------------- | ------ | ------------------------------------------------------------ |
| face_token     | String | 人脸的标识                                                   |
| face_rectangle | Object | 人脸矩形框的位置，包括以下属性。每个属性的值都是整数：top：矩形框左上角像素点的纵坐标left：矩形框左上角像素点的横坐标width：矩形框的宽度height：矩形框的高度 |
| landmark       | Object | 人脸的关键点坐标数组。当传入的 landmark 参数值为 1 时，返回 83 个关键点坐标数组。当传入的 landmark 参数值为 2 时，返回 106 个关键点坐标数组。关于 83 个或 106 个关键点坐标的详细说明与图示，请分别参考文档：《[人脸关键点 Landmark 说明（83 点）](https://console.faceplusplus.com.cn/documents/5671270)》、《[人脸关键点 Landmark 说明（106 点）](https://console.faceplusplus.com.cn/documents/13207408)》 |
| attributes     | Object | 人脸属性特征，具体包含的信息见下表。                         |

#### attributes 中包含的元素说明

| 字段        | 类型   | 说明                                                         |
| ----------- | ------ | ------------------------------------------------------------ |
| gender      | String | 性别分析结果。返回值为：Male男性Female女性                   |
| age         | Int    | 年龄分析结果。返回值为一个非负整数。                         |
| smile       | Object | 笑容分析结果。返回值包含以下属性：value：值为一个 [0,100] 的浮点数，小数点后3位有效数字。数值越大表示笑程度高。threshold：代表笑容的阈值，超过该阈值认为有笑容。 |
| glass       | String | 是否佩戴眼镜的分析结果。返回值为：None不佩戴眼镜Dark佩戴墨镜Normal佩戴普通眼镜注：由于提供了 eyestatus，所以 glass 参数将被移除（计划时间 2017-9-30），请尽快修改程序使用 eyestatus |
| headpose    | Object | 人脸姿势分析结果。返回值包含以下属性，每个属性的值为一个 [-180, 180] 的浮点数，小数点后 6 位有效数字。单位为角度。pitch_angle：抬头roll_angle：旋转（平面旋转）yaw_angle：摇头 |
| blur        | Object | 人脸模糊分析结果。返回值包含以下属性：motionblur：人脸移动模糊度分析结果。gaussianblur：人脸高斯模糊度分析结果。blurness：新的人脸模糊分析结果。 每个属性都包含以下字段：value 的值为是一个浮点数，范围 [0,100]，小数点后 3 位有效数字。threshold 表示人脸模糊度是否影响辨识的阈值。注：由于提供了blurness，所以 motionblur 和 gaussianblur 将被移除（计划时间 2017-9-30），请尽快修改程序使用blurness。目前两个字段的值会与新的blurness值相同 |
| eyestatus   | Object | 眼睛状态信息。返回值包含以下属性：left_eye_status：左眼的状态right_eye_status：右眼的状态每个属性都包含以下字段。每个字段的值都是一个浮点数，范围 [0,100]，小数点后 3 位有效数字。字段值的总和等于 100。occlusion：眼睛被遮挡的置信度no_glass_eye_open：不戴眼镜且睁眼的置信度normal_glass_eye_close：佩戴普通眼镜且闭眼的置信度normal_glass_eye_open：佩戴普通眼镜且睁眼的置信度dark_glasses：佩戴墨镜的置信度no_glass_eye_close：不戴眼镜且闭眼的置信度 |
| emotion     | Object | 情绪识别结果。返回值包含以下字段。每个字段的值都是一个浮点数，范围 [0,100]，小数点后 3 位有效数字。每个字段的返回值越大，则该字段代表的状态的置信度越高。字段值的总和等于 100。anger：愤怒disgust：厌恶fear：恐惧happiness：高兴neutral：平静sadness：伤心surprise：惊讶 |
| facequality | Object | 人脸质量判断结果。返回值包含以下属性：value：值为人脸的质量判断的分数，是一个浮点数，范围 [0,100]，小数点后 3 位有效数字。threshold：表示人脸质量基本合格的一个阈值，超过该阈值的人脸适合用于人脸比对。 |
| ethnicity   | String | 人种分析结果，返回值为：Asian亚洲人White白人Black黑人        |
| beauty      | Object | 颜值识别结果。返回值包含以下两个字段。每个字段的值是一个浮点数，范围 [0,100]，小数点后 3 位有效数字。male_score：男性认为的此人脸颜值分数。值越大，颜值越高。female_score：女性认为的此人脸颜值分数。值越大，颜值越高。 |
| mouthstatus | Object | 嘴部状态信息，包括以下字段。每个字段的值都是一个浮点数，范围 [0,100]，小数点后 3 位有效数字。字段值的总和等于 100。surgical_mask_or_respirator：嘴部被医用口罩或呼吸面罩遮挡的置信度other_occlusion：嘴部被其他物体遮挡的置信度close：嘴部没有遮挡且闭上的置信度open：嘴部没有遮挡且张开的置信度 |
| eyegaze     | Object | 眼球位置与视线方向信息。返回值包括以下属性：left_eye_gaze：左眼的位置与视线状态right_eye_gaze：右眼的位置与视线状态每个属性都包括以下字段，每个字段的值都是一个浮点数，小数点后 3 位有效数字。position_x_coordinate: 眼球中心位置的 X 轴坐标。position_y_coordinate: 眼球中心位置的 Y 轴坐标。vector_x_component: 眼球视线方向向量的 X 轴分量。vector_y_component: 眼球视线方向向量的 Y 轴分量。vector_z_component: 眼球视线方向向量的 Z 轴分量。 |
| skinstatus  | Object | 面部特征识别结果，包括以下字段。每个字段的值都是一个浮点数，范围 [0,100]，小数点后 3 位有效数字。每个字段的返回值越大，则该字段代表的状态的置信度越高。health：健康stain：色斑acne：青春痘dark_circle：黑眼圈 |

## 返回值示例

### 请求成功返回示例

```
{
	"image_id": "Dd2xUw9S/7yjr0oDHHSL/Q==",
	"request_id": "1470472868,dacf2ff1-ea45-4842-9c07-6e8418cea78b",
	"time_used": 752,
	"faces": [{
		"landmark": {
			"mouth_upper_lip_left_contour2": {
				"y": 185,
				"x": 146
			},
			"contour_chin": {
				"y": 231,
				"x": 137
			},
			.............省略关键点信息 
			"right_eye_pupil": {
				"y": 146,
				"x": 205
			},
			"mouth_upper_lip_bottom": {
				"y": 195,
				"x": 159
			}
		},
		"attributes": {
			"gender": {
				"value": "Female"
			},
			"age": {
				"value": 21
			},
			"glass": {
				"value": "None"
			},
			"headpose": {
				"yaw_angle": -26.625063,
				"pitch_angle": 12.921974,
				"roll_angle": 22.814377
			},
			"smile": {
				"threshold": 30.1,
				"value": 2.566890001296997
			}
		},
		"face_rectangle": {
			"width": 140,
			"top": 89,
			"left": 104,
			"height": 141
		},
		"face_token": "ed319e807e039ae669a4d1af0922a0c8"
	}]
}
```

### 请求失败返回示例

```
{
	"time_used": 3,
	"error_message": "MISSING_ARGUMENTS: image_url, image_file, image_base64",
	"request_id": "1470378968,c6f50ec6-49bd-4838-9923-11db04c40f8d"
}
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| ------------ | -------------------------------------- | ------------------------------------------------------------ |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损、或图片文件格式不符合要求。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param>对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |
| 400          | IMAGE_FILE_TOO_LARGE:<param>           | 客户通过参数<param>上传的图片文件太大。本 API 要求图片文件大小不超过 2 MB |
| 403          | INSUFFICIENT_PERMISSION:<param>        | 试用 API Key 无法使用 <param>对应的参数。请勿传入此参数。或者使用正式 API Key 调用。 |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| ------------- | ---------------------------- | ------------------------------------------------------------ |
| 401           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key 没有调用本 API 的权限，具体原因为：用户自己禁止该 api_key 调用、管理员禁止该 api_key 调用、由于账户余额不足禁止调用。目前的 <reason> 有：Denied by Client（用户自己禁止该 api_key 调用）Denied by Admin（管理员禁止该 api_key 调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该 API Key 的 QPS 已经达到上限。如需要提高 API Key 的 QPS 配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了 2MB 限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的 API 不存在。                                        |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/facepp/v3/detect" -F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg" \
-F "return_landmark=1" \
-F "return_attributes=gender,age"
```