# 图像识别 API参考

# 简介

Hi，您好，欢迎使用百度图像识别API服务。

本文档主要针对API开发者，描述百度图像识别接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内[提交工单](http://ticket.bce.baidu.com/#/ticket/create)，咨询问题类型请选择**人工智能服务**
- 如有疑问，进入**[AI社区交流](http://ai.baidu.com/forum/topic/list/171) **:<http://ai.baidu.com/forum/topic/list/171>
- 具有免费调用额度的接口，超过每天的免费额度后会返回错误码：17，错误信息：Open api daily request limit reached（每天流量超限额），**未上线计费的接口，可通过QQ群（659268104）联系群管、或提交工单手动提额**；已上线计费的接口，请直接在控制台开通计费，调用量不受限制，按调用量阶梯计费

## 接口能力

| 产品功能       | 接口名称                 | 接口能力简要描述                                             |
| :------------- | :----------------------- | :----------------------------------------------------------- |
| 通用图像分析   | 图像主体检测             | 识别图像中的主体具体坐标位置。                               |
|                | 通用物体和场景识别高级版 | 识别图片中的场景及物体标签，支持8w+标签类型。                |
| 细粒度图像识别 | 菜品识别                 | 检测用户上传的菜品图片，返回具体的菜名、卡路里、置信度信息。 |
|                | 自定义菜品识别           | 入库自定义的单菜品图，实现上传多菜品图的精准识别，返回具体的菜名、位置、置信度信息 |
|                | logo商标识别             | 识别图片中包含的商品LOGO信息，返回LOGO品牌名称、在图片中的位置、置信度。 |
|                | 动物识别                 | 检测用户上传的动物图片，返回动物名称、置信度信息。           |
|                | 植物识别                 | 检测用户上传的植物图片，返回植物名称、置信度信息。           |
|                | 花卉识别                 | 检测用户上传的花卉图片，返回花卉名称、置信度信息。           |
|                | 果蔬食材识别             | 检测用户上传的果蔬类图片，返回果蔬名称、置信度信息。         |
|                | 地标识别                 | 检测用户上传的地标图片，返回地标名称。                       |
| 车辆分析       | 车型识别                 | 检测用户上传的车辆图片，识别所属车型，包括车辆品牌及具体型号、颜色、年份、位置信息。 |
|                | 车辆检测                 | 检测图片中出现的所有车辆，返回车辆类型与位置。               |
|                | 车流统计                 | 根据视频抓拍图像序列，返回指定区域进出车流量（按车辆类型）。 |
|                | 车辆属性识别             | 检测图像中的各类车辆，并针对小汽车识别11种外观属性，包括：是否有车窗雨眉、是否有车顶架、副驾驶是否有人等。 |

## 请求格式

POST方式调用

**注意**：Content-Type为`application/x-www-form-urlencoded`，然后通过`urlencode`格式化请求体。

## 返回格式

JSON格式

## 请求限制

请求图片需经过`base64编码`：图片的base64编码指将一副图片数据编码成一串字符串，使用该字符串代替图像地址。您可以首先得到图片的二进制，然后用Base64格式编码即可。

**注意**：图片的base64编码是不包含图片头的，如`（data:image/jpg;base64,）`

**请求格式支持**：PNG、JPG、JPEG、BMP、GIF**

| 接口名称           | 图片编码后大小限额                                           |
| :----------------- | :----------------------------------------------------------- |
| 通用物体和场景识别 | base64编码后大小不超过4M，最短边至少30px，最长边最大4096px，长宽比3：1以内 |
| 菜品识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 自定义菜品识别     | base64编码后大小不超过4M，最短边至少300px，最长边最大3000px，长宽比3：1以内 |
| logo商标识别       | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 图像主体检测       | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 动物识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 植物识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 花卉识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 果蔬食材识别       | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 地标识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 车型识别           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 车辆检测           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 车流统计           | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |
| 车辆属性识别       | base64编码后大小不超过4M，最短边至少15px，最长边最大4096px，长宽比3：1以内 |

# 调用方式

调用AI服务相关的API接口有两种调用方式，两种不同的调用方式采用相同的接口URL。

区别在于**请求方式**和**鉴权方法**不一样，请求参数和返回结果一致。

## 调用方式一

**请求URL数据格式**

向API服务地址使用POST发送请求，必须在URL中带上参数：

**access_token:** 必须参数，参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)”。

> 注意：`access_token`的有效期为30天，需要每30天进行定期更换；

POST中参数按照API接口说明调用即可。

例如菜品识别API，使用HTTPS POST发送：

```
https://aip.baidubce.com/rest/2.0/image-classify/v2/dish?access_token=24.f9ba9c5241b67688bb4adbed8bc91dec.2592000.1485570332.282335-8574074
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

> **说明**：方式二鉴权使用的[API认证机制](https://cloud.baidu.com/doc/Reference/AuthenticationMechanism.html)authorization必须通过百度云的[AK/SK](https://cloud.baidu.com/doc/Reference/GetAKSK.html)生成。

# 通用图像分析——通用物体和场景识别高级版

## 接口描述

该请求用于通用物体及场景识别，即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片中的多个物体及场景标签。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v2/advanced_general`

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

| 参数      | 是否必选 | 类型    | 可选值范围 | 说明                                                         |
| :-------- | :------- | :------ | :--------- | :----------------------------------------------------------- |
| image     | true     | string  | -          | Base64编码字符串，以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png，jpeg)，图片大小不超过4M。最短边至少15px，最长边最大4096px。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| baike_num | 否       | integer | 0          | 返回百科信息的结果数，默认不返回                             |

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
通用物体和场景识别
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v2/advanced_general?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段          | 是否必选 | 类型          | 说明                                                      |
| :------------ | :------- | :------------ | :-------------------------------------------------------- |
| log_id        | 是       | uint64        | 唯一的log id，用于问题定位                                |
| result_num    | 是       | unit32        | 返回结果数目，及result数组中的元素个数                    |
| result        | 是       | arrry(object) | 标签结果数组                                              |
| +keyword      | 是       | string        | 图片中的物体或场景名称                                    |
| +score        | 是       | float         | 置信度，0-1                                               |
| +root         | 是       | string        | 识别结果的上层标签，有部分钱币、动漫、烟酒等tag无上层标签 |
| +baike_info   | object   | 否            | 对应识别结果的百科词条名称                                |
| ++baike_url   | string   | 否            | 对应识别结果百度百科页面链接                              |
| ++image_url   | string   | 否            | 对应识别结果百科图片链接                                  |
| ++description | string   | 否            | 对应识别结果百科内容描述                                  |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
	"log_id": 327863200205075661,
	"result_num": 5,
	"result": [{
		"score": 0.967622,
		"root": "公众人物",
		"baike_info": {
			"baike_url": "http://baike.baidu.com/item/%E6%96%B0%E5%9E%A3%E7%BB%93%E8%A1%A3/8035884",
			"image_url": "http://imgsrc.baidu.com/baike/pic/item/91ef76c6a7efce1b27893518a451f3deb58f6546.jpg",
			"description": "新垣结衣(Aragaki Yui)，1988年6月11日出生于冲绳县那霸市。日本女演员、歌手、模特。毕业于日出高中。2001年，参加《nicola》模特比赛并获得最优秀奖。2005年，因出演现代剧《涩谷15》而作为演员出道。2006年，参演校园剧《我的老大，我的英雄》；同年，她还出版了个人首本写真集《水漾青春》。2007年，她从日出高校毕业后开始专注于演艺发展，并发表个人首张音乐专辑《天空》；同年，新垣结衣还主演了爱情片《恋空》，而她也凭借该片获得了多个电影新人奖项。2010年，主演爱情片《花水木》。2011年，主演都市剧《全开女孩》。2012年，相继参演现代剧《Legal High》、剧情片《剧场版新参者：麒麟之翼》。2013年，主演都市剧《飞翔情报室》。2014年，她主演了剧情片《黎明的沙耶》。2016年，主演爱情喜剧《逃避虽可耻但有用》，并凭借该剧获得了多个电视剧女主角奖项。2017年，主演爱情片《恋爱回旋》，凭借该片获得第60届蓝丝带奖最佳女主角；同年11月，她还凭借医疗剧《Code Blue 3》获得第94届日剧学院赏最佳女配角。"
		},
		"keyword": "新垣结衣"
	},
	{
		"score": 0.716067,
		"root": "人物-人物特写",
		"keyword": "头发"
	},
	{
		"score": 0.421281,
		"root": "商品-穿戴",
		"keyword": "围巾"
	},
	{
		"score": 0.22347,
		"root": "商品-五金",
		"keyword": "拉链"
	},
	{
		"score": 0.028031,
		"root": "商品-穿戴",
		"keyword": "脖套"
	}]
}
```

# 通用图像分析——图像主体检测

## 接口描述

用户向服务请求检测图像中的主体位置。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/object_detect`

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

