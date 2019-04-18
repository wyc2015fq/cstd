# 四、用scikit-learn求解多元线性回归问题 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:19:26[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：1485












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_136_default_big.jpeg)



一般情况下，一个因变量是和多个自变量有关的，比如一个商品的价格和原料价格、加工方法、上市时间、品牌价值等有关，也就是多元线性，本节介绍如何用scikit-learn解决多元线性回归问题

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 多元线性回归模型

方程：Y=Xβ 

求解多元线性回归问题就是求解β：

因为X不一定是方阵，所以不能直接β=X-1Y

两边同时乘以Xt，得到XtY=XtXβ

因为XtX是方阵，它的逆是(XtX)-1，所以两边同时乘(XtX)-1得到

(XtX)-1XtY=β



根据这个公式，我们自己设计一个例子，验证一下



设计二元一次方程：y=1+2x1+3x2

取样本为(1,1,1),(1,1,2),(1,2,1)，计算得y=(6,9,8)

注意：这里面常数项1相当于1*x0，只不过这里的x0永远取1



所以我们的

X = [[1,1,1],[1,1,2],[1,2,1]]

y = [[6],[9],[8]]



创建scikit_learn_multvariable_linear_model_demo.py内容如下：

```python
from numpy.linalg import inv
from numpy import dot, transpose

X = [[1,1,1],[1,1,2],[1,2,1]]
y = [[6],[9],[8]]

print dot(inv(dot(transpose(X),X)), dot(transpose(X),y))
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

执行结果为：

```
[[ 1.]
 [ 2.]
 [ 3.]]
```



这里面transpose是求转置，dot是求矩阵乘积，inv是求矩阵的逆



也可以用numpy的最小二乘函数直接计算出β

```python
from numpy.linalg import lstsq
print lstsq(X, y)[0]
```

这里的lstsq就是least square最小二乘的意思



## 用scikit-learn求解多元线性回归



知道了原理，我们就直接尝试一下scikit-learn的线性模型回归吧

```python
from sklearn.linear_model import LinearRegression

X = [[1,1,1],[1,1,2],[1,2,1]]
y = [[6],[9],[8]]

model = LinearRegression()
model.fit(X, y)
x2 = [[1,3,5]]
y2 = model.predict(x2)
print y2
```

执行结果为:
`[[ 22.]]`
刚好y=1+2x1+3x2=1+2*3+3*5=22




