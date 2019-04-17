# R︱shiny实现交互式界面布置与搭建（案例讲解+学习笔记） - 素质云笔记/Recorder... - CSDN博客





2016年11月30日 17:59:04[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：15952










**要学的东西太多，无笔记不能学~~ 欢迎关注公众号，一起分享学习笔记，记录每一颗“贝壳”~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


看了看往期的博客，这个话题竟然是第一次涉及，还真是学无止境啊。



      Web Service 这个概念比较成功了， 在SOAP服务之后， Restful服务的普及， 使得数据库的CRUD操作通过网络无限延伸。 普遍的好处是：

1） 你不需要安装任何程序， 携带数据， 只要有网的地方， 你就能演示。（远程）

2） 只要安装一次， 可以多人使用， 可以让别人试用。（多用户）

3） 不用担心软件或者代码泄密， 可以控制用户使用时间和权限。 （服务）


——————————————————————————————————




# 案例一：中文词云




昨天自己动手实践了一把，做了一个简单的低版本词云实现的web，同时里面内嵌了jiebaR分词，导入文本即可分词、画词云，效果如下图：




![](https://img-blog.csdn.net/20161130165943787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





——————————————————————————————

# 案例二：社交网络




网络上的一个社交网络案例——打开网页即可使用：

https://kehaowu.shinyapps.io/SocialNetwork/


![](https://img-blog.csdn.net/20161130170445838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




这个是个非常简单的应用，我们输入的数据包括人物关系和一些简单的参数。其中人物关系是这样定义的：

比如A和B是friend关系，那么：

输入数据中的Former Person就是A；Later Person就是B，Label就是friend。

依次类推，如果A和B是friend关系，A和C是couples关系，B和C是classmate关系，那么输入数据就应该是：

Former Person: A,A,B

Later Person: B,C,C

Label:friend,couples,classmate


参考博客：[](http://blog.sina.com.cn/s/blog_403aa80a0102vsae.html)

## R语言 用shiny开发一个简单的画人物关系应用






——————————————————————————————


# 案例三:官网的案例库




http://shiny.rstudio.com/gallery/


有案例库，你就知道怎么做了吧？——别告诉我，你没做过PPT，copy一下他们的代码就行啦~

![](https://img-blog.csdn.net/20161130170928121?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





——————————————————————————————







# 一、Shiny安装






## 1. 直接通过包管理器安装



`      install.packages("shiny")`
```

```

## 2. 利用devtools通过github安装



if (!require("devtools"))
  install.packages("devtools")devtools::install_github("rstudio/shiny")



这里要注意， 如果需要利用代理

```
library(httr)set_config(
  use_proxy(url="18.91.12.23", port=8080, username="user",password="password"))
```

或者要设定专门的版本号
devtools::install_version("shiny", version="0.10.2.2")

————————————————————————————————————————————





# 二、ui.R+server.R+global.R


看到网上大多给你扯ui.R+server.R，很少会提到global.R，笔者在自己实践的时候遇到的了global.R的使用问题，网上教程只能自己参悟了。

ui.R：搭框架，控件；


server.R：每个控件背后的算法代码；

global.R:server.R中，万一有很长的代码与调用很多其他packages就可以用global写一个函数集合，然后统一在server.R调用。




基本的框架就是:



```
# ui.R

library(shiny)

shinyUI(fluidPage(

))

# server.R

library(shiny)

shinyServer(function(input, output) {

})
```

往里面的函数里面写东西就行。





1、ui.R——界面设计







```
library(shiny)

# Define UI for application that plots random distributions 
shinyUI(pageWithSidebar(

  # Application title
  headerPanel("Hello Shiny!"),

  # Sidebar with a slider input for number of observations
  sidebarPanel(
    sliderInput("obs", 
                "Number of observations:", 
                min = 0, 
                max = 1000, 
                value = 500)
  ),

  # Show a plot of the generated distribution
  mainPanel(
    plotOutput("distPlot")
  )
))
```
sidebarPanel就是侧面的控件，mainPanel就是右边的控件（默认位置），一般mainPanel通过plotOutput画图。
sliderInput是一个滑动的窗口。





2、server.R








```
library(shiny)


shinyServer(function(input, output) {


  output$distPlot <- renderPlot({

    dist <- rnorm(input$obs)
    hist(dist)
  })
})
```

server.R是数据处理的地方，后端，数据来源都来自于ui.R，譬如obs，可能server处理好的图才是返回给ui.R。譬如dist

要调用ui里面的内容的话就是Input$obs就是上面sliderInput里面输入的内容。






Shiny提供一些缓存优化的机制， 其中最有效的是Reactive Expression 反冲表达式。

Reactive机制使得交互渲染的效率大幅度提高。 举个例子， server.R返回一个数据绘图， 但是每次运行需要重新读取数据。 但是当如果输入没有变化的时候， 这种重复的数据读取会极大的延迟反应， 浪费资源。
基于这种考虑， 我们使用reactive关键词重写了数据读取模块。reactive像一个带cache的模块， 并且当reactive检查到数据输入没有任何变化， 那么输出也不会有变化的时候， reactive就会使用缓存的数据，避免了重复运算， 加速了反应。







3、global.R




那么关系现在是这样的，ui.R负责前端，是数据获取的地方，server.R是处理的地方，返回的是处理好后的数据列；那么global.R是用来补充server.R的功能的。

一般是用来写一个函数，然后直接在server.R调用。

来看一个官网里面的global的案例：



```
library(tm)
library(wordcloud)
library(memoise)

# The list of valid books
books <<- list("A Mid Summer Night's Dream" = "summer",
              "The Merchant of Venice" = "merchant",
              "Romeo and Juliet" = "romeo")

# Using "memoise" to automatically cache the results
getTermMatrix <- memoise(function(book) {
  # Careful not to let just any name slip in here; a
  # malicious user could manipulate this value.
  if (!(book %in% books))
    stop("Unknown book")

  text <- readLines(sprintf("./%s.txt.gz", book),
    encoding="UTF-8")

  myCorpus = Corpus(VectorSource(text))
  myCorpus = tm_map(myCorpus, content_transformer(tolower))
  myCorpus = tm_map(myCorpus, removePunctuation)
  myCorpus = tm_map(myCorpus, removeNumbers)
  myCorpus = tm_map(myCorpus, removeWords,
         c(stopwords("SMART"), "thy", "thou", "thee", "the", "and", "but"))

  myDTM = TermDocumentMatrix(myCorpus,
              control = list(minWordLength = 1))
  
  m = as.matrix(myDTM)
  
  sort(rowSums(m), decreasing = TRUE)
})
```
主要就是写了一个函数，一开始把调用的包都给出来了，然后跟函数一样了。这里的memoise函数是缓存的功能，之后就跟正常使用的代码一样。




————————————————————————————————————————————





# 三、shiny的部署




很简单的是，一般写完之后放在一个文件夹里面，然后runApp一下那个路径的文件夹就行了。

或者Rstudio里面可以直接：”Run App“按钮，你把三个文件导入Rstudio随便点一下就可以，Rstudio会帮你识别。

其中，你想要最右边的那三个代码栏目的话，把”DESCRIPTION“+”Readme.md“复制到那个文件夹里面，自动会显示出来，而且所有的APP都可以复制同一份内容，上面的两个东西可以到library的example里找得到。




![](https://img-blog.csdn.net/20161130175026908?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


————————————————————————————————————————————







# 四、Shiny 服务器






假如你有创建了很多的Shiny服务， 那么一个专门的Shiny服务器会方便部署。

一般可以有两种部署， 一种是自己搭建一个Shiny Server。 这样就可以同时使用很多的Shiny 应用了。

另外一种是直接部署到Shiny云服务上去， 譬如“www.shinyapps.io” 注册一个云账号， 然后把服务部署到云上去。在注册完成后， 你只要遵照详细的链接， 授权， 和部署的步骤，就可以把本地Shiny App上传部署。





参考来自微信公众号： AIaidddzcAI2ML人工智能to机器学习




————————————————————————————————————————————




## 延伸一：一个用于监控Shiny应用的Shiny应用

来源公众号：子豹


核心部分来自Huidong Tian的文章

http://withr.me/a-shiny-app-serves-as-shiny-server-load-balancer



```python
## Setup work directory;
setwd("/srv/shiny-system/Data") 
I <- 0
for (i in 1:60) {
  system("top -n 1 -b -u shiny > top.log")
  dat <- readLines("top.log")
  id <- grep("R *$", dat)
  Names <- strsplit(gsub("^ +|%|\\+", "", dat[7]), " +")[[1]]
  if (length(id) > 0) {
    # 'top' data frame;
    L <- strsplit(gsub("^ *", "", dat[id]), " +")
    dat <- data.frame(matrix(unlist(L), ncol = 12, byrow = T))
    names(dat) <- Names
    dat <- data.frame(Time = Sys.time(), dat[, -ncol(dat)], usr = NA, app = NA)
    dat$CPU <-as.numeric(as.character(dat$CPU))
    dat$MEM <-as.numeric(as.character(dat$MEM))
    # Check if connection number changed;
    for (i in 1:length(dat$PID)) {
      PID <- dat$PID[i]
      system(paste("sudo netstat -p | grep", PID, "> netstat.log"))
      system(paste("sudo netstat -p | grep", PID, ">> netstat.log2"))
      system(paste("sudo lsof -p", PID, "| grep /srv > lsof.log"))
      netstat <- readLines("netstat.log")
      lsof <- readLines("lsof.log")
      dat$usr[i] <- length(grep("ESTABLISHED", netstat) & grep("tcp", netstat))
      dat$app[i] <- regmatches(lsof, regexec("srv/(.*)", lsof))[[1]][2]
    }
    dat <- dat[, c("app", "usr")]
  } else {
    dat <- data.frame(app = "app", usr = 0)
  }
  write.table(dat, file = "CPU.txt")
}
```









# 参考文献：




1、中文教程：http://yanping.me/shiny-tutorial/

2、英文官网：http://shiny.rstudio.com/   

3、[R powered web applications with Shiny](http://zevross.com/blog/2016/04/19/r-powered-web-applications-with-shiny-a-tutorial-and-cheat-sheet-with-40-example-apps/)：一些讲解，比较深入讲解每个模块内容



