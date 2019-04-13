
# Mac下Java开发环境设定：JDK1.8+Maven3.5.4+IDEA设定 - 知行合一 止于至善 - CSDN博客

2018年10月30日 22:16:26[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2619所属专栏：[spring快速入门](https://blog.csdn.net/column/details/13907.html)



这篇文章整理一下Mac下Java开发常用链接和设定。
# java开发
## JDK下载
|下载内容|下载地址|
|---|---|
|JDK|[https://www.oracle.com/technetwork/java/javase/downloads/index.html](https://www.oracle.com/technetwork/java/javase/downloads/index.html)|
比如这里下载JDK8相关的版本:
[https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html](https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)
## 安装与设定
安装： 双击直至安装完成
安装确认：可以看到安装的有JDK9和JDK1.8
`liumiaocn:~ liumiao$ ls /Library/Java/JavaVirtualMachines/
jdk-9.0.4.jdk    jdk1.8.0_191.jdk
liumiaocn:~ liumiao$`设定：在$HOME下的.bash_profile中添加如下内容：
`export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_191.jdk/Contents/Home`
## 版本确认
在新的terminal即可确认当前版本
`liumiaocn:~ liumiao$ java -version
java version "1.8.0_191"
Java(TM) SE Runtime Environment (build 1.8.0_191-b12)
Java HotSpot(TM) 64-Bit Server VM (build 25.191-b12, mixed mode)
liumiaocn:~ liumiao$`
# Maven
## 依赖
> Maven3.3以上的版本需要JDK1.7及其以后的版本

## Maven下载
|下载内容|下载地址|
|---|---|
|Maven|[http://maven.apache.org/download.cgi](http://maven.apache.org/download.cgi)|
比如这里下载Maven3.5.4:
[http://mirror.bit.edu.cn/apache/maven/maven-3/3.5.4/binaries/apache-maven-3.5.4-bin.tar.gz](http://mirror.bit.edu.cn/apache/maven/maven-3/3.5.4/binaries/apache-maven-3.5.4-bin.tar.gz)
## 安装与设定
安装： 解压
> 将上述tar.gz文件解压至/Library/Maven/apache-maven-3.5.4
`sh-3.2# ls /Library/Maven/apache-maven-3.5.4
LICENSE    NOTICE     README.txt bin        boot       conf       lib
sh-3.2#`设定：在$HOME下的.bash_profile中添加如下内容：
`export MAVEN_HOME=/Library/Maven/apache-maven-3.5.4/
export PATH=$PATH:$MAVEN_HOME/bin`
## 版本确认
在新的terminal即可确认当前版本
`liumiaocn:~ liumiao$ mvn --version
Apache Maven 3.5.4 (1edded0938998edf8bf061f1ceb3cfdeccf443fe; 2018-06-18T02:33:14+08:00)
Maven home: /Library/Maven/apache-maven-3.5.4
Java version: 1.8.0_191, vendor: Oracle Corporation, runtime: /Library/Java/JavaVirtualMachines/jdk1.8.0_191.jdk/Contents/Home/jre
Default locale: en_CN, platform encoding: UTF-8
OS name: "mac os x", version: "10.14", arch: "x86_64", family: "mac"
liumiaocn:~ liumiao$`
# IntelliJ IDEA
## 下载IntelliJ IDEA
根据需要下载社区版本或者试用版
|下载内容|下载地址|
|---|---|
|Intellij idea|[http://www.jetbrains.com/idea/](http://www.jetbrains.com/idea/)|
## 设定JDK
mac快捷键：comand + ;
菜单：File / Project Structure…
即可设定所需要使用的JDK
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030164039848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
设定Maven
菜单： IntelliJ IDEA/Preference（快捷键：comand + ，）
然后选择"Build, Execution, Deployment" -> "Maven"
可以看到使用是缺省的Bundled的Maven 3
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030164533116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
修改为刚刚安装的maven，从3.3.9变成了3.5.4
![在这里插入图片描述](https://img-blog.csdnimg.cn/201810301650096.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
如果有需要的话可以修改User settings和Local repository
[
](https://img-blog.csdnimg.cn/201810301650096.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)这样基本开发环境就已经Ready。
[
            ](https://img-blog.csdnimg.cn/201810301650096.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)

[
  ](https://img-blog.csdnimg.cn/20181030164533116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)