# 用通俗易懂的方式告诉你什么是EM算法 - Machine Learning with Peppa - CSDN博客





2018年02月03日 19:39:04[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：2133
所属专栏：[有趣的算法](https://blog.csdn.net/column/details/19022.html)[机器学习与数据挖掘](https://blog.csdn.net/column/details/18961.html)









## 一、EM简介


EM(Expectation Mmaximization) 是一种迭代算法， 用于含隐变量(Latent Variable) 的概率模型参数的极大似然估计， 或极大后验概率估计 EM算法由两步组成， 求期望的E步，和求极大的M步。




EM算法可以看成是特殊情况下计算极大似然的一种算法。

现实的数据经常有一些比较奇怪的问题，比如缺失数据、含有隐变量等问题。当这些问题出现的时候，计算极大似然函数通常是比较困难的，而EM算法可以解决这个问题。

EM算法已经有很多应用，比如最经典的Hidden Markov模型等。经济学中，除了逐渐开始受到重视的HMM模型（例如Yin and Zhao, 2015），其他领域也有可能涉及到EM算法，比如在Train的《Discrete Choice Methods with Simulation》就给出了一个mixed logit 模型的EM算法。

## 二、EM算法的预备知识




1、极大似然估计


（1）举例说明：经典问题——学生身高问题


我们需要调查我们学校的男生和女生的身高分布。 假设你在校园里随便找了100个男生和100个女生。他们共200个人。将他们按照性别划分为两组，然后先统计抽样得到的100个男生的身高。假设他们的身高是服从高斯分布的。但是这个分布的均值u和方差∂2我们不知道，这两个参数就是我们要估计的。记作θ=[u, ∂]T。


问题：我们知道样本所服从的概率分布的模型和一些样本，而不知道该模型中的参数。


我们已知的有两个：（1）样本服从的分布模型（2）随机抽取的样本  需要通过极大似然估计求出的包括：模型的参数


总的来说：极大似然估计就是用来估计模型参数的统计学方法。


（2）如何估计


问题数学化： （1）样本集X={x1,x2,…,xN} N=100 （2）概率密度：p(xi|θ)抽到男生i（的身高）的概率 100个样本之间独立同分布，所以我同时抽到这100个男生的概率就是他们各自概率的乘积。就是从分布是p(x|θ)的总体样本中抽取到这100个样本的概率，也就是样本集X中各个样本的联合概率，用下式表示：![](https://images2015.cnblogs.com/blog/797505/201604/797505-20160401125104144-1825220157.png)




这个概率反映了，在概率密度函数的参数是θ时，得到X这组样本的概率。 需要找到一个参数θ，其对应的似然函数L(θ)最大，也就是说抽到这100个男生（的身高）概率最大。这个叫做θ的最大似然估计量，记为

![](https://images2015.cnblogs.com/blog/797505/201604/797505-20160401125217441-826849783.png)


（3）求最大似然函数估计值的一般步骤


　　首先，写出似然函数：

![](https://images2015.cnblogs.com/blog/797505/201604/797505-20160401125318285-1823853438.png)


　　其次，对似然函数取对数，并整理：

![](https://images2015.cnblogs.com/blog/797505/201604/797505-20160401125358676-1323591207.png)




　　然后，求导数，令导数为0，得到似然方程；


　　最后，解似然方程，得到的参数即为所求。


（4）总结


 　　多数情况下我们是根据已知条件来推算结果，而极大似然估计是已经知道了结果，然后寻求使该结果出现的可能性最大的条件，以此作为估计值。


2、Jensen不等式


（1）定义


设f是定义域为实数的函数，如果对于所有的实数x。如果对于所有的实数x，f(x)的二次导数大于等于0，那么f是凸函数。  Jensen不等式表述如下：      如果f是凸函数，X是随机变量，那么：E[f(X)]>=f(E[X])  。当且仅当X是常量时，上式取等号。


（2）举例

![](https://images2015.cnblogs.com/blog/797505/201604/797505-20160401125718941-1997034466.png)


图中，实线f是凸函数，X是随机变量，有0.5的概率是a，有0.5的概率是b。X的期望值就是a和b的中值了，图中可以看到E[f(X)]>=f(E[X])成立。         Jensen不等式应用于凹函数时，不等号方向反向。





## 三、EM算法推导

如果我们关心的参数为θ，观察到的数据为y，隐藏变量为z，那么根据全概率公式：
![P(y|\theta)=\int P(y|z,\theta)f(z|\theta)dz](https://www.zhihu.com/equation?tex=P%28y%7C%5Ctheta%29%3D%5Cint+P%28y%7Cz%2C%5Ctheta%29f%28z%7C%5Ctheta%29dz)

理论上，只要最大化这个密度函数的对数，就可以得到极大似然估计。然而问题是，对z进行积分很多情况下是非常困难的，特别是z的维数可能与样本量一样大，这个时候如果计算数值积分是非常恐怖的一件事情。

而EM算法可以解决这个问题。根据贝叶斯法则，我们可以得到：
![h(z|y,\theta)=\frac{P(y|z,\theta)f(z|\theta)}{P(y|\theta)}](https://www.zhihu.com/equation?tex=h%28z%7Cy%2C%5Ctheta%29%3D%5Cfrac%7BP%28y%7Cz%2C%5Ctheta%29f%28z%7C%5Ctheta%29%7D%7BP%28y%7C%5Ctheta%29%7D)

EM算法的精髓就是使用这个公式处理z的维数问题。

直觉上，EM算法就是一个猜测的过程：给定一个猜测θ'，那么可以根据这个猜测θ'和现实的数据计算隐藏变量取各个值的概率。有了z的概率之后，再根据这个概率计算更有可能的θ。

准确的来说，EM算法就是如下的迭代过程：
![\theta_{t+1}=\arg\max_\theta \varepsilon  (\theta|\theta_t)=\arg\max_\theta\int h(z|y,\theta_t)\ln[P(y|z,\theta)f(z|\theta)] dz](https://www.zhihu.com/equation?tex=%5Ctheta_%7Bt%2B1%7D%3D%5Carg%5Cmax_%5Ctheta+%5Cvarepsilon++%28%5Ctheta%7C%5Ctheta_t%29%3D%5Carg%5Cmax_%5Ctheta%5Cint+h%28z%7Cy%2C%5Ctheta_t%29%5Cln%5BP%28y%7Cz%2C%5Ctheta%29f%28z%7C%5Ctheta%29%5D+dz)

Train的《Discrete Choice Methods with Simulation》中有一张图非常形象的描述了上面的过程：

![](https://pic2.zhimg.com/80/d7e27557ec930f7c3e35826d968b56a0_hd.jpg)图中LL为上面的ln[P(y|θ)]，ε可以大体等价为上面介绍的迭代过程的目标函数。可以证明的是，在θ_t处，LL和ε相切，且ε<=LL。如此，每一次对ε函数求最小化，都给出了一个θ的最好猜测。从这个角度来讲，EM算法提供了计算极大似然函数的一个优化算法，只不过最经典的Quasi-Newton方法直接使用导数信息更新θ。



使用EM算法的关键主要是求出h函数。下面举一个最经典的mixed-normal的例子，假设：
![z\sim B(p) \\x_1\sim N(\mu,1) \\x_2\sim N(-\mu,1) \\y=z*x_1+(1-z)*x_2](https://www.zhihu.com/equation?tex=z%5Csim+B%28p%29+%5C%5C%0Ax_1%5Csim+N%28%5Cmu%2C1%29+%5C%5C%0Ax_2%5Csim+N%28-%5Cmu%2C1%29+%5C%5C%0Ay%3Dz%2Ax_1%2B%281-z%29%2Ax_2)

即观察到的数据y以概率p来自两个总体，两个总体的均值假设分别为μ和-μ。

我们可以计算：
![P(y|\theta)=\phi(y-\mu)\cdot (1-p)+\phi(y+\mu)\cdot p](https://www.zhihu.com/equation?tex=P%28y%7C%5Ctheta%29%3D%5Cphi%28y-%5Cmu%29%5Ccdot+%281-p%29%2B%5Cphi%28y%2B%5Cmu%29%5Ccdot+p)

其中θ={μ,p}。

同样，可以计算：
![h(z=1|y,\theta)=\frac{(1-p)\cdot \phi(y-\mu)}{(1-p)\cdot \phi(y-\mu)+p\cdot \phi(y+\mu)}=h_1(\theta)\\h(z=0|y,\theta)=\frac{p\cdot \phi(y+\mu)}{(1-p)\cdot \phi(y-\mu)+p\cdot \phi(y+\mu)}=h_0(\theta)](https://www.zhihu.com/equation?tex=h%28z%3D1%7Cy%2C%5Ctheta%29%3D%5Cfrac%7B%281-p%29%5Ccdot+%5Cphi%28y-%5Cmu%29%7D%7B%281-p%29%5Ccdot+%5Cphi%28y-%5Cmu%29%2Bp%5Ccdot+%5Cphi%28y%2B%5Cmu%29%7D%3Dh_1%28%5Ctheta%29%5C%5C%0Ah%28z%3D0%7Cy%2C%5Ctheta%29%3D%5Cfrac%7Bp%5Ccdot+%5Cphi%28y%2B%5Cmu%29%7D%7B%281-p%29%5Ccdot+%5Cphi%28y-%5Cmu%29%2Bp%5Ccdot+%5Cphi%28y%2B%5Cmu%29%7D%3Dh_0%28%5Ctheta%29)

如此，上面的迭代过程就可以写为:
![\varepsilon  (\theta|\theta_t)=h_0(\theta_t)\cdot \ln[(1-p)\cdot \phi(y-\mu)]+h_1(\theta_t)\cdot  \ln[p\cdot \phi(y+\mu)]](https://www.zhihu.com/equation?tex=+%5Cvarepsilon++%28%5Ctheta%7C%5Ctheta_t%29%3Dh_0%28%5Ctheta_t%29%5Ccdot+%5Cln%5B%281-p%29%5Ccdot+%5Cphi%28y-%5Cmu%29%5D%2Bh_1%28%5Ctheta_t%29%5Ccdot++%5Cln%5Bp%5Ccdot+%5Cphi%28y%2B%5Cmu%29%5D)

给定一个初始值，不断的迭代求上面的最优，就可以得到结果。


## EM的两个例子


先来看看EM算法大概用在什么样的情况。
- 
(三硬币模型)假设有3枚硬币，分别记作A,B,C。这些硬币正面出现的概率分别是$\pi,q,p$。进行如下实验，先扔硬币A，根据其结果选出硬币B或硬币C，正面选硬币B，反面选硬币C，然后扔选出的硬币，正面朝上结果记为1，反面朝上结果记为0，独立重复n次(此处为10次)，观测结果如下：1,1,0,1,0,0,1,0,1,1。估计三枚硬币正面朝上的概率。
- 
高斯混合模型参数估计
![P(x|\theta)=\sum^{K}_{k=1}{\alpha_k\phi(x|\theta_{k})}](http://www.zhihu.com/equation?tex=P%28x%7C%5Ctheta%29%3D%5Csum%5E%7BK%7D_%7Bk%3D1%7D%7B%5Calpha_k%5Cphi%28x%7C%5Ctheta_%7Bk%7D%29%7D)

## 四、算法解释


EM算法，就是当你没办法直接从观测之中进行参数估计的时候，通过加一些可以满足特定条件的隐变量，来简化模型，达到可以通过迭代来逐步估计分布的参数的一个算法。


设X为观测数据(obvious data), Z为隐变量(latent variable), ![\theta](http://www.zhihu.com/equation?tex=%5Ctheta)为要估计的参数。


我们做极大似然估计的时候，需要极大化对数似然函数来求出参数的极大似然估计，所以，我们的目标就是极大化一个对数似然函数（以下都用log代替ln，别问为啥我这么写，我也不知道，大多数文献都这么写）


公式1
![L(\theta)=logP(X|\theta)=log\sum_Z{P(Y,Z|\theta)}](http://www.zhihu.com/equation?tex=L%28%5Ctheta%29%3DlogP%28X%7C%5Ctheta%29%0A%3Dlog%5Csum_Z%7BP%28Y%2CZ%7C%5Ctheta%29%7D)


公式2
![L(\theta)=log(\sum_Z{P(X|Z,\theta)P(Z|\theta)})](http://www.zhihu.com/equation?tex=L%28%5Ctheta%29%3Dlog%28%5Csum_Z%7BP%28X%7CZ%2C%5Ctheta%29P%28Z%7C%5Ctheta%29%7D%29+)


要注意，我们加的隐变量Z，要满足两个条件：
- 
加了隐变量后，可以达到简化模型的目的(废话)
- 
加了隐变量，不改变原分布的边缘(重要)


面对公式2，我们看着他很简洁，但是，但是，但是，（重要的事和难过的事都要说三遍！）现实中他很复杂。比如我们加了隐变量的混合高斯模型的对数似然函数就如下:
![L(\theta,\alpha)=\sum^N_{i=1}{\sum^K_{Z_i=1}{    (log\alpha_{Z_i}+log\phi(X_i|\mu_{Z_i},\Sigma_{Z_i})P(Z_i|X_i,\theta))}}](http://www.zhihu.com/equation?tex=L%28%5Ctheta%2C%5Calpha%29%3D%5Csum%5EN_%7Bi%3D1%7D%7B%5Csum%5EK_%7BZ_i%3D1%7D%7B%0A++++%28log%5Calpha_%7BZ_i%7D%2Blog%5Cphi%28X_i%7C%5Cmu_%7BZ_i%7D%2C%5CSigma_%7BZ_i%7D%29P%28Z_i%7CX_i%2C%5Ctheta%29%29%0A%7D%7D)


这个东西，要求出最大值，还是很难的，所以我们要采用EM算法。EM的总体思想就是我们通过迭代逐步极大化这个极大似然函数，最终使 极大似然函数最大，进而求解需要估计的参数。




## 几个有趣的解释





简版：猜（E-step）,反思（M-step）,重复；


背景：公司有很多领导=[A总，刘总，C总]，同时有很多漂亮的女职员=[小甲，小章，小乙]。（请勿对号入座）你迫切的怀疑这些老总跟这些女职员有问题。为了科学的验证你的猜想，你进行了细致的观察。于是，

观察数据：

1）A总，小甲，小乙一起出门了；

2）刘总，小甲，小章一起出门了；

3）刘总，小章，小乙一起出门了；

4）C总，小乙一起出门了；

收集到了数据，你开始了神秘的EM计算：

初始化，你觉得三个老总一样帅，一样有钱，三个美女一样漂亮，每个人都可能跟每个人有关系。所以，每个老总跟每个女职员“有问题”的概率都是1/3;

这样，（E step）

1） A总跟小甲出去过了 1/2 * 1/3 = 1/6 次，跟小乙也出去了1/6次；（所谓的fractional count）

2）刘总跟小甲，小章也都出去了1/6次

3）刘总跟小乙，小章又出去了1/6次

4）C总跟小乙出去了1/3次

总计，A总跟小甲出去了1/6次，跟小乙也出去了1/6次 ; 刘总跟小甲，小乙出去了1/6次，跟小章出去了1/3次；C总跟小章出去了1/3次；

你开始跟新你的八卦了(M step), 

A总跟小甲，小乙有问题的概率都是1/6 / (1/6 + 1/6) = 1/2； 

刘总跟小甲，小乙有问题的概率是1/6 / (1/6+1/6+1/6+1/6) = 1/4; 跟小章有问题的概率是(1/6+1/6)/(1/6 * 4) = 1/2;

C总跟小乙有问题的概率是 1。

然后，你有开始根据最新的概率计算了；（E-step）

1）A总跟小甲出去了 1/2 * 1/2 = 1/4 次，跟小乙也出去 1/4 次；

2）刘总跟小甲出去了1/2 * 1/4 = 1/12 次， 跟小章出去了 1/2 * 1/2 = 1/4 次；

3）刘总跟小乙出去了1/2 * 1/4 = 1/12 次， 跟小章又出去了 1/2 * 1/2 = 1/4 次；

4）C总跟小乙出去了1次；

重新反思你的八卦（M-step）:

A总跟小甲，小乙有问题的概率都是1/4/ (1/4 + 1/4) = 1/2； 

B总跟小甲，小乙是 1/12 / (1/12 + 1/4 + 1/4 + 1/12) = 1/8 ; 跟小章是 3/4 ;

C总跟小乙的概率是1。

你继续计算，反思，总之，最后，你得到了真相！（马总表示我早就知道真相了）




## 参考文章


1. 知乎：https://www.zhihu.com/question/27976634

2. 《统计学习方法》，李航

3. 机器学习入门与实践

4. 机器学习算法：https://www.cnblogs.com/Gabby/p/5344658.html








