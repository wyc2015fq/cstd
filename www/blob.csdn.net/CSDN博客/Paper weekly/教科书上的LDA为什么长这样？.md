
# 教科书上的LDA为什么长这样？ - Paper weekly - CSDN博客


2018年08月30日 13:10:19[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：364


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨DeAlVe
学校丨某211硕士生
研究方向丨模式识别与机器学习

**线性判别分析（Linear Discriminant Analysis, LDA）是一种有监督降维方法**，有关机器学习的书上一定少不了对 PCA 和 LDA 这两个算法的介绍。**LDA 的标准建模形式是这样的**（这里以两类版本为例，文章会在几个关键点上讨论多类情况）：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDCAW3JzicadEEPg9Xv9GhaeGDhMvH6jnsnyrhp7Av0j9H3icGHgdzEKYw/640)

其中，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDrpJCelwTvd2c0N1ibU8Z1tHFs8j9TdAI35RNgwa0fDoCB4mQibkGH6ag/640)是类间散布矩阵，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDJBcnNHxdzf3DcoDlgZbV5nib3QFYgHHRoZYjuyiat1NolWxkSxE5qKxg/640)是类内散布矩阵， w 是投影直线：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDsU2YRSgbH2gBcmAmpc5wFjibBWltXz8xbSd8xMryhA8BMXeLXO9cGwQ/640)

怎么样，一定非常熟悉吧，经典的 LDA 就是长这个样子的。这个式子的目标也十分直观：将两类样本投影到一条**直线**上，使得投影后的类间散布**矩阵**与类内散布矩阵的**比值**最大。

**三个加粗的词隐含着三个问题：**

1. 为什么是类间散布**矩阵**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDticTiaKQSicWWYHlsq4FQxYibbssSFmYt2VmEzWQquEMEFRNYzStWuTFGA/640)呢？直接**均值之差**m1-m2 不是更符合直觉吗？这样求出来的解和原来一样吗？

2. 为什么是投影到**直线**，而不是投影到**超平面**？PCA 是把 d 维样本投影到 c 维 (c<d)，LDA 为什么不能也投影到 c 维，而是直接投影到 1 维呢？同样地，在 K 类 LDA 中，为什么书上写的都是投影到 K-1 维，再高一点不行吗？这是必然吗？

3. 为什么是类间散布与类内散布的**比值**呢？**差值**不行吗？

这篇文章就围绕这三个问题展开。我们先回顾一下经典 LDA 的求解，然后顺次讲解分析这三个问题。

# 回顾经典LDA

原问题等价于这个形式：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDpkIWl0ejbvHiaLsH7mP7jkuQc16ukHmUAiaalJEAmQytCFRXxTcgOmDQ/640)

然后就可以用拉格朗日乘子法了：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD5IrI3fmH1BbmdOseia4K8FJVzsgNa3gUcGdBMqejnpaVmyoxC7UBRUA/640)

求导并令其为 0：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD0fvrcK1ksbSBAkibo9VTZEmwzyY2vUhUIPThNejr5P3JzQJg8nt3wcw/640)

得到解：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDicAvZzlhL4urUMVDhYGnkkZagPiaYuQ67lDxmquE4GgLJveJ8gKcKCLQ/640)

对矩阵![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD9dzwC6UQxcicpDnW46hxhm1Sic29MmBDneatxVgELBdSAY1PStbXFk1w/640)进行特征值分解就可以得到 w。**但是有更简单的解法：**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDAlu31EYicSvscBIIkewNzE3qX1TDSicybgslIPdt8vqYiamQ3iaAjw0TSA/640)

而其中![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDFiccrIXGLLDSicdhfPial7ecMAeDgkXUiaWu5IPcAQ5jeQmticnOzFHuxIQ/640)是一个标量，所以![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD8trLph68p1mzD4UG9Al4w0N2j4tY74xkaQtKGDrnYUlNbqVdf6nuiag/640)和 λw 共线，得到：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDI10ZTkJfhzDVJyPGic6mjzhuRUfq9iaibt1tKE97pia8pUweyn5z8lSUXA/640)

求解完毕。非常优雅，不愧是教科书级别的经典算法，整个求解一气呵成，标准的拉格朗日乘子法。但是求解中还是用到了一个小技巧：![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDFiccrIXGLLDSicdhfPial7ecMAeDgkXUiaWu5IPcAQ5jeQmticnOzFHuxIQ/640)是标量，从而可以免去特征值分解的麻烦。

