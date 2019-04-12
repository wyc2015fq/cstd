[
    ](https://csdnimg.cn/public/favicon.ico)漫步最优化四十五——矩阵S的生成 - 蜗牛 - CSDN博客[
        
                    ](https://csdnimg.cn/public/favicon.ico)[
            
        
          ](https://csdnimg.cn/release/phoenix/template/css/detail-7b6c1cb5d6.min.css)[
        ](https://csdnimg.cn/release/phoenix/themes/skin3-template/skin3-template-9b39979775.min.css)[
    ](https://csdnimg.cn/release/phoenix/themes/skin3-template/skin3-template-9b39979775.min.css)[
    ](https://csdnimg.cn/release/phoenix/themes/skin3-template/skin3-template-9b39979775.min.css)[
    ](https://csdnimg.cn/release/phoenix/themes/skin3-template/skin3-template-9b39979775.min.css)[
    ](https://csdnimg.cn/release/phoenix/themes/skin3-template/skin3-template-9b39979775.min.css)[
    ](https://csdnimg.cn/release/phoenix/themes/skin3-template/skin3-template-9b39979775.min.css)[
          ](https://csdnimg.cn/release/phoenix/themes/skin3-template/skin3-template-9b39979775.min.css)[
        ](https://csdnimg.cn/release/phoenix/template/css/detail-7b6c1cb5d6.min.css)[
    ](https://csdnimg.cn/release/phoenix/template/css/detail-7b6c1cb5d6.min.css)[
    ](https://csdnimg.cn/public/sandalstrap/1.4/css/sandalstrap.min.css).MathJax, .MathJax_Message, .MathJax_Preview{
            display: none
        }[
](https://csdnimg.cn/public/sandalstrap/1.4/css/sandalstrap.min.css)[
](https://csdnimg.cn/release/phoenix/template/css/detail-7b6c1cb5d6.min.css)[
](https://csdnimg.cn/release/phoenix/template/css/detail-7b6c1cb5d6.min.css)[
    ](https://csdnimg.cn/public/common/toolbar/content_toolbar_css/content_toolbar.css)[
](https://csdnimg.cn/public/common/toolbar/content_toolbar_css/content_toolbar.css)
[
](https://csdnimg.cn/release/phoenix/template/css/blog_code-c3a0c33d5c.css)[
](https://csdnimg.cn/release/phoenix/vendor/pagination/paging.css)[
](https://csdnimg.cn/release/phoenix/vendor/pagination/paging.css)[
](https://csdnimg.cn/release/phoenix/vendor/pagination/paging.css)[
](https://csdnimg.cn/release/phoenix/vendor/pagination/paging.css)![]()[
]()
译
# 漫步最优化四十五——矩阵S的生成
2017年11月23日 21:53:39[会敲键盘的猩猩](https://me.csdn.net/u010182633)阅读数：714



$\textbf{想赖着你每一天，}$
$\textbf{耽误多一秒都不愿意。}$
$\textbf{想把你捧在手心，}$
$\textbf{给你我最奢侈的温柔。}$
$\textbf{感恩节快乐，}$
$\textbf{感谢有你。}$
$\textbf{——畅宝宝的傻逼哥哥}$
令$f(\mathbf{x})\in C^2$是$E^n$中的函数并假设$f(\mathbf{x})$在点$\mathbf{x}_k,\mathbf{x}_{k+1}$处的梯度分别为$\mathbf{g}_k,\mathbf{g}_{k+1}$，如果

$$
\begin{equation}
\mathbf{x}_{k+1}=\mathbf{x}_k+\mathbf{\delta_k}
\end{equation}
$$
那么根据泰勒级数可以得出$\mathbf{g}_{k+1}$的元素为

$$
g_{(k+1)m}=g_{km}+\sum_{i=1}^n\frac{\partial g_{km}}{\partial x_{ki}}\delta_{ki}+\frac{1}{2}\sum_{i=1}^n\sum_{j=1}^n\frac{\partial^2g_{km}}{\partial x_{ki}\partial x_{kj}}\delta_{ki}\delta_{kj}+\cdots
$$
其中$m=1,2,\ldots,n$。接下来如果$f(\mathbf{x})$是二次的，那么$f(\mathbf{x})$的二阶导为常数，$\mathbf{g}_{km}$的二阶导为令，所以

$$
g_{(k+1)m}=g_{km}+\sum_{i=1}^n\frac{\partial g_{km}}{\partial x_{ki}}\delta_{ki}
$$
又因为

$$
g_{km}=\frac{\partial f_k}{\partial x_{km}}
$$
因此我们有

$$
g_{(k+1)m}=g_{km}+\sum_{i=1}^n\frac{\partial^2f_k}{\partial x_{ki}\partial x_{km}}\delta_{ki}
$$
其中$m=1,2,\ldots,n$，也就是

$$
\mathbf{g}_{k+1}=\mathbf{g}_k+\mathbf{H}\mathbf{\delta}_k
$$
其中$\mathbf{H}$是$f(\mathbf{x})$的海森矩阵。或者我们可以写成

$$
\begin{equation}
\mathbf{\gamma}_k=\mathbf{H}\mathbf{\delta}_k
\end{equation}
$$
其中

$$
\begin{align}
\mathbf{\delta}_k=\mathbf{x}_{k+1}-\mathbf{x}_k\\
\mathbf{\gamma}_k=\mathbf{g}_{k+1}-\mathbf{g}_k
\end{align}
$$
上面的分析表明，如果$f(\mathbf{x})$在点$\mathbf{x}_k,\mathbf{x}_{k+1}$处的梯度已知，那么可以得到上面的关系，这给出了矩阵$\mathbf{H}$的部分信息。因为$\mathbf{H}$中有$n^2$个未知量(或者如果$\mathbf{H}$是实对称矩阵，那么存在$n(n+1)/2$个未知量)，而等式2只提供了$n$个等式，不能唯一的确定$\mathbf{H}$，然而，如果对连续$n+1$个点的梯度进行估计，也就是点$\mathbf{x}_0,\mathbf{x}_1,\ldots,\mathbf{x}_n$，使得$\mathbf{x}$的变化量组成一个线性无关向量的集合，那么我们就有充分的信息来唯一的确定$\mathbf{H}$，其中$\mathbf{x}$的变化量就是

$$
\begin{align*}
\mathbf{\delta}_0=\mathbf{x}_1-\mathbf{x}_0\\
\mathbf{\delta}_1=\mathbf{x}_2-\mathbf{x}_1\\
\vdots\\
\mathbf{\delta}_{n-1}=\mathbf{x}_n-\mathbf{x}_{n-1}
\end{align*}
$$
为了证明这个事实，我们将等式2给出的$n$个等式重新写成

$$
\begin{equation}
[\mathbf{\gamma}_0\ \mathbf{\gamma}_1\ \cdots\ \mathbf{\gamma}_{n-1}]=\mathbf{H}[\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}]
\end{equation}
$$
因此

$$
\mathbf{H}=[\mathbf{\gamma}_0\ \mathbf{\gamma}_1\ \cdots\ \mathbf{\gamma}_{n-1}][\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}]^{-1}
$$
如果$\mathbf{\delta}_0,\mathbf{\delta}_1,\ldots,\mathbf{\delta}_{n-1}$构成一个线性无关向量集，那么解就存在。
利用上面的原则可以构造下面的算法：

$$
\begin{align*}
&\textbf{算法1：替代的牛顿法}\\
&\textbf{步骤1}\\
&\text{输入}\mathbf{x}_{00}\text{并初始化容忍误差}\varepsilon\\
&\text{令}k=0\\
&\text{输入一个线性关系向量集合}\mathbf{\delta}_0,\mathbf{\delta}_1,\ldots,\mathbf{\delta}_{n-1}\\
&\textbf{步骤2}\\
&\text{计算}\mathbf{g}_{00}\\
&\textbf{步骤3}\\
&\text{对于}i=0\text{到}n-1\\
&\quad\text{令}\mathbf{x}_{k(i+1)}=\mathbf{x}_{ki}+\mathbf{\delta}_i\\
&\quad\text{计算}\mathbf{g}_{k(i+1)}\\
&\quad\text{令}\mathbf{\gamma}_{ki}=\mathbf{g}_{k(i+1)}-\mathbf{g}_{ki}\\
&\textbf{步骤4}\\
&\text{计算}\mathbf{H}_k=[\mathbf{\gamma}_{k0}\ \mathbf{\gamma}_{k1}\ \cdots\ \mathbf{\gamma}_{k(n-1)}][\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}]^{-1}\\
&\text{计算}\mathbf{S}_k=\mathbf{H}_k^{-1}\\
&\textbf{步骤5}\\
&\text{令}\mathbf{d}_k=-\mathbf{S}_k\mathbf{g}_{k0}\\
&\text{找出}\alpha,\text{也即是最小化}f(\mathbf{x}_{k0}+\alpha\mathbf{d}_k)\text{的}\alpha\text{值}\\
&\text{令}\mathbf{x}_{(k+1)0}=\mathbf{x}_{k0}+\alpha_k\mathbf{d}_k\\
&\textbf{步骤6}\\
&\text{如果}\lVert\alpha_k\mathbf{d}_k\rVert<\varepsilon,\text{输出}\mathbf{x}_k^*=\mathbf{x}_{(k+1)0}\text{并且}f(\mathbf{x}^*)=f(\mathbf{x}_{(k+1)0})\text{算法结束}\\
&\textbf{步骤7}\\
&\text{令}k=k+1\text{然后回到步骤3}
\end{align*}
$$
上面的算法基本上是牛顿法的实现，唯一的不同是利用数据来得到$\mathbf{H}^{-1}$。对于凸二次问题，上面的算法一次迭代就得的问题的解，因此非常有效，然而对于非二次问题，同基于牛顿法的其他一样，它也存在相同的缺点。首先需要矩阵的逆，其次必须检查$\mathbf{H}_k$的正定性。
下面介绍一种消除矩阵逆的策略。假设已知正定实对称矩阵$\mathbf{S}_k$，$\mathbf{H}^{-1}$的近似值，然后计算拟牛顿方向为

$$
\begin{equation}
\mathbf{d}_k=-\mathbf{S}_k\mathbf{g}_k
\end{equation}
$$
然后求$\alpha$，也就是最小化$f(\mathbf{x}_k+\alpha\mathbf{d}_k)$的$\alpha$值。对于凸二次问题，

$$
\begin{equation}
\alpha_k=\frac{\mathbf{g}_k^T\mathbf{S}_k\mathbf{g}_k}{(\mathbf{S}_k\mathbf{g}_k)^T\mathbf{H}(\mathbf{S}_k\mathbf{g}_k)}
\end{equation}
$$
其中$\mathbf{S}_k,\mathbf{H}$是正定的。显然，当$\mathbf{x}_k$不是问题的解$\mathbf{x}^*$时，$\alpha_k$大于零，然后确定$\mathbf{x}$的变化量为

