# R语言学习笔记︱Echarts与R的可视化包——地区地图 - 素质云笔记/Recorder... - CSDN博客





2016年05月30日 20:21:46[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：24917












笔者寄语：感谢CDA DSC训练营周末上完课，常老师、曾柯老师加了小课，讲了echart与R结合的函数包recharts的一些基本用法。通过对比谢益辉老师GitHub的说明文档，曾柯老师极大地简化了一些代码，可读性很强。






    关于此包起源，百度联姻d3.js=echarts，echarts+R=[recharts](https://github.com/taiyun/recharts)包（Yang Zhou和Taiyun Wei），谢益辉老师修改可以传递js参数，实现更多功能，




但是呢，谢益辉老师的改良版包还没发出来，于是该神就做了一个函数，先给大家试用（点赞谢益辉老师![技术分享](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)）。

    该函数叫echartR,实质是将Echarts参数封装成list，大多数功能还是和echart函数一样。




**    要想使用该函数需要：从github加载[recharts](https://github.com/taiyun/recharts)包+echartR函数+一些全局设定**




    该包的下载、安装以及echartR函数的下载，看谢益辉老师的GitHub主页，贴网址：https://github.com/madlogos/recharts

    动态图可看：http://madlogos.github.io/recharts/files/echartR.html#force

echartR.R的地址：链接：http://pan.baidu.com/s/1mhASmhE 密码：yq76

——————————————————————————————




**号外 号外 新消息（更新于20161002）：**



**谢老师已经把echartR托管到github上了**，可以调用。同时，




谢老师已经把函数封装好，放在github中，可以下载调用。

官方链接：http://madlogos.github.io/recharts/index_cn.html

地图部分的示例 http://madlogos.github.io/recharts/Basic_Plots_31_Map.html








注意:本加载包仍在开发完善中。recharts基于百度Echarts2的最后一个稳定发布版(v2.2.7)开发。本文档始终反映recharts最新的特性([Github](https://github.com/madlogos/recharts))。安装方法：




```
if (!require(devtools)) library(devtools)
install_github("madlogos/recharts")
```




# 1 知识地图Knowledge Map
- 基本图类- 单个坐标系- 直角坐标系- [散点图Scatter/气泡图Bubble Plot](http://madlogos.github.io/recharts/Basic_Plots_01_Scatterplot_cn.html)
- [条图Bar/柱图Column Chart](http://madlogos.github.io/recharts/Basic_Plots_02_Bar_cn.html)
- [线图Line/面积图Area Chart](http://madlogos.github.io/recharts/Basic_Plots_03_Line_cn.html)
- [蜡烛图/K线图Candlestick/K Chart](http://madlogos.github.io/recharts/Basic_Plots_04_K_cn.html)
- [事件河流图Event River Chart](http://madlogos.github.io/recharts/Basic_Plots_05_eventRiver_cn.html)

- 其他- [力导向布局图Force Chart](http://madlogos.github.io/recharts/Basic_Plots_11_Force_cn.html)
- [和弦图Chord Chart](http://madlogos.github.io/recharts/Basic_Plots_12_Chord_cn.html)
- [词云Word Cloud](http://madlogos.github.io/recharts/Basic_Plots_13_WordCloud_cn.html)
- [韦恩图Venn Chart](http://madlogos.github.io/recharts/Basic_Plots_14_Venn_cn.html)
- [热力图Heatmap](http://madlogos.github.io/recharts/Basic_Plots_15_Heatmap_cn.html)


- 多个坐标系- 极坐标系- [饼图Pie/环状图Ring/玫瑰图Rose Chart](http://madlogos.github.io/recharts/Basic_Plots_21_Pie_cn.html)
- [漏斗图Funnel/金字塔图Pyramid Chart](http://madlogos.github.io/recharts/Basic_Plots_22_Funnel_cn.html)
- [雷达图Radar Chart](http://madlogos.github.io/recharts/Basic_Plots_23_Radar_cn.html)
- [仪表盘Gauge Chart](http://madlogos.github.io/recharts/Basic_Plots_24_Gauge_cn.html)

- 其他- [地图Map](http://madlogos.github.io/recharts/Basic_Plots_31_Map_cn.html)
- [树图Tree Chart](http://madlogos.github.io/recharts/Basic_Plots_32_Tree_cn.html)
- [矩形树图Treemap](http://madlogos.github.io/recharts/Basic_Plots_33_Treemap_cn.html)



- [混合图类](http://madlogos.github.io/recharts/Mixed_Plots_cn.html)
- 坐标轴/控件配置- 调整数据系列- [设置系列`setSeries`](http://madlogos.github.io/recharts/setSeries_cn.html)

- 坐标轴/制图区- [设置坐标轴`setAxis`](http://madlogos.github.io/recharts/Widget_01_Axis_cn.html)
- [设置极坐标系`setPolar`](http://madlogos.github.io/recharts/Widget_02_Polar_cn.html)
- [设置绘图区`setGrid`](http://madlogos.github.io/recharts/Widget_03_Grid_cn.html)

- 图形元件- [添加标注线`addMarkLine`](http://madlogos.github.io/recharts/Widget_11_markLine_cn.html)
- [添加标注点`addMarkPoint`](http://madlogos.github.io/recharts/Widget_12_markPoint_cn.html)

- 控件- [设置标题`setTitle`](http://madlogos.github.io/recharts/Widget_21_Title_cn.html)
- [设置图例`setLegend`](http://madlogos.github.io/recharts/Widget_22_Legend_cn.html)
- [设置时间轴`setTimeline`](http://madlogos.github.io/recharts/Widget_23_Timeline_cn.html)
- [设置工具箱`setToolbox`](http://madlogos.github.io/recharts/Widget_24_Toolbox_cn.html)
- [设置值域选择`setDataRange`](http://madlogos.github.io/recharts/Widget_25_DataRange_cn.html)
- [设置缩放漫游`setDataZoom`](http://madlogos.github.io/recharts/Widget_26_DataZoom_cn.html)
- [设置地图漫游`setRoam`](http://madlogos.github.io/recharts/Widget_27_RoamController_cn.html)

- 样式特性- [样式`aesStyle`](http://madlogos.github.io/recharts/aesStyle_cn.html)


- 美工- [设置提示框`setTooltip`](http://madlogos.github.io/recharts/Widget_31_Tooltip_cn.html)
- [设置符号`setSymbols`](http://madlogos.github.io/recharts/Widget_32_SymbolList_cn.html)
- [设置主题`setTheme`](http://madlogos.github.io/recharts/Widget_33_Theme_cn.html)

- [定制化](http://madlogos.github.io/recharts/Cust_Plots_cn.html)




——————————————————————————————




# 一、基本语法



```
echart(data=数据, x=~x轴变量,y=~y轴变量,type=‘scatter’, palette=调色盘颜色,title=主标题
,subtitle=副标题,xAxis=x轴选项,yAxis=y轴选项,markLine=标记线,markPoint=标记点,等)
```
    该语法使用来源上课讲义之中，具体的关于散点图、气泡图、饼图、玫瑰图啥的看谢老师主页，主要上excel不太好实现的地图。


地图的类型有三类：区域标注、点标注、线标注（数据结构最复杂），举区域标准，关于点标注以及现标准给出与区域标准不同的地方。

    画地图最麻烦的就是数据整理，如果要填写点/线就得重构一些数据。



——————————————————————————————





# 二、区域标注




主要就输入数据类型+函数+加入点/线三个部分。




## 1、区域标注输入数据结构




![技术分享](https://img-blog.csdn.net/20160530202359911)





几个省份，2012,2013,2014三年的GDP数值（数据来源查看谢老师主页），区域标注的数据结构最为简单易懂。

其中省份数据以及年的数据，需要变成因子型，谢老师使用的方法是：





`as.factor(enc2native(dtgdp$Prov))`



这个enc2native用得很有意思，用来解决“中文转码在Windows里一直是老大难”的问题，读取或设置字符向量的编码。






## 2、函数






```
echartR(dtgdp, x = ~Prov, y = ~GDP, series= ~Year, 
        type=c('map','china','area'), palette='gdocs',
        title="GDPs of China Provinces, 2012-2014 (Million USD)",
        subtitle='(source: Wikipedia)',
        subtitle_url="https://raw.githubusercontent.com/madlogos/Shared_Doc/master/Shared_Documents/ChinaGDP.txt",
        dataRangePalette=c('red','orange','yellow','green','limegreen'),
        dataRange=c('High',"Low"),pos=list(toolbox=3),markPoint=top3)
```




函数解读：series代表分类标签（此时为年）；



type是设置中国地图、世界地图（world）、区域标注（area）、点标注(point)；

palette调色板，笔者觉得该函数是地图上点的颜色，可用许多调色板样式 Including ‘aetnagreen‘, ‘aetnablue‘, ‘aetnaviolet‘, ‘aetnaorange‘, ‘aetnateal‘, ‘aetnacranberry‘；

dataRangePalette: 如打开数据漫游，可单独指定漫游色板(同palette功能)，否则采用Echarts默认值，笔者觉得该调色函数，才是最主要的，而且不仅仅局限在“red”、"yellow"，还可以调色RGB中许多颜色类型，比如#FF6A6A（浅红色），详情可查[《RGB颜色查询对照表》](http://www.114la.com/other/rgb.htm)。


dataRange代表左下角那个，high/low；

toolbox=3代表右下角那一列，小铅笔，保存图片的小按钮，3是右下角，1是右上角。







## 3、加点函数markPint




比如加一个浙江省GDP的点，c(浙江，浙江，10000，x，y，T)=（向量名称，点集名称，浙江GDP数值，浙江x坐标，浙江y坐标，是否亮晶晶）

此时谢老师案例中要加入广东、江苏、山东三个省份的点，于是先构成一个data.frame名字叫top3的数据框，注意series以及Prov是字符型。




![技术分享](https://img-blog.csdn.net/20160530202830465)





在函数中，echartR(...,markPoint=top3)调用markPoint即可，还是一样加点的数据整理麻烦，调用很快。

![技术分享](https://img-blog.csdn.net/20160530202429583)











## 4、世界地图




贴一下需要的数据结构以及函数中与前面中国省份不同之处即可。




![技术分享](https://img-blog.csdn.net/20160530202509739)





世界地图举例的数据很简单，一目了然。下面的函数也很中国地图主要不同之处：type=c("map","world","area")。以下为压缩代码，详情请看谢老师主页。





`echartR(worldgdp, x = ~country, y = ~GDP, type=c('map','world','area'))`![技术分享](https://img-blog.csdn.net/20160530202532196)




——————————————————————————————





# 三、点标注




跟区域不同的地方，就是要加入点的坐标（x,y），贴一下数据结构以及函数的不同之处




![技术分享](https://img-blog.csdn.net/20160530202610306)





如果要加点markPoint，数据结构变成下面的图中所示的一样，注意name，city是字符型，effect代表是否有酷炫特效。




![技术分享](https://img-blog.csdn.net/20160530202746449)





函数的不同之处，主要在typle=c("map","china","point")，以下代码为省略版，只是为了看出不同的，详细的回看谢老师的主页。






```
echartR(chinapm25, x=~City, y=~PM25, xcoord=~xcoord, ycoord=~ycoord,
        type=c('map','china','point'),markPoint=top5)
```
![技术分享](https://img-blog.csdn.net/20160530202648365)







——————————————————————————————







# 四、线标注




数据结构如下图，又多了from（起点），to(终点）.

线标注地图比较特殊，数据集要包括y（数量）、x（起点地名）、x1（终点地名）、series（数据系列）、xcoord（起点纬度）、ycoord（起点经度）、xcoord1（终点纬度）、ycoord1（终点经度）。额外标注的话，makeline格式进行了扩充





![技术分享](https://img-blog.csdn.net/20160530202916882)





图中的数据y为空值，说明该线路默认只有一条航班。

加点的数据格式为：




![技术分享](https://img-blog.csdn.net/20160530204214480)





加线格式：




![技术分享](https://img-blog.csdn.net/20160530213939455)





函数为没啥大区别。






```
echartR(flight, x=~From, x1=~To, y=~y, series=~From, xcoord=~Xcoord.x, ycoord=~Ycoord.x,
        xcoord1=~Xcoord.y, ycoord1=~Ycoord.y, type=c('map','china','line'),
        pos=list(toolbox=3), title="南方航空公司主要航班线路",markPoint=Tier1)
```

延伸，要实现以下的内容，需要实现以下几步：


设置主题暗色系、设置新的点数据makepoint、线数据makeline

笔者在自己玩的时候，出现了一个问题，

设置好数据之后，左下角的high-low坐标尺度，一直维持在很高的水平，所以地图中只有一种颜色。




![技术分享](https://img-blog.csdn.net/20160530203318175)






——————————————————————————————




# 延伸一：R语言百度地图接口包






参考CSDN博客：[R语言百度地图接口包](http://m.blog.csdn.net/article/details?id=39401547&from=groupmessage&isappinstalled=0)

1、[下载地址](http://download.csdn.net/detail/trueman007/7944093)

[](http://download.csdn.net/detail/trueman007/7944093)[http://download.csdn.net/detail/trueman007/7944319](http://download.csdn.net/detail/trueman007/7944319)





2、安装方法：


`install.packages('你的目录/RbaiduMaps_0.1.tar.gz', type='source')`




3、运行方法：


`library('RbaiduMaps')`





4、简单画地图：



```
library(png)
mp<-GetMapBD("上海",size=c(640,640),zoom=10)
PlotOnStaticMapBD(mp)
```




![](https://img-blog.csdn.net/20170205180208656)




