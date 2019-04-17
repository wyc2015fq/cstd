# argsort中axis参数理解 - 墨流觞的博客 - CSDN博客





2017年05月31日 21:48:26[墨氲](https://me.csdn.net/dss_dssssd)阅读数：78








#### 简要说明

```
argsort(a, axis=-1, kind='quicksort', order=None)
   按照kin关键字指定的排序方式，对原数组按照从小到大的顺序进行排序，得到一个新的数组，将该数组中每个元素在原数组中的索引值代替该元素得到
```

例子：

```python
x = np.array(np.random.randint(1,100,6))
# x:  array([15, 16, 18, 7, 83, 53])
y = np.argsort(x)
# 输出array([3, 0, 1, 2, 5, 4], dtype=int64)
y_1 = sorted(x)
# [7, 15, 16, 18, 53, 83]
```

#### 解释

```
可以看出，7在x中的索引值为3,
         15在x中的索引值为0,
         16在x中的索引值为1
 简单的等价表达式：
```

```python
ls = []
x_1 = list(x)
for i in y_1:
    ls.append(x.index(i))
```

##### 参数解释

```
axis指定排序的轴，默认axis=-1为最后一个轴
  kind为排序方法， {'quicksort', 'mergesort', 'heapsort'}默认为快速排序
```

一般只关注参数axis，二维情况下，默认按行排序，若指定axis=0,则为按列排序[关于axis更详细的解释](http://blog.csdn.net/dss_dssssd/article/details/71698170)

##### 看一些例子

```
`>>> x = np.array([[0, 3], [2, 2]])
   >>> x
    array([[0, 3],
      [2, 2]])
  # 按列排序
  >>> np.argsort(x, axis=0)
    array([[0, 1],
           [1, 0]])
 # 按行排序
 >>> np.argsort(x, axis=1)
      array([[0, 1],
             [0, 1]]) `
```

```python
# 找出数组中最大的3个数，并保持原来的顺序
  x = np.array(np.random.randint(1,100,10))
  # array([78, 68, 79, 69, 77, 49, 28, 73, 69, 76])
  x[sorted(np.argsort(x)[-3:])]
  # array([78, 79, 76])
  # 首先利用argsort()将元素排序，利用切片，得到最大的3个元素的索引值，在将索引值排序，利用numpy数组索引方式取出所要求的值
```



