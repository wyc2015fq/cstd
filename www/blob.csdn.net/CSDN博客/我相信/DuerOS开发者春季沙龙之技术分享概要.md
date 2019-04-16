# DuerOS开发者春季沙龙之技术分享概要 - 我相信...... - CSDN博客





2019年03月24日 22:06:31[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：18标签：[人工智能																[AI																[DuerOS																[协议](https://so.csdn.net/so/search/s.do?q=协议&t=blog)
个人分类：[AI](https://blog.csdn.net/wireless_com/article/category/7470805)






阳春三月，不用去看“江南草长”，四环路边的花都开了。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROMp9AbicXpprUvjWqLZiaR3MsKWGZXKxKajAOicwoY7rlLrEPwSQ1iasfiaA/640?wx_fmt=jpeg)

若干年前的北大西南门早已消失在了四环路边，曾经的海淀图书城变成了现在的创业大街。周六的下午，在百度的创新体验中心，举办了DuerOS 核心开发者的交流活动。


![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROp3fxgtImKHJWa5Sia5fLYksU9gUEvFDGb6wg5APTO9v646BBSWgEFZA/640?wx_fmt=jpeg)

本次活动涵盖了DBP平台的运营重点和激励方式，产品交互的设计要点，技术讨论和投融资服务四个部分，我主要完成技术部分的分享，现概要如下：

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROJO49CJZzf1R0dw4cJficueia58ogTkVzSF0Tw7ELQlSgHTuT7j2lSUNA/640?wx_fmt=jpeg)

原来准备讲一些技能开发中的进阶内容，但考虑到合作伙伴的侧重点不同，为了覆盖更多的开发者，最终选择了对DuerOS的协议解读。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROWn4LPMQI5SGHYPDA9jNucDrGXicSicWibsL7YJ7ibQQhbJwibYlPFltQYGQ/640?wx_fmt=jpeg)

这是已经上市小度系列智能音箱，不同价位，从无屏到有屏，而且还在不断的演进中。




既然都是人工智能音箱了，那为什么还要在智能音箱上开发技能服务呢？对于这一问题，可以用智能手机的类比来反问。既然都是智能手机了， 那为什么要开发APP呢？




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaRORFUx2OVEJONrNUBy9eYc9YSkKExGa2cKDohtuBI6rHmYib22NWibiaRFg/640?wx_fmt=jpeg)

通过DuerOS Bot Platform （DBP）就可以在基于DuerOS的设备上开发技能服务，详情可以参见“[面向接口/协议？看DuerOS的技能开发](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974063&idx=1&sn=8d15d4d764f6237a359556e6714e7943&chksm=80d32449b7a4ad5f43da9ce81a684d01795c2a8e40d27807dc036f661a9d7dadd2432e2cffd5&scene=21#wechat_redirect)”。技能服务开发的基本原理如下：




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROAmWsiabOU8yfNFIsFvicSsBnhMjFV7CtnCPQ596xXNGibnlXibZ00CofPg/640?wx_fmt=jpeg)

技能开发就是一个web服务，如果把智能音箱当作浏览器，DuerOS 将自然语言的交互变成了类似浏览器上的键盘/鼠标输入，而对于后台服务而言，技能服务几乎没有什么变化。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaRO1rO27pHU0QegURh7GRs1X5fJaj2hvGEFBwoGRAAjv2jUXOMajExmrA/640?wx_fmt=jpeg)

“[面向接口/协议？看DuerOS的技能开发](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974063&idx=1&sn=8d15d4d764f6237a359556e6714e7943&chksm=80d32449b7a4ad5f43da9ce81a684d01795c2a8e40d27807dc036f661a9d7dadd2432e2cffd5&scene=21#wechat_redirect)”中主要谈到了DBP协议，而DuerOS中主要有3类应用协议：设备配网协议、从设备到DuerOS的协议DCS和 DBP协议。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROQCpOIXaCafpWpyMfhWee17MnjYnH2VCDhZCeaIKme7RcQ9ZUq0vD8Q/640?wx_fmt=jpeg)

传输协议主要满足数据装载，多路复用，差错和流控。DuerOS中的传输协议是http/https，需要注意的是，DCS中的传输协议大多是HTTP/2，使用了其中的长连接特性，具体可以参见《[温故知新，HTTP/2](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658973776&idx=1&sn=bf0978f14d6c826961a42db30ba38e93&chksm=80d32576b7a4ac60de414647467c6a76164aa0f2ecd275d35b7bf34061ab6b2fe1a266c2e361&scene=21#wechat_redirect)》。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROV0dB3BuYERh0RNPF3dibZcFdVQryrrcZjoicmjj0keeiaZRlKnrnsVItQ/640?wx_fmt=jpeg)

技能服务的典型构建可以基于有限状态机，其中launchRequest是对话入口，IntentRequest和Event构成消息循环，SessionEndRequest是对话的退出点，从构成一次多轮对话的生命周期。

HTTP中的Request 中包含多个对象，主要如下：

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROWWI0FdWCtJ38Ge5pNSdqFBnuYRMy4ECNGFPyvXUEUVeMWA93UXTia1w/640?wx_fmt=jpeg)

其中Context是一个非常重要的字段，不仅包含了AudioPlayer和VideoPlayer的播放状态信息，而且在system字段中包括了用户的信息和服务技能分相关信息，针对Device设备信息可以面向不同的终端设备完成不同业务逻辑的定制。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROvvTJib6iaCU0FfbmvicOEbwyF3tM4xoatficPWDeAnibfTwzpRzkERz9iaXw/640?wx_fmt=jpeg)


Response 是开发者业务逻辑完成后，返回给DuerOS设备的数据。关于Context中的Intent，可以参见《[感知自然语言理解（NLU）](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974048&idx=1&sn=8890276ba789c43d295333f4d926f282&chksm=80d32446b7a4ad502d94271f67c96c55371db52df85963367c0b65ec5dbfa77477ff56f30b81&scene=21#wechat_redirect)》一文。




ExpectResponse 是一个非常需要关注的字段，有效地使用ExpectResponse 可以显著的提高用户的对话体验。




ShouldEndSession也要注意，除非明确肯定要退出的情形，一般都要设置为False。




Session 相当于浏览器中的Cookie，技能服务可以向Session中设置数据，这样的数据就可以往返于技能服务和DuerOS 之间了。




从数据的类型上分类，技能服务和DuerOS 之间传输的数据主要有三种消息类型：




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROamyUsLUp4WYzIYJAzzYeia8vskmPEzyeboQOC8BqNwdQc9R1ibaOjuYw/640?wx_fmt=jpeg)




事件来自基于DuerOS的设备，指令从技能服务发往基于DuerOS的设备，而服务接口主要是DBP平台为技能服务提供的web接口。事件大致可以分为4种：

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaRO0LI5BSJrXgOlnTdX53MJnDd7XOrKWXib1FPH5RYG6zs8WicqNmmZMaHw/640?wx_fmt=jpeg)

