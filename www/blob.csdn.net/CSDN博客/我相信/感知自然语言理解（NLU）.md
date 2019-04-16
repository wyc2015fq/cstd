# 感知自然语言理解（NLU） - 我相信...... - CSDN博客





2019年03月03日 21:36:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：267









AI即人工智能是一个令人着迷的领域，尤其是基于对话式AI系统的智能音箱的兴起，使人工智能直接走进了我们的家庭。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqZB5Wo1ciaWTgia4Idd0ZesPVS2ymQFSfDvlTSe4bVccrtT3gZEJyia7peS15ynm5IpGmjeRMeMUQhg/640?wx_fmt=jpeg)

自然语言是人类的智慧，自然语言处理（NLP）是AI中最为困难的问题之一，而自然语言理解（NLU）也变成了一个主要的问题，充满了魅力和挑战。一介程序员，没有能力去做相关的研究，但是认知其中的一些基本概念，对于开发具体NLP/NLU相关的应用实现，尤其是对话式智能系统上的应用，还是非常有帮助的。那NLP 和 NLU 有啥区别和联系呢？以下理解并不准确，仅限于认知。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqZB5Wo1ciaWTgia4Idd0ZesPA3cuxibRCiaicYCq6y3ScErd8LHf7lJZpZ5RGMRQVgAAicvkrPKVdgicyrw/640?wx_fmt=png)

## NLU与NLP

先看维基百科对NLP的解释：

`Natural language processing (NLP) is a subfield of computer science, `

`information engineering, and artificial intelligence concerned with `

`the interactions between computers and human (natural) languages, `

`in particular how to program computers to process a`nd analyze large amounts of natural language data.


```

```


`Challenges in natural language processing frequently involve speech `

`recognition, natural language understanding, and natural language generation.`

自然语言处理是一门研究人与人交际中，以及人与计算机交际中的语言问题的学科。NLP的目标是希望通过图灵测试，包括了语音、形态、语法、语义以及语用等方面，也要解决人类语言中的因果、逻辑和推理的问题。

在宗成庆老师的《统计自然语言处理》中，把NLP分为以下十六个方向，其中机器翻译、信息检索和问答系统的应用更广泛。。那么NLP是啥呢？


`Natural-language understanding (NLU) or natural-language interpretation (NLI) `

`is a subtopic of natural-language processing `in artificial intelligence that deals with machine reading comprehension. 

```

```

`NLU is the post-processing of text, after the use of NLP algorithms (identifying parts-of-speech, etc.), `

`that utilizes context from recognition devices (Automatic Speech Recognition [ASR], vision recognition, `

`last conversation, misrecognized words from ASR, personalized profiles, microphone proximity etc.), `

`in all of its forms, to discern meaning of fragmented and run-on sentences to execute an intent from typically voice commands.`

简单的说， NLU是NLP的一个子集，因此在[老码农眼中的简明AI](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658971818&idx=1&sn=aea14e255fc35689b19f5d0fbc281877&chksm=80d32d8cb7a4a49a6b509aab10c49440616436ffbd28ba1d9450e0552f49520c4d98a5b15d21&scene=21#wechat_redirect)中的那张图是错误的，应该是包含的关系。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczrQU14Fx7cSYmsF8XFshzBz04SahU1ZvM6gMPtOYEwWsHicYt1b4v8dYECjuIobc86j1TEmGzz7aPA/640?wx_fmt=png)

NLU的一个主要功能是“execute an intent ”即提取意图。什么是意图呢？

## 意图表达

Intent意图如何理解呢？在我们开发Android 应用的时候，离不开Intent，尽管和NLU中的意图不太一样，但还是对意图的理解有帮助的。

在Android参考文档中，Intent主要是解决Android应用的各项组件之间的通讯，对Intent的定义是执行某操作的一个抽象描述。在NLU中，意图代表用户想要达到的目的，就是在语言表达中所体现出的“用户想干什么”，解决的是人与人，人与机器之间的通讯问题。意图是由什么组成的呢？在Android Intent中由Action，data 和一些属性组成，在NLU中，意图可以由槽位来表达，槽位就是意图的参数信息。槽位是指从句子中抽取出的特定概念，槽位填充为了让用户意图转化为用户明确的指令而补全信息的过程。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqZB5Wo1ciaWTgia4Idd0ZesPlgeG4QMzCtoKiclVdvRSjO5egYC25YxjraMwZEXOooTibKvQHqjcpX8A/640?wx_fmt=jpeg)

举个例子，比如我对一个对话式智能设备说“今天天气怎么样”，意图是什么呢？意图就是“查询天气”。那这个意图是由哪些槽位表达呢？有一个槽位是确定确定，就是时间槽位——“今天”。

