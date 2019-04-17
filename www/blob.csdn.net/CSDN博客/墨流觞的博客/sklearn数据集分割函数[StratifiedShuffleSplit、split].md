# sklearn数据集分割函数[StratifiedShuffleSplit、split] - 墨流觞的博客 - CSDN博客





2018年09月22日 23:06:05[墨氲](https://me.csdn.net/dss_dssssd)阅读数：755







- sklearn.model_selection.StratifiedShuffleSplit(n_splits=10, test_size=’default’, train_size=None, random_state=None)

将数据集划分为几组数据集/测试集。
- split(X, y)

按照y的值将数据集分为训练集或测试集，保证训练集和测试集中各类y值所占的比例与原数据集相同。

考虑一个场景，房屋数据集中认为地区收入在房价中占很重要的作用，想分开的训练集和测试集中各种收入所占的比重与原数据集中所占的比重相同。

代码实现：

```python
from sklearn.model_selection import StratifiedShuffleSplit
    split = StratifiedShuffleSplit(n_splits=1, test_size=0.2, random_state=42)
    for train_index, test_index in split.split(housing, housing["income_cat"]):
        strat_train_set = housing.loc[train_index]
        strat_test_set = housing.loc[test_index]
```

通过例子解释一下：

假设`housing["income_cat"]=[1,1,1,1,1,2,2,22,2,2,2,2,2,2]`也就是4个1，10个2，1所占的比例为1/3，2占的比例为2/3；

则经过`split.split(housing, housing["income_cat"])`后，strat_train_set [“income_cat”]和strat_test_set[“income_cat”]中1和2，所占的比例相同，分别为1/2，和2/3.也就是说，strat_train_set [“income_cat”]中有4个1，8个2；而strat_test_set[“income_cat”]中有1个1，2个2



