
# 2.09 java编写智能合约 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:44:28[尹成](https://me.csdn.net/yincheng01)阅读数：297


**1.本地环境准备**
需要有JDK和Maven
需要安装Gradle，Gradle是一个基于Maven概念的项目自动化构建开源工具
面向Java应用为主，当前其支持的语言限于Java、Groovy、Kotlin和Scala
下载地址：[https://gradle.org/releases/](https://gradle.org/releases/)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120520185053.png)
将下好的包直接解压，然后设置环境变量GRADLE_HOME，添加到PATH
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205201906512.png)
cmd输入gradle -v查看版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205201916538.png)
2.构建Java shim客户端JAR包
cmd中分别输入如下两句编译项目
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205201922400.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205201925274.png)
[](https://img-blog.csdnimg.cn/20181205201922400.png)3.CentOS环境准备
[](https://img-blog.csdnimg.cn/20181205201922400.png)将maven的zip包和gradle的zip包上传到/usr/local下解压
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202033378.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202036343.png)
配置环境变量
[](https://img-blog.csdnimg.cn/20181205202033378.png)JAVA_HOME=/usr/local/jdk1.8.0_102
JRE_HOME=/usr/local/jdk1.8.0_102/jre
GOROOT=/usr/local/go
GOPATH=/home/go_work
MAVEN_HOME=/usr/local/apache-maven-3.2.2
GRADLE_HOME=/usr/local/gradle-3.3
PATH=$PATH:$JAVA_HOME/bin:$GOROOT/bin:$GOPATH/bin:$MAVEN_HOME/bin:$GRADLE_H
OME/bin
CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
export JAVA_HOME JRE_HOME GOROOT GOPATH PATH CLASSPATH MAVEN_HOME GRADLE_HOME[](https://img-blog.csdnimg.cn/20181205202033378.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202110284.png)
使配置生效
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202115468.png)
查看版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202117120.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202119136.png)
创建.m2目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205202123923.png)
4.编写Java智能合�
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

