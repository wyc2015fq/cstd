# 马库斯：DeepMind新出的机器心智网络不错，但有误导性 - 知乎
# 



> 原作 Gary Marcus
Root 编译
量子位 报道 | 公众号 QbitAI

“DeepMind最新出的论文Machine Theory of Mind还不错(fascinating)。”

这是马库斯昨天在Medium上对DeepMind新文章给出的评价，“但他们又犯了哲学上的错误。这已经不是第一次了。”

一个月以前，[马库斯还写过篇长文](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493419%26idx%3D2%26sn%3D6a9cab1bb836763aa09bf1c7ff6cef70%26scene%3D21%23wechat_redirect)谈到这个问题。

虽然DeepMind（以下缩写为DM）官方表明在造围棋算法的过程中，并没有用到人类的知识。

但马库斯指出，DM围棋算法里所用到的蒙特卡洛树搜索就是基于以往在围棋程序上的积累。

“而DM他们的工作却倾向于放大人工智能后天自我塑造作用，”马库斯说，“这有很强的误导性。”

这次机器心智理论的文章，说的是可以自动学会给其他智能体的行为建模。

但马库斯认为，这和以前的AlphaGo的工作非常相似，DM悄悄地搭建了非常多的前提先决条件，包括：

1）默认其他智能体的存在是个事实；
2）不同的智能体可以有不同的观念；
3）这些观念有存在错误的可能（观念可以通过系统的特定的输出节点看出来）。

所以说，他们已经对这个心智理论的底层进行硬编码了。

为了更好的度量，他们针对墙、物体、智能体分别搭建了三个极其不同的具有代表性的平面。

上述所有操作都能在文章的正文和附录中找到，但是却没有在摘要和讨论里大大方方地说出来（马库斯os：搞得那么隐蔽是闹哪样）。

尤其值得重视的是，这次预置的先天知识和框架和之前AlphaGo的是不一样的。

AlphaGo依赖的是蒙特卡洛树搜索，而ToMNet，心智理论神经网络，是依赖于能把单个智能体的已有知识和分析区分开的架构模块。

正如马库斯1月17号的那篇文章里的预测（还有1月2号那篇深度学习）一样，DeepMind当下的工作会出现大量不承认底层预置知识重要性的现象。

（算法）要想在游戏中玩到最后，编程的人越要重视（算法）底层的能力。

可惜，不同的任务之间（对算法）有不同的底层要求。

棋盘游戏要用蒙特利树搜索，理解语言要用句法分析树，理解3D场景要用几何图元（图形软件包中用来描述各种图形元素的函数，或简称为图元，primitive），那些抱团才能解决的问题分析要用到机器心智理论神经网络。

总而言之，所有这些预置的知识都说明了，人工智能不可能是一块白板。更准确地来说，是像伊曼努尔·康德(Immanuel Kant)所预见到的那样，具有时空上的多样性。

只有当编程圈意识到知识原始积累的重要性，造出来的系统才能成事儿。不承认预置知识的贡献，还一个劲儿地称“学习系统”，强调算法所有能力都是后天习得的，只会进一步误导大家。

马库斯希望AI界的人能更多地从底层方面，原则上，开始意识到机器预置知识(innate machinery)的重要性。

最后，附编译来源，
[https://medium.com/@GaryMarcus/deepminds-misleading-campaign-against-innateness-a2ea6eb4d0ba](https://link.zhihu.com/?target=https%3A//medium.com/%40GaryMarcus/deepminds-misleading-campaign-against-innateness-a2ea6eb4d0ba)

你可能感兴趣：
[马库斯现场驳哈萨比斯，AlphaZero并非从零学习](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247491877%26idx%3D1%26sn%3D8ef28621bdbd70c3cd4acb3ca0951772%26chksm%3De8d05657dfa7df41f0aaad15acd2992c1b096d25d3de5c87970259901a7853aebf6ae42b154b%26scene%3D21%23wechat_redirect)

[马库斯发arXiv长文，论证AlphaZero为什么不是块白板](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493419%26idx%3D2%26sn%3D6a9cab1bb836763aa09bf1c7ff6cef70%26scene%3D21%23wechat_redirect)

[马库斯认为深度学习存在十大挑战](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247492785%26idx%3D1%26sn%3D4ad95174df0c32550f57cc7c7f34e21a%26chksm%3De8d053c3dfa7dad5ec703a8dbd3f39ea7b105d43c137e83e9f41227083effa42058edd44cb86%26scene%3D21%23wechat_redirect)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




