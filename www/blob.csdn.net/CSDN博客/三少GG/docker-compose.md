# docker-compose - 三少GG - CSDN博客
2016年01月30日 17:04:19[三少GG](https://me.csdn.net/scut1135)阅读数：683
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

非常棒的链接： https://yeasy.gitbooks.io/docker_practice/content/compose/index.html
# Docker Compose 项目
Docker Compose 是 Docker 官方编排（Orchestration）项目之一，负责快速在集群中部署分布式应用。
本章将介绍 Compose 项目情况以及安装和使用。
## 安装
安装 Compose 之前，要先安装 Docker，在此不再赘述。
### PIP 安装
这种方式最为推荐。
执行命令。
```
$ sudo pip install -U docker-compose
```
安装成功后，可以查看 `docker-compose` 命令的用法。
docker-compose up -d 
docker-compose stop
