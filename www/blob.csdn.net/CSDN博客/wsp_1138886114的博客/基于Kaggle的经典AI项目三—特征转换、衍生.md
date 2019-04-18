# 基于Kaggle的经典AI项目三—特征转换、衍生 - wsp_1138886114的博客 - CSDN博客





2018年09月22日 18:16:58[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：158











### 文章目录
- [准备工作](#_19)
- [一、特征构造](#_52)
- [1.1 分类型变量—重分组](#11__53)
- [1.2 分类型变量—one-hot编码](#12_onehot_84)
- [2.1 连续型变量—非线性衍生](#21__93)
- [2.2 连续型变量—简单组合](#22__121)
- [2.3 连续型变量—正态转换](#23__139)
- [2.4 连续型变量—无量纲化转换](#24__168)
- [整合处理](#_183)



开发环境 jupyter notebook

```python
```python
import pandas as pd
import numpy as np
import seaborn as sns               #绘制heatmap类似于pyEcharts
import matplotlib
from scipy import stats
from scipy.stats import norm
from scipy.stats import skew

import matplotlib.pyplot as plt
%matplotlib inline


from IPython.core.interactiveshell import InteractiveShell
InteractiveShell.ast_node_interactivity = "all"
```
```

### 准备工作

```python
```python
%run ".基于Kaggle的经典AI项目二—数据清洗.ipynb"           #引入数据清洗的工作 

# 类别性变量区分度计算
def anova(train, categorical, y):
    anv = pd.DataFrame(index=categorical)
    anv['feature'] = categorical
    pvals = []
    for c in categorical:
        samples = []
        for cls in train[c].dropna().unique():
            s = train[train[c] == cls][y].values # 获取类别型变量的值
            samples.append(s)                    # 某特征下不同取值对应的房价组合形成二维列表
        pval = stats.f_oneway(*samples)[1]       # 一元方差分析得到 F，P，要P越小，对方差的影响越大。
        pvals.append(pval)
    anv['pval'] = pvals
    anv['disparity'] = np.log(20*1./anv['pval'].values)/np.log(20)  # 区分度
    return anv.sort_values('pval')

cate_feature = [column for column in train\
                .columns if train.dtypes[column] == 'object']       # 类型变量集合
y = 'SalePrice'
corr_cate = anova(train, cate_feature, y) 

train_ana = pd.DataFrame()                                   # 统计变量的主要信息
train_ana['feature_type'] = train.dtypes                     # 加入变量类型
train_ana['cate_cnt'] = train.apply(lambda x : x.nunique())  # 加入每个类别性变量的取值个数
train_ana['conti_corr'] = train.corr()['SalePrice']          # 加入连续性变量相关度
train_ana['cate_corr'] = corr_cate['disparity']              # 加入类别性变量区分度

# train_ana.sort_values(by = ['feature_type', 'conti_corr', 'cate_corr'], ascending=False)
```
```

### 一、特征构造

#### 1.1 分类型变量—重分组

```python
```python
neighborhood_order = train.groupby('Neighborhood')\
                          .median()\
                          .sort_values(by='SalePrice')\
                          .index 

plt.figure(figsize = (14, 4))
sns.boxplot(x = 'Neighborhood', y = 'SalePrice',
            data = train, order=neighborhood_order)
plt.xticks(rotation=90)                       #x轴字体旋转90度

plt.figure(figsize = (14, 4))
sns.countplot(x = 'Neighborhood', data = train, 
              order=neighborhood_order)
plt.xticks(rotation=90)
```
```

![这里写图片描述](https://img-blog.csdn.net/20180811101115676?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
```python
train["SimpleNeighborhood"] = train.Neighborhood\
                                   .replace({'IDOTRR': 'IDOTRR-BrDale' , 'BrDale': 'IDOTRR-BrDale',
                                             'Blueste': 'Blueste-SWISU', 'SWISU': 'Blueste-SWISU' ,
                                             'NPkVill': 'NPkVill-Mitchel', 'Mitchel': 'NPkVill-Mitchel'
                                            }) 

anova(train, ['Neighborhood', 'SimpleNeighborhood'], y)
```
```
|$~$|feature|pval|disparity|
|----|----|----|----|
|SimpleNeighborhood|SimpleNeighborhood|1.725211e-243|187.593052|
|Neighborhood|Neighborhood|1.019383e-240|185.462820|

#### 1.2 分类型变量—one-hot编码

```python
```python
#获取分类型变量
featurego_cat_column = train.select_dtypes(include = [np.object]).columns 

# 通过one-hot编码创建虚拟特性分类值
train_onehot = pd.get_dummies(train[featurego_cat_column])
train_onehot.columns
```
```

#### 2.1 连续型变量—非线性衍生

```python
```python
#获取连续性变量conti_corr列的值降序排序
train_ana.loc[train_ana.feature_type != 'object', ]\
         .sort_values('conti_corr', ascending=False)
          
train_ana.head()
```
```
|$~$|feature_type|cate_cnt|conti_corr|cate_corr|
|----|----|----|----|----|
|Id|int64|1458|-0.027300|NaN|
|MSSubClass|object|15|NaN|65.931200|
|MSZoning|object|5|NaN|29.838963|
|LotFrontage|float64|111|0.209700|NaN|
|LotArea|float64|1039|0.421355|NaN|

```python
```python
# 非线性构造
train["OverallQual-s2"] = train["OverallQual"] ** 2
train["OverallQual-s3"] = train["OverallQual"] ** 3
train["OverallQual-Sq"] = np.sqrt(train["OverallQual"])
train["GrLivArea-2"] = train["GrLivArea"] ** 2
train["GrLivArea-3"] = train["GrLivArea"] ** 3
train["GrLivArea-Sq"] = np.sqrt(train["GrLivArea"]) 

train[['SalePrice', 'OverallQual', 'OverallQual-s2', 'OverallQual-s3', 'OverallQual-Sq',
      'GrLivArea', 'GrLivArea-2', 'GrLivArea-3', 'GrLivArea-Sq']].corr()['SalePrice']
```
```

#### 2.2 连续型变量—简单组合

```python
```python
# Total number of bathrooms
train["TotalBath"] = train["BsmtFullBath"] + (0.5 * train["BsmtHalfBath"]) + \
                     train["FullBath"] + (0.5 * train["HalfBath"]) 

# Total SF for house (incl. basement)
train["AllSF"] = train["GrLivArea"] + train["TotalBsmtSF"] 

# Total SF for 1st + 2nd floors
train["AllFlrsSF"] = train["1stFlrSF"] + train["2ndFlrSF"] 

# Total SF for porch
train["AllPorchSF"] = train["OpenPorchSF"] + train["EnclosedPorch"] + \
                      train["3SsnPorch"] + train["ScreenPorch"] 

train[['TotalBath', 'AllSF', 'AllFlrsSF', 'AllPorchSF', 'SalePrice']].corr()['SalePrice']
```
```

#### 2.3 连续型变量—正态转换

```python
```python
# y值正态变换(查看房价分布)
sns.distplot(train['SalePrice'], fit=norm) 

# 利用Q-Q图判断数据是否偏离正态分布
train['SalePrice_log'] = np.log1p(train['SalePrice'])    #取对数
stats.probplot(train['SalePrice'], plot=plt) 

# 统计train数据集的相关系数（pearson）（只显示['SalePrice','SalePrice_log']列）显示前五行
train.corr().sort_values('SalePrice_log', ascending=False)\
             [['SalePrice', 'SalePrice_log']].head()
```
```

![这里写图片描述](https://img-blog.csdn.net/20180811110634122?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
```python
# 连续型特征正态变换

# 对数值特性进行变换，来减少倾斜异常值的影响
# 一般的经验法则,绝对偏态值> 0.75被认为是倾斜严重
train_num = train.select_dtypes(include=[np.number])\
                 .drop(['SalePrice', 'SalePrice_log'], axis=1) 
                 
skewness = train_num.apply(lambda x: skew(x.dropna()))
skewness = skewness[abs(skewness) > 0.75]
skewness
featurego_skewed_features = skewness.index 

train[featurego_skewed_features] = np.log1p(train[featurego_skewed_features])
```
```

#### 2.4 连续型变量—无量纲化转换

```python
```python
from sklearn.preprocessing import MinMaxScaler    #用于区间缩放
train_num = train.select_dtypes(include=[np.number]).drop(['SalePrice', 'SalePrice_log'], axis=1)
featurego_min_max_scaler = MinMaxScaler()
featurego_min_max_scaler.fit_transform(train_num)

featurego_scaler_numcolumn = train_num.columns

train_num_minmax = np.round(featurego_min_max_scaler\
                            .transform(train[featurego_scaler_numcolumn]), 2)    #四舍五入
train_num_minmax= pd.DataFrame(train_num_minmax, 
                               columns=featurego_scaler_numcolumn + '_minmax',   #重命名
                               index=train.index)
```
```

### 整合处理

```python
```python
# 将处理好的数据 以下三个DataFrame 按列拼接。
train = pd.concat([train, train_onehot, train_num_minmax], axis=1) 

train.drop(['SalePrice'], axis=1, inplace=True)
```
```



