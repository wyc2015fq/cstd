文档中心/OCR Driver License API (V2)

## 版本

2.0.0

## 描述

检测和识别中华人民共和国机动车驾驶证（以下称“驾照”）图像，并转化为结构化的文字信息。只可识别驾照正本(main sheet)正面和副本(second sheet)正面，一张照片最多可识别一个正本正面和一个副本正面。

- 驾照图像须为正拍（垂直角度拍摄），但是允许有一定程度的旋转角度；
- 图片最小 100*100 像素，长宽不得超过4096像素，否则会抛出错误；
- 支持图像里有一个或多个驾照的正本正面或副本正面，仅返回置信度最高的一个正本识别结果和一个副本识别结果，如果没有则该项返回为空。 

### 调用URL：

<https://api-cn.faceplusplus.com/cardpp/v2/ocrdriverlicense>

### 调用方法：

POST

## 请求参数

 

| 是否必选       | 参数名       | 类型                                                         | 参数说明                                                     |
| -------------- | ------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 必选           | api_key      | String                                                       | 调用此API的API Key                                           |
| 必选           | api_secret   | String                                                       | 调用此API的API Secret                                        |
| 必选（二选一） | image_url    | String                                                       | 图片的URL                                                    |
| image_file     | File         | 一个图片，二进制文件，需要用post multipart/form-data的方式上传。图像存储尺寸不能超过2MB，像素尺寸的长或宽都不能超过4096像素。如果同时传入了image_url和image_file参数，本API将使用image_file参数。 |                                                              |
| 可选           | return_score | Int                                                          | 当传入照片输出OCR结果时，是否同时返回置信度，可选参数为0和1。默认此参数是0，即不返回置信度。当经由api_key判断用户为已付费的正式用户，且此参数设定为1时，分别对每部分OCR识别结果同时输出置信度，并同时输出正本/副本的置信度。 |
| 可选           | mode         | string                                                       | 使用该API的快速识别模式还是完备识别模式，可选参数为fast和complete默认此参数是complete，即完备识别模式。快速识别模式只可识别驾照正本(main sheet)正面；完备识别模式支持识别驾照正本和副本。(注：新加参数） |

返回值说明

 

| 字段       | 类型   | 说明                                                         |
| ---------- | ------ | ------------------------------------------------------------ |
| request_id | String | 用于区分每一次请求的唯一的字符串。除非发生404（API_NOT_FOUND )或 403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| main       | array  | 检测出驾驶证**正本**的数组注：如果没有检测出正本则为空数组   |
| second     | array  | 检测出驾驶证**副页**的数组注：如果没有检测出副本则为空数组   |
| time_used  | Int    | 整个请求所花费的时间，单位为毫秒。除非发生404（API_NOT_FOUND )或 403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |

 

### main数组和second数组中单个元素的结构

