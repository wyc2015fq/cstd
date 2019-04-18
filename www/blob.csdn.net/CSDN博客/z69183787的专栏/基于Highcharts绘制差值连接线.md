# 基于Highcharts绘制差值连接线 - z69183787的专栏 - CSDN博客
2014年03月03日 16:45:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1114
话说图形我们一般用于了解趋势, 快速定位问题所在, 但想要对数值进行精确评估, 还是直白的表格 + 数字好了.
但这样问题就来了, 如果我们既想了解趋势, 又想了解具体数值的差异, 那该怎么办呢?
是用柱状图 + 表格吗?
我觉得还是这样的好! 让用户选择图形上的点, 自动比较两点的差值, 给用户最直观的感受.
between-points example
想法有了, 不知道是不是有人做过这样的东西. 搜索一番后, 貌似没有发现, 也可能是我孤陋寡闻吧, 看来只能自己写一个了
图形我就不自己写了, 已经有做的非常好的, 例如[Highcharts](http://highcharts.com/)
那么我来扩展下Highcharts, 做一个plugin好了...
于是between-points诞生了
通过点击Highcharts X-Y轴系列图形(如line, area, column, bar)上的点来获取差值, 以连接线的形式来展现他们的关系.
更多信息请查看[between-points](http://ufologist.github.io/between-points/)
