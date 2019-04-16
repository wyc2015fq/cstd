# python2与python3中关于对NaN类型数据的判断和转换 - 一个跳popping的quant的博客 - CSDN博客





2018年03月06日 17:48:59[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：7664
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









今天在对一堆新数据进行数据清洗的时候，遇到了一个这样的问题：

```python
ValueError: cannot convert float NaN to integer
```

一开始是这样的，我用的jupyter是python35的，使用DataFrame读入了数据，其中有一列是year，默认读入时是将year这一列转换为了float，所以就有了这样的现象：

![](https://img-blog.csdn.net/20180306160513258)

年份都是float类型了，看得我强迫症都犯了。于是通过这样的代码来进行强转，于是就报了上面的错误了。

```python
df.year = [int(y) for y in df.year]
```

简单描述一下问题，其实就是NaN在python35中无法被强转。

首先说一下，NaN类型在python25中在强转int的时候默认是转换为0的，而在python25之后的版本再进行转换的时候就会报以上的错误。

我们先打印看一下np.nan的类型：

```python
print(type(np.nan))
```

```python
<type 'float'>
```

np.nan是float类型，但是在进行int转换的时候就会报错。

**解决方法：**

使用is或者==进行判断是不是NaN，不是NaN进行强转int，是则用0代替。

先说一下==和is使用时的区别：

is和==都是对对象进行比较判断作用的，但对对象比较判断的内容并不相同。


如果有a跟b两个变量，只有数值型和字符串型的情况下，a is b才为True，当a和b是tuple，list，dict、set或者是实例化对象时，a is b为False。

==是python标准操作符中的比较操作符，用来比较判断两个对象的value(值)是否相等。




通过下面的代码可以看出，np.nan==np.nan结果是False，但是np.nan is np.nan却是True。


```python
a = np.nan


print(a == np.nan)
print(a == a)
print(a is np.nan)
print(a is a)


False
False
True
True
```

因此，通过每个元素与自身比较就可以解决了，代码如下：

```python
year = []
for y in df.year:
    if y == y:
        year.append(int(y))
    else:
        year.append(0)
```




































