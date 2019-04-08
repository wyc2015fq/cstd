# 实时语音识别 Mrcp Server文档

# 概述

Hi，您好，欢迎使用百度智能呼叫中心实时语音识别云服务。本文档主要面向呼叫中心开发者，介绍产品合作流程以及使用C++ SDK快速接入服务的。

智能呼叫中心提供的ASR模型为呼叫中心定制版，较通用电话场景下的ASR模型识别效果更佳。服务以云服务方式提供，企业通过集成的MrcpServer，向百度实时输入语音流，百度将实时识别文本推送回企业的业务系统，企业可在此基础上实现金牌销售、机器人、质检等产品。

如果您对合作方式及文档内容有任何疑问，或者您需要深度定制服务，请通过文档下方“联系我们”找到我们。

------

# 合作流程

![img](http://bos.nj.bpc.baidu.com/v1/agroup/ba9e9b99751a0d53e07a95daa54526947dc55c34)

## 流程说明

- 免费服务申请
  - 页面右上方“控制台”点击进入，登录百度账号，进入百度云管理后台页面。
  - 左侧导航栏上方，依次选择 产品服务->人工智能->智能呼叫中心，进入应用“概览”页面。
  - 服务申请。点击“创建应用”，“接口选择”已默认勾选完成，如无其他需求，无需勾选，完成其他资料后，点击最下方“立即创建”按钮。
  - 应用成功则页面显示“创建完毕”，点击”返回应用列表”， 查看新创建应用详情，记录AppID，API Key，Secret Key。
  - 从呼叫中心设备或开发语言角度出发，选择较低集成成本的接入方式（Java SDK、Linux C++ SDK、 MrcpServer）快速接入并试用服务。
- 付费开通 参考“免费账号和服务申请”流程，进入应用“概览”页面，查看服务列表，在所选服务最右侧点击「立即购买」。
- 商务合作提交 进入AI开放平台，选择 产品服务 -> 智能呼叫中心 -> 呼叫中心语音识别 ，选择 “[商务合作](https://cloud.baidu.com/survey/BICCApply.html)”，填写您的联系方式及呼叫中心需求，我们将在2~3个工作日联系您。
- 商务谈判 我们将为您提供产品方案咨询、技术方案咨询、价格咨询等服务，为您定制语音识别模型以提升特定场景的识别准确率； 确定商务合作后，就合同、具体定制费用等相关问题细则进行商务沟通，百度可根据您的调用量提供一定的折扣； 确定费用后，双方签订合同及支付，即可直接调用。在此过程中，百度可提供POC服务、联调服务等。

## 服务内容

- **特训呼叫中心语音识别模型** 百度针对呼叫中心设备特征，特训大量电话数据，形成呼叫中心领域专属模型，为企业呼叫中心各类智能化产品提供精准的语音转文字服务。
- **高时效，低价格，实时语音识别** 您可以使用本服务，将您业务通话过程中的实时音频流以分轨或单轨的形式发送至百度，百度将在为您开辟的专属通路中实时为您返回识别后的文本，为您提供百毫秒级的返回时效，并保障高稳定性、安全性。
- **免费试用，快速验证效果** 您可以通过「立即使用」入口，进入呼叫中心语音识别的控制台页面，根据您的业务需要创建App，获得API Key、Secret Key后，即可免费试用本服务；免费试用无时限，但专属识别通路数量有限制。
- **线上直接调用服务** 您可以通过「立即使用」入口，进入呼叫中心语音识别的控制台页面，在控制台页面下方找到本服务，右侧有「立即购买」入口，您可以付费开通更加稳定且并发通路更多的服务。我们将在您提交付费后为你准备专属的服务器，保障服务效率及您的数据安全。
- **线下商务合作** 您也可以通过「[商务合作](https://cloud.baidu.com/survey/BICCApply.html)」，进入表单提交页面，填写您的业务需求并提交。我们会在线下与您取得联系，确认购买量后，您可以获得商务合作优惠价。我们还可以为您训练打造专属的行业场景模型，提升特定场景的语音识别准确率，满足您的业务需求。
- **单双轨支持** 本服务需要按照并发通路购买。为达到更好的识别效果，我们建议您拆分通话双侧音频流为两通路，可同时发送通话双方的音频流，按两通路来并行识别；如果您的业务仅需要识别一侧语音，也可只发送单侧音频流，按一通路识别；如果您的通话无法将两侧拆分为双轨，可将电话混音发送音频流，按一通路识别。

------

# 快速开始

## Release Notes

| 版本  | 日期       | 说明                                                         |
| :---- | :--------- | :----------------------------------------------------------- |
| 0.1.0 | 2018.02.22 | 第一版！                                                     |
| 0.1.2 | 2018.03.19 | 压缩性能优化                                                 |
| 0.1.3 | 2018.03.29 | 新增切句配置                                                 |
| 0.1.4 | 2018.07.09 | 增加扩展信息，语音识别文本流携带毫秒级时间戳，完整句携带语速 |

## 接入准备

- 参考“流程说明”—>“免费服务申请”完成权限申请；
- 点击[智能电销及实时语音识别-MrcpServer](http://ai.baidu.com/sdk#itma)完成MrcpServer下载；
- 开发环境依赖：
  - Linux 64位 centos 6u3
  - gcc: 482 以上libc，CXX11，百度提供gcc4.8.2压缩包，存放于libs目录下，解压后执行bootstrap.sh完成默认配置
  - [curl](https://curl.haxx.se/) : 7.33
  - ssl : 1.0.1i
  - [daemontools](http://cr.yp.to/daemontools.html) 守护进程
  - 须有root账户权限运行，curl、ssl服务器未安装或版本不够，请自行安装或升级。
- 开发机对conf/recogplugin.json配置中“app.serverAddress”（服务访问）、“app.authAddress”（鉴权）地址开放公网访问权限;
- 详见下文“测试mrcpserver”，运行自带paasclient，熟悉mrcpserver的运行情况

## 并发限制

受内存，核数限制，4核8G内存，并发数为20。

## 语音流改造

| 要求项             | 取值要求                                                 |
| :----------------- | :------------------------------------------------------- |
| 音频格式           | PCM                                                      |
| 采样率             | 8KHz                                                     |
| 采样精度           | 16bits                                                   |
| 声道               | 单声道                                                   |
| 实时语音流发送间隔 | 20ms~160ms内任意10的倍数，建议最佳发送间隔160ms          |
| 压缩选项           | 压缩为原始大小的1/8，节省带宽，但是音频质量会有损失      |
| 分轨               | 为保证识别效果，进行话者分离，拆分通话双侧音频流为两通路 |

## 输出

| key      | 值说明                                                       |
| :------- | :----------------------------------------------------------- |
| 格式     | 详见下文`识别结果`说明                                       |
| 语音识别 | 实时逐字识别文字，由静音检测识别出的完整句，文字对应开始&结束时间，完整句对应语速 |
| 语义识别 | utf8编码，意图、槽位                                         |
| 输出方式 | 回调，实时返回                                               |

> 语速 = 完整句字数 / 秒。其中，单个单词整体算作1个字，不会按实际字母数统计。

------

## 目录结构

```
${SERVER_ROOT}
|--- bin    // 服务可执行程序
	|--- control // 启动脚本
	|--- bootstrap.sh // 初始化环境
|--- conf // 配置文件
	|--- recogplugin.json // 识别插件配置文件
|--- data  // 数据资源
|--- include // 头文件
|--- lib     // 依赖库
|--- libs    // 依赖库
|--- log // 日志目录
|--- plugin // 插件及依赖
```

## 参数配置

识别相关配置文件目录 `${SERVER_ROOT}/conf/recogplugin.json` ，参考“流程说明”—>“免费账号和服务申请”流程，进入应用“概览”页面，点击左侧导航“应用列表”，查询“API Key”和“Secret Key”。必须修改

```
"app.appKey": "API Key",  
"app.appSecret": "Secret Key", 
"app.scope": "brain_bicc", 
```

其他参数，无特殊需求，无需修改，保持现状。

**配置项说明**

| key                  | 类型   | 必选 | 说明                                                         | 默认值           |
| :------------------- | :----- | :--- | :----------------------------------------------------------- | :--------------- |
| app.appKey           | string | Y    | app key                                                      | -                |
| app.appSecret        | string | Y    | app secret                                                   | -                |
| app.compress         | bool   | Y    | 是否开启压缩。为解决带宽，默认压缩                           | true             |
| app.machineName      | string | N    | 器唯一标识。 建议填写mac地址(移除冒号，如14-10-9f-df-45-4f)或有唯一性含义的hostname |                  |
| app.serverAddress    | string | Y    | 服务器地址(若使用https必须libcurl支持)                       | -                |
| app.authAddress      | string | Y    | 验证服务地址(若使用https必须libcurl支持)                     | -                |
| app.upload.threads   | number | N    | 上传线程数(根据并发数和实际网络状况与我们沟通后修改)         | 4                |
| app.upload.duration  | number | N    | 上传线程超时时间(秒，根据实际网络状况与我们沟通后修改)       | 900              |
| log.toStandardOutput | 0/1    | Y    | 日志是否输出至标准输出                                       | 1                |
| log.toFile           | 0/1    | Y    | 日志是否输出至文件                                           | 0                |
| log.filename         | string | Y    | SDK日志输出文件名                                            |                  |
| log.plugin.filename  | string | Y    | 插件日志输出文件名                                           |                  |
| log.maxLogFileSize   | number | N    | 日志文件最大限制                                             | 10 * 1024 * 1024 |
| log.enableDebug      | 0/1    | Y    | 开启Debug日志                                                | -                |
| log.enableInfo       | 0/1    | Y    | 开启Info日志                                                 | -                |
| log.enableWarning    | 0/1    | Y    | 开启Warning日志                                              | -                |
| log.immediateFlush   | 0/1    | Y    | 日志是否实时flush                                            | -                |
| userparams           | string | N    | 用户定制化参数                                               |                  |
| app.scope            | string | Y    | 使用的服务。 brain_bicc: 实时识别。                          |                  |

**关于 userparams 参数** 目前支持的参数包括：

- 结果返回模式

```
"sentenceHandler": {
	"client": "STANDARD_COMPLETED",
	"agent": "STANDARD_COMPLETED"
}
```

client 和 agent表示对应用户与坐席的返回类型设置，支持的结果返回模式包括

| 值                 | 含义         | 备注                               |
| :----------------- | :----------- | :--------------------------------- |
| STANDARD_COMPLETED | 完整句切句   | 屏蔽掉所有过程句子，只下发完整句。 |
| NONE               | 实时翻译结果 | 默认值，逐字识别结果及完整句       |

## 识别结果

识别结果通过回调，返回的字段说明如下：

| key          | 说明           | 必选 | 内容                                                         |
| :----------- | :------------- | :--- | :----------------------------------------------------------- |
| appId        | 标识所属app    | 否   |                                                              |
| callId       | 知识所属call   | 是   |                                                              |
| roleCategory | 知识所属于角色 | 否   | AGENT，座席；CLIENT，客户                                    |
| category     | 知识类型       | 是   | TXT，文本； INTENT，意图； KNOWLEDGE，知识； SPEECH，合成语音（url地址）； EVENT，事件 |
| content      | 知识内容       | 是   | 返回的知识的内容                                             |
| triggerTxt   | 触发知识的文本 | 否   | string                                                       |
| triggerTime  | 触发知识的时间 | 否   | timestamp                                                    |
| extJson      | 扩展字段       | 否   | 扩展字段                                                     |
| logId        | 本次返回标识   | 是   | String                                                       |

extJson字段说明

| key         | 说明         | 必选 | 内容                                                         |
| :---------- | :----------- | :--- | :----------------------------------------------------------- |
| completed   | 识别文本状态 | 是   | 取值1和3。1，过程中逐字识别结果；3，vad切分产生的完整句      |
| snStartTime | 文本开始时间 | 是   | 毫秒级，样例：00:00.100                                      |
| snStopTime  | 文本结束时间 | 是   | 毫秒级，样例：00:01.100                                      |
| speed       | 完整句语速   | 否   | 完整句字数 / 秒。其中，单个单词整体算作1个字，不会按实际字母数统计。 |

返回示例：

```
<result>
	<interpretation grammar="" confidence="1.0">
		<instance>
			<callId>20913-35009.1520997556725710129-423467</callId>
			<logId>02bbdf02-c00a-45eb-ae9d-6a7a6d1f74b9_8</logId>
			<rolecategory>AGENT</rolecategory>
			<categotyId>TXT</categotyId>
			<extJson>
                <snStartTime>00:01.600</snStartTime>
                <snStopTime>00:09.759</snStopTime>
                <speed>5.39</speed>
            </extJson>
		</instance>
		<input mode="speech">医学考试时间</input>
	</interpretation>
</result>
```

------

## 使用mrcpserver

1. 每次替换MrcpServer安装包，都需要在`${SERVER_ROOT}/` 目录，root权限下执行 `sh bootstrap.sh` ，主要功能：完成百度自带gcc4.8.2的环境配置。
2. **启动：** 在`${SERVER_ROOT}/bin`目录执行 `./control start`
3. **停止：** 在`${SERVER_ROOT}/bin`目录执行 `./control stop`
4. **重启：** 在`${SERVER_ROOT}/bin`目录执行 `./control restart`
5. **查看服务状态：**在`${SERVER_ROOT}/bin`目录执行 `./control status`

服务启动中遇到的问题：

- 若执行control start失败，请按照以下步骤排查服务

1、在`${SERVER_ROOT}/bin`目录执行

```
[root@xxx bin]$ ./unimrcpserver 
bash: ./unimrcpserver: /opt/compiler/gcc-4.8.2/lib64/ld-linux-x86-64.so.2: bad ELF interpreter: No such file or directory，
```

说明bootstrap.sh执行失败，请移步`压缩包自带README文件`，手动完成gcc4.8.2的配置。

2、若单独启动unimrcpserver正常，命令行直接输入“supervise”，若supervise非系统命令，说明守护进程安装失败，请检查守护进程。

- 日志中的常见错误

| 错误信息                                  | 解决                                                         |
| :---------------------------------------- | :----------------------------------------------------------- |
| err_code":210                             | recogplugin.json中填写app.appKey，app.appSecret              |
| Config 'app.scope' empty                  | recogplugin.json中填写app.scope                              |
| err_code":321                             | 鉴权失败， 检查本机是否可以访问recogplugin.json中app.authAddress对应地址 |
| "err_code":322                            | 百度云端服务访问失败， 检查本机是否可以访问recogplugin.json中app.serverAddress对应地址 |
| 408 ACK Timeout                           | conf/unimrcpserver.xml 同时配置内网、外网IP，SIP三次握手建立连接， client请求server地址为内网地址， 但server返回status 200包给client使用的是client的外网地址，client未接收到。 可对server抓包，确认返回ip是否符合预期 |
| Quota limit reached: Concurrency exceeded | 访问量超过可用额度， 一次SIP连接创建，配额+1， 请注意，需client向server发送SIP断开，释放配额， 若不主动断开，百度侧将在15分钟之后自动释放配额 |

## 测试mrcpserver

运行测试client: 请至`${SERVER_ROOT}/bin`目录，运行 `./paasclient`, 输入`run recog`
默认ip为：本机内网地址
默认sip端口为：8060
默认rtp端口为：5000-6000
配置外网ip、更改端口配置详见`压缩包自带README文件`

> 当前paasclient代码逻辑为支持一次识别结果返回，与MrcpServer能力无关，MrcpServer可支持连续识别结果返回。

## 客户端要求

保持一通电话单侧（坐席|客户）使用同一个SIP连接。
SIP头部扩展参数P-UserData（可选字段），携带callid及roleid，格式说明及事例如下：

| 字段   | 含义             | 值类型 | 默认值 | 备注                                              |
| :----- | :--------------- | :----- | :----- | :------------------------------------------------ |
| callid | 一通电话唯一标识 | String |        | 格式需要满足 `^[0-9a-zA-Z_\.-]{16,128}$` 正则要求 |
| roleid | 角色标识         | int    | 1      | 可选。0-坐席，1-客户                              |

示例

```
c sip:39.107.25.82:5060 SIP/2.0   
Via: SIP/2.0/UDP 114.215.196.183:5092;rport;branch=z9hG4bKHXm44FX131NFF   
Max-Forwards: 70 
From: <sip:114.215.196.183:5092>;tag=BcjjUe581cFUj 
To: <sip:39.107.25.82:5060> 
CSeq: 118054628 INVITE 
User-Agent: CCP 
P-UserData:callid=aaa79db9-7b80-1236-08a2-00163e02342e,roleid=1
Allow: INVITE, ACK, BYE, CANCEL, OPTIONS, PRACK, MESSAGE, SUBSCRIBE, NOTIFY, REFER, UPDATE
Supported: timer, 100rel 
Content-Type: application/sdp 
Content-Disposition: session
Content-Length: 334
```

重要提示，P-UserData为非必须扩展字段。 返回结果

```
MRCP/2.0 709 RECOGNITION-COMPLETE 2 COMPLETE
Channel-Identifier: b6c5e7958133499c@speechrecog
Completion-Cause: 000 success
Content-Type: application/nlsml+xml
Content-Length: 522

<?xml version="1.0" encoding="utf-8"?>
<result>
	<interpretation grammar="" confidence="1.0">
		<instance>
			<callId>20913-35009.1520997556725710129-423467</callId>
			<logId>02bbdf02-c00a-45eb-ae9d-6a7a6d1f74b9_8</logId>
			<rolecategory>AGENT</rolecategory>
			<categotyId>TXT</categotyId>
			<extJson>
                <snStartTime>00:01.600</snStartTime>
                <snStopTime>00:09.759</snStopTime>
                <speed>5.39</speed>
            </extJson>
		</instance>
		<input mode="speech">医学考试时间</input>
	</interpretation>
</result>
```

# 联系我们

- [商务合作](https://cloud.baidu.com/survey/BICCApply.html)反馈；
- [工单系统](http://ticket.bce.baidu.com/?_=1530034880766/#/ticket/list)反馈，工单类型为“人工智能”->“智能呼叫中心”；
- 电话：010-50802520（10:00~18:00）；
- 邮箱：ics-bd@baidu.com；
- 社区：[智能呼叫中心](http://ai.baidu.com/forum/topic/list/196)；