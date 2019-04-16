# python对字典的基本操作（遍历、排序）总结 - 一个跳popping的quant的博客 - CSDN博客





2018年10月09日 14:40:02[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1762








### Python字典容器

python中的字典同其他语言中的字典作用一样，都是用来存储数据的**容器**。只不过不同于其他序列型数据用下标来访问其中的对象，而是以关键字key来访问其中的对象value。另外，字典也被称为关联数组或者哈希表。

字典的应用场景有很多，下面通过一个投票的例子来解释。

**环境 python3.5**

创建一个字典：

```python
dict = {
    'ZhangSan':2,
    'LiSi':10,
    'WangWu':5,
    'ZhaoLiu':4
}
```

### 字典的遍历

1、遍历字典的key

直接对字典进行遍历得到的是key的遍历结果。

```python
for key in dict:
    print(key)
```

 也可以通过对dict.keys()方法返回的数组进行遍历。

```python
for key in dict.keys():
    print(key)
```

2、遍历字典的value

得到字典中的key之后就可以直接通过索引来得到相对应的value了。

```python
for key in dict.keys():
    print(dict[key])
```

也可以通过dict.values()来直接遍历索引。

```python
for value in dict.values():
    print(value)
```

3、遍历字典的项

通过dict.item()返回的元组数列可以遍历整个结果，每个数据项是一个元组。

```python
for item in dict.items():
    print(item)
```

```python
('ZhangSan', 2)
('LiSi', 10)
('WangWu', 5)
('ZhaoLiu', 4)
```

也可以直接对items进行key和value的遍历。

```python
for key, value in dict.items():
    print(key, value)
```

```python
ZhangSan 2
LiSi 10
WangWu 5
ZhaoLiu 4
```

### 字典的排序

有时候需要对生成的字典的结果按照key或者value进行排序，主要用到的方法就是python的内置函数**sorted()**。

sorted()函数的作用是对所有**可迭代**的对象进行排序操作的函数。

```python
sorted(iterable, cmp=None, key=None, reverse=False)
```

参数说明：
- iterable：是可迭代的数据类型。
- cmp：是用于比较的函数，将key指定的关键字通过该函数规定的比较规则进行比较。有点类似java中的Comparable抽象类中的compareTo()方法。传递两个参数，例如f(a,b)，指定a和b的逻辑比较规则，结果返回正数、负数或者零来分别表示大于、小于和等于。
- key：用于进行比较的数据。
- reverse：排序规则，默认是True降序，或者设置为False来升序。

1、按照key值进行排序

当iterable传入的dict时，dict返回的是对key迭代，所以得到的排序结果也是对key排序后的列表。

```python
sorted_dict = sorted(dict, key=lambda x:x[0])
print(sorted_dict)
```

```
['LiSi', 'WangWu', 'ZhangSan', 'ZhaoLiu']
```

 如果传入的是dict.items()，此时传入的是元组，所以排序后返回的结果也是元组的列表。

```python
sorted_dict = sorted(dict.items(), key=lambda x:x[0])
print(sorted_dict)
```
`[('LiSi', 10), ('WangWu', 5), ('ZhangSan', 2), ('ZhaoLiu', 4)]`
2、按照value值进行排序

按照value排序时，只需把key迭代对象选择为x[1]就可以了。

```python
sorted_dict = sorted(dict.items(), key=lambda x:x[1], reverse=True)
print(sorted_dict)
```

```python
[('LiSi', 10), ('WangWu', 5), ('ZhaoLiu', 4), ('ZhangSan', 2)]
```

 其中，lambda匿名函数的作用类似：

```python
def f(x):
    return x[1]
```





