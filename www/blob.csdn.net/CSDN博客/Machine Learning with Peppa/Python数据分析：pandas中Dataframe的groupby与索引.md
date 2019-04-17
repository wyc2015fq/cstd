# Python数据分析：pandas中Dataframe的groupby与索引 - Machine Learning with Peppa - CSDN博客





2018年03月15日 21:44:16[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：3131








## **一、****索引**

Series和DataFrame都是有索引的，索引的好处是快速定位，在涉及到两个Series或DataFrame时可以根据索引自动对齐，比如日期自动对齐，这样可以省去很多事。

### 缺失值

```python
pd.isnull(obj)
obj.isnull()
```

### 将字典转成数据框，并赋予列名,索引

```python
DataFrame(data, columns=['col1','col2','col3'...],
            index = ['i1','i2','i3'...])
```
- 


### 查看列名

`DataFrame.columns`

### 查看索引

`DataFrame.index`

### 重建索引

```python
obj.reindex(['a','b','c','d','e'...], fill_value=0]
#按给出的索引顺序重新排序，而不是替换索引。如果索引没有值，就用0填充

#就地修改索引
data.index=data.index.map(str.upper)
```

### 列顺序重排（也是重建索引）

```python
DataFrame.reindex[columns=['col1','col2','col3'...])`

#也可以同时重建index和columns

DataFrame.reindex[index=['a','b','c'...],columns=['col1','col2','col3'...])
```
- 


### 重建索引的快捷键

```python
DataFrame.ix[['a','b','c'...],['col1','col2','col3'...]]
```
- 


### 重命名轴索引

```python
data.rename(index=str.title,columns=str.upper)

#修改某个索引和列名，可以通过传入字典
data.rename(index={'old_index':'new_index'},
            columns={'old_col':'new_col'})
```

### 查看某一列

```python
DataFrame['state'] 或 DataFrame.state
```

### 查看某一行

需要用到索引

```python
DataFrame.ix['index_name']
```
- 


### 添加或删除一列

```python
DataFrame['new_col_name'] = 'char_or_number'
#删除行
DataFrame.drop(['index1','index2'...])
#删除列
DataFrame.drop(['col1','col2'...],axis=1)
#或
del DataFrame['col1']
```
- 


### DataFrame选择子集
|类型|说明|
|----|----|
|obj[val]|选择一列或多列|
|obj.ix[val]|选择一行或多行|
|obj.ix[:,val]|选择一列或多列|
|obj.ix[val1,val2]|同时选择行和列|
|reindx|对行和列重新索引|
|icol,irow|根据整数位置选取单列或单行|
|get_value,set_value|根据行标签和列标签选择单个值|
针对series
```python
obj[['a','b','c'...]]
obj['b':'e']=5
```
- 


针对dataframe

```python
#选择多列
dataframe[['col1','col2'...]]

#选择多行
dataframe[m:n]

#条件筛选
dataframe[dataframe['col3'>5]]

#选择子集
dataframe.ix[0:3,0:5]
```

### dataframe和series的运算

会根据 index 和 columns 自动对齐然后进行运算，很方便啊
|方法|说明|
|----|----|
|add|加法|
|sub|减法|
|div|除法|
|mul|乘法|

```python
#没有数据的地方用0填充空值
df1.add(df2,fill_value=0)

# dataframe 与 series 的运算
dataframe - series
规则是：
--------   --------  |
|      |   |      |  |
|      |   --------  |
|      |             |
|      |             v
--------
#指定轴方向
dataframe.sub(series,axis=0)
规则是：
--------   ---  
|      |   | |   ----->
|      |   | | 
|      |   | | 
|      |   | | 
--------   ---
```
- 


### apply函数

```python
f=lambda x:x.max()-x.min()

#默认对每一列应用
dataframe.apply(f)

#如果需要对每一行分组应用
dataframe.apply(f,axis=1)
```

### 排序和排名

```python
#默认根据index排序，axis = 1 则根据columns排序
dataframe.sort_index(axis=0, ascending=False)

# 根据值排序
dataframe.sort_index(by=['col1','col2'...])

#排名，给出的是rank值

series.rank(ascending=False)
#如果出现重复值，则取平均秩次

#在行或列上面的排名
dataframe.rank(axis=0)
```
- 


### 描述性统计
|方法|说明|
|----|----|
|count|计数|
|describe|给出各列的常用统计量|
|min,max|最大最小值|
|argmin,argmax|最大最小值的索引位置（整数）|
|idxmin,idxmax|最大最小值的索引值|
|quantile|计算样本分位数|
|sum,mean|对列求和，均值|
|mediam|中位数|
|mad|根据平均值计算平均绝对离差|
|var,std|方差，标准差|
|skew|偏度（三阶矩）|
|Kurt|峰度（四阶矩）|
|cumsum|累积和|
|Cummins，cummax|累计组大致和累计最小值|
|cumprod|累计积|
|diff|一阶差分|
|pct_change|计算百分数变化|
唯一值，值计数，成员资格
```python
obj.unique()
obj.value_count()
obj.isin(['b','c'])
```
- 


### 处理缺失值

```python
# 过滤缺失值

# 只要有缺失值就丢弃这一行
dataframe.dropna()
#要求全部为缺失才丢弃这一行
dataframe.dropna(how='all')
# 根据列来判断
dataframe.dropna(how='all',axis=1)

# 填充缺失值

#1.用0填充
df.fillna(0)

#2.不同的列用不同的值填充
df.fillna({1:0.5, 3:-1})

#3.用均值填充
df.fillna(df.mean())

# 此时axis参数同前面
```




## 二、Groupby

pandas提供了一个灵活高效的groupby功能，它使你能以一种自然的方式对数据集进行切片、切块、摘要等操作。根据一个或多个键（可以是函数、数组或DataFrame列名）拆分pandas对象。计算分组摘要统计，如计数、平均值、标准差，或用户自定义函数。对DataFrame的列应用各种各样的函数。应用组内转换或其他运算，如规格化、线性回归、排名或选取子集等。计算透视表或交叉表。执行分位数分析以及其他分组分析。




1）查看DataFrame数据及属性

```
df_obj = DataFrame() #创建DataFrame对象
df_obj.dtypes #查看各行的数据格式
df_obj['列名'].astype(int)#转换某列的数据类型
df_obj.head() #查看前几行的数据,默认前5行
df_obj.tail() #查看后几行的数据,默认后5行
df_obj.index #查看索引
df_obj.columns #查看列名
df_obj.values #查看数据值
df_obj.describe() #描述性统计
df_obj.T #转置
df_obj.sort_values(by=['',''])#同上
```

2）使用DataFrame选择数据:

```
df_obj.ix[1:3] #获取1-3行的数据,该操作叫切片操作,获取行数据
df_obj.ix[columns_index] #获取列的数据
df_obj.ix[1:3，[1,3]]#获取1列3列的1~3行数据
df_obj[columns].drop_duplicates() #剔除重复行数据
```
- 


3）使用DataFrame重置数据:
`df_obj.ix[1:3，[1,3]]=1#所选位置数据替换为1`- 


4）使用DataFrame筛选数据(类似SQL中的WHERE):

```bash
alist = ['023-18996609823']
df_obj['用户号码'].isin(alist) #将要过滤的数据放入字典中,使用isin对数据进行筛选,返回行索引以及每行筛选的结果,若匹配则返回ture
df_obj[df_obj['用户号码'].isin(alist)] #获取匹配结果为ture的行
```
- 


5）使用DataFrame模糊筛选数据(类似SQL中的LIKE):

```python
df_obj[df_obj['套餐'].str.contains(r'.*?语音CDMA.*')] #使用正则表达式进行模糊匹配,*匹配0或无限次,?匹配0或1次
```
- 


6）使用DataFrame进行数据转换(后期补充说明)
`df_obj['支局_维护线'] = df_obj['支局_维护线'].str.replace('巫溪分公司(.{2,})支局','\\1')#可以使用正则表达式`


可以设置take_last=ture 保留最后一个,或保留开始一个.补充说明:注意take_last=ture已过时,请使用keep=’last’ 



7）使用pandas中读取数据:
```bash
read_csv('D:\LQJ.csv',sep=';',nrows=2) #首先输入csv文本地址,然后分割符选择等等
df.to_excel('foo.xlsx',sheet_name='Sheet1');pd.read_excel('foo.xlsx', 'Sheet1', index_col=None, na_values=['NA'])#写入读取excel数据，pd.read_excel读取的数据是以DataFrame形式存储
df.to_hdf('foo.h5','df');pd.read_hdf('foo.h5','df')#写入读取HDF5数据
```
- 


8）使用pandas聚合数据(类似SQL中的GROUP BY 或HAVING):

```bash
data_obj['用户标识'].groupby(data_obj['支局_维护线'])
data_obj.groupby('支局_维护线')['用户标识'] #上面的简单写法
adsl_obj.groupby('支局_维护线')['用户标识'].agg([('ADSL','count')])#按支局进行汇总对用户标识进行计数,并将计数列的列名命名为ADSL
```
- 


9）使用pandas合并数据集(类似SQL中的JOIN):
`merge(mxj_obj2, mxj_obj1 ,on='用户标识',how='inner')# mxj_obj1和mxj_obj2将用户标识当成重叠列的键合并两个数据集,inner表示取两个数据集的交集.`- 


10）清理数据

```
df[df.isnull()]
df[df.notnull()]
df.dropna()#将所有含有nan项的row删除
df.dropna(axis=1,thresh=3) #将在列的方向上三个为NaN的项删除
df.dropna(how='ALL')#将全部项都是nan的row删除填充值
df.fillna(0)
df.fillna({1:0,2:0.5}) #对第一列nan值赋0，第二列赋值0.5
df.fillna(method='ffill') #在列方向上以前一个值作为值赋给NaN
```
- 


## 实例

1. 读取excel数据
代码如下

```
import pandas as pd# 读取高炉数据，注意文件名不能为中文
data=pd.read_excel('gaolushuju_201501-03.xlsx', '201501', index_col=None, na_values=['NA'])
print data
```

测试结果如下

```
燃料比  顶温西南  顶温西北  顶温东南  顶温东北
0   531.46   185   176   176   174
1   510.35   184   173   184   188
2   533.49   180   165   182   177
3   511.51   190   172   179   188
4   531.02   180   167   173   180
5   511.24   174   164   178   176
6   532.62   173   170   168   179
7   583.00   182   175   176   173
8   530.70   158   149   159   156
9   530.32   168   156   169   171
10  528.62   164   150   171   169
```
- 


2. 切片处理，选取行或列，修改数据
代码如下：

```python
data_1row=data.ix[1]
data_5row_2col=data.ix[0:5,[u'燃料比',u'顶温西南']
print data_1row,data_5row_2col
data_5row_2col.ix[0:1,0:2]=3
```
- 


测试结果如下：

```
燃料比     510.35
顶温西南    184.00
顶温西北    173.00
顶温东南    184.00
顶温东北    188.00
Name: 1, dtype: float64    
   燃料比  顶温西南
0  531.46   185
1  510.35   184
2  533.49   180
3  511.51   190
4  531.02   180
5  511.24   174
      燃料比  顶温西南
0    3.00     3
1    3.00     3
2  533.49   180
3  511.51   190
4  531.02   180
5  511.24   174
```
- 


格式说明，data_5row_2col.ix[0:1,0:2]，data_5row_2col.ix[0:1,[0,2]],选取部分行和列需加”[]” 
3. 排序
代码如下：

```python
print data_1row.sort_values()
print data_5row_2col.sort_values(by=u'燃料比')
```

测试结果如下：

```
顶温西北    173.00
顶温西南    184.00
顶温东南    184.00
顶温东北    188.00
燃料比     510.35
Name: 1, dtype: float64
      燃料比  顶温西南
1  510.35   184
5  511.24   174
3  511.51   190
4  531.02   180
0  531.46   185
2  533.49   180
```

4. 删除重复的行
代码如下：

```python
print data_5row_2col[u'顶温西南'].drop_duplicates()#剔除重复行数据
```

测试结果如下：

```
0    185
1    184
2    180
3    190
5    174
Name: 顶温西南, dtype: int64
```
- 说明：从测试结果3中可以看出顶温西南index=2的数据与index=4的数据重复，测试结果4显示将index=4的顶温西南数据删除

5. 参考文献
[10分钟搞定pandas](http://pandas.pydata.org/pandas-docs/stable/10min.html%2010%E5%88%86%E9%92%9F%E6%90%9E%E5%AE%9Apandas)
[numpy+pandas series dataframe常用函数](http://blog.csdn.net/baoyan2015/article/details/53503073%20numpy+pandas%20series%20dataframe%E5%B8%B8%E7%94%A8%E5%87%BD%E6%95%B0)
[python基础教程](http://www.runoob.com/python/func-number-round.html%20python%E5%9F%BA%E7%A1%80%E6%95%99%E7%A8%8B)




