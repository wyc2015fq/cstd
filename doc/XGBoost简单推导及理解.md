# XGBoost简单推导及理解

2018年04月04日 00:39:47

小鹅鹅



### 前言

XGBoost的全称是eXtreme Gradient Boosting。作为一个非常有效的机器学习方法，Boosting  Tree是数据挖掘和机器学习中最常用的算法之一。因为它效果好，对于输入要求不敏感，相对LR  的优势如不需要做特征的归一化，自动进行特征选择，模型可解释性较好，可以适应多种损失函数如 SquareLoss，LogLoss  等，往往是从统计学家到数据科学家必备的工具之一，它同时也是kaggle比赛冠军选手最常用的工具。最后，因为它的效果好，在计算速度和准确率上，较GBDT有明显的提升计算复杂度不高，也在工业界中有大量的应用。

前置知识：[GBDT](https://blog.csdn.net/asd136912/article/details/78556362)

### 模型函数形式

给定数据集$\mathcal D={(x_i, y_i)}$ ,XGBoost进行additive training，学习$K$棵树，采用以下函数对样本进行预测： 
$$
\hat y_i=\phi(x_i)=\sum^K_{k=1}f_k(x_i),f_k\in \mathcal F
$$
这里$F$ 是假设空间，是CART回归树 
$$
\mathcal F=\{f(x)=w_{q(x)}\}(q:\Bbb R^m\to T,w\in\Bbb R^T)
$$


**注意**：这里$q(x)$表示将样本x分到了某个叶子节点上，w是叶子节点的分数（leaf score），所以 $w_{q(x)}$ 表示回归树对样本的预测值。这些定义在之后会用到。

### 目标函数

XGBoost的目标函数（函数空间）为 
$$
\mathcal L(\phi)=\sum_{i=1}^Nl(y_i,\hat y_i) + \Omega(f_k)
$$
 其中$l(y_i,\hat y_i)$ 为误差函数，$\Omega(f_k)$为正则项，对每棵回归树的复杂度进行了惩罚。

那么有哪些指标可以衡量树的复杂度？

- 树的深度，内部节点个数，**叶子节点个数(T)**，**叶节点分数(w)**..

- XGBoost用的是 
  $$
  \Omega(f_k)=\gamma T+\frac{1}{2}\lambda||w||^2
  $$
   对叶子节点个数和叶节点分数进行惩罚，相当于在训练过程中做了剪枝。

### 误差函数的泰勒二阶展开

第t次迭代之后，模型的的预测等于前t-1次的模型预测加上第t棵树的预测： 
$$
\hat y_i^{(t)} = \hat y_i^{(t-1)} +f_t(x_i)
$$
 此时目标函数可写作： 
$$
\mathcal L^{(t)}=\sum^n_{i=1}l(y_i,\hat y_i^{(t-1)} +f_t(x_i))+\Omega (f_t)
$$
 公式中$y_i,\hat y_i^{(t-1)}$ 都已知，模型要学习的只有第t棵树$f_t$ 

将误差函数在$\hat y_i^{(t-1)}$ 处进行二阶泰勒展开： 
$$
\mathcal L^{(t)}\approx \sum^n_{i=1}l(y_i,\hat y_i^{(t-1)} )+g_if_t(x_i)+\frac{1}{2}h_if_t^2(x_i)+\Omega (f_t)
$$
 其中 $g_i=\partial_{\hat y^{(t-1)}}l(y_i,\hat y_i^{(t-1)}), h_i=\partial^2_{\hat y^{(t-1)}}l(y_i,\hat y_i^{(t-1)})$

将公式中的常数项去掉，得到: 
$$
\hat L^{(t)}=\sum^n_{i=1}[g_if_t(x_i)+\frac{1}{2}h_if_t^2(x_i)]+\Omega (f_t)
$$
把$f_t,\Omega(f_t)$写成树结构的形式，即把下式代入目标函数中 

$f(x)=w_{q(x)}, \Omega(f)=\gamma T + \frac{1}{2}\lambda||w||^2$

得到： 
$$
\hat L^{(t)}=\sum^n_{i=1}[g_iw_{q(x_i)}+\frac{1}{2}h_iw^2_{q(x_i)}]+\gamma T + \lambda\frac{1}{2}\sum^T_{j=1}w_j^2
$$
其中$\sum^n_{i=1}[g_iw_{q(x_i)}+\frac{1}{2}h_iw_{q(x_i)}^2]$是对样本累加，$\lambda\frac{1}{2}\sum^T_{j=1}w_j^2$是对叶节点累加。

统一起来：定义每个叶节点j上的样本集合为$I_j=\{i|q(x_i)=j\}$
 则目标函数可以写成按叶节点累加的形式： 
$$
\hat L^{(t)}=\sum^T_{j=1}[(\sum_{i \in I_j}g_i)w_j+\frac{1}{2}(\sum_{i \in I_J}h_i+\lambda)w_j^2]+\gamma T \\ =\sum^T_{j=1}[(G_jw_j+\frac{1}{2}(H_j+\lambda)w_j^2]+\gamma T
$$
如果确定了树的结构（即$q(x)$确定），为了使目标函数最小，可以令其导数为0，解得每个叶节点的最优预测分数为: 
$$
w^*_j=-\frac{G_j}{H_j+\lambda}
$$
 代入目标函数，得到最小损失为： 
$$
\hat L^*=-\frac{1}{2}\sum^T_{j=1}\frac{G_j^2}{H_j+\lambda}+\gamma T
$$




### 学习策略

ID3算法采用信息增益 
 C4.5算法采用信息增益比 
 CART分类树采用Gini系数

在XGBoost中，参考上式，$\frac{G_j^2}{H_j+\lambda}$部分衡量了每个叶子节点对总体损失的的贡献，我们希望损失越小越好，则标红部分的值越大越好。

因此，对一个叶子节点进行分裂，分裂前后的增益定义为： 
$Gain=\frac{G^2_L}{H_L+\lambda}+\frac{G^2_R}{H_R+\lambda}-\frac{(G^2_L+G_R)^2}{H_L+H_R+\lambda}-\gamma$
 Gain的值越大，分裂后 L 减小越多。所以当对一个叶节点分割时，计算所有候选(feature,value)对应的Gain，选取Gain最大的进行分割。

### 树节点分裂方法(Split Finding)

- **精确算法**:遍历所有特征的所有可能的分割点，计算gain值，选取值最大的（feature，value）去分割 
   ![这里写图片描述](https://img-blog.csdn.net/20180404002640889?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

- **近似算法**: 对于每个特征，只考察分位点，减少计算复杂度 
   ![img](https://img-blog.csdn.net/20180404002720435?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
   • Global：学习每棵树前，提出候选切分点 
   • Local：每次分裂前，重新提出候选切分点

  实际上XGBoost不是简单地按照样本个数进行分位，而是以二阶导数值作为权重(Weighted Quantile Sketch)

### 稀疏值处理

当特征出现缺失值时，XGBoost可以学习出默认的节点分裂方向。 
 ![img](https://img-blog.csdn.net/20180404002852597?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 其他特性

1. **行抽样，列抽样**(借鉴随机森林) ：降低过拟合，减少计算

2. **支持自定义损失函数**(需二阶可导)

3. **Shrinkage**：每次迭代会将叶子节点权重乘以系数，削弱每棵树影响

4. **支持并行**：XGBoost的并行是在特征粒度上的。我们知道，决策树的学习最耗时的一个步骤就是对特征的值进行排序（因为要确定最佳分割点），XGBoost在训练之前，预先对数据进行了排序，然后保存为block结构，后面的迭代中重复地使用这个结构，大大减小计算量。这个block结构也使得并行成为了可能，在进行节点的分裂时，需要计算每个特征的增益，最终选增益最大的那个特征去做分裂，那么各个特征的增益计算就可以开多线程进行。

5. **可并行的近似直方图算法**：树节点在进行分裂时，我们需要计算每个特征的每个分割点对应的增益，即用贪心法枚举所有可能的分割点。当数据无法一次载入内存或者在分布式情况下，贪心算法效率就会变得很低，所以xgboost还提出了一种可并行的近似直方图算法，用于高效地生成候选的分割点。

   把连续的浮点特征值离散化成k个整数，同时构造一个宽度为k的直方图。在 
    遍历数据的时候，根据离散化后的值作为索引在直方图中累积统计量，当遍 
    历一次数据后，直方图累积了需要的统计量，然后根据直方图的离散值，遍 
    历寻找最优的分割点。 
    ![img](https://img-blog.csdn.net/20180404003644963?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
    • 减小内存占用，比如离散为256个bin时，只需要8bit，节省7/8 
    • 减小了split finding时计算增益的计算量， 从O(#data) 

### 系统设计

- **Column Block**

  - 特征预排序，以column block的结构存于内存中。
  - 存储样本索引（instance indices）
  - block中的数据以稀疏格式（CSC）存储

  这个结构加速了split finding的过程，只需要在建树前排序一次，后面节点分裂时直接根据索引得到梯度信息

- **Cache Aware Access**

  - column block按特征大小顺序存储，相应的样本的梯度信息是分散的，造成内存的不连续访问，降低CPU cache 命中率。
  - 缓存优化方法 
    - 预取数据到buffer中（非连续->连续），再统计梯度信息
    - 调节块的大小

![img](https://img-blog.csdn.net/2018040400354818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 参考

1. <http://www.52cs.org/?p=429>
2. <https://blog.csdn.net/a819825294/article/details/51206410>
3. <http://pan.baidu.com/s/1gfA6FK3>
4. <http://learningsys.org/papers/LearningSys_2015_paper_32.pdf>
5. [acm=1488265641_ffdebf36cef2b1bf7f3f76abf6bfe426”>http://delivery.acm.org/10.1145/2940000/2939785/p785-chen.pdf?ip=202.118.228.100&id=2939785&acc=ACTIVE%20SERVICE&key=BF85BBA5741FDC6E.5C4511229FC427D6.4D4702B0C3E38B35.4D4702B0C3E38B35&CFID=905733202&CFTOKEN=53852884&**acm**=1488265641_ffdebf36cef2b1bf7f3f76abf6bfe426](https://blog.csdn.net/asd136912/article/details/"http://delivery.acm.org/10.1145/2940000/2939785/p785-chen.pdf?ip=202.118.228.100&id=2939785&acc=ACTIVE%20SERVICE&key=BF85BBA5741FDC6E.5C4511229FC427D6.4D4702B0C3E38B35.4D4702B0C3E38B35&CFID=905733202&CFTOKEN=53852884&