# SVM解释：五、SMO算法 - guoziqing506的博客 - CSDN博客





2018年07月23日 08:42:47[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：644
所属专栏：[机器学习经典算法研究](https://blog.csdn.net/column/details/25189.html)









SMO算法是John C. Platt在1998年提出的。论文的题目是”Sequential Minimal Optimization：A Fast Algorithm for Training Support Vector Machines”。它很快便成为最快的二次规划优化算法，特别是在针对线性SVM和数据稀疏时性能更优。

当然了，SMO优越性能的背后是其相当复杂的数学推导，所以，我也把这块最难理解的内容放在了整个SVM学习笔记的最后一篇，我将尝试尽可能地把SMO的算法逻辑讲清楚，如有疏忽或错误，欢迎留言讨论。

## 1. SMO基本原理

我在之前的4篇博客当中，一直有一个遗留问题没有解决，那就是具体如何求解SVM对偶问题，比如我在第3篇博客 [SVM解释：三、线性可分的情况](https://blog.csdn.net/guoziqing506/article/details/81119449) 中的公式(3)中表示的优化问题，这个问题在加入了松弛变量和惩罚因子后，也就是我在第4篇博客 [SVM解释：四、线性不可分的情况](https://blog.csdn.net/guoziqing506/article/details/81120354) 中的公式(3)中表示的优化问题，具体如下：



$\begin{equation}\begin{aligned}\min ~&\frac{1}{2} \sum_{i, j = 1}^{n} \alpha_i \alpha_j y_i y_j X_i^T X_j - \sum_{i = 1}^{n} \alpha_i\\&s.t. ~0 \leq \alpha_i \leq C\\&~~~~~~\sum_{j = 1}^{m} \alpha_i y_i = 0\end{aligned}\end{equation}\tag{1}$

我们观察一下这个优化问题，X和Y（即训练数据）已知，然后在一定的约束范围内求取最优解（即拉格朗日乘子$\alpha$），使目标函数达到最小值。

实际上，求解此类问题的经典算法有很多，但是放在SVM中的话，会在效率上“不尽人意”。因为观察这个目标函数就会发现，其计算的复杂度是依赖于样本数$n$的，如果SVM中训练的数据集比较大，性能就成了问题。出于提高计算效率的目的，SMO算法应运而生。

总结起来一句话：**SMO算法是用来高效地求解公式(1)所示的SVM核心优化问题的**

我们知道，解决这样一个有多变量（$n$个$\alpha_i$）的优化问题确实比较困难，但是如果能多次迭代，每次选择两个变量优化，同时把其他变量看做是固定的常数，这样“分而治之”的话，问题似乎就容易多了。SMO算法的基本思想正是这样一种“分治法”。

显然，这样做有两个问题需要解决：
- 每次选择哪两个变量？
- 每次迭代如何进行优化计算？

这两个问题实际上就是SMO算法的两个组成部分，下面我分别就这两个问题展开阐述。我先说优化计算的过程，最后再谈每次迭代计算是如何选择变量的。

## 2. 优化计算

### 2.1 新的优化问题

首先想想为什么要选择两个变量进行优化。假设我选择的是变量$\alpha_1, \alpha_2$，那就把其他的$n - 2$个$\alpha_i$看做是固定的常数，因为有约束条件$\sum_{j = 1}^{m} \alpha_i y_i = 0$存在（公式(1)的第二个约束条件），所以如果我确定了$\alpha_1$迭代后的新值，自然可以通过等式关系确定$\alpha_2$迭代后的值。

回到(1)式所示的SVM核心优化问题，假设选择的两个变量是$\alpha_1, \alpha_2$，则此时的目标函数展开如下：



$\begin{equation}f(\alpha_1, \alpha_2) = \frac{1}{2} K_{11} \alpha_1^2 + \frac{1}{2} K_{22} \alpha_2^2 + y_1y_2 K_{12} \alpha_1 \alpha_2 + y_1 \alpha_1 v_1 + y_2 \alpha_2 v_2 - \alpha_1 - \alpha_2 + r\end{equation}\tag{2}$

关于这个式子，有以下3点需要说明：
- 
为表示简洁，记$K(X_i, X_j) = K_{i,j}$，你可能会问，(1)式里面不涉及核函数$K(,)$啊，为什么这里冒出来了？其实没有没核函数的参与，并不影响SMO算法的推导。为了和大多SMO算法相关的文献一致，我将$X_1$与$X_2$的内积直接用核函数的形式表示了，你可以理解为先用核函数映射，再执行加入松弛变量和惩罚因子的线性分类。

- 
$v_1, v_2$如下表示。其中$\alpha_i^*$表示$\alpha_i$在上一次迭代中的值。你可以这样理解：每次迭代$\alpha_1$与$\alpha_2$的值都是由上一轮迭代结束时的$\alpha_i$的值计算得到的。 


$\begin{equation}\left\{\begin{aligned}&v_1 = \sum_{i = 3}^{n} y_i\alpha_i^* K_{i1}\\&v_2 = \sum_{i = 3}^{n} y_i\alpha_i^* K_{i2}\end{aligned}\right.\end{equation}\tag{3}$

- 
$r$表示常数项的组合，因为与后面的推导无关，我不写出来了。


新的目标函数有了，再看约束条件，根据公式(1)的两个约束条件，可以得到新约束条件如下：



$\begin{equation}\left\{\begin{aligned}&0 \leq \alpha_1 \leq C\\&0 \leq \alpha_2 \leq C\\&y_1 \alpha_1 + y_2 \alpha_2 = - \sum_{i = 3}^{n} y_i \alpha_i = \gamma\end{aligned}\right.\end{equation}\tag{4}$

根据公式(2),(4)，我们当前得到了一个两个未知数的，带等式与不等式约束的二次优化问题。

### 2.2 模型分析

二次优化问题是我们熟悉的内容，观察一下上面的公式(4)，很显然，这个约束是在边长为$C$的正方形范围内的直线。给约束条件的第3条两侧同时乘$y_1$，得到：



$\begin{equation}\alpha_1 = y_1 \gamma - y_1y_2 \alpha_2\end{equation}\tag{5}$

那现在只需要考虑$\alpha_2$即可了，两变量的优化问题也就变成了单变量的优化问题。我们按照现在的约束条件在Fig.1画出了$\alpha_2$的可行域，不论是哪种情况，可行域都是在边长为$C$的正方形中的一条直线段：



![](https://img-blog.csdn.net/20180721205128300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3ppcWluZzUwNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


大体上说，可行域有两种可能性：
- 
$y_1$与$y_2$异号（一个1，一个-1），此时的直线段斜率为1，如Fig.1(a)所示。我们可以根据$y_1, y_2$的不同取值以及$\gamma$的不同范围分析$\alpha_2$的取值范围，我用下界$L$和上界$H$表示： 


$\begin{equation}\begin{aligned}&L = \max (0, \alpha_2 - \alpha_1)\\&H = \min (C, C + \alpha_2 - \alpha_1)\\\end{aligned}\end{equation}$

- 
$y_1$与$y_2$同号（都是1，或都是-1），此时的直线段斜率为-1，如Fig.1(b)所示。我们可以根据$y_1, y_2$的不同取值以及$\gamma$的不同范围分析$\alpha_2$的取值范围，我用下界$L$和上界$H$表示： 


$\begin{equation}\begin{aligned}&L = \max (0, \alpha_2 + \alpha_1 - C)\\&H = \min (C, \alpha_2 + \alpha_1)\\\end{aligned}\end{equation}$


那也就是说，现在的优化问题是一个单变量的二次优化，且有一定的约束范围。那下面的思路就清晰了：（1）判断二次项系数的正负；（2）求解极小值；（3）判断极小值和约束范围的关系。

### 2.3 优化计算

把公式(5)代入公式(2)，得到下面的公式(6)，这就是单变量二次优化的目标函数：



$\begin{equation}f(\alpha_2) = \frac{1}{2}(K_{11} + K_{22} - 2K_{12})\alpha_2^2 + (AB \cdot K_{12} - AB \cdot K_{11} + y_2v_2 - y_2v_1 + B + 1) \alpha_2 + r'\end{equation}\tag{6}$

其中，$A = y_1 \gamma, B = y_1y_2$，$r'$为常数项，和之前一样，具体的表示我们忽略。

我们知道二次项系数$K_{11} + K_{22} - 2K_{12} \geq 0$，想想什么时候是等于0的呢？就是$X_1 = X_2$的时候，为方便推导，我们暂时假定不会有重复的数据参与计算，也就是说$X_1 \neq X_2$，那么$K_{11} + K_{22} - 2K_{12} > 0$。其实，等于0的情况与大于0的解法是类似的，他是一个一次函数的图像。我们不需要求解极值，直接判断约束区域和一次函数的关系即可。

言归正传，先讨论大于0的情况，此时是一个标准的二次优化问题，是一个开口朝上的抛物线。
- 
令$\alpha_2^{new, unclipped}$表示经过迭代之后，这个二次函数未经剪辑的最小值（也就是极值点）。剪辑在此处的意思是根据约束修正最小值。

- 
令$\alpha_2^{new}$表示经过迭代之后，又剪辑了的最小值（也就是考虑了约束条件的），那么我们有如下结论成立：




$\begin{equation}\alpha_2^{new} = \left\{\begin{aligned}&H,~~~~~~~~~~~~~~~~~~~~~\alpha_2^{new, unclipped} > H\\&\alpha_2^{new, unclipped},~~~~~L \leq \alpha_2^{new, unclipped} \leq H\\&L,~~~~~~~~~~~~~~~~~~~~~~\alpha_2^{new, unclipped} < L\end{aligned}\right.\end{equation}\tag{7}$

下面求取极值。对公式(6)关于$\alpha_2$求导，并令导数为0：



$\begin{equation}f(\alpha_2)' = (K_{11} + K_{22} - 2K_{12})\alpha_2 + (AB \cdot K_{12} - AB \cdot K_{11} + y_2v_2 - y_2v_1 + B + 1) = 0\end{equation}\tag{8}$

现在，我把公式(3)中的$v_1, v_2$带入上面这个公式(8)，化简就能知道如何根据上一轮迭代的结果计算出本轮迭代中目标函数的极值了（即$\alpha_2^{new, unclipped}$），再根据约束范围判定最优解即可。但是，后面我会讲到如何选择每轮迭代的两个变量，所以，为了方便后面的讲解，我在此求取极值的时候，按照如下的方式化简：（注意，做法一样，只是在极值表示结果的形式上做了点处理）

先设置两个新的变量$u_1, u_2$，如下：



$\begin{equation}\left\{\begin{aligned}&u_1 = \sum_{i = 1}^{n} y_i \alpha_i^* K_{i1} + b \\&u_2 = \sum_{i = 1}^{n} y_i \alpha_i K_{i2} + b\\\end{aligned}\right.\end{equation}\tag{9}$

我们发现，$u_1, u_2$就是每一轮迭代中，得到的“临时分类器”对元组$X_1$和$X_2$分类的结果。同时他们与公式(3)中的$v_1, v_2$还存在如下的等式关系：



$\begin{equation}\left\{\begin{aligned}& v_1 = u_1 - y_1\alpha_1^*K_{11} - y_2\alpha_2^*K_{21} - b\\& v_2 = u_2 - y_1\alpha_1^*K_{12} - y_2\alpha_2^*K_{22} - b\\\end{aligned}\right.\end{equation}\tag{10}$

把公式(9)中的$v_1, v_2$代入公式(8)：



$\begin{equation}\begin{aligned}(K_{11} + K_{22} - 2K_{12})\alpha_2 &= AB \cdot (K_{11} - K_{12}) + y_2(v_1 - v_2) - B + 1\\&=(K_{11} + K_{22} - 2K_{12})\alpha_2^* + y_2(u_1 - y_1 - (u_2 - y_2))\end{aligned}\end{equation}\tag{11}$

公式(11)中，$\alpha_2^* $表示$\alpha_2$在上一轮迭代结束后的值，为了更形象，记为$\alpha_2^{old}$.

记$E_i = u_i - y_i$，显然，这个值代表预测值与真实值的差。这里不展开谈，你先有个印象，后面讲解选择变量的时候会用到。

记$\eta = K_{11} + K_{22} - 2K_{12}$，令公式(10)两侧同时除$\eta$，得到下式：



$\begin{equation}\alpha_2^{new, unclipped} = \alpha_2^{old} + y_2\frac{E_1 - E_2}{\eta}\end{equation}\tag{12}$

公式(12)展示了迭代中变量是如何迭代优化的，当然这还是个未经剪辑的结果，我们考虑他的约束条件形成的可行域，这一点我们上面已经说过了，参见公式(7)。最终得到经过剪辑的$\alpha_2$的最优解。再根据公式(4)就可得到$\alpha_1^{new}$.

综上，一轮迭代的优化计算就算是完成了，我们通过上一轮迭代的结果，计算出了本轮迭代中所选取的两个变量的新值。

## 3. 变量选择

SMO算法中对于变量的选择采用的是一种启发式规则。简单来说，在每轮迭代中，选取优化变量的基本指导思想应该是这样的：每轮迭代会确定一个“临时分类器”。显然，我们不关心这个临时分类器的侧面（$H_1, H_2$）之外的元组，而只是关心在侧面之上以及侧面之间（即边缘内）的点，所以在下一轮迭代中，就应该在这些关心的元组中选择两个，对它们对应的拉格朗日乘子进行优化，至于优化计算，上面已经说得比较详细了。

虽然大致确定了变量选择的范围，但是到底应该选择哪两个$\alpha_i$还是问题，假设选择的是$\alpha_i$和$\alpha_j$，那么SMO的启发式规则是这样选择的：
- 先确定在当前的分类器中，违反KKT条件的元组$X_i$；
- 根据$X_i$，找到使得$|E_i - E_j|$最大的元组$X_j$；

在解释上面的规则之前，先回顾一下我在上一篇博客中，讲到了加入松弛变量和惩罚因子后，如何根据拉格朗日乘子$\alpha_i$确定对应训练元组$X_i$与分类器的关系。相关内容可以参见上一篇博客中的公式(4)，我也把它抄在下面：



$\begin{equation}\left\{\begin{aligned}&if ~~\alpha_i = 0,  ~~~~~~~~~~~y_i(WX_i + b) \geq 1\\&if ~~\alpha_i = C,  ~~~~~~~~~~y_i(WX_i + b) \leq 1\\&if ~~0 < \alpha_i < C,  ~~~y_i(WX_i + b) = 1\\\end{aligned}\right.\end{equation}\tag{14}$
- 
寻找$\alpha_i$： 

上面说到，SMO算法的迭代中，我们关心的是那些侧面之上以及侧面之间的点，因此，我们先找那些拉格朗日乘子$0 < \alpha_i < C$所对应的元组，看看它们是否满足公式(14)表示的KKT条件，直到找到一个不满足的，也就确定了第一个优化变量$\alpha_i$；如果都满足、找不到违反KKT的呢，那就再找那些$\alpha_i = C$的，最后找$\alpha_i = 0$的，如果都满足，则SMO算法的迭代终止。

- 
寻找$\alpha_j$： 

由公式(12)可以知道，本轮迭代的最优解的变化程度与$|E_i - E_j|$是呈正比的，为了加快迭代的速度，我们自然想到得找这种变化大的变量进行优化计算，所以在上一步确定了$\alpha_i$后，我们遍历样本集，找到使得$|E_i - E_j|$最大的$X_j$，以它对应的$\alpha_j$为另一个优化变量即可。


上面说到，SMO算法的迭代中，我们关心的是那些侧面之上以及侧面之间的点，因此，我们先找那些拉格朗日乘子$0 < \alpha_i < C$所对应的元组，看看它们是否满足公式(14)表示的KKT条件，直到找到一个不满足的，也就确定了第一个优化变量$\alpha_i$；如果都满足、找不到违反KKT的呢，那就再找那些$\alpha_i = C$的，最后找$\alpha_i = 0$的，如果都满足，则SMO算法的迭代终止。

## 3. $b$值的计算

每轮迭代结束之后，我们都要更新相关的$W$，$b$以及对应每个元组的$E_i$。$W$不用多说了，根据迭代新得到的$\alpha$计算就行，至于$b$则需要找到侧面上的点，结合$W$计算。具体地说，

当$0 < \alpha_1 < C$时，$X_1$在侧面上，满足下式：



$\begin{equation}y_1 - \sum_{i = 1}^{n} \alpha_iy_iK_{i1} = 0\end{equation}$

根据这个式子，解出此时新的$b$值即可（记为$b_1^{new}$）。计算过程我省略了，直接看结果：



$\begin{equation}b_1^{new} = -E_1 - y_1K_{11}(\alpha_1^{new} - \alpha_1^{old}) - y_2K_{21}(\alpha_2^{new} - \alpha_2^{old}) + b^{old} \end{equation}$

同理，当$0 < \alpha_2 < C$时，$X_2$在侧面上，新的$b$值也可以这样计算（记为$b_2^{new}$）：



$\begin{equation}b_2^{new} = -E_2 - y_1K_{12}(\alpha_1^{new} - \alpha_1^{old}) - y_2K_{22}(\alpha_2^{new} - \alpha_2^{old}) + b^{old} \end{equation}$

其实，很容易证明得到，当$\alpha_1, \alpha_2$都在区间$(0, C)$中时，上面两个$b$的计算结果是一致的。那如果现在$\alpha_1, \alpha_2$都不在区间$(0, C)$中呢，我们就去上面两个$b$的平均值，即$b^{new} = (b_1^{new} + b_2^{new}) / 2$

综上，得到$b$的计算公式，即下面的公式(15)：



$\begin{equation}b^{new} = \left\{\begin{aligned}&b_1^{new},~~~~~~~~~~~~~~~~~~~~~0 < \alpha_1 < C\\&b_2^{new},~~~~~~~~~~~~~~~~~~~~~0 < \alpha_2 < C\\&(b_1^{new} + b_2^{new}) / 2,~~~~~~~~~~~~~~otherwise\\\end{aligned}\right.\end{equation}\tag{15}$

## 4. SMO算法步骤

到这里，SMO算法的大致介绍总算是说完了。我把它的步骤概括式的总结一下：
- 初始化$\alpha$，一般情况下令初始的$\alpha_i$全部为0；
- 选取优化变量$\alpha_1$和$\alpha_2$，执行相关的优化计算，得到更新后的$\alpha_1, \alpha_2$；
- 开始新的一轮迭代，重复执行上面的第2步，知道全部的$\alpha_i$满足公式(14)的KKT条件以及公式(1)中的约束条件；



