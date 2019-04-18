# 三、用scikit-learn求解一元线性回归问题 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:18:42[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：421












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_136_default_big.jpeg)



一元线性回归是最简单的一种模型，但应用广泛，比如简单地预测商品价格、成本评估等，都可以用一元线性模型，本节主要讲解scikit-learn一元线性回归的使用以及作图说明

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 一元线性回归

y=f(x)叫做一元函数，回归的意思就是根据已知数据复原某些值，线性回归(regression)就是用线性的模型做回归复原。

那么一元线性回归就是：已知一批(x,y)值来复原另外未知的值。

比如：告诉你(1,1),(2,2),(3,3)，那么问你(4,?)是多少，很容易复原出来(4,4)，这就是一元线性回归问题的求解

当然实际给你的数据可能不是严格线性，但依然让我们用一元线性回归来计算，那么就是找到一个最能代表已知数据的一元线性函数来做复原和求解



## scikit-learn的一元线性回归



编写代码scikit_learn_linear_model_demo.py如下：



```python
import numpy as np
from sklearn.linear_model import LinearRegression

x = [[1],[2],[3],[4],[5],[6]]
y = [[1],[2.1],[2.9],[4.2],[5.1],[5.8]]
model = LinearRegression()
model.fit(x, y)
predicted = model.predict([13])[0]
print predicted
```

执行结果：
`[[ 12.82666667]]`


这里面的model是一个estimator，它通过fit()方法来算出模型参数，并通过predict()方法来预测

LinearRegression的fit()方法就是学习这个一元线性回归模型：

y = a + bx



## 画一元线性图像



为了说明scikit-learn的回归效果，我们用matplotlib画图像试一下，先安装：
`pip install matplotlib`


编写plot_linear.py如下：

```python
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties
font = FontProperties()

plt.figure()
plt.title('this is title')
plt.xlabel('x label')
plt.ylabel('y label')
plt.axis([0, 25, 0, 25])
plt.grid(True)
x = [[1],[2],[3],[4],[5],[6]]
y = [[1],[2.1],[2.9],[4.2],[5.1],[5.8]]
plt.plot(x, y, 'k.')
plt.show()
```

执行后如下图：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/9e3f34d0ea66961155a1514a31d0f193707fa5d9.png)

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

事实上，我们可以把预测和作图两部分结合起来，如下：



```python
import numpy as np
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

x = [[1],[2],[3],[4],[5],[6]]
y = [[1],[2.1],[2.9],[4.2],[5.1],[5.8]]
model = LinearRegression()
model.fit(x, y)
x2 = [[0], [2.5], [5.3], [9.1]]
y2 = model.predict(x2)

plt.figure()
plt.title('linear sample')
plt.xlabel('x')
plt.ylabel('y')
plt.axis([0, 10, 0, 10])
plt.grid(True)
plt.plot(x, y, 'k.')
plt.plot(x2, y2, 'g-')
plt.show()
```



执行后画出图像如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/5817c1dfb64abacc3811b88ec69931bdd8ff85fc.png)

这里面plot()函数的第三个参数非常简单：k表示卡其色khaki，g表示绿色green，r表示红色red，'.'表示点，'-'表示线





## 其他相关用法



方差计算：方差用来衡量样本的分散程度，方差公式是

![](http://www.shareditor.com/uploads/media/my-context/0001/01/cc013f579d8c142c0b717d4b606ab212871a18da.png)

用numpy库的已有的方法是：

```python
np.var([1,2,3,4,5,6], ddof=1)
```

得出方差为：3.5

其中ddof是无偏估计校正技术



协方差计算：协方差表示两个变量的总体变化趋势，如果朝同方向变化则为正，朝反方向变化则为负，不相关则为0，协方差公式是：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/673891e505c7e1ea8594ff5c96bd7478168ef4d6.png)

用numpy库的已有的方法是：

```python
np.cov([1,2,3,4,5,6], [1,2.1,2.9,4.2,5.1,5.8])[0][1]
```

得出协方差为：3.43



事实上，方差/协方差就是线性方程的参数b：1.02

代入一个数据就可以得到a值：1 = a + 1.02 * 1，所以a=-0.02

因此回归方程就是y = -0.02 + 1.02 * x

因此预测x=13时，y=-0.02+1.02*13=13.24

这就是通过最小化成本函数来做回归



## 模型评估



R方度量方法可以评估线性回归效果，R方也叫确定系数，表示模型对现实数据的拟合程度。R方算法为：1-(残差平方和/样本总体平方和)

也可以用model.score()方法直接计算R方

```python
model.score(X_test, y_test)
```






