
# Rundeck基础：1：概要介绍 - 知行合一 止于至善 - CSDN博客

2019年04月10日 00:02:59[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：56


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
Rundeck是一个基于Java和Grails的开源的运维自动化工具，提供了Web管理界面进行操作，同时提供命令行工具和WebAPI的访问控制方式。像Ansible之类的工具一样，Rundeck能够帮助开发和运维人员更好地管理各个节点。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 概要信息
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Rundeck概要信息如下表所示：
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)|项目|说明|
|---|---|
|官网|[http://rundeck.org](http://rundeck.org)|
|开源/闭源|开源|
|源码管理地址|[https://github.com/rundeck/rundeck/](https://github.com/rundeck/rundeck/)|
|License类别|Apache 2.0 License|
|开发语言|Java，Grails|
|当前稳定版本|3.0.20 （2019/04/08）|
|操作系统支持|跨平台，支持Linux/Windows/Mac|
|下载地址：github release|[https://github.com/rundeck/rundeck/releases](https://github.com/rundeck/rundeck/releases)|
|下载地址：官方网站|[https://www.rundeck.com/open-source/download](https://www.rundeck.com/open-source/download)|
|下载地址：官方镜像|[https://hub.docker.com/r/rundeck/rundeck/](https://hub.docker.com/r/rundeck/rundeck/)|
# CE版 vs 商业版
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)选择收费还是不收费的版本，取决与对功能的要求以及预算的多少等多项因素，CE版自然是上述概要信息中所使用的Apache 2.0 License。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)陈本比较
而商业版本大体的预算需要投入多少，Rundeck目前的收费模式是这样的：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409105400762.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
可以看到Node/Projects/Executions都是不做限制的，可以根据需要进行裁剪，整体的费用大概的范围是2万美刀至6万美刀，这是需要每年投入的费用，详细的计价可能需要进一步的商谈了，这也是目前大部分软件的一个模式。
[
](https://img-blog.csdnimg.cn/20190409105400762.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
功能比较
功能比较的信息如下所示：
功能特性CE版本商业版本|
|---|---|---|
Linux和Windows支持有有|
细粒度权限控制有有|
日志/审计追踪有有|
Job管理有有|
目标操作系统的操作编排有有|
专业支持无有|
企业级支持无有|
PRO插件无有|
高级工作流功能无有|
从机复制与故障恢复无有|
多节点集群无有|
统一日志存储无有|
优势特性
已经有这么多工具了，Why Rundeck？结合官方，其所给出得原因如下所示：
Rundeck定位
Rundeck的目标在于为了是的企业运维更好地运行，无论是传统方式还是云端服务均能很好支持，支持用户所选者的工具和脚本语言。从中至少可以看到传统方式和云的支持，多种语言和脚本的支持的定位，提供了整体框架的功能。
运维更好的扩张
使用Rundeck能够实现运维服务的自助，提高了效率，使得运维能够更好更有效地扩张。
消除等待
更好地践行精益运动，改善响应时间，降低工单请求队列的长度。
整体管控
整体进行监控，作业内容安排妥当，一切有条不紊。
审计追踪
操作可监控，记录所有运维行为，可以事后追踪确认是否存在操作违规。
使用场景
运维支持
降低MTTR与客户的指责，同时降低支持的成本。可基于标准运维流程进行写作，限制对于敏感环境的访问支持，可以与监控与工单管理系统进行集成，同时满足审计者对于所有操作历史行为进行追踪的需求。![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409112354488.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
运维服务自助化
以更安全的方式授权作业执行与监控的功能，根据细粒度的权限进行可控地设定。可根据需要进行运维操作流程的设定，保证安全可控的基础之上提高运维操作的速度。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040911280664.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
发布管理自动化
跨环境进行应用部署的自动化管理，可手动或者自动的进行构建与部署行为的触发，Rundeck同时可以执行部署行为之后的验证性测试。![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409113120861.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
大数据与ETL操作（提取/转化/加载）
可用于创建job用于支持数据管理和分析的需要。链接不同的数据源以运行脚本和在数据处理攻击之间进行操作，可通过Rundeck创建自助服务，使得数据专家的知识分享成为标准流程，在整个组织中更好的推广。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409113402783.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
其他内容
[https://liumiaocn.blog.csdn.net/article/details/89135773](https://liumiaocn.blog.csdn.net/article/details/89135773)
参考内容
[https://www.rundeck.com/open-source](https://www.rundeck.com/open-source)

