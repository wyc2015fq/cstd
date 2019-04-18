# Pandas使用DataFrame进行数据分析比赛进阶之路（一） - Nicole的博客 - CSDN博客
2018年05月18日 18:10:33[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：351
这篇文章中使用的数据集是一个足球球员各项技能及其身价的csv表，包含了60多个字段。数据集下载链接：[数据集](http://sofasofa.io/competition.php?id=7#c2)
**1、DataFrame.info()**
这个函数可以输出读入表格的一些具体信息。这对于加快数据预处理非常有帮助。
```
import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('dataset/soccer/train.csv')
print(data.info())
```
``
```
<class 'pandas.core.frame.DataFrame'>
RangeIndex: 10441 entries, 0 to 10440
Data columns (total 65 columns):
id                          10441 non-null int64
club                        10441 non-null int64
league                      10441 non-null int64
birth_date                  10441 non-null object
height_cm                   10441 non-null int64
weight_kg                   10441 non-null int64
nationality                 10441 non-null int64
potential                   10441 non-null int64
                   ...
dtypes: float64(12), int64(50), object(3)
memory usage: 5.2+ MB
None
```
**2、DataFrame.query()**
```
import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('dataset/soccer/train.csv')
print(data.query('lw>cf'))      # 这两个方法是等价的
print(data[data.lw > data.cf])  # 这两个方法是等价的
```
**3、DataFrame.value_counts()**
这个函数可以统计某一列中不同值出现的频率。
```
import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('dataset/soccer/train.csv')
print(data.work_rate_att.value_counts())
```
```
Medium    7155
High      2762
Low        524
Name: work_rate_att, dtype: int64
```
**4、DataFrame.sort_values()**
按照某一列的数值进行排序后输出。
```
import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('dataset/soccer/train.csv')
print(data.sort_values(['sho']).head(5))
```
**5、DataFrame.groupby()**
- 根据国籍（nationality）这一列的属性进行分组，然后分别计算相同国籍的潜力（potential）的平均值。
```
import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('dataset/soccer/train.csv')
potential_mean = data['potential'].groupby(data['nationality']).mean().head(5)
print(potential_mean)
```
```
nationality
1    74.945338
2    72.914286
3    67.892857
4    69.000000
5    70.024242
Name: potential, dtype: float64
```
根据国籍（nationality），俱乐部（club）这两列的属性进行分组，然后分别计算球员潜力（potential）的平均值。
```
import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('dataset/soccer/train.csv')
potential_mean = data['potential'].head(20).groupby([data['nationality'], data['club']]).mean()
print(potential_mean)
```
```
nationality  club
1            148     76
             461     72
5            83      64
29           593     68
43           213     67
51           258     62
52           112     68
54           604     81
63           415     70
64           359     74
78           293     73
90           221     70
96           80      72
101          458     67
111          365     64
             379     83
             584     65
138          9       72
155          543     72
163          188     71
Name: potential, dtype: int64
```
值得注意的是，在分组函数后面**使用一个size（）**函数可以返回带有分组大小的结果。`potential_mean = data['potential'].head(200).groupby([data['nationality'], data['club']]).size()`
```
nationality  club
1            148     1
43           213     1
51           258     1
52           112     1
54           604     1
78           293     1
96           80      1
101          458     1
155          543     1
163          188     1
Name: potential, dtype: int64
```
**6、DataFrame.agg()**
这个函数一般在groupby函数之后使用。
```
import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv('dataset/soccer/train.csv')
potential_mean = data['potential'].head(10).groupby(data['nationality']).agg(['max', 'min'])
print(potential_mean)
```
```
max  min
nationality          
1             76   76
43            67   67
51            62   62
52            68   68
54            81   81
78            73   73
96            72   72
101           67   67
155           72   72
163           71   71
```
**7、DataFrame.apply()**
将某一个函数应用到某一列或者某一行上，可以极大加快处理速度。
```python
import pandas as pd
import matplotlib.pyplot as plt
#  返回球员出生日期中的年份
def birth_date_deal(birth_date):
    year = birth_date.split('/')[2]
    return year
data = pd.read_csv('dataset/soccer/train.csv')
result = data['birth_date'].apply(birth_date_deal).head() 
print(result)
```
```
0    96
1    84
2    99
3    88
4    80
Name: birth_date, dtype: object
```
当然如果使用lambda函数的话，代码会更加简洁：
```
data = pd.read_csv('dataset/soccer/train.csv')
result = data['birth_date'].apply(lambda x: x.split('/')[2]).head()
print(result)
```
