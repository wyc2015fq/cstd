# 【分类】KNN分类算法之Python实现 - zkq_1986的博客 - CSDN博客





2018年10月16日 13:50:25[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：57








KNN称为K最近邻。对于待分类数据，它先计算出与其最相近的K个的样本，然后判断这K个样本中最多的类标签，并将待分类数据标记为这个最多的类标签。

python样例代码：

```python
import numpy as np
from sklearn.neighbors import KNeighborsClassifier as kNN

K = 3

train_mat = np.array([[1, 2, 3], [2, 3, 5], [55, 33, 66], [55, 33, 66]])
label = np.array([0, 0, 1, 1])
neigh = kNN(n_neighbors=K, algorithm='auto', weights='distance', n_jobs=1)
neigh.fit(train_mat, label)
test_mat = np.array([[2, 3, 4], [51, 32, 24]])
res = neigh.predict(test_mat)
print(res)
```

输出：

[0  1]



