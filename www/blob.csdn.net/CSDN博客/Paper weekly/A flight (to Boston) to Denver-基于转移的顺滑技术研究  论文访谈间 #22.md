# A flight (to Boston) to Denver - 基于转移的顺滑技术研究 | 论文访谈间 #22 - Paper weekly - CSDN博客





2017年10月12日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：146









![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/ePTzepwoNWMkNKMB8c7hfib3laa9aKL9BnTibmERsTIqElyVZM4oMSy3qB10NFn9WXWQdHXXhCiaXy4DMRoRSicyIA/640?wxfrom=5&wx_lazy=1)「论文访谈间」是由
 PaperWeekly 和中国中文信息学会青工委联合发起的论文报道栏目，旨在让国内优质论文得到更多关注和认可。 **这是第22 期「论文访谈间」**

**论文作者 | 王少磊，车万翔，刘挺，张岳，张梅山**

**（哈尔滨工业大学，新加坡科技与设计大学，黑龙江大学）**

**特约记者 | Chloe（香港中文大学）**







听几条平时微信聊天发的语音，听听新闻上被采访者的回答，或者看 YouTube 视频的时候注意看自动生成的字幕，你发现了什么？对的，到处有口误！比人们以为的要多得多。口误一般是这种情形：说了几个词发现说错了，接着说几个语气词趁机想一下应该怎么说，然后用正确的表达纠正回来；或者反应迅速，说错后瞬间就立刻纠正。 




放在口语环境中，这不是什么事儿，听众几乎注意不到，而且能理解想表达的正确含义是什么。但是一旦脱离口语环境，把语音转成文本，就会增加人们的阅读难度。




试试看把朋友发给你的一条 30 秒的微信语音转成文字，要花更长时间才能理解吧。如果还要对文本进行句法分析、机器翻译、内容摘要等后续工作，这种不流畅现象就是一个大问题、会造成严重干扰。因此使自动语音识别得到的文本变得流畅易读，对语音转写后处理格外重要。具体地说，**就是要删除其中的停顿词、语气词、重复词，我们称之为“顺滑”技术**。 




来自哈尔滨工业大学社会计算与信息检索研究中心的王少磊、车万翔、刘挺，新加坡科技与设计大学的张岳、黑龙江大学的张梅山在**“Transition-Based Disfluency Detection using LSTMs”**一文中研究了对文本的顺滑处理技术，该论文获自然语言处理领域顶级国际会议
 EMNLP 2017 录用。




我们来简单看一看这篇 paper： 




先看一个经典的顺滑标注例子，请看图1。




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkoNSZ471wneGB4eqHS2XsQSLibuL5Pun0TickUGeOvGx5QRnPDTsnNcw6KICn1oaLibAzE8icT5FMQ7g/0.png)

△ 图1：英文Switchboard语料库里面标注有不流畅信息的例子




**RM（reparandum）：**被丢弃或者被后面的词所纠正的词组




**(+)：**表示 reparandum 结束的中断点




**IM（interregnum）：**紧跟在中断点后，可能出现的停顿、语气词等




**RP（repair）：**纠正 reparandum 的词组




研究大量顺滑标注例子后，作者发现： 




1. **RM 语块时长时短**（英文语料中最长的 RM 语块有 15 个单词！），RM 语块出现位置不固定，可能存在嵌套结构。因此，对长距离依赖现象进行建模非常重要。 




2. **RM 语块和其后的 RP 语块往往有相似性**。比如图 1 中的 RM 语块“to Boston”和其后的 RP 语块“to Denver”，发现它们第一个词相同都是 to，而且词性组合也相同。因此，探索短语块级别的表示非常有用。 




注意到这两点，作者选择基于转移的方法。它可以建模长距离的依赖关系，而且能够很好的利用块级别的信息。 




之前已经有一些学者尝试利用基于转移的方法去解决顺滑问题。他们将句法分析和顺滑任务联合起来。对传统的基于转移的依存句法分析模型，修改模型中的转移动作，从而将顺滑任务融入到句法分析中去。这种方法有自身的缺点：其一，这种联合模型要求训练数据同时标注句法和顺滑信息，降低了算法的实用性，而且实际性能严重依赖于人工的特征工程；其二，句法分析的噪声可能会对顺滑任务的性能造成严重影响。 




所以作者选择探索**不带句法信息的转移系统**。作者引入了一种基于转移的顺滑模型，不仅不依赖任何句法信息，而且还能充分利用块级别的信息以及上下文的全局表示。通过采用和依存句法分析类似的解码算法，递增地构建和标记输入句子中的不流畅块。 