那么，我们能不能再贪心一点，找到一种连这个小技巧都不需要的求解方法呢？答案是可以，上面的问题在下一节中就能得到解决。

# 类间散布 & 均值之差

我们不用类内散布矩阵![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDticTiaKQSicWWYHlsq4FQxYibbssSFmYt2VmEzWQquEMEFRNYzStWuTFGA/640)了，改用均值之差 m1-m2 这个更符合直觉的东西：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDlzYLRUXE9ZCY7m2bFzZeDeMqwHjd4QRxjvia1Oj5LZxVoNLRr3ZxjvA/640)

还是用拉格朗日乘子法：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDXMv0pW9EZ2zrSOj243eqRjbAicicEZehS7tyybMqQfZUzTb0I3OicD8kQ/640)

求导并令其为 0：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDPnzVRicz4AibgicmCY7wD08LqNHpVsagpLxkBKSD6asNGXkEVoe3iaQ86g/640)

得到解：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD04hjoJJt1icwbic47TQ3Clh0tHTdMCzicBAIe47Y2xFAwuvS4brhqe4Dw/640)

怎么样，是不是求解更简单了呢？**不需要任何技巧**，一步一步下来就好了。

为什么说均值之差更符合直觉呢？大家想啊，LDA 的目的是让投影后的两类之间离得更远，类内离得更近。说到类内离得更近能想到的最直接的方法就是让类内方差最小，这正是类内散布矩阵；说到类间离得更远能想到的最直接的方法肯定是让均值之差最大，而不是均值之差与自己的克罗内克积这个奇怪的东西最大。

那么经典 LDA 为什么会用类间散布矩阵呢？我个人认为是这样的表达式看起来更加优雅：分子分母是齐次的，并且这个东西恰好就是广义瑞利商：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDoNGALGq33q7HR7nAFkhng9UoiahpEh0AabGht9QusAp6IaB3zuwtLLw/640)

虽然经典 LDA 求解没有上面这个方法直接，但是问题表述更加规范，所用到的技巧也非常简单，不会给是使用者带来困扰，所以 LDA 最终采用的就是类间散布矩阵了吧。

# 直线 & 超平面

上面那个问题只算是小打小闹，没有太大的意义，但是这个问题就很有意义了：**LDA 为什么直接投影到直线（一维），而不能像 PCA 一样投影到超平面（多维）呢？**我们试试不就完了。

假设将样本投影到![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDj0Hel2NoYlbMKOLYpOTLvvF9EJkg7XVZazicsbSgDU5UJLpoTRgcYrQ/640)上，其中每一个 wi 都是经典 LDA 中的 w 。也就相当于我们不是把样本投影到一条直线上，而是投影到 c 条直线上，也就相当于投影到了超平面上。投影后的样本坐标为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD4YtwicZq8HekXhS9d23CFkudMxWanuaAN86iawMuaEYhOqzn5NGBEZQA/640)

所以样本的投影过程就是：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD6oDz4oCUl0sDX7t9FIYMGfrcQWNo3s2KibhTRteKtodmvAQrfWvno9g/640)

那么，均值的投影过程也是这样：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDAzRTVH8XRXryXv0TULT5jCxjlerfnia1MvXUt4t3Q1deK1hOPexVicmw/640)

投影后的均值之差的二范数：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD1Inzt9DrzmxlKeo9DmzNbTWRJG7icAoq3jzdiaHVX1IG9YkM5EtLsQKA/640)

为什么不用第一行的向量内积而偏要用第二行的迹运算呢？因为这样可以拼凑出类间散布![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDticTiaKQSicWWYHlsq4FQxYibbssSFmYt2VmEzWQquEMEFRNYzStWuTFGA/640)来，和经典 LDA 保持形式的一致。

回顾一下经典 LDA 的形式：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDZ4ibgWyPGeOj9jRVVCLDBEUiciaMRlaoxp37oyVUYKCv1sDv2wEsSCajg/640)

现在我们有了![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDQm0krBkDP9s6ibNTxLtVnhavcvrx8ZFTwdlTcscSwqobyb6KQJZqP4w/640)，还缺个约束，类比一下就可以得到![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDj4F6pLfhp8aepXXrnJN36LCjyrG1licyRIF6TWibZC4Oxl4ZT7MhDUdQ/640)了：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD1fXjFo5G75S8OjpFzC9WdBay0jic58QVibokAOBOMegAsDt99Br213hQ/640)

