# 【学界】 直击系统领域顶会OSDI'18现场，探秘阿里集团基础设施团队 - 知乎
# 

> **文章作者：**临石
**责任编辑：**SY
文章发表于**微信公众号【运筹OR帷幄】：**[【学界】 直击系统领域顶会OSDI'18现场，探秘阿里集团基础设施团队](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/PZWcNi0DYXi3RTrmHlu2Fw)
*欢迎原链接转发，转载请私信*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

作为系统领域顶会双星之一的OSDI（USENIX Symposium on Operating Systems Design and Implementation），在当地时间10月8日-10日，美国加州Carlsbad举办了第13次会议OSDI'18。本次大会参会人数达到了创纪录的650多人，投递论文257篇，收录47篇，录取率不到20%。

本届OSDI三篇Best Paper分别来自Understanding Failures、Operating System和Debugging三大OS传统领域，组委会可谓初心不忘，聚焦系统层面的创新。3篇Best paper中两篇的第一作者都是华人学生，也是一个令人欣喜的现象。**值得一提的是，Operating System的Best Paper：LegoOS: A Disseminated, Distributed OS for Hardware Resource Disaggregation, Yizhou Shan, et al，引用了我们之前发布的Alibaba cluster trace佐证下一代resource disaggregation的设计，说明阿里巴巴的基础技术能力和场景已经开始得到顶级学术会议的重视。**

作为操作系统和体系结构领域的顶级会议，在本次OSDI'18中，阿里巴巴集团在其举办的专题研讨会（BoF）上分别介绍了阿里巴巴OS开发优化方面的最新进展，阿里云存储系统盘古2.0，以及阿里巴巴的资源管理系统Sigma在日常和双十一期间进行的多种优化和面临挑战。研讨会现场吸引了来自全世界数十位专家学者，在系统软件事业部系统性能研究员Kingsum的主持下，大家以演讲介绍加自由讨论的形式，对上述3个话题展开了热烈的讨论。

- 公众号后台发送“**OSDI**”获取阿里研讨会 PPT
![](https://pic1.zhimg.com/v2-eab430a5c36d1fa8aef62ce960e9056c_b.jpg)本次研讨会的主题以及讲师介绍

![](https://pic1.zhimg.com/v2-578e5625fabb2331cd2b2aa35bd58b18_b.jpg)研讨会现场座无虚席
## **阿里巴巴OS的开发与探索**

Alikernel 是阿里巴巴为了更好支持自身业务发展，应对多种技术挑战而进行开发、优化得到的内核系统。随着阿里巴巴集团服务器数量、业务多样性、应用混部规模的不断提高，操作系统和内核需要面临的挑战日趋增多，除了上述提到的规模问题，我们还有快速迭代、成本节约和创新性（例如serverless）的要求。

针对这些需求，基础软件-资深技术专家韩伟东（花名：绍康）详细介绍了Alikernel团队在开发流程优化、容器快速启动、dirty page回收、Parallel Mount等方面所做的工作，并且介绍我们如何积极探索异构硬件（GPU、FPGA等）支持以及更细粒度的资源隔离。
![](https://pic2.zhimg.com/v2-7030424b3b767b4c8ffbe000703d302d_b.jpg)Development Iteration of AliKernel

## **新一代分布式存储系统盘古2.0**

盘古2.0是已经在阿里巴巴广泛部署的新一代分布式存储系统，具有低延时、高IOPS的特性。盘古2.0的多级设计可以满足不同应用在多种场景下的需求，通过引入纯用户态的存储引擎USSOS和软硬件协同设计等机制，盘古2.0可以有效支持创新存储介质和RDMA网络，并大幅度提高CPU效率和NVM支持。

阿里云分布式存储资深技术专家吴忠杰（花名：储道），从现状、需求、设计、架构、新特性以及性能等多个方面，列举了盘古2.0的诸多引人注目的特性。
![](https://pic4.zhimg.com/v2-6fa62b67bfb5397627f32f120452efdb_b.jpg)新一代存储系统在成本控制、性能和QoS保障方面达到的要求

## **阿里巴巴超大规模资源管理系统Sigma**

Sigma 是阿里巴巴集团内部的资源管理系统，是阿里巴巴集团云化战略的关键系统，也是阿里巴巴集团最重要的基础设施之一。它服务于阿里巴巴集团众多经济体，如天猫、淘宝、广告、物流等。也负责双11等电商促销场景下的资源管理，从2011年起，就从容量规划、稳定性、成本控制等多个方面应对双11的极端技术挑战。

在这次分享中，来自系统软件事业部调度系统Sigma的技术专家丁海洋（花名：临石），在Sigma的日常资源管理、双十一资源资源准备和稳定性、以及整个集群的cost efficiency几个方面，介绍了Sigma作出一些优化和面临的挑战。
![](https://pic4.zhimg.com/v2-a74fced5ca39ab4bd36d4eb233ecb62f_b.jpg)Sigma-Fuxi混部架构图

Sigma的日常管理。混部方面，阿里巴巴通过资源隔离、优先级控制、多调度器协调等方式，将混部的平均CPU利用率提高了45%以上，相对之前10%的在线服务平均资源利用率，节省了大量成本。在高质量决策方面，Sigma引入强化学习机制，克服了传统启发式调度算法容易实现、难以优化的问题，大幅度资源部署率提高（在线调度场景）。同时，Sigma还引入了动态额度机制，通过分析业务负载和应用容器的能力，实现了更加激进的动态资源额度控制，有效提高了整体资源利用率。

在双十一资源准备方面。Sigma通过大量创新手段实现高质量调度决策。在线任务的调度约束十分复杂，而且属于在线调度领域的问题（调度器无法准确预知后续来到的待调度任务的特性），传统的启发式调度策略虽然方便实现，但是难以优化。为了解决这个问题，Sigma团队引入强化学习方法，根据带调度任务特性以及集群当前状态，自动对任务的调度方式进行优化决策，将部署的分配律提高了多个百分点。大幅度提升了临时负载应对能力（超过100倍日均压力），同时保证了业务的稳定性，为集团节省大量成本的同时，为商业模式的创新提供有有效的技术支持。

## **后记**

OSDI'18 参会者对阿里巴巴基础设施，尤其是系统软件方面的工作产生了极大的兴趣。斯坦福大学Platform Lab的Faculty Director John Ousterhout 也参加了本次研讨会。值得一提的是，阿里系统软件团队在和斯坦福以及很多高水平学校的老师进行合作。这次研讨会大大提高了这些高水平研究者对阿里巴巴基础设施的了解，我们非常欢迎所有有志之士加入我们，共同打造最稳定、高效的系统软件。

关于本场OSDI'18 阿里巴巴专题研讨会上的 topic 有任何问题欢迎联系阿里巴巴Sigma集群管理技术专家临石：haiyang.dhy@alibaba-inc.com

> **文章来源申明：阿里系统软件技术 优秀文章：[直击系统领域顶会OSDI'18现场，探秘阿里集团基础设施团队](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUzNzYxNjAzMg%3D%3D%26mid%3D2247483990%26idx%3D1%26sn%3D630d304d69ebeeabe26559281ffa3df3%26scene%3D21%23wechat_redirect)**

可以在 **本公众号后台 **回复关键词：“**OSDI**”可以获得阿里研讨会 PPT！~

**『运筹OR帷幄』**长期招收志愿者，如果你有一技之长，爱分享，欢迎加入我们。点**击下方图片**即可了解详情！

扫二维码关注『运筹OR帷幄』公众号：
![](https://pic1.zhimg.com/v2-872dc2d2c5eaf08b38d693490549136c_b.jpg)
[点击查看『运筹OR帷幄』志愿者招募介绍及加入方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486166%26idx%3D1%26sn%3Da598e1475ae4f91afb7c33114ee61a85%26chksm%3Df9706d88ce07e49ec590fc2589d251bbf2b7d605cd127b1a0bdff61b8bad985eddf42365d823%26scene%3D21%23wechat_redirect) ：
![](https://pic1.zhimg.com/v2-d1fbc7022dc62969dcf8a6afdacdfcb4_b.jpg)
[点击查看【骥寻伯乐】板块说明及投稿方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzUxMTYwMzI0OQ%3D%3D%26mid%3D2247486137%26idx%3D2%26sn%3D24e9487d15a8c590645a4d83950c8f20%26chksm%3Df9706de7ce07e4f1507edd01511920bd8f0a3a80384aef8ee8768b68931f872bc6cff5d3485f%26scene%3D21%23wechat_redirect) ：
![](https://pic3.zhimg.com/v2-88ba2272456e93bb2f6b9bf2d55ef1d2_b.jpg)

