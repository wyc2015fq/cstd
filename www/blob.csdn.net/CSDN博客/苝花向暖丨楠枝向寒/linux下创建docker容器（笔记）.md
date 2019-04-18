# linux下创建docker容器（笔记） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月02日 19:37:20[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：985


6.5版本centOS 配置docker

1 先升级内核  ---依次下载 

	cd /etc/yum.repos.d

	yum -y install wget

	wget http://www.hop5.in/yum/el6/hop5.repo

	yum -y install kernel-ml-aufs kernel-ml-aufs-devel

	vi /etc/grub.conf    --进入以后找到default

	default修改为0

2  安装docker

	curl -sSL http://acs-public-mirror.oss-cn-hangzhou.aliyuncs.com/docker-engine/internet | sh -

	这一步动作时间会很久 别着急 多等会

3  配置阿里云加速 由于docker镜像库基本上都是国外所以我们可以把镜像库的加速地址配置成阿里云的  

	https://dev.aliyun.com/search.html   

	到上面注册一个 ，好像淘宝账号就能登录  登录之后 即可查看  

	我的是 https://m9i6didh.mirror.aliyuncs.com

 mkdir -p /etc/docker     

 touch daemon.json   ---在 /etc/docker 下创建daemon.json 文件

4  编辑文件 vi /etc/docker/daemon.json (如果没有要创建 )

	{

		"registry-mirrors": [" https://m9i6didh.mirror.aliyuncs.com"]

	}

	保存退出

	centos6.5 要 vi /etc/sysconfig/docker

	other_args="--registry-mirror= https://m9i6didh.mirror.aliyuncs.com"


5、启动 docker 服务 service docker start

  搜索	docker search centos6.5

  下载	docker pull doumadou/centos6.5_x86_64-base 

                docker images 查看 linux主机中存在的docker镜像

6、往docker中传jdk 和 tomcat  ，windows下是无法 直接和 linux中创建的docker直接进行通讯的。

映射 端口 ---实现输入虚拟机ip 访问 docker 资源

需要用到文件挂载

挂载 之后 容器和 linux 主机 用同一块硬盘，然后再把这硬盘里的东西 copy到本地硬盘上

docker run -itd -v /soft:/soft -p 80:8080 doumadou/centos6.5_x86_64-base /bin/bash    

--itd代表 后台运行，当退出该docker的时候，docker 不会停止

上面这条命令代表创建一个docker，并且访问linnux主机的80端口就会映射到创建的这个docker的8080端口，并且将linux主机soft文件夹挂载到docker。 挂载就相当于 插个U盘，U盘里存着linux下的soft文件夹，然后插到docker上，docker就也能访问了，但是我们需要将这U盘里的文件copy到本地。下面语句就是做这件事。

 docker ps -l              --- 查看最近创建的docker

 docker exec -it 2d166294f9c0 /bin/bash      ---2d166294f9c0 进入这个容器，就是我们刚创建的那个容器

 ls           

 cp /soft/jdk-8u161-linux-x64.tar.gz  usr         ---copy到 usr 文件夹

 cp /soft/apache-tomcat-8.0.50.tar.gz /usr         ---copy到 usr 文件夹

安装解压缩工具 yum -y install tar            ---docker里 没有解压缩工具， 安装一个

解压缩  tar -czvf jdk-8u161-linux-x64.tar.gz 和 tomcat          

配置 jdk 


8 主机访问虚拟机下的docker容器     ---可以通过输入linux的

添加路由 ----在windos系统下的 命令行提示符（管理员）  输入。 注意必须是管理员模式的

	 route add （docker的网段） mask（子网掩码）（虚拟机ip地址）       

     括号里填入对应的值

9、再创建其他docker          ---如果不用 nginx 这部可以跳过

ctrl+D 退出当前 docker

docker ps -l      --查看刚创建的docker

docker commit 2d166294f9c0 tomcat1:v1    --将 当前 docker 转换成镜像

 docker stop 2d166294f9c0 -- 停止最初的docker

 docker rm 2d166294f9c0  --删除最初的docker

docker run -itd 6297a542726c /bin/bash     --运行新生成的镜像创建docker   6297a542726c --是镜像的id






10 运行容器的时候关闭防火墙 --提升权限

	docker run --privileged=true







涉及命令解释

  启动 docker服务       service docker start

  搜索	docker search centos6.5

  下载	docker pull doumadou/centos6.5_x86_64-base 

  查看本地存在的镜像 docker images

  查看正在运行的镜像 docker ps -l

根据镜像创建容器    docker run --privileged=true -itd -p 192.168.46.228:80:80 -v /resource:/res e6df6f85e73d /bin/bash

并开启权限，进行虚拟机80端口与docker80端口进行映射，并将 虚拟机的/resource下的文件 挂载到 docker下的/res中

  交互式运行镜像创建容器 docker run -it doumadou/centos6.5_x86_64-base /bin/bash

  交互式后台运行镜像创建容器 docker run -itd doumadou/centos6.5_x86_64-base /bin/bash

  运行镜像创建容器   docker run doumadou/centos6.5_x86_64-base

  退出容器 ：ctrl+d   退出容器 ，容器会停止运行

  删除容器：  docker rm adc7aba280be 

  进入容器：docker exec -it 2d166294f9c0 /bin/bash

  删除正在运行的容器 ： 

1、先暂停docker stop a23ce1b23e8d

2、再删	

