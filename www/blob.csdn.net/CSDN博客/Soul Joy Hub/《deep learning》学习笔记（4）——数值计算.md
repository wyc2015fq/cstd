# 《deep learning》学习笔记（4）——数值计算 - Soul Joy Hub - CSDN博客

2017年09月21日 09:57:47[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：696
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/78048424](http://blog.csdn.net/u011239443/article/details/78048424)

对于机器学习的问题，有一部分可以通过数学推导的方式直接得到用公式表达的解析解，但对绝大多数的问题来说，解析解是不存在的，需要使用迭代更新的方法求数值解。然而实数的精度是无限的，而计算机能够表达的精度是有限的，这就涉及到许多数值计算方法的问题。

# 4.1 上溢和下溢
- 由于计算机表达实数的精度的有限，在某些情况下许多复杂的复合运算中的四舍五入会导致一个接近0的小数变为0或者一个非常大的数被认为是无穷，这都会导致很严重的后果。
- softmax 函数经常用于预测与 Multinoulli 分布相关联的概率，定义为：

![](http://upload-images.jianshu.io/upload_images/1621805-55a3dc5f14c7c509.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 4.2 病态条件

病态条件：条件数指的是函数相对于输入的微小变化而变化的快慢程度，输入被轻微扰动而迅速改变的函数对于科学计算来说可能会出现问题。

# 4.3 基于梯度的优化方法
- 目标函数，损失函数(Objective function, Loss function)：在求解机器学习问题时需要最大化或者最小化的函数目标。

![](http://upload-images.jianshu.io/upload_images/1621805-faced6000ad80f34.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-6b24e56758d1923a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-8894a381c458efb4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- 全局最小点：在函数的定义域上取得全局最小值的点，这通常来说是许多机器学习问题所要寻找的优化目标点。对神经网络中问题的损失函数来说，可能存在很多的的局部最小点，它们与全局的最小点差距不大，应此通常也可以作为问题的解，下面是一个示例。

![](http://upload-images.jianshu.io/upload_images/1621805-de43bbeb2d79a280.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-f1692e5bd9627c67.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 当 f ′ (x) = 0 且 f ′′ (x) > 0 时，x 是一个局部极小点。同样，当 f ′ (x) = 0 且 f ′′ (x) < 0 时，x 是一个局部极大点。不幸的是，当 f ′′ (x) = 0 时测试是不确定的。在这种情况下，x 可以是一个鞍点或平坦区域的一部分。
- 当 Hessian 是正定的（所有特征值都是正的），则该临界点是局部极小点。当 Hessian 是负定的（所有特征值都是负的），这个点就是局部极大点。在多维情况下，实际上我们可以找到确定该点是否为鞍点的积极迹象（某些情况下）。如果 Hessian 的特征值中至少一个是正的且至少一个是负的，那么 x 是 f 某个横截面的局部极大点，却是另一个横截面的局部极小点。见图4.5中的例子。当所有非零特征值是同号的且至少有一个特征值是 0 时，这个检测就是不确定的。这是因为单变量的二阶导数测试在零特征值对应的横截面上是不确定的：

![](http://upload-images.jianshu.io/upload_images/1621805-7462192a34b2f9bd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-28d467ae1d3f811f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 利用梯度下降法求解的实例

### 问题

![](http://upload-images.jianshu.io/upload_images/1621805-5d98bdc66995ad59.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 求解

![](http://upload-images.jianshu.io/upload_images/1621805-8facdcfa180bec89.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

numpy实现上述过求解的代码：

```python
import numpy as np
import matplotlib.pyplot as plt

## 定义w和b
feature_num = 10
w_real = np.random.random(feature_num)
b_real = np.random.random()

## 生成训练数据
instance_num = 1000
X = np.random.uniform(-100,100,(feature_num, instance_num))
y = np.matmul(w_real, X) + b_real
y = y + np.random.random(y.shape)

## 初始化参数
w = np.random.random(feature_num)
b = np.random.random()

iter_time = 20
step_size = 0.0001
loss_value = []

## 迭代求解
for i in range(iter_time):
    delta = np.matmul(w, X) + b_real - y
    loss_value.append((delta*delta).mean())
    w = w - step_size*(np.matmul(delta, X.T))/instance_num
    b = b - step_size*delta.mean()
plt.clf
plt.plot(np.array(loss_value[1:]))
plt.show()
```

损失变化情况，如下图示：

![](http://upload-images.jianshu.io/upload_images/1621805-51fdd5e60ef142e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

```python
>>>w
array([ 0.79842335,  0.42003747,  0.04151777,  0.05549998,  0.71396875,
        0.60893848,  0.19337114,  0.42662969,  0.27833712,  0.63035355])
>>>w_real
array([ 0.79855966,  0.41969485,  0.04192018,  0.05457261,  0.7145736 ,
        0.60895623,  0.19295485,  0.42620365,  0.27826274,  0.63047412])
```

# 4.4 约束优化

有时候，在 x 的所有可能值下最大化或最小化一个函数 f(x) 不是我们所希望的。相反，我们可能希望在 x 的某些集合 S 中找 f(x) 的最大值或最小值。这被称为约束优化（constrained optimization）。

![](http://upload-images.jianshu.io/upload_images/1621805-dd35806d2692459b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 4.5 实例：线性最小二乘

![](http://upload-images.jianshu.io/upload_images/1621805-c8cea9568c8319eb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-2068647bd2487113.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

关于 x 对 Lagrangian 微分，我们得到方程:

![](http://upload-images.jianshu.io/upload_images/1621805-4c6fedb4f7382134.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](https://img-blog.csdn.net/20170921095704132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

