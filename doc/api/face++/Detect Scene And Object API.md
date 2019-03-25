# Detect Scene And Object API

## 版本

Beta

## 描述

调用者提供图片文件或者图片URL，进行图片分析，识别图片场景和图片主体。

### 图片要求：

图片格式：JPG(JPEG)，PNG

图片像素尺寸：最小48*48像素，最大4096*4096像素

图片文件大小：2MB

### 更新日志

2017年3月28日：支持base64编码的图片。

## 调用URL

https://api-cn.faceplusplus.com/imagepp/beta/detectsceneandobject

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选       | 参数名     | 类型                                                         | 参数说明              |
| :------------- | :--------- | :----------------------------------------------------------- | :-------------------- |
| 必选           | api_key    | String                                                       | 调用此API的API Key    |
| 必选           | api_secret | String                                                       | 调用此API的API Secret |
| 必选（三选一） | image_url  | String                                                       | 图片的URL             |
| image_file     | File       | 一个图片，二进制文件，需要用post multipart/form-data的方式上传。 |                       |
| image_base64   | String     | base64编码的二进制图片数据如果同时传入了image_url、image_file和image_base64参数，本API使用顺序为image_file优先，image_url最低。 |                       |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| scenes        | Array  | 识别出的图片场景信息数组，每个场景包括value：场景名称confidence：对该场景的置信度，是一个浮点数，范围[0,100]，小数点后3位有效数字。注：如果没有检测出场景则为空 |
| objects       | Array  | 检测出的图片物体信息数组，每个物体包括value：物体名称confidence：对该物体的置信度，是一个浮点数，范围[0,100]，小数点后3位有效数字。注：如果没有检测出物体则为空 |
| image_id      | String | 被检测的图片在系统中的标识                                   |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

## 返回值示例

### 请求成功返回示例:

```
{
	"time_used": 1455,
	"scenes": [],
	"image_id": "+zFOHDK2c1tp948KxVxOsA==",
	"objects": [{
		"confidence": 20.888,
		"value": "Alcazar"
	}, {
		"confidence": 20.87,
		"value": "Temple"
	}],
	"request_id": "1473763608,d04a67e3-0bf8-4fdf-93d2-33100ea8654b"
}
```

### 请求失败返回示例:

```
{
	"time_used": 3,
	"error_message": "MISSING_ARGUMENTS: image_url, image_file, image_base64",
	"request_id": "1470378968,c6f50ec6-49bd-4838-9923-11db04c40f8d"
}
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param>对应图像太大的那个参数的名称 |
| 400          | IMAGE_FILE_TOO_LARGE                   | 客户上传的图像文件太大。本 API 要求图片文件大小不超过 2 MB   |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 获取URL所指向的图片超时                                      |
| 400          | INVALID_IMAGE_URL                      | 图片URL无效                                                  |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指此API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/imagepp/beta/detectsceneandobject" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg" 
```

 

 

 