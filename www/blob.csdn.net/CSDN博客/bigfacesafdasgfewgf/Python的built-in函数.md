# Python的built-in函数 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 16:19:23[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1139








Python中内置了好处功能特别强大的函数，完整的函数列表可以查看下面的网站：
[https://docs.python.org/3/library/functions.html](https://docs.python.org/3/library/functions.html)




- abs(x)

求绝对值，只能有一个参数


- cmp(x, y)

比较函数`cmp(x, y)`就需要两个参数，如果`x<y`，返回`-1`，如果`x==y`，返回`0`，如果`x>y`，返回`1。`


```

```


`该函数只有在python2中可用，而且在python2所有版本中都可用。但是在python3中该函数已经被删减掉，这点要特别注意。`


```

```



```

```



- int()

数据类型转换函数，比如`int()`函数可以把其他数据类型转换为整数。类似的函数还包括float(),
 str(), unicode(), bool()等。






```
>>> int('123')
123
>>> int(12.34)
12
>>> float('12.34')
12.34
>>> str(1.23)
'1.23'
>>> unicode(100)
u'100'
>>> bool(1)
True
>>> bool('')
False
```








- all(iterable)

如果iterable的一旦有元素为0、空‘’、false，那么返回值就是false，否则就是true。



`>>> ``all``([``'a'``, ``'b'``, ``'c'``, ``'d'``])  ``#列表list，元素都不为空或0`

`True`

`>>> ``all``([``'a'``, ``'b'`
```
,
 '
```

```
',
 '
```
`d'])  ``#列表list，存在一个为空的元素`

`False`

`>>> ``all``([``0``, ``1``，``2``, ``3``])  ``#列表list，存在一个为0的元素`

`False`

` `

`>>> ``all``((``'a'``, ``'b'``, ``'c'``, ``'d'``))  ``#元组tuple，元素都不为空或0`

`True`

`>>> ``all``((``'a'``, ``'b'`
```
,
 '
```

```
',
 '
```
`d'))  ``#元组tuple，存在一个为空的元素`

`False`

`>>> ``all``((``0``, ``1``，``2``, ``3``))  ``#元组tuple，存在一个为0的元素`

`False`

` `

` `

`>>> ``all``([]) `
```
#
 空列表
```


`True`

`>>> ``all``(()) `
```
#
 空元组
```


`True`





- any(iterable)

和all函数功能类似，不过只有当iterable所有元素都是0、空‘’、false的时候，才返回false，否则返回true。


>>> any(['a', 'b', 'c', 'd'])  #列表list，元素都不为空或0

True

>>> any(['a', 'b', '', 'd'])  #列表list，存在一个为空的元素

True

>>> any([0, '', False])  #列表list,元素全为0,'',false

False



>>> any(('a', 'b', 'c', 'd'))  #元组tuple，元素都不为空或0

True

>>> any(('a', 'b', '', 'd'))  #元组tuple，存在一个为空的元素

True

>>> any((0, '', False))  #元组tuple，元素全为0,'',false

False





>>> any([]) # 空列表

False

>>> any(()) # 空元组

False




- basestring(), isinstance()

判断一个变量是否是str或者是unicode，或者是其他类型。包括str, unicode, int, float. 



示例：




>>> isinstance("Hello world", str)

True

>>> isinstance("Hello world", basestring)

True

>>> isinstance(u"你好", unicode)

True

>>> isinstance(u"你好", basestring)

True




来个实用的




要检查某对象是否为字符串或 Unicode 对象，简单快速的办法是使用内建的 isinstance 和 basestring ，用法如下所示：




def isAString(anobj):

   return isinstance(anobj, basestring)





- bin(x)

将整数x转换为二进制字符串，



`#整数的情况`

`>>> ``bin``(``521``)`

`#这里的显示结果形式与我们平时习惯有些差别，主要是前面多了0b，这是表示二进制的意思。`

`'0b1000001001'`





- index()


## 描述

Python index() 方法检测字符串中是否包含子字符串 str ，如果指定 beg（开始） 和 end（结束） 范围，则检查是否包含在指定范围内，该方法与 python find()方法一样，只不过如果str不在 string中会报一个异常。

## 语法

index()方法语法：
str.index(str, beg=0 end=len(string))

## 参数
- str -- 指定检索的字符串
- beg -- 开始索引，默认为0。
- end -- 结束索引，默认为字符串的长度。

## 返回值

如果包含子字符串返回开始的索引值，否则抛出异常。

## 实例

以下实例展示了index()方法的实例：
#!/usr/bin/python

str1 = "this is string example....wow!!!";
str2 = "exam";

print str1.index(str2);
print str1.index(str2, 10);
print str1.index(str2, 40);

以上实例输出结果如下：
15
15
Traceback (most recent call last):
  File "test.py", line 8, in 
  print str1.index(str2, 40);
ValueError: substring not found

shell returned 1






- chr(x), ord(x)

返回整数x对应的ASCII字符，ord返回字符对应的整数



`>>> ``chr``(``97``)`

`'a'`

`>>> ``chr``(``98``)`

`'b'`

`>>> ``ord``(``'a'``)`

`97`

`>>> ``ord``(``'b'``)`

`98`








- complex(real, imaginary)


创建一个值为real + imag * j的复数或者转化一个字符串或数为复数。如果第一个参数为字符串，则不需要指定第二个参数。

参数real: int, long, float或字符串；

参数imag: int, long, float。







`>>> ``complex``(``1``, ``2``)`

`(``1``+``2j``)`

`#数字`

`>>> ``complex``(``1``)`

`(``1``+``0j``)`

`#当做字符串处理`

`>>> ``complex``(``"1"``)`

`(``1``+``0j``)`


```
#注意：这个地方在“+”号两边不能有空格，也就是不能写成"1
 + 2j"，应该是"1+2j"，否则会报错
```


`>>> ``complex``(``"1+2j"``)`

`(``1``+``2j``)`





- divmod(a, b)

divmod(a,b)方法返回的是a//b（除法取整）以及a对b的余数


返回结果类型为tuple



`>>> ``divmod``(``9``,``2``)`

`(``4``, ``1``)`

`>>> ``divmod``(``11``,``3``)`

`(``3``, ``2``)`

`>>> ``divmod``(``1``+``2j``,``1``+``0.5j``)`

`((``1``+``0j``), ``1.5j``)`





















