实际上，约束也可以选择成![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDNrRwIpwia53ZibcN1wicfpSHOfHmwHX6rvQ3O9jrwj8H2pBCNicA3olHPg/640)，这两个约束实际上都是在限制 W 的解空间，得出来的解是等价的，这两个约束有什么区别呢？我只发现了一点：

回想![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDGticcqxAQ2U6exgW5qoTibrVFOJqmdcqNlibz1xL5reFdCnUXMIKnBywA/640)是 c 条投影直线，为了确保向这 c 条直线投影能等价于向 c 维子空间投影，**我们需要保证 c 条直线是线性无关的**，即  rank(W) =c。看一下约束：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDMsibiaNXqyiaMU4guqFJXl0ZaRzKHjBbQxvwcgPOeice65iaxzsy84LtSOg/640)

右边是秩为 c 的单位矩阵，因为矩阵乘积的秩不大于每一个矩阵的秩，所以左边这三个矩阵的秩都要不小于 c，因此我们得到了 rank(W) = c。**也就是说，**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDiaQpG8icAIeia3CsKMW5qpzjhwbbfa0OWw0SYt0EZBbuSthHzb9icmBOkA/640)**能够保证我们在向一个 c 维子空间投影。而约束**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD59FbLAol1TqqtS0cFrz2JImdRCjFicSwpuAR4ts2dkXtSI8UtcYtIibg/640)**中没有显式地表达这一点**。

我对矩阵的理解还不够深入，不知道![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD9ianUP3yxWj6K9zqiaj8oCsJySg7ia2aHHplPVSXhTBXtFJZgJBD6wN8Q/640)是否也隐含了对秩的约束，所以为了保险起见，我选择了带有显式秩约束的![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDIG0ibT3KK0cr1L7r7mDibqacwKq8ODtltMyFGZQELK2pib6g89u3uk6PA/640)，这样就得到了我们的高维投影版 LDA：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDSwRPejLjK4iaNYyiajYq6pzpYQHhz3iaoj0ayhiaXNertD1ibJdZLLDIVbg/640)

下面来求解这个问题。还是拉格朗日乘子法：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDFvbqwRGA8xziaxIMPTNTavr5WzvaIJMXtWemNPXn7RDvTujCjuqraJg/640)

求导并令其为 0：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDEUWMjujLdmLtFZibYcNDTiaDm3iaNdReV5Itd5GLnXB2994COib8cuzQ6w/640)

得到了：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD5BvhxrnVSdbDdmlGEN4Mw7hSdGWaNm8NKJPHicfypibNN9BpyVxI7JPQ/640)

在大部分情况下，一些协方差矩阵的和![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDJBcnNHxdzf3DcoDlgZbV5nib3QFYgHHRoZYjuyiat1NolWxkSxE5qKxg/640)是可逆的。即使不可逆，上面这个也可以用广义特征值问题的方法来求解，但是这里方便起见我们认为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDJBcnNHxdzf3DcoDlgZbV5nib3QFYgHHRoZYjuyiat1NolWxkSxE5qKxg/640)可逆：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDAcXJqEiazAibvEUrbu9AG6B9JWPSXYsiaRv824aPvP9sGfVNH3c1zy9wQ/640)

我们只要对![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDgWMS2Mwnh9gEVRnFCtcmRJcTRGouHamZwWK8b357ribhVY4CgmopYWA/640)进行特征值分解，就可以得到 d 个特征向量了，挑出最大特征值对应的 c 个特征向量来组成 W，我们不就得到向 c 维子空间的投影了吗？

**真的是这样吗？**

**不是这样的。**诚然，我们可以选出 c 个特征向量，但是其中只有 1 个特征向量真正是我们想要的，另外 c-1 个没有意义。

观察：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDbxvFmEFFtlNcEk1fUaYVlUWBe8RUEeBz23iao3cunIaFyUReQ42FJdw/640)

发现了吗？等式右边的 (m1-m2) 是一个向量，换句话说，是一个秩为 1 的矩阵。那么，这个乘积的秩也不能大于 1，并且它不是 0 矩阵，所以：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDnskl1oGf17yumDf4OPVQ89cZaB7E4VibsicOVVJl0UB6BoAwoDib4pv3w/640)

**秩为 1 的矩阵只有 1 个非零特征值，也只有 1 个非零特征值对应的特征向量 w。**

可能有人会问了，那不是还有零特征值对应的特征向量吗，**用它们不行吗？**

**不行。**来看一下目标函数：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD1FeLGjPDw3x3SvehdftjgsH8VUh10avfb20kWibdcicg65q8opJ2K0uQ/640)

