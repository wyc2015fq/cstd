
# pandas DataFrame 索引（iloc 与 loc 的区别） - Zhang's Wikipedia - CSDN博客


2018年04月27日 22:03:57[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：934


[Pandas——ix vs loc vs iloc区别](https://blog.csdn.net/xw_classmate/article/details/51333646)
## 0. DataFrame
DataFrame 的构造主要依赖如下三个参数：
data：表格数据；
index：行索引；
columns：列名；
index 对行进行索引，columns 对列进行索引；
>> import pandas as pd  
>> data =[[1,2,3],[4,5,6]]>> index = [0,1]  
>> columns=['a','b','c']  
>> df = pd.DataFrame(data=data, index=index, columns=columns)  
>> df
   a  b  c01231456
1.  loc
loc 通过行索引（index）获取行：
>>df.loc[1]a4b5c6
如果 DataFrame 在构造时，索引不是整数而是字符：
index= ['d','e']  
columns=['a','b','c'] 
df = pd.DataFrame(data=data,index=index, columns=columns)
>> df.loc['d']
索引某列：
>> df.loc['d', ['b','c']]  
>> df.loc[:, ['c']]
2. iloc
同 loc 不同，iloc 则是通过行号对行进行索引，通过行索引则会报错：
df.iloc[0:] ：索引全部行；
df.iloc[:, [1]] ：索引第一列
3. df.values ⇒ ndarray
>> df.values
array([[1, 2, 3],
       [4, 5, 6]], dtype=int64)

