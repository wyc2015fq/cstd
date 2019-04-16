# Pandas之Categoricals数据类型 - 一个跳popping的quant的博客 - CSDN博客





2018年07月26日 16:22:11[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：2755








### **Pandas中的Categoricals模块能够有效地编码并显著性地提高了文本分类的效率。**

### 1、There is More to Speed Than Parallelism（速度比并行更重要）

我通常会写一些关于并行的东西。因此，很多人会问我如何并行化他们速度很慢的计算。这个问题的答案通常是**更好地使用pandas。**
- *Q: How do I make my pandas code faster with parallelism?*
- *A: You don’t need parallelism, you can use Pandas better.*

这几乎总是要比使用多核或者多个机器更简单并且更有效率。只有当对存储格式、压缩、数据表示等都做出了合理的选择之后，才应该考虑并行的问题。

现在，我们将讨论pandas如何用数字表示分类文本数据。这是一种廉价并且未被充分利用的技术，并且它可以在通常的查询上获得一个数量级的加速。

## 2、Categoricals

通常我们的数据包含了文本列，其中有许多重复的元素。如：
- Stock symbols – GOOG, APPL, MSFT, …
- Gender – Female, Male, …
- Experiment outcomes – Healthy, Sick, No Change, …
- States – California, Texas, New York, …

我们通常把它们表示为文本，Pandas将文本表示为对象类型，其中保持了Python普通string类型。这是常见的导致运行速度慢的原因，因为对象类型是以python中的对象类型运行的，而不是以正常的C语言的速度运行的。

Pandas categoricals是一种新型并且强大的特征，它可以数字化分类数据，这样我们就可以使用C语言的速度来解决这类文本数据了。

```python
# Example dataframe with names, balances, and genders as object dtypes
    df = pd.DataFrame({'name': ['Alice', 'Bob', 'Charlie', 'Danielle'],
                       'balance': [100.0, 200.0, 300.0, 400.0],
                       'gender': ['Female', 'Male', 'Male', 'Female']},
                        columns = ['name', 'balance', 'gender'])

    print(df.dtypes)
```

```
name        object
balance    float64
gender      object
dtype: object
```

我们可以用许多的重复信息来表示这些列，如性别，用categoricals可以更加有效。这将原始的数据分为了两部分 。

原始数据：
`Female, Male, Male, Female`
1、通过索引将每一类别映射到一个整数上。

```
Female: 0
Male: 1
...
```

2、整数的数组。
`0, 1, 1, 0`
现在这个整数的数组是一个更加紧凑而且是一个正常的C数组。这就允许了C语言加速在之前每一个慢的数据类型上了。Categorizing一个列很简单：
`df['gender'] = df['gender'].astype('category')  # Categorize!`
虽然最后的结果表示都一样：

```
name  balance  gender
0     Alice    100.0  Female
1       Bob    200.0    Male
2   Charlie    300.0    Male
3  Danielle    400.0  Female
```

但是此时的数据类型已经变了：

```
name         object
balance     float64
gender     category
dtype: object
```

这时候打印gender，底部多了一个Categories。

```
0    Female
1      Male
2      Male
3    Female
Name: gender, dtype: category
Categories (2, object): [Female, Male]
```

```python
print(df.gender.cat.categories)
    print(df.gender.cat.codes)
```

```
Index(['Female', 'Male'], dtype='object')
0    0
1    1
2    1
3    0
dtype: int8
```

此时每个结果的存储就只有一个字节了。我们也可以继续增加genders的值，这时Pandas将会相应地增加新的值（2,3...）。

我们的DataFrame数据外观和感觉跟之前都是一样的，但是此时Pandas内部的结构会使用户的体验更加平滑，这样你就不会感觉到你在使用一个整数数组了。

## 3、Performance

让我们试一下更大的数据来看一下效果的差距。

我们使用纽约市出租车数据集中的一小部分，并按照mediallinon ID来分组，找出在某段时间内行驶距离最长的出租车司机。

```python
In [1]: import pandas as pd
In [2]: df = pd.read_csv('trip_data_1_00.csv')

In [3]: %time df.groupby(df.medallion).trip_distance.sum().sort(ascending=False,
inplace=False).head()
CPU times: user 161 ms, sys: 0 ns, total: 161 ms
Wall time: 175 ms

Out[3]:
medallion
1E76B5DCA3A19D03B0FB39BCF2A2F534    870.83
6945300E90C69061B463CCDA370DE5D6    832.91
4F4BEA1914E323156BE0B24EF8205B73    811.99
191115180C29B1E2AF8BE0FD0ABD138F    787.33
B83044D63E9421B76011917CE280C137    782.78
Name: trip_distance, dtype: float64
```

这花费了约170ms，我们通过Categorize看一下效果。

```python
In [4]: %time df['medallion'] = df['medallion'].astype('category')
CPU times: user 168 ms, sys: 12.1 ms, total: 180 ms
Wall time: 197 ms
```

现在我们有了数字分类，我们的计算运行了20ms，提高了一个数量级。

```python
In [5]: %time df.groupby(df.medallion).trip_distance.sum().sort(ascending=False,
inplace=False).head()
CPU times: user 16.4 ms, sys: 3.89 ms, total: 20.3 ms
Wall time: 20.3 ms

Out[5]:
medallion
1E76B5DCA3A19D03B0FB39BCF2A2F534    870.83
6945300E90C69061B463CCDA370DE5D6    832.91
4F4BEA1914E323156BE0B24EF8205B73    811.99
191115180C29B1E2AF8BE0FD0ABD138F    787.33
B83044D63E9421B76011917CE280C137    782.78
Name: trip_distance, dtype: float64
```

通过一次操作将对象类型替换为了Categorize之后，我们几乎可以看到一个数量级的加速。 内存的占用率也显著降低。

## Conclusion

Pandas Categorize有效地编码重复的文本数据。分类对于一些股票符号、性别、实验结果、城市名称等是很有效的，可以极大的提高这些数据的性能。

在处理不方便的大数据或者慢数据时，我们有几个提高性能的选择。存储格式、亚索、列布局和数据表示的良好选择可以显著提高查询时间和内存的使用。这些选择中的每一个都与并行性一样重要，但不会被过度夸大，因此常常被忽略。



原文 [https://blog.dominodatalab.com/pandas-categoricals/](https://blog.dominodatalab.com/pandas-categoricals/)



