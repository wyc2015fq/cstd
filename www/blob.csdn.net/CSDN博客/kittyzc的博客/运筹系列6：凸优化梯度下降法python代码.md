# 运筹系列6：凸优化梯度下降法python代码 - kittyzc的博客 - CSDN博客





2018年08月15日 21:28:55[IE06](https://me.csdn.net/kittyzc)阅读数：397
所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)









# 1. 梯度下降法

梯度下降法与现在火热的机器学习、深度学习息息相关，相关的文章非常多。其基本思路可以理解为一种启发式搜索算法，每次迭代都寻找附近一定范围内的最小值。

基本步骤是：

使用拉格朗日法对一般的凸优化问题进行转化，假设问题为min $L(x)$，从一个初始解$x_0$出发，计算当前点的一阶导数$L'(x_0)$，然后按照一定的步长$\alpha$改变$x_0$的值，变为$x_1 = x_0 -\alpha L'(x_0)$，目标函数从$L(x_0)$变为$L(x_1)$。如此迭代下去，直至$L(x_i)$不再减小为止。如果$\alpha$足够小，不难看出终止条件就是$L'(x_i) = 0$

如果问题是非凸的，那么使用一阶导数还不够，很可能会陷入“鞍点”。此外，非凸问题常常有多个局部最优，梯度下降法作为一种“短视”算法，只能找到局部最优解。
# 2. 凸优化问题的梯度下降法python代码

令L为优化函数，LG为函数的导数，x为初始值，delta为步长，tol为精度，则梯度下降的代码为

```python
import numpy as np
def GD(L,LG,x,delta,tol):
    print(str(x)+":"+str(L(x)))
    xn = x - delta * LG(x)
    if L(x)>L(xn)+tol:
        GD(L,LG,xn,delta,tol)
```

求解的例子为min $L=(x_0-2)^2+(x_1-1)^2$，导数为[$2(x_0-2),2(x_1-1)$]，初始值为[0,0]，不难看出，最优解为0，在[2，1]处取得。代码为：

```
def L(x):
    return (x[0]-2)*(x[0]-2) + (x[1]-1)*(x[1]-1)

def LG(x):
    return np.array([2*(x[0]-2),2*(x[1]-1)])

GD(L,LG,np.array([0,0]),0.1,0.0001)
```

结果输出为：

```
[0 0]:5
[0.4 0.2]:3.2000000000000006
[0.72 0.36]:2.047999999999999
[0.976 0.488]:1.3107199999999999
[1.1808 0.5904]:0.8388607999999999
[1.34464 0.67232]:0.5368709119999999
[1.475712 0.737856]:0.34359738367999987
[1.5805696 0.7902848]:0.21990232555519995
[1.66445568 0.83222784]:0.1407374883553279
[1.73156454 0.86578227]:0.09007199254740988
[1.78525164 0.89262582]:0.05764607523034231
[1.82820131 0.91410065]:0.03689348814741904
[1.86256105 0.93128052]:0.023611832414348215
[1.89004884 0.94502442]:0.015111572745182858
[1.91203907 0.95601953]:0.009671406556917048
[1.92963126 0.96481563]:0.006189700196426927
[1.943705  0.9718525]:0.003961408125713221
[1.954964 0.977482]:0.0025353012004564663
[1.9639712 0.9819856]:0.0016225927682921462
[1.97117696 0.98558848]:0.0010384593717069706
[1.97694157 0.98847078]:0.0006646139978924637
[1.98155326 0.99077663]:0.00042535295865118083
[1.9852426 0.9926213]:0.00027222589353675247
```

停止迭代时，最优值为0.00027222589353675247，在[1.9852426 0.9926213]处取得，离最优点[2,1]很接近了。

# 3. 动量算法和Nesterov算法

固定步长的梯度下降法有很多潜在的问题，首先说一个反复震荡的问题。当某个函数在$x_1$轴上是二次函数，在$x_2$轴上是缓慢下降的一次函数时，梯度下降法会不停的在$x_1$轴上震荡。我们来看例子：

```python
def L(x):
    return (x[0]-2)*(x[0]-2) + abs(x[1])

def LG(x):
    if x[1]>0:
        return np.array([2*(x[0]-2),1)])
    else:
        return np.array([2*(x[0]-2),-1)])
```

```python
%matplotlib inline
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
from matplotlib import pyplot as plt
 
fig = plt.figure()
ax = Axes3D(fig)
x1=np.linspace(0,4,50)
x2=np.linspace(-2,2,50)
X1, X2 = np.meshgrid(x1, x2)
Z=L((X1,X2))
plt.xlabel('x1')
plt.ylabel('x2')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='rainbow')
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181202175641678.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)

这里引入**动量**的概念，即将过往的梯度以一定的折扣率加入本次的梯度中，减少不断正负变化的梯度导致的震荡问题。代码如下：
```python
import numpy as np
def MOMENTUM(L,LG,x,delta,discount,tol):
    pre_g = np.zeros_like(x)
    xn = x - delta * LG(x)
    while L(x)>L(xn)+tol:
        print(str(x)+":"+str(L(x)))
        x = xn
        pre_g = delta * LG(x) + pre_g * discount
        xn = x - pre_g
```

Nesterov方法有一些新的变化，差别主要在于先使用上一轮的梯度（带折扣）计算优化后的点大致在什么位置(代码中的xf)，然后在新的位置上计算梯度并在原位置(x)进行更新得到新的位置(xn)。动量折扣率discount一般设置为0.9。Nesterov代码如下：

```python
import numpy as np
def NESTEROV(L,LG,x,delta,discount,tol):
    pre_g = np.zeros_like(x)
    xn = x - delta * LG(x)
    while L(x)>L(xn)+tol:
        print(str(x)+":"+str(L(x)))
        x = xn
        xf = x - pre_g * discount * delta
        pre_g = LG(xf) + pre_g * discount
        xn = x - pre_g * delta
```

# 4. 自然梯度法

同样是上面提到的问题，自然梯度法的处理思路是：将参数改进修改为模型改进。

使用KL散度来定义概率模型之间的距离：$KL(f(x;\theta_1),f(x;\theta_2))$，则每一步的约束条件由$|\Delta|<\epsilon$变为KL$(f(w),f(w+\Delta))<\epsilon$。

这里使用近似公式：KL$(f(w),f(w+\Delta)) \simeq 0.5\Delta^T F \Delta$，其中$F = E(ll^T)$称为Fisher信息矩阵，$l = \nabla logf(x;w)$称为Score函数。

将约束条件替换掉之后，并使用拉格朗日乘子法求极值，得到$\Delta = -F^{-1}\nabla/\lambda$

与原先的$-\nabla$相比，多了一个Fisher阵的逆和一个系数。
# 5. 共轭梯度法

共轭梯度法的目标是：每一轮优化的方向和优化完的误差正交。






