# python 数据结构（Data Structure） - Nicole的博客 - CSDN博客
2018年04月04日 10:56:18[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：800

importpandasaspdimportnumpyasnpimportmatplotlib.pyplotasplt

# 一、Data Structure
## 1.List
列表是Python自带的数据结构，是一个按照定顺序存储对象的数据结构。通常通过索引访问列表，索引从0开始。
### (1)修改列表:给列表赋值
```python
x = [1,1,1]
x[1] = 2 #使用索引法给特定位置的元素赋值
x
[1, 2, 1]
len(x)
3
x[100] = 2    #注意：不能给不存在的元素赋值，如果列表的长度为2，就不能给索引为100的元素赋值
---------------------------------------------------------------------------
IndexError                                Traceback (most recent call last)
<ipython-input-5-ba9baf74e72a> in <module>()
----> 1 x[100] = 2
IndexError: list assignment index out of range
x = [None]*101
x[100] = 2
len(x) #将x的长度初始化为101
101
```
### （2）删除元素
```python
sample_list = [1, 2, 3]
del sample_list[2]  #删除列表中的元素
sample_list
[1, 2]
```
### （3）切片
除开可以使用索引来访问单个元素外，还可以使用切片来访问特定范围内的元素，切片用于提取序列的元素，第一个索引是包含在切片内，但第二个索引是切片后余下的第一个元素的编号
```python
sample_list[0: 2] # 取第一个到第二个元素
[1, 2]
sample_list[1: ] # 取第二个到最后一个元素
[2, 3]
sample_list[: 2] # 取第一个到第二个元素，序列起始于开头，可以省略前面的索引
[1, 2]
sample_list[-2] #提取列表中倒数第二个元素
2
sample_list[-3:-1]  #从倒数第三个到倒数第一个元素，不包括最后一个元素
[1, 2]
sample_list[-3:0]  #执行切片操作时，如果第一索引指定的元素位于第二个索引指定的元素的后面（倒数第3个元素位于第一个元素的后面），结果就为空序列。
[]
sample_list[-2:] #如果切片借宿于序列末尾，可以省略第二个索引
[2, 3]
sample_list[:] #复制整个序列
[1, 2, 3]
''.join(a)
'hello'
```
### (4)列表方法
```
方法是于对象联系紧密的函数，通常采用下面的调用方式：
```
object.method(argument)
#### append：用于将一个对象附加到列表末尾
```python
lst = [1,2,3]
lst.append(4)  
lst
[1, 2, 3, 4]
```
#### clear:就地清空列表内容
```python
lst = [1,2,3]
lst.clear()
lst
[]
```
#### copy:复制列表
```python
lst = [1,2,3]
b = lst  #常规的复制只是将另一个名称关联到列表
b[1] = 4
lst
[1, 4, 3]
lst = [1,2,3]
b = lst.copy() #不改变lst的值
b[1] = 4
lst
[1, 2, 3]
```
#### extend：能够同时将多个值附加到列表末尾
和拼接的区别在于extend会修改被扩展的序列
```python
a = [1,2,3]
b = [4,5,6]
a.extend(b)  #extend会修改原来的序列
a
[1, 2, 3, 4, 5, 6]
a = [1,2,3]
b = [4,5,6]
a + b
[1, 2, 3, 4, 5, 6]
a   #拼接不会修改原来的序列
[1, 2, 3]
a[len(a):] = b   #采用该方式可以得到和extend同样的效果
a    
[1, 2, 3, 4, 5, 6]
```
#### index：在列表中查找值第一次出现的索引
```python
knights = ['We','are','the','knights','who','say','ni']
knights.index('who')  #搜索单词who，返回其所在的索引
4
knights[4]  #搜索索引为4的元素
'who'
```
#### insert：用于将一个对象插入到列表中
```python
numbers = [1,2,3,5,6,7]
numbers.insert(3,'four') #将four插入到索引为3的位置中
numbers
[1, 2, 3, 'four', 5, 6, 7]
numbers = [1,2,3,5,6,7]
numbers[3:3] = ['four'] #可以使用切片来进行相同的操作
numbers
[1, 2, 3, 'four', 5, 6, 7]
```
#### pop：从列表中删除一个元素（默认情况下删除末尾的元素），并返回该元素
```python
x = [1,2,3]
x.pop() #删除末尾的元素
3
x
[1, 2]
x.pop(0)   #删除索引为0的元素
1
x
[2]
x = [1,2,3]
x.append(x.pop())
x
[1, 2, 3]
```
#### remove:用于删除第一个为指定值的元素
```python
x = ['to','be','or','not','to','be']
x.remove('be')  #删除第一个be
x
['to', 'or', 'not', 'to', 'be']
x.remove('bee')  #当删除一个不在list中的元素时会报错
---------------------------------------------------------------------------
ValueError                                Traceback (most recent call last)
<ipython-input-107-78e8416f31e9> in <module>()
----> 1 x.remove('bee')
ValueError: list.remove(x): x not in list
reverse:按相反的顺序排序列表中的元素
x = [1,2,3]
x.reverse()
x
[3, 2, 1]
```
#### sort:用于对列表就地排序
```python
x =[4,6,2,1,7,9]
y = x.sort()  #因为sort修改x是不反悔任何值额，最终的结果是x经过排序的，而y只包含none
print(y)   #返回结果
None
x
[1, 2, 4, 6, 7, 9]
###正确的方法
x =[4,6,2,1,7,9]
y = x.copy()  #将x关联到y中
y.sort()
x
[4, 6, 2, 1, 7, 9]
y
[1, 2, 4, 6, 7, 9]
###法二：
x =[4,6,2,1,7,9]
y = sorted(x)  #sorted()函数返回值
x
[4, 6, 2, 1, 7, 9]
y
[1, 2, 4, 6, 7, 9]
sorted('python')
['h', 'n', 'o', 'p', 't', 'y']
x = [4,6,2,1,7,9]
x.sort(reverse = True)  #先对x进行排序再反转
x
[9, 7, 6, 4, 2, 1]
```
## 2.set
集合跟數學中的集合是相同的概念，集合中國沒有重複的元素，並且元素之間沒有順序

```python
set([1,2,3,2])  #返回{1,2,3},利用set
a = {1,2,3,2} #或者使用花括号
a
{1, 2, 3}
0xAF 
175
x = {}   #default为dict
type(x)  #可以使用type来查看
dict
x = {"name": "cookie","age":12}   #当有key值时表示dict
x
{'age': 12, 'name': 'cookie'}
x = {12,13,12,45}   #当不存在key值时表示是dict
x
{12, 13, 45}
type(x)
set
```

## 3.Tuple
元组由不能改变的元素组成，你可以像操作列表一样通过索引访问单个元素或者通过切片来访问多个元素）
### （1）创建一个tuplle

```python
1,2,3  #tuple的语法很简单，只要讲一些值用逗号分隔开就能创建一个元组
(1, 2, 3)
(1,2,3) #或者使用圆括号
(1, 2, 3)
() #空元组用不包含任何内容的元括号表示
()
42
42
42,  #虽然只有一个值，当后面也要加上逗号，表示创建的为元组
(42,)
（2）使用list创建tuple
tuple([1,2,3])
(1, 2, 3)
tuple('abc') 
('a', 'b', 'c')
tuple((1,2,3))  #如果参数本身已经是元组，则原封不动返回它
(1, 2, 3)
```

### （3）tuple的访问
```python
sample_tuple = (1,3,2)
sample_tuple[0] #访问tuple中的第一个元素
1
sample_tuple[0] = 100 #报错，因为元组中的元素时不能改变的
---------------------------------------------------------------------------
TypeError                                 Traceback (most recent call last)
<ipython-input-144-cfaaeb36b283> in <module>()
----> 1 sample_tuple[0] = 100 #报错，因为元组中的元素时不能改变的
TypeError: 'tuple' object does not support item assignment
```
## 4.Dict
字典是有一个或多个键值对组成的列表。每个键与其值之间用冒号(:)分隔，项之间用逗号分隔，而整个字典放在花括号中。
### (1)创建Dict
```python
items = [('name','Gumby'),('age',42)]
items
[('name', 'Gumby'), ('age', 42)]
d = dict(items)  #使用dict函数从其他映射或键-值对序列创建字典
d
{'age': 42, 'name': 'Gumby'}
d['name']
'Gumby'
d = dict(name='Gumby',age=42)  #使用dict函数时用等号
d
{'age': 42, 'name': 'Gumby'}
d = {age: 42, name: 'Gumby'}  #key值要用引号括起来
---------------------------------------------------------------------------
NameError                                 Traceback (most recent call last)
<ipython-input-15-8faa84babc92> in <module>()
----> 1 d = {age: 42, name: 'Gumby'}
NameError: name 'age' is not defined
d = {'age': 42, 'name': 'Gumby'}
d
{'age': 42, 'name': 'Gumby'}
```
### (2)dict的基本操作
```python
x = {}
x[42] = 'Foobar' #dict允许将一个值付给一个空字典的键42，而list则不允许这种操作
x
{42: 'Foobar'}
len(x)  #求字典包含的项数
1
del x[42]  #删除x中key值为42的值
x
{}
sample_dict={'name':'daniel','age':27}
sample_dict['name'] #字典中的值可以通过键进行访问
'daniel'
{'name':'cookie','scores':[12,30,20]}   #键值对中的值可以是字符、字符串、数字、其它对象，比如列表等。
{'name': 'cookie', 'scores': [12, 30, 20]}
```
### （3）将字符串格式设置功能应用于字典
```python
phonebook = {'Beth':'9102','Alice':'2341','Cecil':'3258'}
phonebook
{'Alice': '2341', 'Beth': '9102', 'Cecil': '3258'}
"Cecil 's phone number is {Cecil}.".format_map(phonebook)
"Cecil 's phone number is 3258."
```
### (4)字典方法
#### clear：删除所有的字典项，这种操作时就地执行的，因此什么都不会反悔
```python
d = {}
d['name']= 'Gumby'
d['age'] = 42
d
{'age': 42, 'name': 'Gumby'}
returned_value = d.clear()
d
{}
print(returned_value)  #操作就地进行，且不返回任何值
None
```
#### copy：返回一个新字典
##### 浅复制，当替换副本中的值时，原件不收影响
```python
x = {'username':'admin','machines':['foo','bar','baz']}
y = x.copy()
y['username'] = 'mlh'
y['machines'].remove('bar')
y
{'machines': ['foo', 'baz'], 'username': 'mlh'}
x
{'machines': ['foo', 'baz'], 'username': 'admin'}
```
##### 深复制，同时复制值及其包含的所有值
```python
from copy import deepcopy
d = {}
d['names'] = ['Alfred','Bertrand']
d
{'names': ['Alfred', 'Bertrand']}
c = d.copy()  #浅复制
dc = deepcopy(d)  #深复制
d['names'].append('Clive')
c
{'names': ['Alfred', 'Bertrand', 'Clive']}
dc
{'names': ['Alfred', 'Bertrand']}
```
#### fromkeys:创建一个新字典，其中包含指定的键，且每个键对应的值都是None

```python
{}.fromkeys(['name','age']) #先创建一个字典，再对其调用方法fromkeys来创建另一个字典
{'age': None, 'name': None}
dict.fromkeys(['name','age']) #直接对dict调用方法fromkeys
{'age': None, 'name': None}
dict.fromkeys(['name','age'],'(unknown)')  #可以提供特定的值
{'age': '(unknown)', 'name': '(unknown)'}
```

#### get：方法get为访问字典提供了宽松的环境，如果您试图访问dict中没有的item，将会引发错误
```python
d = {}
print(d['name'])  #访问dict中没有的item会引发错误
---------------------------------------------------------------------------
KeyError                                  Traceback (most recent call last)
<ipython-input-228-866f59f378dc> in <module>()
----> 1 print(d['name'])
KeyError: 'name'
print(d.get('name'))  #使用get访问不存在的key时，没有引发异常，而是返回None
None
d.get('name','N/A') #可以指定默认的返回值
'N/A'
d['name'] = 'Eric'
d.get('name')  #如果dict中包含指定的key，则get的作用和普通的字典查找一样
'Eric'
```
#### items:返回一个包含所有字典项的列表，其中每个元素都为（key，value）形式
```python
d = {'title':'python web site','url':'http://www.python.org','spam':0}
d.items() #返回值属于一种名为字典视图的特殊数据类型
dict_items([('title', 'python web site'), ('url', 'http://www.python.org'), ('spam', 0)])
it = d.items()
len(it)  #确定dict的长度
3
('spam',0) in it #执行成员资格检查
True
d['spam'] = 1
('spam',0) in it   #视图的一个优点是不复制，它始终是底层字典的反映
False
d['spam'] = 0
('spam',0) in it
True
```
#### keys：返回一个字典视图，其中包括指定字典中的键
d.keys()#返回dict中的keys

dict_keys(['title', 'url', 'spam'])

#### pop：用于获取与指定键相关联的值，并将该键-值从字典中删除
```python
d = {'x':1,'y':2}
d.pop('x')
1
d
{'y': 2}
d.pop(1)  #参数为dict中的key
---------------------------------------------------------------------------
KeyError                                  Traceback (most recent call last)
<ipython-input-250-83b07e797b3a> in <module>()
----> 1 d.pop(1)
KeyError: 1
```
#### popitem:类似于list.pop，但list.pop弹出列表中的最后一个元素，而popitem随机弹出一个字典项，因为字典项的顺序是不确定的，它没有最后一个元素的概念。
```python
d = {'title':'python web site','url':'http://www.python.org','spam':0}
d.popitem()  #随机第删除一个item
('spam', 0)
d
{'title': 'python web site', 'url': 'http://www.python.org'}
```
#### setdefault：类似get，因为它也获取与指定键相关联的值，但除此之外，setdefault还在字典不包含指定的键时，在字典中添加指定的key-value对
```python
d = {}
d.setdefault('name','N/A')  #如果指定的key不存在，则返回指定的value并相应地更新dict
'N/A'
d
{'name': 'N/A'}
d['name'] = 'Gumby'
d.setdefault('name','N/A') #如果指定的key存在，则返回其value，并保持dict不变
'Gumby'
d
{'name': 'Gumby'}
d = {}
print(d.setdefault('name'))  #和get一样，value是可选的，如果没有指定，默认为None
None
d
{'name': None}
```
#### update：使用一个字典中的item更新另外一个dict
```python
d = {'title':'python web site',
        'url':'http://www.python.org',
         'changed':'Mar 14 22:09:15 MET 2016'}
x = {'title':'python language website'}
d.update(x) #更新d中的title项
d
{'changed': 'Mar 14 22:09:15 MET 2016',
 'title': 'python language website',
 'url': 'http://www.python.org'}
```
#### values：返回一个由dict中的value组成的字典视图
```python
d = {}
d[1] = 1
d[2] = 2
d[3] = 3
d[4] = 1
d.values()
dict_values([1, 2, 3, 1])
d
{1: 1, 2: 2, 3: 3, 4: 1}
```
## 5.Array
array是numpy模块提供的一种一维或者多维数组，当利用机器学习进行模型训练时，我们通常会把数据集转换成array传递给算法进行训练
### （1）数组的形式

```python
np.array([1,2,3])
array([1, 2, 3])
np.array([[3,4,3],[3,4,5]]) #返回一个2*3的多维数组/矩阵
array([[3, 4, 3],
       [3, 4, 5]])
sample_array = np.array([1,2,3])
sample_array
array([1, 2, 3])
```

### （2）数组的基本统计量

```python
sample_array.min()    #返回array中的最小值1
1
sample_array.max()    #返回array中的最大值
3
sample_array.mean()   #返回数组中的均值
2.0
sample_array.sum()   #求和
6
sample_array.std()    #标准差
0.816496580927726
sample_array.shape  #返回数组的形状为
(3,)
sample_array.resize((3,1)) #改变数组的形状
sample_array
array([[1],
       [2],
       [3]])
sample_array.T  #得到数组的转置
array([[1, 2, 3]])
np.array([1,2])*2 #对数组进行加减乘除
array([2, 4])
np.array([1,2])*np.array([1,2])#数组的乘法
array([1, 4])
```

## 6.series
series是pandas模块提供的一种以为的数组，它类似numpy中国的Array,但是有一些特殊的功能，pandas的数据结构都会包含索引，索引可以是数字，也可以是字符

sample_series=pd.Series([,,],index=['a','b','c'])

sample_series.value_counts()#统计每个series中每个值出现的次数

1    2
3    1
dtype: int64

### （1）处理数据
sample_series.unique()

array([1, 3], dtype=int64)

sample_series.abs()sample_series.corr()sample_series.diff()sample_series.describe()sample_series.dropna()sample_series.drop_duplicates()

a    1
b    1
c    3
dtype: int64

### （2）series还提供了很多函数计算统计量

sample_series.mean()sample_series.kurtosis()sample_series.median()sample_series.max()sample_series.min()sample_series.mode()sample_series.skew()

## 7.DataFrame
DataFrame是一种极为强大的数据结构。除了具备Series的功能和结构以外，还提供了更多的数据处理、分析相关的函数和功能。

sample_df=pd.DataFrame({'name':['daniel','cookie','cookie'],"score":[,,]})

sample_df.loc[sample_df['name']=='cookie','score']sample_df.groupby('name').mean()
||score|
|----|----|
| | |
|90| |
|90| |

# 二、read data
## 1、Text File

pd.read_csv(filename,sep=',',header='infer',index_col=None,use_cols=None)

## 2、Excel

pd.read_excel(filename,sheetname=,header=,skiprows=False,index_col=None,use_cols=None)

## 3、Database
pd.read_sql(query,connection)##通常我们需要提供一个连接字符串给这个函数去连接相应的数据库。mysql+pymysql://data_viewer:test@localhost/test?charset=utf8

---------------------------------------------------------------------------NameError                                 Traceback (most recent call last)
<ipython-input-42-af3a40e724d1> in <module>()----> 1pd.read_sql(query, connection)##通常我们需要提供一个连接字符串给这个函数去连接相应的数据库。mysql+pymysql://data_viewer:test@localhost/test?charset=utf8NameError: name 'query' is not defined

# 三、Know Your Data
当通过pandas将数据读取到DataFrame后，首先可以查看数据的一些性质。假设df 是一个DataFrame，通过下面的命令可以查看数据的基本状况。

df=pd.DataFrame({'name':['daniel','cookie','cookie'],"score":[,,]})

## 1.查看数据的基本情况
### （1）查看具体分布
df.head()#查看前N条记录，默认是5条
||name|score|
|----|----|----|
|daniel|90| |
|cookie|80| |
|cookie|100| |

df.sample()#随机查看2条记录
||name|score|
|----|----|----|
|daniel|90| |
|cookie|80| |

df.info()#查看数据的数据类型和空值情况。

<class 'pandas.core.frame.DataFrame'>
RangeIndex: 3 entries, 0 to 2
Data columns (total 2 columns):
name     3 non-null object
score    3 non-null int64
dtypes: int64(1), object(1)
memory usage: 128.0+ bytes
df.describe()#查看数据的基本统计值。
||score|
|----|----|
|3.0| |
|90.0| |
|10.0| |
|80.0| |
|85.0| |
|90.0| |
|95.0| |
|100.0| |

df.corr()#查看数据之间的相关系数。
||score|
|----|----|
|1.0| |

### （2）查看数据缺失情况
np.product(df.shape)#product将数组中的元素连乘，shape求数据框的形状，求数据框中值的个数

12

df.isnull().sum().sum()#先对每一列求和，再对每一行求和

0

df.isnull().sum().sum()/np.product(df.shape)# 查看数据总体的缺失程度

0.0
str.replace(old, new[, max])replace()方法把字符串中的 old（旧字符串） 替换成 new(新字符串)，如果指定第三个参数max，则替换不超过 max 次。
df.isnull().replace(False,True).sum()#将df中的False值替换成True

age      4
count    4
name     4
dtype: int64

df.isnull().sum()/df.isnull().replace(False,True).sum()#查看每个变量的缺失程度

name     0.0
score    0.0
dtype: float64

### （3）分组

df.groupby('state').mean()# 以state进行分组，返回每组的均值
df.groupby(['state','city']).mean()# 以state和city进行分组，返回每组的均值

### 2.Process Data
通常我们的数据中会有一些缺失值，异常值需要处理，以及一些数据需要清理，整理格式，编码等。Pandas提供了很多有用的函数。
### （1）丢弃缺失值

# 删除所有包含缺失值的行df.dropna()# 删除所有包含缺失值的列df.dropna(axis=)

### （2）处理重复值
DataFrame.drop_duplicates(subset=None, keep='first', inplace=False)参数这个drop_duplicate方法是对DataFrame格式的数据，去除特定列下面的重复行。返回DataFrame格式的数据。    subset : column label or sequence of labels, optional    用来指定特定的列，默认所有列    keep : {‘first’, ‘last’, False}, default ‘first’，默认保留第一次出现的项    删除重复项并保留第一次出现的项    inplace : boolean, default False，默认在原有数据上修改    是直接在原来数据上修改还是保留一个副本
# 删除重复的行df.drop_duplicates()

### （3）处理缺失值
# 以0填充所有缺失值df.fillna()#以空值所在列的下一个值进行填充，如果下一个值也是空值，则不填充df.fillna(method='bfill',axis=)#pad (ffill) or backfill (bfill)，用前面一个数值填充还是后面一个数值填充
||age|count|name|
|----|----|----|----|
|20|80|daniel| |
|3|90|cookie| |
|27|100|derrick| |
|26|200|renee| |

### （4）利用apply函数清理数据
# 假设数据中的name都是以first name-last name的形式存储，想要分理出first name可以利用匿名函数df.apply(lambdarow:str(row['name']).split('-')[],axis=)#对每一行使用函数

0     daniel
1     cookie
2    derrick
3      renee
dtype: object
# 对于复杂的处理过程可以编写函数defprocess_zipcode(zipcode):ifzipcode:new_zip=str(zipcode).strip().split('-')iflen(new_zip)==:returnnew_zip[-]eliflen(new_zip)==:returnnew_zip[-]else:returnnew_zip[]else:return
df.apply(lambdarow:process_zipcode(row['zipcode']),axis=)

### （5）生成虚拟变量

# 将df中的非数字的变量转换为以dummy_为前缀的虚拟变量pd.get_dummies(df,prefix='dummy',prefix_sep='_')

## 3.Data Visualization
Pandas的DataFrame提供了matplotlib数据可视化包的接口，可以十分方便的直接通过DataFrame对象进行数据可视化。
```python
df = pd.DataFrame({
    'name': ['daniel', 'cookie', 'derrick', 'renee'],
    'age': [20, 3, 27, 26],
    'count': [80, 90, 100, 200],
})
df.plot() # 默认做出DataFrame中数值型变量的折线图
<matplotlib.axes._subplots.AxesSubplot at 0x28d3c850fd0>
df.plot(kind='bar', x='age', y='count') # Bar Plot
<matplotlib.axes._subplots.AxesSubplot at 0x28d3a0b9748>
df.plot(kind='hist', x='age') # Histogram
<matplotlib.axes._subplots.AxesSubplot at 0x28d3a0fe8d0>
df.plot(kind='box', x='age') # Box Plot
<matplotlib.axes._subplots.AxesSubplot at 0x28d3a1bc898>
df.plot(kind='scatter', x='age', y='count') # Scatter Plot
<matplotlib.axes._subplots.AxesSubplot at 0x28d3a265278>
```

## 4.Write Data
当我们处理完数据后，就需要将数据写入文件或者数据库进行存储。Pandas提供了一下很方便的函数完成这些工作。

# 将数据框存储为test.csv，并且不保存索引，并用utf8进行编码df.to_csv('test.csv',index=False,encoding='uft8')# 将数据写入test.xlsx的test表格，不保存索引，并用utf8进行编码df.to_excel('test.xlsx',index=False,encoding='utf8',sheet_name='test')# 将数据写入数据库中的test表，如果test表存在，就在现有的表中插入这些数据，不保存索引df.to_sql('test',conn,if_exists='append',index=False)