| 参数      | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :-------- | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image     | true     | string | -          | 图像数据，base64编码，要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式 。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| with_face | false    | number | -          | 如果检测主体是人，主体区域是否带上人脸部分，0-不带人脸区域，其他-带人脸区域，裁剪类需求推荐带人脸，检索/识别类需求推荐不带人脸。默认取1，带人脸。 |

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
图像主体检测
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/object_detect?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&with_face=1' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段    | 是否必选 | 类型               | 说明                                   |
| :------ | :------- | :----------------- | :------------------------------------- |
| log_id  | 是       | uint64             | 唯一的log id，用于问题定位             |
| result  | 否       | watermark-location | 裁剪结果                               |
| +left   | 否       | uint32             | 表示定位位置的长方形左上顶点的水平坐标 |
| +top    | 否       | uint32             | 表示定位位置的长方形左上顶点的垂直坐标 |
| +width  | 否       | uint32             | 表示定位位置的长方形的宽度             |
| +height | 否       | uint32             | 表示定位位置的长方形的高度             |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
  "log_id": 895582300,
  "result": {
    "width": 486,
    "top": 76,
    "left": 134,
    "height": 394
  }
}
```

# 细粒度图像识别——菜品识别

## 接口描述

该请求用于菜品识别。即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片的菜品名称、卡路里信息、置信度。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v2/dish`

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

| 参数             | 是否必选 | 类型    | 可选值范围 | 说明                                                         |
| :--------------- | :------- | :------ | :--------- | :----------------------------------------------------------- |
| image            | 是       | string  | -          | Base64编码字符串，以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png，jpeg)，图片大小不超过4M。最短边至少15px，最长边最大4096px。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| top_num          | 否       | unit32  | -          | 返回结果top n,默认5.                                         |
| filter_threshold | 是       | float   | -          | 默认0.95，可以通过该参数调节识别效果，降低非菜识别率.        |
| baike_num        | 否       | integer | 0          | 返回百科信息的结果数，默认不返回                             |

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
菜品识别
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v2/dish?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&top_num=5' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段          | 是否必选 | 类型    | 说明                                   |
| :------------ | :------- | :------ | :------------------------------------- |
| log_id        | 是       | uint64  | 唯一的log id，用于问题定位             |
| result_num    | 否       | unit32  | 返回结果数目，及result数组中的元素个数 |
| result        | 否       | array() | 菜品识别结果数组                       |
| +name         | 否       | string  | 菜名，示例：鱼香肉丝                   |
| +calorie      | 否       | float   | 卡路里，每100g的卡路里含量             |
| +probability  | 否       | float   | 识别结果中每一行的置信度值，0-1        |
| +baike_info   | object   | 否      | 对应识别结果的百科词条名称             |
| ++baike_url   | string   | 否      | 对应识别结果百度百科页面链接           |
| ++image_url   | string   | 否      | 对应识别结果百科图片链接               |
| ++description | string   | 否      | 对应识别结果百科内容描述               |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
	"log_id": 7357081719365269362,
	"result_num": 5,
	"result": [
	{
		"calorie": "119",
		"has_calorie": true,
		"name": "酸汤鱼",
		"probability": "0.396031"
		"baike_info": {
			"baike_url": "http://baike.baidu.com/item/%E9%85%B8%E6%B1%A4%E9%B1%BC/1754055",
			"description": "酸汤鱼，是黔桂湘交界地区的一道侗族名菜，与侗族相邻的苗、水、瑶等少数民族也有相似菜肴，但其中以贵州侗族酸汤鱼最为有名，据考证此菜肴最早源于黎平县雷洞镇牙双一带。制作原料主要有鱼肉、酸汤、山仓子等香料。成菜后，略带酸味、幽香沁人、鲜嫩爽口开胃，是贵州“黔系”菜肴的代表作之一。这道菜通常先自制酸汤，之后将活鱼去掉内脏，入酸汤煮制。"
		}
	},
	{
		"calorie": "38",
		"has_calorie": true,
		"name": "原味黑鱼煲",
		"probability": "0.265432",

	},
	{
		"calorie": "144",
		"has_calorie": true,
		"name": "椒鱼片",
		"probability": "0.0998993"
	},
	{
		"calorie": "98",
		"has_calorie": true,
		"name": "酸菜鱼",
		"probability": "0.0701917"
	},
	{
		"calorie": "257.65",
		"has_calorie": true,
		"name": "柠檬鱼",
		"probability": "0.0471465"
	}]
}
```

# 细粒度图像识别——自定义菜品识别

## 接口描述

该请求用于入库自定义的单菜品图，实现上传多菜品图的精准识别，返回具体的菜名、位置、置信度信息 。自定义菜品识别包括三个子接口：单菜品入库、菜品检索、菜品删除。

注意：在正式使用之前，请[前往细粒度图像识别页面提交合作咨询](http://ai.baidu.com/tech/imagerecognition/fine_grained)，或者申请加入百度图像识别官方QQ群（群号:659268104），提供公司名称、appid、应用场景、所需要入库的图片量，工作人员将协助建库。**注意，工作人员协助建库后以下所有子接口方可使用**。

## 自定义菜品-入库

### 子接口说明

该接口实现单张菜品图片入库，入库时需要同步提交图片及可关联至本地菜品图库的摘要信息（具体变量为brief，brief可传入图片在本地标记id、图片url、图片名称等） 注意：在正式使用之前，请[前往细粒度图像识别页面提交合作咨询](http://ai.baidu.com/tech/imagerecognition/fine_grained)，或者申请加入百度图像识别官方QQ群（群号:659268104），提供公司名称、appid、应用场景、所需要入库的图片量，工作人员将协助开通调用求权限。**注意，工作人员协助开通权限后该接口方可使用**。

### 入库图片格式要求

1、base64编码不超过4M，图片长宽不限，建议长宽范围300pixel～3000pixel， 建议长宽比3:1之内，图片格式为通用的jpg、jpeg、png、bmp。

2、入库图为单个菜品图，如果检测出多菜或者0个菜，则无法入库。

3、入库的单个菜品图的背景尽可能和实际场景(比如餐厅)需要识别的场景相似，如快餐店自助结算的场景，建议拍摄【单个菜在餐盘中】之类的图片。

4、为实现较好的识别效果，单个菜品图尽量入库多张（不同摆盘、不同光线等情况下的图片），越多识别准确率越高。

### 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/realtime_search/dish/add`

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

| 参数    | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :------ | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image   | 是       | string | -          | Base64编码字符串，以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png，jpeg)，图片大小不超过4M。最短边至少300px，最长边最大3000px。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| brief   | 否       | unit32 | -          | 菜品名称摘要信息，检索时带回，不超过256B。由于检索后需要与本地数据库关联，建议brief可传入图片在本地标记id、图片url、图片名称等 |
| sub_lib | 是       | string | -          | 默认传appid                                                  |

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
菜品识别入库
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/realtime_search/dish/add?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&sub_lib=[appid]&brief=1' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段      | 是否必选 | 类型   | 说明                       |
| :-------- | :------- | :----- | :------------------------- |
| log_id    | 否       | uint64 | 唯一的log id，用于问题定位 |
| cont_sign | 否       | string | 输入图片签名               |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
    "cont_sign": "222,333",
    "log_id": 1837888624
}
```

## 自定义菜品-检索

### 子接口说明

在已自建菜品库并入库的情况下，该接口实现单菜品/多菜品的识别。 注意：在正式使用之前，请[前往细粒度图像识别页面提交合作咨询](http://ai.baidu.com/tech/imagerecognition/fine_grained)，或者申请加入百度图像识别官方QQ群（群号:659268104），提供公司名称、appid、应用场景、所需要入库的图片量，工作人员将协助开通调用求权限。**注意，工作人员协助开通权限后该接口方可使用**。

### 请求图片格式要求

base64编码不超过4M，图片长宽不限，建议长宽范围300pixel～3000pixel， 建议长宽比3:1之内，图片格式为通用的jpg、jpeg、png、bmp。

### 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/realtime_search/dish/search`

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

