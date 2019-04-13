
# MLAPP学习笔记-朴素贝叶斯分类器 - simplex - 博客园|[simplex](https://www.cnblogs.com/simplex/)
|
|[博客园](https://www.cnblogs.com/)|::|[首页](https://www.cnblogs.com/simplex/)|::|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|::|[联系](https://msg.cnblogs.com/send/simplex)|::|[订阅](https://www.cnblogs.com/simplex/rss)![订阅](//www.cnblogs.com/images/xml.gif)|::|[管理](https://i.cnblogs.com/)|17 				Posts ::				0 Stories				::				5 Comments				::				0 Trackbacks|
|

|公告


|问题描述
|我们需要解决的问题可以描述如下：\(D\)维特征向量\(X=[x_1,x_2,\cdots,x_D]\)，\(x_i\in{1,2,\cdots,K}\)属于给定的\(C\)个类别之一，用\(y\)标记\(X\)属于的类别。如何根据已知的\(N\)个被正确分类的特征向量\(S=\{(x_1,y_1),(x_2,y_2),\cdots,(x_N,y_N)\}\)来推断未知\(X\)的正确分类？
|上述问题可以认为是要求解概率\(p(y=c|X,S)\)的最大值，我们可以用Generative classifiers来解决这个问题，即采用贝叶斯公式来计算这个概率：
|\(p(y=c|X,\theta)=\frac{p(X|y=c,\theta)p(y=c|\theta)}{p(X)}\)
|其中\(\theta\)是概率分布的未知参数，需要用训练集\(S\)来通过参数估计来得到。
|等式右端分母\(p(X)\)是一个归一化因子，可以不用考虑（给定\(X\)之后，对每个\(y=c\)它都是一个固定值）所以上式可以写为：
|\(p(y=c|X,\theta)\propto p(X|y=c,\theta)p(y=c|\theta)\)
|计算上式重点是计算类条件概率密度（class-conditional density）\(p(X|y=c,\theta)\)。假设各个特征\(x_i\)在任意\(y=c\)的情况下是条件独立的，则类条件概率密度可以写成如下连乘的形式：
|\(p(X|y=c,\theta)=\prod_{j=c}^{D}p(x_j|y=c,\theta_{jc})\)
|利用这种假设的模型即为朴素贝叶斯分类器（naive Bayes classifier）。而条件概率密度的形式则可以根据特征\(x_j\)的不同而选择不同形式的分布，进而用训练集来估计对应分布的参数。比如，对于连续特征可以假设为高斯分布，对于二项特征可以假设为伯努利分布，对于多项特征可以假设为multinoulli分布（即只进行一次实验的的多项式分布，是伯努利分布的推广形式，一个典型例子是掷骰子）。
|模型训练和使用
|模型训练其实就是用训练集\(S\)来对参数\(\theta\)进行估计。通常可以采用MLE、MAP或参数期望值（贝叶斯方法）。MLE是根据最大似然的原则确定参数，MAP是根据最大后验概率的原则来确定参数，这两者都属于点估计，即只得到最后的一个参数值，而不是参数的概率分布\(p(\theta|S)\)。与此不同的是，采用参数期望值的方法首先计算参数的概率分布\(p(\theta|S)\)，然后用对应的期望\(E(\theta|S)\)作为参数的估计值。三种方法得到的参数估计值的表达式各不相同，但是在训练集\(S\)趋向于无穷大时会收敛到相同的值。另外，从对拉普拉斯平滑的讨论可知，MAP和MLE都没有平滑效果，只有用参数期望值才能进行平滑。下面分别介绍MLE方法和贝叶斯方法。
|1. MLE方法
|任给一个训练数据\((X_i,y_i)\)，有下式成立：
|\(p(X_i,y_i|\theta)=p(y_i|\pi)\prod_{j=1}^{D}p(x_{ij}|\theta_j)=\prod_{c=1}^{C}\pi_c^{I(y_i=c)}\prod_{c=1}^{C}\prod_{j=1}^{D}p(x_{ij},y_i|\theta_c)^{I(y_i=c)}\)
|其中，第一个等式即为全概率公式，注意参数\(\theta\)分成了只与\(X\)相关的参数\(\theta_j\)和只与\(y\)相关的参数\(\pi\)。在得到第二个等式的时候，认为类别\(y\)满足multinoulli分布，同时利用了特征\(x_i\)的条件独立性，其中\(I\)代表示性函数。
|利用各个训练数据的独立性，可以得到似然函数：
|\(p(S|\theta)=\prod_{n=1}^{N}p(X_n,y_n|\theta)\)
|由于上式是一个连乘的形式，所以我们把似然函数用对数形式来表示：
|\(\log p(S|\theta)=\sum_{c=1}^{C}N_c\log \pi_c+\sum_{j=1}^{D}\sum_{c=1}^{C}\sum_{i:y_i=c}\log p(X_i,y_i|\theta)\)
|其中\(N_c=\sum_iI(y_i=c)\)为训练集中满足\(y_i=c\)的元素数目。
|由于关于\(y\)的似然函数满足multinoulli分布，可以得到\(\pi_c\)的最大似然估计：
|\(\pi_c^{MLE}=N_c/N\)
|\(\theta_{jc}\)的最大似然估计依赖于各个特征\(x_i\)的似然函数分布。为了简单起见，假设所有\(x_i\)都服从伯努利分布，这样可以得到:
|\(\theta_{jc}^{MLE}=N_{jc}N_c\)
|在根据训练集\(S\)得到估计参数之后，便可以对未知数据计算\(p(X_i,y_i|\theta^{MLE})\)，并选择使似然最大的那个\(y_i=c\)作为对未知数据的分类。

|2. 贝叶斯方法
|如前所述，贝叶斯方法的目标是要求出参数的后验分布\(p(\theta|D)\)。为此，首先引入独立性假设:
|\(p(\theta)=p(\pi)\prod_{j=1}^{D}\prod_{c=1}^{C}p(\theta_{jc})\)
|接着，假设\(p(\pi)\)满足狄利克雷分布\(\textrm{Dir}(\alpha)\)，\(p(\theta_{jc})\)满足Beta分布\(\textrm{Beta}(\beta_0,\beta_1)\)，这样先验概率分布刚好与似然分布共轭。后验概率\(p(\pi|S)\)成为狄利克雷-multinoulli分布，\(p(\theta_{jc})\)成为Beta-二项分布。（注：Beta-二项分布其实是狄利克雷-multinoulli分布在特征等于2的时候的特殊情况）
|这样我们就有：
|\(p(\theta|S)=p(\pi|S)\prod_{j=1}^{D}\prod_{c=1}^{C}p(\theta_{jc}|S)\)
|\(p(\pi|S)=\textrm{Dir}(N_1+\alpha_1,N_2+\alpha_2,\cdots,N_C+\alpha_C)\)
|\(p(\theta_{jc}|S)=\textrm{Beta}(N_c-N_{jc}+\beta_0,N_{jc}+\beta_1)\)
|接下来，要用该模型计算\(p(y|X,S)\)，并据此对\(X\)进行分类。根据贝叶斯定理：
|\(p(y=c|X,S) \propto p(X|y=c,S)p(y=c|S)=p(y=c|S)\prod_{j=1}^Dp(x_j|y=c,S)\)
|由于\(x_i\)和\(y\)分别是伯努利分布和multinoulli分布，而且各自参数的后验分布又分别是Beta分布和狄利克雷分布，所以有：
|\(p(x_j|y=c,S)=E(\theta_{jc}|S)=\frac{N_{jc}+\beta_1}{N_{c}+\beta_0+\beta_1}\)
|\(p(y=c|S)=E(\pi_c|S)=\frac{N_c+\alpha_c}{N+\alpha_0}\)
|上面两个式子的第一个等号成立的原因可以参见MLAPP公式3.51，第二个等式就是Beta分布和狄利克雷分布的期望公式。在大多数情况下，可以选择$\alpha=1$，$\beta=1$，这样就得到拉普拉斯平滑效果。
|把上述公式带回到\(p(y=c|X,S)\)便可以求出不同\(y=c\)对应的（相对）概率，并据此做出分类。
|对比MLE方法和贝叶斯方法可以发现，两者的差别仅在于参数估计方法。在得到估计参数之后对未知数据进行分类的方法包括公式都是一样的。






|posted on|2016-08-20 19:20|[simplex](https://www.cnblogs.com/simplex/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=5791059)|[收藏](#)


|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
Copyright @
	simplex
Powered by:[.Text](http://scottwater.com/blog)and[ASP.NET](http://asp.net)
Theme by:[.NET Monster](http://www.DotNetMonster.com)
