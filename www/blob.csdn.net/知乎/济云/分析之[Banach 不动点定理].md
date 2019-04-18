# 分析之[Banach 不动点定理] - 知乎
# 

现在这里祝各位知乎er 2018年春节愉快. 遇到春节的我很感冒, 停更了一周, 这周一定会加倍补上的. 今天的这一节内容很有意思, 也很简单, 供大家春节一乐吧.

这里我们插入很有意义的一节内容, 简要的介绍一下 **Banach 空间**. 这将对之后我们谈到一致收敛很有用. 本节重点之一是 **Banach 不动点定理**. 下面先给出一个简化版的定理.

> 令 ![I\in \mathbb{R}](https://www.zhihu.com/equation?tex=I%5Cin+%5Cmathbb%7BR%7D) 是一个闭的(不要求有界)的区间. 函数 ![f:I\rightarrow \mathbb{R}](https://www.zhihu.com/equation?tex=f%3AI%5Crightarrow+%5Cmathbb%7BR%7D) 有 ![f(I)\subset I](https://www.zhihu.com/equation?tex=f%28I%29%5Csubset+I) , 并且满足: 对于一个固定的 ![\theta\in [0,1)](https://www.zhihu.com/equation?tex=%5Ctheta%5Cin+%5B0%2C1%29) , 有不等式
![|f(x)-f(y)|\leqslant \theta |x-y|\quad\forall x,y\in I](https://www.zhihu.com/equation?tex=%7Cf%28x%29-f%28y%29%7C%5Cleqslant+%5Ctheta+%7Cx-y%7C%5Cquad%5Cforall+x%2Cy%5Cin+I)
那么 ![f](https://www.zhihu.com/equation?tex=f) 在 ![I](https://www.zhihu.com/equation?tex=I) 上必定存在唯一的**不动点**. i.e ![\xi\in I](https://www.zhihu.com/equation?tex=%5Cxi%5Cin+I) 是 ![f](https://www.zhihu.com/equation?tex=f) 的不动点, 那么 ![f(\xi)=\xi](https://www.zhihu.com/equation?tex=f%28%5Cxi%29%3D%5Cxi) .

下面给出一个简要的证明.思路是简单的.

**证明: **先证明不动点的存在性.  任择 ![x_0\in I](https://www.zhihu.com/equation?tex=x_0%5Cin+I) , 并以迭代的方式构造序列 ![\{x_n\}](https://www.zhihu.com/equation?tex=%5C%7Bx_n%5C%7D) : ![x_n:=f(x_{n-1})~(n\geqslant 1).](https://www.zhihu.com/equation?tex=x_n%3A%3Df%28x_%7Bn-1%7D%29~%28n%5Cgeqslant+1%29.)

这一构造的合理性, 来自于条件 ![f(I)\subset I](https://www.zhihu.com/equation?tex=f%28I%29%5Csubset+I) . 可以说明,这一序列是一个 Cauchy列: 对于 ![n>m,m>1](https://www.zhihu.com/equation?tex=n%3Em%2Cm%3E1) 有
![\begin{align} |x_n - x_m|&=|x_n - x_{n-1}|+|x_{n-1}-x_{n-2}|+\cdots+|x_{m+1}- x_m| \\ &=|f(x_{n-1}) - f(x_{n-2})|+\cdots+|f(x_{m})- f(x_{m-1})| \\ &\leqslant \theta(|x_{n-1}-x_{n-2}|+\cdots+|x_{m+1}- x_m|)\\ &\leqslant \sum_{i=m}^{n-1}\theta^{i}|x_1 - x_0|\\ &= \theta^m\frac{1}{1-\theta}|x_1 - x_0| \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%7Cx_n+-+x_m%7C%26%3D%7Cx_n+-+x_%7Bn-1%7D%7C%2B%7Cx_%7Bn-1%7D-x_%7Bn-2%7D%7C%2B%5Ccdots%2B%7Cx_%7Bm%2B1%7D-+x_m%7C+%5C%5C+%26%3D%7Cf%28x_%7Bn-1%7D%29+-+f%28x_%7Bn-2%7D%29%7C%2B%5Ccdots%2B%7Cf%28x_%7Bm%7D%29-+f%28x_%7Bm-1%7D%29%7C+%5C%5C+%26%5Cleqslant+%5Ctheta%28%7Cx_%7Bn-1%7D-x_%7Bn-2%7D%7C%2B%5Ccdots%2B%7Cx_%7Bm%2B1%7D-+x_m%7C%29%5C%5C+%26%5Cleqslant+%5Csum_%7Bi%3Dm%7D%5E%7Bn-1%7D%5Ctheta%5E%7Bi%7D%7Cx_1+-+x_0%7C%5C%5C+%26%3D+%5Ctheta%5Em%5Cfrac%7B1%7D%7B1-%5Ctheta%7D%7Cx_1+-+x_0%7C+%5Cend%7Balign%7D)

而 ![\theta<1](https://www.zhihu.com/equation?tex=%5Ctheta%3C1) , 由此我们便可推出序列 ![\{x_n\}](https://www.zhihu.com/equation?tex=%5C%7Bx_n%5C%7D) 是一个 Cauchy列. 因 ![\mathbb{R}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D) 是完备的, 这个序列必然收敛到某一 ![\xi \in \mathbb{R}](https://www.zhihu.com/equation?tex=%5Cxi+%5Cin+%5Cmathbb%7BR%7D) , 而 ![I](https://www.zhihu.com/equation?tex=I) 是闭的, 所以 ![\xi\in I](https://www.zhihu.com/equation?tex=%5Cxi%5Cin+I) . 另外, 由满足的不等式可知, 函数 ![f](https://www.zhihu.com/equation?tex=f) 是连续的. 故有

![f(\xi)=\lim_{n\rightarrow \infty}f(x_n)= \lim_{n\rightarrow \infty}x_{n+1}=\xi.](https://www.zhihu.com/equation?tex=f%28%5Cxi%29%3D%5Clim_%7Bn%5Crightarrow+%5Cinfty%7Df%28x_n%29%3D+%5Clim_%7Bn%5Crightarrow+%5Cinfty%7Dx_%7Bn%2B1%7D%3D%5Cxi.)

至此我们就证明了不动点的存在性.下面证明不动点的唯一性.

假若存在一个以上的不动点, 比如 ![\xi_1, \xi_2](https://www.zhihu.com/equation?tex=%5Cxi_1%2C+%5Cxi_2) 都是不动点, 那么 ![f(\xi_1)=\xi_1, f(\xi_2)=\xi_2](https://www.zhihu.com/equation?tex=f%28%5Cxi_1%29%3D%5Cxi_1%2C+f%28%5Cxi_2%29%3D%5Cxi_2) . 由定理要求满足的不等式, 有

![|\xi_1-\xi_2|=|f(\xi_1)-f(\xi_2)|\leqslant \theta |\xi_1-\xi_2|.](https://www.zhihu.com/equation?tex=%7C%5Cxi_1-%5Cxi_2%7C%3D%7Cf%28%5Cxi_1%29-f%28%5Cxi_2%29%7C%5Cleqslant+%5Ctheta+%7C%5Cxi_1-%5Cxi_2%7C.)
注意到 ![\theta<1](https://www.zhihu.com/equation?tex=%5Ctheta%3C1) , 显然只有 ![\xi_1=\xi_2](https://www.zhihu.com/equation?tex=%5Cxi_1%3D%5Cxi_2) 才不致矛盾. 这便说明了不动点的唯一性.

> **[推论]**
令 ![I\in \mathbb{R}](https://www.zhihu.com/equation?tex=I%5Cin+%5Cmathbb%7BR%7D) 是一个闭区间. ![f:I\rightarrow \mathbb{R}](https://www.zhihu.com/equation?tex=f%3AI%5Crightarrow+%5Cmathbb%7BR%7D) 是一个可微函数, 有 ![f(I)\subset I](https://www.zhihu.com/equation?tex=f%28I%29%5Csubset+I) , 并且满足: 对于一个固定的 ![\theta\in (0,1)](https://www.zhihu.com/equation?tex=%5Ctheta%5Cin+%280%2C1%29) , 有不等式 ![|f'(x)|\leqslant \theta \quad\forall x\in I.](https://www.zhihu.com/equation?tex=%7Cf%27%28x%29%7C%5Cleqslant+%5Ctheta+%5Cquad%5Cforall+x%5Cin+I.)  那么 ![f](https://www.zhihu.com/equation?tex=f) 在 ![I](https://www.zhihu.com/equation?tex=I) 上必定存在唯一的不动点.

前面讲过度量空间, 度量空间中的元素之间定义了距离.度量空间是由距离生成的拓扑结构, 对于分析问题来说往往是不够用的, 我们**还需要考虑代数结构**. 在[代数学(线性代数部分)之[线性空间]](https://zhuanlan.zhihu.com/p/33372427)中引入了线性空间的这一代数结构. 下面介绍的是一类很重要的度量空间: **赋范线性空间**. 顾名思义, **把拓扑结构:距离,和代数结构:线性空间结合, 而且对于每个元素赋予了一个称为“范数”的量.**

> 令 ![V](https://www.zhihu.com/equation?tex=V) 是 ![\mathbb{R}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D) (或者 ![\mathbb{C}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BC%7D) )上的线性空间. 映射 ![\lVert\cdot\rVert:V\rightarrow \mathbb{R}](https://www.zhihu.com/equation?tex=%5ClVert%5Ccdot%5CrVert%3AV%5Crightarrow+%5Cmathbb%7BR%7D)  称为一个**范数**,  如果其满足:
(i) 对于 ![v\in V\setminus\{0\}](https://www.zhihu.com/equation?tex=v%5Cin+V%5Csetminus%5C%7B0%5C%7D) , ![\lVert v\rVert>0](https://www.zhihu.com/equation?tex=%5ClVert+v%5CrVert%3E0) ;
(ii) 对于 ![v\in V, \lambda\in \mathbb{R}](https://www.zhihu.com/equation?tex=v%5Cin+V%2C+%5Clambda%5Cin+%5Cmathbb%7BR%7D) , 有 ![\lVert\lambda v\rVert= |\lambda|\lVert v\rVert](https://www.zhihu.com/equation?tex=%5ClVert%5Clambda+v%5CrVert%3D+%7C%5Clambda%7C%5ClVert+v%5CrVert) ;
(iii) 对于 ![v,w \in V](https://www.zhihu.com/equation?tex=v%2Cw+%5Cin+V) , 满足(三角不等式) ![\lVert v+w\rVert\leqslant \lVert v \rVert+\lVert w \rVert](https://www.zhihu.com/equation?tex=%5ClVert+v%2Bw%5CrVert%5Cleqslant+%5ClVert+v+%5CrVert%2B%5ClVert+w+%5CrVert) .
赋予了范数的线性空间,便称为**赋范线性空间**, 记为 ![(V,\lVert \cdot\lVert)](https://www.zhihu.com/equation?tex=%28V%2C%5ClVert+%5Ccdot%5ClVert%29) .

作为一个度量空间, Cauchy序列的概念是自然要有的.

> 赋范线性空间 ![(V,\lVert \cdot\lVert)](https://www.zhihu.com/equation?tex=%28V%2C%5ClVert+%5Ccdot%5ClVert%29) 上的序列 ![\{v_n\}](https://www.zhihu.com/equation?tex=%5C%7Bv_n%5C%7D) 称为柯西列, 当 ![\forall \varepsilon>0, \exists N(\varepsilon)\in \mathbb{N}, \forall n,m >N](https://www.zhihu.com/equation?tex=%5Cforall+%5Cvarepsilon%3E0%2C+%5Cexists+N%28%5Cvarepsilon%29%5Cin+%5Cmathbb%7BN%7D%2C+%5Cforall+n%2Cm+%3EN) ,有 ![\lVert v_n- v_m\rVert](https://www.zhihu.com/equation?tex=%5ClVert+v_n-+v_m%5CrVert) .

在数列与级数一节就提到过完备的度量空间. 定义对于赋范线性空间是一样的.

> **[Banach 空间]**
对于赋范线性空间 ![(V,\lVert \cdot\lVert)](https://www.zhihu.com/equation?tex=%28V%2C%5ClVert+%5Ccdot%5ClVert%29) , 若 ![V](https://www.zhihu.com/equation?tex=V) 中的每一个 Cauchy 列, 都在 ![V](https://www.zhihu.com/equation?tex=V) 中收敛, 那么就称其为**完备的**, 或称 **Banach 空间**. 

Banach 空间的一个子集称为**闭的**, 指 ![\forall \{v_n\}\subset V](https://www.zhihu.com/equation?tex=%5Cforall+%5C%7Bv_n%5C%7D%5Csubset+V) , 若 ![v_n\rightarrow v](https://www.zhihu.com/equation?tex=v_n%5Crightarrow+v) , 则 ![v\in A](https://www.zhihu.com/equation?tex=v%5Cin+A) . 

> **[Banach 不动点定理]**
令 ![(V,\lVert \cdot\lVert)](https://www.zhihu.com/equation?tex=%28V%2C%5ClVert+%5Ccdot%5ClVert%29) 为一 Banach 空间. ![A\subset V](https://www.zhihu.com/equation?tex=A%5Csubset+V) 是一个闭子集. 函数 ![f: A\rightarrow V](https://www.zhihu.com/equation?tex=f%3A+A%5Crightarrow+V) 有 ![f(A)\subset A](https://www.zhihu.com/equation?tex=f%28A%29%5Csubset+A) , 并满足不等式
![\lVert f(v)-f(w)\rVert\leqslant\theta\lVert v-w\rVert\quad \forall v,w \in V](https://www.zhihu.com/equation?tex=%5ClVert+f%28v%29-f%28w%29%5CrVert%5Cleqslant%5Ctheta%5ClVert+v-w%5CrVert%5Cquad+%5Cforall+v%2Cw+%5Cin+V)
其中 ![\theta](https://www.zhihu.com/equation?tex=%5Ctheta) 为一固定的数, 满足 ![0<\theta<1](https://www.zhihu.com/equation?tex=0%3C%5Ctheta%3C1) . 那么![f](https://www.zhihu.com/equation?tex=f) 在 ![A](https://www.zhihu.com/equation?tex=A) 上有唯一的不动点.

在完备的度量空间上, 这个定理又叫**压缩映射原理**. 给定度量空间 ![(X,d)](https://www.zhihu.com/equation?tex=%28X%2Cd%29) , 所谓压缩映射, 即存在 ![0<\alpha<1](https://www.zhihu.com/equation?tex=0%3C%5Calpha%3C1) , 使得映射 ![T:(X,d)\rightarrow(X,d)](https://www.zhihu.com/equation?tex=T%3A%28X%2Cd%29%5Crightarrow%28X%2Cd%29) 满足![d(Tx, Ty)\leqslant \alpha d(x, y)(x,y\in X).](https://www.zhihu.com/equation?tex=d%28Tx%2C+Ty%29%5Cleqslant+%5Calpha+d%28x%2C+y%29%28x%2Cy%5Cin+X%29.)

压缩映射原理, 是泛函分析中最常用,而且形式简单的定理.在分析中诸如隐函数存在性定理等定理, 其实就是这一原理的特殊形式.

接下来更新预告: 一致收敛, Banach 空间, Arzela-Ascoli 定理.

如果您喜欢我的专栏, 欢迎关注并点赞. 有任何问题或者文章错误, 欢迎评论区提问. 谢谢大家~