那么，机器有了这个槽位和意图是否就可以应答了呢？现实中，各地的天气是不一样的，那么描述这个意图要增加一个槽位——地点。 如果对话式智能设备问我“那个城市的天气？”我回答“北京”，这大概就是槽位填充的过程。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqZB5Wo1ciaWTgia4Idd0ZesPjr7KhSlMEgow0PKBhrHF5IBHAJuvcEZqTdWYibAPM11K5xnflic1Lt9w/640?wx_fmt=jpeg)

俗话说，“一句话有十种说法”，同一个意图有着不同的表达方式。问天气的话，我还可以说“给我查查今天的天气”，“我想知道今天天气”，“天气怎么样”等等，如果统计一下，这些问天气的常常用句子，就是所谓的常用表达。

学以致用，我们了解了意图，槽位和常用表达，能做点什么呢？从零开始搭建一个对话系统，demo还可以，真的成为人们所使用的服务的话，恐怕显得有点高不可攀。感谢这个时代，对话式AI操作系统来了（[感知人工智能操作系统](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974036&idx=1&sn=95c6ecf9a6462f06f2ce9dcba453e64b&chksm=80d32472b7a4ad649ca242ff41116d46c95e9a49545d4dbc68470680f9e5684781ecda260ea9&scene=21#wechat_redirect)），例如DuerOS，咱们在可以在上面研发并搭建自己的服务，这就是所谓的“技能”。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczruSJKUn7kP1dmq7s3fKoSMnQGI9GbsHiab49y40Xce7iaSUqAa3Y1F1jaTlb8Im1cdmusLkv0ticksA/640?wx_fmt=jpeg)

## 技能

什么是技能？ 技能就是智能设备的一个能力，或者说一种服务。既然有AI操作系统，为啥还要开发技能呢？ 这和有了windows 还要开发 office 类似， 不同的软件满足不同的需要。技能作为对话式AI操作系统的具体应用，同样是为了不同用户多种多样的个性化需求。

有了对话式AI系统，在技术，技能的开发和一个web应用的开发没有太大的区别，只不过遵循对话式AI系统提供的接口协议即可。以DuerOS为例，示意如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqZB5Wo1ciaWTgia4Idd0ZesPibUoDcogspID23DbudE0sRURqpsmoNDmIL0hDLbYK9JkJgoAO29JM1A/640?wx_fmt=png)

其中涉及的语音识别，自然语言生成及TTS播报，意图识别等等全部由对话式AI系统完成，我们只专注于具体的服务即可。

既然DuerOS已经提供了大量的服务和海量的资源，我们在上面开发技能还有什么必要么？ 在MacOS上已经有了Messager/Facetime之类的应用，并不影响微信应用的研发和普及，这个道理是类似的。我们更了解用户的使用场景和需求，也更清楚具体是业务逻辑，可以让我们聚焦在擅长的领域。

## 在DuerOS上开发技能

在DuerOS开发语音技能是一个相对轻松的事，在技能开放平台（DBP，DuerOS Bot Platfrom，https://dueros.baidu.com/dbp）上可以方便地完成应用的开发，测试和发布。DBP具备Java/Javascript/PHP/Python/Go的SDK，形成了一个开发框架。同时，通过DBP平台上的可视化界面，可以为用户提供个性化的服务。

DuerOS还提供了大量的系统意图，极大了节约了开发成本,下图是部分系统意图：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqZB5Wo1ciaWTgia4Idd0ZesPVPa9JfSuqyX1ibZVSpeTdbw08aOliaSB5jROJCjickBm5fNyzw0Iz009g/640?wx_fmt=png)

词典在意图识别中具有很重要的地位，一般是某一领域词汇的集合，DuerOS根据词典去解析相应的槽位信息，发送给技能应用处理。 我们可以在DBP自定义字典，更可以直接使用系统词典。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqZB5Wo1ciaWTgia4Idd0ZesPj7ibicCIibJPSOPXF7VIVQyAOw279nf2SYbYGPHTTjia04UsUA1RdPeIzA/640?wx_fmt=png)

如果没有固定的词典与槽位关联的话，还可以将常用表达中需要识别的内容标注为通配槽位，那么DuerOS会将这些请求的内容发送给技能应用。

即便是对于某个技能而言，常用表达也仅仅是“常用表达”，难以穷举用户的各种请求，那么遇到用户的特殊问题，咱们开发的技能是否就不能正常应答了呢？不会的，这正是DuerOS的强大所在，这涉及到很多NLU/NLP的技术，其中一种就是泛化。对于特定领域的问答，DBP 平台还提供了数据训练的能力，无需关注机器学习的算法，只需要提供一些语句的资料即可。

感知自然语言理解，可以从开发小技能开始，5分钟即可创建一个小技能，信不信由你！

#### 参考资料
- 
宗成庆，《统计自然语言处理（第2版）》，清华大学出版社，2013

- 
http://www.cs.bham.ac.uk/~pjh/sem1a5/pt1/pt1_history.html

- 
https://dueros.baidu.com/dbp








