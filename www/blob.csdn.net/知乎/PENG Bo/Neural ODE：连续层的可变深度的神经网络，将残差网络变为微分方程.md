# Neural ODE：连续层的可变深度的神经网络，将残差网络变为微分方程 - 知乎
# 



去年我写的一个idea，残差网络可以连续化变成微分方程：
[PENG Bo：通往无限层神经网络 (2)：一个富迭代性的微分方程，与几个小实验​zhuanlan.zhihu.com![图标](https://pic4.zhimg.com/v2-c0e3e874cd88c5b741a8a8849e24ebeb_180x120.jpg)](https://zhuanlan.zhihu.com/p/26613796)
现在总算有人做了：
[https://arxiv.org/pdf/1806.07366.pdf​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1806.07366.pdf)![](https://pic3.zhimg.com/v2-9edc8fadc506afaa5840954f0a71c85e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='649' height='941'></svg>)
## **个人的看法是，还是用这个思路来做卷积网络吧。例如，直观的办法：**
- **构建出flow，把图像中的物体流到canonical的姿态上（canonical的位置，大小，角度，各个部位在canonical的位置，大小，角度）。**
- **或者（或者与此同时），像传统网络一样，把激活的区域流到与特征相关的位置上。**
- **然后根据flow的方法和flow后的结果做事情。这个的优点是容易让人类理解。**

## **有兴趣做的同学可以留个言。**

最近还有一篇文章，认为神经网络就是多项式：
[https://arxiv.org/pdf/1806.06850.pdf​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1806.06850.pdf)
这个其实在 **[Wh](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1608.08225)**y does deep and cheap learning work so well? 已经说过了，适合 toy model + toy data。

这些都可以结合，例如flow就是在identity附近做微扰展开，就是多项式级数。

目前这些替代模型的主要问题都是执行速度慢，不实用。我现在认为，我们应该找与完全不同的硬件架构结合更紧密的方法，才能实现真正超越深度网络。这个下次继续说。

**从前还在知乎写过一个每个神经元用不同的学习速率而且有大致的公式，闲的同学可以去试一试在具体训练的做法，例如先把学习速率在不同层有weight（也许还可以自动学会这个weight，因为也可以变成可训练的参数）。现在的整个网络用单个学习速率的做法必定是有改善空间的。**


