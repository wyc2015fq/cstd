# Merge Face API (V1)

## 版本

V1

## 描述

使用本 API，可以对模板图和融合图中的人脸进行融合操作。融合后的图片中将包含融合图中的人脸特征，以及模板图中的其他外貌特征与内容。

返回值是一段 JSON，包含融合完成后图片的 Base64 编码。

### 图片要求

图片格式：JPG(JPEG)

图片像素尺寸：最小200*200像素，最大4096*4096像素 

图片文件大小：最大 2 MB 

最小人脸像素尺寸： 为了保证融合效果，推荐图片中人脸框（正方形）边长的最小值不低于200像素。

人脸质量：人脸质量越高，则融合效果越好。影响人脸质量的因素包括：对人脸五官的遮挡、不当的光照（强光、暗光、逆光）、过大的人脸角度（推荐 yaw ≤ ±10°, pitch ≤ ±10°）等。

目前不支持黑白照片。

## 调用URL

<https://api-cn.faceplusplus.com/imagepp/v1/mergeface>

## 调用方法

POST

## 权限

所有 API Key 均可调用本 API。

## 请求参数

| 是否必选        | 参数名             | 类型                                                         | 说明                                                         |                    |
| :-------------- | :----------------- | :----------------------------------------------------------- | :----------------------------------------------------------- | ------------------ |
| 必选            | api_key            | String                                                       | 调用此 API 的 API Key                                        |                    |
| 必选            | api_secret         | String                                                       | 调用此 API 的 API Secret                                     |                    |
| 必选(三选一)    | template_url       | String                                                       | 用于人脸融合的模板图                                         | 模板图图片的 URL。 |
| template_file   | File               | 模板图图片的二进制文件，需要用 post multipart/form-data 的方式上传。 |                                                              |                    |
| template_base64 | String             | 模板图图片的 Base64 编码二进制文件。                         |                                                              |                    |
| 必选            | template_rectangle | String                                                       | 指定模板图中进行人脸融合的人脸框位置。参数规格四个正整数，用逗号分隔，依次代表人脸框左上角纵坐标（top），左上角横坐标（left），人脸框宽度（width），人脸框高度（height）。例如：70,80,100,100人脸框说明人脸框的四边是平行于图片边框的，不需要角度信息。建议通过 Detect API 获取图片中的人脸框信息。 |                    |
| 必选(三选一)    | merge_url          | String                                                       | 用于人脸融合的融合图如果未传入 merge_rectangle 参数，则会取此图片中人脸框面积最大的人脸用以融合。请注意，此时图片中必须有人脸。如果传入了 merge_rectangle 参数，则会取 merge_rectangle 所标识的区域中的人脸。 | 融合图的图片URL。  |
| merge_file      | File               | 融合图的二进制文件，需要用 post multipart/form-data 的方式上传。 |                                                              |                    |
| merge_base64    | String             | 融合图的 Base64 编码二进制文件。                             |                                                              |                    |
| 可选            | merge_rectangle    | String                                                       | 指定融合图中用以融合的人脸框位置。参数规格四个正整数，用逗号分隔，依次代表人脸框左上角纵坐标（top），左上角横坐标（left），人脸框宽度（width），人脸框高度（height）。例如：70,80,100,100人脸框说明人脸框的四边是平行于图片边框的，不需要角度信息。建议通过 Detect API 获取图片中的人脸框信息。 |                    |
| 可选            | merge_rate         | Int                                                          | 融合比例，范围 [0,100]。数字越大融合结果包含越多融合图 (merge_url, merge_file, merge_base64 代表图片) 特征。默认值为50 |                    |

### 传图说明

本 API 中传入的所有图片，都支持使用 image_url, image_file,  image_base64 三种方式上传。如果对于同一个人脸图片，同时传入了三种参数，则使用顺序统一为 image_file 最优先，image_url 最低。

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串                             |
| result        | String | 融合后的图片，jpg 格式。base64 编码的二进制图片数据。图片尺寸大小与模板图一致。 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒                             |
| error_message | String | 当发生错误时才返回。                                         |

## 返回值示例

### 请求成功返回示例

```
{
    "time_used": 544,
    "result":……省略 base64 图片数据,
    "request_id": "1510906671,53ca1f2b-c3d8-473a-858c-3b4fd99ef07a"
}
```



### 请求失败返回值示例

```
{
    "time_used": 78,
    "error_message": "AUTHORIZATION_ERROR: Denied by Admin",
    "request_id": "1510933927,13809a5b-c962-4011-abed-667e1fc1b658"
}
```



## 当前 API 特有的 ERROR_MESSAGE

| HTTP状态代码 | 错误信息                                | 说明                                                         |
| :----------- | :-------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT: <param> | 参数 <param> 对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE: <param>             | 参数 <param> 对应的客户上传的图像像素尺寸太大或太小，图片要求请参照本 API 描述。<param> 对应图像太大的那个参数的名称。 |
| 400          | INVALID_IMAGE_URL: <param>              | 无法从参数 <param> 对应的 image_url 下载图片，图片 URL 错误或者无效。 |
| 400          | IMAGE_FILE_TOO_LARGE: <param>           | 参数<param>对应的客户上传的图像文件太大。本 API 要求图片文件大小不超过 2 MB |
| 400          | NO_FACE_FOUND:<param>                   | 参数<param>对应的图片未检测到人脸。                          |
| 400          | BAD_FACE                                | 上传的图片人脸不符合要求。已知情况：只有半张脸               |
| 400          | INVALID_RECTANGLE:<param>               | 传入的人脸框格式不符合要求，或者人脸框位于图片外             |
| 400          | MISSING_ARGUMENTS:<param>               | 缺少必选参数                                                 |
| 412          | IMAGE_DOWNLOAD_TIMEOUT: <param>         | 下载图片超时。                                               |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                      | 说明                                                         |
| :------------ | :---------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR          | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR: <reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED    | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>      | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS: <key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS         | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large      | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                 | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR                | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/imagepp/v1/mergeface" \
-F "api_key=<api_key>"  \
-F "api_secret=<api_secret>"  \
-F "template_file=@template_file.jpg" \
-F "template_rectangle=70,80,100,100"
-F "merge_file=@merge_file.jpg" \
-F "merge_rate=70"
```