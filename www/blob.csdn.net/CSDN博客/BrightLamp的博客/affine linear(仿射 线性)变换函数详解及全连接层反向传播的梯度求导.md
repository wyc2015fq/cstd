
# affine-linear(仿射-线性)变换函数详解及全连接层反向传播的梯度求导 - BrightLamp的博客 - CSDN博客


2018年11月24日 21:58:34[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：779个人分类：[深度学习基础																](https://blog.csdn.net/oBrightLamp/article/category/8347008)
所属专栏：



## 摘要
Affine 仿射层, 又称 Linear 线性变换层, 常用于神经网络结构中的全连接层.
本文给出了 Affine 层的两种定义及相关的反向传播梯度.
## 相关
*配套代码, 请参考文章 :*
Python和PyTorch对比实现affine/linear(仿射/线性)变换函数及全连接层的反向传播
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. Affine 的一种定义
考虑一个输入向量 x, Affine 层的权重为 k 维向量 w, 偏置为标量 b, 则 :
$$
x = (x_1,x_2,x_3,\cdots,x_k)\\
\;\\
w = (w_1, w_2,w_3,\cdots,w_k)\\
\;\\
affine(x_i,w_i,b) = x_iw_i+b
$$
使用 X 表示 m 行 k 列的矩阵, 偏置为标量 b, 则一次仿射变换为 :
$$
a^T=affine(X,w,b)  =  Xw^T + b\\\;\\
a^T=
\begin{pmatrix}
x_{11}&amp;x_{12} &amp;x_{13}&amp;\cdots&amp;x_{1k}\\ 
x_{21}&amp;x_{22}&amp;x_{23}&amp;\cdots&amp;x_{2k}\\ 
x_{31}&amp;x_{32}&amp;x_{33}&amp;\cdots&amp;x_{3k}\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
x_{m1}&amp;x_{m2}&amp;x_{m3}&amp;\cdots&amp;x_{mk}
\end{pmatrix} 
\begin{pmatrix}
w_1\\ 
w_2\\ 
w_3\\ 
\vdots\\
w_k
\end{pmatrix} +b\\
\;\\
a= (a_1,a_2,a_3,\cdots,a_k)
$$
更一般的, 若使用 W 表示 n 行 k 列的矩阵, 偏置为向量 b , 则 n 次仿射变换为 :
$$
W_{n\times k} =\begin{pmatrix}
w_{11}&amp;w_{12} &amp;w_{13}&amp;\cdots&amp;w_{1k}\\ 
w_{21}&amp;w_{22}&amp;w_{23}&amp;\cdots&amp;w_{2k}\\ 
w_{31}&amp;w_{32}&amp;w_{33}&amp;\cdots&amp;w_{3k}\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
w_{n1}&amp;w_{n2}&amp;w_{n3}&amp;\cdots&amp;w_{nk}
\end{pmatrix}\\
\;\\
b_{1 \times n} = (b_1,b_2,b_3,\cdots,b_n)\\\;\\
A_{m\times n} = affine(X,W,b)  = X_{m\times k}W^T_{n\times k} + b_{1 \times n}
$$
使用求和符号表示 A 矩阵中的元素 :
$$
a_{ij} =\sum_{t=1}^{k} x_{it} \cdot  w_{jt} + b_j
$$
取其中一项展开作为示例 :
$$
a_{23} =\sum_{t=1}^{k} x_{2t} \cdot  w_{3t} + b_3= x_{21}w_{31}+x_{22}w_{32}+x_{23}w_{33}+\cdots+x_{2k}w_{3k}+ b_3
$$

## 2. 梯度的定义
三维XYZ空间中的梯度定义:
$$
\nabla e_{(3)} = \frac{\partial e}{\partial x}i+\frac{\partial e}{\partial y}j+\frac{\partial e}{\partial  z}k
$$
式中,$i, j, k$是三个两两相互垂直的单位向量, 或$i, j, k$是正交单位向量组, 或$i, j, k$是一组线性无关的单位向量, 这三种说法是等价的.
推广到$t$维向量空间$V$, 若$t$个向量$I_1, I_2, I_3,\cdots, I_t$是一组两两正交的单位向量, 或单位向量组$I_1, I_2, I_3,\cdots, I_t$线性无关, 那么, 该向量空间$V$中的梯度可定义为 :
$$
\nabla e_{(V)} = \frac{\partial e}{\partial x_1}I_1+\frac{\partial e}{\partial x_2}I_2+\frac{\partial e}{\partial  x_3}I_3+\cdots+\frac{\partial e}{\partial  x_t}I_t
$$
梯度的定义可以在 <高等数学> 中找到, 正交和线性无关的定义可以在 <线性代数> 中找到.
## 3. 反向传播中的梯度求导
若 X 矩阵经过 affine 层变换得到 A 矩阵, 往前 forward 传播得到误差值 error (标量 e ), 求 e 关于 X 的梯度:
$$
A_{m \times n} = X_{m\times k}{W_{n\times k}}^T + b_{1 \times n}\\
\;\\
e=forward(A)
$$

### 3.1 损失值 e 对 A 矩阵的梯度
首先, 我们说求梯度, 究竟是在求什么?
答 : 一个让损失值 e 变小的最快的方向.
比如, e 对 A 的梯度矩阵 :
$$
\frac{de}{dA} = \begin{pmatrix}
\partial e/ \partial a_{11}&amp;\partial e/ \partial a_{12}&amp;\partial e/ \partial a_{13}&amp;\cdots&amp; \partial e/ \partial a_{1n}\\ 
\partial e/ \partial a_{21}&amp;\partial e/ \partial a_{22}&amp;\partial e/ \partial a_{23}&amp;\cdots&amp; \partial e/ \partial a_{2n}\\ 
\partial e/ \partial a_{31}&amp;\partial e/ \partial a_{32}&amp;\partial e/ \partial a_{33}&amp;\cdots&amp; \partial e/ \partial a_{3n}\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
\partial e/ \partial a_{m1}&amp;\partial e/ \partial a_{m2}&amp;\partial e/ \partial a_{m3}&amp;\cdots&amp; \partial e/ \partial a_{mn}\\ 
\end{pmatrix}
$$
为了书写方便, 记 :
$$
\frac{\partial e}{\partial a_{ij}} = a_{ij}&\#x27;\\
\;\\
\nabla e_{(A)}= \frac{de}{dA} =
\begin{pmatrix}
a_{11}&\#x27;&amp; a_{12}&\#x27;&amp; a_{13}&\#x27;&amp;\cdots&amp; a_{1n}&\#x27;\\ 
a_{21}&\#x27;&amp; a_{22}&\#x27;&amp; a_{23}&\#x27;&amp;\cdots&amp; a_{2n}&\#x27;\\ 
a_{31}&\#x27;&amp; a_{32}&\#x27;&amp; a_{33}&\#x27;&amp;\cdots&amp; a_{3n}&\#x27;\\
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
a_{m1}&\#x27;&amp; a_{m2}&\#x27;&amp; a_{m3}&\#x27;&amp;\cdots&amp; a_{mn}&\#x27;
\end{pmatrix}
$$
所有的$a_{ij}&\#x27;$都是已知的, 是上游的 forward 函数帮我们算好的.
只要矩阵 A 中所有的元素按照这个矩阵等比例的更新, 那么就是使 e 值减少最快的方向.
梯度本身的定义并不是一个矩阵, 而是一个向量 :
$$
\nabla e_{(A)}= 
(a_{11}&\#x27;, a_{12}&\#x27;,\cdots, a_{21}&\#x27;, a_{22}&\#x27;,\cdots,a_{m1}&\#x27;, a_{m2}&\#x27;,\cdots, a_{mn}&\#x27;)
$$
这个写法和上面的矩阵写法是等价的.
利用矩阵求导的写法求梯度, 求的是方向导数, 或者单位向量的系数, 和普通的矩阵求导有区别.
### 3.2 A 矩阵的元素关于 X 的梯度
$$
A_{m \times n} = X_{m\times k}{W_{n\times k}}^T + b_{1 \times n}\\
$$
根据矩阵乘法行乘列的定义, 矩阵$X$和$W^T$中的第$j$列向量相乘, 将降维得到一个新的列向量, 作为矩阵 A 中的第$j$列向量, 即 :
$$
W_j=(w_{j1},w_{j2},w_{j3},\cdots,w_{jk})\\
\;\\
XW_j^T= \begin{pmatrix}
a_{1j}\\ 
a_{2j}\\ 
a_{3j}\\ 
\vdots\\
a_{mj}
\end{pmatrix}=A_{:,j}
$$
上面的$:,j$符号表示取矩阵中$j$列的所有行, 结果是一个列向量. 参考的是 numpy 的记法.
矩阵 A 中任意元素的梯度 :
$$
\frac{d a_{ij}}{dX} = 
\begin{pmatrix}
\partial a_{ij}/ \partial x_{11}&amp;\partial a_{ij}/ \partial x_{12}&amp;\partial a_{ij}/ \partial x_{13}&amp;\cdots&amp; \partial a_{ij}/ \partial x_{1k}\\ 
\partial a_{ij}/ \partial x_{21}&amp;\partial a_{ij}/ \partial x_{22}&amp;\partial a_{ij}/ \partial x_{23}&amp;\cdots&amp; \partial a_{ij}/ \partial x_{2k}\\ 
\partial a_{ij}/ \partial x_{31}&amp;\partial a_{ij}/ \partial x_{32}&amp;\partial a_{ij}/ \partial x_{33}&amp;\cdots&amp; \partial a_{ij}/\partial  x_{3k}\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
\partial a_{ij}/ \partial x_{m1}&amp;\partial a_{ij}/ \partial x_{m2}&amp;\partial a_{ij}/ \partial x_{m3}&amp;\cdots&amp; \partial a_{ij}/ \partial x_{mk}\\ 
\end{pmatrix}
$$
为了书写方便, 记 :
$$
\frac{\partial a_{ij}}{\partial x_{pq}} = x_{ij|pq}&\#x27;\\
\;\\
\nabla {a_{ij}}_{(X)}=\frac{d a_{ij}}{dX} = 
\begin{pmatrix}
x_{ij|11}&\#x27;&amp;x_{ij|12}&\#x27;&amp;x_{ij|13}&\#x27;&amp;\cdots&amp;x_{ij|1k}&\#x27;\\
x_{ij|21}&\#x27;&amp;x_{ij|22}&\#x27;&amp;x_{ij|23}&\#x27;&amp;\cdots&amp;x_{ij|2k}&\#x27;\\
x_{ij|31}&\#x27;&amp;x_{ij|32}&\#x27;&amp;x_{ij|33}&\#x27;&amp;\cdots&amp;x_{ij|3k}&\#x27;\\
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
x_{ij|m1}&\#x27;&amp;x_{ij|m2}&\#x27;&amp;x_{ij|m3}&\#x27;&amp;\cdots&amp;x_{ij|mk}&\#x27;\\
\end{pmatrix}
$$

### 3.3 关于 X 的反向传播
按照矩阵元素的定义 :
$$
a_{ij}= \sum_{t=1}^{k} x_{it}\cdot w_{jt} +b_j\\
\;\\
a_{ij}= x_{i1}w_{j1} +x_{i2}w_{j2} +\cdots+x_{iq}w_{jq} +\cdots+x_{ik}w_{jk} +b_j\\
\;\\
x_{ij|pq}&\#x27;=\frac{\partial a_{ij}}{\partial x_{pq}} =
\left\{
 \begin{array}{rr}
  w_{jq}&amp;  p = i\\
 0, &amp;  p \neq i
 \end{array}
\right.\\
$$
根据 <高等数学> 中介绍的复合函数求导法则, 知 :
$$
\frac {\partial e}{\partial x_{pq}} =\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} \frac {\partial e}{\partial a_{ij}}\frac {\partial a_{ij}}{\partial x_{pq}} =\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} a_{ij}&\#x27; x_{ij|pq}&\#x27;\\
$$
删除零项 :
$$
\frac {\partial e}{\partial x_{pq}}=\sum_{j =1}^{j =n} a_{pj}&\#x27;w_{jq}\\
\;\\
\frac {d e}{d X}=\begin{pmatrix}
\sum_{j =1}^{j =n} a_{1j}&\#x27;w_{j1}&amp;\sum_{j =1}^{j =n} a_{1j}&\#x27;w_{j2}&amp;\sum_{j =1}^{j =n} a_{1j}&\#x27;w_{j3}&amp;\cdots&amp;\sum_{j =1}^{j =n} a_{1j}&\#x27;w_{jk}\\\;\\
\sum_{j =1}^{j =n} a_{2j}&\#x27;w_{j1}&amp;\sum_{j =1}^{j =n} a_{2j}&\#x27;w_{j2}&amp;\sum_{j =1}^{j =n} a_{2j}&\#x27;w_{j3}&amp;\cdots&amp;\sum_{j =1}^{j =n} a_{2j}&\#x27;w_{jk}\\\;\\
\sum_{j =1}^{j =n} a_{3j}&\#x27;w_{j1}&amp;\sum_{j =1}^{j =n} a_{3j}&\#x27;w_{j2}&amp;\sum_{j =1}^{j =n} a_{3j}&\#x27;w_{j3}&amp;\cdots&amp;\sum_{j =1}^{j =n} a_{3j}&\#x27;w_{jk}\\
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
\sum_{j =1}^{j =n} a_{mj}&\#x27;w_{j1}&amp;\sum_{j =1}^{j =n} a_{mj}&\#x27;w_{j2}&amp;\sum_{j =1}^{j =n} a_{mj}&\#x27;w_{j3}&amp;\cdots&amp;\sum_{j =1}^{j =n} a_{mj}&\#x27;w_{jk}\\
\end{pmatrix}
$$
这个结果恰好满足矩阵乘法的定义, 分解成矩阵 :
$$
\frac {d e}{d X}=\begin{pmatrix}
a_{11}&\#x27;&amp; a_{12}&\#x27;&amp; a_{13}&\#x27;&amp;\cdots&amp; a_{1n}&\#x27;\\ 
a_{21}&\#x27;&amp; a_{22}&\#x27;&amp; a_{23}&\#x27;&amp;\cdots&amp; a_{2n}&\#x27;\\ 
a_{31}&\#x27;&amp; a_{32}&\#x27;&amp; a_{33}&\#x27;&amp;\cdots&amp; a_{3n}&\#x27;\\
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
a_{m1}&\#x27;&amp; a_{m2}&\#x27;&amp; a_{m3}&\#x27;&amp;\cdots&amp; a_{mn}&\#x27;
\end{pmatrix}
\begin{pmatrix}
w_{11}&amp;w_{12} &amp;w_{13}&amp;\cdots&amp;w_{1k}\\ 
w_{21}&amp;w_{22}&amp;w_{23}&amp;\cdots&amp;w_{2k}\\ 
w_{31}&amp;w_{32}&amp;w_{33}&amp;\cdots&amp;w_{3k}\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
w_{n1}&amp;w_{n2}&amp;w_{n3}&amp;\cdots&amp;w_{nk}
\end{pmatrix}
$$
所以, 损失值 e 对 X 的梯度矩阵为 :
$$
\frac {d e}{d X} =\nabla e_{(A)}W
$$
矩阵$\nabla e_{(A)}$已在前面求得.
### 3.4 关于 W 的反向传播
参考上例求解 :
$$
a_{ij}= \sum_{t=1}^{k} x_{it}\cdot w_{jt} +b_j\\
\;\\
a_{ij}= x_{i1}w_{j1} +x_{i2}w_{j2} +\cdots+x_{iq}w_{jq} +\cdots+x_{ik}w_{jk} +b_j\\
\;\\
w_{ij|pq}&\#x27;=\frac{\partial a_{ij}}{\partial w_{pq}} =
\left\{
 \begin{array}{rr}
  x_{iq} &amp; p = j \\
 0 &amp; p \neq j
 \end{array}
\right.\\\;\\
\frac {\partial e}{\partial w_{pq}} =
\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} \frac {\partial e}{\partial a_{ij}}\frac {\partial a_{ij}}{\partial w_{pq}} =\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} a_{ij}&\#x27; w_{ij|pq}&\#x27;\\
\;\\
\frac {\partial e}{\partial w_{pq}}=\sum_{i =1}^{i =m} a_{ip}&\#x27;x_{iq}\\
\;\\
\frac {d e}{d W}=
\begin{pmatrix}
\sum_{i =1}^{i =m} a_{i1}&\#x27;x_{i1}&amp;\sum_{i =1}^{i =m} a_{i1}&\#x27;x_{i2}&amp;\sum_{i =1}^{i =m} a_{i1}&\#x27;x_{i3}&amp;\cdots&amp;\sum_{i =1}^{i =m} a_{i1}&\#x27;x_{ik}\\
\;\\
\sum_{i =1}^{i =m} a_{i2}&\#x27;x_{i1}&amp;\sum_{i =1}^{i =m} a_{i2}&\#x27;x_{i2}&amp;\sum_{i =1}^{i =m} a_{i2}&\#x27;x_{i3}&amp;\cdots&amp;\sum_{i =1}^{i =m} a_{i2}&\#x27;x_{ik}\\
\;\\
\sum_{i =1}^{i =m} a_{i3}&\#x27;x_{i1}&amp;\sum_{i =3}^{i =m} a_{i3}&\#x27;x_{i2}&amp;\sum_{i =1}^{i =m} a_{i3}&\#x27;x_{i3}&amp;\cdots&amp;\sum_{i =1}^{i =m} a_{i3}&\#x27;x_{ik}\\
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
\sum_{i =1}^{i =m} a_{in}&\#x27;x_{i1}&amp;\sum_{i =3}^{i =m} a_{in}&\#x27;x_{in}&amp;\sum_{i =1}^{i =m} a_{in}&\#x27;x_{i3}&amp;\cdots&amp;\sum_{i =1}^{i =m} a_{in}&\#x27;x_{ik}\\
\end{pmatrix}\\
$$
这个结果恰好满足矩阵乘法的定义, 分解成矩阵 :
$$
\frac {d e}{d W}=
\begin{pmatrix}
a_{11}&\#x27;&amp; a_{21}&\#x27;&amp; a_{31}&\#x27;&amp;\cdots&amp; a_{m1}&\#x27;\\ 
a_{12}&\#x27;&amp; a_{22}&\#x27;&amp; a_{32}&\#x27;&amp;\cdots&amp; a_{m2}&\#x27;\\ 
a_{13}&\#x27;&amp; a_{23}&\#x27;&amp; a_{33}&\#x27;&amp;\cdots&amp; a_{m3}&\#x27;\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
a_{1n}&\#x27;&amp; a_{2n}&\#x27;&amp; a_{3n}&\#x27;&amp;\cdots&amp; a_{mn}&\#x27;\\ 
\end{pmatrix}
\begin{pmatrix}
x_{11}&amp;x_{12} &amp;x_{13}&amp;\cdots&amp;x_{1k}\\ 
x_{21}&amp;x_{22}&amp;x_{23}&amp;\cdots&amp;x_{2k}\\ 
x_{31}&amp;x_{32}&amp;x_{33}&amp;\cdots&amp;x_{3k}\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
x_{m1}&amp;x_{m2}&amp;x_{m3}&amp;\cdots&amp;x_{mk}
\end{pmatrix}
$$
所以, 损失值 e 对 W 的梯度矩阵为 :
$$
\frac {d e}{d W} =\nabla e_{(A)}^TX
$$
矩阵$\nabla e_{(A)}$已在前面求得.
### 3.5 关于 e 对 b 的梯度
参考上例求解 :
$$
a_{ij}= \sum_{t=1}^{k} x_{it}\cdot w_{jt} +b_j\\
\;\\
b_{ij|p}&\#x27;=\frac{\partial a_{ij}}{\partial b_{q}} =
\left\{
 \begin{array}{rr}
 1,&amp;  q = j\\
 0, &amp;  q \neq j
 \end{array}
\right.\\
\;\\
\frac {\partial e}{\partial b_{q}} =
\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} \frac {\partial e}{\partial a_{ij}}\frac {\partial a_{ij}}{\partial b_{q}} =\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} a_{ij}&\#x27; b_{ij|q}&\#x27;\\
\;\\
\frac {\partial e}{\partial b_{q}} =
\sum_{i = 1}^{i=m} a_{iq}&\#x27;\cdot 1 \\
\;\\
\frac {d e}{d b} = (\sum_{i = 1}^{i=m} a_{i1}&\#x27;,\sum_{i = 1}^{i=m} a_{i2}&\#x27;,\sum_{i = 1}^{i=m} a_{i3}&\#x27;, \cdots ,\sum_{i = 1}^{i=m} a_{im}&\#x27;)\\
$$
所以, 损失值 e 对 b 的梯度矩阵为 :
$$
\frac {de}{db}=sum(\nabla e_{(A)},\; axis=0)
$$
矩阵$\nabla e_{(A)}$已在前面求得. 式中的$axis=0$表示对矩阵的第一维求和, 参考的是 numpy 的记法.
## 4. Affine 的另一种定义
上文中, W 矩阵经过转置$W^T$后再参与 Affine 运算.
在目前流行的教材中, 将 W 直接进行 Affine 运算的定义也很多.
$$
A_{m\times n} = affine(X,W,b)  = X_{m\times k}W_{k\times n} + b_{1 \times n}
\;\\
a_{ij}= \sum_{t=1}^{k} x_{it}\cdot w_{tj} +b_j
$$

### 4.1 关于 X 的反向传播
$$
a_{ij}= x_{i1}w_{1j} +x_{i2}w_{2j} +\cdots+x_{iq}w_{qj} +\cdots+x_{ik}w_{kj} +b_j\\
\;\\
x_{ij|pq}&\#x27;=\frac{\partial a_{ij}}{\partial x_{pq}} =
\left\{
 \begin{array}{rr}
  w_{qj}&amp;  p = i\\
 0, &amp;  p \neq i
 \end{array}
\right.\\
$$

$$
\frac {\partial e}{\partial x_{pq}} =\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} \frac {\partial e}{\partial a_{ij}}\frac {\partial a_{ij}}{\partial x_{pq}} =\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} a_{ij}&\#x27; x_{ij|pq}&\#x27;\\
$$

