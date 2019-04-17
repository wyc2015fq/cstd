# 牛津教授吐槽DeepMind心智神经网络，还推荐了这些多智能体学习论文 - 知乎
# 



> 夏乙 发自 凹非寺
量子位 出品 | 公众号 QbitAI

DeepMind最近又有一篇论文刷了屏：

在Machine Theory of Mind中，DeepMind提出了“**机器心智理论网络”ToMnet**，让AI智能体能理解自己和周围智能体的情绪、意图、欲望等等。他们在论文摘要中这样陈述这篇论文的价值：**推进可解释性AI发展的重要一步。**

论文地址：[[1802.07740] Machine Theory of Mind](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1802.07740)
![](https://pic2.zhimg.com/v2-5bfd700b7bc962e4868be7cbf579d669_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
和历史上的每一篇刷屏论文一样，flag立得太高，当然会招来反对。

这次提出质疑的，不少我们熟悉的马库斯老师了，而是牛津大学计算机科学专业副教授希蒙（Shimon Whiteson）。

希蒙上周读完这篇论文之后，大概心里堆积了太多的槽点，于是连发10条Twitter想要一吐为快。他说：
![](https://pic1.zhimg.com/v2-e4ce58297a80e7ed4595673acd625738_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='808'></svg>)
> 我其实很喜欢这篇论文的想法，用元学习方法来为对手建模。但是，深度学习圈写论文的那些毛病，这篇论文就是个极度恶劣的样本！

说起“深度学习圈那些毛病”，你们大概能想到一个老生常谈的问题：**Over Claim**。

## **重要一步？**
![](https://pic1.zhimg.com/v2-7f536a2ab1cfc57b0dbcdcde19a4ba4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='793'></svg>)
希蒙主要针对的，就是DeepMind所宣称的“重要一步”。

“重要一步”这样的定位，就算那些提出新想法、新概念、新问题的**开创性论文**上，用起来也是小心翼翼，何况DeepMind这一篇，无论如何也谈不上开创性。

希蒙批判这篇论文说，它没有讲理论，实验领域简单得跟玩儿似的，算法上的贡献也简直微不足道，只不过是网络架构的一些工程性处理。那如果还能找出“重要一步”，就只能是提出了什么重大的新想法吧？

想法新不新，要看“相关研究”那部分。可是这篇论文它……没有Related Work部分……

## **深度学习健忘症**

这也就牵扯到了深度学习圈外人对圈里人的另一大意见：贵圈写论文之前会不会读一下相关文献？研究者们还专门给这个问题起了个名字，叫“深度学习健忘症（Deep Learning Amnesia）”。
![](https://pic3.zhimg.com/v2-625d96814b9a1d1651609f5414136826_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1218'></svg>)
回到DeepMind这篇论文本身。希蒙认为，这篇论文主要讲的是多智能体学习领域一个并不冷门的主题：对手建模（opponent modelling）。但是，全文从头到尾都没有提到这个概念。

在多智能体学习领域，有一种历史悠久的著名方法，叫做虚构对策（fictitious play），DeepMind提都没有提；多智能体环境中的对手建模，有一个完整的推理框架，叫做交互式的部分可观察马尔科夫决策过程（Interactive POMDPs），DeepMind也完全忽略了。

希蒙老师敲黑板：同学们，随便浏览一篇多智能体学习的综述文章，这些文献都提到了啊！

可是DeepMind这一篇，引用的相关文献完全跳过了这个领域，倒是谈了不少心智理论和贝叶斯。
![](https://pic4.zhimg.com/v2-36c5cb81b82ea94a10ae4081747a105f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='503'></svg>)
对于“健忘症”这个问题，论文一作、DeepMind研究员Neil Rabinowitz在Twitter上回复说，这些文献他们是知道的，但是在introduction里不知道怎么的就忘了引用了。现在arXiv上更新的第二版论文中，已经加上了这些该引用的文献。

## **“相关研究”**

不过，大部分论文中都有的“相关研究”那一节，还是没写。

对此，有一位前来吐槽的围观群众说得好：要不咱们给DeepMind众包个“相关研究”吧？

这显然不是个可行的解决之道，不过对于想爱学习的同学们来说，也不是非要看这一篇论文嘛，多智能体学习这个领域有不少文献综述可以读。

希蒙就列出了两篇：
- A Comprehensive Survey of Multiagent Reinforcement Learning
[A Comprehensive Survey of Multiagent Reinforcement Learning](https://link.zhihu.com/?target=http%3A//ieeexplore.ieee.org/document/4445757/)
- A Framework for Sequential Planning in Multi-Agent Settings
[https://www.jair.org/media/1579/live-1579-2391-jair.pdf](https://link.zhihu.com/?target=https%3A//www.jair.org/media/1579/live-1579-2391-jair.pdf)

围观群众Katja Hofmann也贡献了一篇：
- Autonomous Agents Modelling Other Agents: A Comprehensive Survey and Open Problems
[A Comprehensive Survey and Open Problems](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1709.08071)

## **关于希蒙**

对DeepMind开炮的希蒙是牛津大学计算机专业研究人工智能和机器学习方向的副教授，带领着WhiRL（Whiteson Research Lab），研究领域主要涉及强化学习、深度学习等，以及它们在机器人、游戏、信息检索上的应用。

他带领着学生们发表过不少很有名的论文，比如LipNet、比如和Pieter Abbeel合作的Learning with Opponent−Learning Awareness等等。
![](https://pic1.zhimg.com/v2-6595ac1f6909ad0af9b29c50b96b6578_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='464'></svg>)WhiRL实验室合影，右四为希蒙
要详细了解希蒙老师，可以去他们实验室的主页看看：[Whiteson Research Lab](https://link.zhihu.com/?target=http%3A//whirl.cs.ox.ac.uk/)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


