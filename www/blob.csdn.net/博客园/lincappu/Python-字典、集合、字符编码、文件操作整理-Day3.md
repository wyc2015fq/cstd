# Python-字典、集合、字符编码、文件操作整理-Day3 - lincappu - 博客园







# [Python-字典、集合、字符编码、文件操作整理-Day3](https://www.cnblogs.com/lincappu/p/8157507.html)





## 1、字典

### 1.1、为什么有字典：

有个需求，存所有人的信息 这时候列表就不能轻易的表示完全
names = ['stone','liang']

### 1.2、元组：


定义符号()
t = （1,2,3）
tuple 是元组的意思
列表与元组不一样的地方是它 元组不可以修改
元组其实跟列表差不多，也是存一组数，只不是它一旦创建，便不能再修改，所以又叫只读列表

```cpp
语法  
names = ("alex","jack","eric")
```

它只有2个方法，一个是count,一个是index，完毕。

### 1.3、列表：

list = [1,2,3,4]
用列表去查复杂的信息和取数据就很麻烦
利用字典：存储更复杂的数据

### 1.4、定义字典：

定义：｛key1:value1,key2:value2｝,key-value结构，key必须可hash
特性：
- 可存放多个值
- 可修改指定key对应的值，可变
- 
无序

dic = {'key':'value','age':'18'}
字典的元素是由key:value 组成的
dic= {} --> dict() --> **init**()


### 1.5、小技巧：

按ctrl 直接到对应函数的操作成员列表内

```
>>> dic4 = dict(name='stone',age = 18)
>>> print(dic4)
{'age': 18, 'name': 'stone'}
>>> dic5 = dict({'name':'stone','age':18})
>>> print dic5
  File "<stdin>", line 1
    print dic5
             ^
SyntaxError: Missing parentheses in call to 'print'
>>> print(dic5)
{'age': 18, 'name': 'stone'}
```

## 2、字典的基本操作：

### 2.1、查询

```
dic['key']
dic['name']
直接在字典里面找KEY 取到value
列表的查询：
print(dic["namee"])
没有的话会直接报错
但是字典get不会报错
print(dic.get('naness'))
输出none
```

### 2.2、增加

```
字典中添加是无序的添加
dic['gender']='female'
列表是有序的
```

### 2.3、改

```
dic['name'] = 'stone'
改的是value
```

### 2.4、删除

```python
del 万能的
del dic['name']
字典里面怎么定义的key 调用的时候就是那么写
```

### 2.5、复制：copy 浅copy

```
dic1 = dic.copy()
print(dic1)
```

### 2.6、深入 copy

```
import copy
dic3 = copy.deepcopy(dic)
copy.copy是浅copy
```

### 2.7、快速生成字典fromkeys

```python
dic= {'name':'stone','age':18}
data = dic.fromkeys([1,2,3])
print(date)
输出：
{1: None, 2: None, 3: None}
生成的字典跟声明的字典dic没有关系
```

### 2.8、items

```
print(dic.items())
for i in dic.items():
print(i)
```

### 2.9、key value形式

```
keys将字典中所有的key取出  
将所有的keys以列表的形式显示出来  
for i in dic.keys():
    print('keys is %s,value is %s' %(i,dic[i]))
以列表和元组的形式显示出来
```

### 2.10、删除pop()

```
dic.pop(keys)
注意是只能是填写keys
pop(keys)必须存在才能删除
```

### 2.11、随机删除popitem()

```
随机删除key value
这个函数不需要函数随机删除
```

### 2.12、setdefault

```
dic.setdefault('gender',[]).append('mail') #dic['gender']=[]
或者是
dic['gender'].appemd('female')
```

### 2.13、更新update

```python
方法一：
dic1={'gender':'male','name':'lhf'}
dic.update(dic1)
print(dic)
方法二：
可以根据key进行更新
dic.update(name='stone',gender='male')
print(dic)
```

### 2.14、values取所有的值

```
for i in dic.values(dic1)
    print(i)
```

## 3、字典key的定义规则：

### 3.1、可变的原理：

> 
通过id()进行内存调用查看是否是一个内存空间
字符串和数字是不可变的
列表：是可变的类型 是指列表里面包含的元素可变


### 3.2、key的定义规则
- 必须是不可变的，不可变的类型：数字,字符串,元组
- 
通过内置方法hash 判断一个数据类型是否可变

