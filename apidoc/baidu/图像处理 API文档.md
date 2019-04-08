# 图像处理 API文档

# 图像无损放大

## 接口描述

输入一张图片，可以在尽量保持图像质量的条件下，将图像在长宽方向各放大两倍。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-process/v1/image_quality_enhance`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值                                |
| :----------- | :-------------------------------- |
| Content-Type | application/x-www-form-urlencoded |

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数  | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :---- | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image | true     | string | -          | Base64编码字符串，以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png，jpeg)，图片大小不超过4M。长宽乘积不超过800p x 800px。**注意：图片的base64编码是不包含图片头的，如（data:image/jpg;base64,）** |

**请求代码示例**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

**提示二**：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

```
图像无损放大
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/image_quality_enhance?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段   | 是否必选 | 类型   | 说明                       |
| :----- | :------- | :----- | :------------------------- |
| log_id | 是       | uint64 | 唯一的log id，用于问题定位 |
| image  | 否       | string | base64编码图片             |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
    "log_id":739539874,
    "image":base64str
}
```

# 图像去雾

## 接口描述

对浓雾天气下拍摄，导致细节无法辨认的图像进行去雾处理，还原更清晰真实的图像。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-process/v1/dehaze`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token，参考”[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值                                |
| :----------- | :-------------------------------- |
| Content-Type | application/x-www-form-urlencoded |

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数  | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :---- | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image | true     | string | -          | base64编码后大小不超过4M，最短边至少200px，最长边最大4096px，长宽比3：1以内。**注意：图片的base64编码是不包含图片头的，如（data:image/jpg;base64,）** |

**请求代码示例**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

**提示二**：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

```
图像去雾
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/dehaze?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段   | 是否必选 | 类型   | 说明                       |
| :----- | :------- | :----- | :------------------------- |
| log_id | 是       | uint64 | 唯一的log id，用于问题定位 |
| image  | 否       | string | base64编码图片             |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
     "log_id":739539874,
     "image":base64str
}
```

# 图像对比度增强

## 接口描述

调整过暗或者过亮图像的对比度，使图像更加鲜明。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-process/v1/contrast_enhance`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token，参考”[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值                                |
| :----------- | :-------------------------------- |
| Content-Type | application/x-www-form-urlencoded |

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数  | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :---- | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image | true     | string | -          | base64编码后大小不超过4M，最短边至少64px，最长边最大4096px，长宽比3：1以内。**注意：图片的base64编码是不包含图片头的，如（data:image/jpg;base64,）** |

**请求代码示例**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

**提示二**：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

```
图像对比度增强
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/contrast_enhance?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段   | 是否必选 | 类型   | 说明                       |
| :----- | :------- | :----- | :------------------------- |
| log_id | 是       | uint64 | 唯一的log id，用于问题定位 |
| image  | 否       | string | base64编码图片             |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
    "log_id":739539874,
    "image":base64str
}
```

# 黑白图像上色

## 接口描述

智能识别黑白图像内容并填充色彩，使黑白图像变得鲜活。 注意：在正式使用之前，请在[图像处理页面提交合作咨询](http://ai.baidu.com/tech/imageprocess)，或者申请加入百度图像识别官方QQ群（群号:659268104），提供公司名称、appid、应用场景，工作人员将协助开通调用求权限。**工作人员协助开通权限后该接口方可使用**。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-process/v1/colourize`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token，参考”[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值                                |
| :----------- | :-------------------------------- |
| Content-Type | application/x-www-form-urlencoded |

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数  | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :---- | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image | true     | string | -          | base64编码后大小不超过4M，最短边至少64px，最长边最大800px，长宽比3：1以内。**注意：图片的base64编码是不包含图片头的，如（data:image/jpg;base64,）** |

## 返回说明

**返回参数**

| 字段   | 是否必选 | 类型   | 说明                       |
| :----- | :------- | :----- | :------------------------- |
| log_id | 是       | uint64 | 唯一的log id，用于问题定位 |
| image  | 否       | string | base64编码图片             |

**返回示例**

```
{
	"log_id": "6876747463538438254",
	"image": "处理后图片的Base64编码"
}
```

# 错误码

若请求错误，服务器将返回的JSON文本包含以下参数：

- **error_code：**错误码
- **error_msg：**错误描述信息，帮助理解和解决发生的错误

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
| 1      | Unknown error                           | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队。 |
| 2      | Service temporarily unavailable         | 服务暂不可用，请再次请求， 如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队。 |
| 3      | Unsupported openapi method              | 调用的API不存在，请检查后重新尝试                            |
| 4      | Open api request limit reached          | 集群超限额                                                   |
| 6      | No permission to access data            | 无权限访问该用户数据                                         |
| 13     | Get service token failed                | 获取token失败                                                |
| 14     | IAM Certification failed                | IAM鉴权失败                                                  |
| 15     | app not exsits or create failed         | 应用不存在或者创建失败                                       |
| 17     | Open api daily request limit reached    | 每天请求量超限额，已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费；未上线计费的接口，请通过QQ群（659268104）联系群管手动提额 |
| 18     | Open api qps request limit reached      | QPS超限额，已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费；未上线计费的接口，请通过QQ群（659268104）联系群管手动提额 |
| 19     | Open api total request limit reached    | 请求总量超限额，已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费；未上线计费的接口，请通过QQ群（659268104）联系群管手动提额 |
| 100    | Invalid parameter                       | 无效的access_token参数，请检查后重新尝试                     |
| 110    | Access token invalid or no longer valid | access_token无效                                             |
| 111    | Access token expired                    | access token过期                                             |
| 216200 | empty image                             | 图片为空，请检查后重新尝试                                   |
| 216201 | image format error                      | 上传的图片格式错误，现阶段我们支持的图片格式为：PNG、JPG、JPEG、BMP，请进行转码或更换图片 |
| 216202 | image size error                        | 上传的图片大小错误，请参考输入参数说明重新上传图片           |
| 216630 | recognize error                         | 识别错误，请再次请求，如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队 |
| 282000 | internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队 |