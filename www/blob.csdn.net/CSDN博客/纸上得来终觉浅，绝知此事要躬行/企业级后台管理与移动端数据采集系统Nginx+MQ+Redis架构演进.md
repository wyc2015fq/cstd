# 企业级后台管理与移动端数据采集系统Nginx+MQ+Redis架构演进 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月04日 10:48:23[boonya](https://me.csdn.net/boonya)阅读数：47
对于一个应用系统的演化进程跟用户量有直接关系，只有不断地调整自己的架构满足用户的需求才是第一位的，受益者不光是用户还有企业甚至是开发人员。一个阶段性的东西，只是针对具体情况的分析和实践，所谓架构就是高层建筑，需要考虑高可用性、可扩展性、可维护性、简单易部署。本次博客主要讲解移动端并发和附件上传对后台服务造成影响之后我们做的调整方案及后期规划，对于数据库层面数据量暴增对后端的影响本篇暂不做论述。
**目录**
[第一版](#%E7%AC%AC%E4%B8%80%E7%89%88)
[原始阶段:Tomcat+Mysql](#%E5%8E%9F%E5%A7%8B%E9%98%B6%E6%AE%B5%3ATomcat%2BMysql)
[实验阶段:Nginx转发拆分移动端附件上传接口-仅分离附件接口](#%E5%AE%9E%E9%AA%8C%E9%98%B6%E6%AE%B5%3ANginx%E8%BD%AC%E5%8F%91%E6%8B%86%E5%88%86%E7%A7%BB%E5%8A%A8%E7%AB%AF%E9%99%84%E4%BB%B6%E4%B8%8A%E4%BC%A0%E6%8E%A5%E5%8F%A3-%E4%BB%85%E5%88%86%E7%A6%BB%E9%99%84%E4%BB%B6%E6%8E%A5%E5%8F%A3)
[第一阶段:Nginx（附件静态代理+服务转发）](#%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%3ANginx%EF%BC%88%E9%99%84%E4%BB%B6%E9%9D%99%E6%80%81%E4%BB%A3%E7%90%86%2B%E6%9C%8D%E5%8A%A1%E8%BD%AC%E5%8F%91%EF%BC%89)
[第二阶段:Nginx(css/js附件等静态资源代理+服务转发)+Redis](#%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%3ANginx(css%2Fjs%E9%99%84%E4%BB%B6%E7%AD%89%E9%9D%99%E6%80%81%E8%B5%84%E6%BA%90%E4%BB%A3%E7%90%86%2B%E6%9C%8D%E5%8A%A1%E8%BD%AC%E5%8F%91)%2BRedis)
[第三阶段:Nginx前后端分离+服务或微服务集群+Redis集群+MYSQL主备](#%E7%AC%AC%E4%B8%89%E9%98%B6%E6%AE%B5%3ANginx%E5%89%8D%E5%90%8E%E7%AB%AF%E5%88%86%E7%A6%BB%2B%E6%9C%8D%E5%8A%A1%E6%88%96%E5%BE%AE%E6%9C%8D%E5%8A%A1%E9%9B%86%E7%BE%A4%2BRedis%E9%9B%86%E7%BE%A4%2BMYSQL%E4%B8%BB%E5%A4%87)
[第二版](#%E7%AC%AC%E4%BA%8C%E7%89%88)
[原始阶段:Tomcat+Mysql](#%E5%8E%9F%E5%A7%8B%E9%98%B6%E6%AE%B5%3ATomcat%2BMysql)
[实验阶段:Nginx转发拆分移动端附件上传接口-仅分离附件接口](#%E5%AE%9E%E9%AA%8C%E9%98%B6%E6%AE%B5%3ANginx%E8%BD%AC%E5%8F%91%E6%8B%86%E5%88%86%E7%A7%BB%E5%8A%A8%E7%AB%AF%E9%99%84%E4%BB%B6%E4%B8%8A%E4%BC%A0%E6%8E%A5%E5%8F%A3-%E4%BB%85%E5%88%86%E7%A6%BB%E9%99%84%E4%BB%B6%E6%8E%A5%E5%8F%A3)
[第一阶段:Nginx（附件静态代理+服务转发）](#%E7%AC%AC%E4%B8%80%E9%98%B6%E6%AE%B5%3ANginx%EF%BC%88%E9%99%84%E4%BB%B6%E9%9D%99%E6%80%81%E4%BB%A3%E7%90%86%2B%E6%9C%8D%E5%8A%A1%E8%BD%AC%E5%8F%91%EF%BC%89)
[第二阶段:Nginx(css/js附件等静态资源代理+服务转发)+Redis](#%E7%AC%AC%E4%BA%8C%E9%98%B6%E6%AE%B5%3ANginx(css%2Fjs%E9%99%84%E4%BB%B6%E7%AD%89%E9%9D%99%E6%80%81%E8%B5%84%E6%BA%90%E4%BB%A3%E7%90%86%2B%E6%9C%8D%E5%8A%A1%E8%BD%AC%E5%8F%91)%2BRedis)
[第三阶段:Nginx前后端分离+服务或微服务集群+Redis集群+MYSQL主备](#%E7%AC%AC%E4%B8%89%E9%98%B6%E6%AE%B5%3ANginx%E5%89%8D%E5%90%8E%E7%AB%AF%E5%88%86%E7%A6%BB%2B%E6%9C%8D%E5%8A%A1%E6%88%96%E5%BE%AE%E6%9C%8D%E5%8A%A1%E9%9B%86%E7%BE%A4%2BRedis%E9%9B%86%E7%BE%A4%2BMYSQL%E4%B8%BB%E5%A4%87)
[MQ数据分析能力](#MQ%E6%95%B0%E6%8D%AE%E5%88%86%E6%9E%90%E8%83%BD%E5%8A%9B)
[Redis发布订阅](#Redis%E5%8F%91%E5%B8%83%E8%AE%A2%E9%98%85)
[MQ消息队列](#MQ%E6%B6%88%E6%81%AF%E9%98%9F%E5%88%97)
# 第一版
## 原始阶段:Tomcat+Mysql
![](https://img-blog.csdnimg.cn/20190404103634898.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：前后端融合在一起，高并发应用场景下，后端受到服务器性能和tomcat处理性能的限制。
结论：实施新的架构方案，解决一部分并发问题。
## 实验阶段:Nginx转发拆分移动端附件上传接口-仅分离附件接口
![](https://img-blog.csdnimg.cn/20190404103654275.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：需要移动端从后台服务端获取移动端Token接口。
结论：真实的情况是移动端并发导致后台服务无响应，而非附件上传接口，这个跟tomcat的优化和配置有一定关系。
## 第一阶段:Nginx（附件静态代理+服务转发）
![](https://img-blog.csdnimg.cn/20190404103709885.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：最早的内存缓存数据大多是移动端数据，获取用户位置信息需要移动端提供接口给后台访问。
结论：很好地避免了移动端并发对后台服务的影响，但后端服务还有优化空间。
## 第二阶段:Nginx(css/js附件等静态资源代理+服务转发)+Redis
![](https://img-blog.csdnimg.cn/20190404103732367.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：没有实现负载均衡，MYSQL数据量快速增长问题或不能短期得到解决，并发量再上一个台阶架构难以支撑。
结论：能够提升后端管理系统页面DOM加载速率及相关组织机构的查询速度，用户体验得到提升。
## 第三阶段:Nginx前后端分离+服务或微服务集群+Redis集群+MYSQL主备
![](https://img-blog.csdnimg.cn/20190404111814678.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：MYSQL数据库主备能一定程度解决数据灾难问题，整体架构实施复杂度增加；如果采用微服务，需要前后端进行重构，工作量比较大周期长。
结论：能够很好地解决并发上来之后保证用户高可用性，依然没有解决数据量大的时候数据查询性能问题。
# 第二版
## 原始阶段:Tomcat+Mysql
![](https://img-blog.csdnimg.cn/20190404181824563.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：前后端融合在一起，高并发应用场景下，后端受到服务器性能和tomcat处理性能的限制。
结论：实施新的架构方案，解决一部分并发问题。
## 实验阶段:Nginx转发拆分移动端附件上传接口-仅分离附件接口
![](https://img-blog.csdnimg.cn/20190404181836237.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：需要移动端从后台服务端获取移动端Token接口。
结论：真实的情况是移动端并发导致后台服务无响应，而非附件上传接口，这个跟tomcat的优化和配置有一定关系。
## 第一阶段:Nginx（附件静态代理+服务转发）
![](https://img-blog.csdnimg.cn/20190404181848429.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：最早的内存缓存数据大多是移动端数据，获取用户位置信息需要移动端提供接口给后台访问。
结论：很好地避免了移动端并发对后台服务的影响，但后端服务还有优化空间。
## 第二阶段:Nginx(css/js附件等静态资源代理+服务转发)+Redis
![](https://img-blog.csdnimg.cn/20190404181902686.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：没有实现负载均衡，MYSQL数据量快速增长问题或不能短期得到解决，并发量再上一个台阶架构难以支撑。
结论：能够提升后端管理系统页面DOM加载速率及相关组织机构的查询速度，用户体验得到提升。
## 第三阶段:Nginx前后端分离+服务或微服务集群+Redis集群+MYSQL主备
![](https://img-blog.csdnimg.cn/20190404181917410.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
缺点：MYSQL数据库主备能一定程度解决数据灾难问题，整体架构实施复杂度增加；如果采用微服务，需要前后端进行重构，工作量比较大周期长。
结论：能够很好地解决并发上来之后保证用户高可用性，依然没有解决数据量大的时候数据查询性能问题。
# MQ数据分析能力
## Redis发布订阅
利用Redis的pub/Sub特性，实现消息的发布订阅。通过订阅类型进行消费处理。鉴于消息可靠性等考虑采用MQ，这种方案仅做参考。
![](https://img-blog.csdnimg.cn/20190410150022736.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
说明：
- 后端和移动端发布Redis主题消息
- 数据统计分析服务订阅Redis主题内容
- 数据统计分析服务接收到Redis订阅主题内容后进行计算
- 数据统计分析服务将计算结果放入Redis缓存起来|将结果保存到MySQL
## MQ消息队列
MQ主要的作用就是实现应用解耦、流量削峰、保证顺序执行、发布订阅、点对点通信以及异步操作问题。引入MQ主要是基于统计数据计算服务而存在，目的是为了将运算类业务需求单独提成服务独立运行，通过异步计算统计分析结果并缓存在Redis中或将周期性的统计结果保存到MySQL数据库中。
![](https://img-blog.csdnimg.cn/20190410150132668.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
**说明：**
- 后端和移动端发布MQ主题内容
- 数据统计分析服务订阅MQ主题内容
- 数据统计分析服务接收到MQ订阅主题内容后进行计算
- 数据统计分析服务将计算结果放入Redis缓存起来|将结果保存到MySQL