| 参数    | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :------ | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image   | 是       | string | -          | Base64编码字符串，以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png，jpeg)，图片大小不超过4M。最短边至少300px，最长边最大3000px。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| sub_lib | 是       | string | -          | 默认传appid                                                  |

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
菜品识别检索
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/realtime_search/dish/search?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&sub_lib=[appid]' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段        | 是否必选 | 类型     | 说明                                 |
| :---------- | :------- | :------- | :----------------------------------- |
| log_id      | 是       | uint64   | 唯一的log id，用于问题定位           |
| result_num  | 是       | uint32   | 检索结果数                           |
| result      | 是       | object[] | 识别结果数组                         |
| +location   | 是       | object   | 菜品在图片中的位置                   |
| ++left      | 是       | int      | 水平坐标(左上角原点)                 |
| ++top       | 是       | int      | 垂直坐标(左上角原点)                 |
| ++width     | 是       | int      | 宽度                                 |
| ++height    | 是       | int      | 高度                                 |
| +dishes     | 是       | object[] | 识别结果数组                         |
| ++cont_sign | 是       | string   | 图片签名，可以用来删除图片或问题定位 |
| ++score     | 是       | float    | 图片相关性，0-1                      |
| ++brief     | 是       | string   | 入库菜品时添加的brief信息            |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
    "result_num": 2,
    "result": [
        {
            "location": {
                "left": 127,
                "top": 70,
                "width": 103,
                "height": 101
            },
            "dishes": [
                {
                    "score": 0.47976872324944,
                    "brief": "{\"name\":\"baicai\",\"id\":333}",
                    "cont_sign": "475124309,1080176642"
                }
            ]
        },
        {
            "location": {
                "left": 312,
                "top": 163,
                "width": 78,
                "height": 79
            },
            "dishes": [
                {
                    "score": 0.013810452073812,
                    "brief": "{\"name\":\"doufu\",\"id\":333}",
                    "cont_sign": "475124309,1080176642"
                }
            ]
        }
    ],
    "log_id": 1837888624
}
```

## 自定义菜品-删除

### 子接口说明

该接口实现已入库菜品图片的删除操作。 注意：在正式使用之前，请[前往细粒度图像识别页面提交合作咨询](http://ai.baidu.com/tech/imagerecognition/fine_grained)，或者申请加入百度图像识别官方QQ群（群号:659268104），提供公司名称、appid、应用场景、所需要入库的图片量，工作人员将协助开通调用求权限。**注意，工作人员协助开通权限后该接口方可使用**。

### 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/realtime_search/dish/delete`

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

| 参数      | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :-------- | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image     | 否       | string | -          | Base64编码字符串（和cont_sign二选一），以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png，jpeg)，图片大小不超过4M。最短边至少300px，最长边最大3000px。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| cont_sign | 否       | string | -          | 图片签名（和image二选一，image优先级更高）                   |
| sub_lib   | 是       | string | -          | 默认传appid                                                  |

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
菜品识别删除
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/realtime_search/dish/delete?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&sub_lib=[appid]' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段   | 是否必选 | 类型   | 说明                       |
| :----- | :------- | :----- | :------------------------- |
| log_id | 否       | uint64 | 唯一的log id，用于问题定位 |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
    "log_id": 1837888624
}
```

# 细粒度图像识别—logo商标识别

## 接口描述

该请求用于检测和识别图片中的品牌LOGO信息。即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片中LOGO的名称、位置和置信度。 当效果欠佳时，可以建立子库（在[控制台](https://console.bce.baidu.com/ai/#/ai/imagerecognition/overview/index)创建应用并申请建库）并通过调用logo入口接口完成自定义logo入库，提高识别效果。

## logo商标识别

### 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v2/logo`

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

| 参数       | 是否必选 | 类型    | 可选值范围 | 说明                                                         |
| :--------- | :------- | :------ | :--------- | :----------------------------------------------------------- |
| image      | 是       | string  | -          | 图像数据，base64编码，要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式.**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| custom_lib | 否       | boolean | -          | 是否只检索用户子库，true则只检索用户子库，false(默认)为检索底库+用户子库 |

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
logo商标识别
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v2/logo?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&custom_lib=true' -H 'Content-Type:application/x-www-form-urlencoded'
```

### 返回说明

**返回参数**

| 字段         | 是否必选 | 类型    | 说明                                                         |
| :----------- | :------- | :------ | :----------------------------------------------------------- |
| log_id       | 是       | uint64  | 唯一的log id，用于问题定位                                   |
| result_num   | 是       | uint32  | 识别结果数，标识返回结果数目                                 |
| result       | 否       | array() | 返回结果数组，每一项为一个识别出的logo                       |
| +location    | 是       | object  | 位置信息（左起像素位置、上起像素位置、像素宽、像素高）       |
| ++left       | 是       | number  | 坐起像素位置                                                 |
| ++top        | 是       | number  | 上起像素位置                                                 |
| ++width      | 是       | number  | 像素宽                                                       |
| ++height     | 是       | number  | 像素高                                                       |
| +name        | 是       | string  | 识别的品牌名称                                               |
| +probability | 是       | number  | 分类结果置信度（0--1.0）                                     |
| +type        | 是       | number  | type=0为1千种高优商标识别结果;type=1为2万类logo库的结果；其它type为自定义logo库结果 |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
  "log_id": 843411868,
  "result_num": 1,
  "result": [
    {
      "type": 0,
      "name": "科颜氏",
      "probability": 0.99998807907104,
      "location": {
        "width": 296,
        "top": 20,
        "height": 128,
        "left": 23
      }
    }
  ]
}
```

## logo商标识别—添加

