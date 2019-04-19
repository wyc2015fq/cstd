# Etsy 在试验了一些 SEO 小技巧后得出的结论 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [李小萌](http://www.jobbole.com/members/minxuaner) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Bill Ulammandakh, Daniel Marks and Sungwan Jo](https://codeascraft.com/2017/01/25/optimizing-meta-descriptions-h1s-and-title-tags-lessons-from-multivariate-seo-testing-at-etsy/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
1，网页标题越短越好，搜索关键词匹配的百分比越高越好；2，Meta description尽量长；3，网页里要使用 H1.
最近我们成功地做了个标题标签的对比测试，试验（标题标签能否）提升搜索引擎优化（SEO），结果和方法分享在 [Code as Craft  中的一篇文章](https://codeascraft.com/2016/10/25/seo-title-tag-optimization/) 。在这篇文章中，我们想分享 SEO 测试中学到的更多知识。我们决定在之前一系列成功的 SEO 深入试验（基础上）加倍下注，其中包括这些变化：
- Title 标签
- Meta 描述
- H1
![](http://wx3.sinaimg.cn/large/63918611gy1fe2wq7hw6oj20sg0apq9q.jpg)
我们得出了 3 个惊人结论：
- 缩短标题标签，能改善访问量（和其他重要指标）；
- Meta 描述对自然搜索流量有显著影响；
- H1 对自然搜索流量也有明显影响。
## **方法笔记**
SEO 对比测试的设置和方法理论的详细信息，请参阅上述 Code as Craft 的引用文章。
在这个测试中,我们用了 6 个变量和 2 个对照组。对照组实验前后保持一致。
![](http://wx3.sinaimg.cn/large/63918611gy1fe2wq81r9xj20sg0op0yv.jpg)
为了得到影响访问量变化的准确可能原因，我们最常用 [Google 的 CausalImpact 安装包](https://google.github.io/CausalImpact/CausalImpact.html) 来实施 Causal Impact 建模，实现测试组数据和 控制组数据的标准化。在某些情况下，需要使用 [双重差分法](https://en.wikipedia.org/wiki/Difference_in_differences) ，因为在处理强烈季节性波动时，此方法预估作用大小更可信。
这个测试也受强季节性、假期的其他事件、体育赛事及美国选举的影响。在最后的试验分析中，根据这些影响来调整统计建模，确保准确衡量测试版本的因果效应。
## 学习经验
### **收获 1 ：短标题标签取胜**
这个试验的结果和我们之前的 SEO 标题标签试验的结论是一致的，较短的标题标签，带来的访问量更多。目前我们已经在多重包含很多不同变量的独立试验中，验证了较短标题标签更利于优化的假设，现在确信**对于 Etsy** ，较短的标题标签更利于优化（由自然流量衡量）。我们假设会通过一些不同的因果机制产生这种影响：
1、低 [Levenshtein 距离](https://en.wikipedia.org/wiki/Levenshtein_distance) 和/或高比例匹配目标搜索查询，更受 Google 搜索算法青睐，因而 Etsy 在 Google 搜索结果中排名有所提升。根据 Wikipedia ：“两个单词的 Levenshtein 距离是一个单词变成另外一个单词要求的最少单个字符编辑数量（即插入、删除或替换）。”
2、只包含目标搜索关键词的较短标题标签，对搜索用户而言，显得更相关/诱人。
### **收获2：Meta描述问题**
我们发现在网页上改变 meta description 能导致访问量明显变化。看起来（文字）较长的描述性 meta description 有利于优化，相反，（文字）较短的简洁 meta description 表现较差。我们假定较长的 meta description 通过 2 种可能的因果机制表现更出色：
1、较长的 meta description 在搜索结果页面占更多位置，提高了点击率。
2、较长的 meta description 看起来更权威或内容更丰富，提高了点击率。
### **收获3：H1事宜**
我们发现一个 H1 的变化能给自然搜索流量带来明显影响。但是，网页上 H1 部分的变化似乎和标题标签的变化以难以预料的方式相互影响。例如，在这个试验中，一个标题标签的某个变量变化，提升了访问量。但即使一个 H1 自己以不同变量改变导致访问量轻微增加，当标题标签的变化和一个 H1 标签的变化结合起来时，标题标签的正面效应（也）减弱了。这强调了 在SEO 测试之前， Etsy 页面即使看似微小改动的重要性。
## **对突发事件的解释说明**
#### *唐纳德·特朗普效应*
我们不得不（在一些其他事件中）控制的例子事件，那就是“特朗普效应”。2016年11月9日和10日，我们在一个测试组中观察到一次数据的剧烈偏离。经过调查，发现偏离是由于美国总统选举的第二天，日常访问相关“唐纳德·特朗普”的页面大量增加（+2000% 到 +5180%）所致。尽管这些页面的流量剧增很短暂，只持续了几天，不过他们确实有过度倾斜的可能或减弱试验结果的统计意义。所以，在进行因果影响分析的试验时，这些页面被删除和控制住了。
![](http://wx4.sinaimg.cn/large/63918611gy1fe2wq8e5maj20sg0lpgrp.jpg)
## **结论**
我们的 SEO 试验说明了网页改动前运行 SEO 测试的重要性，会继续给大家提供意外发现的结果。然而，需要注意的是，我们试验的结果只是对 Etsy，不一定是适用于普遍网站的最佳实践。因此，我们鼓励大家通过严格的 SEO 测试，发现对网站（优化）效果最好的策略。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2017/01/cc60538172a8003631f75028bb0348bb.png)![](http://jbcdn2.b0.upaiyun.com/2017/01/db52a6057133cc28d0d5b7e61528971d.jpg)
