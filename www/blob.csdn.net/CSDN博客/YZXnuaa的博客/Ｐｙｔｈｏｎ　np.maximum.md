# Ｐｙｔｈｏｎ　np.maximum - YZXnuaa的博客 - CSDN博客
2018年05月08日 15:04:07[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：693
# np.maximum
```
>>> np.maximum([2, 3, 4], [1, 5, 2])
array([2, 5, 4])
>>> np.maximum(np.eye(2), [0.5, 2]) # broadcasting
array([[ 1. ,  2. ],
       [ 0.5,  2. ]])
>>> np.maximum([np.nan, 0, np.nan], [0, np.nan, np.nan])
array([ NaN,  NaN,  NaN])
>>> np.maximum(np.Inf, 1)
inf
```
值得注意的是，numpy中array的broadcasting特性，非常有用。 
一个常用应用是np.maximum(0,array)，可作为ReLU gate。
