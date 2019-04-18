# SVM速览 - Orisun - 博客园







# [SVM速览](https://www.cnblogs.com/zhangchaoyang/articles/2642591.html)





SVM属于一种前馈神经网络，它在解决小样本、非线性及高维模式识别中表现出许多特有的优势。对于两类分类问题，有训练样本{(X1,y1),(X2,y2),...,(Xn,yn)}，y是分类标记，取值于+1和-1。

![](https://pic002.cnblogs.com/images/2012/103496/2012081615542513.png)

a和b都可以作为分类超平面，但最优超平面只有一个，最优分类平面使间隔最大化。

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}WX_i+b>0,y_i=+1)

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}WX_i+b<0,y_i=-1)

统一起来就是![](http://www.forkosh.com/mathtex.cgi?\dpi{120}y_i(WX_i+b)>0\qquad(1))

分离边缘可表示为![](http://www.forkosh.com/mathtex.cgi?\dpi{120}\rho=\frac{2}{||W_0||})，要使分离边缘最大化，就要使||W0||最小化。于是我们要求解的问题数学表达形式为：

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}min\qquad\frac{1}{2}||W||^2=\frac{1}{2}W^TW)

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}s.t.\qquad{y_i(WX_i+b)>0})

这是个带约束的最优化问题。目标函数是二次的，约束条件是一次的----二次规划。可以用Lagrange系数法解决此类优化问题。引入Lagrange函数如下

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}L(W,b,a)=\frac{1}{2}W^TW-\sum_{i=1}^N{a_i[y_i(WX_i+b)]}\qquad(2))

ai是Lagrange系数，非负。上式中第一项为代价函数，第二项为正，于是最小化代价函数转化为最小化Lagrange函数。分别对W和b求偏导，令导数为0。

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}\frac{\partial{L}}{\partial{W}}=W-\sum_{i=1}^n{a_iy_iX_i}=0)

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}\frac{\partial{L}}{\partial{b}}=\sum_{i=1}^n{a_iy_i}=0)

把上面结果代回（2）式，得到Lagrange对偶问题，即原来的最小化L(W,b,a)变为最大化Q(a)

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}max\qquad{Q(a)}=\sum_{i=1}^n{a_i}-\frac{1}{2}\sum_{i=1}^N{\sum_{j=1}^N{a_ia_jy_iy_jX_i^TX_j}},a_i\ge0\qquad\qquad(3))

要解这个最优化问题，由KKT条件得，最优分类超平面的斜率

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}W_0=\sum{a_i}y^sX^s)最优分类平面的权向量是支持向量的线性组合。这意味着其他非支持向量对分类超平面没有影响。

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}b_0=1-W_0^TX^s)

### 松驰变量

在实际问题中总会有少数几个点越过了分类平面，我们的数学模型应该对这些噪声数据有一定的容忍性。此时（1）式变为

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}y_i(WX_i+b)+\epsilon_i>0\qquad(4))

代价函数变为

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}\frac{1}{2}W^TW+C\sum_{i=1}^n{\epsilon_i})

虽然加入了松驰变量，但是按照上面的推导方法，最终得出的W0和b0都上面上完全一样的！

### 核函数

SVM显然是线性分类器，但数据如果根本就线性不可分怎么办？不用着急，数据在原始空间（称为输入空间）线性不可分，但是映射到高维空间（称为特征空间）后很可能就线性可分了。这同时带来一个问题：在高维空间上求解一个带约束的优化问题显然比在低维空间上计算量要大得多，这就是所谓的“维数灾难”。于是就引入了“核技巧”。假如X映射到高维空间后是Φ(X)，定义核函数：

K(X,Y)=<Φ(X),Φ(Y)>

