# BPTT算法推导 - Hiroki - 博客园




## [BPTT算法推导](https://www.cnblogs.com/wacc/p/5341670.html)


![](https://images2015.cnblogs.com/blog/583155/201608/583155-20160819153519750-1203124108.png)

## 随时间反向传播 （BackPropagation Through Time，BPTT）

### 符号注解：
- \(K\):词汇表的大小
- \(T\):句子的长度
- \(H\):隐藏层单元数
- \(E_t\):第t个时刻（第t个word）的损失函数,定义为交叉熵误差\(E_t=-y_t^Tlog(\hat{y}_t)\)
- \(E\):一个句子的损失函数，由各个时刻（即每个word）的损失函数组成,\(E=\sum\limits_t^T E_t\)。

注： 由于我们要推倒的是SGD算法， 更新梯度是相对于一个训练样例而言的， 因此我们一次只考虑一个句子的误差，而不是整个训练集的误差（对应BGD算法）
- \(x_t\in\mathbb{R}^{K\times 1}\):第t个时刻RNN的输入，为one-hot vector，1表示一个单词的出现，0表示不出现
- \(s_t\in\mathbb{R}^{H\times 1}\):第t个时刻RNN隐藏层的输入
- \(h_t\in\mathbb{R}^{H\times 1}\):第t个时刻RNN隐藏层的输出
- \(z_t\in\mathbb{R}^{K\times 1}\):输出层的汇集输入
- \(\hat{y}_t\in\mathbb{R}^{K\times 1}\):输出层的输出，激活函数为softmax
- \(y_t\in\mathbb{R}^{K\times 1}\):第t个时刻的监督信息，为一个one-hot vector
- \(r_t=\hat{y}_t-y_t\)：残差向量
- \(W\in\mathbb{R}^{H\times K}\):从输入层到隐藏层的权值
- \(U\in\mathbb{R}^{H\times H}\):隐藏层上一个时刻到当前时刻的权值
- \(V\in\mathbb{R}^{K\times H}\):隐藏层到输出层的权值

他们之间的关系：
\[\left\{\begin{aligned}&s_t=Uh_{t-1}+Wx_t\\&h_t=\sigma(s_t)\\&z_t=Vh_t\\& \hat{y}_t=\mathrm{softmax}(z_t) \end{aligned}\right.\]

其中，\(\sigma(\cdot)\)是sigmoid函数。由于\(x_t\)是one-hot向量，假设第\(j\)个词出现，则\(Wx_t\)相当于把\(W\)的第\(j\)列选出来，因此这一步是不用进行任何矩阵运算的，直接做下标操作即可，在matlab里就是\(W(:,x_t)\)。

BPTT与BP类似，是在时间上反传的梯度下降算法。RNN中，我们的目的是求得\(\frac{\partial E}{\partial U},\frac{\partial E}{\partial W},\frac{\partial E}{\partial V}\)，根据这三个变化率来优化三个参数\(U,V,W\)

注意到\(\frac{\partial E}{\partial U}=\sum\limits_t \frac{\partial E_t}{\partial U}\)，因此我们只要对每个时刻的损失函数求偏导数再加起来即可。

1.计算\(\frac{\partial E_t}{\partial V}\)
\[ \begin{aligned}\frac{\partial E_t}{\partial V_{ij}}&=tr\bigg( \big( \frac{\partial E_t}{\partial z_t}\big)^T\cdot \frac{\partial z_t}{\partial V_{ij}}\bigg)\\&=tr\bigg((\hat{y}_t-y_t)^T\cdot\begin{bmatrix}0\\ \vdots \\ \frac{\partial z_{t}^{(i)}}{\partial V_{ij}}\\\vdots\\0\end{bmatrix}\bigg)\\&=r_t^{(i)} h_t^{(j)}\end{aligned}\]

注：推导中用到了之前推导用到的结论。其中\(r_t^{(i)}=(\hat{y}_t-y_t)^{(i)}\)表示残差向量第i个分量，\(h_t^{(j)}\)表示\(h_t\)的第j个分量。

上述结果可以改写为：
\[ \frac{\partial E_t}{\partial V}=(\hat{y}_t-y_t)\otimes h_t\]
\[ \frac{\partial E}{\partial V} = \sum_{k=0}^t (\hat{y}_k-y_k)\otimes h_k \]

其中\(\otimes\)表示向量外积。

2.计算\(\frac{\partial E_t}{\partial U}\)

由于U是各个时刻共享的，所以t之前每个时刻U的变化都对\(E_t\)有贡献，反过来求偏导时，也要考虑之前每个时刻U对E的影响。我们以\(s_k\)为中间变量，应用链式法则：
\[ \frac{\partial E_t}{\partial U} = \sum_{k=0}^t \frac{\partial s_k}{\partial U} \frac{\partial E_t}{\partial s_k}\]

但由于\(\frac{\partial s_k}{\partial U}\)（分子向量，分母矩阵）以目前的数学发展水平是没办法求的，因此我们要求这个偏导，可以拆解为\(E_t\)对\(U_{ij}\)的偏导数：
\[ \frac{\partial E_t}{\partial U_{ij}} = \sum_{k=0}^t tr[(\frac{\partial E_t}{\partial s_k})^T \frac{\partial s_k}{\partial U_{ij}}]=  \sum_{k=0}^t  tr[(\delta_k)^T\frac{\partial s_k}{\partial U_{ij}}]\]

其中，\(\delta_k=\frac{\partial E_t}{\partial s_k}\)，遵循
\[s_k\to h_k\to s_{k+1}\to ...\to E_t\]

的传递关系，应用链式法则有：
\[\delta_k=\frac{\partial h_k}{\partial s_k}\frac{\partial s_{k+1}}{\partial h_k} \frac{\partial E_t}{\partial s_{k+1}}=diag(1-h_k\odot h_k)U^T\delta_{k+1}=(U^T\delta_{k+1})\odot (1-h_k\odot h_k)\]

其中，\(\odot\)表示向量点乘。于是，我们得到了关于\(\delta\) 的递推关系式。由\(\delta_t\)出发，我们可以往前推出每一个\(\delta\)，现在计算\(\delta_t\)：

\begin{equation}\delta_t=\frac{\partial E_t}{\partial s_t}=\frac{\partial h_t}{\partial s_t}\frac{\partial z_t}{\partial h_t}\frac{\partial E_t}{\partial z_t}=diag(1-h_t\odot h_t)\cdot V^T\cdot(\hat{y}_t-y_t)=(V^T(\hat{y}*t-y_t))\odot (1-h_t\odot h_t)\end{equation}将\(\delta_0,...,\delta_t\)代入$ \frac{\partial E_t}{\partial U*{ij}} $有：
\[  \frac{\partial E_t}{\partial U_{ij}} = \sum_{k=0}^t \delta_k^{(i)} h_{k-1}^{(j)} \]

将上式写成矩阵形式：
\[  \frac{\partial E_t}{\partial U} = \sum_{k=0}^t \delta_k \otimes h_{k-1} \]

不失严谨性，定义\(h_{-1}\)为全0的向量。

3.计算\(\frac{\partial E_t}{\partial W}\)

按照上述思路，我们可以得到
\[   \frac{\partial E_t}{\partial W} = \sum_{k=0}^t \delta_k \otimes x_{k} \]

由于\(x_k\)是个one-hot vector，假设其第\(m\)个位置为1，那么我们在更新\(W\)时只需要更新\(W\)的第\(m\)列即可，计算\(\frac{\partial{E_t}}{\partial{W}}\)的伪代码如下：

```
delta_t = V.T.dot(residual[T]) * (1-h[T]**2)
for t from T to 0
    dEdW[ :,x[t] ] += delta_t
    #delta_t = W.T.dot(delta_t) * (1 - h[t-1]**2)
    delta_t = U.T.dot(delta_t) * (1 - h[t-1]**2)
```








