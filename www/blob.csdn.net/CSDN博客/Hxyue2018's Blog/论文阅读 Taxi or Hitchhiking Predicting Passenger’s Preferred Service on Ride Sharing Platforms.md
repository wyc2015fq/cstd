# 论文阅读: Taxi or Hitchhiking: Predicting Passenger’s Preferred Service on Ride Sharing Platforms - Hxyue2018's Blog - CSDN博客





2018年10月12日 14:49:25[Hxyue2018](https://me.csdn.net/Super_Json)阅读数：54标签：[时空数据](https://so.csdn.net/so/search/s.do?q=时空数据&t=blog)
个人分类：[论文阅读](https://blog.csdn.net/Super_Json/article/category/8116129)









> 
这是滴滴2018年发表在SIGIR的一篇论文，论文的核心问题是网约车发展至今，提供的服务越来越多样化（如专车，快车，豪华车，出租车，代驾，电单车等），如何为不同需求的用户推荐适合的服务。主要从时间，空间以及用户的行为特征三个维度进行考虑，构造出这三个模型，实现模型的ensemble，最后评估模型效果。


### **1.问题明确**

文章主要解决的问题是针对不同需求的用户，APP智能的在首屏展现其所需服务，降低乘客多次滑动寻找服务的时间消耗，提升用户体验。因为模型主要通过用户的历史行为数据和spatio-temporal 数据预测乘客所需的服务。

![](https://img-blog.csdn.net/20181013155854741?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1N1cGVyX0pzb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图： 算法落地场景-----智能首屏显示（见红框）

### **2.相关工作及baseline**

这篇文章主要是利用两方面特征1.乘客过去选择的服务行为2.乘客当前所在的位置和时间；来构建模型，context-aware推荐系统，实际上这种基于时空数据对用户建模在信息检索和推荐系统已经较为广泛的应用。



样本选择在过去3个月成单10单的用户同时在训练时间和预测时间（最后一周）内都至少有一单，筛选出的18%的活跃用户占据过去67%的订单，筛选是由于对较新的用户没有足够多的数据进行预测

对于该问题而言，论文用精度来评价模型效果，但是由于不同业务量大小不同导致的类别不均，故此在此基础上同时使用macro-F1 score进行评估

Majority：直接首屏推荐业务线最大的业务accuracy of 0.72 and an F1-score of 0.21.

Local Majority：用户的消费水平可能呈现区域群聚特性，所以直接划分区域进行推荐accuracy of 0.74 and an F1-score of 0.31

Individual Majority：针对每个乘客的历史数据进行推荐accuracy of 0.83 and an F1-score of 0.70.

序列model—Markov转移矩阵：

Spatio-Temporal Model 利用用户起点用来理解用户目的，

**2.思考和收获**

对于这篇论文，定位为推荐还是稍微有点牵强，毕竟推荐的场景并不多样化，而且用户基本上有明确的使用意图，因此更像是一个多分类问题，论文没有使用比较高深的算法，总体比较中规中矩。主要是分立然后三个模型ensemble，主要的收获还是在于，推荐受时空影响这点有借鉴意义。主要是切入点比较新颖，



