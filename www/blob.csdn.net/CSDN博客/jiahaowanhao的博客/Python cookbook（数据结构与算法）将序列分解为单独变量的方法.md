
# Python cookbook（数据结构与算法）将序列分解为单独变量的方法 - jiahaowanhao的博客 - CSDN博客


2018年04月23日 20:55:42[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：29标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


**Python cookbook（数据结构与算法）将序列分解为单独变量的方法**
本文实例讲述了Python cookbook（数据结构与算法）将序列分解为单独变量的方法。分享给大家供大家参考，具体如下：
如果对象是可迭代的（任何序列），则可以进行分解操作，包括元组、列表、字符串、文件、迭代器以及生成器，可通过简单的一个赋值操作分解为单独的变量。
唯一要求：变量的总数和序列相吻合，否则将出错；
Python 2.7.11 (v2.7.11:6d1b6a68f775, Dec 5 2015, 20:32:19) [MSC v.1500 32 bit (Intel)] on win32
Type "copyright", "credits" or "license()" for more information.
>>> p=[4,5]
>>> p
[4, 5]
>>> x,y=p
>>> x
4
>>> y
5
>>> data=['lucy',50,12.1,(2016,07,31)]
>>> data
['lucy', 50, 12.1, (2016, 7, 31)]
>>> name,shares,price,date=data
>>> name
'lucy'
>>> shares
50
>>> price
12.1
>>> date
(2016, 7, 31)
>>> name,shares,price,(year,mon,day)=data
>>> name
'lucy'
>>> shares
50
>>> price
12.1
>>> year
2016
>>> mon
7
>>> day
31
>>> s='hello'
>>> s
'hello'
>>> a,b,c,d,e=s
>>> c
'l'
>>> d
'l'
>>> e
'o'
>>> data=['lucy',50,12.1,(2016,07,31)]
>>> _,shares,price,_=data \#当想丢弃某些值时，可以选一个用不到的变量名作为要丢弃的值的名称，比如此处“_”
>>> shares
50
>>> price
12.1
>>>
注意：请确保丢弃的值选择的变量名在其他地方未曾用到过。

