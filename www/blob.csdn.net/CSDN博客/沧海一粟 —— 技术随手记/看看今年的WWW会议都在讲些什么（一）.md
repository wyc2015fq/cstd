# 看看今年的WWW会议都在讲些什么（一） - 沧海一粟 —— 技术随手记 - CSDN博客





2011年07月31日 20:30:28[慢游](https://me.csdn.net/eengel)阅读数：2462








这十来年互联网飞速发展，导致WWW (InternationalWorld Wide Web Conference)会议的文章很有一些既新颖又实用的想法。闲来无事，看看今年的WWW会议[1]都在讲些什么。



除了keynote之外，今年的WWW一共包含了27个Session[2]，有些似曾相识，有些则让人耳目一新。下面就说说自己比较感兴趣的，也当做了解目前WWW领域的热门所在了。（大多文章并未通读，只是了解个大概罢了。有不正之处只当一笑罢。）



**Session:Intent understanding 意图理解**

题目很大，但这儿收录的三篇文章都是讲的某种特殊应用中的intent understanding：就是用户的搜索行为。联想当前搜索市场的风云变换，惊觉三文都有来自MS的coauthor，既是意料之外，也算情理之中了。



**Session:Recommendation 推荐**

好吧，推荐现在简直太热了，在好多大点的网店，都会有名为“推荐”的功能。以买书为例，从最初的推荐买的人最多的，到后来推荐买这本书的人还买了什么别的书，估计还有推测用户的意图来进行推荐的。

不过这儿的三篇文章倒不是推荐大家买东西的。其中一篇这篇文章对新闻的 关联性进行建模，从而进行推荐。这两天一直在网上看游泳世锦赛的新闻，想象要是有推荐功能的话，应该比现在所有人看到一样的新闻列表要有趣的多。

另外两篇讨论的都是query recommendation，也是与搜索相关。（反观我们自己，生活中已经不可能离开搜索引擎。简直是改变了人类的生活方式啊。）Query recommendation 查询推荐，我猜测是讲的用户在输入某些查询条件时，搜索引擎会推荐一些相近的查询条件或搜索结果。就像现在的google，输入一些字，就会有一个下拉列表出现， 导引我的查询条件更接近我的真实意图。这两篇文章，分别涉及了diverse and relevant query和long-tail query，有兴趣进一步了解的朋友可以读一读。

最初的搜索引擎，用户以“人的视角”输入自己的查询条件，发送到后台;然后搜索引擎会处理这些条件，并返回相应的结果。但搜索引擎对查询条件的处理都是站在“机器的视角”。两种不同的视角造成了“问”与“答”之间显而易见的鸿沟， 也就是我们常说的“答非所问”。

怎么解决这个问题？首先可行的方案是让用户“能者多劳”，多考虑自己的查询条件，用机器比较能接受的语言而不是自然语言，比如用关键字组合而不是一句话作为查询条件。这就使得由于人的查询技能高低，造成了查询效率的很大不同。高手和菜鸟水平迥异，到处可见查询技巧的经验贴。

随着搜索引擎在大众之间的普及，这个要求就显得有点强人所难了。因此研究人员就转而思考怎样减轻用户的压力。我想Query recommendation应该算是努力之一吧：好歹给用户提供点儿线索，不至于盲人摸象。



**Session:Web Mining Web挖掘**

Web mining 包含了三个分支：webcontent mining, web structure mining和web usage mining。个人感觉这儿的三篇文章刚好属于这三个分支：第一篇讲如何用click-log为federated search进行特征曲线建模：web usage mining; 第二篇讲通过网站聚类进行linksuggestion：web structure mining; 第三篇讲在语义网中实现object coreference：web content mining。



**参考文献：**

[1] 20th International WorldWide Web Conference, [http://www2011india.com/](http://www2011india.com/)

[2] 20th International WorldWide Web Conference paper list, [http://portal.acm.org/citation.cfm?id=1963405&CFID=36120206&CFTOKEN=45813092](http://portal.acm.org/citation.cfm?id=1963405&CFID=36120206&CFTOKEN=45813092)