$$
\frac {\partial e}{\partial x_{pq}}=\sum_{j =1}^{j =n} a_{pj}&\#x27;w_{qj}\\
\;\\
\frac {d e}{d X}=\begin{pmatrix}
\sum_{j =1}^{j =n} a_{1j}&\#x27;w_{1j}&amp;\sum_{j =1}^{j =n} a_{1j}&\#x27;w_{2j}&amp;\sum_{j =1}^{j =n} a_{1j}&\#x27;w_{3j}&amp;\cdots&amp;\sum_{j =1}^{j =n} a_{1j}&\#x27;w_{kj}\\\;\\
\sum_{j =1}^{j =n} a_{2j}&\#x27;w_{1j}&amp;\sum_{j =1}^{j =n} a_{2j}&\#x27;w_{2j}&amp;\sum_{j =1}^{j =n} a_{2j}&\#x27;w_{3j}&amp;\cdots&amp;\sum_{j =1}^{j =n} a_{2j}&\#x27;w_{kj}\\\;\\
\sum_{j =1}^{j =n} a_{3j}&\#x27;w_{1j}&amp;\sum_{j =1}^{j =n} a_{3j}&\#x27;w_{2j}&amp;\sum_{j =1}^{j =n} a_{3j}&\#x27;w_{3j}&amp;\cdots&amp;\sum_{j =1}^{j =n} a_{3j}&\#x27;w_{kj}\\
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
\sum_{j =1}^{j =n} a_{mj}&\#x27;w_{1j}&amp;\sum_{j =1}^{j =n} a_{mj}&\#x27;w_{2j}&amp;\sum_{j =1}^{j =n} a_{mj}&\#x27;w_{3j}&amp;\cdots&amp;\sum_{j =1}^{j =n} a_{mj}&\#x27;w_{kj}\\
\end{pmatrix}
$$

