# 机器学习顶会ICML马上开始，哪些亮点值得关注？ - 知乎
# 



> 夏乙 发自 凹非寺
量子位 出品 | 公众号 QbitAI




这几天，吃瓜群众心系莫斯科，机器学习研究者们的目光，飘向了更靠北的斯德哥尔摩。
![](https://pic4.zhimg.com/v2-4a99e1ea6ae30ede3d847991360a5ccb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='749' height='481'></svg>)△ Twitter @AmandaPapp4AI
机器学习顶会ICML 2018马上就要开始了。部分圈内人士已经聚集在瑞典首都斯德哥尔摩，欣赏着晚上9点多的日落；也有一些研究者，正在赶往斯德哥尔摩的路上。

当然，无论国内国外，更多的人都和你我一样，在远程关注。

盛会信息量巨大，最值得关注的有哪些呢？

量子位根据在Twitter、Reddit、微博等国内外社区的观察，整理了一些不容错过的信息。

## **读Paper才是要紧事**

就算你没有中Paper，ICML这种盛会，总有论文值得一读。

首先，这次会议的**论文集PMLR v80**已经出炉了，收录了从2473篇投稿中选出的全部621篇论文。
![](https://pic1.zhimg.com/v2-51e5c667a0ed5989a5388ec01f74b0bc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
论文集地址：
[Proceedings of Machine Learning Research](https://link.zhihu.com/?target=http%3A//proceedings.mlr.press/v80/)

这里有所有论文的PDF和补充资料，数量众多，全部读完当然不太可能，还是要根据自己的研究方向，各取所需。

不过几篇官方认定的**获奖论文**，倒是值得特别关注一下。

**最佳论文**奖（Best Paper Awards）2篇，都与机器学习的安全问题有关：

Obfuscated Gradients Give a False Sense of Security: Circumventing Defenses to Adversarial Examples
Anish Athalye (MIT), Nicholas Carlini (UC Berkeley), David Wagner(UC Berkeley)
论文：[Obfuscated Gradients Give a False Sense of Security: Circumventing Defenses to Adversarial Examples](https://link.zhihu.com/?target=http%3A//proceedings.mlr.press/v80/athalye18a.html)
代码：[anishathalye/obfuscated-gradients](https://link.zhihu.com/?target=https%3A//github.com/anishathalye/obfuscated-gradients)

这篇论文早在今年2月就引起了轰动。当时，ICLR 2018接收论文列表刚刚公布，一作小哥哥Athalye说，ICLR录用的对抗样本防御论文，他们的模型攻破了7/8。
![](https://pic3.zhimg.com/v2-5b5089bc2da598551b5624b671ed13d6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='472'></svg>)
他所说的现象，就是这篇获奖论文所得“混淆梯度(Obfuscated Gradients)”。

Delayed Impact of Fair Machine Learning
Lydia Liu, Sarah Dean, Esther Rolf, Max Simchowitz, Moritz Hardt
(几位作者都来自UC Berkeley)
[Delayed Impact of Fair Machine Learning](https://link.zhihu.com/?target=http%3A//proceedings.mlr.press/v80/liu18c.html)

这篇论文所研究的是机器学习静态公平性标准的长期影响。

**最佳论文亚军**（Best Paper Runner Up Awards）3篇：

The Mechanics of n-Player Differentiable Games
David Balduzzi, Sebastien Racaniere, James Martens, Jakob Foerster, Karl Tuyls, Thore Graepel
（作者来自DeepMind、哈佛大学）
[The Mechanics of n-Player Differentiable Games](https://link.zhihu.com/?target=http%3A//proceedings.mlr.press/v80/balduzzi18a.html)

Near Optimal Frequent Directions for Sketching Dense and Sparse Matrices
Zengfeng Huang（复旦大学）
[Near Optimal Frequent Directions for Sketching Dense and Sparse Matrices](https://link.zhihu.com/?target=http%3A//proceedings.mlr.press/v80/huang18a.html)

Fairness Without Demographics in Repeated Loss Minimization
Tatsunori Hashimoto, Megha Srivastava, Hongseok Namkoong, Percy Liang（斯坦福）
[Fairness Without Demographics in Repeated Loss Minimization](https://link.zhihu.com/?target=http%3A//proceedings.mlr.press/v80/hashimoto18a.html)

**时间检验**奖（Test Of Time Award）1篇：

A Unified Architecture for Natural Language
Processing: Deep Neural Networks with
Multitask Learning (from 2008)
Ronan Collobert and Jason Weston
[https://ronan.collobert.com/pub/matos/2008_nlp_icml.pdf](https://link.zhihu.com/?target=https%3A//ronan.collobert.com/pub/matos/2008_nlp_icml.pdf)

## **“大怼之日”**

除了顶会的核心——论文之外，今年ICML有一个Workshop格外受关注。
![](https://pic3.zhimg.com/v2-7c8a1cc1b2ff07c3073a7a78de5f002a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='450'></svg>)
这次会议的7月15日可谓“大怼之日”，当天，有一场名叫Machine Learning: The Great Debates的Workshop要举办，简称MLGD2018。

参加Workshop的辩手，有通过提交论文入选的，也有主办方邀请的。斯坦福大学教授Percy Liang、最爱参加辩论的纽约大学教授Gary Marcus等都在其中。

大怼之日有4个要怼的辩题，每个话题都有两名正方、两名反方。这些辩题分别是：

**关于严格**：

严格程度的提高将加速该领域的进展，我们可能实施的增强严谨性的做法并不会对其他价值造成过度折衷。

**关于安全**：

当前机器学习系统的漏洞非常严重，我们不应允许它们在实际环境中进行一般部署。

**关于公平性**：

为了有效地解决公平问题，机器学习社区不能减少对技术问题的公平性。 相反，它必须越来越明确地进行广泛的制度变革，注意技术本身的政治影响。

**关于深度学习**：

目前和可预见的深度学习方法具有固有的局限性，如果没有其他技术做辅助，它提升机器智能的能力有天花板。

这里，是完整的日程：
[Program](https://link.zhihu.com/?target=https%3A//www.machinelearningdebates.com/program/)

ICML怼过之后，这个Workshop还会在IJCAI-ECAI、AAMAS等会议上继续开怼。

## **研究机构各显神通**

ICML即将开始，各机构也就纷纷在官方博客上介绍自家的论文、展位、参会人员等等。

入选论文太多的Google，洋洋洒洒列出了52篇论文。另外，Google的同学们还组织了3场Workshop，包括机器学习中的人类可解释性（WHI）、强化学习里的探索、深度生成模型的理论基础和应用。更多详情以及链接，都在他们的博客中：
[https://ai.googleblog.com/2018/07/google-at-icml-2018.html](https://link.zhihu.com/?target=https%3A//ai.googleblog.com/2018/07/google-at-icml-2018.html)

DeepMind列出了29项研究的介绍，还贴心地给出了各项研究Oral陈述和Poster展示的时间地点：
[https://deepmind.com/blog/deepmind-papers-icml-2018/](https://link.zhihu.com/?target=https%3A//deepmind.com/blog/deepmind-papers-icml-2018/)

Facebook官方博客也介绍了自家17篇论文：
[https://research.fb.com/facebook-research-at-icml-2018/](https://link.zhihu.com/?target=https%3A//research.fb.com/facebook-research-at-icml-2018/)

除此之外，他们也举办了一场Workshop，主题是强化学习中的预测和生成建模。

腾讯AI实验室也在官方公众号上，解读了他们的入选论文：

[ICML 2018 | 腾讯AI Lab详解16篇入选论文](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/xpX7eDG5ivk8ZYRjAEsH5g)

微软说，他们有26篇论文被大会收录，派出了40多人去参会，展位，也基本搭好了。
![](https://pic3.zhimg.com/v2-ffa524a14804ce2d92875f3cea72c5e2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
在录取论文列表公布的时候，量子位[初步统计过这些论文的出产机构](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247498866%26idx%3D2%26sn%3D419f1df7cf27ce9557ee24fbd0722703%26chksm%3De8d04b00dfa7c2164c54db38e0ea6434d6f54e9bce6ffb4d1acc7c0c583811c54d55d6823fb2%26scene%3D21%23wechat_redirect)。贡献了最多一作论文的，是Google（含DeepMind）；我国战果最丰硕的，则是清华大学。

## **最后，附传送门**

ICML的整体议程，在这里：
[ICML 2018](https://link.zhihu.com/?target=https%3A//icml.cc/Conferences/2018/Schedule)

以及，重要的事情再说一遍，论文集的地址要记好：
[Proceedings of Machine Learning Research](https://link.zhihu.com/?target=http%3A//proceedings.mlr.press/v80/)

**- 完 -**

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


