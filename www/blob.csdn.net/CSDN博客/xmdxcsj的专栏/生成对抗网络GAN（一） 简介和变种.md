# 生成对抗网络GAN（一） 简介和变种 - xmdxcsj的专栏 - CSDN博客





2017年11月12日 16:57:56[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1760








## 基本概念[1]

### 目标函数

[零和游戏（zero-sum game）](https://zh.wikipedia.org/wiki/零和博弈)
[纳什均衡](https://zh.wikipedia.org/wiki/納什均衡點)
[minimax算法](https://zh.wikipedia.org/wiki/极小化极大算法)

GAN借鉴了零和游戏的思想，引入生成网络和辨别网络，让两个网络互相博弈，当辨别网络不能辨别数据来自于真实分布还是生成网络的时候，此时的生成网络可以当做一个数据分布到另一个数据分布的转化器。 

假设生成网络$G$，辨别网络$D$，noise数据$z$，noise分布$p_z(z)$，data数据$x$，data分布$p_g$

目标函数： 


$min_{G}max_{D}V(D,G)=\mathbb E_{x \sim p_{data}(x)}[log D(x)]+\mathbb E_{z \sim p_{z}(z)}[log(1-D(G(z)))]$

max: 最大化辨别网络的辨别能力 

min: 相当于最大化把$G(z)$当成$x$的概率（以假乱真）
### 训练流程

![这里写图片描述](https://img-blog.csdn.net/20171112165656745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

实际参数更新如下图 
![这里写图片描述](https://img-blog.csdn.net/20171112165726154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 优缺点

优点： 

- 不需要马尔科夫链 

- 不需要对隐变量做推断
缺点： 

- $p_g(x)$没有显式的表达 

- $D$和$G$的训练需要同步，不好训练
## GAN种类

### DCGAN[2]

DCGAN(deep convolutional generative adversarial networks)主要的贡献是成功的将CNN用到GAN，然后可以使用训练好的GAN当做监督学习的特征提取器。 

对CNN结构的主要调整包括： 

- 不再使用pooling，discriminator和generator分别使用strided convolution和fractional-strided convolution 

- generator和discriminator使用batchnorm，对于训练深层的generator非常关键，generator的输出层和discriminator的输入层不使用batchnorm 

- 删除深层网络中的全连接层 

- generator的所有层的激活函数（最后一层使用Tanh）使用ReLU 

- discriminator所有层的激活函数使用LeakyReLU
### LS-GAN[3]

LS-GAN使用least squares作为loss function： 
![Alt text](./1495529121066.png)

a是fake data的label，b是real data的label 

相比于原始GAN的sigmoid的交叉熵，有两个优势： 

- 得到的分布更接近真实分布，效果更好（不仅考虑分类正确，还对距离远的施加惩罚） 

- 训练过程稳定，减弱了vanishing gradient的影响（对比两个函数曲线可知）
两组参数 

a=-1, b=1, c=0 

a=0, b=1, c=1
## 参考文献

[1].Generative Adversarial Nets 

[2].Unsupervised representation learning with deep convolutional generative adversarial networks. 

[3].Least Squares Generative Adversarial Networks










