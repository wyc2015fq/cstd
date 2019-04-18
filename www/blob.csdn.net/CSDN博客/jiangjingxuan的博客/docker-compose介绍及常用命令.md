# docker-compose介绍及常用命令 - jiangjingxuan的博客 - CSDN博客





2017年02月07日 14:31:58[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：1509










[Ellon_Daris](http://www.cnblogs.com/Ellon-Daris/)



## [docker-compose介绍及常用命令](http://www.cnblogs.com/Ellon-Daris/p/5455062.html)


### compose项目简介

compose项目主要用于编排部署基于docker的应用。compose主要用于开发／测试场合。适合小规模应用的部署。compose以命令行方式运行，本身没有daemon。只提供应用编排部署的能力。


相对于kubernetes来说功能比较简单，相当于kubernetes的一个子集。

compose命令大多依赖于yml文件，默认使用当前目录下的“docker-compose.yml“，也可以通过－f指定。

### compose命令详解
- - build(构建yml中某个服务的镜像) 

如上，web这个服务是依赖于镜像build的。在本地 也存在Dockerfile文件。 

可以使用docker-compose build来构建服务的镜像。 
- ps(查看已经启动的服务状态）
- kill(停止某个服务）
- logs(可以查看某个服务的log）
- port(打印绑定的public port） 
- pull(pull服务镜像) 
- up(启动yml定义的所有服务）
- stop(停止yml中定义的所有服务）
- start(启动被停止的yml中的所有服务） 
- kill(强行停止yml中定义的所有服务）
- rm（删除yml中定义的所有服务）
- restart(重启yml中定义的所有服务） 
- scale(扩展某个服务的个数，可以向上或向下）
- migrate-to-labels(这个没有实际尝试。根据介绍是将服务从1.2迁移到1.3带labels的版本。docker之前不支持label）
- version（查看compose的版本）





