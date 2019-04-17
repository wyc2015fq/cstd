# python可变对象与不可变对象，is与== - 别说话写代码的博客 - CSDN博客





2019年01月03日 01:07:22[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：18标签：[python is ==																[python可变对象																[python不可变对象](https://so.csdn.net/so/search/s.do?q=python不可变对象&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=python可变对象&t=blog)




**可变数据类型：列表list和字典dict；**可变数据类型，允许变量的值发生变化，即如果对变量进行append、+=等这种操作后，只是改变了变量的值，而不会新建一个对象，变量引用的对象的地址也不会变化，不过对于相同的值的不同对象，在内存中则会存在不同的对象，即每个对象都有自己的地址，相当于内存中对于同值的对象保存了多份，这里不存在引用计数，是实实在在的对象。

**不可变数据类型：整型int、浮点型float、字符串型string和元组tuple**。不可变数据类型，不允许变量的值发生变化，如果改变了变量的值，相当于是新建了一个对象，而对于相同的值的对象，在内存中则只有一个对象，内部会有一个引用计数来记录有多少个变量引用这个对象；

```python
a='123'   #不可变对象
b='123'
print(a==b)
print(a is b)

a=1    #不可变对象
b=1
print(a==b)
print(a is b)
```

输出为：

![](https://img-blog.csdnimg.cn/20190103010027892.png)

```python
a=(1)   #不可变对象
b=(1)
print(a==b)
print(a is b)

a=(1,1)   #不知道为啥
b=(1,1)
print(a==b)
print(a is b)
```

结果：

![](https://img-blog.csdnimg.cn/20190103010553575.png)

```python
a={'a':1,'b':2}    #可变对象
b={'a':1,'b':2}
print(a==b)
print(a is b)

a=[1,2,3]   #可变对象
b=[1,2,3]
print(a==b)
print(a is b)
```

输出：

![](https://img-blog.csdnimg.cn/20190103010649433.png)](https://so.csdn.net/so/search/s.do?q=python is ==&t=blog)




