# 基于Kaggle的经典AI项目四—特征筛选 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 22:41:31[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：134
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)












### 文章目录
- [1 方差筛选法](#1__29)
- [2 高成对相关性去除](#2__51)
- [3 Filter过滤法（有问题）](#3_Filter_82)
- [4 Wrapper包装法 ( RFE—RandomForestRegressor 回归随机森林 )](#4_Wrapper__RFERandomForestRegressor___99)
- [5 Embedded嵌入法 ( SelectFromModel=RandomForestRegressor )](#5_Embedded__SelectFromModelRandomForestRegressor__128)



开发环境 jupyter notebook 

```python
```python
import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib
from scipy.stats import norm
from scipy import stats

import matplotlib.pyplot as plt
%matplotlib inline

from scipy.stats import skew
from scipy.stats.stats import pearsonr

from IPython.core.interactiveshell import InteractiveShell
InteractiveShell.ast_node_interactivity = "all"
```
```

```python
```python
%run ".基于Kaggle的经典AI项目三—特征转换、衍生.ipynb"      #引入特征转换、衍生的工作  

# 删除数值型中的 ID列、SalePrice_log列
train_num = train.select_dtypes(include=[np.number]).drop(labels=['Id','SalePrice_log'], axis=1)

train_num.head()
print (train_num.shape)
```
```

##### 1 方差筛选法

```python
```python
# 去掉低方差, 没有波动的变量
from sklearn.feature_selection import VarianceThreshold

# 设定的阈值
varthreshold = 0.01                                     
sel_varthres = VarianceThreshold(threshold=varthreshold)
sel_varthres.fit(train_num)

sel_not_varthres_var = train_num.columns[np.logical_not(sel_varthres.get_support())]
sel_varthres_var = train_num.columns[(sel_varthres.get_support())]

# 查看处理好的列名
sel_not_varthres_var 
plt.hist(train_num['Street'])

# 将处理好的数据重命名 并赋值给 train_num 
featurefilter_sel_varthres_var = sel_varthres_var
train_num = train_num[featurefilter_sel_varthres_var]
train_num.shape
```
```

##### 2 高成对相关性去除

```python
```python
corrbase = train_num.corr()                                  # 相关性计算
corr = pd.DataFrame(np.triu(corrbase.values, k=1),           # 保留对角线上方，不会处理重复的"成对相关列"
                    index=corrbase.index, 
                    columns=corrbase.columns)


nonzerolist = np.transpose(np.nonzero((corr[corr>0.7]\
                                       .fillna(0).values)))  # 得到高相关"成对相关列"的索引list


corr_y = train.corr()['SalePrice_log']                      # 与y值得相关性统计


# 通过与y的相关性做筛选的条件，最终结果
corr_high = pd.DataFrame({'corr_column1' : corr.columns[nonzerolist[:, 0]],
              'corr_column1_y': corr_y[corr.columns[nonzerolist[:, 0]]].values,
              'corr_column2' : corr.columns[nonzerolist[:, 1]],
              'corr_column2_y': corr_y[corr.columns[nonzerolist[:, 1]]].values
             }, index=np.arange(nonzerolist.shape[0]))

# 找出来要删除的列名，如果找出保留的的列会不准确，因为可能会在另一个"成对相关列"中被删除
corr_high_delete = corr_high.apply(lambda row: row[0] if row[1]<row[3] else row[2], axis=1)
corr_high_delete.drop_duplicates(inplace=True)

# 将【高成对相关性列】重命名； 在源数据上删除【高成对相关性列】并赋值给 train_num 
featurefilter_corr_high_delete = corr_high_delete
train_num = train_num.drop(labels=featurefilter_corr_high_delete, axis=1)
print (train_num.shape)
```
```

##### 3 Filter过滤法（有问题）

```python
```python
# 相关系数法pearsonr, 或者直接用corr阈值法
from sklearn.feature_selection import SelectPercentile
from scipy.stats import pearsonr
import numpy as np  

sel_percentbest = SelectPercentile(lambda X, Y: 
                                   np.array(map(lambda x:pearsonr(x, Y)[0], X.T)).T, 
                                   percentile=80)
sel_percentbest.fit(train_num, train['SalePrice_log'])

sel_percentbest_var = train_num.columns[sel_percentbest.get_support()] #有问题
featurefilter_sel_percentbest_var = sel_percentbest_var
train_num = train_num[featurefilter_sel_percentbest_var]
print train_num.shape
```
```

##### 4 Wrapper包装法 ( RFE—RandomForestRegressor 回归随机森林 )

```python
```python
from sklearn.feature_selection import RFE
from sklearn.ensemble import RandomForestRegressor
from sklearn.tree import DecisionTreeRegressor
from sklearn.tree import DecisionTreeClassifier

rf = RandomForestRegressor(random_state=30)
sel_rfe = RFE(rf, n_features_to_select=int(train_num.shape[1]*0.8)) #方法参数
sel_rfe.fit(train_num, train['SalePrice_log'])

"""
RFE(estimator=RandomForestRegressor(bootstrap=True, criterion='mse', max_depth=None,
           max_features='auto', max_leaf_nodes=None,
           min_impurity_decrease=0.0, min_impurity_split=None,
           min_samples_leaf=1, min_samples_split=2,
           min_weight_fraction_leaf=0.0, n_estimators=10, n_jobs=1,
           oob_score=False, random_state=30, verbose=0, warm_start=False),
  n_features_to_select=151, step=1, verbose=0)
"""

sel_rfe_var = train_num.columns[sel_rfe.get_support()]             
sel_rfe_var                                                         #列名 

# 将筛选的特征列 添加到原始数据中
featurefilter_sel_rfe_var = sel_rfe_var
train_num = train_num[featurefilter_sel_rfe_var]
print (train_num.shape)
```
```

##### 5 Embedded嵌入法 ( SelectFromModel=RandomForestRegressor )

```python
```python
from sklearn.feature_selection import SelectFromModel
from sklearn.ensemble import RandomForestRegressor

rf = RandomForestRegressor(n_estimators=400, random_state=30)
sel_frommodel = SelectFromModel(rf, threshold='0.5*median')

sel_frommodel.fit(train_num, train['SalePrice_log'])
sel_frommodel_var = train_num.columns[sel_frommodel.get_support()]
sel_frommodel_var 

"""
SelectFromModel(estimator=RandomForestRegressor(bootstrap=True, criterion='mse', max_depth=None,
           max_features='auto', max_leaf_nodes=None,
           min_impurity_decrease=0.0, min_impurity_split=None,
           min_samples_leaf=1, min_samples_split=2,
           min_weight_fraction_leaf=0.0, n_estimators=400, n_jobs=1,
           oob_score=False, random_state=30, verbose=0, warm_start=False),
        norm_order=1, prefit=False, threshold='0.5*median')
"""

featurefilter_sel_frommodel_var = sel_frommodel_var
train_num = train_num[featurefilter_sel_frommodel_var]
train_num['SalePrice_log'] = train['SalePrice_log']
print (train_num.shape)
```
```