> 
hash('sdfas')
-41789881
hash['s'] #hash 列表
Traceback (most recent call last):
File "


- 
字典当中的key是唯一的：不能是重复的


#### 判断一个字典里有没有这个值

菜单思路：
首先一个死循环

### 数据类型的小结分类说明

## 4、集合

### 4.1、定义

由不同元素组成的集合，集合中是一组无序排列的可hash值，可以作为字典的key

#### 4.1.1、集合的创建

```bash
{1,2,3,1}
或
定义可变集合set
>>> set_test=set('hello')
>>> set_test
{'l', 'o', 'e', 'h'}
改为不可变集合frozenset
>>> f_set_test=frozenset(set_test)
>>> f_set_test
frozenset({'l', 'e', 'h', 'o'})
```

### 4.2、特性
- 集合的目的是将不同的值存放到一起，不同的集合间用来做关系运算，无需纠结于集合中单个值
- 
元素唯一：

例子：
s1= {'a',1,2,'b','c','c'}
print(s1)
不会有相同的元素


### 4.3、集合的关系运算

```python
in
not in
＝＝
！＝
<,<=
>,>=
|,|=:合集
&.&=:交集
－,－=:差集
^,^=:对称差分
```

#### 4.3.1、交集
`查看不同集合中共有的元素：s1&s2（集合的内置方法）== intersection()`
#### 4.3.2、并集

```cpp
s1|s2 == union()
```

#### 4.3.3、差集
`s1-s2 == `
#### 4.3.4、对称差集
`把多个集合的交集去除s1^s2`
#### 4.3.5、子集
`s2 <= s1 s2是s1的子集`
#### 4.3.6、父集
`s2 => s1 s1是s2的子集`
### 4.4、集合其他的内置方法

#### 4.4.1、update

```python
s1.update('2')
s1.update((1,2,3,4))
s2 = {'h','2','1'}
s1.update(s2)
s1.update('hello')
```

#### 4.4.2、增加

```python
s1.add('hello')
print(s1)
```

#### 4.4.3、随机删除pop()

#### 4.4.4、指定删除

```python
remove()
s1.remove('a')
s1.remove('w')
print(s1)
以上用remove()没有元素会报错：
不报错删除：
s1:discard('w')
用以上的会默认输出none 不会抛出异常
```

#### 4.4.5、拷贝：

```
copy 跟字典是一样的
difference_update s1 =s1 -s2
```

### 4.5、集合工厂函数

