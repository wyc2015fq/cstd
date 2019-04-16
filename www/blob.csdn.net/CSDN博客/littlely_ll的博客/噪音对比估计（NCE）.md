# 噪音对比估计（NCE） - littlely_ll的博客 - CSDN博客





2018年02月04日 12:52:40[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：4936








噪音对比估计（NCE, Noise Contrastive Estimation）是一种新的统计模型估计方法，由Gutmann和Hyv¨arinen提出来，能够用来解决神经网络的复杂计算问题，因此在图像处理和自然语言处理中得到广泛应用。在介绍NCE之前，首先复习一些基础知识。

# 全概率公式

设实验$E$的样本空间为$S$，$A$为$E$的事件，$B_1,B_2,\cdots, B_n为S$的一个划分，且$P(B_i)>0 (i=1,2,\cdots, n)$，则： 


$P(A)=P(A|B_1)P(B_1)+P(A|B_2)P(B_2)+\cdots + P(A|B_n)P(B_n)\tag{1}$

式（1）为全概率公式
# (0-1)分布对数似然函数

假设$p$服从(0-1)伯努利分布，$x_1,x_2,\cdots,x_n$为样本量$X_1,X_2,\cdots,X_n$的样本值，则$p$的似然函数为$L(\theta)=\Pi_{i=1}^np^{x_i}(1-p)^{1-x_i}$，转化为对数似然函数为： 


$lnL(\theta)=\Sigma_{i=1}^n[x_ilnp+(1-x_i)ln(1-p)]\tag{2}$

# NCE的推导

假设$X=(x_1,x_2,\cdots, x_{T_d})$是从真实的数据（或语料库）中抽取样本，但是样本服从什么样的分布我们不知道，那么先假设其中的每个$x_i$服从一个未知的概率密度函数$p_d$。这样我们需要一个相对可参考的分布反过来去估计概率密度函数$p_d$，这个可参考的分布或称之为噪音分布应该是我们知道的，比如高斯分布，均匀分布等。假设这个噪音分布的概率密度函数为$p_n$，从中抽取样本数据为$Y=(y_1,y_2,\cdots, y_{T_n})$，而这个数据称之为噪声样本，我们的目的就是通过学习一个分类器把这两类样本区别开来，并能从模型中学到数据的属性，噪音对比估计的思想就是“通过比较而学习”。

## 定义

让$U=X\bigcup Y=\{u_1,u_2,\cdots, u_{T_d+T_n}\}$，其中$T_d$为数据样本个数，$T_n$为噪音分布的样本个数[1](#fn:1)。那么我们认为$u_t$服从(0-1)分布，给每个$u_t$一个标签$C_t$，则 


$C_t=\begin{cases}1 &if &u_t\in X \\0 & if& u_t\in Y\end{cases}$

由于$p_d$未知，我们让$p(\cdot|C=1)=p_m(.;\theta)$，我们假设存在一个$\theta^*$使得$p_d(.)=p_m(.;\theta^*)$，那么，就可以认为经验分布$p_d(.)$为参数分布簇$p_m(.;\theta)$中的一员。 

给定以上定义，我们得到： 


$p(u|C=1)=p_m(u;\theta)\quad \#data\\p(u|C=0)=p_n(u) \quad \#noise\tag{3}$

先验分布： 


$P(C=1)=\frac{T_d}{T_d+T_n}\\P(C=0)=\frac{T_n}{T_d+T_n}\tag{4}$

根据全概率公式： 


$P(u)=P(u|C=1;\theta)P(C=1) + P(u|C=0)P(C=0)\\=p_m(u;\theta)\frac{T_d}{T_d+T_n} + p_n(u)\frac{T_n}{T_d+T_n}\tag{5}$

而我们要得到后验概率$P(C=1|u;\theta)$，则根据贝叶斯公式得： 


$P(C=1|u;\theta)=\frac{P(u|C=1;\theta)P(C=1)}{P(u)}\\=\frac{p_m(u;\theta)\frac{T_d}{T_d+T_n}}{p_m(u;\theta)\frac{T_d}{T_d+T_n} + p_n(u)\frac{T_n}{T_d+T_n}}\\=\frac{p_m(u;\theta)}{p_m(u;\theta)+\frac{T_n}{T_d}p_n(u)}$

则 


$P(C=1|u;\theta)=\frac{p_m(u;\theta)}{p_m(u;\theta)+vp_n(u)}\tag{6}$

同理 


$P(C=0|u;\theta)=\frac{vp_n(u)}{p_m(u;\theta)+vp_n(u)}\tag{7}$

其中，$v=\frac{T_n}{T_d}$

然后引进一个对数比率$G(u;\theta)=ln\frac{p_m(u;\theta)}{p_n(u)}$，让$P(C=1|u;\theta)=h(u;\theta)$，那么$h(u;\theta)$可写为： 


$h(u;\theta)=r_v(G(u;\theta))\tag{8}$

其中，$r_v(u)=\frac{1}{1+vexp(-u)}$

由于类别$C_t$独立同分布于伯努利分布，则条件对数似然函数为： 


$L(\theta)=\Sigma_{t=1}^{T_d+T_n}[C_tlnP(C_t=1|u_t;\theta)+(1-C_t)lnP(C_t=0|u_t)]\\=\Sigma_{t=1}^{T_d}ln[h(x_t;\theta)]+\Sigma_{t=1}^{T_n}ln[1-h(y_t;\theta)]\tag{9}$

注意到，如果给式（9）加上个负号就成为了交叉熵函数了。从结果可以看出，我们进行的无监督学习的密度估计可由监督学习算法logistic regression来学习，这就是监督学习与无监督学习的联系。
# NCE的估计

我们知道，正则化的密度函数$p_m(.;\hat{\theta})$满足以下约束： 


$\int p_m(u;\theta)du=1\quad \#正则化约束\\p_m(.;\theta)\ge 0\quad \#密度函数为正的约束$

如果对于所有的$\theta$，上述约束都成立，那么模型是正则化的，并且我们可以用极大似然函数去估计$\theta$，如果只满足密度函数为正的约束，则认为模型为非正则化。我们的一个主要假设就是存在一个子集$\theta^*$使得非正则化的模型积分为1（即满足正则化$p_d(.)=p_m(.;\theta)$），非正则化的模型表示为$p_m^0(.;\alpha)$。 

我们定义配分函数$Z(\alpha)$为： 


$Z(\alpha)=\int p_m^0(u;\alpha)du\tag{10}$
$Z(\alpha)$就是把非正则化模型$p_m^0(.;\alpha)$转化为正则化模型$p_m^0(.;\alpha)/Z(\alpha)$

这里的问题是，$\alpha$到$Z(\alpha)$的映射是一个积分，除非$p_m^0(.;\alpha)$是一个简单的形式的时候，一般不能得到解析式。对于高维的问题，计算$Z(\alpha)$更加困难，所以尽可能的不要计算$Z(\alpha)$。而NCE就是不用计算$Z(\alpha)$的一种估计方法。其基本思想就是认为$Z$或等价地$c=ln1/Z$为模型的一个参数[2](#fn:2)，NCE加入这个正则化参数$c$并估计 


$lnp_m(;.;\theta)=lnp_m^0(.;\alpha)+c\tag{11}$

现在参数$\theta=(\alpha,c)$。这样估计参数$\hat{\theta}=(\hat{\alpha},\hat{c})$使得非正则化模型$p_m^0(.;\alpha)$形状匹配$p_d$，而$\hat{c}$提供了正则化项，从而使得整个模型得到正则化。 

经过训练，$\hat{c}$为$ln1/Z(\alpha)$的估计，通过最大化下面的目标函数得到的$\theta$的估计为 


$J_T\theta= \frac{1}{T_d}\{\Sigma_{t=1}^{T_d}ln[h(x_t;\theta)]+\Sigma_{t=1}^{T_n}ln[1-h(y_t;\theta)]\}\tag{12}$

上式再化简一下得： 


$J_T\theta= \frac{1}{T_d}\Sigma_{t=1}^{T_d}ln[h(x_t;\theta)]+v\frac{1}{T_n}\Sigma_{t=1}^{T_n}ln[1-h(y_t;\theta)]\tag{13}$

注意到，$h(.;\theta)\in(0,1)$，并且 


$h(.;\theta)=\begin{cases}0 & limG(.;\theta)\rightarrow -\infty\\1 & limG(.;\theta)\rightarrow \infty\end{cases}$

对于logistic regression来说，变量趋向于两个极端区别度越大，所以对于学习后的最优的参数$\hat{\theta}_T$而言，在$u_T\in X$的情况下会使得$G(u_T;\hat{\theta}_T)$尽可能的大，对于$u_T\in Y$尽可能的小，换而言之，我们就是让$P(C=1;u;\theta)$对于大多数的$x_i$尽可能的大，对于大多数$y_i$尽可能的小，这样就可以区分两种不同的数据。而word2vec中的negtive sample是NCE的一种特殊情况。
**参考文献**

【Michael Gutmann, Aapo Hyv¨arinen】Noise-contrastive estimation: A new estimation principle for unnormalized statistical models 

【Michael U. Gutmann, Aapo Hyv¨arinen】Noise-Contrastive Estimation of Unnormalized Statistical Models, with Applications to Natural Image Statistics 

【David Meyer】Notes on Noise Contrastive Estimation (NCE) 

【Chris Dyer】Notes on Noise Contrastive Estimation and Negative Sampling 

【Farhana Ferdousi Liza, Marek Grzes】Improving Language Modelling with Noise Contrastive Estimation
- 我们在Gutmann和Hyv¨arinen提出的NCE模型上做了一些修改，认为两个分布的样本不相等，其他的一些文章是从经验分布中抽取样本为1（$T_d=1$）， 从噪音分布中抽取样本为$k$$(T_n=k)$，总的抽样为$k+1$，都是我们设置的特殊化 [↩](#fnref:1)
- 在其他文献中也有把$Z$直接设置为1 [↩](#fnref:2)








