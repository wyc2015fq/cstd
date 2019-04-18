# pandas进行数据的交集与并集方式的数据合并 - 小灰笔记 - CSDN博客





2017年04月16日 11:01:47[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：13171








数据合并有多种方式，其中最常见的应该就是交集和并集的求取。之前通过分析总结过pandas数据merge功能默认的行为，其实默认下求取的就是两个数据的“交集”。

有如下数据定义：

In [**26**]:
df1

Out[**26**]: 

   data1 key

0      0   b

1      1   b

2      2   a

3      3   c

4      4   a

5      5   a

6      6   b




In [**27**]:
df2

Out[**27**]: 

   data2 key

0      0   a

1      1   b

2      2   d

3      3   b




进行merge的结果：

In [**28**]:
pd.merge(df1,df2)

Out[**28**]: 

   data1 key  data2

0      0   b      1

1      0   b      3

2      1   b      1

3      1   b      3

4      6   b      1

5      6   b      3

6      2   a      0

7      4   a      0

8      5   a      0




从上面的结果中能够看出，merge的默认处理行为是求取了两组数据key的交集，但是对于key的值进行了并集的求取。其实也很好理解，如果仅仅是求取交集而数据没有任何合并那就不叫做数据合并了。

接下来试一下制定了参数的的交集数据合并处理：

In [**29**]:
pd.merge(df1,df2,how='inner')

Out[**29**]: 

   data1 key  data2

0      0   b      1

1      0   b      3

2      1   b      1

3      1   b      3

4      6   b      1

5      6   b      3

6      2   a      0

7      4   a      0

8      5   a      0




In [**30**]:
result_inner = pd.merge(df1,df2,how='inner')




In [**31**]:
result_default = pd.merge(df1,df2)




In [**32**]:
result_inner == result_default

Out[**32**]: 

  data1   key data2

0  True  True  True

1  True  True  True

2  True  True  True

3  True  True  True

4  True  True  True

5  True  True  True

6  True  True  True

7  True  True  True

8  True  True  True




通过上面的结果可以看出：制定了参数的的交集数据合并处理的结果与数据合并方法merge的默认行为是一致的。

再试一下并集数据合并处理方法，这需要制定参数how为outer：

In [**35**]:
result_outer = pd.merge(df1,df2,how='outer')




In [**36**]:
result_outer

Out[**36**]: 

    data1 key  data2

0     0.0   b    1.0

1     0.0   b    3.0

2     1.0   b    1.0

3     1.0   b    3.0

4     6.0   b    1.0

5     6.0   b    3.0

6     2.0   a    0.0

7     4.0   a    0.0

8     5.0   a    0.0

9     3.0   c    NaN

10    NaN   d    2.0




通过上面的执行结果可以看出：合并后的数据中的key拥有了两组数据所有的key，而数据虽然有一部分两组数据不能够重合，但也通过NaN的值进行了相应的填补。

还有一点需要注意的，那就是合并之后的数据个数。合并后的数据中，key的个数是两组数据中分别拥有的数据的笛卡尔乘积。如果其中一组没有的时候，进行合并的时候另一组数据中会创建一个NaN数值的对象与之进行合并。



