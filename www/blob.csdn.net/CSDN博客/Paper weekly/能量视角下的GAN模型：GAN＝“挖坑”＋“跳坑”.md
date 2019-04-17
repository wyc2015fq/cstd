# 能量视角下的GAN模型：GAN＝“挖坑”＋“跳坑” - Paper weekly - CSDN博客





2019年02月12日 08:48:36[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：102









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨苏剑林

单位丨广州火焰信息科技有限公司

研究方向丨NLP，神经网络

个人主页丨kexue.fm




在这个系列中，我们尝试从能量的视角理解 GAN。我们会发现这个视角如此美妙和直观，甚至让人拍案叫绝。 




本视角直接受启发于 Bengio 团队的新作 ***Maximum Entropy Generators for Energy-Based Models ***[1]，这篇文章前几天出现在 arXiv 上。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hm5E3icRXnpRibHOicU00TQX8FQibicyCmrhAAdAEzpQcicJXWyHr4ibrWRGWtg/640?wx_fmt=png)




当然，能量模型与 GAN 的联系由来已久，并不是这篇文章的独创，只不过这篇文章做得仔细和完善一些。另外本文还补充了自己的一些理解和思考上去，力求更为易懂和完整。 




作为第一篇文章，我们先来给出一个直白的类比推导：**GAN 实际上就是一场前仆后继（前挖后跳？）的“挖坑”与“跳坑”之旅。**



![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmniazKBQz3m2Gdg2bXr8bfVbhmPydyaXZOiafyRiab4J9tPDTXUzick1Vbg/640?wx_fmt=png)

**▲**“看那挖坑的人，有啥不一样～”




总的来说，**本文大致内容如下：**




1. 给出了 GAN/WGAN 的清晰直观的能量图像；



2. 讨论了判别器（能量函数）的训练情况和策略；



3. 指出了梯度惩罚一个非常漂亮而直观的能量解释； 




4. 讨论了 GAN 中优化器的选择问题。




# 前“挖”后“跳”




在这部分中，我们以尽量通俗的比喻来解释什么是能量视角下的 GAN。 




首先我们有一批样本 x1, x2, … , xn，我们希望能找到一个生成模型，这个模型有能力造出一批新的样本![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmTOEQXfS1W2SQh3IbPKQ71XXXr3opibp6libNnnHQQIbnR2dib43yeqLbw/640?wx_fmt=png)，我们希望这批新样本跟原样本很相似。怎么造呢？很简单，分两步走。 




**“挖坑”**




**第一步，挖坑：**我们挖很多坑，这些坑的分布可以用一个能量函数 U(x) 描述，然后我们要把真实样本 x1, x2, … , xn 都放在坑底，然后把造出来的假样本![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmTOEQXfS1W2SQh3IbPKQ71XXXr3opibp6libNnnHQQIbnR2dib43yeqLbw/640?wx_fmt=png)放到“坑腰”：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmUGQM0IOc9IReLl0NCc8MiaaAc3ZB6FBjMmGhFjAu3IkBzzOeTv66CSA/640?wx_fmt=png)

**▲**GAN第一步：“挖坑”





![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hms17wfysuRbQl0skaCGbhefZ0eiczq0ayibdtSEpX4vnfgxWO1S1D94jQ/640?wx_fmt=png)

**▲**然后我们把真假样本放到适当的位置




**“跳坑”**




**第二步，跳坑：**把 U(x) 固定住，也就是不要再动坑了，然后把假样本![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmTOEQXfS1W2SQh3IbPKQ71XXXr3opibp6libNnnHQQIbnR2dib43yeqLbw/640?wx_fmt=png)松开，显然它们就慢慢从滚到坑底了，而坑底代表着真实样本，所以![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmTOEQXfS1W2SQh3IbPKQ71XXXr3opibp6libNnnHQQIbnR2dib43yeqLbw/640?wx_fmt=png)都变得很像真样本了：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmRGjOG9JResJv8BNcUVkngrqrHiaVY5eSkz9FicX5aO8rA9j792BUc7RQ/640?wx_fmt=png)

**▲**GAN第二步：“跳坑”




这便是 GAN 的工作流程。




# 把GAN写下来




