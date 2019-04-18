# python pandas中对Series数据进行轴向连接 - 小灰笔记 - CSDN博客





2017年04月18日 08:17:38[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：4268








有时候我们想要的数据合并结果是数据的轴向连接，在pandas中这可以通过concat来实现。操作的对象通常是Series。

Ipython中的交互代码如下：




In [**17**]:
**from****pandas****import** Series,DataFrame




In [**18**]:
series1 = Series(range(2),index = ['a','b'])




In [**19**]:
series2 = Series(range(3),index = ['c','d','e'])




In [**20**]:
series3 = Series(range(2),index = ['f','g'])




In [**21**]:
**import****pandas****as****pd**




进行三个Series的连接：




In [**22**]:
pd.concat([series1,series2,series3])

Out[**22**]: 

a    0

b    1

c    0

d    1

e    2

f    0

g    1

dtype: int64




默认情况下，pandas执行的是按照axis=0进行连接。如果进行axis=1的连接，结果如下：




In [**24**]:
S1=pd.concat([series1,series2,series3],axis=1)




In [**25**]:
S1

Out[**25**]: 

     0    1    2

a  0.0  NaN  NaN

b  1.0  NaN  NaN

c  NaN  0.0  NaN

d  NaN  1.0  NaN

e  NaN  2.0  NaN

f  NaN  NaN  0.0

g  NaN  NaN  1.0




In [**26**]: type(S1)

Out[**26**]:
pandas.core.frame.DataFrame




结果是一个DataFrame，回头再看一下前面的Series的连接后的最终类型：

In [**27**]: type(pd.concat([series1,series2,series3]))

Out[**27**]:
pandas.core.series.Series

两种方式的结果并不相同，一个结果是Series，另一个则是DataFrame。

In [**29**]:
series3 = Series(range(2),index = ['f','e'])




In [**30**]:
pd.concat([series1,series2,series3])

Out[**30**]: 

a    0

b    1

c    0

d    1

e    2

f    0

e    1

dtype: int64




从上面的一点测试中可以看出，concat的操作仅仅是单纯的连接，并没有涉及到数据的整合。如果想要进行整合，还是使用merge的方法。



