
# PaaS参考架构国标内容摘要 - 知行合一 止于至善 - CSDN博客

2019年02月18日 20:42:33[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：940


推荐性PaaS国家标准GB/T 36327-2018于2019年1月1日开始执行，同时目前PaaS相关的标准还有国标GB/T 35301-2017，我们来摘要一下标准所关注的内容。
# GB/T 36327-2018 信息技术 云计算 平台即服务（PaaS）应用程序管理要求
PaaS应用程序管理要求相关标准信息如下所示，为推荐性现行国家标准。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216141132473.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# GB/T 35301-2017 信息技术 云计算 平台即服务（PaaS）参考架构
PaaS参考架构相关标准信息如下所示，为推荐性现行国家标准。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019021614155254.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# PaaS参考架构
PaaS参考架构主要围绕用户视图和功能视图进行了阐述与说明。
## 基本定义
PaaS：Platform as a service：云计算中能够提供部署，管理和运行应用程序能力的服务模式。
云平台（Cloud Platform）：则能够按需提供这种能力的操作环境。
## 功能视图
功能视图从非常High的Level进行了抽象，主要提到了如下 功能组件/功能层/跨层功能的组合情况。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216143445137.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
同样用户视图与功能视图的关联也是非常High Level的表述，详细如下所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216143807758.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
### PaaS分层说明
PaaS功能视图分层包括用户层/访问层/服务层三层。参考架构如下图所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216161837722.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## 用户视图
用户视图是这份标准的重点阐述内容，PaaS用户视图中定义了三种用户角色：
PaaS提供者
PaaS客户
PaaS合作者
三种用户角色与其子角色信息详细如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216144209942.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## PaaS提供者活动
PaaS提供者及其相关子角色活动详细如下所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216161203352.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## PaaS客户活动
PaaS客户是使用云平台的个人和组织，相关的活动详细如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216155151214.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
PS：ICT系统是不是加上个注解与说明更好一些
[
](https://img-blog.csdnimg.cn/20190216155151214.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
## PaaS合作者活动
[
](https://img-blog.csdnimg.cn/20190216155151214.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)PaaS合作者及其相关子角色活动详细如下所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216161427465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# PaaS平台关注的NFR
PaaS平台所关注的非功能性需求，主要包括如下内容：
可审计性
可用性
服务治理
互操作性
维护与版本控制
性能
可移植性
隐私保护
健壮性
可逆性
安全
SLA
# 参考文献
[http://www.gb688.cn/bzgk/gb/newGbInfo?hcno=A95528B497C5D9DE6496AE246CEBF68B](http://www.gb688.cn/bzgk/gb/newGbInfo?hcno=A95528B497C5D9DE6496AE246CEBF68B)
[http://www.gb688.cn/bzgk/gb/newGbInfo?hcno=657037C120BA2C1EE1D8373C01663004](http://www.gb688.cn/bzgk/gb/newGbInfo?hcno=657037C120BA2C1EE1D8373C01663004)

[
  ](https://img-blog.csdnimg.cn/20190216155151214.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)