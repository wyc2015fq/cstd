# python 中 reduce 函数的使用 - lincappu - 博客园







# [python 中 reduce 函数的使用](https://www.cnblogs.com/lincappu/p/8148851.html)





**reduce()**函数也是Python内置的一个高阶函数。

reduce()函数接收的参数和 map()类似，**一个函数 f，一个list**，但行为和 map()不同，reduce()传入的函数 f 必须接收两个参数，reduce()对list的每个元素反复调用函数f，并返回最终结果值。

例如，编写一个f函数，接收x和y，返回x和y的和：




```python
def
```

```python
f(x, y):
```



```python

```

```python
return
```

```python
x
```

```python
+
```

```python
y
```




调用 **reduce(f, [1, 3, 5, 7, 9])**时，reduce函数将做如下计算：




```python
先计算头两个元素：f(
```

```python
1
```

```python
,
```

```python
3
```

```python
)，结果为
```

```python
4
```

```python
；
```



```python
再把结果和第
```

```python
3
```

```python
个元素计算：f(
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
)，结果为
```

```python
9
```

```python
；
```



```python
再把结果和第
```

```python
4
```

```python
个元素计算：f(
```

```python
9
```

```python
,
```

```python
7
```

```python
)，结果为
```

```python
16
```

```python
；
```



```python
再把结果和第
```

```python
5
```

```python
个元素计算：f(
```

```python
16
```

```python
,
```

```python
9
```

```python
)，结果为
```

```python
25
```

```python
；
```



```python
由于没有更多的元素了，计算结束，返回结果
```

```python
25
```




上述计算实际上是对 list 的所有元素求和。虽然Python内置了求和函数sum()，但是，利用reduce()求和也很简单。

**reduce()还可以接收第3个可选参数，作为计算的初始值。**如果把初始值设为100，计算：



```python
reduce
```

```python
(f, [
```

```python
1
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
5
```

```python
,
```

```python
7
```

```python
,
```

```python
9
```

```python
],
```

```python
100
```

```python
)
```



结果将变为125，因为第一轮计算是：

计算初始值和第一个元素：**f(100, 1)**，结果为**101**。



在python 3.0.0.0以后, reduce已经不在built-in function里了, 要用它就得from functools import reduce.



用途：