使用入库接口请先在[控制台](https://console.bce.baidu.com/ai/#/ai/imagerecognition/overview/index)创建应用并申请建库，建库成功后方可正常使用。

**添加logo图片规范**

1. logo商标入库需要待识别商标的紧致包围盒图像，下面是一组示例： ![img](https://ai.bdstatic.com/file/D0711B1F943B47AAA35D48A9D1BC6249)

2. 每种商标不同场景多入库一些图像有助于获得更好的识别效果。如上图中“美素佳儿”图标。

3. 入库的图像最好是每种商标的最小识别单元。例如入库下面第一个图像的效果不如将下面后三个商标图片入库的效果好。

   入库后识别效果一般：

![img](https://ai.bdstatic.com/file/746AA91D120145C19D606478F9462342)

分别入库后效果更好

![img](https://ai.bdstatic.com/file/FCE2873299734956AAE1DCFC141DA0CA)![img](https://ai.bdstatic.com/file/26DC1C9676C7448FA00929C37ADB1D25)![img](https://ai.bdstatic.com/file/5D7E6091A89E462F85B70A32282575AA)

**不适合入库的图片示例**

1. 尽量不要含有背景，像下面图入库效果肯定不佳，需要裁剪。

![img](https://ai.bdstatic.com/file/84554A7300694F2EB29DDB88EE2512FA)![img](https://ai.bdstatic.com/file/5B89DF79820A42F8BDD59A6520997C2E)![img](https://ai.bdstatic.com/file/EE31970F97194B07AD745D616B18FAF2)

1. 不含logo的图像入库，不会返回该图像，因为算法先回做商标检测，如果没有商标，返回未识别结果，如下图。 ![img](https://ai.bdstatic.com/file/1743E0D10D334658B6F554F5038768C4)

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/realtime_search/v1/logo/add`

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
| image | 是       | string | -          | 图像数据，base64编码，要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式.**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| brief | 是       | string | -          | 检索时带回。此处要传对应的name与code字段，name长度小于100B，code长度小于150B。示例："{"name": "abc", "code":"ddd"}" |

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
logo商标识别—添加
curl -i -k 'https://aip.baidubce.com/rest/2.0/realtime_search/v1/logo/add?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&custom_lib={'name': 'abc', 'code': 'abc'}' -H 'Content-Type:application/x-www-form-urlencoded'
```

### 返回说明

**返回参数**

| 字段      | 是否必选 | 类型   | 说明                       |
| :-------- | :------- | :----- | :------------------------- |
| log_id    | 是       | uint64 | 唯一的log id，用于问题定位 |
| cont_sign | 是       | string | 输入图片签名，可用于删除   |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
  "log_id": 972934653,
  "cont_sign": "217113248,419422301"
}
```

## logo商标识别—删除

使用删除接口请先在[控制台](https://console.bce.baidu.com/ai/#/ai/imagerecognition/overview/index)创建应用并申请建库，建库成功后先调用入库接口完成logo图片入库，删除接口用户在已入库的logo图片中删除图片。

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/realtime_search/v1/logo/delete`

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

| 参数      | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :-------- | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image     | 是       | string | -          | base64编码后的图片数据（和cont_sign二选一）。要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式. **注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| cont_sign | 是       | string | -          | 图片签名（和image二选一，image优先级更高）                   |

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
logo商标识别—删除
curl -i -k 'https://aip.baidubce.com/rest/2.0/realtime_search/v1/logo/delete?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

### 返回说明

**返回参数**

| 字段   | 是否必选 | 类型   | 说明                       |
| :----- | :------- | :----- | :------------------------- |
| log_id | 是       | uint64 | 唯一的log id，用于问题定位 |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
  "log_id": 50488417
}
```

# 细粒度图像识别—动物识别

## 接口描述

该请求用于识别一张图片，即对于输入的一张图片（可正常解码，且长宽比较合适），输出动物识别结果。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/animal`

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

| 参数名称  | 是否必选 | 类型    | 默认值 | 说明                                                         |
| :-------- | :------- | :------ | :----- | :----------------------------------------------------------- |
| image     | 是       | string  |        | 图像数据，base64编码，要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| top_num   | 否       | integer | 6      | 返回预测得分top结果数，默认为6                               |
| baike_num | 否       | integer | 0      | 返回百科信息的结果数，默认不返回                             |

**请求代码示例**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

bash

PHP

Java

Python

C++

C#

```
动物识别
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/animal?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&top_num=6' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 参数          | 类型          | 是否必须 | 说明                         |
| :------------ | :------------ | :------- | :--------------------------- |
| log_id        | uint64        | 是       | 唯一的log id，用于问题定位   |
| result        | arrry(object) | 是       | 识别结果数组                 |
| +name         | string        | 是       | 动物名称，示例：蒙古马       |
| +score        | uint32        | 是       | 置信度，示例：0.5321         |
| +baike_info   | object        | 否       | 对应识别结果的百科词条名称   |
| ++baike_url   | string        | 否       | 对应识别结果百度百科页面链接 |
| ++image_url   | string        | 否       | 对应识别结果百科图片链接     |
| ++description | string        | 否       | 对应识别结果百科内容描述     |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
	"log_id": 7392482912853822863,
	"result": [{
		"score": "0.993811",
		"name": "叉角羚",
		"baike_info": {
			"baike_url": "http://baike.baidu.com/item/%E5%8F%89%E8%A7%92%E7%BE%9A/801703",
			"description": "叉角羚(学名：Antilocapra americana)：在角的中部角鞘有向前伸的分枝，故名。体型中等，体长1-1.5米，尾长7.5-10厘米，肩高81-104厘米，成体重36-60千克，雌体比雄体小；背面为红褐色，颈部有黑色鬃毛，腹部和臀部为白色，颊面部和颈部两侧有黑色块斑；毛被下面为绒毛，上覆以粗糙、质脆的长毛，由于某些皮肤肌的作用，能使其毛被呈不同角度，以利于保暖或散热。植食。叉角羚奔跑速度非常快，最高时速达100千米。一次跳跃可达3.5-6米。善游泳。夏季组成小群活动，冬季则集结成上百只的大群。为寻找食物和水源，一年中常进行几次迁移。性机警，视觉敏锐，能看到数千米外的物体。遇险时，臀部的白色毛能立起，向同伴告警。分布于北美洲。"
		}
	},
	{
		"score": "0.000289439",
		"name": "印度羚"
	},
	{
		"score": "0.000186248",
		"name": "藏羚羊"
	},
	{
		"score": "0.000147176",
		"name": "跳羚"
	},
	{
		"score": "0.000134434",
		"name": "驯鹿"
	},
	{
		"score": "9.86555e-05",
		"name": "高鼻羚羊"
	}]
}
```

# 细粒度图像识别—植物识别

## 接口描述

该请求用于识别一张图片，即对于输入的一张图片（可正常解码，且长宽比较合适），输出植物识别结果。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/plant`

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

| 参数名称  | 是否必选 | 类型    | 说明                                                         |
| :-------- | :------- | :------ | :----------------------------------------------------------- |
| image     | 是       | string  | 图像数据，base64编码，要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| baike_num | 否       | integer | 0                                                            |

**请求代码示例**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

bash

PHP

Java

Python

C++

C#

```
植物识别
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/plant?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 参数          | 类型          | 是否必须 | 说明                         |
| :------------ | :------------ | :------- | :--------------------------- |
| log_id        | uint64        | 是       | 唯一的log id，用于问题定位   |
| result        | arrry(object) | 是       | 植物识别结果数组             |
| +name         | string        | 是       | 植物名称，示例：吉娃莲       |
| +score        | uint32        | 是       | 置信度，示例：0.5321         |
| +baike_info   | object        | 否       | 对应识别结果的百科词条名称   |
| ++baike_url   | string        | 否       | 对应识别结果百度百科页面链接 |
| ++image_url   | string        | 否       | 对应识别结果百科图片链接     |
| ++description | string        | 否       | 对应识别结果百科内容描述     |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
	"log_id": 1705495792822072357,
	"result": [{
		"score": 0.99979120492935,
		"name": "莲",
		"baike_info": {
			"baike_url": "http://baike.baidu.com/item/%E8%8E%B2/2717141",
			"description": "莲(Nelumbo nucifera)，又称荷、荷花、莲花、芙蕖、鞭蓉、水芙蓉、水芝、水芸、水旦、水华等，溪客、玉环是其雅称，未开的花蕾称菡萏，已开的花朵称鞭蕖，莲科，属多年生水生宿根草本植物，其地下茎称藕，能食用，叶入药，莲子为上乘补品，花可供观赏。是我国十大名花之一。是印度的国花。莲，双子叶植物，睡莲科。多年生挺水草本植物。根状茎横走，粗而肥厚，节间膨大，内有纵横通气孔道，节部缢缩。叶基生，挺出水面，盾形，直径30-90cm，波状边缘，上面深绿色，下面浅绿色。叶柄有小刺，长1-2m，挺出水面。花单生，直径10-25cm，椭圆花瓣多数，白色或粉红色；花柄长1-2m。花托在果期膨大，直径5-10cm，海绵质。坚果椭圆形和卵圆形，长1.5-2.0cm，灰褐色。种子卵圆形，长1.2-1.7cm，种皮红棕色。生于池塘、浅湖泊及稻田中。中国南北各省有自生或栽培，经济价值高。人们习惯上称种子为“莲子”、地下茎为“藕”、花托为“莲蓬”、叶为“荷叶”。"
		}
	},
	{
		"score": 0.00015144718054216,
		"name": "红睡莲"
	},
	{
		"score": 1.2172759852547e-05,
		"name": "白睡莲"
	},
	{
		"score": 6.305016540864e-06,
		"name": "延药睡莲"
	},
	{
		"score": 3.6133328649157e-06,
		"name": "华夏慈姑"
	}]
}
```

# 细粒度图像识别—花卉识别

## 接口描述

检测用户上传的花卉图片，输出图片的花卉识别结果名称及对应的概率打分。 注意：在正式使用之前，请[前往细粒度图像识别页面提交合作咨询](http://ai.baidu.com/tech/imagerecognition/fine_grained)，或者申请加入百度图像识别官方QQ群（群号:659268104），提供公司名称、appid、应用场景、所需要入库的图片量，工作人员将协助开通调用求权限。**注意，工作人员协助开通权限后该接口方可使用**。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/flower`

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

| 参数      | 是否必选 | 类型    | 可选值范围 | 说明                                                         |
| :-------- | :------- | :------ | :--------- | :----------------------------------------------------------- |
| image     | 是       | string  | -          | Base64编码字符串，以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png，jpeg)，图片大小不超过4M。最短边至少15px，最长边最大4096px。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| top_num   | 否       | unit32  | -          | 返回结果top n，默认5。                                       |
| baike_num | 否       | integer | 0          | 返回百科信息的结果数，默认不返回                             |

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
花卉识别
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/flower?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段          | 是否必选 | 类型     | 说明                                   |
| :------------ | :------- | :------- | :------------------------------------- |
| log_id        | 是       | uint64   | 唯一的log id，用于问题定位             |
| result_num    | 否       | unit32   | 返回结果数目，及result数组中的元素个数 |
| result        | 否       | object() | 标签结果数组                           |
| +name         | 否       | string   | 花卉名称                               |
| +score        | 否       | double   | 置信度，0-1                            |
| +baike_info   | object   | 否       | 对应识别结果的百科词条名称             |
| ++baike_url   | string   | 否       | 对应识别结果百度百科页面链接           |
| ++image_url   | string   | 否       | 对应识别结果百科图片链接               |
| ++description | string   | 否       | 对应识别结果百科内容描述               |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
	"log_id": 2233046923807801461,
	"result": [{
		"score": 0.79123616218567,
		"name": "荷花",
		"baike_info": {
			"baike_url": "http://baike.baidu.com/item/%E8%8D%B7%E8%8A%B1/158674",
			"description": "荷花(Lotus flower)属山龙眼目，莲科，是莲属二种植物的通称。又名莲花、水芙蓉等。是莲属多年生水生草本花卉。地下茎长而肥厚，有长节，叶盾圆形。花期6至9月，单生于花梗顶端，花瓣多数，嵌生在花托穴内，有红、粉红、白、紫等色，或有彩纹、镶边。坚果椭圆形，种子成卵形。荷花种类很多，分观赏和食用两大类。原产亚洲热带和温带地区，中国早在周朝就有栽培记载。荷花全身皆宝，藕和莲子能食用，莲子、根茎、藕节、荷叶、花及种子的胚芽等都可入药。其出污泥而不染之品格恒为世人称颂。“接天莲叶无穷碧，映日荷花别样红”就是对荷花之美的真实写照。荷花“中通外直，不蔓不枝，出淤泥而不染，濯清涟而不妖”的高尚品格，历来为古往今来诗人墨客歌咏绘画的题材之一。1985年5月荷花被评为中国十大名花之一。荷花是印度,越南的国花。"
		}
	},
	{
		"score": 0.16631570458412,
		"name": "莲"
	},
	{
		"score": 0.015812600031495,
		"name": "非花"
	},
	{
		"score": 0.0058604963123798,
		"name": "碗莲"
	},
	{
		"score": 0.0039983494207263,
		"name": "并蒂莲"
	}]
}
```

# 细粒度图像识别—果蔬类食材识别

## 接口描述

该请求用于识别果蔬类食材，即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片中的果蔬食材结果。 注意：在正式使用之前，请[前往细粒度图像识别页面提交合作咨询](http://ai.baidu.com/tech/imagerecognition/fine_grained)，或者申请加入百度图像识别官方QQ群（群号:659268104），提供公司名称、appid、应用场景、所需要入库的图片量，工作人员将协助开通调用求权限。**注意，工作人员协助开通权限后该接口方可使用**。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/classify/ingredient`

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

| 参数    | 是否必选 | 类型   | 可选值范围 | 说明                                                         |
| :------ | :------- | :----- | :--------- | :----------------------------------------------------------- |
| image   | 是       | string | -          | Base64编码字符串，以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png)，图片大小不超过4M。最短边至少15px，最长边最大4096px。 **注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| top_num | 否       | unit32 | -          | 返回预测得分top结果数，如果为空或小于等于0默认为5；如果大于20默认20 |

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
细粒度图像识别
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/classify/ingredient?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段       | 是否必选 | 类型     | 说明                       |
| :--------- | :------- | :------- | :------------------------- |
| log_id     | 是       | uint64   | 唯一的log id，用于问题定位 |
| result     | 是       | object() | 识别结果                   |
| result_num | 否       | unit32   | 识别结果数                 |
| +name      | 否       | string   | 图像中的食材名称           |
| +score     | 否       | double   | 得分，0-1                  |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
    "log_id": 1091287364,
    "result_num": 20,
    "result": [
        {
            "name": "非果蔬食材",
            "score": 0.99999988079071
        },
        {
            "name": "油菜",
            "score": 1.1175458780599e-8
        },
        {
            "name": "蛇果",
            "score": 7.2776291659693e-9
        },
        {
            "name": "国光苹果",
            "score": 5.6971951600815e-9
        },
        {
            "name": "海枣",
            "score": 4.2508210285064e-9
        },
        {
            "name": "琼瑶浆葡萄",
            "score": 4.1451895249622e-9
        },
        {
            "name": "京锐号辣椒",
            "score": 3.9915102334476e-9
        },
        {
            "name": "冬瓜",
            "score": 3.3611948779821e-9
        },
        {
            "name": "长江豆",
            "score": 2.9944848822794e-9
        },
        {
            "name": "黑加仑",
            "score": 2.7750137743254e-9
        },
        {
            "name": "面包果",
            "score": 2.3357531464541e-9
        },
        {
            "name": "椰子",
            "score": 1.9741890344704e-9
        },
        {
            "name": "美人瓜",
            "score": 1.9319581490151e-9
        },
        {
            "name": "莲藕",
            "score": 1.759222323372e-9
        },
        {
            "name": "黑奥林",
            "score": 1.7266311713726e-9
        },
        {
            "name": "芥菜",
            "score": 1.6180708994895e-9
        },
        {
            "name": "样芹菜",
            "score": 1.5472728653876e-9
        },
        {
            "name": "篙巴",
            "score": 1.4084827748562e-9
        },
        {
            "name": "花生",
            "score": 1.3972580870103e-9
        },
        {
            "name": "魁绿猕猴桃",
            "score": 1.3920842256709e-9
        }
    ]
}
```

# 细粒度图像识别—地标识别

## 接口描述

该请求用于识别地标，即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片中的地标识别结果。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/landmark`

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
| image | 是       | string | -          | Base64编码字符串，以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png)，图片大小不超过4M。最短边至少15px，最长边最大4096px。 **注意：图片的base64编码是不包含图片头的，如（data:image/jpg;base64,）** |

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
地标识别
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/landmark?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段      | 是否必选 | 类型   | 说明                             |
| :-------- | :------- | :----- | :------------------------------- |
| log_id    | 是       | uint64 | 请求标识码，随机数，唯一         |
| result    | 是       | dict   | 识别结果                         |
| +landmark | 是       | string | 地标名称，无法识别则返回空字符串 |

