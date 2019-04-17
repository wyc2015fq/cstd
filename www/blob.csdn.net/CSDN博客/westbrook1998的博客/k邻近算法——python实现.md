# k邻近算法——python实现 - westbrook1998的博客 - CSDN博客





2018年08月01日 15:20:01[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：168








> 
K最近邻(k-Nearest Neighbor，KNN)分类算法，是一个理论上比较不成熟的方法，也是最简单的机器学习算法之一。该方法的思路是：如果一个样本在特征空间中的k个最相似(即特征空间中最邻近)的样本中的大多数属于某一个类别，则该样本也属于这个类别。


代码：

```python
import numpy as np
from math import sqrt
from collections import Counter


class KNNClassfier:

    def __init__(self, k):
        """
        Initialize the kNN classifier
        :param k:
        """
        if k <= 1:
            raise RuntimeError("k must be valid (>1)")

        self.k = k
        self._x_train = None
        self._y_train = None

    def fit(self, x_train, y_train):
        """
        Train the kN N classifier according to the training set
        :param x_train:
        :param y_train:
        :return:
        """
        if x_train.shape[0] != y_train.shape[0]:
            raise RuntimeError("the size of x_train must be equal to the size of y_train")
        if self.k > x_train.shape[0]:
            raise RuntimeError("the size of x_train must be as least k")

        self._x_train = x_train
        self._y_train = y_train
        return self

    def predict(self, x_predict):
        """
        return result data based on test set
        :param x_predict:
        :return:
        """
        if self._x_train is None or self._y_train is None:
            raise RuntimeError("must fit before predict")
        if x_predict.shape[1] != self._x_train.shape[1]:
            raise RuntimeError("the feature number of x_predict must be equal to x_train")
        y_predict = np.array([self._predict(x) for x in x_predict])
        return y_predict

    def _predict(self, x):
        """
        return the predicted value of a single test data
        :param x:
        :return:
        """
        if x.shape[0] != self._x_train.shape[1]:
            raise RuntimeError("the feature number of x must be equal to x_train")
        distances = [sqrt(np.sum(x_train - x) ** 2) for x_train in self._x_train]
        nearest = np.argsort(distances)
        topK_y = [self._y_train[i] for i in nearest[:self.k]]
        votes = Counter(topK_y)
        return votes.most_common()[0][0]

    def __str__(self):
        """
        print string
        :return:
        """
        return "KNN(k=%d)" % self.k


if __name__ == "__main__":
    x_train = np.array([[3.393533211, 2.331273381],
                        [3.110073483, 1.781539638],
                        [1.343808831, 3.368360954],
                        [3.582294042, 4.679179110],
                        [2.280362439, 2.866990263],
                        [7.423436942, 4.696522875],
                        [5.745051997, 3.533989803],
                        [9.172168622, 2.511101045],
                        [7.792783481, 3.424088941],
                        [7.939820817, 0.791637231]
                        ])
    y_train = np.array([0, 0, 0, 0, 0, 1, 1, 1, 1, 1])
    x_test = np.array([
        [8.093607318, 3.365731514],
        [7.254879955, 8.745666941]
    ])
    k = 6

    KNN = KNNClassfier(k)
    KNN.fit(x_train, y_train)
    ans = KNN.predict(x_test)
    print(ans)
```



