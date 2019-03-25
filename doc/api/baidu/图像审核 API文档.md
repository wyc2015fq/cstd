# 图像审核 API文档

# 简介

Hi，您好，欢迎使用百度图像审核API服务。

本文档主要针对API开发者，描述百度图像审核接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**；
- 如有疑问，进入[AI社区交流](http://ai.baidu.com/forum/topic/list/172)或提交[合作咨询](https://cloud.baidu.com/survey/AICooperativeConsultingApply.html)

## 接口能力

| 接口名称           | 接口能力简要描述                                             |
| :----------------- | :----------------------------------------------------------- |
| 组合服务接口       | 提供一个API接口，同时调用多个模型服务。支持通用文字识别、网络图片文字识别、色情识别、暴恐识别、政治敏感识别、广告检测、图文审核、恶心图像识别、图像质量检测一系列模型服务的调用。可通过入参灵活指定需要调用的模型服务，传入图片或图片URL，返回指定模型的识别结果。相比于图像审核接口，本接口对各个模型的结果进行了组合，json原样输出。 |
| 自定义图像审核接口 | 为用户提供色情识别、暴恐识别、政治敏感人物识别、广告识别、图文审核、恶心图像识别等一系列图像识别接口的一站式服务调用，并且支持用户在控制台中自定义配置所有接口的报警阈值和疑似区间，上传自定义文本黑库和敏感人物名单等。 |
| GIF色情识别接口    | 识别GIF图片的色情程度，返回图片的色情、性感、正常分值。      |
| 短视频审核接口     | 针对属于同一个短视频的图片集进行审核并针对该短视频返回结果，用户可以指定具体的审核维度 |
| 用户头像审核       | 通过人脸检测、文字识别、色情识别、暴恐识别、政治敏感识别、公众人物检测等多个维度，识别图片是否合规，直接返回审核结果，并支持自定义黑词库和审核规则配置。 |
| 图像审核反馈接口   | 用户可以使用该接口对图像审核的结果进行反馈，提交badcase      |

## 请求格式

POST方式调用

**注意**： Content-Type为`application/x-www-form-urlencoded`，然后通过`urlencode`格式化请求体。 其中，图像审核组合服务接口需设置为：Content-Type为`application/json;charset=utf-8`

## 返回格式

JSON格式

## 请求限制

请求图片需经过`base64编码`：图片的base64编码指将一副图片数据编码成一串字符串，使用该字符串代替图像地址。您可以首先得到图片的二进制，然后用Base64格式编码即可。

**注意**：图片的base64编码是不包含图片头的，如`（data:image/jpg;base64,）`

**请求格式支持**：PNG、JPG、JPEG、BMP；Gif色情识别支持GIF图片。

**图片大小支持**：图片大小4M以下，最长边不大于4096。

# 调用方式

调用AI服务相关的API接口有两种调用方式，两种不同的调用方式采用相同的接口URL。

区别在于**请求方式**和**鉴权方法**不一样，请求参数和返回结果一致。

## 调用方式一

**请求URL数据格式**

向API服务地址使用POST发送请求，必须在URL中带上参数：

**access_token: ** 必须参数，参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)”。

> 注意：`access_token`的有效期为30天，需要每30天进行定期更换；

POST中参数按照API接口说明调用即可。

例如人脸识别API，使用HTTPS POST发送：

```
https://aip.baidubce.com/rest/2.0/face/v1/detect?access_token=24.f9ba9c5241b67688bb4adbed8bc91dec.2592000.1485570332.282335-8574074
```

**获取access_token示例代码**

bash

PHP

Java

Python

C++

C#

Node

```
#!/bin/bash
curl -i -k 'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=【百度云应用的AK】&client_secret=【百度云应用的SK】'
```

> **说明**：方式一鉴权使用的Access_token必须通过API Key和Secret Key获取。

## 调用方式二

**请求头域内容**

在请求的HTTP头域中包含以下信息：

- host（必填）
- x-bce-date （必填）
- x-bce-request-id（选填）
- authorization（必填）
- content-type（必填）
- content-length（选填）

作为示例，以下是一个标准的人脸识别的请求头域内容:

```
POST /rest/2.0/face/v1/detect HTTP/1.1
accept-encoding: gzip, deflate
x-bce-date: 2015-03-24T13:02:00Z
connection: keep-alive
accept: */*
host: aip.baidubce.com
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
content-type: application/x-www-form-urlencoded
authorization: bce-auth-v1/46bd9968a6194b4bbdf0341f2286ccce/2015-03-24T13:02:00Z/1800/host;x-bce-date/994014d96b0eb26578e039fa053a4f9003425da4bfedf33f4790882fb4c54903
```

