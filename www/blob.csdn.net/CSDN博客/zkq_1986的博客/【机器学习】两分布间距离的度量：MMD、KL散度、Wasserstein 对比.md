# 【机器学习】两分布间距离的度量：MMD、KL散度、Wasserstein 对比 - zkq_1986的博客 - CSDN博客





2019年01月10日 15:00:31[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：353








MMD：最大均值差异

Wasserstein距离[1]


- 实验
- 数据来源

**Amazon review benchmark dataset. **The Amazon review dataset is one of the most widely used benchmarks for domain adaptation and sentiment analysis. It is collected from product reviews from Amazon.com and contains four types (domains), namely books (B), DVDs (D), electronics (E) and kitchen appliances (K). For each domain, there are 2,000 labeled reviews and approximately 4,000 unlabeled reviews (varying slightly across domains) and the classes are balanced. In our experiments, for easy computation, we follow (Chen et al. 2012) to use the 5,000 most frequent terms of unigrams and bigrams as the input and totally A24 = 12 adaptation tasks are constructed.

**Office-Caltech object recognition dataset. **The Office-Caltech dataset released by (Gong et al. 2012) is comprised of 10 common categories shared by the Office-31 and Caltech-256 datasets. In our experiments, we construct 12 tasks across 4 domains: Amazon (A), Webcam (W), DSLR (D) and Caltech (C), with 958, 295, 157 and 1,123 image samples respectively. In our experiments, Decaf features are used as the input. Decaf features (Donahue et al. 2014) are the 4096-dimensional FC7-layer hidden activations extracted by the deep convolutional neural network AlexNet.
- 实验结果

![](https://img-blog.csdnimg.cn/20190110145716814.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/201901101457177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2,size_16,color_FFFFFF,t_70)

从上面结果可知，Wasserstein距离基本比MMD的效果好，但MMD也在个别情况下优于Wasserstein距离。





# 参考文献

[1] Shen J, Qu Y, Zhang W, et al. Wasserstein Distance Guided Representation Learning for Domain Adaptation[J]. AAAI-2017.