请看图2。




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkoNSZ471wneGB4eqHS2XsQu56JfZvRuXicrJNhpcWJYicb3TQLUPGML1IibKsibT7DmlSJc1DVDETfgw/0.png)

△ 图2：处理输入‘want a flight to boston to denver’时的模型状态




模型状态由四个部分组成（按从上到下从左到右的顺序）： 




**O（output）：**一个用于表示已经被标记为流畅的词的传统的 LSTM 




**A（action）：**一个用于表示转移系统采取动作的完整历史记录的
 LSTM




**S（stack）：**一个用于表示部分已经被标记为不流畅的词的
 stack LSTM，用来捕获块级别信息




**B（buffer）：**一个用于表示尚未被处理的句子的
 Bi-LSTM




输入一个不流畅的句子。在初始时刻，O、A、S 都为空；B 是整句话，句子的首单词在最前面。然后转移系统采取动作，逐个处理单词。对第一个单词，如果它流畅，把它从 B 放入 O，同时清空 S，这一动作记为“OUT”；如果它不流畅，把它从 B 放入 S，这一动作记为“DEL”。就这样产生了一个动作序列，最后输出了一个流畅的句子。




请看图3，输入“a flight to boston to denver”，详细列出了每步模型状态。




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkoNSZ471wneGB4eqHS2XsQRMBh76ibAqicjK5FsmZ9g6SAuR2h9zLORiaxqbnWgoEr4rorBNRIzlkBw/0.png)

△ 图3：处理“a flight to boston to denver”每步模型状态




为了减少误差传播，作者特意采用了两种策略：1. 柱搜索；2. Scheduled Sampling。 




在常用的英文 Switchboard 测试集的实验表明，本文的模型与目前所有方法相比，取得了最好的结果。




**作者有话说**




**1. 研究得不够透彻之处 **



目前的研究方案本质上还是基于数据驱动的，其性能严重依赖于数据本身，并不是在理解句子基础上对其进行处理，这就导致其在实际应用中会出现很多奇葩的错误，比如当遇到“哔哩 哔哩”这个词组时候，就会倾向于把前面的“哔哩”给删除掉，因为在训练数据中，这种前后重复的词组，大部分情况下都会把前面的词给删除掉。 





**2. Future Work **



自动语音识别（ASR）得到的文本中的不流畅现象主要分为两部分，一部分是 ASR 系统本身识别错误造成的，另一部分是 speaker 话中自带的。顺滑任务主要关注的是 speaker 话中自带的不流畅现象，并没有能力处理识别错误造成的不流畅现象。




要想构建一个高效实用的转写系统，还需要针对语音识别造成的错误进行处理，也就是在保留说话人真实意图的基础上，对句子进行适当的转写。




**关于中国中文信息学会青工委**



中国中文信息学会青年工作委员会是中国中文信息学会的下属学术组织，专门面向全国中文信息处理领域的青年学者和学生开展工作。



![0?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmNz2wClQVGRH7F67doXPxhWhymfb2PoDp7bI8BuqwLicb7HaZLWO2ibxMdBvzJ0oQrfEwia1xf9jqOQ/0?)






** 社区活动**




**  不得不读的GAN  **




GAN 是现在最热门的研究方向之一，那么到底有哪些 paper 是值得一读的？




为此，我们将在 PaperWeekly 社区发起一次 **GAN专题论文推荐**，欢迎大家一起参与推荐和 GAN 相关的论文。针对活动中出现的优质论文，我们将组织发起**论文共读**，由一名学术大咖为活动参与者进行在线论文解读。如果你推荐的论文被大家认可，获得了全场最高点赞数，我们将为你送出一份 **PaperWeekly 神秘大礼包**。




** 活动时间**




**10 月 9 日-10 月 20 日**




** 活动地点**




http://www.paperweekly.site/activity/3




**参与方式**




** 1 ** 复制活动链接进入活动页面


** 2 ** 点击**我要参加**，页面右上角将会收到活动消息通知（需注册登录）

** 3 ** 点击网站右上方的**推荐论文**进行推荐

**** 4 ****或从**今日arXiv**右侧下载arXiv
 Paper推荐**插件**，即可进行一键推荐

**** 5 ****为了更好的使用体验，建议**通过PC端访问网站**




*尚未注册PaperWeekly社区的用户请先申请注册。网站目前采用审核制注册，请如实填写个人信息，我们将在**12小时内**完成审核。




**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmUEtia3RCJ5eZHIskGRIYc1Uen4885tjUqeiaS963f1BQ5PdHHibppPMuHbfbpVsbicYsU00j6RwOA2w/640?)

▽ 点击 | 阅读原文|
 查看论文