| 识别对象       | 字段       | 类型                                                         | 说明                                                         |
| -------------- | ---------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| main（正本）   | confidence | float                                                        | 返回驾驶证正本置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |
| version        | Object     | 驾驶证正本版本及其置信度，返回字段分为以下两部分：content：表示驾驶证正本版本，int型，返回 2，表示是2013版本驾驶证；返回 1，表示是2008或更早版本驾驶证。confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| address        | Object     | 住址及其置信度，返回字段分为以下两部分：content：表示住址，string型confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| birthday       | Object     | 生日及其置信度，返回字段分为以下两部分： content：表示生日，string型，格式为YYYY-MM-DDconfidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| gender         | Object     | 性别及其置信度，返回字段分为以下两部分： content：表示性别（男/女），string型，其中取值为：“男”，“女”confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| license_number | Object     | 返回字段分为以下两部分： content：驾驶证号，string型confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| name           | Object     | 姓名及其置信度，返回字段分为以下两部分：content：姓名，string型confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| class          | Object     | 准驾车型及其置信度，返回字段分为以下两部分：content：准驾车型，string型confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| nationality    | Object     | 国籍及置信度，返回字段分为以下两部分：content：国籍，，string型confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| issued_by      | Object     | 签发机关及置信度，返回字段分为以下两部分：content：签发机关，string型confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| issue_date     | Object     | 初次领证日期及置信度，返回字段分为以下两部分：content：初次领证日期，string型，格式为YYYY-MM-DDconfidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| valid_from     | Object     | 有效日期及置信度，返回字段分为以下两部分：content：有效日期，，string型，格式为YYYY-MM-DDconfidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| valid_for      | Object     | 有效年限及置信度，返回字段分为以下两部分：content：有效年限，string型，例如 6年confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| valid_date     | Object     | 有效期限及置信度，返回字段分为以下两部分：content：有效期限格式为：YYYY-MM-DD至YYYY-MM-DD，string型，根据驾驶证版本不同，可能会返回valid_from和valid_for两个字段，另一种情况只返回valid_date字段。confidence：值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| second（副页） | confidence | float                                                        | 返回驾驶证副本置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |
| name           | Object     | 姓名及置信度，返回字段分为以下两部分：content：姓名，string型confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| license_number | Object     | 驾驶证号及置信度，返回字段分为以下两部分： content：驾驶证号，string型confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |
| file_number    | Object     | 档案编号及置信度，返回字段分为以下两部分：content：档案编号，string型confidence：表示置信度，值为一个 [0,100] 的浮点数，小数点后 3 位有效数字，仅正式用户设置return_score值为1时返回。 |                                                              |

## 返回值示例

### 正确请求返回示例

```
{
"second": [
{
"license_number": {
"content": "342221199505035555",
"confidence": 99.95
},
"confidence": 99.978,
"name": {
"content": "郭敏",
"confidence": 99.992
},
"file_number": {
"content": "500200660530",
"confidence": 99.993
}
}
],
"request_id": "1520565741,6fbba005-0de7-4784-9f63-3e8dcc55ca4e",
"time_used": 2497,
"main": [
{
"confidence": 99.904,
"valid_from": {
"content": "",
"confidence": 0
},
"name": {
"content": "郭敏",
"confidence": 99.742
},
"gender": {
"content": "女",
"confidence": 99.164
},
"address": {
"content": "重庆市渝北区宝圣大道555号",
"confidence": 95.756
},
"issued_by": {
"content": "重庆市公安局交通管理局",
"confidence": 99.398
},
"issue_date": {
"content": "20141225",
"confidence": 99.987
},
"birthday": {
"content": "19950503",
"confidence": 93.881
},
"valid_for": {
"content": "",
"confidence": 0
},
"version": {
"content": 2,
"confidence": 0
},
"license_number": {
"content": "342221199505035555",
"confidence": 99.983
},
"nationality": {
"content": "中国",
"confidence": 100
},
"valid_date": {
"content": "20141225至20201225",
"confidence": 99.954
},
"class": {
"content": "C1",
"confidence": 99.996
}
}
]
}
```

###  请求失败返回示例

```
{
   "error_message" : "INVALID_IMAGE_SIZE: image_file",
   "request_id" : "1461740007,71eeb124-08f0-47b3-8fc8-ac048cfa134d",
   "time_used" : 4
}
```

## 当前API特有的ERROR_MESSAGE

 

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| ------------ | -------------------------------------- | ------------------------------------------------------------ |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像太大。具体是指图像像素尺寸的长或宽超过4096像素。<param>对应图像太大的那个参数的名称 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |

 

## 通用的ERROR_MESSAGE

 

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| ------------- | ---------------------------- | ------------------------------------------------------------ |
| 403           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有：Denied. （没有权限调用当前API） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl "https://api-cn.faceplusplus.com/cardpp/v2/ocrdriverlicense" -F api_key=<api_key>
-F api_secret=<api_secret>
-F image=@image_driver_license.jpg 
```

在线支持