
# 漫步最优化三十九——Fletcher-Reeves法 - 蜗牛 - CSDN博客


2017年11月07日 21:34:00[会敲键盘的猩猩](https://me.csdn.net/u010182633)阅读数：696



$\textbf{你的目光像桥梁，}$
$\textbf{指引我通往你心路的捷径。}$
$\textbf{你的魅力像磁铁，}$
$\textbf{加快我靠向你身边的步伐。}$
$\textbf{想在你颈边轻轻呼吸，}$
$\textbf{想在你耳边吐露真言，}$
$\textbf{想成为你生命的韵律。}$
$\textbf{Pasito a pasito, suave suavecito}$
$\textbf{Nos vamos pegando, poquito a poquito}$
$\textbf{Y es que esa belleza es un rompecabezas}$
$\textbf{Pero pa montarlo aun tengo la pieza}$
$\qquad\qquad\qquad\quad\textbf{——畅宝宝的傻逼哥哥}$
Fletcher-Reeves法是共轭梯度法的变种，它的主要特征是参数$\alpha_k,k=0,1,2,\ldots$是用线搜索最小化$f(\mathbf{x}+\alpha\mathbf{d}_k)$确定的，这与最速下降或者牛顿法一样。而不同点在于$\mathbf{d}_k$是对$\mathbf{d}_{k-1},\mathbf{d}_{k-2},\ldots,\mathbf{d}_0$共轭，而不是最速梯度方向或者牛顿方向。
如果所求的问题是凸的，二次的且方向按共轭梯度发选择，那么

$$
\frac{df(\mathbf{x}_k+\alpha\mathbf{d}_k)}{d\alpha}=\mathbf{g}_{k+1}^T\mathbf{d}_k=0
$$
其中$k=0,1,2,\ldots$，更进一步，共轭的方向集合确保

$$
\frac{df(\mathbf{x}_k+\alpha\mathbf{d}_i)}{d\alpha}=\mathbf{g}_{k+1}^T\mathbf{d}_i=0\quad\text{for}\ 0\leq i\leq k
$$
或者

$$
\mathbf{g}_{k}^T\mathbf{d}_i=0\quad\text{for}\ 0\leq i<k
$$
所以通过线搜索确定的$\alpha_k$等价于共轭梯度法。因为线搜索需要更多的计算量，所以Fletcher-Reeves的修正是退化的一步，但不管怎样，这样做得到两个非常明显的好处：
这个修正使得该方法更加适应非二次问题的最小化，这是因为对不在解邻域内的点，f(\mathbf{x})沿着\mathbf{d}_k方向能够更大程度的减少，这是因为对于非二次问题，共轭梯度法得到的\alpha沿着\mathbf{d}_k方向不会得到最小值。
这个修正避免了推导计算海森矩阵。
如果每$n$次迭代后重新初始化，那么Fletcher-Reeves算法能够收敛，该算法的具体实现如下：

$$
\begin{align*}
&\textbf{算法1：共轭梯度算法}\\
&\textbf{步骤1}\\
&\text{输入}\mathbf{x}_0\text{并初始化容忍误差}\varepsilon\\
&\textbf{步骤2}\\
&\text{令}k=0\\
&\text{计算}\mathbf{g}_0\text{并令}\mathbf{d}_0=-\mathbf{g}_0\\
&\textbf{步骤3}\\
&\text{求}\alpha_k,\text{也就是最小化}f(\mathbf{x}_k+\alpha\mathbf{d}_k)\text{的}\alpha\\
&\text{令}\mathbf{x}_{k+1}=\mathbf{x}_k+\alpha_k\mathbf{d}_k\\
&\textbf{步骤4}\\
&\text{如果}\lVert\alpha_k\mathbf{d}_k\rVert<\varepsilon,\text{输出}\mathbf{x}^*=\mathbf{x}_{k+1},f(\mathbf{x}^*)=f_{k+1}\text{算法结束}\quad\quad\quad\quad\\
&\textbf{步骤5}\\
&\text{如果}k=n-1,\text{令}\mathbf{x}_0=\mathbf{x}_{k+1},\text{然后回到步骤2}\\
&\textbf{步骤6}\\
&\text{计算}\mathbf{g}_{k+1}\\
&\text{计算}\\
&\beta_k=\frac{\mathbf{g}_{k+1}^T\mathbf{g}_{k+1}}{\mathbf{g}_k^T\mathbf{g}_k}\\
&\text{令}\mathbf{d}_{k+1}=-\mathbf{g}_{k+1}+\beta_k\mathbf{d}_k\\
&\text{令}k=k+1\text{然后回到步骤3}
\end{align*}
$$


