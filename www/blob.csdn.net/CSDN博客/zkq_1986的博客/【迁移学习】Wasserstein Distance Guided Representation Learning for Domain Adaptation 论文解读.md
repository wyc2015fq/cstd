# 【迁移学习】Wasserstein Distance Guided Representation Learning for Domain Adaptation 论文解读 - zkq_1986的博客 - CSDN博客





2018年12月10日 11:42:40[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：152








paper：Wasserstein Distance Guided Representation Learning for Domain Adaptation

Shen J, Qu Y, Zhang W, et al. Wasserstein Distance Guided Representation Learning for Domain Adaptation[J]. AAAI-2017.

# 1. 方法

本文提出用Wasserstein距离来衡量领域之间的差异。

![](https://img-blog.csdnimg.cn/20181210113514213.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

图1 本文的迁移学习结构图



![https://github.com/RockySJ/WDGRL/raw/master/toy/toy_visualization.png](https://github.com/RockySJ/WDGRL/raw/master/toy/toy_visualization.png)

# 2 实验

## 2.1 数据来源

**Amazon review benchmark dataset. **The Amazon review dataset is one of the most widely used benchmarks for domain adaptation and sentiment analysis. It is collected from product reviews from Amazon.com and contains four types (domains), namely books (B), DVDs (D), electronics (E) and kitchen appliances (K). For each domain, there are 2,000 labeled reviews and approximately 4,000 unlabeled reviews (varying slightly across domains) and the classes are balanced. In our experiments, for easy computation, we follow (Chen et al. 2012) to use the 5,000 most frequent terms of unigrams and bigrams as the input and totally A24 = 12 adaptation tasks are constructed.

**Office-Caltech object recognition dataset. **The Office-Caltech dataset released by (Gong et al. 2012) is comprised of 10 common categories shared by the Office-31 and Caltech-256 datasets. In our experiments, we construct 12 tasks across 4 domains: Amazon (A), Webcam (W), DSLR (D) and Caltech (C), with 958, 295, 157 and 1,123 image samples respectively. In our experiments, Decaf features are used as the input. Decaf features (Donahue et al. 2014) are the 4096-dimensional FC7-layer hidden activations extracted by the deep convolutional neural network AlexNet.

## 2.2 实验结果

![](https://img-blog.csdnimg.cn/2018121011351584.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

比前不迁移，提高约5%；比前人的工作，提高约0.3%。

![](https://img-blog.csdnimg.cn/20181210113515422.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

比前不迁移，提高约7%；比前人的工作，提高约0.7%。

# 3. 开源代码

https://github.com/RockySJ/WDGRL



