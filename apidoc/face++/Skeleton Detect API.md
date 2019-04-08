# Skeleton Detect API

## 版本

V1

## 描述

传入图片进行人体检测和骨骼关键点检测，返回人体14个关键点

支持对图片中的所有人体进行骨骼检测

## 图片要求

图片格式：JPG(JPEG)

图片像素尺寸：最小 100*100 像素，最大 4096*4096 像素

图片文件大小：2 MB

为了保证较好的识别结果，人体矩形框大小建议在200*200像素及以上 

## 调用RUL

<https://api-cn.faceplusplus.com/humanbodypp/v1/skeleton>

## 调用方法

post

## 权限

所有API key均能调用此API

## 请求参数

| 是否必选       | 参数名     | 类型                                                         | 参数说明                                                     |
| :------------- | :--------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key    | String                                                       | 调用此 API 的 API Key                                        |
| 必选           | api_secret | String                                                       | 调用此 API 的 API Secret                                     |
| 必选（三选一） | image_url  | String                                                       | 图片的 URL注：在下载图片时可能由于网络等原因导致下载图片时间过长，建议使用 image_file 或 image_base64 参数直接上传图片 |
| image_file     | File       | 一个图片，二进制文件，需要用 post multipart/form-data 的方式上传 |                                                              |
| image_base64   | String     | base64 编码的二进制图片数据如果同时传入了 image_url、image_file 和 image_base64 参数，本API使用顺序为 image_file 优先，image_url 最低 |                                                              |

## 返回参数

| 返回字段说明  | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串                             |
| skeletons     | Object | 被检测出的人体数组，具体包含内容见下文。 注：如果没有检测出人体则为空 |
| image_id      | String | 被检测的图片在系统中的标识                                   |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒                             |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在 |

## skeletons中单个元素的结构

| 字段           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| body_rectangle | Object | 人体矩形框的位置，包括以下属性。每个属性的值都是整数：top：矩形框左上角像素点的纵坐标left：矩形框左上角像素点的横坐标width：矩形框的宽度height：矩形框的高度 |
| landmark       | Object | 包含14个骨骼关键点的对象类型，具体包含信息见下表及说明图     |



## landmark对象结构

下文字段均代表一个骨骼关键点，包含字段 x: 横坐标位置; y: 纵坐标位置，单位均为像素

