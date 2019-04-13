
# Jenkins实践基础：Alpine版Jenkins(LTS版) - 知行合一 止于至善 - CSDN博客

2019年01月09日 19:27:19[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：294标签：[jenkins																](https://so.csdn.net/so/search/s.do?q=jenkins&t=blog)[master																](https://so.csdn.net/so/search/s.do?q=master&t=blog)[sonarqube																](https://so.csdn.net/so/search/s.do?q=sonarqube&t=blog)[maven																](https://so.csdn.net/so/search/s.do?q=maven&t=blog)[docker																](https://so.csdn.net/so/search/s.do?q=docker&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=maven&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=sonarqube&t=blog)个人分类：[Jenkins																](https://blog.csdn.net/liumiaocn/article/category/8598907)
[
																					](https://so.csdn.net/so/search/s.do?q=sonarqube&t=blog)所属专栏：[DevOps系列之：持续构建与集成](https://blog.csdn.net/column/details/27057.html)[
							](https://so.csdn.net/so/search/s.do?q=sonarqube&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=master&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=jenkins&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=jenkins&t=blog)

![在这里插入图片描述](https://img-blog.csdn.net/20160809065608330)
在Easypack中的Alpine容器中，我们将会挑选一些非常流行的工具进行自定义设定以及进行最佳实践的整理，基本思路都是在官方镜像的最新版本之上进行强化。本次为持续集成利器Jenkins。
[
](https://img-blog.csdn.net/20160809065608330)
# 强化之处
[
](https://img-blog.csdn.net/20160809065608330)尺寸较小，base镜像基于alpine
基于官方LTS版本进行集成
可以自由调整版本
初期化时候需要交互处理去除，直接内嵌缺省用户，无须设定
内嵌pipeline等常用plugin
内嵌blueocean
内嵌maven
内嵌sonar-scanner
内嵌robotframework
内嵌kubectl客户端
内嵌docker客户端
内嵌jnlp-slave方式
[
](https://img-blog.csdn.net/20160809065608330)
# 环境变量设定
[
](https://img-blog.csdn.net/20160809065608330)常用环境变量设定如下所示：
[
](https://img-blog.csdn.net/20160809065608330)|环境变量|说明|
|---|---|
|JENKINS_MODE|JENKINS模式，可设定为master或者slave|
|JENKINS_ADMIN_ID|登陆用户ID|
|JENKINS_ADMIN_PW|登陆用户密码|
|JENKINS_MASTER_URL|slave方式启动时jnlpurl设定|
|JENKINS_SLAVE_SECRET|slave方式启动时secret设定|
|JENKINS_SLAVE_WORKDIR|slave方式启动时工作目录设定|
另外，JENKINS_OPTS和JAVA_OPTS同样可用
[
](https://img-blog.csdn.net/20160809065608330)
# Autobuild
[
](https://img-blog.csdn.net/20160809065608330)与dockerhub结合，自动构建，自由更新。
[
](https://img-blog.csdn.net/20160809065608330)
# 当前版本
[
](https://img-blog.csdn.net/20160809065608330)|项番|当前版本|jenkins官方LTS稳定最新版本|
|---|---|---|
|版本|2.150.1|2.150.1|
# docker pull
[
](https://img-blog.csdn.net/20160809065608330)`命令：docker pull liumiaocn/jenkins:2.150.1`[
](https://img-blog.csdn.net/20160809065608330)
# docker-compose方式
[
](https://img-blog.csdn.net/20160809065608330)`[root@liumiaocn master]# tree
.
├── data
└── docker-compose.yml
1 directory, 1 file
[root@liumiaocn master]# cat docker-compose.yml 
version: '2'
services:
  # jenkins service based on Jenkins LTS version
  jenkins:
    image: liumiaocn/jenkins:2.150.1
    ports:
      - "32002:8080"
      - "50000:50000"
    environment:
      - JENKINS_ADMIN_ID=root
      - JENKINS_ADMIN_PW=liumiaocn
      - JENKINS_MODE=master
    volumes:
      - ./data/:/data/jenkins
    restart: "no"
[root@liumiaocn master]#`[
](https://img-blog.csdn.net/20160809065608330)启动&确认
[
](https://img-blog.csdn.net/20160809065608330)`[root@platform master]# docker-compose up -d
Creating network "master_default" with the default driver
Creating master_jenkins_1 ... 
Creating master_jenkins_1 ... done
[root@platform master]# docker-compose ps
      Name                    Command               State                         Ports                       
-------------------------------------------------------------------------------------------------------------
master_jenkins_1   /bin/tini -- /usr/local/bi ...   Up      0.0.0.0:50000->50000/tcp, 0.0.0.0:32002->8080/tcp 
[root@platform master]#`[
](https://img-blog.csdn.net/20160809065608330)
# docker run
[
](https://img-blog.csdn.net/20160809065608330)`命令：docker run -p 32002:8080 -p 50000:50000 -e JENKINS_ADMIN_ID=root -e JENKINS_ADMIN_PW=liumiaocn -e JENKINS_MODE=master -v /tmp/data:/data/jenkins --name jenkins liumiaocn/jenkins:2.150.1`[
](https://img-blog.csdn.net/20160809065608330)
# 页面确认
[
](https://img-blog.csdn.net/20160809065608330)使用上述32002端口和root/liumiaocn的初始管理用户进行登陆
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190109191556245.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
因为plugin等已经在初始化时进行了安装，此处直接点击X跳过即可
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190109191654217.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
点击Start using jenkins即可开始使用了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190109191730225.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 相关组件确认
## Maven
`[root@liumiaocn ~]# docker exec -it master_jenkins_1 sh
/ # /usr/local/share/maven/bin/mvn -v
Apache Maven 3.6.0 (97c98ec64a1fdfee7767ce5ffb20918da4f719f3; 2018-10-24T18:41:47Z)
Maven home: /usr/local/share/maven
Java version: 1.8.0_121, vendor: Oracle Corporation, runtime: /usr/lib/jvm/java-1.8-openjdk/jre
Default locale: en_US, platform encoding: UTF-8
OS name: "linux", version: "3.10.0-693.el7.x86_64", arch: "amd64", family: "unix"
/ #`
## docker客户端
`/ # docker -v
Docker version 18.09.0, build 4d60db4
/ #`
## kubectl
`/ # kubectl version
Client Version: version.Info{Major:"1", Minor:"10", GitVersion:"v1.10.12", GitCommit:"c757b93cf034d49af3a3b8ecee3b9639a7a11df7", GitTreeState:"clean", BuildDate:"2018-12-19T11:16:52Z", GoVersion:"go1.9.3", Compiler:"gc", Platform:"linux/amd64"}
...省略
/ #`
## robot framework
`/ # robot --version
Robot Framework 3.1.1 (Python 2.7.15 on linux2)
/ #`
# github地址
|项目|URL|
|---|---|
|Jenkins-Alpine容器|[https://github.com/liumiaocn/easypack/tree/master/containers/alpine/jenkins](https://github.com/liumiaocn/easypack/tree/master/containers/alpine/jenkins)|
更新相关组件
使用上述设定，一般只需修改版本信息即可，但是注意如果是jenkins需要实现确认sha的hash值。

[
  ](https://img-blog.csdnimg.cn/20190109191654217.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)