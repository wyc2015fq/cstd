# 智能写作  API参考

# 简介

Hi，您好，欢迎使用百度智能写作API服务。

本文档主要针对API开发者，描述百度智能写作平台接口服务的相关技术内容。如果您对文档内容有任何疑问，可以通过以下几种方式联系我们：

- 在百度云控制台内**提交工单**，咨询问题类型请选择**人工智能服务**；
- 如有疑问，进入[AI社区交流](http://ai.baidu.com/forum/topic/list/169)
- 或加入我们的官方QQ群进行相关问题讨论或咨询：QQ群号743926523
- 邀测阶段如遇到调用无权限问题，请联系客服QQ491837080

## 接口能力

| 接口名称 | 接口能力简要描述                             |
| :------- | :------------------------------------------- |
| 智能春联 | 根据输入关键词生成春联，包括上联、下联和横批 |
| 智能写诗 | 通过输入的命题关键词自动生成一首7言绝句      |

## 请求格式

POST方式调用

## 返回格式

JSON格式

## 调用方式

调用AI服务相关的API接口有两种调用方式，两种不同的调用方式采用相同的接口URL。

区别在于请求方式和鉴权方法不一样，请求参数和返回结果一致。

## 调用方式一

向API服务地址使用POST发送请求，必须在URL中带上参数：

**access_token:** 必须参数，参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)。

POST中参数按照API接口说明调用即可。

例如自然语言处理API，使用HTTPS POST发送：

```
https://aip.baidubce.com/rpc/2.0/nlp/v1/lexer?access_token=24.f9ba9c5241b67688bb4adbed8bc91dec.2592000.1485570332.282335-8574074
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

> **说明：** 方式一鉴权使用的Access_token必须通过API Key和Secret Key获取。

## 调用方式二

**请求头域内容**

NLP的API服务需要在请求的HTTP头域中包含以下信息：

- host（必填）
- x-bce-date （必填）
- x-bce-request-id（选填）
- authorization（必填）
- content-type（选填）
- content-length（选填）

作为示例，以下是一个标准的请求头域内容:

```
POST rpc/2.0/nlp/v1/wordseg? HTTP/1.1
accept-encoding: gzip, deflate
x-bce-date: 2015-03-24T13:02:00Z
connection: keep-alive
accept: */*
host: aip.baidubce.com
x-bce-request-id: 73c4e74c-3101-4a00-bf44-fe246959c05e
content-type: application/x-www-form-urlencoded;
authorization: bce-auth-v1/46bd9968a6194b4bbdf0341f2286ccce/2015-03-24T13:02:00Z/1800/host;x-bce-date/994014d96b0eb26578e039fa053a4f9003425da4bfedf33f4790882fb4c54903
```

> **说明：** 方式二鉴权使用的[API认证机制](https://cloud.baidu.com/doc/Reference/AuthenticationMechanism.html)authorization必须通过百度云的[AK/SK](https://cloud.baidu.com/doc/Reference/GetAKSK.html)生成。

# 智能春联接口

## 接口描述

根据用户输入的命题关键词自动生成一副春联，包括上联、下联和横批。

注：请使用方注意接口使用的合规性，不得利用该接口从事不合规场景的应用；如有违规使用，一经发现，AI开放平台有权立即停止提供服务

## 请求说明

**请求示例**

HTTP方法: `POST`

请求URL: `https://aip.baidubce.com/rpc/2.0/nlp/v1/couplets`

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body请求示例:

```
{ 
"text": "百度",
"index": 0
}
```

**请求参数**

| 参数  | 是否必选 | 类型   | 描述                                                         |
| :---- | :------- | :----- | :----------------------------------------------------------- |
| text  | 是       | string | 字符串（限5字符数以内）即春联的主题                          |
| Index | 否       | int    | 整数 默认为数值为0，即第一幅春联。每换一次，数值加1即可，一定数量后会返回之前的春联结果。 |

## 返回说明

**返回参数**

| 参数   | 类型   | 描述                            |
| :----- | :----- | :------------------------------ |
| log_id | uint64 | 请求唯一标识码                  |
| first  | string | 春联上联：成7-9个字的上联       |
| second | string | 春联下联：生成7-9个字的春联下联 |
| center | string | 春联横批：生成4个字的春联横批   |

> **提示：** 请求body为json字符串，无须对body进行urlencode（百分号编码）

**返回示例**

```
{
   "log_id": 7310552510652020090,
   "first": "喜气千年千里新",
   "second": "清风百度百花艳",
   "center": "千云祥集"
}
```

# 智能写诗接口

## 接口描述

根据用户输入的命题关键词自动生成一首7言绝句。

注：请使用方注意接口使用的合规性，不得利用该接口从事不合规场景的应用；如有违规使用，一经发现，AI开放平台有权立即停止提供服务

## 请求说明

**请求示例**

HTTP方法: `POST`

请求URL: `https://aip.baidubce.com/rpc/2.0/nlp/v1/poem

URL参数：

| 参数         | 值                                                           |
| :----------- | :----------------------------------------------------------- |
| access_token | 通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#/Auth)” |

Header如下：

| 参数         | 值               |
| :----------- | :--------------- |
| Content-Type | application/json |

Body请求示例:

```
{ 
"text": "百度",
"index": 0
}
```

**请求参数**

| 参数  | 是否必选 | 类型   | 描述                                                         |
| :---- | :------- | :----- | :----------------------------------------------------------- |
| text  | 是       | string | 字符串（限5字符数以内，）即作诗的主题                        |
| Index | 否       | int    | 整数 默认为数值为0，即第一首诗。每换一次，数值加1即可，一定数量后会返回之前的作诗结果 |

## 返回说明

**返回参数**

| 参数    | 类型   | 描述                     |
| :------ | :----- | :----------------------- |
| log_id  | uint64 | 请求唯一标识码           |
| title   | string | 诗名，即用户输入作诗主题 |
| content | string | 作诗结果：为一首7言绝句  |

**返回示例**

```
{
  "log_id": 7310552510652020090,
  "title":      "百度"
  "content": "神州百度庆元功
              华夏千秋唱大风
              伟业宏图开盛世
              红旗锦绣耀长空"
}
```

## 错误码

| 错误码 | 错误信息                  | 描述                                           |
| :----- | :------------------------ | :--------------------------------------------- |
| 52000  | 成功                      | 无                                             |
| 52001  | 请求超时                  | 重试                                           |
| 52002  | 系统错误                  | 重试                                           |
| 52003  | 未授权用户                | 检查您的appid是否正确                          |
| 52004  | 输入解析失败              | 检查输入编码格式，有无特殊字符，是否是json格式 |
| 52005  | 输入字段有误              | 是否包含text字段                               |
| 52006  | 输入文本长度有误          | 输入不能超过5个字符                            |
| 52007  | 输入文本包含政治&黄色内容 | 换输入文本                                     |
| 52008  | 后台服务返回错误          | 重试                                           |
| 54003  | 访问频率受限              | 请降低您的调用频率                             |
| 54100  | 查询接口参数为空          | 检查是否少传参数                               |
| 54102  | 无写诗结果                | 重试                                           |
| 2      | 后端连接超时              | 重试                                           |