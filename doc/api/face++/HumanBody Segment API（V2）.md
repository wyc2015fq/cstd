# HumanBody Segment API（V2）

## 版本

2.0

V2 升级内容：增加抠出人像的图片返回。

### 更新日志

2018 年 10 月 25 日：可支持最大图片提升为4096*4096像素。

2019 年 01 月 03 日：可以选择是否返回灰度图。

## 描述

识别传入图片中人体的完整轮廓，进行人形抠像。

当图像中有多个人时，暂不支持从重叠部分区分出单个人的轮廓。

### 图片要求

图片格式：JPG(JPEG)，PNG

图片像素尺寸：最小 48*48 像素，最大4096*4096 像素

图片文件大小：2 MB

## 调用URL

<https://api-cn.faceplusplus.com/humanbodypp/v2/segment>

## 调用方法

POST

## 权限

所有 API Key 均可调用本 API。

## 请求参数

| 是否必选       | 参数名           | 类型                                                         | 参数说明                                                     |
| :------------- | :--------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key          | String                                                       | 调用此 API 的 API Key                                        |
| 必选           | api_secret       | String                                                       | 调用此 API 的 API Secret                                     |
| 必选（三选一） | image_url        | String                                                       | 图片的 URL注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用image_file参数直接上传图片 |
| image_file     | File             | 一个图片，二进制文件，需要用 post multipart / form-data 的方式上传 |                                                              |
| image_base64   | String           | base64 编码的二进制图片数据如果同时传入了 image_url、image_file 和 image_base64 参数，本 API 使用顺序为 image_file 优先，image_url 最低 |                                                              |
| 可选           | return_grayscale | int                                                          | 是否返回抠像后的灰度图，默认值为1。0：不返回灰度图1：返回灰度图注：如果只需要抠出的人像图，建议设置为0（不返回灰度图）。可以节省API调用时间，更快速的得到结果 |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| result        | String | 一个通过 Base64 编码的灰度图文件，图片中每个像素点的灰度值 V = confidence * 255，confidence（置信度）为原图对应像素点位于人体轮廓内的置信度，取值范围[0, 1]。例如：原图是一个 400*300 的图片，那么返回一个400*300的灰度图，如果一个像素点位于人体轮廓内的置信度为0.91，这个像素的灰度值为置信度232（0.91*255）. |
| image_id      | String | 被检测的图片在系统中的标识。                                 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |
| body_image    | String | 一个通过base64 编码的图片文件。解码后文件为抠出人像的图片，背景为透明色。图片大小与原图一致。阈值：置信度大于0.5的像素展示出来。（V2添加参数） |

##  返回值示例

### 请求成功返回示例:

待update

```json
` ``{` `    ``"image_id"``: ``"7OO7N1dYiJjszvV38oKVpw=="``,` `    ``"result"``: ``"/9j/2wCEAAYEBQYFBAYGBQYHBwYIChAKCgkJChQODwwQFxQYGBcUFhYaHSUfGhsjHBYWICwgIyYn.............s3/AOWq/nUjeOLNesq/ufWo3YmhDVhScV/9k="``,``"body_image"``: ``"/9j/2wCEAAYEBQYFBAYGBQYHBwYIChAKCgkJChQODwwQFxQYGBcUFhYaHSUfGhsjHBYWICwgIyYn.............s3/AOWq/nUjeOLNesq/ufWo3YmhDVhScV/9k="``,``  ``"request_id"``: ``"1470378968,c6f50ec6-49bd-4838-9923-11db04c40f8d"``,``    ``"time_used"``: 573"time_used": 573` `}`
```

### 请求失败返回示例:

```json
`{` `    ``"time_used"``: 3,` `    ``"error_message"``: ``"MISSING_ARGUMENTS: image_url, image_file, image_base64"``,` `    ``"request_id"``: ``"1470378968,c6f50ec6-49bd-4838-9923-11db04c40f8d"` `}`
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param>对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |
| 400          | IMAGE_FILE_TOO_LARGE:<param>           | 客户通过参数 <param>上传的图片文件太大。本 API 要求图片文件大小不超过 2 MB。 |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| HTTP 状态代码 | 错误信息                     | 说明                                                         |
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
`curl -X POST ``"https://api-cn.faceplusplus.com/humanbodypp/v2/segment"` `-F ``"api_key=<api_key>"` `\` `-F ``"api_secret=<api_secret>"` `\` `-F ``"image_file=@image_file.jpg"`
```