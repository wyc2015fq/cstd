# HumanBody Detect API（V1）

## 版本

1.0

## 描述

传入图片进行人体检测和人体属性分析。

本 API 支持检测图片内的所有人体，并且支持对检测到的人体直接进行分析，获得每个人体的各类属性信息。

### 图片要求

图片格式：JPG(JPEG)，PNG

图片像素尺寸：最小 48*48 像素，最大 1280*1280 像素

图片文件大小：2 MB

## 调用URL

https://api-cn.faceplusplus.com/humanbodypp/v1/detect

## 调用方法

POST

## 权限

所有 API Key 均可调用本 API。

 

## 请求参数 

| 是否必选       | 参数名            | 类型                                                         | 参数说明                                                     |
| :------------- | :---------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key           | String                                                       | 调用此 API 的 API Key                                        |
| 必选           | api_secret        | String                                                       | 调用此 API 的 API Secret                                     |
| 必选（三选一） | image_url         | String                                                       | 图片的 URL注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用 image_file 或 image_base64 参数直接上传图片 |
| image_file     | File              | 一个图片，二进制文件，需要用 post multipart / form-data 的方式上传 |                                                              |
| image_base64   | String            | base64 编码的二进制图片数据如果同时传入了 image_url、image_file 和 image_base64 参数，本API使用顺序为 image_file 优先，image_url 最低 |                                                              |
| 可选           | return_attributes | String                                                       | 是否检测并返回根据人体特征判断出的性别，服装颜色等属性。合法值为：none不检测属性注：本参数默认值为 nonegenderupper_body_clothlower_body_cloth希望检测并返回的属性需要将属性组成一个用逗号分隔的字符串，属性之间的顺序没有要求关于各属性的详细描述，参见下文“返回值”说明的 "attributes" 部分 |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串                             |
| humanbodies   | Array  | 被检测出的人体数组，具体包含内容见下文注：如果没有检测出的人体则为空数组 |
| image_id      | String | 被检测的图片在系统中的标识                                   |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒                             |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在 |

### humanbodies 数组中单个元素的结构

| 字段                | 类型   | 说明                                                         |
| :------------------ | :----- | :----------------------------------------------------------- |
| confidence          | Float  | 人体检测的置信度，范围 [0,100]，小数点后3位有效数字，数字越大表示检测到的对象为人体的置信度越大 |
| humanbody_rectangle | Object | 人体矩形框的位置，包括以下属性。每个属性的值都是整数：top：矩形框左上角像素点的纵坐标left：矩形框左上角像素点的横坐标width：矩形框的宽度height：矩形框的高度 |
| attributes          | Object | 人体属性特征，具体包含的信息见下表                           |

#### attributes 中包含的元素说明

| 字段             | 类型   | 说明                                                         |
| :--------------- | :----- | :----------------------------------------------------------- |
| gender           | Object | 性别分析结果，返回值包含以下字段。每个字段的值都是一个浮点数，范围 [0,100]，小数点后 3 位有效数字。字段值的总和等于 100。male：性别为男性的置信度female：性别为女性的置信度 |
| upper_body_cloth | Object | 上身分析结果。返回值包含以下属性：upper_body_cloth_color：上身衣物颜色，值为下方颜色列表中与上身衣物颜色最接近的颜色值upper_body_cloth_color_rgb：上身衣物颜色 RGB 值。upper_body_cloth_color_rgb 属性包含以下字段：r ：红色通道值g：绿色通道值b：蓝色通道值 |
| lower_body_cloth | Object | 下身分析结果。返回值包含以下属性：lower_body_cloth_color：下身衣物颜色，值为下方颜色列表中与下身衣物颜色最接近的颜色值lower_body_cloth_color_rgb：下身衣物颜色 RGB 值。lower_body_cloth_color_rgb 属性包含以下字段：r ：红色通道值g：绿色通道值b：蓝色通道值 |

### 颜色列表

| 颜色值  | 颜色说明 | R    | G    | B    |
| :------ | :------- | :--- | :--- | :--- |
| black   | 黑色     | 0    | 0    | 0    |
| white   | 白色     | 255  | 255  | 255  |
| red     | 红色     | 255  | 0    | 0    |
| green   | 绿色     | 0    | 255  | 0    |
| blue    | 蓝色     | 0    | 0    | 255  |
| yellow  | 黄色     | 255  | 255  | 0    |
| magenta | 洋红     | 255  | 0    | 255  |
| cyan    | 青色     | 0    | 255  | 255  |
| gray    | 灰色     | 128  | 128  | 128  |
| purple  | 紫色     | 128  | 0    | 128  |
| orange  | 橙色     | 255  | 128  | 0    |

## 返回值示例

### 请求成功返回示例:

```json
{
	"image_id": "7OO7N1dYiJjszvV38oKVpw==",
	"request_id": "1491569448,de5a441f-6c6f-4955-896c-37b8bb2d4197",
	"time_used": 915,
	"humanbodies": [{
		"attributes": {
			"gender": {
				"male": 99.000,
				"female": 1.000
			},
			"upper_body_cloth":{
				"upper_body_cloth_color":white
				"upper_body_cloth_color_rgb":{
					"r":255,
					"g":255,
					"b":255
				}
			},
			"lower_body_cloth":{
				"lower_body_cloth_color":white
				"lower_body_cloth_color_rgb":{
					"r":255,
					"g":255,
					"b":255
				}
			}
		},
		"humanbody_rectangle": {
			"width": 456,
			"top": 0,
			"height": 500,
			"left": 0
		},
		"confidence": 99.905
	}]
}
```

### 请求失败返回示例:

```json
{
	"time_used": 3,
	"error_message": "MISSING_ARGUMENTS: image_url, image_file, image_base64",
	"request_id": "1470378968,c6f50ec6-49bd-4838-9923-11db04c40f8d"
}
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数 <param> 对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param> 对应图像太大的那个参数的名称。 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的 image_url 下载图片，图片 URL 错误或者无效。     |
| 400          | IMAGE_FILE_TOO_LARGE:<param>           | 客户通过参数 <param>上传的图片文件太大。本 API 要求图片文件大小不超过 2 MB。 |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时。                                               |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key 没有调用本 API 的权限，具体原因为：用户自己禁止该 api_key 调用、管理员禁止该 api_key 调用、由于账户余额不足禁止调用。目前的 <reason> 有：Denied by Client（用户自己禁止该 api_key 调用）Denied by Admin（管理员禁止该 api_key 调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该 API Key 的 QPS 已经达到上限。如需要提高 API Key 的 QPS 配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了 2MB 限制。该错误的返回格式为纯文本，不是 json 格式。 |
| 404           | API_NOT_FOUND                | 所调用的 API 不存在。                                        |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```shell
curl -X POST "https://api-cn.faceplusplus.com/humanbodypp/v1/detect" -F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg" \
-F "return_attributes=gender"
```