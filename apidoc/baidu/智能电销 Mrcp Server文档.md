# 智能电销 Mrcp Server文档

# 概述

Hi，您好，欢迎使用百度智能电销PaaS服务。本文档主要针对呼叫中心开发者，描述百度智能电销方案的相关技术内容。使用之前，请在[官网](http://ai.baidu.com/solution/itma)了解ITMA相关服务内容。如果您对合作方式及文档内容有任何疑问，查看文档下方“联系我们”。

智能电销对传统电销模式进行智能改造，百度通过PaaS服务向合作企业提供AI技术（包括语音识别、意图识别、对话管理、多轮对话），合作企业开发相关应用，通过实时语音语义SDK，向百度实时输出语音流，百度提供客户意图推送服务，合作企业根据收到的意图向坐席人员提示应对话术。辅助坐席人员快速反馈，与客户顺畅沟通，促进销售成单。

## 功能

智能电销解决方案，提供实时语音识别、实时意图分析、话术管理、人与人的对话分析功能，让坐席人员在与客户沟通中，实时了解用户意图，根据推进话术做出应答 。

- 实时语音识别：在坐席人员与客户的沟通过程中，语音实时传输，同步返回识别结果。
- 实时意图分析：根据上下文分析对话，实时返回客户的意图。
- 对话管理：为满足呼叫中心大批量并发数据，提供对话管理服务，保障同时处理多通对话；同时能够监控单通对话状态，支持上下文管理，辅助提升意图识别准确率。
- 人与人的对话分析：突破传统的人机对话模式，创新支持智能分析人与人之间的对话。支持特定场景模型训练及管理监控坐席人员并发数量等数据。

------

# 合作流程

![img](http://bos.nj.bpc.baidu.com/v1/agroup/e6e937eef6664df79d2946acbd01640d57dfc81d)

## 流程说明

- 服务申请
  - 页面右上方“控制台”点击进入，登录百度账号，进入百度云管理后台页面。
  - 左侧导航栏上方，依次选择 产品服务->人工智能->智能呼叫中心，进入应用“概览”页面。
  - 服务申请。点击“创建应用”，“接口选择”已默认勾选完成，如无其他需求，无需勾选，完成其他资料后，点击最下方“立即创建”按钮。
  - 应用成功则页面显示“创建完毕”，点击”返回应用列表”， 查看新创建应用详情，记录AppID，API Key，Secret Key。
  - 从呼叫中心设备或开发语言角度出发，选择较低集成成本的接入方式（Java SDK、Linux C++ SDK、 MrcpServer）快速接入并试用服务。
- 商务合作提交

欢迎来到AI开放平台解决方案 -> 智能电销 解决方案，选择 “商务合作” ，填写您的联系方式及呼叫中心需求，我们将在2~3个工作日联系您

- 线下商务流程

我们将在您提交商务合作后与您联系，为您提供业务咨询、业务梳理、产品方案咨询、技术方案咨询等服务，为您定制智能电销场景 确定商务合作后，就合同、具体定制费用等相关问题细则进行商务沟通 确定费用后，双方签订合同及支付，即可进入开发中，在此过程中，百度可提供定制化的产品设计服务及技术方案设计服务

- 开通控制台白名单

确定商务合作后，我们将为您开通控制台白名单。您需要注册并登录您的百度账号，并且注册开发者信息。开通白名单后，您的提交您可以提前预览智能电销服务在您开始调用核心接口后展示的统计服务。 访问控制台，请在AI开放平台首页右上角 -> 控制台，登录后使用。

## 合作内容

- 服务咨询与设计定制

双方确认商务合作后，我们将根据企业需要的目标电销业务场景，为客户提供业务咨询、业务梳理、服务方案咨询及设计、技术方案咨询及设计服务

- 核心接口服务

本方案为智能电销方向的PaaS服务，百度提供场景定制的“实时语音流输入，实时意图输出”的核心接口

- 企业实时语音流改造

企业侧需要改造呼叫中心话务平台，将目标外呼业务场景的电销坐席侧与客户侧电话沟通中的实时语音流通过百度提供的SDK，实时传入，百度将实时提供通话双方的语义意图，通过SDK回传至企业

------

## 企业建立智能话术推荐系统

企业侧需要根据百度回传的实时语义意图，建立智能化书推荐系统，制定意图对应的应答话术，并通过系统实时推荐展示给电销坐席，以便坐席实时与客户熟练沟通

![img](http://bos.nj.bpc.baidu.com/v1/agroup/3881255d0437b0c3dee6503825915ceaf9f3667e)

# 快速开始

## Release Notes

| 版本  | 日期       | 说明                                                         |
| :---- | :--------- | :----------------------------------------------------------- |
| 0.1.0 | 2018.02.22 | 第一版！                                                     |
| 0.1.2 | 2018.03.19 | 压缩性能优化                                                 |
| 0.1.3 | 2018.03.29 | 新增切句配置                                                 |
| 0.1.4 | 2018.07.09 | 增加扩展信息，语音识别文本流携带毫秒级时间戳，完整句携带语速 |

## 接入准备

- 参考“流程说明”—>“服务申请”完成权限申请；
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
| 格式     | 详见下文`识别结果`                                           |
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

## 配置说明

识别相关配置文件目录 `${SERVER_ROOT}/conf/recogplugin.json` ，参考“流程说明”—>“免费账号和服务申请”流程，进入应用“概览”页面，点击左侧导航“应用列表”，查询“API Key”和“Secret Key”。必须修改

```
"app.appKey": "API Key",  
"app.appSecret": "Secret Key", 
"app.scope": "brain_ai_talker", 
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
| app.scope            | string | Y    | 使用的服务。 brain_ai_talker: 智能电销。                     |                  |

**关于 userparams 参数**

目前支持的参数包括：

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
			<categotyId>INTENT</categotyId>
		</instance>
		<input mode="speech">确认</input>
	</interpretation>
</result>
```

------

## 使用mrcpserver

1. 每次替换MrcpServer安装包，都需要在`${SERVER_ROOT}/` 目录，root权限下执行 `sh bootstrap.sh`
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