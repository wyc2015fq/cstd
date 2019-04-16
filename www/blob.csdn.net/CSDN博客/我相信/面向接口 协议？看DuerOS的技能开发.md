# 面向接口/协议？看DuerOS的技能开发 - 我相信...... - CSDN博客





2019年03月07日 22:13:21[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1890









一般地，开发一个对话系统或者机器人问答系统会涉及诸多领域的技术，除了硬件系统之外，还包括语言识别，自然语言处理/识别，知识图谱的搭建，自然语言生成及TTS播报等等，这对于企业及开发者个人而言，几乎是难以完成的任务。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGSk3V15W1c1KsibicIYbGOoySwK7UgwnBsQiapia6G8LPhIfhN0yoGjCV1A/640?wx_fmt=jpeg)

有幸的是，对话式AI操作系统例如DuerOS的诞生，使我们可以直面业务逻辑，相对轻松地完成语音类服务的开发与实现。在过去的研发中，经常使用的往往是面向接口的设计方法，那么，面向对话式操作系统的开发，是如何实现的呢？

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGs5WWN7WPRcp40nu4O9FO5V0rvj35nyI5L7TF568u0LiaOFNJkzpYq9w/640?wx_fmt=jpeg)

## 接口

在OO的时候，通常会讲到SOLID原则： 
- 
单一职责

- 
开闭原则

- 
里氏替换

- 
接口隔离

- 
依赖反转


其中接口隔离是其中的一项重要原则。接口的概念外延较广泛，wiki上的解释是这样的：

`An interface is a shared boundary across` which two or more separate components of a computer system exchange information. The exchange can be between software, computer hardware, peripheral devices, humans, and combinations of these.

接口大体上从载体上分为硬件接口和软件接口，从交互上可以分为人机接口和机-机（M2M）接口。软件中的接口为组件可以提供了常量、数据类型、过程类型、异常规范和方法签名等。有时，公共变量也被定义为接口的一部分。接口是一种很高的抽象，很多时候都会涉及到接口，因此要注意上下文和具体的应用场景。

在面向对象的编程语言中，尤其是不支持多继承的编程语言，往往有着特殊的具体含义，interface 一词成为了关键字，例如Java。Java接口中所用的方法一般都是抽象方法和常量，但是在Java8及以后的版本中，开始支持静态成员以及默认的函数实现，例如：

```
public interface MyTestInterface {
    int MAX_STORAGE_SIZE = 1024;

    default void doTest() {
        System.out.println("Hello, Test!");
    }
}
```

尽管如此，作为一个从Java 1.2 开始使用的老程序员而言，Java的发展还是略显迟缓。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGPluCEiauIuBWD3oheQgMRk90UFGjLQiaTkibqaB93jjQzeSzAzicUnQcvQ/640?wx_fmt=jpeg)

关于接口描述语言（IDL），大约最早出现在是CORBA的规范中，自己在1999年初次使用IDL的时候，非常感叹它的神奇，在当时简直就是跨平台开发的基础。实际上，OMG IDL不是作为程序设计语言体现在CORBA体系结构中的，而是用来描述产生对象调用请求的客户对象和服务对象之间的接口的语言。IDL文件描述数据类型和方法框架，而服务对象则为一个指定的对象实现提供上述数据和方法。一般地，IDL文件描述了服务器提供的服务功能，客户机可以根据该接口文件描述的方法向服务器提出业务请求。在大多数CORBA产品中都提供了IDL到相关编程语言的编译器。 在Android的应用开发中，AIDL实际上就是IDL的一个领域应用而已。

那么接口和协议有什么联系和区别呢？

## 协议

