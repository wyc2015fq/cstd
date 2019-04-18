# pandas+matplotlib——习题三 - wsp_1138886114的博客 - CSDN博客





2018年07月19日 18:35:05[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：315









- - - - - [1. 导入数据并查看](#1-导入数据并查看)
- [2. 删除 【Unname:0】列 和 【Id】列(两种方法)](#2-删除-unname0列-和-id列两种方法)
- [3 判断出数据集中 男孩名字、女孩名字谁多](#3-判断出数据集中-男孩名字女孩名字谁多)
- [4 按照 【Name】列将数据集进行分组 并求和](#4-按照-name列将数据集进行分组-并求和)
- [5 以列【Count】的值对DataFrame降序排序，并查看](#5-以列count的值对dataframe降序排序并查看)
- [6 统计数据集中名字个数？(不包含重复项，至少使用两种方法)](#6-统计数据集中名字个数不包含重复项至少使用两种方法)
- [7 出现次数最少的名字共有几个？](#7-出现次数最少的名字共有几个)








`运行环境 jupyter notebook`

数据集下载地址：[https://download.csdn.net/download/wsp_1138886114/10558012](https://download.csdn.net/download/wsp_1138886114/10558012)


```python
import  matplotlib.pyplot as plt 
from pandas import DataFrame,Series
import pandas as pd
import numpy as np
```

##### 1. 导入数据并查看

```
data01 = pd.read_csv("datasets/US_Baby_Names_right.csv")
data01.head(5)
```
|$~$|**Unnamed: 0**|**Id**|**Name**|**Year**|**Gender**|**State**|**Count**|
|----|----|----|----|----|----|----|----|
|**0**|11349|11350|Emma|2004|F|AK|62|
|**1**|11350|11351|Madison|2004|F|AK|48|
|**2**|11351|11352|Hannah|2004|F|AK|46|
|**3**|11352|11353|Grace|2004|F|AK|44|
|**4**|11353|11354|Emily|2004|F|AK|41|

##### 2. 删除 【Unname:0】列 和 【Id】列(两种方法)

```python
#方法1
data02 = data01.drop(labels=['Unnamed: 0','Id'],inplace=False,axis=1)   #inplace=False 不删除源数据
data02.head(5)
#方法2
del data01['Unnamed: 0']
del data01['Id']
```
|$~$|**Name**|**Year**|**Gender**|**State**|**Count**|
|----|----|----|----|----|----|
|**0**|Emma|2004|F|AK|62|
|**1**|Madison|2004|F|AK|48|
|**2**|Hannah|2004|F|AK|46|
|**3**|Grace|2004|F|AK|44|
|**4**|Emily|2004|F|AK|41|

##### 3 判断出数据集中 男孩名字、女孩名字谁多

```
Gender_data = data01["Gender"].value_counts()
Gender_data 
输出：
F    558846
M    457549
Name: Gender, dtype: int64 
---------------------------------------
a = Gender_data['F']       #名称索引
b = Gender_data['M']       #名称索引
if a>b:
    print("女孩数量大于男孩")
else:
    print("男孩数量大于女孩") 
输出：
女孩数量大于男孩
```

##### 4 按照 【Name】列将数据集进行分组 并求和

```
names = data01.groupby('Name').sum()
names.head() 
输出：
```
|$~$|**Year**|**Count**|
|----|----|----|
|**Name**|||
|**Aaban**|4027|12|
|**Aadan**|8039|23|
|**Aadarsh**|2009|5|
|**Aaden**|393963|3426|
|**Aadhav**|2014|6|

##### 5 以列【Count】的值对DataFrame降序排序，并查看
`names.sort_values("Count",ascending=False).head()`|$~$|**Year**|**Count**|
|----|----|----|
|**Name**|||
|**Jacob**|1141099|242874|
|**Emma**|1137085|214852|
|**Michael**|1161152|214405|
|**Ethan**|1139091|209277|
|**Isabella**|1137090|204798|

##### 6 统计数据集中名字个数？(不包含重复项，至少使用两种方法)

```
data03 = data01.drop_duplicates(['Name'])
data03.value_counts() 
------------------------------------------------------ 
len(baby_names["Name"].unique())
len(names)
输出：
    17632
```
|$~$|**Unnamed: 0**|**Id**|**Name**|**Year**|**Gender**|**State**|**Count**|**gender_M**|
|----|----|----|----|----|----|----|----|----|
|**0**|11349|11350|Emma|2004|F|AK|62|0|
|**1**|11350|11351|Madison|2004|F|AK|48|0|
|**2**|11351|11352|Hannah|2004|F|AK|46|0|
|**3**|11352|11353|Grace|2004|F|AK|44|0|
|**4**|11353|11354|Emily|2004|F|AK|41|0|

##### 7 出现次数最少的名字共有几个？

```python
len(names[names.Count == names.Count.min()])
输出：
    2578
```



