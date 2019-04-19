# R语言数据可视化初体验 - BlueSky - CSDN博客
2015年12月10日 00:08:17[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：2776
转自？http://tech.uc.cn/?p=2465
前些天通过一张美丽的图片，闯进了[FlowingData](http://www.smallmeans.com/new-york-times-infographics/)这个世界，开始真正去了解数据可视化。“The greatest value of a picture is when it forces us to notice what we never expected to see”是该网站about中的第一句话，在逛了不少可视化的网站和博客之后，个人觉得这句话是数据可视化最好的注解。大数据像是‘黑盒子’一样的新世界，现在我们用数据挖掘来探索这个世界的规则和信息。但是没有问题导向的数据挖掘需要花费大量的时间来探索。另外眼睛才是我们认识世界最直接有效的渠道，所以如果我们能够将数据世界具体化呈现出来，那么其实很多规则我们用眼睛就可以看得到。甚至看到更多隐藏的信息。数据可视化不仅仅是为了展示数据，而是通过图像残留来增加人思考的时间和质量，加大概率发现隐藏在数据背后的未知。
[这幅图](https://www.mapbox.com/labs/twitter-gnip/languages/#3/57.56/-23.20)是twitter近3亿用户按经纬度和语言的分布。逻辑上很简单的图，但当却耐人寻味。你可以把一张欧洲地理地图、公路铁路地图或者其他信息图与其进行对比，你会快速的清楚大部分的twitter用户所处的各种环境，甚至更多的信息，当然带来更多的思考点，而且这样的图形会经常浮现，让你整天处于思考状态。
另外发现了很多非常有意思的数据可视化网站：[纽约时报的可视化工作实验室](http://www.smallmeans.com/new-york-times-infographics/)(商业性强);[Information is beautiful](http://www.informationisbeautiful.net/)(想象力丰富，数据可视化比赛);[Flowingdata](http://flowingdata.com/)(前沿技术应用、产品化)。　　
## **R的可视化**
目前数据可视化大军大都在UCD方面，[像百度的流量研究院](http://tongji.baidu.com/data/hour),和腾讯的[qq游戏地图](http://qqgame.qq.com/online.shtml)这些都是优秀的数据可视化产品，完美的效果和用户体验。但对数据从事人员来说，数据可视化是锦上添花的一部分，他们更重要的工作是从数据中挖掘出规则和信息，然后才将这些结果可视化出来。说到这里就有一种语言浮出水面：R。R是一种优秀的具有很强数据可视化功能的数据分析语言。
很些人总结出了一些数据可视化的步骤，如[这里](http://ucdchina.com/snap/12671)。这里我讲一下我的方法：类比代数学中的元素与映射，数据可视化本质上就是两样东西，一样是实体（你的研究对象，可以是任何范畴），一样是联系（就是两个或多个实体之间的关系）。其中实体和联系都有属性，比如把人当成实体，那么人有身高、体重等属性；而人与人之间的联系的属性有像亲密程度、贫富差距等属性。那我们需要的就是把这些信息最清晰的表现出来，我觉得这样的逻辑方式可以帮帮助我们对可视化进行分解，更容易解放想象力。
 下面以中国航空数据图作为例子来讲一下。从[这里](http://www.stanford.edu/~cengel/cgi-bin/anthrospace/wp-content/uploads/2012/03/airports.csv)可以得到机场的数据。从[这里](http://www.stanford.edu/~cengel/cgi-bin/anthrospace/wp-content/uploads/2012/03/PEK-openflights-export-2012-03-19.csv)可以得到所有航线的数据。这两个数据集都是包含全世界的，需要从中将中国的数据提出。还需要做一些数据处理和分析工作，篇幅关系下面只给出了图形部分的代码。分几步来说：
##### 1.将机场当成实体，它的一个属性是航线的数量。用直方图来体现
![](http://tech.uc.cn/wp-content/uploads/2013/11/boke_zhifan.jpeg)

C
```
ggplot(cnt) + geom_bar(aes(x = cnt$count))
```
##### 2.机场有另外一个属性：地理位置。用地图来体现，并将航线数量展示上去
![](http://tech.uc.cn/wp-content/uploads/2013/11/boke_tu3.jpeg)

C
```
p=ggmap(get_googlemap(center='china',zoom=4,maptype='terrain'),extent='device')
p+geom_point(data=chinaport,aes(x=lon,y=lan),colour='blue',size(chinaport$count/10)
             alpha=0.6)
```
##### 3.接下来就是将机场的联系展示上去。
![](http://tech.uc.cn/wp-content/uploads/2013/11/boke_7.jpeg)

C
```
ggmap(get_googlemap(center = 'china', zoom=4, maptype='roadmap'),extent='device')+#画地图
geom_point(data=chinaport,aes(x=lon,y=lan),colour = 'blue',
           size=(chinaport$count/20),alpha=0.8)+  #画机场
geom_line(data=dat,aes(x=lon,y=lan,group=group), size=0.1, #画航线
          alpha=0.05,color='red4')
```
##### 4.有时候一张图是不足以体现数据的，这里用一张gif图来表现每个机场的航线数。
![](http://tech.uc.cn/wp-content/uploads/2013/11/hanxian.gif)

C
```
require(animation) #导入所用包
draw = function(i){  #画图函数
   one = chinaport[1:i,]  
   p=ggmap(get_googlemap(center='china',zoom=4,maptype='terrain'),extent='device')  
   p+geom_point(data=one,aes(x=lon,y=lan),colour='blue',size(one$count/15),alpha=0.6)+  
   geom_point(data=one,aes(x=lon,y=lan),colour='red',alpha=0.6) 
                  } 
oopts = ani.options(ffmpeg = "D:/Program Files (x86)/ImageMagick-6.8.7-Q8/ffmpeg.exe") #设置ffmpeg 
saveVideo({  
    for(i in 1:36) print(draw(i)) #循环调用画图函数  
    ani.options(interval = 0.6, nmax = 230) },  
    video.name = "hanxian.gif", other.opts = "-b 500k")
```
## **可重复和自动化统计报告**
国内一个专注于R语言自动化报告的学者[谢益辉](http://yihui.name/),开发了一个基于R语言的Knitr包，主要用来实现可重复统计报告。Knitr包主要是实现在标记语言[Markdown](http://wowubuntu.com/markdown/)中嵌入R代码，然后执行Markdown脚本就可以生成一份具有R程序分析结果的报告。
具体[步骤](http://cos.name/2012/06/reproducible-research-with-knitr/)为：
1. 先在R中建一份R-Markdown(.md)文件，可直接在其中写Markdown脚本。
2. 可以通过以下方式插入R脚本，并可以通过调参，控制R程序的输出包括表和图的各种属性控制。
3. 最后通过Knitr来运行这份.md文件可直接生产一份html文档，也可通过latex进一步产生一份pdf报告。
通过这个方案，其实我们可以定制一些日常可视化的数据分析报告，这不仅要比密密麻麻的数据报表来得要鲜活得多，而且支持更加深入的分析和针对性的结果展示.
注：本文采用R中的ggplot2包实现可视化，这是一个图层逻辑的可视化工具（跟本文的可视化逻辑很符合），在这并没有详细阐述，待接下来再写一篇来好好介绍一下神奇的ggplot2。
