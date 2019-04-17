# 主题模型LDA - 浅梦的博客 - CSDN博客





2017年08月31日 09:59:36[浅梦s](https://me.csdn.net/u012151283)阅读数：308








# 多项分布和狄利克雷分布

## 多项分布

某随机实验如果有k个可能结局A1、A2、…、Ak，分别将他们的出现次数记为随机变量X1、X2、…、Xk，它们的概率分布分别是p1，p2，…，pk，那么在n次采样的总结果中，A1出现n1次、A2出现n2次、…、Ak出现nk次的这种事件的出现概率P有下面公式： 


$P(X_1=n_1,...,X_k=n_k)=\begin{cases}\frac{n!}{n_1!...n_k!}p_1^{n_1}...p_k^{n_k}&,\sum_{i=1}^kn_i=n; \\0& ,\text{otherwise}\end{cases}$

另一种形式写为： 


$P(X_1=n_1,...,X_k=n_k)=\begin{cases}n!\prod\limits_{i=1}^k\frac{p_i^{n_i}}{{n_i!}}&,\sum_{i=1}^kn_i=n; \\0& ,\text{otherwise}\end{cases}$
多项分布可以看作时候二项分布推广到多维的形式

## 狄利克雷分布

dirichlet distribution就是由2种结果bernoulli trial导出的beta distribution外推到k种的generalization 

K阶段狄利克雷分布的概率密度函数如下： 
$f(x_1,...,x_K;a_1,...,a_K)=\frac{1}{B(\overrightarrow a)}\prod_{k=1}^{K}p_k^{a_k-1},p_k\in[0,1]$

简记为 
$Dir(\overrightarrow p|\overrightarrow a)=\frac{1}{B(\overrightarrow a)}\prod_{k=1}^{K}p_k^{a_k-1}$,其中 
$B(\overrightarrow a)=\frac{\prod\limits_{k=1}^K\Gamma(a_k)}{\Gamma(\sum\limits_{k=1}^Ka_k)}$

期望 
$E(p_i)=\frac{a_i}{\sum\limits_{k=1}^Ka_k}$

协方差 
$Cov(p_i,p_j)=\frac{a_ia_0[i=j]-a_ia_j}{a_0^2(a_0+1)}$
$a_0=\sum_{k=1}^Ka_k$
## 对称狄利克雷分布

在对称狄利克雷分布中所有$a_i$的取值相同，所以分布可以由唯一的$a_k$和阶数K确定。 
$Dir(\overrightarrow p|a,K)=\frac{1}{B_K(a)}\prod\limits_{k=1}^Kp_k^{a-1}$

其中 
$B_k(a)=\frac{\Gamma^K(a_k)}{\Gamma(K\cdot a)}$

对称狄利克雷分布性质- 当$a=1$，退化为均匀分布（类比Beta(1,1)）
- 当$a>1$时，$p_1=p_2=...=p_k$的概率增加（更偏向于各分量取值相同）
- 当$a<1$时，$p_i=1,p_{-i}=0$的概率增大，（偏向于某分量取值更大）

## 共轭性质

类比于二项分布的共轭先验是$Beta$分布，多项分布的共轭先验是狄利克雷分布。 

假设参数$x=(x_1,x_2,...,x_k)$有先验分布$Dir(K,a_1,...,a_k)$，即 
$p(x;a_1,...,a_k)=\frac{1}{B(a)}\prod\limits_{i=1}^kx_i^{a_i-1}$

另有似然函数 
$p(y|x)\sim Multi(x)$

则后验概率 
$p(x|y)\sim\frac{1}{Z}\prod\limits_{i=1}^kx_i^{a_i+n_i-1}$

与Dirichlet分布形式一致。 
![这里写图片描述](https://img-blog.csdn.net/20171211172534882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 主题模型

主题模型是一族生成式有向图模型，主要用于处理离散型的数据（如文本集合）。LDA是主题模型的典型代表。

## 基本概念

词word是待处理数据的基本离散单元。 

文档document是待处理的数据对象，由一组词组成，这些词在文档是**不计顺序**的。 

话题topic表示一个概念，表示为一系列相关的词，以及它们在该概念下出现的概率。
## LDA中的两个关键要素
- 一系列关于词语的分布（topics）
- 每个文档有一个话题的分布

## LDA生成过程
- 生成每个话题，就是一个词语的分布 
$\beta_k\sim \text{Dirichlet}(\gamma),k=1,...,K$
- 对于每一个文档，生成一个关于话题的分布 
$\theta_d\sim\text{Dirichelet}(a),d=1,...,D$
- 对于第d篇文档中的第n各词语$x_{dn}$
- 将其分配给一个主题 
$c_{dn}\sim\text{Discrete}(\theta_d)$
- 从选择的话题中生成词语 
$x_{dn}\sim\text{Disccrete}(\beta c_{dn})$

# LDA和矩阵分解

对于一篇特定的文档d，如何计算$P(x_{dn}=i|\beta,\theta_d)$? 

通过将话题的簇分配积分得到。 


$\begin{align*}P(x_{dn}=i|\beta,\theta)&=\sum\limits_{k=1}^KP(x_{dn}=i,c_{dn}=k|\beta,\theta_d)\\&=\sum\limits_{k=1}^KP(x_{dn}=i|\beta,c_{dn}=k)P(c_{dn}=k|\theta_d)\\&=\sum\limits_{k=1}^K\beta_{ki}\cdot\theta_{d_k}\end{align*}$

现在令$B=[\beta_1,...,\beta_K]$，$\Theta=[\theta_1,...,\theta_D]$，则$P(x_{dn}=i|\beta,\theta)=(B\Theta)_{id}$

换句话说，我们可以通过一个由两个含有非负项的矩阵相乘得到的矩阵得到。









