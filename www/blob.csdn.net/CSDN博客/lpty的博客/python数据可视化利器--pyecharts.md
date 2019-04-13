
# python数据可视化利器--pyecharts - lpty的博客 - CSDN博客

2017年08月03日 18:42:30[lpty](https://me.csdn.net/sinat_33741547)阅读数：15116



## 前言
[echarts](http://echarts.baidu.com)是什么？下面是来自官方的介绍：
ECharts，缩写来自Enterprise Charts，商业级数据图表，一个纯Javascript的图表库，可以流畅的运行在PC和移动设备上，兼容当前绝大部分浏览器（IE6/7/8/9/10/11，chrome，firefox，Safari等），底层依赖轻量级的Canvas类库ZRender，提供直观，生动，可交互，可高度个性化定制的数据可视化图表。创新的拖拽重计算、数据视图、值域漫游等特性大大增强了用户体验，赋予了用户对数据进行挖掘、整合的能力。
支持折线图（区域图）、柱状图（条状图）、散点图（气泡图）、K线图、饼图（环形图）、雷达图（填充雷达图）、和弦图、力导向布局图、地图、仪表盘、漏斗图、事件河流图等12类图表，同时提供标题，详情气泡、图例、值域、数据区域、时间轴、工具箱等7个可交互组件，支持多图表、组件的联动和混搭展现。
## 用法
使用echarts还是需要一定的前端知识，这里介绍一个python包–pyecharts，利用几行代码轻松生成echarts风格的图表。
## 安装
`pip install pyecharts`
## 实例
`from pyecharts import Bar
attr = ["{}month".format(i) for i in range(1, 13)]
attr = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
v1 = [2.0, 4.9, 7.0, 23.2, 25.6, 76.7, 135.6, 162.2, 32.6, 20.0, 6.4, 3.3]
v2 = [2.6, 5.9, 9.0, 26.4, 28.7, 70.7, 175.6, 182.2, 48.7, 18.8, 6.0, 2.3]
bar = Bar("Bar chart", "precipitation and evaporation one year")
bar.add("precipitation", attr, v1, mark_line=["average"], mark_point=["max", "min"])
bar.add("evaporation", attr, v2, mark_line=["average"], mark_point=["max", "min"])
bar.render()`CSDN的Markdown模式，上传图片总是失败，我就不贴图了：[效果点这里](https://user-images.githubusercontent.com/19553554/35388262-078a4afc-020e-11e8-8acc-cc7bc8a4e8a6.gif)
[官方文档](http://pyecharts.org/#/zh-cn/)还有许多很赞的模板，写的也很好，看文档吧。

