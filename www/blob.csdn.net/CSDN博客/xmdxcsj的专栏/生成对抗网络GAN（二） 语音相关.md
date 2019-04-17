# 生成对抗网络GAN（二） 语音相关 - xmdxcsj的专栏 - CSDN博客





2017年11月12日 16:59:33[xmucas](https://me.csdn.net/xmdxcsj)阅读数：4531








## 多任务对抗学习[1]

![这里写图片描述](https://img-blog.csdn.net/20171112165853291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

为了获得对噪音的鲁棒性，引入多任务学习，分为三个网络： 

- 输入网络（绿色），用作特征提取器 

- senone输出网络（红色），用作senone分类 

- domain输出网络（蓝色），domain这里指噪音的类型，总共17种噪声
为了增加对噪音的鲁棒性，增加了GRL层（gradient reversal layer），网络在反向传播的时候，对于domain网络过来的梯度取了$-\alpha$，也就是增加噪音分类的错误率，以便获得senone-discriminative domain-invariant的特征。 

[2]和[1]的思想类似。

## SEGAN[3]

主要用来做语音增强（比如降噪）等。 

结合conditional GAN和LSGAN，使用$L_1$norm，最后的loss如下： 


$min_{D}V_{LSGAN}(D)=\frac{1}{2}\mathbb E_{x \sim p_{data}(x,x_c)}[(D(x,x_c)-1)^2]+\frac{1}{2}\mathbb E_{x_c \sim p_{data}(x_c), z\sim p_{z}(z)}[D(G(z,x_c))^2]$


$min_{G}V_{LSGAN}(G)=\frac{1}{2}\mathbb E_{x \sim p_{data}(x_c), z\sim p_{z}(z)}[(D(G(z,x_c))-1)^2]+\lambda \| G(z, \tilde{x})-x\|_1$

一些参数的含义如下： 
$x$: noise speech 
$x_c$: clean speech 
$z$: 服从正态分布的noise samples
训练流程如下： 
![这里写图片描述](https://img-blog.csdn.net/20171112165919207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

训练的时候需要clean speech和noisy speech的pair，以保证在去除噪声的同时保留原始语音的信息。
## 参考文献

[1].Adversarial Multi-task Learning of Deep Neural Networks for Robust Speech Recognition 

[2].Invariant Representations for Noisy Speech Recognition 

[3].SEGAN: Speech Enhancement Generative Adversarial Network






