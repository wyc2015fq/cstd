# Python实现逻辑回归(Logistic Regression in Python) - zj360202的专栏 - CSDN博客





2017年12月01日 16:29:47[zj360202](https://me.csdn.net/zj360202)阅读数：28219









本文基于yhat上[Logistic Regression in Python](http://blog.yhathq.com/posts/logistic-regression-and-python.html)，作了中文翻译，并相应补充了一些内容。本文并不研究逻辑回归具体算法实现，而是使用了一些算法库，旨在帮助需要用Python来做逻辑回归的训练和预测的读者快速上手。


逻辑回归是一项可用于预测二分类结果(binary outcome)的统计技术，广泛应用于金融、医学、犯罪学和其他社会科学中。逻辑回归使用简单且非常有效，你可以在许多机器学习、应用统计的书中的前几章中找到个关于逻辑回归的介绍。逻辑回归在许多统计课程中都会用到。


我们不难找到使用R语言的高质量的逻辑回归实例，如UCLA的教程[R Data Analysis Examples: Logit Regression](http://www.ats.ucla.edu/stat/r/dae/logit.htm)就是一个很好的资源。Python是机器学习领域最流行的语言之一，并且已有许多Python的资源涵盖了[支持向量积](http://www.yaksis.com/posts/why-use-svm.html)和[文本分类](http://www.slideshare.net/japerk/nltk-in-20-minutes)等话题，但少有关于逻辑回归的资料。


本文介绍了如何使用Python来完成逻辑回归。



## 简介


示例代码中使用了一些算法包，请确保在运行这些代码前，你的电脑已经安装了如下包:
- [numpy](http://www.numpy.org/): Python的语言扩展，定义了数字的数组和矩阵
- [pandas](http://pandas.pydata.org/): 直接处理和操作数据的主要package
- [statsmodels](https://pypi.python.org/pypi/statsmodels): 统计和计量经济学的package，包含了用于参数评估和统计测试的实用工具
- [pylab](http://matplotlib.org/): 用于生成统计图


可参考 [Windows安装Python机器学习包](http://dblab.xmu.edu.cn/blog/python-machine-learning-package-windows/) 或 [Ubuntu/CentOS安装Python机器学习包](http://dblab.xmu.edu.cn/blog/python-machine-learning-package-ubuntu/) 来搭建所需要的环境。

## 逻辑回归的实例


在此使用与[Logit Regression in R](http://www.ats.ucla.edu/stat/r/dae/logit.htm)相同的数据集来研究Python中的逻辑回归，目的是要辨别不同的因素对研究生录取的影响。


数据集中的前三列可作为预测变量(predictor variables)：
- `gpa`
- `gre`分数
- `rank`表示本科生母校的声望


第四列`admit`则是二分类目标变量(binary target variable)，它表明考生最终是否被录用。

## 加载数据


使用 `pandas.read_csv`加载数据，这样我们就有了可用于探索数据的`DataFrame`。



```python
import pandas as pd
import statsmodels.api as sm
import pylab as pl
import numpy as np
 
# 加载数据
# 备用地址: http://cdn.powerxing.com/files/lr-binary.csv
df = pd.read_csv("http://www.ats.ucla.edu/stat/data/binary.csv")
 
# 浏览数据集
print df.head()
#    admit  gre   gpa  rank
# 0      0  380  3.61     3
# 1      1  660  3.67     3
# 2      1  800  4.00     1
# 3      1  640  3.19     4
# 4      0  520  2.93     4
 
# 重命名'rank'列，因为dataframe中有个方法名也为'rank'
df.columns = ["admit", "gre", "gpa", "prestige"]
print df.columns
# array([admit, gre, gpa, prestige], dtype=object)
```









注意到有一列属性名为`rank`，但因为`rank`也是pandas
 dataframe中一个方法的名字，因此需要将该列重命名为”prestige”.

## 统计摘要(Summary Statistics) 以及 查看数据


现在我们就将需要的数据正确载入到Python中了，现在来看下数据。我们可以使用`pandas`的函数`describe`来给出数据的摘要–`describe`与R语言中的`summay`类似。这里也有一个用于计算标准差的函数`std`，但在`describe`中已包括了计算标准差。


我特别喜欢`pandas`的`pivot_table/crosstab`聚合功能。`crosstab`可方便的实现多维频率表(frequency
 tables)(有点像R语言中的`table`)。你可以用它来查看不同数据所占的比例。



```python
# summarize the data
print df.describe()
#             admit         gre         gpa   prestige
# count  400.000000  400.000000  400.000000  400.00000
# mean     0.317500  587.700000    3.389900    2.48500
# std      0.466087  115.516536    0.380567    0.94446
# min      0.000000  220.000000    2.260000    1.00000
# 25%      0.000000  520.000000    3.130000    2.00000
# 50%      0.000000  580.000000    3.395000    2.00000
# 75%      1.000000  660.000000    3.670000    3.00000
# max      1.000000  800.000000    4.000000    4.00000
 
# 查看每一列的标准差
print df.std()
# admit      0.466087
# gre      115.516536
# gpa        0.380567
# prestige   0.944460
 
# 频率表，表示prestige与admin的值相应的数量关系
print pd.crosstab(df['admit'], df['prestige'], rownames=['admit'])
# prestige   1   2   3   4
# admit                   
# 0         28  97  93  55
# 1         33  54  28  12
 
# plot all of the columns
df.hist()
pl.show()
```












运行代码后，绘制的柱状统计图如下所示：

![使用pylab绘制的柱状统计图](http://cdn.powerxing.com/imgs/logit_hist.png)使用pylab绘制的柱状统计图

## 虚拟变量(dummy variables)


虚拟变量，也叫哑变量，可用来表示分类变量、非数量因素可能产生的影响。在计量经济学模型，需要经常考虑属性因素的影响。例如，职业、文化程度、季节等属性因素往往很难直接度量它们的大小。只能给出它们的“Yes—D=1”或”No—D=0”，或者它们的程度或等级。为了反映属性因素和提高模型的精度，必须将属性因素“量化”。通过构造0-1型的人工变量来量化属性因素。

`pandas`提供了一系列分类变量的控制。我们可以用`get_dummies`来将”prestige”一列虚拟化。

`get_dummies`为每个指定的列创建了新的带二分类预测变量的DataFrame，在本例中，`prestige`有四个级别：1，2，3以及4（1代表最有声望），`prestige`作为分类变量更加合适。当调用`get_dummies`时，会产生四列的dataframe，每一列表示四个级别中的一个。



```python
# 将prestige设为虚拟变量
dummy_ranks = pd.get_dummies(df['prestige'], prefix='prestige')
print dummy_ranks.head()
#    prestige_1  prestige_2  prestige_3  prestige_4
# 0           0           0           1           0
# 1           0           0           1           0
# 2           1           0           0           0
# 3           0           0           0           1
# 4           0           0           0           1
 
# 为逻辑回归创建所需的data frame
# 除admit、gre、gpa外，加入了上面常见的虚拟变量（注意，引入的虚拟变量列数应为虚拟变量总列数减1，减去的1列作为基准）
cols_to_keep = ['admit', 'gre', 'gpa']
data = df[cols_to_keep].join(dummy_ranks.ix[:, 'prestige_2':])
print data.head()
#    admit  gre   gpa  prestige_2  prestige_3  prestige_4
# 0      0  380  3.61           0           1           0
# 1      1  660  3.67           0           1           0
# 2      1  800  4.00           0           0           0
# 3      1  640  3.19           0           0           1
# 4      0  520  2.93           0           0           1
 
# 需要自行添加逻辑回归所需的intercept变量
data['intercept'] = 1.0
```








这样，数据原本的`prestige`属性就被`prestige_x`代替了，例如原本的数值为2，则`prestige_2`为1，`prestige_1`、`prestige_3`、`prestige_4`都为0。


将新的虚拟变量加入到了原始的数据集中后，就不再需要原来的`prestige`列了。在此要强调一点，生成m个虚拟变量后，只要引入m-1个虚拟变量到数据集中，未引入的一个是作为基准对比的。


最后，还需加上常数intercept，`statemodels`实现的逻辑回归需要显式指定。

## 执行逻辑回归


实际上完成逻辑回归是相当简单的，首先指定要预测变量的列，接着指定模型用于做预测的列，剩下的就由算法包去完成了。


本例中要预测的是`admin`列，使用到`gre`、`gpa`和虚拟变量`prestige_2`、`prestige_3`、`prestige_4`。`prestige_1`作为基准，所以排除掉，以防止[多元共线性](http://en.wikipedia.org/wiki/Multicollinearity#Remedies_for_multicollinearity)(multicollinearity)和引入分类变量的所有虚拟变量值所导致的陷阱([dummy
 variable trap](http://en.wikipedia.org/wiki/Dummy_variable_(statistics)))。



```python
# 指定作为训练变量的列，不含目标列`admit`
train_cols = data.columns[1:]
# Index([gre, gpa, prestige_2, prestige_3, prestige_4], dtype=object)
 
logit = sm.Logit(data['admit'], data[train_cols])
 
# 拟合模型
result = logit.fit()
```









在这里是使用了`statesmodels`的[Logit](http://en.wikipedia.org/wiki/Logit)函数，更多的模型细节可以查阅`statesmodels`的[文档](http://statsmodels.sourceforge.net/stable/index.html)

## 使用训练模型预测数据


（本小节是博主补充的）通过上述步骤，我们就得到了训练后的模型。基于这个模型，我们就可以用来预测数据，代码如下：



```python
# 构建预测集
# 与训练集相似，一般也是通过 pd.read_csv() 读入
# 在这边为方便，我们将训练集拷贝一份作为预测集（不包括 admin 列）
import copy
combos = copy.deepcopy(data)
 
# 数据中的列要跟预测时用到的列一致
predict_cols = combos.columns[1:]
 
# 预测集也要添加intercept变量
combos['intercept'] = 1.0
 
# 进行预测，并将预测评分存入 predict 列中
combos['predict'] = result.predict(combos[predict_cols])
 
# 预测完成后，predict 的值是介于 [0, 1] 间的概率值
# 我们可以根据需要，提取预测结果
# 例如，假定 predict > 0.5，则表示会被录取
# 在这边我们检验一下上述选取结果的精确度
total = 0
hit = 0
for value in combos.values:
  # 预测分数 predict, 是数据中的最后一列
  predict = value[-1]
  # 实际录取结果
  admit = int(value[0])
 
  # 假定预测概率大于0.5则表示预测被录取
  if predict > 0.5:
    total += 1
    # 表示预测命中
    if admit == 1:
      hit += 1
 
# 输出结果
print 'Total: %d, Hit: %d, Precision: %.2f' % (total, hit, 100.0*hit/total)
# Total: 49, Hit: 30, Precision: 61.22
```









在这里，我是简单的将原始数据再作为待预测的数据进行检验。通过上述步骤得到的是一个概率值，而不是一个直接的二分类结果（被录取/不被录取）。通常，我们可以设定一个阈值，若 predict 大于该阈值，则认为是被录取了，反之，则表示不被录取。


在上面的例子中，假定预测概率大于 0.5 则表示预测被录取，一共预测有 49 个被录取，其中有 30 个预测命中，精确度为 61.22%。

## 结果解释

`statesmodels`提供了结果的摘要，如果你使用过R语言，你会发现结果的输出与之相似。



```python
# 查看数据的要点
print result.summary()
```







```python
Logit Regression Results                           
==============================================================================
Dep. Variable:                  admit   No. Observations:                  400
Model:                          Logit   Df Residuals:                      394
Method:                           MLE   Df Model:                            5
Date:                Sun, 03 Mar 2013   Pseudo R-squ.:                 0.08292
Time:                        12:34:59   Log-Likelihood:                -229.26
converged:                       True   LL-Null:                       -249.99
                                        LLR p-value:                 7.578e-08
==============================================================================
                 coef    std err          z      P>|z|      [95.0% Conf. Int.]
------------------------------------------------------------------------------
gre            0.0023      0.001      2.070      0.038         0.000     0.004
gpa            0.8040      0.332      2.423      0.015         0.154     1.454
prestige_2    -0.6754      0.316     -2.134      0.033        -1.296    -0.055
prestige_3    -1.3402      0.345     -3.881      0.000        -2.017    -0.663
prestige_4    -1.5515      0.418     -3.713      0.000        -2.370    -0.733
intercept     -3.9900      1.140     -3.500      0.000        -6.224    -1.756
==============================================================================
```

```python
# 查看每个系数的置信区间
print result.conf_int()
#                    0         1
# gre         0.000120  0.004409
# gpa         0.153684  1.454391
# prestige_2 -1.295751 -0.055135
# prestige_3 -2.016992 -0.663416
# prestige_4 -2.370399 -0.732529
# intercept  -6.224242 -1.755716
```

```python
print np.exp(result.params)
# gre           1.002267
# gpa           2.234545
# prestige_2    0.508931
# prestige_3    0.261792
# prestige_4    0.211938
# intercept     0.018500
```


我们也可以使用置信区间来计算系数的影响，来更好地估计一个变量影响录取率的不确定性。



```python
# odds ratios and 95% CI
params = result.params
conf = result.conf_int()
conf['OR'] = params
conf.columns = ['2.5%', '97.5%', 'OR']
print np.exp(conf)
#                   2.5%     97.5%        OR
# gre           1.000120  1.004418  1.002267
# gpa           1.166122  4.281877  2.234545
# prestige_2    0.273692  0.946358  0.508931
# prestige_3    0.133055  0.515089  0.261792
# prestige_4    0.093443  0.480692  0.211938
# intercept     0.001981  0.172783  0.018500
```





## 更深入的挖掘


为了评估我们分类器的效果，我们将使用每个输入值的逻辑组合（logical combination）来重新创建数据集，如此可以得知在不同的变量下预测录取可能性的增加、减少。首先我们使用名为 `cartesian` 的辅助函数来生成组合值（来源于: [如何使用numpy构建两个数组的组合](http://stackoverflow.com/questions/1208118/using-numpy-to-build-an-array-of-all-combinations-of-two-arrays)）


我们使用 `np.linspace` 创建 “gre” 和 “gpa” 值的一个范围，即从指定的最大、最小值来创建一个线性间隔的值的范围。在本例子中，取已知的最大、最小值。



```python
# 根据最大、最小值生成 GRE、GPA 均匀分布的10个值，而不是生成所有可能的值
gres = np.linspace(data['gre'].min(), data['gre'].max(), 10)
print gres
# array([ 220.        ,  284.44444444,  348.88888889,  413.33333333,
#         477.77777778,  542.22222222,  606.66666667,  671.11111111,
#         735.55555556,  800.        ])
gpas = np.linspace(data['gpa'].min(), data['gpa'].max(), 10)
print gpas
# array([ 2.26      ,  2.45333333,  2.64666667,  2.84      ,  3.03333333,
#         3.22666667,  3.42      ,  3.61333333,  3.80666667,  4.        ])
 
 
# 枚举所有的可能性
combos = pd.DataFrame(cartesian([gres, gpas, [1, 2, 3, 4], [1.]]))
# 重新创建哑变量
combos.columns = ['gre', 'gpa', 'prestige', 'intercept']
dummy_ranks = pd.get_dummies(combos['prestige'], prefix='prestige')
dummy_ranks.columns = ['prestige_1', 'prestige_2', 'prestige_3', 'prestige_4']
 
# 只保留用于预测的列
cols_to_keep = ['gre', 'gpa', 'prestige', 'intercept']
combos = combos[cols_to_keep].join(dummy_ranks.ix[:, 'prestige_2':])
 
# 使用枚举的数据集来做预测
combos['admit_pred'] = result.predict(combos[train_cols])
 
print combos.head()
#    gre       gpa  prestige  intercept  prestige_2  prestige_3  prestige_4  admit_pred
# 0  220  2.260000         1          1           0           0           0    0.157801
# 1  220  2.260000         2          1           1           0           0    0.087056
# 2  220  2.260000         3          1           0           1           0    0.046758
# 3  220  2.260000         4          1           0           0           1    0.038194
# 4  220  2.453333         1          1           0           0           0    0.179574
```






现在我们已生成了预测结果，接着通过画图来呈现结果。我编写了一个名为 `isolate_and_plot` 的辅助函数，可以比较给定的变量与不同的声望等级、组合的平均可能性。为了分离声望和其他变量，我使用了 `pivot_table` 来简单地聚合数据。



```python
def isolate_and_plot(variable):
    # isolate gre and class rank
    grouped = pd.pivot_table(combos, values=['admit_pred'], index=[variable, 'prestige'],
                            aggfunc=np.mean)
 
    # in case you're curious as to what this looks like
    # print grouped.head()
    #                      admit_pred
    # gre        prestige            
    # 220.000000 1           0.282462
    #            2           0.169987
    #            3           0.096544
    #            4           0.079859
    # 284.444444 1           0.311718
 
    # make a plot
    colors = 'rbgyrbgy'
    for col in combos.prestige.unique():
        plt_data = grouped.ix[grouped.index.get_level_values(1)==col]
        pl.plot(plt_data.index.get_level_values(0), plt_data['admit_pred'],
                color=colors[int(col)])
 
    pl.xlabel(variable)
    pl.ylabel("P(admit=1)")
    pl.legend(['1', '2', '3', '4'], loc='upper left', title='Prestige')
    pl.title("Prob(admit=1) isolating " + variable + " and presitge")
    pl.show()
 
isolate_and_plot('gre')
isolate_and_plot('gpa')
```







结果图显示了 gre, gpa 和 prestige 如何影响录取。可以看出，随着 gre 和 gpa 的增加，录取可能性如何逐渐增加，并且，不同的学校声望对录取可能性的增加程度相差很大。

![gre与prestige的关系](http://cdn.powerxing.com/imgs/isolated_rank_and_gre.png)
![gpa与prestige的关系](http://cdn.powerxing.com/imgs/isolated_rank_and_gpa.png)

## 结束语


逻辑回归是用于分类的优秀算法，尽管有一些更加性感的，或是黑盒分类器算法，如SVM和随机森林（RandomForest）在一些情况下性能更好，但深入了解你正在使用的模型是很有价值的。很多时候你可以使用随机森林来筛选模型的特征，并基于筛选出的最佳的特征，使用逻辑回归来重建模型。

## 其他资源
- [UCLA的R教程](http://www.ats.ucla.edu/stat/r/dae/logit.htm)
- [scikit-learn文档](http://scikit-learn.org/stable/modules/linear_model.html#logistic-regression)
- [纯Python实现逻辑回归](http://blog.smellthedata.com/2009/06/python-logistic-regression-with-l2.html)
- [逻辑回归在线交互式例子](http://www.vassarstats.net/logreg1.html)