$$
\frac {d e}{d X}=\begin{pmatrix}
a_{11}&\#x27;&amp; a_{12}&\#x27;&amp; a_{13}&\#x27;&amp;\cdots&amp; a_{1n}&\#x27;\\ 
a_{21}&\#x27;&amp; a_{22}&\#x27;&amp; a_{23}&\#x27;&amp;\cdots&amp; a_{2n}&\#x27;\\ 
a_{31}&\#x27;&amp; a_{32}&\#x27;&amp; a_{33}&\#x27;&amp;\cdots&amp; a_{3n}&\#x27;\\
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
a_{m1}&\#x27;&amp; a_{m2}&\#x27;&amp; a_{m3}&\#x27;&amp;\cdots&amp; a_{mn}&\#x27;
\end{pmatrix}
\begin{pmatrix}
w_{11}&amp;w_{21} &amp;w_{31}&amp;\cdots&amp;w_{k1}\\ 
w_{12}&amp;w_{22}&amp;w_{32}&amp;\cdots&amp;w_{k2}\\ 
w_{13}&amp;w_{23}&amp;w_{33}&amp;\cdots&amp;w_{k3}\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
w_{1n}&amp;w_{2n}&amp;w_{3n}&amp;\cdots&amp;w_{kn}
\end{pmatrix}
$$