| 字段            | 类型   | 说明 | 图示                                                         |
| :-------------- | :----- | :--- | :----------------------------------------------------------- |
| head            | Object | 头部 | ![img](https://cdn.faceplusplus.com.cn/facepp/images/cn/wiki/skeleton.png) |
| neck            | Object | 脖子 |                                                              |
| left_shoulder   | Object | 左肩 |                                                              |
| left_elbow      | Object | 左肘 |                                                              |
| left_hand       | Object | 左手 |                                                              |
| right_shoulder  | Object | 右肩 |                                                              |
| right_elbow     | Object | 右肘 |                                                              |
| right_hand      | Object | 右手 |                                                              |
| `left_buttocks` | Object | 左臀 |                                                              |
| left_knee       | Object | 左膝 |                                                              |
| left_foot       | Object | 左脚 |                                                              |
| right_buttocks  | Object | 右臀 |                                                              |
| right_knee      | Object | 右膝 |                                                              |
| right_foot      | Object | 右脚 |                                                              |





## 返回值示例

### 请求成功返回值示例

```json
{
    "time_used": 2299,
    "image_id": "lHjvzhnGndA/zZja3ho2fA==",
    "skeletons": [
        {
            "body_rectangle": {
                "width": 781,
                "top": 315,
                "height": 910,
                "left": 134
            },
            "landmark": {
                "left_buttocks": {
                    "y": 901,
                    "x": 452,
                    "score": 0.7299447
                },
                "head": {
                    "y": 9,
                    "x": 305,
                    "score": 0.5368539
                },
                "neck": {
                    "y": 673,
                    "x": 378,
                    "score": 0.6446256
                },
                "left_shoulder": {
                    "y": 692,
                    "x": 769,
                    "score": 0.7006054
                },
                "left_hand": {
                    "y": 597,
                    "x": 769,
                    "score": 0.41823307
                },
                "left_knee": {
                    "y": 901,
                    "x": 329,
                    "score": 0.6185796
                },
                "right_elbow": {
                    "y": 901,
                    "x": 12,
                    "score": 0.4332367
                },
                "right_shoulder": {
                    "y": 635,
                    "x": 12,
                    "score": 0.724305
                },
                "right_hand": {
                    "y": 901,
                    "x": 232,
                    "score": 0.56872773
                },
                "left_foot": {
                    "y": 901,
                    "x": 452,
                    "score": 0.3753785
                },
                "left_elbow": {
                    "y": 901,
                    "x": 769,
                    "score": 0.51084995
                },
                "right_buttocks": {
                    "y": 901,
                    "x": 134,
                    "score": 0.5565492
                },
                "right_knee": {
                    "y": 901,
                    "x": 85,
                    "score": 0.47690523
                },
                "right_foot": {
                    "y": 901,
                    "x": 452,
                    "score": 0.52765757
                }
            }
        },
        {
            "body_rectangle": {
                "width": 559,
                "top": 533,
                "height": 740,
                "left": 394
            },
            "landmark": {
                "left_buttocks": {
                    "y": 732,
                    "x": 114,
                    "score": 0.8086717
                },
                "head": {
                    "y": 270,
                    "x": 464,
                    "score": 0.4876535
                },
                "neck": {
                    "y": 270,
                    "x": 79,
                    "score": 0.5550944
                },
                "left_shoulder": {
                    "y": 378,
                    "x": 359,
                    "score": 0.64766926
                },
                "left_hand": {
                    "y": 362,
                    "x": 481,
                    "score": 0.6320832
                },
                "left_knee": {
                    "y": 563,
                    "x": 271,
                    "score": 0.49012265
                },
                "right_elbow": {
                    "y": 378,
                    "x": 551,
                    "score": 0.6178683
                },
                "right_shoulder": {
                    "y": 347,
                    "x": 324,
                    "score": 0.5143001
                },
                "right_hand": {
                    "y": 378,
                    "x": 551,
                    "score": 0.6799432
                },
                "left_foot": {
                    "y": 732,
                    "x": 9,
                    "score": 0.4409459
                },
                "left_elbow": {
                    "y": 362,
                    "x": 499,
                    "score": 0.56940734
                },
                "right_buttocks": {
                    "y": 732,
                    "x": 201,
                    "score": 0.6996023
                },
                "right_knee": {
                    "y": 424,
                    "x": 551,
                    "score": 0.48145446
                },
                "right_foot": {
                    "y": 732,
                    "x": 9,
                    "score": 0.43120167
                }
            }
        }
    ],
    "request_id": "1533886107,224911b6-e136-42bd-aebb-e1aec327e018"
}
```

### 请求错误返回值示例

```json
{
	"time_used": 3,
	"error_message": "MISSING_ARGUMENTS: image_url, image_file, image_base64",
	"request_id": "1470378968,c6f50ec6-49bd-4838-9923-11db04c40f8d"
}
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数 <param> 对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param> 对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的 image_url 下载图片，图片 URL 错误或者无效       |
| 400          | IMAGE_FILE_TOO_LARGE:<param>           | 客户通过参数 <param>上传的图片文件太大。本 API 要求图片文件大小不超过 2 MB |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| 401           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key 没有调用本 API 的权限，具体原因为：用户自己禁止该 api_key 调用、管理员禁止该 api_key 调用、由于账户余额不足禁止调用。目前的 <reason> 有：Denied by Client（用户自己禁止该 api_key 调用）Denied by Admin（管理员禁止该 api_key 调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该 API Key 的 QPS 已经达到上限。如需要提高 API Key 的 QPS 配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了 2MB 限制。该错误的返回格式为纯文本，不是 json 格式。 |
| 404           | API_NOT_FOUND                | 所调用的 API 不存在。                                        |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/humanbodypp/v1/skeleton" -F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg"
```