文档中心/OCR ID Card API

## 版本

1.0

## 描述

检测和识别中华人民共和国第二代身份证的关键字段内容，并支持返回身份证正反面信息、身份证照片分类判断结果。

### 图片要求 ：

图片格式：JPG(JPEG)，PNG
图片像素尺寸：最小48*48像素，最大4096*4096像素
图片文件大小：2MB

### 更新日志

2017年6月7日：OCR ID Card 算法重大升级；只有正式 API Key 才能使用 legality 参数返回分类结果。

2017年3月28日：支持base64编码的图片。

## 调用URL

https://api-cn.faceplusplus.com/cardpp/v1/ocridcard

## 调用方法

POST 

## 权限

所有 API Key 均可调用本 API。请注意，只有正式 API Key 才能使用 legality 参数返回分类结果，免费 API Key 使用此参数不返回分类结果。

## 请求参数

 

| 是否必选       | 参数名     | 类型                                                         | 参数说明                                                     |
| -------------- | ---------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 必选           | api_key    | String                                                       | 调用此API的API Key                                           |
| 必选           | api_secret | String                                                       | 调用此API的API Secret                                        |
| 必选（三选一） | image_url  | String                                                       | 图片的URL注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用 image_file 或image_base64 参数直接上传图片。 |
| image_file     | File       | 一个图片，二进制文件，需要用 post multipart/form-data 的方式上传。 |                                                              |
| image_base64   | String     | base64编码的二进制图片数据如果同时传入了image_url、image_file和image_base64参数，本API使用顺序为image_file优先，image_url最低。 |                                                              |
| 可选           | legality   | Int                                                          | 是否返回身份证照片合法性检查结果。值可以取：1: 返回0: 不返回注：默认值为 0注意：2017年6月7日之后，只有正式 API Key 能够调用此参数返回分类结果，免费 API Key 调用后无法返回分类结果。 |

## 返回值说明

 

| 字段          | 类型   | 说明                                                         |
| ------------- | ------ | ------------------------------------------------------------ |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| cards         | Array  | 检测出证件的数组注：如果没有检测出证件则为空数组             |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### cards数组中单个元素的结构

 

| 字段           | 类型   | 说明                                                         |
| -------------- | ------ | ------------------------------------------------------------ |
| type           | Int    | 证件类型。返回1，代表是身份证。                              |
| address        | string | 住址                                                         |
| birthday       | String | 生日，格式为YYYY-MM-DD                                       |
| gender         | string | 性别（男/女）                                                |
| id_card_number | string | 身份证号                                                     |
| name           | string | 姓名                                                         |
| race           | string | 民族（汉字）                                                 |
| side           | string | 表示身份证的国徽面或人像面。返回值为：front: 人像面back: 国徽面 |
| issued_by      | string | 签发机关                                                     |
| valid_date     | string | 有效日期，返回值有两种格式：一个16位长度的字符串：YYYY.MM.DD-YYYY.MM.DD或是：YYYY.MM.DD-长期 |
| legality       | Object | 身份证照片的合法性检查结果。返回结果为身份证五种分类的概率值（取［0，1］区间实数，小数点后3位有效数字，概率值总和等于1.0）。五种分类为：ID Photo （正式身份证照片）Temporary ID Photo  （临时身份证照片）Photocopy （正式身份证的复印件）Screen （手机或电脑屏幕翻拍的照片）Edited （用工具合成或者编辑过的身份证图片）注意：此项功能只有正式 API Key 能够调用。如果使用免费 API Key 请求此参数，则返回的五个概率的数值都为0。 |

注：当某个字段内容无法确定时，该字段的结果会返回空；对各个字段的值并未做逻辑校验（如：时间不应有2月30日，身份证长度不应为15位等）。

## 返回值示例

### 正面示例

```
{
	"cards": [{
		"gender": "女",
		"name": "牛XX",
		"id_card_number": "XXXXXX19841013XXXX",
		"birthday": "1984-10-13",
		"race": "汉",
		"address": "广东省深圳市XXXXXXXX",
		"legality": {
			"Edited": 0.001,
			"Photocopy": 0.0,
			"ID Photo": 0.502,
			"Screen": 0.496,
			"Temporary ID Photo": 0.0
		},
		"type": 1,
		"side": "front"
	}],
	"time_used": 2151,
	"request_id": "1473759244,40dfde25-6d1a-4c90-a994-813556c81e30"
}
```

### 背面示例

```
{
	"cards": [{
		"issued_by": "北京市公安局海淀分局",
		"side": "back",
		"valid_date": "2010.11.13-2020.11.13"
	}],
	"time_used": 2151,
	"request_id": "1473759244,40dfde25-6d1a-4c90-a994-813556c81e30"
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
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本 API 描述。<param>对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |
| 400          | IMAGE_FILE_TOO_LARGE                   | 客户上传的图像文件太大。本 API 要求图片文件大小不超过 2 MB   |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| ------------- | ---------------------------- | ------------------------------------------------------------ |
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
curl -X POST "https://api-cn.faceplusplus.com/cardpp/v1/ocridcard" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg"
```

 

在线支持