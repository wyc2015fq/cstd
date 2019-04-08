# Search API

## 版本

3.0

## 描述

在一个已有的 FaceSet 中找出与目标人脸最相似的一张或多张人脸，返回置信度和不同误识率下的阈值。

支持传入图片或 face_token 进行人脸搜索。使用图片进行搜索时会选取图片中检测到人脸尺寸最大的一个人脸。

### 图片要求

图片格式：JPG(JPEG)，PNG
图片像素尺寸：最小 48*48 像素，最大 4096*4096 像素
图片文件大小：2MB
最小人脸像素尺寸： 系统能够检测到的人脸框为一个正方形，正方形边长的最小值为 150 像素。

### 更新日志

2017年3月28日：支持base64编码的图片。

## 调用URL

https://api-cn.faceplusplus.com/facepp/v3/search

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。其中 face_rectangle 参数只有正式 API Key 可以使用，试用 API Key 无法使用。

## 请求参数

| 是否必选                        | 参数名              | 类型                                                         | 参数说明                                                     |
| :------------------------------ | :------------------ | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选                            | api_key             | String                                                       | 调用此 API 的 API Key                                        |
| 必选                            | api_secret          | String                                                       | 调用此 API 的 API Secret                                     |
| 必选（四选一）                  | face_token          | String                                                       | 进行搜索的目标人脸的 face_token，优先使用该参数              |
| image_url                       | String              | 目标人脸所在的图片的 URL                                     |                                                              |
| image_file                      | File                | 目标人脸所在的图片，二进制文件，需要用 post multipart/form-data 的方式上传。 |                                                              |
| image_base64                    | String              | base64 编码的二进制图片数据如果同时传入了 image_url、image_file 和 image_base64 参数，本 API 使用顺序为 image_file 优先，image_url 最低。 |                                                              |
| 必选（二选一）                  | faceset_token       | String                                                       | 用来搜索的 FaceSet 的标识                                    |
| outer_id                        | String              | 用户自定义的 FaceSet 标识                                    |                                                              |
| 可选                            | return_result_count | Int                                                          | 控制返回比对置信度最高的结果的数量。合法值为一个范围 [1,5] 的整数。默认值为 1 |
| 可选（仅正式 API Key 可以使用） | face_rectangle      | String                                                       | 当传入图片进行人脸检测时，是否指定人脸框位置进行检测。如果此参数传入值为空，或不传入此参数，则不使用此功能。本 API 会自动检测图片内所有区域的所有人脸。如果使用正式 API Key 对此参数传入符合格式要求的值，则使用此功能。需要传入一个字符串代表人脸框位置，系统会根据此坐标对框内的图像进行人脸检测，以及人脸关键点和人脸属性等后续操作。系统返回的人脸矩形框位置会与传入的 face_rectangle 完全一致。对于此人脸框之外的区域，系统不会进行人脸检测，也不会返回任何其他的人脸信息。参数规格：四个正整数，用逗号分隔，依次代表人脸框左上角纵坐标（top），左上角横坐标（left），人脸框宽度（width），人脸框高度（height）。例如：70,80,100,100注：只有在传入 image_url、image_file 和 image_base64 三个参数中任意一个时，本参数才生效。 |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| results       | Array  | 搜索结果对象数组注：如果传入图片但图片中未检测到人脸，则无法进行人脸搜索，本字段不返回。 |
| thresholds    | Object | 一组用于参考的置信度阈值，包含以下三个字段。每个字段的值为一个 [0,100] 的浮点数，小数点后 3 位有效数字。1e-3：误识率为千分之一的置信度阈值；1e-4：误识率为万分之一的置信度阈值；1e-5：误识率为十万分之一的置信度阈值；如果置信值低于“千分之一”阈值则不建议认为是同一个人；如果置信值超过“十万分之一”阈值，则是同一个人的几率非常高。请注意：阈值不是静态的，每次返回的阈值不保证相同，所以没有持久化保存阈值的必要，更不要将当前调用返回的 confidence 与之前调用返回的阈值比较。注：如果传入图片但图片中未检测到人脸，则无法进行人脸搜索，本字段不返回。 |
| image_id      | String | 传入的图片在系统中的标识。注：如果未传入图片，本字段不返回。 |
| faces         | Array  | 传入的图片中检测出的人脸数组，采用数组中的第一个人脸进行人脸搜索。注：如果未传入图片，本字段不返回。如果没有检测出人脸则为空数组 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### results 数组中单个元素的结构

| 字段       | 类型   | 说明                                                         |
| :--------- | :----- | :----------------------------------------------------------- |
| face_token | String | 从 FaceSet 中搜索出的一个人脸标识 face_token。               |
| confidence | Float  | 比对结果置信度，范围 [0,100]，小数点后3位有效数字，数字越大表示两个人脸越可能是同一个人。 |
| user_id    | String | 用户提供的人脸标识，如果未提供则为空。                       |

### faces数组中单个元素的结构

| 字段           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| face_token     | String | 人脸的标识                                                   |
| face_rectangle | Object | 人脸矩形框的位置，包括以下属性。每个属性的值都是整数：top：矩形框左上角像素点的纵坐标left：矩形框左上角像素点的横坐标width：矩形框的宽度height：矩形框的高度 |

## 返回值示例

### 成功请求返回值示例

```
{
	"request_id": "1470481443,0d749845-7153-4f5e-a996-ffc5a1ac0a79",
	"time_used": 1126,
	"thresholds": {
		"1e-3": 65.3,
		"1e-5": 76.5,
		"1e-4": 71.8
	},
	"results": [{
		"confidence": 96.46,
		"user_id": "234723hgfd",
		"face_token": "4dc8ba0650405fa7a4a5b0b5cb937f0b"
	}]
}
```

 

### 失败请求返回值示例

```
{
	"time_used": 5,
	"error_message": "INVALID_OUTER_ID",
	"request_id": "1469761051,ec285c20-8660-47d3-8b91-5dc2bffa0049"
}
```

## 当前 API 特有的 ERROR_MESSAGE

| HTTP状态代码 | 错误信息                                | 说明                                                         |
| :----------- | :-------------------------------------- | :----------------------------------------------------------- |
| 400          | INVALID_FACE_TOKEN: <face_token>        | 使用 face_token 作为参数时，所传的 face_token 不存在。       |
| 400          | INVALID_OUTER_ID                        | 使用 outer_id 作为参数时，所传的 outer_id 不存在             |
| 400          | INVALID_FACESET_TOKEN                   | 使用 faceset_token 作为参数时，所传的 faceset_token 不存在   |
| 400          | EMPTY_FACESET                           | FaceSet 中没有 face_token                                    |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT: <param> | 参数 <param> 对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE: <param>             | 参数 <param> 对应的客户上传的图像像素尺寸太大或太小，图片要求请参照本 API 描述。<param>对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL: <param>              | 无法从参数 <param> 对应的 image_url 下载图片，图片 URL 错误或者无效 |
| 400          | IMAGE_FILE_TOO_LARGE:<param>            | 客户通过参数<param>上传的图片文件太大。本 API 要求图片文件大小不超过 2 MB |
| 403          | INSUFFICIENT_PERMISSION:<param>         | 试用 API Key 无法使用 <param>对应的参数。请勿传入此参数。或者使用正式 API Key 调用。 |
| 412          | IMAGE_DOWNLOAD_TIMEOUT: <param>         | 下载图片超时                                                 |

 

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
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
curl -X POST "https://api-cn.faceplusplus.com/facepp/v3/search" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "face_token=c2fc0ad7c8da3af5a34b9c70ff764da0" \
-F "outer_id=facesetid"
```

# 