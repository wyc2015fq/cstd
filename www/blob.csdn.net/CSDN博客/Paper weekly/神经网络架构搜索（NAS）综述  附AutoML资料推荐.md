# 神经网络架构搜索（NAS）综述 | 附AutoML资料推荐 - Paper weekly - CSDN博客





2018年08月31日 12:06:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：399









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




本文是一篇**神经网络架构搜索综述文章**，从 Search Space、Search Strategy、Performance Estimation Strategy 三个方面对架构搜索的工作进行了综述，几乎涵盖了所有近几年的优秀工作。




■ 论文 | Neural Architecture Search: A Survey

■ 链接 | https://www.paperweekly.site/papers/2249

■ 作者 | Thomas Elsken / Jan Hendrik Metzen / Frank Hutter




# 引言




深度学习模型在很多任务上都取得了不错的效果，但调参对于深度模型来说是一项非常苦难的事情，众多的超参数和网络结构参数会产生爆炸性的组合，常规的 random search 和 grid search 效率非常低，**因此最近几年神经网络的架构搜索和超参数优化成为一个研究热点。**



**本文从网络架构搜索的三个方面进行了分类综述**，包括： 



- 
搜索空间 

- 
搜索策略 

- 
评价预估





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSmBSMibk6LpnGLshe5ZwGmC8NciapyLrJRfaWMjJqAEaWt7p1qkakvlEz5khWOCIQMicHKKNtAj5hg/640)



# 问题定义




**网络架构和超参数优化的问题，有以下的特点：**




1. 评价函数未知，是一个黑箱优化问题，因为评价往往是在 unseen dataset 上进行评价；




2. 非线性；




3. 非凸；




4. 混合优化，既有离散空间，又有连续空间； 




5. 一次优化结果的评价非常耗时，大型的深度学习模型参数数以亿计，运行一次结果需要几周时间；




6. 在某些应用场景中，存在多个目标。比如：移动端的模型结构优化，既希望得到尽量高的准确率，又希望有非常好的模型计算效率。



# 搜索空间




搜索空间定义了优化问题的变量，网络结构和超参数的变量定义有所不同，不同的变量规模对于算法的难度来说也不尽相同。




早期很多工作都是用以遗传算法为代表的进化算法对神经网络的超参数和权重进行优化，因为当时的神经网络只有几层，每层十几个神经元，也不存在复杂的网络架构，参数很有限，可直接进行优化。而深度学习模型一方面有着复杂的网络结构，另一方面权重参数通常都以百万到亿来计，进化算法根本无法优化。




但换个思路，假如我们找到了一组网络架构参数和对应的超参数，深度学习模型的性能其实是由这组参数来控制和决定的，所以**只需要对复杂模型的架构参数和对应的超参数进行优化即可**。




目前常见的一种网络架构是链状结构，如下图：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSmBSMibk6LpnGLshe5ZwGmocC7YflKRnzPN1c1wXBuIzY9fv3pLYzA55Dxe0pjrNiazp1lfaKh4ag/640)




这种结构相当于一个 N 层的序列，每一层有几种可选的算子，比如卷积、池化等，每种算子包括一些超参数，比如卷积尺寸、卷积步长等。




最近的一些工作受启发于一些人工设计的网络架构，研究带有多分支的网络，如下图：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSmBSMibk6LpnGLshe5ZwGm2G4jaT3zjPbPsQHw2fewMwubzGSBEiadSEc9b9HJBdWnC9lYuONsGGg/640)




很多的深层 RNN 会有类似的结构，很多的网络结构虽然很深，但会有许多重复 cell，将 cell 抽象出来之后，复杂的结构也会变得简单，一方面可以减少优化变量数目，另一方面相同的 cell 在不同任务之间进行迁移，如下图。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSmBSMibk6LpnGLshe5ZwGmIZZDKfx8Micsr8XwE3kN9jmKKVDfCiaFFMdMJrXTtlckDmKHC49JqXhg/640)




网络架构搜索问题由于其高维度、连续和离散混合等诸多难点，在搜索空间维度这块如果可以做些降维，将会大大提升效果，Zoph 在 2018 年的工作用了 cell 这种方式相比于 2017 年的工作有 7 倍的加速。




# 搜索策略




