# Highcharts图表结构分析：详解坐标轴及其类型 - z69183787的专栏 - CSDN博客
2014年03月05日 11:43:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2584
Highcharts所有的图表除了饼图都有X轴和Y轴，默认情况下，x轴显示在图表的底部，y轴显示在左侧（多个y轴时可以是显示在左右两侧），通过chart.inverted = true 可以让x，y轴显示位置对调。下图为图表中坐标轴组成部分。
![](http://hiphotos.baidu.com/exp/pic/item/ac2fc3c451da81cbbe1d84a55066d0160824319b.jpg)
一、坐标轴组成部分
轴标题-Axis Title
坐标轴标题。默认情况下，x轴为null（也就是没有title），y轴为'Value'，设置坐标轴标题的代码如下：
xAxis:{
title:{
text:'x轴标题'
}
}
yAxis:{
title:{
text:'y轴标题'
}
}
更多关于Axis Title属性请查看API文档相关内容 xAxis.title、yAxis.title。
轴标签-Axis Labels
坐标轴标签（分类）。Labels常用属性有enabled、formatter、setp、staggerLines。
1、开启
是否启用Labels。x，y轴默认值都是true，如果想禁用（或不显示）Labels，设置该属性为false即可。
2、格式化程序
标签格式化函数。默认实现是：
formatter:function(){ returnthis.value; }
this.value代码坐标轴上当前点的值（也就是x轴当前点的x值，y轴上当前点的y值），除了value变量外，还有axis、chart、isFirst、isLast可用。例如调用this.isFirst的结果如下图所示：
![](http://hiphotos.baidu.com/exp/pic/item/4651a712c8fcc3cebb0dce5b9045d688d53f204d.jpg)
自定义标签格式1
另外一个例子，实现更高级的自定义格式化函数，截图如下：
![](http://hiphotos.baidu.com/exp/pic/item/d57e9994a4c27d1ead013dac19d5ad6eddc43877.jpg)
自定义标签格式2
实现代码如下：
yAxis: {       
labels: {
formatter:function(){
if(this.value <=100) {
return"第一等级("+this.value+")";
}elseif(this.value >100 && this.value <=200) {
return"第二等级("+this.value+")";
}else{
return"第三等级("+this.value+")";
}
}
}
Labels显示间隔，数据类型为number（或int）。下图说明了step的用法和作用：
![](http://hiphotos.baidu.com/exp/pic/item/b853d6fcc3cec3fd843da11dd488d43f87942713.jpg)
Y Lables width step
4、staggerLines
水平轴Lables显示行数。（该属性只对水平轴有效）当Lables内容过多时，可以通过该属性控制显示的行数。和该属性相关的还有maxStaggerLines属性。
更多关于Lables的属性请查看API文档 xAxis.labels、yAxis.labels。
轴刻度-Axis Tick
Tick为坐标轴刻度。默认情况下x轴刻度高(tickLength属性)为5px，宽为1px；y轴宽为0px(也就是不显示刻度)。Tick相关的属性主要有tickLength、tickWidth、tickColor、tickInterval、tickmarkPlacement。
1、tickLength、tickWidth、tickColor
分别代表刻度线的长度、宽度、颜色。
2、tickInterval
刻度间隔。其作用和Lables.step类似，就是不显示过多的x轴标签内容，不同的是，tickInterval是真正意义上的调整刻度，而Lables.step只是调整Labels显示间隔。所以在实际应用中，tickInterval用的多。
针对不同数据类型的坐标轴有不同的默认值。对于线性数据和Datetime类型数据，其默认值是tickPixelInterval值，对于Categorty表示间隔一个category。
3、tickmarkPlacement
刻度线对其方式，有between和on可选，默认是between。设置为on后的变化如下图：
![](http://hiphotos.baidu.com/exp/pic/item/4651a712c8fcc3ceb47fcf5b9045d688d53f20df.jpg)
xAxis tickmarkPlacement on
更多关于Tick的属性请查看API文档。
网格线-Grid Line
坐标轴网格线。默认情况下，x轴网线宽度为0,y轴网格线宽度为1px。网格线共有三个属性可设置，分别是:gridLineWidth、gridLineColor、gridLineDashStyle。
1、gridLineWidth
网格线宽度。x轴默认为0，y轴默认为1px。
2、gridLineColor
网格线颜色。默认为：#C0C0C0。
3、gridLineDashStyle
网格线线条样式。和Css border-style类似，常用的有：Solid、Dot、Dash。
下图为自定义x和y轴的gridLines效果图：
![](http://hiphotos.baidu.com/exp/pic/item/6398ecd3572c11df7c85b94b612762d0f603c2c7.jpg)
自定义网格线
多元轴-Multiple Axis
多个轴。在Highcharts中， 坐标可以是多个，最常见的是多个y轴。多轴存在时，Axis是一个数组，而在赋值时，通过Axis数组的下标与数据关联。如下图所示：
![](http://hiphotos.baidu.com/exp/pic/item/0865b518972bd407bb71d59579899e510eb309e0.jpg)
多元轴-Multiple Axis
总结如下：
Series中设置每个轴值时，用轴数组下标关联。
设置opposite: true表示该轴位置反转，即为y轴时显示在右侧，为x轴时显示在顶部（和正常情况下x轴在下，y轴左构成反转）。
在Series中可以设置该轴的类型，多个轴不同的类型（例如：type:'spline'），就构成了多种图表并存的混合图。
二、坐标轴类型
坐标轴中，可以通过Type指定坐标轴类型，有linear、logarithmic、datetime、category可选，默认是：linear。指定类型的实例代码如下：
// The types are 'linear', 'logarithmic' and 'datetime' yAxis: { type: 'linear', } // Categories are set by using an array xAxis: { categories: ['Apples', 'Bananas', 'Oranges'] }
linear
线性轴。默认类型，x轴按照Axis.tickInterval值增长，y轴默认是自适应。
logarithmic
对数轴。按照数学中的对数增长，例如1,2,4,8... 用的不多，主要用于对数图表，实例请查看在线演示平台的对数直线图。
datetime
时间轴。时间使用和Javascript 日期对象一样，即用一个距1970年1月1日0时0分0秒的毫秒数表示时间，也就是时间戳。更多Javascript 日期对象请阅读 W3C school 相关内容。
Highcharts有很多时间格式化函数，列举如下：
1、Date.getTime()
获取当前时间戳。实例用法如下： 
time = Date.getTime(); //time = 1384442746960 (ms) 当前时间为 2013-11-14 23:25:46
2、Date.UTC(year,month,day,hours,minutes,seconds,millisec)
通过UTC方式获取指定时间的毫秒数，例如获取 2013-11-14 00:00:00的毫秒数代码如下：
time = Date.UTC(2013,11,14,0,0,0,0); // time = 1386979200000 (ms);
3、Highcharts.dateFormat(String format)
Highcharts时间格式化函数，同 PHP格式化函数。具体用法参考API文档 Highcharts.dateFormat() ，当然，在本教程的《函数使用》章节中具体讲解。
category
数组轴。用的最多也最简单，这里就不多说。
