# 【数据平台】dataframe一列成多列 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年11月21日 19:36:25[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：7526
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、场景：一列中的值出现 1|2|3|56，这样用|分割的值，需要将其转换成4列。




2、操作：



```python
names=df['names'].str.split('|',expand=True)#多名字分列
```

```python
names.columns=['ids0','ids1','ids2','ids3','ids4','ids5','ids6','ids7']#
```
df=df.join(names)




其中names字段的列是包含|分割的值，最后将多列合并会原dataframe里。




3、函数：


- `Series.str.``split`(*pat=None*,
*n=-1*, *expand=False*)[[source]](http://github.com/pandas-dev/pandas/blob/v0.21.0/pandas/core/strings.py#L1474-L1477)

Split each string (a la re.split) in the Series/Index by givenpattern, propagating NA values. Equivalent to
[`str.split()`](https://docs.python.org/3/library/stdtypes.html#str.split).
|Parameters:|
|----|
|**split** : Series/Index or DataFrame/MultiIndex of objects|



||
|----|
||



