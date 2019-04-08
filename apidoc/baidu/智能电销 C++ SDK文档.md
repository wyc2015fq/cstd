# 智能电销 C++ SDK文档

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

| 版本  | 日期       | 说明                                           |
| :---- | :--------- | :--------------------------------------------- |
| 0.1.0 | 2018.02.22 | 第一版！                                       |
| 0.1.2 | 2018.03.19 | 压缩性能优化                                   |
| 0.1.3 | 2018.03.29 | 新增切句配置，完善demo                         |
| 0.1.4 | 2018.06.14 | 语音识别文本流携带毫秒级时间戳，完整句携带语速 |
| 0.1.6 | 2018.10.26 | 新增通话结束确认机制                           |

## 接入准备

- 参考“流程说明”—>“服务申请”完成权限申请；
- 点击[智能呼叫中心-智能电销C++ SDK](http://ai.baidu.com/sdk#itma)完成C++ SDK下载；
- 开发环境依赖：
  - Linux 64位
  - [cmake](https://cmake.org/) :3.1，编译工具
  - gcc: 482 以上
  - [curl](https://curl.haxx.se/) : 7.33
  - ssl : 1.0.1i，如果开启https必须
  - [jsoncpp](https://github.com/open-source-parsers/jsoncpp) : 可使用SDK提供的jsoncpp库
- 开发机对SDK内config.json配置中“app.serverAddress”（服务访问）、“app.authAddress”（鉴权）地址开放公网访问权限；
- 编译并运行SDK自带demo程序，熟悉SDK工作过程。

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
| 格式     | `EEventTypeWorkStatusResult`，详见下文“[识别结果](http://ai.baidu.com/docs#result)”说明 |
| 语音识别 | 实时逐字识别文字，由静音检测识别出的完整句，文字对应开始&结束时间，完整句对应语速 |
| 语义识别 | utf8编码，意图、槽位                                         |
| 输出方式 | 回调，实时返回                                               |

> 语速 = 完整句字数 / 秒。其中，单个单词整体算作1个字，不会按实际字母数统计。

## SDK目录

```
${SDK_ROOT}
  |--- demo    // demo 源码和cmake文件
  |--- include // SDK 头文件
  |--- lib     // SDK 库文件
  |--- thirdparty  // 第三方库
       |--- include
       |--- lib
       |--- lib64
```

## Demo 运行

1、请参考cmake官方文档配置gcc路径等参数。 2、修改demo/config.json文件 参考“流程说明”—>“免费账号和服务申请”流程，进入应用“概览”页面，点击左侧导航“应用列表”，查询“API Key”和“Secret Key”。

```
"app.appKey": "API Key",  
"app.appSecret": "Secret Key", 
"app.scope": "brain_ai_talker", 
```

3、编译及运行

```
cd ${SDK_ROOT}/demo
cmake .
make
//{并发数} {循环数} {坐席语音文件} {客户语音文件}
./bd_itma_demo 1 1 salesman.wav customer.wav
```

4、自带音频文件 为方便开发者进行测试，自带示例音频文件。 - 坐席侧salesman.wav； - 用户侧customer.wav。

## 调用接口及参数说明

**全局接口**

在使用SDK之前，必须进行初始化操作。

1、配置全局回调。

```
auto *user_callback_param = new int(12345);
// set callback before init
cntl->set_callback(itma_callback, user_callback_param);
```

传入`typedef void (*ItmaCntlCallback)(Event& e, void* data)`类型的函数指针。user_callback_param与constroller绑定（全局唯一），会回调给用户。

SDK将通过此回调通知用户SDK内部的工作状态，包括所有生成的Session的工作状态。

| event_type                        | 说明                                  | 附带参数列表                                        |
| :-------------------------------- | :------------------------------------ | :-------------------------------------------------- |
| EEventTypeWorkStatusPktPosted     | 【DEBUG】SDK发出一个数据包的回调      |                                                     |
| EEventTypeWorkStatusSpeechStarted | 服务端成功收到了用户开启session的请求 | K_CALL_ID                                           |
| EEventTypeWorkStatusSpeechEnded   | 服务端成功释放了用户结束session的请求 | K_CALL_ID                                           |
| EEventTypeWorkStatusResult        | 识别结果/意图结果回调                 | K_CALL_ID, K_EVENT_RESULT                           |
| EEventTypeError                   | 错误回调                              | K_EVENT_ERR_CODE, K_EVENT_ERR_SUB_CODE. K_ERROR_MSG |
| EEventTypeFatal                   | 错误回调，表明SDK无法继续运行         | K_EVENT_ERR_CODE, K_EVENT_ERR_SUB_CODE. K_ERROR_MSG |

2、参数设置

| 配置参数列表         | 类型   | 必选 | 说明                                                         | 默认值           |
| :------------------- | :----- | :--- | :----------------------------------------------------------- | :--------------- |
| app.appKey           | string | Y    | app key                                                      | -                |
| app.appSecret        | string | Y    | app secret                                                   | -                |
| app.compress         | 0/1    | Y    | 是否开启压缩。开启压缩后，网络传输字节减少至约1/7， 对识别效果几乎没有影响。但需消耗一定的CPU资源。 | -                |
| app.machineName      | string | N    | 机器唯一标识。 建议填写mac地址(移除冒号，如14-10-9f-df-45-4f)或有唯一性含义的hostname | ${hostname}      |
| app.scope            | string | Y    | 使用的服务。 brain_ai_talker： 智能电销。                    |                  |
| app.serverAddress    | string | Y    | 服务器地址(若使用https必须libcurl支持)                       | -                |
| app.authAddress      | string | Y    | 验证服务地址(若使用https必须libcurl支持)                     | -                |
| app.upload.threads   | number | N    | 上传线程数(根据并发数和实际网络状况与我们沟通后修改)         | 1                |
| app.upload.duration  | number | N    | 上传线程超时时间(秒，根据实际网络状况与我们沟通后修改)       | 900              |
| log.toStandardOutput | 0/1    | Y    | 日志是否输出至标准输出                                       | 1                |
| log.toFile           | 0/1    | Y    | 日志是否输出至文件                                           | 0                |
| log.filename         | string | Y    | 日志文件名                                                   |                  |
| log.maxLogFileSize   | number | N    | 日志文件最大限制                                             | 10 * 1024 * 1024 |
| log.enableDebug      | 0/1    | Y    | 开启Debug日志                                                | -                |
| log.enableInfo       | 0/1    | Y    | 开启Info日志（建议在生产环境中开启）                         | -                |
| log.enableWarning    | 0/1    | Y    | 开启Warning日志                                              | -                |
| log.immediateFlush   | 0/1    | Y    | 日志是否实时flush                                            | -                |

设置方法

```
ItmaController* cntl = global_controller();
std::string err_msg;
// 使用配置文件初始化
cntl->init("./config.json", err_msg);

// 也可以使用Json::Value类初始化
Json::Value cfg;
cfg["app.appKey"] = "app key";
cfg["app.appSecret"] = "app secret";
cntl->init(cfg, err_msg);
```

**单通电话调用**

每通电话分为三种状态：电话开始、电话过程中、电话结束。 基本流程：

```
session = global_controller.new_session()
session.speech_start()
while(the call is still active) {
  session.speech_doing()
}
session.speech_end()

global_control.release_session(session)
```

> ItmaSession 类的所有方法都是线程不安全的

1、电话开始 创建新的session。 global_controller 必须已初始化。若指定call_id，请确保call_id 的全局唯一性，且满足以下正则（推荐使用[UUID](https://zh.wikipedia.org/wiki/%E9%80%9A%E7%94%A8%E5%94%AF%E4%B8%80%E8%AF%86%E5%88%AB%E7%A0%81)生成）。

```
^[0-9a-zA-Z_\.-]{16,128}$
```

若不指定，系统会随机生成一个call_id。

```
// 获得一个新的session

ItmaSession* session;
// 指定call_id
global_controller()->new_session("yourcallid.12345", session, err_msg);

// 指定其他参数
Json::Value param;
param[K_AGENT_DN] = 1234;
param[K_ROLEID] = 2; // 同时有坐席和用户
global_controller()->new_session(param, session, err_msg)
```

new_session param参数列表

| KEY        | 说明                       | 类型   | 是否必须      | 可选值                                      |
| :--------- | :------------------------- | :----- | :------------ | :------------------------------------------ |
| K_AGENT_DN | 坐席号                     | number | N             |                                             |
| K_RATE     | 采样率                     | number | N（默认8000） | 目前只支持8000                              |
| K_ROLEID   | 语音的分轨说明             | number | N（默认2）    | 0：只有坐席 1: 只有用户 2：同时有坐席和用户 |
| K_PARAM    | 其他定制化参数，请参考下文 | string | N             |                                             |

关于 K_PARAM 参数 K_PARAM参数用于传入定制化参数，KEY为K_PARAM， VALUE必须为string，string内容为可解析的json，如

```
param[K_PARAM] = R"({
	"sentenceHandler": {
		"client": "STANDARD_COMPLETED",
		"agent": "STANDARD_COMPLETED"
	}
})";
```

请务必检查设置的字符串格式是否正确。目前K_PARAM支持的参数包括

结果返回模式

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

配置Session回调

```
auto *user_callback_param = new int(23456);
session->set_callback(itma_session_callback, user_callback_param);
```

传入`typedef void (*ItmaSessionCallback)(ItmaSession* session, const Event& e, void* user_callback_param);`类型的函数指针。user_callback_param与session绑定，会回调给用户。 回调的参数与内容与全局回调一致，但是只会回调与该Session相关的内容。

2、电话过程中 语音流发送，发送间隔20ms（160字节）~160ms（2560字节）内任意10的倍数。 推荐发送间隔为160ms，语音流长度2560字节。

```
// 开始识别
session->speech_start(...);
// 放入数据, 
while({having binary data}) {
	// not thread safe
	session->speech_doing(binary, ...);  
}
```

请务必检查所有的speech_doing的返回值是否为0。

> 若开启了压缩，SDK会在执行`session->speech_doing(binary, ...)`的线程中进行一定的计算操作，会有一定的CPU时间消耗。 4核机器上，若开启压缩，并发数请勿超过20

3、电话结束

```
// 结束识别
session->speech_end(...);
```

一通电话结束后，请务必调用speech_end 来结束通话。否则在超时之前，这通未结束的电话会持续占用您的并发配额。

服务端需要额外的时间来释放这通电话的资源，如果开发者希望等待直到服务端资源释放，可以通过如下方式：

```
// 服务端需要额外的逻辑释放资源
while (true) {
   if (global_controller()->release_session(session) != 0) {
       std::this_thread::sleep_for(std::chrono::milliseconds(100));
   } else {
       DEMOLOG << "Session released with ack: " << session << std::endl;
       break;
   }
}
```

## 识别结果

识别结果通过回调，以`EEventTypeWorkStatusResult`的类型返回，返回的字段说明如下：

| key          | 说明           | 必选 | 内容                                                         |
| :----------- | :------------- | :--- | :----------------------------------------------------------- |
| appId        | 标识所属app    | 否   |                                                              |
| callId       | 知识所属call   | 是   |                                                              |
| roleCategory | 知识所属于角色 | 否   | AGENT，座席；CLIENT，客户                                    |
| category     | 知识类型       | 是   | TXT，文本； INTENT，意图； KNOWLEDGE，知识； SPEECH，合成语音（url地址）； EVENT，事件 |
| content      | 知识内容       | 是   | 返回的知识的内容                                             |
| triggerTxt   | 触发知识的文本 | 否   | string                                                       |
| triggerTime  | 触发知识的时间 | 否   | timestamp                                                    |
| extJson      | 扩展字段       | 否   | 扩展Json对象                                                 |
| logId        | 本次返回标识   | 是   | String                                                       |

extJson字段说明

| key         | 说明         | 必选 | 内容                                                         |
| :---------- | :----------- | :--- | :----------------------------------------------------------- |
| completed   | 识别文本状态 | 是   | 取值1和3。1，过程中逐字识别结果；3，vad切分产生的完整句      |
| snStartTime | 文本开始时间 | 是   | 毫秒级，样例：00:00.100                                      |
| snStopTime  | 文本结束时间 | 是   | 毫秒级，样例：00:01.100                                      |
| speed       | 完整句语速   | 否   | 完整句字数 / 秒。其中，单个单词整体算作1个字，不会按实际字母数统计。 |

示例

```
{
	"callId" : "xxxxx.21532-32024.1519268928131-5812",
	"category" : "TXT",
	"content" : "医学考试时间",
	"logId" : "f579116d-7741-4bf4-b94b-caf2cdd650cf_-1",
	"roleCategory" : "AGENT",
	"triggerTime" : 1519268932820，
	"extJson":{
		"snStopTime":"00:00.900",
		"speed":"1.25",
		"snStartTime":"00:00.100",
		"completed":3
	}
}
```

## 统一错误处理

SDK大部分方法遵循类C风格的统一错误处理方案：返回值非0为错误，若错误，具体的错误信息将传入参数err_msg。如：

```
auto cntl = global_controller();
std::string err_msg;
if (cntl->init(..., err_msg) != 0) {
	std::cout << "failed to xxxxx: " << err_msg << std::endl;
	// 错误处理
};
```

错误码均定义在enum ERROR_CODE中(itma_define.h). 在运行中遇到的错误请查看err_msg参数以及日志说明。

## 资源回收

一次识别结束后，使用

```
global_controller()->release_session(session)
```

来释放此次识别Session的资源。不调用此方法会造成内存泄漏。

当不再需要使用所有功能后，使用如下方法回收SDK的资源：

```
while (true) {
  if (global_controller()->try_stop(err_msg) != 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } else {
    break;
  }
}
```

## Demo 示例

以下为Demo运行的示例日志（已关闭debug日志）

```
2018-03-27 21:18:15,504 INFO [ITMA] [itma_controller.cpp:59] 139640604037376 My SDKID is CPP_0.1.3-BIBIBI-30216_29255
2018-03-27 21:18:15,585 INFO [ITMA] [token_holder.cpp:61] 139640604037376 Token refreshed: 24.8dd9b6580e51dc52e9e4a693342279a2.2592000.1524748695.282335-10811527:1524748695585
2018-03-27 21:18:15,585 INFO [ITMA] [download_runner.cpp:63] 139640563357440 [Download] start nth: 0
2018-03-27 21:18:15,607 INFO [ITMA] [itma_controller.cpp:460] 139640325535488 New session created 0x7f00840094f0 BIBIBI-30216_59273.1522156695607101933-962863
[Demo] Now recognize file salesman.wav customer.wav with id: BIBIBI-30216_59273.1522156695607101933-962863
2018-03-27 21:18:16,586 INFO [ITMA] [upload_runner.cpp:44] 139640552867584 [Upload] Started, nth: 0
2018-03-27 21:18:16,645 INFO [ITMA] [itma_controller.cpp:145] 139640563357440 Speech started: "BIBIBI-30216_59273.1522156695607101933-962863"
[Demo] [Session:0x7f00840094f0] call BIBIBI-30216_59273.1522156695607101933-962863 started at server, user_param: 0
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, TXT: 您好，
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, CLIENT, TXT: 诶您好，
2018-03-27 21:18:26,611 INFO [ITMA] [upload_runner.cpp:44] 139640552867584 [Upload] Started, nth: 1
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, TXT: 哎您好先生不好意思这么晚也打扰到您了这边是信用卡中心的，今天上午有跟您来过电的您还记的吗？
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, TXT: 诶您，
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, CLIENT, TXT: 几点我知道，
2018-03-27 21:18:36,699 INFO [ITMA] [upload_runner.cpp:44] 139640552867584 [Upload] Started, nth: 2
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, TXT: 呃就是说当时您说呃不太听的见不方便是吧，
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, TXT: 哦就是说您这个呃像像您不是说您还有其他银行的信用卡对吧，
2018-03-27 21:18:46,786 INFO [ITMA] [upload_runner.cpp:44] 139640552867584 [Upload] Started, nth: 3
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, CLIENT, TXT: 银行的卡我肯定有啊，
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, TXT: 呃您其他银行的信用卡先生，
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, INTENT: 信息确认
2018-03-27 21:18:56,874 INFO [ITMA] [upload_runner.cpp:44] 139640552867584 [Upload] Started, nth: 4
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, TXT: 呃没有是吧，
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, CLIENT, TXT: 没有，
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, TXT: 那天您名下有房贷吗？
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, INTENT: 信息确认
[Demo] call BIBIBI-30216_59273.1522156695607101933-962863, AGENT, TXT: 嗯对，
```

## 分布式使用

识别服务支持分布式。 启用压缩：由于压缩算法具有一定的效果优化策略，需确保一通电话单侧（坐席|客户）语音流由同一个SDK实例上传。 非压缩：可由同一SDK实例上传，也可采用一通电话的音频数据从不同的SDK实例（不同进程、不同机器）上传。 若有疑问，可通过工单或商务合作联系我们。

# FAQ

1. Q：运行的时候报错 curl_easy_perform() failed: Unsupported protocol

   A：请确保link openssl。或修改配置将所有服务端地址修改为http（不推荐，不安全）

2. Q：Quota limit reached

   A：并发或日配额超限，请购买配额或通过工单联系我们合作。在使用过程中，开发者需要自行维护并行的通话数，确保没有超过您允许的最高并发数。如果是单机使用，可以在内存中维护正在通话的数量，如果是分布式使用，可以使用Redis等方案进行统计和控制。

3. Q：No permission to access data

   A：确保在ai.baidu.com控制台勾选了对应权限，并在config.json中设置了正确的app.scope

# 联系我们

- [商务合作](https://cloud.baidu.com/survey/BICCApply.html)反馈；
- [工单系统](http://ticket.bce.baidu.com/?_=1530034880766/#/ticket/list)反馈，工单类型为“人工智能”->“智能呼叫中心”；
- 电话：010-50802520（10:00~18:00）；
- 邮箱：ics-bd@baidu.com；
- 社区：[智能呼叫中心](http://ai.baidu.com/forum/topic/list/196)；