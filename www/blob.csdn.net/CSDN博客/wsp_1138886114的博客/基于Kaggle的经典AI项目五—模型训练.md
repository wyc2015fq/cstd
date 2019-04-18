# 基于Kaggle的经典AI项目五—模型训练 - wsp_1138886114的博客 - CSDN博客





2018年09月22日 17:50:01[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：143











### 文章目录
- [1 决策树回归DecisionTreeRegressor](#1_DecisionTreeRegressor_13)
- [2 岭回归Ridge](#2_Ridge_53)
- [3 弹性网回归ElasticNet](#3_ElasticNet_97)
- [4 算法融合](#4__179)



开发环境 jupyter notebook

```python
```python
%run "基于Kaggle的经典AI项目四—特征筛选.ipynb"

train_y = train_num['SalePrice_log']
train_X = train_num.drop('SalePrice_log', axis=1)

model_column = train_X.columns
train_X.shape                          # 查看数据集
model_column                           # 查看数据集
```
```

##### 1 决策树回归DecisionTreeRegressor

```python
```python
from sklearn.tree import DecisionTreeRegressor
from sklearn.grid_search import GridSearchCV

# 参数优化 - GridSearchCV
# 交叉检验 - GridSearchCV
tuned_parameters = {"criterion": ["mse"],
              "min_samples_split": [2, 10, 20],
              "max_depth": [2, 10, 20, 40],
              "min_samples_leaf": [1, 5, 10],
              "max_leaf_nodes": [5, 10, 20, 40],
              }

clf = DecisionTreeRegressor()
clf = GridSearchCV(clf, tuned_parameters, cv=5)
clf.fit(train_X, train_y)

clf.best_params_
for params, mean_score, scores in clf.grid_scores_:
    print("%0.3f (+/-%0.03f) for %r"
          % (mean_score, scores.std() * 2, params))

# 模型结果展示   变量重要性显示
important_features = pd.Series(data=clf.best_estimator_.feature_importances_, 
                               index=train_X.columns).sort_values(ascending=False)
important_features = important_features[important_features>0]

plt.figure(figsize=(20,10))
important_features.plot(kind='bar')

# 效果评估
from sklearn.metrics import mean_squared_error

pred_y = clf.predict(train_X)
np.sqrt(mean_squared_error(np.expm1(train_y),  np.expm1(pred_y)))
plt.figure(figsize=(20,10))
plt.scatter(x=np.expm1(train_y), y=np.expm1(pred_y))
```
```

![这里写图片描述](https://img-blog.csdn.net/20180821190607451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 2 岭回归Ridge

```python
```python
from sklearn.linear_model import Ridge
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import KFold

# 参数优化 - for
# 交叉检验 - KFold
n_folds = 5
def rmse_cv(model):
    rmse= np.sqrt(-cross_val_score(model, 
                                   train_X, train_y, 
                                   scoring="neg_mean_squared_error", 
                                   cv = KFold(n_folds, shuffle=True, random_state=42)))
    return(rmse)

alphas = [0.05, 0.1, 0.3, 1, 3, 5]
cv_ridge = [rmse_cv(Ridge(alpha = alpha)).mean() 
            for alpha in alphas]

# score.std()
cv_ridge = pd.Series(cv_ridge, index = alphas)
cv_ridge.plot(title = "Validation - LassoCV")
plt.xlabel("alpha")
plt.ylabel("rmse")

ridge = Ridge(alpha = 1)
ridge.fit(train_X, train_y)

"""
Ridge(alpha=1, copy_X=True, 
     fit_intercept=True, max_iter=None,
     normalize=False, random_state=None, 
     solver='auto', tol=0.001)
"""

# 模型结果展示
# 变量重要性显示
important_features = pd.Series(data=ridge.coef_, index=train_X.columns).sort_values(ascending=False)
important_features = important_features[np.abs(important_features)>0.01]
plt.figure(figsize=(20,10))
important_features.plot(kind='bar')
```
```

![这里写图片描述](https://img-blog.csdn.net/20180821190617664?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 3 弹性网回归ElasticNet

```python
```python
# 参数优化
# 交叉检验
from sklearn.linear_model import ElasticNetCV

elasticNet = ElasticNetCV(l1_ratio = [0.1, 0.3, 0.5, 0.6, 0.7, 0.8, 0.85, 0.9, 0.95, 1],
                          alphas = [0.0001, 0.0003, 0.0006, 0.001, 0.003, 0.006, 
                                    0.01, 0.03, 0.06, 0.1, 0.3, 0.6, 1, 3, 6], 
                          max_iter = 50000, cv = 10)
elasticNet.fit(train_X, train_y)
alpha = elasticNet.alpha_
ratio = elasticNet.l1_ratio_
print("Best l1_ratio :", ratio)
print("Best alpha :", alpha )

print("Try again for more precision with l1_ratio centered around " + str(ratio))
elasticNet = ElasticNetCV(l1_ratio = [ratio * .85, ratio * .9, 
                                      ratio * .95, ratio, 
                                      ratio * 1.05, ratio * 1.1, 
                                      ratio * 1.15],
                          alphas = [0.0001, 0.0003, 0.0006, 0.001, 
                                    0.003, 0.006, 0.01, 0.03, 0.06, 
                                    0.1, 0.3, 0.6, 1, 3, 6], 
                          max_iter = 50000, cv = 10)
elasticNet.fit(train_X, train_y)
if (elasticNet.l1_ratio_ > 1):
    elasticNet.l1_ratio_ = 1    
alpha = elasticNet.alpha_
ratio = elasticNet.l1_ratio_
print("Best l1_ratio :", ratio)
print("Best alpha :", alpha )

print("Now try again for more precision on alpha, with l1_ratio fixed at " + str(ratio) + 
      " and alpha centered around " + str(alpha))
elasticNet = ElasticNetCV(l1_ratio = ratio,
                          alphas = [alpha * .6, alpha * .65, 
                                    alpha * .7, alpha * .75, 
                                    alpha * .8, alpha * .85,
                                    alpha * .9, alpha * .95, 
                                    alpha, alpha * 1.05, alpha * 1.1, 
                                    alpha * 1.15, alpha * 1.25, alpha * 1.3, 
                                    alpha * 1.35, alpha * 1.4], 
                          max_iter = 50000, cv = 10)
elasticNet.fit(train_X, train_y)
if (elasticNet.l1_ratio_ > 1):
    elasticNet.l1_ratio_ = 1    
alpha = elasticNet.alpha_
ratio = elasticNet.l1_ratio_
print("Best l1_ratio :", ratio)
print("Best alpha :", alpha )

# 模型稳定性
score = rmse_cv(elasticNet)
print(" Averaged base models score: {:.4f} ({:.4f})\n".format(score.mean(), score.std()))
# Averaged base models score: 0.1199 (0.0098)

# 模型结果展示
# 变量重要性显示
important_features = pd.Series(data=elasticNet.coef_, 
                               index=train_X.columns).sort_values(ascending=False)
important_features = important_features[np.abs(important_features)>0.01]
plt.figure(figsize=(20,10))
important_features.plot(kind='bar')

# 效果评估
from sklearn.metrics import mean_squared_error

pred_y = elasticNet.predict(train_X)
np.sqrt(mean_squared_error(np.expm1(train_y),  np.expm1(pred_y)))
# 19977.375075754539

# 效果评估展示(左图)
plt.scatter(pred_y, pred_y - train_y, c = "blue", marker = "s", label = "Training data")
plt.hlines(y = 0, xmin = 10.5, xmax = 13.5, color = "red")


# Plot predictions（右图）
plt.scatter(pred_y, train_y, c = "blue", marker = "s", label = "Training data")
plt.plot([10.5, 13.5], [10.5, 13.5], c = "red")
```
```

![这里写图片描述](https://img-blog.csdn.net/20180821191222112?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 4 算法融合

```python
```python
from sklearn.base import BaseEstimator
from sklearn.base import RegressorMixin
from sklearn.base import TransformerMixin
from sklearn.metrics import mean_squared_error
from sklearn.base import clone
from scipy.stats import pearsonr

class AveragingModels(BaseEstimator, RegressorMixin, TransformerMixin):
    def __init__(self, models):
        self.models = models
        
    # we define clones of the original models to fit the data in
    def fit(self, X, y):
        self.models_ = [clone(x) for x in self.models]
        
        # Train cloned base models
        for model in self.models_:
            model.fit(X, y)

        return self
    
    #Now we do the predictions for cloned models and average them
    def predict(self, X):
        predictions = np.column_stack([
            model.predict(X) for model in self.models_
        ])
        return np.mean(predictions, axis=1)  

# 模型相关性分析
pred_y_ridge = ridge.predict(train_X)
pred_y_elasticNet = elasticNet.predict(train_X)
pred_y_clf = clf.predict(train_X)

pearsonr(pred_y_ridge.T, pred_y_elasticNet.T)
pearsonr(pred_y_clf.T, pred_y_elasticNet.T)

# 模型训练
averaged_models = AveragingModels(models = (ridge, elasticNet))
averaged_models.fit(train_X, train_y)

# 交叉验证
n_folds = 5
def rmse_cv(model):
    rmse= np.sqrt(-cross_val_score(model, train_X, train_y, scoring="neg_mean_squared_error", 
                                   cv = KFold(n_folds, shuffle=True, random_state=42)))
    return(rmse)
score = rmse_cv(averaged_models)
print(" Averaged base models score: {:.4f} ({:.4f})\n".format(score.mean(), score.std()))
#  相关性未去除 ：Averaged base models score: 0.1124 (0.0090)
#  单独 elasticNet ：Averaged base models score: 0.1180 (0.0081)

# 效果评估
pred_y = averaged_models.predict(train_X)
np.sqrt(mean_squared_error(np.expm1(train_y),  np.expm1(pred_y)))
#  相关性未去除 ：19865.234119524695
#  单独 elasticNet 20128.678118486128

# 效果评估展示(左图)
plt.scatter(pred_y, pred_y - train_y, c = "blue", marker = "s", label = "Training data")
plt.hlines(y = 0, xmin = 10.5, xmax = 13.5, color = "red")

# Plot predictions（右图）
plt.scatter(pred_y, train_y, c = "blue", marker = "s", label = "Training data")
plt.plot([10.5, 13.5], [10.5, 13.5], c = "red") 

#最终模型
model_ridge = ridge
model_elasticNet = elasticNet
model_averaged_models = averaged_models
```
```

![这里写图片描述](https://img-blog.csdn.net/2018082119170290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