注意，上述两步不仅仅是简单的比喻，而是 GAN 的完整描述了。根据上述两个步骤，我们甚至可以直接把 GAN 训练公式写出来。 




**判别器**




首先看“挖坑”，我们说了要将真样本放到坑底，假样本放到坑腰，以便后面假样本可以滚到坑底，这意味着假样本的“平均海拔”要高于真样本的“平均海拔”，也就是说：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmpNTcGvGDB5dVm9sibu52f8RsTXU4k3TJWrZFQx152nKZ45nVJL9tFDQ/640?wx_fmt=png)




尽量小，这里我们用 p(x) 表示真实样本的分布，q(x) 表示假样本的分布。假样本通过 x=G(z) 生成，而 z∼q(z) 是标准正态分布。 




**梯度惩罚**




另外，我们还说真样本要在坑底，用数学的话说，坑底就是一个极小值点，导数等于 0 才好，即要满足![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmwmBJkK8ujoa0d3bLoF0Qiab3K4du0sGJGrUoEl6VA2B1OnWMicdboLFw/640?wx_fmt=png)是最理想的，换成优化目标的话，那就是![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmXfsFHmFgHOIoOodFqjW7jqj7OS0nPibffq8ATEue4m0xk8ndIXjBCxA/640?wx_fmt=png)越小越好。两者综合起来，我们就得到 U 的优化目标：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hm2JgyBicT2bEPSkicUKmS40hLabnKvDXKg25I7IHFJgYY2G7icIYrhicjuw/640?wx_fmt=png)




**注：以往对于梯度惩罚，我们总会有两个困惑：**1）梯度惩罚究竟是以 0 为中心好还是以 1 为中心好；2）梯度惩罚要对真样本、假样本还是真假插值样本进行？




现在，基于能量视角，我们可以得到“对真样本进行以 0 为中心的梯度惩罚”比较好，因为这意味着（整体上）要把真样本放在极小值点处。




至此，在能量视角下，我们对梯度惩罚有了一个非常直观的回答。




**生成器**




然后看“跳坑”，也就是坑挖好了， U 固定了，我们让假样本滚到坑底，也就是让 U(x) 下降，滚到最近的一个坑，所以：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmO0d145fHwzuauKIdVBogee6Bzia9LEoz3SicL35ibvj0BibJqMabW00EEw/640?wx_fmt=png)




可以看到，判别器实际上就是在“造势”，而生成器就是让势能最低，这便是能量 GAN 的主要思想。




**交替训练**




如果真实情况的坑都像上面的图那么简单，那么可能就只需要两步就能训练完一个生成模型了。




但是真实情况下的坑可能是很复杂的，比如下图中假样本![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hm6k0mPXLI9aXeOQ3j2fE2LmFFesfjAAkc8sm5Kib0oro1FdZZB2FhDOw/640?wx_fmt=png)慢慢下滑，并不一定能到达 x1 的坑，而是到达一个中间的坑，这个中间的坑并非代表真样本，可能仅仅是“次真”的样本，所以我们需要不断地改进假样本，也需要不断地把坑修正过来（比如争取能下一步把阻碍前进的峰“削掉”）。




这也就是说，我们需要反复、交替地执行 (1)、(3) 两步。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmZhfRmEoCTH46SeAXrtOAw73Vo9jhZRVAKpTYy7SibVeibaEE0bd7iaZtA/640?wx_fmt=png)

**▲**真实情况下坑的分布可能更复杂




**坑的学问**




看，头脑中想象着几个坑，我们就可以把 GAN 的完整框架导出来了，而且还是先进的 **WGAN-GP 的升级版：以 0 为中心的梯度惩罚。**GAN 不过是一场坑的学问。 




