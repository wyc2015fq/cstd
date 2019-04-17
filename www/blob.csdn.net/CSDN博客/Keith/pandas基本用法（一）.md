# pandas基本用法（一） - Keith - CSDN博客





2017年03月27日 23:58:37[ke1th](https://me.csdn.net/u012436149)阅读数：3880







- 读取csv文件

```python
import pandas
food_info = pandas.read_csv(file_name)# 返回一个DataFrame对象
n_rows = food_info.head(n) #获取前n行数据，返回的依旧是个DataFrame
column_names = food_info.columns   #获取所有的列名
dimensions = food_info.shape #获取数据的shape
```
- Index

默认情况下，使用`pandas.read_csv()`读取`csv`文件的时候，会默认将数据的第一行当做列标签，还会为每一行添加一个行标签。我们可以使用这些标签来访问DataFrame中的数据。 
![这里写图片描述](https://img-blog.csdn.net/20170327235659859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzNjE0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
DataFrame

- Series对象

`pandas`的核心组件，构成`DataFrame`的基本单元。 
![这里写图片描述](https://img-blog.csdn.net/20170327235718718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzNjE0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Series

- 如何选择一行数据

```python
data = food_info.loc[0] #使用loc[n]获取第n行数据，如果只是获取一行数据的话，返回Series
#如何选择多行呢，和numpy的语法是一样的
datas = food_info.loc[1:2] #返回DataFrame,返回行标签为1,2的，这个和numpy不一样，而且loc[]也不支持-n

data = food_info.loc[i][j] # i-th row, j-th column
```
- 使用`DataFrame.dtypes`获取每列的数据类型
- 使用`DataFrame[indices]`获取列数据。注意，要是获取多个列的话`selenium_thiamin = food_info[["Selenium_(mcg)",'Thiamin_(mg)']]`，不要少了个中括号。

## 关于len()

```python
len(data_frame) # 是data_frame的行数
len(data_frame.loc[0]) #是data_frame的列数
```

## 关于set()

```python
set(data_frame) # 返回column name
set(data_frame["column1"]) # 返回第一列中不重复的值
set(data_frame.loc[0]) #返回第一行中不重复的值
```

## 如何遍历 DataFrame 中的数据

```python
# 使用 for 是不可行的！！！！！！！！！！！！！！！！！！！！！
for data in data_frame:
    pass

# 使用 len 和 loc
for i in range(len(data_frame)):
    cur_data = data_frame.loc[i]

# 使用 .iteriterms()
for i, series in df.iteritems():
    print(i, ":", type(series))
```

## 如何保存成 CSV 文件

使用 `data_frame.to_csv()` 。为了使用这个方法，首先要创建一个 `DataFrame` 实例。

```python
import pandas as pd
# from_dict, 使用 dict 把每列的数据准备好
predictions = {}
predictions["id"] = ["001", "002"]
predictions["value"] = ["hello boy", "hello world"]

dataFrame = pd.DataFrame.from_dict(predictions)
print(dataFrame)

# from_record, 准备好每一行的数据
predictions2 = []
predictions2.append(["001", 0, 0])
predictions2.append(["002", 0, 0])
dataFrame2 = pd.DataFrame.from_records(predictions2, columns=["id", "is_man", "is_good"])
print(dataFrame2)
```

**判断获取的值是否为 nan**

```python
res = series['val']
pd.isna(res) # nan 返回 True 否则返回 False
```

## 参考资料

[https://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.html](https://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.html)





