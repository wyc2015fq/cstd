# 为什么我们更宠爱“随机”梯度下降？（SGD） - 知乎
# 

**特别声明：**本文仅有的一点贡献就是**用自己的理解翻译了** Leon Bottou 等人的论文 <Optimization Methods for Large-Scale Machine Learning>，初窥门径，才疏学浅，疏漏之处，望不吝指教**。**

%---------------------------------正文-------------------------------------------------

## 引子：

大家都知道，训练深度网络一般用的是 SGD （Stochastic Gradient Descent | 随机梯度下降）而不是 GD (Gradient Descent | 梯度下降)，但是有没有考虑过 SGD 为什么比 GD 更受大家宠爱，SGD 是如何在用较低的 Computational Complexity （一般可以大概理解成，达成目标需要计算 Gradient 的次数）的同时还能保证比较好的训练效果。

**本文主要给出几个特殊的例子**，给大家一个从直觉性，实验上和理论上认知，为什么有时候，相对于GD 我们更宠爱 SGD?

我们主要从以下三个方面，一起看一看 **SGD **相对于** GD **的优势。

Prat I: 直觉上  ( Intuitive Motivation)

Prat II: 实验上  ( Practical Motivation )

Prat III: 理论上 （Theoretical Motivation）

## **背景知识：**

