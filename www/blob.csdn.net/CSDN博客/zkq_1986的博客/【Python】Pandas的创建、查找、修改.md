# 【Python】Pandas的创建、查找、修改 - zkq_1986的博客 - CSDN博客





2018年08月20日 23:27:09[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：253








DataFrame创建方法有很多，常用基本格式是：DataFrame 构造器参数：DataFrame(data=[],index=[],coloumns=[])

```
In [272]: df2=DataFrame(np.arange(16).reshape((4,4)),index=['a','b','c','d'],columns=['one','two','three','four'])


In [273]: df2

Out[273]:

one two three four

a 0 1 2 3

b 4 5 6 7

c 8 9 10 11

d 12 13 14 15
```



## 1.2 用传入等长列表组成的字典来创建
- 



```
In [204]: data={'c':['1','2'],'a':['5']} #创建不等长字典序列


In [205]: data

Out[205]: {'a': ['5'], 'c': ['1', '2']}


In [206]: df=DataFrame(data)

Traceback (most recent call last):

...


ValueError: arrays must all be same length # 报错，传入的数组必须等长


In [207]: data={'c':['1','2'],'a':['5','6']} #创建<strong>等长字典序列

In [208]: df=DataFrame(data)


In [209]: df

Out[209]:

a c # 创建完成后'a','c'自动按照字典序排序，并且创建时自定加上索引

0 5 1

1 6 2
```

创建完成后'a','c'自动按照字典序排序，并且创建时自定加上索引



如果指定了columns名称，则会按照指定顺序创建。

```
In [210]: df=DataFrame(data,columns=['c','a'])


In [211]: df

Out[211]:

c a #按照指定顺序创建。

0 1 5

1 2 6
```



![](https://img-blog.csdn.net/20160523192135127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



