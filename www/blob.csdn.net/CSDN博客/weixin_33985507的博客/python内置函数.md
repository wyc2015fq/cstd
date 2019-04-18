# python内置函数 - weixin_33985507的博客 - CSDN博客
2018年07月11日 21:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
python3中有许多的内置函数，下面我们就一一列举并进行举例：
1、abs()：abs()函数返回数字的绝对值。
返回值：函数返回x（数字）的绝对值，如果参数是一个复数，则返回他的大小。
实例：
#!/usr/bin/python3
```
print ("abs(-40) : ", abs(-40))
          print ("abs(100.10) : ", abs(100.10))
 运行结果为：
          abs(-40) :  40
          abs(100.10) :  100.1
```
2、dict():dict()函数用于创建一个字典。
返回值：返回字典。
实例：
>>>dict()                        # 创建空字典
{}
>>> dict(a='a', b='b', t='t')     # 传入关键字
{'a': 'a', 'b': 'b', 't': 't'}
>>> dict(zip(['one', 'two', 'three'], [1, 2, 3]))   # 映射函数方式来构造字典
{'three': 3, 'two': 2, 'one': 1
>>> dict([('one', 1), ('two', 2), ('three', 3)])    # 可迭代对象方式来构造字                                               典
{'three': 3, 'two': 2, 'one': 1}
>>>
3、help():help() 函数用于查看函数或模块用途的详细说明。
实例：
![12092767-c16242d04e32faf9.png](https://upload-images.jianshu.io/upload_images/12092767-c16242d04e32faf9.png)
help().png
4、min():min() 方法返回给定参数的最小值，参数可以为序列。
返回值：返回给定参数的最小值。
实例:print("min(20, 10, 50, 100) :" , min(20, 10, 50, 100))
运行结果：min(20, 10, 50, 100) : 10
5、setattr():setattr()函数对应getatt()
