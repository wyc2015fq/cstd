
# NLP Chinese Corpus：大规模中文自然语言处理语料 - Paper weekly - CSDN博客


2019年02月18日 13:01:46[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：432


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

中文的信息无处不在，但如果想要获得大量的中文语料，却是不太容易，有时甚至非常困难。在 2019 年初这个时点上，普通的从业者、研究人员或学生，并没有一个比较好的渠道获得极大量的中文语料。

笔者想要训练一个中文的词向量，在百度和 Github 上搜索了好久，收获却很少：要么语料的量级太小，要么数据过于成旧，或需要的处理太复杂。不知道你是否也遇到了这样的问题？

我们这个项目，就是为了解决这一问题贡献微薄之力。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglXum3JqQPeJ0J9kElWQNib5udb3UKb5wBW40hs3FOZVnJsTFe8bNEo7yVPWXkSn1ib0Db0VovHeIsQ/640?wx_fmt=png)

**项目地址：**

https://github.com/brightmart/nlp_chinese_corpus

# 维基百科json版 (wiki2019zh)

**104 万个词条（1,043,224 条；原始文件大小 1.6G，压缩文件 519M；数据更新时间：2019.2.7）**

**下载链接：**

https://storage.googleapis.com/nlp_chinese_corpus/wiki_zh_2019.zip

**可能的用途：**可以做为通用中文语料，做预训练的语料或构建词向量，也可以用于构建知识问答。

**结构**

```python
{
```
```python
"id"
```
```python
:<id>,
```
```python
"url"
```
```python
:<url>,
```
```python
"title"
```
```python
:<title>,
```
```python
"text"
```
```python
:<text>} 其中，title是词条的标题，text是正文；通过
```
```python
"\n\n"
```
```python
换行。
```

**例子**

```python
{
```
```python
"id"
```
```python
:
```
```python
"53"
```
```python
,
```
```python
"url"
```
```python
:
```
```python
"https://zh.wikipedia.org/wiki?curid=53"
```
```python
,
```
```python
"title"
```
```python
:
```
```python
"经济学"
```
```python
,
```
```python
"text"
```
```python
:
```
```python
"经济学\n\n经济学是一门对产品和服务的生产、分配以及消费进行研究的社会科学。西方语言中的“经济学”一词源于古希腊的。\n\n经济学注重的是研究经济行为者在一个经济体系下的行为，以及他们彼此之间的互动。在现代，经济学的教材通常将这门领域的研究分为总体经济学和个体经济学。微观经济学检视一个社会里基本层次的行为，包括个体的行为者（例如个人、公司、买家或卖家）以及与市场的互动。而宏观经济学则分析整个经济体和其议题，包括失业、通货膨胀、经济成长、财政和货币政策等。..."
```
```python
}
```

**效果**

```python
经济学
```

```python
经济学是一门对产品和服务的生产、分配以及消费进行研究的社会科学。西方语言中的“经济学”一词源于古希腊的。
```

```python
经济学注重的是研究经济行为者在一个经济体系下的行为，以及他们彼此之间的互动。在现代，经济学的教材通常将这门领域的研究分为总体经济学和个体经济学。微观经济学检视一个社会里基本层次的行为，包括个体的行为者（例如个人、公司、买家或卖家）以及与市场的互动。而宏观经济学则分析整个经济体和其议题，包括失业、通货膨胀、经济成长、财政和货币政策等。
```

```python
其他的对照还包括了实证经济学（研究「是什么」）以及规范经济学（研究「应该是什么」）、经济理论与实用经济学、行为经济学与理性选择经济学、主流经济学（研究理性-个体-均衡等）与非主流经济学（研究体制-历史-社会结构等）。
```

```python
经济学的分析也被用在其他各种领域上，主要领域包括了商业、金融、和政府等，但同时也包括了如健康、犯罪、教育、法律、政治、社会架构、宗教、战争、和科学等等。到了
```
```python
21
```
```python
世纪初，经济学在社会科学领域各方面不断扩张影响力，使得有些学者讽刺地称其为「经济学帝国主义」。
```

```python
在现代对于经济学的定义有数种说法，其中有许多说法因为发展自不同的领域或理论而有截然不同的定义，苏格兰哲学家和经济学家亚当·斯密在
```
```python
1776
```
```python
年将政治经济学定义为「国民财富的性质和原因的研究」，他说：
```

