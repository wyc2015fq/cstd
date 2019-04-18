# 资深专家都知道的Docker常用命令 - weixin_33985507的博客 - CSDN博客
2019年01月07日 08:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
开发人员一直在努力提高Docker的使用率和性能，命令也在不停变化。Docker命令经常被弃用，或被替换为更新且更有效的命令，本文总结了近年来资深专家最常用的命令列表并给出部分使用方法。
目前，Docker共有13个管理命令和41个通用命令，以下是常用Docker命令列表：
- docker help—检查最新Docker可用命令；
- docker attach—将本地输入、输出、错误流附加到正在运行的容器；
- docker commit—从当前更改的容器状态创建新镜像；
- docker exec—在活动或正在运行的容器中运行命令；
- docker history—显示镜像历史记录；
- docker info—显示系统范围信息；
- docker inspect—查找有关docker容器和镜像的系统级信息；
- docker login—登录到本地注册表或Docker Hub；
- docker pull—从本地注册表或Docker Hub中提取镜像或存储库；
- docker ps—列出容器的各种属性；
- docker restart—停止并启动容器；
- docker rm—移除容器；
- docker rmi—删除镜像；
- docker run—在隔离容器中运行命令；
- docker search—在Docker Hub中搜索镜像；
- docker start—启动已停止的容器；
- docker stop—停止运行容器；
- docker version—提供docker版本信息。
## 查找Docker版本和系统信息
无论是在本地还是云端，我们都需要检查Docker版本和系统信息，可以使用以下命令找到Docker版本：
`$ docker versionClient:Version: 18.03.0-ceAPI version: 1.37Go version: go1.9.4Git commit: 0520e24Built: Wed Mar 21 23:06:22 2018OS/Arch: darwin/amd64Experimental: falseOrchestrator: swarmServer:Engine:Version: 18.03.0-ceAPI version: 1.37 (minimum version 1.12)Go version: go1.9.4Git commit: 0520e24Built: Wed Mar 21 23:14:32 2018OS/Arch: linux/amd64Experimental: true`
另一个重要命令是docker info：
`$ docker infoContainers: 0Running: 0Paused: 0Stopped: 0Images: 0Server Version: 18.03.0-ceStorage Driver: overlay2Backing Filesystem: extfsSupports d_type: trueNative Overlay Diff: trueLogging Driver: json-fileCgroup Driver: cgroupfsPlugins:Volume: localNetwork: bridge host ipvlan macvlan null overlayLog: awslogs fluentd gcplogs gelf journald json-file logentries splunk syslog…`
该命令将显示部分设备信息，比如服务器版本、存储驱动程序、内核版本、操作系统、总内存等。当尝试为当前Docker安装启动新资源或尝试找出系统级资源分配问题时，这些信息非常有用，这也是检查运行、停止容器数量及下载镜像数量的快速方法。
## 搜索下载Docker镜像
可以使用docker search命令在Docker Hub搜索已有镜像：
`$ docker search ubuntuNAME DESCRIPTION STARS OFFICIAL AUTOMATEDubuntu Ubuntu is a Debian-based Linux operating sys… 7861 [OK]dorowu/ubuntu-desktop-lxde-vnc Ubuntu with openssh-server and NoVNC 190 [OK]rastasheep/ubuntu-sshd Dockerized SSH service, built on top of offi… 156 [OK]ansible/ubuntu14.04-ansible Ubuntu 14.04 LTS with ansible 93 [OK]ubuntu-upstart Upstart is an event-based replacement for th… 87 [OK]neurodebian NeuroDebian provides neuroscience research s… 50 [OK]…`
以上对ubuntu的搜索显示可用镜像、描述和官方状态等信息。如果要下载ubuntu镜像，可以使用docker pull命令：
`$ docker pull ubuntuUsing default tag: latestlatest: Pulling from library/ubuntu6b98dfc16071: Pull complete4001a1209541: Pull complete6319fc68c576: Pull completeb24603670dc3: Pull complete97f170c87c6f: Pull completeDigest: sha256:5f4bdc3467537cbbe563e80db2c3ec95d548a9145d64453b06939c4592d67b6dStatus: Downloaded newer image for ubuntu:latest$`
## Docker镜像
使用docker info命令查找拥有镜像数量：
`$ docker infoContainers: 0Running: 0Paused: 0Stopped: 0Images: 1…`
docker images命令可详细列出拥有镜像：
`$ docker imagesREPOSITORY          TAG                 IMAGE ID            CREATED             SIZEubuntu              latest              113a43faa138        2 weeks ago         81.2MB`
假设下载NGINX镜像，可以运行docker pull命令：
`$ docker pull nginx`
如果检查Docker镜像，会看到如下内容：
`$ docker imagesREPOSITORY          TAG                 IMAGE ID            CREATED             SIZEubuntu              latest              113a43faa138        2 weeks ago         81.2MBnginx               latest              cd5239a0906a        2 weeks ago         109MB`
如果寻找Ubuntu 16.04版本，可使用16.04标签下载特定版本：
`$ docker pull ubuntu:16.04`
随后，将出现两个Ubuntu映像版本：
`$ docker imagesREPOSITORY          TAG                 IMAGE ID            CREATED             SIZEubuntu              16.04               5e8b97a2a082        2 weeks ago         114MBubuntu              latest              113a43faa138        2 weeks ago         81.2MBnginx               latest              cd5239a0906a        2 weeks ago         109MB`
注意：无需注册Docker Hub即可提取镜像。如果想将镜像推送到Docker Hub，需要注册然后使用docker login命令登录：
`$ docker login --username=yourhubusername --email=youremail@company.com`
## 运行镜像的Docker容器
假设想在docker上运行NGINX服务器，可运行以下命令：
`$ docker run -p 8080:80 nginx`
如果已用run命令从Docker Hub提取nginx映像创建NGINX容器。 -p 8080:80会告诉Docker将localhost端口8080映射到Docker容器端口80，应该能够从http:// localhost:8080访问NGINX服务器。
NGINX容器已附加到命令行。如果退出命令行，容器将会停止，可使用detach（’-  d）启动NGINX容器，这样即使退出命令行也可继续运行。
`$ docker run -p 8080:80 -d nginx`
以上命令将以分离模式启动容器并返回命令行。
## 使用docker ps命令列出Docker容器
docker ps命令允许查找正在运行的所有容器：
`$ docker psCONTAINER ID         IMAGE         COMMAND         CREATED           STATUS          PORTS             NAMES6162daed25da         nginx  \u0026quot;nginx -g 'daemon of…\u0026quot; 7 minutes ago   Up 7 minutes  0.0.0.0:8080-\u0026gt;80/tcp  hardcore_torvalds`
如上显示容器的各种属性，可以看到是从nginx镜像创建并显示端口转发信息。CONTAINER ID和NAMES属性需要特别提及，可以使用上述两大属性唯一标识容器。二者都可自动生成，但也可在容器创建过程中为容器命名。
创建一个名为my_nginx的NGINX容器：
`$ docker run --name my_nginx -p 8888:80 -d nginx`
再次列出所有Docker容器：
`$ docker psCONTAINER ID        IMAGE               COMMAND                  CREATED              STATUS              PORTS                  NAMESe7b19b6ad778        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   About a minute ago   Up About a minute   0.0.0.0:8888-\u0026gt;80/tcp   my_nginx6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   15 minutes ago       Up 16 minutes       0.0.0.0:8080-\u0026gt;80/tcp   hardcore_torvalds`
请注意，新容器的名称为“my_nginx”。 处理大量容器时，可以使用命名约定，这可以更好得组织容器。
docker ps命令仅显示正在运行的容器。如果对上述情况使用docker info命令：
`$ docker infoContainers: 2Running: 2Paused: 0Stopped: 0Images: 3`
可以看到有两个容器正在运行，如果一个暂停或已停止，则不会仅使用docker ps命令查看这些容器，必须使用all（ -  a）选项：
`$ docker ps -a`
## 启动，停止，重启和杀死容器
假设要停止’my_nginx’容器，可使用CONTAINER ID或NAME。
`$ docker stop my_nginxmy_nginx`
列出Docker容器：
`$ docker psCONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS                  NAMES6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   27 minutes ago      Up 27 minutes       0.0.0.0:8080-\u0026gt;80/tcp   hardcore_torvalds$ docker ps -aCONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS                          PORTS                  NAMESe7b19b6ad778        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   12 minutes ago      Exited (0) About a minute ago                          my_nginx6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   27 minutes ago      Up 27 minutes                   0.0.0.0:8080-\u0026gt;80/tcp   hardcore_torvalds`
如果在没有-a选项的情况下运行docker ps，只显示正在运行的容器。在第二种情况下，可以看到’my_nginx’容器处于退出状态。
让我们docker start  容器：
`$ docker start my_nginxmy_nginx`
检查Docker容器列表：
`$ docker psCONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS                  NAMESe7b19b6ad778        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   16 minutes ago      Up 29 seconds       0.0.0.0:8888-\u0026gt;80/tcp   my_nginx6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   30 minutes ago      Up 30 minutes       0.0.0.0:8080-\u0026gt;80/tcp   hardcore_torvalds`
可以看到STATUS显示容器’my_nginx’再次被启动。
如果由于某些问题而需重启容器，则可使用restart命令，这比单独停止和启动容器速度更快：
`$ docker restart my_nginx`
可以像进程一样终止docker容器，比如终止my_nginx容器：
`$ docker kill my_nginx`
再次列出Docker容器：
`$ docker ps -aCONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS                       PORTS                  NAMESe7b19b6ad778        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   22 minutes ago      Exited (137) 7 seconds ago                          my_nginx6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   36 minutes ago      Up 36 minutes                0.0.0.0:8080-\u0026gt;80/tcp   hardcore_torvalds`
容器my_nginx未运行。此外，可在信息中看到一个正在运行的容器和一个已停止容器：
`$ docker infoContainers: 2Running: 1Paused: 0Stopped: 1Images: 3…`
## Docker Exec Bash和Docker SSH
如果需要与shell进行交互以创建服务或解决问题，可以使用docker exec命令创建交互式shell。比如，用bash shell从ubuntu映像启动一个容器：
`$ docker run --name my_ubuntu -it ubuntu:latest bashroot@a19c770b8621:/#`
root @haracle＃表示位于Docker容器的bash shell中，可以运行shell命令：
`root@a19c770b8621:/# lsbin boot dev etc home lib lib64 media mnt opt proc root run sbin srv sys tmp usr varroot@a19c770b8621:/# pwd/`
列出Docker容器：
`$ docker psCONTAINER ID        IMAGE               COMMAND                  CREATED              STATUS              PORTS                  NAMESa19c770b8621        ubuntu:latest       \u0026quot;bash\u0026quot;                   About a minute ago   Up About a minute                          my_ubuntu6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   About an hour ago    Up About an hour    0.0.0.0:8080-\u0026gt;80/tcp   hardcore_torvalds`
如上，my_ubuntu正在运行。假设想将Docker ssh放入’my_ubuntu’容器中，可以使用docker exec bash：
`$ docker exec -it my_ubuntu bashroot@a19c770b8621:/#`
使用docker exec向容器发出命令。例如，可以直接用命令提示符在my_ubuntu容器上运行ls命令：
`$ docker exec -it my_ubuntu lsbin dev home lib64 mnt proc run srv tmp varboot etc lib media opt root sbin sys usr`
## 以分离模式启动容器并使用docker attach
以上示例用附加模式启动ubuntu容器，我们也可在分离模式下启动：
`$ docker run -it -d --name my_ubuntu_2 ubuntu:latest bash75b28b7208359137b3e1dc2843387918e28b4c6c4860ef0cdeac79c205f5cbc4`
验证容器是否正在运行：
`$ docker psCONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS                  NAMES75b28b720835        ubuntu:latest       \u0026quot;bash\u0026quot;                   3 minutes ago       Up 3 minutes                               my_ubuntu_2a19c770b8621        ubuntu:latest       \u0026quot;bash\u0026quot;                   15 minutes ago      Up 15 minutes                              my_ubuntu6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   2 hours ago         Up 2 hours          0.0.0.0:8080-\u0026gt;80/tcp   hardcore_torvalds`
使用docker attach命令获取docker exec bash-like效果：
`$ docker attach my_ubuntu_2root@75b28b720835:/#`
## 检查Docker镜像历史
Docker社区镜像是分层创建的，可使用Docker history命令查看镜像创建方式。先看看有哪些镜像：
`$ docker imagesREPOSITORY          TAG                 IMAGE ID            CREATED             SIZEubuntu              16.04               5e8b97a2a082        2 weeks ago         114MBubuntu              latest              113a43faa138        2 weeks ago         81.2MBnginx               latest              cd5239a0906a        2 weeks ago         109MB`
查看nginx镜像历史：
`$ docker history nginxIMAGE               CREATED             CREATED BY                                      SIZE                COMMENTcd5239a0906a        2 weeks ago         /bin/sh -c #(nop)  CMD [\u0026quot;nginx\u0026quot; \u0026quot;-g\u0026quot; \u0026quot;daemon…   0B                  \u0026lt;missing\u0026gt;           2 weeks ago         /bin/sh -c #(nop)  STOPSIGNAL [SIGTERM]         0B                  \u0026lt;missing\u0026gt;           2 weeks ago         /bin/sh -c #(nop)  EXPOSE 80/tcp                0B                  \u0026lt;missing\u0026gt;           2 weeks ago         /bin/sh -c ln -sf /dev/stdout /var/log/nginx…   22B                 \u0026lt;missing\u0026gt;           2 weeks ago         /bin/sh -c set -x  \u0026amp;\u0026amp; apt-get update  \u0026amp;\u0026amp; apt…   53.7MB              \u0026lt;missing\u0026gt;           2 weeks ago         /bin/sh -c #(nop)  ENV NJS_VERSION=1.15.0.0.…   0B                  \u0026lt;missing\u0026gt;           2 weeks ago         /bin/sh -c #(nop)  ENV NGINX_VERSION=1.15.0-…   0B                  \u0026lt;missing\u0026gt;           7 weeks ago         /bin/sh -c #(nop)  LABEL maintainer=NGINX Do…   0B                  \u0026lt;missing\u0026gt;           8 weeks ago         /bin/sh -c #(nop)  CMD [\u0026quot;bash\u0026quot;]                 0B                  \u0026lt;missing\u0026gt;           8 weeks ago         /bin/sh -c #(nop) ADD file:ec5be7eec56a74975…   55.3MB`
使用镜像历史命令查找最近更改，如果在新版本镜像中发现问题，则此命令可帮助找到原因，也可用以下命令：
`$ docker image history nginx`
## Docker检查容器
可以使用docker inspect命令查找有关系统的信息，运行docker ps命令列出Docker容器：
`$ docker psCONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS                  NAMES6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   2 hours ago         Up 2 hours          0.0.0.0:8080-\u0026gt;80/tcp   hardcore_torvalds`
使用CONTAINER ID检查容器（也可使用容器名称）：
`$ docker inspect 6162daed25da[{\u0026quot;Id\u0026quot;: \u0026quot;6162daed25da50b98afca5f7ed8caca2289bf309b2547d87ae8674b899d604a4\u0026quot;,\u0026quot;Created\u0026quot;: \u0026quot;2018-06-25T05:46:37.902211302Z\u0026quot;,\u0026quot;Path\u0026quot;: \u0026quot;nginx\u0026quot;,\u0026quot;Args\u0026quot;: [………\u0026quot;DriverOpts\u0026quot;: null}}}}]`
该命令将以JSON格式提供大量信息，以下是查找容器IP地址的技巧：
`$ docker inspect 6162daed25da | grep \u0026quot;IPAddress\u0026quot;            \u0026quot;SecondaryIPAddresses\u0026quot;: null,            \u0026quot;IPAddress\u0026quot;: \u0026quot;172.17.0.2\u0026quot;,                    \u0026quot;IPAddress\u0026quot;: \u0026quot;172.17.0.2\u0026quot;,`
## 使用docker cp将文件从本地复制到容器
再次列出Docker容器：
`$ docker psCONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS              PORTS                  NAMES6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   3 hours ago         Up 3 hours          0.0.0.0:8080-\u0026gt;80/tcp   hardcore_torvalds`
NGINX容器在端口8080上运行。因此，如果转到http：// localhost：8080，将看到以下内容：
`\u0026quot;Welcome to nginx!\u0026quot;`
如果看到此页面，则nginx Web服务器已成功安装并正常运行，需要进一步配置。
首先，在本地目录中创建index.html：
`\u0026lt;html\u0026gt;\u0026lt;Header\u0026gt;\u0026lt;title\u0026gt;My Website\u0026lt;/title\u0026gt;\u0026lt;/header\u0026gt;\u0026lt;body\u0026gt;Hello world\u0026lt;/body\u0026gt;\u0026lt;/html\u0026gt;`
使用带有ls的docker exec命令检查NGINX容器中具有index.html的文件夹：
`$ docker exec -it hardcore_torvalds ls /usr/share/nginx/html50x.html index.html`
使用创建的本地文件覆盖容器index.html文件：
`$ docker cp index.html hardcore_torvalds:usr/share/nginx/html/`
如果再次检查[http://localhost:](%5C)8080，应该可以看到问候语“Hello world”。
可使用docker cp命令在本地计算机和创建容器间移动文件，此方法可用于覆盖配置文件。
## 创建Docker镜像
如果想复制更多“Hello World”容器，必须将当前容器保存为镜像。
首先，停止容器：
`$ docker stop hardcore_torvalds`
列出所有Docker容器：
`$ docker ps -aCONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS                         PORTS               NAMES75b28b720835        ubuntu:latest       \u0026quot;bash\u0026quot;                   About an hour ago   Exited (0) About an hour ago                       my_ubuntu_2a19c770b8621        ubuntu:latest       \u0026quot;bash\u0026quot;                   2 hours ago         Exited (0) About an hour ago                       my_ubuntu6162daed25da        nginx               \u0026quot;nginx -g 'daemon of…\u0026quot;   3 hours ago         Exited (0) 27 seconds ago                          hardcore_torvalds`
从STATUS中，可以看到NGINX’corecore_torvalds’容器已停止。使用docker commit命令创建新镜像：
`$ docker commit 6162daed25da nginx_hello_world_templatesha256:117d060587a316035ed5a776e613d9cfbeee9fbfe202c6edc9203820c7da987b`
如果现在检查，将看到新镜像：
`$ docker imagesREPOSITORY                   TAG                 IMAGE ID            CREATED             SIZEnginx_hello_world_template   latest              117d060587a3        40 seconds ago      109MBubuntu                       16.04               5e8b97a2a082        2 weeks ago         114MBubuntu                       latest              113a43faa138        2 weeks ago         81.2MBnginx                        latest              cd5239a0906a        2 weeks ago         109MB`
可以使用此镜像并启动新Docker容器，新创建容器将具有“Hello world”页面，而不是“Welcome NGINX”页面。示例如下：
`$ docker run -it -d -p 8886:80 nginx_hello_world_template4e042d76c39125471951626ba42cd609a65c73f041943298f74f4fc43dc5596a$`
## 删除Docker容器和镜像
Docker容器和镜像会占用硬盘空间，因此最好定期清理。首先停止所有容器，然后列出所有容器，使用以下Docker命令执行操作：
`$ docker ps -aCONTAINER ID        IMAGE                        COMMAND                  CREATED             STATUS                         PORTS               NAMES4e042d76c391        nginx_hello_world_template   \u0026quot;nginx -g 'daemon of…\u0026quot;   2 minutes ago       Exited (0) 8 seconds ago                           boring_keldysh75b28b720835        ubuntu:latest                \u0026quot;bash\u0026quot;                   2 hours ago         Exited (0) About an hour ago                       my_ubuntu_2a19c770b8621        ubuntu:latest                \u0026quot;bash\u0026quot;                   2 hours ago         Exited (0) About an hour ago                       my_ubuntu6162daed25da        nginx                        \u0026quot;nginx -g 'daemon of…\u0026quot;   3 hours ago         Exited (0) 11 minutes ago                          hardcore_torvalds`
如上，有4个处于停止状态的容器，可使用docker rm命令删除容器：
`$ docker rm 4e042d76c391 75b28b720835 a19c770b8621 6162daed25da4e042d76c39175b28b720835a19c770b86216162daed25da`
可使用NAMES代替CONTAINER ID，容器列表现在应该是干净的：
`$ docker ps -aCONTAINER ID IMAGE COMMAND CREATED STATUS PORTS NAMES`
列出Docker镜像：
`$ docker imagesREPOSITORY                   TAG                 IMAGE ID            CREATED             SIZEnginx_hello_world_template   latest              117d060587a3        11 minutes ago      109MBubuntu                       16.04               5e8b97a2a082        2 weeks ago         114MBubuntu                       latest              113a43faa138        2 weeks ago         81.2MBnginx                        latest              cd5239a0906a        2 weeks ago         109MB`
可使用docker rmi命令和IMAGE ID删除docker镜像：
`$ docker rmi 117d060587a3 5e8b97a2a082 113a43faa138 cd5239a0906a`
最后，Docker镜像列表应该是干净的：
`$ docker imagesREPOSITORY TAG IMAGE ID CREATED SIZE`
原文链接：[https://dzone.com/articles/top-docker-commands-itsyndicate](%5C)