$$
\begin{equation}
\mathbf{\delta}_k=\alpha_k\mathbf{d}_k
\end{equation}
$$
根据等式1得到新点$\mathbf{x}_{k+1}$。通过计算$\mathbf{x}_k,\mathbf{x}_{k+1}$处的梯度，利用等式4得到梯度的变化量$\mathbf{\gamma}_k$，接下里对$\mathbf{S}_k$进行修正得到

$$
\begin{equation}
\mathbf{S}_{k+1}=\mathbf{S}_k+\mathbf{C}_k
\end{equation}
$$
其中$\mathbf{C}_k$是$n\times n$修正矩阵，从数据计算得出的。从初始点$\mathbf{x}_0$,初始正定矩阵$\mathbf{S}_0,\mathbf{S}_0=\mathbf{I}_n$开始，迭代使用上面的过程，得到序列$\mathbf{\delta}_0,\mathbf{\delta}_1,\ldots,\mathbf{\delta}_k,\mathbf{\gamma}_0,\mathbf{\gamma}_1,\ldots,\mathbf{\gamma}_k,\mathbf{S}_1,\mathbf{S}_2,\ldots,\mathbf{S}_{k+1}$。如果

$$
\begin{equation}
\mathbf{S}_{k+1}\mathbf{\gamma}_i=\mathbf{\delta}_i\quad\text{for}\ 0\leq i\leq k
\end{equation}
$$
然后对于$k=n-1$，我们可以写作

$$
\mathbf{S}_n[\mathbf{\gamma}_0\ \mathbf{\gamma}_1\ \cdots\ \mathbf{\gamma}_{n-1}]=[\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}]
$$
或者

$$
\begin{equation}
\mathbf{S}_n=[\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}][\mathbf{\gamma}_0\ \mathbf{\gamma}_1\ \cdots\ \mathbf{\gamma}_{n-1}]^{-1}
\end{equation}
$$
根据等式5与11可得

$$
\mathbf{S}_n=\mathbf{H}^{-1}
$$
接下来如果$k=n$，等式6到8得到

$$
\begin{align*}
\mathbf{d}_n=-\mathbf{H}^{-1}\mathbf{g}_n\\
\alpha_n=1\\
\mathbf{\delta}_n=-\mathbf{H}^{-1}\mathbf{g}_n
\end{align*}
$$
所以根据等式1可得

$$
\mathbf{x}_{n+1}=\mathbf{x}_n-\mathbf{H}^{-1}\mathbf{g}_n=\mathbf{x}^*
$$
上面的过程产生了一族拟牛顿算法，他们有一个基本属性：对于凸二次问题，$n+1$次迭代内中止，根据修正矩阵$\mathbf{C}_n$的差异从而得出各种各样的算法。
对于任何$\mathbf{C}_n$,$\mathbf{S}_{k+1}$必须满足等式10，并且下面的属性也是我们想要的：
- 向量$\mathbf{\delta}_0,\mathbf{\delta}_1,\ldots,\mathbf{\delta}_{n-1}$- 构成一个共轭方向集- 正定矩阵$\mathbf{S}_k$- 产生正定矩阵$\mathbf{S}_{k+1}$第一个属性确保共轭方向的性质能够应用到拟牛顿法上，第二个属性确保每次迭代中$\mathbf{d}_k$是下降方向。为了证明这个事实，考虑点$\mathbf{x}_k+\mathbf{\delta}_k$并令

$$
\mathbf{\delta}_k=\alpha\mathbf{d}_k
$$
其中

$$
\mathbf{d}_k=-\mathbf{S}_k\mathbf{g}_k
$$
对于$\alpha>0$，泰勒级数给出

$$
f(\mathbf{x}_k+\mathbf{\delta}_k)=f(\mathbf{x}_k)+\mathbf{g}_k^T\mathbf{\delta}_k+\frac{1}{2}\mathbf{\delta}_k^T\mathbf{H}(\mathbf{x}_k+c\mathbf{\delta}_k)\mathbf{\delta}_k
$$
其中$c$是常数，$0\leq c\leq 1$。消除$\mathbf{\delta}_k$，我们得到

$$
\begin{align*}
f(\mathbf{x}_k+\mathbf{\delta}_k)&=f(\mathbf{x}_k)-\alpha\mathbf{g}_k^T\mathbf{S}_k\mathbf{g}_k+o(\alpha\lVert\mathbf{d}_k\rVert)\\
&=f(\mathbf{x}_k)-[\alpha\mathbf{g}_k^T\mathbf{S}_k\mathbf{g}_k-o(\alpha\lVert\mathbf{d}_k\rVert)]
\end{align*}
$$
其中$o(\alpha\lVert\mathbf{d}_k\rVert)$是余项，比$\alpha\lVert\mathbf{d}_k\rVert$更快接近零。接下来如果$\mathbf{S}_k$是正定的，那么对于充分小的$\alpha>0$，我们有

$$
\alpha\mathbf{g}_k\mathbf{S}_k\mathbf{g}_k-o(\alpha\lVert\mathbf{d}_k\rVert)>0
$$
因为$\alpha>0,\mathbf{g}_k^T\mathbf{S}_k\mathbf{g}_k>0,o(\alpha\lVert\mathbf{d}_k\rVert)\to 0$，因此

