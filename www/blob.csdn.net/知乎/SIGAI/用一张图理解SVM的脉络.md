# 用一张图理解SVM的脉络 - 知乎
# 


```
导言
SVM在之前的很长一段时间内是性能最好的分类器，它有严密而优美的数学基础作为支撑。在各种机器学
习算法中，它是最不易理解的算法之一，要真正掌握它的原理有一定的难度。在本文中，SIGAI将带领大
家通过一张图来理清SVM推导过程的核心过程。
```

## 简介

在各种机器学习算法中，SVM是对数学要求较高的一种，一直以来不易被初学者掌握。如果能把握住推导的整体思路，则能降低理解的难度，在本文中[SIGAI](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26scene%3D21%23wechat_redirect)将通过一张图来为大家讲述SVM的整个推导过程。

SVM由Vapnik等人在1995年提出，在出现之后的20多年里它是最具影响力的机器学习算法之一。在深度学习技术出现之前，使用高斯核的SVM在很多问题上一度取得了最好的效果。SVM不仅可以用于分类问题，还可以用于回归问题。它具有泛化性能好，适合小样本等优点，被广泛应用于各种实际问题。

下面我们开始整个推导过程。先看这张图：
![](https://pic1.zhimg.com/v2-b287d5329d9e730559af07a66288755c_b.jpg)
最简单的SVM从线性分类器导出，根据最大化分类间隔的目标，我们可以得到线性可分问题的SVM训练时求解的问题。但现实应用中很多数据是线性不可分的，通过加入松弛变量和惩罚因子，可以将SVM推广到线性不可分的情况，具体做法是对违反约束条件的训练样本进行惩罚，得到线性不可分的SVM训练时优化的问题。这个优化问题是一个凸优化问题，并且满足Slater条件，因此强对偶成立，通过拉格朗日对偶可以将其转化成对偶问题求解。

到这里为止，支持向量机还是一个线性模型，只不过允许有错分的训练样本存在。通过核函数，可以将它转化成非线性模型，此时的对偶问题也是一个凸优化问题。这个问题的求解普遍使用的是SMO算法，这是一种分治法，它每次选择两个变量进行优化，这两个变量的优化问题是一个带等式和不等式约束条件的二次函数极值问题，可以求出公式解，并且这个问题也是凸优化问题。优化变量的选择通过KKT条件来确定。

下面我们按照这个思路展开，给出SVM完整的推导过程，难点在于拉格朗日对偶和KKT条件。

## 预备知识

为了大家能够理解推导过程，我们先介绍KKT条件。在微积分中我们学习过，带等式约束的最优化问题可以用拉格朗日乘数法求解，对于既有等式约束又有不等式约束的问题，也有类似的条件定义函数的最优解-这就是KKT条件。对于如下优化问题：
![](https://pic1.zhimg.com/v2-48c5c717232260aad3d06a0a17a9d54c_b.jpg)
首先构造拉格朗日乘子函数：
![](https://pic4.zhimg.com/v2-782956ad2ffb15037e426ddae3fb2457_b.jpg)
其中 ![\lambda_{j}\mu_{k}](https://www.zhihu.com/equation?tex=%5Clambda_%7Bj%7D%5Cmu_%7Bk%7D) 称为拉格朗日乘子。最优解 ![x^{*}](https://www.zhihu.com/equation?tex=x%5E%7B%2A%7D) 必须满足如下条件：
![](https://pic4.zhimg.com/v2-3152cc7674461b49010512c2db4d4a8f_b.jpg)
除了原本应该满足的等式约束和不等式约束之外,$
![](https://pic3.zhimg.com/v2-9c459c74151c818d942e500a4dcb18c2_b.jpg)
和拉格朗日乘数法一样。唯独多了
![](https://pic2.zhimg.com/v2-1d6e94ce44a094757a08f6fafe5127d1_b.jpg)
这一条件。

下面介绍拉格朗日对偶。对偶是最求解优化问题的一种手段，它将一个优化问题转化为另外一个更容易求解的问题，这两个问题是等价的。常见的对偶有拉格朗日对偶、Fenchel对偶。这里我们介绍拉格朗日对偶。

对于如下带等式约束和不等式约束的最优化问题：
![](https://pic2.zhimg.com/v2-6f5c5cb715800b52c79cfe35fa674e85_b.jpg)
仿照拉格朗日乘数法构造如下广义拉格朗日函数：
![](https://pic3.zhimg.com/v2-3faa594f2cfe98bcd1d0a0e65c86d352_b.jpg)
同样的称 ![\lambda_{i},\nu_{i}](https://www.zhihu.com/equation?tex=%5Clambda_%7Bi%7D%2C%5Cnu_%7Bi%7D) 为拉格朗日乘子。变量 ![\lambda_{i}](https://www.zhihu.com/equation?tex=%5Clambda_%7Bi%7D)必须满足 ![\lambda_{i}\geq 0](https://www.zhihu.com/equation?tex=%5Clambda_%7Bi%7D%5Cgeq+0) 的约束。接下来将上面的问题转化为如下所谓的原问题形式，其最优解为：
![](https://pic3.zhimg.com/v2-3286fb94c352c2d57735b68f79a08252_b.jpg)
等式右边的含义是先固定住变量x，将其看成常数，让拉格朗日函数对乘子变量 ![\lambda,\nu](https://www.zhihu.com/equation?tex=%5Clambda%2C%5Cnu) 求最大值。消掉这两组变量之后，再对变量x求最小值。为了简化表述，定义如下最大化问题：
![](https://pic1.zhimg.com/v2-923f9e3352d06b0980a3273ede582a98_b.jpg)
这是一个对乘子变量求最大值的问题，将x看成常数。这样原问题被转化为先对乘子变量求最大值，再对x求最小值。这个原问题和我们要求解的最小化问题有同样的解，如果x违反了等式或不等式约束，上面问题的最优解是无穷大，因此不可能是问题的解。如果x满足等式和不等式约束，上面的问题的最优解就是 ![f(x)](https://www.zhihu.com/equation?tex=f%28x%29) , 因此二者等价。通过这样的构造，将带约束条件的问题转化成对x没有约束的问题。详细的证明在SIGAI后续的文章中会给出。

接下来定义对偶问题与其最优解：
![](https://pic3.zhimg.com/v2-40972f61d8f8c3f31cce56d7bfb9484e_b.jpg)
其中
![](https://pic4.zhimg.com/v2-8c9d189f9f44b51c11ff56aaefc5d283_b.jpg)
和上面的做法相反，这里是先固定拉格朗日乘子，调整x让拉格朗日函数对x求极小值；然后再调整拉格朗日乘子对函数求极大值。

原问题和对偶问题只是改变了求极大值和极小值的顺序，每次操控的变量是一样的。如果原问题和对偶问题都存在最优解，则对偶问题的最优值不大于原问题的最优值，即：
![](https://pic1.zhimg.com/v2-37d3e1207fba92fb45b2ea905672bb84_b.jpg)
这称为弱对偶，后面的文章中我们会给出证明。原问题最优值和对偶问题最优值的差 ![p^{*}-d^{*}](https://www.zhihu.com/equation?tex=p%5E%7B%2A%7D-d%5E%7B%2A%7D) 称为对偶间隙。如果原问题和对偶问题有相同的最优解，我们就可以把求解原问题转化为求解对偶问题，这称为强对偶。强对偶成立的一种前提条件是Slater条件。

Slater条件指出，一个凸优化问题如果存在一个候选x使得所有不等式约束都严格满足，即对于所有的i都有 ![g_{i}\left( x \right)<0](https://www.zhihu.com/equation?tex=g_%7Bi%7D%5Cleft%28+x+%5Cright%29%3C0) 不等式不取等号。则存在 ![x_{*},\lambda_{*},\nu_{*}](https://www.zhihu.com/equation?tex=x_%7B%2A%7D%2C%5Clambda_%7B%2A%7D%2C%5Cnu_%7B%2A%7D)使得它们分别为原问题和对偶问题的最优解，并且：
![](https://pic1.zhimg.com/v2-be0a0337d68918464ef38b1c60727790_b.jpg)
Slater条件是强对偶成立的充分条件而不是必要条件。强对偶的意义在于：我们可以将求原问题转化为求对偶问题，有些时候对偶问题比原问题更容易求解。强对偶只是将原问题转化成对偶问题，而这个对偶问题怎么求解则是另外一个问题。

## 线性可分的情况

首先我们来看最简单的情况，线性可分的SVM。对于二分类问题，线性分类器用一个超平面将两类样本分开，对于二维平面，这个超平面是一条直线。线性分类器的判别函数为：

![](https://pic3.zhimg.com/v2-aaf5e3678d02bf2050fcf5305133ddea_b.jpg)
其中，w为权重向量，b为偏置项，是一个标量。一般情况下，给定一组训练样本可以得到不止一个线性分类器，下图就是一个例子：
![](https://pic4.zhimg.com/v2-0b1df2dc51427aa3ccb01ccb3cf99047_b.jpg)两个不同的线性分类器
上面的两个线性分类器都可以将两类样本分开，既然有不止一个可行的线性分类器，那么哪个分类器是最好的？SVM的目标是寻找一个分类超平面，它不仅能正确的分类每一个样本，并且要使得每一类样本中距离超平面最近的样本到超平面的距离尽可能远。

给定一批训练样本，假设样本的特征向量为x，类别标签为y，取值为+1或者-1，分别代表正样本和负样本。SVM为这些样本寻找一个最优分类超平面，其方程为：
![](https://pic4.zhimg.com/v2-bd8cbde79576a9b6c4f4d4a5d7fa4c17_b.jpg)
$首先要保证每个样本都被正确分类。对于正样本有：
![](https://pic2.zhimg.com/v2-05b6e4632aeaeaf07e7662842c56c62d_b.jpg)
对于负样本有：
![](https://pic4.zhimg.com/v2-07249ab4cb7950d7d877f0c5c33257cb_b.jpg)
由于正样本的的类别标签为+1，负样本的类别标签为-1，可以统一写成如下不等式约束：
![](https://pic1.zhimg.com/v2-a4ef72396a7c9d7569b0bcf1c4915254_b.jpg)
第二个要求是超平面离两类样本的距离要尽可能大。根据解析几何中点到平面的距离公式，每个样本点离分类超平面的距离为：
![](https://pic2.zhimg.com/v2-1969ed36e08d5c038d462ba18dda69dd_b.jpg)
其中
![](https://pic1.zhimg.com/v2-5ea488febddff40f9abd8aa78bbc1d70_b.jpg)
是向量的L2范数。上面的分类超平面方程有冗余，如果将方程两边都乘以不等于0的常数，还是同一个超平面。利用这个特点可以简化问题的表述。对w和b加上如下约束：
![](https://pic1.zhimg.com/v2-41c3f0d3fe75e59e0a2886509d335b54_b.jpg)
即离超平面最近的正、负样本代入超平面方程之后绝对值为1。这样可以消掉这个冗余，同时简化了点到平面距离的计算公式。对分类超平面的约束变成：
![](https://pic4.zhimg.com/v2-b382ac705e7f04b67474700eed96bf8f_b.jpg)
这是上面那个不等式约束的加强版。分类超平面与两类样本之间的间隔为：
![](https://pic2.zhimg.com/v2-6c441b5829f563cca307273256d9ddc9_b.jpg)
目标是使得这个间隔最大化，这等价于最小化下面的函数：
![](https://pic4.zhimg.com/v2-073bba3f438fe9bbf142fe135b0cea0b_b.jpg)
带上前面定义约束条件之后优化问题可以写成：
![](https://pic4.zhimg.com/v2-1b2735b18b576588c593f7817fd2b36b_b.jpg)
下图是线性可分的SVM示意图：
![](https://pic4.zhimg.com/v2-de0047ce32ef6a3be708ba18ab09be1f_b.jpg)线性可分的支持向量机示意图
## 线性不可分的情况

线性可分的SVM不具有太多的实用价值，因为现实问题中样本一般都不是线性可分的，接下来我们将它进行扩展，得到能够解决线性不可分问题的模型。为了处理这个问题，当线性不可分时通过加上松弛变量和惩罚因子对错误分类的样本进行惩罚，可以得到如下最优化问题：
![](https://pic1.zhimg.com/v2-290a8ba89c5532edf521fa0aa7581354_b.jpg)
其中 ![\xi_{i}](https://www.zhihu.com/equation?tex=%5Cxi_%7Bi%7D) 是松弛变量，如果它不为0，表示样本突破了不等式约束条件。C为惩罚因子，是人工设定的大于0的参数，用来对突破了不等式约束条件的样本进行惩罚。可以证明这个问题是凸优化问题，因此可以保证求得全局最优解，在后面的文章中[SIGAI](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26scene%3D21%23wechat_redirect)会给出证明，请关注我们的微信公众号。

另外，上述问题是满足Slater条件的。如果令w=0，b=0，![\xi_{i}](https://www.zhihu.com/equation?tex=%5Cxi_%7Bi%7D)=0，则有：
![](https://pic3.zhimg.com/v2-db39b42a41e036b9f9b57580f06ff87a_b.jpg)
不等式条件严格满足，因此强对偶条件成立，原问题和对偶问题有相同的最优解。因此可以转化成对偶问题求解，这样做的原因是原问题的不等式约束太多太复杂，不易于求解。

## 对偶问题

下面介绍如何将原问题转化成对偶问题。首先将上面最优化问题的等式和不等式约束方程写成标准形式：
![](https://pic4.zhimg.com/v2-86fc191685a8a2e8a3c85f88fef32913_b.jpg)
然后构造拉格朗日乘子函数：
![](https://pic3.zhimg.com/v2-2f1389e1b6546566e0737a7ad7dee60a_b.jpg)
其中 ![\alpha，\beta](https://www.zhihu.com/equation?tex=%5Calpha%EF%BC%8C%5Cbeta) 是拉格朗日乘子。转换成对偶问题的具体做法是先固定住这两组乘子变量，对w，b， ![\xi](https://www.zhihu.com/equation?tex=%5Cxi) 求偏导数并令它们为0，得到如下方程组：
![](https://pic4.zhimg.com/v2-11e0dd89de08009c1cfbf1cd28345e1b_b.jpg)
将上面的这些结果代入拉格朗日函数中，消掉这些变量，得到关于乘子变量 ![\alpha，\beta](https://www.zhihu.com/equation?tex=%5Calpha%EF%BC%8C%5Cbeta) 的函数，然后控制乘子变量，对函数取极大值
![](https://pic4.zhimg.com/v2-77d5e68482e56e8734504c4a2457e74b_b.jpg)
由于有等式约束 ![a_{i}+b_{i}=c](https://www.zhihu.com/equation?tex=a_%7Bi%7D%2Bb_%7Bi%7D%3Dc) , 并且有不等式约束 ![\beta_{i}\geq 0](https://www.zhihu.com/equation?tex=%5Cbeta_%7Bi%7D%5Cgeq+0) , 因此有 ![\alpha_{i}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D)![\leq c](https://www.zhihu.com/equation?tex=%5Cleq+c)

这等价与如下最优化问题
![](https://pic4.zhimg.com/v2-8ad4c2d4d7bfb97c395b30fb448cbd53_b.jpg)
转化成对偶问题之后，不等式和等式约束都很简单，求解更为容易。可以证明，上面这个问题是也凸优化问题，可以保证求得全局最优解，在SIGAI后续的文章中我们将给出证明，请大家关注我们的微信公众号。将w的值代入超平面方程，最后的策函数为：
![](https://pic3.zhimg.com/v2-a43a2efa5c79be23076148c1826e8eba_b.jpg)
那些 ![\alpha_{i}\ne 0](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%5Cne+0) 的样本即为支持向量，下面是支持向量的示意图：
![](https://pic1.zhimg.com/v2-5fc37388c42bb3dbe607a66b630eb598_b.jpg)支持向量示意图
## 核函数

虽然加入了松弛变量和惩罚因子，但支持向量机还是一个线性模型，只是允许错分样本的存在，这从它的决策函数也可以看出来。接下来要介绍的核映射使得支持向量机成为非线性模型，决策边界不再是线性的超平面，而可以是形状非常复杂的曲面。

如果样本线性不可分，可以对特征向量进行映射将它转化到一般来说更高维的空间，使得在该空间中是线性可分的，这种方法在机器学习中被称为核技巧。核映射将特征向量变换到另外一个空间：
![](https://pic2.zhimg.com/v2-2628b322aa399bcf24eafb239b232ad5_b.jpg)
在对偶问题中计算的是两个样本向量之间的内积，因此映射后的向量在对偶问题中为：
![](https://pic1.zhimg.com/v2-d44395066a858cdbbd2effbb2d3ddd34_b.jpg)
直接计算效率太低，而且不容易构造映射函数。如果映射函数选取得当，能够确保存在函数K，使得下面等式成立：
![](https://pic4.zhimg.com/v2-8b30a2af43a3dbc3ab01188adaf1e997_b.jpg)
这样只需先对向量做内积然后用函数K进行变换，这等价于先对向量做核映射然后再做内积。在这里我们看到了求解对偶问题的另外一个好处，对偶问题中出现的是样本特征向量之间的内积，而核函数刚好作用于这种内积，替代对特征向量的核映射。满足上面条件的函数称为核函数，常用的核函数有以下几种：
![](https://pic3.zhimg.com/v2-6dda2a5ff9ef87196e3dca06cc51936a_b.jpg)各种核函数与它们的计算公式
核函数的精妙之处在于不用真的对特征向量做核映射，而是直接对特征向量的内积进行变换，而这种变换却等价于先对特征向量做核映射然后做内积。

为向量加上核映射后，要求解的最优化问题变为：
![](https://pic3.zhimg.com/v2-fe9738b4c2f5e31fc6c3fd1830497246_b.jpg)
根据核函数满足的等式条件，它等价于下面的问题：
![](https://pic3.zhimg.com/v2-ad74025956286d14da3ce25df9834186_b.jpg)
最后得到的分类判别函数为：
![](https://pic4.zhimg.com/v2-e78ceb40a9464450f5dfb3b2d3acdf0b_b.jpg)
和不用核映射相比，只是求解的目标函数、最后的判定函数对特征向量的内积做了核函数变换。如果K是一个非线性函数，上面的决策函数则是非线性函数，此时SVM是非线性模型。当训练样本很多、支持向量的个数很大的时候，预测时的速度是一个问题，因此很多时候我们会使用线性支持向量机。

如果我们定义矩阵Q，其元素为：
![](https://pic2.zhimg.com/v2-0cd782b59d294a51ce55280d8112b8a1_b.jpg)
同时定义矩阵K，其元素为：
![](https://pic2.zhimg.com/v2-3d169ecb0bf1f39c562b32d3b231c9bd_b.jpg)
对偶问题可以写成矩阵和向量形式：
![](https://pic3.zhimg.com/v2-c614a050735f5add7af72560d7732bb6_b.jpg)
可以证明，这个对偶问题同样是凸优化问题，这是由核函数的性质保证的，在SIGAI公众号SVM系列的后续文章中我们会介绍。下图是使用高斯核的SVM对异或问题的分类结果：
![](https://pic3.zhimg.com/v2-6f8f275c4835a3fc4da10975b7341cda_b.jpg)
只要参数设置得当，使用高斯核的支持向量机确实能解决非线性分类问题，分类边界可以是非常复杂的曲线。

## KKT条件

对于带等式和不等式约束的问题，在最优点处必须满足KKT条件，将KKT条件应用于SVM原问题的拉格朗日乘子函数，得到关于所有变量的方程，对于原问题中的两组不等式约束，根据KKT条件必须满足：
![](https://pic3.zhimg.com/v2-75addc6526c5e1e6fba14da595ca7086_b.jpg)
对于第一个方程，如果 ![\alpha_{i}>0](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%3E0) ，则必须有
![](https://pic3.zhimg.com/v2-f07d77779908954c3991bfbdeed0c1fa_b.jpg)
即
![](https://pic3.zhimg.com/v2-504fc0b14e41e701ecbd16d438223f2a_b.jpg)
而由于 ![\xi\geq0](https://www.zhihu.com/equation?tex=%5Cxi%5Cgeq0) ，因此必定有
![](https://pic1.zhimg.com/v2-d0f13742e1d584d42d5337805f097e14_b.jpg)
再来看第二种情况：如果 ![\alpha_{i}=0](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%3D0) ，则对
![](https://pic3.zhimg.com/v2-33edcbe1cbf1ae6b728d727b8b2eeb86_b.jpg)
的值没有约束。由于有 ![a_{i}+b_{i}=C](https://www.zhihu.com/equation?tex=a_%7Bi%7D%2Bb_%7Bi%7D%3DC) 的约束，因此 ![\beta_{i}=C](https://www.zhihu.com/equation?tex=%5Cbeta_%7Bi%7D%3DC) ；又因为 ![\beta_{i}\xi_{i}=0](https://www.zhihu.com/equation?tex=%5Cbeta_%7Bi%7D%5Cxi_%7Bi%7D%3D0)

的限制，如果 ![\beta_{i}>0](https://www.zhihu.com/equation?tex=%5Cbeta_%7Bi%7D%3E0)，则必须有 ![\xi_{i}=0](https://www.zhihu.com/equation?tex=%5Cxi_%7Bi%7D%3D0) 。由于原问题中有约束条件
![](https://pic4.zhimg.com/v2-219cd33f330809890e343eb013ecbe17_b.jpg)
而由于![\xi_{i}=0](https://www.zhihu.com/equation?tex=%5Cxi_%7Bi%7D%3D0)，因此有
![](https://pic4.zhimg.com/v2-9fc34d886a6f135ac0acc9901dbee877_b.jpg)
对于 ![\alpha_{i}>0](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%3E0) 的情况，我们又可以细分为 ![\alpha_{i}<C](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%3CC) 和 ![\alpha_{i}=C](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%3DC) 。如果![\alpha_{i}<C](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%3CC)，由于有 ![a_{i}+\beta_{i}=C](https://www.zhihu.com/equation?tex=a_%7Bi%7D%2B%5Cbeta_%7Bi%7D%3DC) 的约束，因此有![\beta_{i}>0](https://www.zhihu.com/equation?tex=%5Cbeta_%7Bi%7D%3E0)；因为有 ![\beta_{i}\xi_{i}=0](https://www.zhihu.com/equation?tex=%5Cbeta_%7Bi%7D%5Cxi_%7Bi%7D%3D0) 的约束，因此 ![\xi_{i}=0](https://www.zhihu.com/equation?tex=%5Cxi_%7Bi%7D%3D0) 。不等式约束：
![](https://pic4.zhimg.com/v2-21812d6acbabbc2aeb3a47a45b05ac0f_b.jpg)
变为
![](https://pic2.zhimg.com/v2-a8bf6c17d21669a718fa93f5bf85c411_b.jpg)
$$由于 ![0<\alpha_{i}<C](https://www.zhihu.com/equation?tex=0%3C%5Calpha_%7Bi%7D%3CC) 时，既要满足
![](https://pic4.zhimg.com/v2-ecc90b4956a25fea1f8ff524f08de77f_b.jpg)
又要满足
![](https://pic1.zhimg.com/v2-734b06f6163ea74fb54173bb7dc93d50_b.jpg)
因此有
![](https://pic4.zhimg.com/v2-81b149890ff901e3b19891e526578eab_b.jpg)
将三种情况合并起来，在最优点处，所有的样本都必须满足：
![](https://pic1.zhimg.com/v2-c7528a51978bee44bd03ae3c8fe24b60_b.jpg)
上面第一种情况对应的是自由变量即非支持向量，第二种情况对应的是支持向量，第三种情况对应的是违反不等式约束的样本。在后面的求解算法中，会应用此条件来选择优化变量。

## SMO算法

前面我们给出了SVM的对偶问题，但并没有说明对偶问题怎么求解。由于矩阵Q的规模和样本数相等，当训练样本数很大的时候，这个矩阵的规模很大，求解二次规划问题的经典算法将会遇到性能问题。下面将介绍SVM最优化问题的高效求解算法-经典的SMO算法。

SMO算法由Platt等人在1998年提出，是求解SVM对偶问题的高效算法。这个算法的思路是每次在优化变量中挑出两个分量进行优化，而让其他分量固定，这样才能保证满足等式约束条件，这是一种分治法的思想。

下面先给出对于这两个变量的优化问题（称为子问题）的求解方法。假设选取的两个分量为 ![\alpha_{i},\alpha_{j}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%2C%5Calpha_%7Bj%7D) ，其他分量都固定即当成常数。由于
![](https://pic1.zhimg.com/v2-ee7bafb25b562b3f383e8ab3ee98e1c0_b.jpg)
以及
![](https://pic2.zhimg.com/v2-e66bc4d462da235a6b85a54ea3094dc1_b.jpg)
对这两个变量的目标函数可以写成：
![](https://pic2.zhimg.com/v2-d0d88f1776537fcfc2900c8db94a0995_b.jpg)
其中c是一个常数。前面的二次项很容易计算出来，一次项要复杂一些，其中：
![](https://pic4.zhimg.com/v2-ae323c6ec7558aa68afd1a75d9fc1013_b.jpg)
这里的变量 ![\alpha^{*}](https://www.zhihu.com/equation?tex=%5Calpha%5E%7B%2A%7D) 为变量а在上一轮迭代后的值。上面的目标函数是一个两变量的二次函数，我们可以直接给出最小值的解析解（公式解），只要你学过初中数学，都能理解这个方法。这个问题的约束条件为：
![](https://pic3.zhimg.com/v2-a22bddbb80d3e79b6b1cb1cbfdac9a62_b.jpg)
前面两个不等式约束构成了一个矩形，最后一个等式约束是一条直线。由于 ![y_{i}y_{j}](https://www.zhihu.com/equation?tex=y_%7Bi%7Dy_%7Bj%7D) 的取值只能为+1或者-1，如果它们异号，等式约束为 ![\alpha_{i}-\alpha_{j}=\xi](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D-%5Calpha_%7Bj%7D%3D%5Cxi) 它确定的可行域是一条斜率为1的直线段。因为 ![\alpha_{i},\alpha_{j}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%2C%5Calpha_%7Bj%7D) ,，要满足约束条件 ![0\leq \alpha_{i} \leq C](https://www.zhihu.com/equation?tex=0%5Cleq+%5Calpha_%7Bi%7D+%5Cleq+C) 和 ![0\leq \alpha_{j} \leq C](https://www.zhihu.com/equation?tex=0%5Cleq+%5Calpha_%7Bj%7D+%5Cleq+C) ，它们的可行域如下图所示：
![](https://pic1.zhimg.com/v2-45830b796bb5148f418f08cda063a004_b.jpg)
上图中的两条直线分别对应于 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 为1和-1的情况。如果是上面那条直线，则 ![\alpha_{j}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bj%7D) 的取值范围为 ![[-\xi,C]](https://www.zhihu.com/equation?tex=%5B-%5Cxi%2CC%5D) 。如果是下面的那条直线，则为 ![[0,C-\xi]](https://www.zhihu.com/equation?tex=%5B0%2CC-%5Cxi%5D) 。

对于这两种情况![\alpha_{j}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bj%7D)的下界和上界可以统一写成如下形式：
![](https://pic3.zhimg.com/v2-b181b2046beb12478b460548f4c140a2_b.jpg)
下边界是直线和x轴交点的x坐标以及0的较大值；上边界是直线和的交点的x坐标和C的较小值。

再来看第二种情况。如果 ![y_{i},y_{j}](https://www.zhihu.com/equation?tex=y_%7Bi%7D%2Cy_%7Bj%7D) 同号，等式约束为 ![y_{i}+y_{j}=\xi](https://www.zhihu.com/equation?tex=y_%7Bi%7D%2By_%7Bj%7D%3D%5Cxi)

此时的下界和上界为：
![](https://pic2.zhimg.com/v2-ffc43b50f05747d8f79c0722866b6811_b.jpg)
这种情况如下图所示：

![](https://pic4.zhimg.com/v2-4494992d2a9f1e1d4858b20a81dd33eb_b.jpg)
利用这两个变量的等式约束条件，可以消掉 ![\alpha_{i}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D) 只剩下一个变量 ![\alpha_{j}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bj%7D) ，目标函数是它的二次函数。我们可以直接求得这个二次函数的极值，假设不考虑约束条件得到的极值点为，则最终的极值点为：
![](https://pic4.zhimg.com/v2-d593c8f00c9dad4ef23051be6eb42973_b.jpg)
这三种情况如下图所示：

![](https://pic3.zhimg.com/v2-3ee946d740b19f1ba7fbf27ad3075fb6_b.jpg)3种情况下的二次函数极小值
上图中第一种情况是抛物线的最小值点在[L,H]中；第二种情况是抛物线的最小值点大于H，被截断为H；第三种情况是小于L，被截断为L。

下面我们来计算不考虑截断时的函数极值。为了避免分-1和+1两种情况，我们将上面的等式约束两边同乘以 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 有：
![](https://pic1.zhimg.com/v2-c1db04a8d0760a73f82def68a1cf963c_b.jpg)
变形后得到：
![](https://pic3.zhimg.com/v2-41b53cf1338aff0dcf98d958de28258a_b.jpg)
为了表述简介，令 ![W=y_{i}\xi](https://www.zhihu.com/equation?tex=W%3Dy_%7Bi%7D%5Cxi) ，将上面方程代入目标函数中消掉 ![\alpha_{i}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D) ，有：
![](https://pic3.zhimg.com/v2-b8b67aa1edf3960b19160bf4e57685ba_b.jpg)
对自变量求导并令导数为0，得：
![](https://pic2.zhimg.com/v2-0781b4e14cbde2cd99b6b60b2476e861_b.jpg)
而
![](https://pic1.zhimg.com/v2-2dbee77a9c739bafe79cdc4f8496f6e0_b.jpg)
化简得：
![](https://pic1.zhimg.com/v2-263c3f6fb4310a42e4ecfaa869d1f380_b.jpg)
即：
![](https://pic2.zhimg.com/v2-ca69f6660d6f559fb084745073e3cfa9_b.jpg)
将w和v带入，化简得：
![](https://pic4.zhimg.com/v2-dbb363f3e136062e7d96c3e30641b8a7_b.jpg)
如果令
![](https://pic2.zhimg.com/v2-9b6ac9df9ceac41bd4807c656251f41d_b.jpg)
上式两边同时除以 ![\eta](https://www.zhihu.com/equation?tex=%5Ceta) ，可以得到
![](https://pic4.zhimg.com/v2-f2ea23707a64bf2753b15c07fa8be4b7_b.jpg)
其中 ![E_{i}=u_{i}-y_{i}](https://www.zhihu.com/equation?tex=E_%7Bi%7D%3Du_%7Bi%7D-y_%7Bi%7D) ，考虑前面提推导过的约束：
![](https://pic1.zhimg.com/v2-07343702e726bdd9489bf6369232cd80_b.jpg)
 在求得 ![a_{j}](https://www.zhihu.com/equation?tex=a_%7Bj%7D) 之后，根据等式约束条件我们就可以求得另外一个变量的值：
![](https://pic1.zhimg.com/v2-34f3cf671643e6ffa45a79a7d12d0a08_b.jpg)
目标函数的二阶导数为 ![\eta](https://www.zhihu.com/equation?tex=%5Ceta) ，前面假设二阶导数 ![\eta>0](https://www.zhihu.com/equation?tex=%5Ceta%3E0) ，从而保证目标函数是凸函数即开口向上的抛物线，有极小值。如果 ![\eta<0](https://www.zhihu.com/equation?tex=%5Ceta%3C0) 或者 ![\eta=0](https://www.zhihu.com/equation?tex=%5Ceta%3D0) ，该怎么处理？对于线性核或正定核函数，可以证明矩阵K的任意一个上述子问题对应的二阶子矩阵半正定，因此必定有 ![\eta\geq0](https://www.zhihu.com/equation?tex=%5Ceta%5Cgeq0) ，[S](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26scene%3D21%23wechat_redirect)IGAI公众号后面的文章中我们会给出证明。无论本次迭代时 ![\alpha_{i}\alpha_{j}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D%5Calpha_%7Bj%7D) 的初始值是多少，通过上面的子问题求解算法得到是在可行域里的最小值，因此每次求解更新这两个变量的值之后，都能保证目标函数值小于或者等于初始值，即函数值下降。

上面已经解决了两个变量问题的求解，接下来说明怎么选择这两个变量，最简单的是使用启发式规则。第一个变量的选择方法是在训练样本中选取违反KKT条件最严重的那个样本。在前面我们推导过，在最优点处训练样本是否满足KKT条件的判据是：
![](https://pic4.zhimg.com/v2-2976c9a2b51435cc22a4e76a44d605bf_b.jpg)
其中
![](https://pic4.zhimg.com/v2-19f8805ec9f34dc512c29225dc73dfff_b.jpg)
首先遍历所有满足约束条件 ![0<\alpha_{i}<C](https://www.zhihu.com/equation?tex=0%3C%5Calpha_%7Bi%7D%3CC) 的样本点，即位于间隔边界上的支持向量点，检验它们是否满足KKT条件。如果这些样本都满足KKT条件，则遍历整个训练样本集，判断它们是否满足KKT条件，直到找到一个违反KKT条件的变量 ![\alpha_{i}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bi%7D) ，找到了第一个变量之后，接下来寻找 ![\alpha_{j}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bj%7D) ，选择的标准是使得它有足够大的变化。根据前面的推导我们知道 ![a_{j}^{new}](https://www.zhihu.com/equation?tex=a_%7Bj%7D%5E%7Bnew%7D) 依赖于 ![|E_{i}-E_{j}|](https://www.zhihu.com/equation?tex=%7CE_%7Bi%7D-E_%7Bj%7D%7C)，因此，我们选择使得![|E_{i}-E_{j}|](https://www.zhihu.com/equation?tex=%7CE_%7Bi%7D-E_%7Bj%7D%7C)最大的 ![\alpha_{j}](https://www.zhihu.com/equation?tex=%5Calpha_%7Bj%7D) 。由于 ![a_{j}](https://www.zhihu.com/equation?tex=a_%7Bj%7D) 已经选出来了，因此 ![E_{j}](https://www.zhihu.com/equation?tex=E_%7Bj%7D) 已经知道了。如果 ![E_{i}>0](https://www.zhihu.com/equation?tex=E_%7Bi%7D%3E0) 则选择最小的![E_{j}](https://www.zhihu.com/equation?tex=E_%7Bj%7D)，否则选择最大的![E_{j}](https://www.zhihu.com/equation?tex=E_%7Bj%7D)。

至此，我们给出了支持向量机求解的问题的完整推导过程，通过这张图，你将能更容易地理解这个算法，如果在理解的过程中有任何疑问，可以向[S](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26scene%3D21%23wechat_redirect)IGAI公众号发消息，我们将为你解答。

```
推荐文章
[1]  机器学习-波澜壮阔40年 SIGAI 2018.4.13.
[2]  学好机器学习需要哪些数学知识？SIGAI 2018.4.17.
[3]  人脸识别算法演化史 SIGAI 2018.4.20.
[4]  基于深度学习的目标检测算法综述 SIGAI 2018.4.24.
[5]  卷积神经网络为什么能够称霸计算机视觉领域？ SIGAI 2018.4.26
```

![](https://pic1.zhimg.com/v2-7bd1593c1dffc30a2707c117821edef0_b.jpg)

