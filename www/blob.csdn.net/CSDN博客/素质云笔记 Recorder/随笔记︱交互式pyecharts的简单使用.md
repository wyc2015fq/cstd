
# 随笔记︱交互式pyecharts的简单使用 - 素质云笔记-Recorder... - CSDN博客

2018年08月07日 22:38:50[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1383


pyecharts 是一个用于生成 Echarts 图表的类库。Echarts 是百度开源的一个数据可视化 JS 库。用 Echarts 生成的图可视化效果非常棒，为了与 Python 进行对接，方便在 Python 中直接使用数据生成图，我写了这个项目。以下是最新版echart的靓图。当然，pyecharts貌似没有这么齐全。
![这里写图片描述](https://img-blog.csdn.net/20180807215851316?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
官方文档：[http://pyecharts.herokuapp.com/](http://pyecharts.herokuapp.com/)
pyecharts 图表配置：[http://pyecharts.org/\#/zh-cn/prepare](http://pyecharts.org/#/zh-cn/prepare)
github：[https://github.com/pyecharts/pyecharts](https://github.com/pyecharts/pyecharts)
安装：
`pip``install``pyecharts`还有很多已经打包好的中国地图、世界地图包：
`$ pip install echarts``-countries``-pypkg``$ pip install echarts``-china``-provinces``-pypkg``$ pip install echarts``-china``-cities``-pypkg``$ pip install echarts``-china``-counties``-pypkg``$ pip install echarts``-china``-misc``-pypkg``$ pip install echarts``-united``-kingdom``-pypkg`
---目 录

---[目 录](#目-录)


---[一、简单示例](#一简单示例)

---[二、笔者关注的几个图表](#二笔者关注的几个图表)

---[2.1 画出花样的散点图](#21-画出花样的散点图)

---[2.2 关系图 + NetworkX](#22-关系图-networkx)

---[2.3  WordCloud（词云图）](#23-wordcloud词云图)

---[2.4 TreeMap（矩形树图）](#24-treemap矩形树图)


---[3 相关组件](#3-相关组件)

---[3.1 Grid：并行显示多张图](#31-grid并行显示多张图)

---[3.2 Overlap：结合不同类型图表叠加画在同张图上](#32-overlap结合不同类型图表叠加画在同张图上)

---[3.3 Page：同一网页按顺序展示多图](#33-page同一网页按顺序展示多图)

---[3.4 Timeline：提供时间线轮播多张图](#34-timeline提供时间线轮播多张图)



---一、简单示例

---from

---pyecharts import Bar
attr = [

---"Jan"

---,

---"Feb"

---,

---"Mar"

---,

---"Apr"

---,

---"May"

---,

---"Jun"

---,

---"Jul"

---,

---"Aug"

---,

---"Sep"

---,

---"Oct"

---,

---"Nov"

---,

---"Dec"

---]
v1 = [

---2.0

---,

---4.9

---,

---7.0

---,

---23.2

---,

---25.6

---,

---76.7

---,

---135.6

---,

---162.2

---,

---32.6

---,

---20.0

---,

---6.4

---,

---3.3

---]
v2 = [

---2.6

---,

---5.9

---,

---9.0

---,

---26.4

---,

---28.7

---,

---70.7

---,

---175.6

---,

---182.2

---,

---48.7

---,

---18.8

---,

---6.0

---,

---2.3

---]
bar = Bar(

---"Bar chart"

---,

---"precipitation and evaporation one year"

---)
bar.

---add

---(

---"precipitation"

---, attr, v1, mark_line=[

---"average"

---], mark_point=[

---"max"

---,

---"min"

---])
bar.

---add

---(

---"evaporation"

---, attr, v2, mark_line=[

---"average"

---], mark_point=[

---"max"

---,

---"min"

---])
bar.render()

---\#bar
![这里写图片描述](https://img-blog.csdn.net/20180807220437863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---其中，

---bar.render()

---，是以html形式保存在本地文件中；

---bar

---，是在当前环境下，输出图表。

---输出方式还有PDF：

---bar

---.render

---(path=

---"render.png"

---)

---同时也可以使用flask/Django进行封装，

![这里写图片描述](https://img-blog.csdn.net/20180807220659450?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---二、笔者关注的几个图表

---2.1 画出花样的散点图

---from

---pyecharts import EffectScatter
es = EffectScatter(

---"动态散点图各种图形示例"

---)
es.

---add

---(

---""

---, [

---10

---], [

---10

---], symbol_size=

---20

---, effect_scale=

---3.5

---,
       effect_period=

---3

---, symbol=

---"pin"

---)
es.

---add

---(

---""

---, [

---20

---], [

---20

---], symbol_size=

---12

---, effect_scale=

---4.5

---,
       effect_period=

---4

---,symbol=

---"rect"

---)
es.

---add

---(

---""

---, [

---30

---], [

---30

---], symbol_size=

---30

---, effect_scale=

---5.5

---,
       effect_period=

---5

---,symbol=

---"roundRect"

---)
es.

---add

---(

---""

---, [

---40

---], [

---40

---], symbol_size=

---10

---, effect_scale=

---6.5

---,
       effect_brushtype=

---'fill'

---,symbol=

---"diamond"

---)
es.

---add

---(

---""

---, [

---50

---], [

---50

---], symbol_size=

---16

---, effect_scale=

---5.5

---,
       effect_period=

---3

---,symbol=

---"arrow"

---)
es.

---add

---(

---""

---, [

---60

---], [

---60

---], symbol_size=

---6

---, effect_scale=

---2.5

---,
       effect_period=

---3

---,symbol=

---"triangle"

---)
es
![这里写图片描述](https://img-blog.csdn.net/20180807221437115?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---2.2 关系图 + NetworkX

---from

---pyecharts

---import

---Graph
nodes = [{

---"name"

---:

---"结点1"

---,

---"symbolSize"

---:

---10

---},
         {

---"name"

---:

---"结点2"

---,

---"symbolSize"

---:

---20

---},
         {

---"name"

---:

---"结点3"

---,

---"symbolSize"

---:

---30

---},
         {

---"name"

---:

---"结点4"

---,

---"symbolSize"

---:

---40

---},
         {

---"name"

---:

---"结点5"

---,

---"symbolSize"

---:

---50

---},
         {

---"name"

---:

---"结点6"

---,

---"symbolSize"

---:

---40

---},
         {

---"name"

---:

---"结点7"

---,

---"symbolSize"

---:

---30

---},
         {

---"name"

---:

---"结点8"

---,

---"symbolSize"

---:

---20

---}]
links = []

---for

---i

---in

---nodes:

---for

---j

---in

---nodes:
        links.append({

---"source"

---: i.get(

---'name'

---),

---"target"

---: j.get(

---'name'

---)})
graph = Graph(

---"关系图-力引导布局示例"

---)
graph.add(

---""

---, nodes, links, is_label_show=

---True

---,
          graph_repulsion=

---8000

---, graph_layout=

---'circular'

---,
          label_text_color=

---None

---)
graph
![这里写图片描述](https://img-blog.csdn.net/20180807221648817?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---其中，pyecharts处理不了太复杂的关系图，可以借用： networkx 库（可参考笔者的博文：

---[关系图︱python 关系网络的可视化NetworkX(与Apple.Turicreate深度契合)](https://blog.csdn.net/sinat_26917383/article/details/78963590)

---）

---from

---__future__

---import

---unicode_literals

---import

---networkx

---as

---nx

---from

---networkx.readwrite

---import

---json_graph

---from

---pyecharts

---import

---Graph
g = nx.Graph()
categories = [

---'网关'

---,

---'节点'

---]
g.add_node(

---'FF12C904'

---, name=

---'Gateway 1'

---, symbolSize=

---40

---, category=

---0

---)
g.add_node(

---'FF12CA02'

---, name=

---'Node 11'

---, category=

---1

---)
g.add_node(

---'FF12C326'

---, name=

---'Node 12'

---, category=

---1

---)
g.add_node(

---'FF45C023'

---, name=

---'Node 111'

---, category=

---1

---)
g.add_node(

---'FF230933'

---, name=

---'Node 1111'

---, category=

---1

---)
g.add_edge(

---'FF12C904'

---,

---'FF12CA02'

---)
g.add_edge(

---'FF12C904'

---,

---'FF12C326'

---)
g.add_edge(

---'FF12CA02'

---,

---'FF45C023'

---)
g.add_edge(

---'FF45C023'

---,

---'FF230933'

---)
g_data = json_graph.node_link_data(g)
eg = Graph(

---'设备最新拓扑图'

---)
eg.add(

---'Devices'

---, nodes=g_data[

---'nodes'

---], links=g_data[

---'links'

---], categories=categories)

---\# eg.show_config()

---eg
![这里写图片描述](https://img-blog.csdn.net/20180807221927272?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---2.3  WordCloud（词云图）

---from

---pyecharts import WordCloud
name = [

---'Sam S Club'

---,

---'Macys'

---,

---'Amy Schumer'

---,

---'Jurassic World'

---,

---'Charter Communications'

---,

---'Chick Fil A'

---,

---'Planet Fitness'

---,

---'Pitch Perfect'

---,

---'Express'

---,

---'Home'

---,

---'Johnny Depp'

---,

---'Lena Dunham'

---,

---'Lewis Hamilton'

---,

---'KXAN'

---,

---'Mary Ellen Mark'

---,

---'Farrah Abraham'

---,

---'Rita Ora'

---,

---'Serena Williams'

---,

---'NCAA baseball tournament'

---,

---'Point Break'

---]

---value

---= [

---10000

---,

---6181

---,

---4386

---,

---4055

---,

---2467

---,

---2244

---,

---1898

---,

---1484

---,

---1112

---,

---965

---,

---847

---,

---582

---,

---555

---,

---550

---,

---462

---,

---366

---,

---360

---,

---282

---,

---273

---,

---265

---]
wordcloud = WordCloud(width=

---1300

---, height=

---620

---)
wordcloud.

---add

---(

---""

---, name,

---value

---, word_size_range=[

---20

---,

---100

---])
wordcloud

---支持中文。

![这里写图片描述](https://img-blog.csdn.net/20180807222330831?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---2.4 TreeMap（矩形树图）

---矩形树图是一种常见的表达『层级数据』『树状数据』的可视化形式。它主要用面积的方式，便于突出展现出『树』的各层级中重要的节点。

---from

---pyecharts

---import

---TreeMap

---treemap

---=

---TreeMap

---(

---"矩形树图示例"

---, width=

---1200

---, height=

---600

---)

---import

---os

---import

---json

---with

---open(os.path.join(

---".."

---,

---"json"

---,

---"treemap.json"

---),

---"r"

---, encoding=

---"utf-8"

---)

---as

---f:

---data

---= json.load

---(

---f

---)

---treemap

---.add(

---"演示数据"

---,

---data

---, is_label_show=

---True

---, label_pos='inside')

---treemap

---.render()
![这里写图片描述](https://img-blog.csdn.net/20180807223728524?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---3 相关组件

---3.1 Grid：并行显示多张图

![这里写图片描述](https://img-blog.csdn.net/20180807223433953?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---3.2 Overlap：结合不同类型图表叠加画在同张图上

![这里写图片描述](https://img-blog.csdn.net/20180807223521784?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---3.3 Page：同一网页按顺序展示多图

![这里写图片描述](https://img-blog.csdn.net/20180807223548255?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---3.4 Timeline：提供时间线轮播多张图

![这里写图片描述](https://img-blog.csdn.net/20180807223622206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

