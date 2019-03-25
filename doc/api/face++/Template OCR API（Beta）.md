# Template OCR API（Beta）

## 版本

Beta

## 描述

用户通过控制台生成自定义模板并发布后，调用此API传入待识别图片及模板ID，返回模板定义的识别域内容

### **注意：**

- 只能识别已发布的模板
- 一次只能识别一张待识别图片

### 图片要求：

图片格式：支持JPEG，JPG，PNG，BMP

图片尺寸：图片最长边不超过4096像素，最短边不少于512像素

图片大小：图片大小不超过4M

## 调用URL

https://api-cn.faceplusplus.com/cardpp/beta/templateocr

## 调用方法

POST

## 权限

所有 API Key 均可调用本 API



## 请求参数 

| 是否必选       | 参数名      | 类型                                                         | 参数说明                                                     |
| :------------- | :---------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key     | String                                                       | 调用此API的API Key                                           |
| 必选           | api_secret  | String                                                       | 调用此 API 的 API Secret                                     |
| 必选（三选一） | image_url   | String                                                       | 图片的 URL注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用 image_file 或image_base64 参数直接上传图片。 |
| image_file     | File        | 一个图片，二进制文件，需要用 post multipart/form-data 的方式上传。 |                                                              |
| image_base64   | String      | base64编码的二进制图片数据注：如果同时传入了image_url、image_file和image_base64参数，本 API 使用顺序为image_file优先，image_url最低。 |                                                              |
| 必选           | template_id | String                                                       | 指定的模板ID（模版ID在创建模板后由系统自动生成）             |
| 可选           | extra_info  | String                                                       | 该参数的值是一个单字符串或者以英文逗号分割的字符串，表示需要返回的额外信息。当前只支持extra_info=position，表示返回识别域的位置信息（识别域四个点坐标） |

## 返回值说明

| 字段       | 类型   | 说明                                                         |
| :--------- | :----- | :----------------------------------------------------------- |
| request_id | String | 用于区分每一次请求的唯一的字符串。除非发生404（API_NOT_FOUND ) 或403 （AUTHORIZATION_ERROR）错误，此字段必定返回 |
| result     | Object | 被检测出的文字信息，由一个或多个data对象组成。注：如果没有检测出文字则为空 |
| time_used  | Int    | 整个请求所花费的时间，单位为毫秒。除非发生404（API_NOT_FOUND )或403 （AUTHORIZATION_ERROR）错误，此字段必定返回 |

### data对象的元素结构

| 字段  | 类型   | 说明                                                         |
| :---- | :----- | :----------------------------------------------------------- |
| key   | String | 创建模板时定义的识别域名                                     |
| value | Json   | 识别结果。包括一个text字段，说明如下：text：表示识别的结果，以列表格式返回，每个元素表示一行的识别结果。position：当请求的extra_info为position时，返回识别域四个点的坐标。以列表格式返回，每个元素表示一个点的坐标[x,y] |

##  返回值示例

### 请求成功返回示例

```json
{
    "request_id": "1540536486,13ce32c9-fbed-439b-ae73-4981b8c4311b",
    "result": [
        {
            "key": "item1",
            "value": {
                "text": ["test1"],
                "position": [ {"x":,1615,"y",547}, {"x",2657,"y":537}, {"x":2664,"y":839}, {"x":1614,"y":846} ]
            }
        },
        {
            "key": "item2",
            "value": {
                "text": ["test1","test2"],
				"position": [ {"x":,1600,"y",1036}, {"x",2680,"y":1030}, {"x":2685,"y":1226}, {"x":1599,"y":1229} ]
            }
        },
    ],
    "time_used": 5837
}
```



### 请求失败返回示例

 

```json
`{` `    ``"time_used"``: 3,` `    ``"error_message"``: ``"MISSING_ARGUMENTS: image_url, image_file, image_base64"``,` `    ``"request_id"``: ``"1470378968,c6f50ec6-49bd-4838-9923-11db04c40f8d"` `}`
```





## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损、或图片文件格式不符合要求 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param>对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |
| 400          | IMAGE_FILE_TOO_LARGE:<param>           | 客户通过参数<param>上传的图片文件太大。本 API 要求图片文件大小不超过4MB |
| 400          | INVALID_TEMPID                         | TemplateID无效或未找到对应的生效模板                         |
| 400          | MISMATCH_TEMPLATE                      | 不匹配指定模版，可能的原因包括：非模版的图片未检测到文字或者文字较少图片质量较差图片模糊光线较暗 |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key 没有调用本 API 的权限，具体原因为：用户自己禁止该 api_key 调用、管理员禁止该 api_key 调用、由于账户余额不足禁止调用。目前的 <reason> 有：Denied by Client（用户自己禁止该 api_key 调用）Denied by Admin（管理员禁止该 api_key 调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制注：这里的并发控制数超出限制，是指该 API Key 的 QPS 已经达到上限。如需要提高 API Key 的 QPS 配额请查看价格方案或者联系我们 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数                                             |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了限制（6M）。该错误的返回格式为纯文本，不是json格式 |
| 404           | API_NOT_FOUND                | 所调用的 API 不存在                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队 |

## 调用示例

 

```shell
curl -X POST "https://api-cn.faceplusplus.com/cardpp/beta/templateocr" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg"
-F "template_id"=1234567
```