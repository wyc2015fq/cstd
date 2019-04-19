# R-资源大全 - 三少GG - CSDN博客
2014年02月27日 15:46:39[三少GG](https://me.csdn.net/scut1135)阅读数：1072

必备R包：
1.
**shiny  网站图形展示**
[http://www.rstudio.com/shiny/](http://www.rstudio.com/shiny/)
代码规范：
# Google's R Style Guide
R is a high-level programming language used primarily for statistical computing and graphics. [https://google-styleguide.googlecode.com/svn/trunk/Rguide.xml](https://google-styleguide.googlecode.com/svn/trunk/Rguide.xml)
R讲座：
1. 
京东大数据R语言应用   刘思喆                
[http://www.infoq.com/cn/presentations/jingdong-big-data-r-language-applications?utm_source=infoq&utm_medium=videos_homepage&utm_campaign=videos_row2](http://www.infoq.com/cn/presentations/jingdong-big-data-r-language-applications?utm_source=infoq&utm_medium=videos_homepage&utm_campaign=videos_row2)
2.R能不能处理大数据
[http://www.bjt.name/2013/05/big-data-r-application/](http://www.bjt.name/2013/05/big-data-r-application/)
3. rhadoop  下载地址：[https://github.com/RevolutionAnalytics/RHadoop/wiki/Downloads](https://github.com/RevolutionAnalytics/RHadoop/wiki/Downloads)
安装讲解：
[http://cos.name/videos/intro-to-rhadoop/](http://cos.name/videos/intro-to-rhadoop/)
4. R 相关 [http://cos.name/author/conan/](http://cos.name/author/conan/)
5.
[http://blog.csdn.net/jack237/article/details/8210598](http://blog.csdn.net/jack237/article/details/8210598)
6. 
[http://staff.ustc.edu.cn/~zwp/teach/Stat-Comp/statcomp.htm](http://staff.ustc.edu.cn/~zwp/teach/Stat-Comp/statcomp.htm)
**Google和facebook如何应用R进行数据挖掘**
在R用户组织的主题为“R与预测分析科学”的panel会议上，有来自工业界的四位代表发表了讲话，介绍各自在工业界是如何应用R进行数据挖掘。他们分别是：
Bo Cowgill, Google
Itamar Rosenn, Facebook
David Smith, Revolution Computing
Jim Porzak, The Generations Network
他们分别介绍了在各个公司是如何使用R进行预测分析，R作为分析工具的优势和劣势，并且提供了学习案例，以下是对他们的介绍的相关总结。
Panel介绍
R作为一门编程语言在以下三个方面具有很强的优势：数据处理，统计和数据可视化。和其他数据分析工具不同的是，它是由统计学家开发的，它是免费的软件，并且可以通过用户开发的包进行扩展，目前大约有2000多个包在CRAN中。
很多包可以应用在预测分析中。Jim重点介绍了 Max Kuhn 的caret包，它提供了大量的分类和回归模型，包括神经网络和朴素贝叶斯模型。
Bo Cowgill, Google
根据Bo Cowgill 的介绍，R是google最流行的统计分析包，事实上，google也是R基础的捐助者。他讲述道：R最好的事情是，它是统计学家发明的。它最糟糕的事情是，它是统计学家发明的。无论如何，他很乐观地看待R开发者社区的发展，R文档也逐步在改进，它的性能也在逐步提高。
Google主要使用R进行数据探索和构建模型原型，它并不是应用在生产系统，在Bo的团队中，R主要运行在桌面环境中。Bo主要根据以下的流程使用R：（1）使用其他的工具提取数据；（2）将数据加载到R中；（3）使用R建模分析；（4）在生产环境中使用c++或者python实现结果模型。
Itamar Rosenn, Facebook
Itamar介绍了facebook数据团队使用R的情况，他回答了新用户提数的两个问题：预测用户是否保持在某个数据点，如果他们停留，如何预测他们在三个月之后是否还会停留。
对于第一个问题，Itamar的团队使用递归划分推断出仅仅两个数据点被预测出来用户是否保留在facebook上:(1)新用户拥有多个会话；（2）输入用户基本信息时。
对于第二个问题，他们使用最小角度回归方法建立逻辑回归模型（lars包），根据三个类别的行为发现用户三个月的活动：(1)用户被其他用户访问的频率；（2）第三方应用程序使用的频率；（3）即将访问该站点的用户。
David Smith, Revolution Computing
David的公司，R改革计算，不仅仅使用R，而且R是他们的核心业务。David描述道：他们对R的贡献类似于redhat对linux 的贡献。他的公司处理使用R遇到的一些问题，例如，（1）支持老版本软件，即向下兼容；（2）通过他们的ParallelR套件可以支持并行计算。
David展示了他们的生命科学客户是如何使用R通过randomForest包对基因组数据集进行分类处理，以及如何使用他们的foreach包对分类树分析进行并行处理。
他还提到他们和其他公司合作将R应用在生产环境中，将特定的脚本放在服务器上，用户通过客户端调用该脚本进行数据处理。
Jim Porzak, The Generations Network
Jim简单介绍了如何使用R进行市场分析。尤其是，Jim还使用flexclust为sun公司的客户数据进行聚类分析，并且应用该结果数据识别高价值销售的主导业务。
在Q&A环节，还有很多提问，并且进行了回答。
1在使用R的过程中，如何解决内存限制问题？
R工作区是在RAM上，因此他的大小是受到限制的。
办法：
（1） 使用R的数据库连接功能（例如RMySQL），对数据进行切片处理
（2） 抽样处理
（3） 在独立的服务器或者在amazon的云计算环境中运行脚本
2R如何与其他工具和语言进行交互？
CRAN里面有一些包提供了和matlab，splus，SAS，excel的交互接口，另外，还提供了与python和java的接口包（Rpy和RJava）。
