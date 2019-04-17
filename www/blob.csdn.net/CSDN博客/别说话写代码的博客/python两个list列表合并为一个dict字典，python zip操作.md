# python两个list列表合并为一个dict字典，python zip操作 - 别说话写代码的博客 - CSDN博客





2019年01月03日 00:48:59[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：95
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









**zip()** 函数用于将可迭代的对象作为参数，将对象中对应的元素打包成一个个元组，然后返回由这些元组组成的列表。

如果各个迭代器的元素个数不一致，则返回列表长度与最短的对象相同，利用 * 号操作符，可以将元组解压为列表。

```python
l1 = [1, 2, 3, 4, 5];
l2 = ["a", "b", "c", "d","e"];
for i in zip(l1,l2):
    print(i)
```

执行结果为： 

![](https://img-blog.csdnimg.cn/20190103004802663.png)

```python
l1 = [1, 2, 3, 4, 5];
l2 = ["a", "b", "c", "d","e"];
print(dict(zip(l1,l2)))

l1 = [1, 2, 3, 4];  #长度短一个
l2 = ["a", "b", "c", "d","e"];
print(dict(zip(l1,l2)))

l1 = [1, 2, 3, 4, 5, 6];
l2 = ["a", "b", "c", "d","e"];  #长度短一个
print(dict(zip(l1,l2)))
```

两个list长度不同，以list短的为主

输出为：

![](https://img-blog.csdnimg.cn/20190103004452374.png)