<,>表示内积运算。我们看到核函数将m维高维空间的内积运算转化为n维低维输入空间的核函数计算，从而巧妙地解决了在高维特征空间中计算的“维数灾难”等问题，从而为在高维特征空间解决复杂的分类或[回归](http://www.cnblogs.com/zhangchaoyang/articles/2640700.html)问题奠定了理论基础。

常用的核函数可分为两类，即内积核函数和平移不变核函数，如：
- 多项式核函数 ![](http://www.forkosh.com/mathtex.cgi?\dpi{120}K(X,X^p)=[X\cdot{X^p}+1]^q)，采用该核函数的支持向量机是一个q阶多项式分类器，参数q由用户决定。
- 高斯核函数![](http://www.forkosh.com/mathtex.cgi?\dpi{120}K(X,X^p)=exp(-\frac{|X-X^p|^2}{2\sigma^2}))，采用该核函数的支持向量机是一种[径向基函数分类器](http://www.cnblogs.com/zhangchaoyang/articles/2591663.html)。
- Sigmoid核函数![](http://www.forkosh.com/mathtex.cgi?\dpi{120}K(X,X^p)=tanh(kX\cdot{X^p})+c)，其中tanh是双曲正切函数，![](http://www.forkosh.com/mathtex.cgi?tanh(x)=\frac{e^x-e^{-x}}{e^x+e^{-x}})，采用该核函数的支持向量机是一个[单隐层感知器神经网络](http://www.cnblogs.com/zhangchaoyang/articles/2269523.html)。

使用内积核函数在特征空间建立的最优超平面定义为

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}\sum_{p=1}^P{a_py_pK(X,X_p)}=0)

###  SVM学习算法的一般步骤

准备一组训练样本{（X1,d1），（X2,d2），...，（Xp,dp），...，（XP,dP）} 

如果能够选择变换函数Φ（这取决于设计者在这方面的知识），就按下面的步骤来。
- 使用变换函数Φ将输入向量映射到高维特征空间。
- 解下列最优化问题，求出Lagrange系数a0
![](http://www.forkosh.com/mathtex.cgi?\dpi{120}max\qquad{Q(a)}=\sum_{p=1}^Pa_p-\frac{1}{2}\sum_{p=1}^P{\sum_{j=1}^P{a_pa_jd^pd^j\Phi^T(X^p)\Phi(X^j)}}\qquad\qquad(5))

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}s.t.)

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}\sum_{p=1}^Pa_pd^p=0,)
![](http://www.forkosh.com/mathtex.cgi?\dpi{120}0\le{a_p}\le{C}) （加入了松弛变量的） 或  ![](http://www.forkosh.com/mathtex.cgi?\dpi{120}a_p\ge{0})
- 计算最优权值
![](http://www.forkosh.com/mathtex.cgi?\dpi{120}W_0=\sum_{p=1}^Pa_{0p}d^p\Phi(X^p))
- 对于待分类的模式X，计算分类判别函数
![](http://www.forkosh.com/mathtex.cgi?\dpi{120}f(X)=sgn[\sum_{p=1}^Pa_{0p}d^p\Phi^T(X^p)\Phi(X)+b_0])

若能够选择一个内积函数K(Xp,X)，则可避免向高维空间的映射。此时算法步骤为：
- 解下列最优化问题，求出Lagrange系数a0
![](http://www.forkosh.com/mathtex.cgi?\dpi{120}max\qquad{Q(a)}=\sum_{p=1}^Pa_p-\frac{1}{2}\sum_{p=1}^P{\sum_{j=1}^P{a_pa_jd^pd^jK(X^p,X^j)}}\qquad\qquad(6))
![](http://www.forkosh.com/mathtex.cgi?\dpi{120}s.t.)

![](http://www.forkosh.com/mathtex.cgi?\dpi{120}\sum_{p=1}^Pa_pd^p=0,)
![](http://www.forkosh.com/mathtex.cgi?\dpi{120}0\le{a_p}\le{C})  或  ![](http://www.forkosh.com/mathtex.cgi?\dpi{120}a_p\ge{0})
- 计算最优权值
![](http://www.forkosh.com/mathtex.cgi?\dpi{120}W_0=\sum_{p=1}^Pa_{0p}d^pY^p)
- 对于待分类的模式X，计算分类判别函数
![](http://www.forkosh.com/mathtex.cgi?\dpi{120}f(X)=sgn[\sum_{p=1}^Pa_{0p}d^pK(X^p,X)+b_0])

解决像(5)、（6）这样的最优化问题，一种流行的方法是[SMO](http://www.cnblogs.com/jerrylead/archive/2011/03/18/1988419.html)([Sequential Minimal Optimzation](http://zh.wikipedia.org/wiki/%E5%BA%8F%E5%88%97%E6%9C%80%E5%B0%8F%E4%BC%98%E5%8C%96%E7%AE%97%E6%B3%95))算法。












