
# Gradle进阶：4:结合Sonarqube进行代码审查 - 知行合一 止于至善 - CSDN博客

2018年12月14日 21:55:23[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1416


![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
作为代码质量检查的流行工具，比如Sonarqube能够检查代码的“七宗罪”，跟代码结合起来能够更好地提高代码的质量，这篇文章将会介绍如何结合gradle和sonarqube对代码质量与测试覆盖率进行分析。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# Sonarqube
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)Sonarqube可以使用docker版本快速搭建，可以参看一下Easypack整理的镜像，具体使用可以参看如下链接，这里不再赘述：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)[https://hub.docker.com/r/liumiaocn/sonarqube/](https://hub.docker.com/r/liumiaocn/sonarqube/)
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 环境假定
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)本文使用到的sonarqube为本机32003可以访问到的服务。
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# gradle的sonarqube插件
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)gradle中的sonarqube的插件是org.sonarqube，用来在gradle中调用sonarqube进行代码质量分析。详细介绍请参看：
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)[https://plugins.gradle.org/plugin/org.sonarqube](https://plugins.gradle.org/plugin/org.sonarqube)
当前插件最新版本：2.6.2（更新于2018/2/13）
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 使用方式
[
](https://img-blog.csdnimg.cn/2018111707041592.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)指定plugin
在build.gradle中添加如下插件版本信息
plugins {
  id "org.sonarqube" version "2.6.2"
}添加apply plugin信息
apply plugin: "org.sonarqube"sonarqube设定信息
最简单的需求的情况下，非多工程项目做以上配置即可，接下来就是传递给gradle所需要的sonarqube的详细信息，而这些可以通过property的方式传入，详细的使用方式在前面的基础中都已介绍，此处不再赘述，主要设定的内容为：URL：systemProp.sonar.host.url=http://localhost:32003
用户名：systemProp.sonar.login=admin
密码：systemProp.sonar.password=admin
设定方式可以使用如下方式：
sonarqube {
    properties {
        property "sonar.host.url", "http://localhost:32003"
        property "sonar.login", "admin"
        property "sonar.password", "admin"
    }
}build.gradle详细信息
liumiaocn:springboot liumiao$ cat build.gradle 
buildscript {
	ext {
		springBootVersion = '2.1.1.RELEASE'
	}
	repositories {
		mavenCentral()
	}
	dependencies {
		classpath("org.springframework.boot:spring-boot-gradle-plugin:${springBootVersion}")
	}
}
plugins {
  id "org.sonarqube" version "2.6.2"
}
sonarqube {
    properties {
        property "sonar.host.url", "http://localhost:32003"
        property "sonar.login", "admin"
        property "sonar.password", "admin"
    }
}
apply plugin: 'java'
apply plugin: 'jacoco'
apply plugin: 'org.sonarqube'
apply plugin: 'org.springframework.boot'
apply plugin: 'io.spring.dependency-management'
group = 'com.liumiaocn'
version = '0.0.1-SNAPSHOT'
sourceCompatibility = 1.8
repositories {
	mavenCentral()
}

dependencies {
	implementation('org.springframework.boot:spring-boot-starter-web')
        testImplementation('org.springframework.boot:spring-boot-starter-test')
}
jacocoTestReport {
    reports {
        xml.enabled false
        html.enabled true
    }
}
check.dependsOn jacocoTestReport
liumiaocn:springboot liumiao$扫描方式
可以使用gradle sonarqube即可进行扫描
执行日志
liumiaocn:springboot liumiao$ gradle sonarqube
> Task :sonarqube
SCM provider autodetection failed. No SCM provider claims to support this project. Please use sonar.scm.provider to define SCM of your project.
Class not found: javax.annotation.Nonnull
Class not found: javax.annotation.meta.When
Class not found: javax.annotation.meta.TypeQualifierNickname
Class not found: org.junit.jupiter.api.extension.ExtendWith
BUILD SUCCESSFUL in 6s
5 actionable tasks: 1 executed, 4 up-to-date
liumiaocn:springboot liumiao$结果确认
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214214843339.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
扫描结果确认
可以看到质量扫描结果和覆盖率的信息都进行了显示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214215030273.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
脆弱性信息详细
查出了一个缺陷，实际是一个误报，详细可参看maven使用方式的详细介绍。
[https://blog.csdn.net/liumiaocn/article/details/83550282](https://blog.csdn.net/liumiaocn/article/details/83550282)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214215219167.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
代码覆盖率
代码覆盖率的详细信息也可以在sonarqube上进行直接确认
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214215433925.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
总结
使用SonarQube可以很好的检查出代码的问题，但是在实际的项目中需要进行裁剪和定制，扫出来的不一定有问题，没有扫出来也不一定万事大吉，但是一定程度的代码规范和质量提高还是很有用处，实际在使用的时候应该根据具体情况进行实施。
参考文章
[https://docs.sonarqube.org/display/SCAN/Analyzing+with+SonarQube+Scanner+for+Gradle](https://docs.sonarqube.org/display/SCAN/Analyzing+with+SonarQube+Scanner+for+Gradle)
[https://plugins.gradle.org/plugin/org.sonarqube](https://plugins.gradle.org/plugin/org.sonarqube)

