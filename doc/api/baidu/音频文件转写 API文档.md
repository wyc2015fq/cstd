# 音频文件转写 API文档

# 概述

Hi，您好，欢迎使用百度智能呼叫中心音频文件转写云服务。针对呼叫中心开发者，本文档主要介绍了产品合作流程，以及基于REST API快速接入服务的相关技术说明。

智能呼叫中心提供的ASR模型为呼叫中心定制版，较通用电话场景下的ASR模型识别效果更佳。服务以云服务方式提供，企业通过请求REST API，向百度输入音频文件存储地址，百度将识别文本推送回企业的业务系统，企业可在此基础上实现离线质检等产品。

如果您对合作方式及文档内容有任何疑问，或者您需要深度定制服务，请通过文档下方“联系我们”提供方式找到我们。

## 合作流程

![img](http://bos.nj.bpc.baidu.com/v1/agroup/ba9e9b99751a0d53e07a95daa54526947dc55c34)

## 流程说明

- 免费服务申请
- 页面右上方“控制台”点击进入，登录百度账号，进入百度云管理后台页面。
- 左侧导航栏上方，依次选择 产品服务->人工智能->智能呼叫中心，进入应用“概览”页面。
- 服务申请。点击“创建应用”，“接口选择”已默认勾选完成，如无其他需求，无需勾选，完成其他资料后，点击最下方“立即创建”按钮。
- 应用成功则页面显示“创建完毕”，点击”返回应用列表”， 查看新创建应用详情，记录AppID，API Key，Secret Key。
- 从开发者语言角度出发，选择较低集成成本接入REST API，快速接入并试用服务。
- 付费开通 参考“免费账号和服务申请”流程，进入应用“概览”页面，查看服务列表，在所选服务最右侧点击「立即购买」。
- 商务合作提交
  进入AI开放平台，选择 产品服务 -> 智能呼叫中心 -> 呼叫中心语音识别 ，选择 “[商务合作](https://cloud.baidu.com/survey/BICCApply.html)”，填写您的联系方式及呼叫中心需求，我们将在2~3个工作日联系您。
- 商务谈判
  我们将为您提供产品方案咨询、技术方案咨询、价格咨询等服务，为您定制语音识别模型以提升特定场景的识别准确率； 确定商务合作后，就合同、具体定制费用等相关问题细则进行商务沟通，百度可根据您的调用量提供一定的折扣； 确定费用后，双方签订合同及支付，即可直接调用。在此过程中，百度可提供POC服务、联调服务等。

## 服务内容

- **特训呼叫中心语音识别模型**
  百度针对呼叫中心设备特征，特训大量电话数据，形成呼叫中心领域专属模型，为企业呼叫中心各类智能化产品提供精准的语音转文字服务。
- **低成本，高安全，音频文件语音识别服务**
  您可以使用本服务，将您业务通话的录音存储地址以分轨或单轨的形式发送至百度，百度获取并利用夜间识别，在每通电话识别后在每通电话识别完成后立即返回结果。以最低的成本满足您的业务需求，并保障高稳定性、安全性。
- **免费试用，快速验证效果**
  您可以通过「立即使用」入口，进入呼叫中心语音识别的控制台页面，根据您的业务需要创建App，获得API Key、Secret Key后，即可免费试用本服务；免费试用无时限，但识别总小时数有限制。
  免费试用标准输出：语音文件提交成功，进入免费等待队列，整点启动识别任务，返回结果时间受具体语音时长及免费任务队列排队情况决定
  付费标准输出：每日晚22:10启动识别当天上传的全部音频文件，每通电话识别完成后立即返回结果
  其他输出要求：您可以联系我们进行商务合作，我们将为您定制识别时间及返回时效
- **自助购买，直接调用**
  您可以通过「立即使用」入口，进入呼叫中心语音识别的控制台页面，在控制台页面下方找到本服务，右侧有「立即购买」入口，您可以付费开通更加稳定且识别小时数更多的服务。我们将在您提交付费后为你准备专属您的服务器，保障服务效率及您的数据安全。
- **线下商务合作，定制服务**
  您也可以通过「[商务合作](https://cloud.baidu.com/survey/BICCApply.html)」，进入表单提交页面，填写您的业务需求并提交。我们会在线下与您取得联系，确认购买量后，您可以获得商务合作优惠价。我们还可以为您训练打造专属的行业场景模型，提升特定场景的语音识别准确率，满足您的业务需求。
- **单双轨支持**
  本服务需要按照通话识别小时数进行购买。为达到更好的识别效果，我们建议您拆分通话双侧为两个音频文件，可按通话ID将两侧的两个音频文件发至百度，我们将为您还原双侧通话，并按通话时长计算识别小时数。如果您的业务仅需要识别一侧语音，也可只发送单侧音频文件；如果您的通话无法将两侧拆分为双轨录制音频文件，可将电话混音音频文件发送至百度。

# 快速开始

## 接入准备

- 参考“流程说明”—>“免费账号和服务申请”完成权限申请；
- 检查录音文件是否符合为wav或者pcm，非压缩，8000Hz采样率，16bits位深，波形音频格式
- 检查录音音频文件地址可被外网访问和下载

## 接口调用方式

- 使用应用申请的API Key、Secret Key获取Access Token,详见"获取Access Token"
- 使用获取的AccessToken调用上传请求接口,详见"上传请求接口"
- 使用获取的AccessToken调用结果查询接口,详见"结果查询接口"

## 获取Access Token

#### 请求URL数据格式

向授权服务地址https://aip.baidubce.com/oauth/2.0/token发送请求（推荐使用POST），并在URL中带上以下参数：

- grant_type： 必须参数，固定为client_credentials;
- client_id： 必须参数，应用的API Key;
- client_secret： 必须参数，应用的Secret Key;

例如：

```
https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=Va5yQRHlA4Fq5eR3LT0vuXV4&client_secret=0rDSjzQ20XUj5itV6WRtznPQSzr5pVw2&
```

获取access_token各开发语言示例代码请参考[accessToken获取](http://ai.baidu.com/docs#/Auth/top)

#### 服务器返回的JSON文本参数如下：

- access_token： 要获取的Access Token；
- expires_in： Access Token的有效期(秒为单位，一般为1个月)；
- 其他参数忽略，暂时不用;

例如：

```
{
  "refresh_token": "25.b55fe1d287227ca97aab219bb249b8ab.315360000.1798284651.282335-8574074",
  "expires_in": 2592000,
  "scope": "public wise_adapt",
  "session_key": "9mzdDZXu3dENdFZQurfg0Vz8slgSgvvOAUebNFzyzcpQ5EnbxbF+hfG9DQkpUVQdh4p6HbQcAiz5RmuBAja1JJGgIdJI",
  "access_token": "24.6c5e1ff107f0e8bcef8c46d3424a0e78.2592000.1485516651.282335-8574074",
  "session_secret": "dfac94a3489fe9fca7c3221cbf7525ff"
}
```

#### 若请求错误，服务器将返回的JSON文本包含以下参数：

error： 错误码；关于错误码的详细信息请参考下方鉴权认证错误码。 error_description： 错误描述信息，帮助理解和解决发生的错误。

#### 例如认证失败返回：

```
{
    "error": "invalid_client",
    "error_description": "unknown client id"
}
```

#### 鉴权认证错误码

| error          | error_description            | 解释             |
| :------------- | :--------------------------- | :--------------- |
| invalid_client | unknown client id            | API Key不正确    |
| invalid_client | Client authentication failed | Secret Key不正确 |

## 上传请求接口

### 获取识别翻译结果有两种方式：

- 采用轮询方式时，提交识别请求后，使用提交参数中appId,callId查询翻译结果。
- 采用回调方式时，提交识别请求的回调url。识别完成后会把结果发送到回调地址。

### 上传请求URL地址说明

| 协议  | URL                                                          | 方法                    | 参数       | 备注                                  |
| :---- | :----------------------------------------------------------- | :---------------------- | :--------- | :------------------------------------ |
| HTTPS | https://aip.baidubce.com/rpc/2.0/session/offline/upload/asr?access_token={accessToken} | POST（application/json) | json字符串 | accessToken获取详见"获取Access Token" |

### 输入参数列表

| 参数名        | 含义                                                         | 必选          | 值类型 |
| :------------ | :----------------------------------------------------------- | :------------ | :----- |
| appId         | 用户百度云申请appId                                          | 必选          | Long   |
| callId        | 唯一电话识别参数,建议使用UUID,不超过128位,业务方保证(appId,callId)联合唯一 | 必选          | String |
| companyName   | 录音所属公司                                                 | 必选          | String |
| agentFileUrl  | 用户销售侧文件存储URL或者单个文件的混音文件                  | 必选          | String |
| clientFileUrl | 用户客户侧文件存储URL                                        | 非必选        | String |
| callbackUrl   | 用户获取翻译结果回调接口,若填写则通过地址回调，若不填则须客户使用查询结果接口进行查询 | 非必选        | String |
| suffix        | 文件名后缀                                                   | wav或pcm 必选 | String |

### 注意事项

- agentFileUrl,clientFileUrl 链接有效性。 请保证下载链接可以被公网访问，关于https：请保证根证书有效且已加入到各公用根证书库。
- agentFileUrl,clientFileUrl 实际可下载文件流要求。请保证文件流满足单声道，8k采样率，16bits采样点，pcm（非pcma律或者pcmu律）语音流。 对文件不确定可用相应软件进行检验。 参考软件为 Adobe Audition CC，录音文件转换工具可参考[音频文件转码](https://ai.baidu.com/docs#/ASR-Tool-convert/top) ![img](http://agroup-bos.su.bcebos.com/cccb50b81d3e62cce5d2fd61a50ec63e8300fb21)
- callbackUrl链接有效性。请保证回调链接可以被公网访问。关于https：请保证根证书有效且已加入到各公用根证书库。

### 请求参数示例

```
输入双侧录音音频地址
{
    "appId":11309111,
    "companyName":"百度",
    "callId":"20e59200-57da-423e-b613-6a8ce126d0a2",
    "agentFileUrl":"http://xx.xxx.com/xxx",
    "clientFileUrl":"http://xx.xxx.com/xxx",
    "callbackUrl":"http://xx.xxx.com/xxx",
    "suffix":"wav"
}
或输入单侧录音音频地址
{
    "appId": 11309111,
    "companyName":"百度",
    "callId":"20e59200-57da-423e-b613-6a8ce126d0a2",
    "agentFileUrl":"http://xx.xxx.com/xxx",
    "callbackUrl":"http://xx.xxx.com/xxx",
    "suffix":"wav"
}
```

### 返回参数列表

| 参数名 | 含义       | 必选 | 值类型 |
| :----- | :--------- | :--- | :----- |
| status | 错误状态码 | 是   | Long   |
| msg    | 错误信息   | 是   | String |
| data   | 返回数据   | 是   | json   |

### 返回格式示例

```
正确状态返回
{
    "status": 0,
    "msg": "OK",
    "data": ""
}
错误状态返回
{
    "status": 50012,
    "msg": "callId长度大于128位,请修改callId",
    "data": ""
}
```

### 状态码列表

| status | msg                                  | 处理措施                                                     |
| :----- | :----------------------------------- | :----------------------------------------------------------- |
| 50000  | appId或callId无效                    | 请确认是否是申请音频文件转写应用的appId,详见"流程说明"或者callId是否有效 |
| 50001  | appId和callId联合不唯一,请修改callId | 请修改callId，保证唯一后重试                                 |
| 50008  | agentFileUrl参数非法,不是url格式     | 修改agentFileUrl参数后重试                                   |
| 50009  | clientFileUrl参数非法,不是url格式    | 修改clientFileUrl参数后重试                                  |
| 50010  | callbackUrl参数非法,不是url格式      | 修改callbackUrl参数后重试                                    |
| 50011  | suffix参数非法,不是wav或pcm          | 修改suffix参数为wav或pcm一种后重试                           |
| 50012  | callId长度大于128位,请修改callId     | 修改callId后重试                                             |
| 9999   | 系统错误                             | 通过联系我们进行反馈                                         |

## 结果查询接口

### 结果查询接口URL地址说明

| 协议  | URL                                                          | 方法                    | 参数       | 备注                                  |
| :---- | :----------------------------------------------------------- | :---------------------- | :--------- | :------------------------------------ |
| HTTPS | https://aip.baidubce.com/rpc/2.0/search/info?access_token={accessToken} | POST（application/json) | json字符串 | accessToken获取详见"获取Access Token" |

### 参数列表

| 参数     | 含义         | 必选 | 说明   |
| :------- | :----------- | :--- | :----- |
| category | 用户查询类型 | 必选 | String |
| paras    | 查询参数     | 必选 | json   |

### 请求参数示例

```
{
    "category":"OFFLINE_ASR_RESULT",
    "paras":{
        "appId": 11309111,                                        // 百度云appId,必选
        "callId":"20e59200-57da-423e-b613-6a8ce126d0a2"           //用户上传某一通的callId,必选
        }    
}
```

### 返回参数列表

| 参数名 | 含义       | 必选 | 值类型   |
| :----- | :--------- | :--- | :------- |
| status | 错误状态码 | 是   | Long     |
| msg    | 错误信息   | 是   | String   |
| data   | 返回数据   | 是   | json对象 |

### 返回格式示例

```
正确状态返回
{
    "status": 0,
    "msg": "OK",
    "data": {
         "content":"[{ \"sentence\":\"啊您好这边信用卡客服中心的宣传情况你要广发言的信用卡的需要吗？\", \"roleCategory\":\"client\", \"snStartTime\":\"00:01.600\", \"snStopTime\":\"00:07.200\", \"duration\":\"6\" } , { \"sentence\":\"哦广发银行的信用卡有在使用吗？\", \"roleCategory\":\"agent\", \"snStartTime\":\"00:08.960\", \"snStopTime\":\"00:12.320\", \"duration\":\"4\" }]"     
     }
}

错误状态返回
{
    "status": 50003,
    "msg": " 文件校验失败,请查看文件是否是8k,16bit采样率的wav或pcm格式",
    "data": ""
}
```

#### content数据参数列表

content为压缩后json字符串,文件转写的结果数据,当status为0时存在

| 参数名       | 含义               | 必选 | 值类型 |
| :----------- | :----------------- | :--- | :----- |
| sentence     | 语音片段翻译文本   | 是   | String |
| roleCategory | 客户还是销售侧标识 | 是   | String |
| snStartTime  | 语音片段开始时间   | 是   | String |
| snStopTime   | 语音片段结束时间   | 是   | String |
| duration     | 语音片段时长       | 是   | String |

### 返回格式示例

```
[
        {
            "sentence":"喂，喂可以听到吗？", //语音片段翻译文本
            "roleCategory":"client",      //客户还是销售侧标识
            "snStartTime":"00:00.160", //语音片段开始时间
            "snStopTime":"00:05.120", //语音片段结束时间
            "duration":"5"  //语音片段时长
        },
        {
            "sentence":"嗯先生您好呃这边是信用卡客服中心给您来电的我的工号是三零零五，冒昧问一下您这边有使用过广发银行的信用卡吗？没有吖吖，先生是这样的咱这边是这样的通知那您广发银行推出一张终身免年费的白金信用卡下卡了都能是两万到十万的全国一电话行取款都是免手续费啊，两个工作日呢会广发这我明白客户经理联系您您可以详细了解下，那您这边有谂过其他银行的信用卡用过半年时间的吗？",
            "roleCategory":"agent",
            "snStartTime":"00:05.440",
            "snStopTime":"00:35.359",
           "duration":"30" 
       }
]
```

### 状态码列表

| status | msg                                                       | 处理措施                                                     |
| :----- | :-------------------------------------------------------- | :----------------------------------------------------------- |
| 50000  | appId或callId无效                                         | 请确认是否是申请音频文件转写应用的appId,详见"流程说明"或者callId是否有效 |
| 50002  | 文件URL下载失败,请查看URL是否可被下载                     | 请确保URL是否可被下载后重试                                  |
| 50003  | 文件校验失败,请查看文件是否是8k,16bit采样率的wav或pcm格式 | 音频文件格式转换后重试                                       |
| 50004  | 上传成功，请耐心等待翻译结果                              | 耐心等待翻译结果                                             |
| 50005  | 正在翻译，请耐心等待翻译结果                              | 耐心等待翻译结果                                             |
| 50006  | 正在处于翻译任务队列中，请耐心等待翻译结果                | 耐心等待翻译结果                                             |
| 50007  | 翻译过程出现异常                                          | 通过联系我们进行反馈                                         |
| 9999   | 系统错误                                                  | 通过联系我们进行反馈                                         |

## 百度侧回调给业务方识别结果

地址：（业务方上传离线文件url请求的中callbackUrl）

请求方式 POST， application/json

### 参数列表

| 参数        | 含义                                                        | 必选 | 值类型    |
| :---------- | :---------------------------------------------------------- | :--- | :-------- |
| appId       | 标识该知识属于哪个app                                       | 是   | Long      |
| userId      | 用户                                                        | 是   | Long      |
| callId      | 标识该知识属于哪通电话                                      | 是   | String    |
| logId       | 日志追查id                                                  | 是   | String    |
| category    | TXT，识别文本; REPORT(报告)，任务结束后告诉此批任务完成状况 | 是   | String    |
| content     | 返回的知识的内容                                            | 是   | String    |
| triggerTime | 触发这个知识的时间                                          | 是   | timestamp |

### 返回格式示例：

```
{
    "status": 0,
    "msg": "ok",
    "data": {
      "appId":11309111,
      "userId": 11309111,
      "callId":"20e59200-57da-423e-b613-6a8ce126d0a2",
      "category":"TXT",
      "logId":"20e59200-57da-423e-b613-6a8ce126d0a3",
      "content":"[{ \"sentence\":\"啊您好这边信用卡客服中心的宣传情况你要广发言的信用卡的需要吗？\", \"roleCategory\":\"agent\", \"snStartTime\":\"00:01.600\", \"snStopTime\":\"00:07.200\", \"duration\":\"6\" } , { \"sentence\":\"哦广发银行的信用卡有在使用吗？\", \"roleCategory\":\"agent\", \"snStartTime\":\"00:08.960\", \"snStopTime\":\"00:12.320\", \"duration\":\"4\" }]",
       "triggerTime":1531376960047
	}
}
或
{
    "status": 0,
    "msg": "ok",
    "data": {
      "appId":11309111,
      "userId": 11309111,
      "callId":"20e59200-57da-423e-b613-6a8ce126d0a2",
      "category":"REPORT",
      "logId":"20e59200-57da-423e-b613-6a8ce126d0a3",
      "content":"{\"success\":{\"count\":10},\"fail\":{\"download\":[\"20e59200-57da-423e-b613-6a8ce126d0a3\",\"20e59200-57da-423e-b613-6a8ce126d0a4\"],\"format\":[\"20e59200-57da-423e-b613-6a8ce126d0a5\",\"20e59200-57da-423e-b613-6a8ce126d0a6\"],\"other\":[\"20e59200-57da-423e-b613-6a8ce126d0a7\",\"20e59200-57da-423e-b613-6a8ce126d0a8\"]}}",
       "triggerTime":1531376960047
	}
}
```

### roleCategory 为TXT 时：content字段示例

| 参数名       | 含义               | 必选 | 值类型 |
| :----------- | :----------------- | :--- | :----- |
| sentence     | 语音片段翻译文本   | 是   | String |
| roleCategory | 客户还是销售侧标识 | 是   | String |
| snStartTime  | 语音片段开始时间   | 是   | String |
| snStopTime   | 语音片段结束时间   | 是   | String |
| duration     | 语音片段时长       | 是   | String |

### 返回格式示例：

```
content为压缩后json字符串
"content":"[{ \"sentence\":\"啊您好这边信用卡客服中心的宣传情况你要广发言的信用卡的需要吗？\", \"roleCategory\":\"agent\", \"snStartTime\":\"00:01.600\", \"snStopTime\":\"00:07.200\", \"duration\":\"6\" } , { \"sentence\":\"哦广发银行的信用卡有在使用吗？\", \"roleCategory\":\"agent\", \"snStartTime\":\"00:08.960\", \"snStopTime\":\"00:12.320\", \"duration\":\"4\" }]"
其中的json字符串示例
[
        {
            "sentence":"喂，喂可以听到吗？", //语音片段翻译文本
            "roleCategory":"client",      //客户还是销售侧标识
            "snStartTime":"00:00.160", //语音片段开始时间
            "snStopTime":"00:05.120", //语音片段结束时间
            "duration":"5"  //语音片段时长
        },
        {
            "sentence":"嗯先生您好呃这边是信用卡客服中心给您来电的我的工号是三零零五，冒昧问一下您这边有使用过广发银行的信用卡吗？没有吖吖，先生是这样的咱这边是这样的通知那您广发银行推出一张终身免年费的白金信用卡下卡了都能是两万到十万的全国一电话行取款都是免手续费啊，两个工作日呢会广发这我明白客户经理联系您您可以详细了解下，那您这边有谂过其他银行的信用卡用过半年时间的吗？",
            "roleCategory":"agent",
            "snStartTime":"00:05.440",
            "snStopTime":"00:35.359",
            "duration
       }
]
```

### roleCategory 为REPORT 时（任务结束后告诉此批任务完成状况）,content字段示例：

#### success字段参数列表

| 参数名 | 含义     | 必选 | 值类型 |
| :----- | :------- | :--- | :----- |
| count  | 成功个数 | 是   | Long   |

#### fail字段参数列表

| 参数名   | 含义                                             | 必选 | 值类型   |
| :------- | :----------------------------------------------- | :--- | :------- |
| download | 语音片段翻译文本                                 | 是   | String[] |
| format   | 文件格式错误导致的翻译失败的callId数组           | 是   | String[] |
| other    | 其他失败原因， 如 网络抖动等原因导致，可具体分析 | 是   | String[] |

### 返回格式示例：

```
content为压缩后json字符串
 "content":"{\"success\":{\"count\":10},\"fail\":{\"download\":[\"20e59200-57da-423e-b613-6a8ce126d0a3\",\"20e59200-57da-423e-b613-6a8ce126d0a4\"],\"format\":[\"20e59200-57da-423e-b613-6a8ce126d0a5\",\"20e59200-57da-423e-b613-6a8ce126d0a6\"],\"other\":[\"20e59200-57da-423e-b613-6a8ce126d0a7\",\"20e59200-57da-423e-b613-6a8ce126d0a8\"]}}"
其中的json字符串示例
{
    "success":{
	    "count":10   //成功个数
	 },
    "fail": {
        "download": [
           "20e59200-57da-423e-b613-6a8ce126d0a3",
           "20e59200-57da-423e-b613-6a8ce126d0a4"
        ],         // 文件下载失败导致的翻译失败的callId数组
        "format": [ 
           "20e59200-57da-423e-b613-6a8ce126d0a5",
           "20e59200-57da-423e-b613-6a8ce126d0a6"
        ],        // 文件格式错误导致的翻译失败的callId数组
        "other": [ 
           "20e59200-57da-423e-b613-6a8ce126d0a7",
           "20e59200-57da-423e-b613-6a8ce126d0a8"
        ]        // 其他失败原因， 如 网络抖动等原因导致，可具体分析 
    }
}
```

# 联系我们

- [商务合作](https://cloud.baidu.com/survey/BICCApply.html)反馈；
- [工单系统](http://ticket.bce.baidu.com/?_=1530034880766/#/ticket/list)反馈，工单类型为“人工智能”->“智能呼叫中心”；
- 电话：010-50802520（10:00~18:00）；
- 邮箱：ics-bd@baidu.com；
- 社区：[智能呼叫中心](http://ai.baidu.com/forum/topic/list/196)；









# BICC-OFFLINE-ASR-JAVA-SDK文档

# 概述

Hi，您好，欢迎使用百度智能呼叫中心音频文件转写云服务。针对呼叫中心开发者，本文档主要介绍了产品合作流程，以及基于REST API快速接入服务的相关技术说明。

智能呼叫中心提供的ASR模型为呼叫中心定制版，较通用电话场景下的ASR模型识别效果更佳。服务以云服务方式提供，企业通过请求JAVA SDK，向百度输入音频文件存储地址，百度将识别文本推送回企业的业务系统，企业可在此基础上实现离线质检等产品。

如果您对合作方式及文档内容有任何疑问，或者您需要深度定制服务，请通过文档下方“联系我们”提供的方式找到我们。

## 合作流程

![img](http://bos.nj.bpc.baidu.com/v1/agroup/ba9e9b99751a0d53e07a95daa54526947dc55c34)

## 流程说明

- 免费服务申请
  - 进入[百度AI开放平台](https://ai.baidu.com/)，页面右上方“控制台”点击进入，登录百度账号，进入百度云管理后台页面。
  - 左侧导航栏上方，依次选择 产品服务->人工智能->智能呼叫中心，进入应用“概览”页面。
  - 服务申请。点击“创建应用”，“接口选择”已默认勾选完成，如无其他需求，无需勾选，完成其他资料后，点击最下方“立即创建”按钮。
  - 应用成功则页面显示“创建完毕”，点击”返回应用列表”， 查看新创建应用详情，记录AppID，API Key，Secret Key。
  - 从开发者语言角度出发，选择较低集成成本接入JAVA SDK，快速接入并试用服务。
- 付费开通
  参考“免费账号和服务申请”流程，进入应用“概览”页面，查看服务列表，在所选服务最右侧点击「立即购买」。
- 商务合作提交
  进入[百度AI开放平台](https://ai.baidu.com/)，选择 产品服务->语音技术->智能呼叫中心，选择 “[商务合作](https://cloud.baidu.com/survey/BICCApply.html)”，填写您的联系方式及呼叫中心需求，我们将在2~3个工作日联系您。
- 商务谈判
  我们将为您提供产品方案咨询、技术方案咨询、价格咨询等服务，为您定制语音识别模型以提升特定场景的识别准确率。
  确定商务合作后，就合同、具体定制费用等相关问题细则进行商务沟通，百度可根据您的调用量提供一定的折扣。
  确定费用后，双方签订合同及支付，即可直接调用。在此过程中，百度可提供POC服务、联调服务等。

## 服务内容

- **特训呼叫中心语音识别模型**
  百度针对呼叫中心设备特征，特训大量电话数据，形成呼叫中心领域专属模型，为企业呼叫中心各类智能化产品提供精准的语音转文字服务。
- **低成本，高安全，音频文件语音识别服务**
  您可以使用本服务，将您业务通话的录音存储地址以分轨或单轨的形式发送至百度，百度获取并利用夜间识别，在每通电话识别后在每通电话识别完成完成后立即返回结果。以最低的成本满足您的业务需求，并保障高稳定性、安全性。
- **免费试用，快速验证效果**
  您可以通过「立即使用」入口，进入呼叫中心语音识别的控制台页面，根据您的业务需要创建App，获得API Key、Secret Key后，即可免费试用本服务；免费试用无时限，但识别总小时数有限制，免费识别总小时数为10小时。
  免费试用标准输出：语音文件提交成功，进入免费等待队列，整点启动识别任务，返回结果时间受具体语音时长及免费任务队列排队情况决定。 付费标准输出：每日晚22:10启动识别当天上传的全部音频文件，每通电话识别完成后立即返回结果。
  其他输出要求：您可以联系我们进行商务合作，我们将为您定制识别时间及返回时效。
- **自助购买，直接调用**
  您可以通过「立即使用」入口，进入呼叫中心语音识别的控制台页面，在控制台页面下方找到本服务，右侧有「立即购买」入口，您可以付费开通更加稳定且识别小时数更多的服务。我们将在您提交付费后为你准备专属您的服务器，保障服务效率及您的数据安全。
- **线下商务合作，定制服务**
  您也可以通过「[商务合作](https://cloud.baidu.com/survey/BICCApply.html)」，进入表单提交页面，填写您的业务需求并提交。我们会在线下与您取得联系，确认购买量后，您可以获得商务合作优惠价。我们还可以为您训练打造专属的行业场景模型，提升特定场景的语音识别准确率，满足您的业务需求。
- **单双轨支持**
  本服务需要按照通话识别小时数进行购买。为达到更好的识别效果，我们建议您拆分通话双侧为两个音频文件，可按通话ID将两侧的两个音频文件发至百度，我们将为您还原双侧通话，并按通话时长计算识别小时数。如果您的业务仅需要识别一侧语音，也可只发送单侧音频文件；如果您的通话无法将两侧拆分为双轨录制音频文件，可将电话混音音频文件发送至百度，但是返回的结果将无法区分双侧。

# 快速开始

## 接入准备

- 参考“流程说明”—>“免费账号和服务申请”完成权限申请，获取appId,apiKey,secretKey。
- 检查录音文件是否符合为wav或者pcm，非压缩，8000Hz采样率，16bits位深，波形音频格式。
- 检查录音音频文件地址可被外网访问和下载。

## SDK目录

```
${SDKROOT}
|--- bicc-offline-asr-java-demo
	|--- src
		|--- main
			|--- java
			|--- resource
				|-- bicc-offline-asr-sdk-java-1.0.0.jar
	|--- pom.xml
	|--- BICC-OFFLINE-ASR-JAVA-SDK.md
	|--- README.md
	
```

压缩包内为bicc-offline-asr-java-demo maven工程，SDK jar包在src/main/resources/目录下，sdk 的引入方式如下：

```
<!--本地bicc-offline-asr-sdk-java-1.0.0.jar-->
<dependency>
    <groupId>com.baidu.aip.bicc</groupId>
    <artifactId>bicc-offline-asr-sdk-java</artifactId>
    <version>1.0.0-SNAPSHOT</version>
    <scope>system</scope>
    <systemPath>${project.basedir}/src/main/resource/bicc-offline-asr-sdk-java-1.0.0.jar
    </systemPath>
</dependency>
 <!--相关依赖开始-->
<dependency>
    <groupId>com.baidu.aip</groupId>
    <artifactId>java-sdk</artifactId>
    <version>4.5.0</version>
</dependency>
<dependency>
    <groupId>com.fasterxml.jackson.core</groupId>
    <artifactId>jackson-core</artifactId>
    <version>2.9.6</version>
    <scope>compile</scope>
</dependency>
<dependency>
    <groupId>com.fasterxml.jackson.core</groupId>
    <artifactId>jackson-databind</artifactId>
    <version>2.9.6</version>
</dependency>
<dependency>
    <groupId>commons-validator</groupId>
    <artifactId>commons-validator</artifactId>
    <version>1.6</version>
    <exclusions>
        <exclusion>
	    <groupId>commons-beanutils</groupId>
		<artifactId>commons-beanutils</artifactId>
        </exclusion>
        <exclusion>
		<groupId>commons-digester</groupId>
		<artifactId>commons-digester</artifactId>
        </exclusion>
        <exclusion>
		<groupId>commons-beanutils</groupId>
		<artifactId>commons-beanutils</artifactId>
        </exclusion>
        <exclusion>
		<groupId>commons-logging</groupId>
		<artifactId>commons-logging</artifactId>
        </exclusion>
        <exclusion>
		<groupId>commons-collections</groupId>
		<artifactId>commons-collections</artifactId>
        </exclusion>
    </exclusions>
</dependency>
<!--相关依赖结束-->
```

用户使用IDE直接导入工程即可运行：

- IDEA 自带maven，直接导入工程即可运行
- Eclipse 首先确定已经安装配置好maven插件，然后通过 File->import->Existing Maven Projects 的方式导入工程

## SDK调用顺序

1、创建一个BICCAsrUploadClient实例

```
BICCAsrUploadClient biccAsrUploadClient = new BICCAsrUploadClient(appId, apiKey, secretKey);
```

2、调用实例中asr()上传接口,进行文件URL上传

```
 UpLoadResponse upLoadResponse = biccAsrUploadClient.asr(callId, companyName, agentFileUrl, clientFileUrl,suffix, callbackUrl);         
 或者
 UpLoadRequest upLoadRequest = new UpLoadRequest(callId, companyName, agentFileUrl, clientFileUrl, suffix, callbackUrl);
 UpLoadResponse upLoadResponse = biccAsrUploadClient.asr(upLoadRequest);
```

3、调用search()查看上传结果接口,进行结果查询

```
UpLoadResponse searchResponse = biccAsrUploadClient.search(callId);
```

## SDK调用注意事项

1、获取结果方式

- 采用轮询方式时，使用asr()提交识别请求后，使用提交参数中callId通过search()查询翻译结果。
- 采用回调方式时，使用asr()提交识别参数输入callbackUrl。识别完成后会把结果发送到回调地址。

2、翻译时效性

- 免费试用标准输出：语音文件提交成功，进入免费等待队列，整点启动识别任务，返回结果时间受具体语音时长及免费任务队列排队情况决定。
- 付费标准输出：每日晚22:10启动识别当天上传的全部音频文件，每通电话识别完成后立即返回结果。
- 其他输出要求：您可以联系我们进行商务合作，我们将为您定制识别时间及返回时效。

3、保证网络畅通

- SDK内部是通过REST API进行上传，请保证运行服务机器能够访问公网，另若采取回调模式请保证回调地址可被公网访问

## SDK目录

## 重要接口说明

```
com.baidu.aip.bicc.offline.asr.BICCAsrUploadClient;
```

JAVA SDK对外暴露的实例类，调用程序通过调用该类的有参数构造函数、asr()、search()进行初始化实例、文件URL上传及上传结果查询。

### 初始化实例

```
BICCAsrUploadClient biccAsrUploadClient = new BICCAsrUploadClient(appId, apiKey, secretKey);
 /**
     * 有参构造函数
     *
     * @param appId       百度云->人工智能->智能呼叫中心,申请的应用的appId,必填
     * @param apiKey      百度云->人工智能->智能呼叫中心,申请的应用的apiKey,必填
     * @param secretKey   百度云->人工智能->智能呼叫中心,申请的应用的secretKey,必填
     */
    public BICCAsrUploadClient(String appId, String apiKey, String secretKey) 
     
```

- 说明: BICCAsrUploadClient 有参数构造函数
- 参数:设置appId,apiKey,secretKey

### 调用文件URL上传接口

#### 注意事项

- agentFileUrl,clientFileUrl 链接有效性。 请保证下载链接可以被公网访问，关于https：请保证根证书有效且已加入到各公用根证书库。
- agentFileUrl,clientFileUrl 实际可下载文件流要求。请保证文件流满足单声道，8k采样率，16bits采样点，pcm（非pcma律或者pcmu律）语音流。 对文件不确定可用相应软件进行检验。 参考软件为 Adobe Audition CC，录音文件转换工具可参考[音频文件转码](https://ai.baidu.com/docs#/ASR-Tool-convert/top) ![img](http://agroup-bos.su.bcebos.com/cccb50b81d3e62cce5d2fd61a50ec63e8300fb21)
- callbackUrl链接有效性。请保证回调链接可以被公网访问。关于https：请保证根证书有效且已加入到各公用根证书库。

\####上传接口有两种使用方法：
1、使用 asr(String callId, String companyName, String agentFileUrl, String clientFileUrl,String suffix, String callbackUrl)

```
    /**
     * 文件URL上传接口
     *
     * @param callId        必须   唯一电话识别参数,建议使用UUID,不超过128位,业务方保证(appId,callId)联合唯一,若为空则系统采取默认生成的UUID
     * @param companyName   必须(NotNull)    录音所属公司
     * @param agentFileUrl  必须(NotNull)    销售侧文件存储URL或者单个文件的混音文件URL
     * @param clientFileUrl 非必须(可为Null)  客户侧文件存储URL
     * @param suffix        必须(NotNull)    wav或pcm
     * @param callbackUrl   非必须(可为Null)  用户获取翻译结果回调接口,若NotNull则通过地址回调，若为Null客户使用查询结果接口进行查询
     *
     * @return UpLoadResponse
     *
     * @throws JsonProcessingException
     * @throws IOException
     * @throws ParamException
     */
    public UpLoadResponse asr(String callId, String companyName, String agentFileUrl, String clientFileUrl,String suffix, String callbackUrl)throws JsonProcessingException, IOException, ParamException
```

- 说明 : 文件URL上传接口
- 参数 : 设置callId, companyName, agentFileUrl, clientFileUrl, suffix, callbackUrl
- 返回值 : UpLoadResponse
- 异常 : JsonProcessingException, JsonProcessingException, ParamException

2、使用asr(UpLoadRequest upLoadRequest)

```
    /**
     * 上传识别接口
     *
     * @param upLoadRequest 必须
     *
     * @return UpLoadResponse
     *
     * @throws JsonProcessingException
     * @throws IOException
     * @throws ParamException
     */
public UpLoadResponse asr(UpLoadRequest upLoadRequest) throws JsonProcessingException, IOException,ParamException
```

- 说明 : 文件URL上传接口
- 参数 : 设置UpLoadRequest
- 返回值 : UpLoadResponse
- 异常 : JsonProcessingException, JsonProcessingException, ParamException

其中UpLoadRequest类说明

```
    com.baidu.aip.bicc.offline.common.beans.upload.UpLoadRequest
    /**
     * 构造函数
     *
     * @param callId        必须  唯一电话识别参数,建议使用UUID,不超过128位,业务方保证(appId,callId)联合唯一,若为空则系统采取默认生成的UUID
     * @param companyName   必须(NotNull)    录音所属公司
     * @param agentFileUrl  必须(NotNull)    销售侧文件存储URL或者单个文件的混音文件URL
     * @param clientFileUrl 非必须(可为Null)  客户侧文件存储URL
     * @param suffix        必须(NotNull)    wav或pcm
     * @param callbackUrl   非必须(可为Null)  用户获取翻译结果回调接口,若NotNull则通过地址回调，若为Null客户使用查询结果接口进行查询
     */
    public UpLoadRequest(String callId, String companyName, String agentFileUrl, String clientFileUrl,String suffix, String callbackUrl)
```

### 查询上传结果接口

```
    /**
     * 查询上传后结果
     *
     * @param callId  必须(NotNull)  文件URL上传中使用的callId
     *
     * @return UpLoadResponse
     *
     * @throws JsonProcessingException
     * @throws IOException
     * @throws ParamException
     */
    public UpLoadResponse search(String callId) throws JsonProcessingException, IOException, ParamException
```

### 上传结果响应UpLoadResponse类说明

```
package com.baidu.aip.bicc.offline.common.beans.upload;

import java.util.List;

import com.baidu.aip.bicc.offline.common.consts.StatusCodeCategory;

/**
 * UpLoadResponse bean
 * Date: 2018/07/28
 * Time: 15:42
 */
public class UpLoadResponse {
    /**
     * 返回状态枚举 statusCode
     */
    private StatusCodeCategory statusCode;
    /**
     * asr识别内容 asrContents
     */
    private List<AsrContent> asrContents;
    /**
     * 上传文件相应的请求 upLoadRequest
     */
    private UpLoadRequest upLoadRequest;
}
```

#### 返回结果转成json示例

```
文件URL上传接口返回结果：

{
    "statusCode":"ERR_NONE",
    "upLoadRequest":{
        "appId":11534636,
        "callId":"68418f41-3750-4ad4-ba76-425ee712c381",
        "companyName":"百度",
        "agentFileUrl":"http://10.99.207.78:8901/100_rd_only_client/380d7b58-9567-4388-95a7-97b2551fab11_AGENT.pcm",
        "suffix":"pcm"
    }
}
或者
{
    "statusCode":"ERR_UPLOAD_FILE_AGENT_URL_INVALID",
    "upLoadRequest":{
        "appId":11534636,
        "callId":"5b0517c9-e6f3-4fab-a8d6-d314518ab203",
        "companyName":"百度",
        "agentFileUrl":"http://10.99.207.78:8901/100_rd_only_client/380d 7b58-9567-4388-95a7-97b2551fab11_AGENT.pcm",
        "suffix":"pcm"
    }
}
查询上传结果接口返回结果：

{
    "statusCode":"ERR_UPLOAD_FILE_SUCCESS",
    "upLoadRequest":{
        "appId":11534636,
        "callId":"68418f41-3750-4ad4-ba76-425ee712c381"
    }
}
或者
{
    "statusCode":"ERR_UPLOAD_FILE_DOING",
    "upLoadRequest":{
        "appId":11534636,
        "callId":"68418f41-3750-4ad4-ba76-425ee712c381"
    }
}
或者
{
    "statusCode":"ERR_NONE",
    "asrContents":[
        {
            "sentence":"喂您好喂您好这里是客户客服顾问工号二零零六二是张先生吗？",
            "roleCategory":"agent",
            "snStartTime":"00:01.120",
            "snStopTime":"00:09.119",
            "duration":"8"
        },
        {
            "sentence":"对对对对嗯，",
            "roleCategory":"agent",
            "snStartTime":"00:20.00",
            "snStopTime":"00:21.440",
            "duration":"1"
        }
    ],
    "upLoadRequest":{
        "appId":11534636,
        "callId":"9d8fc8e9-e99f-4528-b6f5-1a3a8bcb9a6c"
    }
}
```

### 状态码枚举StatusCodeCategory类说明

```
package com.baidu.aip.bicc.offline.common.consts;

/**
 * 返回结果的 statuscode 枚举类
 * Date: 2018/07/27
 * Time: 21:00
 */
public enum StatusCodeCategory {
    ERR_NONE(0, "OK"),
    ERR_UPLOAD_FILE_INVALID_APPID_CALLID(50000, "appId或callId无效"),
    ERR_UPLOAD_FILE_CALLID_UNIQUE(50001, "appId和callId联合不唯一,请修改callId"),
    ERR_UPLOAD_FILE_DOWNLOAD(50002, "文件URL下载失败,请查看URL是否可被下载"),
    ERR_UPLOAD_FILE_FORMAT(50003, "文件校验失败,请查看文件是否是8k,16bit采样率的wav或pcm格式"),
    ERR_UPLOAD_FILE_SUCCESS(50004, "上传成功，请耐心等待翻译结果"),
    ERR_UPLOAD_FILE_DOING(50005, "正在翻译，请耐心等待翻译结果"),
    ERR_UPLOAD_FILE_QUEUE(50006, "正在处于翻译任务队列中，请耐心等待翻译结果"),
    ERR_UPLOAD_FILE_EXCEPTION(50007, "翻译过程出现异常"),
    ERR_UPLOAD_FILE_AGENT_URL_INVALID(50008, "agentFileUrl参数非法,不是url格式"),
    ERR_UPLOAD_FILE_CLIENT_URL_INVALID(50009, "clientFileUrl参数非法,不是url格式"),
    ERR_UPLOAD_FILE_CALLBACK_URL_INVALID(50010, "callbackUrl参数非法,不是url格式"),
    ERR_UPLOAD_FILE_SUFFIX_INVALID(50011, "suffix参数非法,不是wav或pcm"),
    ERR_UPLOAD_FILE_CALLID_INVALID(50012, "callId长度大于128位,请修改callId"),
    ERR_SYSTEM(9999, "系统错误");
 }
```

### 识别结果AsrContent类说明

```
package com.baidu.aip.bicc.offline.common.beans.upload;

/**
 * AsrContent bean
 * Date: 2018/07/28
 * Time: 15:43
 */
public class AsrContent {
    /**
     * asr识别整句内容
     */
    private String sentence;
    /**
     * asr识别整句角色
     */
    private String roleCategory;
    /**
     * asr识别整句开始时间,精确度为毫秒
     */
    private String snStartTime;
    /**
     * asr识别整句结束时间,精确度为毫秒
     */
    private String snStopTime;
    /**
     * asr识别整句持续时间,单位为秒
     */
    private String duration;
}
```

### 百度侧回调给业务方识别结果

地址：（业务方上传离线文件时指定的callbackUrl）

请求方式 POST， application/json

### 参数列表

| 参数        | 含义                                                        | 必选 | 值类型    |
| :---------- | :---------------------------------------------------------- | :--- | :-------- |
| appId       | 标识该知识属于哪个app                                       | 是   | Long      |
| userId      | 用户                                                        | 是   | Long      |
| callId      | 标识该知识属于哪通电话                                      | 是   | String    |
| logId       | 日志追查id                                                  | 是   | String    |
| category    | TXT，识别文本; REPORT(报告)，任务结束后告诉此批任务完成状况 | 是   | String    |
| content     | 返回的知识的内容                                            | 是   | String    |
| triggerTime | 触发这个知识的时间                                          | 是   | timestamp |

### 百度侧回调请求json示例

```
{
      "appId":11309111,
      "userId": 11309111,
      "callId":"20e59200-57da-423e-b613-6a8ce126d0a2",
      "category":"TXT",
      "logId":"20e59200-57da-423e-b613-6a8ce126d0a3",
      "content":"[{ \"sentence\":\"啊您好这边信用卡客服中心的宣传情况你要广发言的信用卡的需要吗？\", \"roleCategory\":\"agent\", \"snStartTime\":\"00:01.600\", \"snStopTime\":\"00:07.200\", \"duration\":\"6\" } , { \"sentence\":\"哦广发银行的信用卡有在使用吗？\", \"roleCategory\":\"agent\", \"snStartTime\":\"00:08.960\", \"snStopTime\":\"00:12.320\", \"duration\":\"4\" }]",
      "triggerTime":1531376960047
}
或者
{
      "appId":11309111,
      "userId": 11309111,
      "callId":"20e59200-57da-423e-b613-6a8ce126d0a2",
      "category":"REPORT",
      "logId":"20e59200-57da-423e-b613-6a8ce126d0a3",
      "content":"{\"success\":{\"count\":10},\"fail\":{\"download\":[\"20e59200-57da-423e-b613-6a8ce126d0a3\",\"20e59200-57da-423e-b613-6a8ce126d0a4\"],\"format\":[\"20e59200-57da-423e-b613-6a8ce126d0a5\",\"20e59200-57da-423e-b613-6a8ce126d0a6\"],\"other\":[\"20e59200-57da-423e-b613-6a8ce126d0a7\",\"20e59200-57da-423e-b613-6a8ce126d0a8\"]}}",
      "triggerTime":1531376960047
}
```

### category 为TXT 时：content字段示例

| 参数名       | 含义               | 必选 | 值类型 |
| :----------- | :----------------- | :--- | :----- |
| sentence     | 语音片段翻译文本   | 是   | String |
| roleCategory | 客户还是销售侧标识 | 是   | String |
| snStartTime  | 语音片段开始时间   | 是   | String |
| snStopTime   | 语音片段结束时间   | 是   | String |
| duration     | 语音片段时长       | 是   | String |

```
content为压缩后json字符串
"content":"[{ \"sentence\":\"啊您好这边信用卡客服中心的宣传情况你要广发言的信用卡的需要吗？\", \"roleCategory\":\"agent\", \"snStartTime\":\"00:01.600\", \"snStopTime\":\"00:07.200\", \"duration\":\"6\" } , { \"sentence\":\"哦广发银行的信用卡有在使用吗？\", \"roleCategory\":\"agent\", \"snStartTime\":\"00:08.960\", \"snStopTime\":\"00:12.320\", \"duration\":\"4\" }]"
其中的json字符串示例
[
        {
            "sentence":"喂，喂可以听到吗？", //语音片段翻译文本
            "roleCategory":"client",      //客户还是销售侧标识
            "snStartTime":"00:00.160", //语音片段开始时间
            "snStopTime":"00:05.120", //语音片段结束时间
            "duration":"5"  //语音片段时长
        },
        {
            "sentence":"嗯先生您好呃这边是信用卡客服中心给您来电的我的工号是三零零五，冒昧问一下您这边有使用过广发银行的信用卡吗？没有吖吖，先生是这样的咱这边是这样的通知那您广发银行推出一张终身免年费的白金信用卡下卡了都能是两万到十万的全国一电话行取款都是免手续费啊，两个工作日呢会广发这我明白客户经理联系您您可以详细了解下，那您这边有谂过其他银行的信用卡用过半年时间的吗？",
            "roleCategory":"agent",
            "snStartTime":"00:05.440",
            "snStopTime":"00:35.359",
            "duration
       }
]
```

### category 为REPORT 时（任务结束后告诉此批任务完成状况）,content字段示例：

#### success字段参数列表

| 参数名 | 含义     | 必选 | 值类型 |
| :----- | :------- | :--- | :----- |
| count  | 成功个数 | 是   | Long   |

#### fail字段参数列表

| 参数名   | 含义                                             | 必选 | 值类型   |
| :------- | :----------------------------------------------- | :--- | :------- |
| download | 语音片段翻译文本                                 | 是   | String[] |
| format   | 文件格式错误导致的翻译失败的callId数组           | 是   | String[] |
| other    | 其他失败原因， 如 网络抖动等原因导致，可具体分析 | 是   | String[] |

### 格式示例：

```
content为压缩后json字符串
 "content":"{\"success\":{\"count\":10},\"fail\":{\"download\":[\"20e59200-57da-423e-b613-6a8ce126d0a3\",\"20e59200-57da-423e-b613-6a8ce126d0a4\"],\"format\":[\"20e59200-57da-423e-b613-6a8ce126d0a5\",\"20e59200-57da-423e-b613-6a8ce126d0a6\"],\"other\":[\"20e59200-57da-423e-b613-6a8ce126d0a7\",\"20e59200-57da-423e-b613-6a8ce126d0a8\"]}}"
其中的json字符串示例
{
    "success":{
	    "count":10   //成功个数
	 },
    "fail": {
        "download": [
           "20e59200-57da-423e-b613-6a8ce126d0a3",
           "20e59200-57da-423e-b613-6a8ce126d0a4"
        ],         // 文件下载失败导致的翻译失败的callId数组
        "format": [ 
           "20e59200-57da-423e-b613-6a8ce126d0a5",
           "20e59200-57da-423e-b613-6a8ce126d0a6"
        ],        // 文件格式错误导致的翻译失败的callId数组
        "other": [ 
           "20e59200-57da-423e-b613-6a8ce126d0a7",
           "20e59200-57da-423e-b613-6a8ce126d0a8"
        ]        // 其他失败原因， 如 网络抖动等原因导致，可具体分析 
    }
}
```

### 百度回调识别结果CallBackRequest 类说明，建议使用

- 在Spring工程中可使用@RequestBody CallBackRequest callBackRequest 进行反序列化

```
package com.baidu.aip.bicc.offline.common.beans.callback;

import java.io.IOException;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import com.baidu.aip.bicc.offline.common.beans.report.AsrReport;
import com.baidu.aip.bicc.offline.common.beans.upload.AsrContent;
import com.baidu.aip.bicc.offline.common.consts.KnowledgeCategory;
import com.baidu.aip.bicc.offline.common.json.JsonUtils;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.JsonMappingException;

/**
 * CallBackRequest bean
 * Date: 2018/07/28
 * Time: 16:13
 */
public class CallBackRequest {

    /**
     * 百度云申请应用的appId,必选
     */
    private Long appId;

    /**
     * 百度云颁发的userId
     */
    private Long userId;

    /**
     * 标识这通电话的唯一标识符
     */
    private String callId;

    /**
     * 知识类型<br>
     * TXT为识别结果，REPORT为任务结束后告诉此批任务完成报告
     */
    private KnowledgeCategory category;

    /**
     * 知识内容
     */
    private String content = "";

    /**
     * 触发知识的时间
     */
    private Long triggerTime = new Date().getTime();

    /**
     * logId标识
     */
    private String logId = UUID.randomUUID().toString();
    
    /**
     * asr识别内容
     */
    private List<AsrContent> asrContents;
    /**
     * asr报告内容
     */
     
    private AsrReport asrReport;

      /**
     * 获取识别结果
     *
     * @return List<AsrContent>
     *
     * @throws JsonParseException
     * @throws JsonMappingException
     * @throws IOException
     */
    public List<AsrContent> getAsrContents() throws JsonParseException, JsonMappingException, IOException 

    /**
     * 获取任务批次翻译的REPORT
     *
     * @return
     *
     * @throws JsonParseException
     * @throws JsonMappingException
     * @throws IOException
     */
    public AsrReport getAsrReport() throws JsonParseException, JsonMappingException, IOException 
```

其中 AsrReport，AsrFailReport，AsrSuccessReport类说明如下

```
package com.baidu.aip.bicc.offline.common.beans.report;

/**
 * 离线翻译报告回调
 * Date: 2018/07/28
 * Time: 15:43
 */
public class AsrReport {
    /**
     * 离线识别成功信息
     */
    private AsrSuccessReport success;
    /**
     * 离线识别报告-失败信息数组
     */
    private AsrFailReport fail;
}
package com.baidu.aip.bicc.offline.common.beans.report;

import java.util.List;

/**
 * 离线识别报告-失败信息数组
 * User: Created by zhangweidong02
 * Date: 2018/07/28
 * Time: 15:43
 */
public class AsrFailReport {
    /**
     * 文件下载失败callId数组
     */
    private List<String> download;
    /**
     * 文件格式错误callId数组
     */
    private List<String> format;
    /**
     * 其他原因失败失败callId数组
     */
    private List<String> other;
     /**
     * callback 失败的callId
     */
    private List<String> callBack;
}
package com.baidu.aip.bicc.offline.common.beans.report;

import java.util.List;

import com.baidu.aip.bicc.offline.common.json.JsonUtils;

/**
 * 离线识别成功信息
 * Date: 2018/07/28
 * Time: 15:43
 */
public class AsrSuccessReport {
    /**
     * 识别成功计数
     */
    private int count;

    private List<String> info;
}
    
```

# 联系我们

- [商务合作](https://cloud.baidu.com/survey/BICCApply.html)反馈；
- [工单系统](http://ticket.bce.baidu.com/?_=1530034880766/#/ticket/list)反馈，工单类型为“人工智能”->“智能呼叫中心”；
- 邮箱：ics-bd@baidu.com；
- 社区：[智能呼叫中心](http://ai.baidu.com/forum/topic/list/196)；

