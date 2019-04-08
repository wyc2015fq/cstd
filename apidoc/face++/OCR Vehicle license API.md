# OCR Vehicle license API

## 版本

1.0

## 描述

检测和识别中华人民共和国机动车行驶证（以下称“行驶证”）图像为结构化的文字信息。目前只支持行驶证主页正面，不支持副页正面反面。

-  行驶证图像须为正拍（垂直角度拍摄），但是允许有一定程度的旋转角度；
- 仅支持图像里有一个行驶证的主页正面，如果同时出现多页、或正副页同时出现，可能会返回空结果。

### 图片要求：

图片格式：JPG(JPEG)，PNG
图片像素尺寸：最小48*48像素，最大4096*4096像素
图片文件大小：2MB

### 更新日志

2017年3月28日：支持base64编码的图片。

## 调用URL

https://api-cn.faceplusplus.com/cardpp/v1/ocrvehiclelicense

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

 

| 是否必选       | 参数名     | 类型                                                         | 参数说明                                                     |
| :------------- | :--------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key    | String                                                       | 调用此API的API Key                                           |
| 必选           | api_secret | String                                                       | 调用此API的API Secret                                        |
| 必选（三选一） | image_url  | String                                                       | 图片的URL注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用image_file参数直接上传图片。 |
| image_file     | File       | 一个图片，二进制文件，需要用post multipart/form-data的方式上传。 |                                                              |
| image_base64   | String     | base64编码的二进制图片数据如果同时传入了image_url、image_file和image_base64参数，本API使用顺序为image_file优先，image_url最低。 |                                                              |

 

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| cards         | Array  | 检测出证件的数组注：如果没有检测出证件则为空数组             |
| image_id      | String | 被检测的图片在系统中的标识                                   |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### cards数组中单个元素的结构

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| type          | Int    | 证件版本。返回 3，表示是行驶证。                             |
| plate_no      | String | 号牌号码。                                                   |
| vehicle_type  | String | 车辆类型。                                                   |
| owner         | String | 所有人。                                                     |
| address       | String | 住址。                                                       |
| use_character | String | 使用性质。                                                   |
| model         | String | 品牌型号。                                                   |
| vin           | String | 车辆识别代号。                                               |
| engine_no     | String | 发动机号码。                                                 |
| register_date | String | 注册日期，格式为YYYY-MM-DD                                   |
| issue_date    | String | 发证日期，格式为YYYY-MM-DD                                   |
| side          | String | 表示行驶证的正面或者反面。该字段目前只会返回“front”，表示是正面。 |
| issued_by     | String | 签发机关。                                                   |

注：不能确定字段内容时，结果会是空；

## 返回值示例

### 正确请求返回示例

```
{
	"cards": [{
		"issue_date": "2010-04-02",
		"vehicle_type": "小型轿车",
		"issued_by": "江商省上饶市公安局交通警察支队",
		"vin": "LFVXXXXXXXXX536",
		"plate_no": "皖EHXXXX",
		"side": "front",
		"use_character": "非营运",
		"address": "江西省上饶县XXXXXXXXXXX",
		"owner": "邹XX",
		"model": "路虎AUD",
		"register_date": "2010-04-02",
		"type": 3,
		"engine_no": "ANXXXXX43"
	}],
	"time_used": 3474,
	"request_id": "1473763408,e2c2a352-a6bd-4620-9f74-c96841aed500"
}
```

### 请求失败返回示例 

```
{
	"error_message": "INVALID_IMAGE_SIZE: image",
	"request_id": "71eeb124-08f0-47b3-8fc8-ac048cfa134d",
	"time_used": 4
} 
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param>对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |
| 400          | IMAGE_FILE_TOO_LARGE                   | 客户上传的图像文件太大。本 API 要求图片文件大小不超过 2 MB   |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

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

```shell
curl "https://api-cn.faceplusplus.com/cardpp/v1/ocrvehiclelicense" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image=@image_vehicle_license.jpg" 
```

 

 

 

