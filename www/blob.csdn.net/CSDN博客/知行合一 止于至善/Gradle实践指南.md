
# Gradle实践指南 - 知行合一 止于至善 - CSDN博客

置顶2018年11月23日 05:46:17[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2163


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
扔掉了Maven的pom，捡起了groovy的拐杖，Gradle就是那个有人爱有人恨的工具。在这里我们不谈爱恨，只讲使用；不论高低，只有实例，每篇都使用最简单的示例，用于解释gradle的使用。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 基础篇
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)|序号|内容|链接|建议阅读时间|预计操作时间|
|---|---|---|---|---|
|1|简介与安装|[https://blog.csdn.net/liumiaocn/article/details/84158525](https://blog.csdn.net/liumiaocn/article/details/84158525)|3分钟|3分钟|
|2|Gradle的HelloWorld|[https://blog.csdn.net/liumiaocn/article/details/84192318](https://blog.csdn.net/liumiaocn/article/details/84192318)|3分钟|2分钟|
|3|生命周期管理|[https://blog.csdn.net/liumiaocn/article/details/84230004](https://blog.csdn.net/liumiaocn/article/details/84230004)|5分钟|3分钟|
|4|Task的使用方式|[https://blog.csdn.net/liumiaocn/article/details/84230057](https://blog.csdn.net/liumiaocn/article/details/84230057)|5分钟|5分钟|
|5|task的依赖与执行条件|[https://blog.csdn.net/liumiaocn/article/details/84230280](https://blog.csdn.net/liumiaocn/article/details/84230280)|5分钟|3分钟|
|6|多工程构建|[https://blog.csdn.net/liumiaocn/article/details/84230107](https://blog.csdn.net/liumiaocn/article/details/84230107)|5分钟|5分钟|
|7|Property使用方式|[https://blog.csdn.net/liumiaocn/article/details/84230143](https://blog.csdn.net/liumiaocn/article/details/84230143)|5分钟|5分钟|
|8|使用plugin进行jar文件打包|[https://blog.csdn.net/liumiaocn/article/details/84230323](https://blog.csdn.net/liumiaocn/article/details/84230323)|5分钟|5分钟|
|9|wrapper的使用|[https://blog.csdn.net/liumiaocn/article/details/84232536](https://blog.csdn.net/liumiaocn/article/details/84232536)|3分钟|5分钟|
|10|使用gradle init进行初始化|[https://blog.csdn.net/liumiaocn/article/details/84232595](https://blog.csdn.net/liumiaocn/article/details/84232595)|2分钟|1分钟|
|11|使用Kotlin的Gradle|[https://blog.csdn.net/liumiaocn/article/details/84478356](https://blog.csdn.net/liumiaocn/article/details/84478356)|5分钟|5分钟|
|12|gradle的Option使用说明|[https://blog.csdn.net/liumiaocn/article/details/84934340](https://blog.csdn.net/liumiaocn/article/details/84934340)|5分钟|-|
# 进阶篇
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)|序号|内容|链接|建议阅读时间|预计操作时间|
|---|---|---|---|---|
|1|结合spring boot进行web开发|[https://blog.csdn.net/liumiaocn/article/details/84935401](https://blog.csdn.net/liumiaocn/article/details/84935401)|5分钟|5分钟|
|2|结合JUnit进行单体测试|[https://blog.csdn.net/liumiaocn/article/details/84937100](https://blog.csdn.net/liumiaocn/article/details/84937100)|3分钟|3分钟|
|3|结合Jacoco进行覆盖率确认|[https://blog.csdn.net/liumiaocn/article/details/84230462](https://blog.csdn.net/liumiaocn/article/details/84230462)|5分钟|3分钟|
|4|结合Sonarqube进行代码审查|[https://blog.csdn.net/liumiaocn/article/details/84230429](https://blog.csdn.net/liumiaocn/article/details/84230429)|5分钟|5分钟|
|5|结合Jenkins进行持续集成|[https://blog.csdn.net/liumiaocn/article/details/85239816](https://blog.csdn.net/liumiaocn/article/details/85239816)|5分钟|5分钟|
|6|结合容器进行构建|TBD|-|-|
|7|结合Nexus进行构件管理|TBD|-|-|
|8|结合Artifactory进行构件管理|TBD|-|-|
|9|构建性能优化|TBD|-|-|
# 高级篇
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)未定
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)注： 如有其他感兴趣的topic也可以私信我
[
            ](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

