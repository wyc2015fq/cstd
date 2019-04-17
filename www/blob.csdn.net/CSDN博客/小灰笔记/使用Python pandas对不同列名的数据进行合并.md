# 使用Python pandas对不同列名的数据进行合并 - 小灰笔记 - CSDN博客





2017年04月16日 11:28:57[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：8931








使用pandas的merge方法进行数据合并的时候，如果列名相同则可以省去列名的指定，pandas会进行自动的识别。但是如果两组数据的列名不一致，不加任何参数直接进行默认的操作则会导致报错。

例如有如下两组数据：

In [**40**]:
df1

Out[**40**]: 

   data1 key1:

0      0     c

1      1     d

2      2     e

3      3     f

4      4     g

5      5     a

6      6     b




In [**41**]:
df2

Out[**41**]: 

   data2 key2

0      0    a

1      1    c

2      2    d




尝试进行数据合并，结果提示如下：

In [**42**]:
pd.merge(df1,df2)

---------------------------------------------------------------------------

MergeError                                Traceback (most recent call last)

<ipython-input-42-8a74cf63ba8e> in
<module>()

----> 1pd.merge(df1,df2)




/Library/Python/2.7/site-packages/pandas/tools/merge.pyc in
merge(left, right, how, on, left_on, right_on, left_index, right_index, sort, suffixes, copy, indicator)

**     59**                          right_on=right_on, left_index=left_index,

**     60**                          right_index=right_index, sort=sort,
 suffixes=suffixes,

---> 61                          copy=copy, indicator=indicator)

**     62**return op.get_result()

**     63**if __debug__:




/Library/Python/2.7/site-packages/pandas/tools/merge.pyc in
__init__(self, left, right, how, on, left_on, right_on, axis, left_index, right_index, sort, suffixes, copy, indicator)

**    536**             warnings.warn(msg,
 UserWarning)

**    537**

--> 538self._validate_specification()

**    539**

**    540**# note this function has side effects




/Library/Python/2.7/site-packages/pandas/tools/merge.pyc in
_validate_specification(self)

**    881**                     self.right.columns)

**    882**if len(common_cols)==0:

--> 883raise MergeError('No common columns to perform merge on')

**    884**ifnot common_cols.is_unique:

**    885**                     raise MergeError("Data columns not unique: %s"




MergeError: No common columns to perform merge on




这样的操作会导致类似上面的错误，pandas找不到能够进行合并的列。如果进行合并，其实有两个思路可以考虑。第一种是根据现在的数据重新创建列名一致的数据对象，然后进行新的数据对象合并。但是，这样在一定程度上来说就不是对最原始的数据进行处理了。不过，很多时候或许这也是一个值得考虑的手段。另外一种方法就是利用pandas自带的功能，在进行数据合并的时候直接指明需要合并的列的名称。具体的操作如下：

In [**45**]:
pd.merge(df1,df2,left_on='key1:',right_on='key2')

Out[**45**]: 

   data1 key1:  data2 key2

0      0     c      1    c

1      1     d      2    d

2      5     a      0    a




最初输入pd.merge(df1,df2,left_on=‘key1’,right_on=‘key2’)结果报错的时候很诧异，检查数据才看到自己在创建数据的时候多出了一个冒号。不过，对于merge操作的默认行为测试中并没有用到列名，前面的测试也就无需再做一次。在上面的运行结果中，数据列分别使用了两组数据中独立的名字，如果把两组数据的独立名字改成一致，这其实就是一个指定列的合并。而彼时，默认的merge合并应该会奏效，因为pandas能够找到两个列名一致的列。然而，指定了合并列名的操作中数据的标题会被pandas进行修改以进行区分。

In [**49**]:
df1

Out[**49**]: 

   data key1

0     0    c

1     1    d

2     2    e

3     3    f

4     4    g

5     5    a

6     6    b




In [**50**]:
df2

Out[**50**]: 

   data key2

0     0    a

1     1    c

2     2    d




In [**51**]:
pd.merge(df1,df2)

Out[**51**]: 

   data key1 key2

0     0    c    a

1     1    d    c

2     2    e    d




In [**52**]:
pd.merge(df1,df2,left_on='key1',right_on='key2')

Out[**52**]: 

   data_x key1  data_y key2

0       0    c       1    c

1       1    d       2    d

2       5    a       0    a




从上面的结果中可以看出，pandas把两个data分别以下划线架x，y的形式进行了区分。






