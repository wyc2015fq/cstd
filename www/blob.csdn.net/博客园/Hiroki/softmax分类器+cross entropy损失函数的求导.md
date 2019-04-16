# softmax分类器+cross entropy损失函数的求导 - Hiroki - 博客园




## [softmax分类器+cross entropy损失函数的求导](https://www.cnblogs.com/wacc/p/5341676.html)


softmax是logisitic regression在多酚类问题上的推广，\(W=[w_1,w_2,...,w_c]\)为各个类的权重因子，\(b\)为各类的门槛值。不要想象成超平面，否则很难理解，如果理解成每个类的打分函数，则会直观许多。预测时我们把样本分配到得分最高的类。

Notations:
- \(x\):输入向量，\(d\times 1\)列向量，\(d\)是feature数
- \(W\):权重矩阵，\(c\times d\)矩阵，\(c\)是label数
- \(b\)：每个类对应超平面的偏置组成的向量， \(c\times 1\)列向量
- \(z=Wx+b\):线性分类器输出， \(c\times 1\)列向量
- \(\hat{y}\):softmax函数输出， \(c\times 1\)列向量
- 记\(\vec{e}_j=[0,...,1,...,0]^T\in\mathbb{R}^{c\times 1}\)，其中\(1\)出现在第\(j\)个位置
- \(1_c\)表示一个全\(1\)的\(c\)维列向量
- \(y\)：我们要拟合的目标变量，是一个one-hot vector（只有一个1，其余均为0），也是 \(c\times 1\)列向量 。 我们将其转置，表示为一个列向量：
\[y=[0,...,1,...,0]^T\]

他们之间的关系：
\[\left\{\begin{aligned}&z=Wx+b\\& \hat{y}=\mathrm{softmax}(z)=\frac{exp(z)}{1_c^Texp(z)} \end{aligned}\right.\]

cross-entropy error定义为：
\[ CE(z) = -y^Tlog(\hat{y}) \]

因为\(y\)是一个one-hot vector（即只有一个位置为1），假设\(y_k=1\)，那么上式等于\(-log(\hat{y}_k)=-log(\frac{exp(z_k)}{\sum\limits_i exp(z_i)})=-z_k+log(\sum\limits_i exp(z_i))\)

依据chain rule有：
\[ \begin{aligned}\frac{\partial CE(z)}{\partial W_{ij}}
&=tr\bigg(\big(\frac{\partial CE(z)}{\partial z}\big)^T\frac {\partial z}{\partial W_{ij}}\bigg)\\
&=tr\bigg( \big(\frac{\partial \hat{y}}{\partial z}\cdot\frac{\partial CE(z)}{\partial \hat{y}}\big)^T\frac {\partial z}{\partial W_{ij}} \bigg)\end{aligned}\]

注：这里我用了`Denominator layout`，因此链式法则是从右往左的。

我们一个一个来求。
\[\begin{equation}\begin{aligned}\frac{\partial \hat{y}}{\partial z}&=\frac{\partial ( \frac{exp(z)}{1_c^Texp(z)})}{\partial z}\\&= \frac{1}{1_c^Texp(z)}\frac{\partial exp(z)}{\partial z}+ \frac{\partial (\frac{1}{1_c^Texp(z)})}{\partial z}( exp(z) )^T\\&= \frac{1}{1_c^Texp(z)}diag(exp(z))-\frac{1}{(1_c^Texp(z))^2}exp(z)exp(z)^T\\&=diag(\frac{exp(z)}{1_c^Texp(z)})-\frac{exp(z)}{1_c^Texp(z)}\cdot (\frac{exp(z)}{1_c^Texp(z)})^T\\&=diag(\mathrm{ softmax}(z))- \mathrm{ softmax}(z) \mathrm{ softmax}(z)^T\\&=diag(\hat{y})-\hat{y}\hat{y}^T \end{aligned}\label{eq1}\end{equation}\]

注：上述求导过程使用了`Denominator layout`。

设$a=a( \boldsymbol{ x}),\boldsymbol{u}= \boldsymbol{u}( \boldsymbol{x}) \(，这里\) \boldsymbol{ x}\(特意加粗表示是列向量，\)a\(没加粗表示是一个标量函数，\) \boldsymbol{u}\(加粗表示是一个向量函数。在`Numerator layout`下，\)\frac{\partial a \boldsymbol{u}}{ \boldsymbol{x}}=a\frac{\partial \boldsymbol{u}}{\partial \boldsymbol{x}}+ \boldsymbol{u}\frac{\partial a}{\partial \boldsymbol{x}} \(，而在`Denominator layout`下，则为\)\frac{\partial a \boldsymbol{u}}{\partial \boldsymbol{x}}=a\frac{\partial \boldsymbol{u}}{\partial \boldsymbol{x}}+\frac{\partial a}{\partial \boldsymbol{x}} \boldsymbol{u}^T$，对比可知上述推导用的实际是`Denominator layout`。

以下推导均采用 Denominator layout，这样的好处是我们用梯度更新权重时不需要对梯度再转置。

\[\begin{equation}\frac{\partial CE(z)}{\partial \hat{y}}=\frac{\partial log(\hat{y})}{\partial \hat{y}}\cdot \frac{\partial (-y^Tlog(\hat{y}))}{\partial log(\hat{y})}=\big(diag(\hat{y})\big)^{-1}\cdot(-y)\label{eq2}\end{equation}\]
\(z\)的第\(k\)个分量可以表示为：\(z_k=\sum\limits_j W_{kj}x_j+b_k\)，因此
\[\begin{equation}\frac{\partial z}{\partial W_{ij}} =\begin{bmatrix}\frac{\partial z_1}{\partial W_{ij}}\\\vdots\\\frac{\partial z_c}{\partial W_{ij}}\end{bmatrix}=[0,\cdots, x_j,\cdots, 0]^T=x_j \vec{e}_i\label{eq3}\end{equation}\]

其中\(x_j\)是向量\(x\)的第\(j\)个元素，为标量，它出现在第\(i\)行。

综合\(\eqref{eq1},\eqref{eq2},\eqref{eq3}\)，我们有
\[ \begin{aligned}\frac{\partial CE(z)}{\partial W_{ij}}&=tr\bigg(\big( (diag(\hat{y})-\hat{y}\hat{y}^T)\cdot  (diag(\hat{y}))^{-1}  \cdot (-y) \big)^T\cdot x_j \vec{e}_i \bigg)\\&=tr\bigg(\big( \hat{y}\cdot (1_c^Ty)-y\big)^T\cdot x_j \vec{e}_i \bigg)\\&=(\hat{y}-y)^T\cdot x_j \vec{e}_i={err}_ix_j\end{aligned}\]

其中\({err}_i=(\hat{y}-y)_i\)表示残差向量的第\(i\)项

我们可以把上式改写为
\[ \frac{\partial CE(z)}{\partial W}=(\hat{y}-y)\cdot x^T \]

同理可得
\[ \frac{\partial CE(z)}{\partial b}=(\hat{y}-y) \]

那么在进行随机梯度下降的时候，更新式就是：
\[ \begin{aligned}&W \leftarrow W - \lambda (\hat{y}-y)\cdot x^T \\&b \leftarrow b - \lambda (\hat{y}-y)\end{aligned}\]

其中\(\lambda\)是学习率








