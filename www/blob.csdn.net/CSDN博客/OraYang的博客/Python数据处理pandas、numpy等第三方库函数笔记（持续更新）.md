
# Python数据处理pandas、numpy等第三方库函数笔记（持续更新） - OraYang的博客 - CSDN博客

2017年09月11日 11:15:17[OraYang](https://me.csdn.net/u010665216)阅读数：476所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



## 说明
因为在平时学习中，对于pandas、numpy等python库的一些函数用法时常忘记，特在此做个汇总与整理，便于下次查找~
## numpy
nonzero(a)
：返回非零的数组下标
举例：
>>x = np.array([[1,0,0], [0,2,0], [1,1,0]])
>>x
array([[1, 0, 0],
[0, 2, 0],
[1, 1, 0]])
>>np.nonzero(x)
(array([0, 1, 2, 2], dtype=int64), array([0, 1, 0, 1], dtype=int64))
in1d
:Test whether each element of a 1-D array is also present in a second array.
Returns a boolean array the same length as ar1 that is True where an element of ar1 is in ar2 and False otherwise.
举例：
>> test = np.array([0, 1, 2, 5, 0])
>> states = [0, 2]
>> mask = np.in1d(test, states)
>> mask
array([ True, False,  True, False,  True], dtype=bool)
>> test[mask]
array([0, 2, 0])
>> mask = np.in1d(test, states, invert=True)
>> mask
array([False,  True, False,  True, False], dtype=bool)
>> test[mask]
array([1, 5])
train_test_split

sklearn.model_selection.train_test_split(*arrays, **options)
举例：

>> import numpy as np
>> from sklearn.model_selection import train_test_split
>> X, y = np.arange(10).reshape((5, 2)), range(5)
>> X
array([[0, 1],
[2, 3],
[4, 5],
[6, 7],
[8, 9]])
>> list(y)
[0, 1, 2, 3, 4]
>> X_train, X_test, y_train, y_test = train_test_split(
…     X, y, test_size=0.33, random_state=42)
…
>> X_train
array([[4, 5],
[0, 1],
[6, 7]])
>> y_train
[2, 0, 3]
>> X_test
array([[2, 3],
[8, 9]])
>> y_test
[1, 4]
GroupKFold

sklearn.model_selection.GroupKFold(n_splits=3)
举例：

>> from sklearn.model_selection import GroupKFold
>> X = np.array([[1, 2], [3, 4], [5, 6], [7, 8]])
>> y = np.array([1, 2, 3, 4])
>> groups = np.array([0, 0, 2, 2])
>> group_kfold = GroupKFold(n_splits=2)
>> group_kfold.get_n_splits(X, y, groups)
>> print(group_kfold)
GroupKFold(n_splits=2)
>> for train_index, test_index in group_kfold.split(X, y, groups):
…     print(“TRAIN:”, train_index, “TEST:”, test_index)
…     X_train, X_test = X[train_index], X[test_index]
…     y_train, y_test = y[train_index], y[test_index]
…     print(X_train, X_test, y_train, y_test)
…
TRAIN: [0 1] TEST: [2 3]
[[1 2]
[3 4]] [[5 6]
[7 8]] [1 2] [3 4]
TRAIN: [2 3] TEST: [0 1]
[[5 6]
[7 8]] [[1 2]
[3 4]] [3 4] [1 2]shuffle()
返回随机排序后的序列。
numpy.random.shuffle(x)
Examples
>> arr = np.arange(10)
>> np.random.shuffle(arr)
>> arr
[1 7 5 2 9 4 3 6 0 8]
Multi-dimensional arrays are only shuffled along the first axis:
>> arr = np.arange(9).reshape((3, 3))
>> np.random.shuffle(arr)
>> arr
array([[3, 4, 5],
[6, 7, 8],
[0, 1, 2]])
array、asarray
array和asarray都可以将结构数据转化为ndarray，但是主要区别就是当数据源是ndarray时，array仍然会copy出一个副本，占用新的内存，但asarray不会。所以如下：
![asarray](https://img-blog.csdn.net/20171013142329535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
range()、arange()
range生成一个序列，arange生成一个ndarray
numpy.random.rand(m,n)
返回m*n维的数组，数值取件（0，1）

