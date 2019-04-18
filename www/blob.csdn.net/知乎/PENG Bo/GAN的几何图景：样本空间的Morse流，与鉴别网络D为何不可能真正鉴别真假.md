# GAN的几何图景：样本空间的Morse流，与鉴别网络D为何不可能真正鉴别真假 - 知乎
# 



目前对于GAN的解释，大多来自于统计分布和K-L散度等等，只适合toy model。

我认为，不应该从统计分布的角度讨论图像。正如不应该用CE loss判断两幅图像的相似程度。

在此我从几何的角度讨论GAN到底在干什么，以及D网络的一些特点。

## ***一、GAN的特点***

GAN的训练中有某些现象值得注意：

**1. 对于任何非toy model的模型，D并没有真正的“鉴别真假”的作用。**

每batch生成的样本中，D分数最高和最低的，人眼很难看出质量有优劣区别。因此，所有GAN论文都不会输出"按D分数排序"的结果。

如果D真能在每个batch鉴别真假，那就太简单了！只挑D分数最高的那些作为batch的输出，就可以生成大量很真实的图片。可惜世界上没有这么好的事情。
![](https://pic2.zhimg.com/v2-d38ecbbb0c42fdd415b4aeda6829f189_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='572'></svg>)
例如，上图来自于同一个batch，按D分数排列，左上是D最高分的，右下是D最低分的，是否有质量区别？可能在某些地方有一点点，但非常不明显。

后文会说为什么D不可能有效鉴别真假。

2. 看一个视频，对于64个固定的代码 z ，在训练过程中，生成的图像的演变：

完整视频：[DCGAN的训练](https://link.zhihu.com/?target=http%3A//v.youku.com/v_show/id_XMjk3NzE0NTA3Ng%3D%3D.html%3Fspm%3Da2h3j.8428770.3416059.1)



![](https://pic1.zhimg.com/v2-600f14ffbdf9f53c2fe7fb134b6922b0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='384' height='384'></svg>)



在每个batch，GAN生成的所有图像，都有非常统一的风格变化。

**我们会发现，每个batch时， D实际只专注于识别某些小细节**（例：眼睛的某个地方是不是没画好；这种事情深度卷积网络特别擅长）**，因此G在每个batch只会为了对抗这些小细节而改变**（例：同时改变所有眼睛的绘画方式；这种事情深度卷积网络特别擅长）。

同时，请注意一开始 G 对于不同代码 z 生成的样本很类似，后来就慢慢找到各自演化的目标（视频中到后来很明显）。

还有很多现象，我们在后文讨论。




## ***二、GAN的几何图景***

1. 我用一个简单直接的几何图景，描述GAN中实际在发生什么：
![](https://pic1.zhimg.com/v2-2dfc8497d0d5481ab205e91dda648394_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1250' height='605'></svg>)


![](https://pic3.zhimg.com/v2-31a44e082f8b6c868688adb53c9b6236_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1090' height='942'></svg>)
2. 补充一下：**如果D对于其它东西也等于1，那么沿着D走不一定会到达 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) ，也可能到达其它东西。由于 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) 的维数远小于 K-1（即，D的正常pre-image的维数），这是非常有可能的事情。**

因此D不可能真正鉴别真假，除非能把K-1维压缩到样本流形的维数，而这难度太大了，除非手工构造一些比较丑陋的模型去比较一堆代码。

目前的GAN是在不断训练中逐渐完成这个任务，有点像在找不同D的交集。

写到这里我感觉这种构造可用于证明某些数学问题，因为这个perturb G的方法较为有趣。

此外可见，**D的真正作用是提供G的演变梯度。D给出的数值是多少不重要，重要的是这个数值在求导后是否能给G一个好的梯度。**

3. 不妨看个更简化的图，看"沿着D走"的情景：
![](https://pic3.zhimg.com/v2-3a82759009ff35c93534e0087b106d3a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='999' height='527'></svg>)
这里我们把 ![\mathbb{R}^K](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5EK) 简化为 1 维的，于是 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) 就简化为离散的点。那么 M 沿着 D 走，就是往它最靠近的 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) 走的过程。

所以最初的M应该尽量分散，这样就更可能最终到达不同的 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) 。

注意，对于那些起初在“山顶”的M，就面临着抉择问题，要往左边还是右边的 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) 走？**这就是一种自发对称性破碎。**我们在观察 G 生成的样本时，也会发现这个抉择过程。对于高维的情况，这是很普遍的。




## ***三、GAN的实际训练***

具体的训练过程，还有两点区别：
- 我们的训练样本数量有限，只能代表 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) 的极小一点部分。
- 而且，我们的实际训练是用mini-batch的方法。

因此，实际训练过程，更接近于这样，每次把 M 的一个局部沿着 D 提起来一点：
![](https://pic1.zhimg.com/v2-f395fc06926dbe47f926764dfb70f180_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='560' height='692'></svg>)
其实我之前还想，由于 ![\mathbb{R}^K](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5EK) 的维数太大了，是否不能指望 D 可以一次提动所有维，提动整个 M？

不过，画出来才发现，D 每次基本都会提动所有地方，除非刚好遇到 index 不好的情况。**也许正是因为如此，所以GAN才能收敛这么快。**

剩下的主要障碍，就是此前说的，D等于1的地方太大了，远远比 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) 大。

总之，这里也许可以算出某些漂亮的关于训练方法的结果。

随着batch的进行，我们不断地每次提一个局部，就是像这样：
![](https://pic3.zhimg.com/v2-966487a885a7ddfe3bfa574efad4aa36_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='581' height='686'></svg>)
最终，在绝大多数情况下， M 也会慢慢被逐渐拉起来，往 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) 靠近。不过，这个方法无疑也有点“千疮百孔”的感觉。

样本数据的覆盖度，在此尤为重要。如果样本可以很好地覆盖 ![\tilde{M}](https://www.zhihu.com/equation?tex=%5Ctilde%7BM%7D) ，效果就会好很多。




## ***四、后续***

我在想的问题是，如何将 Auto-encoder 与上述图景结合考虑。 Auto-encoder 基本上是 dual 的过程，但有微妙区别，还得想想。

GAN和AE都不依赖于标签，只依赖于数据本身，所以都是非常本质的。

**我发现，在训练过程中，AE提供给G的梯度，和D提供给G的梯度，往往是前者越来越小，后者越来越大，而且几何中间值非常稳定，而且与数据集无关。**这到底是什么原因，确实会非常有趣。

各位有什么看法和意见，也欢迎讨论，谢谢。


