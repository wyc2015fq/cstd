# 【Python】Pandas里面dataframe 中loc和iloc函数的区别 - zkq_1986的博客 - CSDN博客





2018年08月20日 23:37:23[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：682








用官网中的数据举例：df 为下面的dataframe

A         B         C         D
2013-01-01  0.469112 -0.282863 -1.509059 -1.135632
2013-01-02  1.212112 -0.173215  0.119209 -1.044236
2013-01-03 -0.861849 -2.104569 -0.494929  1.071804
2013-01-04  0.721555 -0.706771 -1.039575  0.271860
2013-01-05 -0.424972  0.567020  0.276232 -1.087401
2013-01-06 -0.673690  0.113648 -1.478427  0.524988
1. loc函数

loc为 Selection by Label函数，简单的来讲，即为按标签取数据，标签是什么，就是上面的'2013-01-01' ~'2013-01-06', 'A'~'D'

下面举几个例子，第一个参数选择index，第二个参数选择column

![](https://static.oschina.net/uploads/space/2018/0408/155338_2RJ1_3350450.png)

2. iloc函数

iloc函数为Selection by Position，即按位置选择数据，即第n行，第n列数据，所以传入的是位置的整数型参数。

下面举几个列子：

![](https://static.oschina.net/uploads/space/2018/0408/155801_LqNZ_3350450.png)

ref: http://pandas.pydata.org/pandas-docs/stable/10min.html



