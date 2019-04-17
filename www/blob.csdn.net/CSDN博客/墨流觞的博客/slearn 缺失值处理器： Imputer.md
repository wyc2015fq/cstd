# slearn  缺失值处理器： Imputer - 墨流觞的博客 - CSDN博客





2018年09月24日 19:00:02[墨氲](https://me.csdn.net/dss_dssssd)阅读数：820








`class sklearn.preprocessing.Imputer(missing_values=’NaN’, strategy=’mean’, axis=0, verbose=0, copy=True)`

参数：
- missing_values: integer or “NaN”, optional (default=”NaN”)
- strategy : string, optional (default=”mean”)
- The imputation strategy.
- If “mean”, then replace missing values using the mean along the axis. 使用平均值代替
- If “median”, then replace missing values using the median along the axis.使用中值代替
- If “most_frequent”, then replace missing using the most frequent value along the axis.使用众数代替，也就是出现次数最多的数


- axis: 默认为 axis=0
- axis = 0, 按列处理
- aixs =1 , 按行处理


说实话，我还是没太弄明白aixs的具体含义，总感觉在不同的函数中有不同的含义。。还是使用前查找一下官方文档吧，毕竟大多数时候处理的都是2维数组,文档中的参数很容易理解。
**注意：**
- Imputer 只接受DataFrame类型
- Dataframe 中必须全部为数值属性

所以在处理的时候注意，要进行适当处理。
- 数值属性的列较少，可以将数值属性的列取出来 单独取出来

```python
```python
import pandas as pd
import numpy as np

df=pd.DataFrame([["XXL", 8, "black", "class 1", 22],
["L", np.nan, "gray", "class 2", 20],
["XL", 10, "blue", "class 2", 19],
["M", np.nan, "orange", "class 1", 17],
["M", 11, "green", "class 3", np.nan],
["M", 7, "red", "class 1", 22]])

df.columns=["size", "price", "color", "class", "boh"]
print(df)
# out:
'''
  size  price   color    class   boh
0  XXL    8.0   black  class 1  22.0
1    L    NaN    gray  class 2  20.0
2   XL   10.0    blue  class 2  19.0
3    M    NaN  orange  class 1  17.0
4    M   11.0   green  class 3   NaN
5    M    7.0     red  class 1  22.0
'''
from sklearn.preprocessing import Imputer
# 1. 创建Imputer器
imp =Imputer(missing_values="NaN", strategy="mean",axis=0 )
# 先只将处理price列的数据， 注意使用的是   df[['price']]   这样返回的是一个DataFrame类型的数据！！！！
# 2. 使用fit_transform()函数即可完成缺失值填充了
df["price"]=imp.fit_transform(df[["price"]])
df
# out:
'''
   size	price	color	class	boh
0	XXL	8.0	black	class 1	22.0
1	L	9.0	gray	class 2	20.0
2	XL	10.0	blue	class 2	19.0
3	M	9.0	orange	class 1	17.0
4	M	11.0	green	class 3	NaN
5	M	7.0	red	class 1	22.0
'''

# 直接处理price和boh两列
df[['price', 'boh']] = imp.fit_transform(df[['price', 'boh']])
df
# out:
'''
size	price	color	class	boh
0	XXL	8.0	black	class 1	22.0
1	L	9.0	gray	class 2	20.0
2	XL	10.0	blue	class 2	19.0
3	M	9.0	orange	class 1	17.0
4	M	11.0	green	class 3	20.0
5	M	7.0	red	class 1	22.0
'''
```
```
- 数值属性的列较多，相反文本或分类属性（text and category  attribute)较少，可以先删除文本属性，处理完以后再合并

```python
```python
from sklearn.preprocessing import Imputer
# 1.创建Iimputer
imputer = Imputer(strategy="median")
# 只有一个文本属性，故先去掉
housing_num = housing.drop("ocean_proximity", axis=1)
# 2. 使用fit_transform函数
X = imputer.fit_transform(housing_num)
# 返回的是一个numpyarray，要转化为DataFrame
housing_tr = pd.DataFrame(X, columns=housing_num.columns)

# 将文本属性值添加
housing_tr['ocean_proximity'] = housing["ocean_proximity"]

housing_tr[:2]
# out：
'''
    longitude	latitude	housing_median_age	total_rooms	total_bedrooms	population	households	median_income
0	-121.89 	37.29     	38.0  	              1568.0	    351.0	     710.0	     339.0	    2.7042
1	-121.93	    37.05   	14.0	              679.0	        108.0	     306.0   	113.0	   6.4214
'''
```
```



