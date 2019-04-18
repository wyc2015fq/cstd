# 我的python学习笔记（3） numpy - 长歌行 - CSDN博客





2017年06月24日 22:14:17[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1440
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)








时间有限，行文仓促，代码在[https://github.com/bentan2013/PythonAndGIS/blob/master/numpy/basic.ipynb](https://github.com/bentan2013/PythonAndGIS/blob/master/numpy/basic.ipynb)
## 为什么用numpy？


- 支持多维数组
- 自身支持多种矩阵运算
- 可以和机器学习的各种开源框架的tensor相互转化



## numpy的安装？
安装完pip，就可以使用pip install numpy


## numpy基础

使用之前需要先引用包
```cpp
import numpy as np
```
之后就可以使用np.somefunction()来引用其中的函数，数据结构了。



### 创建方式

可以从列表生成，数组生成，插值生成，生成格网，随机初始化，初始化为0或者1




```python
#从列表生成
tmp = np.array([1,2,3])

数组为:
[1 2 3]

#插值生成 linspace logspace
mat = np.linspace(0, 18, 9)
mat: array([  0.  ,   2.25,   4.5 ,   6.75,   9.  ,  11.25,  13.5 ,  15.75,  18.  ])

# 创建格网
g = np.mgrid[1:4, 0:4]
print(g)

[[[1 1 1 1]
  [2 2 2 2]
  [3 3 3 3]]

 [[0 1 2 3]
  [0 1 2 3]
  [0 1 2 3]]]

# 随机生成
r = np.random.rand(3,3)
print(r)
# 带初值生成，注意括号((col, row, ..), type)
zero = np.zeros((3,3), dtype=np.float64)
print(zero)
one = np.ones((3,3))
print(one)

[[ 0.15798138  0.76426381  0.26221403]
 [ 0.98298431  0.94655075  0.00696479]
 [ 0.30875877  0.33322367  0.03711038]]
[[ 0.  0.  0.]
 [ 0.  0.  0.]
 [ 0.  0.  0.]]
[[ 1.  1.  1.]
 [ 1.  1.  1.]
 [ 1.  1.  1.]]
```


### 基本的描述元素


```python
tmp = np.array([1,2,3])

print('数组为:')
print(tmp)
print("数据类型为：")
print(type(tmp))
print('数组大小：')
print(tmp.size)
print('数据类型：')
print(tmp.dtype)
print('数组维度：')
print(tmp.ndim)
print('数组形状：')
print(tmp.shape)
```

```python
数组为:
[1 2 3]
数据类型为：
<class 'numpy.ndarray'>
数组大小：
3
数据类型：
int64
数组维度：
1
数组形状：
(3,)
```




### 序列化





test=np.random.rand(,)print(test)np.save('../data/random-5.npy',test)tmp=np.load('../data/random-5.npy')


### 索引和切片





```python
In [63]:
# 切片 索引 和 赋值

info = np.array([['小明', '男', '10'],
               ['小红', '女', '11'],
               ['小刚', '男', '12']])
info
Out[63]:
array([['小明', '男', '10'],
       ['小红', '女', '11'],
       ['小刚', '男', '12']], 
      dtype='<U2')
In [83]:
#(最小值:最大值:采样间隔)  

# 按行来取值
小红 = info[1:2:1]  
print(小红)

# 按列取值
name = info[0:3, :1]
print(name)

age_index = [2]
print(info[::, age_index])
[['小红' '女' '11']]
[['小明']
 ['小红']
 ['小刚']]
[['10']
 ['11']
 ['12']]
```




### 各种矩阵运算






```python
# 矩阵运算

mat = np.random.rand(3,3)
print(mat)

mat = mat + 2
mat = mat * -1
print(mat)

dot = np.dot(mat, mat)
print(dot)
# more: dot cross inner, outer  .T, .H  inv .I  mean std .max() .min() sum
[[ 0.19335883  0.6568079   0.1705884 ]
 [ 0.43722217  0.80471827  0.7774497 ]
 [ 0.77102649  0.41027065  0.61815674]]
[[-2.19335883 -2.6568079  -2.1705884 ]
 [-2.43722217 -2.80471827 -2.7774497 ]
 [-2.77102649 -2.41027065 -2.61815674]]
[[ 17.30081202  18.51063623  17.82297017]
 [ 19.87781102  21.0360812   20.35196874]
 [ 19.20720217  20.43268156  19.56390816]]
```


综合应用




以读csv文件为例，我们可以使用 csv模组或者直接用numpy来读，通过对比代码，我们会发现，numpy更加简洁。




### 使用CSV来读csv


```python
# 读文件
import csv

index = [2,4,5]
# 读出前十个地区
with open('../data/cities.csv', 'rt') as file:
    reader = csv.DictReader(file)
    district = [row['district'] for row in reader]
    print(district[:10])
['北京市', '天安门', '东城区', '西城区', '崇文区', '宣武区', '朝阳区', '丰台区', '石景山区', '海淀区']
```




### 使用numpy



```python
pos_index = [4, 5]
pos = np.genfromtxt('../data/cities.csv',delimiter=',', skip_header=True)[:, pos_index]
print(pos[:10])
[[ 116.4    39.9 ]
 [ 116.38   39.9 ]
 [ 116.42   39.93]
 [ 116.37   39.92]
 [ 116.43   39.88]
 [ 116.35   39.87]
 [ 116.43   39.92]
 [ 116.28   39.85]
 [ 116.22   39.9 ]
 [ 116.3    39.95]]
```


果然很简洁不是吗？顺便显示下这些点吧。


```python
import matplotlib.pyplot as plt
%matplotlib inline
plt.scatter(pos[:, 0], pos[:,1])
plt.show()
```


![](https://ooo.0o0.ooo/2017/06/24/594e72b986cb6.png)


本系列代码请见：[github](https://github.com/bentan2013/PythonAndGIS)


# 人生苦短，我用python





