# sklearn   joblib 交叉验证函数 cross_val_score - 墨流觞的博客 - CSDN博客





2018年09月26日 22:01:17[墨氲](https://me.csdn.net/dss_dssssd)阅读数：259








`sklearn.model_selection.cross_val_score(estimator, X, y=None, groups=None, scoring=None, cv=’warn’, n_jobs=None, verbose=0, fit_params=None, pre_dispatch=‘2*n_jobs’, scoring=’raise-deprecating’)`

只介绍几个常用的参数：
- estimator 将要训练的模型
- X 训练集
- y 标签
- scoring： string 选择返回的误差函数类型，
- cv : int 默认分成3个folds，指定后分成cv个folds

简单的介绍一下k-folds 交叉验证：

随机将数据集分为k个不同的子集，称为folds， 然后在训练和验证决策树模型k次，每一次选择一个不同的子集作为验证集，在另外k-1个子集上训练。返回结果是结果是一个长度为k 的数组，值为在k个fold上的误差值

```python
```python
def display_scores(scores):
    print("Scores:", scores)
    print("Mean:", scores.mean())
    print("Standard deviation:", scores.std())

from sklearn.ensemble import RandomForestRegressor
forest_reg = RandomForestRegressor()

forest_reg.fit(housing_prepared, housing_labels)

forest_scores = cross_val_score(forest_reg, housing_prepared, housing_labels,
                            scoring="neg_mean_squared_error", cv=10)


housing_predicitions  =forest_reg.predict(housing_prepared)
forest_mse = mean_squared_error(housing_predicitions, housing_labels)
forest_rmse = np.sqrt(forest_mse)
print(forest_rmse)


forest_rmse_scores = np.sqrt(-forest_scores)

display_scores(lin_rmse_scores)
```
```

out:

> 
22056.180234873842

Scores: [66782.73843989 66960.118071   70347.95244419 74739.57052552

68031.13388938 71193.84183426 64969.63056405 68281.61137997

71552.91566558 67665.10082067]

Mean: 69052.46136345083

Standard deviation: 2731.6740017983466
注意效果并不好，并且在训练集的误差要低于验证集上的误差很多，这说明模型仍然存在过拟合问题， 解决方法：
- 简化模型
- 增加正则化等约束
- 增加训练集

当然，应该要尝试更多其他的模型， 支持向量机等，最终确定1到2中潜在的模型

最好将每个试验过的模型保存以便于后面的使用，确保保留了超参数和训练的参数，验证集上的表现分数，甚至预测值。 可以使用pickle或sklearn.externals.joblib,后者在序列化大数组是更高效

用法和pickle相同呢。

```python
```python
from sklearn.externals import joblib
# joblib.dump(my_model, "my_model.pkl")
# # and later...
# my_model_loaded = joblib.load("my_model.pkl")
```
```





