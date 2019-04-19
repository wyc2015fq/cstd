# etcd部署-docker篇 - 三少GG - CSDN博客
2017年02月28日 15:38:06[三少GG](https://me.csdn.net/scut1135)阅读数：4505
## 入门必看：[http://cizixs.com/2016/08/02/intro-to-etcd](http://cizixs.com/2016/08/02/intro-to-etcd)
## Running etcd in standalone mode
[https://coreos.com/etcd/docs/latest/v2/docker_guide.html](https://coreos.com/etcd/docs/latest/v2/docker_guide.html)

etcd discovery分为两种：
自定义的discovery #利用一个已有的etcd集群来提供discovery服务来搭建一个新的etcd集群
公共的discovery     #discovery.etcd.io
这里我们使用公共的etcd discovery服务进行集群搭建：
1：创建一个用于discovery的token(size=3 表示集群的大小)
**curl -w "\n" 'https://discovery.etcd.io/new?size=3'**
#返回如下结果 作为 –discovery参数
[https://discovery.etcd.io/8ebee6723eaf3f5c7724e879f8797e85](https://discovery.etcd.io/8ebee6723eaf3f5c7724e879f8797e85)
**1.单节点etcd实例的部署**
多节点etcd集群的部署又分为以下两种情况:
#### [](https://github.com/HackerWilson/docker-etcd-deployment/blob/master/docs/zh/README.md#1%E5%A4%9A%E4%B8%AAetcd%E8%8A%82%E7%82%B9%E5%88%86%E5%88%AB%E9%83%A8%E7%BD%B2%E5%9C%A8%E4%B8%8D%E5%90%8C%E7%9A%84host)2.1多个etcd节点分别部署在不同的host
#### 2.2 同一host上部署多个etcd节点
参考文章：
https://github.com/HackerWilson/docker-etcd-deployment/blob/master/docs/zh/README.md
[https://skyao.gitbooks.io/leaning-etcd3/content/documentation/op-guide/container.html](https://skyao.gitbooks.io/leaning-etcd3/content/documentation/op-guide/container.html)
https://www.zhangluya.com/?p=147
