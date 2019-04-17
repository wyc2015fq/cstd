# CTC学习笔记（二） 训练和公式推导 - xmdxcsj的专栏 - CSDN博客





2016年06月26日 18:57:46[xmucas](https://me.csdn.net/xmdxcsj)阅读数：28533标签：[神经网络](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)
个人分类：[神经网络](https://blog.csdn.net/xmdxcsj/article/category/5855803)









## 整体思路

训练流程和传统的神经网络类似，构建loss function，然后根据BP算法进行训练，不同之处在于传统的神经网络的训练准则是针对每帧数据，即每帧数据的训练误差最小，而CTC的训练准则是基于序列（比如语音识别的一整句话）的，比如最大化$p(z|x)$，序列化的概率求解比较复杂，因为一个输出序列可以对应很多的路径，所有引入前后向算法来简化计算。

## 前期准备
- 输入 
$x$,长度为T
- 输出集合 
$A$表示正常的输出 
$A'=A \bigcup \{blank\}$表示输出全集 
$A'^T$表示输入x对应的输出元素集合- 输出序列 
$\pi$表示输出路径 
$l$表示输出label序列 
$\mathcal{F}$表示路径到label序列的映射关系- 概率 
$y_k^t$表示时间t输出k的概率 
$p(\pi|x)=\displaystyle\prod_{t=1}^{T}y_{\pi_t}^{t}$表示基于输入x的输出$\pi$路径的概率 
$p(l|x)=\displaystyle\sum_{\pi \in \mathcal{F}^{-1}(l)}p(\pi|x)$表示输出label序列的概率是多条路径的概率和。
## 前后向算法

![这里写图片描述](https://img-blog.csdn.net/20160626185316096)

考虑到计算$p(l|x)$需要计算很多条路径的概率，随着输入长度呈指数化增加，可以引入类似于HMM的前后向算法来计算该概率值。 

为了引入blank节点，在label首尾以及中间插入blank节点，如果label序列原来的长度为U，那么现在变为U’=2U+1。
### 前向

前向变量为$\alpha(t,u)$，表示t时刻在节点u的前向概率值，其中$u\in [1,2U+1]$. 

初始化值如下： 


$\alpha(1,1)=y_b^1$


$\alpha(1,2)=y_{l_1}^1$


$\alpha(1,u)=0, \forall u>2$

递推关系： 


$\alpha(t,u)=y_{l'_{u}}^t\displaystyle\sum_{i=f(u)}^{u}\alpha(t-1,i)$

其中 


$f(u) =  \begin{cases}    u-1       & \quad \text{if } l'_u=blank \text{ or }l'_{u-2}=l'_u\\    u-2      & \quad \text{otherwise}\\  \end{cases}$

注：如果l表示{c,a,t}，那么l’表示为{b,c,b,a,b,t,b}，所以原来在l中的下标u为2，在l’中的下标u变为4。 


$\alpha(t,u)=0  \forall u< U'-2(T-t)-1$对应于上图中的右上角部分，因为时间的限制，有些节点不可能到达最后的终止节点。 

根据上图，很容易理解前向的递推关系。
### 后向

初始化值： 


$\beta(T,U')=1$


$\beta(T,U'-1)=1$


$\beta(T,u)=0, \forall u<U'-2$


$\alpha(1,u)=0, \forall u>2$

递推关系： 


$\beta(t,u)=\displaystyle\sum_{i=u}^{g(u)}\beta(t+1,i)y_{l'_{i}}^{t+1}$

其中 


$g(u) =  \begin{cases}    u+1       & \quad \text{if } l'_u=blank \text{ or }l'_{u+2}=l'_u\\    u+2      & \quad \text{otherwise}\\  \end{cases}$
### 取log

概率计算在log计算，避免underflow，其中log加可以通过以下形式转化： 


$ln(a+b)=lna+ln(1+e^{lnb-lna})$

## 训练

### loss function

CTC的loss function使用最大似然： 


$L(S)=\displaystyle\sum_{(x,z)\in S}L(x,z)$


$L(x,z)=-lnp(z|x)$

根据前后向变量，可以求得： 


$p(z|x)=\displaystyle\sum_{u=1}^{|z'|}\alpha(t,u)\beta(t,u)$
$|z'|$表示z对应的label长度的U’，$\alpha(t,u)\beta(t,u)$表示t时刻经过节点u的所有路径的概率和。 


$L(x,z)=-ln\displaystyle\sum_{u=1}^{|z'|}\alpha(t,u)\beta(t,u)$
### bp训练

$y_k^t$表示t时刻输出k的概率 
$a_k^t$表示t时刻对应输出节点k在做softmax转换之前的值 


$\frac{\partial L(x,z)}{\partial y_k^t}=-\frac{1}{p(z|x)}\frac{\partial p(z|x)}{\partial y_k^t}$

只需要考虑t时刻经过k节点的路径即可 


$\frac{\partial p(z|x)}{\partial y_k^t}=\displaystyle\sum_{u\in B(z,k)}\frac{\partial \alpha(t,u)\beta(t,u)}{\partial y_k^t}$

其中$B(z,k)$表示节点为k的集合 

考虑到 


$\alpha(t,u)\beta(t,u)=\displaystyle\sum_{\pi \in X(t,u)}\displaystyle\prod_{t=1}^{T}y_{\pi_t}^{t}$

其中$X(t,u)$表示所有在t时刻经过节点u的路径。 

所以 


$\frac{\partial p(z|x)}{\partial y_k^t}=\displaystyle\sum_{u\in B(z,k)}\frac{\alpha(t,u)\beta(t,u)}{y_k^t}$

可以到损失函数对$y_k^t$偏导数 


$\frac{\partial L(x,z)}{\partial y_k^t}=-\frac{1}{p(z|x)y_k^t}\displaystyle\sum_{u\in B(z,k)}{\alpha(t,u)\beta(t,u)}$

同时可以得到损失函数对于$a_k^t$偏导数 


$\frac{\partial L(x,z)}{\partial a_k^t}=y_k^t-\frac{1}{p(z|x)}\displaystyle\sum_{u\in B(z,k)}{\alpha(t,u)\beta(t,u)}$

推导参考： 
![这里写图片描述](https://img-blog.csdn.net/20160626185349499)
![这里写图片描述](https://img-blog.csdn.net/20160626185407437)

后续可以使用[BPTT算法](http://blog.csdn.net/xmdxcsj/article/details/50088967)得到损失函数对神经网络参数的偏导。
## 参考

[《Supervised Sequence Labelling with Recurrent Neural Networks》 chapter7](https://www.cs.toronto.edu/~graves/preprint.pdf)











