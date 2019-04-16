# 网络数据抓取及其R实现（以链家楼盘为例） - littlely_ll的博客 - CSDN博客





置顶2016年12月19日 21:39:01[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：2691








$\quad$本次数据抓取的链家新房楼盘，由于对网络抓取数据还不熟悉，所以现在只抓取了北京地区的楼盘。我在学习过程中主要参考资料为《基于R语言的自动数据收集》，这本书确实不错，在前八章讲解的基础知识，以本书的配套[网站资源](http://www.r-datacollection.com/materials.html)为讲解基础,但是在按照书中抓取数据时有些地方和书本讲的不太一样，我认为主要是网络结构的变化引起的，大部分还是可行的。现在我看到了第九章，再往后都是一些网络数据抓取项目实例，所以后面的东西还需要认真学习.闲话少说，首先给出[链家楼盘](http://bj.fang.lianjia.com/loupan/)的链接.可以看一下源代码![源代码](https://img-blog.csdn.net/20161220105035822?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 可以看出此处的源代码还是比较清楚识别的。下面是数据抓取代码：
```
library(XML)
library(RCurl)
library(stringr)
library(tidyr)
library(dplyr)

url <- "http://bj.fang.lianjia.com/loupan/"
html_form <- getURL(url = url)
parsed_form <- htmlParse(html_form)
name <- xpathSApply(parsed_form,"//h2/a",xmlValue)
area <- xpathSApply(parsed_form,"//div[@class = 'area']/span",xmlValue) #在area处只能用单引号括住，不能用双引号，下同
onsold <- xpathSApply(parsed_form, "//div[@class='type']/span[@class='onsold']",xmlValue)
type <- xpathSApply(parsed_form, "//div[@class='type']/span[@class='live']",xmlValue)
average <- xpathSApply(parsed_form, "//div[@class='average']/span[@class='num']",xmlValue)
building <- cbind(name=name,area = area, onsold=onsold,live=type,average=average)
buildings <- as.data.frame(building)
## cleaning the data
buildings_1 <- separate(data=buildings,col=area,into=c("a1","area"),sep=" ")
buildings_1$a1 <- NULL
buildings_2 <- separate(data=buildings_1,col=area,into=c("area"),sep="m2")
buildings_2 <- separate(data=buildings_1,col=area,into=c("area","a"),sep="m")
buildings_2$a <- NULL
write.csv(buildings_2,"building.csv",row.names = F)
```

以上是我抓取的链家楼盘北京地区的第一页数据，并把它保存为building数据，但是下面我就遇到了一些麻烦，主要是在我写入文件时名称是不变的，这样前面已经写入的文件被后来的文件所覆盖了，可能是使用for循环也不合适，现在还没有找到合适的办法，如果有朋友知道更好的办法的话，欢迎提出来，本人感激不尽。没办法我只能一个一个的来，幸好没几页。下面是用最笨的办法做的：

```
for(i in 2:2){ ##此处每次运行修改一下，从第2页至第4页手动修改（笨办法）
  urls <- str_c(url, "pg", i ,"/")
  html_form <- getURL(url = urls)
  parsed_form <- htmlParse(html_form)
  name <- xpathSApply(parsed_form,"//h2/a",xmlValue)
  area <- xpathSApply(parsed_form,"//div[@class = 'area']/span",xmlValue)
  onsold <- xpathSApply(parsed_form, "//div[@class='type']/span[@class='onsold']",xmlValue)
  type <- xpathSApply(parsed_form, "//div[@class='type']/span[@class='live']",xmlValue)
  average <- xpathSApply(parsed_form, "//div[@class='average']/span[@class='num']",xmlValue)
  building <- cbind(name=name,area = area, onsold=onsold,live=type,average=average)
  buildings <- as.data.frame(building)
  ##cleaning data
  buildings_1 <- separate(data=buildings,col=area,into=c("a1","area"),sep=" ")
  buildings_1$a1 <- NULL
  buildings_2 <- separate(data=buildings_1,col=area,into=c("area"),sep="m2")
  buildings_2 <- separate(data=buildings_1,col=area,into=c("area","a"),sep="m")
  buildings_2$a <- NULL
  write.csv(buildings_2,"building2.csv",row.names = F) #此处building2.csv处需要手动修改，和上面修改的相对应（笨办法）
}

##有一个办法，可以按照下面来做，比如
a <- 0
for(i in 1:5){
a1 <- i
a <- c(a,a1) ##向a中添加值，先合并，最后整体分析
```

最后把数据合并，清理并分析

```
##merge data into "buildings".
buildings <- read.csv("building.csv")
buildings$average <- as.numeric(buildings$average)
building1 <- arrange(buildings,desc(average))
## plot data
ggplot(building1,aes(name,average,fill=name)) + 
  geom_bar(stat="identity") + coord_flip()
ggplot(building1,aes(average,fill=I("blue"))) + 
  geom_histogram() + 
  theme(panel.grid.major = element_blank())
ggplot(building1,aes(live)) + 
  geom_bar(aes(fill=live))
ggplot(building1,aes(live)) + 
  geom_bar(aes(fill=live)) + 
  coord_polar()
```

看一下整理后的数据 
![数据](https://img-blog.csdn.net/20161220105501198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在住房面积中，我没有把最低与最高面积分开，如果要研究住房面积与其他变量的关系，可以把数据分开。 
![条形图](https://img-blog.csdn.net/20161219211223842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


$1. 条形图$

在画图的时候注意到，单位平方价格小于1000的都是万元/每套，所以从图上显示有数值非常小的部分。从图中可以看出，楼盘价格都在50000之上有8家，在100000之上（这可是单价呀）的有4家。北京真是待不起啊！奋斗一年真心买不起一平厕所的。 
![直方图](https://img-blog.csdn.net/20161219211403735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


$2. 直方图$

从直方图可以看出，楼盘价格是右偏的，看来北京新开的楼盘没有价格低的，但是也可以看出大部分的楼盘25000及其以下，对比上述条形图。50000之上的单价可能就是别墅了。 
![柱状图](https://img-blog.csdn.net/20161219211459486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


$3. 柱状图$
![饼图](https://img-blog.csdn.net/20161219211538644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


$4. 饼图$

柱状图和饼图可以看出住房类别，普通住宅和别墅占大头，商业类很少，这也说明北京有钱人真不少，要不然在这人稠地贵的帝都也不会出现这么多别墅，竟然隐隐要赶上普通住宅了，要知道，别墅是最占用地面积的。 

先简单分析到这，因为数据较少，无法深入分析，所以我还要继续努力，争取抓取多些数据。




