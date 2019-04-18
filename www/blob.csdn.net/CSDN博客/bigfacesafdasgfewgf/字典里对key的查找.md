# 字典里对key的查找 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 16:45:47[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：450
个人分类：[Python](https://blog.csdn.net/puqutogether/article/category/2595125)








首先构建一个dict类型，可以直接初始化：

```python
phonebook = {'你': '2341', '是': '9102', '谁': '3258'}
if '2341' in phonebook:
    print 'abc'
```




也可以先定义再赋值：


```python
dict1={}
if 'a' not in dict1: 
     dict1[a]=1  #这种方式就可以增加字典里面的元素了，比list要来的方便
```




这里需要注意的是，查找字典里面的key我们既可以使用dict1.has_key('a')函数，也可以直接用if 'a' not in dict1 和if 'a' in dict1。




另外说一下list如何添加元素：

list1.append(‘a’) 

即使用append函数







字典的定义和初始化还可以使用：

data={i: randn() for i in range(7)}




