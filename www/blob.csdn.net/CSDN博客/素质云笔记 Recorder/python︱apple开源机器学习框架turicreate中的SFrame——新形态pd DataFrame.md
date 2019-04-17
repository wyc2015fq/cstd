# python︱apple开源机器学习框架turicreate中的SFrame——新形态pd.DataFrame - 素质云笔记/Recorder... - CSDN博客





2017年12月14日 18:44:10[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2550








apple开源机器学习框架turicreate中的SFrame，是一种新形态的dataframe，作为之前热爱过R语言的dataframe的玩家来看，还不够简洁，不过有自己独特的功能。 

github:[https://apple.github.io/turicreate/docs/api/generated/turicreate.SFrame.html](https://apple.github.io/turicreate/docs/api/generated/turicreate.SFrame.html)

导入模块的方式：

```python
# 第一种
import turicreate as tc
# 第二种
from turicreate import SFrame
```

# 一、数据读入与导出

支持csv/txt/json

## 1.1 导入与导出csv——read_csv

主函数

```python
'''
read_csv(url, delimiter=',', header=True, error_bad_lines=False, comment_char='', 
    escape_char='\\', double_quote=True, quote_char='"', skip_initial_space=True, 
    column_type_hints=None, na_values=['NA'], line_terminator='\n', usecols=[], nrows=None,
    skiprows=0, verbose=True, nrows_to_infer=100, **kwargs)
'''
```

读入举例：

```python
# 离线版
sf = tc.SFrame.read_csv('/../data.csv')
# 在线版
>>> url = 'https://static.turi.com/datasets/rating_data_example.csv'
>>> sf = SFrame.read_csv(url)
```

导出举例：

```
# 第一种
sf.export_csv('/../1.csv')
# 第二种
sf.save('../data.csv', format='csv')
```

## 1.2 json格式数据的读入导出

```
# 读入
>>> !cat input.json
[{'a':1,'b':1}, {'a':2,'b':2}, {'a':3,'b':3}]
>>> SFrame.read_json('input.json', orient='records')
Columns:
        a       int
        b       int
Rows: 3
Data:
+---+---+
| a | b |
+---+---+
| 1 | 1 |
| 2 | 2 |
| 3 | 3 |
+---+---+

# 导出
sf.export_json('/../1.json')
sf.save('1.json', format='json')
```

其中!cat 有查看功能。

## 1.3 自由格式保存

```bash
sf.save('/../mysframedir')
sf2 = tc.load_sframe('/../mysframedir')
```

有点像R里面的workspace的保存，一股脑保存出去，然后再读进来。 

.

# 二、SFrame基本操作

## 2.1 生成SFrame框

借助SArray来生成：

```
from turicreate import SArray
ids = tc.SArray([1,2,3])
vals = tc.SArray(['A','B','C'])
#ids = SArray([1,2,3])
#vals = SArray(['A','B','C'])
sf = SFrame({'id':ids,'val':vals})
sf
```

下面也有从dict格式导入、还有从pd.dataframe两种格式导入。

```bash
sf = tc.SFrame({'id':[1,2,3],'val':['A','B','C']})  # 直接导入
sf = tc.SFrame(pd.DataFrame({'id':[1,2,3],'val':['A','B','C']}))  # pd.dataframe
sf
```

## 2.2 列操作

### 2.2.1 选中列

通过[‘name’]选中。

```
sf = SFrame({'id':[1,2,3],'val':['A','B','C']})
sf['id']
sf[['val','id']]
del sf["id"] # 删除列
```

这个跟pd.DataFrame一样 

通过一个函数来选择：

```
>>> sf = turicreate.SFrame({'user_id': [1,2,3],
...                       'user_name': ['alice', 'bob', 'charlie'],
...                       'zipcode': [98101, 98102, 98103]
...                      })
>>> # This line is equivalent to `sf2 = sf[['user_id', 'zipcode']]`
>>> sf2 = sf.select_columns(['user_id', 'zipcode'])
>>> sf2
+---------+---------+
| user_id | zipcode |
+---------+---------+
|    1    |  98101  |
|    2    |  98102  |
|    3    |  98103  |
+---------+---------+
[3 rows x 2 columns]
```

### 2.2.2 列举列名

```
# 找列名以及每一列的type
sf = SFrame({'id':[1,2,3],'val':['A','B','C']})
print(sf.column_names())
print(sf.column_types())
```

### 2.2.3 新增列 .add_column（）

```
# 新增列
>>> sf = turicreate.SFrame({'id': [1, 2, 3], 'val': ['A', 'B', 'C']})
>>> sa = turicreate.SArray(['cat', 'dog', 'fossa'])
>>> # This line is equivalant to `sf['species'] = sa`
>>> res = sf.add_column(sa, 'species')
>>> res
+----+-----+---------+
| id | val | species |
+----+-----+---------+
| 1  |  A  |   cat   |
| 2  |  B  |   dog   |
| 3  |  C  |  fossa  |
+----+-----+---------+
[3 rows x 3 columns]
```

## 2.3 行操作

### 2.3.1 选中行

```python
sf[1]
sf[1:]

>>> {'id': 2, 'val': 'B'}
```

切片一样，但是sf[1]代表选中第二行，返回的是dict格式！奇特！

### 2.3.2 新增行

等同于pd.concat

```
# 新增数据：行联合
>>> sf = turicreate.SFrame({'id': [4, 6, 8], 'val': ['D', 'F', 'H']})
>>> sf2 = turicreate.SFrame({'id': [1, 2, 3], 'val': ['A', 'B', 'C']})
>>> sf = sf.append(sf2)
>>> sf
+----+-----+
| id | val |
+----+-----+
| 4  |  D  |
| 6  |  F  |
| 8  |  H  |
| 1  |  A  |
| 2  |  B  |
| 3  |  C  |
+----+-----+
[6 rows x 2 columns]
```

## 2.4 逻辑选中行列 + 复合选中

```bash
sf[(sf['id'] >= 1) & (sf['id'] <= 2)]
```

```
sf = SFrame({'id':[1,2,3],'val':['A','B','C'],'val2':[5,6,7]})
sf[2]['id']
```

如何选中’id’列的第三行内容，可以通过这样的方式选中。 

（sf.ix[2,1]的方式不可用）

## 2.5 新增index

SFrame本身不会生成Index，跟pd的不一样。需要额外生成上去。

```
# 新增Index
>>> sf = turicreate.SFrame({'a': [1, None, None], 'b': ['a', 'b', None]})
+------+------+
|  a   |  b   |
+------+------+
|  1   |  a   |
| None |  b   |
| None | None |
+------+------+
>>> sf.add_row_number()
+----+------+------+
| id |  a   |  b   |
+----+------+------+
| 0  |  1   |  a   |
| 1  | None |  b   |
| 2  | None | None |
+----+------+------+
[3 rows x 3 columns]
```

## 2.6 缺失值处理

### 2.6.1 缺失值直接去掉

有以下几种模式：

```
# 只保留全部有数字的行
>>> sf = turicreate.SFrame({'a': [1, None, None], 'b': ['a', 'b', None]})
>>> sf.dropna()
+---+---+
| a | b |
+---+---+
| 1 | a |
+---+---+
[1 rows x 2 columns]

# 只去掉全部为None的行
>>> sf.dropna(any="all")
+------+---+
|  a   | b |
+------+---+
|  1   | a |
| None | b |
+------+---+
[2 rows x 2 columns]

# 按照a这行进行去缺失值
>>> sf.dropna('a', any="all")
+---+---+
| a | b |
+---+---+
| 1 | a |
+---+---+
[1 rows x 2 columns]

# 把缺失值去掉的也保存下来
>>> sf = turicreate.SFrame({'a': [1, None, None], 'b': ['a', 'b', None]})
>>> good, bad = sf.dropna_split()
>>> good
+---+---+
| a | b |
+---+---+
| 1 | a |
+---+---+
[1 rows x 2 columns]
>>> bad
+------+------+
|  a   |  b   |
+------+------+
| None |  b   |
| None | None |
+------+------+
[2 rows x 2 columns]
```

### 2.6.2 缺失值填补

```
# 缺失值填补
>>> sf = turicreate.SFrame({'a':[1, None, None],
...                       'b':['13.1', '17.2', None]})
>>> sf = sf.fillna('a', 0)
>>> sf
+---+------+
| a |  b   |
+---+------+
| 1 | 13.1 |
| 0 | 17.2 |
| 0 | None |
+---+------+
[3 rows x 2 columns]
```

## 2.7 数据排序

数据排序有以下几种：

```
# 按某行排序
>>> sf = turicreate.SFrame({'a':[1,3,2,1],
...                       'b':['a','c','b','b'],
...                       'c':['x','y','z','y']})
>>> sf
+---+---+---+
| a | b | c |
+---+---+---+
| 1 | a | x |
| 3 | c | y |
| 2 | b | z |
| 1 | b | y |
+---+---+---+
[4 rows x 3 columns]

>>> sf.sort('a')
+---+---+---+
| a | b | c |
+---+---+---+
| 1 | a | x |
| 1 | b | y |
| 2 | b | z |
| 3 | c | y |
+---+---+---+
[4 rows x 3 columns]

>>> sf.sort('a', ascending = False)
+---+---+---+
| a | b | c |
+---+---+---+
| 3 | c | y |
| 2 | b | z |
| 1 | a | x |
| 1 | b | y |
+---+---+---+
[4 rows x 3 columns]

>>> sf.sort(['a', 'b'])
+---+---+---+
| a | b | c |
+---+---+---+
| 1 | a | x |
| 1 | b | y |
| 2 | b | z |
| 3 | c | y |
+---+---+---+
[4 rows x 3 columns]
```

## 2.8 数据合并

等同于pd.merge

```
# 数据合并
>>> animals = turicreate.SFrame({'id': [1, 2, 3, 4],
...                           'name': ['dog', 'cat', 'sheep', 'cow']})
>>> sounds = turicreate.SFrame({'id': [1, 3, 4, 5],
...                          'sound': ['woof', 'baa', 'moo', 'oink']})
>>> animals.join(sounds, how='inner')
+----+-------+-------+
| id |  name | sound |
+----+-------+-------+
| 1  |  dog  |  woof |
| 3  | sheep |  baa  |
| 4  |  cow  |  moo  |
+----+-------+-------+
[3 rows x 3 columns]

>>> animals.join(sounds, on='id', how='left')
+----+-------+-------+
| id |  name | sound |
+----+-------+-------+
| 1  |  dog  |  woof |
| 3  | sheep |  baa  |
| 4  |  cow  |  moo  |
| 2  |  cat  |  None |
+----+-------+-------+
[4 rows x 3 columns]

>>> animals.join(sounds, on=['id'], how='right')
+----+-------+-------+
| id |  name | sound |
+----+-------+-------+
| 1  |  dog  |  woof |
| 3  | sheep |  baa  |
| 4  |  cow  |  moo  |
| 5  |  None |  oink |
+----+-------+-------+
[4 rows x 3 columns]

>>> animals.join(sounds, on={'id':'id'}, how='outer')
+----+-------+-------+
| id |  name | sound |
+----+-------+-------+
| 1  |  dog  |  woof |
| 3  | sheep |  baa  |
| 4  |  cow  |  moo  |
| 5  |  None |  oink |
| 2  |  cat  |  None |
+----+-------+-------+
[5 rows x 3 columns]
```

## 2.9 数据去重

```
# 去重
>>> sf = turicreate.SFrame({'id':[1,2,3,3,4], 'value':[1,2,3,3,4]})
>>> sf
+----+-------+
| id | value |
+----+-------+
| 1  |   1   |
| 2  |   2   |
| 3  |   3   |
| 3  |   3   |
| 4  |   4   |
+----+-------+
[5 rows x 2 columns]

>>> sf.unique()
+----+-------+
| id | value |
+----+-------+
| 2  |   2   |
| 4  |   4   |
| 3  |   3   |
| 1  |   1   |
+----+-------+
[4 rows x 2 columns]
```

.

# 三、SFrame数据运行

## 3.1  使用apply并行

```
# 如何使用apply
>>> sf = turicreate.SFrame({'user_id': [1, 2, 3], 'movie_id': [3, 3, 6],
                          'rating': [4, 5, 1]})
>>> sf.apply(lambda x: str(x['user_id']) + str(x['movie_id']) + str(x['rating']))
dtype: str
Rows: 3
['134', '235', '361']
```

## 3.2 分组汇总sf.groupby

```
>>> import turicreate.aggregate as agg
>>> url = 'https://static.turi.com/datasets/rating_data_example.csv'
>>> sf = turicreate.SFrame.read_csv(url)
>>> sf
+---------+----------+--------+
| user_id | movie_id | rating |
+---------+----------+--------+
|  25904  |   1663   |   3    |
|  25907  |   1663   |   3    |
|  25923  |   1663   |   3    |
|  25924  |   1663   |   3    |
|  25928  |   1663   |   2    |
|  25933  |   1663   |   4    |
|  25934  |   1663   |   4    |
|  25935  |   1663   |   4    |
|  25936  |   1663   |   5    |
|  25937  |   1663   |   2    |
|   ...   |   ...    |  ...   |
+---------+----------+--------+
[10000 rows x 3 columns]

# 分组加总
>>> user_count = sf.groupby(key_column_names='user_id',
...                         operations={'count': agg.COUNT()})
>>> user_count
+---------+-------+
| user_id | count |
+---------+-------+
|  62361  |   1   |
|  30727  |   1   |
|  40111  |   1   |
|  50513  |   1   |
|  35140  |   1   |
|  42352  |   1   |
|  29667  |   1   |
|  46242  |   1   |
|  58310  |   1   |
|  64614  |   1   |
|   ...   |  ...  |
+---------+-------+
[9852 rows x 2 columns]

#
>>> user_rating_stats = sf.groupby(key_column_names='user_id',
...                                operations={
...                                    'mean_rating': agg.MEAN('rating'),
...                                    'std_rating': agg.STD('rating')
...                                })
>>> user_rating_stats
+---------+-------------+------------+
| user_id | mean_rating | std_rating |
+---------+-------------+------------+
|  62361  |     5.0     |    0.0     |
|  30727  |     4.0     |    0.0     |
|  40111  |     2.0     |    0.0     |
|  50513  |     4.0     |    0.0     |
|  35140  |     4.0     |    0.0     |
|  42352  |     5.0     |    0.0     |
|  29667  |     4.0     |    0.0     |
|  46242  |     5.0     |    0.0     |
|  58310  |     2.0     |    0.0     |
|  64614  |     2.0     |    0.0     |
|   ...   |     ...     |    ...     |
+---------+-------------+------------+
[9852 rows x 3 columns]
```

## 3.3 数据dataframe框内拆解

```
# 数据dataframe框内拆解
>>> sf = turicreate.SFrame({'topic':[1,2,3,4],
...                       'words': [{'a':3, 'cat':2},
...                                 {'a':1, 'the':2},
...                                 {'the':1, 'dog':3},
...                                 {}]
...                      })
+-------+----------------------+
| topic |        words         |
+-------+----------------------+
|   1   |  {'a': 3, 'cat': 2}  |
|   2   |  {'a': 1, 'the': 2}  |
|   3   | {'the': 1, 'dog': 3} |
|   4   |          {}          |
+-------+----------------------+
[4 rows x 2 columns]

# 一个拆成两列
>>> sf.stack('words', new_column_name=['word', 'count'])
+-------+------+-------+
| topic | word | count |
+-------+------+-------+
|   1   |  a   |   3   |
|   1   | cat  |   2   |
|   2   |  a   |   1   |
|   2   | the  |   2   |
|   3   | the  |   1   |
|   3   | dog  |   3   |
|   4   | None |  None |
+-------+------+-------+
[7 rows x 3 columns]

# 拆成一列
>>> sf = turicreate.SFrame({'topic':[1,2,3],
...                       'friends':[[2,3,4], [5,6],
...                                  [4,5,10,None]]
...                      })
>>> sf
+-------+------------------+
| topic |     friends      |
+-------+------------------+
|  1    |     [2, 3, 4]    |
|  2    |      [5, 6]      |
|  3    | [4, 5, 10, None] |
+----- -+------------------+
[3 rows x 2 columns]
```

## 3.4 格式转换

```
# 格式转换
SFrame.to_dataframe()   Convert this SFrame to pandas.DataFrame.
SFrame.to_numpy()   Converts this SFrame to a numpy array
SFrame.to_sql(conn, table_name[, ...])  Convert an SFrame to a single table in a SQL database.
```


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




