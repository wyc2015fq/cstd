# Numpy——练习题 - wsp_1138886114的博客 - CSDN博客





2018年05月28日 18:47:44[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：231
所属专栏：[科学计算库与可视化](https://blog.csdn.net/column/details/23452.html)









### 一、加载csv数据

```python
import numpy as np
close,amount=np.loadtxt("./data.csv",delimiter=",",usecols=(6,7),unpack=True) #导入收盘价、成交量两列
    # delimiter=: 分隔符
    # usecols=(): 将所有获取数据的列索引放入元组中
    # unpack=True: 将获取的所有元素解耦，可赋值给不同的变量，default=False
```

### 二、统计分析

> 
#### 2.1初步函数统计

```
price_weight = np.average(close,weights=amount)  #成交量加权平均价
price_mean = np.mean(close)                      #收盘价的算术平均价

price_max = np.max(amount)                       #最大成交量
price_min = np.min(amount)                       #最小成交量
price_ptp = np.ptp(amount)                       #成交量的极差

price_mediam = np.median(close)                  #收盘价的中位数

sorted_price=np.msort(close)                     #将收盘价排序，默认升序
var_price=np.var(close)                          #计算收盘价的方差
```

#### 2.2 计算股票收益率、年波动率及月波动率

```
returns=np.diff(close) / close[:-1]    #计算收盘价的简单收益率
                                       # np.diff() 后一天与前一天的差值（最后一天的不计算）
                                       # arr[:-1]  除去最后一天的数组

returns_index=np.where(returns > 0)    #获取收益率为正值的元素的索引组成的元组
result=np.take(close,returns_index)    #根据索引值，获取相应的数组元素

std_price=np.std(close)                #计算收盘价的标准差
logreturns=np.diff(np.log(close))      #对数收益率

计算年波动率: 
 #年波动率等于对数收益率的标准差除以其均值，再除以交易日倒数的平方根，通常交易日取252天。
annual_volatility = (np.std(logreturns) / np.mean(logreturns)) / np.sqrt(1/252) 

月波动率
 #月波动率等于对数收益率的标准差除以其均值，再除以交易月倒数的平方根，交易月为12月。
mon_volatility=annual_volatility * np.sqrt(1/12)
```

#### 2.3 日期处理函数

```
import numpyas np
from datetime import datetime
datestr="2018-05-28"
newDate=datetime.strptime(datestr,"%Y-%m-%d")
print(newDate)      #2018-05-28 00:00:00
weekday=newDate.date().weekday()
print("{}是星期{}".format(datestr,weekday+1))
```

#### 2.3 综合统计

```
需求：文件中的数据为给定时间范围内某股票的数据，分别对应的平均收盘价及哪天的平均收盘价最高，
哪天的平均收盘价最低。
```

```python
importnumpy as np
 from datetime import datetime

 def datestr2num(s):                     #将日期格式字符串转换并得到星期
     return datetime.strptime(s.decode("utf8"), "%d-%m-%Y").date().weekday()     
 date,close=np.loadtxt("./data.csv",delimiter=",",usecols=(1,6),converters={1:datestr2num},unpack=True)  #导入data数据，分别获取交易日期和收盘价数据
 average=[]

 for i in range(5):
     mask= date==i                       #mask索引，获取date为i的mask索引
     close_w=close.compress(mask)        #根据mask索引返回星期为i+1的所有收盘价
     average.append(np.mean(close_w))    #将星期数为i+1的平均收盘价放入average列表中
 maxIndex=np.argmax(average)   #获取数组中最大值的索引
 minIndex=np.argmin(average)   #获取数组中最小值的索引
 print("星期{}的收盘价的平均收盘价最高！".format(maxIndex+1))
 print("星期{}的收盘价的平均收盘价最低！".format(minIndex+1))
```


### 课后作业

> 
###### 第一题

```python
import numpy as np
print('------------数组元素倒序--------------')
arr_1 = np.arange(8)
arr_2 = arr_1[::-1]
print(arr_1)
print(arr_2)
```

###### 第二题

```
import numpy as np

print(---------------方法1---------------)
arr_1=np.ones((10,10))
arr_1[1:9,1:9]=0        #【第二行到第八行：第二列第八列】
print(d5)
>>>
[[1. 1. 1. 1. 1. 1. 1. 1. 1. 1.]
 [1. 0. 0. 0. 0. 0. 0. 0. 0. 1.]
 [1. 0. 0. 0. 0. 0. 0. 0. 0. 1.]
 [1. 0. 0. 0. 0. 0. 0. 0. 0. 1.]
 [1. 0. 0. 0. 0. 0. 0. 0. 0. 1.]
 [1. 0. 0. 0. 0. 0. 0. 0. 0. 1.]
 [1. 0. 0. 0. 0. 0. 0. 0. 0. 1.]
 [1. 0. 0. 0. 0. 0. 0. 0. 0. 1.]
 [1. 0. 0. 0. 0. 0. 0. 0. 0. 1.]
 [1. 1. 1. 1. 1. 1. 1. 1. 1. 1.]]

print('-----------遍历替换----方法2--------------')
arr_7 = np.ones((10,10))
for i in range(len(arr_7)):
    for j in range(len(arr_7)):
        if i >0 and i<=8:
            if j >0 and j<=8:
                arr_7[i][j]=0
print(arr_7)

print('------------矩阵拼接--方法3--------------')
arr_8 = np.ones((1,8))                
arr_ce = np.ones((1,10))
arr_9 = np.full((8,8),0)               #创建 八行八列 全为零矩阵
arr_10 = np.concatenate((arr_8,arr_9),axis=0)
arr_11 = np.concatenate((arr_10,arr_8),axis=0)
result_1 = np.insert(arr_11,0,values=arr_ce,axis=1)
result_2 = np.insert(result_1,9,values=arr_ce,axis=1)
print(result_2)
```




