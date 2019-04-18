# （I）Banach空间和不动点定理 3: 紧性 - 知乎
# 

## 一、引言：

要学好Banach空间中集合的“几何性质”，两个性质是必须领会的：一个是“紧性”，另一个是“凸性”。还有一个重要的性质：连续性，但是这是Banach空间间算子的性质而不是集合的性质。“紧性”、“凸性”和“连续性”。这三个基本性质要非常熟悉。下面是一个小小的总脉络，看完这篇后请看着这个图去回忆你学会了多少。


![](https://pic3.zhimg.com/v2-52cd3a1be8c68503d164ace2986a8d1e_b.png)
紧性本质上是一个衡量集合是否足够“紧”的性质，在证明存在性的时候具有非常巨大的作用。它最大的作用是在无限维空间中“模仿”有限维的欧式空间。而且它能用来证明很多其他东西，比如代数基本定理。**下一节，我会用一个不动点定理再操一次代数基本定理。代数基本定理的证明方法多到数不清**。

一般的“函数”或者说有限维空间上的函数和无限维空间上的函数的差别是非常巨大的。这一点，我在回答：[能不能把泛函简单地理解为函数？ - 知乎](https://www.zhihu.com/question/56339023/answer/148611460) 中有所说明：

特别的，“连续函数在有界闭集上有界而且可以取到最大和最小值”。这个数学分析中的结论在一般的Banach空间上是不成立的。 

**例子：**
![](https://pic2.zhimg.com/v2-759d80039624be06217d240d02ddc949_b.png)
## 二、定义和基本结论

如果![\{U_\alpha\}_{\alpha\in J}](https://www.zhihu.com/equation?tex=%5C%7BU_%5Calpha%5C%7D_%7B%5Calpha%5Cin+J%7D)是一族开集，它被成为集合![S](https://www.zhihu.com/equation?tex=S)的开覆盖，如果满足![S\subset \cup U_\alpha](https://www.zhihu.com/equation?tex=S%5Csubset+%5Ccup+U_%5Calpha)。![S](https://www.zhihu.com/equation?tex=S)是空间![X](https://www.zhihu.com/equation?tex=X)中的一个子集，如果![S](https://www.zhihu.com/equation?tex=S)的**任意**开覆盖![\{U_\alpha\}_{\alpha\in J}](https://www.zhihu.com/equation?tex=%5C%7BU_%5Calpha%5C%7D_%7B%5Calpha%5Cin+J%7D)一定存在有限子覆盖（也就是![\{U_n\}_{n=1}^N](https://www.zhihu.com/equation?tex=%5C%7BU_n%5C%7D_%7Bn%3D1%7D%5EN)满足![U_n\in \{U_\alpha\}_{\alpha\in J}](https://www.zhihu.com/equation?tex=U_n%5Cin+%5C%7BU_%5Calpha%5C%7D_%7B%5Calpha%5Cin+J%7D)而且![S\subset \cup_{N\geq n\geq 1} U_n](https://www.zhihu.com/equation?tex=S%5Csubset+%5Ccup_%7BN%5Cgeq+n%5Cgeq+1%7D+U_n)),那么这个集合被称为紧集。

恩，这个概念第一眼看起来像天书，不过请多多理解。

还有一个看起来有点眼熟的概念：如果![S](https://www.zhihu.com/equation?tex=S)中的任意无限数列![\{x_\alpha\}](https://www.zhihu.com/equation?tex=%5C%7Bx_%5Calpha%5C%7D)必然存在一个收敛子列![\{x_n\}](https://www.zhihu.com/equation?tex=%5C%7Bx_n%5C%7D)而且它的极限在![S](https://www.zhihu.com/equation?tex=S)中，那么这个集合![S](https://www.zhihu.com/equation?tex=S)被成为列紧集。在度量空间中紧=列紧（这个证明在一般的“点集拓扑”教材中都是存在的）
![](https://pic1.zhimg.com/v2-494f4b76a43271f93721612773336658_b.gif)



**欧式空间中紧集：**

数学分析中的**"开覆盖定理"**怎么说来着？
![](https://pic4.zhimg.com/v2-b02c20ddc8fa9d22b2622b88c712dba3_b.png)
所以开覆盖定理给出了一个“紧性”的等价刻画，但是，这个刻画只在有限维空间中成立。

**反例：**设![e_i=(0,0,\cdots, 1,\cdots,\cdots)](https://www.zhihu.com/equation?tex=e_i%3D%280%2C0%2C%5Ccdots%2C+1%2C%5Ccdots%2C%5Ccdots%29)一个无穷列满足第![i](https://www.zhihu.com/equation?tex=i)个为1，其余为0.设![S](https://www.zhihu.com/equation?tex=S)是![\ell^2](https://www.zhihu.com/equation?tex=%5Cell%5E2)中的所有![e_i](https://www.zhihu.com/equation?tex=e_i)构成的的集合。不难发现这是一个无穷个元素构成的集合，这个集合也是闭集。但是，对于任意![i\neq j](https://www.zhihu.com/equation?tex=i%5Cneq+j),![\|e_i-e_j\|=\sqrt{2}](https://www.zhihu.com/equation?tex=%5C%7Ce_i-e_j%5C%7C%3D%5Csqrt%7B2%7D)成立。也就是说，这个集合不可能有收敛子列，那么这个集合就不是紧的了。

但是，在一般的空间中紧性可以推出“有界”和“闭”。 不妨让我们定义有一个更弱的概念：相对（列）紧。一个集合被成为相对紧如果如果![S](https://www.zhihu.com/equation?tex=S)中的任意无限数列![\{x_\alpha\}](https://www.zhihu.com/equation?tex=%5C%7Bx_%5Calpha%5C%7D)必然存在一个收敛子列![\{x_n\}](https://www.zhihu.com/equation?tex=%5C%7Bx_n%5C%7D)。 

区别在哪里？
![](https://pic4.zhimg.com/v2-41bc2d269f4ba98bba991160743c30a7_b.gif)
相对紧中不要求极限在![S](https://www.zhihu.com/equation?tex=S)中，也就是极限点也许在![S](https://www.zhihu.com/equation?tex=S)外。 举一个例子，![S=(0,1)](https://www.zhihu.com/equation?tex=S%3D%280%2C1%29)

，这个集合在欧式空间中不是紧的，但是它是相对紧的。

我们可以证明“紧”可以推出“相对列紧”，那么反过来呢？事实上，

**紧=相对紧+闭**
![](https://pic4.zhimg.com/v2-a9938d849d0adeb7b15f6ae5ed29c073_b.png)

**相对紧可以推出有界**

![](https://pic3.zhimg.com/v2-a4899deeb85db5cd1e110c3247dd9952_b.png)


现在，我们问一个“开覆盖”定理的“逆问题”： 

**如果一个赋范空间中的有界闭集都是“紧集”，那么这个空间一定是有限维的吗？**

答案是“yes”，**而且事实上只要这个空间中单位球壳是紧的，那么这个空间一定是有限维的。**

![](https://pic2.zhimg.com/v2-f4033360813f8de54d688ba78f0f4265_b.png)

![](https://pic2.zhimg.com/v2-3c8877806f5bd8ace334f5268815b715_b.png)


## 三、紧性的用处

那么紧性有什么用呢？简单地说，紧性可以让我们“像处理有限维”那样处理一般的无限维问题。比如，我们开头说过，“一般空间上连续泛函不一定能在一个有界闭集上取到最小值”，但是它在一个紧集上可以取到最大和最小值。这个定理的本性理解归功于威尔斯特拉斯，所以这个定理也叫“威尔斯特拉斯定理”。
![](https://pic2.zhimg.com/v2-902c034fb2fb79842ef59392f86734f1_b.png)
**想一想，一般的拓扑空间和度量空间中这个定理怎么证明。**

## 代数基本定理的“简单”证明

好了，我们来证一次代数学基本定理（任意复多项式必然有解），这个证明不算短，但是非常“简单”，它利用了非常少的“复分析”知识。

我归纳一下思路，首先我们证明多项式![p(z)](https://www.zhihu.com/equation?tex=p%28z%29)的绝对值一定可以在某个有界集合上取到全局的最小值。也就是说，

![|p(z_0)|=\inf_{z\in \mathbb{C}}|p(z)|](https://www.zhihu.com/equation?tex=%7Cp%28z_0%29%7C%3D%5Cinf_%7Bz%5Cin+%5Cmathbb%7BC%7D%7D%7Cp%28z%29%7C)，这一点不难理解，首先当![|z|\to \infty](https://www.zhihu.com/equation?tex=%7Cz%7C%5Cto+%5Cinfty)的时候，![|p(z)|\to\infty](https://www.zhihu.com/equation?tex=%7Cp%28z%29%7C%5Cto%5Cinfty)。所以我们要找最小值必须限定在一个有界集上，利用（相对）紧性和威尔斯特拉斯定理，这个最小值点必然可以取到。第二步，如果![p(z_0)\neq 0](https://www.zhihu.com/equation?tex=p%28z_0%29%5Cneq+0)，那么我走可以找到一个![z_1](https://www.zhihu.com/equation?tex=z_1)使得![|p(z_1)|<|p(z_0)|](https://www.zhihu.com/equation?tex=%7Cp%28z_1%29%7C%3C%7Cp%28z_0%29%7C)。具体来说把，根据泰勒展开，我们不难发现可以找到![k\geq 1](https://www.zhihu.com/equation?tex=k%5Cgeq+1)和非0的系数使得

![p(z)=p(z_0)+c_k(z-z_0)^k+c_{k+1}(z-z_0)^{k+1}+\cdots+c_n(z-z_0)^n](https://www.zhihu.com/equation?tex=p%28z%29%3Dp%28z_0%29%2Bc_k%28z-z_0%29%5Ek%2Bc_%7Bk%2B1%7D%28z-z_0%29%5E%7Bk%2B1%7D%2B%5Ccdots%2Bc_n%28z-z_0%29%5En)

如果![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)足够小，那么

![|c_{k+1}|\epsilon+|c_{k+2}|\epsilon^2+\cdots +|c_{n}|\epsilon^{n-k}<|c_k|](https://www.zhihu.com/equation?tex=%7Cc_%7Bk%2B1%7D%7C%5Cepsilon%2B%7Cc_%7Bk%2B2%7D%7C%5Cepsilon%5E2%2B%5Ccdots+%2B%7Cc_%7Bn%7D%7C%5Cepsilon%5E%7Bn-k%7D%3C%7Cc_k%7C)和![|c_k|\epsilon^k<|p(z_0)|](https://www.zhihu.com/equation?tex=%7Cc_k%7C%5Cepsilon%5Ek%3C%7Cp%28z_0%29%7C)

成立，而且不难从几何上知道，我们不难找到![z_1](https://www.zhihu.com/equation?tex=z_1)使得![|z_1-z_0|=\epsilon](https://www.zhihu.com/equation?tex=%7Cz_1-z_0%7C%3D%5Cepsilon)而且

![|p(z_0)+c_k(z_1-z_0)^k|=|p(z_0)|-|c_k|\epsilon^k](https://www.zhihu.com/equation?tex=%7Cp%28z_0%29%2Bc_k%28z_1-z_0%29%5Ek%7C%3D%7Cp%28z_0%29%7C-%7Cc_k%7C%5Cepsilon%5Ek)成立。
![](https://pic1.zhimg.com/v2-bbf0289285e2309b2b59de7d102b4230_b.png)


于是，我们发现

![|p(z_1)|\leq |p(z_0)+c_k(z-z_0)^k|+|c_{k+1}(z-z_0)^{k+1}+\cdots+c_n(z-z_0)^n|<|p(z_0)|](https://www.zhihu.com/equation?tex=%7Cp%28z_1%29%7C%5Cleq+%7Cp%28z_0%29%2Bc_k%28z-z_0%29%5Ek%7C%2B%7Cc_%7Bk%2B1%7D%28z-z_0%29%5E%7Bk%2B1%7D%2B%5Ccdots%2Bc_n%28z-z_0%29%5En%7C%3C%7Cp%28z_0%29%7C)

下面附上完整证明：


![](https://pic3.zhimg.com/v2-11353397f86854d964ab5bf2f7ed602a_b.png)![](https://pic1.zhimg.com/v2-5eadda9f54eb7dfede8f6a3097d7bfe8_b.png)![](https://pic3.zhimg.com/v2-5e1492256bcd3df9657b3d539cbdb9ca_b.png)
## 下一集：[（I）Banach空间和不动点定理 4: Schauder 不动点及其应用](https://zhuanlan.zhihu.com/p/26651162?group_id=842140282614067200)

