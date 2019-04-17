# pandas 处理缺失值[dropna、drop、fillna] - 墨流觞的博客 - CSDN博客





2018年09月22日 18:37:18[墨氲](https://me.csdn.net/dss_dssssd)阅读数：1998








面对缺失值三种处理方法：
- option 1： 去掉含有缺失值的样本（行）
- option 2：将含有缺失值的列（特征向量）去掉
- option 3：将缺失值用某些值填充（0，平均值，中值等）

对于dropna和fillna,dataframe和series都有，在这主要讲datafame的

#### 对于option1：

使用`DataFrame.dropna(axis=0, how='any', thresh=None, subset=None, inplace=False)`

参数说明：
- axis:
- axis=0: 删除包含缺失值的行
- axis=1: 删除包含缺失值的列

- how: 与axis配合使用
- how=‘any’ :只要有缺失值出现，就删除该行货列
- how=‘all’:  所有的值都缺失，才删除行或列

- thresh： axis中至少有thresh个非缺失值，否则删除

比如 axis=0，thresh=10：标识如果该行中非缺失值的数量小于10，将删除改行
- subset: list

在哪些列中查看是否有缺失值
- inplace: 是否在原数据上操作。如果为真，返回None否则返回新的copy，去掉了缺失值

建议在使用时将全部的缺省参数都写上，便于快速理解

examples:

```python
```python
df = pd.DataFrame(
                                        {"name": ['Alfred', 'Batman', 'Catwoman'],         
                                          "toy": [np.nan, 'Batmobile', 'Bullwhip'],
                                         "born": [pd.NaT, pd.Timestamp("1940-04-25")     
                                                        pd.NaT]})
 			>>> df
 			       name        toy       born
 			0    Alfred        NaN        NaT
 			1    Batman  Batmobile 1940-04-25
 			2  Catwoman   Bullwhip        NaT
 			
 			# Drop the rows where at least one element is missing.
 			>>> df.dropna()
 			     name        toy       born
 			1  Batman  Batmobile 1940-04-25
 			
 			# Drop the columns where at least one element is missing.
 			>>> df.dropna(axis='columns')
 			       name
 			0    Alfred
 			1    Batman
 			2  Catwoman
 			
 			# Drop the rows where all elements are missing.
 			>>> df.dropna(how='all')
 			       name        toy       born
 			0    Alfred        NaN        NaT
 			1    Batman  Batmobile 1940-04-25
 			2  Catwoman   Bullwhip        NaT
 			
 			# Keep only the rows with at least 2 non-NA values.
 			>>> df.dropna(thresh=2)
 			       name        toy       born
 			1    Batman  Batmobile 1940-04-25
 			2  Catwoman   Bullwhip        NaT
 			
 			# Define in which columns to look for missing values.
 			>>> df.dropna(subset=['name', 'born'])
 			       name        toy       born
 			1    Batman  Batmobile 1940-04-25
 			
 			# Keep the DataFrame with valid entries in the same variable.	
 			>>> df.dropna(inplace=True)
 			>>> df
 			     name        toy       born
 			1  Batman  Batmobile 1940-04-25
```
```

#### 对于option 2:

可以使用dropna 或者drop函数
`DataFrame.drop(labels=None, axis=0, index=None, columns=None, level=None, inplace=False, errors='raise')`
- labels: 要删除行或列的列表
- axis: 0 行 ；1 列

```python
```python
df = pd.DataFrame(np.arange(12).reshape(3,4),                 
	                  columns=['A', 'B', 'C', 'D'])
	
	>>>df
	   	   A  B   C   D
		0  0  1   2   3
		1  4  5   6   7
		2  8  9  10  11

	# 删除列
	>>> df.drop(['B', 'C'], axis=1)
	   A   D
	0  0   3
	1  4   7
	2  8  11
	>>> df.drop(columns=['B', 'C'])
	   A   D
	0  0   3
	1  4   7
	2  8  11
	
	# 删除行（索引）
	>>> df.drop([0, 1])
	   A  B   C   D
	2  8  9  10  11
```
```

#### 对于option3

使用`DataFrame.fillna(value=None, method=None, axis=None, inplace=False, limit=None, downcast=None, **kwargs)`
- value:  scalar, dict, Series, or DataFrame

dict 可以指定每一行或列用什么值填充
- method： {‘backfill’, ‘bfill’, ‘pad’, ‘ffill’, None}, default None

在列上操作
- ffill / pad: 使用前一个值来填充缺失值
- backfill / bfill :使用后一个值来填充缺失值

- limit 填充的缺失值个数限制。*应该不怎么用*

```python
```python
f = pd.DataFrame([[np.nan, 2, np.nan, 0],
                   [3, 4, np.nan, 1],
                   [np.nan, np.nan, np.nan, 5],
                   [np.nan, 3, np.nan, 4]],
                   columns=list('ABCD'))
 >>> df
     A    B   C  D
0  NaN  2.0 NaN  0
1  3.0  4.0 NaN  1
2  NaN  NaN NaN  5
3  NaN  3.0 NaN  4

# 使用0代替所有的缺失值
>>> df.fillna(0)
    A   B   C   D
0   0.0 2.0 0.0 0
1   3.0 4.0 0.0 1
2   0.0 0.0 0.0 5
3   0.0 3.0 0.0 4

# 使用后边或前边的值填充缺失值
>>> df.fillna(method='ffill')
    A   B   C   D
0   NaN 2.0 NaN 0
1   3.0 4.0 NaN 1
2   3.0 4.0 NaN 5
3   3.0 3.0 NaN 4

>>>df.fillna(method='bfill')
     A	B	C	D
0	3.0	2.0	NaN	0
1	3.0	4.0	NaN	1
2	NaN	3.0	NaN	5
3	NaN	3.0	NaN	4

# Replace all NaN elements in column ‘A’, ‘B’, ‘C’, and ‘D’, with 0, 1, 2, and 3 respectively.
# 每一列使用不同的缺失值
>>> values = {'A': 0, 'B': 1, 'C': 2, 'D': 3}
>>> df.fillna(value=values)
    A   B   C   D
0   0.0 2.0 2.0 0
1   3.0 4.0 2.0 1
2   0.0 1.0 2.0 5
3   0.0 3.0 2.0 4

#只替换第一个缺失值
 >>>df.fillna(value=values, limit=1)
    A   B   C   D
0   0.0 2.0 2.0 0
1   3.0 4.0 NaN 1
2   NaN 1.0 NaN 5
3   NaN 3.0 NaN 4
```
```

房价分析：

在此问题中，只有bedroom一列有缺失值，按照此三种方法处理代码为：

```python
```python
# option 1 将含有缺失值的行去掉
housing.dropna(subset=["total_bedrooms"])  

# option 2 将"total_bedrooms"这一列从数据中去掉
housing.drop("total_bedrooms", axis=1)  

 # option 3 使用"total_bedrooms"的中值填充缺失值
median = housing["total_bedrooms"].median()
housing["total_bedrooms"].fillna(median)
```
```

##### sklearn提供了处理缺失值的	Imputer类，具体的使用教程在这[https://blog.csdn.net/dss_dssssd/article/details/82831240](https://blog.csdn.net/dss_dssssd/article/details/82831240)



