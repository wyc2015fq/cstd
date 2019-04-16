# [Python可视化] pyecharts安装入门及绘制中国贵州地图 - 杨秀璋的专栏 - CSDN博客





2018年04月10日 20:25:54[Eastmount](https://me.csdn.net/Eastmount)阅读数：7587
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









前端或网站开发的博友可能都使用过强大的Echarts技术。ECharts是一个纯Javascript的图表库，可以流畅的运行在PC和移动设备上，兼容当前绝大部分浏览器，底层依赖轻量级的Canvas类库ZRender，提供直观、生动、可交互、可高度个性化定制的数据可视化图表。ECharts提供了常规的折线图、柱状图、散点图、饼图、K线图，用于统计的盒形图，用于地理数据可视化的地图、热力图、线图，用于关系数据可视化的关系图、treemap，多维数据可视化的平行坐标，还有用于BI的漏斗图、仪表盘，并且支持图与图之间的混搭。

下图是Echarts简单绘制折线图的示例，左边是脚本代码，右边是显示图形，非常美观。
[http://echarts.baidu.com/echarts2/doc/example/line1.html#helianthus](http://echarts.baidu.com/echarts2/doc/example/line1.html#helianthus)


![](https://img-blog.csdn.net/2018040915230811)


echarts官方网址：[http://echarts.baidu.com/tutorial.html](http://echarts.baidu.com/tutorial.html)
Eastmount文章： [[Echarts可视化] 一.入门篇之简单绘制中国地图和贵州地区](https://blog.csdn.net/eastmount/article/details/69938479)


既然Echarts这么好用，Python也有相应的第三方扩展包支持，它就是这篇文章讲解的pyecharts库。pyecharts是一个用于生成 Echarts 图表的类库，即Echarts与Python的对接，并推荐大家结合Django或Flask使用。下面开始入门介绍，希望这篇文章对你有所帮助，如果有错误或不足之处，还请海涵。






## 一. 安装pyecharts

pyecharts是兼容Python2和Python3的，直接使用pip进行安装：


```python
pip install pyecharts
```

安装如下图所示：

![](https://img-blog.csdn.net/20180409164904932)





安装成功之后，我们直接导入该扩展包使用。常见的图形包括：（参考[赖德发大神博客](https://blog.csdn.net/u013421629/article/details/78183637)）


```python
Bar（柱状图/条形图） 
Bar3D（3D 柱状图） 
Boxplot（箱形图） 
EffectScatter（带有涟漪特效动画的散点图） 
Funnel（漏斗图） 
Gauge（仪表盘） 
Geo（地理坐标系） 
Graph（关系图） 
HeatMap（热力图） 
Kline（K线图） 
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
ThemeRiver（主题河流图） 
WordCloud（词云图）
```








## 二. 入门知识

**1.柱状图**

代码如下：

```python
# -*- coding:utf-8 -*-
from pyecharts import Bar

bar = Bar("贵州GDP柱状图", "副标题")
bar.add("GDP",
        ["贵阳市", "遵义市", "六盘水市", "安顺市", "黔东南州"],
        [40, 30, 26, 22, 15])
bar.show_config()
bar.render()
```

代码运行之后，会在本地生成一个render.html文件，打开输出如下所示图形。



![](https://img-blog.csdn.net/20180409171253223)

**from pyecharts import Bar**
#从pyecharts库中导入Bar子类
**bar = Bar("贵州GDP柱状图", "副标题")**
#定义Bar()柱状图，同时设置主标题和副标题
**bar.add()**
#调用add()函数添加图表的数据和设置各种配置项
**bar.show_config()**
#打印输出图表的所有配置项
**bar.render()**
#生成render.html文件，也可以设置路径和文件名




**2.横向柱状图**

代码如下：

```python
# -*- coding:utf-8 -*-
from pyecharts import Bar

bar = Bar("贵州GDP柱状图", "副标题")
city = ["贵阳市", "遵义市", "六盘水市", "安顺市", "黔东南州"]
data1 = [40, 30, 26, 22, 15]
data2 = [13, 43, 32, 38, 20]
bar.add("2017年GDP", city, data1)
bar.add("2016年GDP", city, data2, is_convert=True)
bar.show_config()
bar.render()
```
输出如下图所示：


![](https://img-blog.csdn.net/20180409174355330)







**3.带有涟漪特效动画的散点图**

这段代码参考简书网 [https://www.jianshu.com/p/b718c307a61c](https://www.jianshu.com/p/b718c307a61c)，强烈推荐大家学习chenjiandongx大神的文章。完整代码如下：

```python
# -*- coding:utf-8 -*-
from pyecharts import EffectScatter

es = EffectScatter("动态散点图各种图形示例")
es.add("", [10], [10], symbol_size=20, effect_scale=3.5, effect_period=3, symbol="pin")
es.add("", [20], [20], symbol_size=12, effect_scale=4.5, effect_period=4, symbol="rect")
es.add("", [30], [30], symbol_size=30, effect_scale=5.5, effect_period=5, symbol="roundRect")
es.add("", [40], [40], symbol_size=10, effect_scale=6.5, effect_brushtype='fill', symbol="diamond")
es.add("", [50], [50], symbol_size=16, effect_scale=5.5, effect_period=3, symbol="arrow")
es.add("", [60], [60], symbol_size=6, effect_scale=2.5, effect_period=3, symbol="triangle")
es.render()
```
运行结果如下图所示：


![](https://img-blog.csdn.net/20180409230659952)









**4.绘制3D图形**

绘制3D折线图代码如下：


```python
# -*- coding:utf-8 -*-
from pyecharts import Line3D
import random
data = [[1,2,3,4], [1,2,3,4], [0,4,8,16]]
Line3D = Line3D("3D 折线图示例", width=1200, height=600)
Line3D.add("", data, is_visualmap=True)
Line3D.render()
```
输出图形如下所示：

![](https://img-blog.csdn.net/20180410185519434)

绘制3D散点图，并设置随机散点坐标，代码如下所示：

```python
# -*- coding:utf-8 -*-
from pyecharts import Scatter3D
import random
data = [[random.randint(0, 100), random.randint(0, 100), random.randint(0, 100)] for _ in range(80)]
range_color = ['#313695', '#4575b4', '#74add1', '#abd9e9', '#e0f3f8', '#ffffbf',
               '#fee090', '#fdae61', '#f46d43', '#d73027', '#a50026']
scatter3D = Scatter3D("3D 散点图示例", width=1200, height=600)
scatter3D.add("", data, is_visualmap=True, visual_range_color=range_color)
scatter3D.render()
```
输出结果非常美观，如下图所示：


![](https://img-blog.csdn.net/2018041019411998)



**5.仪表盘**

代码如下：


```python
# -*- coding:utf-8 -*-
from pyecharts import Gauge

g = Gauge("仪表盘图形","副图标")
g.add("重大项目", "投资占比", 66.66)
g.show_config()
g.render("g.html")
```

输出图形如下所示：



![](https://img-blog.csdn.net/20180410194452751)



**6.水球图**

代码如下：


```python
# -*- coding:utf-8 -*-
from pyecharts import Liquid

liquid = Liquid("水球图")
liquid.add("Liquid", [0.8])
liquid.show_config()
liquid.render()
```

输出如下图所示：

![](https://img-blog.csdn.net/20180410194714522)


讲到这里基本的图形讲解完毕，更多知识推荐大家结合实际应用研究。






## 三. 绘制中国地图


如果直接绘制中国地图，只会显示一个角落，如下图所示：



![](https://img-blog.csdn.net/20180410194905893)

**1.安装**

这是需要安装第三方地图支持包，代码如下：
**    pip install echarts-countries-pypkg     pip install echarts-china-provinces-pypkg    pip install echarts-china-cities-pypkg**

**    pip install echarts-countries-pypkg     pip install echarts-china-provinces-pypkg    pip install echarts-china-cities-pypkg**

其中echarts-countries-pypkg 是全球国家地图，echarts-china-provinces-pypkg是中国省级地图， echarts-china-cities-pypkg是中国城市地图。安装过程如下图所示：



![](https://img-blog.csdn.net/20180410195118463)

![](https://img-blog.csdn.net/20180410195137898)

![](https://img-blog.csdn.net/2018041019521966)




**2.绘制中国地图**

安装之后我们来看一个简单的中国地图代码，这是作者微信好友全国各省分布情况，代码如下：

```python
#-*- coding:utf-8 -*-
from pyecharts import Map
value =[2, 60, 2, 6, 80, 2, 5, 2, 1, 4, 5, 1,
        4, 1, 5, 2, 2, 5, 4, 1, 1, 10, 2]
attr =["安徽", "北京", "福建", "广东", "贵州", "海南", "河北", "河南", "黑龙江",
       "湖北", "湖南", "吉林", "江苏", "辽宁", "山东", "山西", "陕西", "上海",
       "四川", "天津", "云南", "浙江", "重庆"]
map=Map("各省微信好友分布", width=1200, height=600)
map.add("", attr, value, maptype='china', is_visualmap=True,
        visual_text_color='#000')
map.show_config()
map.render()
```
输出结果如下图所示，因为作者在北京读书，贵州人，所以这两个地方好友最多。


![](https://img-blog.csdn.net/20180410195500980)


再来看一组中国地图，全国的空气质量情况，代码如下：

```python
#-*- coding:utf-8 -*-
from pyecharts import Geo

data = [
    ("海门", 9),("鄂尔多斯", 12),("招远", 12),("舟山", 12),("齐齐哈尔", 14),("盐城", 15),
    ("赤峰", 16),("青岛", 18),("乳山", 18),("金昌", 19),("泉州", 21),("莱西", 21),
    ("日照", 21),("胶南", 22),("南通", 23),("拉萨", 24),("云浮", 24),("梅州", 25)]
geo = Geo("全国主要城市空气质量", "data from pm2.5",
          title_color="#fff", title_pos="center",
          width=1200, height=600, background_color='#404a59')
attr, value = geo.cast(data)
geo.add("", attr, value, visual_range=[0, 200],
        visual_text_color="#fff", symbol_size=15, is_visualmap=True)
geo.show_config()
geo.render("kongqi.html")
```

输出结果如下所示，它会提示错误“coordinates is not found”坐标没有定义，知乎说可以用 geo_cities_coords 参数自定义经纬度处理解决。

![](https://img-blog.csdn.net/2018041020041523)


而我的解决方法是怀疑中文乱码错误，并且成功解决，即中文前面加个u"贵阳"，代码如下：

```python
#-*- coding:utf-8 -*-
from pyecharts import Geo

data = [
    (u"海门", 9),(u"鄂尔多斯", 12),(u"招远", 12),(u"舟山", 12),(u"齐齐哈尔", 14),(u"盐城", 15),
    (u"赤峰", 16),(u"青岛", 18),(u"乳山", 18),(u"金昌", 19),(u"泉州", 21),(u"莱西", 21),
    (u"日照", 21),(u"胶南", 22),(u"南通", 23),(u"拉萨", 24),(u"云浮", 24),(u"梅州", 25)]
geo = Geo(u"全国主要城市空气质量", "data from pm2.5",
          title_color="#fff", title_pos="center",
          width=1200, height=600, background_color='#404a59')
attr, value = geo.cast(data)
geo.add("", attr, value, visual_range=[0, 200],
        visual_text_color="#fff", symbol_size=15, is_visualmap=True)
geo.show_config()
geo.render("kongqi.html")
```
输出如下图所示：

![](https://img-blog.csdn.net/20180410200808572)



**3.绘制贵州地图**

最后补充绘制贵州省地图的代码：

```python
#-*- coding:utf-8 -*-
from pyecharts import Map

value = [95, 70, 30, 45, 80,
         10, 25, 40, 5]
attr = [u'贵阳市', u'遵义市',  u'六盘水市', u'安顺市', u'毕节市', 
        u'铜仁市', u"黔东南苗族侗族自治州", u"黔南布依族苗族自治州",
        u"黔西南布依族苗族自治州"]
map = Map(u"贵州地图示例", width=1200, height=600)
map.add("", attr, value, maptype=u'贵州',
        is_visualmap=True, visual_text_color='#000')
map.show_config()
map.render()
```

输出如下图所示：

![](https://img-blog.csdn.net/20180410201614151)









作者是在学习Python获取微信信息，从而可视化画图学习的pyecharts库，也希望感兴趣的同学学习下itchat库。
参考作者文章： [[Python微信开发] 一.itchat入门知识及微信自动回复、微信签名词云分析](https://blog.csdn.net/eastmount/article/details/79618039)


狂风骤起吹落花，飘零万里到天涯。
天垂六幕千山外，娜璋云游看晚霞。
最近四月把婚纱照拍了，开心！同时，写博客已经5年了，每个月都坚持写，这感觉真棒，也谢谢CSDN。



![](https://img-blog.csdn.net/20180410202013329)

![](https://img-blog.csdn.net/20180410202040693)





(By:Eastmount 2018-04-10 晚上9点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))