对这个GAN的进一步讨论，可以参考我之前写的文章[WGAN-div：默默无闻的WGAN填坑者](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492672&idx=1&sn=f21535d54506b87a2a2e20ea75cca024&chksm=96ea3bc0a19db2d6f25770b6ad6b9c5fd99767bea55bfcbb9f57c0b4507fc81711949b9d48ad&scene=21#wechat_redirect)或者论文 ***Which Training Methods for GANs do actually Converge? ***[2]。 




# 进一步思考




上述图景还能帮助我们回答很多问题。比如判别器能不能不要梯度惩罚？为什么 GAN 的训练、尤其是生成器的训练多数都不用带动量的优化器，或者就算用带动量的优化器，也要把动量调小一点？还有 mode collapse（模式坍缩）是怎么发生呢？



**Hinge Loss**




梯度惩罚在理论上很漂亮，但是它确实太慢，所以从实践角度来看，其实能不用梯度惩罚的话最好不用梯度惩罚。但是如果不用梯度惩罚，直接最小化式 (1)，很容易数值不稳定。 




这不难理解，因为没有约束情况下，很容易对于真样本有 U(x)→−∞ ，对于假样本有 U(x)→+∞ ，也就是判别器优化得太猛了，差距拉得太大（无穷大）了。




那么一个很自然的想法是，分别给真假样本分别设置一个阈值，U(x) 的优化超过这个阈值就不要再优化了，比如：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hm3q6HduBpWCp6qhzUnjovAn4JaVLO0EFmz4l3Vkz4AYKOUj7bbGgxAw/640?wx_fmt=png)




这样一来，对于 x∼p(x)，如果 U(x)<−1，则 max(0,1+U(x))=0，对于 x∼q(x)，U(x)>1，则 max(0,1−U(x))=0，这两种情况下都不会在优化 U(x) 了，也就是说对于真样本 U(x) 不用太小，对于假样本 U(x) 不用太大，从而防止了 U(x) 过度优化了。 




**这个方案就是 SNGAN、SAGAN、BigGAN 都使用的 hinge loss 了。**




当然，如果 U(x) 本身就是非负的（比如 EBGAN 中用自编码器的 MSE 作为 U(x)），那么可以稍微修改一下式 (4) ：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmg9ibicibcusOJRPjIFolklnLalYMHYy7qLOyo7N8ciaGibXK5Me5lxLE3icA/640?wx_fmt=png)




其中 m>0。




**优化器**




至于优化器的选择，其实从“跳坑”那张图我们就可以看出答案来。 




带动量的优化器有利于我们更快地找到更好的极小值点，但是对于 GAN 来说，其实我们不需要跑到更好的极小值点，我们只需要跑到最近的极小值点，如果一旦跳出了最近的极小值点，跑到更低的极小值点，那么可能就丧失了多样性，甚至出现 mode collapse。 




比如下图中的![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmWYnMPzhTI9zHhgia1JEicxDfgJUQ2EF6kg7J5XWjp1sjjafQnAxWPHlg/640?wx_fmt=png)，不带动量的优化算法能让![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmWYnMPzhTI9zHhgia1JEicxDfgJUQ2EF6kg7J5XWjp1sjjafQnAxWPHlg/640?wx_fmt=png)跑到 x2 处就停下来，如果带动量的话，那么可能越过 x2 甚至跑到 x1 去了。尽管 x1 也是真样本，但是这样一来![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmR6RpXOeMnK8XW87yX6ZKM7trSqPNfTticH80r7W4LYhOrp0cuz1jXlw/640?wx_fmt=png)同时向 x1 靠拢，也许没有假样本能生成 x2 了，从而丧失了多样性。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmlzgJ9DfrKoiajClD4mBQibqLQIvuZ3Vj1BlDnWY9qbtLyygrbTSGhVNA/640?wx_fmt=png)

**▲**带动量与不带动量的优化轨迹比较：不带动量时，假样本只需要落到最近一个坑，如果带动量的话，可能越过最近的坑，到达更远的坑去，导致假样本聚集在某些真样本附近，丧失多样性。




所以，在 GAN 的优化器中，动量不能太大，太大反而有可能丧失生成样本的多样性，或者造成其他的不稳定情况。 




**Mode Collapse**




什么是 mode collapse？为什么会发生 mode collapse？还是可以用这个图景来轻松解释。 




前面我们画的图把假样本![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmQR5aLoESQ1vWUshjfNuy7cw8XtGjwDzpX3BOOrmt6dXrxFLNpuRUCQ/640?wx_fmt=png)画得很合理，但是如果一旦初始化不好、优化不够合理等原因，使得![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmQR5aLoESQ1vWUshjfNuy7cw8XtGjwDzpX3BOOrmt6dXrxFLNpuRUCQ/640?wx_fmt=png)同时聚在个别坑附近，比如：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmkeug2AE5eGjUCaeNxnF2YTiccuRwmmFicHAhkaYJibrL0wibFt15Fw3Bxg/640?wx_fmt=png)

