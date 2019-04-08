# 智能电销 JAVA SDK文档

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

| 版本  | 日期       | 说明                                        |
| :---- | :--------- | :------------------------------------------ |
| 0.1.0 | 2017.08.22 | 第一版！                                    |
| 0.2.0 | 2017.10.23 | 修复服务器连接不稳定的问题                  |
| 0.3.0 | 2018.06.19 | 引入session机制，增加语速、识别时间结果返回 |

## 接入准备

- 参考“流程说明”—>“服务申请”完成权限申请；
- 点击[智能呼叫中心-智能电销Java SDK](http://ai.baidu.com/sdk#itma)完成Java SDK下载；
- 开发环境Java版本升至1.8及以上；
- 为节省公网传输带宽，SDK内置语音压缩算法（压缩比例1/8），若启用压缩功能，请在linux x64操作系统下集成，非压缩支持Linux，Windows，Mac；
- 开发机对SDK内conf/sdk.properties配置中“service.http.server.url”（服务访问）、“token.http.server.url”（鉴权）地址开放公网访问权限；
- 编译并运行SDK自带demo程序，熟悉SDK工作过程。

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
| 格式     | Json字符串，详见下文识别结果说明                             |
| 语音识别 | utf8编码，实时逐字识别文字， 由静音检测识别出的完整句，完整句文字对应开始&结束时间，语速 |
| 语义识别 | utf8编码，意图、槽位                                         |
| 输出方式 | 回调，实时返回                                               |

> 语速 = 完整句字数 / 秒。其中，单个单词整体算作1个字，不会按实际字母数统计。

## SDK 目录

```
- conf 目录下有默认的配置文件sdk.properites 
- libs 库文件目录。需要复制该目录到您自己项目里集成
- src
	|--- main 
		|---java 目录下有demo项目的java文件
		|---resources 目录下默认的日志配置文件logback.xml
	|--- test
- resources 测试音频文件目录     

以下文件和目录可以忽略：
gradle* 为gradle命令相关的文件
.idea   IntelliJ IDEA的 IDE项目文件
```

## Demo 运行

- 修改conf/sdk.properties 文件 参考“流程说明”—>“免费账号和服务申请”流程，进入应用“概览”页面，点击左侧导航“应用列表”，查询“API Key”和“Secret Key”。

```
app.appKey = API Key
app.appSecret = Secret Key
app.scope = brain_ai_talker
```

- 编译及运行

Windows：

```
gradlew run --no-daemon

//如果安装过cygwin， 可以运行 
sh run-test.sh
```

Linux Mac

```
sh gradlew run  --no-daemon 

//或者 
sh run-test.sh
```

任意平台IntelliJ IDEA 软件

```
 File->Open... => 选中项目目录打开
```

## 调用接口及参数说明

**配置文件（sdk.properties）参数说明**

| 参数名                               | 客户必填         | 含义                              | 说明                                                         |
| :----------------------------------- | :--------------- | :-------------------------------- | :----------------------------------------------------------- |
| app.appKey                           | 必填             | api key                           | 从网页上申请应用的API Key                                    |
| app.appSecret                        | 必填             | secret key                        | 从网页上申请应用的Secret Key                                 |
| app.scope                            | 必填             | 服务类型                          | 智能电销 值为“brain_ai_talker”                               |
| app.machineName                      | 必填             | SDK名称                           | 您自定义的sdk名称，注意一份配置文件用一个sdk名称，方便区分   |
| upload.threadNum                     | 非必填，可用默认 | 上传线程数                        | 根据网络和您的实际通话数，调整该值。                         |
| upload.maxThreadNum                  | 非必填，可用默认 | 上传线程数最大值                  | 如果发现您的网络不佳，会自动增加上传线程数到该值             |
| send.blockingQueue.size              | 非必填，可用默认 | Sdk内部队列最大允许的缓冲包的个数 | 如果网速较慢，则发送的包会卡在sdk, 如果卡住的包数量超过该值，则会抛出异常 |
| http.upload.requestDurationInSeconds | 非必填，可用默认 | 单个上传请求的保持时间            | 建议1800s，正常情况不需要修改                                |
| upload.http.chunkSize                | 非必填，可用默认 | 最大chunk尺寸                     | 建议6144字节，且比每个音频包略大， 每个音频包<=100ms，正常情况不需修改此参数 |
| service.http.server.url              | 使用自带配置     | 百度云服务访问地址                | 遇见https错误可改为http                                      |
| token.http.server.url                | 使用自带配置     | 百度鉴权访问地址                  | 遇见https错误可改为http                                      |

**全局接口**

1、启动阶段初始化controller 仅调用一次，多次调用会报错，主要工作为发起鉴权请求，初始化线程等，可能比较耗时。

```
Controller controller = new Controller(new LogBeforeUploadListener(), new PrintAfterDownloadListener());
// LogBeforeUploadListener 为IBeforeUploadListener的实现类
// PrintAfterDownloadListener 为 IAfterDownloadListener的实现类，打印收到的数据
```

2、新建session的Config，设置识别参数 Session.Config 类参数说明

| 参数名   | 类型   | 是否常用 | 默认值                                                       | 说明                                                         |
| :------- | :----- | :------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| roleId   | RoleId | 常用     | 必填                                                         | AGENT(0), CUSTOMER(1), BOTH(2)三选一， AGENT单路坐席音频， CUSTOMER 单路用户音频， BOTH， 本次通话有上述2个音频 |
| compress | bool   | 常用     | 必填                                                         | 是否需要压缩                                                 |
| agentDn  | int    | 不常用   | 选填，默认0                                                  | 坐席号，会回传                                               |
| params   | Map    | 不常用   | 切句模式等定制化参数，具体设置见下表， 注意这里是Map， SDK内部会转为json字符串 |                                                              |

定制化参数params 请务必检查设置的字符串格式是否正确。 结果返回模式

```
"sentenceHandler": {
	"client": "STANDARD_COMPLETED",
	"agent": "STANDARD_COMPLETED"
}
```

上述示例对应的java代码为

```
// Session.Config config 

Map<String, Object> sentenceHandlerParams = new LinkedHashMap<>();
sentenceHandlerParams.put("agent", "STANDARD_COMPLETED");
sentenceHandlerParams.put("client", "STANDARD_COMPLETED");
Map<String, Object> params = new LinkedHashMap<>();
params.put("sentenceHandler", sentenceHandlerParams);
```

client 和 agent表示对应用户与坐席的返回类型设置，支持的结果返回模式包括

| 值                 | 含义         | 备注                               |
| :----------------- | :----------- | :--------------------------------- |
| STANDARD_COMPLETED | 完整句切句   | 屏蔽掉所有过程句子，只下发完整句。 |
| NONE               | 实时翻译结果 | 默认值，逐字识别结果及完整句       |

接口调用

```
Session.Config config = Session.createConfig(Session.Config.RoleId.AGENT, false);
// AGENT 表示坐席的单路电话识别， 
// false 表示不启动传输压缩， 传输压缩只能在linux上启用
```

3、服务停止结束controller 仅调用一次，多次调用会报错，主要工作为释放资源。

```
controller.stop();
```

**单通电话调用**

每通电话分为三种状态：电话开始、电话过程中、电话结束。服务过程中不断循环这三个状态的调用。

1、电话开始 初始化session并发送开始包

```
Session session = controller.startSession(config);
```

如果发送正常的话，会有如下日志

```
{"roleId":0,"callId":"s1-ASR-26356-1-1528802449609","name":"speech_start","sendTs":1528802449609,"beforeCompressTs":0,"afterCompressTs":0,"beforeUploadTs":1528802449634,"compress":0,"sdkId":"s1-java0.3-26356-0eff1144","connectionId":"uploadT-2","ts":1528802449637,"seqId":0,"agentDn":"123","rate":8000,"params":"","heartBeat":false,"logid":"uploadT-2-Event:speech_start[s1-ASR-26356-1-1528802449609--0-P]-0"} 
```

2、电话过程中

语音流发送接口
语音流发送间隔20ms（160字节）~160ms（2560字节）内任意10的倍数。
推荐发送间隔为160ms，语音流长度2560字节。

如果一通电话（callId）有两路通话的话，坐席一侧语音流发送调用sendFirstRoleSpeech，用户的那一路数据调sendSecondRoleSpeech。
如果只有一路通话，请不要调用sendSecondRoleSpeech。

```
session.sendFirstRoleSpeech(bytes); // 一通电话只有坐席或者用户一路电话。  如果roleId=2， 这里表示坐席的那一路电话。
session.sendSecondRoleSpeech(bytes); // 如果roleId=2 会有第二路通话。这里表示用户的那一路电话。
```

若启用压缩，两方法均有一定耗时。 调用语音流发送接口，SDK通过IBeforeUploadListener 回调接口返回处理状态。 其中，重点关注void onSendLocalException(Exception exception, int level);

```
public interface IBeforeUploadListener extends ILevel {

/**
* 上行时遇到异常回调
*
*  level = 8 sdk遇见异常，但是通常会自行处理
*  level = 16 表示SDK已经不能处理该异常，此时用户需要关注, 建议立刻排查问题
*
* @param exception
* @param level
*/
void onSendLocalException(Exception exception, int level);

/**
* 排查性能时使用：该包的即将上传
* @param info
*/
void onBeforeUpload(SendInfo info);

/**
*  排查性能时使用：该包上传完毕
* @param info
*/
void onAfterUpload(SendInfo info);

/**
* 留空，忽略该接口即可
* @param info
*/
void onBeforeCompress(SendInfo info);

/**
* 留空，忽略该接口即可
* @param info
*/
void onAfterCompress(SendInfo info);
}
```

识别结果回调接口 SDK使用回调的形式告知用户报错消息和返回结果

```
/**
* 下行监听
* <p>
*/
public interface IAfterDownloadListener {
/**
* 服务器正常返回的内容，里面有识别结果和其它有用的业务信息
*
* @param json
*/
void onReceive(String json);

/**
* 返回json里有未知name值， 通常代表出错
*
* @param json
*/
void onReceiveError(String json);

/**
* 下行时SDK报错
*
* @param exception
* @param level level = 16 表示非常严重的错误，不能忽视
*/
void onRecieveLocalException(Exception exception, int level);
}
```

3、电话结束
发送结束数据包
sendEndSpeech 发送通话结束包，如果不发送的话，当前CallId会占用您的配额。
destroy请在sendEndSpeech 后调用，调用后Session内部释放资源，再次调用send*方法会报错。

```
session.sendEndSpeech(); // 必须等坐席用户全部发送完成，才能end。
session.destroy(); // 释放压缩传输数据的资源
```

## 识别结果

Json字符串形式，字段说明如下：

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

extJson字段说明：

| key         | 说明         | 必选 | 内容                                                         |
| :---------- | :----------- | :--- | :----------------------------------------------------------- |
| completed   | 识别文本状态 | 是   | 取值1和3。1，过程中逐字识别结果；3，vad切分产生的完整句      |
| snStartTime | 文本开始时间 | 是   | 毫秒级，样例：00:00.100                                      |
| snStopTime  | 文本结束时间 | 是   | 毫秒级，样例：00:01.100                                      |
| speed       | 完整句语速   | 否   | 完整句字数 / 秒。 其中，单个单词整体算作1个字，不会按实际字母数统计。 |

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

## jvm 启动参数

```
-Daip.talker.conf.filename=conf/sdk.properties   //sdk.properties路径
-Dlogback.configurationFile=/path/to/logback.xml //logback.xml路径
```

## 日志配置

日志使用[logback框架](https://logback.qos.ch/)， 配置文件为src/main/resources/logback.xml。 详细配置请访问[logback配置](https://logback.qos.ch/manual/configuration.html)

## 报错信息及原因

demo除了HttpClient库报错外，共有以下三大类异常：

1. InitException 初始化报错，通常在初始化过程中，配置或者调用的次序不正确；
2. SendException 发送出错， 通常在发送语音数据的时候，未按照指定的要求；
3. LevelException SDK运行过程中，如与服务端交互时，网络异常等因素。其中level等于8，表面有异常情况发生。16表示sdk遇见严重错误。

| 报错内容                                         | Exception              | 说明                                                         |
| :----------------------------------------------- | :--------------------- | :----------------------------------------------------------- |
| Conf file not existed                            | InitException          | 没有配置文件，demo中为conf/sdk.properties                    |
| Conf file load failed                            | InitException          | 读取配置文件遇见IOException                                  |
| the specified key is not set in conf file        | ConfigurationException | 配置文件中没有xx参数，请确保demo中的conf/sdk.properties没有额外的修改 |
| callId is not correct. pattern is                | InitException          | 如果是自定义callId， 需要按照报错中pattern的正则表达式，推荐机器名+uuid |
| jni libConverter.so load failed                  | InitException          | 没有找到libConverter.so ， 确保您在linux x64 环境下运行，并且 libConverter.so在您的库目录下。 参见demo设置的-Djava.library.path参数 |
| token init failed, maybe appKey appSecret error  | InitException          | 配置文件中的app.appKey 或者 app.appSecret填错                |
| audio data length is not correct, length         | SendException          | 发送数据包时，数据为0字节或者超过5120字节                    |
| Session has been destoried                       | SendException          | 调用sendFirstRoleSpeech时， session已经调用过destory         |
| roleId is not BOTH or Session has been destroyed | SendException          | session初始化时指定的不是2路通话， 但是用户调用了sendSecondRoleSpeech。 或者session已经调用过destory |
| Extra Upload commands too many, num=             | LevelException         | 与服务端通讯时，响应速度过慢，SDK会自动新增上传线程。 如果线程数超过upload.maxThreadNum，会有此错误， 此时数据包被过分积压或者与服务端传输数据时耗时严重，不能实时识别 |
| token check failed                               | LevelException         | token没有实时语音识别或者智能电销的权限                      |
| Event check fail to recieve ack, times:3         | LevelException         | 重试3次后，在规定的时间均未得到服务端的确认， 请检查是否有包积压或者网络通讯问题 |

## 分布式运行

识别服务支持分布式。 启用压缩：由于压缩算法具有一定的效果优化策略，需确保一通电话单侧（坐席|客户）语音流由同一个SDK实例上传。 非压缩：可由同一SDK实例上传，也可采用一通电话的音频数据从不同的SDK实例（不同进程、不同机器）上传。

# 联系我们

- [商务合作](https://cloud.baidu.com/survey/BICCApply.html)反馈；
- [工单系统](http://ticket.bce.baidu.com/?_=1530034880766/#/ticket/list)反馈，工单类型为“人工智能”->“智能呼叫中心”；
- 电话：010-50802520（10:00~18:00）；
- 邮箱：ics-bd@baidu.com；
- 社区：[智能呼叫中心](http://ai.baidu.com/forum/topic/list/196)；