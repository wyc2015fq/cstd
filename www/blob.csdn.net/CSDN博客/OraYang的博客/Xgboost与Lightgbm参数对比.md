
# Xgboost与Lightgbm参数对比 - OraYang的博客 - CSDN博客

2017年07月19日 14:34:30[OraYang](https://me.csdn.net/u010665216)阅读数：4675所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



最近在使用Xgboost与Lightgbm模型来训练数据，众所周知，lightgbm是2016年末微软开源的工具，具体相关信息可以参考一下链接：
[Lightgbm: https://github.com/Microsoft/LightGBM]()
[Xgboost: https://github.com/dmlc/xgboost]()
两大神器的优缺点，咱们就不讨论了，微软官方给了一些example对比，详情请戳：https://github.com/Microsoft/LightGBM/wiki/Experiments

那么既然这个两个工具这么牛掰，在使用的过程中，怎么根据Xgboost的参数来将LightGbm应用到具体应用场景中呢？
本文根据Kaggle竞赛中使用的相关情况，整理总结出相关参数对应关系如下：

参数比对|Xgboots|LightGbm|
|booster(|default=gbtree)|boosting(|default=|gbdt)|
|eta(|default=0.3)|learning_rate(|default=|0.1)|
|max_depth(|default=6)|num_leaves(|default=|31)|
|min_child_weight(|default=1)
|min_sum_hessian_in_leaf(|1e-3)
|
|gamma(|default=0)
|min_gain_to_split(|default=0)
|
|subsample(|default=1)
|bagging_fraction(|default=1.0)
|
|colsample_bytree(|default=1)
|feature_fraction(|default=1.0)
|
|alpha(|default=0)
|lambda_l1|(|default=0)
|
|lambda|(|default=1)
|lambda_l2|(|default=0)|
|objective(|default=reg:linear)|application(default=regression)|
|eval_metric(default according to objective)|metric|
|nthread|num_threads|

待续Xgboost参数调优完全指南附带demo实践的：[传送门](http://blog.csdn.net/u010665216/article/details/78532619)

