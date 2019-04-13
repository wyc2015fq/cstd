
# 漫步最优化四十一——Powell法(下) - 蜗牛 - CSDN博客


2017年11月09日 21:16:11[会敲键盘的猩猩](https://me.csdn.net/u010182633)阅读数：433



$\textbf{四季交替，}$
$\textbf{光阴推动了变更，}$
$\textbf{但你一直印在我心。}$
$\textbf{遇见你的那天，}$
$\textbf{心似击鼓般兴奋，}$
$\textbf{呼吸似乎都有了震音。}$
$\textbf{想你想的入神，}$
$\textbf{想你充满能量，}$
$\textbf{想你醒了也像晕了，}$
$\textbf{想你不由自主的笑了。}$
$\textbf{——畅宝宝的傻逼哥哥}$
对于步骤1，$\mathbf{d}_{01},\mathbf{d}_{02},\ldots,\mathbf{d}_{0n}$为坐标方向。对于步骤2，$f(\mathbf{x})$沿着$\mathbf{x}_{k0},\mathbf{x}_{k1},\ldots,\mathbf{x}_{kn}$最小化。对于步骤3，$f(\mathbf{x})$在新的共轭方向最小化，对于凸二维问题来说，该算法搜索模式如图4所示。
Powell算法的主要优点是不需要海森矩阵，更进一步，通过使用基于线搜索的一维算法，梯度也不需要。
但是Powell算法有时候不一定线性无关，这样的话生成的方向集无法生成$E^n$，即便是凸二次问题也存在这样的情况，如果第二步中最小化$f(\mathbf{x}_{k(j-1)}+\alpha\mathbf{d}_{kj})$时存在某个$j$使得$\alpha_{kj}=0$，就会导致这样的情况。这时候步骤三将得到

$$
\mathbf{d}_{k(n+1)}=\sum_{\substack{i=1\\i\neq j}}^n\alpha_{ki}\mathbf{d}_{ki}
$$
即新生成的方向不包含$\mathbf{d}_{kj}$，因为$\mathbf{d}_{kj}$别舍弃掉了，这样的$n$个方向集合无法张成$E^n$，这就意味着至少有两个方向是线性相关的，这样的话算法无法收敛到问题的解。
上面的问题可以被避免到，那就是如果出现线性相关，那么下次迭代的话我们不改变方向集，然后得到新的共轭方向。因为下次迭代的时候我们是从新的点$\mathbf{x}_{k}$开始的，所以是可能产生新的方向的。

![这里写图片描述](https://img-blog.csdn.net/20171109210451022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171109210451022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图4
原则上如果至少有一个$\alpha_{ki}$为零，那么将产生线性相关。不幸的是，由于计算机精度有限，$\alpha_{ki}$的值不可能为零，所以检查$\alpha_{ki}$不是可靠的方法，接下里介绍一种可替代的方法，依然是Powell得出来的。
如果方向向量$\mathbf{d}_{ki},i=1,2,\ldots,n$被归一化使得

$$
\mathbf{d}_{ki}^T\mathbf{Hd}_{ki}\quad\text{for}\ i=1,2,\ldots,n
$$
那么矩阵的行列式

$$
\mathbf{D}=[\mathbf{d}_{k1}\ \mathbf{d}_{k2}\ \cdots\ \mathbf{d}_{kn}]
$$
要想为最大值，那么当且仅当方向$\mathbf{d}_{ki}$属于共轭集合，所以如果舍弃非共轭方向$\mathbf{d}_{1k}$然后将共轭方向$\mathbf{d}_{k(n+1)}$加入到$\mathbf{D}$中，那么$\mathbf{D}$的行列式将会增加。另一方面，如果$\mathbf{d}_{k(n+1)}$的加入使得$\mathbf{D}$内产生线性相关，那么$\mathbf{D}$的行列式将减小。基于这个原则，Powell开发了修正的算法，该算法中使用一个测试来确定新生成的方向是否用于下次迭代，该测试也能识别是否$n$个老方向中的一个用新方向来替代，所以减小了线性相关的风险。
有个非常相似的技术也是用来消除线性相关，该方法由Zangwill提出。从计算上看，该方法比Powell修正更加经济有效，因此很值得详细的介绍一下。
Zangwill的方法是在Powell算法上进行了如下修正。
步骤1中的初始方向选为单位坐标向量集
\begin{align*}
\mathbf{D}_0
&=[\mathbf{d}_{01}\ \mathbf{d}_{02}\ \cdots\ \mathbf{d}_{0n}]\\
&=\begin{bmatrix}
1&0&\cdots&0\\
0&1&\cdots&0\\
\vdots&\vdots&&\vdots\\
0&0&\cdots&1
\end{bmatrix}
\end{align*}
\mathbf{D}_0的行列式\Delta_0为单位1。
步骤2中确定\alpha_{ki},i=1,2,\ldots,n的方法跟以前一样，然后选出最大的\alpha_{ki}即
\alpha_{km}=\max\{\alpha_{k1},\alpha_{k2},\ldots,\alpha_{kn}\}
步骤3中跟以前一样生成新的方向，然后归一化，
\mathbf{d}_{k(n+1)}=\frac{1}{\lambda_k}(\mathbf{x}_{kn}-\mathbf{x}_{k0})
其中
\lambda_k=\lVert\mathbf{x}_{kn}-\mathbf{x}_{k0}\rVert
步骤4不变，步骤5中，用上面得到的新方向代替方向\mathbf{d}_{km}，当然前提是这个替换能够保证
\mathbf{D}_k=[\mathbf{d}_{k1}\ \mathbf{d}_{k2}\ \cdots\ \mathbf{kn}]
的行列式有限且比常数\varepsilon_1大，其中0<\varepsilon_1\leq 1，即
0<\varepsilon_1<\Delta_k=\det\mathbf{D}_k\leq 1
否则的话，我们用最近的方向集用于下次迭代。因为
\Delta_k=\det[\mathbf{d}_{k1}\ \cdots\ \mathbf{d}_{k(m-1)}\ \mathbf{d}_{km}\ \mathbf{d}_{k(m+1)}\ \cdots\ \mathbf{d}_{kn}]
且
\mathbf{d}_{k(n+1)}=\frac{1}{\lambda_k}\sum_{i=1}^n\alpha_{ki}\mathbf{d}_{ki}
代替了\mathbf{d}_{km}，所以
\Delta_k^\prime=\frac{\alpha_{km}}{\alpha_k}\Delta_k
注意这里补充两个知识点
如果常数乘以一列并加到另一列，那么行列式不变。
如果某列乘以常数，那么行列式同样乘以该常数。
根据第一个知识点可知\Delta_k^\prime相加的效果可以消除掉，根据第二个知识点可知需要乘以\alpha_{km}/\lambda_k。如果
\frac{\alpha_{km}}{\lambda_k}\Delta_k>\varepsilon_1
那么我们令
\mathbf{d}_{(k+1)m}=\mathbf{d}_{k(n+1)}\quad\mathbf{d}_{(k+1)i}=\mathbf{d}_{ki}
其中i=1,2,\ldots,m-1,m+1,\ldots,n。否则的话
\mathbf{d}_{(k+1)i}=\mathbf{d}_{ki}
其中i=1,2,\ldots,n。同时也要更新\Delta_k
\delta_{k+1}=
\begin{cases}
\frac{\alpha_{km}}{\lambda_k}\Delta_k&\text{if}\ \frac{\alpha_{km}}{\lambda_k}\Delta_k>\varepsilon_1\\
\Delta_k&\text{otherwise}
\end{cases}
上面修正得到的结果就是方向矩阵的行列式一直有限且为正，这就表明方向一直是线性无关的。第二项的策略能够确保行列式的值\Delta_k尽可能大，从而避免线性相关。
修正的算法称为Zangwill算法，对于凸二次问题来说，该算法能够收敛。

