# OCR Bank Card API(Beta)

## 版本

Beta

## 描述

检测和识别银行卡，并返回银行卡卡片边框坐标及识别出的银行卡号。当前 Beta 版本一次只支持识别一张银行卡，图像内有多张卡时，返回识别结果置信度最高的银行卡。支持任意角度的识别

### 图片要求 ：

图片格式：JPG(JPEG)，PNG

图片文件大小：2 MB

卡片像素尺寸：最小 100*100 像素，最大 4096*4096 像素，短边不得低于 100 像素。

## 调用URL

https://api-cn.faceplusplus.com/cardpp/beta/ocrbankcard

## 调用方法

POST

## 权限

所有 API Key 均可调用本 API。

## 请求参数

 

| 是否必选       | 参数名     | 类型                                                         | 参数说明                                                     |
| :------------- | :--------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key    | String                                                       | 调用此 API 的 API Key                                        |
| 必选           | api_secret | String                                                       | 调用此 API 的 API Secret                                     |
| 必选（三选一） | image_url  | String                                                       | 图片的 URL注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用 image_file 或 image_base64 参数直接上传图片。 |
| image_file     | File       | 一个图片，二进制文件，需要用 post multipart/form-data 的方式上传。 |                                                              |
| image_base64   | String     | base64编码的二进制图片数据注：如果同时传入了image_url、image_file 和 image_base64 参数，本 API 使用顺序为 image_file优先，image_url 最低。 |                                                              |

## 返回值说明

 

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| bank_cards    | Array  | 检测出证件的数组注：如果没有检测出证件则为空数组             |
| image_id      | String | 被检测的图片在系统中的标识                                   |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### bank_cards 数组中单个元素的结构

| 字段   | 类型   | 说明                                                         |
| :----- | :----- | :----------------------------------------------------------- |
| bound  | Object | 银行卡卡片四个角的像素点坐标，包含以下属性：right_top：银行卡右上角的像素点坐标left_top：银行卡左上角的像素点坐标right_bottom：银行卡右下角的像素点坐标left_bottom：银行卡左下角的像素点坐标每个属性都包含以下字段。每个字段的值都是整数。x：像素点横坐标位置y：像素点纵坐标位置 |
| number | String | 银行卡号。返回值为纯数字，无空格。注：返回值的位数不做合法性校验，只会返回识别到的数字。 |

## 返回值示例

### 请求成功返回示例

 

```
{
    "bank_cards": [{
        "bound": {
            "right_top": {
                "x": 770,
                "y": 20
            },
            "left_top": {
                "x": 17,
                "y": 20
            },
            "right_bottom": {
                "x": 770,
                "y": 420
            },
            "left_bottom": {
                "x": 17,
                "y": 420
            }
        },
        "number": "6225768761224816"
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

## 当前 API 特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本 API 描述。<param>对应图像太大的那个参数的名称 |
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

 

```
curl -X POST "https://api-cn.faceplusplus.com/cardpp/beta/ocrbankcard" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg"
```

 

 