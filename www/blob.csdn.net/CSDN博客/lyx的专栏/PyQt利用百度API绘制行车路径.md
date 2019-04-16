# PyQt利用百度API绘制行车路径 - lyx的专栏 - CSDN博客





2017年05月18日 10:12:41[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：882








实验室师弟要做这个课题，所以写个demo作为参考。任务不太难，基本要求就是能够在Qt界面上根据车辆的起始经纬度，绘制出实际地图上的行车轨迹。

# 1.构建Qwebview控件。

![](https://img-blog.csdn.net/20170518095814953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


首先，我们qt的界面中插入QWebView控件。这个控件十分傻瓜，大家任务他就是一个浏览器就可以了。

这个控件有一个最核心的方法，就是load，相当于我们往一个浏览器中输入网址，然后回车。

# 2.获取百度API

百度API真心是越来越好了，还记得四五年前，那个时候界面和api说明可没有这么友好。

我们找到需要的绘制路径的demo。

![](https://img-blog.csdn.net/20170518100108001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其实，我们要的就是把右边这样的效果显示在qt界面上，而右面这个效果其实就是浏览器解释了左边的html和js代码。那么思路就是，我们在程序中修改左边的代码，然后交给上面创建的QWebView控件去显示就可以了。那么我们把左边这段代码复制到本地，保存为html文件。这里，我命名为ht.html。

# 3.显示

最后，就是把html文件交给webview来显示了，用的方法就是前面说的load。

上代码：



```python
# -*- coding: utf-8 -*-

import sys
import os
from PyQt4 import QtGui, uic, QtCore



class car_trace_plot(object):
    def __init__(self):
        qtCreatorFile = os.path.join('car_trace.ui')
        self.ui = uic.loadUi(qtCreatorFile)
        self.data_init()


    def data_init(self):
        self.ui.webView.load(QtCore.QUrl("./ht.html"))

    def show(self):
        self.ui.show()

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    ui = car_trace_plot()
    ui.show()
    sys.exit(app.exec_())
```


我们看到，代码很简单，其实核心的就一句：



```python
self.ui.webView.load(QtCore.QUrl("./ht.html"))
```
这里，ht.html就是复制下来的百度api代码，当然，要将里面的key替换成自己的appkey。

# 4.看一下ht.html文件



```
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
<style type="text/css">
body, html,#allmap {width: 100%;height: 100%;overflow: hidden;margin:0;font-family:"微软雅黑";}
</style>
<script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=2049b392dbb504c076890883bd92485a"></script>
<title>根据起终点经纬度驾车导航</title>
</head>
<body>
<div id="allmap"></div>
</body>
</html>
<script type="text/javascript">

// 百度地图API功能
var map = new BMap.Map("allmap");
map.centerAndZoom(new BMap.Point(116.404, 39.915), 11);

var p1 = new BMap.Point(116.301934,39.977552);
var p2 = new BMap.Point(116.508328,39.919141);

var driving = new BMap.DrivingRoute(map, {renderOptions:{map: map, autoViewport: true}});
driving.search(p1, p2);
</script>
```

这里，想要修改成我们希望的样子，字需要改变后面这段js代码中
`map.centerAndZoom(new BMap.Point(116.404, 39.915), 11);`
和



```
var p1 = new BMap.Point(116.301934,39.977552);
var p2 = new BMap.Point(116.508328,39.919141);
```
就可以了。前者是显示地图的中心点，后者是两个经纬度点，分别代表轨迹的初始位置。

那么，我们的显示工作，其实就是变成了修改这几个字符了，整个架构就完整了。

最后看一下这个demo的效果吧。

![](https://img-blog.csdn.net/20170518101022959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们其实可以加点按钮，然后让每条路径的生成有延时，那么看起来会更加好看（当然，这个demo只有一个trace）。















