# EM算法 - Orisun - 博客园







# [EM算法](https://www.cnblogs.com/zhangchaoyang/articles/2623364.html)





本文试图用最简单的例子、最浅显的方式说明EM（Expectation Maximization）算法的应用场景和使用方法，而略去公式的推导和收敛性的证明。

以下内容翻译自《[Data-Intensive Text Processing with MapReduce](http://lintool.github.com/MapReduceAlgorithms/MapReduce-book-final.pdf)》。

### Maximum Likelihood Estimation

Maximum Likelihood Estimation(MLE)是要选择一个最佳参数θ*，使得从训练集中观察到和情况出现的概率最大。即模型：

![](https://pic002.cnblogs.com/images/2012/103496/2012080420421659.png)

举例来说明。如下图

![](https://pic002.cnblogs.com/images/2012/103496/2012080420441173.png)

一个小黑球沿着一个三角形的木桩滚入杯子a或b中，可建立一个概率模型，由于是二值的，设服从Bernoulli分布，概率密度函数为：

![](https://pic002.cnblogs.com/images/2012/103496/2012080420541765.png)

p是k=0的概率，也是我们要使用MLE方法要确定的参数。

在上面的滚球实验中，我们令Y是待确定的参数，X是观察到的结果。连续10次实验，观察到的结果是X=（b,b,b,a,b,b,b,b,b,a）。小球进入a杯的概率为p，则满足10次实验的联合概率为：

![](https://pic002.cnblogs.com/images/2012/103496/2012080421023210.png)

为了使X发生的概率最大，令上式一阶求导函数为0，得p=0.2。

### 含有隐含变量的弹球实验

![](https://pic002.cnblogs.com/images/2012/103496/2012080421094245.png)

如上图，现在又多了两块三角形木桩，分别标记序号为0，1，2。并且实验中我们只知道小球最终进入了哪个杯子，中间的路线轨迹无从得知。

X取值于{a,b,c}表示小球进入哪个杯子。Y取值于{0,1,2,3}表示小球进入杯子前最后一步走的是哪条线路。小球在3个木桩处走右边侧的概率分别是p=(p0,p1,p2)。跟上例中一样，X表示训练集观测到的值，p是模型参数，而这里的Y就是"隐含变量"。

假如我们做了N次实验，小球经过路径0,1,2,3的次数依次是N0,N1,N2,N3，则：

![](https://pic002.cnblogs.com/images/2012/103496/2012080423200615.png)

### 带隐含变量的MLE

现在我们来考虑这个概率模型：Pr(X,Y;θ)。只有X是可观察的，Y和θ都是未知的。

![](https://pic002.cnblogs.com/images/2012/103496/2012080423240234.png)

经过一组实验，我们观察到X的一组取值x=(x1,x2,...,xl)，则联合概率为：

![](https://pic002.cnblogs.com/images/2012/103496/2012080423262821.png)

MLE就是要求出最佳的参数θ*，使得：

![](https://pic002.cnblogs.com/images/2012/103496/2012080423285592.png)

这个时候要求θ*，”令一阶求函数等于0“的方法已经行不通了，因为有隐含变量Y的存在。实现上上式很难找到一种解析求法，不过一种迭代的爬山算法可求解该问题。

### Expectation Maximization

EM是一种迭代算法，它试图找到一系列的估计参数θ(0),θ(1),θ(2),....使得训练数据的marginal likelihood是不断增加的，即：

![](https://pic002.cnblogs.com/images/2012/103496/2012080423365138.png)

算法刚开始的时候θ(0)赋予随机的值，每次迭代经历一个E-Step和一个M-Step，迭代终止条件是θ(i+1)与θ(i)相等或十分相近。

E-Step是在θ(i)已知的情况下计算X=x时Y=y的后验概率：

![](https://pic002.cnblogs.com/images/2012/103496/2012080508535865.png)

f(x|X)是一个权值，它表示观测值x在所有观察结果X中出现的频率。

M-Step：

![](https://pic002.cnblogs.com/images/2012/103496/2012080509015212.png)

其中![](https://pic002.cnblogs.com/images/2012/103496/2012080509032269.png)在E-Step已经求出来了。这时候可以用“令一阶导数等于0”的方法求出θ'。

EM算法收敛性的证明需要用到[Jensen不等式](http://www.cnblogs.com/jerrylead/archive/2011/04/06/2006936.html)，这里略去不讲。

### 举例计算

就拿上文那个3个木桩的滚球实验来说，做了N次实验，滚进3个杯子的次数依次是Na,Nb,Nc。

先给![](https://pic002.cnblogs.com/images/2012/103496/2012080509332431.png)赋予一个随机的值。

E-Step:

![](https://pic002.cnblogs.com/images/2012/103496/2012080509110066.png)

同时我们注意到

![](https://pic002.cnblogs.com/images/2012/103496/2012080509120763.png)

![](https://pic002.cnblogs.com/images/2012/103496/2012080509134542.png)

其它情况下Y的后验概率都为0。

M-Step:

我们只需要计算非0项就可以了

![](https://pic002.cnblogs.com/images/2012/103496/2012080509161081.png)

上面的每行第3列和第4列相乘，最后再按行相加，就得到关于θ(i+1)的函数，分别对p0,p1,p2求偏导，令导数为0，可求出p'0,p'1,p'2。

![](https://pic002.cnblogs.com/images/2012/103496/2012080509231752.png)

这里补充一个求导公式：

![](https://pic002.cnblogs.com/images/2012/103496/2012080509283966.png)

我们这个例子非常简单，计算θ(1)已经是最终的解了，当然你要计算出θ(2)才下此结论。

### 结束语

对于一般的模型，EM算法需要经过若干次迭代才能收敛，因为在M-Step依然是采用求导函数的方法，所以它找到的是极值点，即局部最优解，而非全局最优解。也正是因为EM算法具有局部性，所以它找到的最终解跟初始值θ(0)的选取有很大关系。

在求解[HMM的学习问题](http://www.cnblogs.com/zhangchaoyang/articles/2220398.html)、确立[高斯混合模型](http://www.cnblogs.com/zhangchaoyang/articles/2624882.html)参数用的都是EM算法。












