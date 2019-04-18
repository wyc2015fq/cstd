# Docker系列教程19-Docker Compose简介 - weixin_33985507的博客 - CSDN博客
2018年05月07日 22:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
经过前文讲解，我们可使用Dockerfile（或Maven）构建镜像，然后使用docker命令操作容器，例如docker run、docker kill等。
然而，使用分布式应用一般包含若干个服务，每个服务一般都会部署多个实例。如果每个服务都要手动启停，那么效率之低、维护量之大可想而知。
本章我们来讨论如何使用Docker Compose来轻松、高效地管理容器。为了简单起见，本章将Docker Compose简称为Compose。
Compose是一个用于定义和运行多容器Docker应用程序的工具，前身是Fig。它非常适合用在开发、测试、构建CI工作流等场景。本书所使用的Compose版本是1.10.0。
## TIPS
Compose的GitHub：[https://github.com/docker/compose](https://github.com/docker/compose)
## 本文首发
[http://www.itmuch.com/docker/19-docker-compose-summary/](http://www.itmuch.com/docker/19-docker-compose-summary/)
## 干货分享
![3865516-e702e5f8c238701f.png](https://upload-images.jianshu.io/upload_images/3865516-e702e5f8c238701f.png)
全是干货
