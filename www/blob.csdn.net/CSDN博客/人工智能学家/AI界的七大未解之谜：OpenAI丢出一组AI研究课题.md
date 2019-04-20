# AI界的七大未解之谜：OpenAI丢出一组AI研究课题 - 人工智能学家 - CSDN博客
2018年02月01日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：148
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWY8zUXD6Uic5mpPcReuep9Q8feQPNVT7paRpEL808bYuic1tGaHEeuZ24AKrAiaqncRJA7uTOXBWNKw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
量子位 出品 | 公众号 QbitAI
##### 作者：林鳞 
今天，OpenAI在官方博客上丢出了7个研究过程中发现的未解决问题。
OpenAI希望这些问题能够成为新手入坑AI的一种有趣而有意义的方式，也帮助从业者提升技能。
OpenAI版AI界七大未解之谜，现在正式揭晓——
**1. Slitherin’**
难度指数：☆☆
实现并解决贪吃蛇的多玩家版克隆作为Gym环境。
环境：场地很大，里面有多条蛇，蛇通过吃随机出现的水果生长，一条蛇在与另一条蛇、自己或墙壁相撞时即死亡，当所有的蛇都死了，游戏结束。
智能体：使用自己选择的自我对弈的RL算法解决环境问题。你需要尝试各种方法克服自我对弈的不稳定性。
检查学习行为：智能体是否学会了适时捕捉食物并避开其他蛇类？是否学会了攻击、陷害、或者联合起来对付竞争对手？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWY8zUXD6Uic5mpPcReuep9Q0RXzVnHeaZUYWPkl9iaNth9r98YFM5Du8NicibamguEhMuT1ibibiaBlxzBQ/640?wx_fmt=png)
**2. 分布式强化学习中的参数平均**
难度指数：☆☆☆
这指的是探究参数平均方案对RL算法中样本复杂度和通信量影响。一种简单的解决方法是平均每个更新的每个worker的梯度，但也可以通过独立地更新worker、减少平均参数节省通信带宽。
这样做还有一个好处：在任何给定的时间内，我们都有不同参数的智能体，可能出现更好的探测行为。另一种可能是使用EASGD这样的算法，它可以在每次更新时将参数部分结合在一起。
**3. 通过生成模型完成的不同游戏中的迁移学习**
难度指数：☆☆☆
这个流程如下：
训练11个Atari游戏的策略。从每个游戏的策略中，生成1万个轨迹，每个轨迹包含1000步行动。
将一个生成模型（如论文Attention Is All You Need提出的Transformer）与10个游戏产生的轨迹相匹配。
然后，在第11场比赛中微调上述模型。
你的目标是量化10场比赛预训练时的好处。这个模型需要什么程度的训练才能发挥作用？当第11个游戏的数据量减少10x时，效果的大小如何变化？如果缩小100x呢？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWY8zUXD6Uic5mpPcReuep9QWYtOibQ18JgHibvPwrtdxQHpVTpvKX3iaic4CBvyiaibndMHv8xLYibGKibEcQ/640?wx_fmt=png)
**4. 线性注意Transformer**
难度指数：☆☆☆
Transformer模型使用的是softmax中的软注意力（soft attention）。如果可以使用线性注意力（linear attention），我们就能将得到的模型用于强化学习。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWY8zUXD6Uic5mpPcReuep9QekAOEdFfwuGU1y0YeuN3Q5iaxmksdLtk49jqjfPyDlZRQGVemN8WkXg/640?wx_fmt=png)
具体来说，在复杂环境下使用Transformer部署RL不切实际，但运行一个具有快速权重（fast weight）的RNN可行。
你的目标是接受任何语言建模任务，训练Transformer，然后找到一种在不增加参数总数情况下，用具有不同超参数的线性注意Transformer获取每个字符/字的相同位元的方法。
先给你泼盆冷水：这可能是无法实现的。再给你一个潜在的有用提示，与使用softmax注意力相比，线性注意转化器很可能需要更高的维度key/value向量，这能在不显著增加参数数量的情况下完成。
**5. 已学习数据的扩充**
难度指数：☆☆☆
可以用学习过的数据VAE执行“已学习数据的扩充”。
我们首先可能需要在输入数据上训练一个VAE，然后将每个训练点编码到一个潜在的空间，之后在其中应用一个简单（如高斯）扰动，最后解码回到观察的空间。用这种方法是否能得到更好的泛化，目前还是一个谜题。
这种数据扩充的一个潜在优势是，它可能包含视角变换、场景光纤变化等很多非线性转换。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWY8zUXD6Uic5mpPcReuep9QIXk4ibicTAu9c6vy4Y4FqF7kOP4vj2HNnmia35hM6ZibKAeYtYPGiaDFAmg/640?wx_fmt=png)
**6. 强化学习中的正则化**
难度指数：☆☆☆☆
这指的是实验性研究和定性解释不同正则化方法对RL算法的影响。
在监督学习中，正则化对于优化模型和防止过拟合具有极其重要的意义，其中包含一些效果很赞的方法，如dropout、批标准化和L2正则化等。
然而，在策略梯度和Q-learning等强化学习算法上，研究人员还没有找到合适的正则化方法。顺便说一下，人们在RL中使用的模型要比在监督学习中使用的模型小得多，因为大模型表现更差。
这儿有一篇老论文供参考：
http://sologen.net/papers/RegularizationInReinforcementLearning(PhD-Dissertation-Farahmand).pdf
**7. Olympiad Inequality问题的自动解决方案**
难度指数：☆☆☆☆☆
Olympiad Inequality问题很容易表达，但解决这个问题往往需要巧妙的手法。
建立一个关于Olympiad Inequality问题的数据集，编写一个可以解决大部分问题的程序。目前还不清楚机器学习在这里是否有用，但你可以用一个学习的策略减少分支因素。
最后，附OpenAI原贴地址：
https://blog.openai.com/requests-for-research-2/
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