$$
\frac {d e}{d X} =\nabla e_{(A)}W^T
$$

### 4.2 关于 W 的反向传播
$$
a_{ij}= x_{i1}w_{1j} +x_{i2}w_{2j} +\cdots+x_{ip}w_{pj} +\cdots+x_{ik}w_{kj} +b_j\\
\;\\
w_{ij|pq}&\#x27;=\frac{\partial a_{ij}}{\partial w_{pq}} =
\left\{
 \begin{array}{rr}
  x_{ip} &amp; q = j \\
 0 &amp; q \neq j
 \end{array}
\right.\\\;\\
\frac {\partial e}{\partial w_{pq}} =
\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} \frac {\partial e}{\partial a_{ij}}\frac {\partial a_{ij}}{\partial w_{pq}} =\sum_{i = 1}^{i=m}\sum_{j =1}^{j =n} a_{ij}&\#x27; w_{ij|pq}&\#x27;\\
$$

$$
\frac {\partial e}{\partial w_{pq}}=\sum_{i =1}^{i =m} a_{iq}&\#x27;x_{ip}\\
\;\\
\frac {d e}{d W}=
\begin{pmatrix}
\sum_{i =1}^{i =m} a_{i1}&\#x27;x_{i1}&amp;\sum_{i =1}^{i =m} a_{i2}&\#x27;x_{i1}&amp;\sum_{i =1}^{i =m} a_{i3}&\#x27;x_{i1}&amp;\cdots&amp;\sum_{i =1}^{i =m} a_{in}&\#x27;x_{i1}\\
\;\\
\sum_{i =1}^{i =m} a_{i1}&\#x27;x_{i2}&amp;\sum_{i =1}^{i =m} a_{i2}&\#x27;x_{i2}&amp;\sum_{i =1}^{i =m} a_{i3}&\#x27;x_{i2}&amp;\cdots&amp;\sum_{i =1}^{i =m} a_{in}&\#x27;x_{i2}\\
\;\\
\sum_{i =1}^{i =m} a_{i1}&\#x27;x_{i3}&amp;\sum_{i =3}^{i =m} a_{i2}&\#x27;x_{i3}&amp;\sum_{i =1}^{i =m} a_{i3}&\#x27;x_{i3}&amp;\cdots&amp;\sum_{i =1}^{i =m} a_{in}&\#x27;x_{i3}\\
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
\sum_{i =1}^{i =m} a_{i1}&\#x27;x_{ik}&amp;\sum_{i =3}^{i =m} a_{i2}&\#x27;x_{ik}&amp;\sum_{i =1}^{i =m} a_{i3}&\#x27;x_{ik}&amp;\cdots&amp;\sum_{i =1}^{i =m} a_{in}&\#x27;x_{ik}\\
\end{pmatrix}\\
$$

