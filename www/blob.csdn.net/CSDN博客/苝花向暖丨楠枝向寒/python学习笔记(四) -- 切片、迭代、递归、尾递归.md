# python学习笔记(四) -- 切片、迭代、递归、尾递归 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月09日 22:54:53[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：57
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



## 递归

```
def fact(n):
    if n==1:
        return 1
    return n * fact(n - 1)
```

上面就是一个简单的递归案例，但是这种写法性能很低。

有一种优于上面的写法，叫做尾递归。说白了，就是return的时候 不进行数学运算，仅仅调用自身的函数，就如上面的案例

n * fact(n - 1) ，在fact(n-1)返回结果之前，n这个变量的值是需要一直保存在内存当中的。

所以说应该尽量避免这种情况的出现，如下面这种

```
def fact(n):
    return fact_iter(n, 1)

def fact_iter(num, product):
    if num == 1:
        return product
    return fact_iter(num - 1, num * product)
```

将结果计算出来以后作为参数传递下去，但是python以及java都没有对尾递归进行优化，所以当 递归深度大于等于1000时会导致栈溢出。

## 切片（类似于java中的subString（）、subList（））

切片即提取出 list、string、tuple中的一部分

```
>>> L = list(range(100))    生成100个数，从0到99
>>> L
[0, 1, 2, 3,....,98, 99]
>>> L[0:5]   从 第一个截取到第五个，注意切片是从0开始的，后面的5即代表第6个元素（左包右不包）
[0, 1, 2, 3, 4]
>>> L[:5]   前面的不写代表从 第一个开始截取
[0, 1, 2, 3, 4]
>>> L[95:]  后面的不写代表 截取到最后一个
[95, 96, 97, 98, 99]
>>> L[95:99]   
[95, 96, 97, 98]
>>> L[:10:2]  从第一个开始，一直到第十个元素，每2个元素提取出来一个
[0, 2, 4, 6, 8]
>>> T = tuple(L)  元组也支持 切片操作
>>> T
(0, 1, 2, 3, ...., 98, 99)
>>> T[::5]
(0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95)
>>> a = 'ABCDEG'  字符串也支持
>>> a[:5]
'ABCDE'
>>> a[:-1]    -1代表最后一个元素，-2代表倒数第二个 ，以此类推
'ABCDE'
>>> a[::2]
'ACE'
>>> a[1:5:2]
'BD'
```

我在网上看到一个涉及切片和递归的小案例，实现取出字符串前后的空字符

```
>>> def trim(s):
    print (s)
	if s[:1] != ' ' and s[-1:] != ' ':  如果第一个字符和最后一个字符均不为空则直接返回
		return s
	elif s[:1] == ' ':   如果第一个字符为空，则去除第一个字符，然后递归继续调用
		return trim(s[1:])
	else :               如果第一个字符不为空，只能说明最后一个字符为空，去除最后一个字符，递归
		return trim(s[:-1])

	
>>> a = ' hello world  '
>>> trim(a)
   hello world  
  hello world  
 hello world  
hello world  
hello world 
hello world
'hello world'
```

可以很清晰的看出是先去除前面的空格，然后再去除后面的空格。

## 迭代 （就是遍历）

主要说 遍历 dict 即 java中的map，别的没什么好说的

python中的 for循环 特别类似 java中的 for....each

同样也是不光可以遍历有索引位的，还可以遍历没有索引位的。

我们可以通过以下方法来判断一个数据类型是否可以被迭代

```
>>> from collections import Iterable
>>> isinstance('abc',Iterable)
True
>>> isinstance([1,2,3],Iterable)
True
>>> isinstance({},Iterable)
True
>>> isinstance(123,Iterable)
False
```

下面演示 迭代 dict

```
>>> d = {'a':1,'b':2,'c':3}    默认遍历的是 dict 的 key
>>> for key in d:              
...     print key
... 
a
c
b

>>> for value in d.values():   通过d.values()方法 来遍历 value
...     print value
... 
1
3
2

>>> for k,v in d.items():    也可以同时遍历，类似java中的entrySet
...     print k,v 
... 
a 1
c 3
b 2
```

如果想获得类似java中的下标可以通过如下的方法

```
>>> for i,value in enumerate(['A','B','C']):
...     print(i,value)
... 
(0, 'A')
(1, 'B')
(2, 'C')
```

如果数据是一个 list<tuple>，可以通过下面的方式遍历

```
>>> for x,y in [(1,1),(2,4),(3,9)]:
...     print(x,y)
... 
(1, 1)
(2, 4)
(3, 9)
```

