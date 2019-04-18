# python中list类型的append()和extend()的区别 - bigfacesafdasgfewgf - CSDN博客





2015年03月23日 10:18:53[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2019










    python中的list类型常用的有append()函数和extend()函数。它二者功能类似，可之间又有什么区别呢？




    1. append()



```python
append()用法示例：
>>> mylist = [1,2,0,'abc']
>>> mylist
[1, 2, 0, 'abc']
>>> mylist.append(4)
>>> mylist
[1, 2, 0, 'abc', 4]
>>> mylist.append('haha')
>>> mylist
[1, 2, 0, 'abc', 4, 'haha']
```





    2. extend()






```python
extend()用法示例：
>>> mylist
[1, 2, 0, 'abc', 4, 'haha']
>>> mylist.extend(['lulu'])
>>> mylist
[1, 2, 0, 'abc', 4, 'haha', 'lulu']
>>> mylist.extend([aaa,'lalalala'])
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
NameError: name 'aaa' is not defined
>>> mylist.extend(['123123','lalalala'])
>>> mylist
[1, 2, 0, 'abc', 4, 'haha', 'lulu', '123123', 'lalalala']
>>> mylist.extend([111111,222])
>>> mylist
[1, 2, 0, 'abc', 4, 'haha', 'lulu', '123123', 'lalalala', 111111, 222]
```



分析可以发现：

append()只能在list中添加一个元素；

extend()只能添加另外一个list。












