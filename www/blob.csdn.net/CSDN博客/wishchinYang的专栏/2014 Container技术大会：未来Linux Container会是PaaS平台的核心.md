# 2014 Container技术大会：未来Linux Container会是PaaS平台的核心 - wishchinYang的专栏 - CSDN博客
2014年12月28日 18:07:22[wishchin](https://me.csdn.net/wishchin)阅读数：611
个人分类：[PaaS																[IAAS](https://blog.csdn.net/wishchin/article/category/2802193)](https://blog.csdn.net/wishchin/article/category/2802251)
                
不应错过2014 Container技术大会的九大理由。 
**一、Docker官方人员再次来到北京，首次向中国布道Docker技术。**2013年Docker高级软件工程师Jerome Petazzoni[，曾在云计算大会上就对“重量级”虚拟化和“轻量级“虚拟化的特点和适用场景](http://www.csdn.net/article/2013-06-06/2815591)，发表过演讲。当时的Docker项目刚刚起步，今年他将首次代表Docker官方，向我们展示一个成熟的轻量级虚拟化技术。
**二、探秘Google的Container技术。**Google内部早在10多年前就已在生产环境中使用Container技术，cgroup就是来自Google的Paul Menage的贡献，并在2007年从2.6.24版本开始被写入内核。2014年6月的DockerCon大会上，Google正式启动Kubernetes项目，Kubernetes为Docker构建了一个精致的、可扩展和便携式管理框架，据说这是Google内部神秘项目Borg的开源版本。系统一经开源，[就得到了IBM、红帽、微软、Docker、Mesosphere、CoreOS和SaltStack等厂商的支持](http://www.csdn.net/article/2014-07-11/2820620-welcome-microsoft-redhat-ibm-docker-and-more-to-the-kubernetes-community)。本次大会，我们也正在邀请Google的Kubernetes项目核心开发者莅临现场，借由他的演讲，或可窥探Google内部的神秘技术。
**三、国内最佳实践首度公开。**Docker/Container能做什么？Docker/Container如何实践？Docker/Container都有哪些坑儿？Docker/Container会对公有云产生哪些影响？这是一线技术人员最关心的话题。本次大会，腾讯云和搜狐云景将用实践告诉你答案。在腾讯内部，大家所熟知的QQ空间，相册，农场等很多系统，底层都跑在Container上的，腾讯云的弹性web引擎也是基于Container构建的。搜狐云景也是国内早期的Container实践者之一。
**四、Docker网络和安全技术。**Docker从2013年发起距今仅有1年多，技术还不成熟，网络和安全一直是Docker的短板。国鼎网络空间安全技术有限公司，高级副总裁/CTO艾奇伟（大鹰）将解析Docker的安全性。道里云公司CEO毛文波将带来“为云服务连通Docker，基于网络虚拟化实现的云资源集约”。
**五、逐渐清晰的PaaS乱局。**PaaS平台纷纷开始拥抱Docker，红帽认为未来Linux Container会是PaaS平台的核心，Docker镜像也可能会成为分发中间件的标准方式。 [刚刚发布的OpenShift
 V3版本中](https://www.openshift.com/blogs/openshift-v3-platform-combines-docker-kubernetes-atomic-and-more)，集成了Docker、Kubernetes（由Google开源的Container编排工具）、Geard（用于将多个宿主机间的Docker容器集成到systemd中，红帽出品）和Atomic（红帽开源的Docker容器管理和部署工具）。 VMware的想法也是不谋而合，近日 [Cloud
 Foundry的runtime开始支持Docker](http://thenewstack.io/docker-on-diego-cloud-foundrys-new-elastic-runtime)，被写入Diego组件。IBM是DevOps最坚定的推崇者，自然不会错过Container技术。来自红帽、VMware、IBM的讲师，都将在2014 Container技术大会中发表演讲，PaaS乱局下的层层迷雾或将散去。
**六、Docker中文社区**。 在Docker技术为大家所知的过程中，Docker中文社区一直致力于在国内推广Docker及相关的技术，以及Docker在国内的存储服务。Docker中文技术社区创始人马全一，将在本次大会中，介绍如何构建Docker的开发环境，以及如何为Docker增加功能。
**七、领略风生水起的Docker创业技术。**2014年，围绕着Docker衍生了大量的创业公司和开源项目，CoreOS完成800万美元的A轮融资后，紧接着就收购私有Docker仓库服务商Quay.io；Docker容器托管服务Tutum刚刚获得了由RTC Venture领投的265万美金的天使融资。本次大会，Tutum软件工程师冯洪林及Splunk 软件工程师邵育亮将在大会发表演讲，带来Container镜像的相关技术；北京引众思源科技有限公司CEO，VisualOps创始人赵鹏，将带来“Docker的服务发现解决方案”；北京数字睿博科技发展有限公司创始人桂阳，将带来“CoreOS与容器管理”。
**八、活动启动前的增值服务。**目前我们已经建立**Container技术大会微信群**，所有讲师都已在微信群中。凡购买门票的同学，都可以优先**受邀**入群。既可以提前与所有讲师进行互动交流，又能在会后持续与一线开发者保持学习交流。 更为关键的是，无论是寻找志同道合的创业伙伴，还是扩充团队技术骨干，这都是最佳的沟通平台。
**九、2014 Container技术大会的举办时间是10月18日(周六)。**不耽误工作时间，用自己的休息时间来参与技术培训！抢占最新技术风口。 如果您的公司正在或有志于Docker/Container技术开发，想抓住新技术实现技术新跨越，那就不应该错过本次**[2014 Container技术大会](http://con2.csdn.net/)。**
最后，**2014Container技术大会**议题已经确定，内容足够干，讲师绝对经验丰富，还等什么？Go！
|议题|演讲嘉宾|
|----|----|
|The Docker Project Today and What's Next|**Jerome Petazzoni  ** Docker高级软件工程师|
|Use Containers to Build Scalable, Reliable Systems|**Brendan Burns（确认中）**   Google高级软件工程师|
|腾讯云docker实践（拟）|**刘永峰  **   腾讯云高级产品经理|
|Container在搜狐云景的实践（拟）|**于顺治**     搜狐技术中心搜狐云景经理|
|Docker的安全性解析|**艾奇伟（大鹰）**  国鼎网络空间安全技术有限公司，高级副总裁/CTO|
|为云服务连通Docker，基于网络虚拟化实现的云资源集约|**毛文波**    道里云公司CEO|
|红帽Docker实践（拟）|**蔡书**        红帽软件公司资深解决方案架构师|
|IBM公有云平台Bluemix: DevOps 体验，弹性运行时系统与轻量级容器|**杨博**        IBM中国开发中心云平台主架构师|
|Container技术在Cloud Foundry中的应用和演化|**喻勇**        VMware中国研发中心高级经理 **孙宏亮 **   浙江大学计算机学院研究生|
|构建 Docker 的开发环境|**马全一**    Docker中文社区创始人|
|How we build the Tutum image|**邵育亮**    Splunk 软件工程师 **冯洪林**    Tutum软件工程师|
|Docker的服务发现解决方案|**赵鹏 **       北京引众思源科技有限公司CEO，VisualOps创始人|
|CoreOS与容器管理|**桂阳**       北京数字睿博科技发展有限公司创始人|
|合而不同——OpenStack&Docker|**李明宇**    中国科学院软件研究所工程师|
