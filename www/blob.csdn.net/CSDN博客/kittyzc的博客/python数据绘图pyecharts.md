# python数据绘图pyecharts - kittyzc的博客 - CSDN博客





2018年10月15日 10:14:22[IE06](https://me.csdn.net/kittyzc)阅读数：242
所属专栏：[python小tips](https://blog.csdn.net/column/details/31935.html)









pyecharts的绘图非常漂亮，这里总结一下学习内容。

# 1. 安装相关：

使用python2.7前加入中文支持：

#coding=utf-8

目前pyecharts不带地图文件，因此如果要使用地图的话必须自己下载：
```
$ pip install echarts-countries-pypkg
$ pip install echarts-china-provinces-pypkg
$ pip install echarts-china-cities-pypkg
$ pip install echarts-china-counties-pypkg
$ pip install echarts-china-misc-pypkg
```

如果想更改风格，可以安装主题插件：

```
$ pip install echarts-themes-pypkg
```

主题列表包括：vintage、macarons、infographic、shine、roma、westeros、wonderland、chalk、halloween、essos、walden、purple-passion、romantic，效果参见[这里](http://pyecharts.org/#/zh-cn/themes)。

# 2. 基本图形列表：

Bar（柱状图/条形图）

Bar3D（3D 柱状图）

Boxplot（箱形图）

EffectScatter（带有涟漪特效动画的散点图）

Funnel（漏斗图）

Gauge（仪表盘）

Geo（地理坐标系）

GeoLines（地理坐标系线图）

Graph（关系图）

HeatMap（热力图）

Kline/Candlestick（K线图）

Line（折线/面积图）

Line3D（3D 折线图）

Liquid（水球图）

Map（地图）

Parallel（平行坐标系）

Pie（饼图）

Polar（极坐标系）

Radar（雷达图）

Sankey（桑基图）

Scatter（散点图）

Scatter3D（3D 散点图）

Surface3D（3D 曲面图）

ThemeRiver（主题河流图）

Tree（树图）

TreeMap（矩形树图）

WordCloud（词云图）

另外网上还有一个基于pyecharts的iclientpy库，可以方便的绘制热力图：
```python
from iclientpy import viz
data = []
with open('heat.csv') as f:
    lines = f.readlines()#[1:]
    for l in lines:
        l = l.split(',')
        data.append([float(l[1]),float(l[0]),int(l[2])])
chart=viz.heat(data=data, radius=22, min_opacity=0.5, blur=50)
chart
```

其中heat.csv格式如下：

```
116.3980865,39.90852356,758
111.3278961,30.73493958,361
120.1540375,30.2872467,317
......
```

可以使用官方的 [http://jupyter.supermap.io/](http://jupyter.supermap.io/) 地址进行尝试。

# 3. 综合示例：

```
from pyecharts import Bar
a = ['{}月'.format(i) for i in range(1,13)]
v1 = [2.6,5.9,9.0,26.4,2.0, 4.9, 7.0,23.2,12.2,1,2.1,2.0]
v2 = [2.6,12.2,1,2.1,2.0,5.9,9.0,26.4,2.0, 4.9, 7.0,23.2]
bar = Bar('柱状图示例')
bar.add('蒸发量',a,v1,mark_point=['max','min'],is_datazoom_show=True,is_more_utils = True)
bar.add('降水量',a,v2,mark_point=['max','min'],is_datazoom_show=True,is_more_utils = True)
bar
```

显示图形为：
![在这里插入图片描述](https://img-blog.csdn.net/20181015101332827?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





