# 超分辨率分析（四）--Deep Image Prior - 工作笔记 - CSDN博客





2019年03月06日 20:09:15[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：1029
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









转自：[https://zhuanlan.zhihu.com/p/34504284](https://zhuanlan.zhihu.com/p/34504284) 略删减，我怎么感觉这篇文章作者都没理解网络学习这个事情的本质。

**本文中作者展示生成网络在经过任何学习之前就能够捕获大量的低级图像统计信息，也就是说，这些信息可能并不是通过大量的数据集学习得来。文中具体的实验方法，就是用一个随机初始化的生成网络，仅通过给定的图像就能得到重建后的图像，这种方法在去噪、超分辨、修补等人物上都有优异的表现。它也连接了两个非常流行的图像重建方法派别：基于学习的方法和基于非学习方法（例如self-similarity）。**

本文中提出的Deep image prior方法可用于降噪、超分辨率、图像修复等领域，具体的见下面的例子

深度卷积神经网络（ConvNets）在图像去噪、超分辨等重建任务上达到了 state-of-the-art。相似结构的ConvNets更普遍地用在生成图像上，例如GAN、variational autoencoders、direct pixelwise error minimization。

这些 ConvNets 几乎都是基于大量图像数据集的训练，因此有一种假设说他们优异的表现由于它们从数据中学习真实图像先验的能力。然而单单学习还不足以解释深度网络的优异表现。例如，在文章 [Understanding deep learning requires rethinking generalization] 中，即使将标签随机打乱，同样泛化性能好的图像分类网络也可以很好地拟合这些数据。因此，泛化要求网络结构与数据结构“共鸣”。然而它们相互作用的机理，尤其是图像生成的，还尚不清楚。

在本文中，作者展示了一个与期望相反的现象，大量图像统计信息是由卷积生成网络的结构捕获的，而非任何学习能力。这对于解决各种图像恢复问题所需的图像统计信息尤其如此，在这些图像恢复问题中，我们需要图像先验来整理退化过程中丢失的信息。

为了展示这些，作者使用一个未训练的网络，来解决上述的重建问题，做法是让这个网络去拟合该张退化的图像，再无别的数据。在这个框架中，网络的权重就像是恢复的图像的参数化。给定一个退化的图像和对应的观测模型，网络的权重随机初始化，并且被拟合以最大化它们的似然性。

这个简单的构想在图像重建任务重很具竞争性。网络中没有任何一层面是从数据学习来的，而且网络的权重总是随机初始化，因此唯一的先验信息就是网络结构本身。这可能是第一次直接研究由卷积生成网络捕获的先验，而不依赖于从图像学习网络参数。
- Denoising and generic reconstruction

![](https://img-blog.csdnimg.cn/20190306234319675.jpg)

给定一个目标图片 ![x_0](https://www.zhihu.com/equation?tex=x_0) 和一个随机向量 ![z](https://www.zhihu.com/equation?tex=z) ,通过一个生成网络 ![f_\theta(z)](https://www.zhihu.com/equation?tex=f_%5Ctheta%28z%29) 生成接近于目标图片 ![x_0](https://www.zhihu.com/equation?tex=x_0) 的图片。我们可以通过以下损失函数来优化：

![\min_\theta\|f_\theta(z)-x_0\|^2](https://www.zhihu.com/equation?tex=%5Cmin_%5Ctheta%5C%7Cf_%5Ctheta%28z%29-x_0%5C%7C%5E2)

如上图所示，我们希望通过损失函数来优化生成目标图片（corrupted），虽然在50K次迭代之后我们得到了非常接近于目标图片的结果，但是在2400次迭代的时候，模型意外的得到了一个更好的结果。这表明对于一张加噪后的图片 ![x_0 = x + \epsilon](https://www.zhihu.com/equation?tex=x_0+%3D+x+%2B+%5Cepsilon) ，生成网络在生成 ![x_0](https://www.zhihu.com/equation?tex=x_0) 之前，会先生成没有噪声的自然图片 ![x](https://www.zhihu.com/equation?tex=x) ，然后在自然图片分布之上模拟出加噪后的效果。
- Super-resolution

![](https://img-blog.csdnimg.cn/2019030623434026.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9lYXNvbi5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)





![E(x， x_0)=\|d(x) - x_0\|^2](https://www.zhihu.com/equation?tex=E%28x%EF%BC%8C+x_0%29%3D%5C%7Cd%28x%29+-+x_0%5C%7C%5E2)

其中 ![d(\cdot)](https://www.zhihu.com/equation?tex=d%28%5Ccdot%29) 为一个降采样操作： ![R^{3\times tH \times tW}\to R^{3\times H \times W}](https://www.zhihu.com/equation?tex=R%5E%7B3%5Ctimes+tH+%5Ctimes+tW%7D%5Cto+R%5E%7B3%5Ctimes+H+%5Ctimes+W%7D) ，损失函数的作用是找到一个超分辨率的图片，然后降采样得到一张和 ![x_0](https://www.zhihu.com/equation?tex=x_0) 相同的图片。实现如下:

![E(x, x_0) = \|d(f_\theta(z)) - x\|^2](https://www.zhihu.com/equation?tex=E%28x%2C+x_0%29+%3D+%5C%7Cd%28f_%5Ctheta%28z%29%29+-+x%5C%7C%5E2)

在迭代的过程中，生成模型会先生成超分辨率图片 ![x](https://www.zhihu.com/equation?tex=x) （ ![3\times tH\times tW](https://www.zhihu.com/equation?tex=3%5Ctimes+tH%5Ctimes+tW) ），然后对 ![x](https://www.zhihu.com/equation?tex=x) 简单的降采样操作 ![d(\cdot)](https://www.zhihu.com/equation?tex=d%28%5Ccdot%29) （downsample）.
- Inpainting

对于图像修复的应用，采用的能量损失函数如下：

![E(x, x_0) = \|(x-x_0)\odot m\|^2](https://www.zhihu.com/equation?tex=E%28x%2C+x_0%29+%3D+%5C%7C%28x-x_0%29%5Codot+m%5C%7C%5E2)

同样地， ![x=f_\theta(z)](https://www.zhihu.com/equation?tex=x%3Df_%5Ctheta%28z%29)

![](https://img-blog.csdnimg.cn/20190306234403565.jpg)

对于这些应用（降噪、超分辨率、图像修复），可以总结为一句话：**生成网络在生成非自然图像之前，会先生成相应的自然图像，然后在自然图像上构造该不自然图像**。关于这篇文章的题目Deep Image Prior，我的翻译是“深度图像先验”，传统的针对这些应用的方法，无不是通过大量的数据进行训练，得到的结果自然是基于这些训练数据的先验结果。但是本文中的模型不需要大量的数据进行计算，**唯一的先验就是深度模型随机初始化的参数和深度结构本身**——深度图像先验。



