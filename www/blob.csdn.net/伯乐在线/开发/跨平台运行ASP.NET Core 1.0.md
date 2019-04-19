# 跨平台运行ASP.NET Core 1.0 - 文章 - 伯乐在线
原文出处： [B.it](http://www.cnblogs.com/ImBit/p/5375577.html#3412609)
**前言**
首先提一下微软更名后的叫法：
- ASP.NET 5 更名为 ASP.NET Core 1.0
- .NET Core 更名为 .NET Core 1.0
- Entity Framework 7 更名为 Entity Framework Core 1.0 或者简称 EF Core 1.0
现在伴随着ASP.NET Core 1.0 RC2版的更新速度，许多官方文档都跟不上，还停留在RC1版的使用方式上（RC1版是继Beta版之后第一个发布的稳定版本）。RC1和RC2的区别在于RC1版使用dnvm、dnx来restore packages和run(dnu是包含在dnx中的)，而RC2版会基于.NET Core CLI，使用dotnet命令来进行building,compiling, NuGet package management, running, testing。
如果想了解更多ASP.NET Core 1.0 的发布进度和里程碑可以查看：[ASP.NET Core 1.0 Schedule and Roadmap](https://github.com/aspnet/home/wiki/roadmap)。就目前而言，在某些系统中我们已经可以遗弃mono，只使用.NET Core 和.NET Core CLI 就可以提供对.Net程序进行生成编译运行环境（同时包含.NET Core 和.NET Core CLI的.NET Core SDK二进制包，可以在[这里](https://github.com/dotnet/cli#installers-and-binaries)下载）。
至于mono的未来，微软的SocttHunter和Scott Hasnselman说到：切换到MIT开源许可协议的mono将作为公司未来构建一个统一.NET库的计划中的一部分，[了解详情](http://www.infoq.com/cn/news/2016/04/NET-Mono-Together)。可见开源的mono会逐步融入.NET Core体系中。
-  .NET Core SDK=[.Net Core](https://github.com/dotnet/core)+[.NET Core command-line](https://github.com/dotnet/cli)(简称CLI.NET CLI.NET Core CLI，dotnet commands)
-  .NET Core=[基础类库](https://github.com/dotnet/corefx)[.NET Core Framework(CoreFX)](https://github.com/dotnet/corefx)+[运行时.NET Core Runtime(CoreCLR)](https://github.com/dotnet/coreclr)+[编译器管理（如管理编译器Roslyn)](https://github.com/dotnet/roslyn)+[More](https://github.com/dotnet/core#net-core-platform)
本文章主要介绍ASP.NET Core 1.0  RC2版的跨平台部署，（【部署环境】作为第一步，有助于理解程序的运行环境和机理）。如果想了解跨平台部署RC1版，请查看
- [Installing ASP.NET 5 On Cross-Platform](http://docs.asp.net/en/latest/getting-started/index.html)
- [Running ASP.NET 5 applications in Linux Containers with Docker](https://blogs.msdn.microsoft.com/webdev/2015/01/14/running-asp-net-5-applications-in-linux-containers-with-docker/)
- [.NET Core + ASP.NET Core 1.0 RC1 bits](https://get.asp.net/)
下面会分别介绍在Dokcer、CentOS、Ubuntu环境中的部署方式，Windows比较简单，就不作介绍。运行的ASP.NET Core程序以[cli-samples](https://github.com/aspnet/cli-samples)的HelloMvc为例。
**一、在Docker中运行**
1、环境
这里演示的Docker宿主机环境是CentOS7,下面介绍Docker在CentOS7上的安装。在其他系统中安装Docker，请查看[Install Docker Engine](https://docs.docker.com/engine/installation/)
提示：CentOS在7中已经启用firewalld作为新的防火墙，替换掉原先的iptables。而Docker对firewalld的兼容性虽然不太好，但对下面的例子运行中暂时没发现有影响。
![731719-20160415160208910-2081263879](http://jbcdn2.b0.upaiyun.com/2016/05/40d683597203b1ba39c63a51897745471.png)
2、安装docker
首先查看CentOS的内核版本（CentOS7系统必须为64位，内核必须3.10及以上）

Shell
```
uname -r
3.10.0-229.el7.x86_64
```
更新已经安装的包


Shell
```
sudo yum update
```
安装docker

Shell
```
curl -fsSL https://get.docker.com/ | sh
```
启动docker


Shell
```
sudo service docker start 或 systemctl start docker
```
运行docker hello world demo


Shell
```
sudo docker run hello-world
```
3、创建docker用户组
此用户组具备管理员权限,以后执行docker命令可以不加sudo（这里只做演示，直接使用root）
(1) 以带有管理员权限的用户登入控制台
`(2) sudo usermod -aG docker `your_username(你的用户名)
(3) 登出再登入控制台
(4) 不带sudo执行docker run hello-world，看是否正确输出Hello world
(5) 设置开机启动sudo chkconfig docker on 或 systemctl enable docker
4、 docker相关概念
container：容器，用来加载各种image,可以看成操作系统。加载各种app。允许多个容器同时运行，并且容器一般无法相互访问宿主服务器资源，除非挂载数据卷volume之类的,数据卷的使用可以参照：[http://my.oschina.net/guol/blog/271225](http://my.oschina.net/guol/blog/271225)）。容器中可以运行一些网络应用，要让外部也可以访问这些应用，可以通过 `-P` 或 `-p` 参数来指定端口映射。
image：镜像文件，可以当成app运行在container中，image有一个Dockerfile文件，保存折生成自身image时要执行的命令。


Shell
```
docker images //显示所有镜像文件
docker ps -q -a //显示所有容器
docker run -it --name dn_container -d -p 5000:5000  microsoft/dotnet:latest //新增一个容器用于加载 microsoft/dotnet:latest镜像文件，-p host port:container port
docker run -it --name dn_container -d -p 127.0.0.1:5000:5000  microsoft/dotnet:latest
docker start dn_container //启动容器
docker attach dn_container //关联并进入启动容器，需要先启动容器
docker stop dn_container //停止容器
docker kill $(docker ps -a -q) //强制停止所有容器
docker rm $(docker ps -a -q) //删除所有容器
doker commit ContainerID NewImageName //将容器里的内容提交为新的镜像文件
docker rmi -f 7d9495d03763 //删除所有容器
//标记镜像,push到Docker Hub
docker images
docker tag a66b7258f574 sobit17/sobit-dotnet:1.0.0-beta-002252
docker images
docker login --username=test --email=test@qq.com
docker push sobit17/sobit-dotnet
```
5、制作HelloMvc image
在运行[cli-samples](https://github.com/aspnet/cli-samples)的HelloMvc例子前，需要把HelloMvc打包成docker image文件。
5.1 首先安装git,克隆cli-samples下来

Shell
```
mkdir app
cd app
git clone https://github.com/aspnet/cli-samples.git
cd cli-samples
```
需要显式指定HelloMvc的端口（即使指定的是5000端口），目前尝试过不加下面这段代码，发现默认的5000并不起作用
**![](http://jbcdn2.b0.upaiyun.com/2016/05/51cce97fe4a1b147a8589a3a3cdfa86b.png)**
5.2 在HelloMvc的当前目录执行,并备好NuGet.config和Dockerfile文件
NuGet.config：读取package下载的源地址（nuget seeds）；
Dockerfile：制作image必须；
***5.2.1创建***NuGet.config文件
可以从上级目录cli-samples中拷贝一个NuGet.config到HelloMvc目录中，如果没有这个NuGet.config 在还原package时会默认到.nuget/NuGet/加载NuGet.config。因为源不对，会导致某些packeage下载不到。

Shell
```
//拷贝命令
cp -p  /root/app/cli-samples/NuGet.Config  /root/app/cli-samples/HelloMvc/NuGet.Config
```
![](http://jbcdn2.b0.upaiyun.com/2016/05/088486b4bf3d792e01d2977023ae8dd0.png)
5.2.2创建Dockerfile文件
在创建image时，[.NET CLI Preview Docker Image](https://hub.docker.com/r/microsoft/dotnet/)提供了两个基础dotnet image用于继承（里面装载的其实是部署了.NET Core的Ubuntu系统），区别在于它们的Dockerfile不同
- dotnet:0.0.1-alpha
- dotnet**:**0.0.1-alpha-onbuild
dotnet:0.0.1-alpha的Dockerfile，基于buildpack-deps:trusty-scm
![](http://jbcdn2.b0.upaiyun.com/2016/05/06cbc023ede2aa27271513fcfbf1dcca.png)
 dotnet:0.0.1-alpha-onbuild基于dotnet:0.0.1-alpha，（ONBUILD  ONBUILD指定的命令在构建镜像时并不执行,而是在它的子镜像中执行）
![](http://jbcdn2.b0.upaiyun.com/2016/05/e8659bb34ef5697ec37a0d39022aceb4.png)
所以HelloMvc的Dockerfile可以有两种写法，各选其一
(1) 基于dotnet:0.0.1-alpha

C#
```
FROM microsoft/dotnet:latest
RUN mkdir -p /webapp
COPY . /webapp
WORKDIR /webapp
RUN ["dotnet", "restore"]
RUN sudo apt-get -y update \
 && sudo apt-get install -y dialog make automake libtool curl 
 RUN curl -sSL https://github.com/libuv/libuv/archive/v1.8.0.tar.gz | sudo tar zxfv - -C /usr/local/src \
 && cd /usr/local/src/libuv-1.8.0 \
 && sudo sh autogen.sh \
 && sudo ./configure \
 && sudo make \
 && sudo make install \
 && sudo rm -rf /usr/local/src/libuv-1.8.0 \
 && cd ~/ \
 && sudo ldconfig
 
 EXPOSE 5600
 ENTRYPOINT ["dotnet", "run"]
```
(2)基于dotnet:0.0.1-alpha-onbuild

C#
```
FROM microsoft/dotnet:0.0.1-alpha-onbuild
 
RUN sudo apt-get -y update \
 && sudo apt-get install -y dialog make automake libtool curl 
RUN curl -sSL https://github.com/libuv/libuv/archive/v1.8.0.tar.gz | sudo tar zxfv - -C /usr/local/src \
 && cd /usr/local/src/libuv-1.8.0 \
 && sudo sh autogen.sh \
 && sudo ./configure \
 && sudo make \
 && sudo make install \
 && sudo rm -rf /usr/local/src/libuv-1.8.0 \
 && cd ~/ \
 && sudo ldconfig
 
 EXPOSE 5600
```
5.2.3 检测Dockerfile是否能正常使用
由于.NET Core时刻在更新，所以导致官网[NET CLI Preview Docker image](https://hub.docker.com/r/microsoft/dotnet/) 提供的dotnet image更新不及时而在执行dotnet restore时候会报错：unknown keyword platform（主要是dotnet image内部的
.NET Core和CLI版本过旧，且存在bug），会导致基于它的咱们的Dockefile执行失败。 如果不确定你当前的dotnet image是否有上述问题，下面会介绍一种检测方法和问题的解决方案
（1）检测方法
执行下面命令加载dotnet image，且一般会自动启动test_container并进入到image内部环境中，如果你服务器上没有dotnet image,下面命令也会自动下载
docker run –name test_container -it microsoft/dotnet:latest
如果test_container未启动，则执行下面命令启动并关联进入
docker start test_container
dokcer attach test_container
进入到image内部环境中

Shell
```
mkdir test
cd test
it clone https://github.com/aspnet/cli-samples.git
cd cli-samples
dotnet restore
```
查看是否报错
清除测试文件

Shell
```
cd ~rm -r test
```
（2）解决方案
手工制作一个使用最新.NET Core SDK的dotnet image,也可以使用我已经做好的image   sobit17/sobit-dotnet:1.0.0-beta-002252，当然如果没报错可以省略这一步，用回原来的dotnet image.
制作新的image这里选用dotnet:0.0.1-alpha为原型，也可以使用dotnet**:**0.0.1-alpha-onbuild
加载dotnet image

Shell
```
docker run --name dotnet_container -it microsoft/dotnet:0.0.1-alpha
```
如果dotnet_container 未启动，则执行下面命令启动并关联进入

Shell
```
docker start dotnet_container 
dokcer attach dotnet_container
```
查看.NET Core版本（dotnet image中使用的是Ubuntu14.04系统）

Shell
```
dotnet --version
或
dotnet --info
```
由于对应Ubuntu的dotnet package最高只到1.0.0-beta-00.1793，但这个版本也有上述bug。（查找可以使用apt-cache search XX|grep XX(XX为你知道的一部份软件名)，也可以从这里查看[http://apt-mo.trafficmanager.net/repos/dotnet/pool/main/d/dotnet/](http://apt-mo.trafficmanager.net/repos/dotnet/pool/main/d/dotnet/)）
所以不能利用
apt-get upgrade  dotnet
或
apt-get remove dotnet  = 1.0.0.001598-1
apt-get install dotnet=1.0.0.002252-1（Ubuntu没这个最新版本）
只能从github下载二进制文件,安装到最新版本（最新版本可以从github上面查到:[https://github.com/dotnet/cli#installers-and-binaries](https://github.com/dotnet/cli#installers-and-binaries)）


Shell
```
apt-get update
wget https://dotnetcli.blob.core.windows.net/dotnet/beta/Binaries/Latest/dotnet-dev-ubuntu-x64.latest.tar.gz
tar -zxf dotnet-dev-ubuntu-x64.latest.tar.gz -C /usr/bin
```
或


Shell
```
apt-get update
wget https://dotnetcli.blob.core.windows.net/dotnet/beta/Binaries/Latest/dotnet-dev-ubuntu-x64.latest.tar.gz
tar -zxf dotnet-dev-ubuntu-x64.latest.tar.gz
ln -s /dn_new/dotnet /usr/bin/dotnet
```
接下来执行下面命令


Shell
```
dotnet --info //查看版本
exit //退出容器
docker images //查找dotnet_container 的ConrainerID
docker commit 3a09b2588478（ContainerID） dotnet_new（new image name） //commit成一个新的image
```
至此base image制作完毕。
接下来只要修改HelloMvc的Dockerfile以指定这个新的image作为基础镜像

Shell
```
FROM dotnet_new 或 sobit17/sobit-dotnet:1.0.0-beta-002252
RUN mkdir -p /webapp
COPY . /webapp
WORKDIR /webapp
RUN ["dotnet", "restore"]
##备注：安装libuv这一过程其实可以在制作dotnet_new镜像时也安装进去，就不需要下面这段【注释在Dockerfile中需删掉】
RUN sudo apt-get -y update \
 && sudo apt-get install -y dialog make automake libtool curl 
RUN curl -sSL https://github.com/libuv/libuv/archive/v1.8.0.tar.gz | sudo tar zxfv - -C /usr/local/src \
 && cd /usr/local/src/libuv-1.8.0 \
 && sudo sh autogen.sh \
 && sudo ./configure \
 && sudo make \
 && sudo make install \
 && sudo rm -rf /usr/local/src/libuv-1.8.0 \
 && cd ~/ \
 && sudo ldconfig
EXPOSE 5600
ENTRYPOINT ["dotnet", "run"]
```
所有把HelloMvc制作成image的资料已经准备完毕。
5.2.4 制作 HelloMvc image,当前目录移动到HelloMvc中,执行下面命令（制作过程中可能会因为网络问题报错，可以重复执行下面这个命令直到成功就可以了）

Shell
```
docker build -t hellomvc-image . //记得命令最后有'.'
```
6、编译运行HelloMvc
- 编译试运行：docker run –rm -w /webapp hellomvc-image
- 运行：docker run -t –name web_container -d -p 5100:5600  hellomvc-image
打开浏览器 [http://IP:5100](http://ip:5100/) （防火墙和查看网络相关命令请查看本篇文章第四点）
![](http://jbcdn2.b0.upaiyun.com/2016/05/ade0ed0aa3dd09cb6f54fda185e54844.png)
**二、在CentOS 7中运行**
1、环境
这里演示的环境是CentOS7,64bit
2、安装.NET Core SDK

Shell
```
sudo yum updatemkdir dn_cli
cd dn_cli
wget https://dotnetcli.blob.core.windows.net/dotnet/beta/Binaries/Latest/dotnet-dev-centos-x64.latest.tar.gz
tar -zxf dotnet-dev-centos-x64.latest.tar.gz
```
尝试执行命令：. /dotnet
提示错误：
Failed to load /root/dotnet/bin/libcoreclr.so, error: libunwind.so.8: cannot open shared object file: No such file or directory
解决方法：
yum install -y libunwind
尝试执行命令：. /dotnet
提示错误：
Failed to initialize CoreCLR, HRESULT: 0x80131500
解决方法：
yum install -y icu
尝试执行命令：. /dotnet –info，正常。
永久保存环境变量
找到/etc/profile文件(当用户第一次登录时,该文件被执行.)，在最后面添加：
PATH=~/dn_cli:$PATH
export PATH
即时生效
source /etc/profile
3、下载cli-samples的HelloMvc例子


Shell
```
cd ~sudo yum -y install git
mkdir app
cd app
git clone https://github.com/aspnet/cli-samples.git
cd cli-samples
dotnet restore
cd HelloMvc
```
需要显式指定HelloMvc的端口（即使指定的是5000端口），目前尝试过不加下面这段代码，发现默认的5000并不起作用
![731719-20160415155016629-1056578589](http://jbcdn2.b0.upaiyun.com/2016/05/25822f3d653833ac0f99417286f1e63b.png)
4、编译运行HelloMvc
将执行命令的当前目录定位到HelloMvc，并执行下面命令

Shell
```
dotnet run
```
打开浏览器 http://IP:5600（防火墙和查看网络相关命令请查看本篇文章第四点）
![731719-20160415154836879-1536445895](http://jbcdn2.b0.upaiyun.com/2016/05/94665c395f45e7e14b97efa23f297e54.png)
**三、在Ubuntu 14.04中运行**
1、安装Ubuntu Installers


Shell
```
apt-get updatemkdir dn_cli
cd dn_cli
wget https://dotnetcli.blob.core.windows.net/dotnet/beta/Installers/Latest/dotnet-host-ubuntu-x64.latest.deb
wget https://dotnetcli.blob.core.windows.net/dotnet/beta/Installers/Latest/dotnet-sharedframework-ubuntu-x64.latest.deb
wget https://dotnetcli.blob.core.windows.net/dotnet/beta/Installers/Latest/dotnet-sdk-ubuntu-x64.latest.deb
//安装
dpkg -i dotnet-host-ubuntu-x64.latest.deb
dpkg -i dotnet-sharedframework-ubuntu-x64.latest.deb
dpkg -i dotnet-sdk-ubuntu-x64.latest.deb//如果执行上述安装语句后提示缺少依赖包，可以执行下面命令会自动补全依赖包，然后再重新执行报错的安装语句apt-get -f install
```
2、下载cli-samples的HelloMvc例子


Shell
```
cd ~apt-get -y install git
mkdir app
cd app
git clone https://github.com/aspnet/cli-samples.git
cd cli-samples
dotnet restore
cd HelloMvc
```
需要显式指定HelloMvc的端口（即使指定的是5000端口），目前尝试过不加下面这段代码，发现默认的5000并不起作用
![731719-20160415155421988-960951010](http://jbcdn2.b0.upaiyun.com/2016/05/a7c939b286c52b7d9c1c8a143a1d4447.png)
3、编译运行HelloMvc

```
dotnet run
```
打开浏览器 http://IP:5600（防火墙和查看网络相关命令请查看本篇文章第四点）
![](http://jbcdn2.b0.upaiyun.com/2016/05/0fa6ea97b2ada290f9e9aea08f4ad3c4.png)
**四、其他**
1、开启、禁用firewalld等相关命令（iptables、docker相同）

Shell
```
systemctl status firewalld或firewalld.service  ##查看firewalld是否启动。active(running)代表启动
systemctl start firewalld  ##如果firewalld没启动，可以使用此命令启动
systemctl enable firewalld  ##设置为随机器启动
systemctl stop firewalld  ##关闭firewalld
systemctl disable firewalld ##清除随机器启动
systemctl restart firewalld ##重启firewalld
```
2、防火墙firewalld常用命令

Shell
```
firewall-cmd --state           ##查看防火墙状态，是否是running
firewall-cmd --list-all         ##查看所有规则
firewall-cmd --reload         ##重新载入配置，比如添加规则之后，需要执行此命令
firewall-cmd --get-zones   ##列出支持的zone
firewall-cmd --get-services##列出支持的服务，在列表中的服务是放行的
firewall-cmd --query-service ftp##查看ftp服务是否支持，返回yes或者no
firewall-cmd --add-service=ftp  ##临时开放ftp服务
firewall-cmd --add-service=ftp --permanent##永久开放ftp服务
firewall-cmd --add-service=http --permanent##永久开放http服务
firewall-cmd --remove-service=ftp --permanent##永久移除ftp服务
firewall-cmd --add-port=80/tcp --permanent  ##永久添加80端口  
firewall-cmd --zone=public --add-port=80/tcp --permanent   ##--zone #作用域
```
3、防火墙iptables常用命令

Shell
```
iptables -L -n  --line-number     #查看防火墙的规则链
iptables -I INPUT -m state --state RELATED，ESTABLISHED -j ACCEPT 
iptables -I INPUT -p tcp --dport 80 -m state --state NEW -j ACCEPT
iptables -I INPUT -d 172.16.100.1 -p tcp --dport 22 -m state --state NEW -j ACCEPT
iptables -A INPUT -i lo -j ACCEPT              #测试不加此条会导致mysql在内部无法连接
查看/etc/sysconfig/iptables文件（记录着防火墙的规则链）
如果没有这个文件，请执行下面命令保存
iptables-save > /etc/sysconfig/iptables
查看保存后的规则
cat /etc/sysconfig/iptables
```
4、部分网络端口查询命令

Shell
```
netstat -tunlp -a # 查看所有网络端口情况
netstat -l -n  #查看监听(Listen)的端口,-n 代表显示端口数字而不是名称
netstat -antp #查看所有建立的TCP连接
```
相关资料：
- [Getting started with .NET Core](http://dotnet.github.io/getting-started/)
- [Running ASP.NET 5 applications in Linux Containers with Docker](https://blogs.msdn.microsoft.com/webdev/2015/01/14/running-asp-net-5-applications-in-linux-containers-with-docker/)
- [Get Started with Docker Engine for Linux](https://docs.docker.com/linux/)
- [.NET CLI Preview Docker Image](https://hub.docker.com/r/microsoft/dotnet/)
- [Installing ASP.NET 5 On Linux](http://docs.asp.net/en/latest/getting-started/installing-on-linux.html)
- [Docker Dockerfile详解](http://blog.csdn.net/wsscy2004/article/details/25878223)
- [最新版dotnet-cli下的ASP.NET Core和asp.net mvc【RC2尝鲜】](http://blog.csdn.net/yangzhenping/article/details/51050459)
- [ASP.NET Core 1.0 Hello World](http://news.cnblogs.com/n/539653/)
- [CentOS 7.1下安装dotnet core](http://www.cnblogs.com/modestmt/p/5368923.html)
