
# Taylor Formula与Gradient Descent - lpty的博客 - CSDN博客

2019年02月15日 15:14:00[lpty](https://me.csdn.net/sinat_33741547)阅读数：91



## Taylor Formula
泰勒公式，它的一般形式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190215100809346.jpg)
一般希望将复杂形式的函数用较为简单的方式来表示，另一种表述便是，用量上的复杂来解决质上的困难。
那为什么泰勒展开式是这种形式的？
上面说要用简单的形式表示复杂函数，那么如何选择表达式呢？类比切圆法，是不是可以用局部的线性近似来表示整体，假如有函数$y=x^3$，自变量的变化量为$Δx$，则：
$Δy=(x+Δx)^3 - x^3=3x^2Δx+3x(Δx^2)+(Δx^3)$
假设在x的足够小的邻域内，即$Δx$–>0，按照局部近似的想法，舍去后两项即$Δx$的高阶无穷小项，
$Δy=3x^2Δx$
也就是说在自变量足够小的邻域内，$Δy$是可以用$Δx$的线性函数来表示的，从复杂函数表示为线性函数，形式上就简单很多了，整理有：
$Δy=f(x_0+Δx)-f(x_0)≈f^′(x_0)*Δx$，$Δx=x-x_0$
即
$f(x)-f(x_0)=f^′(x_0)(x-x_0)$
$f(x)=f(x_0)+f^′(x_0)(x-x_0)$
上面提到这是舍掉了高阶无穷小的局部近似，假如希望能得到更高的精度，直觉的就是进行多阶的求导。上述公式称为一阶泰勒展开式，那么多阶泰勒展开式如何表示？
将上式再整理，$f(x)=f^{(0)}(x_0)(x-x_0)^{(0)}+f^{(1)}(x_0)(x-x_0)^{(1)}$，那么可以假设多阶泰勒展开式：
$f(x)=f^{(0)}(x_0)(x-x_0)^{(0)}+f^{(1)}(x_0)(x-x_0)^{(1)}+f^{(2)}(x_0)(x-x_0)^{(2)}+...=f(x)=\sum _{i=0} ^{m}f^{(i)}(x_0)(x-x_0)^{(i)}$
接下来进行验证，考虑对$f^{(m)}(x_0)(x-x_0)^{(m)}$，进行求导：
一阶导：$mf^{(m)}(x_0)(x-x_0)^{(m-1)}$,
二阶导：$m(m-1)f^{(m)}(x_0)(x-x_0)^{(m-2)}$
…
m阶导：$m!f^{(m)}(x_0)$
m+1阶导：0，由于$m!f^{(m)}(x_0)$已经是常数，常数的导数为0。
所以，$f^{(m)}(x_0)(x-x_0)^{(m)}$的m阶导就是$m!f^{(m)}(x_0)$，但是希望的是$f(x_0)$的m阶导是$f^{(m)}(x_0)$，观察规律直接将$!m$除掉即可，
$f(x)=\sum _{i=0} ^{m} \frac{f^{(i)}(x_0)}{!m}(x-x_0)^{(i)}$
按上述公式进行多次近似，经过同一点，切线相同，弯曲程度也相同了，近似的也就越精确了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
所以，泰勒展开就是用形式简单的多项式来近似在$x_0$邻域内的函数，展开越多近似程度越高。
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
## Jacobian Matrix
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)在向量分析中, 雅可比矩阵是一阶偏导数以一定方式排列成的矩阵, 其行列式称为雅可比行列式。
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)假设$F: Rn→Rm$是一个从欧式n维空间转换到欧式m维空间的函数. 这个函数由m个实函数组成:
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)$y_1(x_1,…,x_n), …, y_m(x_1,…,x_n)$.
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)这些函数的偏导数(如果存在)可以组成一个m行n列的矩阵, 这就是所谓的雅可比矩阵：
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)$\begin{bmatrix} \frac{\partial y_1}{\partial x_1} &amp; \cdots &amp; \frac{\partial y_1}{\partial x_n} \\ \vdots &amp; \ddots &amp; \vdots \\ \frac{\partial y_m}{\partial x_1} &amp; \cdots &amp; \frac{\partial y_m}{\partial x_n}  \end{bmatrix}$.
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)此矩阵表示为：${J_F}({x_1}, \ldots ,{x_n})$，或者$\frac{{\partial ({y_1}, \ldots ,{y_m})}}{{\partial ({x_1}, \ldots ,{x_n})}}$这个矩阵的第$i$行是由梯度函数的转置$y_i(i=1,…,m)$表示的.
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)如果$p$是$R_n$中的一点,$F$在$p$点可微分, 那么在这一点的导数由$J_F(p)$给出(这是求该点导数最简便的方法). 在此情况下, 由$F(p)$描述的线性算子即接近点$p$的$F$的最优线性逼近,$x$逼近于$p$:
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)$F({\bf{x}}) \approx F({\bf{p}}) + {J_F}({\bf{p}}) \cdot ({\bf{x}} – {\bf{p}})$
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
## Hessian Matrix
[
](https://img-blog.csdnimg.cn/20190215111932104.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)在数学中, 海森矩阵是一个自变量为向量的实值函数的二阶偏导数组成的方块矩阵, 此函数如下：
$f({x_1},{x_2} \ldots ,{x_n})$
如果f的所有二阶导数都存在, 那么$f$的海森矩阵即可表示为：
.
$\begin{bmatrix}  \frac{\partial^2 f}{\partial x_1^2} &amp; \frac{\partial^2 f}{\partial x_1\,\partial x_2} &amp; \cdots &amp; \frac{\partial^2 f}{\partial x_1\,\partial x_n} \\  \\  \frac{\partial^2 f}{\partial x_2\,\partial x_1} &amp; \frac{\partial^2 f}{\partial x_2^2} &amp; \cdots &amp; \frac{\partial^2 f}{\partial x_2\,\partial x_n} \\  \\  \vdots &amp; \vdots &amp; \ddots &amp; \vdots \\  \\  \frac{\partial^2 f}{\partial x_n\,\partial x_1} &amp; \frac{\partial^2 f}{\partial x_n\,\partial x_2} &amp; \cdots &amp; \frac{\partial^2 f}{\partial x_n^2}  \end{bmatrix}$
.
#### 正定矩阵
1、判定方法
若所有特征值均不小于零，则称为半正定。
若所有特征值均大于零，则称为正定。
2、海森矩阵的正定性
Hessian矩阵的正定性在判断优化算法可行性时非常有用，简单地说，海森矩阵正定，则
(1)函数的二阶偏导数恒 > 0
(2)函数的变化率（斜率）即一阶导数始终处于递增状态
(3)函数为凸
## Gradient Descent
在机器学习任务中，需要最小化损失函数$L(\theta)$，其中$\theta$是要求解的模型参数。
梯度下降法常用来求解这种无约束最优化问题，它是一种迭代方法：
选取初值 ，根据梯度方向不断迭代，更新$\theta$的值，进行损失函数的极小化。
迭代公式：$\theta^t = \theta^{t-1}+{\color{red} {\Delta \theta}}$;
将$L(\theta^t) 在 \theta^{t-1}$处进行一阶泰勒展开：
$L(\theta^t) =L(\theta^{t-1} + {\color{red} {\Delta\theta}}) \approx L(\theta^{t-1})+ L^{&\#x27;}(\theta^{t-1}) {\color{red} {\Delta\theta}}$
要使得$L(\theta^t) &lt;L(\theta^{t-1})$，可取：
${\color{red} {\Delta\theta}} =-\alpha L^{&\#x27;}(\theta^{t-1})$，则：$\theta^t = \theta^{t-1} -\alpha L^{&\#x27;}(\theta^{t-1})$
这里$\alpha$是 步长，也是常说的学习率，上式的意思便是沿着梯度方向对参数进行更新。
## Newton’s Method
牛顿法是二阶泰勒展开。
将$L(\theta^t)$在$\theta^{t-1}$处进行二阶泰勒展开：$L(\theta^t) \approx L(\theta^{t-1}) + L^{&\#x27;}(\theta^{t-1}) {\color{red} {\Delta\theta}} +\frac{1}{2} L^{&\#x27;&\#x27;}(\theta^{t-1}) {\color{red} {\Delta\theta ^2}}$
为了简化分析过程，假设参数是标量（即$\theta$只有一维），则可将一阶和二阶导数分别记为$g$和$h$:
$L(\theta^t) \approx L(\theta^{t-1}) + g {\color{red} {\Delta\theta}} +\frac{1}{2} h{\color{red} {\Delta\theta ^2}}$
要使得$L(\theta^t)$取得极小值，即让$g {\color{red} {\Delta\theta}} +\frac{1}{2} h{\color{red} {\Delta\theta ^2}}$极小，所以这里转化为求前述式子极小值的问题，那么：
令：$\frac{\partial \left( g {\color{red} {\Delta\theta}} +\frac{1}{2} h{\color{red} {\Delta\theta ^2}} \right)}{\partial {\color{red} {\Delta\theta}}} = 0$，得到
${\color{red} {\Delta\theta}} = -\frac{g}{h}$，故$\theta^t = \theta^{t-1}+{\color{red} {\Delta \theta}} = \theta^{t-1} - \frac{g}{h}$
由于泰勒展开只是近似相等, 这里求得的$\theta$只能表示当前值会更加接近要求的极值, 于是迭代求解就自然被推出了，将参数$\theta$推广到向量形式，迭代公式：$\theta^t = \theta^{t-1}-H^{-1}g$，这里$H$是 海森矩阵。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190215143939354.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
## 梯度下降与牛顿法
梯度下降是一阶收敛，牛顿法是二阶收敛，所以牛顿法就更快。
如果更通俗地说的话，比如你想找一条最短的路径走到一个盆地的最底部，梯度下降法每次只从你当前所处位置选一个坡度最大的方向走一步；而牛顿法在选择方向时，不仅会考虑坡度是否够大，还会考虑你走了一步之后，坡度是否会变得更大。所以，可以说牛顿法比梯度下降法看得更远一点，能更快地走到最底部。
根据wiki上的解释，从几何上说，牛顿法就是用一个二次曲面去拟合你当前所处位置的局部曲面，而梯度下降法是用一个平面去拟合当前的局部曲面，通常情况下，二次曲面的拟合会比平面更好，所以牛顿法选择的下降路径会更符合真实的最优下降路径。
一般认为牛顿法可以利用到曲线本身的信息, 比梯度下降法更容易收敛（迭代更少次数）, 如下图是一个最小化一个目标方程的例子, 红色曲线是利用牛顿法迭代求解, 绿色曲线是利用梯度下降法求解.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190215144015441.jpg)
## 参考
1、[https://www.zhihu.com/question/25627482](https://www.zhihu.com/question/25627482)
2、[https://blog.csdn.net/SoHardToNamed/article/details/80550935](https://blog.csdn.net/SoHardToNamed/article/details/80550935)
3、[http://jacoxu.com/jacobian矩阵和hessian矩阵/](http://jacoxu.com/jacobian%E7%9F%A9%E9%98%B5%E5%92%8Chessian%E7%9F%A9%E9%98%B5/)
4、[https://blog.csdn.net/qq_39521554/article/details/78895869](https://blog.csdn.net/qq_39521554/article/details/78895869)
5、[https://blog.csdn.net/baimafujinji/article/details/51167852](https://blog.csdn.net/baimafujinji/article/details/51167852)
6、[https://zhuanlan.zhihu.com/p/38186912](https://zhuanlan.zhihu.com/p/38186912)

