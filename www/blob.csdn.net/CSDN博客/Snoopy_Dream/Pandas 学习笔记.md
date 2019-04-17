# Pandas 学习笔记 - Snoopy_Dream - CSDN博客





2018年02月04日 21:19:58[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：109









## 转自：[https://www.jianshu.com/p/6b7516e6fafb](https://www.jianshu.com/p/6b7516e6fafb)

## 1. 安装

如果做数据分析用途建议使用Anaconda，自带pandas numy 以及很多库，还有集成开发环境Spyder（自带的变量查看器很好用） WIN LINUX MAC均支持。

Anaconda 安装地址：[https://www.continuum.io/downloads](https://link.jianshu.com?t=https://www.continuum.io/downloads)

安装之后可以在terminal 输入 conda 开头的命令（类似pip），例如list 查看已经安装的包，以及进行常用的install update等动作。

## 2. Import

绝大部分时候pandas都被使用者import为pd，根据作者的描述其实pandas是panel data的缩写（而不是熊猫）

> 
import pandas as pd


## 3. DataFrame

DataFrame是Pandas用来处理数据最常见的格式，一张二维的表，有行，列和值。类似于一个数据库里的table 或者excel中的worksheet。如果有一个DataFrame叫df, df.columns可以得到所有的列标签，同理df.index可以得到所有的行标签。

## 4. 读取数据

#### 4.1 从excel中读取数据

> 
raw = pd.read_excel('%s%s.xlsx' %path %filename, sheetname='Data',  skiprows= 1)


数据会被读取到一个叫raw的DataFrame中，sheetname可以指定读某个工作表，skiprow可以跳过初始N行的数据。

#### 4.2 从csv中读取数据

> 
raw = pd.read_csv('%s%s.csv' %path %filename)


## 5. 增删改查

#### 5.1 增删列

新增列,位置在最后一列

> 
raw['新列名'] = 'string'


在中间增列，使用 df.insert()

> 
df.insert(位置,'列名',值)


例如，在raw df第二列（index不算一列）插入一列，名为city，值为source_data的 [city]列

> 
raw.insert(1,'column_name',source_data['data1'])


删除列

> 
del raw['列名']


#### 5.2 改列名

5.2.1 一次性改变所有的列名

> 
cols = ['name_1', 'name_2', 'name_3']

raw= raw[cols]


5.2.2 修改某个列名

使用df.rename()， 注意如果df中有多个old_name列的话都会被一并重命名为new_name

> 
df=df.rename(columns = {'old_name':'new_name'})


#### 5.3 改index

把某列设为index,原index会被删除

> 
raw = raw.set_index('column_name')


reset_index()，新index是以0开始的递增整数列，原index会变成一个新的列。

> 
raw = raw.reset_index()


如果不需要原来的index中的值可以加drop = True：

> 
raw = raw.reset_index(drop=True)


#### 5.4 编辑值（计算值）

5.4.1 四则运算

> 
raw['data1'] = raw['data1'] *100

raw['data2'] = (raw['data1']+raw['data3'])/raw['data4']

raw['total'] = raw.sum(axis=1)


#### 5.5 查列

5.5.1 筛选某列包含某值(raw df中 GEO CODE为CN的所有数据)

> 
raw = raw[raw['GEO_CODE']=='CN']


5.5.2 多条件筛选

> 
raw = raw[(raw['GEO_CODE']=='CN')&(raw['METRIC']=='Conversion Rate')]


5.5.3 筛选多个列

> 
required_key = ['User_ID','SEO visits','SEA visits','Conversion Rate']

raw = raw[raw['METRIC_KEY'].isin(required_key)]


#### 5.6 去重

5.6.1 去重使用drop_duplicates()，主要有2个参数：

subset 需要去重的值

keep，在遇到重复值时保留第一个(keep = 'first')or最后一(keep = 'last')

> 
df = df.drop_duplicates(subset = 'column_name', keep = 'last')


5.6.2 因为去重时，保留的值很简单是取第一个或最后一个，所以需要搭配sort_values()来保证留下的值是你想要的。sort_values()默认是升序ascending，由小到大。

> 
df = df.sort_values(by='column_name')

df = df.drop_duplicates(subset = 'column_name', keep = 'last')


## 6 Excel功能相关

#### 6.1 Excel的数据透视表

> 
pd.pivot_table()


主要有3个参数，index, columns,value, 以及aggfunc
- index相当于行标签

- columns相当于列标签

- value相当于用来计算值，配合aggfunc来计算count/mean/average

*注意value不能使用index 和columns已经使用过的值，这点和excel不同。*

> 
pivot= pd.pivot_table(raw, values = 'Response ID', index= ['Country'], columns=['NPS category'],  aggfunc=np.size)


aggfunc目前用过的有计数np.size 汇总np.sum 平均np.average np.mean 中位数np.median

#### 6.2 DataFrame的融合 (vlookup or hookup)

因为excel的公式是在某个单元格中，而DataFrame一般是一次性处理行或列的数据，给某行／列根据其他行／列的数据引用赋值就相当于表格的融合。

主要用到2个方法：
- pd.merge()

- pd.concat()


详情请看下节

#### 6.3 pd.merge()

pd.merge()非常类似数据库中join的操作，参数很丰富：

> 
merged_df = pd.merge(left, right, how='inner', on=None, left_on=None, right_on=None, left_index=False, right_index=False, sort=True, suffixes=('_x', '_y'), copy=True, indicator=False)


merge可以提供关系型数据库中常用的几种合并方式，空值会用NaN填充：

下面是几个常用参数的详解：

**参数on**

> 
pd.merge(df1, df2, on = 'xxx') #on的参数用来确定2个表共同的column。


*on在这里就相当于vlookup中lookup value的定位*

**参数merge**

> 
pd.merge(df1, df2, how= 'xxx') #how的参数用来确定 merge method 。


Merge method和SQL join的对应关系如下：
|Merge method|SQL Join Name|Description|
|----|----|----|
|left|LEFT OUTER JOIN|只使用左表的键(key)|
|right|RIGHT OUTER JOIN|只使用右表的键|
|outer|FULL OUTER JOIN|使用两表的并集的键|
|iner|INNER JOIN|使用两表的交集的键|
*如果使用pd.merge实现vlookup时，正好二者的index就是共有值，只要pd.merge(main_data,to_lookup_data,on ='left')就OK了*
参数left_on right_on

to bu input

参数left_index right_index

to bu input

#### 6.4 pd.concat()

如果两个DataFrame column相同，二者上下拼接在一起 (增加数据行)

> 
pd.concat([df1,df2])


如果两个DataFrame index相同，二者左右拼接在一起 (增加数据列)

> 
pd.concat([df1,df2], axis = 1)


如果有多个DataFrame, column相同的情况下:

> 
dfs = [df1,df2,df3,df4]

result = pd.concat(dfs)


关于pd.merge()和pd.concat() 更多细节请参考官网：

[http://pandas.pydata.org/pandas-docs/stable/merging.html](https://link.jianshu.com?t=http://pandas.pydata.org/pandas-docs/stable/merging.html)

## 7. 数据输出

假设现在有一个名为raw的DataFrame需要输出到C盘根目录

#### 7.1 输出到csv

DataFrame自带to_csv()功能，注意如果有中文建议加encoding参数，如果不需要index可加 index= False 参数。

> 
raw.to_csv('C:\File_name.csv', encoding = 'utf-8', index = False)


#### 7.2 输出到Excel

使用pandas自带的 Excel Writer生成2010格式的excel,

> 
from pandas import ExcelWriter

path = 'C:\'

writer = ExcelWriter('%sFile_name.xlsx' %path) #指定Excel文件名

raw.to_excel(writer, sheet_name = 'worksheet_name') #指定工作表名称

writer.save()


#### 7.3 输出到数据库

如果要存数据库呢? RDBS和NOSQL
- Mysql

- MongoDB


To be input..

## 8.使用datetime进行时间相关的操作

在python中用datetime也可以实现同excel中常用的日期函数一样的功能

#### 8.1 创建现在的时间点为对象

> 
import datetime

now = datetime.datetime.now()

today = datetime.datetime.today()


#### 8.2 时间的位移

> 
start_date = dt.date(today.year-2,today.month-1,today.day)

end_date = dt.date(today.year,today.month-3,today.day+1)


如果月份/日期 超过限制会报错

所以可能需要写一个循环去输出这些日期

```bash
date_list = []
while start_date < end_date:
    if start_date.month < 12:
        date_list.append(start_date.strftime('%Y-%m'))
        start_date = datetime.date(start_date.year,start_date.month +1,start_date.day)
    else:
        date_list.append(start_date.strftime('%Y-%m'))        
        start_date = datetime.date(start_date.year+1,start_date.month-11,start_date.day)
```

#### 8.3 调整格式

如上面所示，使用strftime()可以调整时间的格式，可以调整的选项非常多，参考：
[http://www.runoob.com/python/att-time-strftime.html](https://link.jianshu.com?t=http://www.runoob.com/python/att-time-strftime.html)
- %y 两位数的年份表示（00-99）
- %Y 四位数的年份表示（000-9999）
- %m 月份（01-12）
- %d 月内中的一天（0-31）
- %H 24小时制小时数（0-23）
- %I 12小时制小时数（01-12）
- %M 分钟数（00=59）
- %S 秒（00-59）
- %a 本地简化星期名称
- %A 本地完整星期名称
- %b 本地简化的月份名称
- %B 本地完整的月份名称
- %c 本地相应的日期表示和时间表示
- %j 年内的一天（001-366）
- %p 本地A.M.或P.M.的等价符
- %U 一年中的星期数（00-53）星期天为星期的开始
- %w 星期（0-6），星期天为星期的开始
- %W 一年中的星期数（00-53）星期一为星期的开始
- %x 本地相应的日期表示
- %X 本地相应的时间表示
- %Z 当前时区的名称
- %% %号本身

#### 8.4 周数的计算

#### 8.5 工作日的计算

Excel中有个很方便的函数叫networkdays，给出起始日期，结束日期和holiday可以计算两个日期间的工作天数。而pandas或者datetime对这个需求支持的不好，所以找到了这个module: **business_calendar**
[https://pypi.python.org/pypi/business_calendar/](https://link.jianshu.com?t=https://pypi.python.org/pypi/business_calendar/)

8.5.1 计算日期之间的工作日数量
例如，求16年2月1日~29日的工作日有几天，已知条件:
- 周一到周五都上班
- 2月8日到12日为休假

```bash
date1 = datetime.datetime(2016,1,31)#注意如果写2月1日，当天是不包含在内的，所以写1月31日
date2 = datetime.datetime(2016,2,29) 
cal = Calendar(workdays =[MO, TU, WE, TH, FR], holidays=['2016-02-08','2016-02-09','2016-02-10','2016-02-11','2016-02-12'])
bsday = cal.busdaycount(date1, date2)
print (bsday)
```

8.5.2 计算从某天开始，N个工作日之后的日期

**残念的是，这个module并不支持所谓"调休"的概念。sigh**



作者：steveysy
链接：https://www.jianshu.com/p/6b7516e6fafb
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
            


