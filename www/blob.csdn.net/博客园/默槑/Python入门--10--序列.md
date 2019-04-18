# Python入门--10--序列 - 默槑 - 博客园







# [Python入门--10--序列](https://www.cnblogs.com/modaidai/p/6867012.html)





一、与列表、元祖的相同与不同

　　1、都可以通索引得到元素

　　2、默认索引从0开始

　　3、可以通过分片得到一个范围内的元素集合

　　4、有很多共同的操作符

二、

1、list()这个函数用法

　　a="we are famliy"

　　a=list(a)

　　会输出：['w', 'e', ' ', 'a', 'r', 'e', ' ', 'f', 'a', 'm', 'l', 'i', 'y']

　　b=(1,2,3,4,5,6,7,8)

　　b= list(b)

　　会输出：[1,2,3,4,5,6,7,8]

2、tuple()

　　用法同上

3、str()

　　用法同上

4、max()返回序列的最大值

　　max(a)   #会返回y　　比较的是字母的ASCII码

　　数字就不用说了

5、min()返回序列的最大值

　　dd='1234567890'

　　min(dd)　　　　#输出0

6、sum()求和

　　sum(b)   #36

　　sum(b,4)　　#后面的数也加上 ，输出40

7、sorted() 排序

　　功能如同list.sort()　　　　

　　d=[1,2,4,896,84,65]

　　d.sort()
　　print d　　　　　　#会有排序



　　sorted(b)　　　　

　　print b　　　　　　#试一下

8、reversed() 翻转

　　b.reverse()

　　print b　　　　　　#这是reverse的用法



　　print list(reversed(b))    　　#会输出翻转之后的结果 不用print不会输出 这是reversed的用法

　　　　　　　　　　　　　　　　#要用list强制转换为列表

9、 enumerate() 枚举

　　list(enumerate(b))　　　　　　#试一下

10、 zip()



　　q=[1,2,3,4,5,6,7,8]
　　w=[78,98,565,45,466]
　　list1=list(zip(q,w))
　　print list1　　　　　　　　　　#输出[(1, 78), (2, 98), (3, 565), (4, 45), (5, 466)]

 11、extend()

下面举个栗子

a = ["a", "b", "c", "d"]

b = ["b", "e"]

c = ["a", "b", "c", "d", "e"] 
a.extend(b) 
array = list(set(a)) 
print(array) 


array = list(set(a)|set(b)) 
print(array)                                             #这两种输出的结果是一样的     很方便



12、eval()    刚学到的，很实用！

　　功能：将字符串str当成有效的表达式来求值并返回计算结果。

　　语法： eval(source[, globals[, locals]]) -> value

　　参数：

　　　　source：一个Python表达式或函数compile()返回的代码对象

　　　　globals：可选。必须是dictionary

　　　　locals：可选。任意map对象
可以把list,tuple,dict和string相互转化。
  #################################################
  字符串转换成列表
  >>>a = "[[1,2], [3,4], [5,6], [7,8], [9,0]]"
  >>>type(a)
  <type 'str'>
  >>> b = eval(a)
  >>> print b
  [[1, 2], [3, 4], [5, 6], [7, 8], [9, 0]]
  >>> type(b)
 <type 'list'>
 #################################################
 字符串转换成字典
 >>> a = "{1: 'a', 2: 'b'}"
 >>> type(a)
 <type 'str'>
 >>> b = eval(a)
 >>> print b
 {1: 'a', 2: 'b'}
 >>> type(b)
 <type 'dict'>
 #################################################
 字符串转换成元组
 >>> a = "([1,2], [3,4], [5,6], [7,8], (9,0))"
 >>> type(a)
 <type 'str'>
 >>> b = eval(a)
 >>> print b
 ([1, 2], [3, 4], [5, 6], [7, 8], (9, 0))
 >>> type(b)
 <type 'tuple'>











