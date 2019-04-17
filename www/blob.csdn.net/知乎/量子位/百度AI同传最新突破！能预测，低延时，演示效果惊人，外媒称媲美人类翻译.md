# 百度AI同传最新突破！能预测，低延时，演示效果惊人，外媒称媲美人类翻译 - 知乎
# 



> 夏乙 栗子 乾明 一璞 发自 三区五地 
量子位 报道 | 公众号 QbitAI
![](https://pic4.zhimg.com/v2-8db0ada18955c8fc964be53a590417bb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='601'></svg>)



同传AI，刚刚在国内掀起过[暴风骤雨](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247504573%26idx%3D1%26sn%3Dd09c109af0b300e3bd45b9877ddc4699%26chksm%3De8d065cfdfa7ecd94c6c34c0818cd90d181f4a1db513cc75b72e287f1a69ac60c02f44a46e88%26scene%3D21%23wechat_redirect)。


但现在，百度于硅谷宣布了最新重大突破——一个名为**STACL**的同传AI，论文结果优异，Demo效果惊人。

MIT科技评论、IEEE Spectrum等一众外媒，还纷纷给出好评，这是2016年百度Deep Speech 2发布以来，又一项让技术外媒们如此激动的新进展。

百度自己披露：与现在大多数AI“实时”翻译系统不同，STACL的特点是**能预测**和**延时可控**，能够在演讲者讲话后几秒钟开始翻译，并在句子结束后几秒钟内完成。

STACL不走“整句说完再翻译”的路线，甚至还会预测发言者未来几秒的内容，于是延时更短，更接近人类同传。

究竟能达到什么程度？IEEE Spectrum采访后给出类比：跟联合国会议里的人类同传相媲美。

实际效果果真如此？那突破显然重大。

## **Demo展示：翻译AI会预测**



![](https://pic2.zhimg.com/v2-d84861e088870cdbbb983996ba64f2b9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='198'></svg>)



肉眼看，好像快要**同时**了。

Demo可以看出，百度STACL的翻译工作**延时非常短**，与原句只差几个字。

虽说这个AI目前只是同步翻译成文字，还没有合成译文的语音，但这不是重点。关键是，不知道你有没有注意到，这种“只差几个字”有多难得。

还没等“美国总统布什在莫斯科与俄罗斯总统普京在莫斯科会晤。”这句汉语说到莫斯科，自动翻译的英语已经出现了“**meet**”，也就是汉语句末的“会晤”。



![](https://pic2.zhimg.com/v2-8d3f01e392ebd14e9cd7dde48c1a5e9d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1280' height='204'></svg>)



这个“meet”，是AI从前半句话里**脑补**出来的，依据的是美国总统最可能与其他人发生怎样的活动。

因此，百度这个同传AI，**不用等一句话说完**，就能开始翻译了。

对于翻译AI来说，预测是一项很罕见的技能。不同语言的语序总有差异，所以，那些没有预测能力的翻译AI，通常需要等到人类讲完一句话，再开始翻译。

再举一个栗子 (下图) ，从“百度在18年前”这半句里，AI预测出了**百度创立**的内容“started a business”。



![](https://pic2.zhimg.com/v2-4ce5e62b3ddbee33241c6a159dcf6389_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1110' height='405'></svg>)



那么，科学地讲，百度AI的预测效果到底怎么样？


衡量翻译质量，要看BLEU分。

在**中译英延后5个字**的情况下，比起传统的整句翻译AI，百度的BLEU分要低了3.4分。

毕竟是预测，发生错误是自然的。而这个差距在百度看来是可以接受的。

比如，百度AI可能从前半句话里预测出**会面顺利**进行，而事实可能是会面**并不顺利**。翻译就容易出现失误。

面对这样的情况，AI目前并没有纠错的能力。

不过，用小小的延时，就可以换取更高的准确度：如从延3个词到延5个词。用户可以根据需求随意调整。
![](https://pic4.zhimg.com/v2-2ed2a8b9c90a7da36c3ab48eaf7573a7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1200' height='438'></svg>)
而当两种语言语序相差不多的时候，例如描述戴安娜王妃车祸事件的新闻，STACL和传统整句翻译的成绩相比，就没有明显差异了。

这种预测能力，是哪来的？

答案是一个名叫wait-k的模型，它**把预测和翻译无缝整合到了一起**，百度把它和两个翻译模型结合起来做了实验，一个是比较早期的RNN翻译模型，另一个是Google在2017年推出即大热的Transformer。

不过，wait-k不仅仅适用于这两个模型，而是可以用在任何序列映射（sequence-to-sequence）模型上。也就是说，只要有个模型能搞定两种语言之间的翻译，加上这个wait-k做一点小改造，就能实现同传了。

一个机器翻译系统，离不开分析输入语言的编码器和输出目标语言的解码器，而wait-k对机器翻译模型所做的小改动，就在解码器上。它让解码器在输出内容的同时，能预测编码器还没有输入的东西。

百度用这样一个公式描述了这种解码策略：



![](https://pic3.zhimg.com/v2-aa30ded950f1c1f07cfb6a449ab450ba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='755' height='112'></svg>)



其中，x代表输入的内容，y代表输出，t代表时间步，而k表示的是解码器比编码器抢跑的词数。

技术细节在论文STACL: Simultaneous Translation with Integrated Anticipation and Controllable Latency中有详细的介绍。



![](https://pic3.zhimg.com/v2-58d9514091eb01af5137883ccac66306_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='393'></svg>)



地址：[https://arxiv.org/abs/1810.08398](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1810.08398)

## **团队介绍**



![](https://pic1.zhimg.com/v2-1d04fc23ae9bd6f244f08fe789680568_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='229'></svg>)



By the way，按署名次序，介绍一下9位论文作者。

Mingbo Ma，百度硅谷AI实验室科学家，拥有三个学校的计算机博士学位，今年8月刚从俄勒冈州立大学博士毕业，另外两个学位在美国东北大学和纽约城市大学，本科毕业于吉林大学，此前曾在苹果和IBM实习。

黄亮，俄勒冈州立大学助理教授，百度美研首席科学家，此前曾就职于IBM和谷歌，并在纽约城市大学、南加州大学担任过助理教授，博士毕业于宾夕法尼亚大学。

Hao Xiong，爱丁堡大学计算机硕士，本科毕业于河海大学，2015年加入百度任软件工程师，负责大容量数据处理系统和资源调度系统。

Kaibo Liu，俄勒冈州立大学助理研究员，今年6月刚到百度实习，本科毕业于北大。

Chuanqiang Zhang，低调的百度技术员工，目前无更多资料。

何中军，百度主任架构师，从事机器翻译研究与开发十余年，多篇论文发表在本领域权威国际会议ACL、EMNLP、COLING、AAAI等，参与项目曾获2015年度国家科学技术进步奖二等奖。

Hairong Liu，百度硅谷AI实验室科学家，曾就职于三星和普渡大学，华中科大电子电气博士。

Xing Li，中科院自然语言处理和机器学习硕士，百度硅谷人工智能实验室的高级经理，曾任英特尔技术leader，在百度担任过技术工程经理，领导百度贴吧研发团队。

王海峰，百度高级副总裁，e-Staff，百度AIG负责人、百度研究院院长，哈工大计算机博士。



![](https://pic4.zhimg.com/v2-7042f240bb2ada56b1ff93644d14d88b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='740' height='494'></svg>)△ 王海峰
## **第一波评价**

百度这次技术突破宣布后，首先在外媒引发报道。

有好评。其中评价最高的是IEEE Spectrum，认为百度开发的这个新系统，揭示了一种通过预测未来而保持稳定的翻译工具，可以与联合国会议期间提供同传服务的口译人员相媲美，让人们离软件巴别鱼又近了一步。

有对比。比如将百度的这次突破与谷歌联系起来。南华早报评论：随着百度新的翻译系统亮相，百度向谷歌发起了挑战。

还有将百度和谷歌放一起当“耦合”的。Engadget观点：

> 虽然这个系统仍旧有局限性，而且无法在必要的时候取代人类翻译。但是它在谷歌缺席中国的情况下，给出了一种新的选择。

SiliconANGLE则援引分析师评论称：“亚洲的语言翻译方面仍然存在一定的局限性，看到本地的玩家加入到这个游戏中来，的确是一件好事。”

但目前最多的态度是观望。

在技术宅密度比较高的Hacker News和Reddit上，百度的这条消息，并没有引发太多的讨论。

截至早上7点40分，Reddit只有一条评论，Hacker News上有两条评论——虽然这个消息发布已有5个多小时。

Reddit上面的一条评论有点“水”，只是说这个Demo很好。



![](https://pic3.zhimg.com/v2-9aeb5a6f4f7ae062430f20ac0364b602_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='483'></svg>)



Hacker News的两条评论中，一条比较有技术含量，提出了如何翻译德语的问题，并给出了一些示例，另一条是说，这个Demo很好。




![](https://pic3.zhimg.com/v2-cd4d1472c769132a417173b55bb30942_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='584'></svg>)



比较有趣的是，两个说这个Demo很好的网友，昵称基本上是一样的。


不过这个技术到底好不好，更多中外关注者马上都能给出判断。

因为百度最快会在下周的年度技术大会上，进行公开展示。

究竟是重大突破，抑或还只是完美Demo，到时一看便知。

当然，有科研实力有技术复现的同学，现在就能去试试了。

欢迎反馈你的测后评价~

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动


