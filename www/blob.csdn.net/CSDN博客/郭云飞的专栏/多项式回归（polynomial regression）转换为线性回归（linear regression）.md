
# 多项式回归（polynomial regression）转换为线性回归（linear regression） - 郭云飞的专栏 - CSDN博客


2017年11月16日 17:26:09[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：4035


**一、介绍**
一元m次多项式回归方程：
![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D218/sign=4b2be08146086e066ea8384a3a097b5a/0b46f21fbe096b63059622e006338744eaf8acf7.jpg)
二元二次多项式回归方程：
![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D299/sign=a45621e3fc246b607f0eb57dd2f91a35/96dda144ad345982a8e89a2206f431adcbef841b.jpg)
多元多次的多项式回归方程较复杂，加之实际生产生活中一元m次多项式归回就已经能够解决了，所以略！

对于一元m次多项式回归方程，令：
![](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D188/sign=8db6d5a5b6315c6047956fe7b5b0cbe6/3ac79f3df8dcd100d7e98191788b4710b9122f0c.jpg)
则该一元m次多项式就转化为m元线性回归方程：
![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D218/sign=4b2be08146086e066ea8384a3a097b5a/0b46f21fbe096b63059622e006338744eaf8acf7.jpg)
因此，用多元线性函数的回归方法就可解决多项式回归问题！需要指出的是，在多项式回归分析中，检验回归系数![](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D16/sign=50db9c48ad86c9170c03563fc83d6fc6/29381f30e924b8996c10018864061d950a7bf656.jpg)是否显著，实质上就是判断自变量*x*的*i*次方项![](https://gss0.bdstatic.com/94o3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D16/sign=2c49d96efa1f3a295ec8d1c8982534d2/aa64034f78f0f7360fde34b10055b319ebc41359.jpg)对因变量*y*的影响是否显著。

对于二元二次多项式回归方程，令：
![](https://gss2.bdstatic.com/-fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D286/sign=9d072d50fd03918fd3d13ac2673c264b/359b033b5bb5c9eaebee7953df39b6003af3b365.jpg)
则该二元二次多项式函数就转化为五元线性回归方程：
![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D275/sign=af20c54adbc8a786ba2a4d095208c9c7/d50735fae6cd7b89146e5f33052442a7d9330e7b.jpg)

**二、一元m次多项式回归的最小二乘解**
![](https://img-blog.csdn.net/20171116164621444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
用矩阵表示他们的关系：
![](https://img-blog.csdn.net/20171116164724538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
用矩阵符号表示：
![](https://img-blog.csdn.net/20171116164814207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此处推导过程忽略（参考线性回归最小二乘解的推导过程，基本过程是对每一个参数求偏导，令偏导 = 0，解联立方程组即可），最小二乘法解：
![](https://img-blog.csdn.net/20171116165130295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**三、Python环境下利用sklearn库写的简单示例**

```python
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
import matplotlib.pyplot as plt
# seed
rng = np.random.RandomState(123)
# construct samples. give a x, generate y with noise
def genY(x):
	a0, a1, a2, a3, e = 0.1, -0.02, 0.03, -0.04, 0.05
	yr = a0 + a1*x + a2*(x**2) + a3*(x**3) + e
	y = yr + 0.03*rng.rand(1)
	return y
# plot
plt.figure()
plt.title('polynomial regression(sklearn)')
plt.xlabel('x')
plt.ylabel('y')
plt.grid(True)
x_tup = np.linspace(-1, 1, 30)
y = [genY(a) for a in x_tup]
print y
x = x_tup.reshape(-1,1)
y = np.array(y).reshape(-1,1)
plt.plot(x, y, 'k.')
qf = PolynomialFeatures(degree = 3)
qModel = LinearRegression()
qModel.fit(qf.fit_transform(x), y)
print '----'
print qf.get_params()
xp = np.linspace(-1, 2, 100)
yp = qModel.predict(qf.transform(xp.reshape(-1, 1)))
plt.plot(xp, yp, 'r-')
plt.show()
```
注释：PolynomialFeatures类的成员函数fit_transform根据自变量元数和指数次数（degree）转换成线性回归中的自变量，然后利用线性回归LinearRegression进行拟合。运行结果如下：
![](https://img-blog.csdn.net/20171116165850866?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**四、除了利用最小二乘直接解出参数的值外，也可以用梯度下降法最小化损失函数来训练出参数的值**

```python
#-*- coding:utf-8 -*-
import numpy as np
import tensorflow as tf
import matplotlib.pyplot as plt
rng = np.random.RandomState(123)
def genY(x):
    a0, a1, a2, a3, e = 0.1, -0.02, 0.03, -0.04, 0.05
    yr = a0 + a1*x + a2*(x**2) + a3*(x**3) + e
    y = yr + 0.03*rng.rand(1)
    return y
    
plt.figure()
plt.title('polynomial regression(tensorflow)')
plt.xlabel('x')
plt.ylabel('y')
plt.grid(True)
x = np.linspace(-1, 1, 30)
y = [genY(a) for a in x]
x = x.reshape(-1,1)
y = np.array(y).reshape(-1,1)
plt.plot(x, y, 'k.')
X = tf.placeholder('float')
Y = tf.placeholder('float')
W = tf.Variable([0.] * 4)
print W
def Model(x, w):
    terms = []
    for i in range(0, 4):
        term = tf.multiply(w[i], tf.pow(x, i))
        terms.append(term)
    rs = tf.add_n(terms)
    return rs
YModel = Model(X, W)
Cost = tf.reduce_sum(tf.square(Y - YModel))
LearnRate = 0.01
train_op = tf.train.GradientDescentOptimizer(LearnRate).minimize(Cost)
with tf.Session() as sess:
	Init = tf.global_variables_initializer()
	sess.run(Init)
	for i in range(0, 100):
		for (_x, _y) in zip(x, y):
			sess.run(train_op, feed_dict = {X: _x, Y: _y})
	print sess.run(W)
	xp = np.linspace(-1, 2, 100)
	yp = 0
	for i in range(0, 4):
		yp += sess.run(W)[i] * np.power(xp, i)
	plt.plot(xp, yp, 'g-')
	plt.show()
```
![](https://img-blog.csdn.net/20171116172526547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





