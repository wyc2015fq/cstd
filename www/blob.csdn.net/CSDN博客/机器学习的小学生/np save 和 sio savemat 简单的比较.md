
# np.save 和 sio.savemat 简单的比较 - 机器学习的小学生 - CSDN博客


2017年10月27日 20:11:01[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：2526



```python
#coding=gbk
```
```python
import numpy
```
```python
as
```
```python
np
import scipy.io
```
```python
as
```
```python
sio
```
```python
# mat
```
```python
# 对两种save 方法进行比较
```
```python
# 比较结果，同样的数据，通过.mat 存储后在导入变成了二维的，因为matlab对[4,]和[1,4]没有区别，但python有区别。
```
```python
# 结论，所以使用的时候要注意些。
```
```python
arr1 = np.array([
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
])
arr2 = np.array([
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
])
sio.savemat(
```
```python
'test.mat'
```
```python
, {
```
```python
'a'
```
```python
:arr1})
```
```python
a
```
```python
= sio.loadmat(
```
```python
'test.mat'
```
```python
)
```
```python
a
```
```python
=
```
```python
a
```
```python
[
```
```python
'a'
```
```python
]
np.save(
```
```python
'test.npy'
```
```python
,arr2)
b = np.
```
```python
load
```
```python
(
```
```python
'test.npy'
```
```python
,
```
```python
'r'
```
```python
)
print(
```
```python
a
```
```python
,b)
print(
```
```python
a
```
```python
.shape,b.shape)
```
**Result**:
```python
[[1 2 3 4]]
```
```python
[
```
```python
3
```
```python
4
```
```python
5
```
```python
6
```
```python
]
(
```
```python
1
```
```python
,
```
```python
4
```
```python
) (
```
```python
4
```
```python
,)
```

