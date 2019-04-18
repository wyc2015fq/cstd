# 生成模型--损失函数改进之WGAN系列 - wydbyxr的博客 - CSDN博客
2018年10月26日 10:32:58[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：381
# WGAN系列
  1）WGAN 就是在 GAN 的基础上改进的比较成功的一种。WGAN 利用 Wasserstein 距离代替 JS 距离，从而避免了一定的梯度消失问题，这也为很多 GAN 模型的扩展提供了指导。
  2）WGAN-GP 是在 WGAN 的基础上改进的，WGAN 在保证 Lipschitz 限制条件时，作者是采用将限制判别器神经网络的所有参数（这里指权重 w）不超过某个范围 [-c,c]。这样做带来了参数上的两极化，为了改进 WGAN，WGAN-GP 提出了梯度惩罚概念用于满足 Lipschitz 限制条件。
  3）Curriculum GANs 其是在 WGAN-GP 的基础上改进的，但是思路是沿袭 WGAN 所以也称之为 WGAN-C。
# Wasserstein GAN(WGAN)
  Facebook发表的论文。
  参考资料：
[https://www.leiphone.com/news/201702/EU12GVIe2pNGtgEk.html?viewType=weixin](https://www.leiphone.com/news/201702/EU12GVIe2pNGtgEk.html?viewType=weixin)
[http://www.dataguru.cn/article-10570-1.html](http://www.dataguru.cn/article-10570-1.html)
[https://zhuanlan.zhihu.com/p/25071913](https://zhuanlan.zhihu.com/p/25071913)
## Wassertein距离
  Wassertein 指的是Wassertein距离(也称Earth-Mover（EM）距离)，WGAN就是用Wasserstein距离代替JS散度。
  Wasserstein距离又叫Earth-Mover距离(EM距离)，用于衡量两个分布之间的距离。
  定义：是和分布组合起来的所有可能的联合分布的集合。对于每一个可能的联合分布，可以从中采样得到一个样本和，并计算出这对样本的距离，所以可以计算该联合分布下，样本对距离的期望值。在所有可能的联合分布中能够对这个期望值取到的下界就是Wasserstein距离。
  直观理解：在这个路径规划下把土堆挪到土堆所需要的消耗。而Wasserstein距离就是在最优路径规划下的最小消耗。所以Wesserstein距离又叫Earth-Mover距离。
  优点：Wessertein距离相比KL散度和JS散度的优势在于，即使两个分布的支撑集没有重叠或者重叠非常少，仍然能反映两个分布的远近。而js散度在此情况下是常量，KL散度可能无意义。
  根据Kantorovich-Rubinstein对偶原理，可以得到Wasserstein距离的等价形式。
## WGAN的突破性
  有以下几个爆点：
  以上一切好处不需要精心设计的网络架构，最简单的多层全连接网络就可以做到。
  1）彻底解决GAN训练不稳定的问题，不再需要小心平衡生成器和判别器的训练程度
  2）基本解决了collapse mode的问题，确保了生成样本的多样性
  3）训练过程中终于有一个像交叉熵、准确率这样的数值来指示训练的进程，这个数值越小代表GAN训练得越好，代表生成器产生的图像质量越高。
## 实际操作的改变
  改进后相比原始GAN的算法实现流程却只改了四点，从代码上来看只需要在原始的GAN上简单的修改。
  1）判别器最后一层去掉sigmoid
  2）生成器和判别器的loss不取log
  3）每次更新判别器的参数之后把它们的绝对值截断到不超过一个固定常数c
  4）不要用基于动量的优化算法（包括momentum和Adam），推荐RMSProp，SGD也行
  另外，在WGAN中有 weight clipping。
## 问题：如果用L1距离的话，就是WGAN，那这样生成模型和判别模型就没有那种解析关系了吧？
  回答：对的，用L1的情况下，没有那么完美的关系，用L2是存在的。那么对于大多数应用的话，用L1，L2没有本质差别。
# WGAN-GP
  再度改进WGAN
  改进后的 WGAN-GP 中更换为了梯度惩罚 gradient penalty，判别器参数就能够学到合理的参数取值，从而显著提高训练速度，解决了原始WGAN收敛缓慢的问题，在实验中还第一次成功做到了“纯粹的”的文本GAN训练。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181026103229264.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_27,color_FFFFFF,t_70)
