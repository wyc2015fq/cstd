# pandas plot参数 - 墨流觞的博客 - CSDN博客





2018年09月22日 10:50:47[墨氲](https://me.csdn.net/dss_dssssd)阅读数：1374








#### 封装matplotlib的plot函数
- 
pandas.plot

DataFrame.plot(x=None, y=None, kind=‘line’, ax=None, subplots=False, sharex=None, sharey=False, layout=None, figsize=None, use_index=True, title=None, grid=None, legend=True, style=None, logx=False, logy=False, loglog=False, xticks=None, yticks=None, xlim=None, ylim=None, rot=None, fontsize=None, colormap=None, table=False, yerr=None, xerr=None, secondary_y=False, sort_columns=False, **kwds)
- 
kind : str
- 
‘line’ : line plot (default)

- 
‘bar’ : vertical bar plot

- 
‘barh’ : horizontal bar plot

- 
‘hist’ : histogram

- 
‘box’ : boxplot

- 
‘kde’ : Kernel Density Estimation plot

- 
‘density’ : same as ‘kde’

- 
‘area’ : area plot

- 
‘pie’ : pie plot

- 
‘scatter’ : scatter plot

- 
‘hexbin’ : hexbin plot


- 
alpha

点的不透明度，当点的透明度很高时，单个点的颜色很浅。这样点越密集，对应区域颜色越深。通过颜色很浅就可以就看一看出数据的几种区域。alpha=0，无色，整个绘图区域无图，类似于[R, G, B, alpha]四通道中的alpha通道

```python
```python
housing_copy.plot(kind="scatter", x='longitude', y='latitude', alpha=0.1)
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180922101011105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
```python
housing_copy.plot(kind="scatter", x='longitude', y='latitude', alpha=1)
```
```

![](https://img-blog.csdn.net/20180922100814723?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
s  :注意此参数为kind="scatter"下才有的，否则会报错 unknown property

各个点的大小，数值越大，对应点越大

```python
```python
import matplotlib.pyplot as plt
allDf = pd.DataFrame({
    'x':[0,1,2,4,7,6],
    'y':[0,3,2,4,5,7],
    's':[0,1,2,3,4,5],
    'c':['red','green','blue','red','green','blue']
},index = ['p1','p2','p3','p4','p5','p6'])

print(allDf) 

allDf.plot(x='x', y='y', kind="scatter",s=allDf['s']*10 , label='s')
plt.legend()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180922103621861?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
c : 此参数也是kind="scatter"下才能用的，为每一点赋予颜色。

建议用以下语句，只改变c就好。保留cmap和colorbar

```python
```python
allDf = pd.DataFrame({
  		    'x':[0,1,2,4,7,6],
  		    'y':[0,3,2,4,5,7],
  		    's':[0,1,2,3,4,5],
  		    'c':[1,20, 5, 15, 25, 30]
  		    },index = ['p1','p2','p3','p4','p5','p6'])
  allDf.plot(x='x', y='y', kind="scatter",c='c',cmap=plt.get_cmap("jet"), colorbar=True)
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180922104243913?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

利用c和s两个参数在图像上显示housing,其中点大小表示人口密度，颜色表示房屋价格多少。
```python
```python
housing.plot(kind="scatter", x="longitude", y="latitude", alpha=0.3,
 								s=housing["population"]/100, label="population",
 								c="median_house_value", cmap=plt.get_cmap("jet"), colorbar=True,
 								)
 plt.legend()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20180922104627493?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)








