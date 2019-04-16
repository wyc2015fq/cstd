# pandas的引用与复制 - lyx的专栏 - CSDN博客





2017年04月23日 14:59:52[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：7371








        之前一直以为pandas任何的切片和筛选都是引用，也就是说，会改变最原始的数据。但是前几天发现并不是这样的。

        下面对最常见的几种pandas 数据截取的方式做一个整理。



```python
import pandas as pd
def df_gen():
    l1 = [1,2,3]
    l2 = [4,5,6]
    l3 = [7,6,5]
    df_t = pd.DataFrame()
    df_t['a'] = l1
    df_t['b'] = l2
    df_t['c'] = l3
    return df_t
print 'df:', df_gen()

print 'refernece:'

df = df_gen()
d1 = df.a
d1[0] = 999
print '1', df
df = df_gen()
d1 = df.ix[0]
d1[0] = 999
print '2', df
df = df_gen()
d1 = df.loc[1,:]
d1[0] = 999
print '3', df
df = df_gen()
d1 = df['a']
d1[0] = 999
print '4', df


print 'copy:'

df = df_gen()
d1 = df.iloc[1]
d1[0] = 999
print '1', df
df = df_gen()
d1 = df[df.a > 1]
d1[0] = 999
print '2', df
df = df_gen()
d1 = df.loc[df.a > 1, 'b']
d1[0] = 999
print '3', df
```
        上面总共7种方式，前面四种是引用的方式，后面的三种是复制。


        输出是这样一串：

df:    a  b  c

0  1  4  7

1  2  5  6

2  3  6  5

refernece:

1      a  b  c

0  999  4  7

1    2  5  6

2    3  6  5

1      a  b  c

0  999  4  7

1    2  5  6

2    3  6  5

1      a  b  c

0    1  4  7

1  999  5  6

2    3  6  5

1      a  b  c

0  999  4  7

1    2  5  6

2    3  6  5

copy:

F:/int/ProjectIntern/DatabaseTransfer/dd.py:37: SettingWithCopyWarning: 

A value is trying to be set on a copy of a slice from a DataFrame



See the caveats in the documentation: http://pandas.pydata.org/pandas-docs/stable/indexing.html#indexing-view-versus-copy

  d1[0] = 999

1    a  b  c

0  1  4  7

1  2  5  6

2  3  6  5

1 F:/int/ProjectIntern/DatabaseTransfer/dd.py:41: SettingWithCopyWarning: 

A value is trying to be set on a copy of a slice from a DataFrame.

Try using .loc[row_indexer,col_indexer] = value instead



See the caveats in the documentation: http://pandas.pydata.org/pandas-docs/stable/indexing.html#indexing-view-versus-copy

  d1[0] = 999

   a  b  c

0  1  4  7

1  2  5  6

2  3  6  5

1    a  b  c

0  1  4  7

1  2  5  6

2  3  6  5
        我们发现pandas不是refernce的时候会有警告。refernce的时候，df原始的值被改变了，说明d1只是一个引用，而后面的copy则不然。

        在使用pandas的时候要注意这一特性。