> **说明**：方式二鉴权使用的[API认证机制](https://cloud.baidu.com/doc/Reference/AuthenticationMechanism.html)，authorization必须通过百度云的[AK/SK](https://cloud.baidu.com/doc/Reference/GetAKSK.html)生成。

# 组合服务接口

## 接口描述

提供一个API接口，同时调用多个模型服务。支持文字识别的多个接口、网络图片文字识别、色情识别、暴恐识别、政治敏感识别、广告检测、图文审核、恶心图像识别、图像质量检测一系列模型服务的调用。可通过入参灵活指定需要调用的模型服务，传入图片或图片URL，返回指定模型的识别结果。 *色情识别会基于百度默认的审核标准给出一个推荐值（conclusion），如果您需要对具体的标签做重新的划分，建议使用细粒度标签分类的结果（result_fine）*
组合服务接口根据请求参数确定要使用的模型服务类型，具体的请求参数和返回结果见下方描述。


其中文字识别相关接口同时提供独立的服务接口，针对每个模型的出入参，可参考百度AI官网每个独立服务接口的文档。色情识别、暴恐识别、政治敏感识别、广告检测、图文审核、恶心图像识别、图像质量检测不提供独立的服务接口，相关出入参详细描述见下方。

## 接口使用说明


接口权限管理： 使用图像审核组合服务接口，您需要首先在控制台中开通单个模型服务的权限。例如：您希望通过图像审核组合服务接口调用通用文字识别、色情识别、广告检测三个模型服务，则您需要登录并进入图像审核控制台，新建应用并勾选文字识别下的通用文字识别、图像审核下的色情识别、广告检测。


接口调用量限制： 使用图像审核组合服务接口，您的每日最大调用次数限制、最大并发限制，依赖于您选择调用的具体模型服务的调用次数限制和并发限制。具体数值可以在相应的服务控制台中查看。


接口调用量报表： 图像审核组合服务接口不提供单独的调用量报表，根据您每次调用实际选择的模型服务，在控制台监控报表中可查看每个模型服务的调用量统计。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/api/v1/solution/direct/img_censor`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值                             |
| :----------- | :----------------------------- |
| Content-Type | application/json;charset=utf-8 |

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数       | 类型       | 是否必须 | 说明                                                         |
| :--------- | :--------- | :------- | :----------------------------------------------------------- |
| image      | string     | 否       | 图像数据，base64编码，不能与imgUrl并存                       |
| imgUrl     | string     | 否       | 图像Url，不能与image并存，不需要urlEncode                    |
| scenes     | array      | 是       | 指定本次调用的模型服务，以字符串数组表示。元素含义如下： ocr:通用文字识别 politician：政治敏感识别 antiporn：色情识别 terror：暴恐识别 anti_spam：图文审核（对图像中的文字内容进行合规性审核） webimage：网络图片文字识别 disgust:恶心图像识别 watermark:广告检测 quality:图像质量检测 accurate：通用文字识别（高精度含位置版） accuratebasic：通用文字识别（高精度版） |
| scenesConf | jsonObject | 否       | 对特定服务，支持的个性化参数，若不填则使用默认设置。jsonObject说明： key为要设置入参的服务类型，可取值同scenes字段，例如： 1、ocr：通用文字识别 2、webimage：网络图片文字识别 value为各模型服务个性化参数，详情请参照下文或百度AI官网“[文字识别具体接口文档](http://ai.baidu.com/docs#/OCR-API/top)” |

Body示例：

```
{	// image 与 imgUrl二选其一
	"image": "/9j/4AAQSkZJRgABAQEPMJkR0FdXVridlPy/9k=",
	"imgUrl": "http://aip.bdstatic.com/portal/dist/1522402512491/ai_images/technology/imagecensoring/demo-card-4.jpg",
	"scenes": ["ocr", "politician", "antiporn", "terror", "webimage", "disgust", "watermark", "quality", "accurate", "accuratebasic"],
	"sceneConf": { // 可不填写
		"webimage": {}, // 可不填写
		"ocr": {
			"detect_direction": "false",
			"recognize_granularity": "big",
			"language_type": "CHN_ENG",
			"mask": "-"
		}
	}
}
```

**请求示例代码**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

**提示二**：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

```
#!/bin/bash
curl -i -k 'http://aip.baidubce.com/api/v1/solution/direct/img_censor?access_token=【调用鉴权接口获取的token】' --data '{"imgUrl":"https%3a%2f%2fss0.bdstatic.com%2f94oJfD_bAAcT8t7mm9GUKT-xh_%2ftimg%3fimage%26quality%3d100%26size%3db4000_4000%26sec%3d1494581871%26di%3d133fdf9f920fd7a9d6171f59bc965e8d%26src%3dhttp%3a%2f%2fimg3.duitang.com%2fuploads%2fblog%2f201404%2f19%2f20140419204851_Bhv24.thumb.700_0.jpeg","scenes":["ocr","face","public","politician","antiporn","terror","webimage"],"sceneConf":{"ocr":{"recognize_granularity":"big","language_type":"CHN_ENG","detect_direction":"true","detect_language":"true"},"antiporn":{}}}' -H 'Content-Type:application/json; charset=UTF-8'
```

## 返回说明

**返回参数**

| 字段          | 类型   | 是否必须 | 说明                                                         |
| :------------ | :----- | :------- | :----------------------------------------------------------- |
| log_id        | uint64 | 是       | 请求标识码，随机数，唯一。                                   |
| result        | object | 是       | 返回结果json串，其内包含用户要调用的各个模型服务的返回结果。 |
| ocr           | object | 否       | 文字识别服务返回结果。请参照下文，文字识别说明或“[通用文字识别接口文档](http://ai.baidu.com/docs#/OCR-API/top)” |
| politician    | object | 否       | 政治敏感识别返回结果。 请参照下文，政治敏感识别说明。        |
| antiporn      | object | 否       | 色情识别返回结果。请参照下文，色情识别说明。                 |
| terror        | object | 否       | 暴恐识别返回结果。请参照下文，暴恐识别说明。                 |
| webimage      | object | 否       | 网络图片文字识别结果。请参照下文，网络图片文字识别说明或“[网络图片文字识别接口文档](http://ai.baidu.com/docs#/OCR-API/top)” |
| disgust       | object | 否       | 恶心图像识别结果。请参照下文，恶心图像识别说明。             |
| watermark     | object | 否       | 广告检测返回结果。请参照下文，广告检测说明。                 |
| quality       | object | 否       | 图像质量检测返回结果。请参照下文，图像质量检测说明。         |
| accurate      | object | 否       | 通用文字识别（高精度含位置版）返回结果。请参照下文，通用文字识别（高精度含位置版）说明或“[通用文字识别（含位置高精度版）接口文档](http://ai.baidu.com/docs#/OCR-API/top)” |
| accuratebasic | object | 否       | 通用文字识别（高精度版）返回结果。请参照下文，通用文字识别（高精度版）说明或“[通用文字识别（高精度版）接口文档](http://ai.baidu.com/docs#/OCR-API/top)” |

**返回示例**

```
{
	"result": {
		"antiporn": {
			"result": [{
				"probability": 0.000078,
				"class_name": "性感"
			}, {
				"probability": 0.000001,
				"class_name": "色情"
			}, {
				"probability": 0.999922,
				"class_name": "正常"
			}],
			"conclusion": "正常",
			"log_id": 1612587304059199822,
			"result_fine": [{
				"probability": 0.0,
				"class_name": "一般色情"
			}, {
				"probability": 0.998109,
				"class_name": "一般正常"
			}, {
				"probability": 0.0,
				"class_name": "卡通色情"
			}, {
				"probability": 0.001783,
				"class_name": "卡通正常"
			}, {
				"probability": 0.000026,
				"class_name": "特殊类"
			}, {
				"probability": 0.000077,
				"class_name": "女性性感"
			}, {
				"probability": 0.000001,
				"class_name": "男性性感"
			}, {
				"probability": 0.0,
				"class_name": "SM"
			}, {
				"probability": 0.0,
				"class_name": "低俗"
			}, {
				"probability": 0.000001,
				"class_name": "性玩具"
			}, {
				"probability": 0.000002,
				"class_name": "亲密行为"
			}],
			"result_num": 11,
			"confidence_coefficient": "确定"
		},
		"webimage": {
			"log_id": 8274566822628354611,
			"words_result": [{
				"words": "百度A|开放平台·深度学习平台"
			}, {
				"words": "吧包"
			}, {
				"words": "Paddlepaddle是并行分布式全功能深度学习框架"
			}, {
				"words": "易学易用、高效灵活"
			}, {
				"words": "支持海量图像识别分类/机器翻译/自动驾驶等多个领域"
			}, {
				"words": "现已全面开源, Intel已加入开源贡献"
			}, {
				"words": "Paddlepaddle成为第一个官方支持MKL-DNN的深度学习框架"
			}, {
				"words": "邮件咨询: siebold.com"
			}, {
				"words": "Paddlepaddle开放路线图"
			}, {
				"words": "口防口"
			}],
			"words_result_num": 10,
			"direction": 0
		},
		"terror": {
			"result": 4.6869706693542E-7,
			"log_id": 2466710332049314848,
			"result_fine": [{
				"score": 0.99997806549072,
				"name": "正常"
			}, {
				"score": 0.000021282241505105,
				"name": "警察部队"
			}, {
				"score": 1.4284228022632E-8,
				"name": "血腥"
			}, {
				"score": 1.1518556419787E-7,
				"name": "尸体"
			}, {
				"score": 2.634956217662E-7,
				"name": "爆炸火灾"
			}, {
				"score": 1.5594283198084E-8,
				"name": "杀人"
			}, {
				"score": 3.0972262976547E-8,
				"name": "暴乱"
			}, {
				"score": 2.6761359350758E-8,
				"name": "暴恐人物"
			}, {
				"score": 1.6384501577704E-7,
				"name": "军事武器"
			}, {
				"score": 2.4037409840361E-9,
				"name": "暴恐旗帜"
			}, {
				"score": 3.3956993661377E-8,
				"name": "血腥动物或动物尸体"
			}],
			"result_coarse": [{
				"score": 0.99999958276749,
				"name": "正常"
			}, {
				"score": 4.6869706693542E-7,
				"name": "暴恐"
			}]
		},
		"watermark": {
			"result": [{
				"probability": 0.92880970239639,
				"type": "QR code"
			}],
			"log_id": 964898612068672707,
			"result_num": 1
		},
		"accurate": {
			"log_id": 7895736306899747812,
			"words_result": [{
				"words": "百度A|开放平台·深度学习平台",
				"location": {
					"top": 159,
					"left": 1161,
					"width": 234,
					"height": 19
				},
				"chars": [{
					"char": "百",
					"location": {
						"top": 159,
						"left": 1161,
						"width": 16,
						"height": 19
					}
				}, {
					"char": "度",
					"location": {
						"top": 159,
						"left": 1177,
						"width": 16,
						"height": 19
					}
				}, {
					"char": "A",
					"location": {
						"top": 159,
						"left": 1194,
						"width": 12,
						"height": 19
					}
				}, {
					"char": "|",
					"location": {
						"top": 159,
						"left": 1206,
						"width": 10,
						"height": 19
					}
				}, {
					"char": "开",
					"location": {
						"top": 159,
						"left": 1217,
						"width": 16,
						"height": 19
					}
				}, {
					"char": "放",
					"location": {
						"top": 159,
						"left": 1234,
						"width": 16,
						"height": 19
					}
				}, {
					"char": "平",
					"location": {
						"top": 159,
						"left": 1251,
						"width": 15,
						"height": 19
					}
				}, {
					"char": "台",
					"location": {
						"top": 159,
						"left": 1268,
						"width": 15,
						"height": 19
					}
				}, {
					"char": "·",
					"location": {
						"top": 159,
						"left": 1285,
						"width": 10,
						"height": 19
					}
				}, {
					"char": "深",
					"location": {
						"top": 159,
						"left": 1296,
						"width": 16,
						"height": 19
					}
				}, {
					"char": "度",
					"location": {
						"top": 159,
						"left": 1313,
						"width": 15,
						"height": 19
					}
				}, {
					"char": "学",
					"location": {
						"top": 159,
						"left": 1330,
						"width": 15,
						"height": 19
					}
				}, {
					"char": "习",
					"location": {
						"top": 159,
						"left": 1346,
						"width": 16,
						"height": 19
					}
				}, {
					"char": "平",
					"location": {
						"top": 159,
						"left": 1363,
						"width": 16,
						"height": 19
					}
				}, {
					"char": "台",
					"location": {
						"top": 159,
						"left": 1380,
						"width": 15,
						"height": 19
					}
				}]
			}, {
				"words": "海量图",
				"location": {
					"top": 447,
					"left": 1123,
					"width": 40,
					"height": 16
				},
				"chars": [{
					"char": "海",
					"location": {
						"top": 447,
						"left": 1123,
						"width": 13,
						"height": 16
					}
				}, {
					"char": "量",
					"location": {
						"top": 447,
						"left": 1137,
						"width": 13,
						"height": 16
					}
				}, {
					"char": "图",
					"location": {
						"top": 447,
						"left": 1151,
						"width": 12,
						"height": 16
					}
				}]
			}, {
				"words": " Paddlepaddle开放路线图",
				"location": {
					"top": 563,
					"left": 1110,
					"width": 340,
					"height": 28
				},
				"chars": [{
					"char": "P",
					"location": {
						"top": 567,
						"left": 1112,
						"width": 15,
						"height": 20
					}
				}, {
					"char": "a",
					"location": {
						"top": 567,
						"left": 1123,
						"width": 23,
						"height": 20
					}
				}, {
					"char": "d",
					"location": {
						"top": 567,
						"left": 1141,
						"width": 23,
						"height": 20
					}
				}, {
					"char": "d",
					"location": {
						"top": 567,
						"left": 1160,
						"width": 19,
						"height": 20
					}
				}, {
					"char": "l",
					"location": {
						"top": 567,
						"left": 1175,
						"width": 16,
						"height": 20
					}
				}, {
					"char": "e",
					"location": {
						"top": 567,
						"left": 1187,
						"width": 16,
						"height": 20
					}
				}, {
					"char": "p",
					"location": {
						"top": 567,
						"left": 1199,
						"width": 21,
						"height": 20
					}
				}, {
					"char": "a",
					"location": {
						"top": 567,
						"left": 1216,
						"width": 23,
						"height": 20
					}
				}, {
					"char": "d",
					"location": {
						"top": 567,
						"left": 1235,
						"width": 21,
						"height": 20
					}
				}, {
					"char": "d",
					"location": {
						"top": 567,
						"left": 1252,
						"width": 20,
						"height": 20
					}
				}, {
					"char": "l",
					"location": {
						"top": 567,
						"left": 1268,
						"width": 16,
						"height": 20
					}
				}, {
					"char": "e",
					"location": {
						"top": 567,
						"left": 1281,
						"width": 13,
						"height": 20
					}
				}, {
					"char": "开",
					"location": {
						"top": 563,
						"left": 1307,
						"width": 24,
						"height": 28
					}
				}, {
					"char": "放",
					"location": {
						"top": 563,
						"left": 1333,
						"width": 23,
						"height": 28
					}
				}, {
					"char": "路",
					"location": {
						"top": 563,
						"left": 1367,
						"width": 24,
						"height": 28
					}
				}, {
					"char": "线",
					"location": {
						"top": 563,
						"left": 1393,
						"width": 24,
						"height": 28
					}
				}, {
					"char": "图",
					"location": {
						"top": 563,
						"left": 1427,
						"width": 23,
						"height": 28
					}
				}]
			}, {
				"words": "回口",
				"location": {
					"top": 627,
					"left": 1236,
					"width": 99,
					"height": 39
				},
				"chars": [{
					"char": "回",
					"location": {
						"top": 627,
						"left": 1236,
						"width": 31,
						"height": 39
					}
				}, {
					"char": "口",
					"location": {
						"top": 627,
						"left": 1306,
						"width": 29,
						"height": 39
					}
				}]
			}],
			"words_result_num": 4,
			"direction": 0
		},
		"politician": {
			"result": [],
			"log_id": 8102854853659331584,
			"include_politician": "否",
			"result_num": 0,
			"result_confidence": "确定"
		},
		"disgust": {
			"result": 3.0946500828577E-7,
			"log_id": 1092037867664999522
		},
		"accuratebasic": {
			"log_id": 2618396726972462630,
			"words_result": [{
				"words": "百度A|开放平台·深度学习平台"
			}, {
				"words": "海量图"
			}, {
				"words": " Paddlepaddle开放路线图"
			}, {
				"words": "回口"
			}],
			"words_result_num": 4
		},
		"ocr": {
			"log_id": 6855534665455327133,
			"words_result": [{
				"words": "百度AI开放平台·深度学习平台"
			}, {
				"words": "Paddlepaddle是并行分布式全功能深度学习框架"
			}, {
				"words": "易学易用、高高效灵活"
			}, {
				"words": "支持海量图像识别分类/机器翻译/自动驾驶等多个领域"
			}, {
				"words": "现已全面开源, Intel已加入开源贡献"
			}, {
				"words": "Paddlepaddlel成为第一个官方支持MKL-DNN的深度学习框架"
			}, {
				"words": "件词: siebold,cor"
			}, {
				"words": "量"
			}, {
				"words": "Paddlepaddle开放路线图"
			}, {
				"words": "口口"
			}],
			"words_result_num": 10,
			"language": -1,
			"direction": 0
		},
		"quality": {
			"result": {
				"aesthetic": 0.514856432875,
				"clarity": 0.53470981121063
			},
			"log_id": 5106174173360322794
		}
	},
	"log_id": 152264128109105
}
```

### 通用文字识别说明

请求参数：

| 参数             | 是否必选 | 类型    | 可选值范围                                            | 说明                                                         |
| :--------------- | :------- | :------ | :---------------------------------------------------- | :----------------------------------------------------------- |
| language_type    | false    | string  | CHN_ENG、ENG、POR、FRE、GER、 ITA、SPA、RUS、JAP、KOR | 识别语言类型，默认为CHN_ENG。可选值包括： - CHN_ENG：中英文混合； - ENG：英文； - POR：葡萄牙语； - FRE：法语； - GER：德语； - ITA：意大利语； - SPA：西班牙语； - RUS：俄语； - JAP：日语； - KOR：韩语 |
| detect_direction | false    | boolean | true、false                                           | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| detect_language  | false    | string  | true、false                                           | 是否检测语言，默认不检测。当前支持（中文、英语、日语、韩语） |
| probability      | false    | string  | true、false                                           | 是否返回识别结果中每一行的置信度                             |

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 字段             | 类型    | 是否必选 | 说明                                                         |
| :--------------- | :------ | :------- | :----------------------------------------------------------- |
| direction        | int32   | 否       | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id           | uint64  | 是       | 唯一的log id，用于问题定位                                   |
| words_result     | array() | 是       | 识别结果数组                                                 |
| words_result_num | uint32  | 是       | 识别结果数，表示words_result的元素个数                       |
| +words           | string  | 否       | 识别结果字符串                                               |
| probability      | object  | 否       | 识别结果中每一行的置信度值，包含average：行置信度平均值，variance：行置信度方差，min：行置信度最小值 |

返回值样例：

```
 {
	"log_id": 2471272194,
	"words_result_num": 2,
	"words_result": [{
			"words": " TSINGTAO"
		},
		{
			"words": "青島睥酒"
		}
	]
}
```

### 政治敏感识别说明

请求参数:

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数               | 类型          | 是否必须 | 说明                                                       |
| :----------------- | :------------ | :------- | :--------------------------------------------------------- |
| include_politician | string        | 是       | 是否包含政治人物，取值为”是”或”否”                         |
| result_confidence  | string        | 是       | 对于include_politician字段的置信度，取值为“确定”或“不确定” |
| result_num         | uint32        | 是       | 实际检测到人脸数目(不大于max_face_num，默认为5)            |
| result             | array(object) | 是       | 识别出的结果数据                                           |
| +location          | object        | 是       | 人脸在输入图片中的位置                                     |
| ++left             | uint32        | 是       | 人脸区域离左边界的距离                                     |
| ++top              | uint32        | 是       | 人脸区域离上边界的距离                                     |
| ++width            | uint32        | 是       | 人脸区域的宽度                                             |
| ++height           | uint32        | 是       | 人脸区域的高度                                             |
| +stars             | object[]      | 是       | 政治人物数组                                               |
| ++name             | string        | 是       | 姓名                                                       |
| ++star_id          | string        | 是       | 人物id，全局唯一                                           |
| ++probability      | float         | 是       | 相似度，[0, 1]                                             |
| log_id             | uint64        | 是       | 请求标识码，随机数，唯一                                   |

返回值样例：

```
    {
        "log_id": 3268660173,
        "include_politician": "是",
        "result_confidence": "确定",
        "result_num": 1,
        "result": [
            {
                "location": {
                    "left": 132,
                    "top": 168,
                    "width": 238,
                    "height": 223
                },
                "stars": [
                    {
                        "name": "习近平",
                        "star_id": "515617",
                        "probability": 0.9750030040741
                    }
                ]
            }
        ]
    }
```

### 色情识说明

请求参数：

```
无额参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数         | 类型          | 是否必须 | 说明                                                     |
| :----------- | :------------ | :------- | :------------------------------------------------------- |
| log_id       | uint64        | 是       | 请求标识码，随机数，唯一                                 |
| result_num   | Int           | 是       | 返回结果数目，即：**result_fine数组中元素个数**          |
| result       | Array[Object] | 是       | 结果数组，每项内容对应一个三分类维度的结果               |
| result_fine  | Array[Object] | 是       | **结果数组，每项内容对应一个细粒度分类维度的结果，新增** |
| conclusion   | String        | 是       | 最终认定分类结果                                         |
| +class_name  | String        | 是       | 分类结果名称，示例：色情                                 |
| +probability | double        | 是       | 分类结果置信度，示例：0.89471650123596                   |

**默认标签分类** **色情** ：一般色情、卡通色情、SM **性感**：男性性感、女性性感 **正常**：低俗、特殊类、性玩具、亲密行为、卡通正常、儿童裸露、自然男性裸露、艺术品色情、卡通女性性感、卡通亲密行为、一般正常

**如果您的业务对于具体标签的判断和上述默认分类不一致，请使用"result_fine"中的细粒度标签进行违规判断**

返回值样例：

```
{
	"result": [{
		"probability": 0.000078,
		"class_name": "性感"
	}, {
		"probability": 0.000001,
		"class_name": "色情"
	}, {
		"probability": 0.999922,
		"class_name": "正常"
	}],
	"conclusion": "正常",
	"log_id": 9029727869599109902,
	"result_fine": [{
		"probability": 0.0,
		"class_name": "一般色情"
	}, {
		"probability": 0.998109,
		"class_name": "一般正常"
	}, {
		"probability": 0.0,
		"class_name": "卡通色情"
	}, {
		"probability": 0.001783,
		"class_name": "卡通正常"
	}, {
		"probability": 0.000026,
		"class_name": "特殊类"
	}, {
		"probability": 0.000077,
		"class_name": "女性性感"
	}, {
		"probability": 0.000001,
		"class_name": "男性性感"
	}, {
		"probability": 0.0,
		"class_name": "SM"
	}, {
		"probability": 0.0,
		"class_name": "低俗"
	}, {
		"probability": 0.000001,
		"class_name": "性玩具"
	}, {
			"probability": 0.000001,
		"class_name": "儿童裸露"
	}, {
			"probability": 0.000001,
		"class_name": "自然男性裸露"
	}, {
			"probability": 0.000001,
		"class_name": "艺术品色情"
	}, {
			"probability": 0.000001,
		"class_name": "卡通女性性感"
	}, {
		"probability": 0.000002,
		"class_name": "卡通亲密行为"
	}],
			"probability": 0.000002,
		"class_name": "特殊类"
	}],
	"result_num": 11,
	"confidence_coefficient": "确定"
}
```

### 暴恐识别说明

请求参数：

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数        | 类型          | 是否必须 | 说明                                                     |
| :---------- | :------------ | :------- | :------------------------------------------------------- |
| log_id      | uint64        | 是       | 请求标识码，随机数，唯一                                 |
| result      | double        | 是       | 得分，范围[0.0,1.0]，分值越高代表是暴恐的可能性越高      |
| result_fine | Array[Object] | 是       | **结果数组，每项内容对应一个细粒度分类维度的结果，新增** |
| score       | double        | 是       | 分类结果置信度，示例：0.89471650123596                   |
| name        | string        | 是       | 分类名                                                   |

返回值样例：

```
{
	"errno": 0,
	"msg": "success",
	"data": {
		"log_id": "463999181",
		"result": 0.99994814395905,
		"result_coarse": [
			{
				"name": "正常",
				"score": 0.000051898459787481
			},
			{
				"name": "暴恐",
				"score": 0.99994814395905
			}
		],
		"result_fine": [
			{
				"name": "正常",
				"score": 0.000051839437219314
			},
			{
				"name": "警察部队",
				"score": 1.311379804747e-8
			},
			{
				"name": "血腥",
				"score": 2.3871018584032e-7
			},
			{
				"name": "尸体",
				"score": 3.8288050063784e-8
			},
			{
				"name": "爆炸火灾",
				"score": 1.2556726325386e-8
			},
			{
				"name": "杀人",
				"score": 6.4856834569582e-7
			},
			{
				"name": "暴乱",
				"score": 9.2038261456651e-9
			},
			{
				"name": "暴恐人物",
				"score": 0.99994593858719
			},
			{
				"name": "军事武器",
				"score": 4.4469405935388e-8
			},
			{
				"name": "暴恐旗帜",
				"score": 0.0000012568434613058
			},
			{
				"name": "血腥动物或动物尸体",
				"score": 1.4398612302458e-9
			},
			{
				"name": "车祸",
				"score": 3.8261271928519e-10
			}
		]
	}
}
```

### 网络图片文字识别说明

请求参数：

| 参数             | 是否必选 | 类型    | 可选值范围  | 说明                                                         |
| :--------------- | :------- | :------ | :---------- | :----------------------------------------------------------- |
| detect_direction | false    | boolean | true、false | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| detect_language  | FALSE    | string  | true、false | 是否检测语言，默认不检测。当前支持（中文、英语、日语、韩语） |

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 字段             | 是否必选 | 类型 | 说明                                                         |
| :--------------- | :------- | :--- | :----------------------------------------------------------- |
| direction        | int32    | 否   | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id           | uint64   | 是   | 唯一的log id，用于问题定位                                   |
| words_result     | array()  | 是   | 识别结果数组                                                 |
| words_result_num | uint32   | 是   | 识别结果数，表示words_result的元素个数                       |
| +words           | string   | 否   | 识别结果字符串                                               |
| probability      | object   | 否   | 识别结果中每一行的置信度值，包含average：行置信度平均值，variance：行置信度方差，min：行置信度最小值 |

返回值样例：

```
{
	"log_id": 5014546094448334973,
	"words_result": [{
		"words": "百度A|开放平台·深度学习平台"
	}, {
		"words": "吧包"
	}, {
		"words": "Paddlepaddle是并行分布式全功能深度学习框架"
	}, {
		"words": "易学易用、高效灵活"
	}, {
		"words": "支持海量图像识别分类/机器翻译/自动驾驶等多个领域"
	}, {
		"words": "现已全面开源, Intel已加入开源贡献"
	}, {
		"words": "Paddlepaddle成为第一个官方支持MKL-DNN的深度学习框架"
	}, {
		"words": "邮件咨询: siebold.com"
	}, {
		"words": "Paddlepaddle开放路线图"
	}, {
		"words": "口防口"
	}],
	"words_result_num": 10,
	"direction": 0
}
```

### 恶心图像识别说明

请求参数：

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数   | 类型   | 是否必须 | 说明                                                  |
| :----- | :----- | :------- | :---------------------------------------------------- |
| log_id | uint64 | 是       | 请求标识码，随机数，唯一                              |
| result | double | 是       | 得分，范围[0.0,1.0]，分值越高代表是恶心图的可能性越高 |

返回值样例：

```
{
    "result": 0.000017522097550682,
    "log_id": 2239381760
}
```

### 广告检测说明

请求参数:

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数         | 类型          | 是否必须 | 说明                                                         |
| :----------- | :------------ | :------- | :----------------------------------------------------------- |
| result_num   | uint32        | 是       | 返回结果数目，即：result数组中元素个数                       |
| result       | array(object) | 是       | 返回结果数组，每一项为一个检测出的结果                       |
| +probability | double        | 是       | 分类结果置信度[0-1.0]                                        |
| +type        | string        | 是       | 返回结果的类型 (watermark水印、bar code条形码、QR code二维码) |
| log_id       | uint64        | 是       | 请求标识码，随机数，唯一                                     |

返回值样例：

```
{
    "log_id": 2959788741,
    "result_num": 1,
    "result": [
        {
            "probability": 0.50555914640427,
            "type": "watermark"
        }
    ]
}
```

\###图文审核说明

正确返回值说明参见：[百度文本审核v2返回说明](http://ai.baidu.com/docs#/TextCensoring-API/6f26464f)

返回值样例：

```
{
    "result": {
        "anti_spam": {
            "log_id": 15452927873875309,
            "words_result": [ "能源烟草行业", "2015年度行业技术杰出贡献奖"],
            "words_number": 8,
            "vertexes_location": [
                [{
                    "x": 253,
                    "y": 88
                }, {
                    "x": 267,
                    "y": 88
                }, {
                    "x": 267,
                    "y": 104
                }, {
                    "x": 253,
                    "y": 104
                }],
            "antispam_info": {
                "score": 0.42614105,
                "description": "恶意推广",
                "label": 4
            },
            "spam": 0,
            "antispam_info_detail": {
                "pass": [{
                    "score": 0.012,
                    "description": "暴恐违禁",
                    "label": 1
                }, {
                    "score": 0.019,
                    "description": "文本色情",
                    "label": 2
                }, {
                    "score": 0.0,
                    "description": "政治敏感",
                    "label": 3
                }, {
                    "score": 0.42614105,
                    "description": "恶意推广",
                    "label": 4
                }, {
                    "score": 0.002,
                    "description": "低俗辱骂",
                    "label": 5
                }, {
                    "score": 0.0,
                    "description": "低质灌水",
                    "label": 6
                }],
                "reject": [],
                "review": []
            },
            "vertexes_location_num": 8
        }
    },
    "log_id": 15452927868301670
}
```

### 图像质量检测说明

请求参数:

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数      | 类型   | 是否必须 | 说明                                       |
| :-------- | :----- | :------- | :----------------------------------------- |
| aesthetic | double | 是       | 美观度分数 范围[0.0,1.0]，分值越高质量越高 |
| clarity   | double | 是       | 清晰度分数 范围[0.0,1.0]，分值越高质量越高 |
| log_id    | uint64 | 是       | 请求标识码，随机数，唯一                   |

返回值样例：

```
    {
        "log_id": 716033439,
        "result": {
            "aesthetic": 0.56124178278668 // 美观度分数 范围[0.0,1.0]
            "clarity": 0.82762561241786 // 清晰度分数 范围[0.0,1.0]
        }
    }
```

### 用文字识别（高精度含位置版）说明

请求参数:

| 参数                  | 类型    | 是否必选 | 可选值范围  | 说明                                                         |
| :-------------------- | :------ | :------- | :---------- | :----------------------------------------------------------- |
| recognize_granularity | string  | 否       | big、small  | 是否定位单字符位置，big：不定位单字符位置，默认值；small：定位单字符位置 |
| detect_direction      | boolean | 否       | true、false | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括: - true：检测朝向； - false：不检测朝向。 |
| vertexes_location     | string  | 否       | true、false | 是否返回文字外接多边形顶点位置，不支持单字位置。默认为false  |
| probability           | string  | 否       | true、false | 是否返回识别结果中每一行的置信度                             |

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 字段               | 是否必选 | 类型    | 说明                                                         |
| :----------------- | :------- | :------ | :----------------------------------------------------------- |
| direction          | 否       | int32   | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id             | 是       | uint64  | 唯一的log id，用于问题定位                                   |
| words_result       | 是       | array() | 定位和识别结果数组                                           |
| words_result_num   | 是       | uint32  | 识别结果数，表示words_result的元素个数                       |
| +vertexes_location | 否       | array() | 当前为四个顶点: 左上，右上，右下，左下。当vertexes_location=true时存在 |
| ++x                | 是       | uint32  | 水平坐标（坐标0点为左上角）                                  |
| ++y                | 是       | uint32  | 垂直坐标（坐标0点为左上角）                                  |
| +location          | 是       | array() | 位置数组（坐标0点为左上角）                                  |
| ++left             | 是       | uint32  | 表示定位位置的长方形左上顶点的水平坐标                       |
| ++top              | 是       | uint32  | 表示定位位置的长方形左上顶点的垂直坐标                       |
| ++width            | 是       | uint32  | 表示定位位置的长方形的宽度                                   |
| ++height           | 是       | uint32  | 表示定位位置的长方形的高度                                   |
| +words             | 否       | string  | 识别结果字符串                                               |
| +chars             | 否       | array() | 单字符结果，recognize_granularity=small时存在                |
| ++location         | 是       | array() | 位置数组（坐标0点为左上角）                                  |
| +++left            | 是       | uint32  | 表示定位位置的长方形左上顶点的水平坐标                       |
| +++top             | 是       | uint32  | 表示定位位置的长方形左上顶点的垂直坐标                       |
| +++width           | 是       | uint32  | 表示定位定位位置的长方形的宽度                               |
| +++height          | 是       | uint32  | 表示位置的长方形的高度                                       |
| ++char             | 是       | string  | 单字符识别结果                                               |
| probability        | 否       | object  | 识别结果中每一行的置信度值，包含average：行置信度平均值，variance：行置信度方差，min：行置信度最小值 |

返回值样例：

```
   {
	"log_id": 651609908832414809,
	"words_result": [{
		"words": "百度A|开放平台·深度学习平台",
		"location": {
			"top": 23,
			"left": 17,
			"width": 280,
			"height": 21
		},
		"chars": [{
			"char": "百",
			"location": {
				"top": 23,
				"left": 17,
				"width": 17,
				"height": 21
			}
		}, {
			"char": "度",
			"location": {
				"top": 23,
				"left": 41,
				"width": 17,
				"height": 21
			}
		}]
	}],
	"words_result_num": 1,
	"direction": 0
}
```

### 通用文字识别（高精度版）说明

请求参数:

| detect_direction | false | boolean | true、false | 是否检测图像朝向，默认不检测，即：false。朝向是指输入图像是正常方向、逆时针旋转90/180/270度。可选值包括:
\- true：检测朝向；
\- false：不检测朝向。 | | probability | false | string | true、false | 是否返回识别结果中每一行的置信度 |

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 字段             | 类型    | 是否必选 | 说明                                                         |
| :--------------- | :------ | :------- | :----------------------------------------------------------- |
| direction        | int32   | 否       | 图像方向，当detect_direction=true时存在。 - -1:未定义， - 0:正向， - 1: 逆时针90度， - 2:逆时针180度， - 3:逆时针270度 |
| log_id           | uint64  | 是       | 唯一的log id，用于问题定位                                   |
| words_result     | array() | 是       | 识别结果数组                                                 |
| words_result_num | uint32  | 是       | 识别结果数，表示words_result的元素个数                       |
| +words           | string  | 否       | 识别结果字符串                                               |
| probability      | object  | 否       | 识别结果中每一行的置信度值，包含average：行置信度平均值，variance：行置信度方差，min：行置信度最小值 |

返回值样例：

```
    {
		"log_id": 2102549997900172625,
		"words_result": [{
			"words": "百度A|开放平台·深度学习平台"
		}],
		"words_result_num": 1
	}
```

# 自定义图像审核接口

## 接口描述

为用户提供色情识别、暴恐识别、政治敏感人物识别、广告识别、图像垃圾文本识别（反作弊）、恶心图像识别等一系列图像识别接口的一站式服务调用，并且**支持用户在控制台中自定义配置所有接口的报警阈值和疑似区间，上传自定义文本黑库和敏感人物名单等**。相比于组合服务接口，本接口除了支持自定义配置外，还对返回结果进行了总体的包装，按照用户在控制台中配置的规则直接返回是否合规，如果不合规则指出具体不合规的内容。

## 请求说明

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/solution/v1/img_censor/user_defined`

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

| 参数   | 类型   | 是否必须 | 说明                                                         |
| :----- | :----- | :------- | :----------------------------------------------------------- |
| image  | string | N        | 待审核图片Base64编码字符串，以图片文件形式请求时必填。不能与imgUrl并存。 |
| imgUrl | string | N        | 网图URL地址，以网图形式请求，图片Url需要做UrlEncode。不能与image并存。 |

**请求示例代码**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

**提示二**：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

```
图像审核接口
curl -i -k 'https://aip.baidubce.com/rest/2.0/solution/v1/img_censor/user_defined?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段           | 类型     | 是否必须 | 说明                                                         |
| :------------- | :------- | :------- | :----------------------------------------------------------- |
| log_id         | Long     | 是       | 请求唯一id                                                   |
| error_code     | uint64   | 否       | 错误提示码，失败才返回，成功不返回                           |
| error_msg      | string   | 否       | 错误提示信息，失败才返回，成功不返回                         |
| conclusion     | string   | 否       | 审核结果描述，成功才返回，失败不返回。可取值 1.合规, 2.不合规, 3.疑似, 4.审核失败 |
| conclusionType | uint64   | 否       | 审核结果标识，成功才返回，失败不返回。可取值1:合规, 2:不合规, 3:疑似, 4:审核失败 |
| data           | object[] | 否       | 审核项详细信息，响应成功并且conclusion为疑似或不合规时才返回，响应失败或conclusion为合规是不返回。 |
| +error_code    | uint64   | 否       | 内层错误提示码，底层服务失败才返回，成功不返回               |
| +error_msg     | string   | 否       | 内层错误提示信息，底层服务失败才返回，成功不返回             |
| +type          | Integer  | 否       | 审核类型，1：色情、2：性感、3：暴恐、4:恶心、5：水印码、6：二维码、7：条形码、8：政治人物、9：敏感词、10：自定义敏感词 |
| +msg           | String   | 否       | 不合规项描述信息                                             |
| +probability   | double   | 否       | 不合规项置信度                                               |
| +stars         | object[] | 否       | 政治人物列表数组，只有政治人物审核不通过才有                 |
| +words         | string   | 否       | 审核不通过敏感词，只有敏感词审核不通过才有                   |

**失败返回示例**

```
{
    "log_id": 149319909347709,
    "error_code": 282800,
    "error_msg":"configId param is error or null"
}
```

**成功返回示例-合规**

```
{
    "log_id": 123456789,
    "conclusion": "合规",
	"conclusionType":1
}
```

**成功返回示例-不合规**

```
{
    "log_id": 123456789,
    "conclusion": "不合规",
	"conclusionType":2,
    "data": [
        {
            "msg": "存在色情内容",
            "probability": 0.94308,
            "type": 1
        },
        {
            "msg": "存在性感内容",
            "probability": 0.94308,
            "type": 2
        },
        {
            "msg": "存在暴恐内容",
            "probability": 0.94308,
            "type": 3
        },
        {
	    "msg": "存在恶心内容",
	    "probability": 0.9688154,
	    "type": 4
	},
        {
            "msg": "存在政治敏感内容",
            "stars": [
                {
                    "probability": 0.94308,
                    "name": "习近平"
                },
                {
                    "probability": 0.44308,
                    "name": "彭丽媛"
                }
            ],
            "type": 8
        },
        {
            "msg": "存在二维码内容",
            "probability": 0.94308,
            "type": 6
        },
        {
            "msg": "存在水印码内容",
            "probability": 0.94308,
            "type": 5
        },
        {
            "msg": "存在条形码内容",
            "probability": 0.94308,
            "type": 7
        },
        {
            "msg": "包含联系方式",
            "probability": 0.94308,
            "words": "包含联系方式",
            "type": 8
        }
    ]
}
```

# GIF色情图像识别

## 接口描述

该请求用于鉴定GIF图片的色情度，对于非gif接口，请使用图像审核组合接口。接口会对图片中每一帧进行识别，并返回所有检测结果中色情值最大的为结果。目前支持三个维度：色情、性感、正常。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/antiporn/v1/detect_gif`

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

| 参数  | 类型   | 是否必须 | 说明                   |
| :---- | :----- | :------- | :--------------------- |
| image | string | 是       | 图像数据，base64编码。 |

**请求示例代码**

请参考色情识别的代码内容。

**访问限制**

| 检查项          | 限制条件              |
| :-------------- | :-------------------- |
| 图片格式        | gif                   |
| 每帧编码后大小  | < 4M                  |
| 帧数            | 不超过50              |
| GIF图片整体大小 | base64编码后不超过20M |

## 返回说明

**返回参数**

| 参数             | 类型          | 是否必选 | 描述                                     |
| :--------------- | :------------ | :------- | :--------------------------------------- |
| log_id           | uint64        | 是       | 请求标识码，随机数，唯一                 |
| frame_count      | uint64        | 是       | gif总帧数                                |
| porn_probability | double        | 是       | 色情识别置信度                           |
| result_num       | Int           | 是       | 返回结果数目，即：result数组中元素个数   |
| result           | Array[Object] | 是       | 结果数组，每项内容对应一个分类维度的结果 |
| +class_name      | String        | 是       | 分类结果名称，示例：色情                 |
| +probability     | double        | 是       | 分类结果置信度，示例：0.89471650123596   |

**返回示例**

```
{
   "frame_count":9,
   "result":[
      {
         "probability":0.006611,
         "class_name":"色情"
      },
      {
         "probability":0.100528,
         "class_name":"性感"
      },
      {
         "probability":0.89286,
         "class_name":"正常"
      }
   ],
   "result_num":3,
   "porn_probability":0.006611,
   "logid":616892616
}
```

# 用户头像审核

## 接口描述

通过人脸检测、文字识别、色情识别、暴恐识别、公众人物检测等多个维度，识别图片是否合规，直接返回审核结果，并支持自定义黑词库和审核规则配置。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/solution/v1/face_audit`

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

| 参数    | 类型   | 是否必须 | 说明                                                         |
| :------ | :----- | :------- | :----------------------------------------------------------- |
| images  | string | N        | 待审核图片Base64编码字符串，以图片文件形式请求时必填。不能与imgUrls并存。 |
| imgUrls | string | N        | 网图URL地址，以网图形式请求，图片Url需要做UrlEncode。不能与images并存。 |

**请求示例代码**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

**提示二**：部分语言依赖的类或库，请在代码注释中查看下载地址。

bash

PHP

Java

Python

C++

C#

```
#!/bin/bash
curl -i -k 'https://aip.baidubce.com/rest/2.0/solution/v1/face_audit?access_token=【调用鉴权接口获取的token】' --data 'configId=1&images=%2f9j%2f4AAQSkZJRgABAQEASABIAAD%2f【图片Base64编码需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段         | 类型     | 是否必须 | 说明                                                         |
| :----------- | :------- | :------- | :----------------------------------------------------------- |
| error_code   | uint32   | 否       | 错误码，错误才返回，成功不返回，具体错误码请参考下文         |
| error_msg    | string   | 否       | 错误提示，错误才返回，成功不返回                             |
| result       | object[] | 否       | 识别结果集合，成功才返回，默认[]                             |
| +res_code    | uint32   | 否       | 业务校验结果 0：校验通过，1：校验不通过                      |
| +res_msg     | object[] | 否       | 未校验通过的项,[]描述的是未校验通过的规则，具体参考 result中的res_msg业务错误码定义 |
| +error_code  | uint32   | 否       | 底层服务调用异常码，正常不返回，异常才返回                   |
| +error_msg   | string   | 否       | 底层服务调用提示信息，正常不返回，异常才返回                 |
| +data        | object   | 否       | 识别详细结果数据，默认[]，data中的key含义： ocr:文字识别 face:人脸检测 public：公众人物 politicians:政治敏感识别 antiporn：色情识别 terror：暴恐识别 quality：图像质量检测 |
| ++ocr        | object   | 否       | 文字识别服务返回结果。请参照“[通用文字识别接口文档](http://ai.baidu.com/docs#/OCR-API/top)” |
| ++face       | object   | 否       | 人脸检测服务返回结果。请参照“[人脸检测接口文档](http://ai.baidu.com/docs#/Face-API/top)” |
| ++antiporn   | object   | 否       | 色情识别返回结果。请参照下文，色情识别说明。                 |
| ++terror     | object   | 否       | 暴恐识别返回结果。请参照本文档 下文，暴恐识别说明。          |
| ++public     | object   | 否       | 公众人物服务返回结果。请参照下文，公众人物识别说明。         |
| ++politician | object   | 否       | 政治敏感识别返回结果。请参照下文，政治敏感识别说明。         |
| ++quality    | object   | 否       | 图像质量返回结果。请参照下文，图像质量识别说明。             |
| log_id       | uint64   | 是       | 请求标识码，随机数，唯一。                                   |

result中的res_msg业务错误码定义：

| 返回码 | 提示信息           | 描述                                          |
| :----- | :----------------- | :-------------------------------------------- |
| 101    | OCR相关            | 文字中包含手机号码                            |
| 102    | OCR相关            | 文字中包含“QQ”号字样，或包含超过9位的连续数字 |
| 103    | OCR相关            | 文字中包含竞品名称                            |
| 104    | OCR相关            | 文字中包含疑似手机号/QQ的连续数字             |
| 201    | face相关           | 图片中不包含人脸                              |
| 202    | face相关           | 图片中包含人脸                                |
| 203    | face相关           | 图片中包含多个人脸（默认为>=2）               |
| 204    | face相关           | 图片中包含明星脸                              |
| 205    | face相关           | 片中包含政治人物人脸                          |
| 206    | face相关           | 片中包含公众人物人脸                          |
| 207    | face相关           | 自定义人脸库识别未通过                        |
| 301    | 黄反识别及审核相关 | 图片中包含色情内容                            |
| 302    | 黄反识别及审核相关 | 图片中包含性感内容，如穿着比较暴露            |
| 401    | 暴恐识别及审核相关 | 图片中包含血腥暴力场景内容                    |
| 501    | 图像美观度相关     | 图像美观度低于阀值                            |
| 502    | 图像美观度相关     | 图像美观度高于阀值                            |
| 503    | 图像美观度相关     | 图像美观度不等于阀值                          |

**失败返回示例**

```
{
    "log_id": 149319909347709, 
    "error_code": 282804,
    "error_msg":"download image error"
}
```

**成功返回示例**

```
{
   "log_id": 149319909347709,
    "result": [
        {
            "res_code": 0,
            "res_msg": [
                2001,
                2002,
                2003
            ],
            "data": {
                "ocr": {
                    "log_id": 2471272194,
                    "words_result_num": 2,
                    "words_result": [
                        {
                            "words": " TSINGTAO"
                        },
                        {
                            "words": "青島睥酒"
                        }
                    ]
                },
                "face": {
                    "result_num": 1,
                    "result": [
                        {
                            "location": {
                                "left": 90,
                                "top": 92,
                                "width": 111,
                                "height": 99
                            },
                            "face_probability": 1,
                            "rotation_angle": 6,
                            "yaw": 11.61234664917,
                            "pitch": -0.30852827429771,
                            "roll": 8.8044967651367,
                            "landmark": [
                                {
                                    "x": 105,
                                    "y": 110
                                }
                            ],
                            "landmark72": [
                                {
                                    "x": 88,
                                    "y": 109
                                }
                            ],
                            "gender": "male",
                            "gender_probability": 0.99358034133911,
                            "glasses": 0,
                            "glasses_probability": 0.99991309642792,
                            "race": "yellow",
                            "race_probability": 0.99960690736771,
                            "qualities": {
                                "occlusion": {
                                    "left_eye": 0.000085282314103097,
                                    "right_eye": 0.00001094374601962,
                                    "nose": 3.2677664307812e-7,
                                    "mouth": 2.6582130940866e-10,
                                    "left_cheek": 8.752236624332e-8,
                                    "right_cheek": 1.0212766454742e-7,
                                    "chin": 4.2632994357028e-10
                                },
                                "blur": 4.5613666312237e-41,
                                "illumination": 0,
                                "completeness": 0,
                                "type": {
                                    "human": 0.98398965597153,
                                    "cartoon": 0.016010366380215
                                }
                            }
                        }
                    ],
                    "log_id": 2418894422
                },
                "antiporn": [
                    {
                        "class_name": "色情",
                        "probability": 0.014619
                    },
                    {
                        "class_name": "正常",
                        "probability": 0.171783
                    },
                    {
                        "class_name": "性感",
                        "probability": 0.813598
                    }
                ]
            }
        }
    ]
}
```

**色情识别识别说明**

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 字段                   | 类型                 | 是否必须 | 说明                                                     |
| :--------------------- | :------------------- | :------- | :------------------------------------------------------- |
| confidence_coefficient | string               | 是       | 结果是否确定，分为“确定”和“不确定”两种。                 |
| result_num             | uint32               | 是       | 返回结果数目，即：result数组中元素个数。                 |
| result                 | array(array(double)) | 是       | 结果数组，每项内容对应一个分类维度的结果。               |
| conclusion             | string               | 是       | 本张图片最终鉴定的结果，分为“色情”，“性感”，“正常”三种。 |
| log_id                 | uint64               | 是       | 请求标识码，随机数，唯一。                               |

其中元素的每项内容包含以下字段：

| 字段        | 类型   | 是否必须 | 说明           | 示例             |
| :---------- | :----- | :------- | :------------- | :--------------- |
| class_name  | string | 是       | 分类结果名称   | 色情             |
| probability | double | 是       | 分类结果置信度 | 0.89471650123596 |

**返回示例**

```
result: [
    {"class_name": "色情", ""probability": 0.014619}，
    {"class_name": "正常", ""probability": 0.171783}，
    {"class_name": "性感", ""probability": 0.813598}
    ]
```

**暴恐识别说明**

请求参数:

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 字段          | 类型                 | 是否必须 | 说明                                                         |
| :------------ | :------------------- | :------- | :----------------------------------------------------------- |
| result        | array(array(double)) | 是       | 暴恐置信度得分。                                             |
| log_id        | uint64               | 是       | 请求标识码，随机数，唯一。                                   |
| result_coarse | object[]             | 是       | 粗粒度得分结果                                               |
| name          | string               | 是       | 粗粒度标签，包含两个标签：正常、暴恐                         |
| score         | float                | 是       | 对应标签的置信度得分，越高可信度越高                         |
| result_fine   | object[ ]            | 是       | 细粒度得分结果                                               |
| name          | string               | 是       | 细粒度标签，包含9个标签：正常、警察部队、血腥、尸体、爆炸火灾、杀人、暴乱、暴恐人物、军事武器 |
| score         | float                | 是       | 对应标签的置信度得分，越高可信度越高                         |

**返回示例**

```
{
	"errno": 0,
	"msg": "success",
	"data": {
		"result": 0.0082325544208288,
		"result_coarse": [
			{
				"name": "正常",
				"score": 0.99176746606827
			},
			{
				"name": "暴恐",
				"score": 0.0082325544208288
			}
		],
		"result_fine": [
			{
				"name": "正常",
				"score": 0.98908758163452
			},
			{
				"name": "警察部队",
				"score": 0.0062405453063548
			},
			{
				"name": "血腥",
				"score": 0.0009653537417762
			},
			{
				"name": "尸体",
				"score": 0.001054480439052
			},
			{
				"name": "爆炸火灾",
				"score": 0.00011743687355192
			},
			{
				"name": "杀人",
				"score": 0.0011699661845341
			},
			{
				"name": "暴乱",
				"score": 0.000021190358893364
			},
			{
				"name": "暴恐人物",
				"score": 0.0010401027975604
			},
			{
				"name": "军事武器",
				"score": 0.00030337597127073
			}
		]
	}
}
```

**政治敏感识别说明**

请求参数:

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数               | 类型          | 是否必须 | 说明                                                       |
| :----------------- | :------------ | :------- | :--------------------------------------------------------- |
| include_politician | string        | 是       | 是否包含政治人物，取值为”是”或”否”                         |
| result_confidence  | string        | 是       | 对于include_politician字段的置信度，取值为“确定”或“不确定” |
| result_num         | uint32        | 是       | 实际检测到人脸数目(不大于max_face_num)                     |
| result             | array(object) | 是       | 识别出的结果数据                                           |
| +location          | object        | 是       | 人脸在输入图片中的位置                                     |
| ++left             | uint32        | 是       | 人脸区域离左边界的距离                                     |
| ++top              | uint32        | 是       | 人脸区域离上边界的距离                                     |
| ++width            | uint32        | 是       | 人脸区域的宽度                                             |
| ++height           | uint32        | 是       | 人脸区域的高度                                             |
| +stars             | object[]      | 是       | 政治人物数组                                               |
| ++name             | string        | 是       | 姓名                                                       |
| ++star_id          | string        | 是       | 人物id，全局唯一                                           |
| ++probability      | float         | 是       | 相似度，[0, 1]                                             |
| log_id             | uint64        | 是       | 请求标识码，随机数，唯一                                   |

返回值样例：

```
    {
        "log_id": 3268660173,
        "include_politician": "是",
        "result_confidence": "确定",
        "result_num": 1,
        "result": [
            {
                "location": {
                    "left": 132,
                    "top": 168,
                    "width": 238,
                    "height": 223
                },
                "stars": [
                    {
                        "name": "习近平",
                        "star_id": "515617",
                        "probability": 0.9750030040741
                    }
                ]
            }
        ]
    }
```

**公众人物识别说明**

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数          | 类型     | 是否必须 | 说明                                   |
| :------------ | :------- | :------- | :------------------------------------- |
| result_num    | uint32   | 是       | 实际检测到人脸数目(不大于max_face_num) |
| result        | object[] | 是       | 识别出的结果数据                       |
| +location     | object   | 是       | 人脸在输入图片中的位置                 |
| ++left        | uint32   | 是       | 人脸区域离左边界的距离                 |
| ++top         | uint32   | 是       | 人脸区域离上边界的距离                 |
| ++width       | uint32   | 是       | 人脸区域的宽度                         |
| ++height      | uint32   | 是       | 人脸区域的高度                         |
| +stars        | object[] | 是       | 公众人物数组                           |
| ++name        | string   | 是       | 姓名                                   |
| ++star_id     | string   | 是       | 人物id，全局唯一                       |
| ++probability | float    | 是       | 相似度，[0, 1]                         |
| log_id        | uint64   | 是       | 请求标识码，随机数，唯一               |

返回值样例：

```
{
    "log_id": 3268660173,
    "result_num": 1,
    "result": [
        {
            "location": {
                "left": 132,
                "top": 168,
                "width": 238,
                "height": 223
            },
            "stars": [
                {
                    "name": "习近平",
                    "star_id": "515617",
                    "probability": 0.9750030040741
                }
            ]
        }
    ]
}
```

**图像质量检测说明**

请求参数:

```
无额外参数
```

错误返回值：

| 参数       | 类型   | 必须 | 说明                                                 |
| :--------- | :----- | :--- | :--------------------------------------------------- |
| error_code | uint32 | 是   | 错误码，参考错误码表错误码说明。只在异常响应中出现   |
| error_msg  | string | 是   | 错误信息，参考错误码表错误码说明。只在异常响应中出现 |
| log_id     | uint64 | 是   | 请求标识码，随机数，唯一                             |

正确返回值：

| 参数      | 类型   | 是否必须 | 说明                     |
| :-------- | :----- | :------- | :----------------------- |
| aesthetic | double | 是       | 美观度分数 范围[0.0,1.0] |
| clarity   | double | 是       | 清晰度分数 范围[0.0,1.0] |
| log_id    | uint64 | 是       | 请求标识码，随机数，唯一 |

返回值样例：

```
    {
        "log_id": 716033439,
        "result": {
            "aesthetic": 0.56124178278668 // 美观度分数 范围[0.0,1.0]，分值越高代表美观度越高
            "clarity": 0.82762561241786 // 清晰度分数 范围[0.0,1.0]，分值越高代表清晰度越高
        }
    }
```

# 短视频审核接口

## 接口描述

【请点击[申请邀测](https://cloud.baidu.com/survey/AICooperativeConsultingApply.html)提交需求申请开通该接口使用权限】针对属于同一个短视频的图片集进行审核并针对该短视频返回结果，用户可以指定具体的审核维度

## 请求说明

HTTP 方法：`POST` 请求URL：

```
https://aip.baidubce.com/rest/2.0/solution/v1/video_censor/user_defined
```

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

### 请求参数

请求为Json格式，可以通过重传图片内容/图片md5/请求log_id，进行反馈。

| 参数名称 | 数据类型 | 是否必须 | 备注                                                         | 样例                                                         |
| :------- | :------- | :------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| appid    | long     | Y        | 用户在console的AppId                                         | 11765843                                                     |
| scenes   | array    | Y        | 用户计划调用的底层服务，以字符串表示， 不同元素以英文逗号分隔。元素含义如下： antiporn：色情识别 terror：暴恐识别 disgust:恶心图 watermark:水印、二维码 politician：政治人物识别 public：公众人物识别 logo:logo识别 | antiporn, terror, politician,public,watermark,disgust,logo   |
| imgUrls  | string   | N        | 网图URL地址列表，图片Url需要做UrlEncode， 多图以英文逗号分隔，一次最多传递30个图片地址 | http://face-slk.bj.bcebos.com/jiangzemin.jpg,http://mybuketslk.gz.bcebos.com/demo-card-4.jpg,http://mybuketslk.gz.bcebos.com/terror.jpg,http://mybuketslk.gz.bcebos.com/timg.jpg,http://mybuketslk.gz.bcebos.com/anti1.png,http://face-slk.bj.bcebos.com/bk.jpg |
| extTag   | string   | Y        | 用户地自定义审核标识,用户根据此字段 关联审核结果例如视频ID，或主播ID | {"vid":1}                                                    |

### 响应参数说明：

| 参数名称          | 数据类型 | 是否必须 | 备注                                                         | 样例      |
| :---------------- | :------- | :------- | :----------------------------------------------------------- | :-------- |
| log_id            | long     | Y        | 请求唯一id                                                   |           |
| error_code        | long     | N        | 错误提示码，失败才返回，成功不返回                           |           |
| error_msg         | string   | N        | 错误提示信息，失败才返回，成功不返回                         |           |
| conclusion        | string   | N        | 最终审核结果描述，成功才返回，失败不返回。 可取值：合规，不合规，疑似，审核失败 |           |
| conclusionType    | uint64   | N        | 最终审核结果标识，成功才返回，失败不返回。 可取值1：合规,2：不合规，3：疑似，4：审核失败 |           |
| extTag            | string   | Y        | 用户地自定义审核标识,用户根据此字段关联 审核结果例如视频ID，或主播ID | {"vid":1} |
| result            | object   | N        | 审核归类统计详细信息，响应成功并且 conclusion为疑似、不合规、审核失败时才返回， 审核合规不返回 |           |
| +antiporn         | object   | N        | 黄反归类结果                                                 |           |
| +terror           | object   | N        | 暴恐归类结果                                                 |           |
| +disgust          | object   | N        | 恶心图归类结果                                               |           |
| +watermark        | object   | N        | 水印归类结果                                                 |           |
| +politician       | object   | N        | 政治敏感归类结果                                             |           |
| +public           | object   | N        | 公众人物归类结果                                             |           |
| +logo             | object   | N        | logo识别归类结果                                             |           |
| ++conclusion      | string   | N        | 单项审核结果描述，成功才返回，失败不返回。 可取值：合规，不合规，疑似，审核失败 |           |
| ++conclusionType  | uint64   | N        | 单项审核结果标识，成功才返回，失败不返回。 可取值1：合规,2：不合规，3：疑似，4：审核失败 |           |
| ++maxClassName    | string   | N        | 不合规分类名称                                               |           |
| ++count           | uint64   | N        | 不合规或疑似图片数                                           |           |
| ++maxUrl          | string   | N        | 置信度最高的url                                              |           |
| ++urls            | array    | N        | 不合规或疑似url集合                                          |           |
| ++maxProbability  | float    | N        | 不合规项最高置信度                                           |           |
| ++watermarks      | array    | N        | 广告监测特定结果                                             |           |
| +++maxClassName   | string   | N        | 广告监测分类，按照三分类返会watermark： 水印、QR code：二维码、bar code：条形码 |           |
| +++maxUrl         | string   | N        | 置信度最高的url                                              |           |
| +++urls           | string   | N        | 广告监测分类url列表                                          |           |
| +++maxProbability | String   | N        | 图片列表检测到的最高置信度                                   |           |
| +++count          | uint64   | N        | 此类图片出现的图片数                                         |           |
| ++stars           | array    | N        | 公众人物、政治人物特定结果、返回人物列表                     |           |
| +++name           | string   | N        | 人名                                                         |           |
| +++maxUrl         | string   | N        | 置信度最高的url                                              |           |
| +++urls           | string   | N        | 人物url列表                                                  |           |
| +++maxProbability | String   | N        | 人物最高置信度                                               |           |
| +++count          | uint64   | N        | 此人物出现的图片数                                           |           |
| ++logos           | array    | N        | logo识别特定结果、返回logo列表                               |           |
| +++name           | string   | N        | logo名                                                       |           |
| +++maxUrl         | string   | N        | 置信度最高的url                                              |           |
| +++urls           | string   | N        | 此logo的url列表                                              |           |
| +++maxProbability | String   | N        | 此logo最高置信度                                             |           |
| +++count          | uint64   | N        | 此logo出现的图片数                                           |           |

#### 成功响应示例 ——合规：

```
{
	"log_id": 153112445889003,
	"conclusion": "合规",
	"conclusionType": 1,
	"extTag": "{\"vid:1\"}"
}
```

#### 成功响应示例——不合规：

```
{
	"log_id": 153112578882604,
	"conclusion": "不合规",
	"conclusionType": 2,
	"extTag": "{\"vid:1\"}",
	"result": {
		"antiporn": {
			"conclusion": "疑似",
			"conclusionType": 3,
			"maxClassName": "一般色情",
			"urls": ["http://mybuketslk.gz.bcebos.com/anti1.png"],
			"maxUrl": "http://mybuketslk.gz.bcebos.com/anti1.png",
			"maxProbability": 1.0,
			"count": 1
		},
		"terror": {
			"conclusion": "不合规",
			"conclusionType": 2,
			"maxClassName": "暴恐人物",
			"urls": ["http://face-slk.bj.bcebos.com/bk.jpg"],
			"maxUrl": "http://face-slk.bj.bcebos.com/bk.jpg",
			"maxProbability": 0.95554465,
			"count": 1
		},
		"watermark": {
			"conclusion": "疑似",
			"conclusionType": 3,
			"maxClassName": "watermark",
			"watermarks": [{
				"maxClassName": "watermark",
				"maxProbability": 0.5,
				"maxUrl": "http://face-slk.bj.bcebos.com/bk.jpg",
				"urls": ["http://face-slk.bj.bcebos.com/bk.jpg", "http://face-slk.bj.bcebos.com/jiangzemin.jpg", "http://mybuketslk.gz.bcebos.com/timg.jpg", "http://mybuketslk.gz.bcebos.com/timg.jpg"],
				"count": 4
			}, {
				"maxClassName": "QR code",
				"maxProbability": 0.5,
				"maxUrl": "http://mybuketslk.gz.bcebos.com/demo-card-4.jpg",
				"urls": ["http://mybuketslk.gz.bcebos.com/demo-card-4.jpg"],
				"count": 1
			}]
		},
		"public": {
			"conclusion": "不合规",
			"conclusionType": 2,
			"maxClassName": "杨幂",
			"stars": [{
				"name": "杨幂",
				"maxProbability": 0.97526217,
				"maxUrl": "http://face-slk.bj.bcebos.com/liyanhong2.jpg",
				"urls": ["http://face-slk.bj.bcebos.com/liyanhong2.jpg"],
				"count": 1
			}, {
				"name": "江泽民",
				"maxProbability": 0.8811051,
				"maxUrl": "http://face-slk.bj.bcebos.com/jiangzemin.jpg",
				"urls": ["http://face-slk.bj.bcebos.com/jiangzemin.jpg"],
				"count": 1
			}, {
				"name": "川岛和津实",
				"maxProbability": 0.7025537,
				"maxUrl": "http://mybuketslk.gz.bcebos.com/anti1.png",
				"urls": ["http://mybuketslk.gz.bcebos.com/anti1.png"],
				"count": 1
			}]
		},
		"politician": {
			"conclusion": "不合规",
			"conclusionType": 2,
			"maxClassName": "江泽民",
			"stars": [{
				"count": 1,
				"maxProbability": 0.88149726,
				"maxUrl": "http://face-slk.bj.bcebos.com/jiangzemin.jpg",
				"name": "江泽民",
				"urls": ["http://face-slk.bj.bcebos.com/jiangzemin.jpg"]
			}]
		},
		"logo": {
			"conclusion": "不合规",
			"conclusionType": 2,
			"maxClassName": "百度",
			"logos": [{
				"maxProbability": 0.9999672,
				"maxUrl": "http://mybuketslk.gz.bcebos.com/timg.jpg",
				"name": "百度",
				"urls": ["http://mybuketslk.gz.bcebos.com/timg.jpg", "http://mybuketslk.gz.bcebos.com/timg.jpg"],
				"count": 2
			}, {
				"maxProbability": 0.7416412,
				"maxUrl": "http://mybuketslk.gz.bcebos.com/timg.jpg",
				"name": "dianping",
				"urls": ["http://mybuketslk.gz.bcebos.com/timg.jpg"],
				"count": 1
			}]
		},
		"disgust": {
			"conclusion": "不合规",
			"conclusionType": 2,
			"maxClassName": "恶心图",
			"urls": ["http://mybuketslk.gz.bcebos.com/terror.jpg"],
			"maxUrl": "http://mybuketslk.gz.bcebos.com/terror.jpg",
			"maxProbability": 0.99577975,
			"count": 1
		}
	}
}
```

#### 成功响应示例 ——疑似：

```
{
	"log_id": 153112898732207,
	"conclusion": "疑似",
	"conclusionType": 3,
	"extTag": "{\"vid:1\"}",
	"result": {
		"antiporn": {
			"conclusion": "疑似",
			"conclusionType": 3,
			"maxClassName": "一般色情",
			"urls": ["http://mybuketslk.gz.bcebos.com/anti1.png"],
			"maxUrl": "http://mybuketslk.gz.bcebos.com/anti1.png",
			"maxProbability": 1.0,
			"count": 1
		},
		"terror": {
			"conclusion": "合规",
			"conclusionType": 1
		},
		"watermark": {
			"conclusion": "疑似",
			"conclusionType": 3,
			"maxClassName": "QR code",
			"watermarks": [{
				"count": 1,
				"maxClassName": "QR code",
				"maxProbability": 0.5,
				"maxUrl": "http://mybuketslk.gz.bcebos.com/demo-card-4.jpg",
				"urls": ["http://mybuketslk.gz.bcebos.com/demo-card-4.jpg"]
			}]
		},
		"public": {
			"conclusion": "疑似",
			"conclusionType": 3,
			"maxClassName": "川岛和津实",
			"stars": [{
				"count": 1,
				"maxProbability": 0.700683,
				"maxUrl": "http://mybuketslk.gz.bcebos.com/anti1.png",
				"name": "川岛和津实",
				"urls": ["http://mybuketslk.gz.bcebos.com/anti1.png"]
			}]
		},
		"politician": {
			"conclusion": "合规",
			"conclusionType": 1
		},
		"logo": {
			"conclusion": "合规",
			"conclusionType": 1
		},
		"disgust": {
			"conclusion": "合规",
			"conclusionType": 1
		}
	}
}
```

#### 失败响应示例：

```
{
    "log_id": 149319909347709, 
    "error_code": 282000,
    "error_msg":"logic internal error"
}
```

# 图像审核反馈接口

## 接口描述

用户可以使用该接口对图像审核的结果进行反馈，提交badcase

## 请求说明

HTTP 方法：`POST` 请求URL：

```
https://aip.baidubce.com/rpc/2.0/feedback/v1/report
```

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body中放置请求参数，参数详情如下：

**参数解释**

请求为Json格式，可以通过重传图片内容/图片md5/请求log_id，进行反馈。

| 参数名    | 取值               | 是否必选                                | 说明                                                         |
| :-------- | :----------------- | :-------------------------------------- | :----------------------------------------------------------- |
| api_url   | 与数据库中取值一致 | Y                                       | 所调用的API                                                  |
| correct   | int                | Y                                       | 结果是否正确，正确：1，错误：0                               |
| image     | string             | N(image/image_md5/log_id三选一)         | 图像BASE64编码数据                                           |
| image_md5 | string             | N(image/image_md5/log_id三选一)         | 原请求的图像的MD5值                                          |
| log_id    | long               | N(image/image_md5/log_id三选一)         | 原请求返回的log_id                                           |
| level     | int                | N（该字段仅适用于色情识别结果反馈）     | 填写您认为的正确结果，色情：1，性感：2，正常：3，难以判断：4 |
| watermark | int                | N（该字段仅适用于广告检测结果反馈）     | 不包含水印：0， 包含：1                                      |
| qr_code   | int                | N（该字段仅适用于广告检测结果反馈）     | 不包含二维码：0， 包含：1                                    |
| bar_code  | int                | N（该字段仅适用于广告检测结果反馈）     | 不包含条形码：0， 包含：1                                    |
| clarity   | int                | N（该字段仅适用于图像质量检测结果反馈） | 0: 不清晰, 1: 清晰                                           |
| aesthetic | int                | N（该字段仅适用于图像质量检测结果反馈） | 0: 不美观，1：美观                                           |
| extra     | string             | N                                       | 备注，扩展字段                                               |

**请求示例**

```
{
  "feedback": [
    {
      "api_url": "https://aip.baidubce.com/rest/2.0/antiporn/v1/detect",
      // api请求返回的log_id 
      "image_logid": 123456, 
      "level": 1,
      "correct": 1
    },
    {
      "api_url": "https://aip.baidubce.com/rest/2.0/antiporn/v1/detect",
      // api调用的image base64
      "image": "BASE64后的内容", 
      "level": 1,
      "correct": 0
    },
    {
      "api_url": "https://aip.baidubce.com/rest/2.0/antiporn/v1/detect",
      // api调用的image md5
      "image_md5": "MD5字符串", 
      "level": 1,
      "correct": 1
    }
  ]
}
```

## 返回说明

**返回示例**

```
{
  "log_id": 234232235,
  "result": [
    {
      "error_code": 0,
      //成功
      "error_msg": ""
    },
    {
      "error_code": 282004,
      // image_logid 或者 image_md5 或者 image 不存在 
      // 三者必选其一
      "error_msg": "params error" 
    },
    {
      "error_code": 0,
      "error_msg": ""
    }
  ]
}
```

# 错误码

若请求错误，服务器将返回的JSON文本包含以下参数：

- **error_code**：错误码。
- **error_msg**：错误描述信息，帮助理解和解决发生的错误。

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
| 1      | Unknown error                           | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（375765194）或工单联系技术支持团队。 |
| 2      | Service temporarily unavailable         | 服务暂不可用，请再次请求， 如果持续出现此类错误，请通过QQ群（375765194）或工单联系技术支持团队。 |
| 3      | Unsupported openapi method              | 调用的API不存在，请检查后重新尝试                            |
| 4      | Open api request limit reached          | 集群超限额                                                   |
| 6      | No permission to access data            | 无权限访问该用户数据                                         |
| 17     | Open api daily request limit reached    | 每天流量超限额                                               |
| 18     | Open api qps request limit reached      | QPS超限额                                                    |
| 19     | Open api total request limit reached    | 请求总量超限额                                               |
| 100    | Invalid parameter                       | 无效参数                                                     |
| 110    | Access token invalid or no longer valid | Access Token失效                                             |
| 111    | Access token expired                    | Access token过期                                             |
| 216015 | module closed                           | 模块关闭                                                     |
| 216100 | invalid param                           | 非法参数                                                     |
| 216101 | not enough param                        | 参数数量不够                                                 |
| 216102 | service not support                     | 业务不支持                                                   |
| 216103 | param too long                          | 参数太长                                                     |
| 216110 | appid not exist                         | APP ID不存在                                                 |
| 216111 | invalid userid                          | 非法用户ID                                                   |
| 216200 | empty image                             | 空的图片                                                     |
| 216201 | image format error                      | 图片格式错误                                                 |
| 216202 | image size error                        | 图片大小错误                                                 |
| 216300 | db error                                | DB错误                                                       |
| 216400 | backend error                           | 后端系统错误                                                 |
| 216401 | internal error                          | 内部错误                                                     |
| 216500 | unknown error                           | 未知错误                                                     |
| 282000 | logic internal error                    | 业务逻辑层内部错误                                           |
| 282001 | logic backend error                     | 业务逻辑层后端服务错误                                       |
| 282002 | input encoding error                    | 请求参数编码错误                                             |
| 282100 | image transcode error                   | 图片压缩转码错误                                             |
| 282202 | antiporn detect timeout                 | 检测超时                                                     |
| 282203 | image frame size error                  | gif单帧大小超限                                              |
| 282204 | image frames limit error                | gif总帧数超限                                                |
| 282205 | image fromat must gif                   | 图片格式错误                                                 |
| 282800 | configId param is error or null         | 配置Id不存在                                                 |
| 282801 | image and imgUrl are empty              | image和imgUrl均为空                                          |
| 282802 | either image and imgUrl has value       | image和imgUrl只能有一个有值                                  |
| 282804 | download image error                    | 图片下载失败                                                 |
| 282805 | internal request failed                 | 调用底层服务异常                                             |
| 282806 | image length or width error             | 图片宽高异常                                                 |
| 282872 | too many img urls max num is 30         | 图片链接过多                                                 |
| 282873 | some img urls invalid                   | 部分url异常                                                  |