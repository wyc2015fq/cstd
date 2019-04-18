# matplotlib + pandas——数据可视化 - wsp_1138886114的博客 - CSDN博客





2018年05月31日 11:13:26[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：749
所属专栏：[科学计算库与可视化](https://blog.csdn.net/column/details/23452.html)










**利用pandas进行数据分析 + matplotlib进行可视化展示**




- - - - - [1 第一个绘图](#1-第一个绘图)
- [2 pandas利用DataFrame的plot方法绘图](#2-pandas利用dataframe的plot方法绘图)
- [3 pandas利用DataFrame绘图柱状图/堆积图（加载csv数据）](#3-pandas利用dataframe绘图柱状图堆积图加载csv数据)
- [4 pandas绘图频率直方图(加载csv文件)](#4-pandas绘图频率直方图加载csv文件)
- [5 pandas绘图箱型图](#5-pandas绘图箱型图)
- [6 利用pandas 处理后的数据绘图](#6-利用pandas-处理后的数据绘图)- [6.1◆条形图](#61条形图)
- [6.2◆折线图](#62折线图)

- [7 利用pandas 绘制柱状堆积图](#7-利用pandas-绘制柱状堆积图)
- [8 利用pandas 绘制点状图](#8-利用pandas-绘制点状图)- [8.1◆绘制 散点图](#81绘制-散点图)
- [8.2◆绘制 气泡图](#82绘制-气泡图)

- [9 利用pandas 绘制饼图](#9-利用pandas-绘制饼图)








##### 1 第一个绘图

> 
```
（原本在matplotlib中需要几段代码，在pandas中只需要一行代码）
```

```
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

 # Series 默认索引为x轴
s = pd.Series(           
            np.random.randn(10).cumsum() #产生一个10行4列的随机数
            index = np.arange(0,100,10)  #0-100之间，以10作为分割
            )
s.plot()
plt.show()
```


##### 2 pandas利用DataFrame的plot方法绘图

> 
```
df = pd.DataFrame(np.random.randn(10,4),       #生成10行4列标准正态分布的数据
                  columns = list('ABCD'),
                  index = np.range(0,100,10)   #指定索引为 x 轴
                )
pd.plot()
plt.show()
```


![这里写图片描述](https://img-blog.csdn.net/2018062713424044?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


```
修改 pd.plot(subplots = True)  #将4个数据分配到不同的画布
```


![这里写图片描述](https://img-blog.csdn.net/2018062714001969?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 3 pandas利用DataFrame绘图柱状图/堆积图（加载csv数据）

> 
```
案例1
fig = plt.figure() 

fig,axes = plt.subplots(2,1)
data = pd.Series(np.random.randn(16),
index = list('abcdefghijklmnop'))
data.plot(kind = 'bar',ax = axes[0],color = 'k',alpha = 0.7)
data.plot(kind = 'barh',ax = axes[1],color = 'k',alpha = 0.7)
plt.show()
```


![这里写图片描述](https://img-blog.csdn.net/20180627140147278?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


```
案例2
pd = pd.read_csv('flights.csv')     #导入数据（注意观察数据格式）
number = pd["passengers"].groupby([pd["year"],pd["month"]]).sum().unstack()
number.plot(kind="bar",figsize=(23,8))
```

```
flights.csv 文件（由于网页不能导入文件，这里只是展示一部分数据）
```
|$~$|year|month|passengers|
|----|----|----|----|
|0|1949|January|112|
|1|1949|February|118|
|2|1949|March|132|
|3|1949|April|129|
|4|1949|May|121|
|5|1949|June|135|
|6|1949|July|148|
|7|1949|August|148|
|8|1949|September|136|
|…|…|…|…|
|138|1960|July|622|
|139|1960|August|606|
|140|1960|September|508|
|141|1960|October|461|
|142|1960|November|390|
|143|1960|December|432|

![这里写图片描述](https://img-blog.csdn.net/20180627105421180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 4 pandas绘图频率直方图(加载csv文件)

> 
```bash
df = pd.read_excel('./pandas-matplotlib.xlsx','Sheet1')  #导入数据文件

fig = plt.figure()              # 创建画板
ax = fig.add_subplot(111)       # 创建绘图区域
fig.set_size_inches(10,5)       # 设置已创建画布大小
ax.hist(df['Age'],bins = 7)     # 绘制频率直方图
plt.title('年龄平均分布图')
plt.xlabel('Age')
plt.ylabel('Employee')
plt.show()
```

```
pandas-matplotlib.xlsx中 Sheet1 数据如下：
```
|**EMPID**|**Gender**|**Age**|**Sales**|**BMI**|**Income**|
|----|----|----|----|----|----|
|E001|M|34|123|Normal|350|
|E002|F|40|114|Overweight|450|
|E003|F|37|135|Obesity|169|
|E004|M|30|139|Underweight|189|
|E005|F|44|117|Underweight|183|
|E006|M|36|121|Normal|80|
|E007|M|32|133|Obesity|166|
|E008|F|26|140|Normal|120|
|E009|M|32|133|Normal|75|
|E010|M|36|133|Underweight|40|


##### 5 pandas绘图箱型图    

> 
```bash
df = pd.read_excel('./pandas-matplotlib.xlsx','Sheet1')  #导入数据文件
fig = plt.figure()
ax = fig.add_subplot(111)
ax.boxplot(df['Age'])           #绘制箱型图
plt.show()
```


##### 6 利用pandas 处理后的数据绘图

> 
###### 6.1◆条形图

```
var = df.groupby('Gender').Sales.sum()
fig = plt.figure()
ax1 = fig.add_subplot(211)
ax1.set_xlabel('Gender')
ax1.set_ylabel('Sum of Sales')
ax1.set_title('Gender wise Sum of Sales')
var.plot(kind = 'bar')
```

###### 6.2◆折线图

```
line = df.groupby('BMI').Sales.sum()
 #print(line)
ax1 = fig.add_subplot(212)
ax1.set_xlabel('BMI')
ax1.set_ylabel('Sum of Sales')
ax1.set_title('BMI wise Sum of Sales')
line.plot(kind = 'line')

plt.tight_layout()
plt.show()
```


##### 7 利用pandas 绘制柱状堆积图

> 
```php
df = pd.read_excel('./excel/pandas-matplotlib.xlsx','Sheet1')  #导入数据文件

fig = plt.figure()
var = df.groupby(['BMI', 'Gender']).Sales.sum()
var.unstack().plot(kind='bar', stacked=True, color=['red', 'blue'])
plt.show()
```


![这里写图片描述](https://img-blog.csdn.net/2018062714211024?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



##### 8 利用pandas 绘制点状图

> 
###### 8.1◆绘制 散点图

```
fig = plt.figure()
ax1 = fig.add_subplot(111)
ax.scatter(df['Age'],df[Sales])
plt.show()
```

###### 8.2◆绘制 气泡图

```bash
df = pd.read_excel('./excel/pandas-matplotlib.xlsx','Sheet1')  #导入数据文件

fig = plt.figure()
ax = fig.add_subplot(111)
ax.scatter(df['Age'],
           df['Sales'],
           s=df['Income'])  #在散点图上添加第三个参数：表明收入（气泡的大小） 

plt.show()
```


![这里写图片描述](https://img-blog.csdn.net/20180627142623432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



##### 9 利用pandas 绘制饼图

> 
```php
var = df.groupby(['Gender']).sum()
x_list = var['Sales']
label_list = var.index
plt.axis('equal')
plt.pie(x_list, labels=label_list, 
        autopct='%1.1f%%',   # 保留两位小时
        shadow = True,       # 设置阴影
        explode=[0,0.1])     # 0 ：扇形不分离，0.1：分离0.1单位
plt.title('expense')
plt.show()
```


![这里写图片描述](https://img-blog.csdn.net/20180627155215418?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







