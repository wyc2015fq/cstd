# 谷歌胜诉，Android 是正当使用 Java API - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [黄小非](http://www.jobbole.com/members/huangxiaofei) 翻译。未经许可，禁止转载！
英文出处：[Arstechnica](http://arstechnica.com/tech-policy/2016/05/google-wins-trial-against-oracle-as-jury-finds-android-is-fair-use/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
![](http://ww3.sinaimg.cn/large/7cc829d3gw1f49svkjgnrj20dc08naan.jpg)
旧金山消息——经过了两个星期的庭审，联邦法院最终于当地时间周四裁定Google的Android操作系统**并没有**违反Oracle的版权，因为Google重新编写的 37 个（有争议的）API受“正当使用（fair use）”法律条款的保护*（译注：“fair use”是一项法律条款，指允许人们在不影响作者利益的情况下有限地引用他人的作品。）*。经过三天的深思熟虑，陪审团终于达成了一致并形成了判决。
「陪审团的女士和先生们，请慎重裁定，你们的决定将被人铭记。」法庭书记员在陪审团投票裁决之前说。陪审团一共由10名男性和女性组成。
法庭裁决实际上就是要解决一个问题，Google对Java API的使用是否受版权法中”正当使用“条款的保护。陪审团成员一致性裁定“受保护”，这可帮了Google的大忙。法庭裁决为这次本月初开始的庭审划上了句号。如果让Oracle赢的话，那么陪审团则会进入“定损阶段”，从而判定Google要赔偿多少钱。因为现在是Google赢了，所以庭审就结束了。
”对于你们在本案中做出的极大努力，我向你们致敬！“，美国地方法庭法官 William Alsup 说*（译注：就是为了本案而自学 Java 的法官）*，他从 2010 年开始就一直参与此案的诉讼。「我代表美国地方法庭感谢大家，现在你们的任务完成了！我会到陪审员室和大家单独握手致谢。」
10 名陪审员中有 4 位向聚在大厅中的记者表示无可奉告，而其他 6 位则直接从法庭后门离开。
「我们感激陪审员们的裁决。」Google的首席律师 Robert Van Nest 说这句话时正准备进电梯，Google 的其他内部律师也和他一起。「就这样。」Oracle 的律师（对这个判决）没有发表任何评论。
Google 在随后发表的声明中说，这是一次属于每个人的胜利。「今天的裁决表明，Android对Java API的使用属于‘正当使用’，这是Android生态系统的胜利，也是Java程序社区的胜利，也是所有利用开源和免费编程语言开发创新消费产品的开发者的胜利。」这个声明是Google的发言人通过Email发布的。
然而，Oracle已经决定要上诉。
「我们坚决认为Google开发的Android系统非法地使用了Java核心技术，来打入移动设备市场。Oracle则是要通过法律诉讼来阻止Google的非法行为。我们相信我们有足够多的理由提起上诉，并计划将上诉级别提高到联邦法院级别，」Oracle的总辩护律师Dorian Daley在声名中说。
Google的胜利对前一段时间“针对编程语言API免费使用的抨击”风潮有一定缓解作用。（虽然已经判决），现在这个案子却（至少）还是一个关于”API可以被版权法限制“的上诉案件。不过，历史上第一次”用版权控制API“的高调尝试现在被”正当使用“条款成功阻止了。
目前不是很清楚Oracle打算在”定损阶段“向Google要多少钱，不过很可能是90亿美元。因为早先Oracle在其专家报告中给出了这个价钱。
在这次为期大约2周的庭审中，陪审员听取了Sun公司，Google公司以及Oracle公司现任和前任CEO的证词，还有这些公司一线程序员和计算机专家的证词。
Oracle是从与Sun公司的收购中得到了Java的所有权，然后在2010年起诉Google对API非法使用。2012年，该案第一次庭审，美国地方法官William Alsup判决API完全不受版权法保护，但是Alsup的判决在上诉时被撤销。在本月的庭审中，Google最后有争议的37个API也被判决为”正当使用“。
在庭审中，Oracle[争辩](http://arstechnica.com/tech-policy/2016/05/oracle-slams-google-to-jury-you-dont-take-peoples-property/)说Google抄袭了部分Java API的包结构，以及相关的声明代码，以此作为”争取赔偿的捷径”。Android则成功反驳，Oracle的Java商用许可证，有意大量针对特定的手机，这是有漏洞的。
「他们拷贝了11,500行代码，」Oracle律师Peter Bicks在[最终辩护陈词](http://arstechnica.com/tech-policy/2016/05/oracle-slams-google-to-jury-you-dont-take-peoples-property/)中说。「这是毋庸置疑的。他们拿到了代码，抄袭了代码，然后把抄了的代码放到了Android里。」
Google则反驳：Java语言一向是“免费和开放”使用的——这当然也包括“重新实现的Java API”。Sun 公司和其CEO Jonathan Schwartz 认可 Android 是一个合法的，即便不是很便利，但具有竞争力的产品。
Oracle的CEO Larry Ellison 一开始对Android持欢迎态度，但是「在（Oracle）自己尝试利用Java来构建智能手机的做法失败以后，他的态度发生了转变。」Google律师Robert Van Nest在作证时[跟陪审团如是说](http://arstechnica.com/tech-policy/2016/05/googles-closing-argument-android-was-built-from-scratch-the-fair-way/)。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
![](http://www.jobbole.com/wp-content/uploads/2016/04/78805a221a988e79ef3f42d7c5bfd4187.jpeg)
