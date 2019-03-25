# PQ(product quantization) 算法---(二)

2018年07月08日 17:14:38 [hairuiJY](https://me.csdn.net/u010368556) 阅读数：816



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/u010368556/article/details/80960661

**PQ（productquantization）算法**：乘积量化，这里的乘积为笛卡尔积；从字面理解包括了两个过程**特征的分组量化过程**和**类别的笛卡尔积过程**。它属于ANN(approximatenearest neighbor)算法。与它相关的算法有E2LSH（EuclideanLocality-SensitiveHashing）， KD-trees，K-means。

**假设有一个数据集：** 由n个D维向量组成（如：n行D列）

**K-means：**的做法就是给定类别数目K，对整个数据集进行聚类，目标函数是所有样本到类中心的距离和最小，迭代计算优化目标函数，得到**K个类中心**和**每个样本所属的类别**。

**PQ算法：**目标函数不变和K-means一样，是所有样本到类中心的距离和最小，迭代计算优化目标函数；**但是**它把原来的向量（n * D）空间分解为若干个低维向量空间的笛卡尔积，并对分解得到的低维向量空间分别做量化（quantization）。这样每个向量就能由多个低维空间的量化code组合表示。过程如下：

（1）数据集为K个类别，每个样本以一个vector的形式表示，维数为d，将vector的各个分量分成m组；

（2）将所有vector的某组分量作为数据集，采用k-means算法得到![img](https://img-blog.csdn.net/20180708171208658?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAzNjg1NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)个类中心，运行m次k-means算法，则每组都有![img](https://img-blog.csdn.net/20180708171139248?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAzNjg1NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)个类中心，记这![img](https://img-blog.csdn.net/20180708171218122?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAzNjg1NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)个类中心为一个集合。

（3）将上述得到的m个集合做笛卡尔积，就得到整个数据集的类中心了。

 ![img](https://img-blog.csdn.net/20180708171232637?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAzNjg1NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 参考：

<https://www.cnblogs.com/mafuqiang/p/7161592.html>

<https://blog.csdn.net/CHIERYU/article/details/50321473>

 