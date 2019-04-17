# OReilly.Hands-On.Machine.Learning.with.Scikit-Learn.and.TensorFlow学习笔记汇总 - 墨流觞的博客 - CSDN博客





2018年09月27日 14:29:37[墨氲](https://me.csdn.net/dss_dssssd)阅读数：77标签：[python																[tensorflow																[sklearn](https://so.csdn.net/so/search/s.do?q=sklearn&t=blog)
个人分类：[Python数据处理及可视化](https://blog.csdn.net/dss_dssssd/article/category/8077244)





其中用到的知识点我都记录在博客中了：[https://blog.csdn.net/dss_dssssd](https://blog.csdn.net/dss_dssssd)

### 第一章知识点总结：
- 
supervised learning
- k-Nearest Neighbors
- Linear Regression
- Logistic Regression
- Support Vector Machines (SVMs)
- Decision Trees and Random Forests
- Neural networks

**Note:** 有些神经网络结构是非监督性的，比如自动编码机(autoencoders)和限制玻尔兹曼机(restricted Boltzmann machines);还有些神经网络结构是半监督(semisupervised)的,比如深度信念网络(deep belief network)和非监督预训练(unsupervised pretraining)

- 
unsupervised leanrning
- 
Clustering
- k-Means
- Hierarchical Cluster Analysis (HCA)
- Expectation Maximization

- 
Visualization and dimensionality reduction
- Principal Component Analysis (PCA)
- Kernel PCA
- Locally-Linear Embedding (LLE)
- t-distributed Stochastic Neighbor Embedding (t-SNE)

- 
Association rule learning
- Apriori
- Eclat


非监督学习算法用途：
- 
**数据可视化**  t-SNE in cirfa-10
![](./2.png)- 
**数据降维处理** ，也称为 **特征提取** 在喂给机器学习算法之前，最好利用降维算法对训练数据进行维度缩减，将会大大加快运行速度

- 
**异常检测(anomaly detection)**,比如检测信用卡异常交易记录来预防欺骗行为；或者在喂给机器学习算法时，从训练数据集中抛离异常值
![](./1.png)- 
**关联规则学习(association rule learning):** 在大型数据库中发现变量之间的有趣的连系。目的是利用一些有趣性的量度来识别数据库中发现的强规则。比如从销售数据中发现的规则 {洋葱, 土豆}→{汉堡} 会表明如果顾客一起买洋葱和土豆，他们也有可能买汉堡的肉。

- 
semisupervised learning (半监督学习)

是监督学习与无监督学习相结合的一种学习方法。半监督学习使用大量的未标记数据，以及同时使用少量的标记数据，来进行模式识别工作。

深度信念网络(deep belief learning)：堆叠限制玻尔兹曼机(restricted Boltzmann machines (RBMs))。- 
强化学习 (Reinforcement Learning)
![](./3.png)
不需要准确的数据/标签对，有点类似于pid闭环控制算法，但复杂度更高

[强化学习介绍](https://blog.csdn.net/coffee_cream/article/details/57085729)

### 第二章的学习ipynb文件：

[https://github.com/MengRe/deep-learning-notes/blob/master/chap2.ipynb](https://github.com/MengRe/deep-learning-notes/blob/master/chap2.ipynb)

### 第三章学习文件
- 
#### csdn文档
[https://blog.csdn.net/dss_dssssd/article/details/82958085](https://blog.csdn.net/dss_dssssd/article/details/82958085)
- 
### ipynb文件
[https://github.com/MengRe/deep-learning-notes](https://github.com/MengRe/deep-learning-notes)](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




