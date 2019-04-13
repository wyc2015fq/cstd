
# python---pandas.merge使用 - leofionn的博客 - CSDN博客


2018年05月26日 16:27:59[leofionn](https://me.csdn.net/qq_36142114)阅读数：76个人分类：[python																](https://blog.csdn.net/qq_36142114/article/category/7385154)



## 0. merge 函数参数
”’
merge: 合并数据集， 通过left， right确定连接字段，默认是两个数据集相同的字段
参数 说明
left 参与合并的左侧DataFrame
right 参与合并的右侧DataFrame
how 连接方式：‘inner’（默认）；还有，‘outer’、‘left’、‘right’
on 用于连接的列名，必须同时存在于左右两个DataFrame对象中，如果位指定，则以left和right列名的交集作为连接键
left_on 左侧DataFarme中用作连接键的列
right_on 右侧DataFarme中用作连接键的列
left_index 将左侧的行索引用作其连接键
right_index 将右侧的行索引用作其连接键
sort 根据连接键对合并后的数据进行排序，默认为True。有时在处理大数据集时，禁用该选项可获得更好的性能
suffixes 字符串值元组，用于追加到重叠列名的末尾，默认为（‘_x’,‘_y’）.例如，左右两个DataFrame对象都有‘data’，则结果中就会出现‘data_x’，‘data_y’
copy 设置为False，可以在某些特殊情况下避免将数据复制到结果数据结构中。默认总是赋值
”’
## 1.merge默认按相同字段合并，且取两个都有的。
```python
import
```
```python
pandas
```
```python
as
```
```python
pd
df1=pd.DataFrame({
```
```python
'name'
```
```python
:[
```
```python
'kate'
```
```python
,
```
```python
'herz'
```
```python
,
```
```python
'catherine'
```
```python
,
```
```python
'sally'
```
```python
],
```
```python
'age'
```
```python
:[
```
```python
25
```
```python
,
```
```python
28
```
```python
,
```
```python
39
```
```python
,
```
```python
35
```
```python
]})
df2=pd.DataFrame({
```
```python
'name'
```
```python
:[
```
```python
'kate'
```
```python
,
```
```python
'herz'
```
```python
,
```
```python
'sally'
```
```python
],
```
```python
'score'
```
```python
:[
```
```python
70
```
```python
,
```
```python
60
```
```python
,
```
```python
90
```
```python
]})
pd.merge(df1,df2)
```
1
2
3
4
5
6
7
”’
age name score
0 25 kate 70
1 28 herz 60
2 35 sally 90
”’
## 2. 当左右连接字段不相同时，使用left_on,right_on
```python
pd.
```
```python
merge
```
```python
(df1,df2,left_on=
```
```python
"name"
```
```python
,right_on=
```
```python
'call_name'
```
```python
)
```
1
”’
age name call_name score
0 25 kate kate 70
1 28 herz herz 60
2 35 sally sally 90
”’
## 3. 合并后，删除重复的列
```python
pd.
```
```python
merge
```
```python
(df1,df2,left_on=
```
```python
'name'
```
```python
,right_on=
```
```python
'call_name'
```
```python
).
```
```python
drop
```
```python
(
```
```python
'name'
```
```python
,axis=
```
```python
1
```
```python
)
```
1
”’
age call_name score
0 25 kate 70
1 28 herz 60
2 35 sally 90
”’
## 4.参数how的使用
“1)默认：inner 内连接，取交集”
```python
pd.
```
```python
merge
```
```python
(df1,df2,
```
```python
on
```
```python
=
```
```python
'name'
```
```python
,
```
```python
how
```
```python
=
```
```python
'inner'
```
```python
)
```
1
”’
age name score
0 25 kate 70
1 28 herz 60
2 35 sally 90
”’
“2)outer 外连接，取并集，并用nan填充”
```python
df3=pd.DataFrame({
```
```python
'name'
```
```python
:[
```
```python
'kate'
```
```python
,
```
```python
'herz'
```
```python
,
```
```python
'sally'
```
```python
,
```
```python
'cristin'
```
```python
],
```
```python
'score'
```
```python
:[
```
```python
70
```
```python
,
```
```python
60
```
```python
,
```
```python
90
```
```python
,
```
```python
30
```
```python
]})
pd.
```
```python
merge
```
```python
(df1,df3,
```
```python
on
```
```python
=
```
```python
'name'
```
```python
,
```
```python
how
```
```python
=
```
```python
'outer'
```
```python
)
```
1
2
3
”’
age name score
0 25 kate 70
1 28 herz 60
2 39 catherine NaN
3 35 sally 90
4 NaN cristin 30
”’
“3)left 左连接， 左侧取全部，右侧取部分”
```python
pd.
```
```python
merge
```
```python
(df1,df3,
```
```python
on
```
```python
=
```
```python
'name'
```
```python
,
```
```python
how
```
```python
=
```
```python
'left'
```
```python
)
```
1
”’
age name score
0 25 kate 70
1 28 herz 60
2 39 catherine NaN
3 35 sally 90
”’
“4) right 有连接，左侧取部分，右侧取全部”
```python
pd.
```
```python
merge
```
```python
(df1,df3,
```
```python
on
```
```python
=
```
```python
'name'
```
```python
,
```
```python
how
```
```python
=
```
```python
'right'
```
```python
)
```
1
”’
age name score
0 25 kate 70
1 28 herz 60
2 35 sally 90
3 NaN cristin 30
”’