协议，一般是指通信协议，例如大家熟知的互联网协议——TCP/IP。网络是个大话题，可以参见[老曹眼中的网络编程基础](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658971183&idx=1&sn=f834681a59249700df838fd70097a045&chksm=80d33309b7a4ba1feb0376498474298abd484bf79acd0bac80f2feef6ca1120ae715c0318343&scene=21#wechat_redirect)。

在编程语言中，协议指示的是调用方和目标对象之间的交互链。一般描述为：
- 
对象可以理解的消息。

- 
这些消息可能附带的参数。

- 
这些消息返回的结果类型。

- 
尽管修改了对象的状态，但仍然保留的不变量。

- 
需要由客户端处理到对象的异常情况。

- 
对于通信而言，还包括方法的调用序列和决策点，例如在UML交互图中的表示：通信图、序列图、交互概述图/活动图等。


在面向对象的编程语言中，同样地，也有把protocol作为关键字的编程语言，例如Objective-C。在Objective-C中，同样不支持多继承，即不允许一个类有多个父类，于是Objective-C提供了类似的实现方法，也就是协议。协议有点类似于Java里的接口，不同点就是在协议里，可以提供可选的方法，不要求全部继承。例如：

```
@protocol myprotocolName
- (void)requiredMethod;
@optional
- (void)anOptionalMethod1;
- (void)anOptionalMethodn;
- (void)otherOptionalMethod;
@required
- (void)anotherRequiredMethod;
@end
```

Objective-C的协议包括正式协议和非正式协议，这里不再赘述。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGJXI1ZDE0c35a2O1wLHN1noq8ATtob5SaEnNp78qucPGIleWXeA2jXA/640?wx_fmt=jpeg)

协议和接口在很多时候是交叠的，但视角不同，接口面向的是实体对象，而协议聚焦在交互上。本质上，任何的协议都是有字典和语法两部分组成，从而形成通信上的共识。

对程序员而言，往往更关注传输协议和应用协议。传输协议主要完成数据装配，多路复用，差错检测和流量控制。应用协议才是具体业务的数据和状态描述及内容交互。

那么，对话系统的应用开发是如何面向接口和协议的呢？

## 对话系统的应用技能开发

DuerOS 是对话式人工智能交互系统，简称对话式AI系统。应用DuerOS的典型产品之一就是智能音箱。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGPm9thc8cIDdZMeZaicMDE06Uhtics1wjLlP5DLnCDElicq7J26e0X5Yicw/640?wx_fmt=jpeg)

