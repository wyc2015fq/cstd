# Python中的哈希常识小结 - 小灰笔记 - CSDN博客





2017年08月16日 20:52:27[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：6866








       Python中，哈希是一种将相对复杂的值简化成小整数的计算方式。哈希值可以表示出原值所有的位，有些哈希值会得出非常大的数值，这样的算法通常用于密码学。

       Python中也有基础的模块库可以支持部分哈希的算法。

       不同的平台、不同的系统哈希值的计算可能会不同，这里简单对我自己的电脑做一个试探。写如下测试代码：

x = object()

print(hash(x))

print(id(x))

print(id(x)/hash(x))



x = 123



print(hash(x))

print(id(x))

print(id(x)/hash(x))

       在Win10的Ubuntu系统中运行如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08/16$python hash.py

8776798099977

140428769599632

16

123

13196608

107289

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/02_programme_language/03_python/03_OOP/2017/08/16$python -V

Python 2.7.6

       在Win10下的执行结果如下;

E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08\16>pythonhash.py

138802563341

2220841013456

16.0

123

1407562656

11443598.829268293



E:\01_workspace\02_programme_language\03_python\03_OOP\2017\08\16>python-V

Python 3.6.0

       两个平台同时又是两个不同的软件版本，执行的结果确实是有一点差异。但是，试探的对象创建的例子却跟我在其他地方看见的方式差不多，相应的哈希是通过id除以16实现的。只不过，在py2中的计算是整型，而py3中的计算则是浮点数。