$$
\frac {d e}{d W}=
\begin{pmatrix}
x_{11}&amp;x_{21} &amp;x_{31}&amp;\cdots&amp;x_{m1}\\ 
x_{12}&amp;x_{22}&amp;x_{31}&amp;\cdots&amp;x_{m2}\\ 
x_{13}&amp;x_{32}&amp;x_{33}&amp;\cdots&amp;x_{m3}\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
x_{1k}&amp;x_{2k}&amp;x_{3k}&amp;\cdots&amp;x_{mk}
\end{pmatrix}
\begin{pmatrix}
a_{11}&\#x27;&amp; a_{12}&\#x27;&amp; a_{13}&\#x27;&amp;\cdots&amp; a_{1n}&\#x27;\\ 
a_{21}&\#x27;&amp; a_{22}&\#x27;&amp; a_{23}&\#x27;&amp;\cdots&amp; a_{2n}&\#x27;\\ 
a_{31}&\#x27;&amp; a_{32}&\#x27;&amp; a_{33}&\#x27;&amp;\cdots&amp; a_{3n}&\#x27;\\
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
a_{m1}&\#x27;&amp; a_{m2}&\#x27;&amp; a_{m3}&\#x27;&amp;\cdots&amp; a_{mn}&\#x27;
\end{pmatrix}
$$

$$
\frac {d e}{d W} = X^T\nabla e_{(A)}
$$

### 4.3 关于 e 对 b 的梯度
同上:
$$
\frac {de}{db}=sum(\nabla e_{(A)},\; axis=0)
$$
全文完.

