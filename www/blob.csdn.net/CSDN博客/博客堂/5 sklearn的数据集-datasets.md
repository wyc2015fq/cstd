# 5 sklearn的数据集-datasets - 博客堂 - CSDN博客





2017年06月30日 10:25:43[最小森林](https://me.csdn.net/u012052268)阅读数：6842








# sklearn的数据集-datasets


- [sklearn的数据集-datasets](#sklearn的数据集-datasets)- [sklearn 强大数据库](#1-sklearn-强大数据库)
- [文档介绍](#2-文档介绍)- [1 经典数据](#21-经典数据)
- [2 构造数据](#22-构造数据)

- [例子1房价](#3-例子1房价)
- [例子2创建虚拟数据并可视化](#4-例子2创建虚拟数据并可视化)




## 1 sklearn 强大数据库

data sets，有很多有用的，可以用来学习算法模型的数据库。 
**eg**: boston 房价, 糖尿病, 数字, Iris 花。

主要有两种： 

- 封装好的经典数据。**eg**: boston 房价, 糖尿病, 数字, Iris 花。在代码中以“load”开头。 

- 自己设计参数，然后生成的数据，例如用来训练线性回归模型的数据（强大）。在代码中以“make”开头
## 2 文档介绍

### 2.1 经典数据

![image](https://morvanzhou.github.io/static/results/sklearn/2_3_1.png)

例如，点击进入 boston 房价的数据，可以看到 sample 的总数，属性，以及 label 等信息。

![image](https://morvanzhou.github.io/static/results/sklearn/2_3_2.png)

### 2.2 构造数据

如果是自己生成数据，按照函数的形式，输入 sample，feature，target 的个数等等。

```python
sklearn.datasets.make_regression(n_samples=100, n_features=100, n_informative=10, n_targets=1, bias=0.0, effective_rank=None, tail_strength=0.5, noise=0.0, shuffle=True, coef=False, random_state=None)[source]
```

## 3 例子1，房价

```python
# 导入满满的数据库
from sklearn import datasets
# 分割数据的模块，把数据集分为训练集和测试集
from sklearn.cross_validation import train_test_split
# 导入 线性回归 方法
from sklearn.linear_model import LinearRegression


# 载入数据
loaded_data = datasets.load_boston()
# 获取数据的输入与输出,sklearn很规则的吧数据分为了data（输入）与target（输出）两部分
data_X = loaded_data.data
data_y = loaded_data.target
# 将数据集分割成 训练集 与 测试集，切顺序是打乱的。其中测试集占30%
X_train,X_test,y_train,y_test = train_test_split(data_X,data_y,test_size=0.3)



# 创建模型，可以直接用默认值去建立 model，默认值也不错，也可以自己改变参数使模型更好。
model = LinearRegression()
# 用 training data 去训练模型
model.fit(X_train, y_train)


# 再打印出预测值，这里用 X 的前 4 个来预测，同时打印真实值，作为对比。
print(model.predict(X_test[:4, :]))
print(y_test[:4])

'''
结果如下：

[ 19.56926228  33.20448769  31.89484639  39.34984426]
[ 18.6  33.2  30.3  43.5]
'''
```

## 4 例子2：创建虚拟数据并可视化

```python
# 导入满满的数据库，这里用它的第二个方面：构造数据
from sklearn import datasets
# 导入 线性回归 方法
from sklearn.linear_model import LinearRegression
# 画图工具
import matplotlib.pyplot as plt


# 构造用于回归的数据make_regression
# 参数的意思：100个例子，1种特征，1种输出，噪声的大小为5
X,y = datasets.make_regression(n_samples=100,n_features=1,n_targets=1,noise=2)

# 图像化我们刚刚创建的数据
plt.plot(X,y,'o')
plt.show()
```

![image](https://morvanzhou.github.io/static/results/sklearn/2_3_3.png)

可以看到用函数生成的 Linear Regression 用的数据。

noise 越大的话，点就会越来越离散，例如 noise 由 10 变为 50.

![image](https://morvanzhou.github.io/static/results/sklearn/2_3_4.png)




