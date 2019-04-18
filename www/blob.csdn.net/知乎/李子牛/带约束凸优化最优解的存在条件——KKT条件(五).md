# 带约束凸优化最优解的存在条件——KKT条件(五) - 知乎
# 

## 简介

在优化目标函数的时候，有时候我们会考虑其对偶问题。这里有两种考虑：1）对偶问题比原问题容易求解；2）对偶问题能提供一种新的解释。比如对支持向量机的对偶问题分析，我们最后得出分界面只由少量“支持向量”决定。这篇文章将给出对于**带约束凸优化问题的最优解的存在条件**。

## Lagrange对偶函数

考虑标准形式的优化问题(下文称该问题为原问题)：

![\begin{align} \min_{x \in \mathbb{R^n}}\, &f_0(x)  \\   \mathrm{subject \,\, to\,} &f_i(x) \leq 0, i = 1, \cdots, m \\ & h_i(x) = 0, i = 1, \cdots, p  \end{align}  \tag{1} ](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmin_%7Bx+%5Cin+%5Cmathbb%7BR%5En%7D%7D%5C%2C+%26f_0%28x%29++%5C%5C+++%5Cmathrm%7Bsubject+%5C%2C%5C%2C+to%5C%2C%7D+%26f_i%28x%29+%5Cleq+0%2C+i+%3D+1%2C+%5Ccdots%2C+m+%5C%5C+%26+h_i%28x%29+%3D+0%2C+i+%3D+1%2C+%5Ccdots%2C+p++%5Cend%7Balign%7D++%5Ctag%7B1%7D+)