**▲**Mode Collapse图示




这时候按照上述过程优化，所有假样本都都往 xn 奔了，所以模型只能生成单一（个别）样式的样本，这就是 mode collapse。 




简单来看，mode collapse 是因为假样本们太集中，不够“均匀”，所以我们可以往生成器那里加一个项，让假样本有均匀的趋势。这个项就是假样本的熵 H(X)=H(G(Z))，我们希望假样本的熵越大越好，这意味着越混乱、越均匀，所以生成器的目标可以改为：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hmf9l2NY7uGoHL2XKGSDoWEW02QCyjxib2S9Il5gGPCBxxU3TzFic38jeA/640?wx_fmt=png)




这样理论上就能解决 mode collapse 的问题。至于 H(X) 怎么算，我们后面会详细谈到。




# 能量视角之美




对于 GAN 来说，最通俗易懂的视角当属“造假者-鉴别者”相互竞争的类比，这个视角直接导致了标准的 GAN。但是，这个通俗的类比无法进一步延伸到 WGAN 乃至梯度惩罚等正则项的理解。




相比之下，能量视角相当灵活，它甚至能让我们直观地理解 WGAN、梯度惩罚等内容，这些内容可以说是目前 GAN 领域最“先进”的部分成果了。虽然看起来能量视角比“造假者-鉴别者”形式上复杂一些，但其实它的物理意义也相当清晰，稍加思考，我们会感觉到它其实更为有趣、更具有启发性，有种“越嚼越有味”的感觉。




# 参考文献




[1] Rithesh Kumar, Anirudh Goyal, Aaron Courville, Yoshua Bengio, “Maximum Entropy Generators for Energy-Based Models”, arXiv preprint arXiv:1901.08508, 2019. 

[2] Lars Mescheder, Andreas Geiger, Sebastian Nowozin, “Which Training Methods for GANs do actually Converge?”, ICML 2018.




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看作者其他文章：**




- 
[变分自编码器VAE：原来是这么一回事 | 附开源代码](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488480&idx=1&sn=7bb9a4dd60680df5013670287a9e0cc2&chksm=96e9cc60a19e457618b2ffdea2a7e13ba172ea3fdfbfda07de53aae57126047f1b71a6969d76&scene=21#wechat_redirect)[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)

- 
[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)

- 
[从变分编码、信息瓶颈到正态分布：论遗忘的重要性](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493326&idx=1&sn=7ba19fe14ee11bff0e1c865adcb52ca1&chksm=96ea394ea19db0587dc096898730f7522a8e3a7bb3b55bac576422eea63a987ea97ad5886bca&scene=21#wechat_redirect)


- 
[深度学习中的互信息：无监督提取特征](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492040&idx=1&sn=f90a6b899e62748c4db489ce06276869&chksm=96ea3e48a19db75e8c07d942a4772bb6c784fac7bcb117da2023186546cfe1876b121a8121cc&scene=21#wechat_redirect)

- 
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)

- 
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)

- 
[细水长flow之f-VAEs：Glow与VAEs的联姻](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491695&idx=1&sn=21c5ffecfd6ef87cd4f1f754795d2d63&chksm=96ea3fefa19db6f92fe093e914ac517bd118e80e94ae61b581079023c4d29cedaaa559cb376e&scene=21#wechat_redirect)

- 
[深度学习中的Lipschitz约束：泛化与生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492180&idx=1&sn=3ea92a3a9f1306efde89ce1777b80da6&chksm=96ea3dd4a19db4c20dcbc9627b0eb307672b4d61008a93c42814fa6728ca7b6f7c293cff1d80&scene=21#wechat_redirect)












**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**




总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向） 

• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接 

• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志




**📬 投稿邮箱：**

• 投稿邮箱：hr@paperweekly.site

• 所有文章配图，请单独在附件中发送 

• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通










🔍




现在，在**「知乎」**也能找到我们了

进入知乎首页搜索**「PaperWeekly」**

点击**「关注」**订阅我们的专栏吧







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 查看作者博客




