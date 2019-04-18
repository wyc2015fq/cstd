# 使用pandas对矢量化数据进行替换处理 - 小灰笔记 - CSDN博客





2017年04月23日 23:37:31[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2559
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









使用pandas处理向量化的数据，进行数据的替换时不仅仅能够进行字符串的替换也能够处理数字。

做简单的示例如下：




In [**4**]:
data = Series(range(5))




In [**5**]:
data

Out[**5**]: 

0    0

1    1

2    2

3    3

4    4

dtype: int64




In [**6**]:
data.replace(3,333)

Out[**6**]: 

0      0

1      1

2      2

3    333

4      4

dtype: int64




In [**7**]:
data

Out[**7**]: 

0    0

1    1

2    2

3    3

4    4

dtype: int64




In [**8**]:
data.replace({2:np.nan,4:444})

Out[**8**]: 

0      0.0

1      1.0

2      NaN

3      3.0

4    444.0

dtype: float64




从上面可以看出，替换可以进行单个数字的替换，也可以穿入一个字典进行一个序列的替换。

简单的替换虽然也可以通过赋值进行修改，但是通过赋值进行修改的时候一般首先得进行数据替换对象的查找。但是，通过Series对象的replace方法进行数据替换的方便之处则在于省掉了数据对象的查询。



