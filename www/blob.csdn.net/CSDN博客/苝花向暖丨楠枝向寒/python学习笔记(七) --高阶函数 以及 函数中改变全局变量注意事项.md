# python学习笔记(七) --高阶函数 以及 函数中改变全局变量注意事项 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月13日 22:52:00[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：137
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**Table of Contents**

[map()函数](#map()%E5%87%BD%E6%95%B0)

[reduce()函数](#reduce()%E5%87%BD%E6%95%B0)

[函数中改变全局变量注意事项](#%E5%87%BD%E6%95%B0%E4%B8%AD%E6%94%B9%E5%8F%98%E5%85%A8%E5%B1%80%E5%8F%98%E9%87%8F%E6%B3%A8%E6%84%8F%E4%BA%8B%E9%A1%B9)

[filter函数](#filter%E5%87%BD%E6%95%B0)

[sorted 函数](#sorted%20%E5%87%BD%E6%95%B0)

[sorted 和 sort的区别](#sorted%20%E5%92%8C%20sort%E7%9A%84%E5%8C%BA%E5%88%AB)

## map()函数

map需要2个或2个以上的参数

map函数的第一个参数：要调用的方法名，注意方法名后面不需要加 小括号

map函数后面的参数：调用参数一的方法时需要传入的参数，这个参数必须是可以被for循环遍历的，即必须是Iterable对象。

例如

def fn(x,y)

map(方法名fn , 调用fn方法需要传递的第一个参数，调用fn方法需要传递的第二个参数)

map函数会同时遍历 除了第一个参数以外的参数，并同时传入参数一的方法当中。

如下所示

```python
>>> a = map(func,"123","123")
>>> a
<map object at 0x0000000002BC7518>  #map对象无法直接打印出来
>>> type(a)
<class 'map'>
>>> isinstance(a,Iterator)  #我们可以看到map是Iterator对象
True
>>> for n in a:   #那我们就可以通过for循环将其遍历并打印
	print(n)

11
22
33
>>> list(a)     #也可以通过 list()转换成list
[]              #值得注意的是，for循环去遍历Iterator对象，实际上调用的是next(),而list()也是调用    
                 next()方法去迭代，然后将迭代的结果放入列表当中，在调用list()之前，对象a已经指向 
                 最尾部，所以list()返回来的是一个空列表        
               

>>> a = map(func,"123","123") #重新给对象a赋值
>>> list(a)  
['11', '22', '33']
```

map函数传入 2个参数，那么就只遍历第二个参数，然后调用第一个参数

```python
#一个简单的方法可以用lambda表达式来代替
#格式： lambda 方法参数 : 方法返回值
>>> p = map(lambda x:x*x,[n for n in range(5)]) 
>>> list(p)
[0, 1, 4, 9, 16]
```

## reduce()函数

reduce()函数规定必须且只能有两个参数

参数一：与map函数一样是方法名后面不用跟小括号

参数二：一个Iterable对象

reduce函数与递归的执行过程是一样的

直接上代码

```python
>>> def fn(x,y):
	return x+y

>>> a = reduce(fn,[1,2,3,4,5])
>>> a
15
```

执行过程分析：

遍历[1,2,3,4,5] 取出 1和2作为 参数调用 fn(1,2) 返回 3

返回的结果3作为参数，再取出[1,2,3,4,5]中的下一个即 3作为参数调用fn(3,3) 返回 6 

再次调用就是 fn(6,4) 返回 10， 再次调用是fn(10,5) 返回15

简单表达就是 fn(fn(fn( fn(1, 2), 3), 4), 5)

转换为方法来体现如下：

```python
>>> def fn(x,y):
	if(y==5):
		return (x+y)
	return fn(x+y,y+1)

>>> fn(1,2)
15
```

 网上找了个 map和reduce结合使用，实现字符串转成int类型的小案例

```python
DIGITS = {'0': 0, '1': 1, '2': 2, '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9}

def char2num(s):
    return DIGITS[s]

def str2int(s):
    return reduce(lambda x, y: x * 10 + y, map(char2num, s))
```

## 函数中改变全局变量注意事项

下面这样写没毛病

```python
>>> a = 5   #全局变量
>>> def func(b=1,c=2):
	a = b+c   #系统认为这个a和上面那个a不是一个，这个为局部变量
	print(a)  

	
>>> func()   #函数内的a为3
3
>>> a #全局变量的a还是为5
5
```

下面这样写就会出现问题

```python
>>> def func(b=1,c=2):
	print(a)  #问题出在这一行，我这里打印变量a，python就会在函数内找名字为a的这个变量，没有找到，    
              #就会去函数外找名为a的全局变量，显然我们之前定义了a=5，所以它找到了。
	a = b+c   #但这一行又声明了局部变量a，这种局部变量与全局变量名相同时，在给局部变量赋值前，是不        
              #允许出现对该变量名的引用的
	print(a)

	
>>> func()
Traceback (most recent call last):
  File "<pyshell#9>", line 1, in <module>
    func()
  File "<pyshell#8>", line 2, in func
    print(a)
UnboundLocalError: local variable 'a' referenced before assignment
```

通过上面的案例或许会有疑惑，那如果我想更改全局变量a的值应该怎么做？

可以通过global关键字来实现

```python
>>> def func(b=1,c=2):
	print (b,c)
	global a   #网上有人说global必须在函数的第一行，实际不用，只需要在引用变量a之前就行
	print(a)
	a = b+c
	print(a)

>>> func()
1 2
5
3
```

如果在global的关键字上面出现了 全局变量名就会报错 

```python
>>> def func(b=1,c=2):
	print (b,c)
	a = b+c
	print(a)
	global a
	print(a)
	
SyntaxError: name 'a' is used prior to global declaration
```

## filter函数

可以用作按需筛选出符合条件的元素

filter 和 map非常类似，但是filter的参数中只允许有一个Iterable对象。

filter会遍历这个Iterable对象作为参数去调用 第一个参数的方法，通过方法返回值是True或者是false来决定是否去除或保留该元素。

下面去除空字符串的案例 

```python
>>> def not_empty(s):
	return s and s.strip()  # 在python中 空字符串、空list、空tuple、空dict、空set均为False
                            # A and B，A为false返回A，否则返回B
>>> list(filter(not_empty,['A','','B',None,'C',' '])) #filter会根据返回的数据来判断是Ture或false，是Ture就保留，是False就删除
['A', 'B', 'C']
```

下面是取出奇数的案例

```python
>>> def is_odd(n):
    return n % 2 == 1  #这里也可以直接返回True或False

>>> list(filter(is_odd, [1, 2, 4, 5, 6, 9, 10, 15]))
[1, 5, 9, 15]
```

在 [这里](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001431821084171d2e0f22e7cc24305ae03aa0214d0ef29000) 有一个生成素数的案例

重点说明一个不好理解的问题

```python
def _not_divisible(n):
    return lambda x: x % n > 0

def primes():
    yield 2
    it = _odd_iter() 
    while True:
        n = next(it) 
        yield n
        it = filter(_not_divisible(n), it) # 疑问行
```

看了上面的案例，刚接触python的人肯定会有疑问

疑问一：在filter函数中的第一个参数位置，之前都是方法名不写小括号也不传入参数，但是这里传入了一个参数

疑问二：func(n)方法中，有两个需要赋值的变量，一个是x，一个是n，n就是调用时传递的参数，那么x是如何赋值的呢

我自己写了个案例来测试

```python
>>> def func(n):
	return lambda x : x+n   
>>> a = map(func(5),[1,2,3,4,5])   #将func(n)中的 n 赋值为5，x为[1,2,3,4,5] 遍历出来的值
>>> for n in a:
	print(n)
	
6       # 1+5
7       # 2+5
8       # 3+5
9       # 4+5
10      # 5+5
```

```python
>>> def func(n):
	return lambda n : n       #func中除了变量n没有其他变量

>>> a = map(func(5),[1,2,3,4,5]) #列表遍历出来的值不会传入func中
>>> for n in a:
	print(n)

	
1
2
3
4
5
```

```python
>>> def func(n):
	return x+n     #没有lambda表达式无法接受到x的值

>>> a = map(func(5),[1,2,3,4,5])
Traceback (most recent call last):
  File "<pyshell#159>", line 1, in <module>
    a = map(func(5),[1,2,3,4,5])
  File "<pyshell#158>", line 2, in func
    return x+n
NameError: name 'x' is not defined
```

解惑一：map、reduce、filter 会自动的将 后面的Iterable类型的参数遍历传入第一个参数位置的方法中，但是如果我们手动传入一个参数，优先用我们自己传的作为被调用方法参数。

解惑二：如果被调用的函数中 return 语句包含 lambda表达式，且表达式中有需要被赋值的变量但被调用的函数中又没有对该变量进行定义，那么将会用map、reduce、filter函数遍历的列表的值作为该变量的值。

## sorted 函数

sorted可以对Iterable对象进行排序，下面以list和dict进行演示

```python
>>> sorted([36, 5, -12, 9, -21])  #默认从小到大
[-21, -12, 5, 9, 36]
>>> sorted([36, 5, -12, 9, -21],reverse=True) #reverse=True倒序排序
[36, 9, 5, -12, -21]
>>> sorted([36, 5, -12, 9, -21], key=abs)  #key=abs 以绝对值排序
[5, 9, -12, -21, 36]
>>> sorted(['bob', 'about', 'Zoo', 'Credit']) #对字符串排序根据ASCII码来判断顺序
['Credit', 'Zoo', 'about', 'bob']  # 大写字母ASCII小于小写字母ASCII，a 小于 z

#下面是忽略大小写对字符串进行排序
>>> sorted(['bob', 'about', 'Zoo', 'Credit'],key=str.lower) #将所有字母小写再排序
['about', 'bob', 'Credit', 'Zoo']
>>> sorted(['bob', 'about', 'Zoo', 'Credit'],key=str.upper) #将所有字母大写再排序 
['about', 'bob', 'Credit', 'Zoo']
>>> sorted(['bob', 'about', 'Zoo', 'Credit'],key=str.capitalize) #将首字母大写再排序
['about', 'bob', 'Credit', 'Zoo']

#下面对dict进行排序
>>> a = {'e':'3','d':'4','c':'5'} 
>>> sorted(a)   #直接使用出现问题，按照key进行排序，且只能获取到key
['c', 'd', 'e']
#进行改进
>>> def func(t): #按照key排序
	return t[0]

>>> def function(t): #按照value排序
	return t[1]
>>> sorted(a.items(),key=func)  #让 key=方法名，方法的返回值即排序标准的关键字
[('c', '5'), ('d', '4'), ('e', '3')]
>>> sorted(a.items(),key=function)
[('e', '3'), ('d', '4'), ('c', '5')]

#进行优化
>>> sorted(a.items(),key=lambda x : x[0]) #x代表一个键值对，x[0]键值对中的键(key)
[('c', '5'), ('d', '4'), ('e', '3')]
>>> sorted(a.items(),key=lambda x : x[1]) #x[1]键值对中的值(value)
[('e', '3'), ('d', '4'), ('c', '5')]

#将dict以key排序，并放入list
>>> [(k,a[k]) for k in sorted(a.keys())]
[('c', '5'), ('d', '4'), ('e', '3')]

#对 list里面是tuple 进行排序
>>> a = [(1,'a'),(3,'b'),(2,'c'),(5,'d'),(4,'e')]
>>> sorted(a,key=lambda x : x[0])
[(1, 'a'), (2, 'c'), (3, 'b'), (4, 'e'), (5, 'd')]
>>> sorted(a,key=lambda x : x[1])
[(1, 'a'), (3, 'b'), (2, 'c'), (5, 'd'), (4, 'e')]
```

## sorted 和 sort的区别

```
>>> help(a.sort)
Help on built-in function sort:

sort(*, key=None, reverse=False) method of builtins.list instance
    Stable sort *IN PLACE*.

>>> help(sorted)
Help on built-in function sorted in module builtins:

sorted(iterable, /, *, key=None, reverse=False)
    Return a new list containing all items from the iterable in ascending order.
    
    A custom key function can be supplied to customize the sort order, and the
    reverse flag can be set to request the result in descending order.
```

1:使用范围不同

sorted可以对所有Iterable对象进行排序

sort是list对象专属的方法

2:返回值不同

sorted将原数据copy一份，然后对新数据进行排序并返回

sort 在原有的数据直接进行修改，没有返回值

