# 大数据挖掘为什么不选择R? - 三少GG - CSDN博客
2013年12月22日 21:36:32[三少GG](https://me.csdn.net/scut1135)阅读数：1145
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

# Tools: any thoughts on open source R vs. Rapid-I ?
R Pros:
- it's a programming language: you can do what you want
- number of algorithms: many analysis and data transformation schemes already exist
- integration and expandability: embedding an R script into own programs is pretty easy, writing
 extensions as well since you already know the language then
- widely used in academia and in education of statisticians: huge user base
- the no 1 option when it comes to pure statistics and data is not too large
R Cons:
- it's a programming language: you have to write / adapt source code for every single step
- **scalability often is an issue: if you have large sets of data, you will easily get into trouble** (since R is licensed under GPL and IP rights are hold by thousands of people I highly double that a 100% legal way for an open-core
 license model can be used here so that a company can jump in and help you out here)
- hardly any native support for enterprise usage / deployment: no process definitions, no
 scheduling, no integration, no...
- nothing for web-based applications: harder deployment etc.
RapidMiner Pros:
- gray-box programming: you can program your analysis but without writing source code (and
 you still have access to all details and are able to change them)
- processes: each program / analysis script is a parametrized process which can be re-used
 and - more important - connected or embedded into business processes
- scalability: depending on the used algorithms, RapidMiner can use much more data. By using
 the server version (RapidAnalytics), this can even be improved and the Enterprise Edition of RapidMiner offer methods for in-database-mining
- cluster support: multiple servers can be used as computation cluster
- business analytics: RM is much stronger when it comes to analytical ETL, data and text mining,
 and - especially with the Enterprise Edition of the server RapidAnalytics - predictive reporting and dashboards
- integration and expandability: easy integration of processes as web services (via RapidAnalytics),
 RM integrates Weka, R (best of both worlds), and offers options for additional extensions with scripts if something is missing
RapidMiner Cons:
- probably a smaller user base than R
- the integration between RapidMiner and R is not (yet!) as perfect as it should be
- less statistical methods than R but more methods derived from machine learning / data mining
- "There is an operator for that" is an often heard answer by RapidMiner-People. However, many operators (the basic building blocks of analysis processes) mean higher
 complexity
- the graphical user interface is really powerful but this also adds to complexity
This is certainly not a complete list or overview and there are many more aspects than those I have discussed above. And which tool is more appropriate certainly depends on your background and requirement. However, I still hope that it helps.
The good thing is: both are open source and can be tested, and due to the open source nature you can also have the best of both worlds with a single solution (RapidMiner + R Extension). So just give them a try and test them yourself!

