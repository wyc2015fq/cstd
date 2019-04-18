# 【Python】SVM 线性可分 硬间隔最大化 - YZXnuaa的博客 - CSDN博客
2018年03月13日 10:42:01[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：268
1. 代码
from sklearn import svm
import numpy as np
import matplotlib.pyplot as plt
# np.random.seed(0)
x = np.r_[np.random.randn(20, 2) - [2, 2], np.random.randn(20, 2) + [2, 2]]  # 正态分布来产生数字,20行2列*2
print(x)
# y 的写法值得学习！
y = [0] * 20 + [1] * 20  # 20个class0，20个class1
# svm设置：
clf = svm.SVC(kernel='linear')
clf.fit(x, y)
# 获取w
w = clf.coef_[0]
print('w', w)
a = -w[0] / w[1]  # 斜率
# 画图划线
xx = np.linspace(-5, 5)  # (-5,5)之间x的值
yy = a * xx - (clf.intercept_[0]) / w[1]  # xx带入y，截距
# 画出与点相切的线
b = clf.support_vectors_[0]
yy_down = a * xx + (b[1] - a * b[0])
b = clf.support_vectors_[-1]
yy_up = a * xx + (b[1] - a * b[0])
print("W:", w)
print("a:", a)
print("\nsupport_vectors_:\n", clf.support_vectors_)
print("clf.coef_:", clf.coef_)
# 测试, 两个中括号！
for i in range(20):
    a = np.random.randn(1, 2) * 10
print(a)
    print('测试', clf.predict(a))
plt.figure(figsize=(8, 4))
plt.plot(xx, yy)
plt.plot(xx, yy_down)
plt.plot(xx, yy_up)
plt.scatter(clf.support_vectors_[:, 0], clf.support_vectors_[:, 1], s=80)
plt.scatter(x[:, 0], x[:, 1], c=y, cmap=plt.cm.Paired)  # [:，0]列切片，第0列
plt.axis('tight')
plt.show()