**搜索策略定义了使用怎样的算法可以快速、准确找到最优的网络结构参数配置。**常见的搜索方法包括：随机搜索、贝叶斯优化、进化算法、强化学习、基于梯度的算法。其中，2017 年谷歌大脑的那篇强化学习搜索方法将这一研究带成了研究热点，后来 Uber、Sentient、OpenAI、Deepmind 等公司和研究机构用进化算法对这一问题进行了研究，这个 task 算是进化算法一大热点应用。




注：国内有很多家公司在做 AutoML，其中用到的一种主流搜索算法是进化算法。




**强化学习**




强化学习是一种非常有意思的范式，几乎只要可以提炼出强化学习四要素，原问题就可以用强化学习来求解。 




在 NAS 任务中，将架构的生成看成是一个 agent 在选择 action，reward 是通过一个测试集上的效果预测函数来获得（这个函数类似于工程优化问题中的 surrogate model，即代理模型）。**这类工作整体的框架都是基于此，不同的点在于策略表示和优化算法。**




一个工作是，用 RNN 来表示策略函数，初始化时先用策略梯度算法赋初值，然后用 PPO 来进行优化。另一个工作是，用简单的 Q-learning 算法来训练策略函数，序列地进行动作选择，即选择 layer 的类型和相关的超参数。




因为在 NAS 任务中，agent 与环境没有交互，可以降阶为无状态的多臂老虎机（MAB）问题。 这里的几个工作都是近两三年的新工作，后面会对这些 paper 进行专门解读。




**进化算法**




进化算法是一大类算法，大概的框架也基本类似，先随机生成一个种群（N 组解），开始循环以下几个步骤：选择、交叉、变异，直到满足最终条件。最近几年流行一种基于概率模型的进化算法 EDA (Estimation Distribution of Algorithm)，基本的思路类似遗传算法，不同的是没有交叉、变异的环节，而是通过 learning 得到一个概率模型，由概率模型来 sample 下一步的种群。 




用进化算法对神经网络超参数进行优化是一种很古老、很经典的解决方案，90 年代的学者用进化算法同时优化网络结构参数和各层之间的权重，因为当时的网络规模非常小，所以还能解决，但后续深度学习模型网络规模都非常大，无法直接优化。 




**进化算法是一种无梯度的优化算法（Derivative Free Optimization Algorithm），优点是可能会得到全局最优解，缺点是效率相对较低**，近两年几家高科技企业做 NAS 时都在用进化算法优化网络结构，同时用基于梯度的方法（BP）来优化权值。在 NAS 任务中，进化算法的交叉算子和任务结合比较紧，被定义为一些类似添加、删除层的操作，而非简单的更改某一位编码。 




用进化算法解决 NAS 问题，不同的工作可能聚焦在不同的过程中，比如如何 sample 种群，如何 update 种群，如何生成子代种群等。这些工作将会被在后面的文章中进行解读。




**贝叶斯优化**




贝叶斯优化（Bayesian Optimization）是超参数优化问题的常用手段，尤其是针对一些低维的问题，基于高斯过程（Gaussian Processes）和核方法（kernel trick）。对于高维优化问题，一些工作融合了树模型或者随机森林来解决，取得了不错的效果。 




除了常见的三大类方法，一些工作也在研究分层优化的思路，比如将进化算法和基于模型的序列优化方法融合起来，取各种方法的优势。Real 在 2018 年的一个工作对比了强化学习、进化算法和随机搜索三类方法，前两种的效果会更好一些。




# 评价预估




评价预估类似于工程优化中的代理模型（surrogate model），因为深度学习模型的效果非常依赖于训练数据的规模，大规模数据上的模型训练会非常耗时，对优化结果的评价将会非常耗时，所以需要一些手段去做近似的评估。 




一种思路是**用一些低保真的训练集来训练模型**，低保真在实际应用可以有多种表达，比如训练更少的次数，用原始训练数据的一部分，低分辨率的图片，每一层用更少的滤波器等。用这种低保真的训练集来测试优化算法会大大降低计算时间，但也存在一定的 bias，不过选择最优的架构并不需要绝对数值，只需要有相对值就可以进行排序选优了。 




