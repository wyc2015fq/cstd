# Pandas——基础（创建与查看） - wsp_1138886114的博客 - CSDN博客





2018年05月26日 09:34:50[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：682
所属专栏：[科学计算库与可视化](https://blog.csdn.net/column/details/23452.html)









### 一、Pandas数据结构

> 
##### 1.1 pandas学习资料

```
http://pandas.pydata.org/pandas-docs/stable/10min.html#operation
http://pandas.pydata.org/pandas-docs/stable/api.html    (API资料查询)
```

**安装：推荐Anaconda** ，关于Python数据分析和科学计算的分发包，已经封装了pandas conda list 

  执行命令：`pip install pandas`
**导入Pandas模块，并检查pandas 版本**（默认包别名：pd） 
`import pandas as pd`
`pd.__version__`
##### 1.2 Pandas数据结构

```
主要两个数据结构：Series 、DataFrame 是基础数据处理基础包
series :
    是数据类型（integers,string,floating,objects,etc）的一维数组     
    由索引和数据组成：pandas.Series(data = None,index = None,dtype = None)
通过 ndarray 创建    
    若没有指定索引，自动生成0到N-1整型索引/Index参数长度与data一样
```

```
1. 通过 ndarray 构建 Series
pd.Series(np.random.randn(4)),index = (['aa','ab','c','d'])  #index参数长度与data一样
```

###### 通过 list / dict 构建
- 如果没有指定索引，  index默认为字典的key值
- 如果index被重新赋值，新的index只是调整value值顺序
- index与key值相交，那么对应的数据就是字典的value
- 若index值不在字典key值中，那么value将会被初始化为NaN
- 若字典的key值不在index中，则value被删除

```
2. 通过 dict 构建 Series
data1 = {'a':0.,'b':1.,'c':2.}      #index不传入值
pd.Series(data1)                    #pandas自动添加dict的key作为index
pd.Series(data1,index={'z1','c','a','z2'})    
>>>
    c     2.0
    z1    NaN
    a     0.0
    z2    NaN
    dtype: float64
  # index传入值，c,a 对应的value值调整顺序;
  # z1,z2对应的值赋值为NaN;b对应的value被删除
```

##### 1.3 DataFrame 二维标记数据结构

```
可看成电子表格或SQL关系库中的表格，支持多种数据类型创建 如 dict,ndarray,series,dataframe。
 同列数据类型一致/不同列数据类型可以不同。由数据和索引组成
```

`pandas.DataFrame(data = None,index = None,columns = None,dtype = None)`

  data 最常见二维数组 ndarray，由数组，列表组成的字典

###### 通过 ndarray 创建 DataFrame

若没有指定索引，自动生成0到N-1 的整数型`index`和`columns`  。 index和columns参数长度必须与data参数一样

```
通过 ndarray 构建 DataFrame
pd.DataFrame(np.random.randn(5,4))    #五行四列不包括列名与行索引
 #指定列名与行索引
pd.DataFrame(np.random.randn(5,4),index = list('abcde'),columns = list('colu')) 
>>>
            c           o           l          u
    a   -0.353548   -0.784018   1.345773    1.217834
    b   0.763407    1.516695    0.405163    0.302634
    c   1.647324    -0.276042   0.162789    -0.346908
    d   -0.429555   0.117893    -0.572518   -0.489215
    e   0.016941    2.150515    -0.151753   -0.522222
```

###### 通过 字典 创建 DataFrame

```
pop = {"省份":["上海","浙江","江苏"],
      "年份":[2001,2009,2015],
      "人口":[235555,4563322,2112323]}
df = DataFrame(pop,columns=["省份","年份","人口"])   #指定列名排序
>>>
        省份  年份      人口
    0   上海  2001    235555
    1   浙江  2009    4563322
    2   江苏  2015    2112323
```

###### 通过 字典嵌套 创建 DataFrame

```
provice = {"省份":{"pro1":"上海","pro2":"浙江","pro3":"江苏"},
           "年份":{"pro1":"2001","pro2":"2009","pro3":"2009"},
           "人口":{"pro1":"99822123","pro2":"34434343","pro3":"9085556"},
          }
data = pd.DataFrame(provice)
>>>
          人口       年份    省份
pro1    99822123    2001    上海
pro2    34434343    2009    浙江
pro3    9085556     2009    江苏
```

### 二、数据查看

> 
##### 2.1 数据抽样：大体感觉数据

```
DataFrame.sample(n=None frac=None,replace=None,weights=None,random_state = None,axis=None)
frac：        #取样比例；    
weights：     #取样权重
DataFrame.head(n=5)
DataFrame.tail(n=5)
train.sample(n=6)          #抽样6条记录
train.head(n=6)            #开头6条
train.tail(n=6)            #结尾6条
train.values
```

##### 2.2 对象属性查看

```
train.shape                #维度，行列数
train.index                #行索引
train.columns              #列名
train.dtype                #类型
train info()               #整体属性查看
```

###### Pandas主要几种dtype

```
object          -字 符 串
int             -整    型
float           -浮 点 型
datetime        -时间类型
bool            -布尔类型
```

##### 2.3 数据特征查看

```
某列空值计数
某列有那些不同值
    unique 一维对象函数
不同值的计数
    value_counts 一维对象函数
        train['age'].isnull().sum()   #空值计数
        train['age'].unique()         #唯一值序列
        train['age'].value_counts     #不同值计数
是否有重复样本
    duplicated(subset = None,keep = 'first')
    subset      是否只需要检查某几列
    keep：First 支持从前到后，将后出现的相同行判断为重复值
          Last  ...从后至前
    返回值：对每一行返回一个bool值
    train.duplicated().sum()           #是否有重复样本
        df1 = train.head()             #只取整数的前五行做演示
        df1['Swx']                     #某列值
        df1['Swx'].duplicated()        #是否有重复值
        df1['Swx'].duplicated().sum()  #重复值数量
```

##### 2.4 合计信息的查看

```
单列统计信息
mean(axis = None,skipna = None,level = None)
axis:行列标记
skipna:是否跳过NaN值
level：对于多层索引有效
示例：
    train['Age'].count()
    train['Age'].max()
    train['Age'].argmax()
    train['Age'].idxmax()
    train['Age'].quantile(0.5)
    train['Age'].std()
    train['Age'].describe()     #显示详细信息
整体合计信息
describe(percentiles=None,include=None,exclude=None)
    percentiles     #  显示百分比
    include         #  显示数据类型
    exclude         #  不需要显示什么类型的数据
    numpy.number 所有数值型
    numpy.object 类别型（字符型）
```


### 三、数据选择 [切片]（特定行列抽取）

> 
切片选择：选出对应的行和列 
`train.loc[['index2','index4','index5'],['Sex','Ticket']]`

###### 3.1. 行列表示方法

```
1.1名称    train.columns(列名)
1.2位置    train.index   (索引)
index([u'index1',……],dtype='object',name=u'mingzi')
```

###### 3.2 切片表示方法(得到N*M方块)

```bash
● 单值切片

ser_obj['index2']                     #名称
ser_obj[1]                            #位置
```

```
● 连续切片

ser_obj['index2':'index5']            #名称
ser_obj[1:4]                          #位置,不包含最后一个数字
ser_obj[-4:-1]                        #从后往前数
ser_obj[:]                            #全部数据
```

```
● 列表切片

ser_obj[['index1','index1','index5']]  #名称
ser_obj[0,4,2]                         #位置(第一层第四行第二列)
```

```bash
● 布尔切片

ser_obj[ser_obj=='male']               #抽取符合male的值
ser_obj[ser_obj.isin(['female'])]
```

###### 3.3 行列抽取方法

```bash
▶ 行模式
操作符 [ ]：不管是索引位置 / 索引名称 都是用操作符
只能用连续、布尔切片方式。（单值、列表切片方式会报错）

train['index1':'index3']       #名称
train[0:2]                     #位置
```

```
▶ 列模式

操作符 [[ ]]：不管是索引位置or索引名称 都是用操作符
只能单值、列表切片方式。（用连续、布尔切片方式会报错）

train[['Age','sex']]           #名称
train[[5,4]]                   #位置
```

```bash
▶ 行列模式
loc [ ]   索引名称函数（行/列 名）
iloc [ ]  索引位置函数（行/列 位置）
ix [ ]    混合函数，先用名称尝试切片，匹配不上再用位置

train.loc['index1':'index3',['Age','Name']] #可以结合四种切片方式
train.iloc[0:2,0]                           #可以结合四种切片方式
train.ix['index1':'index3',0:2]
```

```
▶ 值模式
at [ ]     索引名称函数
iat [ ]    索引位置函数

train.at['index1','Age']
train.iat[1,2]
```

##### 3.4 其它数据选择方法

```
select_dtype    #通过列的数据类型选择数据
DataFrame.select_dtype(include = None,exclude = None)
```

```python
train.select_dtype(include=['float64'])  #列举浮点型64位
filter                          #通过匹配行列名称 来选择某些行或列
DataFrame.filter(items = None,like = None,regex = None,axis = None)
---'items','like','regex'       是排他的，只能用其中之一
    items:                      索引名称列表
    like:                       匹配包含like字符串的索引名称
    regex:                      匹配符合正则表达式的索引名称
    axis：                      0表示匹配行，1表示匹配列
```






