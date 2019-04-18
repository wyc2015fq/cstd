# （I）Banach空间和不动点定理 2 : 不动点定理和钟摆问题 - 知乎
# 

这一小节我们主要讲“压缩映像原理”或者“Banach不动点”定理，还有这个定理的应用。
> [（I）Banach空间和不动点定理 （1） - 知乎专栏](https://zhuanlan.zhihu.com/p/26346061)

这个定理很“简单”，但是用处很大，可以证明很大一类的“存在性”问题：**钟摆问题、常微分方程基本定理、隐函数定理、和牛顿法的收敛性。** 这个不动点在现在的研究论文中也是基本的方法，很多现在论文中依然使用了这个不动点。它也是不动点定理中的**一条大腿**，另外一条大腿**我后面会提及。**

**本小节的主要知识点**

**一、度量空间**

**二、压缩映像原理（Banach不动点定理）**

**三、各种应用：常微分方程的存在性、隐函数定理和钟摆问题。**

## 一、钟摆问题和一般的常微分方程

我们考虑一个理想的钟摆问题：用长度![l](https://www.zhihu.com/equation?tex=l)的绳子挂着一个重量为![m](https://www.zhihu.com/equation?tex=m)的摆，然后让它摆动。
![](https://pic2.zhimg.com/v2-8e65075f7319ec2766fe96cb29477815_b.png)
![](https://pic1.zhimg.com/v2-91eace608b7e26170fda0cee28306fdc_b.gif)
设某个时刻![t](https://www.zhihu.com/equation?tex=t)的角度为![\theta(t)](https://www.zhihu.com/equation?tex=%5Ctheta%28t%29),则在切线方向上我们用牛顿定理可以得到下面的方程

![-mg\sin \theta(t)=ml\theta''(t)](https://www.zhihu.com/equation?tex=-mg%5Csin+%5Ctheta%28t%29%3Dml%5Ctheta%27%27%28t%29).

过多的物理我不涉及了，有兴趣的看

> [Pendulum (mathematics)](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Pendulum_%28mathematics%29)

从数学来看，上面的方程是一个非线性的常微分方程，我们的在这一节的目标是**证明这个方程的解是存在的。 **

设![x(t):=(\theta(t), \theta'(t))](https://www.zhihu.com/equation?tex=x%28t%29%3A%3D%28%5Ctheta%28t%29%2C+%5Ctheta%27%28t%29%29)和![F(x)=(\theta'(t),-\frac{g}{l}\sin \theta(t))](https://www.zhihu.com/equation?tex=F%28x%29%3D%28%5Ctheta%27%28t%29%2C-%5Cfrac%7Bg%7D%7Bl%7D%5Csin+%5Ctheta%28t%29%29),那么上面的问题化成

![\frac{dx}{dt}=F(x)\quad x(0)=(\theta(0),\theta'(0))](https://www.zhihu.com/equation?tex=%5Cfrac%7Bdx%7D%7Bdt%7D%3DF%28x%29%5Cquad+x%280%29%3D%28%5Ctheta%280%29%2C%5Ctheta%27%280%29%29).

这样引出一个更一般的常微分方程问题
![\frac{dx}{dt}=F(t,x)\quad x(0)=x_0](https://www.zhihu.com/equation?tex=%5Cfrac%7Bdx%7D%7Bdt%7D%3DF%28t%2Cx%29%5Cquad+x%280%29%3Dx_0)

## 二、度量（距离）空间和压缩映像原理

![](https://pic2.zhimg.com/v2-3a2b0d666602d7b9068681b6505ad501_b.png)度量空间![(V,\rho)](https://www.zhihu.com/equation?tex=%28V%2C%5Crho%29)和一个赋范线性空间的区别有哪些？第一、![V](https://www.zhihu.com/equation?tex=V)不满足“线性结构”，它不一定是线性空间。第二、设![\rho(x,y):=\|x-y\|](https://www.zhihu.com/equation?tex=%5Crho%28x%2Cy%29%3A%3D%5C%7Cx-y%5C%7C)，则![\rho(\cdot,\cdot)](https://www.zhihu.com/equation?tex=%5Crho%28%5Ccdot%2C%5Ccdot%29)是一个距离。这个时候![X](https://www.zhihu.com/equation?tex=X)上的一个子集![S](https://www.zhihu.com/equation?tex=S)带上距离![\rho(x,y):=\|x-y\|](https://www.zhihu.com/equation?tex=%5Crho%28x%2Cy%29%3A%3D%5C%7Cx-y%5C%7C)是一个距离空间。 所以一个一个赋范线性空间的子集自然是一个度量空间。 

类似于在上一节的赋范线性空间，我们可以引入收敛、闭集和连续算子；

（1）：如果![\lim_{n\to\infty}\rho(u_n,u)=0](https://www.zhihu.com/equation?tex=%5Clim_%7Bn%5Cto%5Cinfty%7D%5Crho%28u_n%2Cu%29%3D0),我们说![u_n\to u](https://www.zhihu.com/equation?tex=u_n%5Cto+u)。

（2）：开球![B_r(x)=\{y\in V; \rho(y,x)<r\}](https://www.zhihu.com/equation?tex=B_r%28x%29%3D%5C%7By%5Cin+V%3B+%5Crho%28y%2Cx%29%3Cr%5C%7D)

 （3）：一个集合中任意点都可以找一个小开球使得其在这个集合中，它就是开集。

（4）： 我们把闭集定义为开集的补集

  类似于上一节，我们可以证明,请想一想如何证明

![S](https://www.zhihu.com/equation?tex=S)是闭集当且当![(x_n)\subset S, x_n\to x,\implies x\in S](https://www.zhihu.com/equation?tex=%28x_n%29%5Csubset+S%2C+x_n%5Cto+x%2C%5Cimplies+x%5Cin+S).

好了，**我们考虑一个度量空间的完备性问题**：设![(x_n)](https://www.zhihu.com/equation?tex=%28x_n%29)是Cauchy列（也就是基本列：对于![\epsilon>0](https://www.zhihu.com/equation?tex=%5Cepsilon%3E0)，存在![N](https://www.zhihu.com/equation?tex=N), 当![m,n\geq N, ](https://www.zhihu.com/equation?tex=m%2Cn%5Cgeq+N%2C+)时必然有![\rho(u_n,u_m)\leq \epsilon ](https://www.zhihu.com/equation?tex=%5Crho%28u_n%2Cu_m%29%5Cleq+%5Cepsilon+)）。
![](https://pic4.zhimg.com/v2-223e6f0f35c1555faf720b829d7867af_b.png)
## 三、压缩映像原理

现在我们映入一个算子：压缩映像。 

设![T:(V,\rho)\to (V,\rho)](https://www.zhihu.com/equation?tex=T%3A%28V%2C%5Crho%29%5Cto+%28V%2C%5Crho%29),如果存在![L\geq 0](https://www.zhihu.com/equation?tex=L%5Cgeq+0)使得对于任意![x,y\in V](https://www.zhihu.com/equation?tex=x%2Cy%5Cin+V),

![\rho(T(x),T(y))\leq L\rho(x,y)](https://www.zhihu.com/equation?tex=%5Crho%28T%28x%29%2CT%28y%29%29%5Cleq+L%5Crho%28x%2Cy%29)成立，那么我们称这个算子是Lipschitz的。 

一个例子，如果![V=\mathbb{R}](https://www.zhihu.com/equation?tex=V%3D%5Cmathbb%7BR%7D)和![T:\mathbb{R}\to \mathbb{R}](https://www.zhihu.com/equation?tex=T%3A%5Cmathbb%7BR%7D%5Cto+%5Cmathbb%7BR%7D)是一个连续可导函数，如果![\sup_{x\in\mathbb{R} }|T'(x)|<\infty](https://www.zhihu.com/equation?tex=%5Csup_%7Bx%5Cin%5Cmathbb%7BR%7D+%7D%7CT%27%28x%29%7C%3C%5Cinfty)，那么这个算子是Lipschitz的。 

如果![L<1](https://www.zhihu.com/equation?tex=L%3C1)，我们管这个映射是压缩映像。也就是说：

![](https://pic1.zhimg.com/v2-07baf667a74ae9e81d823cc8d9c6d0dc_b.png)对于一个算子![T](https://www.zhihu.com/equation?tex=T)，一个点![x\in V](https://www.zhihu.com/equation?tex=x%5Cin+V)如果它满足![T(x)=x](https://www.zhihu.com/equation?tex=T%28x%29%3Dx),那么我们管叫“不动点”。

![](https://pic4.zhimg.com/v2-0bac2c90b9d58c04693a46dfe5b0a11b_b.png)证明：随便给一个初值![x_0\in V](https://www.zhihu.com/equation?tex=x_0%5Cin+V), 设![x_{n+1}=T(x_n), \, n\geq 0](https://www.zhihu.com/equation?tex=x_%7Bn%2B1%7D%3DT%28x_n%29%2C+%5C%2C+n%5Cgeq+0). 不难发现![\rho(x_{n+1},x_n)\leq L \rho(x_{n},x_{n-1})\leq \cdots \leq L^n\rho(x_1,x_0)](https://www.zhihu.com/equation?tex=%5Crho%28x_%7Bn%2B1%7D%2Cx_n%29%5Cleq+L+%5Crho%28x_%7Bn%7D%2Cx_%7Bn-1%7D%29%5Cleq+%5Ccdots+%5Cleq+L%5En%5Crho%28x_1%2Cx_0%29).

从而我们发现

![\rho(x_{n+k},x_n)\leq \sum_{i=0}^{k-1} \rho(x_{n+i+1},x_{n+i})\leq  \sum_{i=0}^{k-1}  L^{n+i}\rho(x_1,x_0)\leq \frac{L^n}{1-L}\rho(x_1,x_0)](https://www.zhihu.com/equation?tex=%5Crho%28x_%7Bn%2Bk%7D%2Cx_n%29%5Cleq+%5Csum_%7Bi%3D0%7D%5E%7Bk-1%7D+%5Crho%28x_%7Bn%2Bi%2B1%7D%2Cx_%7Bn%2Bi%7D%29%5Cleq++%5Csum_%7Bi%3D0%7D%5E%7Bk-1%7D++L%5E%7Bn%2Bi%7D%5Crho%28x_1%2Cx_0%29%5Cleq+%5Cfrac%7BL%5En%7D%7B1-L%7D%5Crho%28x_1%2Cx_0%29).

也就是说这是一个基本列，由于这个距离空间是完备的，则存在![x\in V](https://www.zhihu.com/equation?tex=x%5Cin+V)使得![x_n\to x](https://www.zhihu.com/equation?tex=x_n%5Cto+x)成立。

![x=\lim_{n\to \infty}x_n=\lim_{n\to \infty} T(x_{n-1})=T(\lim_{n\to\infty} x_{n-1})=T(x)](https://www.zhihu.com/equation?tex=x%3D%5Clim_%7Bn%5Cto+%5Cinfty%7Dx_n%3D%5Clim_%7Bn%5Cto+%5Cinfty%7D+T%28x_%7Bn-1%7D%29%3DT%28%5Clim_%7Bn%5Cto%5Cinfty%7D+x_%7Bn-1%7D%29%3DT%28x%29)。

所以这个是一个不动点。唯一性很简单，大家自己思考。 

## 四、不动点的应用

“不动点定理”是一个大家族，有各种不同的不动点定理。这个定理有什么用？

我们讲一下利用不动点定理的基本逻辑：对于一个存在性问题，我们构造一个度量空间和一个映射，使得存在性问题等价于这个映射的不动点。只要证明这个映射存在不动点，那么原来的存在性问题就搞定了。 

**（1）牛顿法解方程：**

对于一个方程![f(x)=0](https://www.zhihu.com/equation?tex=f%28x%29%3D0),我们考虑算子

![](https://pic2.zhimg.com/v2-553b968525347332dab9ac0f31e0e1e5_b.png)不动点![T(x)=x](https://www.zhihu.com/equation?tex=T%28x%29%3Dx)恰好是方程的根。这里我们要求![ \frac{df}{dx}](https://www.zhihu.com/equation?tex=+%5Cfrac%7Bdf%7D%7Bdx%7D)（至少在根的附近）不等于0。

同时，我们求导发现

![](https://pic4.zhimg.com/v2-f0f2566d048fa424cbbab6fa78a45f1b_b.png)所以在一个根![x_0](https://www.zhihu.com/equation?tex=x_0)附近，导数为0，可以保证在这个点附近![B_r(x_0)](https://www.zhihu.com/equation?tex=B_r%28x_0%29)，算子是压缩映射。 同时也不难发现如果![r](https://www.zhihu.com/equation?tex=r)充分小，我们可以保证![T(B_r(x_0))\subset B_r(x_0)](https://www.zhihu.com/equation?tex=T%28B_r%28x_0%29%29%5Csubset+B_r%28x_0%29). 根据压缩映射原理，算子![T](https://www.zhihu.com/equation?tex=T)在度量空间![B_r(x)](https://www.zhihu.com/equation?tex=B_r%28x%29)有唯一的不动点。 而且，我们的“压缩映像原理”中的证明说明数列

![](https://pic3.zhimg.com/v2-7055c7d84ebbf11039f6f5b44583483a_b.png)收敛。这也是数值解方程的一种思路，这种方法叫牛顿法。
![](https://pic2.zhimg.com/v2-42d7bc43c6f18044b1b8e98fffcd14dd_b.png)

**（2）隐函数定理**
![](https://pic3.zhimg.com/v2-cb18543da59512398bc73b376ec242ee_b.png)
   在这个问题中我们选取的算子是

![(T\varphi)(x)=\varphi(x)-(\frac{\partial f}{\partial y}(x_0,y_0))^{-1}f(x,\varphi(x)) ](https://www.zhihu.com/equation?tex=%28T%5Cvarphi%29%28x%29%3D%5Cvarphi%28x%29-%28%5Cfrac%7B%5Cpartial+f%7D%7B%5Cpartial+y%7D%28x_0%2Cy_0%29%29%5E%7B-1%7Df%28x%2C%5Cvarphi%28x%29%29+),

这里![(\frac{\partial f}{\partial y})^{-1}(x_0,y_0)](https://www.zhihu.com/equation?tex=%28%5Cfrac%7B%5Cpartial+f%7D%7B%5Cpartial+y%7D%29%5E%7B-1%7D%28x_0%2Cy_0%29)的作用是“单位化”这个问题，它可以保证![T](https://www.zhihu.com/equation?tex=T)在![(x_0,y_0)](https://www.zhihu.com/equation?tex=%28x_0%2Cy_0%29)的变化率为0。

具体的证明如下：
![](https://pic2.zhimg.com/v2-34d6df534ab089e4f04eed66d70db0f9_b.png)![](https://pic4.zhimg.com/v2-b30dda5161391670e81489bd76a41317_b.png)![](https://pic4.zhimg.com/v2-2ba5a59e1a0b7d4389a732dcd9f446b7_b.png)

**（3）常微分方程的存在性（一维）**

   对于开头的常微分方程，我们考虑一维的情况，构造函数空间

![C[-h,h]](https://www.zhihu.com/equation?tex=C%5B-h%2Ch%5D)是![[-h,h]](https://www.zhihu.com/equation?tex=%5B-h%2Ch%5D)上的连续函数构成的Banach空间，

和定义它上面的一个算子

![T(x)(t)=\xi+\int_0^t f(s,x(s))ds](https://www.zhihu.com/equation?tex=T%28x%29%28t%29%3D%5Cxi%2B%5Cint_0%5Et+f%28s%2Cx%28s%29%29ds)。

如果这个算子有一个不动点，则

![x(t)=\xi+\int_0^t f(s,x(s))ds](https://www.zhihu.com/equation?tex=x%28t%29%3D%5Cxi%2B%5Cint_0%5Et+f%28s%2Cx%28s%29%29ds)，

而它满足原来的常微分方程。
![](https://pic1.zhimg.com/v2-fc3f2dd12dd8856126b16f9d929c61a0_b.png)
**（4）常微分系统和钟摆问题**

现在我们回到开头的钟摆问题。这个时候，我们需要考虑常微分系统，也就是常微分方程组问题。

这里我们选取的空间需要调整为![C([0,T];\mathbb{R}^N)](https://www.zhihu.com/equation?tex=C%28%5B0%2CT%5D%3B%5Cmathbb%7BR%7D%5EN%29)，范数调整为

![\sup_{t\in[0,T]}(e^{-\gamma t}\|u(t)\|)](https://www.zhihu.com/equation?tex=%5Csup_%7Bt%5Cin%5B0%2CT%5D%7D%28e%5E%7B-%5Cgamma+t%7D%5C%7Cu%28t%29%5C%7C%29). 

这个范数的选取具有某种技巧性，这个技巧可以让证明变得简单，因为上面那个证明有一个缺陷。那就是得要求![h](https://www.zhihu.com/equation?tex=h)充分小，**下面这个证明没有这个缺陷**。 

![](https://pic2.zhimg.com/v2-fe0dc03b12c7746b10f9319768fff669_b.png)![](https://pic3.zhimg.com/v2-96c9b90ab93372846db0373748e46082_b.png)好了，我们应用这个结果，根据一开始的设定，我们不难发现

![\|F((x_1,y_1))-F((x_2,y_2))\|_1=|y_1-y_2|+\frac{g}{l}|\sin(x_1)-\sin(x_2)||\leq \max\{1,\frac{g}{l}\}(|x_1-y_1|+|x_2-y_2|)](https://www.zhihu.com/equation?tex=%5C%7CF%28%28x_1%2Cy_1%29%29-F%28%28x_2%2Cy_2%29%29%5C%7C_1%3D%7Cy_1-y_2%7C%2B%5Cfrac%7Bg%7D%7Bl%7D%7C%5Csin%28x_1%29-%5Csin%28x_2%29%7C%7C%5Cleq+%5Cmax%5C%7B1%2C%5Cfrac%7Bg%7D%7Bl%7D%5C%7D%28%7Cx_1-y_1%7C%2B%7Cx_2-y_2%7C%29)。

所以这个非线性项满足Lipschitz连续性。 这里我们选取1-范数而不是通常的2-范数只是为了计算方便，所以一开始的钟摆问题的解是存在的。

## 下一篇：[（I）Banach空间和不动点定理 3: 紧性 - 知乎专栏](https://zhuanlan.zhihu.com/p/26519111)