我们一般想要优化的函数形式如下
![](https://pic1.zhimg.com/v2-7e613f3785afa43e186097640e4364e4_b.jpg)

我们知道**梯度下降**每一次迭代都需要出所有的梯度，即每一次算n个梯度，进行下面的迭代
![](https://pic2.zhimg.com/v2-c86a6c736ead1a170da96a5c238e503d_b.jpg)

而**随机梯度下降**，每一次选一个 ![f_{i}](https://www.zhihu.com/equation?tex=f_%7Bi%7D) 计算梯度，然后迭代
![](https://pic3.zhimg.com/v2-1abec5ad44cfb7cf1c66ba742c5f02ee_b.jpg)

## **Part I: 直觉上  ( Intuitive Motivation)**

**结论是：相对于非随机算法，SGD 能更有效的利用信息，特别是信息比较冗余的时候。**

考虑下面一个特殊情况，假设你要 ![\min_{x} \sum_{i=1}^{2n} f_i(x)](https://www.zhihu.com/equation?tex=%5Cmin_%7Bx%7D+%5Csum_%7Bi%3D1%7D%5E%7B2n%7D+f_i%28x%29) ，但是 ![f_1=f_2=\cdots= f_n](https://www.zhihu.com/equation?tex=f_1%3Df_2%3D%5Ccdots%3D+f_n) , ![f_{n+1} = f_{n+2}=\cdots=f_{2n}](https://www.zhihu.com/equation?tex=f_%7Bn%2B1%7D+%3D+f_%7Bn%2B2%7D%3D%5Ccdots%3Df_%7B2n%7D) , 这样的话其实你有很多项是冗余的，这样的话你 ![\min_{x} \sum_{i=1}^{2n} f_i(x)](https://www.zhihu.com/equation?tex=%5Cmin_%7Bx%7D+%5Csum_%7Bi%3D1%7D%5E%7B2n%7D+f_i%28x%29) 取得的最小值点 ![x^{\star}](https://www.zhihu.com/equation?tex=x%5E%7B%5Cstar%7D) , 其实和你 ![\min_{x} f_1(x) + f_{n+1}(x)](https://www.zhihu.com/equation?tex=%5Cmin_%7Bx%7D+f_1%28x%29+%2B+f_%7Bn%2B1%7D%28x%29) 确定的最小值点是一样的。 如果采取 GD 去迭代，你每一次要算 ![2n](https://www.zhihu.com/equation?tex=2n) 个梯度，但是如果用 SGD 其实就是每一次选一个计算梯度，因为 ![f_1=f_2=\cdots= f_n](https://www.zhihu.com/equation?tex=f_1%3Df_2%3D%5Ccdots%3D+f_n) ，SGD 算一个梯度的下降方向，就等于 GD 算十个得到的下降方向，所以无形之间，帮我们排除了很多冗余信息。

在实际中，可能没有这么巧合的情况，但是如果是在数据集特别特别大的情况下，虽然可能没有 ![f_1=f_2=\cdots= f_n](https://www.zhihu.com/equation?tex=f_1%3Df_2%3D%5Ccdots%3D+f_n) ，但是很有可能出现一些函数比较接近，也就是 ![f_1\approx f_2 \cdots \approx f_n ](https://www.zhihu.com/equation?tex=f_1%5Capprox+f_2+%5Ccdots+%5Capprox+f_n+) ,这样的话， SGD无形之间排除冗余信息的能力就又触发了。

## **Prat II: 实验上  ( Practical Motivation )**

**结论是：相对于非随机算法， SGD 在前期迭代效果卓越。**

![](https://pic3.zhimg.com/v2-132ba60f9a5d61e23c091fde63a77142_b.jpg)

我直接截论文上面的图，大家观赏一下

L-BFGS是一种需要算所有梯度，还要拟合Hessian 的一个优化算法.。不过既然是要算 full gradient,  大家直接理解成一种像 GD 一样的非随机的算法吧。 x 轴可以看成计算的梯度的数量，y轴可以看成是和真实最小值的误差。（个人理解哈，可能有偏差，大家会个意呗，想精确了解的，自己去看看原论文呗~~）

比较上图可得，随机算法 SGD 前期每个 iteration 找到的迭代点，可以显著的接近最小值点。

**这里又有一个特别好玩的小例子来解释为什么 SGD 前期表现好，后期就水了现象。**

**这是我最想翻译的部分！！其他可以跳过，这里要认真听了哈~~。**

假设我们需要优化的函数全部是二次函数，形式如下

![](https://pic3.zhimg.com/v2-92fc6b885cbc90ccfc20834788576a66_b.jpg)

其中 ![a_i](https://www.zhihu.com/equation?tex=a_i) 是常数，并且满足
![](https://pic1.zhimg.com/v2-869c589b6f1974d23464ddfd95d098f0_b.jpg)

我们求导，令导数等于零得到
![](https://pic3.zhimg.com/v2-17b1cf51b0131403345c5ad782bb0caa_b.jpg)
其实这就是一堆sample, 在平方距离公式下，离他们最近的点就是他们的均值。

结合我们的假设公式（2.2）我们得到，最小值点在0处，也就是
![](https://pic2.zhimg.com/v2-f36e42ee6c627293d635d524827fdc3d_b.jpg)
所以函数的最小值点在0处。我们现在看看 SGD 的表现，假设我们最开始的初始点在最左边，**然后无论你选到那个二次函数的分支，沿着梯度，都能向靠近最小值点的方向移动。所以SGD 前期效率很高。**

我们假设选中最左边那个 ![f_1](https://www.zhihu.com/equation?tex=f_1) ，举个例子。

![](https://pic4.zhimg.com/v2-241ef9c6033341d4b6cf3612fbfa2747_b.jpg)

而到了后期，我们的 ![x_t](https://www.zhihu.com/equation?tex=x_t) 足够靠近我们的最小值点 ![x^{\star}](https://www.zhihu.com/equation?tex=x%5E%7B%5Cstar%7D) ，那么单纯靠选择一个二次函数的导数方向移动，便不能达到很好的效果，时而远离最小值点 **“0”**，时而接近最小值点，我们的前进方向开始**犹豫徘徊**，这时候便需要计算更多的梯度信息来进行决策了。你看下图，**选 ![f_2](https://www.zhihu.com/equation?tex=f_2) 会背离，选 ![f_3](https://www.zhihu.com/equation?tex=f_3) 会接近，**就如同诗经《蒹葭》里面描述你追求你心爱的姑娘一样，溯洄从之，宛在水中央。 溯游从之，宛在水中坻。 **朦朦胧胧不知方向，忽近忽远~~。这个时候你需要更多的信息了。**

![](https://pic1.zhimg.com/v2-944b4bb792a1f019a113d3c48d2bb0cc_b.jpg)
多好的例子啊！！！奇美无比~~~。

我突然意识到，随机梯度下降怎么和与女生相处那么相似。刚开始，离的远的时候，不经意间便能产生好感，慢慢靠近。等靠的近了，好感想转换成爱情的时候，便开始茫然失措，不知方向，忽远忽近~~。

谢谢 [@王赟 Maigo](https://www.zhihu.com/people/0c708d923f8fb00142e1f25cdd4951d9) 提出的问题，忽近忽远后怎么办？ 

**所以对于随机梯度下降，我其实不太清楚如何解决忽近忽远，但是如果想随机梯度下降收敛的比较好，我知道的方法有，一是采取递减的 stepsize，原理是什么我还没搞清楚，二就是 Variance reduction 的一些方法吧，比如采用更多的样本算梯度，或者用SAGA, SVRG这种方法吧。个人猜测，这些做法是不是为了解决忽近忽远的问题，其实我不太清楚，当个讨论题，希望有大牛能帮我们解答一下。**

## Prat III: 理论上 （Theoretical Motivation）

**结论是：如果样本数量大，那么 SGD的Computational Complexity 依然有优势。**

我们知道， GD 在强凸的情况下收敛速度是 Linear Convergence, 最差的情况下，至少需要迭代 ![O\bigg(\log(\frac{1}{\epsilon})\bigg)](https://www.zhihu.com/equation?tex=O%5Cbigg%28%5Clog%28%5Cfrac%7B1%7D%7B%5Cepsilon%7D%29%5Cbigg%29) 次，才能达到 ![\|\sum_{i=1}^{n} f_i(x_t) - f^{\star} \| \leqslant \epsilon](https://www.zhihu.com/equation?tex=%5C%7C%5Csum_%7Bi%3D1%7D%5E%7Bn%7D+f_i%28x_t%29+-+f%5E%7B%5Cstar%7D+%5C%7C+%5Cleqslant+%5Cepsilon) 的精度，加上 GD 每一次需要计算 n 个梯度，所以总的 Computational Complexity 是 ![O\bigg(n \log(\frac{1}{\epsilon})\bigg)](https://www.zhihu.com/equation?tex=O%5Cbigg%28n+%5Clog%28%5Cfrac%7B1%7D%7B%5Cepsilon%7D%29%5Cbigg%29)![O\bigg(n \log(\frac{1}{\epsilon})\bigg)](https://www.zhihu.com/equation?tex=O%5Cbigg%28n+%5Clog%28%5Cfrac%7B1%7D%7B%5Cepsilon%7D%29%5Cbigg%29) 。

至于 SGD， 结论是 为了达到 ![\mathbb{E} \bigg[\|\sum_{i=1}^{n} f_i(x_t) - f^{\star} \| \bigg] \leqslant \epsilon](https://www.zhihu.com/equation?tex=%5Cmathbb%7BE%7D+%5Cbigg%5B%5C%7C%5Csum_%7Bi%3D1%7D%5E%7Bn%7D+f_i%28x_t%29+-+f%5E%7B%5Cstar%7D+%5C%7C+%5Cbigg%5D+%5Cleqslant+%5Cepsilon) , 在最差的情况下，我们需要迭代次数是 ![O\bigg( \frac{1}{\epsilon} \bigg)](https://www.zhihu.com/equation?tex=O%5Cbigg%28+%5Cfrac%7B1%7D%7B%5Cepsilon%7D+%5Cbigg%29) ，但是因为 SGD 每一次就算一个梯度，所以它的 Computational Complexity 也是 ![O\bigg( \frac{1}{\epsilon} \bigg)](https://www.zhihu.com/equation?tex=O%5Cbigg%28+%5Cfrac%7B1%7D%7B%5Cepsilon%7D+%5Cbigg%29) 。这些复杂度的证明我看看以后有时间写一下好了，当个小练习，但是不是今天的重点，故略去。

放在表格里面就是这样子的了
![](https://pic3.zhimg.com/v2-8999340d3b8d6376584256fadb722c2a_b.jpg)

对比与 GD 的 ![O\bigg(n \log(\frac{1}{\epsilon})\bigg)](https://www.zhihu.com/equation?tex=O%5Cbigg%28n+%5Clog%28%5Cfrac%7B1%7D%7B%5Cepsilon%7D%29%5Cbigg%29) ，SGD 的 ![O\bigg( \frac{1}{\epsilon} \bigg)](https://www.zhihu.com/equation?tex=O%5Cbigg%28+%5Cfrac%7B1%7D%7B%5Cepsilon%7D+%5Cbigg%29) 受所需的精度 ![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon) 的影响较大, 但是如果是在 Large Large Data 的设定下，n 会特别特别大， 这样 SGD 的Computational Complexity 依然是可以小于 GD 的Computational Complexity，换就换就是，如果n特别大，那么 SGD 在Computational Complexity 上依然有优势

## 总结：

好了总结一下， SGD 相比与 GD 优势如下：

**Prat I: 相对于非随机算法，SGD 能更有效的利用信息，特别是信息比较冗余的时候。**

**Prat II:****相对于非随机算法， SGD 在前期迭代效果卓越。**

**Prat III:****如果样本数量大，那么 SGD的Computational Complexity 依然有优势。 **

**以上三条，吹牛必背。**

**%---------------------------------------------------------------------------------------------**

**你说大公司面试会不会问到随机梯度的优势？你如用这文章成功的秀了一把，记得回来给我打赏，哈哈。**

谢谢你看到最后，但愿这些对你有帮助，有启发的话，**点个赞呗**，只收藏不点赞鼓励一下，人家都没有写下去的热情了。

最后，你如果热情不减，还想继续了解优化算法，欢迎关注**我**[@未名](https://www.zhihu.com/people/08b0dcf08389c7e2ca1676442ef45079)和**我的专栏 **[非凸优化学习之路 - 知乎专栏](https://zhuanlan.zhihu.com/optimization)。。。**我会写写自己学习凸\非凸优化的心得**，大家一起学习交流。

专栏文章大概有：

第一篇文章是： [从Nesterov的角度看：我们为什么要研究凸优化？ - 知乎专栏](https://zhuanlan.zhihu.com/p/27435669)

第二篇文章是：[非凸优化基石：Lipschitz Condition - 知乎专栏](https://zhuanlan.zhihu.com/p/27554191)

第三篇文章是：[当我们谈论收敛速度时，我们都在谈什么？ - 知乎专栏](https://zhuanlan.zhihu.com/p/27644403)

%-------------------------------------------------------------------------------------------------------

对了最近在追剧 《春风十里不如你》，更新会更慢了，还有《冰与火之歌》第七季，更新会更更慢了，别怪我，怪这些个电视剧吧~~~

参考文献：

【1】 Leon Bottou 等人的论文  <Optimization Methods for Large-Scale Machine Learning>

