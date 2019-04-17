# TeamCity持续集成和持续交付Docker - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年03月19日 22:56:04[boonya](https://me.csdn.net/boonya)阅读数：1746








原文地址：[https://hub.docker.com/r/jetbrains/teamcity-server/](https://hub.docker.com/r/jetbrains/teamcity-server/)


##### TeamCity服务器 - 强大的持续集成和持续交付功能


这是官方的[JetBrains TeamCity](https://www.jetbrains.com/teamcity/)服务器镜像。
该镜像适用于生产用途和评估目的。


##### 镜像标签

docker映像中的所有默认标记都指向Linux映像。Windows Docker镜像有后缀`-windowsservercore``和-nanoserver`



- `jetbrains/teamcity-server`, `jetbrains/teamcity-server:latest` ([ubuntu](https://github.com/JetBrains/teamcity-docker-server/blob/master/ubuntu/Dockerfile))
- `jetbrains/teamcity-server:latest-windowsservercore`, `jetbrains/teamcity-server:latest-windowsservercore-ltsc2016` ([windowsservercore ltsc2016](https://github.com/JetBrains/teamcity-docker-server/blob/master/windowsservercore-ltsc2016/Dockerfile))
- `jetbrains/teamcity-server:latest-nanoserver`, `jetbrains/teamcity-server:latest-nanoserver-sac2016` ([nanoserver sac2016](https://github.com/JetBrains/teamcity-docker-server/blob/master/nanoserver-sac2016/Dockerfile))
- `jetbrains/teamcity-server:latest-windowsservercore-1709` ([windowsservercore 1709](https://github.com/JetBrains/teamcity-docker-server/blob/master/windowsservercore-1709/Dockerfile))
- `jetbrains/teamcity-server:latest-nanoserver-1709` ([nanoserver 1709](https://github.com/JetBrains/teamcity-docker-server/blob/master/nanoserver-1709/Dockerfile))

##### 如何使用镜像

从Docker Hub存储库中提取镜像

`docker pull jetbrains/teamcity-server`


使用以下命令在Linux容器内使用TeamCity服务器启动容器：



```
docker run -it --name teamcity-server-instance  \
    -v <path to data directory>:/data/teamcity_server/datadir \
    -v <path to logs directory>:/opt/teamcity/logs  \
    -p <port on host>:8111 \
    jetbrains/teamcity-server
```

或Windows 容器



```
docker run -it --name teamcity-server-instance
    -v <path to data directory>:C:/ProgramData/JetBrains/TeamCity
    -v <path to logs directory>:C:/TeamCity/logs
    -p <port on host>:8111
    jetbrains/teamcity-server:latest-nanoserver
```



**<path to data directory>**是作为[TeamCity数据目录](https://confluence.jetbrains.com/display/TCDL/TeamCity+Data+Directory)的主机计算机目录，其中TeamCity存储项目设置和构建结果。通过空目录为全新的开始。未设置映射时，您将在容器关闭时失去所有TeamCity设置。
**<path to logs directory>**是存储TeamCity服务器日志的主机目录。映射可以省略，但是在容器关闭时日志将会丢失，这将使问题调查变得不可能。

除了数据目录之外，TeamCity还存储一组用户并在SQL数据库中创建结果。
默认情况下，TeamCity服务器使用存储在数据目录下文件系统上的内部数据库。但是，生产使用需要[外部数据库](https://confluence.jetbrains.com/display/TCDL/Setting+up+an+External+Database#SettingupanExternalDatabase-DefaultInternalDatabase)。

要使用服务器进行生产，请务必查看并应用这些[建议](https://confluence.jetbrains.com/display/TCDL/Installing+and+Configuring+the+TeamCity+Server#InstallingandConfiguringtheTeamCityServer-ConfiguringServerforProductionUse)。

您至少需要一个TeamCity代理才能运行构建。检查[`jetbrains/teamcity-agent`](https://hub.docker.com/r/jetbrains/teamcity-agent/) 和 [`jetbrains/teamcity-minimal-agent镜像`](https://hub.docker.com/r/jetbrains/teamcity-minimal-agent/)


##### Windows容器限制

有关Windows容器中已知问题的详细信息，请参阅[已知问题](https://confluence.jetbrains.com/display/TCDL/Known+Issues#KnownIssues-TeamCityWindowsDockerImages)页面。

##### 其他命令

当需要将其他环境变量传递给服务器进程时，请使用常规-e选项。例如要传递TEAMCITY_SERVER_MEM_OPTS环境变量，请使用：



```
docker run -it --name teamcity-server-instance   \
       -e TEAMCITY_SERVER_MEM_OPTS="-Xmx2g -XX:MaxPermSize=270m -XX:ReservedCodeCacheSize=350m" \
       -v <path to data directory>:/data/teamcity_server/datadir  \
       -v <path to log directory>:/opt/teamcity/logs   \
       -p <port on host>:8111 \
       jetbrains/teamcity-server
```

要运行maintainDB脚本（例如，用于服务器备份），请停止正在运行的容器并从主机执行以下命令：




```
docker run -it --name teamcity-server-instance  \
    -v <path to data directory>:/data/teamcity_server/datadir  \
    -v <path to log directory>:/opt/teamcity/logs  \
    -p <port on host>:8111 \
    jetbrains/teamcity-server \
    "/opt/teamcity/bin/maintainDB.sh" "backup"
```
确保所有本地系统路径与主服务器启动命令保持一致。

要更改TomCat容器内的TeamCity应用程序的上下文，请通过`-e TEAMCITY_CONTEXT=/context` 至 `docker run 默认的一个是ROOT，意味着服务器可以在http：// host /`

##### 升级TeamCity

确保检查通用的TeamCity[升级说明](https://confluence.jetbrains.com/display/TCDL/Upgrade)。
如果你没有改变容器，你可以停止正在运行的容器，通过通常的命令将新版本的图像和服务器拉入。
如果您更改了图像，则需要将更改复制到新的TeamCity服务器映像。一般来说，使用Docker的常识来执行升级。


##### 许可


该镜像可在[TeamCity许可](https://www.jetbrains.com/teamcity/buy/license.html)下获得。
TeamCity可免费使用100个构建配置（作业）和3个代理的限制。[许可细节](https://confluence.jetbrains.com/display/TCDL/Licensing+Policy)。


##### 反馈

向TeamCity官方[问题跟踪器](https://youtrack.jetbrains.com/issues/TW)报告建议问题。


##### 在Hood之下


此图片基于TeamCity基础图像构建，其中包括：


- ubuntu：xenial（Linux）
- microsoft / windowsservercore或microsoft / nanoserver（Windows）
- Oracle JRE 8 Update 161,64位
- curl，unzip（Linux）




##### 限制


由于Oracle许可证策略，该映像基于JRE 8 Update 161，并提供有限的Java工具集。如果您需要Java诊断工具（例如，为TeamCity进程进行内存转储或进行线程转储），请通过现有的Java安装将JDK 8 Update 161,64位安装到容器中。


##### 其他TeamCity镜像


- [Minimal Build Agent](https://hub.docker.com/r/jetbrains/teamcity-minimal-agent/)
- [Build Agent](https://hub.docker.com/r/jetbrains/teamcity-agent/)

##### Dockerfile源码


[https://github.com/JetBrains/teamcity-docker-server](https://github.com/JetBrains/teamcity-docker-server)










