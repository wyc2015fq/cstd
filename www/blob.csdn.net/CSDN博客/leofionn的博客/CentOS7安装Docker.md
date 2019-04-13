
# CentOS7安装Docker - leofionn的博客 - CSDN博客


2018年08月21日 21:10:41[leofionn](https://me.csdn.net/qq_36142114)阅读数：83个人分类：[docker																](https://blog.csdn.net/qq_36142114/article/category/7957889)


Docker 要求 CentOS 系统的内核版本高于 3.10 ，查看本页面的前提条件来验证你的CentOS 版本是否支持 Docker 。通过 uname -r 命令查看你当前的内核版本或者使用cat /etc/centos-release查看版本（支持centos7.x以上）
`uname -r`centos版本
CentOS Linux release 7.2.1511 (Core)
## 1、初步安装和启动docker
`yum update -y
yum -y install docker
systemctl start docker`
## 2、设置镜像
`vi /etc/docker/daemon.json
{
  "registry-mirrors": ["https://aj2rgad5.mirror.aliyuncs.com"]
}`
## 3、重启docker
`systemctl daemon-reload
systemctl restart docker.service`
## 4、测试docker是否正常安装和运行
`docker run hello-world`
## 5、查看结果
`Hello from Docker!
This message shows that your installation appears to be working correctly.`

