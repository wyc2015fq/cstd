
# 讨论Markov Random Field和Gibbs Random Field心得 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月19日 09:50:55[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：56


Markov Random Field是属于high level的image analysis，比如给定一幅图像，经过分析可以得出结论：
1. 整个图像是属于马尔科夫分布的，即图像中的每个像素基本上都是只和邻近的像素有关系，受邻域外的像素影响很小，甚至可以忽略不计（在马尔科夫模型中），这是从整个图像的分布的角度上来考虑问题
2. Gibbs模型更关注于点与点(pixel)之间的数学关系，试图用一个数学表达式(energy function)表达出一个clique里的pixel之间的关系。从因果关系上讲，不知道可不可以这样假设：我们已经分析出来图像大都是符合马尔科夫分布，但由于马尔科夫在数学计算上的困难性，我们试图去构造出一个数学上简单计算的模型，这个模型中像素间有一定的关系。这时正好发现了Markov Random Field和Gibbs Random Field的等价性（需要学习这个等价性是从什么角度考虑的），于是everybody is happy.
2010-11-12  18:34:17
continued...
[https://blog.csdn.net/afgh2587849/article/details/6006286](https://blog.csdn.net/afgh2587849/article/details/6006286)