另一种主流思路是**借鉴于工程优化中的代理模型**，在很多工程优化问题中，每一次优化得到的结果需要经过实验或者高保真仿真（有限元分析）进行评价，实验和仿真的时间非常久，不可能无限制地进行评价尝试，学者们提出了一种叫做代理模型的回归模型，用观测到的点进行插值预测，这类方法中最重要的是在大搜索空间中如何选择尽量少的点预测出最优结果的位置。 




第三种主流思路是**参数级别的迁移**，用之前已经训练好的模型权重参数对target问题进行赋值，从一个高起点的初值开始寻优将会大大地提高效率。在这类问题中，积累了大量的历史寻优数据，对新问题的寻优将会起到很大的帮助，用迁移学习进行求解，是一个很不错的思路。 




另一种比较有意思的思路叫做**单次（One-Shot）架构搜索**，这种方法将所有架构视作一个 one-shot 模型（超图）的子图，子图之间通过超图的边来共享权重。




# 思考和评论




**网络结构和超参数优化是自动机器学习（AutoML）中关键的一个环节**，在上一个人工智能时代，浅层神经网络比较火的时候，自动调参也有很多工作，只不过区别在于不仅仅对超参数进行优化，还对网络的权值一起进行了优化。 




在这个人工智能时代来临之后，各种深度模型的效果非常好，重新点燃了这个方向的研究激情，加上强化学习的火热，各种流派百家争鸣。 




**这个领域中，今后比较有意思的研究点包括：**



- 
研究针对多任务和多目标问题的 NAS； 

- 
研究更加灵活的搜索变量表示，类似 cell 这种方便进行迁移的表示方式； 

- 
挖掘更多的、有难度的 benchmark； 

- 
迁移学习的引入，进一步提高效率。 





本文作者是 Thomas Elsken，来自德国弗莱堡大学的博士生，他们组维护一个 **AutoML 博客**[1]。这篇综述涵盖了 NAS 任务近几年几乎所有的工作，总结地非常详细。他们组里还在撰写一本 **AutoML 的学术专著——*****AUTOML: METHODS, SYSTEMS, CHALLENGES***[2]。




**AutoML相关Paper Repo推荐：**




> 
**1. awesome-automl-papers**


https://github.com/hibayesian/awesome-automl-papers

**2. awesome-architecture-search**

https://github.com/markdtw/awesome-architecture-search





**AutoML相关创业公司推荐：**



> 
**1. 第四范式**


https://www.4paradigm.com/

**2. 探智立方**

http://iqubic.net/

**3. 智易科技**


https://www.zhiyi.cn/

**4. 智铀科技**

https://www.wisutech.com/

**5. 云脑科技**


http://www.cloudbrain.ai/

**6. Sentient**

https://www.sentient.ai/





当然，Google、Microsoft、Salesforce、阿里云也都有 AutoML 服务，很多大公司内部也都有自己的平台，Uber、OpenAI、Deepmind 也都在 NAS 任务上做研究。从发展趋势来看，AutoML 是未来人工智能发展的一个重要方向，但目前现阶段的研究成果成熟度和实际产品应用成熟度都存在巨大的提升空间。




# 相关链接




[1]. https://www.automl.org/

[2]. https://www.automl.org/book/




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多论文解读：**




- 
[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)

- 
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[CMU论文解读：基于GAN和VAE的跨模态图像生成](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490998&idx=1&sn=31ef7c8947641454b2efff17e055cef3&chksm=96e9c236a19e4b20afba4fd59b70e1e9f6f25a5032b8a3998ed6bd7fd89c806953e14bf990e0&scene=21#wechat_redirect)


- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)[哈佛NLP组论文解读：基于隐变量的注意力模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490536&idx=1&sn=0998c5dd4e20841f3542ee328de1f1b4&chksm=96e9c468a19e4d7e24a38f3d9c4b1b4ea48d729c7db124f2b92a2309f0eee982bb9f9bc8356e&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)

- 
[COLING 2018最佳论文：序列标注经典模型复现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490099&idx=1&sn=2d2497999186b979dd557fe3133b7606&chksm=96e9c5b3a19e4ca550a7ae55705af84e941b1aba14cb21f3f2ffc366df837d387575f8529cf2&scene=21#wechat_redirect)

- 
[一文解析OpenAI最新流生成模型「Glow」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。