```python
让-巴蒂斯特·赛伊在
```
```python
1803
```
```python
年将经济学从公共政策里独立出来，并定义其为对于财富之生产、分配、和消费的学问。另一方面，托马斯·卡莱尔则讽刺的称经济学为「忧郁的科学」（Dismal science），不过这一词最早是由马尔萨斯在
```
```python
1798
```
```python
年提出。约翰·斯图尔特·密尔在
```
```python
1844
```
```python
年提出了一个以社会科学定义经济学的角度：
```

```python
.....
```

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglXum3JqQPeJ0J9kElWQNib5EB2yic43S0x3Ubvr0iaIN5b5x2hZL6ic0brqiaSic4YV4lC1eunljPGR9dg/640?wx_fmt=jpeg)

# 新闻语料json版 (news2016zh)

**250 万篇新闻（原始数据 9G，压缩文件 3.6G；新闻内容跨度：2014-2016 年)**

**下载链接****（密码: film）****：**

https://pan.baidu.com/s/1LJeq1dkA0wmYd9ZGZw72Xg

**数据描述**

包含了 250 万篇新闻。新闻来源涵盖了 6.3 万个媒体，含标题、关键词、描述、正文。

数据集划分：数据去重并分成三个部分。训练集：243 万；验证集：7.7 万；测试集，数万，不提供下载。

**可能的用途：**可以做为【通用中文语料】，训练【词向量】或做为【预训练】的语料；也可以用于训练【标题生成】模型，或训练【关键词生成】模型（选关键词内容不同于标题的数据）；亦可以通过新闻渠道区分出新闻的类型。

**结构**

```python
{
```
```python
'news_id'
```
```python
: <news_id>,
```
```python
'title'
```
```python
:<title>,
```
```python
'content'
```
```python
:<content>,
```
```python
'source'
```
```python
: <source>,
```
```python
'time'
```
```python
:<time>,
```
```python
'keywords'
```
```python
: <keywords>,
```
```python
'desc'
```
```python
: <desc>,
```
```python
'desc'
```
```python
: <desc>}
```

```python
其中，title是新闻标题，content是正文，keywords是关键词，desc是描述，source是新闻的来源，time是发布时间
```

**例子**

```python
{
```
```python
"news_id"
```
```python
:
```
```python
"610130831"
```
```python
,
```
```python
"keywords"
```
```python
:
```
```python
"导游，门票"
```
```python
,
```
```python
"title"
```
```python
:
```
```python
"故宫淡季门票40元 “黑导游”卖外地客140元"
```
```python
,
```
```python
"desc"
```
```python
:
```
```python
"近日有网友微博爆料称，故宫午门广场售票处出现“黑导游”，专门向外地游客出售高价门票。昨日，记者实地探访故宫，发现“黑导游”确实存在。窗口出售"
```
```python
,
```
```python
"source"
```
```python
:
```
```python
"新华网"
```
```python
,
```
```python
"time"
```
```python
:
```
```python
"03-22 12:00"
```
```python
,
```
```python
"content"
```
```python
:
```
```python
"近日有网友微博爆料称，故宫午门广场售票处出现“黑导游”，专门向外地游客出售高价门票。昨日，记者实地探访故宫，发现“黑导游”确实存在。窗口出售40元的门票，被“黑导游”加价出售，最高加到140元。故宫方面表示，请游客务必通过正规渠道购买门票，避免上当受骗遭受损失。目前单笔门票购买流程不过几秒钟，耐心排队购票也不会等待太长时间。....再反弹”的态势，打击黑导游需要游客配合，通过正规渠道购买门票。"
```
```python
}
```

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglXum3JqQPeJ0J9kElWQNib5IiaYwA7ra0ARX71iaY8wFoz09RdBViafYZjDR3iblaibiby4l6EvZHnRSE0g/640?wx_fmt=png)

# 百科类问答json版 (baike2018qa)

**150 万个问答（原始数据 1G 多，压缩文件 663M；数据更新时间：2018 年)**

**下载链接（密码: fu45）****：**

https://pan.baidu.com/s/12TCEwC_Q3He65HtPKN17cA

**数据描述**

