# coursera-斯坦福-机器学习-吴恩达-第8周笔记-无监督学习 - 博客堂 - CSDN博客





2017年12月19日 22:00:53[最小森林](https://me.csdn.net/u012052268)阅读数：3600
所属专栏：[斯坦福-吴恩达《机器学习》之路](https://blog.csdn.net/column/details/18191.html)









![image](http://upload-images.jianshu.io/upload_images/3077865-381d7a24734c77ae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

# coursera-斯坦福-机器学习-吴恩达-第8周笔记-无监督学习



- [coursera-斯坦福-机器学习-吴恩达-第8周笔记-无监督学习](#coursera-斯坦福-机器学习-吴恩达-第8周笔记-无监督学习)- [1聚类算法clutering](#1聚类算法clutering)- [1聚类算法简介](#11聚类算法简介)
- [2K-means](#12k-means)
- [21kmeans的目标函数](#121kmeans的目标函数)
- [22随机初始化](#122随机初始化)
- [23选择类别数](#123选择类别数)
- [3考试quiz](#13考试quiz)

- [维数约减 dimensionality reduction](#2-维数约减-dimensionality-reduction)- [1数据压缩](#21数据压缩)
- [2数据可视化](#22数据可视化)

- [3维度约简-主成分分析法PCA](#3维度约简-主成分分析法pca)- [1 PCA是做什么的](#31-pca是做什么的)
- [2PCA的计算过程](#32pca的计算过程)

- [4应用PCA](#4应用pca)- [1PCA反向压缩](#41pca反向压缩)
- [2怎么选择维度k](#42怎么选择维度k)
- [3使用PCA的场景](#43使用pca的场景)

- [5总复习quiz与编程](#5总复习quiz与编程)- [1quiz主成份分析](#51quiz主成份分析)
- [2编程题](#52编程题)- [K-means clustering](#1-k-means-clustering)
- [2主成分分析](#2主成分分析)







对于无监督学习我们主要学习两种算法：聚类（K-means）和维度约简（PCA法）。

## 1聚类算法clutering

### 1.1聚类算法简介

无监督学习：我们面对的是一组无标记的训练数据， 数据之间， 不具任何相关联的标记。如图：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-16/62891890.jpg)

我们得到的数据 看起来像这样：一个数据集， 一堆数据点，但没有任何标记以供参考。所以从训练数据中， 我们只能看到 x 1、 x 2… 等等… 到 x(m) 没有任何标记 y 供参考。 

就此数据而言， 其中一种可能的结构 是 所有的数据 可以大致地划分成 两个类或组。 因此，像我介绍的 这种划分组的算法， 称为  

聚类算法。 这是我们第一种 无监督学习算法。

> 
**记住，聚类算法clutering只是无监督学习的一种，不是所有的无监督学习都是聚类算法**


### 1.2K-means

K-means也是聚类算法中最简单的一种。但是里面包含的思想却是不一般。

K-means算法是将样本聚类成k个簇（cluster），具体算法描述如下：
- 
随机选取k个聚类质心点（cluster centroids）为![image](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/20110406160145717.png)。

- 
重复下面过程直到收敛 {
- [ ] 对于每一个样例i，计算其应该属于的类


![image](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104061601464654.png)
- [ ]    对于每一个类j，重新计算该类的质心

![image](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104061601474753.png)

}

下图展示了对n个样本点进行K-means聚类的效果，这里k取2。 
![image](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104061601534923.png)
一个练习题：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-16/7484812.jpg)

### 1.2.1kmeans的目标函数

在大多数我们已经学到的 监督学习算法中。 算法都有一个优化目标函数 或者某个代价函数（又叫：畸变函数）需要通过算法进行最小化 。

事实上 K均值也有 一个优化目标函数或者 需要最小化的代价函数。

![image](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104061601464654.png)

注意，这个值只会随着迭代下降，不会上升。

### 1.2.2随机初始化

这一节我们讨论： 如何避开局部最优来构建K均值聚类方法 。

有几种不同的方法 可以用来随机 初始化聚类中心 ，但是 事实证明， 有一种方法比其他 大多数方法 更加被推荐。

![](http://oqy7bjehk.bkt.clouddn.com/17-12-18/20474706.jpg)

可以避免 可能局部，获得全局最优的结果 。

### 1.2.3选择类别数

讨论一下 K-均值聚类的最后一个细节 ：我想选择聚类数目的更好方法。 或者说是如何去选择 参数大写K的值 。

说实话 这个问题上没有一个 非常标准的解答 、或者能自动解决它的方法。

目前用来决定聚类数目的 最常用的方法 ，仍然是通过看可视化的图， 或者看聚类算法的输出结果 ，或者其他一些东西来手动地决定聚类的数目。

两种常见方法： 

1. 肘部法则

例如下面的例子，分别考虑3和5，画出loss图像。

![](http://oqy7bjehk.bkt.clouddn.com/17-12-18/26571725.jpg)
- 从后续需求（生意）角度考虑

![](http://oqy7bjehk.bkt.clouddn.com/17-12-18/6770429.jpg)

下面有个练习题：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-18/21753139.jpg)

### 1.3考试quiz
- 
For which of the following tasks might K-means clustering be a suitable algorithm? Select all that apply.答案ad
- [x] From the user usage patterns on a website, figure out what different groups of users exist.
- [ ] Given historical weather records, predict if tomorrow’s weather will be sunny or rainy.
- [ ] Given many emails, you want to determine if they are Spam or Non-Spam emails.
- [x] Given a set of news articles from many different news websites, find out what are the main topics covered.

- 
第二题： 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-18/43376572.jpg)- 
K-means is an iterative algorithm, and two of the following steps are repeatedly carried out in its inner-loop. Which two?


K-means是一种迭代算法，以下两个步骤在其内部循环中重复执行。哪两个？
- [ ] The cluster assignment step, where the parameters c(i) are updated.
- 
[ ] Move the cluster centroids, where the centroids μk are updated.
- Suppose you have an unlabeled dataset {x(1),…,x(m)}. You run K-means with 50 different random initializations, and obtain 50 different clusterings of the data. What is the recommended way for choosing which one of these 50 clusterings to use?答案C

- 
[ ] Plot the data and the cluster centroids, and pick the clustering that gives the most “coherent” cluster centroids.

- [ ] Manually examine the clusterings, and pick the best one.
- [x] Compute the distortion function J(c(1),…,c(m),μ1,…,μk), and pick the one that minimizes this.
- 
[ ] Use the elbow method. 
- 第 5 个问题 Which of the following statements are true? Select all that apply.答案BC

- 
[ ] Since K-Means is an unsupervised learning algorithm, it cannot overfit the data, and thus it is always better to have as large a number of clusters as is computationally feasible.

- [ ] If we are worried about K-means getting stuck in bad local optima, one way to ameliorate (reduce) this problem is if we try using multiple random initializations.- 
- [ ] For some datasets, the “right” or “correct” value of K (the number of clusters) can be ambiguous, and hard even for a human expert looking carefully at the data to decide.
- [ ] The standard way of initializing K-means is setting μ1=⋯=μk to be equal to a vector of zeros.

## 2 维数约减 (dimensionality reduction)

这节开始介绍 第二种无监督学习问题 它叫维数约减 (dimensionality reduction) 。

我们希望使用维数约简 的原因有以下几个 ：  

1. 一个原因是数据压缩。  

数据压缩不仅通过 压缩数据使得数据 占用更少的计算机 内存和硬盘空间， 它还能给算法提速 它还能给算法提速 。 

2. 另一个原因就是可视化。通过降维进行可视化，进而更好地理解数据。
### 2.1数据压缩

在实际工作中，数据的维度通常是很大的（100+）。那么数据维度约简(data dimensionality reduction)是很有必要的。

举一个例子：3维降成2维：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-18/95447653.jpg)

### 2.2数据可视化

![](http://oqy7bjehk.bkt.clouddn.com/17-12-18/63859389.jpg)

比如这个国家GDP的数据，数据有50维。不能画出来，但是个以用两个维度大体上表示出来。比如这样： 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-18/44423147.jpg)

横轴可以表示国家面积，国家总GDP等，纵轴可以表示人均GDP，幸福度等等。
那么怎么找出这种可以很好表示其他特征的特征呢？下面一节介绍这个算法PCA（主成分分析法）。

## 3维度约简-主成分分析法PCA

对于降维问题来说 目前 最流行 最常用的算法是 主成分分析法 (Principal Componet Analysis, PCA） 

### 3.1 PCA是做什么的

PCA是寻找到一个低维的平面 对数据进行投影 ，以便 最小化投影误差的平方（ 最小化每个点 与投影后的对应点之间的距离的平方值 ）。

定义：想把数据从n维降到k维（k < n），就在这个空间里面找k个单位向量来表示数据，使得数据点投影到这个面上的误差最小。如下例子：2到1 和 3到2

![](http://oqy7bjehk.bkt.clouddn.com/17-12-19/36897107.jpg)

下面介绍 线性回归与2维PCA的区别：虽然都是找一条直线去拟合但是，
- 计算loss的方式不同（垂直）。
- PCA没有标签Y（非监督）。

![](http://oqy7bjehk.bkt.clouddn.com/17-12-19/18002544.jpg)

### 3.2PCA的计算过程❤❤❤

设有m条n维数据。将原始数据按列组成n行m列矩阵X
- 将X的每一行（代表一个属性字段）进行零均值化，即减去这一行的均值
- 求出协方差矩阵![image](http://mmbiz.qpic.cn/mmbiz_png/951TjTgiabky2jXhnzqYC7JdLQBWPVKFGoe8LAUg4T7G4wTwkFz5emCfhKSc1QHlLRtEYKKEsEB8G7jsRGTBViag/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
- 求出协方差矩阵的特征值及对应的特征向量（使用svd函数）。（特征向量最能代表原数据）
- 将特征向量按对应特征值大小从上到下按行排列成矩阵，取前k行组成矩阵$U_r$。（找k个最能代表原数据的向量）
- $Y=U^T \times X$即为降维到k维后的数据

例子如下：  假设我们得到的2维数据如下：

![image](http://mmbiz.qpic.cn/mmbiz_png/UGE9l9r0r79zA7Qaia619gN7nkllr7WaJZYcoMLIKZ2Gl8HjoBQIktTXxA7rWzicXd1kHbOVCeb0IqkdUEibGOJZA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

行代表了样例，列代表特征，这里有10个样例，每个样例两个特征。可以这样认为，有10篇文档，x是10篇文档中“learn”出现的TF-IDF，y是10篇文档中“study”出现的TF-IDF。也可以认为有10辆汽车，x是千米/小时的速度，y是英里/小时的速度，等等。 

1. 第一步均值归一化

分别求x和y的平均值，然后对于所有的样例，都减去对应的均值。这里x的均值是1.81，y的均值是1.91，那么一个样例减去均值后即为（0.69,0.49），得到

![image](http://mmbiz.qpic.cn/mmbiz_png/UGE9l9r0r79zA7Qaia619gN7nkllr7WaJCMKicQul29pgvMaUKdCjK4Dkic8vtBs3VXKOBexhXsvKr6vU6IvTgqLw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
- 计算协方差矩阵

求特征协方差矩阵，如果数据是3维，那么协方差矩阵是

![image](http://mmbiz.qpic.cn/mmbiz_png/UGE9l9r0r79zA7Qaia619gN7nkllr7WaJaMkO4KlkiazZmOsfH4x9CuvwicbQic0KFuDdib65CzyJibtpdb6MLP0uJuw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

这里只有x和y（两维），求解得

![image](http://mmbiz.qpic.cn/mmbiz_png/UGE9l9r0r79zA7Qaia619gN7nkllr7WaJeYKLZnHrEx9Svic2PrOA2lZlnBK2DtTj5JRYogOzQVvG5Sz8tEkiaYNA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

对角线上分别是x和y的方差，非对角线上是协方差。协方差大于0表示x和y若有一个增，另一个也增；小于0表示一个增，一个减；协方差为0时，两者独立。协方差绝对值越大，两者对彼此的影响越大，反之越小。
- 求协方差的特征值和特征向量，得到 
![image](http://mmbiz.qpic.cn/mmbiz_png/UGE9l9r0r79zA7Qaia619gN7nkllr7WaJaxTArXVIODGuKvLkVHAXYkEt5eERq7ghbbA3ErHuY1S157euWxtc2Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
上面是两个特征值，下面是对应的特征向量，特征值0.0490833989对应特征向量为。。。，这里的特征向量都归一化为单位向量。
- 取前k个最有代表意义的特征向量：

将特征值按照从大到小的顺序排序，选择其中最大的k个，然后将其对应的k个特征向量分别作为列向量组成特征向量矩阵。

这里特征值只有两个，我们选择其中最大的那个，这里是1.28402771，对应的特征向量是 ( 0.677873399,0.735178656)
- 得到最后的数据

将样本点投影到选取的特征向量上。假设样例数为m，特征数为n，减去均值后的样本矩阵为DataAdjust(m * n)，协方差矩阵是n * n，选取的k个特征向量组成的矩阵为EigenVectors(n * k)。那么投影后的数据FinalData为 (0.677873399,0.735178656)T

这样，就将原始样例的 n 维特征变成了 k 维，这 k 维就是原始特征在 k 维上的投影。

## 4应用PCA

### 4.1PCA反向压缩

既然PCA可以将高维数据压缩到低维，那么反着使用PCA则可以将低维数据恢复到高维。

因为$Y=U^T \times X$，所以换算一下$ U\times Y= X$这里的X只是近似值。

那么当n和k相同的时候会发生什么呢？看下题：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-19/34785784.jpg)

1. U的维度为方阵 

2. 反着求x，为原值 

3. 保存率为100%
### 4.2怎么选择维度k

在 PCA 算法中 我们把n维特征变量 降维到k维特征变量 。这个数字k也被称作 主成分的数量 或者说是我们保留的主成分的数量 。 

在这个视频中 我会给你们一些参考 告诉你们 人们是怎样思考如何选择 PCA 的参数k的 。

我们先来思考两个值： 

1. 第一个是：PCA 所做的是 尽量最小化 平均平方映射误差 (Average Squared Projection Error) 。 

2. 第二个是：我还要定义一下 数据的总变差 (Total Variation) 。 它的意思是 “平均来看 我的训练样本 距离零向量多远？
我们把两个数的比值作为衡量PCA算法的有效性，比如

![](http://oqy7bjehk.bkt.clouddn.com/17-12-19/74910273.jpg)

所以一个比较好的办法是：定义一个阈值，然后实验k，看看那个最小的k合适。计算步骤如下：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-19/85328785.jpg)

这里有个技巧：svd函数会返回一个对角矩阵S，他的元素可以很快的计算这个阈值。

### 4.3使用PCA的场景

主成份分析法主要有以下用途： 

1. 数据压缩 

    1. 减少内存的占用、存储硬盘 

    2. 加速算法的运转 

2. 数据可视化：3维2维
有些人觉的PCA也可以用来防止过拟合，但是这是不对的。应该用正则化。正则化使用y标签最小化损失函数，使用了y标签信息。而PCA只单纯的看x的分部就删除了一些特征，损失率很多信息。

总结一下PCA的用法：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-19/98230239.jpg)

## 5总复习quiz与编程

### 5.1quiz主成份分析：
- 
第 1 个问题 

Consider the following 2D dataset:Which of the following figures correspond to possible values that PCA may return for u(1) (the first eigenvector / first principal component)? Check all that apply (you may have to check more than one figure).答案AB

- 
Which of the following is a reasonable way to select the number of principal components k?  (Recall that n is the dimensionality of the input data and m is the number of input examples.)答案：C 
- [ ] C.  Choose k to be the smallest value so that at least 99% of the variance is retained.

- 
Suppose someone tells you that they ran PCA in such a way that “95% of the variance was retained.” What is an equivalent statement to this?答案：C
- [ ] $\frac{ \frac{1}{m} \sum_{i=1}^m ||x^{(i)}- x^{(i)}_{approx}||^2}{\frac{1}{m} \sum_{i=1}^m ||x^{(i)}||^2} \leq 0.05$

- 
第 4 个问题 

Which of the following statements are true? Check all that apply.
- 
[ ] Even if all the input features are on very similar scales, we should still perform mean normalization (so that each feature has zero mean) before running PCA.

- 
[ ] Given input data x∈Rn, it makes sense to run PCA only with values of k that satisfy k≤n. (In particular, running it with k=n is possible but not helpful, and k>n does not make sense.)


- 
第 5 个问题 

Which of the following are recommended applications of PCA? Select all that apply.答案AC
- [ ] Data compression: Reduce the dimension of your data, so that it takes up less memory / disk space.
- [ ] Data compression: Reduce the dimension of your input data x(i), which will be used in a supervised learning algorithm (i.e., use PCA so that your supervised learning algorithm runs faster). 


### 5.2编程题

在本练习中，您将实现K均值聚类算法并将其应用于压缩图像。 在第二部分中，您将使用主成分分析来查找面部图像的低维表示。

#### 1 K-means clustering

先从二维的点开始，使用K-means进行分类。

1.1 Implement K-means

![image](http://static.zybuluo.com/EtoDemerzel/tkqsz1g6rupn8dq3yhricibz/image_1bvhcvi2vd2v166h1nhlq9uqe5m.png)

K-means步骤如上，在每次循环中，先对所有点更新分类，再更新每一类的中心坐标。
1.1.1 Finding closest centroids

对每个example，根据公式： 
![image](http://static.zybuluo.com/EtoDemerzel/4hdabyidl14amiy5ptwr0k6c/image_1bvhd7nem1lh31ijm1h0atjc1pm42j.png)
找到距离它最近的centroid，并标记。若有数个距离相同且均为最近，任取一个即可。

打开findClosestCentroids.m代码如下：

```matlab
for i=1:size(X,1)  
    adj=sqrt((X(i,:)-centroids(1,:))*(X(i,:)-centroids(1,:))');  
    idx(i)=1;  
    for j=2:K  
        temp=sqrt((X(i,:)-centroids(j,:))*(X(i,:)-centroids(j,:))');  
        if(temp<adj)  
            idx(i)=j;  
            adj=temp;  
        end  
    end  
end
```

1.1.2 Compute centroid means

对每个centroid，根据公式：![image](http://static.zybuluo.com/EtoDemerzel/5yyj3r6wjss8vsr1ug905jcn/image_1bvhddrbtfpl7kh1mdifn416er3g.png)求出该类所有点的平均值（即中心点）进行更新。

打开computeCentroids.m写入：

```matlab
for i=1:K  
    if(size(find(idx==i),2)~=0)  
        centroids(i,:)=mean(X(find(idx==i),:));  
    else  
        centroids(i,:)=zeros(1,n);  
    end  
end
```

1.2 K-means on example dataset

ex7.m中提供了一个例子，其中中 K 已经被手动初始化过了。

我们要把点分成三类，迭代次数为10次。三类的中心点初始化为(3,3),(6,2),(8,5). 

得到如下图像。（中间的图像略去，只展示开始和完成时的图像） 

这是初始图像： 
![image](http://static.zybuluo.com/EtoDemerzel/vdmjv77mgp6yr2lppgkm989f/image_1bvhdo54a1obu1c561jua1uui7uk3t.png)
进行10次迭代后的图像：![image](http://static.zybuluo.com/EtoDemerzel/nn68hj7np0y1756ntv4ms9qk/image_1bvhdoub019fjfj0en71k6o8ri4a.png)

可以看到三堆点被很好地分成了三类。图片上同时也展示了中心点的移动轨迹。
1.3 Random initialization

ex7.m中为了方便检验结果正确性，给定了K的初始化。而实际应用中，我们需要随机初始化。 

kMeansInitCentroids.m 完成如下代码：

```
randidx = randperm(size(X, 1));
% Take the first K examples as centroids
centroids = X(randidx(1:K), :);
```

1.4 Image compression with K-means

用K-means进行图片压缩。 

用一张128\times 128的图片为例，采用RGB，总共需要128\times 128 \times 24 = 393216个bit。 

这里我们对他进行压缩，把所有颜色分成16类，以其centroid对应的颜色代替整个一类中的颜色，可以将空间压缩至16\times 24 + 128\times 128 \times 4 = 65920 个bit。 

用题目中提供的例子，效果大概如下：
![image](http://static.zybuluo.com/EtoDemerzel/4rgucvo4a6oysg20fpewlall/image_1bvheqvbshfk1j1318ll1pis15lo5n.png)

#### 2主成分分析

在这个练习中，您将使用主成分分析（PCA）来执行降维。 您将首先尝试使用示例2D数据集来直观了解PCA如何工作，然后将其用于5000张面部图像数据集的较大数据集。

所提供的脚本ex7 pca.m将帮助您逐步完成练习的前半部分。

先对例子中的二维向量实现降低到一维。 

绘制散点图如下：![image](http://static.zybuluo.com/EtoDemerzel/stqmgizpaqhmp73fmcb7bh0g/image_1bvhfjdid1ri0ihgghc12qh5826h.png)
2.2 Implementing PCA

首先需要计算数据的协方差矩阵（covariance matrix）。 

然后使用 Octave/MATLAB中的SVD函数计算特征向量（eigenvector）。

可以先对数据进行normalization和feature scaling的处理。 

协方差矩阵如下计算：![image](http://static.zybuluo.com/EtoDemerzel/wfh2ygl1rqbhbsg146pu6yfc/image_1bvhi4bs61c8m169qagiaf4osb7b.png)
然后用SVD函数求特征向量。 

故完成pca.m如下：
`[U,S,V] = svd(1/m * X' * X);`
把求出的特征向量绘制在图上： 
![image](http://static.zybuluo.com/EtoDemerzel/rm3px2nsux8e333lzhtlgkx4/image_1bvhivt7a1c3pijbr46183r1d5k88.png)
2.3 Dimensionality reduction with PCA

将高维的examples投影到低维上。

2.3.1 Projecting the data onto the principal components

完成projectData.m如下：

```
Ureduce = U(:,1:K);
Z = X * Ureduce;
```

2.3.2 Reconstructing an approximation of the data

从投影过的低维恢复高维recoverData.m：

```
Ureduce = U(:, 1:K);
X_rec = Z * Ureduce';
```

2.3.3 Visualizing the projections

![image](http://static.zybuluo.com/EtoDemerzel/5b55d0q62ucupvc78q6qvjsf/image_1bvhq0b1j1cj36k6el8ars19rb8l.png)

根据上图可以看出，恢复后的图只保留了其中一个特征向量上的信息，而垂直方向的信息丢失了。

2.4 Face image dataset

对人脸图片进行dimension reduction。ex7faces.mat中存有大量人脸的灰度图（32 \times 32) , 因此每一个向量的维数是 32 \times 32 = 1024。 

如下是前一百张人脸图： 
![image](http://static.zybuluo.com/EtoDemerzel/4htax2yujv331naz6714beou/image_1bvhq9lgl8tr18sc1nj91ek983r9i.png)
2.4.1 PCA on faces

用PCA得到其主成分，将其重新转化为 32\times 32 的矩阵后，对其可视化，如下：(只展示前36个）

![image](http://static.zybuluo.com/EtoDemerzel/khg9umw7zuiyp6nlcwxnvc8t/image_1bvhqqr4a1ha7vdi1p61ufgh0e9v.png)

2.4.2 Dimensionality reduction

取前100个特征向量进行投影，

![image](http://static.zybuluo.com/EtoDemerzel/zwc7byhk6oh4uu7v67r2fvga/image_1bvhqupea1jc681t1g6018eb10leac.png)

可以看出，降低维度后，人脸部的大致框架还保留着，但是失去了一些细节。这给我们的启发是，当我们在用神经网络训练人脸识别时，有时候可以用这种方式来提高速度。





















