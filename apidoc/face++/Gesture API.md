# Gesture API

## 版本

V1

## 描述

调用者提供图片文件或者图片URL，检测图片中出现的所有的手部，并返回其在图片中的矩形框位置与相应的手势含义。目前可以识别 19 种手势。

识别出的手部位置会以一个矩形框表示。矩形框包含的范围从手腕到指尖。

注意：本算法目前是专为移动设备自拍场景设计，在其他场景下对手势的识别精度可能不足。

### 图片要求

图片格式：JPG(JPEG)，PNG

图片像素尺寸：最小 300*300 像素，最大 4096*4096 像素。图片短边不得低于 300 像素。

图片文件大小：2MB

最小手部像素尺寸： 系统能够检测到的手部框为一个矩形框。矩形框的最短边边长建议不小于图片最短边边长的 1 / 10。例如图片为 4096*3200 像素，则建议的最小手部框最短边尺寸为 320 像素。如果不满足此要求，则可能会影响识别精度。

## 调用URL

https://api-cn.faceplusplus.com/humanbodypp/v1/gesture

## 调用方法

POST

## 权限

所有 API Key 均可调用本 API。

 

## 请求参数 

| 是否必选       | 参数名         | 类型                                                         | 参数说明                                                     |
| :------------- | :------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key        | String                                                       | 调用此API的API Key                                           |
| 必选           | api_secret     | String                                                       | 调用此API的API Secret                                        |
| 必选（三选一） | image_url      | String                                                       | 图片的URL。注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用image_file参数直接上传图片。 |
| image_file     | File           | 一个图片，二进制文件，需要用post multipart/form-data的方式上传。 |                                                              |
| image_base64   | String         | base64编码的二进制图片数据如果同时传入了image_url、image_file和image_base64参数，本API使用顺序为image_file优先，image_url最低。 |                                                              |
| 可选           | return_gesture | String                                                       | 是否计算并返回每个手的手势信息。合法值为：1返回注：本参数默认值为 1, 0不返回 |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| hands         | Array  | 被检测出的手部数组注：如果没有检测出手则为空数组             |
| image_id      | String | 被检测的图片在系统中的标识                                   |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### hands 数组中单个元素的结构

| 字段           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| hand_rectangle | Object | 手部矩形框，坐标数字为整数，代表像素点坐标top：左上角纵坐标left：左上角横坐标width：宽度height：高度 |
| gesture        | Object | 手势识别结果，包括以下字段。每个字段的值是一个浮点数，范围 [0,100]，小数点后3位有效数字，总和等于100。unknown：未定义手势heart_a：比心 Aheart_b：比心 Bheart_c：比心 Cheart_d：比心 Dok：OKhand_open：手张开thumb_up：大拇指向上thumb_down：大拇指向下rock：ROCKnamaste：合十palm_up：手心向上fist：握拳index_finger_up：食指朝上double_finger_up：双指朝上victory：胜利big_v：大 V 字phonecall：打电话beg：作揖thanks：感谢注：关于手势的具体说明与图示，详见[手势识别](https://console.faceplusplus.com.cn/documents/10065685) |

## 返回值示例

### 请求成功返回示例：

```json
{
	"image_id": "7OO7N1dYiJjszvV38oKVpw==",
	"request_id": "1491569448,de5a441f-6c6f-4955-896c-37b8bb2d4197",
	"time_used": 915,
	"hands": [{
		"gesture": {
			"unknown": 0.000
			"heart_a": 99.000
			"heart_b": 1.000
			"heart_c": 0.000
			"heart_d": 0.000
			"ok": 0.000
			"hand_open": 0.000
			"thumb_up": 0.000
			"thumb_down": 0.000
			"rock": 0.000
			"namaste": 0.000
			"palm_up": 0.000
			"fist": 0.000
			"index_finger_up": 0.000
			"double_finger_up": 0.000
			"victory": 0.000
			"big_v": 0.000
			"phonecall": 0.000
			"beg": 0.000
			"thanks": 0.000
		},
		"hand_rectangle": {
			"width": 456,
			"top": 0,
			"height": 500,
			"left": 0
		},
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
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损、或图片文件格式不符合要求。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param>对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |
| 400          | IMAGE_FILE_TOO_LARGE:<param>           | 客户通过参数<param>上传的图片文件太大。本 API 要求图片文件大小不超过 2 MB |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```shell
curl -X POST "https://api-cn.faceplusplus.com/humanbodypp/v1/gesture" -F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg" \
-F "return_gesture=1"
```