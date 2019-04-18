# ***快速理解Docker - 容器级虚拟化解决方案 - wishchinYang的专栏 - CSDN博客
2014年12月28日 17:32:38[wishchin](https://me.csdn.net/wishchin)阅读数：667
原文链接：http://blog.csdn.net/colorant/article/details/20608157
[云计算](http://baike.baidu.com/view/1316082.htm)（cloudcomputing）是基于互联网的相关服务的增加、使用和交付模式，通常涉及通过互联网来提供动态易扩展且经常是虚拟化的资源。
**[云计算](http://baike.baidu.com/view/1316082.htm)**可以认为包括以下几个层次的服务：基础设施即服务（IaaS），[平台即服务](http://baike.baidu.com/view/1734078.htm)（PaaS）和[软件即服务](http://baike.baidu.com/view/1559138.htm)（SaaS）。
### IaaS：基础设施即服务
[IaaS](http://baike.baidu.com/view/2482595.htm)(*Infrastructure-as-a- Service*)：基础设施即服务。消费者通过*Internet*可以从完善的[计算机](http://baike.baidu.com/view/3314.htm)基础设施获得服务。例如：硬件服务器租用。
### PaaS：平台即服务
[PaaS](http://baike.baidu.com/view/1413359.htm)(*Platform-as-a- Service*)：平台即服务。*PaaS*实际上是指将[软件](http://baike.baidu.com/view/37.htm)研发的平台作为一种服务，以*SaaS*的模式提交给用户。因此，*PaaS*也是*SaaS*模式的一种应用。但是，*PaaS*的出现可以加快*SaaS*的发展，尤其是加快*SaaS*应用的开发速度。例如：软件的个性化定制开发。
### SaaS：软件即服务
[SaaS](http://baike.baidu.com/view/369107.htm)(*Software-as-a- Service*)：[软件即服务](http://baike.baidu.com/view/1559138.htm)。它是一种通过*Internet*提供[软件](http://baike.baidu.com/view/37.htm)的模式，用户无需购买软件，而是向提供商租用基于*Web*的软件，来管理企业经营活动。例如：阳光云服务器。
作者：刘旭晖 Raymond 转载请注明出处
Email：[colorant at 163.com](mailto:colorant@163.com)
BLOG：[http://blog.csdn.net/colorant/](http://blog.csdn.net/colorant/)
Docker是什么
        简单的说Docker是一个构建在LXC之上的,基于进程容器(Processcontainer)的轻量级VM解决方案.Docker近几年已经成为IAAS服务的事实标准，得到几乎所有顶级国家大厂和国内厂商的支持！
        拿现实世界中货物的运输作类比,为了解决各种型号规格尺寸的货物在各种运输工具上进行运输的问题,我们发明了集装箱
![](https://img-blog.csdn.net/20140306094606015)
Docker的初衷也就是将各种应用程序和他们所依赖的运行环境打包成标准的container/image,进而发布到不同的平台上运行
![](https://img-blog.csdn.net/20140306094705468)
从理论上说这一概念并不新鲜,各种虚拟机Image也起着类似的作用
Docker container和普通的虚拟机Image相比,最大的区别是它并不包含操作系统内核.
![](https://img-blog.csdn.net/20140306094828703)
普通虚拟机将整个操作系统运行在虚拟的硬件平台上,进而提供完整的运行环境供应用程序运行,而Docker则直接在宿主平台上加载运行应用程序.本质上他在底层使用LXC启动一个Linux
 Container,通过cgroup等机制对不同的container内运行的应用程序进行隔离,权限管理和quota分配等
每个container拥有自己独立的各种命名空间(亦即资源)包括:
PID进程,
 MNT文件系统, NET网络,
 IPC, UTS主机名等
**与****LXC****有什么不同**
基本上你可以认为目前的Docker是LXC的一个高级封装,提供了各种辅助工具和标准接口方便你使用LXC,你可以依靠LXC和各种脚本实现与docker类似的功能，就像你不使用APT/yum等工具也可以自己搞定软件包安装一样，你使用他们的关键原因是方便易用！
实际使用中，你一般不用关心底层LXC的细节,同时也不排将来docker实现基于非LXC方案的可能性
在LXC的基础上,
 Docker额外提供的Feature包括：标准统一的打包部署运行方案，历史版本控制，
 Image的重用，Image共享发布等等
**Container****构建方案**
除了LXC，Docker的核心思想就体现在它的运行容器构建方案上
![](https://img-blog.csdn.net/20140306094901265)
为了最大化重用Image，加快运行速度，减少内存和磁盘footprint,
 Docker container运行时所构造的运行环境，实际上是由具有依赖关系的多个Layer组成的。例如一个apache的运行环境可能是在基础的rootfs
 image的基础上，叠加了包含例如Emacs等各种工具的image，再叠加包含apache及其相关依赖library的image，这些image由AUFS文件系统加载合并到统一路径中，以只读的方式存在，最后再叠加加载一层可写的空白的Layer用作记录对当前运行环境所作的修改。
有了层级化的Image做基础，理想中，不同的APP就可以既可能的共用底层文件系统，相关依赖工具等，同一个APP的不同实例也可以实现共用绝大多数数据，进而以copy
 on write的形式维护自己的那一份修改过的数据等
历史和生态环境
Docker项目从启动到现在不过一年多时间，发展势头还是很迅猛的
2013.01做为dotcloud内部项目开始启动
2013.03.27正式作为public项目发布
2014.1被BLACK DUCK评选为2013年10大开源新项目“TOP
 10 OPEN SOURCE ROOKIE OF THE YEAR”
目前的状态 ( 2014.3 )
Docker 0.8.1
10000+ github stars(top 50)
350+ contributors
1500+ fork
![](https://img-blog.csdn.net/20140306094922218)
具体应用方面，可以看到百度至少在2013年10月份就已经成功使用Docker支持其BAE平台的Paas服务
安装运行和使用
Docker虽然是号称build once，
 runeverywhere。但是实际上还是受其引擎依赖关系的限制的，目前的版本具体来说对系统要求：
- Linux Kernel 3.8+
- LXCsupport
- 64bitOS
- AUFS
以上要求，以ubuntu为例，需要12.04配合
 3.8kernel升级，或者 ubuntu 13.04+
在ubuntu12.04上，基本安装步骤如下
sudoapt-get update sudo apt-get install linux-image-generic-lts-raringlinux-headers-generic-lts-raring
sudoapt-key adv --keyserver keyserver.ubuntu.com --recv-keys36A1D7869245C8950F966E92D8576A8BA88D21E9
sudosh -c "echo deb [http://get.docker.io/ubuntu](http://get.docker.io/ubuntu)docker main\ > /etc/apt/sources.list.d/docker.list"
sudoapt-get update 
sudoapt-get install lxc-docker
如果你在安装之前想要先体验一下docker的基本操作命令等的话，可以尝试一下这个在线的live教程[https://www.docker.io/gettingstarted/#h_tutorial](https://www.docker.io/gettingstarted/#h_tutorial)
常用命令
分类列一下常用的CLI命令
- 仓库相关
search/ pull / push / login etc.
例：docker pull ubuntu从仓库下载ubuntuimage
- Images操作相关
images/ rmi / build / export  / import / save /load etc.
例：docker images -t以树形结构列出当前本地Image
- 运行相关
run / start / stop / restart / attach /kill etc.
docker run -i -t ubuntu /bin/bash启动ubuntu
 image，并交互式的运行shell
- 杂项
Docker diff  / commit
Dockerinfo / ps / inspect / port / logs / top / history etc.
具体docker命令的使用参见[http://docs.docker.io/en/latest/reference/commandline/](http://docs.docker.io/en/latest/reference/commandline/)
常见问题
- 使用Non root用户
目前版本的docker由于使用Socket进行通讯，因此需要root用户权限
 sudo xxx，或者将需要使用Dockerclient的用户加入docker用户组
sudogpasswd -a ${USER} docker
- 网络相关问题
当你在网关背后需要通过代理连接docker的index数据库时，可以手动加上http_proxy环境变量来启动dockerdaemon
HTTP_PROXY=http://proxy_server:port
 docker -d &
更好的做法是修改/etc/default/docker ( on ubuntu ),添加exporthttp_proxy=proxy_server:port
同样，docker container如果无法自动正确的从host环境中获得DNS的配置，则需要手动指定DNS服务器地址，这可以通过
 docker -run --dns=xxx 来实现，也可以修改/etc/default/docker添加例如DOCKER_OPTS="-dns8.8.8.8"
- 特权模式
在正常情况下在container内部你没有权限操作device设备，而当前版本中，container内部部分文件例如/etc/hosts;/etc/hostname;
 /etc/resolve.conf等文件是动态通过mount动态以只读的形式加载上来的，理论上说你应该找到合适的方法去保证这些自动生成并加载的文件的正确性
 (例如通过--dns设置
 resolve.conf )，但是如果由于特殊原因你需要手动修改，那么你可以通过特权模式启动 docker client：
 docker run --privileged，然后你可以卸载这些文件，自己再创建新的版本
- 过多的层级依赖关系
以Layer的方式实现APP和相关library的cheap
 reuse和fast update是Docker的关键所在，不过受目前AUFS文件系统的限制，默认Layer的层级最多只能达到127（曾经只有42），在实际使用中有多种情况可能导致你的container的层级关系快速增长到这个极限值，撇开这么多layer叠加以后AUFS的效率不谈，更多情况下是你无法再更新构建你的image了
- 使用Dockerfile构建Image时，每条指令都会给最终的Image增加一层layer依赖关系.
- 以修改，提交，再修改再提交的方式不停的调整，更新你的Image
- 从仓库中下载的别人的Image已经包含众多的层级依赖关系，而你需要进一步更新以创建你自己的版本
前两者在一定程度上还是你自己可能把控的，最后一种情况就没办法了。这个问题最终必将影响Docker的实际可用性，目前的解决方案包括：
- 使用Dockerfile时,尽可能合并多个操作：例如使用
 "&&" 或 ";"合并运行多个shell命令；将多个shell命令写成脚本，在dockerfile中添加并运行这个脚本
- 通过Export再Import
 Image，丢弃所有历史信息和依赖关系，创建一个全新的image
将来可能的解决方案包括：
- 在Dockerfile中添加对多步操作的合并提交的支持
- 外部的image Flat工具的支持，目标是能够保留历史信息等
- 非AUFS的其它Storage解决方案
Future development
虽然Docker目前默认使用LXC和AUFS，但是Docker的核心思想本身，并不强制绑定这两者，0.8版本已经可以使用BTRFS，而整个Docker框架也改成了插件式的架构，便于添加替换各个功能模块
![](https://img-blog.csdn.net/20140306095014562)
例如更多的Storage方案的支持，规避AUFS当前的问题，除了LXC以外更多的虚拟化方案等
