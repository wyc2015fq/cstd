# python函数式编程lambda,filter,map,reduce的使用

## 

## lambda

lambda是python的一种很有趣的语法，可以快速定义单行的最小函数。来源于Lisp。特点：

- 匿名函数；
- 可带参数大于等于0个。理论上可以接受任意多个参数，实际操作时尽量不要传入太多东西。好的做法还是另外定义一个普通函数，然后想让它多长就多长；
- 使用在需要封装特殊的，非重用代码上，避免代码中出现大量的单行函数

### 使用lambda替代单行函数

```python
1 def funs(x):
2     print x*2
3 
4 f = lambda x: x*2
5 
6 print funs(2) # 4
7 print f(2) # 4
```

### 使用多个参数

```python
1 def add(x, y):
2   return x+y
3 
4 add_lambda = lambda x, y: x + y
5 most_args = lambda *z: z
6 
7 print add(2, 4) # 6
8 print add_lambda(2, 4) # 6
9 print most_args(1, 2, "most") # (1, 2, 'most')
```

### 实现str.title()功能

将句子中单词开始字符转成大写字母,剩余的字符都是小写的,但是

```python
1 "this's is blog".title() # "This'S Is Blog"
```

这里面单引号后面的也大写了。 使用正则表达式和lambda解决此问题：

```python
1 import re
2 
3 def titlecase(s):
4   return re.sub(r"[A-Za-z]+('[A-Za-z]+)?", lambda mo: mo.group(0)[0].upper() + mo.group(0)[1:].lower(), s)
5 
6 titlecase("this's is blog") #"This's Is Blog"
```

PS:也可以使用string模块下面capwords方法

```python
1 import string
2 
3 string.capwords("this's is blog") #"This's Is Blog"
```

## 内建函数filter(),map(),reduce()

### filter(func, seq)

调用一个布尔函数 func 来迭代遍历每个 seq 中的元素；返回一个使 func 返回值为 ture 的元素的序列。下图很好的反映了filter函数的流程：

![filter](http://farm8.staticflickr.com/7425/9416185128_1e702ed691.jpg)

例子：获取奇数

```python
1 from random import randint as ri
2 
3 allNums = []
4 
5 for eachNum in range(9):
6   allNums.append(ri(1, 99))
7 
8 print filter(lambda n: n%2, allNums)
```

### map(func, seq1[, seq2…])

将函数 func 作用于给定序列（s)的每个元素，并用一个列表来提供返回值；如果 func 为 None， func 表现为一个身份函数，返回一个含有每个序列中元素集合的 n 个元组的列表。

例子：将π转换为具体的度数。 下图很好阐释了以下代码

```python
1 map(lambda x: (x*180) ,[ri(1, 99) for i in range(6)])
```

![map1](http://farm6.staticflickr.com/5446/9416185390_433fdc5dd9.jpg)

例子：接受多个序列

```python
1 map(lambda x, y: x + y, [1,3,5], [2,4,6]) #[3, 7, 11]
2 map(None, [1,3,5], [2,4,6]) #[(1, 2), (3, 4), (5, 6)]
```

其中接受None函数的map与zip比较相似

```python
zip([1,3,5], [2,4,6]) #[(1, 2), (3, 4), (5, 6)]
```

![map2](http://farm4.staticflickr.com/3768/9416185224_dd40a3df4b.jpg)

### reduce(func, seq[, init])

将二元函数作用于 seq  序列的元素，每次携带一对（先前的结果以及下一个序列元素），连续的将现有的结果和下一个值作用在获得的随后的结果上，最后减少我们的序列为一个单一的返回值；如果初始值  init 给定，第一个比较会是 init 和第一个序列元素而不是序列的头两个元素。

例子：求和,求阶乘

```python
1 print 'the total is:', reduce((lambda x,y: x+y), range(5)) #the total is: 10
2 print 'the factorial is:', reduce((lambda x,y: x*y), range(1, 5)) # the factorial is: 24
```

![reduce](http://farm3.staticflickr.com/2878/9413420619_8e03e9dd0d.jpg)

## 参考资料

- 一本不错的书《python核心编程》
- 一个不错的参考网站PEP(python enhancement proposal):<http://www.python.org/dev/peps/>