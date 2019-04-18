# Python学习笔记：类的总结 - alw2009的博客 - CSDN博客
2016年09月25日 22:44:32[traveler_zero](https://me.csdn.net/alw2009)阅读数：164
                
1.实例的方法属性可能被重新绑定到数据对象上， 通过命名习惯来避免冲突。
2.每个值都是对象， 可以用object.__class__打印类型信息
3.对C++程序员来说， Python里类的函数属性都是虚函数。
4.多继承的情况下， Python对名字的搜索是动态的， 为了支持super(), 同时线性化
访问， 钻石继承是提高查找效率。
5.定义私有成员， 防止在子类里被重写。
```python
class Mapping:
    def __init__(self, iterable):
        self.items_list = []
        self.__update(iterable)
    def update(self, iterable):
        for item in iterable:
            self.items_list.append(item)
    __update = update   # private copy of original update() method
class MappingSubclass(Mapping):
    def update(self, keys, values):
        # provides new signature for update()
        # but does not break __init__()
        for item in zip(keys, values):
            self.items_list.append(item)
```
6.python for原理
    for需要__iter__-->__next__-->StopIteration
```python
class Reverse:
    """Iterator for looping over a sequence backwards."""
    def __init__(self, data):
        self.data = data
        self.index = len(data)
    def __iter__(self):
        return self
    def __next__(self):
        if self.index == 0:
            raise StopIteration
        self.index = self.index - 1
        return self.data[self.index]
 	
  rev = Reverse('spam')
  for char in rev:
```
7.Generator Expressions
   Generator可以用来产生iterator
   如：
```python
def reverse(data):
    for index in range(len(data)-1, -1, -1):
        yield data[index]
```
   Generator Expression很像List Comprehenssion但用的是小括号，更加内存友好
，用做函数参数可以让代码跟简洁。
            
