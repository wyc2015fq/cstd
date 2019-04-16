# 用D3.js进行医疗数据可视化 （三）坐标轴 (Axes) - 沧海一粟 —— 技术随手记 - CSDN博客





2015年07月22日 20:34:52[慢游](https://me.csdn.net/eengel)阅读数：5558








# 介绍

这这篇文章中，我们来尝试一下改进坐标轴的呈现方式。所有的代码都是在之前[使用D3.js进行医疗数据可视化的一些实践 （二）](http://blog.csdn.net/eengel/article/details/47003229)中的代码基础上改的。因此这儿只把相应改动的代码贴出来。



# 任务一：让X轴的刻度以及相应的网格按月分布，并将刻度值改成中文




## 可视化效果

![](https://img-blog.csdn.net/20150722203226068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 代码



```java
var xAxis = d3.svg.axis()
                .scale(_x.range([0, quadrantWidth()]))
                .orient("bottom")
                .tickFormat(d3.time.format("%y年%m月"))  //set tick name in Chinese
                .ticks(d3.time.months, 1);  //set ticks for x axis as monthly
```




## 讨论

d3.svg.axis() [1]有好几个调整ticks的API。这儿为了让X轴的刻度按月分布，使用了axis().ticks(d3.time.months,1)。同样如果想按不同的时间间隔，D3提供了多种d3.time的API供选择。

改变X轴刻度值是通过调用axis().tickFormat(d3.time.format("%y年%m月"))完成的。这儿也体现了D3的灵活性，tickFormat可以接受一个函数作为规定刻度值呈现方式的方法。最重要的是，这个函数可以自己实现！



# 任务二：显示坐标轴名字

## 可视化效果

![](https://img-blog.csdn.net/20150722203239067?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 代码

```java
d3.select("g.x.axis")   //add label for x axis
        	.append("text")
            .attr("class", "axislabel")
            .attr("text-anchor", "end")
            .attr("x", quadrantWidth)
            .attr("y", 30)
            .text("日期");
```


```java
d3.select("g.y.axis")   //add label for y axis
	     	.append("text")
	        .attr("class", "axislabel")
	        .attr("text-anchor", "start")
	        .attr("x", -_margins.left)
	        .attr("y", -6)
	        .text("诊疗人次数（万人次）");
```




## 讨论

D3没有现成的方法显示坐标轴名字，所以需要我们自己创建一个<text>元素放置名字。唯一的关键就是要放对地方。比如上述实现中X轴的名字放在X轴的右下角，那么就需要设置相应的text-anchor, x, y值。Y轴的名字同理。

相应的布局示意如下。

![](https://img-blog.csdn.net/20150722203245600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



# 任务三：旋转X轴刻度值

## 可视化效果



![](https://img-blog.csdn.net/20150722204956240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 代码



```java
d3.selectAll("g.x g.tick text")
        	.attr("x", -24)
        	.attr("transform", "rotate(-30)");
```




## 讨论

这个方法可以改进刻度值的显示方式，特别是当图表的宽度不够而刻度值又无法缩减时，会起到很好的效果。

# 参考文献

1.    SVGAxes. [https://github.com/mbostock/d3/wiki/SVG-Axes](https://github.com/mbostock/d3/wiki/SVG-Axes)



