# 数据集管理API参考

本文档主要说明数据集管理API如何使用，如有疑问可以通过以下方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**
- 进入[EasyDL社区交流](http://ai.baidu.com/forum/topic/list/190) ,与其他开发者进行互动
- 加入EasyDL官方QQ群（群号:679517246）联系群管

# 数据集创建API

## 接口描述

该接口可用于创建数据集。

## 接口鉴权

同模型上线后获取的API：

1、在[EasyDL——控制台](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/overview/index)创建应用

2、应用详情页获取API Key和Secret Key

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL：`https://aip.baidubce.com/rpc/2.0/easydl/dataset/create`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

**请求参数**

| 字段         | 必选 | 类型   | 说明                                                         |
| :----------- | :--- | :----- | :----------------------------------------------------------- |
| type         | 是   | string | 数据集类型，可包括： IMAGE_CLASSIFICATION, OBJECT_DETECTION, SOUND_CLASSIFICATION, TEXT_CLASSIFICATION 分别对应：图像分类、物体检测、声音分类、文本分类 |
| dataset_name | 是   | string | 数据集名称，长度不超过20个utf-8字符                          |

## 返回说明

**返回参数**

| 字段       | 必选 | 类型   | 说明                       |
| :--------- | :--- | :----- | :------------------------- |
| log_id     | 是   | number | 唯一的log id，用于问题定位 |
| dataset_id | 否   | number | 创建的数据集ID             |

# 数据集列表API

## 接口描述

该接口可用于查看数据集列表。返回数据集的名称、类型、状态等信息。

## 接口鉴权

同模型上线后获取的API：

1、在[EasyDL——控制台](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/overview/index)创建应用

2、应用详情页获取API Key和Secret Key

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL：`https://aip.baidubce.com/rpc/2.0/easydl/dataset/list`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

**请求参数**

| 字段  | 必选 | 类型   | 说明                                                         |
| :---- | :--- | :----- | :----------------------------------------------------------- |
| type  | 是   | string | 数据集类型，可包括： IMAGE_CLASSIFICATION, OBJECT_DETECTION, SOUND_CLASSIFICATION, TEXT_CLASSIFICATION 分别对应：图像分类、物体检测、声音分类、文本分类 |
| start | 否   | number | 起始序号，默认为0                                            |
| num   | 否   | number | 数量，默认20，最多100                                        |

## 返回说明

**返回参数**

| 字段          | 必选 | 类型          | 说明                       |
| :------------ | :--- | :------------ | :------------------------- |
| log_id        | 是   | number        | 唯一的log id，用于问题定位 |
| total_num     | 否   | number        | 数据集总数                 |
| results       | 否   | array(object) | 数据集列表                 |
| +dataset_id   | 否   | number        | 数据集ID                   |
| +dataset_name | 否   | string        | 数据集名称                 |
| +type         | 否   | string        | 数据集类型                 |
| +status       | 否   | string        | 数据集状态                 |

# 分类（标签）列表API

## 接口描述

该接口可用于查看分类（标签）。返回分类（标签）的名称、包含数据量等信息。

## 接口鉴权

同模型上线后获取的API：

1、在[EasyDL——控制台](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/overview/index)创建应用

2、应用详情页获取API Key和Secret Key

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL：`https://aip.baidubce.com/rpc/2.0/easydl/label/list`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

**请求参数**

| 字段       | 必选 | 类型   | 说明                                                         |
| :--------- | :--- | :----- | :----------------------------------------------------------- |
| type       | 是   | string | 数据集类型，可包括： IMAGE_CLASSIFICATION, OBJECT_DETECTION, SOUND_CLASSIFICATION, TEXT_CLASSIFICATION 分别对应：图像分类、物体检测、声音分类、文本分类 |
| dataset_id | 是   | number | 数据集ID                                                     |
| start      | 否   | number | 起始序号，默认0                                              |
| num        | 否   | number | 数量，默认20，最多100                                        |

## 返回说明

**返回参数**

| 字段          | 必选 | 类型          | 说明                       |
| :------------ | :--- | :------------ | :------------------------- |
| log_id        | 是   | number        | 唯一的log id，用于问题定位 |
| total_num     | 否   | number        | 标签总数                   |
| results       | 否   | array(object) | 标签列表                   |
| +label_id     | 否   | string        | 标签/分类ID                |
| +label_name   | 否   | string        | 标签/分类名称              |
| +entity_count | 否   | number        | 图片/声音/文本数量         |

# 添加数据API

## 接口描述

该接口可用于在指定数据集添加数据。

## 接口鉴权

同模型上线后获取的API：

1、在[EasyDL——控制台](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/overview/index)创建应用

2、应用详情页获取API Key和Secret Key

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL：`https://aip.baidubce.com/rpc/2.0/easydl/dataset/addentity`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

**请求参数**

| 字段           | 必选 | 类型          | 说明                                                         |
| :------------- | :--- | :------------ | :----------------------------------------------------------- |
| type           | 是   | string        | 数据集类型，可包括： IMAGE_CLASSIFICATION, OBJECT_DETECTION, SOUND_CLASSIFICATION, TEXT_CLASSIFICATION 分别对应：图像分类、物体检测、声音分类、文本分类 |
| dataset_id     | 是   | number        | 数据集ID                                                     |
| appendLabel    | 否   | boolean       | 确定添加标签/分类的行为：追加(true)、替换(false)。默认为追加(true)。 |
| entity_content | 是   | string        | type为IMAGE_CLASSIFY/OBJECT_DETECT/SOUND_CLASSIFY时，填入图片/声音的base64编码；type为TEXT_CLASSIFY时，填入utf-8编码的文本。**内容限制为：图像分类base64前10M；物体检测base64前10M；声音分类base64前4M，声音时长1~15秒；文本分类10000个汉字** |
| entity_name    | 是   | string        | 文件名                                                       |
| labels         | 是   | array(object) | 标签/分类数据                                                |
| +label_name    | 是   | string        | 标签/分类名称（由数字、字母、中划线、下划线组成），长度限制20B |
| +left          | 否   | number        | 物体检测时需给出，标注框左上角到图片左边界的距离(像素)       |
| +top           | 否   | number        | 物体检测时需给出，标注框左上角到图片上边界的距离(像素)       |
| +width         | 否   | number        | 物体检测时需给出，标注框的宽度(像素)                         |
| +height        | 否   | number        | 物体检测时需给出，标注框的高度(像素)                         |

## 返回说明

**返回参数**

| 字段   | 必选 | 类型   | 说明                       |
| :----- | :--- | :----- | :------------------------- |
| log_id | 是   | number | 唯一的log id，用于问题定位 |

# 数据集删除API

## 接口描述

该接口可用于删除数据集。

## 接口鉴权

同模型上线后获取的API：

1、在[EasyDL——控制台](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/overview/index)创建应用

2、应用详情页获取API Key和Secret Key

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL：`https://aip.baidubce.com/rpc/2.0/easydl/dataset/delete`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

**请求参数**

| 字段       | 必选 | 类型   | 说明                                                         |
| :--------- | :--- | :----- | :----------------------------------------------------------- |
| type       | 是   | string | 数据集类型，可包括： IMAGE_CLASSIFICATION, OBJECT_DETECTION, SOUND_CLASSIFICATION, TEXT_CLASSIFICATION 分别对应：图像分类、物体检测、声音分类、文本分类 |
| dataset_id | 是   | number | 数据集ID                                                     |

## 返回说明

**返回参数**

| 字段   | 必选 | 类型   | 说明                       |
| :----- | :--- | :----- | :------------------------- |
| log_id | 是   | number | 唯一的log id，用于问题定位 |

# 分类（标签）删除API

## 接口描述

该接口可用于删除分类（标签）。

## 接口鉴权

同模型上线后获取的API：

1、在[EasyDL——控制台](https://console.bce.baidu.com/ai/?_=1537962688426&ed&no_xss&locale=zh-cn#/ai/easydl/overview/index)创建应用

2、应用详情页获取API Key和Secret Key

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL：`https://aip.baidubce.com/rpc/2.0/easydl/label/delete`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

**请求参数**

| 字段       | 必选 | 类型   | 说明                                                         |
| :--------- | :--- | :----- | :----------------------------------------------------------- |
| type       | 是   | string | 数据集类型，可包括： IMAGE_CLASSIFICATION, OBJECT_DETECTION, SOUND_CLASSIFICATION, TEXT_CLASSIFICATION 分别对应：图像分类、物体检测、声音分类、文本分类 |
| dataset_id | 是   | number | 数据集ID                                                     |
| label_name | 是   | string | 标签/分类名称                                                |

## 返回说明

**返回参数**

| 字段   | 必选 | 类型   | 说明                       |
| :----- | :--- | :----- | :------------------------- |
| log_id | 是   | number | 唯一的log id，用于问题定位 |

# 错误码

若请求错误，服务器将返回的JSON文本包含以下参数：

- **error_code：**错误码。
- **error_msg：**错误描述信息，帮助理解和解决发生的错误。

例如Access Token失效返回：

```
{
  "error_code": 110,
  "error_msg": "Access token invalid or no longer valid"
}
```

需要重新获取新的Access Token再次请求即可。

| 错误码 | 错误信息                                | 描述                                                         |
| :----- | :-------------------------------------- | :----------------------------------------------------------- |
| 406000 | internal server error                   | 服务错误                                                     |
| 406001 | param[xx] invalid                       | 参数xx不合法，请检查相关参数                                 |
| 406002 | dataset not exist                       | 数据集不存在                                                 |
| 406003 | dataset already exists                  | 数据集已存在                                                 |
| 406004 | dataset can not be modified temporarily | 数据集暂不可修改                                             |
| 406005 | label not exist                         | 标签/分类不存在                                              |
| 1      | Unknown error                           | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（679517246）或工单联系技术支持团队。 |
| 2      | Service temporarily unavailable         | 服务暂不可用，请再次请求， 如果持续出现此类错误，请通过QQ群（679517246）或工单联系技术支持团队。 |
| 3      | Unsupported openapi method              | 调用的API不存在，请检查后重新尝试                            |
| 4      | Open api request limit reached          | 集群超限额                                                   |
| 6      | No permission to access data            | 无权限访问该用户数据                                         |
| 13     | Get service token failed                | 获取token失败                                                |
| 14     | IAM Certification failed                | IAM鉴权失败                                                  |
| 15     | app not exsits or create failed         | 应用不存在或者创建失败                                       |
| 17     | Open api daily request limit reached    | 每天请求量超限额，已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费；未上线计费的接口，请通过QQ群（679517246）联系群管手动提额 |
| 18     | Open api qps request limit reached      | QPS超限额，已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费；未上线计费的接口，请通过QQ群（679517246）联系群管手动提额 |
| 19     | Open api total request limit reached    | 请求总量超限额，已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费；未上线计费的接口，请通过QQ群（679517246）联系群管手动提额 |
| 100    | Invalid parameter                       | 无效的access_token参数，请检查后重新尝试                     |
| 110    | Access token invalid or no longer valid | access_token无效                                             |
| 111    | Access token expired                    | access token过期                                             |
| 282000 | internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（679517246）或工单联系技术支持团队。 |
| 216100 | invalid param                           | 请求中包含非法参数，请检查后重新尝试                         |
| 216101 | not enough param                        | 缺少必须的参数，请检查参数是否有遗漏                         |
| 216102 | service not support                     | 请求了不支持的服务，请检查调用的url                          |
| 216103 | param too long                          | 请求中某些参数过长，请检查后重新尝试                         |
| 216110 | appid not exist                         | appid不存在，请重新核对信息是否为后台应用列表中的appid       |
| 216200 | empty image                             | 图片为空，请检查后重新尝试                                   |
| 216201 | image format error                      | 上传的图片格式错误，现阶段我们支持的图片格式为：PNG、JPG、JPEG、BMP，请进行转码或更换图片 |
| 216202 | image size error                        | 上传的图片大小错误，现阶段我们支持的图片大小为：base64编码后小于4M，分辨率不高于4096*4096，请重新上传图片 |
| 216203 | image size error                        | 上传的图片base64编码有误，请校验base64编码方式，并重新上传图片 |
| 216630 | recognize error                         | 识别错误，请再次请求，如果持续出现此类错误，请通过QQ群（679517246）或工单联系技术支持团队。 |
| 216631 | recognize bank card error               | 识别银行卡错误，出现此问题的原因一般为：您上传的图片非银行卡正面，上传了异形卡的图片或上传的银行卡正品图片不完整 |
| 216633 | recognize idcard error                  | 识别身份证错误，出现此问题的原因一般为：您上传了非身份证图片或您上传的身份证图片不完整 |
| 216634 | detect error                            | 检测错误，请再次请求，如果持续出现此类错误，请通过QQ群（679517246）或工单联系技术支持团队。 |
| 282003 | missing parameters: {参数名}            | 请求参数缺失                                                 |
| 282005 | batch  processing error                 | 处理批量任务时发生部分或全部错误，请根据具体错误码排查       |
| 282006 | batch task  limit reached               | 批量任务处理数量超出限制，请将任务数量减少到10或10以下       |
| 282100 | image transcode error                   | 图片压缩转码错误                                             |
| 282101 | image split limit reached               | 长图片切分数量超限                                           |
| 282102 | target detect error                     | 未检测到图片中识别目标                                       |
| 282103 | target recognize error                  | 图片目标识别错误                                             |
| 282114 | url size error                          | URL长度超过1024字节或为0                                     |
| 282808 | request id: xxxxx not exist             | request id xxxxx 不存在                                      |
| 282809 | result type error                       | 返回结果请求错误（不属于excel或json）                        |
| 282810 | image recognize error                   | 图像识别错误                                                 |
| 283300 | Invalid argument                        | 入参格式有误，可检查下图片编码、代码格式是否有误             |
| 336000 | Internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（679517246）或工单联系技术支持团队 |
| 336001 | Invalid Argument                        | 入参格式有误，比如缺少必要参数、图片base64编码错误等等，可检查下图片编码、代码格式是否有误。有疑问请通过QQ群（679517246）或工单联系技术支持团队 |