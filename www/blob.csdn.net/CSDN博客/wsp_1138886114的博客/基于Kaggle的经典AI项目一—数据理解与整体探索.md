# 基于Kaggle的经典AI项目一—数据理解与整体探索 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 22:24:43[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：115











### 文章目录
- [1 数据整体理解](#1__1)
- [2 数据探索](#2__28)
- [2.1 因变量分布探索——分析 “房价”](#21___29)
- [2.2 数值型自变量探索](#22__38)
- [I ：相关系数矩阵 （图如下）](#I___39)
- [II ：saleprice高相关变量 相关系数矩阵 (图如上)](#II_saleprice___49)
- [附：字段描述](#_67)




### 1 数据整体理解

数据获取地址：[https://www.kaggle.com/c/house-prices-advanced-regression-techniques/data](https://www.kaggle.com/c/house-prices-advanced-regression-techniques/data)

或者下载地址：[https://download.csdn.net/download/wsp_1138886114/10593880](https://download.csdn.net/download/wsp_1138886114/10593880)

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
train = pd.read_csv("train.csv")            #导入数据 整体感知
evaluate = pd.read_csv("test.csv")
train.head()
train.shape
evaluate.shape 
train.info()
```
```

### 2 数据探索

##### 2.1 因变量分布探索——分析 “房价”

```python
```python
train['SalePrice'].describe()                # 概览 

sns.distplot(train['SalePrice'], fit=norm)   # 房价分布图 

stats.probplot(train['SalePrice'], plot=plt) # 利用Q-Q图判断数据是否偏离正态分布
```
```

![这里写图片描述](https://img-blog.csdn.net/20180802194029506?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 2.2 数值型自变量探索

###### I ：相关系数矩阵 （图如下）

```python
```python
# DataFrame.corr()方法可以给出两个列之间的相关性，我们利用该方法用于检测特征和目标变量之间的相关性。
corr = train.select_dtypes(include = [np.number]).iloc[:, 1:].corr()
corr.head() 

plt.figure(figsize=(14, 14))
sns.heatmap(corr, vmax=1, square=True)   #热力图:所有变量之间的相关性分布
```
```

![这里写图片描述](https://img-blog.csdn.net/20180802195330181?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### II ：saleprice高相关变量 相关系数矩阵 (图如上)

```python
```python
# 选取绝对值>corr_threshvalue的变量做分析
corr_threshvalue = 0.5 
corr_cols = corr.loc[:, corr.loc['SalePrice',:].abs() > corr_threshvalue]\
                .sort_values(by='SalePrice', axis=1 , ascending=False).columns
corr_thresh = train[corr_cols].corr()

plt.figure(figsize=(6, 6))
sns.set(font_scale=1.25)
sns.heatmap(corr_thresh, cbar=True, annot=True, 
            square=True, fmt='.2f', annot_kws={'size': 10})
```
```

######III ：saleprice高相关变量 散点图

#####2.3 分类型自变量探索

#####2.3.1 一元方差分析

#####2.3.2 重要变量可视化展示
##### 附：字段描述

下面是在数据描述文件中的一些字段:

· SalesPrice——以美元计算的房地产的售价。这是你要预测的目标变量。

Id: String,

MSSubClass: String, // 参与销售住宅的类型:有年代新旧等信息

MSZoning: String, // 房屋类型:农用,商用等

LotFrontage: String, // 距离街道的距离

LotArea: String, // 房屋的面积

Street: String, // 通向房屋的Street是用什么铺的

Alley: String, // 通向房屋的Alley是用什么铺的

LotShape: String, // 房屋的户型,规整程度

LandContour: String, // 房屋的平坦程度

Utilities: String, // 设施,通不通水电气

LotConfig: String, // 死路,处于三岔口等

LandSlope: String, // 坡度

Neighborhood: String, // 房屋建筑所在的地理位置
Condition1: String,

Condition2: String,

BldgType: String, // 住宅类型,住的家庭数,是否别墅等

HouseStyle: String, // 住宅类型,隔断等

OverallQual: String, // 房屋的质量

OverallCond: String, // 房屋位置的质量

YearBuilt: String, // 建造的时间

YearRemodAdd: String, // 改造的时间

RoofStyle: String, // 屋顶的类型

RoofMatl: String, // 屋顶的材料

Exterior1st: String, // 外观覆盖的材质

Exterior2nd: String, // 如果超过一种,则有第二种材质

MasVnrType: String, // 表层砌体类型

MasVnrArea: String, // 表层砌体面积

ExterQual: String, // 外观材料质量

ExterCond: String, // 外观材料情况

Foundation: String, // 地基类型

BsmtQual: String, // 地下室质量

BsmtCond: String, // 地下室的基本情况

BsmtExposure: String, // 地下室采光

BsmtFinType1: String, // 地下室的完成情况比例

BsmtFinSF1: String, // 地下室的完成面积

BsmtFinType2: String, // 如果有多个地下室的话

BsmtFinSF2: String, // 如果有多个地下室的话

BsmtUnfSF: String, // 未完成的地下室面积

TotalBsmtSF: String, // 地下室面积

Heating: String, // 供暖类型

HeatingQC: String, // 供暖质量

CentralAir: String, // 是否有中央空调

Electrical: String, // 电气系统

_1stFlrSF: String, // 1楼面积

_2ndFlrSF: String, // 2楼面积

LowQualFinSF: String, // 低质量完成的面积(楼梯占用的面积)

GrLivArea: String, // 地面以上居住面积

BsmtFullBath: String, // 地下室都是洗手间

BsmtHalfBath: String, // 地下室一半是洗手间

FullBath: String, // 洗手间都在一层以上

HalfBath: String, // 一半洗手间在一层以上

BedroomAbvGr: String, // 卧室都在一层以上

KitchenAbvGr: String, // 厨房在一层以上

KitchenQual: String, // 厨房质量

TotRmsAbvGrd: String, // 所有房间都在一层以上

Functional: String, // 房屋的功能性等级

Fireplaces: String, // 壁炉位置

FireplaceQu: String, // 壁炉质量

GarageType: String, // 车库类型

GarageYrBlt: String, // 车库建造时间

GarageFinish: String, // 车库的室内装修

GarageCars: String, // 车库的汽车容量

GarageArea: String, // 车库面积

GarageQual: String, // 车库质量

GarageCond: String, // 车库情况

PavedDrive: String, // 铺路的材料

WoodDeckSF: String, // 木地板面积

OpenPorchSF: String, // 露天门廊面积

EnclosedPorch: String, // 独立门廊面积

_3SsnPorch: String, // three season门廊面积

ScreenPorch: String, // 纱门门廊面积

PoolArea: String, // 游泳池面积

PoolQC: String, // 游泳池质量

Fence: String, // 栅栏质量

MiscFeature: String, // 上面不包含其他功能

MiscVal: String, // 上面不包含其他功能的价格

MoSold: String, // 月销量

YrSold: String, // 年销量

SaleType: String, // 销售方式

SaleCondition: String, // 销售情况