**返回示例**

```
{"log_id": 3450013152046070669, "result": {"landmark": "狮身人面像"}}
```

# 细粒度图像识别—红酒识别（邀测）

## 接口描述

该服务用于识别红酒标签，即对于输入的一张图片（可正常解码，长宽比适宜，且酒标清晰可见），输出图片中的红酒名称。

注意：图片中酒标要清晰可见，示例图片如下

![img](https://ai.bdstatic.com/file/5DA8EEB0566D4331A85B28029A092118)

注： 1、**邀测的接口，不能直接在控制台调用，可通过QQ群（659268104）联系群管、或提交工单申请开通测试权限**。

2、**邀测的接口，暂未封装服务端SDK，只能通过API调用**。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/redwine`

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
| image | 是       | string | -          | Base64编码字符串，以图片文件形式请求时必填。(支持图片格式：jpg，bmp，png)，图片大小不超过4M。最短边至少15px，最长边最大4096px。 **注意：图片的base64编码是不包含图片头的，如（data:image/jpg;base64,）** |

**请求代码示例**

**提示一**：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

**提示二**：部分语言依赖的类或库，请在代码注释中查看下载地址。

## 返回说明

**返回参数**

| 字段     | 是否必选 | 类型   | 说明                                 |
| :------- | :------- | :----- | :----------------------------------- |
| log_id   | 是       | uint64 | 请求标识码，随机数，唯一             |
| result   | 是       | dict   | 识别结果                             |
| +redwine | 是       | string | 红酒标签名称，无法识别则返回空字符串 |

**返回示例**

```
{"log_id": 3450013152046070669, "result": {"redwine": "银色高地阙歌干红"}}
```

# 车辆分析—车型识别

## 接口描述

该请求用于检测一张车辆图片的具体车型。即对于输入的一张图片（可正常解码，且长宽比适宜），输出图片的车辆品牌及型号、颜色及年份、位置信息。

注：当前只支持单主体识别，若图片中有多个车辆，则识别目标最大的车辆。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL： `https://aip.baidubce.com/rest/2.0/image-classify/v1/car`

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

| 参数      | 是否必选 | 类型    | 可选值范围 | 说明                                                         |
| :-------- | :------- | :------ | :--------- | :----------------------------------------------------------- |
| image     | 是       | string  | -          | 图像数据，base64编码，要求base64编码后大小不超过4M，最短边至少15px，最长边最大4096px,支持jpg/png/bmp格式 。**注意：图片需要base64编码、去掉编码头后再进行urlencode。** |
| top_num   | 否       | uint32  | -          | 返回结果top n，默认5。                                       |
| baike_num | 否       | integer | 0          | 返回百科信息的结果数，默认不返回                             |

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
车型识别
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/car?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&top_num=5' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段            | 是否必选 | 类型         | 说明                         |
| :-------------- | :------- | :----------- | :--------------------------- |
| log_id          | 否       | uint64       | 唯一的log id，用于问题定位   |
| color_result    | 是       | string       | 颜色                         |
| result          | 否       | car-result() | 车型识别结果数组             |
| +name           | 否       | string       | 车型名称，示例：宝马x6       |
| +score          | 否       | double       | 置信度，示例：0.5321         |
| +year           | 否       | string       | 年份                         |
| +baike_info     | 否       | object       | 对应识别结果的百科词条名称   |
| ++baike_url     | 否       | string       | 对应识别结果百度百科页面链接 |
| ++image_url     | 否       | string       | 对应识别结果百科图片链接     |
| ++description   | 否       | string       | 对应识别结果百科内容描述     |
| location_result | 否       | string       | 车在图片中的位置信息         |

**返回示例**

```
HTTP/1.1 200 OK
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
Cache-Control: no-cache
Server: BWS
Date: Tue, 18 Oct 2016 02:21:01 GMT
Content-Type: application/json;charset=UTF-8
{
	"log_id": 4086212218842203806,
	"location_result": {
		"width": 447,
		"top": 226,
		"height": 209,
		"left": 188
	},
	"result": [{
		"baike_info": {
			"baike_url": "http://baike.baidu.com/item/%E5%B8%83%E5%8A%A0%E8%BF%AAChiron/20419512",
			"description": "布加迪Chiron是法国跑车品牌布加迪出品的豪华超跑车。配置四涡轮增压发动机，420 公里每小时，有23种颜色的选择，售价高达260万美元。"
		},
		"score": 0.98793351650238,
		"name": "布加迪Chiron",
		"year": "无年份信息"
	},
	{
		"score": 0.0021970034576952,
		"name": "奥迪RS5",
		"year": "2011-2017"
	},
	{
		"score": 0.0021096928976476,
		"name": "奥迪RS4",
		"year": "无年份信息"
	},
	{
		"score": 0.0015581247862428,
		"name": "奥迪RS7",
		"year": "2014-2016"
	},
	{
		"score": 0.00082337751518935,
		"name": "布加迪威航",
		"year": "2004-2015"
	}],
	"color_result": "颜色无法识别"
}
```

# 车辆分析—车辆检测（邀测）

## 接口描述

传入单帧图像，**检测图片中所有机动车辆，返回每辆车的类型和坐标位置**，可识别小汽车、卡车、巴士、摩托车、三轮车5大类车辆，**并对每类车辆分别计数，可返回含有统计值和检测框的渲染结果图**，支持指定不规则区域的车辆统计。

**当前主要适用于普通监控场景，如道路、停车场等**，无人机高空拍摄的图片，因车辆目标较小，识别效果可能欠佳，后续会扩展支持超高空拍摄场景，如有该场景的需求，请通过QQ群或提交工单，详细沟通测试情况。

普通监控场景示例图片：

![img](https://ai.bdstatic.com/file/CB8E31658FB5401DA06FA87E0D5603E4)

注： 1、**邀测的接口，不能直接在控制台调用，可通过QQ群（659268104）联系群管、或提交工单申请开通测试权限**。

2、**邀测的接口，暂未封装服务端SDK，只能通过API调用**。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL：`https://aip.baidubce.com/rest/2.0/image-classify/v1/vehicle_detect`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token，参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值                                |
| :----------- | :-------------------------------- |
| Content-Type | application/x-www-form-urlencoded |

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数  | 是否必选 | 类型   | 可选值范围                  | 说明                                                         |
| :---- | :------- | :----- | :-------------------------- | :----------------------------------------------------------- |
| image | 是       | string | 0-255彩色图像（base64编码） | 图像数据，Base64编码字符串，不超过4M。最短边至少10px，最长边最多4096px。支持图片格式：jpg，bmp，png。 **注意：图片的base64编码是不包含图片头的，如（data:image/jpg;base64,）** |
| show  | 否       | string | true/false                  | 是否返回结果图（含统计值和跟踪框）。选true时返回渲染后的图片(base64)，其它无效值或为空则默认false。 |
| area  | 否       | string | 小于原图像素范围            | 只统计该区域内的车辆数，缺省时为全图统计。 逗号分隔，如‘x1,y1,x2,y2,x3,y3...xn,yn'，按顺序依次给出每个顶点的x、y坐标（默认尾点和首点相连），形成闭合多边形区域。 服务会做范围（顶点左边需在图像范围内）及个数校验（数组长度必须为偶数，且大于3个顶点）。只支持单个多边形区域，建议设置矩形框，即4个顶点。**坐标取值不能超过图像宽度和高度，比如1280的宽度，坐标值最大到1279**。 |

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
车辆检测
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/vehicle_detect?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段         | 是否必选 | 类型     | 说明                                                         |
| :----------- | :------- | :------- | :----------------------------------------------------------- |
| vehicle_num  | 是       | object   | 检测到的车辆数目                                             |
| +car         | 是       | int      | 小汽车数量                                                   |
| +truck       | 是       | int      | 卡车数量                                                     |
| +bus         | 是       | int      | 巴士数量                                                     |
| +motorbike   | 是       | int      | 摩托车数量                                                   |
| +tricycle    | 是       | int      | 三轮车数量                                                   |
| vehicle_info | 是       | object[] | 每个框的具体信息                                             |
| +location    | 是       | object   | 跟踪到的车辆检测框位置                                       |
| ++left       | 是       | int      | 车辆检测框左坐标                                             |
| ++top        | 是       | int      | 车辆检测框顶坐标                                             |
| ++width      | 是       | int      | 车辆检测框宽度                                               |
| ++height     | 是       | int      | 车辆检测框高度                                               |
| +ID          | 是       | int      | 车辆的ID编号                                                 |
| +type        | 是       | string   | 车辆类型                                                     |
| image        | 否       | string   | 结果图，含跟踪框和统计值（渲染jpg图片byte内容的base64编码，得到后先做base64解码再以字节流形式imdecode） |

**返回示例**

未检测到任何车辆：

```
{
    "vehicle_num":
    {
        "car":0,
        "truck":0,
        ...
        "tricycle":0
    },
    "vehicle_info":[]
}
```

检测到2辆小汽车、1辆卡车：

```
{
    "vehicle_num":
    {
        "car":2,
        "truck":1,
 
        ...
 
        "tricycle":0
 
    },
 
    "vehicle_info":
    [
        {
            "ID":0
            "location":
            {
                "left": 100,
                "top": 200,
                "width": 200,
                "height": 400,
             }
            "type": "car"
        },
        {
            "ID": 1
            "location":
            {
                "left": 400,
                "top": 200,
                "width": 200,
                "height": 400,
             }
            "type": "car"
         },
         {
            "ID": 2
            "location":
            {
                "left": 600,
                "top": 200,
                "width": 300,
                "height": 400,
             }
            "type": "truck"
         }
    ]
}
```

# 车辆分析—车流统计（邀测）

## 接口描述

根据传入的连续视频图片序列，**进行车辆检测和追踪，返回每个车辆的坐标位置、车辆类型（包括小汽车、卡车、巴士、摩托车、三轮车5大类）。在原图中指定区域，根据车辆轨迹判断驶入/驶出区域的行为，统计各类车辆的区域进出车流量，可返回含统计值和跟踪框的渲染图**。

（注：为保证有效跟踪，建议抽帧频率5-10fps）

**当前主要适用于普通监控场景，如道路、停车场等**，无人机高空拍摄的图片，因车辆目标较小，识别效果可能欠佳，后续会扩展支持超高空拍摄场景，如有该场景的需求，请通过QQ群或提交工单，详细沟通测试情况。

渲染图示例：

![img](https://ai.bdstatic.com/file/B2C06D414177403C93EB0AE150278261)

注： 1、**邀测的接口，不能直接在控制台调用，可通过QQ群（659268104）联系群管、或提交工单申请开通测试权限**。

2、**邀测的接口，暂未封装服务端SDK，只能通过API调用**。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL：`https://aip.baidubce.com/rest/2.0/image-classify/v1/traffic_flow`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token，参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值                                |
| :----------- | :-------------------------------- |
| Content-Type | application/x-www-form-urlencoded |

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数      | 是否必选 | 类型   | 取值范围                    | 说明                                                         |
| :-------- | :------- | :----- | :-------------------------- | :----------------------------------------------------------- |
| case_id   | 是       | int32  | -                           | 任务ID（通过case_id区分不同视频流，自拟，不同序列间不可重复） |
| case_init | 是       | string | true/false                  | 每个case的初始化信号，为true时对该case下的跟踪算法进行初始化，为false时重载该case的跟踪状态。当为false且读取不到相应case的信息时，直接重新初始化 |
| image     | 是       | string | 0-255彩色图像（base64编码） | 图像数据，Base64编码字符串，不超过4M。最短边至少10px，最长边最多4096px。支持图片格式：jpg，bmp，png。 **注意：图片的base64编码是不包含图片头的，如（data:image/jpg;base64,）** |
| show      | 否       | string | true/false                  | 是否返回结果图（含统计值和跟踪框）。选true时返回渲染后的图片(base64)，其它无效值或为空则默认false。 |
| area      | 是       | string | 小于原图像素范围            | 只统计进出该区域的车辆。 逗号分隔，如‘x1,y1,x2,y2,x3,y3...xn,yn'，按顺序依次给出每个顶点的x、y坐标（默认尾点和首点相连），形成闭合多边形区域。 服务会做范围（顶点左边需在图像范围内）及个数校验（数组长度必须为偶数，且大于3个顶点）。只支持单个多边形区域，建议设置矩形框，即4个顶点。**坐标取值不能超过图像宽度和高度，比如1280的宽度，坐标值最大到1279**。 |

**area参数设置说明**

进出区域方向：从区域外走到区域内就是in，相反就是out。

如下图：绿色框为区域框，左边红色箭头的方向为in，右边蓝色箭头的方向为out。

![img](https://ai.bdstatic.com/file/A39B18C5D7C64C3FAC074406E675A6F9)

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
车流统计
curl -i -k 'https://aip.baidubce.com/rest/2.0/image-classify/v1/traffic_flow?access_token=【调用鉴权接口获取的token】' --data 'image=【图片Base64编码，需UrlEncode】&case_id=1&case_init=false&area=1,1,719,1,719,719,1,719' -H 'Content-Type:application/x-www-form-urlencoded'
```

## 返回说明

**返回参数**

| 字段          | 是否必选 | 类型     | 说明                                                         |
| :------------ | :------- | :------- | :----------------------------------------------------------- |
| vehicle_num   | 是       | object   | 检测到的车辆数目                                             |
| +car          | 是       | int      | 小汽车数量                                                   |
| +truck        | 是       | int      | 卡车数量                                                     |
| +bus          | 是       | int      | 巴士数量                                                     |
| +motorbike    | 是       | int      | 摩托车数量                                                   |
| +tricycle     | 是       | int      | 三轮车数量                                                   |
| vehicle_info  | 是       | object[] | 每个框的具体信息                                             |
| +location     | 是       | object   | 跟踪到的车辆检测框位置                                       |
| ++left        | 是       | int      | 车辆检测框左坐标                                             |
| ++top         | 是       | int      | 车辆检测框顶坐标                                             |
| ++width       | 是       | int      | 车辆检测框宽度                                               |
| ++height      | 是       | int      | 车辆检测框高度                                               |
| +ID           | 是       | int      | 车辆的ID编号                                                 |
| +type         | 是       | string   | 车辆类型                                                     |
| vehicle_count | 是       | object[] | 进出区域的车流统计                                           |
| +car          | 是       | object   | 小汽车                                                       |
| ++in          | 是       | int      | 当前帧相应车型驶入区域的数量（下同）                         |
| ++out         | 是       | int      | 当前帧相应车型驶出区域的数量（下同）                         |
| +truck        | 是       | object   | 卡车                                                         |
| ++in          | 是       | int      |                                                              |
| ++out         | 是       | int      |                                                              |
| +bus          | 是       | object   | 巴士                                                         |
| ++in          | 是       | int      |                                                              |
| ++out         | 是       | int      |                                                              |
| +motorbike    | 是       | object   | 摩托车                                                       |
| ++in          | 是       | int      |                                                              |
| ++out         | 是       | int      |                                                              |
| +tricycle     | 是       | object   | 三轮车                                                       |
| ++in          | 是       | int      |                                                              |
| ++out         | 是       | int      |                                                              |
| image         | 否       | string   | 结果图，含跟踪框和统计值（渲染jpg图片byte内容的base64编码，得到后先做base64解码再以字节流形式imdecode） |

**返回示例**

未检测到任何车：

```
{
    "vehicle_num":
    {
        "car":0,
        "truck":0,
        ...
        "tricycle":0
    },
    "vehicle_info":[],
    “vehicle_count”:
    {
        "car":
        {
            "in":0,
            "out":0
        },
        "truck":
        {
            "in":0
            "out":0
        },
        ...
    }
}
```

检测到1辆小汽车、1辆卡车，无轨迹，无车进出区域：

```
{
    "vehicle_num":
    {
        "car":1,
        "truck":1,
 
        ...
 
        "tricycle":0
 
    },
 
    "vehicle_info":[],
    “vehicle_count”:
    {
        "car":
        {
            "in":0,
            "out":0
        },
        "truck":
        {
            "in":0
            "out":0
        },
 
        ...
 
    }
}
```

检测到2辆小汽车、1辆卡车，3条轨迹，1辆卡车离开区域：

```
{
    "vehicle_num":
    {
        "car":2,
        "truck":1,
 
        ...
 
        "tricycle":0
 
    },
 
    "vehicle_info":
    [
        {
            "ID":3
            "location":
            {
                "left": 100,
                "top": 200,
                "width": 200,
                "height": 400,
             }
            "type": "car"
        },
        {
            "ID": 5
            "location":
            {
                "left": 400,
                "top": 200,
                "width": 200,
                "height": 400,
             }
            "type": "car"
         },
         {
            "ID": 6
            "location":
            {
                "left": 600,
                "top": 200,
                "width": 300,
                "height": 400,
             }
            "type": "truck"
         }
    ],
    “vehicle_count”:
    {
        "car":
        {
            "in":0,
            "out":0
        },
        "truck":
        {
            "in":0
            "out":1
        },
 
        ...
 
    }
}
```

# 车辆分析—车辆属性识别（邀测）

## 接口描述

传入单帧图像，检测图片中所有车辆，返回每辆车的类型和坐标位置，可识别小汽车、卡车、巴士、摩托车、三轮车、自行车6大类车辆，并**针对小汽车识别11种外观属性，包括：是否有车窗雨眉、是否有车顶架、副驾驶是否有人、驾驶位是否系安全带、遮阳板是否放下、车辆朝向等**。

当前主要适用于普通监控场景，如道路、停车场等，无人机高空拍摄的图片，因车辆目标较小，识别效果可能欠佳，后续会扩展支持超高空拍摄场景，如有该场景的需求，请通过QQ群或提交工单，详细沟通测试情况。

普通监控场景示例图片：

![img](https://ai.bdstatic.com/file/0BB0B5F41C9D459F9260360118794216)

注：

1、**邀测的接口，不能直接在控制台调用，可通过QQ群（659268104）联系群管、或提交工单申请开通测试权限**。

2、**邀测的接口，暂未封装服务端SDK，只能通过API调用**。

## 请求说明

**请求示例**

HTTP 方法：`POST`

请求URL：`https://aip.baidubce.com/rest/2.0/image-classify/v1/vehicle_attr`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token，参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值                                |
| :----------- | :-------------------------------- |
| Content-Type | application/x-www-form-urlencoded |

Body中放置请求参数，参数详情如下：

**请求参数**

| 参数  | 是否必选 | 类型   | 取值范围                    | 说明                                                         |
| :---- | :------- | :----- | :-------------------------- | :----------------------------------------------------------- |
| image | 是       | string | 0-255彩色图像（base64编码） | 图像数据，Base64编码字符串，不超过4M。最短边至少10px，最长边最多4096px。支持图片格式：jpg，bmp，png。 注意：图片的base64编码是不包含图片头的，如（data:image/jpg;base64,） |
| type  | 否       | string |                             | 是否选定某些属性输出对应的信息，可从12种输出属性中任选若干，用英文逗号分隔（例如vehicle_type,roof_rack,skylight）。**默认输出全部属性** |

附：type字段说明

| ID   | type                | 说明                   | 类别数 | 类别                                                         |
| :--- | :------------------ | :--------------------- | :----- | :----------------------------------------------------------- |
| 0    | vehicle_type        | 车辆类型               | 6      | 小汽车、卡车、公交车、摩托、三轮车、自行车                   |
| 1    | window_rain_eyebrow | 是否有车窗雨眉         | 2      | 无车窗雨眉、有车窗雨眉                                       |
| 2    | roof_rack           | 是否有车顶架           | 2      | 无车顶架、有车顶架                                           |
| 3    | skylight            | 是否有天窗             | 2      | 无天窗、有天窗                                               |
| 4    | in_car_item         | 是否有车内摆放物       | 2      | 无车内摆放物、有车内摆放物                                   |
| 5    | rearview_item       | 是否有后视镜悬挂物     | 2      | 无后视镜悬挂物、有后视镜悬挂物                               |
| 6    | copilot             | 副驾驶是否有人         | 2      | 副驾驶无人、副驾驶有人                                       |
| 7    | driver_belt         | 驾驶位是否系安全带     | 2      | 驾驶位未系安全带、驾驶位系安全带                             |
| 8    | copilot_belt        | 副驾驶位是否系安全带   | 2      | 副驾驶位未系安全带、副驾驶位系安全带                         |
| 9    | driver_visor        | 驾驶位遮阳板是否放下   | 2      | 驾驶位遮阳板未放下、驾驶位遮阳板放下                         |
| 10   | copilot_visor       | 副驾驶位遮阳板是否放下 | 2      | 副驾驶位遮阳板未放下、副驾驶位遮阳板放下                     |
| 11   | direction           | 车辆朝向               | 8      | 正前方，右前方，正右方，右后方，正后方，左后方，正左方，左前方 |

**请求代码示例**

提示一：使用示例代码前，请记得替换其中的示例Token、图片地址或Base64信息。

提示二：部分语言依赖的类或库，请在代码注释中查看下载地址。

undefined

## 返回说明

**返回参数**

| 字段                  | 是否必选 | 类型       | 说明                                                         |
| :-------------------- | :------- | :--------- | :----------------------------------------------------------- |
| vehicle_num           | 是       | int        | 检测到的车辆框数目                                           |
| vehicle_info          | 是       | object数组 | 每个车辆检测框的具体信息                                     |
| +location             | 是       | object     | 检测到的车辆框位置                                           |
| ++left                | 是       | int        | 检测框左坐标                                                 |
| ++top                 | 是       | int        | 检测框顶坐标                                                 |
| ++width               | 是       | int        | 检测框宽度                                                   |
| ++height              | 是       | int        | 检测框高度                                                   |
| +attributes           | 否       | object数组 | 包含所有车辆属性内容                                         |
| ++vehicle_type        | 否       | object     | 车辆类型                                                     |
| +++name               | 否       | string     | 如"小汽车"                                                   |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++window_rain_eyebrow | 否       | object     | 是否有车窗雨眉                                               |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大（即有车窗雨眉），实际应用中可根据测试情况选取合适的score阈值 |
| ++roof_rack           | 否       | object     | 是否有车顶架                                                 |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++skylight            | 否       | object     | 是否有天窗                                                   |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++in_car_item         | 否       | object     | 是否有车内摆放物                                             |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++rearview_item       | 否       | object     | 是否有后视镜悬挂物                                           |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++copilot             | 否       | object     | 副驾驶是否有人                                               |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++driver_belt         | 否       | object     | 驾驶位是否系安全带                                           |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++copilot_belt        | 否       | object     | 副驾驶位是否系安全带                                         |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++driver_visor        | 否       | object     | 驾驶位遮阳板是否放下                                         |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++copilot_visor       | 否       | object     | 副驾驶位遮阳板是否放下                                       |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |
| ++direction           | 否       | object     | 车辆朝向                                                     |
| +++name               | 否       | string     | 如"正前方"                                                   |
| +++score              | 否       | float      | 对应概率分数，分数越大代表属性的正向取值可能性越大，实际应用中可根据测试情况选取合适的score阈值 |

**返回示例**

未检测到任何车辆：

```
    {
        "vehicle_num": 0,
        "vehicle_info":
        [
            {
                "location":
                {
                    "left": 0,
                    "top": 0,
                    "width": 0,
                    "height": 0
                    "score"： 0
                 }
                "attributes":
                {
                }
            }
        ]
    }
```

检测到2辆小汽车：

```
    {
         "vehicle_num": 2,
         "vehicle_info":
         [
            {
                "location":
                {
                    "left": 100,
                    "top": 200,
                    "width": 200,
                    "height": 400
                }
                "attributes":
                {
                    "vehicle_type":
                    {
                        "name": "小汽车",
                        "score": 0.852
                    }
                     "window_rain_eyebrow":
                    {
                        "score": 0.937
                    }
                    "roof_rack":
                    {
                        "score": 0.889
                    }
                    "skylight":
                    {
                        "score": 0.925
                    }
                    "in_car_item":
                    {
                        "score": 0.774
                    }
                    "rearview_item":
                    {
                        "score": 0.774
                    }
                    "copilot":
                    {
                        "score": 0.711
                    }
                    "driver_belt":
                    {
                        "score": 0.633
                    }
                    "copilot_belt":
                    {
                        "score": 0.721
                    }
                    "driver_visor":
                    {
                        "score": 0.455
                    }
                    "copilot_visor":
                    {
                        "score": 0.455
                    }
                    "direction":
                    {
                        "name": "正前方",
                        "score": 0.455
                    }
                }
            }
            {
                "location":
                {
                    "left": 400,
                    "top": 200,
                    "width": 200,
                    "height": 400
                }
                "attributes":
                {
                    "vehicle_type":
                    {
                        "name": "小汽车",
                        "score": 0.921
                    }
                    "window_rain_eyebrow":
                    {
                        "score": 0.918
                    }
                    "roof_rack":
                    {
                        "score": 0.850
                    }
                    "skylight":
                    {
                        "score": 0.714
                    }
                    "in_car_item":
                    {
                        "score": 0.787
                    }
                    "rearview_item":
                    {
                        "score": 0.787
                    }
                    "copilot":
                    {
                        "score": 0.711
                    }
                    "driver_belt":
                    {
                        "score": 0.633
                    }
                    "copilot_belt":
                    {
                        "score": 0.721
                    }
                    "driver_visor":
                    {
                        "score": 0.455
                    }
                    "copilot_visor":
                    {
                        "score": 0.455
                    }
                    "direction":
                    {
                        "name": "右前方",
                        "score": 0.455
                    }
                }
             }
         ]
    }
```

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
| 282000 | internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队。 |
| 216100 | invalid param                           | 请求中包含非法参数，请检查后重新尝试                         |
| 216101 | not enough param                        | 缺少必须的参数，请检查参数是否有遗漏                         |
| 216102 | service not support                     | 请求了不支持的服务，请检查调用的url                          |
| 216103 | param too long                          | 请求中某些参数过长，请检查后重新尝试                         |
| 216110 | appid not exist                         | appid不存在，请重新核对信息是否为后台应用列表中的appid       |
| 216200 | empty image                             | 图片为空，请检查后重新尝试                                   |
| 216201 | image format error                      | 上传的图片格式错误，现阶段我们支持的图片格式为：PNG、JPG、JPEG、BMP，请进行转码或更换图片 |
| 216202 | image size error                        | 上传的图片大小错误，现阶段我们支持的图片大小为：base64编码后小于4M，分辨率不高于4096*4096，请重新上传图片 |
| 216203 | image size error                        | 上传的图片base64编码有误，请校验base64编码方式，并重新上传图片 |
| 216630 | recognize error                         | 识别错误，请再次请求，如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队。 |
| 216631 | recognize bank card error               | 识别银行卡错误，出现此问题的原因一般为：您上传的图片非银行卡正面，上传了异形卡的图片或上传的银行卡正品图片不完整 |
| 216633 | recognize idcard error                  | 识别身份证错误，出现此问题的原因一般为：您上传了非身份证图片或您上传的身份证图片不完整 |
| 216634 | detect error                            | 检测错误，请再次请求，如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队。 |
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
| 336000 | Internal error                          | 服务器内部错误，请再次请求， 如果持续出现此类错误，请通过QQ群（659268104）或工单联系技术支持团队 |
| 336001 | Invalid Argument                        | 入参格式有误，比如缺少必要参数、图片base64编码错误等等，可检查下图片编码、代码格式是否有误。有疑问请通过QQ群（659268104）或工单联系技术支持团队 |