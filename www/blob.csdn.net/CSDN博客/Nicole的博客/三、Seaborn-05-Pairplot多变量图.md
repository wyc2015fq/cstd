# 三、Seaborn-05-Pairplot多变量图 - Nicole的博客 - CSDN博客
2018年06月03日 22:36:16[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：601
```python
```
#-*- coding:utf-8 -*-
from __future__ import division
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
```
```
seaborn.pairplot(data, hue=None, hue_order=None, palette=None, vars=None, x_vars=None, y_vars=None, kind='scatter', diag_kind='hist', markers=None, size=2.5, aspect=1, dropna=True, plot_kws=None, diag_kws=None, grid_kws=None)¶
数据指定：
> 
vars : 与data使用，否则使用data的全部变量。参数类型：numeric类型的变量list。
{x, y}_vars : 与data使用，否则使用data的全部变量。参数类型：numeric类型的变量list。
dropna : 是否剔除缺失值。参数类型：boolean, optional
特殊参数：
> 
kind : {‘scatter’, ‘reg’}, optional Kind of plot for the non-identity relationships.
diag_kind : {‘hist’, ‘kde’}, optional。Kind of plot for the diagonal subplots.
基本参数：
> 
size : 默认 6，图的尺度大小（正方形）。参数类型：numeric
hue : 使用指定变量为分类变量画图。参数类型：string (变量名)
hue_order : list of strings Order for the levels of the hue variable in the palette
palette : 调色板颜色
markers : 使用不同的形状。参数类型：list
aspect : scalar, optional。Aspect * size gives the width (in inches) of each facet.
{plot, diag, grid}_kws : 指定其他参数。参数类型：dicts
返回：
> 
PairGrid 对象
### 1、散点图
```cpp
```
sns.set(style="ticks", color_codes=True)
iris = sns.load_dataset("iris")
g = sns.pairplot(iris)
```
```
![](https://upload-images.jianshu.io/upload_images/3341358-181c1bc96e6b849d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
05_01.png
### 2、指定分类变量的，散点图
`g2 = sns.pairplot(iris, hue="species")`
![](https://upload-images.jianshu.io/upload_images/3341358-8559927b17853637.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
05_05.png
使用调色板
`g3 = sns.pairplot(iris, hue="species", palette="husl")`
![](https://upload-images.jianshu.io/upload_images/3341358-1d90711f5aea821d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
05_06.png
使用不同的形状
`g4 = sns.pairplot(iris, hue="species", markers=["o", "s", "D"])`
![](https://upload-images.jianshu.io/upload_images/3341358-f3ffb05e902f1cbc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
05_07.png
### 3、改变对角图
使用 KDE
`g5 = sns.pairplot(iris, diag_kind="kde")`
![](https://upload-images.jianshu.io/upload_images/3341358-ed62ebad7f83cf43.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
05_03.png
使用回归
`g6 = sns.pairplot(iris, kind="reg")`
![](https://upload-images.jianshu.io/upload_images/3341358-07d25cd26f686961.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
05_04.png
### 4、改变点形状，使用参数，使用 edgecolor
```php
```
g7 = sns.pairplot(iris, diag_kind="kde", markers="+",
                  plot_kws=dict(s=50, edgecolor="b", linewidth=1),
                  diag_kws=dict(shade=True))
```
```
![](https://upload-images.jianshu.io/upload_images/3341358-b2772de52bdcf453.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
05_02.png
原出处：https://www.jianshu.com/p/6e18d21a4cad
