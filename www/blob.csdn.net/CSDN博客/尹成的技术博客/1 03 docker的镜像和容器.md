
# 1.03 docker的镜像和容器 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:31:12[尹成](https://me.csdn.net/yincheng01)阅读数：77个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**1.docker的底层技术实现架构**
docker提供了一个开发、打包、运行app的平台
把app和底层infrastructure（基础设施）隔离开
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506042575.png)
2.docker engine
docker engine是核心，里面有后台进程，叫dockerd，提供了REST API接口，还提供了CLI接口，可以看出，docker就是一种C/S的架构
3.整体架构
Client是客户端，命令行用的命令
中间docker_host是安装docker的机器，可以与Client在同一台机器，这里面重要的概念是镜像（Images）和容器（Containers）
Registry是存储镜像公共的服务器，类似于Github，可以存储和获取Registry
4.底层技术支持
Namespaces：做隔离pid、net、ipc、mnt、uts，隔离后，可以启多个容器
Control groups：做资源限制，可以让一个容器只用200M内存，另一个用300M等
Union file systems：Container和image的分层
5.docker image概述
文件和meta data的集合（root filesystem）
分层的，并且每一层都可以添加、改变、删除文件，成为一个新的image
不同的image可以共享相同的layer（层）
image本身是read-only的
linux有系统空间和用户空间，在系统空间之上，创建不同发行版本的image，在baseImage之上可以进行添加和删除文件，例如安装mysql，就会产生新的一层，成为一个新的image
查看本地存在的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060439122.png)
写一个Dockerfile
\#选择了一个baseImage
FROM ubuntu
\#在image之上运行了update和install
RUN apt-get update && apt-get install -y stress
\#要暴露的端口
EXPOSE 6379
\#程序入口
ENTRYPOINT ["/usr/bin/redis-server"]也可以从Registry拉取image，类似于github
例如拉取一个image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060450923.png)
查看本地image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060458382.png)
点击Explore，可以查看官方提供的一些image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060509904.png)
点进某个image，下载时，参照右边的命令就可以，也可以点击左边Tag，根据版本号获取
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060520348.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060526785.png)
[](https://img-blog.csdnimg.cn/20181205060520348.png)6.制作baseImage
[](https://img-blog.csdnimg.cn/20181205060520348.png)baseImage：基于系统的基础镜像
创建文件
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060537420.png)
\#include<stdio.h>
int main(){
  printf("hello docker\n");
}安装依赖包
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060546557.png)
编译.c文件
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060553199.png)
可以简单执行一下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060604614.png)
在当前目录创建文件Dockerfile
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060614111.png)
\#从头开始
FROM scratch
\#加到根目录
ADD hello /
\#运行command，就是根目录下的hello
CMD ["/hello"]制作baseImage
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060622841.png)
查看制作的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060629989.png)
查看image分层
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060637858.png)
运行docker
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060646458.png)
7.container概念和使用
列举当前本地正在运行的container容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060654896.png)
列举出所有容器，包括正在运行的和退出的
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506070114.png)
运行一个不存在的image，查看container运行并释放
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060706981.png)
交互式运行容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060716391.png)
克隆一个会话，查看当前正在运行的container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060723987.png)
可以在里面进行centOS相关操作，退出后
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060731303.png)
查看所有docker命令，分为Management Commands和Commands
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060739820.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060747752.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060754404.png)
删除container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060802142.png)
简写：删除一个container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060809193.png)
简写：删除一个image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060815401.png)
列举出所有的containerID
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060820891.png)
批量清理运行释放后的container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060828981.png)
container有正在运行的，还有未运行的情况下，先列出status=exited的container，然后最后只删除已经运行结束的container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060841283.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060849845.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060856102.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060902492.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060908406.png)
[](https://img-blog.csdnimg.cn/20181205060902492.png)8.创建Image的两种方式
[](https://img-blog.csdnimg.cn/20181205060902492.png)第一种
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060919183.png)
运行centos
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060927779.png)
安装上传下载工具
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060934111.png)
退出container
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506094088.png)
根据安装了lrzsz的image，创建新的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060945351.png)
查看新创建的image，与原本centos大小不一样
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205060952762.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061005248.png)
第二种
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061012933.png)
创建目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061018618.png)
创建Dockerfile文件
FROM centos
RUN yum -y install lrzsz创建image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061027818.png)
在一个临时的container中安装lrzsz
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061038126.png)
查看创建的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061044810.png)
建议采用第二种方式创建image，分享时，可以将Dockerfile分享给别人，不涉及共享层
9.Dockerfile详解
FROM
为了安全，尽量使用官方的image作为base image
FROM scratch			\#从头制作base image
FROM centos			\#使用已有的base image
FROM ubuntu:14.04	\#指定使用的base image的版本LABEL
定义了一些说明信息
LABEL Metadata不可少，就像代码的注释
LABEL maintainer="sjc@126.com"
LABEL version="1.0"
LABEL description="This is description"RUN
每执行一条RUN，就会多一层
为了美观，尽量避免无用的分层
用&&合并语句，用反斜杠合并多条命令为一行
\#反斜线换行
RUN yum -y update && yum -y install lrzsz \
	net-tools