谈到在智能音箱上的应用开发，往往让人联想到使用音箱提供的SDK，例如Android SDK 或者 Linux的SDK，然后将应用下载到音箱上。这种理解还停留在多年前的APP开发阶段，对人工智能操作系统存在着极大的误区，可以参见[感知人工智能操作系统](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974036&idx=1&sn=95c6ecf9a6462f06f2ce9dcba453e64b&chksm=80d32472b7a4ad649ca242ff41116d46c95e9a49545d4dbc68470680f9e5684781ecda260ea9&scene=21#wechat_redirect)。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGibEzYfMuYhlULansUBalEK61auCnaMGaXs3icvc17Rats1ibh2nWrrS5A/640?wx_fmt=jpeg)

在基于DuerOS的设备例如智能音箱上，应用的开发和Web服务的开发没什么区别。 简单地，可以把智能音箱理解成浏览器，只是原来的鼠标点击和键盘输入换成了语音交互即可。在智能设备上开发应用是通过DuerOS Bot Platform（简称DBP）实现的，交互协议称为DuerOS Conversation Service（简称DCS），包括了智能终端与DuerOS之间的协议，和DuerOS与应用服务（Bot）之间的协议。

作为开发者， 我们主要实现DuerOS与应用服务（Bot）之间的协议，方便起见，把它也叫做DBP协议。

## DBP 协议浅析

DBP协议把HTTP/HTTPS 作为传输协议，关于http的相关内容可以参见[温故知新，HTTP/2](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658973776&idx=1&sn=bf0978f14d6c826961a42db30ba38e93&chksm=80d32576b7a4ac60de414647467c6a76164aa0f2ecd275d35b7bf34061ab6b2fe1a266c2e361&scene=21#wechat_redirect)。 DBP协议中应用协议的数据是通过JSON来表述。

### 请求与响应

智能音箱上的应用实际上就是对DBP协议中的请求作出响应的Web服务。 一个典型的request 结构示例如下：

```
{ 
    "version": "2.0",
    "session": {
    },
    "context": {
        "System": {
            "user": {
            },
            "application": {
            },
            "device": {
                "deviceId": "{{STRING}}",
                "supportedInterfaces": {
                    "VoiceInput": {},
                    "VoiceOutput": {},
                    "AudioPlayer": {},
                    "VideoPlayer": {},
                    "Display": {}
                }
            }
        },
        "AudioPlayer": {},
        "VideoPlayer": {}
    },
    "request": {}
}
```

session表示用户会话信息，一次session过程是从开始用户调起技能到结束，表示用户与技能的一次会话。

Context描述了设备端的状态数据，能力配置参数以及用户相关信息，包括System即系统参数和播放器的状态。

request 才是具体的payload， 是经DuerOS 经过AI处理后的用户请求。

Response 是开发者实现的主要内容， 结构示例如下：

```
{
    "version" : "2.0",
    "context" : {
        "intent" : {
            "name" : "{{STRING}}",
            "slots" : {
                "{{STRING}}" : {
                }
            }
        },
        "expectResponse" : [
        {}
    ],
    },

    "session" : {
        "attributes" : { 
            "{{STRING}}": "{{STRING}}"
        },
    },
    "response" : {
        "outputSpeech" : {
            "type" : "{{STRING}}",
            "text" : "{{STRING}}",
            "ssml" : "{{STRING}}",
        },
        "reprompt" : {
            "outputSpeech" : {
                "type" : "{{STRING}}",
                "text" : "{{STRING}}",
                "ssml" : "{{STRING}}",
            }
        },
        "card" : {}
        "directives" : [],
        "expectSpeech": {{BOOLEAN}},
        "shouldEndSession" : {{BOOLEAN}}
    }
}
```

Context用于反馈给DuerOS的intent结果；Intent是技能应用认为对本次query更加合理的意图解析，expectResponse用于推测用户可能的回复，DuerOS会在后续的query中优化意图解析模型。关于意图和槽位，可以参见[感知自然语言理解（NLU）](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974048&idx=1&sn=8890276ba789c43d295333f4d926f282&chksm=80d32446b7a4ad502d94271f67c96c55371db52df85963367c0b65ec5dbfa77477ff56f30b81&scene=21#wechat_redirect)。 

session存储了在DuerOS 会话的属性数据，如果本次session不结束，那么在下一个发送给技能的请求中，在session.attributes字段会携带这些属性给到至技能应用，相当于数据到终端上绕了一圈。

response是技能应用回复给DuerOS的payload，包括语音播报的内容和风格，提示性建议，展现输出和技能指令等。

一对Request/Response构成了DBP协议的主体。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGibEzYfMuYhlULansUBalEK61auCnaMGaXs3icvc17Rats1ibh2nWrrS5A/640?wx_fmt=jpeg)

### 事件和指令

事件和指令是DBP协议中的重要消息形式，从智能终端和DuerOS发往我们开发的技能应用的消息称为事件，从技能应应用发往智能终端和DuerOS的消息称为指令。

在用户通过语音输入后，DuerOS会对语音请求进行识别和理解，并将理解结果发送给技能应用。有三个事件相当于定义了应用技能的生命周期：
- 
LaunchRequest：代表开启技能应用

- 
IntentRequest：相当于进入技能应用的消息处理循环

- 
SessionEndedRequest：相当于被动结束技能应用，当然，技能应用可以主动退出。


智能终端上的处理状态事件是通过DuerOS透传给技能应用的，主要包括音频播放器audioplayer的音频播放事件集和视频播放器videoplayer的视频播放事件集，对于有屏终端而言，还包括form事件，即用户屏幕上的点击事件等。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGsdyMPxxmERJ1n1HpOyPJwqZw4Gt9dJficrXt0YyVDfaTgoJLAicSoSWA/640?wx_fmt=jpeg)

具体地，音频事件包括：
- 
AudioPlayer.PlaybackStarted事件

- 
AudioPlayer.PlaybackStopped事件

- 
AudioPlayer.PlaybackFinished事件

- 
AudioPlayer.PlaybackNearlyFinished事件

- 
AudioPlayer.ProgressReportIntervalElapsed事件





视频播放事件包括：
- 
VideoPlayer.PlaybackStarted事件

- 
VideoPlayer.PlaybackStopped事件

- 
VideoPlayer.PlaybackFinished事件

- 
VideoPlayer.PlaybackNearlyFinished事件

- 
VideoPlayer.ProgressReportIntervalElapsed事件

- 
VideoPlayer.ProgressReportDelayElapsed事件

- 
VideoPlayer.PlaybackStutterStarted事件

- 
VideoPlayer.PlaybackStutterFinished事件

- 
VideoPlayer.PlaybackPaused事件

- 
VideoPlayer.PlaybackResumed事件

- 
VideoPlayer.PlaybackQueueCleared事件

- 
PlaybackScheduledStopReached事件


Form事件包括显示控件的点击事件，主要包括Form.ButtonClicked和Form.RadioButtonClicked事件等。为了方便开发，增强展示形式的表现力，DBP协议还提供了展现卡片和展现模版，对于的事件包括Display.ElementSelected事件和Display.ButtonClicked事件。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGlfNRxYnu80AicK6AKMjoXG4cK69ChYIXZ8OtVOb6h5ulpc855WgXjYg/640?wx_fmt=jpeg)

