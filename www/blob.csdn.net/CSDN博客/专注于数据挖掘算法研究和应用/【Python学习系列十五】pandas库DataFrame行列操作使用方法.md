# 【Python学习系列十五】pandas库DataFrame行列操作使用方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月13日 16:55:04[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4968
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









参考：http://pandas.pydata.org/pandas-docs/stable/api.html#dataframe



```python
data['w']  #选择表格中的'w'列，使用类字典属性,返回的是Series类型

data.w    #选择表格中的'w'列，使用点属性,返回的是Series类型

data[['w']]  #选择表格中的'w'列，返回的是DataFrame类型

data[['w','z']]  #选择表格中的'w'、'z'列

data[0:2]  #返回第1行到第2行的所有行，前闭后开，包括前不包括后

data[1:2]  #返回第2行，从0计，返回的是单行，通过有前后值的索引形式，
       #如果采用data[1]则报错

data.ix[1:2] #返回第2行的第三种方法，返回的是DataFrame，跟data[1:2]同

data['a':'b']  #利用index值进行切片，返回的是**前闭后闭**的DataFrame, 
        #即末端是包含的  
data.irow(0)   #取data的第一行
data.icol(0)   #取data的第一列

data.head()  #返回data的前几行数据，默认为前五行，需要前十行则data.head(10)
data.tail()  #返回data的后几行数据，默认为后五行，需要后十行则data.tail(10)

ser.iget_value(0)  #选取ser序列中的第一个
ser.iget_value(-1) #选取ser序列中的最后一个，这种轴索引包含索引器的series不能采用ser[-1]去获取最后一个，这会引起歧义。

data.iloc[-1]   #选取DataFrame最后一行，返回的是Series
data.iloc[-1:]   #选取DataFrame最后一行，返回的是DataFrame

data.loc['a',['w','x']]   #返回‘a’行'w'、'x'列，这种用于选取行索引列索引已知

data.iat[1,1]   #选取第二行第二列，用于已知行、列位置的选取。
```




下面代码示例实现从csv中读取然后提起个别列重新输出csv

```python
trainds=pd.read_csv(r"D:\train_normal.csv", header=0, encoding='utf-8')
    sampleds = pd.DataFrame(trainds[['age','sex','account_age','is_use']]) 
    sampleds.to_csv('D:\sample.csv',index=False)
```





