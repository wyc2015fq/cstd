# 用JavaScript打造AI应用-从Nodejs SDK 看DuerOS的技能开发 - 我相信...... - CSDN博客





2019年04月10日 21:21:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：14标签：[人工智能																[AI																[DuerOS																[JavaScript](https://so.csdn.net/so/search/s.do?q=JavaScript&t=blog)
个人分类：[AI](https://blog.csdn.net/wireless_com/article/category/7470805)





为什么要掌握JavaScript呢？ 使用JavaScript能能否开发AI应用么？

答案是肯定的。

### 全栈语言JavaScript

就全栈编程语言而言，与python 并驾齐驱的要算是JavaScript了：

基于JavaScript的前端框架百花齐放，Vue、React、Angular都有广泛的应用；

桌面应用有NW.js(Node +webkit)，以及现在的Electron;

嵌入式系统中，也有着Espruino(被称为微控制器的 JavaScript),Tessel (一个集成了Wi-Fi的JavaScript 微处理器)以及国内的ruff.io，详见拙文《[探索嵌入式应用框架（EAF）](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658972846&idx=1&sn=c14af87e9fffcc3d971ee2a492259824&chksm=80d32988b7a4a09ed7281c8b384385e6e913a14d3c413f003a746726ae33a1f5b0ed607c0107&scene=21#wechat_redirect)》；

后台服务基本上就是Nodejs的世界，有着丰富的工具集；

在人工智能领域，就机器学习而言，Javascript也有着诸多的开源框架，TensorFlow.js， Brain.js ， Synaptic.js等

......

关于JavaScript 的一些编程基础，可以参见《[全栈必备JavaScript基础](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658971386&idx=1&sn=d5b529569c01a1f43b9629bea10542af&chksm=80d333dcb7a4baca756bba381a6bbf47f34eb83326cdd4ebe36573984f67e5f53ff49801f3fc&scene=21#wechat_redirect)》。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqB1TsX9ibW8GkT7CKAXZHNgtKEicnoFrgRsT5v6BibJyfYGxqCe2qhWVXH4bvPrUbHicUTfeb59iaS1PQ/640?wx_fmt=jpeg)

那对于JavaScript的开发者如何开发人工智能相关的应用呢？

这还是需要明确具体的应用场景，但是人工智能操作系统（可参见《[感知人工智能操作系统](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974036&idx=1&sn=95c6ecf9a6462f06f2ce9dcba453e64b&chksm=80d32472b7a4ad649ca242ff41116d46c95e9a49545d4dbc68470680f9e5684781ecda260ea9&scene=21#wechat_redirect)》一文）的产生扩大了AI应用的领域。就对话式AI系统（例如DuerOS）而言，平台化更是Javascript开发者的福音。基于AI操作系统和开发平台，Javascript开发者可以更加高效地开发AI应用。

### DuerOS 的 Nodejs 应用示例

关于DuerOS的详细介绍，可以参见《[面向接口/协议？看DuerOS的技能开发](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974063&idx=1&sn=8d15d4d764f6237a359556e6714e7943&chksm=80d32449b7a4ad5f43da9ce81a684d01795c2a8e40d27807dc036f661a9d7dadd2432e2cffd5&scene=21#wechat_redirect)》，当然对NLP的更多理解（可以参见《[感知自然语言理解（NLU）](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974048&idx=1&sn=8890276ba789c43d295333f4d926f282&chksm=80d32446b7a4ad502d94271f67c96c55371db52df85963367c0b65ec5dbfa77477ff56f30b81&scene=21#wechat_redirect)》一文），对于开发对话式AI系统上的技能应用，是大有裨益的。DuerOS提供的Javascript支持是开发技能服务（Skill Bot）的SDK，采用npm加载 , 支持node.js 6.10及以上版本。

构建一个DuerOS的技能服务，一般如下步骤： 

1）在DBP（DuerOS Bot Platform）创建并配置技能服务 

2）构建基于Nodejs的开发环境

3）创建web服务及调用入口 

4）编辑具体的业务逻辑 

5）调试后发布技能服务

关于在DBP平台创建和配置技能，以及调试和发布技能，可以具体参见官网说明dueros.baidu.com/dbp。

环境构建

基于DuerOS的技能服务开发，基本上只需要安装Node，Javascript bot SDK，以及相应的httpserver模块例如express，koa等，就可以完成开发环境的搭建：

```
cd /myworkspace
mkdir js-bot-test
cd js-bot-test
npm init
npm install express --save
npm install bot-sdk --save
```

Web 服务入口

和Java 开发技能服务类似（参见《[从Java SDK看DuerOS的技能开发](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974092&idx=1&sn=729894c83d899dda2659c57c1a34e4ea&chksm=80d324aab7a4adbc33676269f5d63ce03a9ca633bb99c3d6a055e28c435fde993c9cd66cda0f&scene=21#wechat_redirect)》），需要创建一个服务启动的入口，在这里是index.js:

```
const express = require('express');

const Bot = require('./Bot');
let myapp = express();

myapp.post('/', (req, res) => {
    req.rawBody = '';

    req.setEncoding('utf8');
    req.on('data', chunk => {
        req.rawBody += chunk;
    });

    req.on('end', () => {
        let mybot = new Bot(JSON.parse(req.rawBody));

        mybot.run().then(result => {
            res.send(result);
        });
    });
}).listen(8888);
```

使用express 构建了一个监听8888端口的web服务，完成与DuerOS平台之间的通信，在生产环境中，要增加证书的验证。

编写Bot的业务逻辑

最重要的，需要创建具体的技能服务，实现具体的业务逻辑，示例的Bot.js 文件组织结构如下：

```
const BaseBot = require('bot-sdk');
class Bot extends BaseBot {
```

```
constructor(postData) {
        super(postData);
        //服务入口请求的处理
            this.addLaunchHandler(() => {
                return {
                    outputSpeech: '欢迎使用!'
                };
            });
        //相关意图处理
            this.addIntentHandler('myintent1', () => {
                let targetSlot1 = this.getSlot('slot1');
        // 意图1 的逻辑处理以及槽位填充，return 相应的 directives 和 outspeech等
                }); 
            this.addIntentHandler('myintent1', () => {
                let targetSlot2 = this.getSlot('slot2');
                });
        //事件处理
            this.addEventListener（‘event1’，() => {
            //对于于事件1的相关处理，return 相应的 directives 和 outspeech等
            });
```

`//...... `


```
// 退出会话

this.addSessionEndedHandler(function({

// clear status

// 清空状态，结束会话。

return null;

});

}
```


`//其他业务逻辑的相关方法`

主要的框架还是对话的开启和关闭，以及对意图和事件的处理。所有的逻辑处理函数的返回结果可以是json Object 也可以是Promise包裹的json Object。

至此，就可以运行服务来调试技能了，一旦验证完成，即可在DBP 发布自己的技能了。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqB1TsX9ibW8GkT7CKAXZHNgJCWh8MHvcPiaxYJ93Tr4WzqffZX2jsXb0NpHghLMv8uk4ibWQntneIVg/640?wx_fmt=jpeg)

### DuerOS Javascript Bot SDK 代码浅析

“磨刀不误砍柴功”，理解JS Bot SDK的源代码，对基于Nodejs开发DuerOS的技能服务大有裨益。

在github上bot-sdk-node.js/lib/ 的目录下，是JS bot sdk 的核心代码，主要是6个文件：
- 
Bot.js

- 
Certificate.js

- 
NLU.js

- 
Request.js

- 
Response.js

- 
session.js


NLU 模块

NLU 模块主要完成槽位的设置\获取\确认，以及通过ask（）方法完成槽位填充。同时，可以获取意图的名称并确认意图及确认意图状态。需要注意的是setDelegate（）方法的使用，只有在DBP平台完成了意图配置，setDelegate（）才有效，该方法只是将明确的意图判断交给了DuerOS代为处理。

Session 模块

正像《[面向接口/协议？看DuerOS的技能开发](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974063&idx=1&sn=8d15d4d764f6237a359556e6714e7943&chksm=80d32449b7a4ad5f43da9ce81a684d01795c2a8e40d27807dc036f661a9d7dadd2432e2cffd5&scene=21#wechat_redirect)》一文中指出的，Session 和浏览器中的cookie 非常类似，用于在客户端和服务器之间传递持久化数据。Javascript bot sdk 中的Session 模块主要提供了getData（）和setData（）两个方法，用于存储/读取key/value形式的数据。

Certificate 模块

安全性一直是DuerOS 平台所关注的一个重要方面，在DuerOS 与技能服务bot之间是安全通信，尽管在技能调试的时候可以关闭证书的验证。Certificate 模块通过enableVerifyRequestSign()方法开启验证请求参数签名，阻止非法请求， disableVerifyRequestSign()关闭验证请求参数签名，verifyRequest()验证发送请求者是否合法。 关于DBP 平台的安全性，可以以后专门研究讨论。

Request 模块

Request 模块完成了对DBP协议request 请求的封装，主要功能包括：
- 
获取数据对象：getData()，getSession()，getNlu() 

- 
根据上下文获取终端状态信息和事件信息：getAudioPlayerContext()，getVideoPlayerContext()， getScreenContext() 和getEventData()

- 
获取用户信息： getUserId()，getQuery()，getLocation()

- 
获取应用信息： getBotId()，getApiAccessToken() ，getApiEndPoint() ，getExternalAccessTokens()

- 
判断对话状态：isLaunchRequest() ，isSessionEndedRequest() ，isDialogStateCompleted() 


Response 模块

Response 模块完成了对DBP协议Response响应的封装，主要功能包括：
- 
返回结果封装：buildResult(data)，buildResult(data)，defaultResult()是默认结果

- 
对话相关：setShouldEndSession（），

- 
异常处理：illegalRequest()，setFallBack() 是兜底话术

- 
麦克风控制： setExpectSpeech(expectSpeech)

- 
设置指令次序：setAutoDirectivesArrangement（），setStrictDirectivesArrangement()

- 
设置expectResponse：addExpectTextResponse(text)， addExpectSlotResponse(slot)


expectResponse用于推测用户可能的回复，开发者的技能Bot在响应DuerOS请求时，可以添加expectResponse 信息，告诉DuerOS用户在下次交互时可能话术的某些关键词，DuerOS将在下一轮对话中提高语音识别能力，进而提高了意图的准确性，从而提高了用户使用该技能的用户体验。

还有一个语音播报相关的方法formatSpeech(mix) ，该方法自动识别SSML和纯文体，另外在extension 目录下还有还TTS相关的模块，以后可以对TTS和SSML做更多的探讨。

Bot模块

Bot 模块是所有技能bot的基类，实现了一个典型技能服务的基础功能和逻辑框架。Bot 类的构造函数如下：

```
constructor(postData) {
        this.request = new Request(postData);

        this.session = this.request.getSession();

        this.nlu = this.request.getNlu();
        this.response = new Response(this.request, this.session, this.nlu);

        this._eventHandler = new Map();
        this._intentHandler = new Map();

        this.botMonitor = new BotMonitor(postData);
    }
```

Bot的构造以request为参数，完成对request，NLU和Session的获取，生成Response对象，创建意图和事件的处理映射，并且构建了技能服务的监控器。

Bot 类中的主要方法分类如下：
- 
对话相关：addLaunchHandler(handler) ，addSessionEndedHandler(handler) ，waitAnswer() ，endDialog() 

- 
意图相关：addIntentHandler(intent, handler)，getIntentName()，getSlot(field, index = 0)， setSlot(field, value, index = 0)

- 
事件相关：addDefaultEventListener(handler)

- 
安全相关：initCertificate(headers, body)，setPrivateKey(filename)

- 
会话相关：endSession() ，getSessionAttribute(field = null, defaultValue = null)，setSessionAttribute(field, value, defaultValue = null)和setSessionAttribute(field, value, defaultValue = null)，

- 
设备属性判断：isSupportDisplay()，isSupportAudioPlayer()，isSupportVideoPlayer() 

- 
对Response中expectResponse的封装：addExpectSlotResponse(slot)

- 
应用相关，对request中的方法封装： getApiAccessToken()，getApiEndPoint() ，sendMateappNotification(data)

- 
用户相关：getUserProfile()，getRecordSpeech(audioToken)，getDeviceLocation()


Run（）方法是Bot 执行的主体，流程如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqB1TsX9ibW8GkT7CKAXZHNgzsxKibN6cSuWnnBwH1myVMXOSWaFSZSGs2YXicCLa3wBCB2490zaBV2A/640?wx_fmt=png)

为了简洁起见，流程图中忽略了botMonitor的相关操作。

另外， Bot类中还实现了对音视频播放器指令，以及展示卡片和模版的处理。

Directive 指令实现

DuerOS JS Bot SDK 对DBP协议中的Directive指令实现了较为完整的封装。所有的指令都派生自BaseDirective类，指令的种类包括：
- 
启动app指令

- 
录音指令

- 
支付指令

- 
授权指令

- 
音频播放器指令

- 
视频播放器指令

- 
显示指令及相关模版


指令相关源代码位于https://github.com/dueros/bot-sdk-node.js/tree/master/lib/directive。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqB1TsX9ibW8GkT7CKAXZHNgjXUCA1k0xViciaubjedicqC8tqvOxsUs7Pv0oC25Oj8fwkq3lW12jvI2w/640?wx_fmt=jpeg)

### 展示模版和卡片

针对有屏设备，DuerOS Bot SDK 提供了较为丰富的展现模版和展示卡片。

展现模板分body template和list template两种类型。其中body template由图片和文字组成，list template由一系列list item组成，每个list item由图片和文字组成。不同的展现模板适合不同的场景，开发者可以根据技能展现的需求选择合适的模板。模版指令示意如下：

```
{
    "directives": [
        {
            "type": "Display.RenderTemplate",
            "template": {
                "type": "{{STRING}}",
                "token": "{{STRING}}", 
                "backgroundImage": {{ImageStructure}},
                "title": "{{STRING}}",
                "textContent": {
                    "position": "{{ENUM}}",
                    "text": {{TextStructure}}
                }
            }
        }
    ]
}
```

当技能服务回复用户的时候，可以通过使用卡片形式来展现更生动、丰富的内容。常用的展现卡片类型有文本卡片、标准卡片、标准列表卡片、图片卡片。展现卡片随Response消息一起发送给DuerOS。卡片在response中的json格式如下：

```
"card": {
    "type":"txt",
    "token":"{{STRING}}",
    "content":"{{STRING}}",
    "url":"{{STRING}}",
    "anchorText": "{{STRING}}",
    "cueWords":[
        "{{STRING}}",
        "{{STRING}}",
        "{{STRING}}",
        ...
    ]
}
```

针对展现模版和展示卡片，github上给出了比较详细的示例，在DuerOS官网上则给出了具体的布局示意例子。

### 示例代码

作为一个Javascript开发者，如果开发基于DuerOS的技能服务的话， 从示例代码开始往往是个不错的选择。在github上（https://github.com/dueros/bot-sdk-node.js/tree/master/samples），给出了10多个示例代码，有音乐播放的技能服务，个税计算器，历史上的今天等等。

其中的多个示例代码就是DBP官网上的技能模版，猜一猜是哪几个呢？

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqB1TsX9ibW8GkT7CKAXZHNgByjickHhnZZLCek1lINxS6KZeGFwHorC7dglrgIrecTrsy8aMDCbXBA/640?wx_fmt=jpeg)

参考资料：
- 
https://www.robinwieruch.de/machine-learning-javascript-web-developers/

- 
https://github.com/dueros/bot-sdk-node.js

- 
https://dueros.baidu.com/dbp](https://so.csdn.net/so/search/s.do?q=DuerOS&t=blog)](https://so.csdn.net/so/search/s.do?q=AI&t=blog)](https://so.csdn.net/so/search/s.do?q=人工智能&t=blog)




