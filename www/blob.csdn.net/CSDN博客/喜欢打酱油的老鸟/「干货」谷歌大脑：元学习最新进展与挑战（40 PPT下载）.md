
# 「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载） - 喜欢打酱油的老鸟 - CSDN博客


2019年01月30日 12:11:16[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：68


[https://www.toutiao.com/a6651792282443842055/](https://www.toutiao.com/a6651792282443842055/)
2019-01-29 13:51:18
![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/d40cce2768cf4b768ccc618af28426af)
**【新智元导读】**Hugo Larochelle是谷歌大脑的研究员、深度学习领域的专家。他在最近的演讲中讨论了小样本学习这个主题，概述了这个领域的一些最新进展，存在的挑战以及研究机会。
深度学习的成功部分是由于大量有标记数据的可用性。然而，即使在大数据背景下的今天，想获取具有大量优质的、标签正确的数据的数据集，仍是一个难题。
近年**小样本学习(Few-shot Learning)**的发展，旨在解决这个问题。小样本学习属于迁移学习的一种。顾名思义，小样本学习是一种能够让深度学习（或者任何机器学习算法）通过更少的样本学习，而不像传统的那样使用大量样本的技术。
**Hugo Larochelle**是谷歌大脑的研究员，深度学习领域的专家。他在最近的一个workshop中，讨论了**元学习(Meta-learning)**这个主题。元学习是一个非常有前途的框架，用于解决从少量数据进行泛化的问题，即所谓的小样本学习。
在元学习中，我们的模型本身就是一种学习算法：它将输入作为训练集，并输出一个分类器。对于 few-shot learning，直接训练产生的分类器具有良好的泛化性能，适用于只有非常少的标记数据的问题。
在这个workshop中，Hugo Larochelle概述了最近这个主题上取得的一些最新进展，并讨论了仍然存在的挑战和研究机会。
以下是Hugo Larochelle讲座的PPT，文末附下载链接。
**内容概要**
**元学习(Meta-learning)**
**选择一种Meta-learning**
**定义meta-learner的两种方法：**
从已知的学习算法得到启发
kNN/kernel machine: Matching networks (Vinyals et al. 2016)
高斯分类器：Prototypical Networks (Snell et al. 2017)
梯度下降法：(Ravi & Larochelle, 2017) , MAML (Finn et al. 2017)
从黑盒神经网络推导
SNAIL (Mishra et al. 2018)
**存在的挑战：**
超越有监督分类
无监督学习、结构化输出、互动学习
超越Mini-ImageNet
对问题/数据集的分布给出一个现实的定义
**Meta-Dataset**
**小结：**
Meta-training 的分布可以产生很大的不同(至少对于目前的方法而言)
使用“常规训练”作为初始化会产生很大的不同
需要调整MAML以使其更具鲁棒
![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p9.pstatp.com/large/pgc-image/a07d3545d8384a608e81bbff9e041eba)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/fe2b697957c641fda201278eda8abba4)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/cb2fa107896f4fc099b52484d95341e9)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p9.pstatp.com/large/pgc-image/4a680a6ae3cd46aa82b0d0fcae873d44)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p9.pstatp.com/large/pgc-image/4a680a6ae3cd46aa82b0d0fcae873d44)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/3fce754bd0fd4d57bac944d7d3facd2e)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/85d2a13ff63b48da8c108e72522e22d6)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/30a7ec8bbd8d4bf084a8e224e69c5f48)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/49142a9b5af649c69b78a70a2fbf891a)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/6e2a46d981de4b3686bc312e1c67bd38)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/354f25c26c624b65bf6985d0dbb1c868)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/944cb8e4272a4825bd6c8fda05dd5822)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/417b38a78a2848789456b1a8b53b7860)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/e3c67ed3e85d4a748f7c0d1b7109813b)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p9.pstatp.com/large/pgc-image/b8d6c8829284475f9c9ae3d91aa7a20c)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p9.pstatp.com/large/pgc-image/85a9d64994ef46ada048a9cb0d289249)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/a91fea83b4a84ef8ada1ea14cec6ff2d)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/27d0ba4e7ba14023b4c85405a3938906)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/1dbc10ed0ffe44a4a9af217d5c57b4be)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/ec54b1e3fb9d4a82bfe3396bcf22f340)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/9d08f994b3204d1287644b00861355ed)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/436ab1d3873d43318771cc1a584f38d2)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/21910f4bdbc74e12898bece4b93a79db)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/ed800850bf164084a9a79b6aed7da3dc)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/c7916b9f1e554daeb5b7de4762cddd67)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/dd55f246e4564d6695671c16549b6e5a)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/6fd33463b75f41d8b167725e63eefad6)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/2d59b6d9fd4d42bc8f8a4d58f562c23d)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/cb363567c0534fce83a77078618f8399)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/e3368d093cfd4aa189b026a4397af3fe)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/82c552f77228470b9d8909d7205604c4)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/c0ba2735bf0c47a5b873a52429963474)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/c03dad527a9b49bb822ef027d1bd3010)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/a88a07598de34566a6f644832b0ee103)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/566cbcc02f5e4144980759c82959a75c)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/b16ce214e6254c298b63b06100e55340)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/c0cbe8b822fa44c795219b5295c62f2d)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/d2f18018b3984e38b5095685de0b4d46)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p3.pstatp.com/large/pgc-image/0be5cb4823e342c88ebe63c344f66bbd)

![「干货」谷歌大脑：元学习最新进展与挑战（40 PPT下载）](http://p1.pstatp.com/large/pgc-image/7ff7178d13a347e7a604fdfd737da069)

**PPT下载：**
**http://metalearning.ml/2018/slides/meta_learning_2018_Larochelle.**

