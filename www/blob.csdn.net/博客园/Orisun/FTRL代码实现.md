# FTRL代码实现 - Orisun - 博客园







# [FTRL代码实现](https://www.cnblogs.com/zhangchaoyang/articles/6854175.html)





FTRL（Follow The Regularized Leader）是一种[优化方法](https://wenku.baidu.com/view/435728f032d4b14e852458fb770bf78a64293a53)，就如同SGD（Stochastic Gradient Descent）一样。这里直接给出用FTRL优化LR（Logistic Regression）的步骤：

![](https://images2015.cnblogs.com/blog/103496/201705/103496-20170514220808222-537301537.png)

其中$p_t=\sigma(X_t\cdot w)$是LR的预测函数，求出$p_t$的唯一目的是为了求出目标函数（在LR中采用交叉熵损失函数作为目标函数）对参数$w$的一阶导数$g$，$g_i=(p_t-y_t)x_i$。上面的步骤同样适用于FTRL优化其他目标函数，唯一的不同就是求次梯度$g$（次梯度是左导和右导之间的集合，函数可导--左导等于右导时，次梯度就等于一阶梯度）的方法不同。

下面的python代码把FTRL和LR进行了解耦：

```
1 # coding=utf-8
  2 __author__ = "orisun"
  3 
  4 import numpy as np
  5 
  6 
  7 class LR(object):
  8 
  9     @staticmethod
 10     def fn(w, x):
 11         '''决策函数为sigmoid函数
 12         '''
 13         return 1.0 / (1.0 + np.exp(-w.dot(x)))
 14 
 15     @staticmethod
 16     def loss(y, y_hat):
 17         '''交叉熵损失函数
 18         '''
 19         return np.sum(np.nan_to_num(-y * np.log(y_hat) - (1 - y) * np.log(1 - y_hat)))
 20 
 21     @staticmethod
 22     def grad(y, y_hat, x):
 23         '''交叉熵损失函数对权重w的一阶导数
 24         '''
 25         return (y_hat - y) * x
 26 
 27 
 28 class FTRL(object):
 29 
 30     def __init__(self, dim, l1, l2, alpha, beta, decisionFunc=LR):
 31         self.dim = dim
 32         self.decisionFunc = decisionFunc
 33         self.z = np.zeros(dim)
 34         self.n = np.zeros(dim)
 35         self.w = np.zeros(dim)
 36         self.l1 = l1
 37         self.l2 = l2
 38         self.alpha = alpha
 39         self.beta = beta
 40 
 41     def predict(self, x):
 42         return self.decisionFunc.fn(self.w, x)
 43 
 44     def update(self, x, y):
 45         self.w = np.array([0 if np.abs(self.z[i]) <= self.l1 else (np.sign(
 46             self.z[i]) * self.l1 - self.z[i]) / (self.l2 + (self.beta + np.sqrt(self.n[i])) / self.alpha) for i in xrange(self.dim)])
 47         y_hat = self.predict(x)
 48         g = self.decisionFunc.grad(y, y_hat, x)
 49         sigma = (np.sqrt(self.n + g * g) - np.sqrt(self.n)) / self.alpha
 50         self.z += g - sigma * self.w
 51         self.n += g * g
 52         return self.decisionFunc.loss(y, y_hat)
 53 
 54     def train(self, trainSet, verbos=False, max_itr=100000000, eta=0.01, epochs=100):
 55         itr = 0
 56         n = 0
 57         while True:
 58             for x, y in trainSet:
 59                 loss = self.update(x, y)
 60                 if verbos:
 61                     print "itr=" + str(n) + "\tloss=" + str(loss)
 62                 if loss < eta:
 63                     itr += 1
 64                 else:
 65                     itr = 0
 66                 if itr >= epochs:  # 损失函数已连续epochs次迭代小于eta
 67                     print "loss have less than", eta, " continuously for ", itr, "iterations"
 68                     return
 69                 n += 1
 70                 if n >= max_itr:
 71                     print "reach max iteration", max_itr
 72                     return
 73 
 74 
 75 class Corpus(object):
 76 
 77     def __init__(self, file, d):
 78         self.d = d
 79         self.file = file
 80 
 81     def __iter__(self):
 82         with open(self.file, 'r') as f_in:
 83             for line in f_in:
 84                 arr = line.strip().split()
 85                 if len(arr) >= (self.d + 1):
 86                     yield (np.array([float(x) for x in arr[0:self.d]]), float(arr[self.d]))
 87 
 88 if __name__ == '__main__':
 89     d = 4
 90     corpus = Corpus("train.txt", d)
 91     ftrl = FTRL(dim=d, l1=1.0, l2=1.0, alpha=0.1, beta=1.0)
 92     ftrl.train(corpus, verbos=False, max_itr=100000, eta=0.01, epochs=100)
 93     w = ftrl.w
 94     print w
 95 
 96     correct = 0
 97     wrong = 0
 98     for x, y in corpus:
 99         y_hat = 1.0 if ftrl.predict(x) > 0.5 else 0.0
100         if y == y_hat:
101             correct += 1
102         else:
103             wrong += 1
104     print "correct ratio", 1.0 * correct / (correct + wrong)
```

输出：

```
reach max iteration 100000
w= [  4.08813934   1.84596245  10.83446088   3.12315268]
correct ratio 0.9946
```

当把参数调为$\lambda_1=0,\lambda_2=0,\alpha=0.5,\beta=1$时，准确率能达到0.9976。

[train.txt](http://files.cnblogs.com/files/zhangchaoyang/train.txt.zip)文件前4列是特征，第5列是标签。内容形如：

```
-0.567811945258 0.899305436215 0.501926599477 -0.222973905568 1.0
-0.993964260114 0.261988294216 -0.349167046026 -0.923759536056 0.0
0.300707261785 -0.90855090557 -0.248270600228 0.879134142054 0.0
-0.311566995194 -0.698903141283 0.369841040784 0.175901270771 1.0
0.0245841670644 0.782128080056 0.542680482068 0.44897929707 1.0
0.344387543846 0.297686731698 0.338210312887 0.175049733038 1.0
```














