# 条件随机场（CRF） - wsp_1138886114的博客 - CSDN博客





2018年08月21日 15:39:41[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：161








### 一、条件随机场（Conditional Random Fields）。

条件随机场（CRF）是在给定一组输入随机变量条件下另外一组输出随机变量的条件概率分布模型，它是一种判别式的概率无向图模型，既然是判别式，那就是对条件概率分布建模。 CRF的输出随机变量是一个概率无向图模型。

##### 1.1 概率图模型
- 常用的概率图模型分为两类：
有向图（贝叶斯网络、信念网络）. 

无向图（马尔可夫随机场、马尔可夫网络、条件随机场（CRF））
![这里写图片描述](https://img-blog.csdn.net/20180807183546933?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 1.2 马尔可夫性

##### 1.3 团与最大团

![这里写图片描述](https://img-blog.csdn.net/20180807183955655?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 1.4 概率无向图模型的联合概率分布

##### 1.5 条件随机场

##### 1.6 线性链条件随机场

注意在CRF的定义中，我们并没有要求X和Y有相同的结构。 

实现中，一般都假设 $X=(X_1,X_2,...X_n),~~~Y=(Y_1,Y_2,...Y_n)$ 为线性链表示的随机变量序列。  

在给定随机变量序列X的情况下，随机变量Y的条件概率分布P(Y|X)构成条件随机场，即满足马尔科夫性： 


$P(Y_i|X,Y_1,Y_2,...Y_n)=P(Y_i|X,Y_{i−1},Y_{i+1})$
则称P(Y|X)为线性链条件随机场（linear-CRF）。 
![这里写图片描述](https://img-blog.csdn.net/20180807182249851?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2.条件随机场的不同形式

##### 2.1 条件随机场的参数化形式
- 在linear-CRF中，特征函数分为两类：
**第一类**是定义在Y节点上的节点特征函数，这类特征函数只和当前节点有关，记为： 


$s_l(y_i,x,i),~~~l=1,2,...L$
其中L是定义在该节点的节点特征函数的总个数，$i$是当前节点在序列的位置。

**第二类**是定义在Y上下文的局部特征函数，这类特征函数只和**当前节点和上一个节点有关**，记为： 


$t_k(y_{i−1},y_i,x,i),~~~k=1,2,...K$

其中K是定义在该节点的局部特征函数的总个数，i是当前节点在序列的位置。之所以只有上下文相关的局部特征函数，没有不相邻节点之间的特征函数，是因为我们的linear-CRF满足马尔科夫性。

无论是节点特征函数还是局部特征函数，它们的取值只能是0或者1。即满足特征条件或者不满足特征条件。  

同时，我们可以为每个特征函数赋予一个权值，用以表达我们对这个特征函数的信任度。假设tk的权重系数是$λ_k,s_l$ 的权重系数是$μ_l$,则linear-CRF由我们所有的$t_k,λ_k,s_l,μ_l$共同决定。

　　　　此时我们得到了linear-CRF的参数化形式如下： 


$P(y|x)=\frac{1}{Z(x)}exp(∑_{i,k}λ_kt_k(y_{i−1},y_i,x,i)+∑_{i,l}μ_ls_l(y_i,x,i))$

　　　　其中，Z(x)为规范化因子： 


$Z(x)=∑exp(∑i,kλ_kt_k(y_{i−1},y_i,x,i)+∑_{i,l} μ_ls_l(y_i,x,i))$

　　　　回到特征函数本身，每个特征函数定义了一个linear-CRF的规则，则其系数定义了这个规则的可信度。所有的规则和其可信度一起构成了我们的linear-CRF的最终的条件概率分布。
2.2 条件随机场的简化形式 

2.3 条件随机场的矩阵形式

![这里写图片描述](https://img-blog.csdn.net/2018080717331599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**判别式模型** 与 **产生式模型** 的区别 
![这里写图片描述](https://img-blog.csdn.net/20180807172823700?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






