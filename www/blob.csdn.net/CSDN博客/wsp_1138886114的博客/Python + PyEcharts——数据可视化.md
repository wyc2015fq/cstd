# Python + PyEcharts——数据可视化 - wsp_1138886114的博客 - CSDN博客





2018年05月31日 10:00:05[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：14907
所属专栏：[科学计算库与可视化](https://blog.csdn.net/column/details/23452.html)











- - - [一、第一个PyEcharts图标](#一第一个pyecharts图标)
- [二、柱形图/条形图](#二柱形图条形图)- - [Bar.add()方法签名](#baradd方法签名)


- [三、堆叠（柱状）图](#三堆叠柱状图)
- [四、折线/面积图](#四折线面积图)- - [Line.add()方法签名](#lineadd方法签名)


- [五、饼图](#五饼图)- - [Pie.add()方法签名](#pieadd方法签名)


- [六、散点图](#六散点图)- - [Scatter.add()方法签名](#scatteradd方法签名)


- [七 地图pyecharts地图数据接口](#七-地图pyecharts地图数据接口)- - [Map.add() 方法签名](#mapadd-方法签名)


- [八、词云图](#八词云图)- - [WordCloud.add() 方法签名](#wordcloudadd-方法签名)


- [九、图标叠加 Overlap](#九图标叠加-overlap)- - [Overlap.add() 方法签名](#overlapadd-方法签名)








### 一、第一个PyEcharts图标


`以下示例都是在jupyter notebook环境下运行`


> 
**1 安装**：$~~~~~~~~~~~~~~$`pip install pyecharts`
**2 引用pyecharts**`from pyecharts import Bar`
**3 编写实现柱状图**
```
bar = Bar('我的第一个图标','副标题')
bar.add('服装',['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子'],[5,20,36,10,75,90])
```

**4 输出到.html中进行展示:**`bar.render(.././html/bar01.html)`
**5 新建一个html文件**

```python
代码：
import pyecharts
from pyecharts import Bar

bar = Bar("我的第一个图表", "这里是副标题")
bar.use_theme('dark')                                  #暗色背景色
bar.add("服装",                                        #注解==label
        ["衬衫","羊毛衫","雪纺衫","裤子","高跟鞋","袜子"], #横坐标
        [5, 20, 36, 10, 75, 90])                       #纵坐标
bar.render('./picture1.html')                          #文件存储路径（默认保存当前文件路径）
```
`Bar`

![这里写图片描述](https://img-blog.csdn.net/20180627173249570?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 二、柱形图/条形图

> 
##### Bar.add()方法签名

```
add(
    name,x_axis,y_axis,
    is_stack = False,
    bar_category_gap = '20%',**kwargs
    )
name -> str         #图例名称
x_axis -> list      # X轴数据
y_axis -> list      # Y轴数据
is_stack -> bool    #数据堆叠，同个类目轴上系列配置相同的stack 值可以堆叠放置
bar_category_gap -> int/str   #类目轴柱状距离，默认20%
```

```python
代码： 
from pyecharts import Bar
bar = Bar('基本柱状图','副标题')
bar.add('服装',
        ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子'],
        [5,20,36,10,75,90],
        is_more_utils = True    #设置最右侧工具栏
        )
bar.show_config()               #调试输出pyecharts的js配置信息
bar.render('./html/first01.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180627163856460?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



### 三、堆叠（柱状）图

> 
```
代码：
from pyecharts import Bar

attr = ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子']
v1 = [5,20,36,10,75,90]
v2 = [10,25,8,60,20,80]
bar = Bar('柱状信息堆叠图')
bar.add('商家A',attr,v1,is_stack = True)  #is_stack = True才表示堆叠在一起
bar.add('商家B',attr,v2,is_stack = True)
bar.render('./html/first03.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180627173423525?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


```
■ 并列（柱形）图
from pyecharts import Bar

attr = ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子']
v1 = [5,20,36,10,75,90]
v2 = [10,25,8,60,20,80]
bar = Bar('标记线和标记示例')
bar.add('商家A',attr,v1,mark_point = ['average'])     #标记点：商家A的平均值
bar.add('商家B',attr,v2,mark_line = ['min','max'])    #标记线：商家B最小/最大值
bar.render('./html/first04.html')
```

![这里写图片描述](https://img-blog.csdn.net/20180627164831939?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
■ 横向并列（柱形）图 
from pyecharts import Bar

attr = ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子']
v1 = [5,20,36,10,75,90]
v2 = [10,25,8,60,20,80]
bar = Bar('X 轴与 Y 轴交换')
bar.add('商家A',attr,v1)  
bar.add('商家B',attr,v2,is_convert = True)    # is_convert = True:X 轴与 Y 轴交换
bar.render('./html/first04.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180627174120917?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 四、折线/面积图

> 
##### Line.add()方法签名

```
add(
    name,x_axis,y_axis,
    is_symbol_show = True,
    is_smooth = False,
    is_stack = False,
    is_step = False,
    is_fill = False,**kwargs    
    )
以下为属性默认值：
    is_symbol_show = True,      #是否显示标记图形
    is_smooth = False,          #是否显示平滑曲线
    is_stack = False,           #是否数据堆叠
    is_step = False,            #是否是阶梯线
    is_fill = False,**kwargs    #是否填充曲线区域面积
```

```
▶代码1：   
from pyecharts import Line

attr = ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子']
v1 = [5,20,36,10,75,90]
v2 = [10,25,8,60,20,80]
line = Line('折线示例图')
line.add('商家A',attr,v1,mark_point = ['average'])    
line.add('商家B',attr,v2,is_smooth = True, mark_line = ['max','average'])
line.render('./html/first05.html')
 --------------------------------------------------------------
▶代码2：(定制折线图)    
from pyecharts import Line

attr = ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子']
v1 = [5,20,36,10,75,90]
v2 = [10,25,8,60,20,80]
line = Line('折线示例图')
line.add('商家A',attr,v1,
    mark_point = ['average','max','min'],  #标注点：平均值，最大值，最小值
    mark_point_symbol = 'diamond',         #标注点：钻石形状
    mark_point_textcolor = '#40ff27')      #标注点：标注文本颜色
line.add('商家B',attr,v2,mark_point = ['average','max','min'],
    mark_point_symbol = 'arrow',
    mark_point_symbolsize = 40)
line.render('./html/line01.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180627165126572?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


```
▶代码3：(面积图)  
from pyecharts import Line

attr = ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子']
v1 = [5,20,36,10,75,90]
v2 = [10,25,8,60,20,80]
line = Line('折线面积示例图')
line.add('商家A',attr,v1,is_fill = True,
    line_opacity = 0.2,      #线条不透明度
    area_opacity = 0.4,
    symbol = None)  
line.add('商家B',attr,v2,is_fill = True,
    line_color = '#000',     #黑色
    area_opacity = 0.3,      #填充不透明度
    is_smooth = True)
line.render('./html/area01.html')
```


![这里写图片描述](https://img-blog.csdn.net/2018062717031874?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 五、饼图

> 
##### Pie.add()方法签名

```
add(name,attr,value,radius = None,center = None,rosetype = None,**kwargs)
attr:属性名称
radius：饼图半径，数组第一项是内径，第二项是外径，默认[0,75,],设置成百分比
center：圆心，数组第一项是X轴，第二项是Y轴，默认[50,50]
rosetype: 是否展示成南丁格尔图，用过半径区分数据大小，radius和area两种模式，默认radius
```

```python
▶代码1：(饼图)
from pyecharts import Pie

attr = ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子']
v1 = [5,20,36,10,75,90]
pie = Pie('饼图示例')
pie.add('',attr,v1,is_label_show = True)
pie.render('./html/pie01.html') 

▶代码2：(环形图)
from pyecharts import Pie

attr = ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子']
v1 = [5,20,36,10,75,90]
pie = Pie('饼图-环形图示例',title_pos = 'center')
pie.add(
        '',attr,v1,                 #''：图例名（不使用图例）
        radius = [40,75],           #环形内外圆的半径
        is_label_show = True,       #是否显示标签
        label_text_color = None,    #标签颜色
        legend_orient = 'vertical', #图例垂直
        legend_pos = 'left'
        )
pie.render('./html/pie02.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180627170624805?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



### 六、散点图

> 
##### Scatter.add()方法签名

```
add(name,x_axis,y_axis,extra_data = None,symbol_size = 10,**kwargs)
extra_data -> int:第三维数据（可在 visualmap 中将视图元素映射到第三维度）
symbol_size -> int: 标记图形大小，默认为10
```

```
▶代码1：
from pyecharts import Scatter
v1 = [5,20,35,50,65,80]
v2 = [10,20,30,40,50,60]
scatter = Scatter('散点示例图')
scatter.add('A',v1,v2)
scatter.add('B',v1[::-1],v2)    #v1[::-1]代表切片倒序
scatter.render('./html/scatter01.html')     

▶代码2：(引入第三维/类似气泡图)
from pyecharts import Scatter
v1 = [5,20,35,50,65,80]
v2 = [10,20,30,40,50,60]
scatter = Scatter('散点-气泡示例图')
scatter.add('A',v1,v2)
scatter.add('B',v1[::-1],v2,                #v1[::-1]代表切片倒序
            is_visualmap = True,            #显示滑动条
            symbol_size = 30,               #显示图内标点大小
            vasual_range_size = [20,80])    #显示滑动范围
scatter.render('./html/scatter02.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180627180520146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



### 七 地图[pyecharts地图数据接口](http://pyecharts.org/#/zh-cn/datasets)

> 
##### Map.add() 方法签名

```
安装下列地图数据包
 pip install echarts-countries-pypkg
 pip install echarts-china-provinces-pypkg
 pip install echarts-china-cities-pypkg
 pip install echarts-china-counties-pypkg
 pip install echarts-china-misc-pypkg
 pip install echarts-united-kingdom-pypkg
```

```
add(
    name,attr,value,
    maptype = 'china',
    is_roam = True,
    is_map_symobol_show = True 
    **kwargs
    )
maptype -> str: 地图类型，支持China，world，北京，天津，上海，湖南，湖北，……363个二线城市
is_roam -> bool/str 是否开启鼠标缩放，漫游等，默认 True,
                    若只想开启缩放/平移 设置scale/move 设置成 True 开启
is
is_map_symobol_show 是否显示地图标记，默认 True。
```

```
▶代码1：
from pyecharts import Map
value = [155,10,66,78]
attr = ['福建','山东','北京','上海']
map = Map('全国地图示例',width = 1200,height = 600)
map.add('',attr,value,maptype = 'china')
map.render('./html/map01.html')

▶代码2：
from pyecharts import Map
value = [155,10,66,78]
attr = ['汕头市','汕尾市','揭阳市','肇庆市']
map = Map('广东地图示例',width = 1200,height = 600)
map.add('',attr,value,maptype = '广东',
        is_visualmap = True,
        visual_text_color = '#000',
        is_label_show = True
        )
map.render('./html/map02.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180628092325269?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



### 八、词云图

> 
##### WordCloud.add() 方法签名

```
add(
    name,attr,value,
    shape = 'circle',
    word_gap = 20,
    word_size_range = None
    rotate_step = 45
    )
shape-> list :词云图轮廓（circle,cardioid,diamond,triangle-forward,triangle,pentagon,star）
word_gap -> int 单词间隔 默认 20
word_size_range -> int 单词字体大小范围 默认[12,60]
rotate_step -> int 单词旋转角度，默认45。
```

```
from pyecharts import WordCloud
name = [
        'Though','the answer','this question',
        'may at first','seem to border','on the',
        'absurd','reflection','will show','that there',
        'is a','good deal','more in','it than meets','the eye'
        ]
value = [10000,6189,4556,2356,2233,
         1895,1456,1255,981,875,
         542,462,361,265,125]
worldcloud = WordCloud(width = 1300,height = 620)
worldcloud.add('',name,value,word_size_range = [20,100])
worldcloud.render('./html/worldcloud01.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180628094501512?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



### 九、图标叠加 Overlap

> 
自定义结合`Line/ Bar/ Kline/ Scatter/ EffectScatter` 等图表。 

  利用第一张图表为基础，将后面的数据画在第一张图上。

##### Overlap.add() 方法签名

```
add(chart,
    xaxis_index = 0,
    yaxis_index = 0,
    is_add_xaxis = False,
    is_add_yaxis = False)
属性：
is_add_xaxis / is_add_yaxis 是否新增坐标X/Y 默认 False
```

```
▶代码1：bar + line 叠加

from pyecharts import Bar,Line,Overlap
attr = ['A','B','C','D','E','F']
v1 = [10,20,30,40,50,60]
v2 = [38,28,35,58,65,70]
bar = Bar('Line - Bar示例')
bar.add('bar',attr,v1)
line = Line()
line.add('line',attr,v2)

overlop = Overlap()
overlop.add(bar)
overlop.add(line)
overlop.render('./html/line-bar01.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180628095441520?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


```
▶代码2：EffectScatter + Line 叠加
from pyecharts import Line,EffectScatter,Overlap
attr = ['衬衫','羊毛衫','雪纺衫','裤子','高跟鞋','袜子']
v1 = [5,20,36,10,10,90]
line = Line('线性_闪烁图示例')
line.add('',attr,v1,is_random = True)

es = EffectScatter()
es.add('',attr,v1,effect_scale=8)   #闪烁
overlop = Overlap()
overlop.add(line)                   #必须先添加line,在添加es
overlop.add(es)
overlop.render('./html/line-es01.html')
```


![这里写图片描述](https://img-blog.csdn.net/20180705153328878?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)









