# pandas DataFrame的使用 - littlely_ll的博客 - CSDN博客





2018年10月23日 20:56:03[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：265








## DataFrame

### 使用`apply`函数

```python
df = pd.DataFrame({'x':[1,2,3,4,5],'y':['a','a','b','b','c']})
df

   x  y
0  1  a
1  2  a
2  3  b
3  4  b
4  5  c
```

要使得`df`的x和y值拼接起来，增加为一列，可以使用`apply`函数：

```python
def f(df):
    return str(df.x) + df.y
    
df['z'] = df.apply(f, axis=1)
df

   x  y   z
0  1  a  1a
1  2  a  2a
2  3  b  3b
3  4  b  4b
4  5  c  5c
```

`apply`默认的是传递整个dataframe，这里注意要加上`axis=1`，默认的是`axis=0`是把函数按照每一列进行变换，而`axis=1`为按照每一行进行转换

可以使用`lambda`函数：

```python
df['zz'] = df.apply(lambda df: str(df.x)+df.y, axis=1)
#更有效率地，使用join,而不使用“+”
df['zz'] = df.apply(lambda df: "".join([str(df.x),df.y]), axis=1)

 x  y   z  zz
0  1  a  1a  1a
1  2  a  2a  2a
2  3  b  3b  3b
3  4  b  4b  4b
4  5  c  5c  5c
```

### 数据透视表的实现

数据透视表有多种实现方式，一些包装方法如pivot和pivot_table都可以实现，但是也具有局限性，这里使用更一般的方法。

```
df = pd.DataFrame({'birth':[2000,2000,2000,2001,2001,2001,2002,2002,2002],\
'name':['a','a','a','b','b','b','c' ,'c','c'],'code':[1,1,1,2,2,2,3,3,3],\
'habit':['drink','smoke','drink','eat','smoke','drink','eat','smoke','drink',],\
'value':['little','a few','some','some','little','a few','little','a few','some']})

df

 birth  code  habit name   value
0   2000     1  drink    a  little
1   2000     1  smoke    a   a few
2   2000     1  drink    a    some
3   2001     2    eat    b    some
4   2001     2  smoke    b  little
5   2001     2  drink    b   a few
6   2002     3    eat    c  little
7   2002     3  smoke    c   a few
8   2002     3  drink    c    some
```

现在要把habit的值以及name, code, birth这几列作为列标签，value作为其值，即可使用数据透视表实现，其一般的方法为：

```python
#在以这些列为为列标签时，其值需要唯一，因此首先查看这些值是否唯一
df[df.duplicated(['name','code','birth','habit'])]

 birth  code  habit name value
2   2000     1  drink    a  some
```

发现索引为2的行与索引为0的行重复，因此首先删去这一行

```python
df1 = df.drop(df[df.duplicated(['birth','name','code','habit'])].index.values, axis=0).reset_index(drop=True)
df1

birth  code  habit name   value
0   2000     1  drink    a  little
1   2000     1  smoke    a   a few
2   2001     2    eat    b    some
3   2001     2  smoke    b  little
4   2001     2  drink    b   a few
5   2002     3    eat    c  little
6   2002     3  smoke    c   a few
7   2002     3  drink    c    some
```

最里面是找到重复标签索引的值，然后再删去这一行，`axis=0`为行删除，最后重新设置一下索引。这样没有重复值了。

下一步就是设定索引标签：

```python
dfs = df1.set_index(['name','code','birth','habit'])['value'].unstack().reset_index()
dfs

habit name  code  birth   drink     eat   smoke
0        a     1   2000  little    None   a few
1        b     2   2001   a few    some  little
2        c     3   2002    some  little   a few
```

一步一步来看这个结果：

首先设置索引，`set_index`里都是要变成列标签的，特别是habit那一列的值是要变成列标签的。设置后的结果为：

```python
df1.set_index(['name','code','birth','habit'])

                       value
name code birth habit        
a    1    2000  drink  little
                smoke   a few
b    2    2001  eat      some
                smoke  little
                drink   a few
c    3    2002  eat    little
                smoke   a few
                drink    some
```

接着取出值value，**注意：这里如果不取出值，在后续的过程中会形成多级标签**

```python
df1.set_index(['name','code','birth','habit'])['value']

name  code  birth  habit
a     1     2000   drink    little
                   smoke     a few
b     2     2001   eat        some
                   smoke    little
                   drink     a few
c     3     2002   eat      little
                   smoke     a few
                   drink      some
Name: value, dtype: object
```

下一步把habit索引变成列标签，注意，这里`unstack`默认的level参数为-1，即最里层的索引变为列标签

```python
df1.set_index(['name','code','birth','habit'])['value'].unstack()

habit             drink     eat   smoke
name code birth                        
a    1    2000   little    None   a few
b    2    2001    a few    some  little
c    3    2002     some  little   a few
```

当`set_index`中的索引标签顺序发生变化时，在设置`unstack`的level参数时也要变化，例如：

```python
df1.set_index(['name','code','habit','birth'])

                        value
name code habit birth        
a    1    drink 2000   little
          smoke 2000    a few
b    2    eat   2001     some
          smoke 2001   little
          drink 2001    a few
c    3    eat   2002   little
          smoke 2002    a few
          drink 2002     some
          
#habit排在索引标签的第3个，所以在unstack的时候level为2
df1.set_index(['name','code','habit','birth'])['value'].unstack(level=2)  

habit             drink     eat   smoke
name code birth                        
a    1    2000   little    None   a few
b    2    2001    a few    some  little
c    3    2002     some  little   a few
```

最后一步重新设置索引标签，这样就得到我们想要的结果了

```python
df1.set_index(['name','code','habit','birth'])['value'].unstack(level=2).reset_index()

habit name  code  birth   drink     eat   smoke
0        a     1   2000  little    None   a few
1        b     2   2001   a few    some  little
2        c     3   2002    some  little   a few
```

### pandas计算标准差的std方法与numpy的std方法

```python
df = pd.DataFrame({'a':[1,2,3,4],'b':[5,6,7,8]})
df.std()

a 1.290994
b 1.290994
dtype: float64

np.std(df.a)
np.std(df.b)

1.118033988749895
1.118033988749895
```

主要原因为`df`的`std`计算的是无偏估计，而`np.std`计算的是有偏估计，可以设置`np.std`的参数`ddof`为1则与`df.std`的结果一样了。

### df.isna(), df.isnull()和None

pandas的`isnull()`返回的是和对象相同大小的布尔值的对象。可以识别Python的`None`和numpy的`NaN`，但是空字符串’'和numpy的`np.inf`在pandas中并不认为是空值，对于`np.inf`可以设置`pandas.options.mode.use_inf_as_na = True`使得`np.inf`为空值 。

pandas的`isna()`和`isnull()`类似，而python中的`None`和numpy.nan的比较可以看[None vs numpy.nan](https://junjiecai.github.io/posts/2016/Oct/20/none_vs_nan/)

## iterrows与itertuples

`iterrows`对dataframe每行进行迭代，产生一个生成器，迭代的时候第一个元素为一行，第二个元素为每一行的列名和值，可以对其转化为字典的形式，这在使用sqlalchemy插入数据的时候非常有用。

```python
df = pd.DataFrame({'a':[1,2,3,4],'b':[5,6,7,8]})

a = []
for i in df.iterrows():
    a.append(dict(i[1]))

print(a)   
#[{'a': 1, 'b': 5}, {'a': 2, 'b': 6}, {'a': 3, 'b': 7}, {'a': 4, 'b': 8}]
```

`itertuples`对dataframe的每行值形成一个元组，它是一个map对象。

```python
df = pd.DataFrame({'a':[1,2,3,4],'b':[5,6,7,8]})

a = list(df.itertuples(name=False, index=False))
print(a)
#[(1, 5), (2, 6), (3, 7), (4, 8)]
```

### gropuby, agg, transform

```python
df = pd.DataFrame({'rti':['a','a','b','c','b','c','a'],'ts':[10,10,9,12,9,13,11],'rs':[8,8,22,11,12,11,9]})
```
 rti  ts  rs
0   a  10   8
1   a  10   8
2   b   9  22
3   c  12  11
4   b   9  12
5   c  13  11
6   a  11   9

根据数据`df`以`rti`进行分组，并对分组内的`ts`求不同值的个数，对分组内的`rs`进行求和。
`agg`函数需要按字段传递，这个`apply`函数有区别。

```python
df.groupby('rti').agg('ts':'nunique', 'rs':'sum').reset_index()
#与df.groupby('rti').agg('ts':lambda x: len(x.unique()), 'rs':'sum').reset_index()
```
 rti  ts  rs
0   a   2  25
1   b   1  34
2   c   2  22

去重用`nunique`，不去重用`size`。
`transform`是对分组后的元素求聚合函数后再广播到原来数据对应的元素上。与分组前使用`transform`对比，对分组后的dataframe只能使用一个函数，而分组前使用的`transform`不能使用聚合函数，而分组后可以使用聚合函数。**对分组后的dataframe只能使用一个函数而不能使用多个，这是一个Bug,以后可能会修改（见pandas的issue [TypeError: unhashable type: ‘dict’ when using apply/transform? #17309](https://github.com/pandas-dev/pandas/issues/17309)）**

```python
df.groupby('rti').transform('mean')
```
      ts         rs
0  10.333333   8.333333
1  10.333333   8.333333
2   9.000000  17.000000
3  12.500000  11.000000
4   9.000000  17.000000
5  12.500000  11.000000
6  10.333333   8.333333

## idxmin,idxmax

取pandas列最小值或最大值的索引。

```python
df = pd.DataFrame({'a':[9,2,3,4],'b':[5,6,1,8]})
df.idxmin()

#a 1
#b 2

df.a.idxmin()
#1
```

## pandas多条件查询

抽取dataframe中满足在上下四分位内条件的数据：

```python
df = pd.DataFrame({'a':[9,2,3,4,1,6],'b':[5,6,1,8,3,9]})
s = np.percentile(df.a,(25,75))
df[(df.a>s[0]) & (df.a<s[1])]

# a  b
# 3  1
# 4  8
```



