# License Plate API (V1)

## 版本

V1.0

## 描述

调用者传入一张图片文件或图片URL，检测并返回图片中车牌框并识别车牌颜色和车牌号。当传入图片中有多个车牌时，按照车牌框大小排序依次输出。

### 图片要求

图片格式：JPG(JPEG)，PNG
图片像素尺寸：最小 48*48 像素，最大 4096*4096 像素
图片文件大小：2 MB

### 更新日志

2018 年 11 月 20 日：支持新能源车牌的识别。


调用URL

[https://api-cn.faceplusplus.com/imagepp/v1/licenseplate](https://api-cn.faceplusplus.com/imagepp/beta/licenseplate)

## 调用方法

POST

## 权限

所有 API Key 均可调用本 API

## 请求参数

| 是否必选       | 参数名     | 类型                                                         | 参数说明                                                     |
| :------------- | :--------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key    | String                                                       | 调用此 API 的 API Key                                        |
| 必选           | api_secret | String                                                       | 调用此 API 的 API Secret                                     |
| 必选（三选一） | image_url  | String                                                       | 图片的 URL注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用 image_file 或 image_base64 参数直接上传图片 |
| image_file     | File       | 图片二进制文件，需要用 post multipart/form-data 的方式上传   |                                                              |
| image_base64   | String     | base64 编码的二进制图片数据如果同时传入了 image_url、image_file 和 image_base64 参数，本API使用顺序为 image_file 优先，image_url 最低 |                                                              |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串                             |
| results       | Array  | 检测出的车牌数组，具体包含内容见下文注：如果没有检出车牌，则返回结果为空数组 |
| image_id      | String | 被检测的图片在系统中的标识                                   |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒                             |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在 |

### results数组中单个元素的结构

| 字段                 | 类型   | 说明                                                         |
| :------------------- | :----- | :----------------------------------------------------------- |
| bound                | Object | 车牌四个角的像素点坐标，包含以下属性：right_top：车牌右上角的像素点坐标left_top：车牌左上角的像素点坐标right_bottom：车牌右下角的像素点坐标left_bottom：车牌左下角的像素点坐标每个属性都包含以下字段。每个字段的值都是整数。x：像素点横坐标位置y：像素点纵坐标位置 |
| color                | Int    | 识 别出的车牌底色，取值如下:0：蓝色1：黄色2：黑色3：白色4：绿色5：小型新能源6：大型新能源7：缺失8：无效 |
| license_plate_number | String | 识别出的车牌号结果。如"京A12345"、"粤B13623"                 |



## 返回值示例

```
{
        "image_id": "ZZX0dA3yA+75FnXZ3lpDug==",
        "request_id": "1524109138,f262f388-8562-4da1-9a9b-5de2cb05e50f",
        "time_used": 1048,
        "results": [{
                "color": 0,
                "license_plate_number": "黑AF6655",
                "bound": {
                        "left_bottom": {
                                "y": 654,
                                "x": 372
                        },
                        "right_top": {
                                "y": 416,
                                "x": 1334
                        },
                        "right_bottom": {
                                "y": 645,
                                "x": 1302
                        },
                        "left_top": {
                                "y": 424,
                                "x": 404
                        }
                }
        }]
}
```



## 当前 API 特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本 API 描述。<param>对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |
| 400          | IMAGE_FILE_TOO_LARGE:<param>           | 客户通过参数<param>上传的图片文件太大。本 API 要求图片文件大小不超过 2 MB |
| 400          | NO_LICENSE_PLATE_FOUND                 | 用户上传图片中未检测到车牌                                   |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

##  通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为： 用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错 （比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了 要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

 

```
`curl -X POST ``"https://api-cn.faceplusplus.com/imagepp/v1/licenseplate"` `` `-F ``"api_key=<api_key>"` `` `-F ``"api_secret=<api_secret>"` `` `-F ``"image_file=@image_file.jpg"`
```

 