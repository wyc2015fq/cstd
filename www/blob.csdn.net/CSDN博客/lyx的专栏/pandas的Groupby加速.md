# pandas的Groupby加速 - lyx的专栏 - CSDN博客





2019年02月14日 22:05:52[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：746








          在平时的金融数据处理中，模型构建中，经常会用到pandas的groupby。之前的一篇文章中也讲述过groupby的作用：

[https://blog.csdn.net/qtlyx/article/details/80515077](https://blog.csdn.net/qtlyx/article/details/80515077)

         但是，大家都知道，python有一个东西叫做GIL，说白了就是python并没有多线程这种东西。那么，现在如果我们要进行groupby操作怎么办呢？我们可以使用多线程，使用一个叫做joblib的模块，来实现groupby的并行运算，然后在组合，有那么一点map-reduce的感觉。

        我们的场景是这样的：我们希望计算一系列基金收益率的beta。那么按照普通的方法，就是对每一个基金进行groupby，然后每次groupby的时候回归一下，然后计算出beta。我们来看一下计算beta的函数：

```python
def beta_cal_mult(one_fund_df):
    ll = list()
    for ind in range(len(one_fund_df)):
        one_fund_df_sub = one_fund_df.iloc[ind:ind + 20]
        ll.append(cross_regression(one_fund_df_sub, ['NAV_ADJ_RETURN1'], ['bench_mark_return']).params['bench_mark_return'])

    one_fund_df['beta_mult'] = pd.Series(ll).shift(19).tolist()
    # print pd.Series(ll).shift(19)
    return one_fund_df
```

        这段代码中的one_fund_df代表的是某个基金的return的时间序列，也就是NAV_ADJ_RETURN1以及含有计算beta的基准的return，bench_mark_return。

        那么，如果我们现有的数据是这样的：

date code  FUND_FUNDSCALE  NAV_ADJ_RETURN1  bench_mark_return

2015-01-05  000001.OF    4.059972e+09         1.782683           2.270838

2015-01-06  000001.OF    4.059972e+09         0.583820           0.699535

...               ...             ...              ...                ...

2018-11-21  960033.OF    5.157890e+07         0.073306           0.447395

2018-11-22  960033.OF    5.157890e+07        -0.324404          -0.179186

2018-11-23  960033.OF    5.157890e+07        -1.496063          -3.140655

2018-11-26  960033.OF    5.157890e+07        -0.234479          -0.156276

2018-11-27  960033.OF    5.157890e+07         0.486085           0.243327

2018-11-28  960033.OF    5.157890e+07         1.041888           1.251801

2018-11-29  960033.OF    5.157890e+07        -1.394150          -1.790077

        一般来说，我们对code进行groupby，然后apply一下上面这个函数就可以了。如果大家的电脑是多核的，大家在运行的时候会发现，其实只会有一个核被完全使用，而其他的核都是空闲着的。假设我们的数据量很大，而我们的服务器是50核的cpu，那么，这样的场景下，大家肯定会崩溃。

        所以，下面这串代码就是如何实现并行计算了。其实思路很简单，就是pandas groupby之后会返回一个迭代器，其中的一个值是groupby之后的部分pandas。所以，我们可以利用这个迭代器来送到多个进程中进行计算，最后把所有的结果合并整合。

```python
import pandas as pd
from joblib import Parallel, delayed
import multiprocessing
import statsmodels.api as sm


def cross_regression(df_temp, y_name, x_list, constant=True):
    df = df_temp.dropna()
    y = df[y_name]
    x = df[x_list]
    X = sm.add_constant(x) if constant else x
    results = sm.OLS(y, X, hasconst=constant).fit()
    return results

def beta_cal_mult(one_fund_df):
    ll = list()
    for ind in range(len(one_fund_df)):
        one_fund_df_sub = one_fund_df.iloc[ind:ind + 20]
        ll.append(cross_regression(one_fund_df_sub, ['NAV_ADJ_RETURN1'], ['bench_mark_return']).params['bench_mark_return'])

    one_fund_df['beta_mult'] = pd.Series(ll).shift(19).tolist()
    print pd.Series(ll).shift(19)
    return one_fund_df

def applyParallel(dfGrouped, func):
    retLst = Parallel(n_jobs=multiprocessing.cpu_count())(delayed(func)(group) for name, group in dfGrouped)
    return pd.concat(retLst)



data_df = pd.read_hdf('test.h5')
multi_res = applyParallel(data_df.iloc[:10000].groupby('code'), beta_cal_mult)
multi_res.to_hdf('fil.h5', key='data')
```

        上面这段代码的核心其实就是：

```python
multi_res = applyParallel(data_df.groupby('code'), beta_cal_mult)
```

        本来后面应该是：

```python
multi_res = data_df.groupby('code').apply(beta_cal_mult)
```

        而现在是使用了applyParallel函数，这个函数中：

```python
def applyParallel(dfGrouped, func):
    retLst = Parallel(n_jobs=multiprocessing.cpu_count())(delayed(func)(group) for name, group in dfGrouped)
    return pd.concat(retLst)
```

        使用了joblib中的Parallel函数，这个函数其实是进行并行调用的函数，其中的参数n_jobs是使用的计算机核的数目，后面其实是使用了groupby返回的迭代器中的group部分，也就是pandas的切片，然后依次送入func这个函数中。

        当数据量很大的时候，这样的并行处理能够节约的时间超乎想象，强烈建议pandas把这样的一个功能内置到pandas库里面。