$$
\begin{equation}
f(\mathbf{x}_k+\mathbf{\delta}_k)<f(\mathbf{x}_k)
\end{equation}
$$
也就是说如果$\mathbf{S}_k$是正定的，那么$\mathbf{d}_k$是下降方向。
从这点来说，属性2的重要性就很明显了。正定矩阵$\mathbf{S}_0$将产生正定矩阵$\mathbf{S}_1$，然后产生正定矩阵$\mathbf{S}_2$，如此不断进行，最后方向$\mathbf{d}_0,\mathbf{d}_1,\mathbf{d}_2,\ldots$将都是下降方向，这就确保算法能够收敛。
[
                      ](https://csdnimg.cn/release/phoenix/mdeditor/markdown_views-258a4616f7.css)

阅读更多收藏分享



[				](//me.csdn.net/u014528142)![](https://avatar.csdn.net/C/B/2/3_u014528142.jpg)[
			](https://avatar.csdn.net/C/B/2/3_u014528142.jpg)[		](//me.csdn.net/u014528142)
[发表评论](#)
添加代码片
HTML/XMLobjective-cRubyPHPCC++JavaScriptPythonJavaCSSSQL其它
还能输入*1000*个字符



[				](https://blog.csdn.net/u010182633/article/details/74268085)*漫步**最优化*
#### 五——可行域
[				](https://blog.csdn.net/u010182633/article/details/74268085)07-03阅读数 							605
[				](https://blog.csdn.net/u010182633/article/details/74268085)[							](https://blog.csdn.net/u010182633/article/details/74268085)我不介意你慢慢的到来，\textbf{我不介意你慢慢的到来，}我也不介意我们多少次擦肩而过，\textbf{我也不介意我们多少次擦肩而过，}因为我一直相信我们都在相遇的路上马不停蹄。\textbf{因...[						](https://blog.csdn.net/u010182633/article/details/74268085)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/owen7500/article/details/51601627)
#### 机器学习中常见的几种*最优化*
#### 方法
[				](https://blog.csdn.net/owen7500/article/details/51601627)06-07阅读数 							2万+
[				](https://blog.csdn.net/owen7500/article/details/51601627)[							](https://blog.csdn.net/owen7500/article/details/51601627)1.梯度下降法（GradientDescent）2.牛顿法和拟牛顿法（Newton'smethod& Quasi-NewtonMethods）3.共轭梯度法（ConjugateGradient）4.启...[						](https://blog.csdn.net/owen7500/article/details/51601627)博文[来自：	](https://blog.csdn.net/owen7500)云鹤起舞的博客

[				](https://blog.csdn.net/xbmatrix/article/details/56682466)
#### 常见的几种*最优化*
#### 方法
[				](https://blog.csdn.net/xbmatrix/article/details/56682466)02-23阅读数 							1万+
[				](https://blog.csdn.net/xbmatrix/article/details/56682466)[							](https://blog.csdn.net/xbmatrix/article/details/56682466)参考：http://blog.csdn.net/majinlei121/article/details/47260917http://mp.weixin.qq.com/s?__biz=MzI5NzA3...[						](https://blog.csdn.net/xbmatrix/article/details/56682466)博文[来自：	](https://blog.csdn.net/xbmatrix)xbmatrix的博客


[				](https://blog.csdn.net/philosophyatmath/article/details/70684956)
#### 拟牛顿法之DFP算法
[				](https://blog.csdn.net/philosophyatmath/article/details/70684956)04-25阅读数 							735
[				](https://blog.csdn.net/philosophyatmath/article/details/70684956)[							](https://blog.csdn.net/philosophyatmath/article/details/70684956)DFP算法(Davidon-Fletcher-Powellalgorithm)一种秩2拟牛顿法.由戴维登(Davidon,W.D.)于1959年导出，并由弗莱彻(Fletcher,R.)和鲍威尔(Po...[						](https://blog.csdn.net/philosophyatmath/article/details/70684956)博文[来自：	](https://blog.csdn.net/philosophyatmath)张大鹏(朋)的专栏

[				](https://blog.csdn.net/qq_41679006/article/details/81167827)*最优化*
#### 方法：五、无约束*最优化*
#### 方法
[				](https://blog.csdn.net/qq_41679006/article/details/81167827)07-24阅读数 							779
[				](https://blog.csdn.net/qq_41679006/article/details/81167827)[							](https://blog.csdn.net/qq_41679006/article/details/81167827)主要参考书目：-最优化方法及其应用/郭科，陈聆，魏友华.-北京：高等教育出版社,2007.7(2013.7重印)在第四章中，我们解决了确定搜索步长的问题，现在解决确定搜索方向的问题。1、最速下降法基本...[						](https://blog.csdn.net/qq_41679006/article/details/81167827)博文[来自：	](https://blog.csdn.net/qq_41679006)寂风如雪

[				](https://blog.csdn.net/pupilxmk/article/details/80735599)
#### 梯度下降法和(拟)牛顿法区别及介绍
[				](https://blog.csdn.net/pupilxmk/article/details/80735599)06-19阅读数 							1875
[				](https://blog.csdn.net/pupilxmk/article/details/80735599)[							](https://blog.csdn.net/pupilxmk/article/details/80735599)梯度下降法和牛顿法区别梯度下降法和牛顿法都是求解无约束最优化问题的常用方法。假设f(x)f(x)f(x)为RnRn\mathbf{R}^n上具有一阶连续偏导数的函数，要求姐的无约束最优化问题为minx...[						](https://blog.csdn.net/pupilxmk/article/details/80735599)博文[来自：	](https://blog.csdn.net/pupilxmk)pupilxmk的博客

[				](https://blog.csdn.net/philthinker/article/details/78191864)
#### 无约束*最优化*
#### 问题的一般结构与规划方法
[				](https://blog.csdn.net/philthinker/article/details/78191864)10-10阅读数 							2322
[				](https://blog.csdn.net/philthinker/article/details/78191864)[							](https://blog.csdn.net/philthinker/article/details/78191864)无约束问题与最优解最优性条件一维线性搜索精确线性搜索直接搜索法非精确一维搜索法下降算法的收敛性与收敛速度无约束规划最速下降法Newton法Newton-最速下降混合算法阻尼Newton法拟Newton...[						](https://blog.csdn.net/philthinker/article/details/78191864)博文[来自：	](https://blog.csdn.net/philthinker)止于至玄

[				](https://blog.csdn.net/philosophyatmath/article/details/70173128)
#### 拟牛顿法之BFGS算法
[				](https://blog.csdn.net/philosophyatmath/article/details/70173128)04-21阅读数 							3717
[				](https://blog.csdn.net/philosophyatmath/article/details/70173128)[							](https://blog.csdn.net/philosophyatmath/article/details/70173128)什么是拟牛顿法？拟牛顿法是在牛顿法的基础上引入了Hessian矩阵的近似矩阵，避免每次迭代都计算Hessian矩阵的逆，它的收敛速度介于梯度下降法和牛顿法之间。拟牛顿法跟牛顿法一样，也是不能处理太大规...[						](https://blog.csdn.net/philosophyatmath/article/details/70173128)博文[来自：	](https://blog.csdn.net/philosophyatmath)张大鹏(朋)的专栏

[
  ](https://www.csdn.net/company/css/edu-recommend-1.0.1.css)[          ](https://edu.csdn.net/course/detail/5814?utm_source=baidutj)
#### Python从入门到实战 基础入门视频教程（讲解超细致）
[        ](https://edu.csdn.net/course/detail/5814?utm_source=baidutj)[            ](https://edu.csdn.net/course/detail/5814?utm_source=baidutj)Python基础入门视频教程：本课程从Python入门到纯Python项目实战。超100以上课时，内容非常详细，覆盖了Python知识的方方面面，学完后不仅对Python知识有个系统化的了解，更能利用Python做出类似于“密码加密存储管理系统”、“2048游戏”这种实际项目。让你从Python小白编程大牛！[          ](https://edu.csdn.net/course/detail/5814?utm_source=baidutj)学院讲师：黄勇


[				](https://blog.csdn.net/u010182633/article/details/74593858)*漫步**最优化*
#### 八——梯度信息
[				](https://blog.csdn.net/u010182633/article/details/74593858)07-06阅读数 							469
[				](https://blog.csdn.net/u010182633/article/details/74593858)[							](https://blog.csdn.net/u010182633/article/details/74593858)在许多优化方法中，需要目标函数的梯度信息，这个信息由f(x)f(\textbf{x})对nn个变量的一阶与二阶导组成的。如果f(x)∈C1f(\textbf{x})\inC^1，即f(x)f(\tex...[						](https://blog.csdn.net/u010182633/article/details/74593858)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/73928461)*漫步**最优化*
#### 一——引言
[				](https://blog.csdn.net/u010182633/article/details/73928461)06-29阅读数 							888
[				](https://blog.csdn.net/u010182633/article/details/73928461)[							](https://blog.csdn.net/u010182633/article/details/73928461)其实在过去，人们就一直在参与优化过程。最早的优化形式包括不科学的仪式和偏见，例如用动物祭祀神仙，观察星星的位置，鸟的飞行，有些时间段被当成种植作物或开始战争的吉日。随着时代的进步与理性的流行，不合逻辑...[						](https://blog.csdn.net/u010182633/article/details/73928461)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛



[				](https://blog.csdn.net/u010182633/article/details/74853052)*漫步**最优化*
#### 十——极值类型
[				](https://blog.csdn.net/u010182633/article/details/74853052)07-08阅读数 							421
[				](https://blog.csdn.net/u010182633/article/details/74853052)[							](https://blog.csdn.net/u010182633/article/details/74853052)深夜里，你不断徘徊在我的心田，\textbf{深夜里，你不断徘徊在我的心田，}你的每一句誓言都在耳边回荡，\textbf{你的每一句誓言都在耳边回荡，}你闪动的双眼隐藏着你的羞涩。\textbf{你闪...[						](https://blog.csdn.net/u010182633/article/details/74853052)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/78304774)*漫步**最优化*
#### 二十七——二次插值法
[				](https://blog.csdn.net/u010182633/article/details/78304774)10-21阅读数 							652
[				](https://blog.csdn.net/u010182633/article/details/78304774)[							](https://blog.csdn.net/u010182633/article/details/78304774)陪着你，仿佛面朝阳光，\textbf{陪着你，仿佛面朝阳光，}不管走到哪里都是晴天。\textbf{不管走到哪里都是晴天。}在蝴蝶飞舞的百花丛中，\textbf{在蝴蝶飞舞的百花丛中，}一朵一朵的鲜花...[						](https://blog.csdn.net/u010182633/article/details/78304774)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/76554599)*漫步**最优化*
#### 二十一——全局收敛
[				](https://blog.csdn.net/u010182633/article/details/76554599)08-01阅读数 							959
[				](https://blog.csdn.net/u010182633/article/details/76554599)[							](https://blog.csdn.net/u010182633/article/details/76554599)平时我看起来很坚强，\textbf{平时我看起来很坚强，}可是遇到你，我会变成另一个模样。\textbf{可是遇到你，我会变成另一个模样。}我不想考虑太多，\textbf{我不想考虑太多，}因为追随内...[						](https://blog.csdn.net/u010182633/article/details/76554599)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛


[				](https://blog.csdn.net/weixin_38403702/article/details/80478950)
#### 使用Python来*生成*
#### 随机*漫步*
#### 数据，并图文并茂展示
[				](https://blog.csdn.net/weixin_38403702/article/details/80478950)05-28阅读数 							306
[				](https://blog.csdn.net/weixin_38403702/article/details/80478950)[							](https://blog.csdn.net/weixin_38403702/article/details/80478950)随机漫步是这样行走得到的路径：每次行走都完全是随机的，没有明确的方向，结果是由一系列随机决策决定的。你可以这样认为，随机漫步就是蚂蚁在晕头转向的情况下，每次都沿随机的方向前行所经过的路径。在自...[						](https://blog.csdn.net/weixin_38403702/article/details/80478950)博文[来自：	](https://blog.csdn.net/weixin_38403702)weixin_38403702的博客

[				](https://blog.csdn.net/u010182633/article/details/78146689)*漫步**最优化*
#### 二十三——一维优化
[				](https://blog.csdn.net/u010182633/article/details/78146689)10-01阅读数 							328
[				](https://blog.csdn.net/u010182633/article/details/78146689)[							](https://blog.csdn.net/u010182633/article/details/78146689)你的每一句话，回荡在耳边；\textbf{你的每一句话，回荡在耳边；}你闪动的双眼，徘徊在脑海。\textbf{你闪动的双眼，徘徊在脑海。}好像告诉你，\textbf{好像告诉你，}天天在想你。\te...[						](https://blog.csdn.net/u010182633/article/details/78146689)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/74012429)*漫步**最优化*
#### 二——基本优化问题
[				](https://blog.csdn.net/u010182633/article/details/74012429)06-30阅读数 							679
[				](https://blog.csdn.net/u010182633/article/details/74012429)[							](https://blog.csdn.net/u010182633/article/details/74012429)我们还有很长的时间要走，千万不要把哥哥甩在身后；\textbf{我们还有很长的时间要走，千万不要把哥哥甩在身后；}我和你要去闻闻新鲜的春天；\textbf{我和你要去闻闻新鲜的春天；}感受阳光洒满肩上...[						](https://blog.csdn.net/u010182633/article/details/74012429)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/74079977)*漫步**最优化*
#### 三——优化算法的一般结构
[				](https://blog.csdn.net/u010182633/article/details/74079977)07-01阅读数 							842
[				](https://blog.csdn.net/u010182633/article/details/74079977)[							](https://blog.csdn.net/u010182633/article/details/74079977)你的出现，让我的眼中，心中有了蔚蓝的天空；\textbf{你的出现，让我的眼中，心中有了蔚蓝的天空；}你的出现，让梦的城堡，殿堂留住美好的时光。\textbf{你的出现，让梦的城堡，殿堂留住美好的时光...[						](https://blog.csdn.net/u010182633/article/details/74079977)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/75330189)*漫步**最优化*
#### 十五——凸函数优化
[				](https://blog.csdn.net/u010182633/article/details/75330189)07-18阅读数 							738
[				](https://blog.csdn.net/u010182633/article/details/75330189)[							](https://blog.csdn.net/u010182633/article/details/75330189)你在穿山越岭的另一边，\textbf{你在穿山越岭的另一边，}而我也在没有尽头的孤独路上前行。\textbf{而我也在没有尽头的孤独路上前行。}试着体会错误，试着忍住眼泪，\textbf{试着体会错误...[						](https://blog.csdn.net/u010182633/article/details/75330189)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛


[				](https://blog.csdn.net/u012474716/article/details/80256530)
#### numpy 随机数random/随机*漫步*
#### 介绍
[				](https://blog.csdn.net/u012474716/article/details/80256530)05-09阅读数 							510
[				](https://blog.csdn.net/u012474716/article/details/80256530)[							](https://blog.csdn.net/u012474716/article/details/80256530)很多网络演化编程中都会用到生成随机数，今天找了一下随机数说明手册，整理一下：（1）random.seed([x])伪随机数生成模块。如果不提供seed，默认使用系统时间。使用相同的seed，可以获得完...[						](https://blog.csdn.net/u012474716/article/details/80256530)博文[来自：	](https://blog.csdn.net/u012474716)u012474716的博客

[				](https://blog.csdn.net/lanhaixuanvv/article/details/78427548)
#### 使用Python的matplotlib库*生成*
#### 随机*漫步*
#### 图
[				](https://blog.csdn.net/lanhaixuanvv/article/details/78427548)11-02阅读数 							1670
[				](https://blog.csdn.net/lanhaixuanvv/article/details/78427548)[							](https://blog.csdn.net/lanhaixuanvv/article/details/78427548)序言：本教程使用python来生成随机漫步数据，再使用matplotlib将数据呈现出来开发环境操作系统：Windows10IDE：Pycharm2017.1.3Python版本：Python3.6P...[						](https://blog.csdn.net/lanhaixuanvv/article/details/78427548)博文[来自：	](https://blog.csdn.net/lanhaixuanvv)lanhaixuanvv的博客

[				](https://blog.csdn.net/gongweijiao/article/details/39365997)
#### 优化工具 Neos Server
[				](https://blog.csdn.net/gongweijiao/article/details/39365997)09-18阅读数 							1473
[				](https://blog.csdn.net/gongweijiao/article/details/39365997)[							](https://blog.csdn.net/gongweijiao/article/details/39365997)之前花了三四天在windows上配置[						](https://blog.csdn.net/gongweijiao/article/details/39365997)博文[来自：	](https://blog.csdn.net/gongweijiao)辙忆

[				](https://blog.csdn.net/we1583004we/article/details/77197634)
#### python 数据可视化 matplotlib学习三：*生成*
#### 随机*漫步*
#### 数据并使用matplotlib呈现
[				](https://blog.csdn.net/we1583004we/article/details/77197634)08-15阅读数 							1335
[				](https://blog.csdn.net/we1583004we/article/details/77197634)[							](https://blog.csdn.net/we1583004we/article/details/77197634)使用Python生成随机漫步数据，再使用matplotlib将数据呈现出来      随机漫步是这样行走得到的路径：每次行走都完全是随机的，没有明确的方向，结果是由一系列随机决策决定的。自然界中的例子...[						](https://blog.csdn.net/we1583004we/article/details/77197634)博文[来自：	](https://blog.csdn.net/we1583004we)小明的专栏

[				](https://blog.csdn.net/asialee_bird/article/details/79586347)
#### Python3——随机*漫步**生成*
#### 数据并绘制
[				](https://blog.csdn.net/asialee_bird/article/details/79586347)03-16阅读数 							1460
[				](https://blog.csdn.net/asialee_bird/article/details/79586347)[							](https://blog.csdn.net/asialee_bird/article/details/79586347)Python随机漫步生成数据并绘制random_walk.pyfromrandomimportchoice#生成随机漫步的数据类classRandomWalk():def__init__(self,n...[						](https://blog.csdn.net/asialee_bird/article/details/79586347)博文[来自：	](https://blog.csdn.net/asialee_bird)Asia-Lee的博客


[				](https://blog.csdn.net/momingqimiao71/article/details/82259209)
#### Python随机*漫步*
[				](https://blog.csdn.net/momingqimiao71/article/details/82259209)08-31阅读数 							191
[				](https://blog.csdn.net/momingqimiao71/article/details/82259209)[							](https://blog.csdn.net/momingqimiao71/article/details/82259209)#时间漫步importnumpyasnpfromnumpyimportlinalgimportrandomposition=0walk=[position]steps=1000foriinrange(...[						](https://blog.csdn.net/momingqimiao71/article/details/82259209)博文[来自：	](https://blog.csdn.net/momingqimiao71)OliverQueen的博客

[				](https://blog.csdn.net/Sophia_11/article/details/84800435)
#### matplotlib随机*漫步*
#### 详解-绘制随机*生成*
#### 的点（附代码）
[				](https://blog.csdn.net/Sophia_11/article/details/84800435)12-05阅读数 							137
[				](https://blog.csdn.net/Sophia_11/article/details/84800435)[							](https://blog.csdn.net/Sophia_11/article/details/84800435)目录1、绘制随机漫步2、模拟多次随机漫步3、给点着色 4、重新绘制起点和终点5、隐藏坐标轴随机漫步是指每次行走都完全是随机的，没有明确的方向，结果是由一系列随机决策决定的。为模拟随机漫步，我们将创建一...[						](https://blog.csdn.net/Sophia_11/article/details/84800435)博文[来自：	](https://blog.csdn.net/Sophia_11)Sophia_11的博客

[				](https://blog.csdn.net/tina_ttl/article/details/51202566)*最优化*
#### 理论·光滑函数·Hessian*矩阵*
#### ·Jacobian*矩阵*
#### ·方向导数
[				](https://blog.csdn.net/tina_ttl/article/details/51202566)04-20阅读数 							4321
[				](https://blog.csdn.net/tina_ttl/article/details/51202566)[							](https://blog.csdn.net/tina_ttl/article/details/51202566)Jacobian是向量函数f(x)f(\mathbf{x})相对于向量x\mathbf{x}的偏导数以一定方式排列成的矩阵1.向量函数:f(x)f(\mathbf{x})f(x)=⎡⎣⎢⎢⎢⎢⎢⎢f1...[						](https://blog.csdn.net/tina_ttl/article/details/51202566)博文[来自：	](https://blog.csdn.net/tina_ttl)tina的博客

[				](https://blog.csdn.net/fireguard/article/details/16111603)
#### 随机*漫步*
#### 模型及其实现
[				](https://blog.csdn.net/fireguard/article/details/16111603)11-14阅读数 							3544
[				](https://blog.csdn.net/fireguard/article/details/16111603)[							](https://blog.csdn.net/fireguard/article/details/16111603)随机漫步（RandomWalk）思想最早由KarlPearson在1905年提出，它是一种不规则的变动形式，在变动过程当中的每一步都是随机的。通常来说，随机漫步被假定为具有马尔可夫链的性质，也即是每一...[						](https://blog.csdn.net/fireguard/article/details/16111603)博文[来自：	](https://blog.csdn.net/fireguard)fireguard的专栏

[				](https://blog.csdn.net/u013044116/article/details/80457648)*矩阵*
#### 优化问题总结
[				](https://blog.csdn.net/u013044116/article/details/80457648)05-25阅读数 							658
[				](https://blog.csdn.net/u013044116/article/details/80457648)[							](https://blog.csdn.net/u013044116/article/details/80457648)1.$\min\limits_{S}\|S-T\|_{F}^{2}$[						](https://blog.csdn.net/u013044116/article/details/80457648)博文[来自：	](https://blog.csdn.net/u013044116)Every day


[				](https://blog.csdn.net/DayBreak__/article/details/79836887)
#### Python3实现简单的随机*漫步*
#### 散点图
[				](https://blog.csdn.net/DayBreak__/article/details/79836887)04-06阅读数 							418
[				](https://blog.csdn.net/DayBreak__/article/details/79836887)[							](https://blog.csdn.net/DayBreak__/article/details/79836887)importmatplotlib.pyplotaspltfromrandomimportchoiceclassRandomStep():def__init__(self,num_steps=5000)...[						](https://blog.csdn.net/DayBreak__/article/details/79836887)博文[来自：	](https://blog.csdn.net/DayBreak__)DayBreak__的博客

[				](https://blog.csdn.net/qq_40123329/article/details/82015911)
#### python随机*漫步*
[				](https://blog.csdn.net/qq_40123329/article/details/82015911)08-24阅读数 							78
[				](https://blog.csdn.net/qq_40123329/article/details/82015911)[							](https://blog.csdn.net/qq_40123329/article/details/82015911)随机漫步：每次行走都完全是随机的，没有明确的方向，结果是由一系列随机决策决定的。1创建RandomWalk()类fromrandomimportchoiceclassRandomWalk():def_...[						](https://blog.csdn.net/qq_40123329/article/details/82015911)博文[来自：	](https://blog.csdn.net/qq_40123329)qq_40123329的博客

[				](https://blog.csdn.net/u010182633/article/details/78418692)*漫步**最优化*
#### 三十五——共轭
[				](https://blog.csdn.net/u010182633/article/details/78418692)11-01阅读数 							224
[				](https://blog.csdn.net/u010182633/article/details/78418692)[							](https://blog.csdn.net/u010182633/article/details/78418692)自从有了你，\textbf{自从有了你，}绝望与无奈远走高飞。\textbf{绝望与无奈远走高飞。}自从有了你，\textbf{自从有了你，}我的世界不再天灰灰。\textbf{我的世界不再天灰灰。}...[						](https://blog.csdn.net/u010182633/article/details/78418692)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/78272271)*漫步**最优化*
#### 二十四——二分搜索
[				](https://blog.csdn.net/u010182633/article/details/78272271)10-18阅读数 							177
[				](https://blog.csdn.net/u010182633/article/details/78272271)[							](https://blog.csdn.net/u010182633/article/details/78272271)考虑一个单峰函数，在区间[xL,xU][x_L,x_U]内有最小值，这个区间称为不确定范围，通过不断缩小这个不确定范围可以得出f(x)f(x)的最小值x∗x^*。在搜索方法中，使用f(x)f(x)在合...[						](https://blog.csdn.net/u010182633/article/details/78272271)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/SunshineDabby/article/details/6611420)
#### “随机*漫步*
#### ”(Random Walk)模拟演示
[				](https://blog.csdn.net/SunshineDabby/article/details/6611420)07-16阅读数 							1万+
[				](https://blog.csdn.net/SunshineDabby/article/details/6611420)[							](https://blog.csdn.net/SunshineDabby/article/details/6611420)(1)、任务描述有一类问题总称为“随机漫步”(RandomWalk)问题，这类问题长久以来吸引着数学界的兴趣。所有这些问题即使是最简单的解决起来也是极其困难的。而且它们在很大程度上还远没有得到解决。一...[						](https://blog.csdn.net/SunshineDabby/article/details/6611420)博文[来自：	](https://blog.csdn.net/SunshineDabby)Andrew Shaw|失的猛


[				](https://blog.csdn.net/u010182633/article/details/78439852)*漫步**最优化*
#### 三十七——共轭梯度法
[				](https://blog.csdn.net/u010182633/article/details/78439852)11-03阅读数 							370
[				](https://blog.csdn.net/u010182633/article/details/78439852)[							](https://blog.csdn.net/u010182633/article/details/78439852)我卸不下对你的喜欢，\textbf{我卸不下对你的喜欢，}因为爱会慢慢增加重量。\textbf{因为爱会慢慢增加重量。}我醉心于你的发香，\textbf{我醉心于你的发香，}因为它让回想有了画面感。\...[						](https://blog.csdn.net/u010182633/article/details/78439852)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/75090420)*漫步**最优化*
#### 十三——驻点
[				](https://blog.csdn.net/u010182633/article/details/75090420)07-13阅读数 							474
[				](https://blog.csdn.net/u010182633/article/details/75090420)[							](https://blog.csdn.net/u010182633/article/details/75090420)如果考虑的极值点类型(即极大值，极小值)位于可行域的内部，那么我们称为它们驻点，因为在这些点上g(x)=0\textbf{g}(\textbf{x})=\textbf{0}，还有一种驻点是鞍点。定义1...[						](https://blog.csdn.net/u010182633/article/details/75090420)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/52563660)*漫步*
#### 线性代数二十——快速傅里叶变换（下）
[				](https://blog.csdn.net/u010182633/article/details/52563660)09-17阅读数 							1672
[				](https://blog.csdn.net/u010182633/article/details/52563660)[							](https://blog.csdn.net/u010182633/article/details/52563660)快速傅里叶变换傅里叶分析是一个很美妙的理论，而且它还很实用。在频域分析波形可以很好的将信号分离开来，相反的过程又能回到时域中。处于物理和数学的缘故，指定实用指数函数，我们感觉最主要的原因就是：如果我们...[						](https://blog.csdn.net/u010182633/article/details/52563660)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/78298074)*漫步**最优化*
#### 二十六——黄金分割搜索
[				](https://blog.csdn.net/u010182633/article/details/78298074)10-20阅读数 							949
[				](https://blog.csdn.net/u010182633/article/details/78298074)[							](https://blog.csdn.net/u010182633/article/details/78298074)想要一颗赛艇，\textbf{想要一颗赛艇，}和你飞驰在海洋上。\textbf{和你飞驰在海洋上。}想要和你交织在一起，\textbf{想要和你交织在一起，}融化在宇宙银河里。\textbf{融化在宇...[						](https://blog.csdn.net/u010182633/article/details/78298074)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/CGCVHCI/article/details/51049080)
#### Hessian海森*矩阵*
#### 与牛顿*最优化*
#### 方法
[				](https://blog.csdn.net/CGCVHCI/article/details/51049080)04-03阅读数 							4140
[				](https://blog.csdn.net/CGCVHCI/article/details/51049080)[							](https://blog.csdn.net/CGCVHCI/article/details/51049080)Hessian矩阵在数学中,海森矩阵(Hessianmatrix或Hessian)是一个自变量为向量的实值函数的二阶偏导数组成的方块矩阵,此函数如下：f(x1,x2…,xn)f({x_1},{x_2}...[						](https://blog.csdn.net/CGCVHCI/article/details/51049080)博文[来自：	](https://blog.csdn.net/CGCVHCI)CGCVHCI的博客


[				](https://blog.csdn.net/codefight/article/details/79559809)
#### web-html5（一）
[				](https://blog.csdn.net/codefight/article/details/79559809)03-14阅读数 							872
[				](https://blog.csdn.net/codefight/article/details/79559809)[							](https://blog.csdn.net/codefight/article/details/79559809)【任务1-1】实现Q-WalkingE&amp;amp;S购物列表页面中的菜单导航栏。&amp;lt;!DOCTYPEhtml&amp;gt;&amp;lt;head&amp;gt;&amp;lt;m...[						](https://blog.csdn.net/codefight/article/details/79559809)博文[来自：	](https://blog.csdn.net/codefight)就是你，站住！

[				](https://blog.csdn.net/u010182633/article/details/74502512)*漫步**最优化*
#### 七——介绍
[				](https://blog.csdn.net/u010182633/article/details/74502512)07-05阅读数 							192
[				](https://blog.csdn.net/u010182633/article/details/74502512)[							](https://blog.csdn.net/u010182633/article/details/74502512)我的情意，言语从未表达出千万分之一；\textbf{我的情意，言语从未表达出千万分之一；}我的思绪，夜里想来想去始终无法入睡。\textbf{我的思绪，夜里想来想去始终无法入睡。}慢慢累积的记忆，让我...[						](https://blog.csdn.net/u010182633/article/details/74502512)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/75267242)*漫步**最优化*
#### 十四——凸函数与凹函数
[				](https://blog.csdn.net/u010182633/article/details/75267242)07-17阅读数 							7984
[				](https://blog.csdn.net/u010182633/article/details/75267242)[							](https://blog.csdn.net/u010182633/article/details/75267242)我一直都在流浪，\textbf{我一直都在流浪，}可我却不曾见过海洋。\textbf{可我却不曾见过海洋。} 我努力微笑坚强，\textbf{我努力微笑坚强，}用寂寞筑成一道围墙。\textbf{用寂...[						](https://blog.csdn.net/u010182633/article/details/75267242)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/invincibleFF/article/details/80399792)
#### python-随机*漫步*
#### 点的*生成*
[				](https://blog.csdn.net/invincibleFF/article/details/80399792)05-22阅读数 							257
[				](https://blog.csdn.net/invincibleFF/article/details/80399792)[							](https://blog.csdn.net/invincibleFF/article/details/80399792)何为随机漫步？就好比一只蚂蚁晕头转向所走出来的样子，因此很多的点都是随机的。在这里我们可以先梳理一下思路，从第一个点开始，第一个点我们设置为0，0第二个点就需要随机了，随机由两步生成，一个是方向，...[						](https://blog.csdn.net/invincibleFF/article/details/80399792)博文[来自：	](https://blog.csdn.net/invincibleFF)invincibleFF的博客

[				](https://blog.csdn.net/u010182633/article/details/78509249)*漫步**最优化*
#### 四十三——拟牛顿法
[				](https://blog.csdn.net/u010182633/article/details/78509249)11-11阅读数 							376
[				](https://blog.csdn.net/u010182633/article/details/78509249)[							](https://blog.csdn.net/u010182633/article/details/78509249)从相距千里，\textbf{从相距千里，}到心与心的碰撞，\textbf{到心与心的碰撞，}情感是一种随机，\textbf{情感是一种随机，}也是一种必然。\textbf{也是一种必然。}从一个人到两...[						](https://blog.csdn.net/u010182633/article/details/78509249)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛


[				](https://blog.csdn.net/u010182633/article/details/78359968)*漫步**最优化*
#### 三十二——最速下降法
[				](https://blog.csdn.net/u010182633/article/details/78359968)10-26阅读数 							1083
[				](https://blog.csdn.net/u010182633/article/details/78359968)[							](https://blog.csdn.net/u010182633/article/details/78359968)爱需要勇气，\textbf{爱需要勇气，}但是也许是天意，\textbf{但是也许是天意，}让我爱上你，\textbf{让我爱上你，}也许轮回里早已注定，\textbf{也许轮回里早已注定，}今生就该...[						](https://blog.csdn.net/u010182633/article/details/78359968)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/74998344)*漫步**最优化*
#### 十一——局部极小与极大的充分必要条件(上)
[				](https://blog.csdn.net/u010182633/article/details/74998344)07-11阅读数 							871
[				](https://blog.csdn.net/u010182633/article/details/74998344)[							](https://blog.csdn.net/u010182633/article/details/74998344)即便没有那么多浪漫的话，\textbf{即便没有那么多浪漫的话，}我也想与你走过每个冬夏。\textbf{我也想与你走过每个冬夏。}你的出现是我唯一的心动，\textbf{你的出现是我唯一的心动，}你...[						](https://blog.csdn.net/u010182633/article/details/74998344)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/78311581)*漫步**最优化*
#### 二十八——三次插值法
[				](https://blog.csdn.net/u010182633/article/details/78311581)10-22阅读数 							539
[				](https://blog.csdn.net/u010182633/article/details/78311581)[							](https://blog.csdn.net/u010182633/article/details/78311581)没有你的世界，\textbf{没有你的世界，}我会灵魂失控。\textbf{我会灵魂失控。}没有你的世界，\textbf{没有你的世界，}我被乌云拖着走。\textbf{我被乌云拖着走。}没有你的世界...[						](https://blog.csdn.net/u010182633/article/details/78311581)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/78369829)*漫步**最优化*
#### 三十三——牛顿法
[				](https://blog.csdn.net/u010182633/article/details/78369829)10-27阅读数 							234
[				](https://blog.csdn.net/u010182633/article/details/78369829)[							](https://blog.csdn.net/u010182633/article/details/78369829)自从有了你，\textbf{自从有了你，}绝望与无奈远走高飞。\textbf{绝望与无奈远走高飞。}自从有了你，\textbf{自从有了你，}我的世界不再天灰灰。\textbf{我的世界不再天灰灰。}...[						](https://blog.csdn.net/u010182633/article/details/78369829)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/u010182633/article/details/74157219)*漫步**最优化*
#### 四——约束
[				](https://blog.csdn.net/u010182633/article/details/74157219)07-02阅读数 							333
[				](https://blog.csdn.net/u010182633/article/details/74157219)[							](https://blog.csdn.net/u010182633/article/details/74157219)你从天而降的那刻起，我的世界变得难以言喻。\textbf{你从天而降的那刻起，我的世界变得难以言喻。}从陌生到熟悉，我从来没有奢望过爱情的到来。\textbf{从陌生到熟悉，我从来没有奢望过爱情的到来...[						](https://blog.csdn.net/u010182633/article/details/74157219)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛


[				](https://blog.csdn.net/zhang17173/article/details/83447482)
#### （数据可视化）Python实现随机*漫步*
#### 图像
[				](https://blog.csdn.net/zhang17173/article/details/83447482)10-27阅读数 							264
[				](https://blog.csdn.net/zhang17173/article/details/83447482)[							](https://blog.csdn.net/zhang17173/article/details/83447482)参考《Python编程：从入门到实践》1.随机漫步介绍随机漫步是这样行走得到的路径：每次行走都完全是随机的，没有明确的方向，结果是由一系列随机决策决定的。你可以这样认为，随机漫步就是蚂蚁在晕头转向的情...[						](https://blog.csdn.net/zhang17173/article/details/83447482)博文[来自：	](https://blog.csdn.net/zhang17173)zhang17173的博客

[				](https://blog.csdn.net/u010182633/article/details/53792588)*漫步*
#### 数学分析三——开集
[				](https://blog.csdn.net/u010182633/article/details/53792588)12-21阅读数 							1808
[				](https://blog.csdn.net/u010182633/article/details/53792588)[							](https://blog.csdn.net/u010182633/article/details/53792588)为了定义开集，我们首先介绍ε−disc\varepsilon-\text{disc}的概念。定义1\textbf{定义1}对于每个固定的x∈Rnx\inR^n以及ε>0\varepsilon>0，集合...[						](https://blog.csdn.net/u010182633/article/details/53792588)博文[来自：	](https://blog.csdn.net/u010182633)蜗牛

[				](https://blog.csdn.net/VBcom/article/details/7245186)
#### DirectX修复工具增强版
[				](https://blog.csdn.net/VBcom/article/details/7245186)02-09阅读数 							193万+
[				](https://blog.csdn.net/VBcom/article/details/7245186)[							](https://blog.csdn.net/VBcom/article/details/7245186)最后更新：2018-12-20DirectX修复工具最新版：DirectX Repair V3.8 增强版  NEW!版本号：V3.8.0.11638大小: 107MB/7z格式压缩，18...[						](https://blog.csdn.net/VBcom/article/details/7245186)博文[来自：	](https://blog.csdn.net/VBcom)VBcom的专栏

[				](https://blog.csdn.net/autocyz/article/details/48136473)
#### 相关滤波跟踪（MOSSE）
[				](https://blog.csdn.net/autocyz/article/details/48136473)08-31阅读数 							2万+
[				](https://blog.csdn.net/autocyz/article/details/48136473)[							](https://blog.csdn.net/autocyz/article/details/48136473)相关滤波跟踪（MOSSE）在信号处理中，有这么一个概念——相关性（correlation），用来描述两个因素之间的联系。而相关性又分为cross-correlation（互相关，两个信号之间的联系...[						](https://blog.csdn.net/autocyz/article/details/48136473)博文[来自：	](https://blog.csdn.net/autocyz)autocyz

[				](https://blog.csdn.net/legend12300/article/details/52385473)
#### Eclipse 经常 突然卡死，占用CPU非常高，无响应。
[				](https://blog.csdn.net/legend12300/article/details/52385473)08-31阅读数 							1万+
[				](https://blog.csdn.net/legend12300/article/details/52385473)[							](https://blog.csdn.net/legend12300/article/details/52385473)Eclipse 经常 突然卡死，占用CPU非常高，无响应。可能是eclipse.ini文件中配置的缓存大小过小。更改文件为-startupplugins/org.e...[						](https://blog.csdn.net/legend12300/article/details/52385473)博文[来自：	](https://blog.csdn.net/legend12300)legend12300的专栏

[				](https://blog.csdn.net/u010785685/article/details/52366977)
#### redis两种持久化策略
[				](https://blog.csdn.net/u010785685/article/details/52366977)08-30阅读数 							2万+
[				](https://blog.csdn.net/u010785685/article/details/52366977)[							](https://blog.csdn.net/u010785685/article/details/52366977)reids是一个key-value存储系统，为了保证效率，缓存在内存中，但是redis会周期性的把更新的数据写入磁盘或者把修改操作写入追加的记录文件，以保证数据的持久化。 所以：redis是一...[						](https://blog.csdn.net/u010785685/article/details/52366977)博文[来自：	](https://blog.csdn.net/u010785685)那么好,

[				](https://blog.csdn.net/csu_yang/article/details/5912659)
#### [转]wince 蓝牙
[				](https://blog.csdn.net/csu_yang/article/details/5912659)09-28阅读数 							3342
[				](https://blog.csdn.net/csu_yang/article/details/5912659)[							](https://blog.csdn.net/csu_yang/article/details/5912659)[转]wince 蓝牙  上面是WinCE中蓝牙协议栈的架构图：      可以看到，HCI传输层以上的协议层全部由WinCE实现了，所以蓝牙模块只需要实现了基带层和链路管理层，并提供HCI传输接口给...[						](https://blog.csdn.net/csu_yang/article/details/5912659)博文[来自：	](https://blog.csdn.net/csu_yang)csu_yang的专栏

[				](https://blog.csdn.net/sfeng95/article/details/62239539)
#### centos7 开启网络连接
[				](https://blog.csdn.net/sfeng95/article/details/62239539)03-15阅读数 							2万+
[				](https://blog.csdn.net/sfeng95/article/details/62239539)[							](https://blog.csdn.net/sfeng95/article/details/62239539)1.当我们发现无法联网时，我们运行下面命令或者ping命令ip  addr结果没有显示局域网的IP地址2.我们去修改网卡配置文件，把网络连接打开cd /cd  /etc/sys...[						](https://blog.csdn.net/sfeng95/article/details/62239539)博文[来自：	](https://blog.csdn.net/sfeng95)sfeng95的博客

[				](https://blog.csdn.net/ymj7150697/article/details/7384126)
#### expat介绍文档翻译
[				](https://blog.csdn.net/ymj7150697/article/details/7384126)03-22阅读数 							3万+
[				](https://blog.csdn.net/ymj7150697/article/details/7384126)[							](https://blog.csdn.net/ymj7150697/article/details/7384126)原文地址：http://www.xml.com/pub/a/1999/09/expat/index.html因为需要用，所以才翻译了这个文档。但总归赖于英语水平很有限，翻译出来的中文有可能...[						](https://blog.csdn.net/ymj7150697/article/details/7384126)博文[来自：	](https://blog.csdn.net/ymj7150697)ymj7150697的专栏

[				](https://blog.csdn.net/themagickeyjianan/article/details/52386981)
#### python图片处理类之~PIL.Image模块(ios android icon图标自动生成处理)
[				](https://blog.csdn.net/themagickeyjianan/article/details/52386981)08-31阅读数 							6万+
[				](https://blog.csdn.net/themagickeyjianan/article/details/52386981)[							](https://blog.csdn.net/themagickeyjianan/article/details/52386981)1.从pyCharm提示下载PIL包 http://www.pythonware.com/products/pil/ 2.解压后，进入到目录下cd /Users/jianan/Dow...[						](https://blog.csdn.net/themagickeyjianan/article/details/52386981)博文[来自：	](https://blog.csdn.net/themagickeyjianan)专注于cocos+unity+服务器全栈

[				](https://blog.csdn.net/u011726984/article/details/78432431)
#### 将数据从Java Bean复制到另一个Java Bean
[				](https://blog.csdn.net/u011726984/article/details/78432431)11-03阅读数 							7628
[				](https://blog.csdn.net/u011726984/article/details/78432431)[							](https://blog.csdn.net/u011726984/article/details/78432431)将数据从一个对象复制到另一个对象使用Spring的BeanUtils在开发中我们常会在对象之间设置属性值：Order order = new Order();order.setOrderAmount...[						](https://blog.csdn.net/u011726984/article/details/78432431)博文[来自：	](https://blog.csdn.net/u011726984)yidasanqian's blog

[				](https://blog.csdn.net/m0_37739193/article/details/78779686)
#### Flume断点续传深入研究
[				](https://blog.csdn.net/m0_37739193/article/details/78779686)12-12阅读数 							1651
[				](https://blog.csdn.net/m0_37739193/article/details/78779686)[							](https://blog.csdn.net/m0_37739193/article/details/78779686)方法一：在excel source中运用复杂的tail命令在百度中搜索到一篇文章：https://my.oschina.net/leejun2005/blog/288136可以在tail传的时候记...[						](https://blog.csdn.net/m0_37739193/article/details/78779686)博文[来自：	](https://blog.csdn.net/m0_37739193)小强签名设计 的博客

[				](https://blog.csdn.net/liufanghuangdi/article/details/52957094)
#### java+opencv 目标图像调整
[				](https://blog.csdn.net/liufanghuangdi/article/details/52957094)10-28阅读数 							6007
[				](https://blog.csdn.net/liufanghuangdi/article/details/52957094)[							](https://blog.csdn.net/liufanghuangdi/article/details/52957094)java+opencv 进行图像匹配，FeatureDetector ，DescriptorExtractor         DescriptorMatcher 配合使用。分别进行关键点检测，提取...[						](https://blog.csdn.net/liufanghuangdi/article/details/52957094)博文

[				](https://blog.csdn.net/clover_hxy/article/details/50683832)
#### bsgs算法
[				](https://blog.csdn.net/clover_hxy/article/details/50683832)02-18阅读数 							1万+
[				](https://blog.csdn.net/clover_hxy/article/details/50683832)[							](https://blog.csdn.net/clover_hxy/article/details/50683832)bsgs算法bsgs算法，又称大小步算法（某大神称拔山盖世算法）。主要用来解决   A^x=B(mod C)(C是质数)，都是整数，已知A、B、C求x。（poj 2417 Discrete Lo...[						](https://blog.csdn.net/clover_hxy/article/details/50683832)博文[来自：	](https://blog.csdn.net/clover_hxy)clover_hxy的博客

[				](https://blog.csdn.net/t247555529/article/details/48046859)
#### opencv 双目相机标定 自带例子程序的使用
[				](https://blog.csdn.net/t247555529/article/details/48046859)08-28阅读数 							1万+
[				](https://blog.csdn.net/t247555529/article/details/48046859)[							](https://blog.csdn.net/t247555529/article/details/48046859)有了上篇单目标定示例程序的经验，双目标定就是小菜一碟哈。本人目前菜鸟，但还是愿意厚着脸皮分享我一下午的成果。不要拍我...1.找到目录   ...\opencv\sources\sam...[						](https://blog.csdn.net/t247555529/article/details/48046859)博文[来自：	](https://blog.csdn.net/t247555529)t247555529的博客

[				](https://blog.csdn.net/u011860731/article/details/48733073)
#### ThreadLocal的设计理念与作用
[				](https://blog.csdn.net/u011860731/article/details/48733073)09-25阅读数 							6万+
[				](https://blog.csdn.net/u011860731/article/details/48733073)[							](https://blog.csdn.net/u011860731/article/details/48733073)Java中的ThreadLocal类允许我们创建只能被同一个线程读写的变量。因此，如果一段代码含有一个ThreadLocal变量的引用，即使两个线程同时执行这段代码，它们也无法访问到对方的Thread...[						](https://blog.csdn.net/u011860731/article/details/48733073)博文[来自：	](https://blog.csdn.net/u011860731)u011860731的专栏

[				](https://blog.csdn.net/terry_water/article/details/39377255)
#### file_get_contents 得到https类型url失败的方法
[				](https://blog.csdn.net/terry_water/article/details/39377255)09-18阅读数 							1万+
[				](https://blog.csdn.net/terry_water/article/details/39377255)[							](https://blog.csdn.net/terry_water/article/details/39377255)把filefunction getSSLPage($url) {    $ch = curl_init();	curl_setopt($ch, CURLOPT_RETURNTRANSF...[						](https://blog.csdn.net/terry_water/article/details/39377255)博文[来自：	](https://blog.csdn.net/terry_water)Terry - 专注外贸B2C

[				](https://blog.csdn.net/heroacool/article/details/51023921)
#### MATLAB--相机标定教程
[				](https://blog.csdn.net/heroacool/article/details/51023921)03-31阅读数 							2万+
[				](https://blog.csdn.net/heroacool/article/details/51023921)[							](https://blog.csdn.net/heroacool/article/details/51023921)对相机标定原理不熟悉的同学可以参考张正友标记法的译文 MATLAB：2014a待标记图像：http://download.csdn.net/detail/heroacool/9477530应用程序...[						](https://blog.csdn.net/heroacool/article/details/51023921)博文[来自：	](https://blog.csdn.net/heroacool)heroacool的专栏

[				](https://blog.csdn.net/Giraffe_zj/article/details/3343143)
#### JSON传送与接收(java)
[				](https://blog.csdn.net/Giraffe_zj/article/details/3343143)11-20阅读数 							2万+
[				](https://blog.csdn.net/Giraffe_zj/article/details/3343143)[							](https://blog.csdn.net/Giraffe_zj/article/details/3343143)在 JSON 中我们已经介绍过其基本格式，与XML相同，JSON只是一个文字格式，只要客户端与伺服端可以剖析它，就可以利用它作为传送数据的格式，但它是JavaScript的核心特性之一，所以在Java...[						](https://blog.csdn.net/Giraffe_zj/article/details/3343143)博文[来自：	](https://blog.csdn.net/Giraffe_zj)Giraffe_zj的专栏

[				](https://blog.csdn.net/qq_36892341/article/details/73918672)
#### linux上安装Docker(非常简单的安装方法)
[				](https://blog.csdn.net/qq_36892341/article/details/73918672)06-29阅读数 							20万+
[				](https://blog.csdn.net/qq_36892341/article/details/73918672)[							](https://blog.csdn.net/qq_36892341/article/details/73918672)最近比较有空，大四出来实习几个月了，作为实习狗的我，被叫去研究Docker了，汗汗！Docker的三大核心概念：镜像、容器、仓库镜像：类似虚拟机的镜像、用俗话说就是安装文件。容器：类似一个轻量...[						](https://blog.csdn.net/qq_36892341/article/details/73918672)博文[来自：	](https://blog.csdn.net/qq_36892341)我走小路的博客

[				](https://blog.csdn.net/u012373815/article/details/54632176)
#### springboot+mybatis＋SpringSecurity 实现用户角色数据库管理(一)
[				](https://blog.csdn.net/u012373815/article/details/54632176)01-20阅读数 							6万+
[				](https://blog.csdn.net/u012373815/article/details/54632176)[							](https://blog.csdn.net/u012373815/article/details/54632176)本文使用springboot+mybatis＋SpringSecurity 实现用户权限数据库管理实现用户和角色用数据库存储，而资源(url)和权限的对应采用硬编码配置。 也就是角色可以访问的权限通过...[						](https://blog.csdn.net/u012373815/article/details/54632176)博文[来自：	](https://blog.csdn.net/u012373815)哎幽的成长

[				](https://blog.csdn.net/standmyground/article/details/52576934)
#### QEMU 简单使用
[				](https://blog.csdn.net/standmyground/article/details/52576934)09-18阅读数 							8325
[				](https://blog.csdn.net/standmyground/article/details/52576934)[							](https://blog.csdn.net/standmyground/article/details/52576934)安装：  yum install qemu创建image文件：  qemu-img create -f qcow2 guest.qcow2 3G安装linux：  1）下载一个镜像文件，http:...[						](https://blog.csdn.net/standmyground/article/details/52576934)博文[来自：	](https://blog.csdn.net/standmyground)务远的博客

[				](https://blog.csdn.net/gefangshuai/article/details/50328451)
#### 关于SpringBoot bean无法注入的问题（与文件包位置有关）
[				](https://blog.csdn.net/gefangshuai/article/details/50328451)12-16阅读数 							17万+
[				](https://blog.csdn.net/gefangshuai/article/details/50328451)[							](https://blog.csdn.net/gefangshuai/article/details/50328451)问题场景描述整个项目通过Maven构建，大致结构如下：核心Spring框架一个module spring-boot-baseservice和dao一个module server-core提供系统...[						](https://blog.csdn.net/gefangshuai/article/details/50328451)博文[来自：	](https://blog.csdn.net/gefangshuai)开发随笔

[				](https://blog.csdn.net/qq574857122/article/details/16361033)
#### 强连通分量及缩点tarjan算法解析
[				](https://blog.csdn.net/qq574857122/article/details/16361033)11-16阅读数 							57万+
[				](https://blog.csdn.net/qq574857122/article/details/16361033)[							](https://blog.csdn.net/qq574857122/article/details/16361033)强连通分量：简言之 就是找环（每条边只走一次，两两可达）孤立的一个点也是一个连通分量  使用tarjan算法 在嵌套的多个环中优先得到最大环( 最小环就是每个孤立点） 定义：int Ti...[						](https://blog.csdn.net/qq574857122/article/details/16361033)博文[来自：	](https://blog.csdn.net/qq574857122)九野的博客

[				](https://blog.csdn.net/pingnanlee/article/details/68065535)
#### java session基于redis共享方案
[				](https://blog.csdn.net/pingnanlee/article/details/68065535)03-29阅读数 							8450
[				](https://blog.csdn.net/pingnanlee/article/details/68065535)[							](https://blog.csdn.net/pingnanlee/article/details/68065535)session基于redis共享有两种基本的方案 1、基于容器自身的扩展，比如tomcat的session-manage，可以参考如下 地址进行配置https://github.com/jcole...[						](https://blog.csdn.net/pingnanlee/article/details/68065535)博文[来自：	](https://blog.csdn.net/pingnanlee)pingnanlee的专栏

[				](https://blog.csdn.net/wuchengzeng/article/details/50037611)
#### jquery/js实现一个网页同时调用多个倒计时(最新的)
[				](https://blog.csdn.net/wuchengzeng/article/details/50037611)11-25阅读数 							44万+
[				](https://blog.csdn.net/wuchengzeng/article/details/50037611)[							](https://blog.csdn.net/wuchengzeng/article/details/50037611)jquery/js实现一个网页同时调用多个倒计时(最新的)最近需要网页添加多个倒计时. 查阅网络,基本上都是千遍一律的不好用. 自己按需写了个.希望对大家有用. 有用请赞一个哦!//js...[						](https://blog.csdn.net/wuchengzeng/article/details/50037611)博文[来自：	](https://blog.csdn.net/wuchengzeng)Websites

[				](https://blog.csdn.net/Ape55/article/details/73530419)
#### plsql的命令（command）窗口与sql窗口有什么区别20170620
[				](https://blog.csdn.net/Ape55/article/details/73530419)06-21阅读数 							3万+
[				](https://blog.csdn.net/Ape55/article/details/73530419)[							](https://blog.csdn.net/Ape55/article/details/73530419)command窗口是命令窗口，即为sqplus窗口，有命令提示符，识别sqlplus命令，基本的命令都可以执行sql仅可执行DDL、select、DML等...[						](https://blog.csdn.net/Ape55/article/details/73530419)博文[来自：	](https://blog.csdn.net/Ape55)Ape55的博客

[				](https://blog.csdn.net/zhmxy555/article/details/40955607)
#### 【Unity3D Shader编程】之二 雪山飞狐篇：Unity的基本Shader框架写法&颜色、光照与材质
[				](https://blog.csdn.net/zhmxy555/article/details/40955607)11-09阅读数 							7万+
[				](https://blog.csdn.net/zhmxy555/article/details/40955607)[							](https://blog.csdn.net/zhmxy555/article/details/40955607)本篇文章中，我们学习了Unity Shader的基本写法框架，以及学习了Shader中Properties（属性）的详细写法，光照、材质与颜色的具体写法。写了6个Shader作为本文Shader讲解的...[						](https://blog.csdn.net/zhmxy555/article/details/40955607)博文[来自：	](https://blog.csdn.net/zhmxy555)【浅墨的游戏编程Blog】毛星云（浅墨）的专栏

[				](https://blog.csdn.net/u011046042/article/details/47067037)
#### 基于Linux的智能家居的设计(5)
[				](https://blog.csdn.net/u011046042/article/details/47067037)07-26阅读数 							1万+
[				](https://blog.csdn.net/u011046042/article/details/47067037)[							](https://blog.csdn.net/u011046042/article/details/47067037)4  软件设计 软件设计部分主要包括uboot移植、内核编译、系统移植、设备驱动编程、应用程序编程（QT编程、mysql数据库编程、控制系统编程）、各个模块的功能函数（部分是在windows下面的...[						](https://blog.csdn.net/u011046042/article/details/47067037)博文[来自：	](https://blog.csdn.net/u011046042)求是07的专栏

[				](https://blog.csdn.net/hero_fantao/article/details/34533533)
#### 连续特征离散化和归一化
[				](https://blog.csdn.net/hero_fantao/article/details/34533533)06-25阅读数 							3万+
[				](https://blog.csdn.net/hero_fantao/article/details/34533533)[							](https://blog.csdn.net/hero_fantao/article/details/34533533)连续特征进行离散化处理。[						](https://blog.csdn.net/hero_fantao/article/details/34533533)博文[来自：	](https://blog.csdn.net/hero_fantao)hero_fantao的专栏

[				](https://blog.csdn.net/Seenway/article/details/49797921)
#### 钻一钻Tomcat 之 MyEclipse+Tomcat解压版（Ⅲ）
[				](https://blog.csdn.net/Seenway/article/details/49797921)11-12阅读数 							901
[				](https://blog.csdn.net/Seenway/article/details/49797921)[							](https://blog.csdn.net/Seenway/article/details/49797921)在实际的开发中，大多数人其实更倾向于用解压版的Tomcat，在搭配Eclipse或MyEclipse使用时，只需解压tomcat的压缩包，在Eclipse或MyEclipse中配置一下就可以用了 ...[						](https://blog.csdn.net/Seenway/article/details/49797921)博文[来自：	](https://blog.csdn.net/Seenway)Seenway的专栏

[				](https://blog.csdn.net/qq_30822013/article/details/53436772)
#### AS 常用快捷键
[				](https://blog.csdn.net/qq_30822013/article/details/53436772)12-02阅读数 							2872
[				](https://blog.csdn.net/qq_30822013/article/details/53436772)[							](https://blog.csdn.net/qq_30822013/article/details/53436772)上篇中我们讲了Android Studio如何添加插件，这篇我们讲讲AS的快捷键，这里我说明的快捷键都是最最实用的，希望刚刚加入AS的朋友尽快的熟悉一下这几个快捷键，这样可以帮助你提高coding的效...[						](https://blog.csdn.net/qq_30822013/article/details/53436772)博文[来自：	](https://blog.csdn.net/qq_30822013)Simidas

[				](https://blog.csdn.net/Tiaaaaa/article/details/58116346)
#### R语言逻辑回归、ROC曲线和十折交叉验证
[				](https://blog.csdn.net/Tiaaaaa/article/details/58116346)02-27阅读数 							5万+
[				](https://blog.csdn.net/Tiaaaaa/article/details/58116346)[							](https://blog.csdn.net/Tiaaaaa/article/details/58116346)自己整理编写的逻辑回归模板，作为学习笔记记录分享。数据集用的是14个自变量Xi，一个因变量Y的australian数据集。1. 测试集和训练集3、7分组australian ...[						](https://blog.csdn.net/Tiaaaaa/article/details/58116346)博文[来自：	](https://blog.csdn.net/Tiaaaaa)Tiaaaaa的博客

[				](https://blog.csdn.net/liminlu0314/article/details/7276954)
#### GDAL算法进度条使用说明
[				](https://blog.csdn.net/liminlu0314/article/details/7276954)02-20阅读数 							1万+
[				](https://blog.csdn.net/liminlu0314/article/details/7276954)[							](https://blog.csdn.net/liminlu0314/article/details/7276954)在调用GDAL算法的时候，希望能够显示其处理进度信息，其实在GDAL的算法API中，一般最后两个参数就是进度信息的指针。下面分别实现两种进度条信息，一种是在控制台中的进度条，一种是基于QT界面的进度条...[						](https://blog.csdn.net/liminlu0314/article/details/7276954)博文[来自：	](https://blog.csdn.net/liminlu0314)GDAL专栏

[				](https://blog.csdn.net/zd0303/article/details/7058457)
#### MATLAB中注释一段程序
[				](https://blog.csdn.net/zd0303/article/details/7058457)12-09阅读数 							2万+
[				](https://blog.csdn.net/zd0303/article/details/7058457)[							](https://blog.csdn.net/zd0303/article/details/7058457)在MATLAB中，可以注释一段程序。使用“%{”和“%}”。例如%{。。。%}即可。经典方法是用 if 0，但缺点是不够直观，注释掉的内容仍然保持代码的颜色。现在可以用...[						](https://blog.csdn.net/zd0303/article/details/7058457)博文[来自：	](https://blog.csdn.net/zd0303)知识小屋

[            设计制作学习          ](https://edu.csdn.net/combos/o363_l0_t )[            机器学习教程          ](https://edu.csdn.net/courses/o5329_s5330_k )[            Objective-C培训          ](https://edu.csdn.net/courses/o280_s351_k )[            交互设计视频教程          ](https://edu.csdn.net/combos/o7115_s388_l0_t )[            颜色模型          ](https://edu.csdn.net/course/play/5599/104252 )
[            mysql关联查询两次本表](https://www.csdn.net/gather_24/MtTaEg3sMDM5MS1ibG9n.html)[            native底部 react](https://www.csdn.net/gather_10/MtjaIg3sMTUzMy1kb3dubG9hZAO0O0OO0O0O.html)[            extjs glyph 图标](https://www.csdn.net/gather_1b/Ntzagg1sOTU3LWRvd25sb2Fk.html)[            武汉云计算ⅰs培训班](https://www.csdn.net/gather_4a/MtzaEg4sOTEtZWR1.html)[            python学习之通配符%s](https://www.csdn.net/gather_4a/NtDaEg0sMy1lZHUO0O0O.html)

![](https://csdnimg.cn/release/phoenix/images/feedLoading.gif)[
            ](https://csdnimg.cn/release/phoenix/images/feedLoading.gif)
没有更多推荐了，[返回首页](https://blog.csdn.net/)

[                ](https://blog.csdn.net/u010182633)![](https://avatar.csdn.net/2/2/4/3_u010182633.jpg)[
            ](https://avatar.csdn.net/2/2/4/3_u010182633.jpg)[                    ](https://blog.csdn.net/u010182633)
[会敲键盘的猩猩](https://blog.csdn.net/u010182633)
关注
[原创](https://blog.csdn.net/u010182633?t=1)8粉丝815喜欢212评论141
等级：[                    ](https://blog.csdn.net/home/help.html#level)[                ](https://blog.csdn.net/home/help.html#level)访问：51万+积分：7147排名：5294
勋章：
持之以恒
授予每个自然月内发布4篇或4篇以上原创或翻译IT博文的用户。不积跬步无以至千里，不积小流无以成江海，程序人生的精彩需要坚持不懈地积累！




### 最新文章
[漫步最优化四十四——基本拟牛顿法](https://blog.csdn.net/u010182633/article/details/78525729)[漫步最优化四十三——拟牛顿法](https://blog.csdn.net/u010182633/article/details/78509249)[漫步最优化四十二——Partan法](https://blog.csdn.net/u010182633/article/details/78503722)[漫步最优化四十一——Powell法(下)](https://blog.csdn.net/u010182633/article/details/78494168)[漫步最优化四十——Powell法(上)](https://blog.csdn.net/u010182633/article/details/78484327)

### 个人分类
[                    ](https://blog.csdn.net/u010182633/article/category/6998546)漫步最优化[                    ](https://blog.csdn.net/u010182633/article/category/6998546)45篇[                ](https://blog.csdn.net/u010182633/article/category/6998546)[                    ](https://blog.csdn.net/u010182633/article/category/6762278)漫步数理统计[                    ](https://blog.csdn.net/u010182633/article/category/6762278)34篇[                ](https://blog.csdn.net/u010182633/article/category/6762278)[                    ](https://blog.csdn.net/u010182633/article/category/6962742)GAN[                    ](https://blog.csdn.net/u010182633/article/category/6962742)1篇[                ](https://blog.csdn.net/u010182633/article/category/6962742)[                    ](https://blog.csdn.net/u010182633/article/category/6303247)漫步单变量微积分[                    ](https://blog.csdn.net/u010182633/article/category/6303247)38篇[                ](https://blog.csdn.net/u010182633/article/category/6303247)[                    ](https://blog.csdn.net/u010182633/article/category/6364207)漫步线性代数[                    ](https://blog.csdn.net/u010182633/article/category/6364207)27篇[                ](https://blog.csdn.net/u010182633/article/category/6364207)[                    ](https://blog.csdn.net/u010182633/article/category/6571925)漫步数学分析[                    ](https://blog.csdn.net/u010182633/article/category/6571925)49篇[                ](https://blog.csdn.net/u010182633/article/category/6571925)[                    ](https://blog.csdn.net/u010182633/article/category/6540395)漫步凸分析[                    ](https://blog.csdn.net/u010182633/article/category/6540395)9篇[                ](https://blog.csdn.net/u010182633/article/category/6540395)[                    ](https://blog.csdn.net/u010182633/article/category/3186993)PRML[                    ](https://blog.csdn.net/u010182633/article/category/3186993)44篇[                ](https://blog.csdn.net/u010182633/article/category/3186993)[                    ](https://blog.csdn.net/u010182633/article/category/5957129)数学杂谈[                    ](https://blog.csdn.net/u010182633/article/category/5957129)11篇[                ](https://blog.csdn.net/u010182633/article/category/5957129)[                    ](https://blog.csdn.net/u010182633/article/category/5714821)编程[                    ](https://blog.csdn.net/u010182633/article/category/5714821)4篇[                ](https://blog.csdn.net/u010182633/article/category/5714821)
展开

### 归档
[                    2017年11月                    ](https://blog.csdn.net/u010182633/article/month/2017/11)11篇[                ](https://blog.csdn.net/u010182633/article/month/2017/11)[                    2017年10月                    ](https://blog.csdn.net/u010182633/article/month/2017/10)12篇[                ](https://blog.csdn.net/u010182633/article/month/2017/10)[                    2017年9月                    ](https://blog.csdn.net/u010182633/article/month/2017/09)1篇[                ](https://blog.csdn.net/u010182633/article/month/2017/09)[                    2017年8月                    ](https://blog.csdn.net/u010182633/article/month/2017/08)1篇[                ](https://blog.csdn.net/u010182633/article/month/2017/08)[                    2017年7月                    ](https://blog.csdn.net/u010182633/article/month/2017/07)18篇[                ](https://blog.csdn.net/u010182633/article/month/2017/07)[                    2017年6月                    ](https://blog.csdn.net/u010182633/article/month/2017/06)8篇[                ](https://blog.csdn.net/u010182633/article/month/2017/06)[                    2017年5月                    ](https://blog.csdn.net/u010182633/article/month/2017/05)6篇[                ](https://blog.csdn.net/u010182633/article/month/2017/05)[                    2017年4月                    ](https://blog.csdn.net/u010182633/article/month/2017/04)12篇[                ](https://blog.csdn.net/u010182633/article/month/2017/04)[                    2017年3月                    ](https://blog.csdn.net/u010182633/article/month/2017/03)32篇[                ](https://blog.csdn.net/u010182633/article/month/2017/03)[                    2017年2月                    ](https://blog.csdn.net/u010182633/article/month/2017/02)11篇[                ](https://blog.csdn.net/u010182633/article/month/2017/02)[                    2017年1月                    ](https://blog.csdn.net/u010182633/article/month/2017/01)12篇[                ](https://blog.csdn.net/u010182633/article/month/2017/01)[                    2016年12月                    ](https://blog.csdn.net/u010182633/article/month/2016/12)11篇[                ](https://blog.csdn.net/u010182633/article/month/2016/12)[                    2016年11月                    ](https://blog.csdn.net/u010182633/article/month/2016/11)4篇[                ](https://blog.csdn.net/u010182633/article/month/2016/11)[                    2016年9月                    ](https://blog.csdn.net/u010182633/article/month/2016/09)18篇[                ](https://blog.csdn.net/u010182633/article/month/2016/09)[                    2016年8月                    ](https://blog.csdn.net/u010182633/article/month/2016/08)32篇[                ](https://blog.csdn.net/u010182633/article/month/2016/08)[                    2016年7月                    ](https://blog.csdn.net/u010182633/article/month/2016/07)14篇[                ](https://blog.csdn.net/u010182633/article/month/2016/07)[                    2015年12月                    ](https://blog.csdn.net/u010182633/article/month/2015/12)2篇[                ](https://blog.csdn.net/u010182633/article/month/2015/12)[                    2015年11月                    ](https://blog.csdn.net/u010182633/article/month/2015/11)6篇[                ](https://blog.csdn.net/u010182633/article/month/2015/11)[                    2015年10月                    ](https://blog.csdn.net/u010182633/article/month/2015/10)12篇[                ](https://blog.csdn.net/u010182633/article/month/2015/10)[                    2015年8月                    ](https://blog.csdn.net/u010182633/article/month/2015/08)2篇[                ](https://blog.csdn.net/u010182633/article/month/2015/08)[                    2015年6月                    ](https://blog.csdn.net/u010182633/article/month/2015/06)4篇[                ](https://blog.csdn.net/u010182633/article/month/2015/06)[                    2015年5月                    ](https://blog.csdn.net/u010182633/article/month/2015/05)32篇[                ](https://blog.csdn.net/u010182633/article/month/2015/05)[                    2015年4月                    ](https://blog.csdn.net/u010182633/article/month/2015/04)1篇[                ](https://blog.csdn.net/u010182633/article/month/2015/04)
展开

### 热门文章
[特征值和特征向量](https://blog.csdn.net/u010182633/article/details/45921929)阅读数51758
[高斯消元法](https://blog.csdn.net/u010182633/article/details/45225179)阅读数21834
[PCA-特征提取](https://blog.csdn.net/u010182633/article/details/45918737)阅读数19054
[协方差矩阵的几何解释](https://blog.csdn.net/u010182633/article/details/45937051)阅读数18004
[漫步微积分十——复合函数和链式法则](https://blog.csdn.net/u010182633/article/details/51998175)阅读数12225

### 最新评论
[漫步数理统计五——条件概率与独立(...](https://blog.csdn.net/u010182633/article/details/60989601#comments)[weixin_44772684：](https://my.csdn.net/weixin_44772684)请问例三中，计算概率时为什么不考虑有四种不同的花色
[漫步凸分析一——仿射集](https://blog.csdn.net/u010182633/article/details/53337794#comments)[panch3：](https://my.csdn.net/panch3)感谢！在挣扎数学规划书本的时候看到您的帖子，豁然开朗！
[协方差矩阵的几何解释](https://blog.csdn.net/u010182633/article/details/45937051#comments)[baidu_36984161：](https://my.csdn.net/baidu_36984161)[reply]myscnu[/reply]http://www.visiondummy.com/2014/04/geometric-interpretation-covariance-matrix/
[PCA-特征提取](https://blog.csdn.net/u010182633/article/details/45918737#comments)[qq_33167521：](https://my.csdn.net/qq_33167521)您好，能把代码发给我吗？谢谢776594546@qq.com ,
[漫步微积分五——速度和变化率](https://blog.csdn.net/u010182633/article/details/51934679#comments)[yt2235：](https://my.csdn.net/yt2235)漫步微积分五——速度和变化率，公式(6)写错了




点赞取消点赞1
[				](#commentBox)[				](#commentBox)评论[				](#commentBox)
[			](#commentBox)目录


收藏手机看
[					](https://blog.csdn.net/u010182633/article/details/78525729)[					](https://blog.csdn.net/u010182633/article/details/78525729)上一篇[				](https://blog.csdn.net/u010182633/article/details/78525729)[				](#chatqa)[				](#chatqa)更多[							](#chatqa)[							](https://blog.csdn.net/u010182633/article/details/78525729)[							](https://blog.csdn.net/u010182633/article/details/78525729)上一篇[						](https://blog.csdn.net/u010182633/article/details/78525729)
[
	](https://csdnimg.cn/release/blog_editor_html/release1.3.8/ckeditor/plugins/codesnippet/lib/highlight/styles/atom-one-light.css)