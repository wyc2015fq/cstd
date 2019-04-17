# 高效率爬虫框架之 pyspider - zhusongziye的博客 - CSDN博客





2018年08月12日 20:10:51[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：464








在项目实战过程中，我们往往会采用爬虫框架来实现抓取，这样可提升开发效率、节省开发时间。而 pyspider 就是一个非常优秀的爬从框架，它的操作便捷、功能强大、利用它我们可以快速方便地完成爬虫的开发。

# pyspider 框架介绍

![](https://img-blog.csdn.net/20180812201009565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

pyspider 是由国人 binux 编写的强大的网络爬从系统，它带有强大的 WebUI、脚本编辑器、任务监控器、项目管理以及结果处理器，它支持多种数据库后端、多种消息队列、JavaScript 渲染页面的爬取。使用起来非常方便。

其 GiHub 地址为：

https://github.com/binux/pyspider

官方文档地址：

http://docs.pyspider.org/

# pyspider 基本功能

pyspider 的功能有如下几点：
- 
1 提供方便易用的 WebUI 系统，可视化地编写和调式爬虫



- 
2 提供爬取进度监控、爬取结果查看、爬虫项目管理等功能。



- 
3 支持多种后端数据库，如 MySQL、MongoDB、Reids、SQLite、Elasticsearch、PostgreSQL。



- 
4 支持多种消息队列、如 RabbitMQ、Beanstalk、Redis、Kombu。

- 
5 提供优先级控制、失败重试、定时抓取等功能。



- 
6 对接了 PhantomJS、可以抓取 JavaScript 渲染的页面。



- 
7 支持单机和分布式部署、支持 Docker 部署。


如果想要快速方便地实现一个页面的抓取，使用 pyspider 不失为一个好的选择。如快速抓取某个普通新闻网站的新闻内容。但如果应对反爬程度很强、超大规模的抓取、推荐使用 Scrapy、如抓取封 IP、封账号、高频验证的网站的大规模数据采集。

# pyspider 的架构

![](https://img-blog.csdn.net/20180812201019837?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

pyspider 的架构主要分为 Scheduler(调度器)、Fetcher(抓取器)、Processer(处理器)三个部分。整个爬取过程受到 Monitor(监控器)的监控，抓取的结果被 Result Worker(结果处理器)处理。

Scheduler 发起任务调度，Fetcher 负责抓取网页内容，Processer 负责解析网页内容，然后将新生成的 Request 发给 Scheduler 进行调度，将生成的提取结果输出保存。

pyspider 的任务执行流程的逻辑很清晰，具体过程如下所示：
- 
1 每个 pysipder 的项目对应一个 Python 脚本，该脚本定义了一个 Handler 类，它有一个 on_start() 方法。爬取首先调用 on_start() 方法生成最初的抓取任务，然后发送给 Scheduler。



- 
2 Scheduler 将抓取任务分发给 Fetcher 进行抓取，Fetcher 执行并得到响应、随后将响应发送给 Processer。



- 
3 Processer 处理响应并提取出新的 URL 生成新的抓取任务，然后通过消息队列的方式通知 Scheduler 当前抓取任务执行情况，并将新生成的抓取任务发送给 Scheduler。如果生成了新的提取结果，则将其发送到结果队列等待 Result Worker 处理。



- 
4 Scheduler 接收到新的抓取任务，然后查询数据库，判断其如果是新的抓取任务或者是需要重试的任务就继续进行调度，然后将其发送回 Fetcher 进行抓取。



- 
5 不断重复以上工作、直到所有的任务都执行完毕，抓取结束。



- 
6 抓取结束后、程序会回调 on_finished() 方法，这里可以定义后处理过程。