我们刚才得到的最优性条件：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDo0NFib81dmmfUlRWsicLNeXhO19iahQpe7zh3VLXKEmibz22NnBicibbQ6ibg/640)

所以目标函数为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDJicrr4jMtLz54vJhs2fKPslLQRsGibUKuiaX1f9GdNiaSuyBxTnQyibZefg/640)

而我们的 W 只能保证 λ1, λ2, ..., λd 中的一个非零，无论我们怎么选取剩下的 c-1 个 w，目标函数也不会再增大了，因为唯一一个非零特征值对应的特征向量已经被选走了。

**所以，两类 LDA 只能向一条直线投影。**

这里顺便解释一下 K 类 LDA 为什么只能投影到 K-1 维，其实道理是一样的。K 类 LDA 的类间散布矩阵是：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD7jCfCfsmSrhMJ1GN8QQuFolY8iaU4qVz9tVE8chuMvNsMibNwgfuiayiag/640)

可以看出，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDrpJCelwTvd2c0N1ibU8Z1tHFs8j9TdAI35RNgwa0fDoCB4mQibkGH6ag/640)是 K 个秩一矩阵![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDcRF5KvQ7ylicZ1TicQw2VsvgufkgnpT9WltlYkPpibia6lwPtic7bibMReTA/640)的和（因为 mk-m 是秩一的向量），所以它的秩最大为 K。并且![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDYjbZ8ptU7L1MjYZtBZt9HiaBFHrlQdPsrzImhUvBKkibACxianHTnlSyg/640)，所以这 K 项中有一项可以被线性表出。**所以，**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDrpJCelwTvd2c0N1ibU8Z1tHFs8j9TdAI35RNgwa0fDoCB4mQibkGH6ag/640)**的秩最大为 K-1**。也即：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD2osI4ELfW1P6O8mR7IsIGl9OxxuKD8qVZdOUWibDuD9NrLPdibyOHiaxg/640)

只有 K-1 个非零特征值。**所以，K 类 LDA 最高只能投影到 K-1 维。**

咦？刚才第三个问题怎么提的来着，可不可以不用比值用差值，用差值的话会不会解决这个投影维数的限制呢？

# 比值 & 差值

经典 LDA 的目标函数是投影后的类间散布与类内散布的比值，我们很自然地就会想，为什么非得用比值呢，差值有什么不妥吗？ 再试试不就完了。

注意，这一节我们不用向量 w，而使用矩阵 W 来讨论，这也就意味着我们实际上在同时讨论二类 LDA 和多类 LDA，只要把![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDrpJCelwTvd2c0N1ibU8Z1tHFs8j9TdAI35RNgwa0fDoCB4mQibkGH6ag/640)和![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDJBcnNHxdzf3DcoDlgZbV5nib3QFYgHHRoZYjuyiat1NolWxkSxE5qKxg/640)换成对应的就好了。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD5U99abdibTuQaasnWSqYtBowabMia8gicicbvaXo5oiaxkwMINaQDhFtCgA/640)

注意到可以通过放缩 W 来得到任意大的目标函数，所以我们要对 W 的规模进行限制，同时也进行秩限制：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD2dDVLIlicpr8SR1HqxoIZGbSGice9GwIoZoBu5T0uEg10ibCODkjC8PQg/640)

也就得到了差值版的 LDA：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDTM3x8BACib7BevuvR3UjWDHeC70bVFBWonAGlr7paFicOYgS1yGq85cg/640)

依然拉格朗日乘子法：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDVryby9ak61YWGoH7b7TnIIsrWFgkWnE9AXmw4VpiaiaicTR3ldWicX9m5Q/640)

求导并令其为 0：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD2NVD5PwV16R8WXRfmmY2tC77Wk3YLMkibSMZQuBgI9wFKwhIGb7qpibQ/640)

得到了：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDdplarQoFYv1ajHicm8aBvvibK0W5ugbhyhFWXdJKibNgziaHOH8icrb2ibdQ/640)

由![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD4pWriayqc53QdMgweLoHZU48lYPguz2sic75f2jAfKlib1ZPRkjpib0low/640)，有：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDbic0Py0wxlFlZLE1X4C6YvicWD53ffYl5Wn5kXIkHYy0iaLUnIvShTIxw/640)

可以得到：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD1O28hWLyTD0ogUdRibuuMQGF9CkuXEXeomyuTtwkldfwsvULkFhtaDg/640)

若括号内的东西可逆，则上式可以写为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD9ibT2z6mnQWz1ChKYUicRnEjhvf5qFH1FibJ0fNKpyjvbeAiaDibUsVanPQ/640)

