
# Spring基础：快速入门spring boot（9）：使用sonarqube来检查技术债务 - 知行合一 止于至善 - CSDN博客

2018年11月04日 05:41:06[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2139所属专栏：[spring快速入门](https://blog.csdn.net/column/details/13907.html)



![在这里插入图片描述](https://img-blog.csdn.net/20161203082310637)
作为代码质量检查的流行工具，比如Sonarqube能够检查代码的“七宗罪”，跟代码结合起来能够更好地提高代码的质量，让我们来看一下，刚刚写的Springboot2的HelloWorld的代码有什么“罪”。
# Sonarqube
Sonarqube可以使用docker版本快速搭建，可以参看一下Easypack整理的镜像，具体使用可以参看如下链接，这里不再赘述：
[https://hub.docker.com/r/liumiaocn/sonarqube/](https://hub.docker.com/r/liumiaocn/sonarqube/)
# 环境假定
本文使用到的sonarqube为本机32003可以访问到的服务。
# 扫描方式
可以使用sonar-scanner或者mvn sonar:sonar的方式对代码进行扫描，因为这里不是sonarqube的展开，只是解释如何与之进行集合，这篇文章使用更为便利的方式即mvn sonar:sonar。
> 使用命令：mvn sonar:sonar -Dsonar.host.url=http://localhost:32003

> 如果使用缺省的9000端口，-D可以不必使用，虽然这里都可以指定，为了避免初入者进坑太多，这里还是尽可能的一切从简。

> 结果确认
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102110058270.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 扫描结果确认
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102110744316.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 确认之后发现，有一个Bugs&Vulnerabilities。我们来确认一下相关详细信息：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102111045391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 然而，这是一个误报，详细可参看：

> [
](https://img-blog.csdnimg.cn/20181102111045391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> [https://stackoverflow.com/questions/37071032/sonarqube-close-this-configurableapplicationcontext-in-spring-boot-project](https://stackoverflow.com/questions/37071032/sonarqube-close-this-configurableapplicationcontext-in-spring-boot-project)

> [
](https://img-blog.csdnimg.cn/20181102111045391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 对应措施

> [
](https://img-blog.csdnimg.cn/20181102111045391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 因为此行是一个误报，所以直接在该行添加//NOSONAR即可除外。另外为了验证其效果，我们添加一行：String msg = “Unused Message variable”;

> [
](https://img-blog.csdnimg.cn/20181102111045391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 再执行&结果确认

> [
](https://img-blog.csdnimg.cn/20181102111045391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 使用命令：mvn sonar:sonar -Dsonar.host.url=http://localhost:32003

> [
](https://img-blog.csdnimg.cn/20181102111045391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 可以看到，Bugs已经没有了，但是因为增加的一行，出现了两个Code Smells的问题。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102112514795.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 具体确认详细信息：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102112655910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 因为Sonar认为此行代码没有被使用到，所以是无用的语句应该删除。当然这里的分析，即使没有使用springboot的框架，只要是通过maven进行整合的方式都是可以的。

> [
](https://img-blog.csdnimg.cn/20181102112655910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 代码覆盖率

> [
](https://img-blog.csdnimg.cn/20181102112655910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 但是需要注意到，这里没有代码覆盖率，在下篇文章中将介绍如何在Spring boot的项目中通过Jacoco与之结合来引入代码覆盖率的确认。

> [
](https://img-blog.csdnimg.cn/20181102112655910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 总结

> [
](https://img-blog.csdnimg.cn/20181102112655910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
> 使用SonarQube可以很好的检查出代码的问题，但是在实际的项目中需要进行裁剪和定制，扫出来的不一定有问题，没有扫出来也不一定万事大吉，但是一定程度的代码规范和质量提高还是很有用处，实际在使用的时候应该根据具体情况进行实施。

> [
            ](https://img-blog.csdnimg.cn/20181102112655910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

