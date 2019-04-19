# numpy数据类型dtype转换 - Popeye_the_Sailor - CSDN博客
2018年07月13日 00:22:48[_Sailor_](https://me.csdn.net/lz0499)阅读数：329

# [numpy数据类型dtype转换](https://www.cnblogs.com/hhh5460/p/5129032.html)
这篇文章我们玩玩numpy的数值数据类型转换
### 导入numpy
```python
import numpy as np
```
## 一、随便玩玩
### 生成一个浮点数组
```python
>>> a = np.random.random(4)
```
### 看看信息
```python
>>> a
array([ 0.0945377 ,  0.52199916,  0.62490646,  0.21260126])
>>> a.dtype
dtype('float64')
>>> a.shape
(4,)
```
### 改变dtype，发现数组长度翻倍！
```
```python
>>> a.dtype = 'float32'
>>> a
array([  3.65532693e+20,   1.43907535e+00,  -3.31994873e-25,
         1.75549972e+00,  -2.75686653e+14,   1.78122652e+00,
        -1.03207532e-19,   1.58760118e+00], dtype=float32)
>>> a.shape
(8,)
```
```
### 改变dtype，数组长度再次翻倍！
```
```python
>>> a.dtype = 'float16'
>>> a
array([ -9.58442688e-05,   7.19000000e+02,   2.38159180e-01,
         1.92968750e+00,              nan,  -1.66034698e-03,
        -2.63427734e-01,   1.96875000e+00,  -1.07519531e+00,
        -1.19625000e+02,              nan,   1.97167969e+00,
        -1.60156250e-01,  -7.76290894e-03,   4.07226562e-01,
         1.94824219e+00], dtype=float16)
>>> a.shape
(16,)
```
```
### 改变dtype='float'，发现默认就是float64，长度也变回最初的4
```
```python
>>> a.dtype = 'float'
>>> a
array([ 0.0945377 ,  0.52199916,  0.62490646,  0.21260126])
>>> a.shape
(4,)
>>> a.dtype
dtype('float64')
```
```
### 把a变为整数，观察其信息
```python
>>> a.dtype = 'int64'
>>> a
array([4591476579734816328, 4602876970018897584, 4603803876586077261,
       4596827787908854048], dtype=int64)
>>> a.shape
(4,)
```
### 改变dtype，发现数组长度翻倍！
```python
>>> a.dtype = 'int32'
>>> a
array([ 1637779016,  1069036447, -1764917584,  1071690807,  -679822259,
        1071906619, -1611419360,  1070282372])
>>> a.shape
(8,)
```
### 改变dtype，发现数组长度再次翻倍！
```python
>>> a.dtype = 'int16'
>>> a
array([-31160,  24990,  13215,  16312,  32432, -26931, -19401,  16352,
       -17331, -10374,   -197,  16355, -20192, -24589,  13956,  16331], dtype=int16)
>>> a.shape
(16,)
```
### 改变dtype，发现数组长度再次翻倍！
```
```python
>>> a.dtype = 'int8'
>>> a
array([  72, -122,  -98,   97,  -97,   51,  -72,   63,  -80,  126,  -51,
       -106,   55,  -76,  -32,   63,   77,  -68,  122,  -41,   59,   -1,
        -29,   63,   32,  -79,  -13,  -97, -124,   54,  -53,   63], dtype=int8)
>>> a.shape
(32,)
```
```
### 改变dtype，发现整数默认int32！
```
```python
>>> a.dtype = 'int'
>>> a.dtype
dtype('int32')
>>> a
array([ 1637779016,  1069036447, -1764917584,  1071690807,  -679822259,
        1071906619, -1611419360,  1070282372])
>>> a.shape
(8,)
```
```
## 二、换一种玩法
很多时候我们用numpy从文本文件读取数据作为numpy的数组，默认的dtype是float64。
但是有些场合我们希望有些数据列作为整数。如果直接改dtype='int'的话，就会出错！原因如上，数组长度翻倍了！！！
### 下面的场景假设我们得到了导入的数据。我们的本意是希望它们是整数，但实际上是却是浮点数(float64)
```python
>>> b = np.array([1., 2., 3., 4.])
>>> b.dtype
dtype('float64')
```
### 用 astype(int) 得到整数，并且不改变数组长度
```
```python
>>> c = b.astype(int)
>>> c
array([1, 2, 3, 4])
>>> c.shape
(8,)
>>> c.dtype
dtype('int32')
```
```
### 如果直接改变b的dtype的话，b的长度翻倍了，这不是我们想要的（当然如果你想的话）
```
```python
>>> b
array([ 1.,  2.,  3.,  4.])
>>> b.dtype = 'int'
>>> b.dtype
dtype('int32')
>>> b
array([         0, 1072693248,          0, 1073741824,          0,
       1074266112,          0, 1074790400])
>>> b.shape
(8,)
```
```
## 三、结论
### numpy中的数据类型转换，不能直接改原数据的dtype!  只能用函数astype()。
# python numpy 数据类型转换
numpy数据类型转换需要调用方法astype()，不能直接修改dtype。调用astype返回数据类型修改后的数据，但是源数据的类型不会变，需要进一步对源数据的赋值操作才能改变。例如
```python
>>> a=np.array([1.1, 1.2])
>>> a.dtype
dtype('float64')
>>> a.astype(np.int16)
array([1, 1], dtype=int16)
>>> a.dtype
dtype('float64') #a的数据类型并没有变
>>> a=a.astype(np.int16) #赋值操作后a的数据类型变化
>>> a.dtype
dtype('int16')
>>> a
array([1, 1], dtype=int16)
```
直接修改dtype数据会强制用新数据类型表示，并没有转换，因此输出错误数据
```python
>>> a=np.array([1.1, 1.2])
>>> a.dtype
dtype('float64')
>>> a.dtype=np.int16
>>> a.dtype
dtype('int16') 
>>> a
array([-26214, -26215, -26215,  16369,  13107,  13107,  13107,  16371], dtype=int16)
#原来float64相当于4个int16的位宽，这样强制转换后会将他们直接拆开成4个数，
#因此原来的两个float64成了8个int16
```
转载于：
[numpy数据类型dtype转换](https://www.cnblogs.com/hhh5460/p/5129032.html)
[python numpy 数据类型转换](https://blog.csdn.net/miao20091395/article/details/79276721)
