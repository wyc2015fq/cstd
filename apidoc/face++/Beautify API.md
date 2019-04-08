# Beautify API

## 版本

V1

## 描述

对图片进行美颜和美白。

### 图片要求

图片格式：JPG(JPEG)，PNG
图片像素尺寸：最小48*48像素，最大4096*4096像素 
图片文件大小：<=2MB

## 调用URL

[https://api-cn.faceplusplus.com/facepp/v1/](https://api-cn.faceplusplus.com/imagepp/v1/)beautify

## 调用方法

POST

## 请求参数

| 是否必选       | 参数名     | 类型                                                         | 参数说明                                                     |
| :------------- | :--------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key    | String                                                       | 调用此API的API Key                                           |
| 必选           | api_secret | String                                                       | 调用此API的API Secret                                        |
| 必选（三选一） | image_url  | String                                                       | 图片的URL。注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用image_file参数直接上传图片。 |
| image_file     | File       | 一个图片，二进制文件，需要用post multipart/form-data的方式上传。 |                                                              |
| image_base64   | String     | base64编码的二进制图片数据如果同时传入了image_url、image_file和image_base64参数，本API使用顺序为image_file优先，image_url最低。 |                                                              |
| 可选           | whitening  | Int                                                          | 美白程度，取值范围[0,100]0不美白，100代表最高程度本参数默认值为 100 |
| 可选           | smoothing  | Int                                                          | 磨皮程度，取值范围 [0,100]0不磨皮，100代表最高程度本参数默认值为 100 |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| result        | String | 美化后的图片，jpg格式。base64 编码的二进制图片数据。图片尺寸大小与底图一致。 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当发生错误时才返回。                                         |

## 返回值实示例

### 请求成功返回示例

```
`{` `    ``"time_used"``: ``544``,` `    ``"result"``:……省略 base64 图片数据` `}`
```



### 请求失败返回值示例

```
`{` `    ``"time_used"``: ``78``,` `    ``"error_message"``: ``"AUTHORIZATION_ERROR: Denied by Admin"` `}`
```

##  当前 API 特有的 ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损、或图片文件格式不符合要求。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param>对应图像太大或太小的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |
| 400          | IMAGE_FILE_TOO_LARGE:<param>           | 客户通过参数<param>上传的图片文件太大。本 API 要求图片文件大小不超过 2 MB |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| 401           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了 2.5 MB 限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的 API 不存在。                                        |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

 

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/facepp/v1/beautify" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg" \
```