含有 150 万个问题和答案，每个问题属于一个类别。总共有 492 个类别，其中频率达到或超过 10 次的类别有 434 个。

数据集划分：数据去重并分成三个部分。训练集：142.5 万；验证集：4.5 万；测试集，数万，不提供下载。

**可能的用途：**可以做为通用中文语料，训练词向量或做为预训练的语料；也可以用于构建百科类问答；其中类别信息比较有用，可以用于做监督训练，从而构建更好句子表示的模型、句子相似性任务等。

**结构**

```python
{
```
```python
"qid"
```
```python
:<qid>,
```
```python
"category"
```
```python
:<category>,
```
```python
"title"
```
```python
:<title>,
```
```python
"desc"
```
```python
:<desc>,
```
```python
"answer"
```
```python
:<answer>}
```

```python
其中，category是问题的类型，title是问题的标题，desc是问题的描述，可以为空或与标题内容一致。
```

**例子**

```python
{
```
```python
"qid"
```
```python
:
```
```python
"qid_2540946131115409959"
```
```python
,
```
```python
"category"
```
```python
:
```
```python
"生活知识"
```
```python
,
```
```python
"title"
```
```python
:
```
```python
"冬天进补好一些呢，还是夏天进步好啊？ "
```
```python
,
```
```python
"desc"
```
```python
:
```
```python
""
```
```python
,
```
```python
"answer"
```
```python
:
```
```python
"你好！\r\r当然是冬天进补好的了，夏天人体的胃处于收缩状态，不适宜大量的进补，所以我们有时候说：“夏天就要吃些清淡的，就是这个道理的。”\r\r不过，秋季进补要注意“四忌” 一忌多多益善。任何补药服用过量都有害。认为“多吃补药，有病治病，无病强身”是不的。过量进补会加重脾胃、肝脏负担。在夏季里，人们由于喝冷饮，常食冻品，多有脾胃功能减弱的现象，这时候如果突然大量进补，会骤然加重脾胃及肝脏的负担，使长期处于疲弱的消化器官难于承受，导致消化器官功能紊乱。 \r\r二忌以药代食。重药物轻食物的做法是不科学的，许多食物也是好的滋补品。如多吃荠菜可治疗高血压；多吃萝卜可健胃消食，顺气宽胸；多吃山药能补脾胃。日常食用的胡桃、芝麻、花生、红枣、扁豆等也是进补的佳品。\r\r三忌越贵越好。每个人的身体状况不同，因此与之相适应的补品也是不同的。价格昂贵的补品如燕窝、人参之类并非对每个人都适合。每种进补品都有一定的对象和适应症，应以实用有效为滋补原则，缺啥补啥。 \r\r四忌只补肉类。秋季适当食用牛羊肉进补效果好。但经过夏季后，由于脾胃尚未完全恢复到正常功能，因此过于油腻的食品不易消化吸收。另外，体内过多的脂类、糖类等物质堆积可能诱发心脑血管病。"
```
```python
}
```

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglXum3JqQPeJ0J9kElWQNib5uxV0yhmrISjcT8oCcwibJPibnOw9ZzVKM7P2O92VO6vwqtym9ticbudZQ/640?wx_fmt=png)

**公开评测**

欢迎报告模型在验证集上的准确率。任务 1： 类别预测。

报告包括：\#1）验证集上准确率；\#2）采用的模型、方法描述、运行方式，1页PDF；\#3）可运行的源代码（可选）

基于\#2和\#3，我们会在测试集上做测试，并报告测试集上的准确率；只提供了\#1和\#2的队伍，验证集上的成绩依然可以被显示出来，但会被标记为未验证。

# 翻译语料 (translation2019zh)

**520 万个问答( 原始数据 1.1G，压缩文件 596M)**

**下载链接****：**

https://storage.googleapis.com/nlp_chinese_corpus/translation2019zh.zip

**数据描述**

中英文平行语料 520 万对。每一个对，包含一个英文和对应的中文。中文或英文，多数情况是一句带标点符号的完整的话。

对于一个平行的中英文对，中文平均有 36 个字，英文平均有 19 个单词（单词如“she”）

数据集划分：数据去重并分成三个部分。训练集：516 万；验证集：3.9 万；测试集，数万，不提供下载。

