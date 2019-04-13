
# 1.09 容器编排Kubernetes - 尹成的技术博客 - CSDN博客

2018年12月10日 09:35:43[尹成](https://me.csdn.net/yincheng01)阅读数：79个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**1.kubernetes介绍**
Kubernetes是Google开源的容器集群管理系统
简称k8s，2017年底，docker宣布支持k8s，宣布了k8s取得了阶段性胜利
2014年7月，Docker收购Orchard Labs，由此Docker公司开始涉足容器编排领域，Orchard Labs这家2013年由两位牛逼的年轻人创建的公司，有一个当时非常著名的容器编排工具fig，而这个fig就是docker-compose的前身
2015年初，Docker发布Swarm，开始追赶Kubernetes的脚步，正式进入容器编排领域，与k8s竞争
2017年3月，Docker公司宣布Docker企业版诞生，自此开始区分社区版和企业版，从2016年到2017年初，Docker公司的一些列动作充分展示了一个创业公司的盈利压力。Docker公司的一系列努力，并没有能让Docker Swarm走上容器编排的巅峰，相反，Kubernetes因为其优秀的架构和健康的社区环境，得到迅速发展，在生产环境中得到了广泛的应用，然后用户反馈，社区回应，良性循环了下去。2017年各大厂商都开始拥抱Kubernetes，亚马逊AWS，Microsoft Azure，VMware， 有的甚至抛弃了自家的产品
2017年底，Docker宣布在自家企业版里支持Kubernetes，和Swarm一起作为容器编排的解决方案供用户选择
在容器底层技术领域，Docker还是老大，Kubernetes底层更更多的还是选择使用containerd
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205161100685.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
swarm的架构
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205161110256.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
k8s的架构，主称为master，从称为node
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205161121466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
k8s的master节点，API Server是对外的，可以通过UI访问，Scheduler是调度模块，类似于yarn，Controller是控制模块，etcd就是分布式存储，存储k8s整个的状态和配置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205161128199.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
k8s的node节点，pod是容器中调度的最小单位，指具有相同namespace的container的组合，kubelet类似于代理，可以做创建容器的管理，kube-proxy和网络有关，做端口代理和转发，Fluentd是做日志的采集、存储和查询
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205161133871.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
k8s整体架构
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205161324233.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

