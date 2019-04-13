
# Rundeck基础：2：安装设定 - 知行合一 止于至善 - CSDN博客

2019年04月10日 00:40:53[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：26


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
这篇文章介绍一下Rundeck的安装与设定方法，并使用所提供的官方镜像进行安装与设定的介绍。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 安装方法
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
## 方法1:直接使用war文件
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)直接下载war文件，然后结合JRE即可运行，以3.0.20为例，具体步骤如下所示
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Step 1: 本地JDK或者JRE环境
Step 2: 下载Rundeck的war文件
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 下载命令：wget
> [https://dl.bintray.com/rundeck/rundeck-maven/rundeck-3.0.20-20190408.war](https://dl.bintray.com/rundeck/rundeck-maven/rundeck-3.0.20-20190408.war)
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Step 3: 启动Rundeck服务
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 启动命令：java -jar rundeck-3.0.20-20190408.war
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
## 方法2: apt-get安装（Debian/Ubuntu等）
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Step 1: 准备本地JDK或者JRE环境
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 安装命令：apt-get install openjdk-8-jdk-headless
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Step 2: 下载Rundeck的安装文件
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 下载命令：wget
> [https://dl.bintray.com/rundeck/rundeck-deb/rundeck_3.0.20.20190408-1.201904081511_all.deb](https://dl.bintray.com/rundeck/rundeck-deb/rundeck_3.0.20.20190408-1.201904081511_all.deb)
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Step 3: 安装Rundeck服务
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 安装命令：

> dpkg -i rundeck_3.0.20.20190408-1.201904081511_all.deb

> 方法3: yum安装（CentOS/RHEL等）

> Step 1: 准备本地JDK或者JRE环境

> 安装命令：yum install java-1.8.0

> Step 2: 设定Rundeck的安装yum源

> 执行命令：rpm -Uvh
> [https://repo.rundeck.org/latest.rpm](https://repo.rundeck.org/latest.rpm)

> Step 3: 安装Rundeck服务

> 安装命令：

> yum install rundeck

> 方法4: docker方式安装

> Step 1: 下载官方镜像

> docker pull rundeck/rundeck:3.0.20

> 注：目前直接docker rundeck/rundeck会出错，官方镜像没有latest的对应。需要加上版本的tag进行pull操作

> [
> root@liumiaocn ~
> ]
> \# docker pull rundeck/rundeck
> Using default tag: latest
Trying to pull repository docker.io/rundeck/rundeck
> ..
> . 
manifest
> for
> docker.io/rundeck/rundeck:latest not found
> [
> root@liumiaocn ~
> ]
> \#
> Step 2: 启动镜像

> docker run --name rundeck -e RUNDECK_GRAILS_URL=http://localhost:32044 -p 32044:4440 -v data:/home/rundeck/server/data rundeck/rundeck:3.0.20

> 登录

> 缺省方式下，Rundeck可以使用如下用户名/密码进行访问

> 用户名： admin

> 用户密码：admin
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409142435309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 这样，使用管理账号admin创建一个Project，既可以开始Rundeck的使用了

> [
](https://img-blog.csdnimg.cn/20190409142435309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 常见问题

> [
](https://img-blog.csdnimg.cn/20190409142435309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 使用docker镜像方式的情况下，在端口号有mapping或者NAT等状况下，需要设定RUNDECK_GRAILS_URL（原则：为通过浏览器能访问的地址），不然很有可能会出现admin/admin无法登录。

> [
](https://img-blog.csdnimg.cn/20190409142435309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 其他内容

> [
](https://img-blog.csdnimg.cn/20190409142435309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> [https://liumiaocn.blog.csdn.net/article/details/89135773](https://liumiaocn.blog.csdn.net/article/details/89135773)

> [
](https://img-blog.csdnimg.cn/20190409142435309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 参考内容

> [
](https://img-blog.csdnimg.cn/20190409142435309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> [https://hub.docker.com/r/rundeck/rundeck/](https://hub.docker.com/r/rundeck/rundeck/)

> [https://github.com/rundeck/docker-zoo](https://github.com/rundeck/docker-zoo)

> [https://docs.rundeck.com/downloads.html](https://docs.rundeck.com/downloads.html)


> [
](https://img-blog.csdnimg.cn/20190409142435309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
