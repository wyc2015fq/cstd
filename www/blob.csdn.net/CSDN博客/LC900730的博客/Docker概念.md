# Docker概念 - LC900730的博客 - CSDN博客
2017年09月09日 18:33:25[lc900730](https://me.csdn.net/LC900730)阅读数：157
## 核心概念
### Docker镜像
Docker镜像(Image)类似于虚拟机镜像，可以将其理解为一个面向Docker引擎的只读模板，包含了文件系统。 
如一个Docker镜像可以只包含一个完整的Ubuntu操作系统环境，可以称为一个Ubuntu镜像。镜像也可以安装了Apache应用程序(或用户需要的其他软件),可以把它称为一个Apache镜像。 
镜像是创建Docker容器的基础。
### Docker容器
Docker容器(Container)类似于一个轻量级的沙箱，Docker利用容器来运行和隔离应用。 
容器是从镜像中创建的应用运行实例，可以将启动、开始、停止、删除，而这些容器都是相互隔离、互不可见。
可以把容器看做是一个简易版的Linux系统环境(这包含root用户权限、进程空间、用户空间和网络空间等)，以及运行在其中的应用程序打包而成的应用盒子。 
镜像本身是只读的，容器从镜像中启动的时候，Docker会在镜像最上层创建一个可写层，镜像本身保持不变。
### Docke仓库
类似于代码仓库，是Docker集中存放镜像文件的场所。
## Docker使用命令
### 1.获取镜像
#### 默认仓库
Docker pull NAME[:TAG] 
如果不显式指定TAG，默认会选择latesr标签，即下载仓库中最新版本的镜像。 
如docker pull ubuntu:14.04 
上面的命令相当于docker pull registry.hub.docker.com/ubuntu:latest命令，即从默认的注册服务器registry.hub.docker.com中的ubuntu仓库来下载标记为latest的镜像。
#### 指定仓库
如dl.dockerpool.com下载最新的Ubuntu镜像 
docker pull dl.dockerpool.com:5000/ubuntu。 
下载镜像到本地后，即可以随时使用该镜像，利用该镜像创建一个容器，在其中运行bash应用。 
docker run -t    -i        ubuntu      /bin/bash
### 2.查看镜像信息
docker image：查看本机镜像 
docker inspect命令可以获取镜像的详细信息
## 4.容器
容器是镜像的一个运行实例。
### 4.1创建容器
docker create 
启动容器： 
docker run 等价于先执行docker create命令，在执行docker start命令 
如docker run ubuntu /bin/echo ‘hello world’ 
输出hello world之后，容器停止 
当使用docker run来创建并且启动容器时候，Docker在后台运行的标准操作为： 
- 检查本地是否存在指定的镜像，不存在就从公有仓库下载
- 利用镜像创建并启动一个容器
- 分配一个文件系统，并在只读的镜像层外面挂载一层可读写层。
- 从宿主主机配置的网桥接口中桥接一个虚拟接口道容器中去。
- 检查本地是否存在指定的镜像，不存在就从公有仓库下载
- 从地址池配置一个IP地址给容器
- 执行用户指定的应用程序
- 执行完毕后容器终止
下面的命令启动bash终端，允许用户进行交互
```
docker run -t -i ubuntu:14.04  /bin/bash
-t选项让Docker分配一个伪终端(pseudo-tty)并绑定到容器的标准输入上，-i则让容器的标准输入保持打开。
```
