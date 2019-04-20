# DeepMind开源Psychlab平台——搭建AI和认知心理学的桥梁（附论文和代码下载） - 人工智能学家 - CSDN博客
2018年01月27日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：88
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUTgqaiaRxQyKjTwYmS2uZn1EUlTG8kSVInDjh8e93lIJl0trUgnuJmsfc53Hs2LR3tVZT6iaejXqibQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
*来源：专知*
*概要：DeepMind近期开源了Psychlab，这是一个利用认知心理学等领域知识来研究受控环境下人工智能体（Artificial Agent）的行为，从而模拟人类的行为。*
DeepMind近期开源了Psychlab，这是一个利用认知心理学等领域知识来研究受控环境下人工智能体（Artificial Agent）的行为，从而模拟人类的行为。Psychlab通过一系列典型的任务证明其人工智能体（Artificial Agent）的行为的可靠性：包括视觉搜索、连续识别、多目标搜索等任务。同时也提供了简单实用的API，使其他研究者可以在其基础上轻松构建自己的任务。DeepMind出品必出精品！我们在文末附有paper链接和代码地址，感兴趣的读者可不要错过。专知内容组编辑整理。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw0xuaknXFiadib6rJ8VRuuFHdmu9sxVMydEzlkAUnp9w7kLkb7pnBMA8DGrH4dIF14T6pwI58cX1tGQ/640?wx_fmt=png)
Open-sourcing Psychlab
考虑一下在购物的时候，如果你没有把清单上一个商品取出，这告诉我们了什么：“关于大脑此时的功能?”。这可能表示在搜索商品列表的时候，你无法将注意力从一个商品移动到另一个商品；也可能表示记住购物清单是有困难的，也可能这两者都有。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw0xuaknXFiadib6rJ8VRuuFHdyWNh0b6fmC8xB8FzGicvWn49wXhwW1puNSBibEy4icOkqQGwuzx73bZVw/640?wx_fmt=png)
好像单一的任务取决于多种认知能力。 我们在人工智能研究中也面临类似的问题，在这种情况下，任务的复杂性往往会使一个取得成功的agent中的个体技能难以分离。 但是了解agent特定的个体技能可能对改善其整体表现有作用。
为了解决这个问题，心理学家花了近150年的时间来设计严格控制的实验，目的是隔离特定的认知能力。例如，他们可能会使用两个独立的测试来分析超市的场景——一个“视觉搜索”测试，需要被测者定位图案中的特定的形状，可以用来探测注意力；同时他们可能会要求受测试者回忆列表中的东西来测试他们的记忆力。
我们相信可以使用类似的实验方法来更好地理解人工智能体（artificial agents）的行为。 这就是我们开发Psychlab的原因，它能直接运用认知心理学等领域的方法来研究受控环境下人工智能体的行为，Psychlab是在DeepMind实验室研究基础上建立的平台。 今天，我们开放这个平台供其他人使用。
Psychlab对虚拟DeepMindLab环境（virtual DeepMind Lab environment）中常用的人类心理学实验的结构进行重构。这一般包括一个参与者坐在电脑显示器前，用鼠标对屏幕上的任务做出响应。类似地，我们的环境允许虚拟主体在虚拟计算机监视器上执行任务，利用它的注视方向进行响应。这允许人类和人工因素都采取相同的实验设置，最大限度地减少实验差异。 这使得它更容易与认知心理学的文献联系起来，以便从中获得思路。
随着Psychlab的开源版本的发布，我们构建了一系列典型实验任务，并在虚拟计算机监视器上进行测试，我们提供灵活且易于学习的API，使其他人能够很容易构建自己的任务。
- 
视觉搜索（Visual search）——测试搜索目标数组的能力。
- 
连续识别（Continuous recognition）——为不断增长的物品列表测试内存。
- 
任意的视觉运动测试（Arbitrary visuomotor mapping）——测试回忆的刺激，响应配对。
- 
更改检测（Change detection）——测试当数组中更改检测对象并经过短暂延迟后目标重新出现的能力。
- 
视觉敏锐度和对比敏感度（Visualacuity and contrast sensitivity）——测试识别小对比度和低对比度刺激的能力。
- 
玻璃图案检测（Glass pattern detection）——测试全局形式感知。
- 
随机点运动判别（Randomdot motion discrimination）——检测对相关运动的感知能力。
- 
多目标跟踪（Multipleobject tracking）——测试随着时间的推移能够跟踪移动对象。
上述任务中的每一个任务都已被验证，表明我们的实验结果反映了认知心理学文献中的标准结果。
以“视觉搜索”任务为例。 这是一种理解人类选择性注意力的方法，已经研究了在复杂的刺激阵列中定位目标的能力，如超市货架上的一个商品。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw0xuaknXFiadib6rJ8VRuuFHdC4RC9J0TvhMJQUZIB3RMn26F3BBfPRuTTLPNEkriajVpz4G3gia0AuFQ/640?wx_fmt=png)
当人们搜索“在水平条块之间的垂直取向的条块”和“在其他颜色的条块之间搜索粉色条块”的任务时，其反应时间不会根据屏幕上的条块数量改变而改变。换句话说，他们的反应时间是独立的“设定值”。 然而，当任务是“搜索不同形状和不同颜色的条块中的粉色条块时”，人的反应时间随着每增加一个条块就会增加大约50ms。 当人类在Psychlab上完成这个任务时，我们重现了这个结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw0xuaknXFiadib6rJ8VRuuFHdRX4uBDRPFWiaQVRCnVcpHqUpnhK30ejmPxpgCVNBTgRgtWrFYNlt2AA/640?wx_fmt=png)
这张图片说明了在Psychlab的视觉搜索任务上人类和人工代理之间反应时间的差异。
当我们对一个最先进的人工智能体进行同样的测试时，我们发现它虽然可以执行任务，但并没有显示人类模式（human pattern）的反应时间结果。 在这三种情况下，他们用了相同的时间来应对。 在人类视角，这些数据暗示了并行和串行关注的区别。人工智能体似乎只有并行的机制。 识别这种人类与目前的人工智能体之间的差异，为我们未来设计智能体提供了思路。
Psychlab被设计为认知心理学、神经科学和AI之间桥接的工具。 通过开源的方式，我们希望更多研究团队能够在自己的研究中利用它，并帮助我们进一步改进。
**Paper链接：**
https://arxiv.org/abs/1801.08116
**从GitHub下载代码：**
https://github.com/deepmind/lab/tree/master/game_scripts/levels/contributed/psychlab
**原文链接：**
https://deepmind.com/blog/open-sourcing-psychlab/
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
