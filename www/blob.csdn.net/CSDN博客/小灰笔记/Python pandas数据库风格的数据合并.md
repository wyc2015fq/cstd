# Python pandas数据库风格的数据合并 - 小灰笔记 - CSDN博客





2017年04月16日 09:31:41[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：639








我个人不是很熟悉数据库，不是很清楚什么是数据库风格。不过，通过简单的测试看一下这到底是什么功能，做一下简单的总结还是可以的。




In [**1**]:
**import****numpy****as****np**




In [**2**]:
**import****pandas****as****pd**




In [**3**]:
**from****pandas****import** Series,DataFrame




In [**4**]:
df1 = DataFrame({'key':['b','b','a','c','a','a','b'],'data1':range(7)})




In [**5**]:
df2 = DataFrame({'key':['a','b','d'],'data2':range(3)})




In [**6**]:
df1

Out[**6**]: 

   data1 key

0      0   b

1      1   b

2      2   a

3      3   c

4      4   a

5      5   a

6      6   b




In [**7**]:
df2

Out[**7**]: 

   data2 key

0      0   a

1      1   b

2      2   d




In [**8**]:
pd.merge(df1,df2)

Out[**8**]: 

   data1 key  data2

0      0   b      1

1      1   b      1

2      6   b      1

3      2   a      0

4      4   a      0

5      5   a      0




看到这个结果有些不理解，说以df1为基础进行df2的合并，df1的c却丢失了。这就罢了，其实df2的数据也是不完整的。猜想是不是merge方法参数的顺序会对对结果有什么影响？把参数顺序反转一下，结果如下：




In [**9**]:
pd.merge(df2,df1)

Out[**9**]: 

   data2 key  data1

0      0   a      2

1      0   a      4

2      0   a      5

3      1   b      0

4      1   b      1

5      1   b      6




上面的结果与第一次merge的结果完全一致，看来参数的顺序并不会对结果产生什么影响。继续找一下规律，找到一个新的规律：如果是把字母那一列作为key，那么合并的列表中全都是共有的key。而左右两边的数值则分别涵盖了merge的两组数据。为了验证假设，再做一个测试：




In [**10**]:
df2 = DataFrame({'key':['a','b','d','b'],'data2':range(4)})




In [**11**]:
pd.merge(df2,df1)

Out[**11**]: 

   data2 key  data1

0      0   a      2

1      0   a      4

2      0   a      5

3      1   b      0

4      1   b      1

5      1   b      6

6      3   b      0

7      3   b      1

8      3   b      6




In [**12**]:
df2

Out[**12**]: 

   data2 key

0      0   a

1      1   b

2      2   d

3      3   b




从上面的结果可以看出，前面的总结猜测不错。merge功能实现的是共有key的所有数值表的合并。



