
# 为github项目搭建自动制作镜像环境 - Wesley's Blog - CSDN博客


2018年08月03日 13:35:11[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：480


# 简介
这篇博客基于2018.7在制作的一款联网FPS PVE游戏的后台，整个后台也是使用c\#进行搭建的。最近学习了一些云技术（k8s, docker相关），想把这些技术运用到游戏开发部署上来。
而这篇博客的目的，就是为了**实现从 push Code 至 github 上 –> Docker Hub 自动编译并且生成 image 的过程**
# 为Github Repo编写Dockerfile
具体编写`Dockerfile`的方法可以参考：[Docker入门](https://blog.csdn.net/yinanmo5569/article/details/81385192)
在github项目根目录下添加`Dockerfile`
具体`Dockerfile`如下：
`FROM mono
MAINTAINER w2w
# 将github项目拷贝至/app/目录
RUN mkdir /app
COPY . /app   
# 使用msbuild编译项目
RUN msbuild /app/OperationTridentBackEnd.sln /t:Build /p:Configuration=Release
# 运行项目
CMD ["mono", "/app/OperationTridentBackEnd/bin/Release/OperationTridentBackEnd.exe"]
EXPOSE 8000`
# 配置 DockerHub 与 Github 相关联
具体步骤可以参考官方文档：[Configure automated builds on Docker Hub](https://docs.docker.com/docker-hub/builds/)
配置完成之后，每次提交代码之后，docker hub都会自动执行`dockerfile`中的内容来build image，如图：
![这里写图片描述](https://img-blog.csdn.net/20180803133336764?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 使用最新的Image
等build完成之后，就可以通过`docker pull`来获取最新的image：
`docker pull wesleywang97/operationtrident-backend`或者自行手动通过github repo来build：
`docker build -t operationtrident:1.0 https://github.com/ZezhongWang/OperationTrident-BackEnd.git`效果如图
![这里写图片描述](https://img-blog.csdn.net/20180803133349944?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
附项目地址：
docker hub:
[https://hub.docker.com/r/wesleywang97/operationtrident-backend/](https://hub.docker.com/r/wesleywang97/operationtrident-backend/)
github:
[https://github.com/ZezhongWang/OperationTrident-BackEnd](https://github.com/ZezhongWang/OperationTrident-BackEnd)

