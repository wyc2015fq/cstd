# cv岗位 - 机器学习面试知识总结（不断更新） - Snoopy_Dream - CSDN博客





2019年04月11日 23:00:35[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：51








总是看到网上的面经有提到大厂面他们的时候，会问一些机器学习的问题，很多其实都是数学问题，乍一看，好晕啊，什么贝叶斯概率，似然函数什么的。但是怎么说，这是必须要过的一关，而且我对自己的数学还是相当有信心的。

**目录**

[基本概念](#main-toc)

[先验概率](#%E5%85%88%E9%AA%8C%E6%A6%82%E7%8E%87)

[后验概率](#%E5%90%8E%E9%AA%8C%E6%A6%82%E7%8E%87)

[贝叶斯定理（垃圾邮件过滤，单词纠正事件）](#%E8%B4%9D%E5%8F%B6%E6%96%AF%E5%AE%9A%E7%90%86)

[朴素贝叶斯](#%E6%9C%B4%E7%B4%A0%E8%B4%9D%E5%8F%B6%E6%96%AF)

[概率质量函数](#%E6%A6%82%E7%8E%87%E8%B4%A8%E9%87%8F%E5%87%BD%E6%95%B0)

[似然性](#%E4%BC%BC%E7%84%B6%E6%80%A7)

[似然函数](#%E4%BC%BC%E7%84%B6%E5%87%BD%E6%95%B0)

[最大似然估计](#%E6%9C%80%E5%A4%A7%E4%BC%BC%E7%84%B6%E4%BC%B0%E8%AE%A1)

[奥卡姆剃刀](#%E5%A5%A5%E5%8D%A1%E5%A7%86%E5%89%83%E5%88%80)

[线性回归](#%E7%BA%BF%E6%80%A7%E5%9B%9E%E5%BD%92)

[目标函数的由来](#%E7%9B%AE%E6%A0%87%E5%87%BD%E6%95%B0%E7%9A%84%E7%94%B1%E6%9D%A5)

[逻辑回归（LR分类 0-1 sigmod分类）](#%E9%80%BB%E8%BE%91%E5%9B%9E%E5%BD%92%EF%BC%88LR%E5%88%86%E7%B1%BB%200-1%20sigmod%E5%88%86%E7%B1%BB%EF%BC%89)

[决策树与随机森林](#%E5%86%B3%E7%AD%96%E6%A0%91%E4%B8%8E%E9%9A%8F%E6%9C%BA%E6%A3%AE%E6%9E%97)

[熵（信息熵）](#%E7%86%B5%EF%BC%88%E4%BF%A1%E6%81%AF%E7%86%B5%EF%BC%89)

[条件熵](#%E6%9D%A1%E4%BB%B6%E7%86%B5)

[信息增益 ID3](#%E4%BF%A1%E6%81%AF%E5%A2%9E%E7%9B%8A%C2%A0ID3)

[信息增益率 C4.5](#%E4%BF%A1%E6%81%AF%E5%A2%9E%E7%9B%8A%E7%8E%87%C2%A0C4.5)

[结合信息增益和信息增益率](#%E7%BB%93%E5%90%88%E4%BF%A1%E6%81%AF%E5%A2%9E%E7%9B%8A%E5%92%8C%E4%BF%A1%E6%81%AF%E5%A2%9E%E7%9B%8A%E7%8E%87)

[基尼系数 CART](#%E5%9F%BA%E5%B0%BC%E7%B3%BB%E6%95%B0%20CART)

[​](#%E2%80%8B)

[预裁剪](#%E9%A2%84%E8%A3%81%E5%89%AA)

[后裁剪](#%E5%90%8E%E8%A3%81%E5%89%AA)

[随机森林（bagging是将多个模型的预测结果简单地加权平均或者投票。）、](#%E9%9A%8F%E6%9C%BA%E6%A3%AE%E6%9E%97%EF%BC%88bagging%E7%9A%84%E5%85%B8%E5%9E%8B%EF%BC%89)

[xgboost](#xgboost)

[adaBoost](#adaBoost)

[stacking](#stacking)

## 基本概念



### 先验概率
- P(*A*)是A的[先验概率](https://zh.wikipedia.org/wiki/%E5%85%88%E9%AA%8C%E6%A6%82%E7%8E%87)（或[边缘概率](https://zh.wikipedia.org/wiki/%E8%BE%B9%E7%BC%98%E6%A6%82%E7%8E%87)）。之所以称为"先验"是因为它不考虑任何B方面的因素。

### 后验概率
- P(*A*|*B*)是已知B发生后A的[条件概率](https://zh.wikipedia.org/wiki/%E6%9D%A1%E4%BB%B6%E6%A6%82%E7%8E%87)，也由于得自B的取值而被称作A的[后验概率](https://zh.wikipedia.org/wiki/%E5%90%8E%E9%AA%8C%E6%A6%82%E7%8E%87)。

### 贝叶斯定理（垃圾邮件过滤，单词纠正事件）

描述 P(*A*|*B*)和P(*B|A*)的关系

通常，事件A在事件B已发生的条件下发生的概率，与事件B在事件A已发生的条件下发生的概率是不一样的。然而，这两者是有确定的关系的，贝叶斯定理就是这种关系的陈述。

![](https://img-blog.csdnimg.cn/20190411223052841.png)

对于给定观测数据，一个猜测是好是坏后验概率，取决于“这个猜测本身独立的可能性大小（先验概率，Prior ）”和“这个猜测生成我们观测到的数据的可能性大小。

P(*B|A*)/P(*B*)称作是标准似然度，所以** A的后验概率 = A的先验概率 * 标准似然度**

***垃圾邮件过滤***

![](https://img-blog.csdnimg.cn/2019041216521410.png)

![](https://img-blog.csdnimg.cn/20190412165619423.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

P(d2|d1,h+)表示是垃圾邮件，里面含有d1的情况下，也有d2的概率。

![](https://img-blog.csdnimg.cn/20190412170128297.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

我们看上面的例子，证明了贝叶斯的用处。

***拼写纠正***

![](https://img-blog.csdnimg.cn/20190412170831908.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190412170859969.png)




![](https://img-blog.csdnimg.cn/20190412170802946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 朴素贝叶斯

在原始的贝叶斯基础上，假设了我们特征之间是相互独立，互不影响的。

在[机器学习](https://zh.wikipedia.org/wiki/%E6%9C%BA%E5%99%A8%E5%AD%A6%E4%B9%A0)中，**朴素贝叶斯分类器**是一系列以假设特征之间强（朴素）[独立](https://zh.wikipedia.org/wiki/%E7%B5%B1%E8%A8%88%E7%8D%A8%E7%AB%8B%E6%80%A7)下运用[贝叶斯定理](https://zh.wikipedia.org/wiki/%E8%B4%9D%E5%8F%B6%E6%96%AF%E5%AE%9A%E7%90%86)为基础的简单[概率分类器](https://zh.wikipedia.org/w/index.php?title=%E6%A6%82%E7%8E%87%E5%88%86%E7%B1%BB%E6%B3%95&action=edit&redlink=1)。

### 概率质量函数

**概率质量函数**（probability mass function，简写为**pmf**）是[离散随机变量](https://zh.wikipedia.org/wiki/%E7%A6%BB%E6%95%A3%E9%9A%8F%E6%9C%BA%E5%8F%98%E9%87%8F)在各特定取值上的概率。**概率质量函数**和[概率密度函数](https://zh.wikipedia.org/wiki/%E6%A6%82%E7%8E%87%E5%AF%86%E5%BA%A6%E5%87%BD%E6%95%B0)不同之处在于：概率质量函数是对[离散随机变量](https://zh.wikipedia.org/wiki/%E7%A6%BB%E6%95%A3%E9%9A%8F%E6%9C%BA%E5%8F%98%E9%87%8F)定义的，本身代表该值的概率；概率密度函数是对[连续随机变量](https://zh.wikipedia.org/wiki/%E8%BF%9E%E7%BB%AD%E9%9A%8F%E6%9C%BA%E5%8F%98%E9%87%8F)定义的，本身不是概率，只有对连续随机变量的[概率密度函数](https://zh.wikipedia.org/wiki/%E6%A6%82%E7%8E%87%E5%AF%86%E5%BA%A6%E5%87%BD%E6%95%B0)在某区间内进行[积分](https://zh.wikipedia.org/wiki/%E7%A7%AF%E5%88%86)后才是概率。

### 似然性

似然函数可以理解为[条件概率](https://zh.wikipedia.org/wiki/%E6%9D%A1%E4%BB%B6%E6%A6%82%E7%8E%87)的逆反。

条件概率：**用于在已知一些参数的情况下**，预测**结果**；

**P(*y*|*w*) **w是权重参数，已知权重，预测此时的wx得到的y的概率

**似然性：**则是用于在**已知某些观测所得到的结果时**，**参数估值，使得不断逼近于观测所得到的结果。**

**P(*w|y*) **已知了真实y，然后估计此时的权重w，神经网络不就这样的吗，已知了真实的y，然后通过不断估计（更改）w使得**P(*w|y*) **越大（越接近于真实y）

### 似然函数

**似然函数：**是一种关于[统计模型](https://zh.wikipedia.org/wiki/%E7%BB%9F%E8%AE%A1%E6%A8%A1%E5%9E%8B)中的[参数](https://zh.wikipedia.org/wiki/%E5%8F%82%E6%95%B0)的[函数](https://zh.wikipedia.org/wiki/%E5%87%BD%E6%95%B0)，表示模型**参数中的似然性**

### 最大似然估计

我们可以在![\theta](https://wikimedia.org/api/rest_v1/media/math/render/svg/6e5ab2664b422d53eb0c7df3b87e1360d75ad9af)的所有可能取值中寻找一个值使得似然[函数](https://zh.wikipedia.org/wiki/%E5%87%BD%E6%95%B0)取到最大值。这个使可能性最大的![\widehat{\theta}](https://wikimedia.org/api/rest_v1/media/math/render/svg/89a031ed08d81ed4f0df984f0d9f30b7ae07be46)值即称为![\theta](https://wikimedia.org/api/rest_v1/media/math/render/svg/6e5ab2664b422d53eb0c7df3b87e1360d75ad9af)的**最大似然估计**。由定义，最大似然估计是样本的函数。

### 奥卡姆剃刀

先验概率大的模型更好，比如线性回归的时候，一阶的函数最常见，所以最好。

## 线性回归

### 目标函数的由来

首先得到误差的概率质量函数（高斯正态分布），转换求似然函数最大（y固定已知了，估计w权重（更改权重）使得wx接近于y的概率），然后把m个不同样本的P相乘，再转换log似然函数，发现![](https://img-blog.csdnimg.cn/2019041122493939.png)最小，似然函数就最大，因此得到了成本函数。

再说几句梯度下降的直观理解，就是通过不断求梯度，在梯度方向上更新w，使得成本函数J最小。

![](https://img-blog.csdnimg.cn/20190411224221549.png)

![](https://img-blog.csdnimg.cn/20190411224245407.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411224313297.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411224334652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411224349263.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411225150847.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411225218378.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

## 逻辑回归（LR分类 0-1 sigmod分类）

首先逻辑回归是01分类问题

他的**成本函数和线性回归的是一样的道理**，主要注意的是刚开始P是怎么得到的，也就是已知y=1是对w进行估计（更新）使得估计值y’就是概率值，已知y=0是对w进行估计（更新），使得1-估计值y’就是概率值，然后**相互相乘结合。**

从log似然函数中得到。

![](https://img-blog.csdnimg.cn/2019041122571825.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411225804886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411225927427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

## 决策树与随机森林

### 熵（信息熵）

表示的是混乱程度，混乱程度越大，熵越大。

![](https://img-blog.csdnimg.cn/2019041211441934.png)log 2省略了

当p=0.5时，H(p)=1,此时随机变量的不确定性最大

![](https://img-blog.csdnimg.cn/20190412114505273.png)

![](https://img-blog.csdnimg.cn/20190412114612739.png)

### 条件熵

乘以了发生的概率，比如判断西瓜的好坏，特征里面有一个特征是色泽（绿，黑，白）。如何求色泽对于好坏瓜的条件熵呢？

黑的里面，好瓜有4个，坏瓜有2个，所以色泽里黑的熵为下面的0.918

![](https://img-blog.csdnimg.cn/20190412115304195.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

然后色泽里乌黑的瓜6/17，所以乌黑的信息熵（针对好坏瓜而言）是0.918，乌黑（针对好坏瓜而言）条件熵为6/17*0.918

![](https://img-blog.csdnimg.cn/20190412115433251.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 信息增益 ID3

表示特征X使得类Y的不确定性减少的程度。

（分类后的专一性，希望分类后的结果是同类在一起）

色泽的信息增益（针对结果：好坏瓜） = 原始好坏瓜的信息熵 - 色泽的条件熵（针对结果：好坏瓜）

表示加入色泽后使得好坏瓜的不确定性减少了多少
![](https://img-blog.csdnimg.cn/20190412115433251.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 信息增益率 C4.5

单独使用信息增益作为评估的话，会偏近于取值分类多的特征，作为根节点，比如ID，每一个样本一个ID。

所以诞生了信息增益率

Gain_Ratio(D,a) = Gain(D,a) / IV(a)

IV(a)表示的是不再是以针对原本结果好坏瓜的熵，而是针对a为结果目标的熵，比如说是色泽，我们把色泽看成是最终的分类结果，得到色泽的信息熵 （里面的概率就是乌黑所占的比例）

这样一来，如果还是以ID的话，ID本身的熵是很大的，除以她Gain_Ratio(D,a)就变小了。

![](https://img-blog.csdnimg.cn/20190412120647791.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

Gain_Ratio(D,a)，偏近于取值分类少的特征，作为根节点

### 结合信息增益和信息增益率

Gain(D,a) ， 偏近于取值分类多的特征，作为根节点

Gain_Ratio(D,a)，偏近于取值分类少的特征，作为根节点

所以一般是先用Gain选出超过平均的，然后再用Gain_Ratio(D,a)判断。结合两个的优势

### 基尼系数 CART

### ![](https://img-blog.csdnimg.cn/20190412121132234.png)

### 预裁剪

一边训练，一边裁剪，优点是速度快，不容易导致过拟合，但是相对于后裁剪而言，容易造成欠拟合

预备裁剪是提前停止的意思，比如限制深度，叶子节点个数，叶子节点样本数，信息增益量等

### 后裁剪

全部训练完后，从底向上一次裁剪，是对所有的非叶子节点进行运算，看是否要把当前的非叶子节点变成叶子结点。

![](https://img-blog.csdnimg.cn/20190412153615629.png)C是基尼系数

下面的图中，一个节点下面有三个叶子结点，然后依次求出，比较。

![](https://img-blog.csdnimg.cn/20190412160715534.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 随机森林（bagging是将多个模型的预测结果简单地加权平均或者投票。）、

随机：数据采样随机（去燥），特征选择随机（去争议的特征）

数据随机，是指不同的树采样了0.6（比如）的总数据。

森林：多棵树并行处理。
- 重复K次
- 有放回地重复抽样建模
- 训练子模型

     2.模型融合
- 分类问题：voting
- 回归问题：average

Bagging算法不用我们自己实现，随机森林就是基于Bagging算法的一个典型例子，采用的基分类器是决策树。R和python都集成好了，直接调用。

![](https://img-blog.csdnimg.cn/20190412161232292.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

## xgboost

可以分类，可以回归，例如分类问题中用 cross entropy，回归问题用 mean squared error。

应用的是**Gradient boosting算法** 就是通过加入新的弱学习器，来努力纠正前面所有弱学习器的残差，最终这样多个学习器相加在一起用来进行最终预测，准确率就会比单独的一个要高。之所以称为 Gradient，是因为在添加新模型时使用了梯度下降算法来最小化的损失。

那么不断集成弱分类器最后得到强的，如何保证这一点呢？

通过**目标函数**的设计：

以![](https://img-blog.csdnimg.cn/20190412173511318.png)均方差+惩罚项（以树为例）为目标函数，把前**t-1个模型构成的结果固定住**，通过**调整新加进来的模型**，**使得目标函数变小**，这样就导致了越来越好了！

![](https://img-blog.csdnimg.cn/20190412173348417.png)

![](https://img-blog.csdnimg.cn/20190412173404389.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

我们看到这里还是不能把前面**t-1个模型构成的结果固定住，需要利用下图中的**泰勒级数展开**，就可以了。**
![](https://img-blog.csdnimg.cn/20190412174205913.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

最后就是使目标函数最小的问题了，这里用的是梯度下降的方式。

## adaBoost

通过多分类器，按照分类器的精度作为权重，最后做决定。后面的分类器是由前面的分类器分错的样本增大权重，新生成的分类器，具体的公式就不摆了。

![](https://img-blog.csdnimg.cn/20190412180021415.png)

## stacking

[https://zhuanlan.zhihu.com/p/25836678](https://zhuanlan.zhihu.com/p/25836678)（模型融合好文）

[https://www.cnblogs.com/gczr/p/7144508.html](https://www.cnblogs.com/gczr/p/7144508.html)

[https://www.leiphone.com/news/201709/zYIOJqMzR0mJARzj.html](https://www.leiphone.com/news/201709/zYIOJqMzR0mJARzj.html)

如果把Bagging看作是多个基分类器的线性组合，那么Stacking就是多个基分类器的非线性组合。Stacking可以很灵活，它可以将学习器一层一层地堆砌起来。

![](https://pic1.zhimg.com/v2-b78f1f3a33390931a46a8d16d3daa3ec_b.png)![](https://pic2.zhimg.com/v2-fedff96e21984b422af30745d2413151_b.jpg)



![Kaggleæºå¨å­¦ä¹ ä¹æ¨¡åèåï¼stackingï¼å¿å¾](https://static.leiphone.com/uploads/new/article/pic/201709/cfb37e4250561177520f6f7af86d42f4.jpg)

对于每一轮的 5-fold，Model 1都要做满5次的训练和预测。

Titanic 栗子：

Train Data有890行。(请对应图中的上层部分）

每1次的fold，都会生成 713行 小train， 178行 小test。我们用Model 1来训练 713行的小train，然后预测 178行 小test。预测的结果是长度为 178 的预测值。

这样的动作走5次！ 长度为178 的预测值 X 5 = 890 预测值，刚好和Train data长度吻合。这个890预测值是Model 1产生的，我们先存着，因为，一会让它将是第二层模型的训练来源。

重点：这一步产生的预测值我们可以转成 890 X 1 （890 行，1列），记作 P1 (大写P)

接着说 Test Data 有 418 行。(请对应图中的下层部分，对对对，绿绿的那些框框）

每1次的fold，713行 小train训练出来的Model 1要去预测我们全部的Test Data（全部！因为Test Data没有加入5-fold，所以每次都是全部！）。此时，Model 1的预测结果是长度为418的预测值。

这样的动作走5次！我们可以得到一个 5 X 418 的预测值矩阵。然后我们根据行来就平均值，最后得到一个 1 X 418 的平均预测值。

重点：这一步产生的预测值我们可以转成 418 X 1 （418行，1列），记作 p1 (小写p)

走到这里，你的第一层的Model 1完成了它的使命。

第一层还会有其他Model的，比如Model 2，同样的走一遍， 我们有可以得到  890 X 1  (P2) 和  418 X 1 (p2) 列预测值。

这样吧，假设你第一层有3个模型，这样你就会得到：

来自5-fold的预测值矩阵 890 X 3，（P1，P2， P3）  和 来自Test Data预测值矩阵 418 X 3， （p1, p2, p3）。

-----------------------------------------

到第二层了..................

来自5-fold的预测值矩阵 890 X 3 作为你的Train Data，训练第二层的模型

来自Test Data预测值矩阵 418 X 3 就是你的Test Data，用训练好的模型来预测他们吧。

---------------------------------------

最后 ，放出一张Python的Code，在网上为数不多的stacking内容里， 这个几行的code你也早就看过了吧，我之前一直卡在这里，现在加上一点点注解，希望对你有帮助：

![Kaggle机器学习之模型融合（stacking）心得](https://static.leiphone.com/uploads/new/article/pic/201709/d9b737ad6edbc00ffa15aa082fa4bb19.png)





