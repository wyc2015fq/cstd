# statsmodels的回归R2的问题 - lyx的专栏 - CSDN博客





2018年10月10日 21:23:15[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：819








        做量化呢，得经常做回归，各种各样的，ols，wls，正则的lasso， 岭回归等等。回归有一个很重要的整体解释力度的参数就是R2，也就是可决系数。在python中，我们回归一般采用的是statsmodels这个模块，但是回归的时候获得的R2其实有那么点学问，有时候设置错参数可能得到的R2大家会觉得怪怪的。这里就给大家排个雷。首先，我们先给出两组、六个回归函数。

第一组：

```python
def cross_reg1_1(df):
    results = sm.OLS(df.rate, sm.add_constant(df[['plf_ttm']]), hasconst=False).fit()
    return results

def cross_reg1_2(df):
    results = sm.OLS(df.rate, sm.add_constant(df[['plf_ttm']]), hasconst=True).fit()
    return results

def cross_reg1_3(df):
    results = sm.OLS(df.rate, sm.add_constant(df[['plf_ttm']])).fit()
    return results
```

第二组：

```python
def cross_reg2_1(df):
    results = sm.OLS(df.rate, df[['plf_ttm']],  hasconst=False).fit()
    return results

def cross_reg2_2(df):
    results = sm.OLS(df.rate, df[['plf_ttm']],  hasconst=True).fit()
    return results

def cross_reg2_3(df):
    results = sm.OLS(df.rate, df[['plf_ttm']]).fit()
    return results
```

        首先，我们来看一下组内的区别。每一组内部，其实就是有没有hasconstant这个参数，以及这个参数设置的问题。数据中，rate列作为y，plf_ttm作为x。大家不用在乎这是什么，只要关注后面的R2的结果就可以了。

![](https://img-blog.csdn.net/20181010205305916?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        我们分别获取了第一组的情况下的三种回归的r2和回归的结果。我们看到两点：

        1.r2来看，第二种和第三种是一样的。而第一中明显高于后面两种。这是为什么呢？或者说，这里的hasconstant=False到底干了什么呢？

        2.模型参数都有常数项

        我们来看一下statsmodels中的代码注释：

![](https://img-blog.csdn.net/20181010205700170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        我们发现，r2计算的时候分成两种，一种的tss分母是centered，一种是uncentered，而如果constant是包含在模型中，那么就是用centered，如果没有，那么就是uncentered。看注释其实理解起来挺绕的。不管，我们先来看一下什么叫做centered和uncentered吧。

![](https://img-blog.csdn.net/20181010211303395?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181010211318658?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        可以看出来，其实就是tss在计算的时候的差异。tss其实就是真实y值减去y均值的平方的和，sum((y-mean(y)) ** 2),代码大概是这样。这是我们常规认知的tss，也就是这里说的centered tss。而uncentered tss其实就是上面的均值不减。

        到这里，我们知道，如果y的均值不是0，那么显然，uncentered tss会比centered tss大的多，也就是r2会大得多。这也就是为什么我们在第一组中，constant是false的时候，r2这么大。

        那么第二组中把addconstant去掉之后的结果是怎么样的呢？

![](https://img-blog.csdn.net/20181010211905784?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        我们分析一下最后的结果，大家可以思考一下看看对不多哦。汇总之后，上面注释的意思是，如果模型当中是用到了constant显示的定义为True，那么，最后就用我们常规认知的r2，或者，使用了addconstant隐式使用了常数项，那么也是使用常规认知的r2.也就是说，r2层面，显示定义常数项的效力大于隐式定义的。而具体模型的参数，也就是常数项和回归系数则完全由隐式定义是否有常数项决定。



