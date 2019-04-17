# 三段话搞明白什么是Krylov子空间迭代法 - Machine Learning with Peppa - CSDN博客





2018年04月12日 14:33:55[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：3328










Krylov方法是一种 “降维打击” 手段，有利有弊。其特点一是牺牲了精度换取了速度，二是在没有办法求解大型稀疏矩阵时，他给出了一种办法，虽然不精确。

假设你有一个线性方程组：
![Ax=b](https://www.zhihu.com/equation?tex=Ax%3Db)
其中A是已知矩阵，b是已知向量，x是需要求解的未知向量。

当你有这么个问题需要解决时，一般的思路是直接求A的逆矩阵，然后x就出来了：
![x=A^{-1}b](https://www.zhihu.com/equation?tex=x%3DA%5E%7B-1%7Db+)

但是，如果A的维度很高，比方说1000*1000的矩阵，那么A就是一个大型矩阵，大型矩阵是很难求逆的，如果A还是一个稀疏矩阵，那就更难求了。这时聪明的Krylov想到了一种方法来替换![A^{-1}](https://www.zhihu.com/equation?tex=A%5E%7B-1%7D+)

![A^{-1}b\approx  \sum_{i=0}^{m-1}{\beta_{i}A^{i}b}=\beta_{0}b+ \beta_{1}Ab+\beta_{2}A^{2}b...+\beta_{m-1}A^{m-1}b](https://www.zhihu.com/equation?tex=A%5E%7B-1%7Db%5Capprox++%5Csum_%7Bi%3D0%7D%5E%7Bm-1%7D%7B%5Cbeta_%7Bi%7DA%5E%7Bi%7Db%7D%3D%5Cbeta_%7B0%7Db%2B+%5Cbeta_%7B1%7DAb%2B%5Cbeta_%7B2%7DA%5E%7B2%7Db...%2B%5Cbeta_%7Bm-1%7DA%5E%7Bm-1%7Db)
其中![\beta](https://www.zhihu.com/equation?tex=%5Cbeta)都是未知标量，m是你来假设的一个值，最大不能超过矩阵的维度，比如这里例子里是1000.
瞧，这么一处理，我们就不用算![A^{-1}](https://www.zhihu.com/equation?tex=A%5E%7B-1%7D+)了。我们只要求出方程里那些![\beta](https://www.zhihu.com/equation?tex=%5Cbeta)的值，就齐活儿了。

（Krylov通过数学上的推导证明了，当m趋近于矩阵维度时（这里是1000），算出来的值就是精确解了。当然很少有人会真的把m提到那个数量级来算，那样就等于新构建了一个大型线形方程组，计算量还是很大。不过这么转换一下也不是没有好处，毕竟从稀疏矩阵变为了非稀疏矩阵，好求一点，没准就能直接求逆了。）



(这里省略了几步，还要用Arnoldi方法做个循环，先留个空，有同学需要我再补上)




解![\beta](https://www.zhihu.com/equation?tex=%5Cbeta)值要带回第一个公式，得到以下方程：
![0=b-A x^{(m)}=b-A\sum_{i=0}^{m-1}{\beta_{i}A^{i}b}](https://www.zhihu.com/equation?tex=0%3Db-A+x%5E%7B%28m%29%7D%3Db-A%5Csum_%7Bi%3D0%7D%5E%7Bm-1%7D%7B%5Cbeta_%7Bi%7DA%5E%7Bi%7Db%7D+)

有细心的同学一看，说不对劲啊。b的维度是1000，那就是有1000个方程，![\beta](https://www.zhihu.com/equation?tex=%5Cbeta)的数量小于1000. 那不是方程数大于未知数了吗？这种情况应该没法儿求解啊。

对的，这种情况确实没法儿精确求解，只能求近似解。
方程数大于未知数时常用的方法之一是最小二乘法。那么这里可不可以用最小二乘法呢？

一般来说，最小二乘法应用的最重要的条件之一，就是方程须是线性的，最小二乘法一般只用来解线性方程，解非线性的就非常困难，需要进行一些“魔改”，比如基于最小二乘法的Levenberg-Marquardt and trust-region methods，就是matlab里的fsolve函数调用的算法，这里我就不铺开讲了，免得读者分心。我们观察了一下这个方程，正好就是线性的，那么就可以用。

（岔个话，非线性方程组的求解一直是个“老大难”的问题，一般可用的方法只有Newton（牛顿）法，对就是三百年前英国那个牛顿，这么些年一直没啥进步。我们研究Krylov方法，其最重要最广泛的应用，就是可以跟Newton法结合起来，把牛顿法里一般需要手动求解的一个非常复杂的Jacobian矩阵给省去了。创造这一天才结合的科学家将这种耦合算法称作JFNK，就是Jacobian-free Newton Krylov的缩写，意图一目了然，从此科学家们省去了手推Jacobian矩阵的烦恼，人人用了都说好，所以学Krylov算法的同学不顺便学一学JFNK就是“入宝山而空手归”了。）

令![r^{(m)}=b-A x^{(m)}](https://www.zhihu.com/equation?tex=r%5E%7B%28m%29%7D%3Db-A+x%5E%7B%28m%29%7D)

这里![r^{(m)}](https://www.zhihu.com/equation?tex=r%5E%7B%28m%29%7D)是指当m为m时的残量，所谓残量，就是error，就是我们不想要他存在的一个量。从上面的第一个公式就可以看出来，如果我们最终得出的x完全精确，那么r应该等于0. 于是现在这个问题转变为求一个含有多个自变量的表达式的最小值问题。

含有多个自变量的表达式的最小值问题，可以用最小二乘法来解决。最小二乘法的核心就是以下这些个公式：
![\frac{\partial r}{\partial \beta_{0}} =0,\frac{\partial r}{\partial \beta_{1}} =0,\frac{\partial r}{\partial \beta_{2}} =0,...\frac{\partial r}{\partial \beta_{m-1}} =0](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+r%7D%7B%5Cpartial+%5Cbeta_%7B0%7D%7D+%3D0%2C%0A%5Cfrac%7B%5Cpartial+r%7D%7B%5Cpartial+%5Cbeta_%7B1%7D%7D+%3D0%2C%0A%5Cfrac%7B%5Cpartial+r%7D%7B%5Cpartial+%5Cbeta_%7B2%7D%7D+%3D0%2C...%0A%5Cfrac%7B%5Cpartial+r%7D%7B%5Cpartial+%5Cbeta_%7Bm-1%7D%7D+%3D0%0A)
（注：谢@[渭水泱泱](http://www.zhihu.com/people/wei-shui-yang-yang-84) 提醒，这里的r指的是![r^{(m)}](https://www.zhihu.com/equation?tex=r%5E%7B%28m%29%7D+)的平方和）

意思就是在r为最小值的时候，r关于所有变量的偏导都应当为0，这是毫无疑问的。

于是问题转化为了一个求m个方程m个未知数的方程组的问题，而且m通常不大（当然，m是你自己设定的，设那么大不是自找麻烦么）

这种问题就很好解了，一般用前面的![x=A^{-1}b](https://www.zhihu.com/equation?tex=x%3DA%5E%7B-1%7Db+)方法就可以搞定了。

然后问题解决，战斗结束。
回顾一下，大概是这样一个流程：
大型稀疏矩阵求逆-->Krylov方法-->线性方程最小二乘问题-->小矩阵求逆