其定义域 ![D = \bigcap_{i=0}^m \mathrm{dom}(f_i) \cap \bigcap_{i=1}^p \mathrm{dom}(h_i)](https://www.zhihu.com/equation?tex=D+%3D+%5Cbigcap_%7Bi%3D0%7D%5Em+%5Cmathrm%7Bdom%7D%28f_i%29+%5Ccap+%5Cbigcap_%7Bi%3D1%7D%5Ep+%5Cmathrm%7Bdom%7D%28h_i%29) ，最优解为 ![x^* = \arg\min_{x \in D} f_0(x)](https://www.zhihu.com/equation?tex=x%5E%2A+%3D+%5Carg%5Cmin_%7Bx+%5Cin+D%7D+f_0%28x%29) ，最优值为 ![p^* = f_0(x^*)](https://www.zhihu.com/equation?tex=p%5E%2A+%3D+f_0%28x%5E%2A%29) 。

定义问题(1)的**Lagrange函数**![L：\mathbb{R^n \times R^m \times R^p} \mapsto \mathbb{R}](https://www.zhihu.com/equation?tex=L%EF%BC%9A%5Cmathbb%7BR%5En+%5Ctimes+R%5Em+%5Ctimes+R%5Ep%7D+%5Cmapsto+%5Cmathbb%7BR%7D) 为

![L(x,\lambda, \nu) = f_0(x) + \sum_{i=1}^m \lambda_i f_i(x) + \sum_{i=1}^p \nu_i h_i(x) \\](https://www.zhihu.com/equation?tex=L%28x%2C%5Clambda%2C+%5Cnu%29+%3D+f_0%28x%29+%2B+%5Csum_%7Bi%3D1%7D%5Em+%5Clambda_i+f_i%28x%29+%2B+%5Csum_%7Bi%3D1%7D%5Ep+%5Cnu_i+h_i%28x%29+%5C%5C)

定义问题(1)的**Lagrange对偶函数**为 ![g: \mathbb{R^m \times R^p \mapsto R}](https://www.zhihu.com/equation?tex=g%3A+%5Cmathbb%7BR%5Em+%5Ctimes+R%5Ep+%5Cmapsto+R%7D) 为

![g(\lambda, \nu) = \inf_{x \in D}L(x, \lambda, \nu) = \inf_{x\in D} \Big(f_0(x) + \sum_{i=1}^m \lambda_i f_i(x) + \sum_{i=1}^p \nu_i h_i(x)\Big) \\](https://www.zhihu.com/equation?tex=g%28%5Clambda%2C+%5Cnu%29+%3D+%5Cinf_%7Bx+%5Cin+D%7DL%28x%2C+%5Clambda%2C+%5Cnu%29+%3D+%5Cinf_%7Bx%5Cin+D%7D+%5CBig%28f_0%28x%29+%2B+%5Csum_%7Bi%3D1%7D%5Em+%5Clambda_i+f_i%28x%29+%2B+%5Csum_%7Bi%3D1%7D%5Ep+%5Cnu_i+h_i%28x%29%5CBig%29+%5C%5C)

不难发现，对偶函数构成了最优值 ![p^*](https://www.zhihu.com/equation?tex=p%5E%2A) 的下界：对任意 ![\lambda \ge 0](https://www.zhihu.com/equation?tex=%5Clambda+%5Cge+0) 和 ![\nu](https://www.zhihu.com/equation?tex=%5Cnu) ，有：

![g(\lambda, \nu) \leq p^* \tag{2}](https://www.zhihu.com/equation?tex=g%28%5Clambda%2C+%5Cnu%29+%5Cleq+p%5E%2A+%5Ctag%7B2%7D)

> ![Proof:](https://www.zhihu.com/equation?tex=Proof%3A)
假设 ![\tilde x](https://www.zhihu.com/equation?tex=%5Ctilde+x) 是问题(1)的一个可行解，根据约束条件有： ![f_i(\tilde x) \leq 0](https://www.zhihu.com/equation?tex=f_i%28%5Ctilde+x%29+%5Cleq+0) 和 ![h_i(\tilde x) = 0](https://www.zhihu.com/equation?tex=h_i%28%5Ctilde+x%29+%3D+0) 。所以对任意![\lambda \ge 0(\lambda_i \ge 0, i=1, \cdots, m)](https://www.zhihu.com/equation?tex=%5Clambda+%5Cge+0%28%5Clambda_i+%5Cge+0%2C+i%3D1%2C+%5Ccdots%2C+m%29) ，有：
![ \sum_{i=1}^m \lambda_i f_i(\tilde x) + \sum_{i=1}^p \nu_i h_i(\tilde x) \leq 0\\](https://www.zhihu.com/equation?tex=+%5Csum_%7Bi%3D1%7D%5Em+%5Clambda_i+f_i%28%5Ctilde+x%29+%2B+%5Csum_%7Bi%3D1%7D%5Ep+%5Cnu_i+h_i%28%5Ctilde+x%29+%5Cleq+0%5C%5C)
将上式应用到Lagrange函数：
![L(\tilde x, \lambda, \nu) = f_0(\tilde x) + \sum_{i=1}^m \lambda_i f_i(\tilde x) + \sum_{i=1}^p \nu_i h_i(\tilde x) \leq f_0(\tilde x)\\](https://www.zhihu.com/equation?tex=L%28%5Ctilde+x%2C+%5Clambda%2C+%5Cnu%29+%3D+f_0%28%5Ctilde+x%29+%2B+%5Csum_%7Bi%3D1%7D%5Em+%5Clambda_i+f_i%28%5Ctilde+x%29+%2B+%5Csum_%7Bi%3D1%7D%5Ep+%5Cnu_i+h_i%28%5Ctilde+x%29+%5Cleq+f_0%28%5Ctilde+x%29%5C%5C)
因此，
![g(\lambda, \nu) = \inf_{x \in D}L(x,\lambda, \nu) \leq L(\tilde x, \lambda, \nu) \leq f_0(\tilde x) \\](https://www.zhihu.com/equation?tex=g%28%5Clambda%2C+%5Cnu%29+%3D+%5Cinf_%7Bx+%5Cin+D%7DL%28x%2C%5Clambda%2C+%5Cnu%29+%5Cleq+L%28%5Ctilde+x%2C+%5Clambda%2C+%5Cnu%29+%5Cleq+f_0%28%5Ctilde+x%29+%5C%5C)
由于每一个可行解 ![\tilde x](https://www.zhihu.com/equation?tex=%5Ctilde+x) 都满足 ![g(\lambda, \nu) \leq f_0(\tilde x)](https://www.zhihu.com/equation?tex=g%28%5Clambda%2C+%5Cnu%29+%5Cleq+f_0%28%5Ctilde+x%29) ，最优解也必然满足。所以, 不等式(2)成立。

根据(2)，我们可以看出Lagrange对偶函数给出了最优值 ![p^*](https://www.zhihu.com/equation?tex=p%5E%2A) 的一个下界。自然，我们想知道从Lagrange对偶函数可以得到的**最好的下界**是什么？

## Lagrange对偶问题

将上面的想法(最好的下界)转化为如下优化问题：

![\begin{align} \max_{\lambda, \nu} \, &g(\lambda, \nu) \\ \mathrm{subject\,\, to} \,&\lambda_i \ge 0  \quad i =1, \cdots, m \end{align} \tag{3}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmax_%7B%5Clambda%2C+%5Cnu%7D+%5C%2C+%26g%28%5Clambda%2C+%5Cnu%29+%5C%5C+%5Cmathrm%7Bsubject%5C%2C%5C%2C+to%7D+%5C%2C%26%5Clambda_i+%5Cge+0++%5Cquad+i+%3D1%2C+%5Ccdots%2C+m+%5Cend%7Balign%7D+%5Ctag%7B3%7D)

其最优解用 ![(\lambda^*, \nu^*)](https://www.zhihu.com/equation?tex=%28%5Clambda%5E%2A%2C+%5Cnu%5E%2A%29) 表示，最优值用 ![d^* = f_0(\lambda^*, \nu^*)](https://www.zhihu.com/equation?tex=d%5E%2A+%3D+f_0%28%5Clambda%5E%2A%2C+%5Cnu%5E%2A%29) 。

此外，**Lagrange对偶问题(3)是一个凸优化问题**，因为极大化目标是关于 ![\lambda, \nu](https://www.zhihu.com/equation?tex=%5Clambda%2C+%5Cnu) 的凹函数(相当于最小化凸函数)，且约束集合是凸集。因此，对偶问题的凸性和原问题是否为凸优化问题无关。

对偶问题的最优值 ![d^*](https://www.zhihu.com/equation?tex=d%5E%2A) 可以帮助我们设计算法的停止条件。一般而言，我们设计迭代算法的停止条件一般为 ![|f(x_{k+1}) - f(x_k)| \leq \epsilon](https://www.zhihu.com/equation?tex=%7Cf%28x_%7Bk%2B1%7D%29+-+f%28x_k%29%7C+%5Cleq+%5Cepsilon)。然而，如果我们事先求出了 ![d^*](https://www.zhihu.com/equation?tex=d%5E%2A) ，那么我们可以设计停止条件为 ![|f(x_k) - d^*| \leq \epsilon](https://www.zhihu.com/equation?tex=%7Cf%28x_k%29+-+d%5E%2A%7C+%5Cleq+%5Cepsilon) 。

这里用一个例子说明以上的概念。

考虑标准线性规划问题：

![\begin{align} \min_{x\in \mathbb{R^n}} \, &c^Tx \\ \mathrm{subject\,\, to}\, & Ax = b \\ & x_i \ge 0 \quad i = 1, \cdots, n \end{align} \tag{4}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmin_%7Bx%5Cin+%5Cmathbb%7BR%5En%7D%7D+%5C%2C+%26c%5ETx+%5C%5C+%5Cmathrm%7Bsubject%5C%2C%5C%2C+to%7D%5C%2C+%26+Ax+%3D+b+%5C%5C+%26+x_i+%5Cge+0+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+n+%5Cend%7Balign%7D+%5Ctag%7B4%7D)

其Lagrange函数为：

![L(x, \lambda, \nu) = c^Tx -\sum_i^m \lambda_i x_i + \sum_i^{p} \nu_i(a_i^Tx-b_i) = -b^T \nu + (c+A^T\nu - \lambda)^Tx \\](https://www.zhihu.com/equation?tex=L%28x%2C+%5Clambda%2C+%5Cnu%29+%3D+c%5ETx+-%5Csum_i%5Em+%5Clambda_i+x_i+%2B+%5Csum_i%5E%7Bp%7D+%5Cnu_i%28a_i%5ETx-b_i%29+%3D+-b%5ET+%5Cnu+%2B+%28c%2BA%5ET%5Cnu+-+%5Clambda%29%5ETx+%5C%5C)

其Lagrange对偶函数为：

![g(\lambda, \nu) = \inf_x L(x, \lambda, \nu) = -b^T\nu + \inf_x (c + A^T\nu -\lambda)^T x \\](https://www.zhihu.com/equation?tex=g%28%5Clambda%2C+%5Cnu%29+%3D+%5Cinf_x+L%28x%2C+%5Clambda%2C+%5Cnu%29+%3D+-b%5ET%5Cnu+%2B+%5Cinf_x+%28c+%2B+A%5ET%5Cnu+-%5Clambda%29%5ET+x+%5C%5C)

注意到关于 ![x](https://www.zhihu.com/equation?tex=x) 的线性函数只有恒为0时才有下界，所以

![g(\lambda, \nu) =  \begin{cases}  -b^T \nu & \text{if} \,\, A^T\nu - \lambda + c= 0  \\ -\infty     &  \text{otherwise} \end{cases} \tag{5}](https://www.zhihu.com/equation?tex=g%28%5Clambda%2C+%5Cnu%29+%3D++%5Cbegin%7Bcases%7D++-b%5ET+%5Cnu+%26+%5Ctext%7Bif%7D+%5C%2C%5C%2C+A%5ET%5Cnu+-+%5Clambda+%2B+c%3D+0++%5C%5C+-%5Cinfty+++++%26++%5Ctext%7Botherwise%7D+%5Cend%7Bcases%7D+%5Ctag%7B5%7D)

其Lagrange对偶问题为：

![\begin{align} \max \,&g(\lambda, \nu) =  \begin{cases}  -b^T \nu & \text{if} \,\, A^T\nu - \lambda + c= 0  \\ -\infty     &  \text{otherwise} \end{cases} \\ \mathrm{subject\,\, to} \, & \lambda_i \ge 0 \quad  i=1, \cdots, m \end{align} \tag{6}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmax+%5C%2C%26g%28%5Clambda%2C+%5Cnu%29+%3D++%5Cbegin%7Bcases%7D++-b%5ET+%5Cnu+%26+%5Ctext%7Bif%7D+%5C%2C%5C%2C+A%5ET%5Cnu+-+%5Clambda+%2B+c%3D+0++%5C%5C+-%5Cinfty+++++%26++%5Ctext%7Botherwise%7D+%5Cend%7Bcases%7D+%5C%5C+%5Cmathrm%7Bsubject%5C%2C%5C%2C+to%7D+%5C%2C+%26+%5Clambda_i+%5Cge+0+%5Cquad++i%3D1%2C+%5Ccdots%2C+m+%5Cend%7Balign%7D+%5Ctag%7B6%7D)

## 弱对偶

现在回到我们刚才的问题——Lagrange对偶函数能够为原问题提供的**最好的下界**是什么？

由不等式(2)可以知道：

![d^* \leq p^* \tag{7}](https://www.zhihu.com/equation?tex=d%5E%2A+%5Cleq+p%5E%2A+%5Ctag%7B7%7D)

其中 ![d^*, p^*](https://www.zhihu.com/equation?tex=d%5E%2A%2C+p%5E%2A) 分别是对偶问题和原问题的最优值。不等式(7)称为弱对偶，该性质对目标函数和约束条件的凸性都没做要求，是普遍性成立的。

## 强对偶

强对偶意味着 ![d^* = p^*](https://www.zhihu.com/equation?tex=d%5E%2A+%3D+p%5E%2A) 。这表明可以通过求解对偶问题而求解原问题，如果对偶问题比较容易求解。

下面我们先给出强对偶下的一个推论——互补松弛性，然后基于互补松弛型给出强对偶的一个必要条件——KKT条件，最后给出凸函数性KKT条件的充分必要性。

## 互补松弛性

假设原问题和对偶问题的最优值都可以达到且相等。令 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 是原问题的最优解， ![(\lambda^*, \nu^*)](https://www.zhihu.com/equation?tex=%28%5Clambda%5E%2A%2C+%5Cnu%5E%2A%29) 是对偶问题的最优解。这表明，

![\begin{align} f_0(x^*) &= g(\lambda^*, \nu^*) \\ & = \inf_{x} \Big(f_0(x) + \sum_{i=1}^m \lambda_i^* f_i(x) + \sum_{i=1}^p \nu_i^* h_i(x)\Big) \\ &\leq f_0(x^*) + \sum_{i=1}^m \lambda_i^* f_i(x^*) + \sum_{i=1}^p \nu_i^* h_i(x^*)\\  &\leq f_0(x^*) \end{align} \tag{8}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+f_0%28x%5E%2A%29+%26%3D+g%28%5Clambda%5E%2A%2C+%5Cnu%5E%2A%29+%5C%5C+%26+%3D+%5Cinf_%7Bx%7D+%5CBig%28f_0%28x%29+%2B+%5Csum_%7Bi%3D1%7D%5Em+%5Clambda_i%5E%2A+f_i%28x%29+%2B+%5Csum_%7Bi%3D1%7D%5Ep+%5Cnu_i%5E%2A+h_i%28x%29%5CBig%29+%5C%5C+%26%5Cleq+f_0%28x%5E%2A%29+%2B+%5Csum_%7Bi%3D1%7D%5Em+%5Clambda_i%5E%2A+f_i%28x%5E%2A%29+%2B+%5Csum_%7Bi%3D1%7D%5Ep+%5Cnu_i%5E%2A+h_i%28x%5E%2A%29%5C%5C++%26%5Cleq+f_0%28x%5E%2A%29+%5Cend%7Balign%7D+%5Ctag%7B8%7D)

其中第一个等式是因为强对偶的定义，第二个等式是Lagrange函数的定义，第三个不等式是根据Lagrange函数关于 ![x](https://www.zhihu.com/equation?tex=x) 求下确界小于等于其在 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 处的值(请确保你理解这个不等式)，最后一个不等式是因为 ![\lambda_i^* \ge0, f_i(x^*) \leq 0, i = 1, \cdots, m](https://www.zhihu.com/equation?tex=%5Clambda_i%5E%2A+%5Cge0%2C+f_i%28x%5E%2A%29+%5Cleq+0%2C+i+%3D+1%2C+%5Ccdots%2C+m) 以及 ![h_i(x^*) = 0, i =1, \cdots, p](https://www.zhihu.com/equation?tex=h_i%28x%5E%2A%29+%3D+0%2C+i+%3D1%2C+%5Ccdots%2C+p) 。因此，在上面的式子链中，两个不等式取等号。

由于第三个不等式可以取等式，这里有一个重要的结论：

![\sum_{i=1}^m \lambda_i^* f_i(x^*) = 0 \\](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5Em+%5Clambda_i%5E%2A+f_i%28x%5E%2A%29+%3D+0+%5C%5C)

事实上，求和的每一项都非正，因此有：

![\lambda_i^* f(x_i^*) = 0 \quad i = 1, \cdots, m \\](https://www.zhihu.com/equation?tex=%5Clambda_i%5E%2A+f%28x_i%5E%2A%29+%3D+0+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+m+%5C%5C)

所以，

![\lambda_i^* > 0 \Rightarrow  f_i(x^*) = 0 \\ f_i(x_i^*)  < 0 \Rightarrow \lambda_i^* = 0](https://www.zhihu.com/equation?tex=%5Clambda_i%5E%2A+%3E+0+%5CRightarrow++f_i%28x%5E%2A%29+%3D+0+%5C%5C+f_i%28x_i%5E%2A%29++%3C+0+%5CRightarrow+%5Clambda_i%5E%2A+%3D+0)

这表明在最优点处，原问题的不等式起作用时，对于的Lagrange问题的对应的不等式不起作用，反之亦然。

## KKT条件

下面给出强对偶性的必要条件。

现在假设 ![f_0, \cdots, f_m, h_1, \cdots, h_p](https://www.zhihu.com/equation?tex=f_0%2C+%5Ccdots%2C+f_m%2C+h_1%2C+%5Ccdots%2C+h_p) 都是可微的，但并**不假设这些函数是凸函数**。强对偶暗示着 ![L(x, \lambda^*, \nu^*)](https://www.zhihu.com/equation?tex=L%28x%2C+%5Clambda%5E%2A%2C+%5Cnu%5E%2A%29) 关于 ![x](https://www.zhihu.com/equation?tex=x) 求极小在 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 取得最小值(请多读几遍这句话，确保你明白这句话的意思)，因此此函数在 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 处的导数必须为0，即，

![\nabla f_0(x^*) + \sum_{i=1}^m \lambda_i^* \nabla f_i(x^*) + \sum_{i=1}^{p} \nu_i^* \nabla h_i(x^*) = 0 \\](https://www.zhihu.com/equation?tex=%5Cnabla+f_0%28x%5E%2A%29+%2B+%5Csum_%7Bi%3D1%7D%5Em+%5Clambda_i%5E%2A+%5Cnabla+f_i%28x%5E%2A%29+%2B+%5Csum_%7Bi%3D1%7D%5E%7Bp%7D+%5Cnu_i%5E%2A+%5Cnabla+h_i%28x%5E%2A%29+%3D+0+%5C%5C)

因此，我们有，

![\begin{align} f_i(x^*) &\leq 0, \quad i = 1, \cdots, m \\ h_i(x^*) &= 0, \quad i = 1, \cdots, p \\ \lambda_i^* &\ge 0, \quad i = 1, \cdots, m \\ \lambda_i^* f_i(x^*) &= 0, \quad i = 1, \cdots, m \\ \nabla f_0(x^*) + \sum_{i=1}^m \lambda_i^* \nabla f_i(x^*) + \sum_{i=1}^{p} \nu_i^* \nabla h_i(x^*) &= 0 \end{align} \tag{9}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+f_i%28x%5E%2A%29+%26%5Cleq+0%2C+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+m+%5C%5C+h_i%28x%5E%2A%29+%26%3D+0%2C+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+p+%5C%5C+%5Clambda_i%5E%2A+%26%5Cge+0%2C+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+m+%5C%5C+%5Clambda_i%5E%2A+f_i%28x%5E%2A%29+%26%3D+0%2C+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+m+%5C%5C+%5Cnabla+f_0%28x%5E%2A%29+%2B+%5Csum_%7Bi%3D1%7D%5Em+%5Clambda_i%5E%2A+%5Cnabla+f_i%28x%5E%2A%29+%2B+%5Csum_%7Bi%3D1%7D%5E%7Bp%7D+%5Cnu_i%5E%2A+%5Cnabla+h_i%28x%5E%2A%29+%26%3D+0+%5Cend%7Balign%7D+%5Ctag%7B9%7D)

我们称(9)为**Karush-Kuhn-Tucker**(KKT)条件。

## 凸问题的KKT条件

**当原问题是凸问题**( ![f_i](https://www.zhihu.com/equation?tex=f_i) 是凸函数， ![h_i](https://www.zhihu.com/equation?tex=h_i) 是仿射函数)，满足KKT条件的点也是原、对偶问题的最优解。换言之，假设 ![\tilde x, \tilde \lambda , \tilde \nu, ](https://www.zhihu.com/equation?tex=%5Ctilde+x%2C+%5Ctilde+%5Clambda+%2C+%5Ctilde+%5Cnu%2C+) 是满足KKT条件的点，

![\begin{align} f_i(\tilde x) &\leq 0, \quad i = 1, \cdots, m \\ h_i(\tilde x) &= 0, \quad i = 1, \cdots, p \\ \tilde \lambda_i &\ge 0, \quad i = 1, \cdots, m \\ \tilde \lambda_i f_i(\tilde x) &= 0, \quad i = 1, \cdots, m \\ \nabla f_0(\tilde x) + \sum_{i=1}^m \tilde \lambda_i \nabla f_i(\tilde x) + \sum_{i=1}^{p} \tilde \nu_i \nabla h_i(\tilde x) &= 0 \end{align} \tag{10}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+f_i%28%5Ctilde+x%29+%26%5Cleq+0%2C+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+m+%5C%5C+h_i%28%5Ctilde+x%29+%26%3D+0%2C+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+p+%5C%5C+%5Ctilde+%5Clambda_i+%26%5Cge+0%2C+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+m+%5C%5C+%5Ctilde+%5Clambda_i+f_i%28%5Ctilde+x%29+%26%3D+0%2C+%5Cquad+i+%3D+1%2C+%5Ccdots%2C+m+%5C%5C+%5Cnabla+f_0%28%5Ctilde+x%29+%2B+%5Csum_%7Bi%3D1%7D%5Em+%5Ctilde+%5Clambda_i+%5Cnabla+f_i%28%5Ctilde+x%29+%2B+%5Csum_%7Bi%3D1%7D%5E%7Bp%7D+%5Ctilde+%5Cnu_i+%5Cnabla+h_i%28%5Ctilde+x%29+%26%3D+0+%5Cend%7Balign%7D+%5Ctag%7B10%7D)

下面，我们证明充分性： ![\tilde x, \tilde \lambda , \tilde \nu](https://www.zhihu.com/equation?tex=%5Ctilde+x%2C+%5Ctilde+%5Clambda+%2C+%5Ctilde+%5Cnu) 一定满足强对偶。

首先，可以看到(10)的前2个式子说明了 ![\tilde x](https://www.zhihu.com/equation?tex=%5Ctilde+x) 是原问题的可行解。此外，由于 ![\tilde \lambda_i \ge 0](https://www.zhihu.com/equation?tex=%5Ctilde+%5Clambda_i+%5Cge+0) ，根据定义， ![L(x, \tilde \lambda, \tilde \nu)](https://www.zhihu.com/equation?tex=L%28x%2C+%5Ctilde+%5Clambda%2C+%5Ctilde+%5Cnu%29) 是凸函数；而且(10)的最后一个式子说明 ![L(x, \tilde \lambda, \tilde \nu)](https://www.zhihu.com/equation?tex=L%28x%2C+%5Ctilde+%5Clambda%2C+%5Ctilde+%5Cnu%29) 在 ![\tilde x](https://www.zhihu.com/equation?tex=%5Ctilde+x) 处的导数为0。因此，(凸函数+导数为0)， ![L(x, \tilde \lambda, \tilde \nu)](https://www.zhihu.com/equation?tex=L%28x%2C+%5Ctilde+%5Clambda%2C+%5Ctilde+%5Cnu%29) 在 ![\tilde x](https://www.zhihu.com/equation?tex=%5Ctilde+x) 处取得最小值。

![\begin{align}  g(\tilde \lambda, \tilde \nu) &= L(\tilde x, \tilde \lambda, \tilde \nu) \\ &= f_0(\tilde x) + \sum_{i=1}^{m} \tilde \lambda_i f_i(\tilde x) + \sum_{i=1}^{p} \tilde \nu_i h_i(\tilde x) \\ & = f_0(\tilde x) \end{align} \tag{11}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D++g%28%5Ctilde+%5Clambda%2C+%5Ctilde+%5Cnu%29+%26%3D+L%28%5Ctilde+x%2C+%5Ctilde+%5Clambda%2C+%5Ctilde+%5Cnu%29+%5C%5C+%26%3D+f_0%28%5Ctilde+x%29+%2B+%5Csum_%7Bi%3D1%7D%5E%7Bm%7D+%5Ctilde+%5Clambda_i+f_i%28%5Ctilde+x%29+%2B+%5Csum_%7Bi%3D1%7D%5E%7Bp%7D+%5Ctilde+%5Cnu_i+h_i%28%5Ctilde+x%29+%5C%5C+%26+%3D+f_0%28%5Ctilde+x%29+%5Cend%7Balign%7D+%5Ctag%7B11%7D)

其中第一个等式是因为 ![g(\lambda, \nu)](https://www.zhihu.com/equation?tex=g%28%5Clambda%2C+%5Cnu%29) 的定义以及![L(x, \tilde \lambda, \tilde \nu)](https://www.zhihu.com/equation?tex=L%28x%2C+%5Ctilde+%5Clambda%2C+%5Ctilde+%5Cnu%29) 在 ![\tilde x](https://www.zhihu.com/equation?tex=%5Ctilde+x) 处取得最小值。第二个等式是因为 ![L(x, \lambda, \nu)](https://www.zhihu.com/equation?tex=L%28x%2C+%5Clambda%2C+%5Cnu%29) 的定义。第三个等式是因为KKT条件的第2个和第4个式子。至此，(11)说明了对于凸问题，满足KKT条件的 ![\tilde x](https://www.zhihu.com/equation?tex=%5Ctilde+x) 意味着强对偶。

总结

我们在这篇文章里说明了原问题和对偶问题的关系——**对偶函数为原函数的最优值提供了下界保证**。此外，说明了弱对偶以及强对偶的关系——**弱对偶普遍存在，强对偶要求满足KKT；以及松弛互补**。最后，**对于凸问题，KKT条件是充要条件**。

阅读本系列的其他文章
- [凸优化系列(一)](https://zhuanlan.zhihu.com/p/50283897)
- [凸优化问题介绍(二)](https://zhuanlan.zhihu.com/p/50290361)
- [无约束凸优化问题求解——Gradient Descent(三)](https://zhuanlan.zhihu.com/p/50029469)
- [无约束凸优化问题求解——Newton Method(四)](https://zhuanlan.zhihu.com/p/50348622)
- 带约束凸优化最优解的存在条件——KKT条件(五)
- [带等式约束凸优化问题的求解——Newton Method(六)](https://zhuanlan.zhihu.com/p/50411305)
- [带不等式约束凸优化问题的求解——内点法(七)](https://zhuanlan.zhihu.com/p/50443123)

