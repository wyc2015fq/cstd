
# 1.01 容器技术和docker简介 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:28:58[尹成](https://me.csdn.net/yincheng01)阅读数：61个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[](https://blog.csdn.net/yincheng01/article/category/7618299)



**1.部署演变**
在一台物理机部署Application
在windows或者linux操作系统部署Application
Operating System操作系统如win10，Physical server如机器
部署比较慢，要准备物理服务器服务器要部署到机房，安装操作系统，安装Application，需要各种依赖
成本非常高，买物理服务器
资源浪费，CPU和内存根本用不完
很难迁移和扩展
可能会被限定硬件厂商，早期不仅仅有X86系统
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205053422174.png)
虚拟化技术
在物理服务器上，通过Hypervisor做物理资源虚拟化，例如CPU资源，内存资源，硬盘资源
在虚拟化层之上，安装操作系统，也就是虚拟机，做物理资源调度，实现物理资源利用率的提高
一台物理机上运行很多虚拟机，部署很多单独的Application
虚拟化的优点1：资源池，一个物理机的资源分配到了不同的虚拟机里，做物理资源的管理
虚拟化的优点2：很容易扩展，加物理机或者加虚拟机
虚拟化的优点3：很容易云化，亚马逊AWS，阿里云等，大多数公司提供的服务都部署在云上，使用了虚拟化的技术
虚拟化的局限性：每一个虚拟机都是一个完整的操作系统，要给其分配资源，当虚拟机数量增多时，操作系统本身消耗的资源势必增多
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205053435809.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
2.容器的必要性
开发人员开发一个Application需要各种环境，各种依赖
运维人员部署Application时也需要搭建各种环境
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205053449697.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
容器解决了开发和运维人员之间的沟通
通过容器对整个Application打包，将打包好的Application运行在任何环境
3.容器解决的问题
解决了开发和运维之间的矛盾
在开发和运维之间搭建了一个桥梁，是实现DevOps（后面详细讲）的最佳解决方案
4.容器是什么
对软件和其依赖的标准化打包
应用之间相互隔离
共享同一个OS Kernel
可以运行在很多主流操作系统上
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205053501314.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
5.虚拟机与容器的区别
虚拟机是物理层面的隔离，容器是Application层面的隔离
底层Infrastructure是硬件资源
容器不需要Guest OS虚拟机的操作系统，而是直接在本地操作系统之上，实现Application的隔离
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205053513542.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
容器和虚拟化技术也可以集合使用
底层Infrastructure上通过Hypervisor做硬件资源虚拟化，然后创建Guest OS，也就是虚拟机，在虚拟机里安装容器docker，在容器里运行Application
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205053524253.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
6.docker是什么
是容器技术的一种实现，还要其他实现
docker是目前最流行的容器技术的实现
2004~2008年Linux已经出现了LXC，2013年docker包装了LXC，2013年三月开源，2016年docker分为企业版和社区版
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205053535650.png)
7.docker能做什么
简化配置容器最重要的功能，源代码、运行环境、配置都打包到一个容器，运行在不同的系统上
减少在部署时产生的错误
代码流水线管理
提高开发效率
隔离应用
整合服务器
调试能力
多租户
快速部署
8.docker和kubernates
docker
kubernetes，简称k8s，因为k和s之间有8个字母
9.DevOps
DevOps ——解决开发和运维间合作和沟通
不光靠docker，还有版本管理，持续集成等
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205053545905.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
10.docker的应用
在2015年的618大促中，京东大胆启用了基于Docker的容器技术来承载大促的关键业务（图片展现、单品页、团购页），当时基于Docker容器的弹性云项目已经有近万个Docker容器在线上环境运行，并且经受住了大流量的考验
2016年618中，弹性云项目更是担当重任，全部应用系统和大部分的DB服务都已经跑在Docker上，包括商品页面、用户订单、用户搜索、缓存、数据库，京东线上将会启动近15万个Docker容器
京东弹性计算云通过软件定义数据中心与大规模容器集群调度，实现海量计算资源的统一管理，并满足性能与效率方面的需求，提升业务自助上线效率。应用部署密度大幅提升，资源使用率提升，节约大量的硬件资源
docker全球范围用户量图示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205053555585.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