```python
s = set([3,5,9,10])      #创建一个数值集合    
t = set("Hello")         #创建一个唯一字符的集合  
a = t | s          # t 和 s的并集   
b = t & s          # t 和 s的交集   
c = t – s          # 求差集（项在t中，但不在s中）   
d = t ^ s          # 对称差集（项在t或s中，但不会同时出现在二者中）  
    
基本操作：    
t.add('x')            # 添加一项   
s.update([10,37,42])  # 在s中添加多项   
使用remove()可以删除一项：  
t.remove('H')    
len(s)  
set 的长度  
x in s  
测试 x 是否是 s 的成员   
x not in s  
测试 x 是否不是 s 的成员  
  
s.issubset(t)  
s <= t  
测试是否 s 中的每一个元素都在 t 中  
  
s.issuperset(t)  
s >= t  
测试是否 t 中的每一个元素都在 s 中  
  
s.union(t)  
s | t  
返回一个新的 set 包含 s 和 t 中的每一个元素  
  
s.intersection(t)  
s & t  
返回一个新的 set 包含 s 和 t 中的公共元素  
  
s.difference(t)  
s - t  
返回一个新的 set 包含 s 中有但是 t 中没有的元素  
  
s.symmetric_difference(t)  
s ^ t  
返回一个新的 set 包含 s 和 t 中不重复的元素  
  
s.copy()  
返回 set “s”的一个浅复制

class set(object):
"""
set() -> new empty set object
set(iterable) -> new set object

Build an unordered collection of unique elements.
"""
def add(self, *args, **kwargs): # real signature unknown
    """
    Add an element to a set.
    
    This has no effect if the element is already present.
    """
    pass

def clear(self, *args, **kwargs): # real signature unknown
    """ Remove all elements from this set. """
    pass

def copy(self, *args, **kwargs): # real signature unknown
    """ Return a shallow copy of a set. """
    pass

def difference(self, *args, **kwargs): # real signature unknown
    """
    相当于s1-s2
    
    Return the difference of two or more sets as a new set.
    
    (i.e. all elements that are in this set but not the others.)
    """
    pass

def difference_update(self, *args, **kwargs): # real signature unknown
    """ Remove all elements of another set from this set. """
    pass

def discard(self, *args, **kwargs): # real signature unknown
    """
    与remove功能相同，删除元素不存在时不会抛出异常
    
    Remove an element from a set if it is a member.
    
    If the element is not a member, do nothing.
    """
    pass

def intersection(self, *args, **kwargs): # real signature unknown
    """
    相当于s1&s2
    
    Return the intersection of two sets as a new set.
    
    (i.e. all elements that are in both sets.)
    """
    pass

def intersection_update(self, *args, **kwargs): # real signature unknown
    """ Update a set with the intersection of itself and another. """
    pass

def isdisjoint(self, *args, **kwargs): # real signature unknown
    """ Return True if two sets have a null intersection. """
    pass

def issubset(self, *args, **kwargs): # real signature unknown
    """ 
    相当于s1<=s2
    
    Report whether another set contains this set. """
    pass

def issuperset(self, *args, **kwargs): # real signature unknown
    """
    相当于s1>=s2
    
     Report whether this set contains another set. """
    pass

def pop(self, *args, **kwargs): # real signature unknown
    """
    Remove and return an arbitrary set element.
    Raises KeyError if the set is empty.
    """
    pass

def remove(self, *args, **kwargs): # real signature unknown
    """
    Remove an element from a set; it must be a member.
    
    If the element is not a member, raise a KeyError.
    """
    pass

def symmetric_difference(self, *args, **kwargs): # real signature unknown
    """
    相当于s1^s2
    
    Return the symmetric difference of two sets as a new set.
    
    (i.e. all elements that are in exactly one of the sets.)
    """
    pass

def symmetric_difference_update(self, *args, **kwargs): # real signature unknown
    """ Update a set with the symmetric difference of itself and another. """
    pass

def union(self, *args, **kwargs): # real signature unknown
    """
    相当于s1|s2
    
    Return the union of sets as a new set.
    
    (i.e. all elements that are in either set.)
    """
    pass

def update(self, *args, **kwargs): # real signature unknown
    """ Update a set with the union of itself and others. """
    pass
```

## 5、字符编码

## 5.1、 什么是字符编码

计算机要想工作必须通电,也就是说‘电’驱使计算机干活,而‘电’的特性，就是高低电压(高低压即二进制数1,低电压即二进制数0),也就是说计算机只认识数字

编程的目的是让计算机干活，而编程的结果说白了只是一堆字符，也就是说我们编程最终要实现的是：一堆字符驱动计算机干活

所以必须经过一个过程：

　　字符串--------（翻译过程）------->数字

这个过程实际就是一个字符如何对应一个特定数字的标准，这个标准称之为字符编码

```
1.  内存固定使用unicode，硬盘的编码（可以修改的软件编码）
2.  使用什么编码存储，就用什么编码读取
3.  程序运行分两个阶段，1.从硬盘读到内存 2. python 解释器运行已经读到内存的代码
4.  针对一个文件来说，python 与nodpad++ 区别是多了第二个步骤
5.  coding 指定字符编码 是在告诉解释器 使用什么字符编码读取文件
```

## 6、文件处理

### 6.1、文件处理流程

```
1. 打开文件，得到文件句柄并赋值给一个变量
2. 通过句柄对文件进行操作
3. 关闭文件
```

### 6.2、文件操作基本流程

```
f = open('a.txt') #打开文件
first_line = f.readline()  #读取文件第一行
print('first line:',first_line) #打印第一行
print('华丽分隔线'.center(50,'-'))
data = f.read()   # 读取剩下的所有内容,文件大时不要用
print(data)       #打印读取内容
f.close()         #关闭文件
```

### 6.3、详细操作

```
data = open("test").read()
read() 将文件内容全部读取出来
打开文件的时候应该先定义变量：
f = open(test)
print(f.readline())
strip() 会把整行的开头末尾的空格和换行去除
f.readline()一行一行读，从上一行
f.readlines() 会将文件内容输出 以列表展示
```

