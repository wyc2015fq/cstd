# 学习 Spring Boot 知识看这一篇就够了 - 纯洁的微笑 - CSDN博客
2018年05月28日 07:36:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：53231
从2016年因为工作原因开始研究 Spring Boot ，先后写了很多关于 Spring Boot 的文章，发表在技术社区、我的博客和我的公号内。粗略的统计了一下总共的文章加起来大概有六十多篇了，其中一部分是在技术社区做的系列课程。
我在写文章的时候将文章中的示例提取出来，作为开源代码分享了出来让大家以更方便的方式去学习(https://github.com/ityouknow/spring-boot-examples)，现在在 Gihub 上面也将近有4000个 Star，按照 Spring Boot 的标签来筛选也能排到第六位，前面都是 Cas、Spring Boot Admin 这种牛逼呼呼的开源组织。
利用业余时间还做了一个 Spring Boot 中文索引 （http://springboot.fun/）分享 Spring Boot 的学习资源和开源软件，来帮助大家寻找优秀的 Spring Boot 学习案例。今天我将分享过的文章整理出来方便大家统一查看。
在学习 Spring Boot 之前我一直建议大家先看这三篇文章，了解一下微服务的启蒙。
- 
[【译】微服务 - Martin Fowler](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484107&idx=1&sn=d5bbd6c3d3dad950ca12e06b13b5dc78&chksm=ebf6dab4dc8153a2eb9910f52beb581b489b7c5a1054bc3a9c2aea3d0d88dc729113bc2afe3b&scene=21#wechat_redirect)
- 
[微服务架构的理论基础 - 康威定律](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484170&idx=1&sn=5f93458d66937fe27fcdea575097a5ef&chksm=ebf6db75dc81526387e5b325cfb821821f1d8d247d00778616b0ee2dcac316909c5a9e16f2c2&scene=21#wechat_redirect)
- 
[微服务那点事](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484193&idx=1&sn=4791b1bc5098d9430c5f80644b0710fb&chksm=ebf6db5edc8152486bcb82cfc22533be4219d721352b1c7050b2a0d1441541fca2d036947ce2&scene=21#wechat_redirect)
如果你想直接实践，我总计写了二十篇关于基于 Spring Boot 1.X 系列文章：
- 
[springboot(一)：入门篇](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483678&idx=1&sn=461d8e3453031337c7599758dceaf675&chksm=ebf6d961dc81507743a0c0b4a830457156536da61508b56d6689ed75ff8242422df3ce4b36e2&scene=21#wechat_redirect)
- 
[springboot(二)：web综合开发](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483679&idx=1&sn=ce9ac5cdceac9e3abc1c10545401f2d4&chksm=ebf6d960dc81507667f3c1fc2c6ed445050a37309ff690b34e091d836a15e86d64a33f61d4f0&scene=21#wechat_redirect)
- 
[springboot(三)：Spring boot中Redis的使用](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483680&idx=1&sn=f58f3c07c946350420a6b95115a89d1b&chksm=ebf6d95fdc815049f5e42db9313e060159cd0b5a7987f582a36fc5ac315a1ee6948c0866af5f&scene=21#wechat_redirect)
- 
[springboot(四)：thymeleaf使用详解](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483696&idx=1&sn=6aadc07edd24815630b528fbd0ab2c4b&chksm=ebf6d94fdc8150590cfb00a2dd337f2c8a2c7dda6f98238fa0e0a560fda8ef50ef49b852df53&scene=21#wechat_redirect)
- 
[springboot(五)：spring data jpa的使用](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483697&idx=1&sn=4f0f120c9d5f6a880f0a40fb0e8f60b5&chksm=ebf6d94edc8150589a97aad7bc47d0e302a381b6ad56feb0df338095e44a57cd0227a80b8e47&scene=21#wechat_redirect)
- 
[springboot(六)：如何优雅的使用mybatis](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483704&idx=1&sn=b29019ff1d1ec032979935bd94cf544c&chksm=ebf6d947dc81505192aa69a966058dcccc9e7ff2dae5b93703f229f1939397ecad76a29cc6cf&scene=21#wechat_redirect)
- 
[springboot(七)：springboot+mybatis多数据源最简解决方案](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483709&idx=1&sn=ed9345610ab3d62fb22b2e887ac9d09f&chksm=ebf6d942dc8150542c892f1d85068573e2b219a9d473848544f15715f6028388ea16674f4632&scene=21#wechat_redirect)
- 
[springboot(八)：RabbitMQ详解](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483710&idx=1&sn=b4a29d64919479817adcbaaf829c25c5&chksm=ebf6d941dc815057d9fd617cfe22bb58ebd29a9005cd27290c23a3aeab7845a0365600ecb19d&scene=21#wechat_redirect)
- 
[springboot(九)：定时任务](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483711&idx=1&sn=25d7077cf276455f87f7cfe673623001&chksm=ebf6d940dc8150563b05c9f817583d2964ac48bb3e0ac236317ae93c41c181fd444acd693de2&scene=21#wechat_redirect)
- 
[springboot(十)：邮件服务](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483722&idx=1&sn=5be558e8636687ff653a51c97f5ec0c4&chksm=ebf6d935dc8150231f7d2001f5d1e512a7df1f5325eb6e1857b2b3756560a20587ea92425250&scene=21#wechat_redirect)
- 
[springboot(十一)：Spring boot中mongodb的使用](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483723&idx=1&sn=d465acda0f130d574f76d6e40e1dddb6&chksm=ebf6d934dc8150223d38be5507546abcffa5c6bb13fbd77d2818d8e1e3e81174265dd28cfb91&scene=21#wechat_redirect)
- 
[springboot(十二)：springboot如何测试打包部署](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483729&idx=1&sn=e7c7f64d99c27c58de2721d0d7a4cd38&chksm=ebf6d92edc815038950e55807921787bf89f040b0c36b592eb449ffacf784aeaea947ac654a8&scene=21#wechat_redirect)
- 
[springboot(十三)：springboot小技巧](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483905&idx=1&sn=c961351bebb286f55e89971a61d7c864&chksm=ebf6da7edc81536898fb10f0250086789bdb2959f7bd15f472b40b5b5aaa919cf6a332cfacf9&scene=21#wechat_redirect)
- 
[springboot(十四)：整合shiro-登录认证和权限管理](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247483909&idx=1&sn=4cbb467af285750d121d128804bc1a65&chksm=ebf6da7adc81536c992dfc7dc8332ed116d1c2e71d40fbcd720452f9a81f5f9374717d9c8771&scene=21#wechat_redirect)
- 
[springboot(十五)：springboot+jpa+thymeleaf增删改查示例](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484010&idx=1&sn=5e3438d26dbace78792eabb48351af57&chksm=ebf6da15dc8153033d5942ea5445d10b97114fe206e39e7ff3e848b9822d7817fa6a42ad41d8&scene=21#wechat_redirect)
- 
[springboot(十六)：使用Jenkins部署Spring Boot](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484415&idx=1&sn=c1720ea679ebca4329540b099e6d18e8&chksm=ebf6db80dc8152965c773ecffaf82f529618b45510b65fbfca86573ddd9bf2c0d7785ca431b0&scene=21#wechat_redirect)
- 
[springboot(十七)：使用Spring Boot上传文件](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484407&idx=1&sn=116397f734eef821ce55f4bf461bfa44&chksm=ebf6db88dc81529e8067ebc4b5051e5754a7204710b26bf286dd43e2bd245a832331d65d41cf&scene=21#wechat_redirect)
- 
[springboot(十八)：使用Spring Boot集成FastDFS](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484421&idx=1&sn=2e4daca5260cdb3be1c869996d6023c1&chksm=ebf6dc7adc81556c1cf7fb37fc1a68363504d2a7be56ae8f880b5118cd2c15a8142ba2aeccf9&scene=21#wechat_redirect)
- 
[springboot(十九)：使用Spring Boot Actuator监控应用](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484448&idx=1&sn=f3e80f42f97eff35970f2c52fc36ef94&chksm=ebf6dc5fdc8155493d82675868ef614675f26368221fb78dd07e7147b16f584017c670ff76c2&scene=21#wechat_redirect)
- 
[springboot(二十)：使用Spring-boot-admin对Spring-boot服务进行监控](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484453&idx=1&sn=523f07a11859a20a0636d8d8178b8d9a&chksm=ebf6dc5adc81554ca12391d2f4b045f70850f18e75d3112f62a95bc338f0de3d1086901d95aa&scene=21#wechat_redirect)
我们在学习 Spring Boot 的时候还带着团队做了一款 Spring Boot 的开源软件云收藏（www.favorites.ren），现在已经将云收藏升级到到了 Spring Boot 2.0 性能明显得到提升，大家可以登录网站去试试。
[Spring Boot实战：我们的第一款开源软件](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484291&idx=1&sn=11a158f1812d9aae7f4ef95e5a08117d&chksm=ebf6dbfcdc8152ead75b070b41a82d54e7dae5ea4a54fd088ee5a987649ea31de77175d771ff&scene=21#wechat_redirect)
改天写文章给大家分享以下 Spring Boot 1.0 升级到 2.0 所踩的那些坑。Spring Boot 2.0 的文章目前写了近7篇，后续还会持续更新:
- 
[Spring Boot 2.0（一）【重磅】Spring Boot 2.0权威发布](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484480&idx=1&sn=54d529155e35e9f35c3335a68b8d5da2&chksm=ebf6dc3fdc815529454bbbe285b083c4130b070058b06197acd4cc576f4aa0fd203f35034fdc&scene=21#wechat_redirect)
- 
[Spring Boot 2.0（二）：Spring Boot 2.0尝鲜-动态Banner](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484496&idx=1&sn=5c632238932da5b511a3ab84d6bb41e8&chksm=ebf6dc2fdc815539bc9f7ee36867aac70474b5e7f3a2e29b9ed2ea5dfc68b90d055c60ad4dd1&scene=21#wechat_redirect)
- 
[Spring Boot 2.0（三）：Spring Boot开源软件都有哪些?](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484508&idx=1&sn=e2fd6d05f1536914c628fd989de270cc&chksm=ebf6dc23dc81553573ba608284fa1422cc64a0b902cbf6f0946aecf8d8a88e54d9015a9b7330&scene=21#wechat_redirect)
- 
[Spring Boot 2.0（四）：使用 Docker 部署 Spring Boot](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484550&idx=1&sn=3aa97cfa889f7c2b67ffd72126902a8f&chksm=ebf6dcf9dc8155efeda8bda85dd7fddb0f4beb845d8975a3dae34c59584e6ba6f7c5ecfd7f04&scene=21#wechat_redirect)
- 
[Spring Boot 2.0  (五)：Docker Compose+Spring Boot+Nginx+Mysql 实践](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484563&idx=1&sn=2859d044cc94228bef6d9c82a891676b&chksm=ebf6dcecdc8155faeff0f73aea0b178bb47bbc16dfcef4bd5e0cf879ffd0390e7b525c2fdc89&scene=21#wechat_redirect)
- 
[Spring Boot 2.0  (六）：使用 Docker 部署Spring Boot开源软件云收藏](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484585&idx=1&sn=28d5a476c9dbba8414545f49f1fe2f35&chksm=ebf6dcd6dc8155c06ac9945f5111ecbb60daf793ab5ac5a154ef20ec1e593c82d54ad5e95653&scene=21#wechat_redirect)
- 
[Spring Boot 2.0  (七）：Spring Boot 如何解决项目启动时初始化资源](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484760&idx=1&sn=0d865d4c08ac72835dd6dd4fca839fb9&chksm=ebf6dd27dc81543102adb85e4f315502dd6a04a5ce1216e6c64dc10640b174bbc86b731edcb8&scene=21#wechat_redirect)
除过系列文章外，还有一些综合性的文章和转载过一些 Spring Boot 优秀的文章。
- 
[一文读懂 Spring Boot、微服务架构和大数据治理三者之间的故事](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484803&idx=1&sn=01be909a51b114d058d606265510d512&chksm=ebf6ddfcdc8154ea58b901286015c473cdd06da3ca78388dd1d2038ca7d1ddb5f962b6d39d6e&scene=21#wechat_redirect)
- 
[这一篇文章带你感受微服务的生和死，Spring Boot是生和死的主旋律。](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484347&idx=1&sn=f169b99cfefe6b1c584ba8bfc20a90ad&chksm=ebf6dbc4dc8152d2085a947b81e2abf44e4ccd519d447ddb37857251b21d2701fc00bc5e9065&scene=21#wechat_redirect)
- 
[Java 9和Spring Boot 2.0 纷纷宣布支持的HTTP/2到底是什么？](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484745&idx=1&sn=ece050f8b3c5da78cd2a26125763572e&chksm=ebf6dd36dc815420786cdacf001c9bcc804bb56eb1cf380f34fd0d12a8982b003230e16ed6e3&scene=21#wechat_redirect)
- 
[Spring Boot 面试题](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484766&idx=1&sn=3c8c6cb83b83dc60d76974de58409207&chksm=ebf6dd21dc81543792432cba7b92778c8a0cd91c7ee16d85287b530a68576f3662524a0449e6&scene=21#wechat_redirect)
- 
[Service Mesh：下一代微服务？](http://mp.weixin.qq.com/s?__biz=MzI4NDY5Mjc1Mg==&mid=2247484129&idx=1&sn=cf72d7fb08ae8bfc7ebb96dd66012259&chksm=ebf6da9edc8153886d15de7b2aa8035c212347bc9c11b7f81921609f5176a466f9cf947ff166&scene=21#wechat_redirect)
为了大家更方便的学习 Spring Boot ，我将写过的 Spring Boot 文章整理成了一本电子书，分为三个阶段写完分享给大家，方便大家使用电子设备来查阅，在公号内回复：**springboot** 即可免费获取。
同时为了大家更加全面的了解 Spring Boot ，六月我在 gitchat 会有一场线上的分享，大家有兴趣可以选择参加。chat 内容大概如下：
Spring Boot 2.0 的推出又激起了一阵学习 Spring Boot 热，就单从我们个人的博客的访问量大幅增加就可以感受到大家对学习 Spring Boot 的热情，那么在这么多人热衷于学习 Spring Boot 之时，我们自己也在思考： Spring Boot 为什么带来了这么大的变革。它诞生的背景是什么？Spring 企业又是基于什么样的考虑创建的 Spring Boot? 传统企业使用 Spring Boot 会给我们带来什么样变化。
点击阅读原文即可参加，偷偷告诉你参加之后分享有返利。
