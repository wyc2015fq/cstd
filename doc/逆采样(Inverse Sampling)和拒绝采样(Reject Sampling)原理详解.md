# 逆采样(Inverse Sampling)和拒绝采样(Reject Sampling)原理详解

 																				2018年12月05日 18:08:20 					[anshuai_aw1](https://me.csdn.net/anshuai_aw1) 						阅读数：480 										

 									

在蒙特卡罗方法中，有一个关键的问题需要解决，即如何基于概率密度函数去采的n个x的样本集。

逆采样(Inverse Sampling)和拒绝采样(Reject Sampling)就是用于解决这个问题的。

其中，对于常见的分布，如均匀分布，高斯分布，指数分布，t分布，F分布，Beta分布，Gamma分布等，可以采用逆采样的方法进行采样；不过很多时候，我们的x的概率分布不是常见的分布，这些分布的概率分布函数CDF  **不可逆**，因此没有办法用逆采样来采样，这意味着我们没法方便的得到这些非常见的概率分布的样本集。拒绝采样就是用来解决这个问题。

# 一、逆采样(Inverse Sampling)

我们知道，对于常见的均匀分布uniform(0,1)是非常容易采样样本的，一般通过线性同余发生器可以很方便的生成(0,1)之间的伪随机数样本。而其他常见的概率分布，无论是离散的分布还是连续的分布，它们的样本都可以通过uniform(0,1) 的样本转换而得。那么应该如何得到呢？这就是逆采样。

下面我们以指数分布为例，**说明如何通过均匀分布来采样服从指数分布的样本集**。指数分布的概率密度函数PDF为：
$$
pexp(x)=\begin{cases}
  λexp(−λx)& ,x≥0\\
  0&, x<0
 \end{cases} \tag 1
$$
那么它的概率分布函数CDF为：
$$
F(x)=\int_{-\infty}^x p(x)dx \tag 2
$$


下图为指数分布和均匀分布的CDF图。从左图上看，在x≥0的部分是一个单调递增的函数(在定义域上单调非减)，定义域和值域是$[0,+∞)→[0,1)$，在$p(x)$大的地方它增长快，反之亦然。 

![CSDN图标](https://ask.qcloudimg.com/http-save/yehe-781483/qy210rjd7c.jpeg?imageView2/2/w/1620)   

因为它是唯一映射的（在>0的部分，接下来我们只考虑这一部分），所以它的反函数可以表示为F−1(a)，a∈[0,1),值域为[0,+∞)

F−1(a)，a∈[0,1),值域为[0,+∞)

F(x)

F(x)的图)

因为F

F

F−1

F−1

 

x≤ya≤b⇒F(x)≤F(y)⇒F−1(a)≤F−1(b)(3)

x≤ya≤b⇒F(x)≤F(y)⇒F−1(a)≤F−1(b)(3)

利用反函数的定义，我们有：
 F−1(a)&lt;x,iff  a&lt;F(x)(4)

F−1(a)<x,iff  a<F(x)(4)

接下来，我们定义一下[0,1]

[0,1]

 

P⎛⎝⎜a≤x⎞⎠⎟=H⎛⎝⎜x⎞⎠⎟=⎧⎩⎨⎪⎪1x0,x≥1,0≤x≤1,x&lt;0(5)

P(a≤x)=H(x)=⎩⎪⎨⎪⎧1x0,x≥1,0≤x≤1,x<0(5)

根据公式（4）和公式（5）,有：
 P(F−1(a)≤x)=P(a≤F(x))=H(F(x))(6)

P(F−1(a)≤x)=P(a≤F(x))=H(F(x))(6)

因为F(x)

F(x)

[0,1)

[0,1)

 

P(F−1(a)≤x)=H(F(x))=F(x)(7)

P(F−1(a)≤x)=H(F(x))=F(x)(7)

据F(x)

F(x)

F−1(a)

F−1(a)

我们通过F

F的反函数将一个0到1均匀分布的随机数转换成了符合exp分布的随机数

以上推导对于CDF可逆的分布都是一样的

 

F−1exp(a)=−1λ∗log(1−a)(8)

Fexp−1(a)=−λ1∗log(1−a)(8)

具体的映射关系可以看下图(a)，我们从 y 轴 0-1 的均匀分布样本（绿色）映射得到了服从指数分布的样本（红色）。

 ![CSDN图标](https://ask.qcloudimg.com/http-save/yehe-781483/vx4sybdwef.jpeg?imageView2/2/w/1620)  

我们写一点代码来看看效果:

```
def sampleExp(Lambda = 2,maxCnt = 50000):
    ys = []
    standardXaxis = []
    standardExp = []
    for i in range(maxCnt):
        u = np.random.random()
        y = -1/Lambda*np.log(1-u) #F-1(X)
        ys.append(y)
    for i in range(1000):
        t = Lambda * np.exp(-Lambda*i/100)
        standardXaxis.append(i/100)
        standardExp.append(t)
    plt.plot(standardXaxis,standardExp,'r')
    plt.hist(ys,1000,normed=True)
    plt.show()

sampleExp()
1234567891011121314151617
```

**最后绘制出来的直方图可以看出来就是 exp 分布图**，见上图(b)。可以看到随着采样数量的变多，概率直方图和真实的 CDF 就越接近。

以上就是逆采样的过程。我们的结论是：因为CDF是单调函数（累积的概率只能越来越大，直到为1），因此，只要某分布的CDF**可逆**，那么就可以通过均匀分布来采样服从该分布的样本集。

# 二、拒绝采样(Reject Sampling)

## 2.1 原理

对于那些复杂的不可求逆的分布，拒绝采样就是针对此类复杂问题的一种随机采样方法。

我们以求圆周率π

π

π

π

1×1

1×1

πˆ

π^

πˆ

π^会越精准。

 ![CSDN图标](https://ask.qcloudimg.com/http-save/yehe-781483/un1evvmva8.jpeg?imageView2/2/w/1620)  

上面这个例子里说明一个问题，我们想求一个空间里均匀分布的集合面积，可以尝试**在更大范围内按照均匀分布随机采样，如果采样点在集合中，则接受，否则拒绝**。最后的接受概率就是集合在”更大范围“的面积占比。

接下来，我们来形式化地说明拒绝采样。

给定一个概率分布p(z)=1Zpp˜(z)

p(z)=Zp1p~(z)

p˜(z)

p~(z)

Zp

Zp

p(z)

p(z)

参考分布(proposal distribution)

q(x)

q(x)

k

k

z

z

kq(z)≥p˜(z)

kq(z)≥p~(z)

p˜(z)

p~(z)

kq(z)

kq(z)。

 ![CSDN图标](https://images2015.cnblogs.com/blog/1042406/201703/1042406-20170327143755811-993574578.png)  

在每次采样中，首先从q(z)

q(z)

z0

z0

[0,kq(z0)]

[0,kq(z0)]

u0

u0

u0&lt;p˜(z0)

u0<p~(z0)

p(z)

p(z)

在x=z0

x=z0这条线上，从[0,kq(z0)]

[0,kq(z0)]均匀采样中一个值，如果这个值小于p˜(z0)

p~(z0)，即这个均匀采样的这个值落在了p˜(z0)

p~(z0)的下方，我们就接受z0

**z0这个采样值。**

我们知道，每次采样的接受概率计算如下：
 p(accept)=∫p˜(z)kq(z)q(z)dz=1k∫p˜(z)dz

p(accept)=∫kq(z)p~(z)q(z)dz=k1∫p~(z)dz

所以，为了提高接受概率，防止舍弃过多的采样值而导致采样效率低下，k

k

kq(z)≥p˜(z)

kq(z)≥p~(z)的基础上**尽可能小**。

拒绝采样问题可以这样理解，p˜(z)

p~(z)

x

x

kq(z)

kq(z)

x

x

kq(z)

kq(z)

x

x

k

k

k

k

k×1

k×1

p˜(z)

p~(z)

x

x

k

k

p(z)

p(z)

[0,kq(z0)]

[0,kq(z0)]

p˜(z)

p~(z)

p(z)

p(z)分布的点。

## 2.2 实验

我们写一段代码来看看拒绝采样是如何对复杂分布进行采样的。假设，我们要采样的分布是：
 p˜(z)=0.3exp(−(z−0.3)2)+0.7exp(−(z−2)2/0.3)

p~(z)=0.3exp(−(z−0.3)2)+0.7exp(−(z−2)2/0.3)

其归一化常数Zp≈1.2113

Zp≈1.2113

q(z)=Gassian(1.4,1.2)

q(z)=Gassian(1.4,1.2)

kq(z)≥p˜(z)

kq(z)≥p~(z)。python代码如下：

```python
import numpy as np
import matplotlib.pyplot as plt

def f1(x):
    return (0.3*np.exp(-(x-0.3)**2) + 0.7* np.exp(-(x-2.)**2/0.3))/1.2113
x = np.arange(-4.,6.,0.01)
plt.plot(x,f1(x),color = "red")

size = int(1e+07)
sigma = 1.2
z = np.random.normal(loc = 1.4,scale = sigma, size = size)
qz = 1/(np.sqrt(2*np.pi)*sigma)*np.exp(-0.5*(z-1.4)**2/sigma**2)
k = 2.5
#z = np.random.uniform(low = -4, high = 6, size = size)
#qz = 0.1
#k = 10
u = np.random.uniform(low = 0, high = k*qz, size = size)

pz =  0.3*np.exp(-(z-0.3)**2) + 0.7* np.exp(-(z-2.)**2/0.3)
sample = z[pz >= u]
plt.hist(sample,bins=150, normed=True, edgecolor='black')
plt.show()
12345678910111213141516171819202122
```

得到的采样分布如下图：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120517531966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Fuc2h1YWlfYXcx,size_16,color_FFFFFF,t_70)

可以看到，采样结果完全符合原分布。另外如果我们把参考分布换为均匀分布(代码中z,q,k换为注释部分)，仍然得到较好的采样结果，如下图:
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205180629106.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Fuc2h1YWlfYXcx,size_16,color_FFFFFF,t_70)
 可以看到，采样结果也是完全符合原分布。

以上的实验说明了拒绝采样的有效性。

## 2.3 注意事项

通过在2.1节的分析，我们知道：**必须知道复杂分布的概率密度函数PDF，才可以进行拒绝采样**。然而，在现实情况中：

1）对于一些二维分布p(x,y)

p(x,y)

p(x∣y)

p(x∣y)

p(y∣x)

p(y∣x)

p(x,y)

p(x,y)的一般形式，这时我们无法用拒绝采样得到其样本集。

2）对于一些高维的复杂非常见分布p(x1,x2,...,xn)

p(x1,x2,...,xn)

q(x)

q(x)

k

k非常困难。

因此，实际上，我们仍然要找到一种方法可以解决如何方便得到各种复杂概率分布的对应的采样样本集的问题。马尔科夫链有能力帮助找到这些复杂概率分布的对应的采样样本集。而这也是MCMC采样的基础，我们回头会讲解。

# 参考文献

【1】[拒绝采样(reject sampling)原理详解](https://blog.csdn.net/jteng/article/details/54344766)

【2】[采样方法（一）](https://blog.csdn.net/dark_scope/article/details/70992266)

【3】[蒙特卡洛采样之拒绝采样（Reject Sampling）](https://blog.csdn.net/baimafujinji/article/details/51407703)

【4】[一文了解采样方法](https://cloud.tencent.com/developer/article/1162169)