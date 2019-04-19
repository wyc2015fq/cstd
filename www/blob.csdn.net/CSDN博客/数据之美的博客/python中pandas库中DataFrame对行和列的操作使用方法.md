# python中pandas库中DataFrame对行和列的操作使用方法 - 数据之美的博客 - CSDN博客
2018年02月08日 11:46:59[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：274
个人分类：[pandas																[python](https://blog.csdn.net/oppo62258801/article/category/6738404)](https://blog.csdn.net/oppo62258801/article/category/7450104)
版权声明：本博客内容为学习过程中的一些记录，大部分转载内容已标明转载，部分内容如有因疏忽未注明请与我联系，转载原创内容请注明链接，谢谢！ [http://blog.csdn.net/xiaodongxiexie/article/details/53108959](http://blog.csdn.net/xiaodongxiexie/article/details/53108959)
用pandas中的DataFrame时选取行或列：
```
import numpy as np
import pandas as pd
from pandas import Sereis, DataFrame
ser = Series(np.arange(3.))
data = DataFrame(np.arange(16).reshape(4,4),index=list('abcd'),columns=list('wxyz'))
data['w']  #选择表格中的'w'列，使用类字典属性,返回的是Series类型
data.w    #选择表格中的'w'列，使用点属性,返回的是Series类型
data[['w']]  #选择表格中的'w'列，返回的是DataFrame类型
data[['w','z']]  #选择表格中的'w'、'z'列
data[0:2]  #返回第1行到第2行的所有行，前闭后开，包括前不包括后
data[1:2]  #返回第2行，从0计，返回的是单行，通过有前后值的索引形式，
       #如果采用data[1]则报错
data.ix[1:2] #返回第2行的第三种方法，返回的是DataFrame，跟data[1:2]同
data['a':'b']  #利用index值进行切片，返回的是**前闭后闭**的DataFrame, 
        #即末端是包含的  
#——————新版本pandas已舍弃该方法，用iloc代替———————
data.irow(0)   #取data的第一行
data.icol(0)   #取data的第一列
ser.iget_value(0)  #选取ser序列中的第一个
ser.iget_value(-1) #选取ser序列中的最后一个，这种轴索引包含索引器的series不能采用ser[-1]去获取最后一个，这会引起歧义。
#————————————————————————————-----------------
data.head()  #返回data的前几行数据，默认为前五行，需要前十行则data.head(10)
data.tail()  #返回data的后几行数据，默认为后五行，需要后十行则data.tail(10)
data.iloc[-1]   #选取DataFrame最后一行，返回的是Series
data.iloc[-1:]   #选取DataFrame最后一行，返回的是DataFrame
data.loc['a',['w','x']]   #返回‘a’行'w'、'x'列，这种用于选取行索引列索引已知
data.iat[1,1]   #选取第二行第二列，用于已知行、列位置的选取。
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
下面是简单的例子使用验证：
```
import pandas as pd
from pandas import Series, DataFrame
import numpy as np
data = DataFrame(np.arange(15).reshape(3,5),index=['one','two','three'],columns=['a','b','c','d','e'])
data
Out[7]: 
        a   b   c   d   e
one     0   1   2   3   4
two     5   6   7   8   9
three  10  11  12  13  14
#对列的操作方法有如下几种
data.icol(0)   #选取第一列
E:\Anaconda2\lib\site-packages\spyder\utils\ipython\start_kernel.py:1: FutureWarning: icol(i) is deprecated. Please use .iloc[:,i]
  # -*- coding: utf-8 -*-
Out[35]: 
one       0
two       5
three    10
Name: a, dtype: int32
data['a']
Out[8]: 
one       0
two       5
three    10
Name: a, dtype: int32
data.a
Out[9]: 
one       0
two       5
three    10
Name: a, dtype: int32
data[['a']]
Out[10]: 
        a
one     0
two     5
three  10
data.ix[:,[0,1,2]]  #不知道列名只知道列的位置时
Out[13]: 
        a   b   c
one     0   1   2
two     5   6   7
three  10  11  12
data.ix[1,[0]]  #选择第2行第1列的值
Out[14]: 
a    5
Name: two, dtype: int32
data.ix[[1,2],[0]]   #选择第2,3行第1列的值
Out[15]: 
        a
two     5
three  10
data.ix[1:3,[0,2]]  #选择第2-4行第1、3列的值
Out[17]: 
        a   c
two     5   7
three  10  12
data.ix[1:2,2:4]  #选择第2-3行，3-5（不包括5）列的值
Out[29]: 
     c  d
two  7  8
data.ix[data.a>5,3]
Out[30]: 
three    13
Name: d, dtype: int32
data.ix[data.b>6,3:4]  #选择'b'列中大于6所在的行中的第4列，有点拗口
Out[31]: 
        d
three  13
data.ix[data.a>5,2:4]  #选择'a'列中大于5所在的行中的第3-5（不包括5）列
Out[32]: 
        c   d
three  12  13
data.ix[data.a>5,[2,2,2]]  #选择'a'列中大于5所在的行中的第2列并重复3次
Out[33]: 
        c   c   c
three  12  12  12
#还可以行数或列数跟行名列名混着用
data.ix[1:3,['a','e']]
Out[24]: 
        a   e
two     5   9
three  10  14
data.ix['one':'two',[2,1]]
Out[25]: 
     c  b
one  2  1
two  7  6
data.ix[['one','three'],[2,2]]
Out[26]: 
        c   c
one     2   2
three  12  12
data.ix['one':'three',['a','c']]
Out[27]: 
        a   c
one     0   2
two     5   7
three  10  12
data.ix[['one','one'],['a','e','d','d','d']]
Out[28]: 
     a  e  d  d  d
one  0  4  3  3  3
one  0  4  3  3  3
#对行的操作有如下几种：
data[1:2]  #（不知道列索引时）选择第2行，不能用data[1]，可以用data.ix[1]
Out[18]: 
     a  b  c  d  e
two  5  6  7  8  9
data.irow(1)   #选取第二行
Out[36]: 
a    5
b    6
c    7
d    8
e    9
Name: two, dtype: int32
data.ix[1]   #选择第2行
Out[20]: 
a    5
b    6
c    7
d    8
e    9
Name: two, dtype: int32
data['one':'two']  #当用已知的行索引时为前闭后闭区间，这点与切片稍有不同。
Out[22]: 
     a  b  c  d  e
one  0  1  2  3  4
two  5  6  7  8  9
data.ix[1:3]  #选择第2到4行，不包括第4行，即前闭后开区间。
Out[23]: 
        a   b   c   d   e
two     5   6   7   8   9
three  10  11  12  13  14
data.ix[-1:]  #取DataFrame中最后一行，返回的是DataFrame类型,**注意**这种取法是有使用条件的，只有当行索引不是数字索引时才可以使用，否则可以选用`data[-1:]`--返回DataFrame类型或`data.irow(-1)`--返回Series类型
Out[11]: 
        a   b   c   d   e
three  10  11  12  13  14
data[-1:]  #跟上面一样，取DataFrame中最后一行，返回的是DataFrame类型
Out[12]: 
        a   b   c   d   e
three  10  11  12  13  14
data.ix[-1] #取DataFrame中最后一行，返回的是Series类型，这个一样，行索引不能是数字时才可以使用
Out[13]: 
a    10
b    11
c    12
d    13
e    14
Name: three, dtype: int32
data.tail(1)   #返回DataFrame中的最后一行
data.head(1)   #返回DataFrame中的第一行
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
最近处理数据时发现当pd.read_csv()数据时有时候会有读取到未命名的列，且该列也用不到，一般是索引列被换掉后导致的，有强迫症的看着难受，这时候dataframe.drop([columns,])是没法处理的，怎么办呢， 
最笨的方法是直接给列索引重命名：
```
data6
        Unnamed: 0  high    symbol  time
date                
2016-11-01  0   3317.4  IF1611  18:10:44.8
2016-11-01  1   3317.4  IF1611  06:01:04.5
2016-11-01  2   3317.4  IF1611  07:46:25.5
2016-11-01  3   3318.4  IF1611  09:30:04.0
2016-11-01  4   3321.8  IF1611  09:31:04.0
data6.columns = list('abcd')
data6
    a   b   c   d
date                
2016-11-01  0   3317.4  IF1611  18:10:44.8
2016-11-01  1   3317.4  IF1611  06:01:04.5
2016-11-01  2   3317.4  IF1611  07:46:25.5
2016-11-01  3   3318.4  IF1611  09:30:04.0
2016-11-01  4   3321.8  IF1611  09:31:04.0
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
重新命名后就可以用dataframe.drop([columns])来删除了，当然不用我这样全部给列名替换掉了，可以只是改变未命名的那个列，然后删除。不过这个用起来总是觉得有点low，有没有更好的方法呢，有，可以不去删除，直接：
`data7 = data6.ix[:,1:]`- 1
这样既不改变原有数据，也达到了删除神烦列，当然我这里时第0列删除，可以根据实际选择所在的列删除之，至于这个原理，可以看下前面的对列的操作。
