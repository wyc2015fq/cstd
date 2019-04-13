
# Sonar编译问题对应：File [...] can't be indexed twice. - 知行合一 止于至善 - CSDN博客

2018年12月18日 21:00:45[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1171所属专栏：[DevOps系列之：持续构建与集成](https://blog.csdn.net/column/details/27057.html)



# 现象说明
maven的java项目，测试用例和main所在的源码文件均符合缺省写法和格式，但是在使用mvn clean sonar:sonar进行编译时提示can’t be indexed twice错误。
# 相关版本
使用如下相关版本信息：
|组件|版本|
|---|---|
|JDK|1.8|
|Maven|3.5.3|
|Sonarqube|5.6.5|
# 错误信息提示
`ERROR: Caused by: File [...] can't be indexed twice. Please check that inclusion/exclusion patterns produce disjoint sets for main and test files`
# 相关信息说明
maven的pom中对于源码和测试代码的指定可以通过如下设定进行：
<sourceDirectory>源码目录</sourceDirectory>
<testSourceDirectory>测试代码目录<testSourceDirectory>
sonar中对于源码目录和测试代码目录的设定可以通过如下方式进行：
sonar.sources=源码目录
sonar.tests=测试代码目录
# 对策
可以通过设定sonar的exclusion/inclusion属性来解决问题。通过-D传入如下信息即可解决(例)：
`sonar.sources=.
sonar.tests=.
sonar.test.inclusions=**/*Test*/**
sonar.exclusions=**/*Test*/**`
# 参考内容
[https://github.com/SonarOpenCommunity/sonar-cxx/wiki/FAQ](https://github.com/SonarOpenCommunity/sonar-cxx/wiki/FAQ)

