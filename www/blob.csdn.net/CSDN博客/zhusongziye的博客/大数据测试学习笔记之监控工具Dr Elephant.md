# 大数据测试学习笔记之监控工具Dr.Elephant - zhusongziye的博客 - CSDN博客





2018年03月26日 20:50:44[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：614








## 大数据测试学习笔记之监控工具Dr.Elephant

随着大数据测试方面的学习扩展得越来越多，开始找了些关于大数据技术方面的监控工具，本文主要介绍linkedin开源的Dr.Elephant监控工具。




### 介绍

一句话介绍：

Dr.Elephant是Apache Hadoop和Apache Spark的job级和工作流级性能监视和调优工具。

由Linkedin开源贡献，github地址为：

https://github.com/linkedin/dr-elephant

定位：
成一个对Hadoop和Spark任务进行性能监控和调优的工具，能够自动收集Hadoop平台所有的度量标准，并对收集的数据进行分析，并将分析结果以一种简单且易于理解的形式展示出来。




### 为什么选择介绍Dr.Elehant

对于常规的cpu、io、memory等指标的监控，软件测试人员可以数出大量的工具，而对于针对大数据job和工作流级的监控工具估计很多人都不知道了，最近通过在github上搜索相关资源，发现了Dr.Elehant，对其相关文档做了大概的阅读，选择了这款工具作为后续大数据测试学习的一个选择，以便后续在实践过程中能够更好的采集测试指标用于分析。




### Dr.Elephant是如何工作的

Dr.Elephant周期性的从Yarn的Resource Manager获取所有最近成功和失败的应用程序列表。

从Job历史列表中获取每个应用的元数据：counter、配置、任务数据。

一旦它拥有所有的元数据，Dr.Elephant便运行一组启发式(heuristics)，并产生对单个启发式和任务整体性能的诊断报告。

然后，报告中用五个严重性级别来标记不同的结果，以表明潜在的性能问题。




### 核心功能
- 
启发式算法是基于规则的，且是插件式、可配置的，具有良好扩展性，能分析各种各样的框架

- 
创造性的和Azkaban调度器集成，并且支持任意一种Hadoop任务调度器，例如Oozie

- 
能自动统计所有历史任务的性能

- 
能实现Job级别的任务流性能比较

- 
对Hadoop和Spark任务能进行性能诊断

- 
具有良好的扩展性，能支持新种类的任务、应用和调度器

- 
提供REST API，用户能够通过API获取所有信息





### 首页

Dr.Elephant启动后，首页如下：
![](https://img-blog.csdn.net/20180326205003697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




### 任务详情

重点看下任务详情，这个是日常测试过程中非常关注的部分:

点击一个任务，可以进入任务详情页面。




![](https://img-blog.csdn.net/20180326205022920?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




在任务详情，可以看到如下信息：
- 
任务追踪链（Jobtracker）：这个链接指向任务的追踪页面。在这个页面上，可以看到任务的详细信息、日志、map和reduce的task信息。

- 
任务执行链（Job execution）：这个链接指向任务在调度器中的执行页面。例如，在Azkaban调度器上，指向这个任务的执行链接。

- 
任务定义（Job definition）：这个链接指向任务在调度器中的定义页面。例如，在Azkaban调度器上，指向这个任务的属性页面。

- 
任务流执行链（Flow execution）：这个链接指向整个任务流的执行页面。例如，在Azkaban调度器上，指向这个任务流的执行页面。

- 
任务流定义（Flow definition）：同前面的任务定义（Job definition），这个链接指向任务流的定义页面。

- 
任务历史（Job history）：这个链接指向任务历史页面。

- 
任务流历史（Flow history）：这个链接指向任务流历史页。






### 任务等级

Dr.Elephant提供了5个等级待优化的迫切性降序为：CRITICAL > SEVERE > MODERATE > LOW > NONE

下表为详情说明：
|等级|描述|
|----|----|
|RITICAL|任务需要迫切的进行优化|
|SEVERE|任务存在很大的可优化空间|
|MODERATE|任务有待进一步优化|
|LOW|任务存在很小的可优化空间|
|NONE|任务很安全，不需要优化|


### 总结

对Dr.Elephant的学习有待进一步花时间，当前先记录这些，后续进一步学习和完善。







