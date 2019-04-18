# 线性支持向量机中KKT条件的讨论 - 硬曲奇小屋 - CSDN博客





2017年06月05日 17:01:34[HardCookies](https://me.csdn.net/james_616)阅读数：2072








# 线性支持向量机中KKT条件的讨论

此处的模型为训练样本不可分时的线性支持向量机，简称为**线性支持向量机**。即考虑松弛变量$\xi_{i}$和惩罚参数$C$，目标函数为 


$\frac{1}{2} \| w \| ^{2} + C \sum_{i=1}^{N} \xi_{i}$

## 原始问题

则线性不可分的线性支持向量机的学习问题变成如下的凸二次规划（convex quadratic programming）问题，即原始问题 


$\begin{align}\min_{w,b,\xi} \quad & \frac{1}{2} \| w \| ^{2} + C \sum_{i=1}^{N} \xi_{i} \\\text{s.t.} \quad & y_i (w \cdot x_i + b) \ge 1 - \xi_i, \quad i=1,2,\cdots,N \\& \xi_i \ge 0, \quad i=1,2,\cdots,N\end{align}$

## 对偶问题

原始问题的拉格朗日函数为 


$L(w,b,\xi,\alpha,\mu) = \frac{1}{2} \| w \| ^{2} + C \sum_{i=1}^{N} \xi_{i} - \sum_{i=1}^{N} \alpha_i ( y_i (w \cdot x_i + b) - 1 + \xi_i) - \sum_{i=1}^{N} \mu_i \xi_i$

对偶问题拉格朗日函数的极大极小问题，得到以下等价优化问题 


$\begin{align}\min_{\alpha} \quad & \frac{1}{2} \sum_{i=1}^{N} \sum_{j=1}^{N} \alpha_i \alpha_j y_i y_j (x_i \cdot x_j) - \sum_{i=1}^{N} \alpha_i \\\text{s.t.} \quad &\sum_{i=1}^{N} \alpha_i y_i = 0 \\& 0 \le \alpha_i \le C, \quad i=1,2, \cdots,N\end{align}$
## KKT条件

原始问题的解对偶问题的解相同需要满足KKT对偶互补条件，即 


$\begin{eqnarray}& &\alpha_i ( y_i (w \cdot x_i + b) - 1 + \xi_i) = 0 \tag{1} \\& &\mu_i \xi_i = 0 \tag{2}\end{eqnarray}$

对样本$x_i$，记SVM的输出结果为 


$u_i = w \cdot x_i + b$

Platt在序列最小优化（SMO）方法[1](#fn:footer1)中提到，对正定二次优化问题（a positive definite QP problem）的优化点的充分必要条件为KKT条件（Karush-Kuhn-Tucker conditions）。 

对于所有的$i$，若满足以下条件，QP问题可解。 


$\begin{eqnarray}& \alpha_i = 0 \Leftrightarrow y_i u_i \ge 1 \tag{3} \\& 0 < \alpha_i < C \Leftrightarrow y_i u_i =1 \tag{4} \\& \alpha_i = C \Leftrightarrow y_i u_i \le 1 \tag{5} \end{eqnarray}%对eqnarray而言，分别是rCl对齐方式$

其中$y_i u_i$就是每个样本点的**函数间隔**，分离超平面$(w,b)$对应的函数间隔$\hat \gamma$取为1.
### KKT条件的推导

下面我们将要讨论如何从式(1)、(2)得到式(3) ~ (5)。

**(1) $\alpha_i =0 $**

由$C - \alpha_i - \mu_i = 0$，得 


$\mu_i =C$

则由式(2)可知， 


$\xi_i = 0$

再由原始问题的约束条件$y_i (w \cdot x_i + b) \ge 1 - \xi_i$，有 


$y_i u_i \ge 1$
**(2) $0<\alpha_i<C$**

将$\mu_i$乘到式(1)，有 


$\begin{equation}\mu_i \alpha_i y_i u_i - \mu_i \alpha_i + \mu_i \alpha_i \xi_i= 0 \\\mu_i \alpha_i (y_i u_i - 1) = 0 \end{equation}$

又$C - \alpha_i - \mu_i = 0$，则 


$(C - \alpha_i) \alpha_i (y_i u_i - 1) = 0$

因为$0<\alpha_i<C$，所以 


$y_i u_i = 1$

又由式(1)，有 


$y_i u_i = 1 - \xi_i \Rightarrow \xi_i = 0$
**(3) $\alpha_i = C$**

由式(1)，有 


$\begin{eqnarray}& y_i u_i - 1 + \xi_i = 0 \\& y_i u_i = 1 - \xi_i \tag{6}\end{eqnarray}$

因为$\xi_i \ge 0$，所以 


$y_i u_i \le 1$

即可得式(3) ~ (5)，KKT条件得以推导。
### KKT条件的几何解释

在线性不可分的情况下，将对偶问题的解$\alpha^* = (\alpha_1^*,\alpha_2^*,\cdots,\alpha_N^*)^T$中对应于$\alpha_i^* > 0$的样本点$(x_i,y_i)$称为支持向量[2](#fn:fotter2)。 

如下图所示，分离超平面由实线表示，间隔用虚线表示，正例由“o”表示，负例由“x”表示。实例$x_i$到间隔边界的距离为$\dfrac{\xi_i}{\|w\|}$。

![软间隔的支持向量](http://images0.cnblogs.com/blog/790160/201508/281741141878977.png)

软间隔的支持向量$x_i$或者**在间隔边界上**，或者**在间隔边界与分离超平面之间**，或者**在分离超平面误分一侧**。 

这里可以从两种角度来解释，第一种角度就像李航在《统计学习方法》第113页中用到间隔边界的距离边界$\dfrac{\xi_i}{\|w\|}$。因为，$\dfrac{1-\xi_i}{\|w\|}$为样本点$x_i$到分类超平面的距离，$\dfrac{1}{\|w\|}$是分类间隔到分类超平面的距离，可以根据$\xi_i$的大小来判断分类情况。

> 
若$\alpha_i^* < C$，则$\xi_i = 0$，支持向量$x_i$恰好落在间隔边界上； 

  若$\alpha_i^* = C,0 < \xi_i < 1$，则分类正确，$x_i$在间隔边界与分离超平面之间； 

  若$\alpha_i^* = C,\xi_i = 1$，则$x_i$在分离超平面上； 

  若$\alpha_i^* = C,\xi_i > 1$，则$x_i$位于分离超平面误分一侧。
现在我们要从另外一种角度，也就是KKT条件（式(3)~(5)），通过数学推导来得到上面的结果。

#### 在间隔边界上

由式(3)可知，当$0 < \alpha_i^* < C$时，$y_i u_i = 1$，则分类正确，且$u_i = \pm 1$，即在分类间隔边界上。

#### 在间隔边界与分离超平面之间

当$\alpha_i^* = C,0 < \xi_i <1$时，由式(6)得 


$0 < y_i u_i < 1$

则说明$y_i,u_i$同号，分类正确，且函数间隔小于1，即在间隔边界内。
#### 在分离超平面上

当$\alpha_i^* = C, \xi_i = 1$时，由式(6)得 


$y_i u_i = 0 \Rightarrow u_i = 0$

即$x_i$在分离超平面上。
#### 在分离超平面误分一侧

当$\alpha_i^* = C, \xi_i > 1$时，由式(6)得 


$y_i u_i <0$

则分类错误，$x_i$在分离超平面误分的一侧。
以上就是对线性支持向量机中KKT条件的仔细讨论，从公式推导和几何意义上一同解释了**为什么$\alpha_i^*$与$C$的大小关系决定支持向量的位置**。 

这是我在看书的时候不是很明白的问题，现在通过理论上的推导能够清楚地得到结果，也希望能够解答其他有同样问题人的疑惑。

问题是越思考越清楚，希望在今后的学习中继续保持这种认真对待知识的态度。

- Platt J. Sequential minimal optimization: A fast algorithm for training support vector machines[J]. 1998. [↩](#fnref:footer1)
- 李航. 统计学习方法[J]. 清华大学出版社, 北京, 2012. [↩](#fnref:fotter2)














