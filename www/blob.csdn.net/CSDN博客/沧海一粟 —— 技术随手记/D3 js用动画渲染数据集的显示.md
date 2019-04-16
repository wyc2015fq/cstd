# D3.js用动画渲染数据集的显示 - 沧海一粟 —— 技术随手记 - CSDN博客





2016年07月22日 18:30:44[慢游](https://me.csdn.net/eengel)阅读数：2208











# 介绍

数据统计和数据分析离不开数据集。之前几篇博客（[用D3.js进行医疗数据可视化 （一）折线图 (Line Chart)](http://blog.csdn.net/eengel/article/details/46958643) 等）基于的是国家卫生和计划生育委员会统计信息中心的数据，可以说是已经经过了加工，提炼总结出来的数据。而本文涉及的是原始数据集。如果按医疗数据来说，可以是电子病历数据，个人健康数据等。但这儿重点介绍动画效果的实现，不涉及任何与特定数据集有关的内容。

前一段时间有点闲暇时间，想玩一玩D3.js中的动画效果。于是立马想到一个经常碰到的场景：原始数据集的可视化。目前有很多网站允许用户上传数据集，然后提供数据管理甚至分析的服务。在展现原始数据集的时候，大多用表格形式来呈现。一般来说，数据集的加载需要一点时间，所以这个过程如果能做的有趣一点，还是一件挺好玩的事儿。

可做的好玩儿的比如原本数据集（以2维为例）就是行X列规规矩矩排排站好，那进入的时候能不能模拟真实世界中站队的规则：一队队先自己排好，然后集合到一块固定的场地。再比如，集合完毕后，把特殊的位置标注出来，比如有缺失数据的地方。等等。

于是就做了一个简单的程序模拟这一过程。效果如下图。我用Chrome插件Gif Cat抓的gif，初始状态不知道为什么没抓下来。当个参考吧。

![](https://img-blog.csdn.net/20160722182932666?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原理很简单，需要源码的可以从[这儿](https://github.com/slowlyswimming/javascript-visualization/tree/master/dataset-animation)下载。

稍微展开说下目前的实现。




# 加载数据 Load Data

关键的源码见下面。



```java
var circles = column.selectAll("circle")
               .data(data[j].data)
               .enter()
               .append("circle")
               .transition()
               .duration(1000)
               .each("start", function(d) {
                     if(d.value == 1){
          d3.select(this)
              .attr("fill", "white")
              .attr("r", 2)
              .attr("cx", width);                       
                     }else if(d.value == 0){
                          d3.select(this)
             .attr("fill", "#222")
              .attr("cx", width);  
                     }
     })
     .delay(function() {
          return j * (3000/datalen);
     })
.ease(easefuns[2]);
         
          circles
               .attr("cx", xScale(data[j].name))
               .attr("cy", function(d, i) {
                     return yScale(d.caze);
               })
               .attr("class", function(d){
                     return"circle" + d.value;
               })
               .each("end", function(d) {
                     if(d.value == 1){
          d3.select(this)
              .transition()
              .duration(500)
              .attr("fill", "white")
              .attr("r", 1);                      
                     }else if(d.value == 0){
                          d3.select(this)
              .transition()
              .duration(500)
              .attr("fill", "#222")
              .attr("r", 1);
                     }
     });
 
     }
```





加载数据时的动画有几个关键点：

1.    每个点进入屏幕之前的状态，由circles. each("start",function(d) {…})来决定。比如这儿有值的点就渲染成白色大圆点；而缺失值的点就是隐藏的（颜色与背景色相同）。

2.    点从屏幕边缘飞到最终位置的节奏，由circles.delay()和circles.ease()共同决定。circles.delay()设定点进入屏幕的延迟时间。为了让数据点一列一列依次进入，这儿的circles.delay()函数实现将j（列索引）作为了变量，越左边的列进入的越早。有兴趣的同学可以把circles.delay()的返回设成一个常数试试不一样的进入效果。circles.ease()包含了好多可选的缓动函数，规定了数据点从一个位置到另一个位置是如何过渡的。目前使用的是cubic-in-out。模拟的是自然站队场景中快速进入，精确占位的理念。easefuns里面包含了不同的函数，可以试试它们不同的效果。MikeBostock大神已然想到有人会对这些函数好奇，做了一个展示的页面：https://bl.ocks.org/mbostock/248bac3b8e354a9103c4

3.    到达最终位置后的状态，由circles. each("end",function(d) {…})来决定。这儿就把有效点渲染回白色小圆点了。



# 高亮缺失数据 Highlight Missing Data

源码见下面。



```java
nodes
          .transition()
          .duration(1000)
          .delay(500)
               .attr("fill", "red")
               .attr("r", 3)
          .transition()
          .duration(1000)
               .attr("fill", "#222")
               .attr("r", 1);
```





这儿主要就是通过nodes.transition()带着动画函数duration()和delay()，并结合各种样式的设置。简单来说，这儿的效果就是先将缺失点高亮成红色大圆点，然后又再次隐藏。



# 渲染效率

最后再扯两句渲染效率。D3.js对动画的渲染有其局限性，就是当节点增多时，效率会逐步降低。源码中包含若干个样例数据集。比如data-20-10.csv表示20列，10行的数据集。当渲染data-20-10.csv、data-200-10.csv时，我的浏览器基本上还能胜任（MacbookPro, Chrome v51）。但对于data-200-100.csv以及更多数据的数据集，能明显感觉到渲染的迟滞。

这个也许不是D3.js的错，而是浏览器的问题。不知道大家有没有这方面的资料，推荐一下。



