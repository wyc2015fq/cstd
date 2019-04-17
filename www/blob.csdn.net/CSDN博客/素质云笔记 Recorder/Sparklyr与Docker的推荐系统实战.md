# Sparklyr与Docker的推荐系统实战 - 素质云笔记/Recorder... - CSDN博客





2016年10月07日 13:33:03[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1983













作者：Harry Zhu

链接：https://zhuanlan.zhihu.com/p/21574497

来源：知乎

著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



相关内容：
# [sparklyr包：实现Spark与R的接口，会用dplyr就能玩Spark](http://blog.csdn.net/sinat_26917383/article/details/52749138)




# 概述

大数据时代，做数据分析的人才辈出，Java、Scala、Go、Julia、Python、JavaScript都不断涌现出很多数据分析的新工具，然而对于数据分析来说，这些工具都不是分析的核心，分析的核心在于对市场业务、对具体数据的理解。相信你也见过太多脱离具体应用场景和业务数据而空谈算法的『数据分析大师』了。算法的文章在教科书、论文、已经各种文章里面都是大把大把的，然而大道至简，真正能将算法转化为生产力解决实际问题才是关键。
![](https://pic1.zhimg.com/5cb33d4beb0712cc98b1d79dad33cde8_b.jpg)
作为统计学出身的人，真心无力折腾Java的设计模式、JS的异步回调，我们更倾向于把精力放在数据、模型、分析、拟合、预测、检验、报告等等。在SparkR之后，RStudio公司又推出了全新力作Sparklyr，全面继承dplyr的操作规范。通过Sparklyr和Docker的完美结合，Spark的大数据计算引擎门槛进一步降低！不仅仅简化了分布式计算的操作，还简化了安装部署的环节，我们只几乎不需要做什么改动就可以直接运用R中的dplyr进行分布式的计算，几乎不需要学习此前Scala风格的API。
![](https://pic4.zhimg.com/535771f7a9c0df09a69fd51cebb30e83_b.jpg)

# 什么是Sparklyr

Sparklyr顾名思义就是 Spark + dplyr。首先，它实现了将dplyr的data frame所有操作规范对Spark计算引擎的完整封装。其次，它是的R可以透过Spark的MLib机器学习库拓展分布式机器学习算法的能力。最后，对于其他Spark功能，我们还可以通过`sparkapi`来调用所有Spark库中的Scala接口。

目前，最新版本的RStudio 已经集成了Spark引擎，本文将带你快速进入大数据分析领域。
![](https://pic4.zhimg.com/e0a17b588481294638bfd1a67bef9d73_b.jpg)
## 什么是Docker

Docker是类似于虚拟机的一种虚拟化软件，让我们可以在不同操作系统上运行相同的软件。它主要解决了虚拟机安装软件速度比较慢的问题，相对于虚拟机，Docker的启动速度是秒级的。本文将通过一个详细的例子，指导各位R语言的爱好者快速安装带有Spark功能的RStudio软件，快速开启您的大数据之旅。


# 软件安装

鉴于大量数据分析用户还是以Windows操作系统为主，或许还深度绑定了Excel这样的数据分析神器，本文将放弃以Unix视角，采用Windows视角为各位看官介绍软件安装过程。


## 步骤一：安装Windows版本Docker

你可以进入[http://www.docker.com](https://link.zhihu.com/?target=http%3A//www.docker.com)的官网首页，看到软件下载链接，这里需要您的操作系统在Windows
 10及其以上版本。如果不想进入官网，也可以点击这个链接：[https://download.docker.com/win/beta/InstallDocker.msi](https://link.zhihu.com/?target=https%3A//download.docker.com/win/beta/InstallDocker.msi)


下载之后根据系统提示默认安装即可，不得不说，Docker是一个神奇的软件。


## 步骤二：启动Docker软件
![](https://pic1.zhimg.com/1ad5f3d91ac1cb2dc7ea2274abe7dd24_b.png)
点击软件图标即可启动Docker软件(最新版本号和此图有可能并不一致)。接着，您可以选择打开系统自带Powershell软件，向Powershell中复制粘贴如下命令：



```
docker run -d -p 8787:8787 --name financer index.tenxcloud.com/7harryprince/sparkr-rstudio​
```


这时候只需要耐心等待您的大数据分析系统安装完成。（软件大概会占用4G左右的空间，我已经为你预先为你一站式安装了最新的 Shiny, R markdown，R notebook，jdk8，gcc5.3,R 3.3 以及其他数据分析常用的R包）


## 步骤三：访问RStuido软件

一旦软件完成安装，你可以在Chrome或者Edge中输入下面的地址访问到RStudio软件:



```
localhost:8787
```


最后，输入默认账号`harryzhu`,密码`harryzhu`即可进入RStudio软件。
![](https://pic4.zhimg.com/02ad7592f9c98b9252b3f8620e198beb_b.jpg)

# 推荐系统实战


```
library(sparklyr)
library(dplyr)
Sys.setenv(SPARK_HOME="/opt/spark-1.6.0-bin-hadoop2.6")
Sys.getenv("SPARK_HOME")
sc = spark_connect("local")
mtcars_tbl <- copy_to(sc, mtcars,overwrite = TRUE)
```


​未完

求各位看官给点反馈~~~


# 参考资料
- [RStudio又搞出了个大杀器！sparklyr包：实现Spark与R的接口，会用dplyr就能玩Spark](https://link.zhihu.com/?target=http%3A//www.xueqing.tv/cms/article/233)

- [Using Spark with Shiny and R Markdown]() Slide
- [https://channel9.msdn.com/Events/useR-international-R-User-conference/useR2016/Using-Spark-with-Shiny-and-R-Markdown](https://link.zhihu.com/?target=https%3A//channel9.msdn.com/Events/useR-international-R-User-conference/useR2016/Using-Spark-with-Shiny-and-R-Markdown)
 Video

- [https://blogs.msdn.microsoft.com/azuredatalake/2016/08/09/rapid-big-data-prototyping-with-microsoft-r-server-on-apache-spark-context-switching-spark-tuning/](https://link.zhihu.com/?target=https%3A//blogs.msdn.microsoft.com/azuredatalake/2016/08/09/rapid-big-data-prototyping-with-microsoft-r-server-on-apache-spark-context-switching-spark-tuning/)

- [https://databricks.com/blog/2016/07/07/sparkr-tutorial-at-user-2016.html?twitter=@bigdata](https://link.zhihu.com/?target=https%3A//databricks.com/blog/2016/07/07/sparkr-tutorial-at-user-2016.html%3Ftwitter%3D%40bigdata)

- [http://conferences.oreilly.com/strata/hadoop-big-data-ny/public/schedule/detail/52369?twitter=@bigdata](https://link.zhihu.com/?target=http%3A//conferences.oreilly.com/strata/hadoop-big-data-ny/public/schedule/detail/52369%3Ftwitter%3D%40bigdata)

- [Top 5 Mistakes When Writing Spark Applications](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3Fv%3DvfiJQ7wg81Y)



​

​



「欢迎下载sparkrstuido镜像」















