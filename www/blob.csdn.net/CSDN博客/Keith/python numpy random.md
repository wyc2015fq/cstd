# python numpy random - Keith - CSDN博客





2016年12月17日 13:33:37[ke1th](https://me.csdn.net/u012436149)阅读数：747标签：[python																[random																[numpy](https://so.csdn.net/so/search/s.do?q=numpy&t=blog)
个人分类：[python](https://blog.csdn.net/u012436149/article/category/6462204)





# python numpy random

```python
np.random.rand(d0,d1,...,dn)
#Create an array of the given shape and populate it
#with random samples from a uniform distribution over
# [0, 1).

np.random.rand(3,2)
#输出
array([[ 0.14022471,  0.96360618],  #random
       [ 0.37601032,  0.25528411],  #random
       [ 0.49313049,  0.94909878]]) #random
```

```python
np.random.randn(d0,d1,...,dn)
#Create an array of the given shape and populate it
#with random samples from a gaussian(norm,(0,1))
#distribution.
```

```python
np.random.randint(low, high=None, size=None, dtype=np.int)
#从 [low, high) 均匀分布采整数。
np.random.randint(0,3,10)
# array([0, 2, 2, 1, 1, 2, 0, 0, 1, 0])
```

## 参考资料

[https://docs.scipy.org/doc/numpy/reference/routines.random.html](https://docs.scipy.org/doc/numpy/reference/routines.random.html)](https://so.csdn.net/so/search/s.do?q=random&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




