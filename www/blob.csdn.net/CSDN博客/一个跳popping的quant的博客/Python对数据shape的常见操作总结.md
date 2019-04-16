# Python对数据shape的常见操作总结 - 一个跳popping的quant的博客 - CSDN博客





2017年12月08日 16:02:47[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：11744
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









这一阵在用python做DRL建模的时候，尤其是在配合使用tensorflow的时候，加上tensorflow是先搭框架再跑数据，所以调试起来很不方便，经常遇到输入数据或者中间数据shape的类型不统一，导致一些op老是报错。而且由于水平菜，所以一些常用的数据shape转换操作也经常百度了还是忘，所以想再整理一下。 
**一、数据的基本属性**

求一组数据的长度
```
a = [1,2,3,4,5,6,7,8,9,10,11,12]
print(len(a))
print(np.size(a))
```

求一组数据的shape 

list是没有shape属性的，所以需要把它转换成np或者使用np.shape()

```
b = [[1,2,3],[4,5,6],[7,8,9]]
print(np.shape(b))
print(np.array(b).shape)
```

**二、数据的拼接**

append是直接将数组或者数据直接追加到下一个元素的位置，而extend是将数据最外层的[]去掉后追加。

```
c = [1,1,1,1]
d = [[2,2],[[2,2],[2,2]]]
c.append([1,2,3])
d.extend([1,2,3])

[1, 1, 1, 1, [1, 2, 3]]
[[2, 2], [[2, 2], [2, 2]], 1, 2, 3]
```

另外也可以通过numpy中的方法来进行拼接 

其中np.concatenate()的作用更偏向与数据的连接，通过其中的axis参数可以进行指定行列的拼接。 

而np.append()的作用是将value b追加到arr a中。
```
c = np.concatenate((a, b))
d = np.append(a,b)
print(c)
print(d)

[1 1 1 1 2 2 2 2]
[1 1 1 1 2 2 2 2]
```

**三、数据的shape的转换**

1、转置 

  数据的转置也经常会用到，通常可以用到numpy的transpose()方法或者直接将数据转换为numpy array后用.T或者用reshape()方法。
```
a = [[1,1,1],
     [1,1,1]]
b = [[2,2,2],
     [2,2,2]]
c = [[3,3,3],
     [3,3,3]]
b = np.array(b)
c = np.array(c)

print(np.transpose(a))
print(b.T)
print(np.reshape(c, (c.shape[1], c.shape[0])))

[[1 1]
 [1 1]
 [1 1]]
[[2 2]
 [2 2]
 [2 2]]
[[3 3]
 [3 3]
 [3 3]]
```

2、数据展开 

如果是一个多维的数组，可以直接使用np.reshape(-1)来进行转换，reshape是一个很好用的函数，其中的参数含义后面会讲到。

```
c = [[[3,3,3],
     [3,3,3]],
     [[2,2,2],
     [2,2,2]]]

print(np.reshape(c, -1))

[3 3 3 3 3 3 2 2 2 2 2 2]
```

3、维度转换 

有时候可能会用到将一个一维的数组转换为二维，或者是在column方向或row方向上增加维度。 

当给col方向增加维度时，可以直接arr[:,np.newaxis], 

给row方向增加维度时，可以arr[np.newaxis,:] 

另外，这里的np.newaxis可以这样理解： 

一个[1,2,3,4,5]数组的shape是（5，）,如果对它[:,np.newaix]的话，得到的shape就是（5，1） 

对它[np.newaix, :]的话，得到的结果就是（1，5）。**所以说newaxis加在哪个位置，哪个位置相应的维度就会产生一个新的维度。**
```
a = np.array([1,1,1,1])

b = a[np.newaxis,:]
c = a[:,np.newaxis]

print(b)
print(c)

[[1 1 1 1]]
[[1]
 [1]
 [1]
 [1]]
```

另外再说一个将多维数组转换为一维的两种方法：arr.ravel()和arr.flatten()。 

两者的不同之处在于arr.flatten()返回的是arr展开后的数组的复制，而arr.ravel()返回的是arr展开后的本身。 

一个是对值的操作，另一个是对地址的操作。 

类似c、c++中的指针。
```
a = np.array([[1,2,3]])
b = np.array([[1,2,3]])
a1 = a.flatten()
b1 = b.ravel()
print(a)
print(b)
a1[0] = 8
b1[0] = 8
print(a)
print(b)

[[1 2 3]]
[[1 2 3]]
[[1 2 3]]
[[8 2 3]]
```

4、reshape 

def reshape(a, newshape, order=’C’): 

其中newshape参数可以传入一个[]或者tuple。 

当数据的形状不确定时，如果想转换为1行，列数不确定的话，newshape可以传入(1, -1); 

如果想转换为1列，行数不确定的话，newshape可以传入(-1, 1); 

同理如果是2列或者2行的话，就是(-1, 2)或者(2，-1)。 

其中-1代表的是一个模糊控制，就是不确定的意思。
```
a = [[1,1,1],
     [1,1,1]]
b = [[2,2,2],
     [2,2,2]]
c = [[[3,3,3],
     [3,3,3]],
     [[2,2,2],
     [2,2,2]]]

print(np.reshape(c, [-1, 1]))
print(np.reshape(b, [-1, 1]))
print(np.reshape(c, [2, -1]))

[[3]
 [3]
 [3]
 [3]
 [3]
 [3]
 [2]
 [2]
 [2]
 [2]
 [2]
 [2]]
[[2]
 [2]
 [2]
 [2]
 [2]
 [2]]
[[3 3 3 3 3 3]
 [2 2 2 2 2 2]]
```









