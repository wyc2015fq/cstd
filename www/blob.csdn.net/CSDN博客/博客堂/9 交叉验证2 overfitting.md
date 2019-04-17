# 9 交叉验证2 overfitting - 博客堂 - CSDN博客





2017年07月02日 12:01:49[最小森林](https://me.csdn.net/u012052268)阅读数：281








# 交叉验证2 过拟合overfitting


- [交叉验证2 过拟合overfitting](#交叉验证2-过拟合overfitting)- [介绍](#介绍)




## 介绍

sklearn.learning_curve 中的 learning curve 可以很直观的看出我们的 model 学习的进度, 对比发现有没有 overfitting 的问题. 然后我们可以对我们的 model 进行调整, 克服 overfitting 的问题.

```python
from sklearn.learning_curve import learning_curve #学习曲线模块,看出错误率的曲线变化
from sklearn.datasets import load_digits #digits数据集，数字识别0-9
from sklearn.svm import SVC #Support Vector Classifier
import matplotlib.pyplot as plt #可视化模块
import numpy as np

# 加载digits数据集，分别为其手写体对应的8×8像素表示
# 数据集总共有1797个样本，每个样本由64个特征组成， 分别为其手写体对应的8×8像素表示，每个特征取值0~16。
digits = load_digits()
X = digits.data
y = digits.target

# 调用learning_curve函数，交叉验证。得到一连串的训练误差和test误差
train_sizes, train_loss, test_loss = learning_curve(
    SVC(gamma=0.001), X, y, cv=10, scoring='mean_squared_error',
    train_sizes=[0.1, 0.25, 0.5, 0.75, 1])

#平均每一轮所得到的平均方差(共5轮，分别为样本10%、25%、50%、75%、100%)
train_loss_mean = -np.mean(train_loss, axis=1)
test_loss_mean = -np.mean(test_loss, axis=1)

plt.plot(train_sizes, train_loss_mean, 'o-', color="r",
         label="Training")
plt.plot(train_sizes, test_loss_mean, 'o-', color="g",
        label="Cross-validation")

plt.xlabel("Training examples")
plt.ylabel("Loss")
plt.legend(loc="best")
plt.show()
```

![image](https://morvanzhou.github.io/static/results/sklearn/3_3_1.png)



