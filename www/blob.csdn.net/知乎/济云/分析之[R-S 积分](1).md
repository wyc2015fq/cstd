# 分析之[R-S 积分](1) - 知乎
# 

计划 R-S 积分这一节主要谈及一元函数的 **Reimann-Stieltjes **积分的定义、性质, 以及一些基本的计算规则; 有界变差函数. 最后作为积分和巴拿赫不动点定理的一个应用, 导出常微分方程中的 **Picard-Lindelof 定理**(局部上解的存在及唯一性).

最近很忙, 姗姗来迟的更新, 这次内容不多, 主要都是一些概念性的东西. 算是[R-S 积分]第(1)部分吧.

积分, 最早是为了求解(不规则的)形状的“面积”而创造出来的.从古希腊阿基米德切片求球体积, 到中国古代“祖暅原理”都已经出现了积分的朴素思想. 
![](https://pic4.zhimg.com/v2-f13e8831eddc65e168b4b08dbaa33c3f_b.jpg)
如果大家会玩 Minecraft 这个游戏, 对这样一个简单而精巧的想法应该也有所体会: 任何现实生活中的形状, 都可以通过一系列的长方体(形)的组合近似(逼近). **以直代曲, 以规则替代不规则**, 一个函数图像与坐标轴围成的面积, 可以通过分割近似为一系列长方形的和来得到. 这里将从最简单的一元函数积分开始, 来介绍积分理论. 而更严格,更灵活的积分理论, 则需要用“测度”的概念, 这留待以后再谈.

## **积分的定义**

## Reimann 积分

将朴素的“积分思想”严格成积分, 我们先给出以下一些定义:

> 我们称 ![\tau:=(x_0,x_1,\cdots,x_n)](https://www.zhihu.com/equation?tex=%5Ctau%3A%3D%28x_0%2Cx_1%2C%5Ccdots%2Cx_n%29) 为区间 ![I:=[a,b]](https://www.zhihu.com/equation?tex=I%3A%3D%5Ba%2Cb%5D) 的一个**分割**, 若
![a=x_0<x_1<\cdots<x_n=b,~~n\in \mathbb{N}_+.](https://www.zhihu.com/equation?tex=a%3Dx_0%3Cx_1%3C%5Ccdots%3Cx_n%3Db%2C~~n%5Cin+%5Cmathbb%7BN%7D_%2B.)
若 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) 是另一个分割 ![\tau'](https://www.zhihu.com/equation?tex=%5Ctau%27) 的子集, 则称 ![\tau'](https://www.zhihu.com/equation?tex=%5Ctau%27) 为分割 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) 的**加细**, 记为 ![\tau'\geqslant\tau](https://www.zhihu.com/equation?tex=%5Ctau%27%5Cgeqslant%5Ctau) .

下面是黎曼和的定义

> **[黎曼和]**
![f](https://www.zhihu.com/equation?tex=f) 为定义在 ![I:=[a,b]](https://www.zhihu.com/equation?tex=I%3A%3D%5Ba%2Cb%5D) 上的实函数. 设 ![\tau:=(x_0,x_1,\cdots,x_n)](https://www.zhihu.com/equation?tex=%5Ctau%3A%3D%28x_0%2Cx_1%2C%5Ccdots%2Cx_n%29) 是区间 ![I](https://www.zhihu.com/equation?tex=I) 的一个分割, 记 ![\Delta x_i =x_i-x_{i-1}](https://www.zhihu.com/equation?tex=%5CDelta+x_i+%3Dx_i-x_%7Bi-1%7D) , 并设 ![\lVert\tau\rVert:=\max_{1\leqslant i\leqslant n}\{\Delta x_i\}](https://www.zhihu.com/equation?tex=%5ClVert%5Ctau%5CrVert%3A%3D%5Cmax_%7B1%5Cleqslant+i%5Cleqslant+n%7D%5C%7B%5CDelta+x_i%5C%7D) ,  对任意 ![\xi_i\in[x_{i-1},x_i]](https://www.zhihu.com/equation?tex=%5Cxi_i%5Cin%5Bx_%7Bi-1%7D%2Cx_i%5D) , 我们称
![\sum_{i=1}^{n}f(\xi_i)\Delta x_i](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7Bn%7Df%28%5Cxi_i%29%5CDelta+x_i)
为 ![f](https://www.zhihu.com/equation?tex=f) 在 ![I](https://www.zhihu.com/equation?tex=I) 上的一个 **Reimann 和**.

黎曼和的几何意义是明显的: 当分割加细,生成的一系列长方形的和也越来也接近于一个函数图像与坐标轴围成的面积.下面给出积分的具体定义.

> 设 ![f](https://www.zhihu.com/equation?tex=f) 为定义在 ![I](https://www.zhihu.com/equation?tex=I) 上的实函数, 若存在实数 ![A](https://www.zhihu.com/equation?tex=A) , 满足 ![\forall \varepsilon>0](https://www.zhihu.com/equation?tex=%5Cforall+%5Cvarepsilon%3E0) , ![\exists \delta>0](https://www.zhihu.com/equation?tex=%5Cexists+%5Cdelta%3E0) , 对于任意的分割 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau)![\lVert\tau\rVert<\delta](https://www.zhihu.com/equation?tex=%5ClVert%5Ctau%5CrVert%3C%5Cdelta) , 有
![\left|\sum_{i=1}^{n}f(\xi_i)\Delta x_i - A\right|<\varepsilon,~~\forall\xi_i\in[x_{i-1},x_i], i=1,\cdots,n,](https://www.zhihu.com/equation?tex=%5Cleft%7C%5Csum_%7Bi%3D1%7D%5E%7Bn%7Df%28%5Cxi_i%29%5CDelta+x_i+-+A%5Cright%7C%3C%5Cvarepsilon%2C~~%5Cforall%5Cxi_i%5Cin%5Bx_%7Bi-1%7D%2Cx_i%5D%2C+i%3D1%2C%5Ccdots%2Cn%2C)
就称函数 ![f](https://www.zhihu.com/equation?tex=f) 在 ![I](https://www.zhihu.com/equation?tex=I) 上 **Reimann可积**, 用 ![f\in \mathscr{R}](https://www.zhihu.com/equation?tex=f%5Cin+%5Cmathscr%7BR%7D) 表示. 同时 ![A](https://www.zhihu.com/equation?tex=A) 称为 ![f](https://www.zhihu.com/equation?tex=f) 在 ![I](https://www.zhihu.com/equation?tex=I) 上的**定积分**, 记为:
![\int_{a}^{b}f(x)dx=\lim_{\lVert\tau\rVert\rightarrow 0}\sum_{i=1}^{n}f(\xi_i)\Delta x_i.](https://www.zhihu.com/equation?tex=%5Cint_%7Ba%7D%5E%7Bb%7Df%28x%29dx%3D%5Clim_%7B%5ClVert%5Ctau%5CrVert%5Crightarrow+0%7D%5Csum_%7Bi%3D1%7D%5E%7Bn%7Df%28%5Cxi_i%29%5CDelta+x_i.)
![f](https://www.zhihu.com/equation?tex=f) 称为**被积函数**,  ![I](https://www.zhihu.com/equation?tex=I) 称为**积分区间**.

容易证明,在区间 ![I](https://www.zhihu.com/equation?tex=I) 上可积的函数, 必然在 ![I](https://www.zhihu.com/equation?tex=I) 上有界. 但是有界函数未必可积. 数学家达布(Darboux) 给出了有界函数黎曼可积的充分必要条件, 这也是我们之后常用的积分的另一种等价定义.(下面参考Rudin书上的符号来)

> **[黎曼积分的达布定义(充要条件)]**
设 ![f](https://www.zhihu.com/equation?tex=f) 为定义在 ![I](https://www.zhihu.com/equation?tex=I) 上的有界实函数, 对应于 ![I](https://www.zhihu.com/equation?tex=I) 的任意分割 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) ,令
![M_i = \sup_{x_{i-1}\leqslant x\leqslant x_i}f(x),\quad m_i = \inf_{x_{i-1}\leqslant x\leqslant x_i}f(x),](https://www.zhihu.com/equation?tex=M_i+%3D+%5Csup_%7Bx_%7Bi-1%7D%5Cleqslant+x%5Cleqslant+x_i%7Df%28x%29%2C%5Cquad+m_i+%3D+%5Cinf_%7Bx_%7Bi-1%7D%5Cleqslant+x%5Cleqslant+x_i%7Df%28x%29%2C)
称 ![U(\tau, f)=\sum_{i=1}^{n}M_i\Delta x_i](https://www.zhihu.com/equation?tex=U%28%5Ctau%2C+f%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7DM_i%5CDelta+x_i)
为 ![f](https://www.zhihu.com/equation?tex=f) 关于 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) 的 **Darboux 上和**;
![L(\tau, f)=\sum_{i=1}^{n}m_i\Delta x_i](https://www.zhihu.com/equation?tex=L%28%5Ctau%2C+f%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7Dm_i%5CDelta+x_i) 为 ![f](https://www.zhihu.com/equation?tex=f) 关于 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) 的 **Darboux 下和**.\\
记
![\overline{\int_{a}^{b}}f(x)dx=\inf{U(\tau, f)},\\ \underline{\int_{a}^{b}}f(x)dx=\sup{L(\tau, f)},](https://www.zhihu.com/equation?tex=%5Coverline%7B%5Cint_%7Ba%7D%5E%7Bb%7D%7Df%28x%29dx%3D%5Cinf%7BU%28%5Ctau%2C+f%29%7D%2C%5C%5C+%5Cunderline%7B%5Cint_%7Ba%7D%5E%7Bb%7D%7Df%28x%29dx%3D%5Csup%7BL%28%5Ctau%2C+f%29%7D%2C)
以上式子中, 上下确界是对所有分割而取的, 左边两式分别称为 ![f](https://www.zhihu.com/equation?tex=f) 在 ![I](https://www.zhihu.com/equation?tex=I) 上的**上积分**和**下积分.函数**![f](https://www.zhihu.com/equation?tex=f)**在**![I](https://www.zhihu.com/equation?tex=I)**上 Reimann可积,当且仅当上积分与下积分相等**.

在上述定理中,涉及到Darboux 上和与Darboux 下和概念, 它们有一些性质值得注意(利用定义容易证明):
-  对于任意分割 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) , 恒有 ![L(\tau, f)\leqslant U(\tau, f)](https://www.zhihu.com/equation?tex=L%28%5Ctau%2C+f%29%5Cleqslant+U%28%5Ctau%2C+f%29) ;
- 若有 ![\tau'\geqslant\tau](https://www.zhihu.com/equation?tex=%5Ctau%27%5Cgeqslant%5Ctau) , 则上和不增, 下和不减.
- 对于任意两个分割 ![\tau_1,\tau_2](https://www.zhihu.com/equation?tex=%5Ctau_1%2C%5Ctau_2) , 恒有 ![L(\tau_1, f)\leqslant U(\tau_2, f)](https://www.zhihu.com/equation?tex=L%28%5Ctau_1%2C+f%29%5Cleqslant+U%28%5Ctau_2%2C+f%29) ;
- ![\sup{L(\tau, f)}\leqslant\inf{U(\tau, f)}](https://www.zhihu.com/equation?tex=%5Csup%7BL%28%5Ctau%2C+f%29%7D%5Cleqslant%5Cinf%7BU%28%5Ctau%2C+f%29%7D) , 即下积分不超过上积分.

接下来, 将黎曼积分稍微推广一下, 我们来考虑更一般的情况.

## Reimann-Stieltjes 积分

1894年,数学家Stieltjes研究连分数时, 将黎曼积分推广, 引入了**Reimann-Stieljes积分**. 利用这个积分可以将关于连续型和离散型概率分布函数的统计学理论化为统一的形式, 是概率论和金融数学等领域的重要工具.

> **[R-S积分]**
	设 ![\alpha](https://www.zhihu.com/equation?tex=%5Calpha) 为 ![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D) 上的一个单调递增函数, ![f](https://www.zhihu.com/equation?tex=f) 为 ![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D) 上的有界函数. 对应于 ![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D) 的任意分割 ![\tau:=(x_0,x_1,\cdots,x_n)](https://www.zhihu.com/equation?tex=%5Ctau%3A%3D%28x_0%2Cx_1%2C%5Ccdots%2Cx_n%29) , 记
![M_i = \sup_{x_{i-1}\leqslant x\leqslant x_i}f(x),\\ m_i = \inf_{x_{i-1}\leqslant x\leqslant x_i}f(x),\\ \Delta\alpha_i=\alpha(x_i)-\alpha(x_{i-1}),](https://www.zhihu.com/equation?tex=M_i+%3D+%5Csup_%7Bx_%7Bi-1%7D%5Cleqslant+x%5Cleqslant+x_i%7Df%28x%29%2C%5C%5C+m_i+%3D+%5Cinf_%7Bx_%7Bi-1%7D%5Cleqslant+x%5Cleqslant+x_i%7Df%28x%29%2C%5C%5C+%5CDelta%5Calpha_i%3D%5Calpha%28x_i%29-%5Calpha%28x_%7Bi-1%7D%29%2C)
定义 ![f](https://www.zhihu.com/equation?tex=f) 关于 ![\alpha](https://www.zhihu.com/equation?tex=%5Calpha) 和 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) 的 Darboux 上和与下和
![U(\tau, f,\alpha)=\sum_{i=1}^{n}M_i\Delta \alpha_i;\\U(\tau, f,\alpha)=\sum_{i=1}^{n}m_i\Delta \alpha_i,](https://www.zhihu.com/equation?tex=U%28%5Ctau%2C+f%2C%5Calpha%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7DM_i%5CDelta+%5Calpha_i%3B%5C%5CU%28%5Ctau%2C+f%2C%5Calpha%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bn%7Dm_i%5CDelta+%5Calpha_i%2C)
类似的,记
![ \overline{\int_{a}^{b}}fd\alpha=\inf{U(\tau, f,\alpha)},\\ \underline{\int_{a}^{b}}fd\alpha=\sup{L(\tau, f,\alpha)},](https://www.zhihu.com/equation?tex=+%5Coverline%7B%5Cint_%7Ba%7D%5E%7Bb%7D%7Dfd%5Calpha%3D%5Cinf%7BU%28%5Ctau%2C+f%2C%5Calpha%29%7D%2C%5C%5C+%5Cunderline%7B%5Cint_%7Ba%7D%5E%7Bb%7D%7Dfd%5Calpha%3D%5Csup%7BL%28%5Ctau%2C+f%2C%5Calpha%29%7D%2C)
	它们分别称为 ![f](https://www.zhihu.com/equation?tex=f) 关于 ![\alpha](https://www.zhihu.com/equation?tex=%5Calpha) 的上积分和下积分. 若上下积分相等, 则称 ![f](https://www.zhihu.com/equation?tex=f) 关于 ![\alpha](https://www.zhihu.com/equation?tex=%5Calpha) 是**Reimann-Stieltjes可积**的,记为 ![f\in \mathscr{R}(\alpha)](https://www.zhihu.com/equation?tex=f%5Cin+%5Cmathscr%7BR%7D%28%5Calpha%29) . 可简称**R-S可积**.

显然,当我们取 ![\alpha(x)=x](https://www.zhihu.com/equation?tex=%5Calpha%28x%29%3Dx) 时,以上定义,就是黎曼积分的定义. 同时,关于Darboux 上和与下和有类似的讨论:
- 对于任意分割 ![\tau](https://www.zhihu.com/equation?tex=%5Ctau) , 恒有 ![L(\tau, f,\alpha)\leqslant U(\tau, f,\alpha)](https://www.zhihu.com/equation?tex=L%28%5Ctau%2C+f%2C%5Calpha%29%5Cleqslant+U%28%5Ctau%2C+f%2C%5Calpha%29) ;
- 若有 ![\tau'\geqslant\tau](https://www.zhihu.com/equation?tex=%5Ctau%27%5Cgeqslant%5Ctau) , 则上和不增, 下和不减.
- 对于任意两个分割 ![\tau_1,\tau_2](https://www.zhihu.com/equation?tex=%5Ctau_1%2C%5Ctau_2) , 恒有 ![L(\tau_1, f,\alpha)\leqslant U(\tau_2, f,\alpha)](https://www.zhihu.com/equation?tex=L%28%5Ctau_1%2C+f%2C%5Calpha%29%5Cleqslant+U%28%5Ctau_2%2C+f%2C%5Calpha%29) ;
- ![\sup{L(\tau, f,\alpha)}\leqslant\inf{U(\tau, f,\alpha)}](https://www.zhihu.com/equation?tex=%5Csup%7BL%28%5Ctau%2C+f%2C%5Calpha%29%7D%5Cleqslant%5Cinf%7BU%28%5Ctau%2C+f%2C%5Calpha%29%7D) ,即下积分不超过上积分.

有了积分的定义, 现在**考虑判定一个函数是否可积的问题**. 对此,我们有:

> 在 ![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D) 上,  ![f\in\mathscr{R}(\alpha)](https://www.zhihu.com/equation?tex=f%5Cin%5Cmathscr%7BR%7D%28%5Calpha%29) 当且仅当: ![\forall \varepsilon >0, \exists \tau_\varepsilon](https://www.zhihu.com/equation?tex=%5Cforall+%5Cvarepsilon+%3E0%2C+%5Cexists+%5Ctau_%5Cvarepsilon) , 使得 ![U(\tau, f,\alpha)-L(\tau, f,\alpha)<\varepsilon.](https://www.zhihu.com/equation?tex=U%28%5Ctau%2C+f%2C%5Calpha%29-L%28%5Ctau%2C+f%2C%5Calpha%29%3C%5Cvarepsilon.)

应用这一定理, 可以得到一些可积函数类.
- 若 ![\alpha](https://www.zhihu.com/equation?tex=%5Calpha) 为 ![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D) 上的单调递增函数,  ![f](https://www.zhihu.com/equation?tex=f) 在 ![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D) 上连续,则 ![f\in\mathscr{R}(\alpha)](https://www.zhihu.com/equation?tex=f%5Cin%5Cmathscr%7BR%7D%28%5Calpha%29) .
- 若 ![\alpha](https://www.zhihu.com/equation?tex=%5Calpha) 为 ![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D) 上的单调递增连续函数, ![f](https://www.zhihu.com/equation?tex=f) 在 ![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D) 上单调,则 ![f\in\mathscr{R}(\alpha)](https://www.zhihu.com/equation?tex=f%5Cin%5Cmathscr%7BR%7D%28%5Calpha%29) .
- 若![f](https://www.zhihu.com/equation?tex=f) 在 ![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D) 上有界, 且仅有有限个间断点,而$\alpha$在$f$的 每个间断点连续,则 ![f\in\mathscr{R}(\alpha)](https://www.zhihu.com/equation?tex=f%5Cin%5Cmathscr%7BR%7D%28%5Calpha%29) .

下一篇预告: 分析之[R-S积分] Part(2)/ 关键词: 有界变差函数, 不定积分, Picard-Lindelof 定理

