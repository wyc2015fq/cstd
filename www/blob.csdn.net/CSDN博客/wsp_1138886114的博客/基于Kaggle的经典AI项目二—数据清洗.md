# 基于Kaggle的经典AI项目二—数据清洗 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 22:27:38[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：261











### 文章目录
- [1 数据类型 修改](#1___24)
- [2 重复样本处理— 删除重复样本](#2___66)
- [3 缺失值处理](#3__71)
- [3.1 行列--缺失值>40%删除处理](#31_40_72)
- [3.2 统一填充 缺失率少于1%的列](#32__1_135)
- [3.3 相关性高的 连续型变量业务填充](#33___157)
- [3.4 区分度高的分类型变量业务填充](#34__173)
- [3.5 统一填充剩余变量](#35__186)
- [4 连续型变量奇异值处理](#4__217)
- [4.1 y变量处理--重点变量处理](#41_y_262)
- [4.2 其余变量统一处理](#42__280)



开发环境jupyter notebook

```python
```python
import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib
from scipy import stats

import matplotlib.pyplot as plt
%matplotlib inline

from IPython.core.interactiveshell import InteractiveShell
InteractiveShell.ast_node_interactivity = "all"          

import warnings
warnings.filterwarnings("ignore")                        #忽略警告

train = pd.read_csv("train.csv")
evaluate = pd.read_csv("test.csv")
evaluate.head()                                          #查看
evaluate.info()                                          #查看数据信息
```
```

#### 1 数据类型 修改

```python
```python
clear_dtypes = train.drop('SalePrice', axis=1).dtypes    #修改数据类型
train['GarageQual'].value_counts()
train.dtypes['GarageQual'] 

# 有序型变量转换 
train = train.replace({    
                       "BsmtCond" : {"No": 0, "Po" : 1, "Fa" : 2, "TA" : 3, "Gd" : 4, "Ex" : 5},
                       "BsmtExposure" : {"No" : 0, "Mn" : 1, "Av": 2, "Gd" : 3},
                       "BsmtFinType1" : {"No" : 0, "Unf" : 1, "LwQ": 2, "Rec" : 3, "BLQ" : 4,
                                         "ALQ" : 5, "GLQ" : 6},
                       "BsmtFinType2" : {"No" : 0, "Unf" : 1, "LwQ": 2, "Rec" : 3, "BLQ" : 4,
                                         "ALQ" : 5, "GLQ" : 6},
                       "BsmtQual" : {"No" : 0, "Po" : 1, "Fa" : 2, "TA": 3, "Gd" : 4, "Ex" : 5},
                       "ExterCond" : {"Po" : 1, "Fa" : 2, "TA": 3, "Gd": 4, "Ex" : 5},
                       "ExterQual" : {"Po" : 1, "Fa" : 2, "TA": 3, "Gd": 4, "Ex" : 5},
                       "FireplaceQu" : {"No" : 0, "Po" : 1, "Fa" : 2, "TA" : 3, "Gd" : 4, "Ex" : 5},
                       "Functional" : {"Sal" : 1, "Sev" : 2, "Maj2" : 3, "Maj1" : 4, "Mod": 5,
                                       "Min2" : 6, "Min1" : 7, "Typ" : 8},
                       "GarageCond" : {"No" : 0, "Po" : 1, "Fa" : 2, "TA" : 3, "Gd" : 4, "Ex" : 5},
                       "GarageQual" : {"No" : 0, "Po" : 1, "Fa" : 2, "TA" : 3, "Gd" : 4, "Ex" : 5},
                       "HeatingQC" : {"Po" : 1, "Fa" : 2, "TA" : 3, "Gd" : 4, "Ex" : 5},
                       "KitchenQual" : {"Po" : 1, "Fa" : 2, "TA" : 3, "Gd" : 4, "Ex" : 5},
                       "LandSlope" : {"Sev" : 1, "Mod" : 2, "Gtl" : 3},
                       "LotShape" : {"IR3" : 1, "IR2" : 2, "IR1" : 3, "Reg" : 4},
                       "PavedDrive" : {"N" : 0, "P" : 1, "Y" : 2},
                       "PoolQC" : {"No" : 0, "Fa" : 1, "TA" : 2, "Gd" : 3, "Ex" : 4},
                       "Street" : {"Grvl" : 1, "Pave" : 2},
                       "Utilities" : {"ELO" : 1, "NoSeWa" : 2, "NoSewr" : 3, "AllPub" : 4}
                      })  

# 查看数据类型并修改
train['MSSubClass'].dtypes                              # MSSubClass： 出售的房屋类型：分类型
train['YrSold'].dtypes                                  # 房子的出售年月
train['MoSold'].dtypes 

train['MSSubClass'] = train['MSSubClass'].astype(str)   # 修改数据类型
train['YearBuilt'] = train['YearBuilt'].astype(str)
train['YrSold'] = train['YrSold'].astype(str)
train['MoSold'] = train['MoSold'].astype(str)
```
```

#### 2 重复样本处理— 删除重复样本

```python
```python
train.duplicated().sum()                                # 统计重复样本个数 
train.drop_duplicates(inplace=True)                     # 删除重复样本
```
```

#### 3 缺失值处理

###### 3.1 行列–缺失值>40%删除处理

```python
```python
# 行缺失处理--删除缺失率>40%的行

(train.isnull().sum(axis=1)/train.shape[1] > 0.4).sum() # 统计缺失率>40%的行数;train.shape[1]:所有列76列

nans_del_indexs = train.index[(train.isnull()\          # 删除40%的数据都缺失的行
                       .sum(axis=1)/train.shape[1] > 0.4)]
train.drop(labels=nans_del_indexs, axis=0, inplace=True)
```
```

```python
```python
# 列缺失处理 

def anova(train_cate_y, categorical, y):                        # 类别性变量区分度计算
    anv = pd.DataFrame(index=categorical)
    anv['feature'] = categorical
    pvals = []
    for c in categorical:
        print(c)
        samples = []
        for cls in train_cate_y[c].dropna().unique():           # 删除每一列数据 Na 值，去重
            s = train_cate_y[train_cate_y[c] == cls][y].values  # 获取处理好列的 SalePrice值
            samples.append(s)                                   # 某特征下不同取值对应的房价组合形成二维列表
        pval = stats.f_oneway(*samples)[1]                      # 一元方差分析得到F，P：P越小，对方差的影响越大。
        pvals.append(pval)
    anv['pval'] = pvals
    return anv.sort_values('pval')

categorical = [column for column in train\
               .columns if train.dtypes[column] == 'object']    # 类型变量集合
y = 'SalePrice'
corr_cate = anova(train, categorical, y)
corr_cate['disparity'] = np.log(20*1./corr_cate['pval']\
                                .values)/np.log(20)             # 区分度 

# 统计每个列缺失值比例:
NAs = pd.concat([(100*train.isnull().sum(axis=0)/train.shape[0]),
                 100*evaluate.isnull().sum()/evaluate.shape[0]], 
                 axis=1, keys=['Train', 'evaluate'])
NAs['type'] = train.dtypes[NAs.index]

NAs['corr_conti_y'] = train.corr()['SalePrice']                 # 加入连续性变量相关度
NAs['corr_cate_y'] = corr_cate['disparity']                     # 加入类别性变量区分度

NAs = NAs[NAs[['Train', 'evaluate']].sum(axis=1) > 0]\          # 只保留有缺失值的列
      .sort_values(by='Train', ascending=False)
NAs.head()
```
```
|$~$|**Train**|**evaluate**|**type**|**corr_conti_y**|**corr_cate_y**|
|----|----|----|----|----|----|
|**PoolQC**|99.520548|99.794380|float64|0.447936|NaN|
|**MiscFeature**|96.301370|96.504455|object|NaN|1.753202|
|**Alley**|93.767123|92.666210|object|NaN|6.618486|
|**Fence**|80.753425|80.123372|object|NaN|3.026003|
|**FireplaceQu**|47.260274|50.034270|float64|0.295794|NaN|

```python
```python
# 删除缺失率超过40%的列 

clear_nans_del_cols = NAs.index[((NAs[['Train', 'evaluate']]>40).sum(axis=1)>0).values]
clear_nans_del_cols 

train.drop(labels=clear_nans_del_cols, axis=1, inplace=True)
```
```

###### 3.2 统一填充 缺失率少于1%的列
- 连续型用中位数
- 分类型用众数

```python
```python
# 查找 数值型 类别型列

clear_nans_less1_cols = NAs.index[((NAs[['Train', 'evaluate']]<1).sum(axis=1)==2).values].values
clear_nans_less1_cols_num = train[clear_nans_less1_cols].select_dtypes(include=[np.number]).columns
clear_nans_less1_cols_class = train[clear_nans_less1_cols].select_dtypes(include=[np.object]).columns 

# 生成中位数、众数待用
clear_nans_less1_cols_num_median = train[clear_nans_less1_cols_num].median()
clear_nans_less1_cols_class_mode = train[clear_nans_less1_cols_class].mode() 

#  连续型用 中位数填充
#  分类型用 众数填充
train[clear_nans_less1_cols_num] = train[clear_nans_less1_cols_num]\
                                   .fillna(clear_nans_less1_cols_num_median)
train[clear_nans_less1_cols_class] = train[clear_nans_less1_cols_class]\
                                   .fillna(clear_nans_less1_cols_class_mode)
```
```

###### 3.3 相关性高的 连续型变量业务填充

```python
```python
""" 
	GarageYrBlt 车库建成日期 GarageYrBlt:0.055479,0.053425  float64	0.486362
	车库的五个变量GarageType、GarageYrBlt、GarageFinish、GarageQual、GarageCond是由于房子没有车库而缺失。
	GarageYrBlt 为车库的年份，我们用房子的建造年份来替代 

	MasVnrArea 外墙装饰材料的面积，Na即面积为0 MasVnrArea,0.005479,0.010274,float64,0.477493
	LotFrontage 房子与街道的距离，Na即距离为0 LotFrontage,0.177397,0.155479,float64,0.351799
"""
train.loc[train['GarageYrBlt'].isnull(), 'GarageYrBlt'] = train.loc[train['GarageYrBlt']\
                                                                    .isnull(), 'YearBuilt']

train['MasVnrArea'] = train['MasVnrArea'].fillna(0)
train['LotFrontage'] = train['LotFrontage'].fillna(0)
```
```

###### 3.4 区分度高的分类型变量业务填充

```python
```python
"""
	BsmtQual 地下室质量  BsmtQual,0.025342,0.030137,object,NaN,143.208288
	BsmtExposure、BsmtFinType2、BsmtQual、BsmtCond、BsmtFinType1地下室变量，房子没有地下室而缺失。 
	KitchenQual: String,  //厨房质量, 这里用众数替换
""" 

clear_KitchenQual_mode = train['KitchenQual'].mode()               #获取众数

train['BsmtQual'].fillna('None', inplace=True)
train['KitchenQual'].fillna(clear_KitchenQual_mode, inplace=True)  #使用众数填充
```
```

###### 3.5 统一填充剩余变量
- 连续型用 0
- 分类型用 None

```python
```python
# 查看剩余变量情况
remain_col = train.columns[train.isnull().sum(axis=0)>0]
NAs.loc[remain_col, ]
```
```
|$~$|**Train**|**evaluate**|**type**|**corr_conti_y**|**corr_cate_y**|
|----|----|----|----|----|----|
|**MasVnrType**|0.547945|1.096642|object|NaN|50.437263|
|**BsmtCond**|2.534247|3.084304|float64|0.160658|NaN|
|**BsmtExposure**|2.602740|3.015764|float64|0.352958|NaN|
|**BsmtFinType1**|2.534247|2.878684|float64|0.277436|NaN|
|**BsmtFinType2**|2.602740|2.878684|float64|-0.040139|NaN|
|**Electrical**|0.068493|0.000000|object|NaN|14.665360|
|**GarageType**|5.547945|5.209047|object|NaN|51.655312|
|**GarageFinish**|5.547945|5.346127|object|NaN|72.421212|
|**GarageQual**|5.547945|5.346127|float64|0.156693|NaN|
|**GarageCond**|5.547945|5.346127|float64|0.125013|NaN|

```python
```python
# 查找：数值型列；类别型列对其进行 0、None 填充
clear_nans_remain_cols_num = train[remain_col].select_dtypes(include=[np.number]).columns
clear_nans_remain_cols_class = train[remain_col].select_dtypes(include=[np.object]).columns

train[clear_nans_remain_cols_num] = train[clear_nans_remain_cols_num].fillna(0)
train[clear_nans_remain_cols_class] = train[clear_nans_remain_cols_class].fillna('None') 

# 查看缺失值是否处理完毕
train.isnull().sum(axis=0).sum()
```
```

#### 4 连续型变量奇异值处理

```python
```python
train.SalePrice.plot(kind='box',sym='b*')                # y变量箱线图 

# 查看与y变量 高相关性的变量的箱线图
train_number = train.select_dtypes(include = [np.number]).drop(['Id'], axis=1) 

corr = train.corr()
corr_threshvalue = 0.5                                   #number of variables for heatmap 
corr_cols = corr.loc[:, corr.loc['SalePrice',:]\
                     .abs() > corr_threshvalue].columns  #查找房价大于阈值的列 

train_number[corr_cols].plot(sym='b*', kind='box', subplots=True, figsize=(16, 4))
```
```

![这里写图片描述](https://img-blog.csdn.net/20180809193549504?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
```python
# 找到异常值上下临界点
clear_number_q = train_number.quantile(q=[0, 0.05, 0.25, 0.5, 0.75, 0.95, 1], axis=0) 

# outlier_expand（离群拓展）为异常值定义伸缩参数，标准为1.5
lower_outlier_expand = 1.5          
upper_outlier_expand = 3
clear_number_q.loc['lower_outlier',:] = clear_number_q.loc[0.25, :]\
                                        - (clear_number_q.loc[0.75, :]\
                                        - clear_number_q.loc[0.25, :])*lower_outlier_expand
clear_number_q.loc['upper_outlier',:] = clear_number_q.loc[0.75, :]\
                                        + (clear_number_q.loc[0.75, :]\
                                        - clear_number_q.loc[0.25, :])*upper_outlier_expand 

# 统计各个列 高于上临界点的数量
uppper_cnt = (train_number > clear_number_q.loc['upper_outlier', :]).sum()
uppper_cnt 

# 统计各个列 低于下临界点的数量
lower_cnt = (train_number < clear_number_q.loc['lower_outlier', :]).sum()
lower_cnt 

# 汇总各个列的奇异值数量
pd.DataFrame({'uppper_cnt': uppper_cnt, 
              'lower_cnt': lower_cnt, 
              'all': uppper_cnt+lower_cnt,  
              'all_rate': 100*(uppper_cnt+lower_cnt)/train.shape[0],
              'corr': corr.loc['SalePrice',:]
             }).sort_values(by='corr', ascending=False)
```
```

###### 4.1 y变量处理–重点变量处理

```python
```python
# 获取超出阈值（上临界，下临界）的SalePrice
train.ix[train['SalePrice'] > clear_number_q.loc['upper_outlier', 'SalePrice'], 'SalePrice'] = clear_number_q.loc['upper_outlier', 'SalePrice']
train.ix[train['SalePrice'] < clear_number_q.loc['lower_outlier', 'SalePrice'], 'SalePrice'] = clear_number_q.loc['lower_outlier', 'SalePrice'] 

# 双变量分析SaleSt/ GraveStand——重点变量
var = 'GrLivArea'
data = pd.concat([train['SalePrice'], train[var]], axis=1)
data.plot.scatter(x=var, y='SalePrice', ylim=(0,800000)); 

#deleting points 删除点
train.sort_values(by = 'GrLivArea', ascending = False)[:2] 

#删除 ID=1299/524 的行索引
train = train.drop(train[train['Id'] == 1299].index)
train = train.drop(train[train['Id'] == 524].index)
```
```

###### 4.2 其余变量统一处理

```python
```python
clear_number_para = train_number.drop('SalePrice', axis=1).columns 

train[clear_number_para] = train[clear_number_para]\
                           .where(train[clear_number_para] < clear_number_q.loc['upper_outlier', :], 
                                  clear_number_q.loc['upper_outlier', :], axis=1)

train[clear_number_para] = train[clear_number_para]\
                           .where(train[clear_number_para] > clear_number_q.loc['lower_outlier', :], 
                                  clear_number_q.loc['lower_outlier', :], axis=1) 

train[corr_cols].plot(sym='b*', kind='box', subplots=True, figsize=(20, 8))
```
```

![这里写图片描述](https://img-blog.csdn.net/20180810090830248?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



