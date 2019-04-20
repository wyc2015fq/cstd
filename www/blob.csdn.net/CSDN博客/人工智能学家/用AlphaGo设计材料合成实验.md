# 用AlphaGo设计材料合成实验 - 人工智能学家 - CSDN博客
2018年04月16日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：119
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVOMTknGvbpzLsXjcg8JlyKrhxibcqUk5jVGuDR3icFyADKQtNRHBh4vxqj3QiatGJgdJIPTaGeD9oag/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
来源：曾林的科学网博客
 AlphaGo下围棋连挫顶尖高手最终孤独求败的故事几乎家喻户晓。这也引发了大家对人工智能的能力的广泛思考。在科学研究领域，比如说合成实验设计，传统的做法不外乎是依靠经验不断的尝试。
 纵然会有一些热力学基本规律作为指导和参考，可是每一个合成化学的从业人员都知道，合成过程中往往就是一个一个的尝试。大家从小都知道爱迪生发现灯泡灯丝的励志故事，爱迪生尝试了3000种失败的材料才发现真正正确的材料。殊不知这也是科研无奈辛苦一面的真实体现，有几个科研人员有闲情逸致去做3000个失败的实验？浪费无数的资源和样品？那么，我们是否可以利用人工智能的方法让计算机自动帮我们设计可行的化学反应呢？
看过钢铁侠的朋友们都知道，钢铁侠设计自己铠甲的时候只需要和一个siri一样的机器人聊天，先是拖出一张元素周期表：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVOMTknGvbpzLsXjcg8JlyKrhxibcqUk5jVGuDR3icFyADKQtNRHBh4vxqj3QiatGJgdJIPTaGeD9oag/640?wx_fmt=png)
然后就看见钢铁侠随便选了两个元素(Ce和Po)，帅气的摆了几个造型，然后就把一团元气丹之类的东西放进了一个钢铁侠铠甲的雏形里，过了一段时间，铠甲就自动合成好了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVOMTknGvbpzLsXjcg8JlyKDZO34MmSaqk1nU2SK8AOcgh8b0JEdiag2ttmrCEfLZSFRnxsz9WtLKA/640?wx_fmt=png)
相信每一个人都会知道这是假的，可是科研的魅力不就是让这些看上去很假的东西变成现实么？而要实现这些过程的最核心的一步当然不是帅气的pose，酷炫的VR技术，而是需要有一个强大的AI，能够根据选取的最基本材料性质的需求而自行设计化学反应。而这一切目前来说都不再那么科幻。
近期science和nature接连发表了文章报道了利用人工智能预测指导复杂的有机化学反应合成的过程。其中science的文章来自与普林斯顿大学月新泽西当地的一个公司Merck Sharp & Dohme Cooperation.
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVOMTknGvbpzLsXjcg8JlyKX0hJPrOkWN6y4WDujqxibENhjPs2a0GC4H55IjIP4qPcz8uurfyX0Lg/640?wx_fmt=png)
而nature的文章来自于德国，英国的科学家。相对于science的文章，这篇nature所涉及的反应类型更为全面。基于1000多万个已知的化学反应，利用神经网络引擎，这套设计的AI已经可以根据给出的产物来设计对应有机合成反应。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVOMTknGvbpzLsXjcg8JlyKj8qYXs3oh7TnCZ6Kmdx1FtmUmNaJmWNfSPcmVWm1q6hXmBiaJiaKD7JA/640?wx_fmt=png)
其实有机化学根据生成物逆向退出反应物在有机化学合成中的相关理论，早在20世纪就已经被哈佛大学的E. J Corey教授提出，他因此还得了1990年的诺贝尔化学奖。而近几年机器学习和神经网络等技术的兴起，从新激活了大家对这个方向的兴趣。
无独有偶，在无机化学领域，虽然材料的分子式没有有机化学那么复杂。可是材料的成分和组元设计同样是复杂和繁琐的。很多做合成的人其实都会感觉自己合成材料就好比做中餐，知道要加某种元素“少许”，却没有人能够说清楚少许到底是多少。为了改变这种炒菜式的材料设计模式，MIT和UCB等大学的团队同样也在设计相关的数据库和机器学习引擎。一些初步的结果也已经发表在能源材料的权威期刊Chemistry of Materials上：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVOMTknGvbpzLsXjcg8JlyK0sQY9Ve8ria1X1bPl4YdicicwXZ7lF6OpCLt4YPkb45aZY8XAicrxmhRjQ/640?wx_fmt=png)
介于目前能源材料领域的火爆形式，有大量的合成相关文章涌现在各种知名刊物上，这也给机器学习提供了巨大的样本基础。相信通过这些课题组的努力，美国好莱坞大片里面材料设计的场景不再只是人们的幻想。
链接地址：http://blog.sciencenet.cn/blog-3380993-1109211.html 
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