### 6.4、实现循环读取文件部分内容：

方法一：不要用readlines()

```
for index,line in enumerate(f.readlines()):
    if index <5:
        print(line.strip())
    else:
        break

#####enumerate 只能用于列表
```

方法二：

```
for i in range(5):
    print(f.readline().strip())
```

方法三：

```
for line in f:
    print(line.strip())
一行行处理

line_nu = 0
for line in f:
    if line_nu < 5:
        print(line.strip())
        line_nu +=1
    else:
        break
```

### 6.5 文件操作模式

文件句柄 = open('文件路径', '模式')

> 
打开文件时，需要指定文件路径和以何等方式打开文件，打开后，即可获取该文件句柄，日后通过此文件句柄对该文件操作。


打开文件的模式有：

```
r ，只读模式【默认模式，文件必须存在，不存在则抛出异常】
w，只写模式【不可读；不存在则创建；存在则清空内容】
x， 只写模式【不可读；不存在则创建，存在则报错】
a， 追加模式【可读；   不存在则创建；存在则只追加内容】
```

"+" 表示可以同时读写某个文件

```
r+， 读写【可读，可写】
w+，写读【可读，可写】
x+ ，写读【可读，可写】
a+， 写读【可读，可写】
```

"b"表示以字节的方式操作

```
rb  或 r+b
wb 或 w+b
xb 或 w+b
ab 或 a+b
```

注：以b方式打开时，读取到的内容是字节类型，写入时也需要提供字节类型，不能指定编码

### 6.6 文件操作示例

#### 6.6.1 写 "w"

```
f = open("myfile","w")
f.write("我爱北京天安门\n")
f.write("我爱烫头\n")
f.close()
以上输入会以一行的形式显示。换行添加“\n”

注意：w模式会将原文件内容清空并写入新数据！！！
w：创建文件并清空该文件内容
###没有声明模式：默认模式是读模式 r
```

#### 6.6.2、a：追加

```
f = open("myfile",'a')
f.write("我爱\n")
f.close
追加文件内容结尾
```

#### 6.6.3、混合模式：可读可写

```
读写：r+
写读：w+
追加读：a+
insert的内容会 追加到最后面
```

#### 6.6.4、closed判断文件是否关闭
`print(f.closed)`
#### 6.6.5、打印文件的字符集编码
`print(f.encoding)`
#### 6.6.6、操作系统打开文件的索引
`print(f.fileno())`
#### 6.6.7、强制将内存内容刷到磁盘

```
f.flush()
实时的刷新内存内容到磁盘
```

flush原理：

```
文件操作是通过软件将文件从硬盘读到内存写入文件的操作也都是存入内存缓冲区buffer  
（内存速度快于硬盘，如果写入文件的数据都从内存刷到硬盘，
内存与硬盘的速度延迟会被无限放大，效率变低，
所以要刷到硬盘的数据我们统一往内存的一小  块空间即buffer中放，
一段时间后操作系统会将buffer中数据一次  性刷到硬盘）  
flush即，强制将写入的数据刷到硬盘
```

滚动条：

```
import sys,time

for i in  range(10):
    sys.stdout.write('#')
    sys.stdout.flush()
    time.sleep(0.2)
```

#### 6.6.8、判断是否是终端文件，不可读
`f.isatty()`
#### 6.6.9、打印文件名
`f.name :打印文件名`
#### 6.6.10、按字符寻找

```
f.seek()#寻找
f.seek(10)
print(f.readline())
找到什么位置（以字符为n）
```

#### 6.6.11、告诉当前光标位置

```
print(f.tell())
print(f.readline())
```

#### 6.6.12、f.truncate()从光标位置截断并移除后面的所有内容

```
f.truncate() #截断 
注意需要打开文件执行
对于文本的截断没有什么卵用
对于二进制文件进行截断 断点续传
```

#### 6.6.13、f.writelines()

```
data = ["stone\n","liang"]
将列表里面的值 追加到内容前
```

### 6.7、执行脚本传入参数

> 
Python有大量的模块，从而使得开发Python程序非常简洁。类库有包括三中：
Python内部提供的模块
业内开源的模块
程序员自己开发的模块
Python内部提供一个 sys 的模块，其中的 sys.argv 用来捕获执行执行python脚本时传入的参数


```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import sys
 
print sys.argv
```












