# 【Lightgbm】AttributeError: 'LGBMRegressor' object has no attribute 'best_iteration' - CD's Coding - CSDN博客





2018年03月21日 11:15:23[糖果天王](https://me.csdn.net/okcd00)阅读数：2197








## 0x00 情景复现

使用 `lightgbm` 进行简单便捷的fit操作，尝试使用early_stopping， 

以选择最好的一次迭代进行预测时，调用`best_iteration`时报错：

```
import lightgbm as lgb
gbm = lgb.LGBMRegressor(objective='binary',
                        num_leaves=64,
                        learning_rate=0.01,
                        n_estimators=2000)

gbm.fit(X_train, y_train,
        eval_set=[(X_val, y_val)],  # 对于需要best_iteration的，必须存在验证集
        eval_metric='binary_logloss',
        early_stopping_rounds=50)  # 对于需要best_iteration的，可以设置若连续多少轮无优化提前停止

gbm.predict(X_test, num_iteration=gbm.best_iteration)

# 出现报错
---------------------------------------------------------------------------
AttributeError                            Traceback (most recent call last)
<ipython-input-12-0e54b2141afb> in <module>()
      1 # gbm.best_iteration_
----> 2 gbm.predict(X_test, num_iteration=gbm.best_iteration)

AttributeError: 'LGBMRegressor' object has no attribute 'best_iteration'
```

## 0x01 查阅资料

通过查阅[API文档](http://lightgbm.readthedocs.io/en/latest/Python-API.html#lightgbm.LGBMRegressor)可知： 
`class lightgbm.LGBMRegressor`包括以下Attributes：
- 
`n_features_`

int – The number of features of fitted model.

- 
`classes_`

array of shape = [n_classes] – The class label array (only for classification problem).

- 
`n_classes_`

int – The number of classes (only for classification problem).

- 
`best_score_`

dict or None – The best score of fitted model.

- 
`best_iteration_`**<<< HERE **

int or None – The best iteration of fitted model if early_stopping_rounds has been specified.

- 
`objective_`

string or callable – The concrete objective used while fitting this model.

- 
`booster_`

Booster – The underlying Booster of this model.

- 
`evals_result_`

dict or None – The evaluation results if early_stopping_rounds has been specified.

- 
`feature_importances_`

array of shape = [n_features] – The feature importances (the higher, the more important the feature).


## 0x02 解决方案

```
# 加个下划线即可
# print(gbm.best_iteration)  # Error
print (gbm.best_iteration_)  # -> 707

# gbm.predict(X_test, num_iteration=gbm.best_iteration)  # Error
gbm.predict(X_test, num_iteration=gbm.best_iteration_)  # Correct
```



