# Numpy/Pytorch之数据类型与强制类型转换 - qq_37385726的博客 - CSDN博客





2018年08月17日 11:47:19[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：6444
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[1.数据类型简介](#1.%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B%E7%AE%80%E4%BB%8B)

[Numpy](#Numpy)

[Pytorch](#Pytorch)

[2.Python的type()函数](#2.Python%E7%9A%84type()%E5%87%BD%E6%95%B0)

[3.Numpy/Pytorch的dtype属性](#3.Numpy%2FPytorch%E7%9A%84dtype%E5%B1%9E%E6%80%A7)

[4.Numpy中的类型转换](#4.Numpy%E4%B8%AD%E7%9A%84%E7%B1%BB%E5%9E%8B%E8%BD%AC%E6%8D%A2)

[先聊聊我为什么会用到这个函数（不看跳过）](#%E5%85%88%E8%81%8A%E8%81%8A%E6%88%91%E4%B8%BA%E4%BB%80%E4%B9%88%E4%BC%9A%E7%94%A8%E5%88%B0%E8%BF%99%E4%B8%AA%E5%87%BD%E6%95%B0%EF%BC%88%E4%B8%8D%E7%9C%8B%E8%B7%B3%E8%BF%87%EF%BC%89)

[astype()函数](#astype()%E5%87%BD%E6%95%B0)

[输出](#%E8%BE%93%E5%87%BA)

[4.Pytorch中的类型转换](#4.Pytorch%E4%B8%AD%E7%9A%84%E7%B1%BB%E5%9E%8B%E8%BD%AC%E6%8D%A2)

[Way1 : 变量直接调用类型](#Way1%20%3A%20%E5%8F%98%E9%87%8F%E7%9B%B4%E6%8E%A5%E8%B0%83%E7%94%A8%E7%B1%BB%E5%9E%8B)

[Way2 : 变量调用pytorch中的type函数](#Way2%20%3A%20%E5%8F%98%E9%87%8F%E8%B0%83%E7%94%A8pytorch%E4%B8%AD%E7%9A%84type%E5%87%BD%E6%95%B0)

[Way3 : 变量调用pytorch中的type_as函数](#Way3%C2%A0%3A%20%E5%8F%98%E9%87%8F%E8%B0%83%E7%94%A8pytorch%E4%B8%AD%E7%9A%84type_as%E5%87%BD%E6%95%B0)





# 1.数据类型简介

## Numpy

NumPy 支持比 Python 更多种类的数值类型。 下表显示了 NumPy 中定义的不同标量数据类型。
|序号|数据类型及描述|
|----|----|
|1.|bool_存储为一个字节的布尔值(真或假)|
|2.|int_默认整数，相当于 C 的long，通常为int32或int64|
|3.|intc相当于 C 的int，通常为int32或int64|
|4.|intp用于索引的整数，相当于 C 的size_t，通常为int32或int64|
|5.|int8字节(-128 ~ 127)|
|6.|int1616 位整数(-32768 ~ 32767)|
|7.|int3232 位整数(-2147483648 ~ 2147483647)|
|8.|int6464 位整数(-9223372036854775808 ~ 9223372036854775807)|
|9.|uint88 位无符号整数(0 ~ 255)|
|10.|uint1616 位无符号整数(0 ~ 65535)|
|11.|uint3232 位无符号整数(0 ~ 4294967295)|
|12.|uint6464 位无符号整数(0 ~ 18446744073709551615)|
|13.|float_float64的简写|
|14.|float16半精度浮点：符号位，5 位指数，10 位尾数|
|15.|float32单精度浮点：符号位，8 位指数，23 位尾数|
|16.|float64双精度浮点：符号位，11 位指数，52 位尾数|
|17.|complex_complex128的简写|
|18.|complex64复数，由两个 32 位浮点表示(实部和虚部)|
|19.|complex128复数，由两个 64 位浮点表示(实部和虚部)|

> 
**直接使用类型名很可能会报错，正确的使用方式是np.调用，eg,  np.uint8**




## **Pytorch**

`Torch`定义了七种`CPU`张量类型和八种`GPU`张量类型,这里我们就只讲解一下`CPU`中的，其实`GPU`中只是中间加一个`cuda`即可，如`torch.cuda.FloatTensor`：
- torch.FloatTensor(2,3) 构建一个2*3 `Float`类型的张量
- torch.DoubleTensor(2,3) 构建一个2*3 `Double`类型的张量
- torch.ByteTensor(2,3) 构建一个2*3 `Byte`类型的张量
- torch.CharTensor(2,3) 构建一个2*3 `Char`类型的张量
- torch.ShortTensor(2,3) 构建一个2*3 `Short`类型的张量
- torch.IntTensor(2,3) 构建一个2*3 `Int`类型的张量
- torch.LongTensor(2,3) 构建一个2*3 `Long`类型的张量



> 
同样，**直接使用类型名很可能会报错，正确的使用方式是torch.调用，eg，torch.FloatTensor()**






# 2.Python的type()函数

> 
**type函数可以由变量调用，或者把变量作为参数传入。**

**返回的是该变量的类型，而非数据类型。**


```python
data = np.random.randint(0, 255, 300)
print(type(data))
```

**输出**

**<class 'numpy.ndarray'>**



# **3.Numpy/Pytorch的dtype属性**

> 
**返回值为变量的数据类型**


```python
t_out = torch.Tensor(1,2,3)
print(t_out.dtype)
```

**输出**

**torch.float32**

```python
t_out = torch.Tensor(1,2,3)
print(t_out.numpy().dtype)
```

**输出**

**float32**





# **4.Numpy中的类型转换**



### 先聊聊我为什么会用到这个函数（不看跳过）

为了实施trochvision.transforms.ToPILImage()函数

于是我想从numpy的ndarray类型转成PILImage类型

我做了以下尝试

```python
data = np.random.randint(0, 255, 300)
n_out = data.reshape(10,10,3)
print(n_out.dtype)
img = transforms.ToPILImage()(n_out)
img.show()
```

但是很遗憾，报错了

raise TypeError('Input type {} is not supported'.format(npimg.dtype))

TypeError: Input type int32 is not supported

因为要将ndarray转成PILImage要求ndarray是uint8类型的。

于是我认输了。。。

使用了

```python
n_out = np.linspace(0,255,300,dtype=np.uint8)
n_out = n_out.reshape(10,10,3)
print(n_out.dtype)
img = torchvision.transforms.ToPILImage()(n_out)
img.show()
```

**得到了输出**

uint8

![](https://img-blog.csdn.net/20180817113154139?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)    嗯，显示了一张图片

但是呢，就很憋屈，和想要的随机数效果不一样。

**于是我用了astype函数**



### **astype()函数**

> 
由变量调用，但是直接调用不会改变原变量的数据类型，是返回值是改变类型后的新变量，所以要赋值回去。
`n_out = n_out.astype(np.uint8)`

```python
#初始化随机数种子
np.random.seed(0)

data = np.random.randint(0, 255, 300)
print(data.dtype)
n_out = data.reshape(10,10,3)

#强制类型转换
n_out = n_out.astype(np.uint8)
print(n_out.dtype)

img = transforms.ToPILImage()(n_out)
img.show()
```

### 输出

int32

uint8

![](https://img-blog.csdn.net/20180817113751659?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





# 4.Pytorch中的类型转换

pytorch中没有astype函数，**正确的转换方法是**

### **Way1 : 变量直接调用类型**


`tensor = torch.Tensor(3, 5)`
**torch.long() 将tensor投射为long类型**
`newtensor = tensor.long()`
**torch.half()将tensor投射为半精度浮点类型**
`newtensor = tensor.half()`
**torch.int()将该tensor投射为int类型**
`newtensor = tensor.int()`
**torch.double()将该tensor投射为double类型**
`newtensor = tensor.double()`
**torch.float()将该tensor投射为float类型**
`newtensor = tensor.float()`
**torch.char()将该tensor投射为char类型**
`newtensor = tensor.char()`
**torch.byte()将该tensor投射为byte类型**
`newtensor = tensor.byte()`
**torch.short()将该tensor投射为short类型**
`newtensor = tensor.short()`
> 
同样，和numpy中的astype函数一样，是**返回值才是改变类型后的结果**，调用的变量类型不变




### **Way2 : 变量调用pytorch中的type函数**

`type(new_type=None, async=False)`如果未提供`new_type`，则返回类型，否则将此对象转换为指定的类型。 如果已经是正确的类型，则不会执行且返回原对象。

用法如下：

```
self = torch.LongTensor(3, 5)
# 转换为其他类型
print self.type(torch.FloatTensor)
```



### **Way3 : 变量调用pytorch中的type_as函数**

如果张量已经是正确的类型，则不会执行操作。具体操作方法如下：

```
self = torch.Tensor(3, 5)
tesnor = torch.IntTensor(2,3)
print self.type_as(tesnor)
```