注意到，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDrpJCelwTvd2c0N1ibU8Z1tHFs8j9TdAI35RNgwa0fDoCB4mQibkGH6ag/640)的秩不大于 K-1，说明等号左边的秩不大于 K-1，那么等号右边的秩也不大于 K-1，即：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD2u2nWTL8WcPSruuf3bjb1xqVARGFk6iaWiaX4HGtCRZKxpGRY8vsV7SA/640)

**所以我们还是会遇到秩不足，无法求出 K-1 个以上的非零特征值和对应的特征向量。**这样还不够，我们还需要证明的一点是，新的目标函数在零特征值对应的特征向量下依然不会增加。

目标函数（稍加变形）为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyD4laZYBMQ88hsCPY9MoxJQKs4CmB8HQUtqM27rX7uy5eOvB0bNI5SDQ/640)

再利用刚才我们得到的最优性条件（稍加变形）：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDcSiaOHcMoRlGs9nkAJ55CwBhvcx8hcaJsVGiaWPsLl74fg03pUkhCEuQ/640)

所以目标函数为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmiamia0nqdBJ8EI0emg1gdyDeq5hNlZH8ejpJ1ftDnfbEicy7NmAFulTDEXFVWWrViaxvCP6KHNf3kMg/640)

结论没有变化，**新的目标函数依然无法在零特征值的特征向量下增大****。**

综合**新矩阵依然秩不足**和**零特征值依然对新目标函数无贡献**这两点，我们可以得到一个结论：**用差值代替比值也是可以求解的，但是我们不会因此受益。**

既然比值和差值算出来的解性质都一样，那么为什么经典 LDA 采用的是比值而不是差值呢？

我个人认为，这可能是因为比值算出来的解还有别的直觉解释，而差值的可能没有，所以显得更优雅一些。什么直觉解释呢？

在二分类问题下，经典 LDA 是最小平方误差准则的一个特例。若让第一类的样本的输出值等于 N/N1 ，第二类样本的输出值等于 -N/N2 ， N 代表相应类样本的数量，然后用最小平方误差准则求解这个模型，得到的解恰好是（用比值的）LDA 的解。这个部分 PRML 上有讲解。

# 总结

这篇文章针对教科书上 LDA 的目标函数抛出了三个问题，并做了相应解答，在这个过程中一步一步深入理解 LDA。

**第一个问题：可不可以用均值之差而不是类间散布？**

答案：可以，这样做更符合直觉，并且更容易求解。但是采用类间散布的话可以把 LDA 的目标函数表达成广义瑞利商，并且上下齐次更加合理。可能是因为这些原因，经典 LDA 最终选择了类间散布。

**第二个问题：可不可以把 K 类 LDA 投影到大于 K-1 维的子空间中？**

答案：不可以，因为类间散布矩阵的秩不足。K 类 LDA 只能找到 K-1 个使目标函数增大的特征值对应的特征向量，即使选择了其他特征向量，我们也无法因此受益。

**第三个问题：可不可以用类间散布与类内散布的差值，而不是比值？**

答案：可以，在新准则下可以得到新的最优解，但是我们无法因此受益，K 类 LDA 还是只能投影到 K-1 维空间中。差值版 LDA 与比值版 LDA 相比还缺少了一个直觉解释，可能是因为这些原因，经典 LDA 最终选择了比值。

所以，教科书版 LDA 如此经典是有原因的，它在各个方面符合了人们的直觉，本文针对它提出的三个问题都没有充分的理由驳倒它，经典果然是经典。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看其他相关文章：**

[从无监督构建词库看「最小熵原理」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488802&idx=1&sn=eb35229374ee283d5c54d58ae277b9f0&chksm=96e9caa2a19e43b4f624eac3d56532cb9dc7ca017c9e0eaf96387e20e5f985e37da833fbddfd&scene=21#wechat_redirect)
[再谈最小熵原理：飞象过河之句模版和语言结构](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489564&idx=1&sn=a5d2191dd978e7917f06f66022ce8358&chksm=96e9c79ca19e4e8ac874f0fa5808b99b7323149d42a78fade690c6d60c0337ecabe043f616d1&scene=21#wechat_redirect)
[漫谈概率 PCA 和变分自编码器](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490939&idx=1&sn=7799ad3e2e864690c48c10fb3b919f37&chksm=96e9c2fba19e4bed434f875d65a7696b0a49896b719272926a94b182deb5d3752b5f07ac4332&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。


