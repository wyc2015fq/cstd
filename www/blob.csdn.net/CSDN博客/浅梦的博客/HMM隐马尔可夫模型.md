# HMM隐马尔可夫模型 - 浅梦的博客 - CSDN博客





2017年08月27日 17:46:17[浅梦s](https://me.csdn.net/u012151283)阅读数：144
个人分类：[机器学习](https://blog.csdn.net/u012151283/article/category/6653295)









# 隐马尔科夫模型的基本概念

## 定义

HMM是关于时序的概率模型，描述又一个隐藏的马尔科夫链随机生成不可观测的状态随机序列，再由各个状态生成一个观测而产生观测随机序列的过程。

## 基本概念
- 状态序列 

隐藏的马尔科夫链随机生成的状态的序列
- 观测序列 

每个状态生成一个观测，而由此产生的观测的随机序列。序列的每个位置可以看作是一个时刻。

HMM由**初始概率分布，状态转移概率分布以及观测概率分布**确定。 

设$Q$是所有可能的状态的集合，$V$是所有可能的观测的集合。 
$Q=[q_1,q_2,...,q_N],V=[v_1,v_2,...,v_M]$
$I$是长度为$T$的状态序列，$O$是对应的观测序列。 
$I=[i_1,i_2,...,i_T],V=[o_1,o_2,...,o_T]$
$A$是状态转移概率矩阵 
$A=[a_{ij}]_{N \times M}$， 

其中， 
$a_{ij}=P(i_{t+1}=q_j|i_t=q_i)$是在时刻$t$处于状态$q_i$的条件下在时刻$t+1$转移到状态$q_j$的概率。 
$B$是观测概率矩阵 
$B=[b_j(k)]_{N \times M}$， 

其中， 
$b_j(k)=P(o_t=v_k|i_t=q_j)$是在时刻$t$处于状态$q_j$的条件下生成观测$v_k$的概率 
$\pi$是初始状态概率向量： 
$\pi=(\pi_i)$

其中$\pi_i=P(i_1=q_i)$是时刻$t=1$处于状态$q_i$的概率。
隐马尔科夫模型$\lambda$可以用三元符号表示，即 
$\lambda=(A,B,\pi)$，称为隐马尔科夫模型的三要素。

## 两个基本假设

隐马尔科夫模型做了两个基本假设： 

(1) 齐次马尔科夫性假设，即假设隐藏的马尔科夫链在任意时刻t的状态只依赖于其前一时刻的状态，与其他时刻的状态及观测无关，也与时刻t无关。 
$P(i_t,|i_{t-1},0_{t-1},...,i_1,o_1)=P(i_t|i_{t-1}),t=1,2,...,T$

(2)观测独立性假设，即假设任意时刻的观测只依赖于该时刻的马尔科夫链的状态，与其他观测及状态无关。 
$P(o_t|i_T,o_T,i_{T-1},o_{T-1},...,i_{t+1},o_{t+1},i_t,i_{t-1},o_{t-1},...,i_1,o_1)=P(o_t|i_t)$
## 三个基本问题
- 概率计算问题 

给定模型$\lambda = (A,B,\pi)$和观测序列$O=(o_1,o_2,...,o_T)$，计算在模型$\lambda$下观测序列$O$出现的概率$P(O|\lambda)$
- 学习问题 

已知观测序列$O=(o_1,o_2,...,o_T)$,估计模型$\lambda =(A,B,\pi)$参数，使得在该模型下观测序列概率$P(O|\lambda)$最大。即用极大似然估计得方法估计参数。
- 预测问题 

也称为解码(decoding)问题。已知模型$\lambda=(A,B,\pi)$和观测序列$O=(o_1,o_2,...,o_T)$，求队给定观测序列条件概率$P(I|O)$最大的状态序列$I=(i_1i_2,...,i_T)$。即给定观测序列，求最有可能的对应的状态序列。

# 概率计算算法

![这里写图片描述](https://img-blog.csdn.net/20171207204103350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 前向算法

![这里写图片描述](https://img-blog.csdn.net/20171207204328088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171207204430617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171207204617154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 后向算法

![这里写图片描述](https://img-blog.csdn.net/20171207204726304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171207204808007?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 一些概率与期望值的计算

# 学习算法

根据训练数据是包括观测序列和对应的状态序列还是只有观测序列，可以分别由监督学习与非监督学习实现。

## 监督学习方法

![这里写图片描述](https://img-blog.csdn.net/20171207205949693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Baum-Welch算法(EM算法)

![这里写图片描述](https://img-blog.csdn.net/20171207210800086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171207210958069?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 预测算法

分为近似算法和维特比算法。

## 近似算法

![这里写图片描述](https://img-blog.csdn.net/20171207211111322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 维特比算法








