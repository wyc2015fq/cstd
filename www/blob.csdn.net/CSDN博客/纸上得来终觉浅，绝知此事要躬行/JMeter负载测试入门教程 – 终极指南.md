# JMeter负载测试入门教程 – 终极指南 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 21:02:30[boonya](https://me.csdn.net/boonya)阅读数：1087








This article is from:[http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html)




本教程是关于JMeter的。我们将会涉及到的要点如下：
- 
我们会解释JMeter是用来干什么的。
- 
JMeter是如何设计和实现的以及涉及到哪些技术。
- 
如何正确的安装和配置它。
- 
它的用户界面（UI）的基本功能。
- 
详细介绍如何配置不同的测试计划。
- 
展示如何分析它的输出结果。
- 
最后我们会提到一些重要的最佳实践。


本文所提到的所有示例的配置和实现环境是： Java 8 update 0.20，Eclipse Luna 4.4， JMeter 2.11 和MySQL Community Server 5.6.21.


### 目录
- [1. 简介](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#introduction)
- [2. 涉及到的技术](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Technologies%20used)
- [3. 本文主要讲些什么？](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#What%20is%20this%20article%20about?)
- [4. 安装](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Installation)
- [5. GUI 概述](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#GUI%20overview)
- [6. 可用的请求类型](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Available%20requests)
- [7. 测试计划和组件](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Test%20plans%20and%20components)
- [7.1. 线程组](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#ThreadGroup)
- [7.2. 取样器](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#TSamplers)
- [7.3. 逻辑控制器](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Logic%20controllers)
- [7.4. 监听器](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Listeners)
- [7.5. 定时器](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Timers)
- [7.6. 断言](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Assertions)
- [7.7. 配置元件](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Configuration%20nodes)
- [7.8. 前置处理器](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Pre%20processors)
- [7.9. 后置处理器](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Post%20processors)
- [8. 测试计划元素的执行顺序](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#Test%20plan%20elements%20order%20of%20execution)
- [9. 启动/停止测试计划](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20Run/Stop%20test%20plans)
- [10. Web (HTTP) 测试计划](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20Web%20(HTTP)%20test%20plan)
- [11. 数据库测试计划入门](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20mysql)
- [12. Junit测试计划](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20junit)
- [13. 录制HTTP测试](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20recording)
- [14. JMeter 插件](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20plugins)
- [15. JMeter 最佳实践](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20best_practices)
- [16. 总结](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20Summary)
- [17. 下载](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20download)
- [18. 相关资源](http://www.javacodegeeks.com/zh-hans/2015/02/jmeter%E8%B4%9F%E8%BD%BD%E6%B5%8B%E8%AF%95%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B.html#%20resources)



 由于原文内容庞大，故仅转载文章目录介绍，以备下来学习方便！




