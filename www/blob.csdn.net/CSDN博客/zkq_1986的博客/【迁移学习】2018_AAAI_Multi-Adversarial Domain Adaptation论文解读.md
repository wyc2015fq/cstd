# 【迁移学习】2018_AAAI_Multi-Adversarial Domain Adaptation论文解读 - zkq_1986的博客 - CSDN博客





2018年12月07日 15:05:36[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：105








paper: Multi-Adversarial Domain Adaptation

引文：Pei Z, Cao Z, Long M, et al. Multi-Adversarial Domain Adaptation[J]. AAAI, 2018.

# 1. 方法

本文主要解决多领域迁移问题。在多领域中，存在如下图的困难。即多个领域容易被杂糅在一起，或者领域映射发生错误，比如源域有猫、狗，目标域是狗、足球，那么很容易把源域的猫映射到狗上。



![](https://img-blog.csdnimg.cn/20181207150348248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

Figure 1: The difficulty of domain adaptation: discriminative structures may be mixed up or falsely aligned across domains. As an intuitive example, in this figure, the source class cat is falsely aligned with target class dog, making final classification wrong.



用单一的判别器，就很容易出现上述问题。因此，作者提出用多个判别器来负责相应领域的判断。具体地，先将每个数据点的领域概率分别计算出来，然后按概率赋到领域对应的判别器进行判断。有K个领域就对应K个判别器，每个判别器负责一个领域。

由于每个点被按概率赋到相关领域判别器判断，所以能够减少领域对齐出错的现象。

下图是多领域迁移学习的框架图。

![](https://img-blog.csdnimg.cn/20181207150346671.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181207150346733.png)



# 2. 实验

### 2.1 数据来源

**Office-31** comprising 4,652 images and 31 categories collected from three distinct domains: Amazon (A), which contains images downloaded from amazon.com, Webcam (W) and DSLR (D), which contain images respectively taken by web camera and digital SLR camera with different environments.

**ImageCLEF-DA** is a benchmark dataset for ImageCLEF 2014 domain adaptation challenge, which is organized by selecting the 12 common categories shared by the following three public datasets, each is considered as a domain: Caltech-256 (C), ImageNet ILSVRC 2012 (I), and Pascal VOC 2012 (P). The 12 common categories are aeroplane, bike, bird, boat, bottle, bus, car, dog, horse, monitor, motorbike, and people. There are 50 images in each category and 600 images in each domain.

## 2.2 实验结果

![](https://img-blog.csdnimg.cn/20181207150347183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20181207150347192.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

比前人的工作，提高约3%-5%。



# 3. 开源代码

https://github.com/thuml/MADA



