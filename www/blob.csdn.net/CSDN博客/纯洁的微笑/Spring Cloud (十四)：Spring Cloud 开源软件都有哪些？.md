# Spring Cloud (十四)：Spring Cloud 开源软件都有哪些？ - 纯洁的微笑 - CSDN博客
2018年08月07日 08:06:42[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：865
学习一门新的技术如果有优秀的开源项目，对初学者的学习将会是事半功倍，通过研究和学习优秀的开源项目，可以快速的了解此技术的相关应用场景和应用示例，参考优秀开源项目会降低将此技术引入到项目中的成本。为此抽了一些时间为大家寻找了一些非常优秀的 Spring Cloud 开源软件供大家学习参考。
上次写了一篇文章[《Spring Boot 2.0(三)：Spring Boot 开源软件都有哪些》](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484508&idx=1&sn=e2fd6d05f1536914c628fd989de270cc&chksm=ebf6dc23dc81553573ba608284fa1422cc64a0b902cbf6f0946aecf8d8a88e54d9015a9b7330&scene=21#wechat_redirect)给大家介绍优秀的 Spring Boot 开源项目，本篇文章给介绍 Spring Cloud 的优秀开源项目。Spring Cloud 开源项目主要集中在 Github/码云 ，本文所有项目地址也均来自于这两个网站。
## 1、 awesome-spring-cloud
首先给大家介绍的就是 Spring Cloud 中文索引，这是一个专门收集 Spring Cloud 相关资料的开源项目，也有对应的导航页面。
**产品主页**
http://springcloud.fun/
**项目主页**
https://github.com/ityouknow/awesome-spring-cloud
**产品截图**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCxacZNbUpE2GM44xhWhtc45ibXWNEmffeibdLBOicPlKVJNe1GHibOAV6tg/640?wx_fmt=png)
## 2、 PiggyMetrics
一个简单的个人财务系统，基于 Spring Boot，Spring Cloud 和 Docker 简单演示了微服务的架构模式，整个项目几乎包含了 Spring Cloud 的所有特性包括 配置中心、Gateway zuul API 网关、Eureka 服务发现、Hystrix、Turbine仪 表盘应用健康监控等等。
PiggyMetrics 被分解为三个核心微服务。这些服务都是围绕某些业务能力组织的可独立部署的应用程序。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCbXyafnRia3vw4INCyxauVE2tWOuORILdcMDWZqHRZSqHcRypeQftKWQ/640?wx_fmt=png)
PiggyMetrics 的项目架构图
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCS3ht6V8NTBkNdcfbKfQsuKMFhrjZiaV5zmGpcX1waEibrBvyg1WHpnzA/640?wx_fmt=png)
**项目主页**
https://github.com/sqshq/PiggyMetrics
**产品截图**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCIiaribLbdxibLHwKqSwkzj2IhoyokE2j96WAB5k8WcibrJGnvUNM1GibWWw/640?wx_fmt=png)
## 3、 spaascloud-master
spring cloud + vue 全家桶实战，模拟商城，完整的购物流程、后端运营平台，可以实现快速搭建企业级微服务项目。
功能点：
模拟商城，完整的购物流程、后端运营平台对前端业务的支撑，和对项目的运维，有各项的监控指标和运维指标。
技术点：
核心技术为springcloud+vue两个全家桶实现，采取了取自开源用于开源的目标，所以能用开源绝不用收费框架，整体技术栈只有
阿里云短信服务是收费的，都是目前java前瞻性的框架，可以为中小企业解决微服务架构难题，可以帮助企业快速建站。由于服务
器成本较高，尽量降低开发成本的原则，本项目由10个后端项目和3个前端项目共同组成。真正实现了基于RBAC、jwt和oauth2的
无状态统一权限认证的解决方案，实现了异常和日志的统一管理，实现了MQ落地保证100%到达的解决方案。
**产品主页**
http://mall.paascloud.net/index
**项目主页**
https://github.com/paascloud/paascloud-master
**产品截图**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaC5BIia886H6FmiaoxLRhQOEQf0PbrNapKWOYkvVMsynUfjfD93bI0dPgg/640?wx_fmt=png)
## 4、 Cloud-Admin
Cloud-Admin是国内首个基于Spring Cloud微服务化开发平台，核心技术采用Spring Boot2以及Spring Cloud Gateway相关核心组件，前端采用vue-element-admin组件。具有统一授权、认证后台管理系统，其中包含具备用户管理、资源权限管理、网关API管理等多个模块，支持多业务系统并行开发，可以作为后端服务的开发脚手架。代码简洁，架构清晰，适合学习和直接项目中使用。
**项目主页**
https://gitee.com/minull/ace-security
**项目架构**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCFGlWPEY5zatrZLKe01ibfJP7wlicomWjKH8G2bnavEGXyCgA6zhlqPwA/640?wx_fmt=png)
## 5、 spring-cloud-rest-tcc
基于Spring Cloud Netflix的TCC柔性事务和EDA事件驱动示例，结合Spring Cloud Sleuth进行会话追踪和Spring Boot Admin的健康监控，并辅以Hystrix Dashboard提供近实时的熔断监控.
**项目主页**
https://github.com/prontera/spring-cloud-rest-tcc
**项目架构**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCUIcrocaeR9MZqXg1hjqpbNAKs4kicj6AM4Dq7IibU4aszjXibK72JTfQg/640?wx_fmt=png)
## 6、 pig
基于Spring Cloud、oAuth2.0开发，基于Vue前后分离的开发平台，支持账号、短信、SSO等多种登录
**产品主页**
https://www.pig4cloud.com/
**项目主页**
https://gitee.com/log4j/pig
**产品截图**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCgZBgQZWmuBbtf4icJicH2Ww33sJ4aqcI5OJn5NPI4jhBShYITVTSYmIw/640?wx_fmt=png)
## 7、 xxpay-master
XxPay聚合支付使用Java开发，包括spring-cloud、dubbo、spring-boot三个架构版本，已接入微信、支付宝等主流支付渠道，可直接用于生产环境。
**产品主页**
http://www.xxpay.org/
**项目主页**
https://gitee.com/jmdhappy/xxpay-master
**产品截图**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCmRb0ibA7IQ7ic2tZtTCicS034y18ERKnHqzfqOaaicH8GrHmSYIicHJFRdA/640?wx_fmt=jpeg)
## 8、 spring-boot-cloud
基于 Spring Boot、Spring Cloud、Spring Oauth2 和 Spring Cloud Netflix 等框架构建的微服务项目
**项目主页**
https://github.com/zhangxd1989/spring-boot-cloud
**项目架构**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaC2FLicdXKh2ibTh2ENeWtVGTGRh935YE1oLW1MUSbVNL8MbaVIaeh3DBQ/640?wx_fmt=jpeg)
## 9、 FCat
FCat项目基于 Angular 4 + Spring Cloud 的企业级基础功能框架。
**项目主页**
https://gitee.com/xfdm/FCat
**项目架构**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCsPxqzFwrDiaWZx5xPa8ZZIRvY8W0xA0dy384KHic7sRcMr8ZRd59qowQ/640?wx_fmt=png)
## 10、 spring-cloud-examples
Spring Cloud 技术栈示例代码，快速简单上手教程，一个帮助大家学习 Spring Cloud 的开源示例项目，每个 Spring Cloud 组件都有独立的示例供大家参考学习。
**项目主页**
https://github.com/ityouknow/spring-cloud-examples
**项目截图**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpNicqJWEbhMwLb1bGCM5LiaCrjcfaSq4x0pwbIwic6tPpj3Mviao3Sb17tqbXvE6fyjIpggziaicag1PAg/640?wx_fmt=png)
> 
应该还有更多优秀的 Spring Cloud 开源项目，目前仅发现这些，也希望大家多反馈一些优秀的 Spring Cloud 开源项目，统一将这些项目收集到 awesome-spring-cloud 中，方便后续大家学习查找。
END
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpcR0YpEk9cSITT9srthjLfnyo5a7BroaQnXA0uMk8tvhZtMVLkQDYSsXP1ibtlmYibkvBLRRe6ibD7g/640?wx_fmt=png)
