
# seaborn可视化 - leofionn的博客 - CSDN博客


2018年05月24日 19:53:58[leofionn](https://me.csdn.net/qq_36142114)阅读数：744


```python
import matplotlib.pyplot as plt
%matplotlib inline
import seaborn as sns
sns.set()
import numpy as np
import pandas as pd
```
In [8]:
```python
data=np.random.multivariate_normal([0,0],[[5,2],[2,2]],size=2000)
data=pd.DataFrame(data,columns=['x','y'])
```
In [9]:
```python
data.head()
```
Out[9]:
|x|y|
|---|---|---|
|0|0.571117|-0.158731|
|1|2.522765|2.033863|
|2|-3.413121|-0.566827|
|3|-1.788482|-0.308131|
|4|3.565947|2.668333|
In [10]:
```python
for col in 'xy':#频次直方图
    plt.hist(data[col],normed=True,alpha=0.5)
```
```python
/opt/conda/lib/python3.5/site-packages/matplotlib/axes/_axes.py:6462: UserWarning: The 'normed' kwarg is deprecated, and has been replaced by the 'density' kwarg.
  warnings.warn("The 'normed' kwarg is deprecated, and has been "
```
![](https://cdn.kesci.com/rt_upload/C595D3F67A2448EB80E7B5B586EDE753/1527084514045_344.png)
In [11]:
```python
for col in 'xy':#KDE可视化
    sns.kdeplot(data[col],shade=True)
```
![](https://cdn.kesci.com/rt_upload/52968DE705F843728A950857A4D38C3B/1527084531762_713.png)
In [14]:
```python
sns.distplot(data['x'])#频次直方图与KDE的结合
sns.distplot(data['y']);
```
```python
/opt/conda/lib/python3.5/site-packages/matplotlib/axes/_axes.py:6462: UserWarning: The 'normed' kwarg is deprecated, and has been replaced by the 'density' kwarg.
  warnings.warn("The 'normed' kwarg is deprecated, and has been "
```
![](https://cdn.kesci.com/rt_upload/DCA579C20BAF403F86ED227EA6945E92/1527084609784_664.png)
In [15]:
```python
sns.kdeplot(data);#二维KDE图
```
```python
/opt/conda/lib/python3.5/site-packages/seaborn/distributions.py:645: UserWarning: Passing a 2D dataset for a bivariate plot is deprecated in favor of kdeplot(x, y), and it will cause an error in future versions. Please update your code.
  warnings.warn(warn_msg, UserWarning)
```
![](https://cdn.kesci.com/rt_upload/BCECAA0E85CA4EA7863DE4C476BFDDD3/1527084680227_404.png)
In [18]:
```python
with sns.axes_style('white'):
    sns.jointplot('x','y',data,kind='kde')
```
![](https://cdn.kesci.com/rt_upload/54FAD62643DE4F5F927FCEA5D83DCDAF/1527084849595_700.png)
In [19]:
```python
with sns.axes_style('white'):
    sns.jointplot('x','y',data,kind='hex')
```
```python
/opt/conda/lib/python3.5/site-packages/matplotlib/axes/_axes.py:6462: UserWarning: The 'normed' kwarg is deprecated, and has been replaced by the 'density' kwarg.
  warnings.warn("The 'normed' kwarg is deprecated, and has been "
```
![](https://cdn.kesci.com/rt_upload/CE092305720E48C2A988DEE9346318D2/1527084866950_167.png)
In [20]:
```python
iris=sns.load_dataset('iris')
```
In [21]:
```python
iris.head()
```
Out[21]:
|sepal_length|sepal_width|petal_length|petal_width|species|
|---|---|---|---|---|---|
|0|5.1|3.5|1.4|0.2|setosa|
|1|4.9|3.0|1.4|0.2|setosa|
|2|4.7|3.2|1.3|0.2|setosa|
|3|4.6|3.1|1.5|0.2|setosa|
|4|5.0|3.6|1.4|0.2|setosa|
In [24]:
```python
sns.pairplot(iris,hue='species',size=2.5)#矩阵图
```
Out[24]:
```python
<seaborn.axisgrid.PairGrid at 0x7f380c3ac0b8>
```
![](https://cdn.kesci.com/rt_upload/872179AB7EE644608CDB3A27790AD273/1527086496470_700.png)
In [26]:
```python
tips=sns.load_dataset('tips')
tips.head()
```
Out[26]:
|total_bill|tip|sex|smoker|day|time|size|
|---|---|---|---|---|---|---|---|
|0|16.99|1.01|Female|No|Sun|Dinner|2|
|1|10.34|1.66|Male|No|Sun|Dinner|3|
|2|21.01|3.50|Male|No|Sun|Dinner|3|
|3|23.68|3.31|Male|No|Sun|Dinner|2|
|4|24.59|3.61|Female|No|Sun|Dinner|4|
In [32]:
```python
tips['tip_pct']=100*tips['tip']/tips['total_bill']#分面频次直方图
grid=sns.FacetGrid(tips,row='sex',col='time',margin_titles=True)
grid.map(plt.hist,'tip_pct',bins=np.linspace(0,40,15));
```
![](https://cdn.kesci.com/rt_upload/9F7F69C5B3814F4188D30179BD1DC0DE/1527089171304_995.png)
In [35]:
```python
with sns.axes_style(style='ticks'):  # 因子图中不同离散因子分布对比
    g = sns.factorplot('day', 'total_bill', 'sex', data=tips, kind='box')
    g.set_axis_labels('Day', 'Total Bill')
```
![](https://cdn.kesci.com/rt_upload/49F9437F45A34F06875C98CC7D89E516/1527090563207_381.png)
In [37]:
```python
with sns.axes_style('white'):#联合分布图
    sns.jointplot('total_bill','tip',data=tips,kind='hex')
```
```python
/opt/conda/lib/python3.5/site-packages/matplotlib/axes/_axes.py:6462: UserWarning: The 'normed' kwarg is deprecated, and has been replaced by the 'density' kwarg.
  warnings.warn("The 'normed' kwarg is deprecated, and has been "
```
![](https://cdn.kesci.com/rt_upload/480DAAF113354FD9AFB74A2DF87436A8/1527090691743_134.png)
In [38]:
```python
sns.jointplot('total_bill','tip',data=tips,kind='reg')#带回归拟合的联合分布
```
```python
/opt/conda/lib/python3.5/site-packages/matplotlib/axes/_axes.py:6462: UserWarning: The 'normed' kwarg is deprecated, and has been replaced by the 'density' kwarg.
  warnings.warn("The 'normed' kwarg is deprecated, and has been "
```
Out[38]:
```python
<seaborn.axisgrid.JointGrid at 0x7f37faf14dd8>
```
![](https://cdn.kesci.com/rt_upload/94C2F9D86E6547B19427331434F27D65/1527090750758_595.png)
In [40]:
```python
planets=sns.load_dataset('planets')#用行星数据
planets.head()
```
Out[40]:
|method|number|orbital_period|mass|distance|year|
|---|---|---|---|---|---|---|
|0|Radial Velocity|1|269.300|7.10|77.40|2006|
|1|Radial Velocity|1|874.774|2.21|56.95|2008|
|2|Radial Velocity|1|763.000|2.60|19.84|2011|
|3|Radial Velocity|1|326.030|19.40|110.62|2007|
|4|Radial Velocity|1|516.220|10.50|119.47|2009|
In [41]:
```python
with sns.axes_style('white'):
    g=sns.factorplot('year',data=planets,aspect=2,kind='count',color='steelblue')
    g.set_xticklabels(step=5)
```
![](https://cdn.kesci.com/rt_upload/24797AB15C7746389F7C3FAA58482BCF/1527090971206_954.png)
In [42]:
```python
with sns.axes_style('white'):#不同年份、方法发现的行星数量
    g=sns.factorplot('year',data=planets,aspect=4.0,kind='count',hue='method',order=range(2001,2015))
    g.set_ylabels('number of planets discovered')
```
![](https://cdn.kesci.com/rt_upload/508DED955BDE48AF9E84DBC7CDF98394/1527091068229_754.png)