在开发技能服务的时候，需要对这些事件进行处理，而且要结合具体的应用场景。




相应地，指令可大致可以分为4类：

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaRODpHyg8WdBKjeYvduNjtrfHBViadPUW0rArgNSZUCOTuNPic59j7y9ibYw/640?wx_fmt=jpeg)

这里列出了主要的事件和指令，DBP协议具有极强的可扩展性，需要注意官网上的更新。




服务接口是一个比较宽泛的分类，技能服务可以有私有的服务接口，第三方服务的web接口，百度云服务的接口以及DBP 提供的服务接口。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROn2oMN8jkOmoyRCMUUreEO3EsNxAGGncPWIQe8icRtSjTwLWPmogH1Ig/640?wx_fmt=jpeg)

这里主要描述了DBP当前提供的服务：

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROznpaPuVOrdLWnzanD5CLBuVnHjaGRZ3vUURiaUp5yXViczKHx5tMzWibg/640?wx_fmt=jpeg)

DBP 在持续推出接口服务，为开发者提供更加有吸引力的功能，例如：
- 
连接家用打印机

- 
从技能发往设备终端/智能终端伴侣的推送消息

- 
通过智能终端完成录音/录像的能力

- 
电话通知服务





这些个性化的接口和服务一般都需要用户的授权：

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROTW1p29OJUDl8icH7cAiarj17ic4L0ASn1uqv5X8XTpPFmibuhMZN3ZtFqg/640?wx_fmt=jpeg)

完成用户的授权之后，不但可以使用这些接口服务，还可以对用户的身份进行ID映射，从而形成用户系统的贯通，进而完成一些面向用户的个性化服务。




另外，DBP 中对IoT协议有特殊的支持，目前的类型有智能家居、车载设备和机器人。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaROgKzQkem84X4ytB8NibXKV4H68OibpJgs2Ozpia4WfA4Mcwgvdhic07ntiaQ/640?wx_fmt=jpeg)

目前的IoT主要还是面向云对接，通过授权后发现设计，建立连接进而控制和查询设备，以及设备的属性上报等。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoHYuW4SKEByVMNxdLegaRO1vRmVzibgicia2nibicZicmkicXCzkhQPDGG5lCmPW1T6xrY0LAYQoIQXzXoQ/640?wx_fmt=jpeg)

由于时间有限，分享的内容还主要集中在概要的层面，希望能用尽量通俗的语言描述面向DuerOS的技能开发。




会后的讨论更有意思，大家聊到了SSML，探讨了对话树的构成以及其他工具化开发对技能开发效率的提升。




### 士不可以不弘毅，任重而道远。 希望涌现出更多优质技能，让基于DuerOS的设备给人们带来更多的乐趣，陪伴我们的生活，陪伴你我。](https://so.csdn.net/so/search/s.do?q=DuerOS&t=blog)](https://so.csdn.net/so/search/s.do?q=AI&t=blog)](https://so.csdn.net/so/search/s.do?q=人工智能&t=blog)




