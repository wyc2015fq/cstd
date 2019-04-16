# k-prototypes聚类 - littlely_ll的博客 - CSDN博客





2018年04月17日 22:26:40[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1003








聚类有多种方法，常见的有k-means，DBSCAN，分层聚类等，但这些都是处理数值型数据的，而k-modes只是处理分类变量数据，但更多的数据是既含有数值型数据也含有分类数据，因此需要有一种能同时处理两种不同类型数据的聚类方法，而k-prototypes就是这样的一种方法。

## 损失函数

k-prototypes聚类的准则就是使用一个合适的损失函数去度量数值型和分类变量对原型的距离。假设$X=\{X_1,X_2,\cdots,X_n\}$为n个样本集合，$X_i=\{x_{i1},x_{i2},\cdots, x_{im}\}$为样本$i$的$m$个属性值。$k$为聚类个数。而这个损失函数可以定义为： 


$E=\Sigma_{l=1}^k\Sigma_{i=1}^ny_{il}d(X_i, Q_l)\tag{1}$

其中，$Q_l=[q_{l1},q_{l2},\cdots,q_{lm}]$，它表示类别$l$的一个原型（prototype），也就是选择的类别中心。$y_{il}$为切分矩阵$Y$的元素，其实就是在类别$l$中有没有这个样本$i$，有则为1，无则为0，这种形式又叫做**硬分割**。$d$就是相似性度量，经常定义为平方欧氏距离。 

式（1）的内项$E_l=\Sigma_{i=1}^ny_{il}d(X_i,Q_l)$为把$X$分为类别$l$的总损失。当 


$q_{lj}=\frac{1}{n_l}\Sigma_{i=1}^ny_{il}x_{ij}，j=1,...,m\tag{2}$

时，$E_l$达到最小化，其中$n_l=\Sigma_{i=1}^ny_{il}$为类别$l$中样本的个数。 

上面是对于数值型变量，当$X$中含有分类变量时，可以把相似距离改为： 


$d(X_i, Q_l)=\Sigma_{j=1}^{m_r}(x_{ij}^r-q_{lj}^r)^2 + \gamma_l\Sigma_{j=1}^{m_c}\delta(x_{ij}^c,q_{lj}^c)\tag{3}$

其中，当$p=q,\delta(p,q)=0$，当$p\ne q,\delta(p,q)=1$。对于类别$l$中样本$i$，$x_{ij}^r$和$q_{lj}^r$为数值属性，$x_{ij}^c$和$q_{lj}^c$为分类属性，$m_r和m_c$分别为数值变量和分类变量的个数。$\gamma_l$是类别$l$中分类变量的权重。 

对于公式（3），可以看到，这个距离公式是结合了k-means和k-modes的度量距离的方法。因此我们可以把式（1）的内项$E_l$重写为： 


$E_l=\Sigma_{i=1}^ny_{il}\Sigma_{j=1}^{m_r}(x_{ij}^r-q_{lj}^r)^2+\gamma_l\Sigma_{i=1}^ny_{il}\Sigma_{j=1}^{m_c}\delta(x_{ij}^c,q_{lj}^c))\\=E_l^r + E_l^c\tag{4}$

这样，$E_l^r$就代表类别$l$的样本中所有数值属性的总损失，$E_l^c$表示所有分类属性的总损失。 

因此总的损失函数为： 


$E=\Sigma_{l=1}^k(E_l^r+E_l^c)=E^r+E^c\tag{5}$

要求$E$的最小化，就是对数值属性和分类属性分别按照k-means和k-modes的距离计算方法计算，即一个使用欧式平方距离，一个使用汉明距离。 

但是$\gamma_l$也影响聚类的准确性。当$\gamma_l=0$时，只考虑数值变量，相当于k-means，当$\gamma_l$越大，分类变量占的权重越大，聚类结果则由分类变量占主导作用。因此选择合适的$\gamma_l$也是良好聚类的开始。而HUANG的文章指出$\gamma_l$依赖于样本数值属性的标准差$\sigma_l$,因此可把$\sigma_l$作为$\gamma_l$的引导。但是HUANG没有给出具体地方法。
## k-prototypes算法

具体步骤： 

（1）从数据集$X$中对每个聚类选择一个原型（中心样本） 

（2）根据式（3）把$X$中的样本分配到距离原型最近的那个类别中，每次分配后，更新类别的原型 

（3）在所有的样本都分到各自的类中后，重新计算样本到现在原型的距离。如果一个样本距离新的原型比原来的原型要近，那就重新分配到新的原型的类别中去 

（4）重复（3）过程，直到没有样本改变类别
算法包含三个过程：初始化原型选择，初始分配和再分配。 

下面两图为初始分配和再分配的伪代码。 
![这里写图片描述](https://img-blog.csdn.net/20180417212715302?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180417212807504?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解释： 
$X[i]$表示样本$i$，$X[i,j]$为样本$i$的第$j$个属性。O_prototypes[]和C_rototypes[]分别存储数值型和分类型的原型，O_prototypes[i,j]和C_prototypes[i,j]分别代表类别i中原型的数值和分类变量的第j个属性。Distance()和Sigma()分别为计算数值和分类型变量的距离。Clustership[]记录样本的类别，ClusterCount[]记录类别中样本的个数。SumInCluster[]对类别中样本的数值变量求和并用于更新类原型的数值属性，FrequencyInCluster[]记录类别中分类属性的不同取值的个数。HighestFreq()用于更新原型的分类属性。 

k-prototypes算法可能陷入局部最优，因此在运算过程中可以加入一些扰动，或使用模拟退火或遗传算法，但具体怎么实现HUANG没有说明，只是指定了参考文献。 

使用k-prototypes时间复杂度比较高，为O((t+1)kn)，n为样本个数，k为聚类个数，t为再分配过程中迭代次数。在实际实验中，30000个数据聚成8类耗时824秒。70000个数据耗时2051秒。对于30000个数据，聚成16类耗时2038秒，聚成64类耗时1074秒。可见用时非常长了。
**参考文献**

【ZHEXUE HUANG】CLUSTERING LARGE DATA SETS WITH MIXED NUMERIC AND CATEGORICAL VALUES*







