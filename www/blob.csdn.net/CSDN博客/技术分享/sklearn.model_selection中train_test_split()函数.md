# sklearn.model_selection中train_test_split()函数 - 技术分享 - CSDN博客

2018年11月03日 13:42:29[丿一叶秋丶](https://me.csdn.net/zhuqiang9607)阅读数：879


train_test_split()是sklearn.model_selection中的分离器函数，用于将数组或矩阵划分为训练集和测试集，函数样式为：

X_train, X_test, y_train, y_test = train_test_split(train_data, train_target, test_size, random_state，shuffle)

##### 参数解释：
- train_data：待划分的样本数据
- train_target：待划分的对应样本数据的样本标签
- test_size：1）浮点数，在0 ~ 1之间，表示样本占比（test_size = 0.3，则样本数据中有30%的数据作为测试数据，记入X_test，其余70%数据记入X_train，同时适用于样本标签）；2）整数，表示样本数据中有多少数据记入X_test中，其余数据记入X_train
- random_state：随机数种子，种子不同，每次采的样本不一样；种子相同，采的样本不变（random_state不取，采样数据不同，但random_state等于某个值，采样数据相同，取0的时候也相同，这可以自己编程尝试下，不过想改变数值也可以设置random_state = int(time.time())）
- shuffle：洗牌模式，1）shuffle = False，不打乱样本数据顺序；2）shuffle = True，打乱样本数据顺序

##### Python代码：

```
>>> import numpy as np
>>> from sklearn.model_selection import train_test_split
>>> X, y = np.arange(30).reshape((10, 3)), range(10)
>>> X_train, X_test ,y_train, y_test= train_test_split(X, y,test_size=0.3, rando
m_state = 20, shuffle=True)
>>> X_train
array([[15, 16, 17],
       [ 0,  1,  2],
       [ 6,  7,  8],
       [18, 19, 20],
       [27, 28, 29],
       [12, 13, 14],
       [ 9, 10, 11]])
>>> X_test
array([[21, 22, 23],
       [ 3,  4,  5],
       [24, 25, 26]])
>>> y_train
[5, 0, 2, 6, 9, 4, 3]
>>> y_test
[7, 1, 8]
```

