# Windows Server 2016学习 - csdn_baotai的博客 - CSDN博客

2019年03月02日 19:28:10[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：55


- 请描述：远程管理服务器的常用工具。

> 
Active Directory管理中心 Active Directory用户和计算机 DNS管理器 事件查看器 组策略管理器 IIS管理器 性能监视器 资源监视器 任务计划

- 请描述：powerShell的基本语法结构

> 
动词—名词

Get-Help<命令>

Get-Command

Windows PowerShell命令动词：get\New\Set\Restart\Resume\Stop\Suspend\Clear\Limit\Remove\Add\Show\Write

- 请描述：DAS，NAS，SAN的主要区别？

> 
DAS（直接连接存储，Direct Attached Storage）：磁盘与服务器物理连接

优点：易于配置，廉价。

缺点：存在单点故障隐患，访问速度慢。

应用场景：电脑与磁盘的连接，可以分区和格式化。

NAS(网络附加存储，Network Attached Storage)：连接到专用存储设备，并通过网络 共享方式访问的一种存储。

优点：相对连接（相对于SAN），易于配置。

缺点：访问速度较慢，不适合企业级的解决方案

应用场景：类似于百度云盘，不能进行分区和格式化。

SAN（存储区域网络，Storage Area Network）：提高更高的可用性和更大的灵活性。

优点：访问速度最快，易于扩展，集中式存储，级别高的冗余。

缺点：更加昂贵，需要特殊技能。

应用场景：大型企业，可以进行分区和格式化

- 请描述RAID技术的主要功能？

> 
将多块磁盘组合成一个逻辑单元，提供容错和高性能。

- 操作：熟练安装Windows Server 2016操作系统配置系统的Ip参数，计算机名称，防火墙配置磁盘卷空间分配。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190302192553541.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp,size_16,color_FFFFFF,t_70)

安装Windows Server 2016操作系统

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190302192609768.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp,size_16,color_FFFFFF,t_70)

Ip参数配置

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019030219263647.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp,size_16,color_FFFFFF,t_70)

配置计算机名称

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190302192657394.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp,size_16,color_FFFFFF,t_70)

初始化磁盘

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190302192708979.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp,size_16,color_FFFFFF,t_70)

新建简单卷、

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190302192726774.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp,size_16,color_FFFFFF,t_70)

添加磁盘

