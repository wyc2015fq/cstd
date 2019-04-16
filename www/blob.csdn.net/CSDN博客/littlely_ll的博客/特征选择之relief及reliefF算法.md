# 特征选择之relief及reliefF算法 - littlely_ll的博客 - CSDN博客





2017年05月11日 12:01:57[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：9171
所属专栏：[特征提取与选择](https://blog.csdn.net/column/details/15615.html)









# relief算法

Relief算法最早由Kira提出，最初局限于两类数据的分类问题。Relief算法是一种特征权重算法(Feature weighting algorithms)，根据各个特征和类别的相关性赋予特征不同的权重，权重小于某个阈值的特征将被移除。Relief算法中特征和类别的相关性是基于特征对近距离样本的区分能力。算法从训练集D中随机选择一个样本R，然后从和R同类的样本中寻找最近邻样本H，称为Near Hit，从和R不同类的样本中寻找最近邻样本M，称为NearMiss，然后根据以下规则更新每个特征的权重：如果R和Near Hit在某个特征上的距离小于R和Near Miss上的距离，则说明该特征对区分同类和不同类的最近邻是有益的，则增加该特征的权重；反之，如果R和Near Hit在某个特征的距离大于R和Near Miss上的距离，说明该特征对区分同类和不同类的最近邻起负面作用，则降低该特征的权重。以上过程重复m次，最后得到各特征的平均权重。特征的权重越大，表示该特征的分类能力越强，反之，表示该特征分类能力越弱。Relief算法的运行时间随着样本的抽样次数m和原始特征个数N的增加线性增加，因而运行效率非常高。

假设一个样例X有p个特征，S为样本量为n的训练样本集，$F即\{f_1,f_2,...,f_p\}$为特征集，一个样例X由p维向量$(x_1,x_2,...,x_p)$构成，其中，$x_j$为X的第j个特征的值。 

relief算法可以解决名义变量和数值变量，两个样例X和Y的特征的值的差可由下面的函数来定义： 
**当$x_k和y_k$为名义变量时**


$diff(x_k,y_k)=\begin{cases}1 & {如果x_k和y_k不相同}\\0 & {如果x_k和y_k相同}\end{cases}$
**当$x_k和y_k$为数值变量时**


$diff(x_k,y_k)=(x_k-y_k)/\nu_k$
$\nu_k$为归一化单位，把diff值归一到[0,1]区间，可以在之前先把数值变量进行归一化。
relief在下列情况有效：（1）相关性水平对于相关的特征很大，对于不相关的特征很小，（2）$\tau$用来选择相关特征，去除不相关特征。 

relief计算复杂度：$\Theta(pmn)$,p为特征数，m为迭代次数，n为样例数

**relief算法：**

输入：样本集S,抽样次数m,特征权重阈值$\tau$

输出：选择后的特征集

把S分成$S^+$={正例}和$S^-$={负例} 

权重W=(0,0,…,0) 

For i = 1 to m 
$\quad$随机选择一个样例$X\in S$
$\quad$随机选择一个距离X最近邻的一个正例$Z^+\in S^+$
$\quad$随机选择一个距离X最近邻的一个负例$Z^-\in S^-$
$\quad$if X是一个正例 
$\quad$$\quad$then Near-hit=$Z^+$; Near-miss=$Z^-$
$\quad$$\quad$else Near-hit=$Z^-$;Near-miss=$Z^+$
$\quad$for i = 1 to p 
$\quad$$\quad$$W_i=W_i-diff(x_i,near-hit_i)^2+diff(x_i,near-miss_i)^2$

relevance=$\frac{1}{m}W$

for i = 1 to p 
$\quad$if $relevance_i \ge \tau$
$\quad$$\quad$then $f_i$是一个相关的特征 
$\quad$$\quad$else  $f_i$不是相关的特征
# reliefF算法

由于Relief算法比较简单，但运行效率高，并且结果也比较令人满意，因此得到广泛应用，但是其局限性在于只能处理两类别数据，因此1994年Kononeill对其进行了扩展，得到了ReliefF作算法，可以处理多类别问题。该算法用于处理目标属性为连续值的回归问题。ReliefF算法在处理多类问题时，每次从训练样本集中随机取出一个样本R，然后从和R同类的样本集中找出R的k个近邻样本(near Hits)，从每个R的不同类的样本集中均找出k个近邻样本(near Misses)，然后更新每个特征的权重，如下式所示： 


$W(A)=W(A)-\Sigma_{j=1}^kdiff(A,R,H_j)/(mk)+\Sigma_{C\notin class(R)}[\frac{p(C)}{1-p(class(R))}\Sigma_{j=1}^kdiff(A,R,M_j(C))]/(mk)$

上式中，$diff(A,R_1,R_2)$表示样本$R_1和R_2$在特征A上的差，$M_j(C)$表示类$C\notin class(R)$中第j个最近邻样本。如下式： 


$diff(A,R_1,R_2)=\begin{cases}\frac{|R_1[A]-R_2[A]|}{max(A)-min(A)} & \text{If A Is Continuous}\\0 & If A Is Discrete And  R_1[A]=R_2[A]\\1 & if A Is Discrete And R_1[A] \ne R_2[A]\end{cases}$
**reliefF算法：**

输入：训练集D，抽样次数m，特征权重阈值$\delta$，最近邻样本个数k， 

输出：各个特征的特征权重T。 

1. 置所有特征权重为0，T为空集 

2. for i = 1 to m 
$\qquad$从D中随机选择一个样本R； 
$\qquad$从R的同类样本集中找到R的k个最近邻$H_j(j=1,2,..,k)$,从每一个不同类样本集中找到k个最近邻$M_j(C)$; 

3. for A=1 to N(all features) 
$\qquad$$W(A)=W(A)-\Sigma_{j=1}^kdiff(A,R,H_j)/(mk)+\Sigma_{C\notin class(R)}[\frac{p(C)}{1-p(class(R))}\Sigma_{j=1}^kdiff(A,R,M_j(C))]/(mk)$

end.