RUN apt-get -y update && apt-get -y install lrzsz \
	net-tools
\#/bin/bash是shell的一种
RUN /bin/bash -c 'source $HOME/.bashrc;echo $HOME'WORKDIR
进入或创建目录
使用WORKDIR，不要使用RUN cd
尽量使用绝对目录
WORKDIR /root		\#进入/root目录
WORKDIR /test		\#如果没有会自动创建/test目录
WORKDIR demo
RUN pwd			\#输出结果应该是/test/demoADD and COPY
将本地的文件，添加到Docker image里
ADD还可以解压缩到指定目录
大部分情况下，优先使用COPY
ADD hello /
ADD test.tar.gz /	\#添加到根目录并解压WORKDIR /root
COPY hello test/  \#最终hello会在目录 /root/test/hello 中ENV
可以增加Dockerfile的可维护性，尽量使用
ENV MYSQL_VERSION 5.6					\#设置常量
RUN apt-get -y install mysql-server="${MYSQL_VERSION}" \
	&& rm -rf /var/lib/apt/lists/*	\#引用常量10.Dockerfile——CMD vs ENTRYPOINT
先介绍Shell和Exec格式
\#Shell格式
RUN apt-get -y install lrzsz
CMD echo "hello docker"
ENTRYPOINT echo "hello docker"\#Exec格式，需要特定的格式
RUN ["apt-get","-y","install","lrzsz"]
CMD ["/bin/echo","hello docker"]
ENTRYPOINT ["/bin/echo","hello docker"]ENTRYPOINT（比CMD使用的多）
设置容器启动时运行的命令
让容器以应用程序或者服务的形式运行
不会被忽略，一定会执行
演示如下两个Dockerfile构建出的image，运行有什么不同，shell和exec的区别
\#Dockerfile1，shell格式
FROM centos
ENV name Docker
ENTRYPOINT echo "hello $name"\#Dockerfile2，exec格式
FROM centos
ENV name Docker
ENTRYPOINT ["/bin/echo","hello $name"]先使用Dockerfile1创建文件，然后构建如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506111239.png)
运行构建出来的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061120993.png)
用Dockerfile2创建文件，然后构建如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061129891.png)
运行构建出来的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061138265.png)
linux能够识别shell，替换成ENV并执行
修改Dockerfile如下，使之能够识别Exec格式的命令
重新构建和运行image看效果
FROM centos
ENV name Docker
\#-c指明参数，-c后的命令要作为一个命令，在一个引号里
ENTRYPOINT ["/bin/bash","-c","echo hello $name"]CMD
设置容器启动后默认执行的命令和参数
如果docker指定了其他命令，CMD命令被忽略
如果定义了多个CMD，只有最后一个会执行（可以演示）
如下Dockerfile，与上面ENTRYPOINT的效果一样
FROM centos
ENV name Docker
CMD echo "hello $name"实践：再创建另一个ENTRYPOINT的image，运行对比
\#CMD
FROM centos
ENV name Docker
CMD echo "hello $name"\#ENTRYPOINT
FROM centos
ENV name Docker
ENTRYPOINT echo "hello $name"构建image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061156491.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061205667.png)
运行image，创建container，CMD的执行打印命令，被/bin/bash覆盖了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061212547.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061218880.png)
[](https://img-blog.csdnimg.cn/20181205061212547.png)11.分享docker image
[](https://img-blog.csdnimg.cn/20181205061212547.png)image名字一定要以自己docker hub的用户名开头
查看所有image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061229246.png)
登录，输入用户名和密码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061236979.png)
上传自己的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061244715.png)
删除本地的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061252226.png)
从docker hub下载自己上传的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061259272.png)
12.分享Dockerfile
直接分享image不安全
分享docker image不如分享Dockerfile
点击如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061311530.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
与gitHub账户关联
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061317898.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061328205.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061335381.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061343987.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061351523.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
[](https://img-blog.csdnimg.cn/20181205061343987.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)13.搭建私有docker registry
[](https://img-blog.csdnimg.cn/20181205061343987.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)github是公开的，也可以创建自己的仓库
官方提供了搭建私有仓库的方法
docker hub搜索registry如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506140083.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
打开后，可以查看快速搭建本地私有仓库的方法，运行如下命令即可
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061409609.png)
开启另一台安装并启动了docker的虚拟机，关机，复制当前机器就可以
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061416153.png)
查看启动了的registry
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506142370.png)
在之前的机器上安装telnet，查看端口是否可以访问
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061429807.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061436335.png)
在之前机器上重新构建image，IP是仓库机器的IP
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061447257.png)
将image放到本地私有的registry，但此时会报错，不可信任
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061453372.png)
本机创建如下文件
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061501710.png)
加入如下内容，IP是仓库机器的IP
{"insecure-registries":["192.168.174.142:5000"]}![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061513522.png)
修改docker的启动文件
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061523232.png)
加入如下内容
EnvironmentFile=-/etc/docker/daemon.json![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061533663.png)
重启docker服务
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061545353.png)
查看第二台机器的registry中有什么[http://192.168.174.142:5000/v2/_catalog](http://192.168.174.142:5000/v2/_catalog)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061554695.png)
第一台机器将image放到本地私有的registry，IP是仓库机器的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061602324.png)
浏览器再次查看如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061610454.png)
删掉第一台机器的image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061620264.png)
从第二台机器的私有仓库中下载image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061627126.png)
14.Dockerfile案例
创建python文件
from flask import Flask
app = Flask(__name__)
@app.route('/')
def hello():
    return "hello docker"
if __name__ == '__main__':
    app.run()[运行app.py](http://xn--app-bs7ij42a.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506163789.png)
安装flask
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061644843.png)
安装pip
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061650867.png)
安装flask
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061702208.png)
[再次运行app.py](http://xn--app-l01ei38icu0b18k.py)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061710888.png)
浏览器访问如下，这就创建好了一个最简单的web应用
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506171975.png)
将这个web应用打包成docker image，将image运行
创建目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061727280.png)
将app.py移到该目录下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061737948.png)
创建Dockerfile
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061744127.png)
FROM python:2.7
LABEL maintainer="Sun sjc_job@126.com"
RUN pip install flask
COPY app.py /app/
WORKDIR /app
EXPOSE 5000
CMD ["python","app.py"]构建image
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506175419.png)
运行image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061803407.png)
后台运行image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061809188.png)
查看进程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061815527.png)
15.运行中对container操作
后台运行image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061826411.png)
查看所有container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061832369.png)
交互式执行
进入运行中的容器执行/bin/bash
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061839741.png)
查看进程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061845474.png)
退出
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061850655.png)
进入container的python shell
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061858495.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061905515.png)
查看运行中容器的ip
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061913688.png)
停止container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061921162.png)
删除所有退出状态的container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061927732.png)
为container指定名称并启动
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061934214.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061941198.png)
名字也是唯一的，可以用名字停止
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061950826.png)
用名字启动
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205061956533.png)
显示容器的详细信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062002212.png)
查看container的运行日志
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062008114.png)
修改container的名称
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062014177.png)
16.Dockerfile案例2
方式一：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062023608.png)
安装stress
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062030210.png)
查看stress位置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062036351.png)
查看帮助文档
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062041558.png)
启动一个worker，占用256M内存
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506204856.png)
指定内存超出了容器内存
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062059913.png)
方式二：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062105571.png)
创建Dockerfile
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062111676.png)
FROM ubuntu
	RUN apt-get update && apt-get -y install stress
	\#运行容器，执行stress
	ENTRYPOINT ["/usr/bin/stress"]
	\#定义参数,[]是空，等待接收参数
	CMD []构建image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062120220.png)
直接交互运行，类似于查看帮助文档
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506212613.png)
后面直接加参数运行容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062132115.png)
17.对容器资源限制
虚拟机可以对资源进行限制，容器也一样可以
查看帮助文档
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062141545.png)
运行stress，200+200，用了400M内存
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062147639.png)
退出时如果卡死，打开另一个会话，查看docker进程并关闭
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062153436.png)
运行一个500M内存的程序，显示内存不够，证明内存限制生效
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062201190.png)
对CPU的限制
限制CPU的使用，这里是限制相对权重，例如一个容器权重10，另一个5，第一个是第二个的二倍
开三个窗口，前两个窗口运行指令CPU，第三个窗口top查看内存使用
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120506220993.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062214385.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062219553.png)
先运行第一窗口，然后查看top
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062227158.png)
再运行第二窗口，查看top，两个容器占用的CPU是有权重的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205062234431.png)
可以给重要的容器设置多一点，就等于设置了优先级
底层技术支持：Namespaces：做隔离pid、net、ipc、mnt、uts
Control groups：做资源限制
Union file systems：container和image的分层
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

