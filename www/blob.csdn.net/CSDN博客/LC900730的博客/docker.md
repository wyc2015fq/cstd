# docker - LC900730的博客 - CSDN博客
2017年11月06日 19:13:36[lc900730](https://me.csdn.net/LC900730)阅读数：117标签：[docker](https://so.csdn.net/so/search/s.do?q=docker&t=blog)
个人分类：[docker](https://blog.csdn.net/LC900730/article/category/7160342)
docker镜像： 
由文件系统叠加而成，最底端是一个引导文件系统，即bootfs，这很像Linux/Unix的引导文件系统。 
docker镜像的第二层是root文件系统rootfs，位于引导文件系统之上。rootfs可以是一种或者多种操作系统。
Docker一个镜像可以放到另一个镜像的顶部。位于下面的镜像称为父镜像，最底部的镜像称为基础镜像。
## 构建镜像
- 使用docker  commit命令
- 使用docker  build命令和Dockerfile文件
- 使用docker  commit命令
执行后相当于进入docker的bash环境
1⃣️：创建镜像： 
docker pull registry.cn-hangzhou.aliyuncs.com/kaibb/hadoop 
2⃣️：创建容器： 
//创建master节点 
docker run -i -t –name Master -h Master registry.cn-hangzhou.aliyuncs.com/kaibb/hadoop /bin/bash 
-h：容器主机名：Master 
-name：给容器的名字(不指定的话，默认的话是字符串加数字)
-i：标志保证容器中STDIN是开启的，持久的标准输入是交互式shell的半边天 
-t：告诉Docker为要创建的容器的分配一个伪tty终端。
一旦bash结束了，这时候容器也就停止运行了。
//创建slave节点 
docker run -i -t –name Slave1 -h Slave1 registry.cn-hangzhou.aliyuncs.com/kaibb/hadoop /bin/bash
3⃣️：启动容器 
docker start Master
4⃣️：进入容器 
docker attach Master
docker客户端只需要向Docker服务器或者守护进程发出请求，服务器或者守护进程将完成所有工作并且返回结果。
镜像：可以将镜像当作容器的源代码。
创建守护式进程： 
docker run —name daemon_dave -d  ubuntu /bin/sh. -c “while true; do echo hello world ;sleep 1;done”
使用了-d参数，因此docker会将容器放到后台运行。
在容器内部运行进程：我们可以通过docker exec命令在容器内额外启动新进程。 
可以在容器内运行的进程有2种类型：后台任务和交互任务。
- 
停止守护式容器：docker stop 某container
- 
删除容器：docker rm
- 
docker pull时候出现incorrect username，解决方案是：docker login，然后输入用户名／密码。
做端口映射 
宿主机器的32768端口对应容器的22端口 
 docker run -i -t –name centos3 -h centos3  -p 32768:22 centos