**可能的用途：**可以用于训练中英文翻译系统，从中文翻译到英文，或从英文翻译到中文；由于有上百万的中文句子，可以只抽取中文的句子，做为通用中文语料，训练词向量或做为预训练的语料。英文任务也可以类似操作。

**结构**

```python
{
```
```python
"english"
```
```python
: <english>,
```
```python
"chinese"
```
```python
: <chinese>}
```

```python
其中，english是英文句子，chinese是中文句子，中英文一一对应。
```

**例子**

```python
{
```
```python
"english"
```
```python
:
```
```python
"In Italy, there is no real public pressure for a new, fairer tax system."
```
```python
,
```
```python
"chinese"
```
```python
:
```
```python
"在意大利，公众不会真的向政府施压，要求实行新的、更公平的税收制度。"
```
```python
}
```

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglXum3JqQPeJ0J9kElWQNib5hNH3AUEDcHRXfCFDSUjXrSgHBXdfIf2mXgeBgzqetKJWS7wqdfIqNw/640?wx_fmt=jpeg)

**公开评测**

欢迎报告模型在验证集上的准确率。任务 1： 类别预测。

报告包括：\#1）验证集上准确率；\#2）采用的模型、方法描述、运行方式，1页PDF；\#3）可运行的源代码（可选）

基于\#2和\#3，我们会在测试集上做测试，并报告测试集上的准确率；只提供了\#1和\#2的队伍，验证集上的成绩依然可以被显示出来，但会被标记为未验证。

# 贡献语料/Contribution

贡献中文语料，请发送邮件至：

nlp_chinese_corpus@163.com

为了共同建立一个大规模开放共享的中文语料库，以促进中文自然语言处理领域的发展，凡提供语料并被采纳到该项目中，除了会列出贡献者名单（可选）外，我们会根据语料的质量和量级，选出前 20 个同学，结合您的意愿，寄出键盘、鼠标、显示屏、无线耳机、智能音箱或其他等值的物品，以表示对贡献者的感谢。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多往期内容：**

[Airbnb实时搜索排序中的Embedding技巧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494488&idx=1&sn=452ac80c593a9b31252031eac38d0e01&chksm=96ea34d8a19dbdce940ed25bb93507aa6c4d118f84dd0bb965b060f232fe5d41894bbc9edcb6&scene=21#wechat_redirect)
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)
[F-Principle：初探理解深度学习不能做什么](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494694&idx=1&sn=7020fb834ce8307f27ce9c072047d37d&chksm=96ea33a6a19dbab0a6585daa00d5b5c65501dd633fa677c80541fad0e170d92baffe379315c3&scene=21#wechat_redirect)

[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)
[AI Challenger 2018 机器翻译参赛总结](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494103&idx=1&sn=fc372862e0369b1f6a943bf997f6fc1b&chksm=96ea3657a19dbf4108bbc4179e779aa04ef05fe84f0013fa6425b0cd7e761e9880917361c4c1&scene=21#wechat_redirect)
[Google BERT应用之红楼梦对话人物提取](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494507&idx=1&sn=3c3cafef0fb51a7e40d9b9bbab53fd5f&chksm=96ea34eba19dbdfd31eaa760bb7cfd5e18f2e967c83c6ea6693ad9a062c55b3009211d824ca3&scene=21#wechat_redirect)
[深度长文：NLP的巨人肩膀（上）](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493520&idx=1&sn=2b04c009ef75291ef3d19e8fe673aa36&chksm=96ea3810a19db10621e7a661974c796e8adeffc31625a769f8db1d87ba803cd58a30d40ad7ce&scene=21#wechat_redirect)
[NLP的巨人肩膀（下）：从CoVe到BERT](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493731&idx=1&sn=51206e4ca3983548436d889590ab5347&chksm=96ea37e3a19dbef5b6db3143eb9df822915126d3d8f61fe73ddb9f8fa329d568ec79a662acb1&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。

📝**来稿标准：**
• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向）
• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接
• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志

**📬 投稿邮箱：**
• 投稿邮箱：hr@paperweekly.site
• 所有文章配图，请单独在附件中发送
• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通



🔍

现在，在**「知乎」**也能找到我们了
进入知乎首页搜索**「PaperWeekly」**
点击**「关注」**订阅我们的专栏吧


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 获取最新论文推荐


