# Spring系列学习之Spring Cloud Cluster 微服务集群 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月24日 17:29:36[boonya](https://me.csdn.net/boonya)阅读数：312








Github：[https://github.com/spring-cloud/spring-cloud-cluster](https://github.com/spring-cloud/spring-cloud-cluster)

### [Spring Cloud Cluster](https://github.com/spring-cloud/spring-cloud-cluster)

提供Leadership选举，如：Zookeeper, Redis, Hazelcast, Consul等常见状态模式的抽象和实现。



此项目现在已被Spring Integration中的代码取代。 这里的所有接口都有主Spring库中的对应部分，并且实现大部分是并行的（etcd是一个例外，因为第三方库中缺乏对v3的支持）。 1.0.1版本弃用了所有有利于Spring Integration的东西。


注： 对于Hazelcast，支持是在扩展项目中。


Spring Cloud Cluster提供了一组用于在分布式系统中构建“集群”功能的原语。 例如领导选举，集群状态的一致存储，全局锁和一次性令牌。



*注：由于[https://cloud.spring.io/spring-cloud-cluster](https://cloud.spring.io/spring-cloud-cluster)没有详细的介绍，故此篇后续再做补充。*



