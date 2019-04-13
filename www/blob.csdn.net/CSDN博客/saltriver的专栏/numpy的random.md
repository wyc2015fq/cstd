
# numpy的random - saltriver的专栏 - CSDN博客


2016年08月26日 22:38:43[saltriver](https://me.csdn.net/saltriver)阅读数：1647



在Python的random中，可以非常方便的生成随机数，但如果需要生成多种维度的随机数组或矩阵，那么就需要更好更强大的numpy的random。
# 一、基本函数
## （1）随机生成包含N个元素的数组
形式：random.random(N)
形式：random.rand(N)
形式：random.ranf(N)
形式：random.sample(N)
形式：random.random_sample(N)

```python
import numpy as np
print(np.random.random(3))
print(np.random.rand(3))
print(np.random.ranf(3))
print(np.random.sample(3))
print(np.random.random_sample(3))
```
输出结果：

```python
[ 0.8557877 0.36745755 0.44609271]
[ 0.71871547 0.91454405 0.49529504]
[ 0.81503449 0.60967357 0.26630286]
[ 0.61978065 0.22269275 0.48401033]
[ 0.52780951 0.56578495 0.30356478]
```
random.random,random.rand,random.ranf,random.sample,random.random_sample，这几个函数是等价的，都是生成0~1之间随机数的数组，不知道numpy为什么要搞这么多功能一样的冗余函数，可能是为了兼容以前的版本吧。如果不输入参数N，则默认生成一个数。注意np.random.sample与标准库中的random.sample含义完全不一样。
## （2）随机生成包含(M*N)个元素的数组random.random((M*N))

```python
import numpy as np
print("random=",np.random.random([3,2]))
print("rand=",np.random.rand(3,2))
print("ranf=",np.random.ranf((3,2)))
print("sample=",np.random.sample((3,2)))
print("random_sample=",np.random.random_sample((3,2)))
```
输出结果：

```python
random= [[ 0.75084195 0.96951131]
[ 0.37772105 0.3978906 ]
[ 0.02802572 0.43438696]]
rand= [[ 0.72677444 0.8633535 ]
[ 0.69032441 0.25305748]
[ 0.69106003 0.00194737]]
ranf= [[ 0.8468361 0.49833203]
[ 0.58263348 0.58513805]
[ 0.9306215 0.6438961 ]]
sample= [[ 0.64917596 0.49576786]
[ 0.29585299 0.21726225]
[ 0.0484201 0.86272376]]
random_sample= [[ 0.06026404 0.78595412]
[ 0.39164591 0.48605263]
[ 0.31219578 0.96118488]]
```
注意：random.rand的输入参数是一串数，而其他几个函数输入参数是一个序列，序列的类型可以是元组，也可以是列表，输出结果可以形成多维数组。
```python
print(np.random.random([3,2,4,1]))
```
输出结果：

```python
[[[[ 0.31852145]
[ 0.54043198]
[ 0.1763678 ]]
[[ 0.89952898]
[ 0.21197707]
[ 0.02113006]]]
[[[ 0.01087573]
[ 0.86285813]
[ 0.96468875]]
[[ 0.10674783]
[ 0.62386137]
[ 0.23828229]]]]
```

## （3）随机生成包含N个元素的在[a,b]范围内的整数的数组
形式：random.randint(a,b,N)
形式：random.random_integers(a,b,N)

```python
<span style="font-size:18px;">print(np.random.randint(2,9,3))
print(np.random.random_integers(2,9,3))</span>
```
输出结果：

```python
[5 6 3]
[7 8 3]
```
## （4）随机生成包含N个元素的在[a,b]范围内的浮点数的数组
形式：random.uniform(a,b,N)
```python
print(np.random.uniform(2,9,3))
```
输出结果：
```python
[ 2.09766702 7.38542663 3.32406193]
```

# 二、操作序列的函数
## （1）从一个序列seq中随机选取N个元素
形式：random.choice(seq,N)
s = [1,2,3,4,5]

```python
print(np.random.choice(s))
print(np.random.choice(s,3))
```
输出结果:

```python
5
[2 2 5]
```
类似于标准库中random.choice和random.sample2个函数的功能，区别是：一是np.random.choice生成的是数组；二是np.random.choice在序列中随机选取的位置可能相同；三是np.random.choice中N可以大于seq的size。
## （2）将一个序列中的元素随机打乱/重新排列
形式：random.permutation
形式：random.shuffle(seq)

```python
s = [1,2,3,4,5]
print(np.random.permutation(s))
print(s)
t = np.random.shuffle(s)
print(t)
print(s)
```
输出结果：

```python
[3 4 5 2 1]
[1, 2, 3, 4, 5]
None
[4, 3, 2, 1, 5]
```
可以看出，permutation将原序列重新排列后生成一个新序列，原序列不变；shuffle对原序列进行原地重新排列，改变了原序列。
# 三、各种分布的函数
这是numpy.random强大的地方，几乎所有分布都有相对应的函数，像指数分布、对数分布、泊松分布、gamma、beta等。当然，最常用的仍然是均匀分布和正态分布。各种分布的细节就不介绍了，非常多，用到的时候参考numpy手册吧。均匀分布有浮点数和整数的均匀分布，就是前面第一节提到的那些函数，最常用。对于正态分布，有三个对应的函数。
## （1）随机生成符合标准正态分布的N个元素数组
形式：random.standard_normal(N)
```python
print(np.random.standard_normal(3))
```
输出结果：
```python
[ 0.53582113 0.38841927 -1.21489776]
```

## （2）随机生成均值为u，标准差为o的包含N个元素的数组
形式：random.normal(u,o,N)
```python
print(np.random.normal(10,2,3))
```
输出结果：
```python
[ 11.26972979 12.70044985 9.22437455]
```

random.normal(0,1,N)等同于random.standard_normal(N)。
## （3）随机生成N组，每组M个元素的符合标准正态分布的数组
形式：random.randn(N,M)
```python
print(np.random.randn(5,2))
```
输出结果：

```python
[[ 2.3531218 0.34294009]
[ 0.39189985 -1.71165367]
[ 1.68202598 -1.91222334]
[-1.42669599 -0.19984201]
[-0.19637414 0.4687705 ]]
```

实际上输入参数可以很多，形成不同维度的数组。如下例：生成5组，每组2行3列。
```python
print(np.random.randn(5,2,3))
```
输出结果：

```python
[[[-0.59553883 0.2567095 1.19788239]
[-0.22927001 0.74439248 0.99973737]]
[[ 0.40198413 -1.5297974 -1.35220548]
[-1.12401606 1.55585411 -0.902386 ]]
[[-0.13046875 0.5698744 1.70325471]
[ 0.36825335 0.13363337 -0.72413365]]
[[ 1.44607607 -0.23438731 1.00991822]
[-0.86659134 1.85683232 -1.90688971]]
[[ 1.7038288 1.76022888 -0.99409035]
[ 1.22736843 -0.45617018 -0.11044293]]]
```
四、总结
除了有些函数名字不同功能却一样，显得有些冗余外，numpy的random比标准库中的random功能强大很多，且可以兼容python标准库random中的所有功能，在生成随机数组/矩阵和各种数学分布时非常实用。