对这些事件的处理，技能应用相当于得到了用户、智能设备状态和DuerOS数据的输入，然后通过指令（directive）的形式完成交互。

在对话过程中，技能应用可以发出的指令有Dialog.ElicitSlot， Dialog.ConfirmSlot，Dialog.ConfirmIntent和Dialog.Delegate，分别用于槽位的填充和确认，意图的确认，以及通知DuerOS来处理NLU。

在音/视频播放中，技能应用可以发出的指令有AudioPlayer.Play和AudioPlayer.Stop 来通知音频播放器开始和停止播放，VideoPlayer.Play和 VideoPlayer.Stop来通知视频播放器开始和停止播放 VideoPlayer.ClearQueue用来清除播放的资源队列。

在有屏的智能设备上，Form的事件处理可以理解成一般的HTML表单处理，而展示模版的指令只有一个Hint，用于展现技能应用的引导词。技能引导词是技能展现的提示信息，引导用户与技能应用进行交互。每个技能都可以设计引导词，让用户更快速的使用技能。

由此可见，DBP协议的主要部分相对简单，清晰明了。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczryxPVDMK5LEKUD1ZPz7YeGhCv0OicU08tjwhSrkfoEW6iajrojcqg5ibZL8SCATjAZWIb2vyd4B2TTA/640?wx_fmt=jpeg)

## 小结

即使理解了协议的格式和内容，自己实现整个DBP协议也是需要一定的工作量的。所幸的是，DBP平台提供了多种编程语言的SDK，对DBP协议的实现进行了封装，基于这些SDK（Java/JavaScript/Go/PHP/Python），我们的开发变得相对简单，使我们可以聚焦于应用的业务逻辑。

DBP平台还提供了大量的技能开发模版，相当于简化的开发框架，使对话式AI系统的技能应用开发更为简单。另外，DBP平台提供的小技能开发，更是无需编程即可实现一些简单技能应用的开发。

##### 参考资料
- 
https://dueros.baidu.com/dbp

- 
Pugh Ken，“Interface-Oriented Design”，Pragmatic Bookshelf，2006

- 
威廉·斯托林斯，《数据与计算机通信（第十版）》，电子工业出版社，2015









